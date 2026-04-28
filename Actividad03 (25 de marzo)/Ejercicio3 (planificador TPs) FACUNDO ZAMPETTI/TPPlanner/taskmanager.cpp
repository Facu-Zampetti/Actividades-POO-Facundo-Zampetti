#include "taskmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>

void TaskManager::load()
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc.array();

    tasks.clear();

    for (auto val : arr) {
        tasks.append(Task::fromJson(val.toObject()));
    }
}

void TaskManager::save()
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonArray arr;

    for (const Task &t : tasks) {
        arr.append(t.toJson());
    }

    file.write(QJsonDocument(arr).toJson());
}

void TaskManager::addTask(const Task &task)
{
    tasks.append(task);
    save();
    logAction("Se agregó tarea: " + task.title);
}

void TaskManager::removeTask(int index)
{
    if (index >= 0 && index < tasks.size()) {
        logAction("Se eliminó tarea: " + tasks[index].title);
        tasks.removeAt(index);
        save();
    }
}

void TaskManager::updateTask(int index, const Task &task)
{
    if (index >= 0 && index < tasks.size()) {
        tasks[index] = task;
        save();
        logAction("Se editó tarea: " + task.title);
    }
}

void TaskManager::logAction(const QString &action)
{
    QFile file(logPath);
    if (!file.open(QIODevice::Append)) return;

    QTextStream out(&file);
    out << action << "\n";
}