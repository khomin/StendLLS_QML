#ifndef LOG_H
#define LOG_H

#include <QObject>
#include "logModel/logModel.h"

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

    ~Log();

    Q_INVOKABLE LogModel* getLogModel();

public slots:
    void insertLog(QString log, QString type);

private:
    LogModel * logModel;
};

#endif // LOG_H
