#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>

Logger::Logger() {}

void Logger::saveLog(const QString &request, const QString &response, const QString &normalResponse)
{
    QString filePath = QCoreApplication::applicationDirPath() + "/SQLi_Log.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Cannot open log file.";
        return;
    }

    QTextStream out(&file);
    out << "Time: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";
    out << "Target URL: " << request << "\n";
    out << "Normal Response:\n" << normalResponse << "\n";
    out << "Injected Response:\n" << response << "\n";
    out << "----------------------------------------\n";
    file.close();
}

QString Logger::exportLog()
{
    QFile file("SQLi_Log.txt");
    return file.exists() ? "Log file saved: SQLi_Log.txt" : "No logs available to export.";
}
