#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QTextStream>

Logger &Logger::instancia()
{
    static Logger logger;
    return logger;
}

void Logger::registrar(const QString &descripcion)
{
    QMutexLocker locker(&m_mutex);

    const QString path = rutaLog();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    const QString stamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    out << stamp << " | " << descripcion << '\n';
}

QString Logger::rutaLog() const
{
    const QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(baseDir);
    dir.mkpath(".");
    return dir.filePath("eventos.log");
}
