#pragma once

#include "pantalla.h"

#include <QPixmap>

class QLabel;
class QFrame;
class QPushButton;
class QWidget;

class Ventana : public Pantalla
{
    Q_OBJECT
public:
    explicit Ventana(QObject *parent = nullptr);
    ~Ventana() override;

    QWidget *widget() override;

public slots:
    void mostrar() override;
    void ocultar() override;
    void setImagenFondo(const QString &path);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void actualizarFondo();
    QPixmap crearFotoPerfil() const;

    QWidget *m_widget = nullptr;
    QLabel *m_backgroundLabel = nullptr;
    QFrame *m_card = nullptr;
    QPushButton *m_btnCerrar = nullptr;
    QPixmap m_background;
};
