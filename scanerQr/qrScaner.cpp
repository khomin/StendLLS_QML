#include "qrScaner.h"

QrScaner::QrScaner(QObject *parent) : QObject(parent)
{
    setIsValid(false);
    setQrCodeSn("");
}

void QrScaner::insertQrData(QString codeData) {
    QString code_res = codeData.section("#", 3, 3);

    mQrCode = codeData;

    if(code_res.length() != 0) {
        QString header = codeData.section("#", 1, 1) + " " + codeData.section("#", 2, 2);
        try {
            if(DataBase::Instance().checkDeviceQrCode(codeData)) {
                emit qrCodeUpdateSerialNum(codeData);
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
