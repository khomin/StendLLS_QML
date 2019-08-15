#include "stend/stendApi.h"
#include <QtDebug>
#include "settings.h"
#include <QDateTime>

StendApi::StendApi(QObject *parent) : QObject(parent) {
    interval_read_info_counter = 0;

    /* константы теста емкости */
    capValues.min[0] = Settings::Instance().getCap1Min().toInt();
    capValues.min[1] = Settings::Instance().getCap2Min().toInt();
    capValues.min[2] = Settings::Instance().getCap3Min().toInt();
    capValues.max[0] = Settings::Instance().getCap1Max().toInt();
    capValues.max[1] = Settings::Instance().getCap2Max().toInt();
    capValues.max[2] = Settings::Instance().getCap3Max().toInt();

    /* programming mcu */
    programming = new Programming();
    programmingThread = new QThread();
    programming->moveToThread(programmingThread);
    connect(programmingThread, SIGNAL(started()), programming, SLOT(initThread()));
    programmingThread->start();

    this->handlerStendTimer = new QTimer();
    handlerStendTimer->start(500);
    this->timeoutCommandTimer = new QTimer();

    connect(handlerStendTimer, &QTimer::timeout,this, [&]() {
        if(getStendIsConnected()) {
            //--- if command list is empty -> get basic_param
            if(command.isEmpty()) {
                command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::get_basic_param, QJsonObject()));
            }
            /* send requst */
            sendCommand(command.first().first, command.first().second);
            handlerStendTimer->stop();
            timeoutCommandTimer->start(2000);
        }
    });

    connect(timeoutCommandTimer, &QTimer::timeout,this, [&]() {
        timeoutCommandTimer->stop();
        handlerStendTimer->start();
        emit stendNotReply(tr("Not reply"));
    });

    connect(programming, &Programming::exeError,this, [&](QString err) {
        QJsonObject jsonObj;
        jsonObj.insert("testResult", false);
        jsonObj.insert("testStep", "programming");
        jsonObj.insert("percent", 0);
        jsonObj.insert("message", err);
        setStendIsisProcessed(false);
        emit testError(QJsonDocument(jsonObj).toJson());
    });
    connect(programming, &Programming::programmFinished,this, [&]() {
        programming->readSerialNumMcu();
    });
    connect(programming, &Programming::exeReadSerialNumMcu,this, [&](QString mcuNum) {
        /* Добавляем плату в базу */
        try {
            mMcuNumber = mcuNum;
            DataBase::Instance().insertPcbTestData(mcuNum, "inited", llsInfoStruct.test, true);
        } catch(QString ex) {
            emit dataBaseError(ex);
        }
        command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::dut_power_down, QJsonObject()));
    });
    connect(programming, &Programming::exeUpdate,this, [&](int percent) {
        QJsonObject jsonObj;
        jsonObj.insert("testResult", true);
        jsonObj.insert("testStep", "programming");
        jsonObj.insert("percent", percent);
        emit testUpdateStep(QJsonDocument(jsonObj).toJson());
    });
}

StendApi::~StendApi() {
    delete handlerStendTimer;
}

void StendApi::insertDataFromInterface(QByteArray data) {
    if(!command.isEmpty()) {
        if(parsinReply(command.last().first, data)) {
            timeoutCommandTimer->stop();
            handlerStendTimer->start();
            command.removeFirst(); /* flush command */
            setStendIsConnected(true);
        } else {
            emit stendNotReply(tr("Not reply"));
            setStendIsConnected(false);
        }
    }
}

void StendApi::setStatusConnected() {
    command.clear();
    setStendIsConnected(true);
}

void StendApi::setStatusDisconnected() {
    command.clear();
    setStendIsConnected(false);
}

void StendApi::insertErrorFromInterface(QString data) {
    emit stendNotReply(tr("Interface was closed: ") + data);
}

