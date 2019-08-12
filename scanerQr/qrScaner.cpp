#include "qrScaner.h"

QrScaner::QrScaner(QObject *parent) : QObject(parent)
{

}

void QrScaner::insertQrData(QString codeData) {
    QString code_res = codeData.section("#", 3, 3);
    if(code_res.length() != 0) {
        QString a = codeData.section("#", 3, 3);

        QString b = codeData.section("#", 1, 1) + " " + codeData.section("#", 2, 2);

        if(DataBase::Instance().checkDeviceQrCode(codeData)) {
            emit qrCodeUpdateSerialNum(codeData);
            setIsValid(true);
        } else {
            setIsValid(false);
            emit qrCodeError(tr("Scaner error, the number is not in the order list"));
        }
    } else {
        setIsValid(false);
        emit qrCodeError(tr("Scaner error, the number is not valid"));
    }
}
