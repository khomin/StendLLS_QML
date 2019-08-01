#include "log.h"

Log::Log(QObject *parent) : QObject(parent) {
    this->logModel = new LogModel(100);
}

Log::~Log() {
    delete logModel;
}

Q_INVOKABLE LogModel* Log::getLogModel() {
    return logModel;
}

void Log::insertLog(QString log, QString type) {
    logModel->addItem(log, type);
}
