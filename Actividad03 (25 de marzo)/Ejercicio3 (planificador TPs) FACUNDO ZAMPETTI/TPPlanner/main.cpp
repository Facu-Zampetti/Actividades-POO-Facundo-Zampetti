#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"
#include "sessionmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString username;

    // 🔥 verificar sesión
    if (SessionManager::isSessionValid(username)) {
        MainWindow w(username);
        w.show();
        return a.exec();
    }

    LoginWindow login;
    MainWindow *mainWindow = nullptr;

    QObject::connect(&login, &LoginWindow::loginSuccess,
                     [&](QString user){

                         // 🔥 guardar sesión
                         SessionManager::saveSession(user);

                         mainWindow = new MainWindow(user);
                         mainWindow->show();
                     });

    login.show();

    return a.exec();
}