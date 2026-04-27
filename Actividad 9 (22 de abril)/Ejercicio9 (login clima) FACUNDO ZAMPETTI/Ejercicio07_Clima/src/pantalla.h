#pragma once

#include <QObject>

class QWidget;

class Pantalla : public QObject
{
    Q_OBJECT
public:
    explicit Pantalla(QObject *parent = nullptr) : QObject(parent) {}
    ~Pantalla() override = default;

    virtual QWidget *widget() = 0;

public slots:
    virtual void mostrar() = 0;
    virtual void ocultar() = 0;
};
