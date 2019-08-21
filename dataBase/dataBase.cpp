#include "dataBase.h"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonDocument"
#include "QDateTime"
#include "settings.h"

DataBase::DataBase() {
    openConnection(nullptr);
}

DataBase::~DataBase() {}

bool DataBase::openConnection(QString *error) {
    bool res = false;
    QString str;
    if(!QSqlDatabase::isDriverAvailable("QPSQL")) {
        if(error != nullptr) {
            *error = tr("Driver QPSQL not available");
        }
        emit isOpenedChanged();
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
        if(error != nullptr) {
            *error = QString(base.lastError().text());
        }
    }
    emit isOpenedChanged();
    return res;
}

bool DataBase::isConnected() {
    return base.isOpen();
}

bool DataBase::closeConnection() {
    if(base.isOpen()==true)
        base.close();
    emit isOpenedChanged();
    return true;
}

bool DataBase::insertPcbTestData(QString mcu, QString dev_test_state, Globals::sDutTestStruct res_data_test, bool res_test) {
    bool res = false;
    Q_UNUSED(dev_test_state);

    if(!base.isOpen()) {
        openConnection(nullptr);
    }

    QSqlQuery query;

    try {
        QSqlQuery sql_query(base);
        QJsonObject json_object;

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

        qDebug() << "mcu: " << QVariant(mcu).toString();
        qDebug() << "json: " << json;
        qDebug() << "res_test: " << res_test;
        qDebug() << "USER_ID: " << USER_ID;

        query.prepare("SELECT pr_insert_pcb_test_result(?, ?, ?, ?)");
        query.addBindValue(QVariant(mcu).toString());
        query.addBindValue(json);
        query.addBindValue(res_test);
        query.addBindValue(USER_ID);
        res = query.exec();
        if(!res) {
            qDebug() << query.lastError();
            throw ((tr("Database error")) + ": " + query.lastError().databaseText());
        }
    } catch(...) {
        qDebug() << "Exeption: " << query.lastError();
        qDebug() << query.executedQuery();
    }
    return res;
}

bool DataBase::insertAsseblyTestResult(QString mcu, QString dev_test_state, Globals::sDutTestStruct res_data_test, bool res_test) {
    bool res = false;
    Q_UNUSED(dev_test_state);

    if(!base.isOpen()) {
        openConnection(nullptr);
    }

    QSqlQuery query;

    try {
        QSqlQuery sql_query(base);
        QJsonObject json_object;

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

        qDebug() << "mcu: " << QVariant(mcu).toString();
        qDebug() << "json: " << json;
        qDebug() << "res_test: " << res_test;
        qDebug() << "USER_ID: " << USER_ID;

        query.prepare("SELECT pr_insert_lls_assembly_test_result(?, ?, ?, ?)");
        query.addBindValue(QVariant(mcu).toString());
        query.addBindValue(json);
        query.addBindValue(res_test);
        query.addBindValue(USER_ID);
        res = query.exec();
        if(!res) {
            qDebug() << query.lastError();
            throw ((tr("Database error")) + ": " + query.lastError().databaseText());
        }
    } catch(...) {
        qDebug() << "Exeption: " << query.lastError();
        qDebug() << query.executedQuery();
    }
    return res;
}

bool DataBase::checkDeviceQrCode(QString qr_code) {
    bool res = false;
    if(!base.isOpen()) {
        openConnection(nullptr);
    }
    QSqlQuery query(base);
    QString prefix, dts, dev_serial, client;
    prefix = qr_code.section("#", 0, 0);
    client = qr_code.section("#", 1, 1);
    dts = qr_code.section("#", 2, 2);
    dev_serial = qr_code.section("#", 3, 3);
    dev_serial.remove('\r');
    dev_serial.remove('\n');

    query.prepare("SELECT pr_check_device_qr_code(?, ?, ?, ?);");
    query.addBindValue(QVariant(prefix).toString());
    query.addBindValue(QVariant(client).toString());
    query.addBindValue(QVariant(dts).toString());
    query.addBindValue(QVariant(dev_serial).toString());
    res = query.exec();
    qDebug() << query.boundValue(6);
    if(!res) {
        qDebug() << query.lastError();
        throw ((tr("Database error")) + ": " + query.lastError().databaseText());
    }
    query.next();

    return (query.record().value("pr_check_device_qr_code") == true);
}

bool DataBase::testCconnect() {
    QString err;
    return openConnection(&err);
}

