#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QVector>
#include <QString>
#include "task.h"

class TaskManager
{
public:
    void load();
    void save();

    void addTask(const Task &task);
    void removeTask(int index);
    void updateTask(int index, const Task &task);

    QVector<Task> tasks;

    // historial
    void logAction(const QString &action);

private:
    QString filePath = "tasks.json";
    QString logPath = "history.txt";
};

#endif