#include "sessionmanager.h"
#include <QFile>
#include <QDateTime>
#include <QTextStream>

QString SessionManager::filePath = "session.txt";

void SessionManager::saveSession(const QString &username)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return;

    QTextStream out(&file);
    out << username << "\n";
    out << QDateTime::currentSecsSinceEpoch();
}

bool SessionManager::isSessionValid(QString &username)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QTextStream in(&file);

    QString savedUser = in.readLine();
    qint64 savedTime = in.readLine().toLongLong();

    qint64 now = QDateTime::currentSecsSinceEpoch();

    // 5 minutos = 300 segundos
    if (now - savedTime <= 300) {
        username = savedUser;
        return true;
    }

    return false;
}