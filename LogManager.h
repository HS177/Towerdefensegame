#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class LogManager {
public:
    static LogManager& getInstance();
    void logEvent(const QString& event);

private:
    LogManager();
    ~LogManager();
    QFile logFile;
};

#endif