QVector <Globals::sTypeDevicesList> DataBase::getDeviceFullDeviceList() {
    QVector <Globals::sTypeDevicesList> res;
    if(!base.isOpen()) {
        openConnection(nullptr);
    }
    QSqlQuery query(base);
    query.prepare("SELECT dts_id, dts_name, dts_other FROM public.device_subtype;");
    if(query.exec()) {
        while(query.next()) {
            Globals::sTypeDevicesList tdevs;
            tdevs.dts_name = query.record().value("dts_name").toString();
            tdevs.values.dts_id = query.record().value("dts_id").toInt();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(query.record().value(2).toString().toLocal8Bit());
            QJsonObject jsonObject = jsonDoc.object();
            tdevs.values.cnt_value_min = jsonObject["cnt_value_min"].toVariant().toFloat();
            tdevs.values.cnt_value_max = jsonObject["cnt_value_max"].toVariant().toFloat();
            tdevs.values.voltage_value_min = jsonObject["voltage_value_min"].toVariant().toFloat();
            tdevs.values.voltage_value_max = jsonObject["voltage_value_max"].toVariant().toFloat();
            tdevs.values.freq_value_min= jsonObject["freq_value_min"].toVariant().toFloat();
            tdevs.values.freq_value_max = jsonObject["freq_value_max"].toVariant().toFloat();
            tdevs.values.nominal_current = jsonObject["nominal_current"].toVariant().toFloat();
            tdevs.values.tolerance_level = jsonObject["tolerance_level"].toVariant().toFloat();
            tdevs.values.empty_value_minumum_correct = jsonObject["empty_value_minumum_correct"].toVariant().toInt();
            tdevs.values.full_value_minumum_correct = jsonObject["full_value_minumum_correct"].toVariant().toInt();
            tdevs.values.tolerance_current = jsonObject["tolerance_current"].toVariant().toFloat();
            tdevs.values.freq_value_max = jsonObject["freq_value_max"].toVariant().toFloat();
            res.push_back(tdevs);
        }
    } else {
        qDebug() << query.lastError();
    }
    return res;
}

bool DataBase::placeResult(bool res_bool, QString qrCode, QString mcu, QString dev_status, QString resultJson) {
    const QString pr_update_device_pcb = "pr_update_device_pcb";
    bool res = false;

    if(!base.isOpen()) {
        openConnection(nullptr);
    }

    QSqlQuery query(base);
    QString prefix, dts, dev_serial, client;
    prefix = qrCode.section("#", 0, 0);
    client = qrCode.section("#", 1, 1);
    dts = qrCode.section("#", 2, 2);
    dev_serial = qrCode.section("#", 3, 3);
    dev_serial.remove('\r');
    dev_serial.remove('\n');
    mcu = mcu.toUpper();

    query.prepare("SELECT pr_update_device_pcb(?, ?, ?, ?, ?, ?, ?, ?);");
    query.addBindValue(prefix);
    query.addBindValue(client);
    query.addBindValue(dts);
    query.addBindValue(dev_serial);
    query.addBindValue(mcu);
    query.addBindValue(USER_ID);
    query.addBindValue(resultJson);
    query.addBindValue(QVariant(res_bool).toString());

    res = query.exec();
    qDebug() << query.boundValue(6);
    if(!res) {
        qDebug() << query.lastError();
    }
    query.next();
    if(query.record().indexOf(pr_update_device_pcb)) {
        throw tr("No result recorded. This device number is not in the database");
    }
    return res;
}

bool DataBase::getLlsMaxMinValues(int dts_id, float *cnt_value_min, float *cnt_value_max,
                                  float *voltage_value_min, float *voltage_value_max,
                                  float *freq_value_min, float *freq_value_max,
                                  float *nominal_current,
                                  float *tolerance_level,
                                  uint32_t *empty_value_minumum_correct,
                                  uint32_t *full_value_minumum_correct,
                                  float *tolerance_current)
{
    bool res = false;

    if(!base.isOpen()) {
        openConnection(nullptr);
    }

    QSqlQuery query(base);
    query.prepare("SELECT dts_other FROM public.device_subtype INNER JOIN device_type ON dts_dt_id = dt_id WHERE dts_id =? AND dt_prefix = 'FLS';");
    query.addBindValue(dts_id);
    res = query.exec();
    if(res) {
        query.next();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(query.record().value(0).toString().toLocal8Bit());
        QJsonObject jsonObject = jsonDoc.object();
        *cnt_value_min = jsonObject["cnt_value_min"].toVariant().toFloat();
        *cnt_value_max = jsonObject["cnt_value_max"].toVariant().toFloat();
        *voltage_value_min = jsonObject["voltage_value_min"].toVariant().toFloat();
        *voltage_value_max = jsonObject["voltage_value_max"].toVariant().toFloat();
        *freq_value_min= jsonObject["freq_value_min"].toVariant().toFloat();
        *freq_value_max = jsonObject["freq_value_max"].toVariant().toFloat();
        //
        *nominal_current = jsonObject["nominal_current"].toVariant().toFloat();
        *tolerance_level = jsonObject["tolerance_level"].toVariant().toFloat();
        *empty_value_minumum_correct = jsonObject["empty_value_minumum_correct"].toVariant().toInt();
        *full_value_minumum_correct = jsonObject["full_value_minumum_correct"].toVariant().toInt();
        *tolerance_current = jsonObject["tolerance_current"].toVariant().toFloat();
        *freq_value_max = jsonObject["freq_value_max"].toVariant().toFloat();
    } else {
        qDebug() << query.lastError();
        throw ((tr("Database error")) + ": " + query.lastError().databaseText());
    }
    return res;
}

void DataBase::checkUserPermission(QString login, QString password) {
    QString res;
    if(!base.isOpen()) {
        openConnection(nullptr);
    }
    QSqlQuery query(base);
    query.prepare("SELECT user_role FROM public.\"user\" WHERE user_login = ? AND user_hash = ?;");
    query.addBindValue(login);
    query.addBindValue(password);
    if(query.exec()) {
        query.next();
        if(query.record().value(0).toInt() == 1) {
            res = "admin";
        } else if(query.record().value(0).toInt() == 2) {
            res = "user";
        }
    } else {
        qDebug() << query.lastError();
        throw ((tr("Database error")) + ": " + query.lastError().databaseText());
    }
    qDebug() << "userRole: " << res;
    emit userPermissionReadyRead(res);
}