void StendApi::sendCommand(StendProperty::eTypeCommand cmd, QJsonObject jsonArgs) {
    QByteArray dataTx;
    /* template struct for transmit */
    StendProperty::sOutputTcpTempStruct outputTcpTempStruct;
    outputTcpTempStruct.command = cmd;
    outputTcpTempStruct.stend_version = Globals::stend_version;;
    /* константы теста емкости */
    memcpy(&outputTcpTempStruct.capValues, &capValues, sizeof(capValues));
    outputTcpTempStruct.command = cmd;
    outputTcpTempStruct.stend_version = Globals::stend_version;;
    /* константы теста емкости */
    outputTcpTempStruct.capValues = capValues;

    switch(cmd) {
    case StendProperty::get_basic_param: /* запрашиваем базовый пакет */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::reset_test: // сброс теста
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::start_test: // старт теста
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::dut_power_down: /* выключение питания дут */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::dut_power_up: /* включиние питания дут */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
        emit readyWriteDataToInterface(dataTx);
        command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::start_test, QJsonObject()));
        break;

    case StendProperty::reboot: /* перезагрузка стенда */
        break;
    case StendProperty::write_serial_dut:
        QString writeSn = jsonArgs.value("sn").toString();
        if(writeSn.length() <= (int)sizeof(outputTcpTempStruct.writed_serial_num)) {
            strcpy(outputTcpTempStruct.writed_serial_num, writeSn.toUtf8().data());
            packetInsert(outputTcpTempStruct, dataTx, QByteArray(Globals::stend_header));
            emit readyWriteDataToInterface(dataTx);
        }
        break;
    }
}

bool StendApi::headerIsValid(QByteArray & dataRx) {
    if(dataRx.isEmpty()) {
        return false;
    }
    /* check header */
    if(!dataRx.contains(QByteArray(Globals::stend_header))) {
        return false;
    }
    StendProperty::sInputTcpTempStruct* inputTempStructInfo = nullptr;
    if(dataRx.length() >= sizeof(StendProperty::sInputTcpTempStruct) + sizeof(Globals::stend_header)) {
        inputTempStructInfo = (StendProperty::sInputTcpTempStruct*)((uint8_t*) dataRx.data() + strlen(Globals::stend_header));
        if(inputTempStructInfo->version == Globals::stend_version) {
            return true;
        } else {
            qDebug() << "version not compare";
        }
    }
    return false;
}

bool StendApi::parsinReply(StendProperty::eTypeCommand cmd, QByteArray & dataRx) {
    bool res = false;
    if(headerIsValid(dataRx)) {
        StendProperty::sInputTcpTempStruct* pinputTemp = (StendProperty::sInputTcpTempStruct*)((uint8_t*) dataRx.data() + strlen(Globals::stend_header));
        switch(cmd) {
        /* запрашиваем базовый пакет */
        case StendProperty::get_basic_param: {

            copyInputData(llsInfoStruct, pinputTemp);

            emit updateLastData(llsInfoStruct);

            /* update fields */
            QJsonObject jsonObj = convertDataToJson(llsInfoStruct);
            emit updateRealTimeData(QJsonDocument(jsonObj).toJson());

            jsonObj = evaluateTestStatus(llsInfoStruct.test);
            if(testIsFinished(llsInfoStruct.test)) {
                if(!mTestIsFinished){
                    mTestIsFinished = true;
                    DataBase::Instance().insertPcbTestData(
                                mMcuNumber,
                                "programming",
                                llsInfoStruct.test,
                                llsInfoStruct.test.state == Globals::test_ok);
                    setStendIsisProcessed(false);
                    emit testFinihed(QJsonDocument(jsonObj).toJson());
                }
            } else {
                emit testUpdateStep(QJsonDocument(jsonObj).toJson());
            }
            res = true;
        }
            break;

        case StendProperty::reset_test:
            res = true;
            break;

        case StendProperty::start_test:
            res = true;
            break;

        case StendProperty::reboot: /* перезагрузка стенда */
            res = true;
            break;

        case StendProperty::dut_power_down: /* питания дут */
            res = true;
            command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::dut_power_up, QJsonObject()));
            break;

        case StendProperty::dut_power_up: /* питания дут */
            res = true;
            command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::start_test, QJsonObject()));
            break;

        case StendProperty::write_serial_dut:
            res = true;
            emit goodMessage(tr("The serial number was successfully assigned to the device"));
            try {
                DataBase::Instance().placeResult(false, qrCode, mcuSn, "test_defect", QString(json_doc.toJson(QJsonDocument::Compact)));
                emit goodMessage(tr("Device added to broken table"));
            } catch(QString ex) {
                emit dataBaseError(ex);
            }
            break;
        }
    }

    return res;
}

