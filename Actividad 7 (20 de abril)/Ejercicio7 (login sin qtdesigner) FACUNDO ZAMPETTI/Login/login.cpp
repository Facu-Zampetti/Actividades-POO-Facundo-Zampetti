#include "login.h"

Login::Login(){
    lUsuario = new QLabel("Usuario");
    leUsuario = new QLineEdit;
    lClave = new QLabel("Clave");
    leClave = new QLineEdit;
    leClave->setEchoMode(QLineEdit::Password);
    pbIngresar = new QPushButton("Verificar");
    layout = new QGridLayout;

    layout->addWidget(lUsuario, 0, 0, 1, 1);
    layout->addWidget(leUsuario, 0, 1, 1, 2);
    layout->addWidget(lClave, 1, 0, 1, 1);
    layout->addWidget(leClave, 1, 1, 1, 2);
    layout->addWidget(pbIngresar, 2, 1, 1, 1);

    this->setLayout(layout);
    connect(leClave, SIGNAL(returnPressed()), this, SLOT(validarUsuario()));
    connect(pbIngresar, SIGNAL(pressed()), this, SLOT(validarUsuario()));
}

void Login::validarUsuario(){
    if(this->leUsuario->text() == "admin" && this->leClave->text() == "1234"){
        this->close();
    }else{
        this->leClave->clear();
    };
}