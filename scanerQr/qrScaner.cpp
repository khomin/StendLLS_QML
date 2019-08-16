#include "qrScaner.h"

QrScaner::QrScaner(QObject *parent) : QObject(parent)
{
    setIsValid(false);
    setQrCodeSn("");
}

void QrScaner::insertQrData(QString codeData) {
    mQrCode = codeData;
    QString code_res = mQrCode.section("#", 3, 3);
    if(code_res.length() != 0) {
        QString header = mQrCode.section("#", 1, 1) + " " + mQrCode.section("#", 2, 2);
        try {
            if(DataBase::Instance().checkDeviceQrCode(mQrCode)) {
                emit qrCodeUpdateSerialNum(mQrCode);
                setQrCodeSn(code_res);
                setIsValid(true);
            } else {
                setQrCodeSn("NA");
                setIsValid(false);
                emit qrCodeError(tr("Scaner error, the number is not in the order list"));
            }
        } catch(QString ex) {
            emit qrCodeError(ex);
        }
    } else {
        setQrCodeSn("NA");
        setIsValid(false);
        emit qrCodeError(tr("Scaner error, the number is not valid"));
    }
}

QString QrScaner::convertCodeToSerialNumber(QString qrCode) {
    return qrCode.section("#", 3, 3);
}

void QrScaner::resetData() {
    setIsValid(false);
    setQrCodeSn("");
}
