#include "downloaderfondo.h"

#include "logger.h"

#include <QDir>
#include <chrono>
#include <QFile>
#include <QImage>
#include <QLinearGradient>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QStandardPaths>

DownloaderFondo::DownloaderFondo(const QString &url, const QNetworkProxy &proxy, QObject *parent)
    : QObject(parent)
    , m_url(url)
{
    m_network.setProxy(proxy);

    connect(&m_network, &QNetworkAccessManager::finished,
            this, &DownloaderFondo::onDownloadFinished);
}

void DownloaderFondo::prepararImagen()
{
    const QString path = cachePath();
    if (QFile::exists(path)) {
        Logger::instancia().registrar("Fondo: cache encontrada en " + path);
        emit imagenLista(path);
        return;
    }

    if (m_url.isEmpty()) {
        guardarPlaceholder(path);
        emit error("URL de fondo vacia. Se genero imagen local.");
        emit imagenLista(path);
        Logger::instancia().registrar("Fondo: URL vacia, se genero placeholder");
        return;
    }

    QUrl qurl(m_url);
    QNetworkRequest request{qurl};
    request.setTransferTimeout(std::chrono::milliseconds(15000));
    m_network.get(request);
    Logger::instancia().registrar("Fondo: descarga iniciada desde URL publica");
}

void DownloaderFondo::onDownloadFinished(QNetworkReply *reply)
{
    const QString path = cachePath();

    if (reply->error() != QNetworkReply::NoError) {
        const QString reason = reply->errorString();
        reply->deleteLater();

        if (!QFile::exists(path)) {
            guardarPlaceholder(path);
        }

        emit error("No se pudo descargar el fondo. Se usa cache/local.");
        emit imagenLista(path);
        Logger::instancia().registrar("Fondo: error de descarga, fallback local: " + reason);
        return;
    }

    const QByteArray bytes = reply->readAll();
    reply->deleteLater();

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        guardarPlaceholder(path);
        emit error("No se pudo guardar la imagen. Se usa placeholder.");
        emit imagenLista(path);
        Logger::instancia().registrar("Fondo: no se pudo escribir cache, placeholder generado");
        return;
    }

    file.write(bytes);
    file.close();

    Logger::instancia().registrar("Fondo: descarga finalizada y cacheada");
    emit imagenLista(path);
}

QString DownloaderFondo::cachePath() const
{
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/cache";
    QDir dir(cacheDir);
    dir.mkpath(".");
    return dir.filePath("fondo_principal.jpg");
}

void DownloaderFondo::guardarPlaceholder(const QString &path) const
{
    QImage img(1920, 1080, QImage::Format_RGB32);
    QPainter painter(&img);

    QLinearGradient gradient(0, 0, 1920, 1080);
    gradient.setColorAt(0.0, QColor("#193549"));
    gradient.setColorAt(1.0, QColor("#0f2027"));
    painter.fillRect(img.rect(), gradient);

    painter.setPen(Qt::white);
    QFont title("Segoe UI", 40, QFont::Bold);
    painter.setFont(title);
    painter.drawText(img.rect(), Qt::AlignCenter, "Modo Offline");

    painter.end();
    img.save(path, "JPG", 90);
}
