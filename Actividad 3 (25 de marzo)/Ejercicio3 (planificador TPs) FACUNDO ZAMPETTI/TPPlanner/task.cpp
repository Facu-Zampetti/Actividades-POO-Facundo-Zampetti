#include "task.h"

QJsonObject Task::toJson() const
{
    QJsonObject obj;
    obj["title"] = title;
    obj["status"] = status;
    obj["priority"] = priority;
    obj["notes"] = notes;
    return obj;
}

Task Task::fromJson(const QJsonObject &obj)
{
    Task t;
    t.title = obj["title"].toString();
    t.status = obj["status"].toString();
    t.priority = obj["priority"].toString();
    t.notes = obj["notes"].toString();
    return t;
}