#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QWidget>
#include "task.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QTextEdit;
class QPushButton;
QT_END_NAMESPACE

class TaskDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TaskDialog(Task task, QWidget *parent = nullptr);

signals:
    void taskSaved(Task task);

private slots:
    void saveTask();

private:
    Task currentTask;

    QLineEdit *titleEdit;
    QComboBox *statusBox;
    QComboBox *priorityBox;
    QTextEdit *notesEdit;
    QPushButton *saveButton;
};

#endif