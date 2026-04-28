#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager
{
public:
    static void saveSession(const QString &username);
    static bool isSessionValid(QString &username);

private:
    static QString filePath;
};

#endif