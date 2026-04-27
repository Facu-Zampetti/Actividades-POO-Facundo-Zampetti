#include <QApplication>

#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Ejercicio07_Clima");
    QApplication::setOrganizationName("UTN");

    AppController controller;
    controller.iniciar();

    return app.exec();
}
