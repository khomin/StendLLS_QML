#ifndef QRSCANER_H
#define QRSCANER_H

#include <QObject>
#include "dataBase/dataBase.h"

class QrScaner : public QObject
{
    Q_OBJECT
public:
    explicit QrScaner(QObject *parent = nullptr);

signals:
    void qrCodeUpdateSerialNum(QString number);
    void qrCodeError(QString message);

public slots:
    void insertQrData(QString codeData);
};

#endif // QRSCANER_H
