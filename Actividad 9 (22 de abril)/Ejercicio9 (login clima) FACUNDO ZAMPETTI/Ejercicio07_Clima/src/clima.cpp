#include "clima.h"

#include "logger.h"

#include <QDateTime>
#include <chrono>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRandomGenerator>
#include <QTimeZone>
#include <QUrlQuery>

Clima::Clima(const ConfiguracionData &config, const QNetworkProxy &proxy, QObject *parent)
    : Pantalla(parent)
    , m_config(config)
{
    m_connectivityManager.setProxy(proxy);
    m_weatherManager.setProxy(proxy);

    connect(&m_connectivityManager, &QNetworkAccessManager::finished,
            this, &Clima::onConnectivityFinished);
    connect(&m_weatherManager, &QNetworkAccessManager::finished,
            this, &Clima::onWeatherFinished);
}

QWidget *Clima::widget()
{
    return nullptr;
}

void Clima::mostrar()
{
}

void Clima::ocultar()
{
}

void Clima::actualizarClima()
{
    Logger::instancia().registrar("Clima: inicio de deteccion de conectividad");

    QUrl connectUrl("https://clients3.google.com/generate_204");
    QNetworkRequest request{connectUrl};
    request.setTransferTimeout(std::chrono::milliseconds(5000));
    m_connectivityManager.get(request);
}

void Clima::onConnectivityFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        const QString reason = reply->errorString();
        reply->deleteLater();
        emitirOffline("Sin internet. Mostrando datos simulados.");
        Logger::instancia().registrar("Clima: modo offline por error de conectividad: " + reason);
        return;
    }

    reply->deleteLater();

    if (m_config.apiKey.isEmpty() || m_config.apiKey == "CAMBIAR_API_KEY") {
        emitirOffline("Falta API key en config.ini. Mostrando datos simulados.");
        Logger::instancia().registrar("Clima: API key ausente, se usa modo offline");
        return;
    }

    consultarApi();
}

void Clima::onWeatherFinished(QNetworkReply *reply)
{
    const QByteArray payload = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        const QString reason = reply->errorString();
        reply->deleteLater();
        emitirOffline("No se pudo consultar el clima online. Se usan datos simulados.");
        Logger::instancia().registrar("Clima: error API, fallback offline: " + reason);
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(payload, &parseError);
    reply->deleteLater();

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        emitirOffline("Respuesta de clima invalida. Se usan datos simulados.");
        Logger::instancia().registrar("Clima: respuesta JSON invalida: " + parseError.errorString());
        return;
    }

    const QJsonObject root = doc.object();
    if (!root.contains("main") || !root.value("main").isObject()) {
        emitirOffline("Datos de clima incompletos. Se usan datos simulados.");
        Logger::instancia().registrar("Clima: falta objeto main en JSON");
        return;
    }

    const QJsonObject mainObj = root.value("main").toObject();
    if (!mainObj.contains("temp") || !mainObj.value("temp").isDouble()) {
        emitirOffline("Temperatura invalida en la respuesta. Se usan datos simulados.");
        Logger::instancia().registrar("Clima: temperatura no numerica");
        return;
    }

    WeatherInfo info;
    info.temperatura = mainObj.value("temp").toDouble();
    info.ciudad = root.value("name").toString(m_config.ciudad);
    info.horaLocal = horaCordoba();
    info.offline = false;
    info.descripcion = root.value("weather").toArray().isEmpty()
                           ? "Clima actualizado"
                           : root.value("weather").toArray().first().toObject().value("description").toString("Clima actualizado");

    Logger::instancia().registrar("Clima: datos online recibidos para " + info.ciudad);
    emit mensaje("Clima online actualizado correctamente.", false);
    emit climaListo(info);
}

void Clima::consultarApi()
{
    QUrl url("https://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("q", m_config.ciudad);
    query.addQueryItem("units", m_config.unidades.isEmpty() ? "metric" : m_config.unidades);
    query.addQueryItem("appid", m_config.apiKey);
    query.addQueryItem("lang", "es");
    url.setQuery(query);

    QNetworkRequest request{url};
    request.setTransferTimeout(std::chrono::milliseconds(8000));
    m_weatherManager.get(request);

    Logger::instancia().registrar("Clima: consulta API enviada a " + url.toString(QUrl::RemoveQuery));
}

void Clima::emitirOffline(const QString &mensajeAmigable)
{
    WeatherInfo info;
    info.temperatura = 20.0 + QRandomGenerator::global()->bounded(80) / 10.0;
    info.ciudad = m_config.ciudad.isEmpty() ? "Cordoba" : m_config.ciudad;
    info.horaLocal = horaCordoba();
    info.offline = true;
    info.descripcion = "Datos simulados (offline)";

    emit mensaje(mensajeAmigable, true);
    emit climaListo(info);
}

QString Clima::horaCordoba() const
{
    const QTimeZone tz("America/Argentina/Cordoba");
    return QDateTime::currentDateTimeUtc().toTimeZone(tz).toString("HH:mm");
}
