#include "mainwindow.h"
#include "homewindow.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout();

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Usuario");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Contraseña");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");

    messageLabel = new QLabel("");

    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
    layout->addWidget(messageLabel);

    central->setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);

    setWindowTitle("Login");
    resize(300, 200);
}

MainWindow::~MainWindow()
{
}

void MainWindow::handleLogin()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setText("Completar todos los campos");
        return;
    }

    QUrl url("http://localhost:8000/login");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {

            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);

            QString token = doc.object()["access_token"].toString();

            messageLabel->setText("Login exitoso");

            HomeWindow *home = new HomeWindow(username, token);
            home->show();

            this->hide();

        } else {

            QByteArray errorResponse = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(errorResponse);

            QString errorMsg = "Error en login";

            if (doc.isObject()) {
                errorMsg = doc.object()["detail"].toString();
            }

            messageLabel->setText(errorMsg);
        }

        reply->deleteLater();
    });
}