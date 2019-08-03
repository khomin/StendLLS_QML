#include "stend/stendApi.h"
#include <QtDebug>
#include "settings.h"

StendApi::StendApi(QObject *parent) : QObject(parent) {
    dataBase = new DataBase();

    command.push_back(StendProperty::get_basic_param);
    interval_read_info_counter = 0;

    connect_state = StendProperty::disconnected;

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
        if(connect_state == StendProperty::connected) {
            //--- if command list is empty -> get basic_param
            if(command.isEmpty()) {
                command.push_back(StendProperty::get_basic_param);
            }
            /* send requst */
            sendCommand(command.first());
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
        emit testError(QJsonDocument(jsonObj).toJson());
    });
    connect(programming, &Programming::programmFinished,this, [&]() {
        programming->readSerialNumMcu();
    });
    connect(programming, &Programming::exeReadSerialNumMcu,this, [&](QString mcuNum) {
        /* Добавляем плату в базу */
        dataBase->dataBaseStruct.pcb.dut.serial_num_cpu = mcuNum;
        command.push_back(StendProperty::dut_power_down);
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
        if(parsinReply(command.last(), data)) {
            timeoutCommandTimer->stop();
            handlerStendTimer->start();
            command.removeFirst(); /* flush command */
            connect_state = StendProperty::connected;
        } else {
            emit stendNotReply(tr("Not reply"));
            connect_state = StendProperty::disconnected;
        }
    }
}

void StendApi::setStatusConnected() {
    connect_state = StendProperty::connected;
}

void StendApi::setStatusDisconnected() {
    command.clear();
    connect_state = StendProperty::disconnected;
}

void StendApi::insertErrorFromInterface(QString data) {
    emit stendNotReply(tr("Interface was closed: ") + data);
}

void StendApi::sendCommand(StendProperty::eTypeCommand cmd) {
    QByteArray dataTx;
    /* template struct for transmit */
    StendProperty::sOutputTcpTempStruct outputTcpTempStruct;
    outputTcpTempStruct.command = cmd;
    outputTcpTempStruct.stend_version = stend_version;;
    /* константы теста емкости */
    memcpy(&outputTcpTempStruct.capValues, &capValues, sizeof(capValues));
    outputTcpTempStruct.command = cmd;
    outputTcpTempStruct.stend_version = stend_version;;
    /* константы теста емкости */
    outputTcpTempStruct.capValues = capValues;

    switch(cmd) {
    case StendProperty::get_basic_param: /* запрашиваем базовый пакет */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::reset_test: // сброс теста
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::start_test: // старт теста
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::dut_power_down: /* выключение питания дут */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(stend_header));
        emit readyWriteDataToInterface(dataTx);
        break;

    case StendProperty::dut_power_up: /* включиние питания дут */
        packetInsert(outputTcpTempStruct, dataTx, QByteArray(stend_header));
        emit readyWriteDataToInterface(dataTx);
        command.push_back(StendProperty::start_test);
        break;

    case StendProperty::reboot: /* перезагрузка стенда */
        break;
    case StendProperty::write_serial_dut:
        break;
    }
}

