#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class Login : public QWidget{
    Q_OBJECT

private:
    QLabel * lUsuario, * lClave;
    QLineEdit * leUsuario, * leClave;
    QPushButton * pbIngresar;
    QGridLayout * layout;

public:
    Login();

public slots:
    void validarUsuario();
};

#endif // LOGIN_H
