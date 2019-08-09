#include "qrScaner.h"

QrScaner::QrScaner(QObject *parent) : QObject(parent)
{

}

void QrScaner::insertQrData(QString codeData) {
    QString code_res = codeData.section("#", 3, 3);
    if(code_res.length() != 0) {
        // ui->stend_scan->setText(code.section("#", 3, 3));
        QString a = codeData.section("#", 3, 3);

        // ui->dev_info->setText(code.section("#", 1, 1) + " " + code.section("#", 2, 2));
        QString b = codeData.section("#", 1, 1) + " " + codeData.section("#", 2, 2);

        //-- проверка уровней lls пройдена
        if(DataBase::Instance().checkDeviceQrCode(codeData)) {
            emit qrCodeUpdateSerialNum(codeData); //
        } else {
            qDebug() << "Scaner eror, the number is not in the order list";
            emit qrCodeError(tr("Scaner eror, the number is not in the order list"));
        }
    } else {
        qDebug() << "Scaner eror, the number is not valid";
        emit qrCodeError(tr("Scaner eror, the number is not valid"));
    }
}
