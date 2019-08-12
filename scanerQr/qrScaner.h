#ifndef QRSCANER_H
#define QRSCANER_H

#include <QObject>
#include "dataBase/dataBase.h"

class QrScaner : public QObject
{
    Q_OBJECT
public:
    explicit QrScaner(QObject *parent = nullptr);

    Q_PROPERTY(bool isValid READ getIsValid NOTIFY qrCodeIsValidChanged)

signals:
    void qrCodeUpdateSerialNum(QString number);
    void qrCodeError(QString message);
    void qrCodeIsValidChanged();

public slots:
    void insertQrData(QString codeData);

private:
    bool getIsValid() { return isValid; }
    void setIsValid(bool value) { isValid = value; emit qrCodeIsValidChanged(); }

    bool isValid;
};

#endif // QRSCANER_H
