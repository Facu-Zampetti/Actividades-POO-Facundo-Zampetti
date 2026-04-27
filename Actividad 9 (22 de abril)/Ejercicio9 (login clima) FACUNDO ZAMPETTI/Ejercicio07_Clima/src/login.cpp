#include "login.h"

#include "logger.h"
#include "ui_login.h"

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

Login::Login(QObject *parent)
    : Pantalla(parent)
    , m_widget(new QWidget)
    , m_ui(new Ui::LoginWidget)
{
    m_ui->setupUi(m_widget);

    m_widget->setStyleSheet(
        "QWidget#LoginWidget {"
        "  background-color: #06110d;"
        "  color: #d9fbe8;"
        "}"
        "QLineEdit {"
        "  background-color: #0a1c15;"
        "  color: #e8fff4;"
        "  border: 1px solid #1e6f4a;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid #39d98a;"
        "}"
        "QPushButton#loginButton {"
        "  background-color: #00a152;"
        "  color: #03170f;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 11px;"
        "  font-size: 15px;"
        "  font-weight: 800;"
        "}"
        "QPushButton#loginButton:hover {"
        "  background-color: #13c46a;"
        "}"
        "QPushButton#loginButton:disabled {"
        "  background-color: #315543;"
        "  color: #96c2aa;"
        "}"
    );

    m_ui->titleLabel->setStyleSheet("font-size: 34px; font-weight: 800; color: #e9fff4;");
    m_ui->subtitleLabel->setStyleSheet("font-size: 14px; color: #8dcfb0;");
    m_ui->temperatureLabel->setStyleSheet("font-size: 18px; color: #5cf2a2; font-weight: 700;");
    m_ui->timeLabel->setStyleSheet("font-size: 18px; color: #5cf2a2; font-weight: 700;");
    m_ui->weatherStatusLabel->setStyleSheet("font-size: 13px; color: #b8ebd0;");
    m_ui->loginStatusLabel->setStyleSheet("font-size: 13px; color: #d9fbe8;");

    connect(m_ui->loginButton, &QPushButton::clicked, this, &Login::validarCredenciales);
    connect(m_ui->usernameEdit, &QLineEdit::returnPressed, this, &Login::validarCredenciales);
    connect(m_ui->passwordEdit, &QLineEdit::returnPressed, this, &Login::validarCredenciales);
    connect(&m_lockTimer, &QTimer::timeout, this, &Login::actualizarBloqueo);

    m_lockTimer.setInterval(1000);

    Logger::instancia().registrar("Login: pantalla inicializada");
}

Login::~Login()
{
    delete m_ui;
    delete m_widget;
}

QWidget *Login::widget()
{
    return m_widget;
}

void Login::mostrar()
{
    m_widget->show();
}

void Login::ocultar()
{
    m_widget->hide();
}

void Login::actualizarClimaEnUI(const WeatherInfo &info)
{
    m_ui->temperatureLabel->setText(QString("Temperatura: %1 %2C").arg(info.temperatura, 0, 'f', 1).arg(QChar(0x00B0)));
    m_ui->timeLabel->setText("Hora local: " + info.horaLocal);

    const QString origen = info.offline ? "offline" : "online";
    m_ui->weatherStatusLabel->setText(QString("%1 (%2) - %3").arg(info.ciudad, origen, info.descripcion));
}

void Login::mostrarMensaje(const QString &texto, bool esError)
{
    m_ui->loginStatusLabel->setStyleSheet(esError ? "font-size: 13px; color: #ff6b8f;" : "font-size: 13px; color: #67f0ad;");
    m_ui->loginStatusLabel->setText(texto);
}

void Login::setControlesHabilitados(bool habilitado)
{
    m_ui->usernameEdit->setEnabled(habilitado);
    m_ui->passwordEdit->setEnabled(habilitado);
    m_ui->loginButton->setEnabled(habilitado);
}

void Login::validarCredenciales()
{
    if (m_bloqueado) {
        mostrarMensaje("Login bloqueado temporalmente por seguridad.", true);
        return;
    }

    const QString user = m_ui->usernameEdit->text().trimmed();
    const QString pass = m_ui->passwordEdit->text();

    if (user == "admin" && pass == "1234") {
        m_intentosFallidos = 0;
        mostrarMensaje("Credenciales correctas. Preparando ventana principal...", false);
        Logger::instancia().registrar("Login: acceso exitoso");
        emit loginExitoso();
        return;
    }

    m_intentosFallidos++;
    Logger::instancia().registrar("Login: intento fallido #" + QString::number(m_intentosFallidos));

    if (m_intentosFallidos >= 3) {
        m_bloqueado = true;
        m_finBloqueo = QDateTime::currentDateTime().addSecs(30);
        m_lockTimer.start();
        aplicarEstadoBloqueo();
        return;
    }

    const int restantes = 3 - m_intentosFallidos;
    mostrarMensaje(QString("Usuario o contrasena invalidos. Intentos restantes: %1").arg(restantes), true);
}

void Login::actualizarBloqueo()
{
    if (!m_bloqueado) {
        return;
    }

    if (QDateTime::currentDateTime() >= m_finBloqueo) {
        m_bloqueado = false;
        m_intentosFallidos = 0;
        m_lockTimer.stop();
        setControlesHabilitados(true);
        mostrarMensaje("Bloqueo levantado. Puede intentar nuevamente.", false);
        Logger::instancia().registrar("Login: bloqueo temporal finalizado");
        return;
    }

    aplicarEstadoBloqueo();
}

void Login::aplicarEstadoBloqueo()
{
    setControlesHabilitados(false);

    const qint64 restante = QDateTime::currentDateTime().secsTo(m_finBloqueo);
    mostrarMensaje(QString("Demasiados intentos fallidos. Reintente en %1 s.").arg(restante), true);
}
