#pragma once

#include <QNetworkProxy>
#include <QString>

struct ConfiguracionData
{
    QString apiKey;
    QString ciudad;
    QString unidades;
    QString fondoUrl;
    bool proxyEnabled = false;
    QString proxyHost;
    int proxyPort = 8080;
    QString proxyUser;
    QString proxyPassword;
};

class Configuracion
{
public:
    explicit Configuracion(const QString &rutaArchivo);

    void cargar();
    ConfiguracionData datos() const;
    QNetworkProxy proxy() const;

private:
    QString m_rutaArchivo;
    ConfiguracionData m_datos;
};