/* pack raw packet before send */
void StendApi::packetInsert(StendProperty::sOutputTcpTempStruct pOut, QByteArray & pArray, QByteArray packHeader) {
    pArray.setRawData((char*)&pOut, sizeof(pOut));
    /* вставляем заголовок */
    pArray.push_front(packHeader);
}

void StendApi::startTest() {
    QString error;
    QJsonObject jsonObj;
    try {
        if(DataBase::Instance().openConnection(&error)) {
            if(mStendIsConnected) {
                command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::reset_test, QJsonObject()));
                programming->startProgramm();
                mMcuNumber = "";
                mTestIsFinished = false;
                setStendIsisProcessed(true);
            }
        } else { /* not working */
            jsonObj.insert("testResult", false);
            jsonObj.insert("database", "database");
            jsonObj.insert("message", error);
            setStendIsisProcessed(false);
            emit testError(QJsonDocument(jsonObj).toJson());
        }
    } catch(QString ex) {
        jsonObj.insert("testResult", false);
        jsonObj.insert("database", "database");
        jsonObj.insert("message", ex);
        setStendIsisProcessed(false);
        emit testError(QJsonDocument(jsonObj).toJson());
    }
}

QJsonObject StendApi::convertDataToJson(Globals::sDutBaseStruct & dutStruct) {
    QJsonObject jsonObj;
    jsonObj.insert("cnt", (int)dutStruct.cnt);
    jsonObj.insert("addr", dutStruct.addr);
    jsonObj.insert("test.state", dutStruct.test.state);
    jsonObj.insert("test.freq_state", dutStruct.test.freq_state);;
    jsonObj.insert("test.rs232_state", dutStruct.test.rs232_state);
    jsonObj.insert("test.rs485_state", dutStruct.test.rs485_state);
    jsonObj.insert("test.cap_test_result[0]", (int)dutStruct.test.cap_test_result[0]);
    jsonObj.insert("test.cap_test_result[1]", (int)dutStruct.test.cap_test_result[1]);
    jsonObj.insert("test.cap_test_result[2]", (int)dutStruct.test.cap_test_result[2]);
    jsonObj.insert("test.rs232_send_packet", dutStruct.test.rs232_send_packet);
    jsonObj.insert("test.rs232_receive_packet", dutStruct.test.rs232_receive_packet);
    jsonObj.insert("test.rs232_state", dutStruct.test.rs232_state);
    jsonObj.insert("test.rs485_send_packet", dutStruct.test.rs485_send_packet);
    jsonObj.insert("test.rs485_receive_packet", dutStruct.test.rs485_receive_packet);
    jsonObj.insert("test.rs485_state", dutStruct.test.rs485_state);
    jsonObj.insert("power_3_3", dutStruct.power_3_3);
    jsonObj.insert("power_input", dutStruct.power_input);
    jsonObj.insert("programminging.serial_num", dutStruct.programming.serial_num);
    jsonObj.insert("power_current", dutStruct.power_current);
    jsonObj.insert("temp", dutStruct.currentDataRs232.temperature);
    jsonObj.insert("freq", dutStruct.currentDataRs232.isValid ? dutStruct.currentDataRs232.freq : dutStruct.currentDataRs485.freq);

    /* cnt collect*/
    static QJsonArray powerVoltageCollect;
    static QJsonArray powerCurrentCollect;
    static QJsonArray cntCollect;
    static QJsonArray timeCollect;

    QJsonObject powerVoltage
    {
        {"x", QDateTime::currentDateTime().toMSecsSinceEpoch()},
        {"y", (int)dutStruct.power_input}
    };

    powerVoltageCollect.append(powerVoltage);
    if(powerVoltageCollect.count() > maxCollectArrayData) {
        powerVoltageCollect.pop_front();
    }

    QJsonObject powerCurrent
    {
        {"x", QDateTime::currentDateTime().toMSecsSinceEpoch()},
        {"y", (int)dutStruct.power_current}
    };
    powerCurrentCollect.append(powerCurrent);
    if(powerCurrentCollect.count() > maxCollectArrayData) {
        powerCurrentCollect.pop_front();
    }

    QJsonObject cnt
    {
        {"x", QDateTime::currentDateTime().toMSecsSinceEpoch()},
        {"y", (int)dutStruct.cnt}
    };
    cntCollect.append(cnt);
    if(cntCollect.count() > maxCollectArrayData) {
        cntCollect.pop_front();
    }

    jsonObj.insert("powerCollect", powerVoltageCollect);
    jsonObj.insert("currentCollect", powerCurrentCollect);
    jsonObj.insert("cntCollect", cntCollect);

    QString factorySn;
    for(uint8_t i=0; i<sizeof(dutStruct.serial_number); i++) {
        factorySn += QString::number(dutStruct.serial_number[i], 16);
    }
    jsonObj.insert("serial_number", QString::fromUtf8(dutStruct.serial_number, sizeof(dutStruct.serial_number)));
    jsonObj.insert("program_version", dutStruct.program_version);

    jsonObj.insert("mcu_serial_number", Globals::hexToString(dutStruct.mcu_serial_number, sizeof(dutStruct.mcu_serial_number)));
    jsonObj.insert("bootloader_version", dutStruct.bootloader_version);
    return jsonObj;
}

