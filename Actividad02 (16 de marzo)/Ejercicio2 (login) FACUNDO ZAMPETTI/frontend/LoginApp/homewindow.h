#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QString username, QString token, QWidget *parent = nullptr);

private:
    QLabel *welcomeLabel;
    QLabel *tokenLabel;
};

#endif // HOMEWINDOW_H