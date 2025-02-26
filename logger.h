#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger
{
public:
    Logger();
    void saveLog(const QString &request, const QString &response, const QString &normalResponse);
    QString exportLog();
};

#endif // LOGGER_H
