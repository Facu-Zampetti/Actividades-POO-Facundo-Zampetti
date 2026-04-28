#pragma once

#include "configuracion.h"
#include "pantalla.h"

#include <QNetworkAccessManager>

struct WeatherInfo
{
    double temperatura = 0.0;
    QString ciudad;
    QString horaLocal;
    QString descripcion;
    bool offline = false;
};

class Clima : public Pantalla
{
    Q_OBJECT
public:
    explicit Clima(const ConfiguracionData &config, const QNetworkProxy &proxy, QObject *parent = nullptr);

    QWidget *widget() override;

public slots:
    void mostrar() override;
    void ocultar() override;
    void actualizarClima();

signals:
    void climaListo(const WeatherInfo &info);
    void mensaje(const QString &texto, bool esError);

private slots:
    void onConnectivityFinished(QNetworkReply *reply);
    void onWeatherFinished(QNetworkReply *reply);

private:
    void consultarApi();
    void emitirOffline(const QString &mensajeAmigable);
    QString horaCordoba() const;

    ConfiguracionData m_config;
    QNetworkAccessManager m_connectivityManager;
    QNetworkAccessManager m_weatherManager;
};
