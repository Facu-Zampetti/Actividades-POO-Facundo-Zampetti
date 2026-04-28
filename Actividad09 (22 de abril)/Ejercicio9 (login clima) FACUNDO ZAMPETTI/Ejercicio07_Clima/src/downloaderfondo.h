#pragma once

#include <QNetworkAccessManager>
#include <QObject>

class DownloaderFondo : public QObject
{
    Q_OBJECT
public:
    explicit DownloaderFondo(const QString &url, const QNetworkProxy &proxy, QObject *parent = nullptr);

public slots:
    void prepararImagen();

signals:
    void imagenLista(const QString &rutaImagen);
    void error(const QString &mensaje);

private slots:
    void onDownloadFinished(QNetworkReply *reply);

private:
    QString cachePath() const;
    void guardarPlaceholder(const QString &path) const;

    QString m_url;
    QNetworkAccessManager m_network;
};
