#include "dataBase.h"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonDocument"
#include "QDateTime"
#include "settings.h"

DataBase::DataBase() {}

DataBase::~DataBase() {}

bool DataBase::openConnection(QString *error) {
    bool res = false;
    QString str;
    if(!QSqlDatabase::isDriverAvailable("QPSQL")) {
        *error = tr("Driver QPSQL not available");
        return false;
    }
    base = QSqlDatabase::addDatabase("QPSQL");
    base.setHostName(Settings::Instance().getDatabaseHost());
    base.setDatabaseName(Settings::Instance().getDatabaseName());
    base.setUserName(Settings::Instance().getDatabaseUser());
    base.setPassword(Settings::Instance().getDatabasePassword());
    base.setPort(Settings::Instance().getDatabasePort().toInt());
    base.setConnectOptions("connect_timeout=1");

    if(base.open()) {
        qDebug() << "Connection QPSQL OK!";
        res = true;
    } else {
        qDebug() << QString(base.lastError().text());
        *error = QString(base.lastError().text());
    }
    return res;
}

bool DataBase::closeConnection()
{
    if(base.isOpen()==true)
        base.close();
    return true;
}

bool DataBase::insertTestData(QString mcu, QString dev_test_state, sDutTestStruct res_data_test, bool res_test) {
    bool res = false;
    QSqlQuery sql_query(base);
    QJsonObject json_object;
    //
    json_object["freq_state"] = 1;
    for(int i=0; i<3; i++)
        json_object["freq_value" + QString::number(i)] = QString::number(res_data_test.cap_test_result[i]);
    json_object["rs232_state"] = res_data_test.rs232_state;
    json_object["rs232_pack_tx"] = res_data_test.rs232_send_packet;
    json_object["rs232_pack_rx"] = res_data_test.rs232_receive_packet;
    json_object["rs485_state"] = res_data_test.rs485_state;
    json_object["rs485_pack_rx"] = res_data_test.rs485_receive_packet;
    json_object["rs485_pack_tx"] = res_data_test.rs485_send_packet;
    json_object["test_datetime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument json_doc(json_object);
    QString json = json_doc.toJson(QJsonDocument::Compact);

    QSqlQuery query;
    query.prepare("SELECT pr_insert_pcb_test_result(?, ?, ?, ?)");
    query.addBindValue(QVariant(mcu).toString());
    query.addBindValue(json);
    query.addBindValue(res_test);
    query.addBindValue(USER_ID);
    res = query.exec();
    if(!res) {
        qDebug() << query.lastError();
    }
    return res;
}

bool DataBase::testCconnect() {
    return base.open();
}