void StendApi::copyInputData(Globals::sDutBaseStruct & dutStruct, const StendProperty::sInputTcpTempStruct* pinputTemp) {
    Globals::S_curves_data curves_data;
    dutStruct.addr = pinputTemp->info.addr;
    dutStruct.power_3_3 = pinputTemp->info.power_3_3;
    dutStruct.power_input = pinputTemp->info.power_input;
    dutStruct.power_current = pinputTemp->info.power_current;
    dutStruct.power_state = pinputTemp->info.power_state;
    curves_data.power_3_3 = dutStruct.power_3_3;
    curves_data.power_current = dutStruct.power_current;
    curves_data.power_input = dutStruct.power_input;
    curves_data.cnt = pinputTemp->info.cnt;
    dutStruct.cnt = pinputTemp->info.cnt;
    curves_data_vector.push_back(curves_data);
    if(curves_data_vector.size() > 500) {
        curves_data_vector.clear();
    }

    if((pinputTemp->info.serial_number[0]>'9') || (pinputTemp->info.serial_number[0]<'0')) {
        strcpy(dutStruct.serial_number, QString("------------").toStdString().data());
    } else {
        memcpy(&dutStruct.serial_number, &pinputTemp->info.serial_number, sizeof(pinputTemp->info.serial_number));
    }
    memcpy(&dutStruct.mcu_serial_number, &pinputTemp->info.mcu_serial_number, sizeof(pinputTemp->info.mcu_serial_number));
    memcpy(&dutStruct.program_version, &pinputTemp->info.program_version, sizeof(pinputTemp->info.program_version));

    memcpy(&dutStruct.currentDataRs232, &pinputTemp->info.currentDataRs232, sizeof(pinputTemp->info.currentDataRs232));
    memcpy(&dutStruct.currentDataRs485, &pinputTemp->info.currentDataRs485, sizeof(pinputTemp->info.currentDataRs485));

    dutStruct.dateTime = pinputTemp->info.dateTime;
    dutStruct.test = pinputTemp->info.test;
}

