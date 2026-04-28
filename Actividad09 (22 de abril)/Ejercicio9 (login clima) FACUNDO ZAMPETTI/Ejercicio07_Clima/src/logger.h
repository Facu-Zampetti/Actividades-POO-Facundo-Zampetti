#pragma once

#include <QMutex>
#include <QString>

class Logger
{
public:
    static Logger &instancia();

    void registrar(const QString &descripcion);

private:
    Logger() = default;
    QString rutaLog() const;

    QMutex m_mutex;
};
