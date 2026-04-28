#include "configuracion.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

Configuracion::Configuracion(const QString &rutaArchivo)
    : m_rutaArchivo(rutaArchivo)
{
}

void Configuracion::cargar()
{
    if (!QFile::exists(m_rutaArchivo)) {
        QDir dir = QFileInfo(m_rutaArchivo).dir();
        dir.mkpath(".");
    }

    QSettings cfg(m_rutaArchivo, QSettings::IniFormat);

    m_datos.apiKey = cfg.value("clima/api_key", "CAMBIAR_API_KEY").toString().trimmed();
    m_datos.ciudad = cfg.value("clima/ciudad", "Cordoba,AR").toString().trimmed();
    m_datos.unidades = cfg.value("clima/unidades", "metric").toString().trimmed();

    m_datos.fondoUrl = cfg.value("fondo/url", "https://picsum.photos/1920/1080").toString().trimmed();

    m_datos.proxyEnabled = cfg.value("proxy/enabled", false).toBool();
    m_datos.proxyHost = cfg.value("proxy/host", "").toString().trimmed();
    m_datos.proxyPort = cfg.value("proxy/port", 8080).toInt();
    m_datos.proxyUser = cfg.value("proxy/username", "").toString().trimmed();
    m_datos.proxyPassword = cfg.value("proxy/password", "").toString().trimmed();
}

ConfiguracionData Configuracion::datos() const
{
    return m_datos;
}

QNetworkProxy Configuracion::proxy() const
{
    if (!m_datos.proxyEnabled || m_datos.proxyHost.isEmpty()) {
        return QNetworkProxy::NoProxy;
    }

    QNetworkProxy proxy(QNetworkProxy::HttpProxy, m_datos.proxyHost, m_datos.proxyPort,
                        m_datos.proxyUser, m_datos.proxyPassword);
    return proxy;
}
