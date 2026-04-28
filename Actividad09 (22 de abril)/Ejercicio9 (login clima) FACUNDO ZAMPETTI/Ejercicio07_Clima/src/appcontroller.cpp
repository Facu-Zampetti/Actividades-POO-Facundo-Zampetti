#include "appcontroller.h"

#include "downloaderfondo.h"
#include "logger.h"
#include "login.h"
#include "ventana.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QTimer>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_configuracion(QDir(QApplication::applicationDirPath()).filePath("config.ini"))
{
    m_configuracion.cargar();
    m_datos = m_configuracion.datos();

    m_login = new Login(this);
    m_ventana = new Ventana(this);
    m_clima = new Clima(m_datos, m_configuracion.proxy(), this);
    m_downloader = new DownloaderFondo(m_datos.fondoUrl, m_configuracion.proxy(), this);

    connect(m_login, &Login::loginExitoso, this, &AppController::onLoginExitoso);
    connect(m_clima, &Clima::climaListo, m_login, &Login::actualizarClimaEnUI);
    connect(m_clima, &Clima::mensaje, m_login, &Login::mostrarMensaje);
    connect(m_downloader, &DownloaderFondo::imagenLista, this, &AppController::onImagenLista);
    connect(m_downloader, &DownloaderFondo::error, this, &AppController::onErrorFondo);

    auto *timer = new QTimer(this);
    timer->setInterval(300000);
    connect(timer, &QTimer::timeout, m_clima, &Clima::actualizarClima);
    timer->start();
}

AppController::~AppController() = default;

void AppController::iniciar()
{
    Logger::instancia().registrar("App: inicio de aplicacion");
    m_login->mostrar();

    m_clima->actualizarClima();
    m_downloader->prepararImagen();
}

void AppController::onLoginExitoso()
{
    m_loginValidado = true;
    m_login->mostrarMensaje("Usuario valido. Esperando descarga del fondo principal...", false);
    m_login->setControlesHabilitados(false);
    Logger::instancia().registrar("App: usuario autenticado");

    abrirVentanaSiLista();
}

void AppController::onImagenLista(const QString &ruta)
{
    m_imagenLista = true;
    m_rutaImagen = ruta;
    Logger::instancia().registrar("App: imagen principal lista en cache/local");

    abrirVentanaSiLista();
}

void AppController::onErrorFondo(const QString &mensaje)
{
    Logger::instancia().registrar("App: error de fondo: " + mensaje);
    m_login->mostrarMensaje(mensaje, true);
}

void AppController::abrirVentanaSiLista()
{
    if (!m_loginValidado || !m_imagenLista) {
        return;
    }

    m_ventana->setImagenFondo(m_rutaImagen);
    m_ventana->mostrar();
    m_login->ocultar();

    Logger::instancia().registrar("App: ventana principal abierta en full screen");
}
