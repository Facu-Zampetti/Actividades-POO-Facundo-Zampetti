#include "loginwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login");
    resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("TP Planner");
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");
    title->setAlignment(Qt::AlignCenter);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Usuario");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Contraseña");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addStretch();

    setLayout(layout);

    // 🎨 estilo mejorado
    setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #1e3c72, stop:1 #2a5298);
        }
        QLineEdit {
            padding: 10px;
            border-radius: 10px;
            background: white;
            font-size: 14px;
        }
        QPushButton {
            padding: 12px;
            border-radius: 12px;
            background: #00c6ff;
            color: white;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background: #0072ff;
        }
    )");

    // 🔥 ENTER funciona
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);
}

void LoginWindow::handleLogin()
{
    QString user = usernameEdit->text();
    QString pass = passwordEdit->text();

    // 🔴 validar campos vacíos
    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Completa usuario y contraseña");
        return;
    }

    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir users.json");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.array();

    for (auto u : users) {
        QJsonObject obj = u.toObject();
        if (obj["username"].toString() == user &&
            obj["password"].toString() == pass) {

            emit loginSuccess(user);
            close();
            return;
        }
    }

    // 🔴 credenciales incorrectas
    QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos");
}