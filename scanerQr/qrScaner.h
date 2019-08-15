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
    Q_PROPERTY(QString qrCodeSn READ getQrCodeSn NOTIFY qrCodeSnChanged)

    Q_INVOKABLE QString getQrCode() {
        return mQrCode;
    }

signals:
    void qrCodeUpdateSerialNum(QString number);
    void qrCodeError(QString message);
    void qrCodeIsValidChanged();
    void qrCodeSnChanged();

public slots:
    void insertQrData(QString codeData);

private:
    bool getIsValid() { return mIsValid; }
    void setIsValid(bool value) { mIsValid = value; emit qrCodeIsValidChanged(); }

    QString getQrCodeSn() {
        return mQrCodeSn;
    }
    void setQrCodeSn(QString value) { mQrCodeSn = value; emit qrCodeSnChanged(); }

    bool mIsValid;
    QString mQrCodeSn;
    QString mQrCode;
};

#endif // QRSCANER_H
