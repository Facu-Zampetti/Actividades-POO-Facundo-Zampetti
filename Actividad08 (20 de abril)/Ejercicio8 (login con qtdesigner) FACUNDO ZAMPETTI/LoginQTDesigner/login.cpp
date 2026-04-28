#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->leUsuario->setFocus();

    connect(ui->leClave, SIGNAL(returnPressed()), this, SLOT(validarUsuario()));
    connect(ui->pbIngresar, SIGNAL(pressed()), this, SLOT(validarUsuario()));
}

Login::~Login()
{
    delete ui;
}

void Login::validarUsuario(){
    if(ui->leUsuario->text() == "admin" && ui->leClave->text() == "1234"){
        this->close();
    }else{
        ui->leClave->clear();
    }
}
