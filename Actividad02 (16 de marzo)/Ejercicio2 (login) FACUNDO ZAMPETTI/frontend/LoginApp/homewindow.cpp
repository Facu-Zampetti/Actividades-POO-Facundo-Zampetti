#include "homewindow.h"

HomeWindow::HomeWindow(QString username, QString token, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    welcomeLabel = new QLabel("Bienvenido " + username);

    tokenLabel = new QLabel("Token:\n" + token);
    tokenLabel->setWordWrap(true);

    layout->addWidget(welcomeLabel);
    layout->addWidget(tokenLabel);

    setLayout(layout);

    setWindowTitle("Home");
    resize(400, 200);
}