bool StendApi::headerIsValid(QByteArray & dataRx) {
    if(dataRx.isEmpty()) {
        return false;
    }
    /* check header */
    if(!dataRx.contains(QByteArray(stend_header))) {
        return false;
    }
    StendProperty::sInputTcpTempStruct* inputTempStructInfo = nullptr;
    if(dataRx.length() >= sizeof(StendProperty::sInputTcpTempStruct) + sizeof(stend_header)) {
        inputTempStructInfo = (StendProperty::sInputTcpTempStruct*)((uint8_t*) dataRx.data() + strlen(stend_header));
        if(inputTempStructInfo->version == stend_version) {
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
        StendProperty::sInputTcpTempStruct* pinputTemp = (StendProperty::sInputTcpTempStruct*)((uint8_t*) dataRx.data() + strlen(stend_header));
        switch(cmd) {
        /* запрашиваем базовый пакет */
        case StendProperty::get_basic_param: {

            copyInputData(dutInfoStruct, pinputTemp);

            /* update fields */
            QJsonObject jsonObj = convertDataToJson(dutInfoStruct);
            emit updateRealTimeData(QJsonDocument(jsonObj).toJson());

            jsonObj = evaluateTestStatus(dutInfoStruct.test);
            if(testIsFinished(dutInfoStruct.test)) {
                if(!mTestIsFinished){
                    mTestIsFinished = true;
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
            command.push_back(StendProperty::dut_power_up);
            break;

        case StendProperty::dut_power_up: /* питания дут */
            res = true;
            command.push_back(StendProperty::start_test);
            break;

        case StendProperty::write_serial_dut: break;
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
    if(dataBase->openConnection(&error)) {
        if(connect_state == StendProperty::connected) {
            command.push_back(StendProperty::reset_test);
            programming->startProgramm();
            mTestIsFinished = false;
        }
    } else { /* not working */
        QJsonObject jsonObj;
        jsonObj.insert("testResult", false);
        jsonObj.insert("database", "database");
        jsonObj.insert("message", error);
        emit testError(QJsonDocument(jsonObj).toJson());
    }
}

QJsonObject StendApi::convertDataToJson(sDutBaseStruct & dutStruct) {
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

    /* cnt collect*/
    static QJsonArray powerVoltageCollect;
    static QJsonArray powerCurrentCollect;
    static QJsonArray cntCollect;
    powerVoltageCollect.append((int)dutStruct.power_input);
    if(powerVoltageCollect.count() > maxCollectArrayData) {
        powerVoltageCollect.pop_front();
    }
    powerCurrentCollect.append((int)dutStruct.power_current);
    if(powerCurrentCollect.count() > maxCollectArrayData) {
        powerCurrentCollect.pop_front();
    }
    cntCollect.append((int)dutStruct.cnt);
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
    QString mcuSn;
    for(uint8_t i=0; i<sizeof(dutStruct.mcu_serial_number); i++) {
        mcuSn += QString::number(dutStruct.mcu_serial_number[i], 16);
    }
    jsonObj.insert("mcu_serial_number", mcuSn);
    jsonObj.insert("bootloader_version", dutStruct.bootloader_version);
    return jsonObj;
}

void StendApi::copyInputData(sDutBaseStruct & dutStruct, const StendProperty::sInputTcpTempStruct* pinputTemp) {
    S_curves_data curves_data;
    dutStruct.addr = pinputTemp->info.addr;
    dutStruct.power_3_3 = pinputTemp->info.power_3_3;
    dutStruct.power_input = pinputTemp->info.power_input;
    dutStruct.power_current = pinputTemp->info.power_current / 10;
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

    if((pinputTemp->info.serial_number[0]>'9')||(pinputTemp->info.serial_number[0]<'0')) {
        strcpy(dutStruct.serial_number, QString(tr("ТЕСТОВЫЙ_НОМЕР")).toStdString().data());
    } else {
        memcpy(&dutStruct.serial_number, &pinputTemp->info.serial_number, sizeof(pinputTemp->info.serial_number));
    }
    memcpy(&dutStruct.mcu_serial_number, &pinputTemp->info.mcu_serial_number, sizeof(pinputTemp->info.mcu_serial_number));
    memcpy(&dutStruct.program_version, &pinputTemp->info.program_version, sizeof(pinputTemp->info.program_version));
    dutStruct.dateTime = pinputTemp->info.dateTime;
    dutStruct.test = pinputTemp->info.test;
}

QString StendApi::convertTestStateToString(eTestState state) {
    switch (state) {
    case idle: return "idle";
    case process: return "process";
    case test_err: return "fail";
    case test_ok: return "finished";
    }
}

bool StendApi::testIsFinished(sDutTestStruct & tests) {
    if((tests.state == test_ok) || (tests.state == test_err)) {
        return true;
    }
    return false;
}

QJsonObject StendApi::evaluateTestStatus(sDutTestStruct & tests) {
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
