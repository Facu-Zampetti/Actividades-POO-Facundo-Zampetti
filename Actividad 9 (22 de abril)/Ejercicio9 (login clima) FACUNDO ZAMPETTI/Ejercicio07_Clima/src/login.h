#pragma once

#include "clima.h"
#include "pantalla.h"

#include <QDateTime>
#include <QTimer>

namespace Ui {
class LoginWidget;
}

class QWidget;

class Login : public Pantalla
{
    Q_OBJECT
public:
    explicit Login(QObject *parent = nullptr);
    ~Login() override;

    QWidget *widget() override;

public slots:
    void mostrar() override;
    void ocultar() override;
    void actualizarClimaEnUI(const WeatherInfo &info);
    void mostrarMensaje(const QString &texto, bool esError);
    void setControlesHabilitados(bool habilitado);

signals:
    void loginExitoso();

private slots:
    void validarCredenciales();
    void actualizarBloqueo();

private:
    void aplicarEstadoBloqueo();

    QWidget *m_widget = nullptr;
    Ui::LoginWidget *m_ui = nullptr;

    int m_intentosFallidos = 0;
    bool m_bloqueado = false;
    QDateTime m_finBloqueo;
    QTimer m_lockTimer;
};
