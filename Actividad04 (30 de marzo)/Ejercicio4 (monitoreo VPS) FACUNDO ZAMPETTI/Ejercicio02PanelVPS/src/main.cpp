#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Panel Monitoreo VPS");

    MainWindow w;
    w.resize(1100, 700);
    w.show();

    return app.exec();
}
