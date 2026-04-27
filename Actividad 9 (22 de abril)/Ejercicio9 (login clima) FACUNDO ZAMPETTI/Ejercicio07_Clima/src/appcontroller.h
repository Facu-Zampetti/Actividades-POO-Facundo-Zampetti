#pragma once

#include "clima.h"
#include "configuracion.h"

#include <QObject>

class Login;
class Ventana;
class DownloaderFondo;

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController() override;

    void iniciar();

private slots:
    void onLoginExitoso();
    void onImagenLista(const QString &ruta);
    void onErrorFondo(const QString &mensaje);

private:
    void abrirVentanaSiLista();

    Configuracion m_configuracion;
    ConfiguracionData m_datos;

    Login *m_login = nullptr;
    Ventana *m_ventana = nullptr;
    Clima *m_clima = nullptr;
    DownloaderFondo *m_downloader = nullptr;

    bool m_loginValidado = false;
    bool m_imagenLista = false;
    QString m_rutaImagen;
};
