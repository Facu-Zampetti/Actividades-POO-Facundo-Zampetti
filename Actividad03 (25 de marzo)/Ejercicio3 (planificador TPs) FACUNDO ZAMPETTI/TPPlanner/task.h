#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QJsonObject>

class Task
{
public:
    QString title;
    QString status;
    QString priority;
    QString notes;

    QJsonObject toJson() const;
    static Task fromJson(const QJsonObject &obj);
};

#endif