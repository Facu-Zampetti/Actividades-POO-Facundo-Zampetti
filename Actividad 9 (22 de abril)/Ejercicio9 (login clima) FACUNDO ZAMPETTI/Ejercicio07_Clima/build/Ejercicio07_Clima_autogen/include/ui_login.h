/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *weatherLayout;
    QLabel *temperatureLabel;
    QLabel *timeLabel;
    QLabel *weatherStatusLabel;
    QSpacerItem *verticalSpacer2;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *loginStatusLabel;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(760, 520);
        verticalLayout = new QVBoxLayout(LoginWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(36, 28, 36, 28);
        titleLabel = new QLabel(LoginWidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 32px; font-weight: 700; color: #0f172a;"));

        verticalLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(LoginWidget);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #475569;"));

        verticalLayout->addWidget(subtitleLabel);

        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer1);

        weatherLayout = new QHBoxLayout();
        weatherLayout->setObjectName("weatherLayout");
        temperatureLabel = new QLabel(LoginWidget);
        temperatureLabel->setObjectName("temperatureLabel");
        temperatureLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #0b3a53; font-weight: 600;"));

        weatherLayout->addWidget(temperatureLabel);

        timeLabel = new QLabel(LoginWidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #0b3a53; font-weight: 600;"));

        weatherLayout->addWidget(timeLabel);


        verticalLayout->addLayout(weatherLayout);

        weatherStatusLabel = new QLabel(LoginWidget);
        weatherStatusLabel->setObjectName("weatherStatusLabel");
        weatherStatusLabel->setWordWrap(true);
        weatherStatusLabel->setStyleSheet(QString::fromUtf8("color: #334155; font-size: 13px;"));

        verticalLayout->addWidget(weatherStatusLabel);

        verticalSpacer2 = new QSpacerItem(20, 18, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer2);

        usernameEdit = new QLineEdit(LoginWidget);
        usernameEdit->setObjectName("usernameEdit");

        verticalLayout->addWidget(usernameEdit);

        passwordEdit = new QLineEdit(LoginWidget);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(passwordEdit);

        loginButton = new QPushButton(LoginWidget);
        loginButton->setObjectName("loginButton");
        loginButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #0b3a53; color: white; border-radius: 8px; padding: 10px; font-weight: 700; }\n"
"QPushButton:disabled { background-color: #93a7b5; }"));

        verticalLayout->addWidget(loginButton);

        loginStatusLabel = new QLabel(LoginWidget);
        loginStatusLabel->setObjectName("loginStatusLabel");
        loginStatusLabel->setWordWrap(true);
        loginStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; color: #1e293b;"));

        verticalLayout->addWidget(loginStatusLabel);


        retranslateUi(LoginWidget);

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QCoreApplication::translate("LoginWidget", "Login - Clima en Cordoba", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginWidget", "Ingreso al sistema", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("LoginWidget", "Usuario admin con clima y hora local de Cordoba", nullptr));
        temperatureLabel->setText(QCoreApplication::translate("LoginWidget", "Temperatura: --.- \302\260C", nullptr));
        timeLabel->setText(QCoreApplication::translate("LoginWidget", "Hora local: --:--", nullptr));
        weatherStatusLabel->setText(QCoreApplication::translate("LoginWidget", "Estado clima: inicializando...", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "Usuario", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "Contrasena", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWidget", "Ingresar", nullptr));
        loginStatusLabel->setText(QCoreApplication::translate("LoginWidget", "Ingrese sus credenciales.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