QString StendApi::convertTestStateToString(Globals::eTestState state) {
    switch (state) {
    case Globals::idle: return "idle";
    case Globals::process: return "process";
    case Globals::test_err: return "fail";
    case Globals::test_ok: return "finished";
    }
}

bool StendApi::testIsFinished(Globals::sDutTestStruct & tests) {
    if((tests.state == Globals::test_ok) || (tests.state == Globals::test_err)) {
        return true;
    }
    return false;
}

void StendApi::testDatabaseConnect() {
    QTimer::singleShot(500, this, [&]() {
        DataBase::Instance().testCconnect() ?
                    emit goodMessage(tr("Database сonnection established")) :
                    emit badMessage(tr("Database сonnection not established"));
    });
}

bool StendApi::isAvailableLlsRangeValues() {
    if(llsInfoStruct.power_current >= Settings::Instance().getCurMin().toFloat()
            && llsInfoStruct.power_current <= Settings::Instance().getCurMax().toFloat()) {
        return true;
    }
    return false;
}

void StendApi::writeSerialNumToLls(QString sn) {
    QJsonObject object;
    object.insert("sn", sn);
    command.push_back(QPair<StendProperty::eTypeCommand, QJsonObject> (StendProperty::write_serial_dut, object));
}

void StendApi::saveAssemblyTestLlsToDb(QString mcuNum) {
    try {
        if(DataBase::Instance().insertAsseblyTestResult(mcuNum, "tested", llsInfoStruct.test, true)) {
            emit goodMessage(tr("Test result was saved"));
        } else {
            emit badMessage(tr("Test result was not saved"));
        }
    } catch(QString ex) {
        emit dataBaseError(ex);
    }
}

void StendApi::markLlsAsDefective(QString mcuSn, QString qrCode, QString jsonDataStr) {
    auto jsonObj = QJsonDocument(QJsonDocument::fromJson(jsonDataStr.toUtf8())).object();
    jsonObj["test_datetime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument json_doc(jsonObj);
    try {
        DataBase::Instance().placeResult(false, qrCode, mcuSn, "test_defect", QString(json_doc.toJson(QJsonDocument::Compact)));
        emit goodMessage(tr("Device added to broken table"));
    } catch(QString ex) {
        emit dataBaseError(ex);
    }
}

QJsonObject StendApi::evaluateTestStatus(Globals::sDutTestStruct & tests) {
    QJsonObject json;
    QJsonObject json232;
    QJsonObject json485;
    QJsonObject jsonFreq;
    //-- rs232
    json232.insert("testResult", convertTestStateToString(tests.rs232_state));
    json232.insert("sendPackets", tests.rs232_send_packet);
    json232.insert("receivePackets", tests.rs232_receive_packet);
    //-- rs485
    json485.insert("testResult", convertTestStateToString(tests.rs485_state));
    json485.insert("sendPackets", tests.rs485_send_packet);
    json485.insert("receivePackets", tests.rs485_receive_packet);
    //-- freq
    jsonFreq.insert("testResult", convertTestStateToString(tests.freq_state));
    jsonFreq.insert("capStep1", (int)tests.cap_test_result[0]);
    jsonFreq.insert("capStep2", (int)tests.cap_test_result[1]);
    jsonFreq.insert("capStep3", (int)tests.cap_test_result[2]);
    //-- result
    json.insert("testStep", "waitTestNotEnd");
    json.insert("test232", json232);
    json.insert("test485", json485);
    json.insert("testFreq", jsonFreq);
    json.insert("result", convertTestStateToString(tests.state));
    return json;
}
