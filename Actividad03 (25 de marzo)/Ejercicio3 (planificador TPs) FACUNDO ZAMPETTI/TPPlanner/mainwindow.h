#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "taskmanager.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QString username, QWidget *parent = nullptr);

private:
    QGridLayout *grid;
    TaskManager manager;
    void refreshUI();
};

#endif