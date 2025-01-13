#include "LogManager.h"

LogManager::LogManager() : logFile("game_events.log") {
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Unable to open log file for writing!");
    }
}

LogManager::~LogManager() {
    if (logFile.isOpen()) {
        logFile.close();
    }
}

LogManager& LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

void LogManager::logEvent(const QString& event) {
    if (logFile.isOpen()) {
        QTextStream out(&logFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        out << "[" << timestamp << "] " << event << "\n";
    }
}
