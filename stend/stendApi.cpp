#include "stend/stendApi.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtDebug>

#include <QRandomGenerator>

StendApi::StendApi(QObject *parent) : QObject(parent) {
    command.push_back(StendProperty::get_basic_param);
    interval_read_info_counter = 0;

    connect_state = StendProperty::disconnected;

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
    outputTcpTempStruct.capValues = *capValues;

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
        StendProperty::sInputTcpTempStruct* inputTempStructInfo = (StendProperty::sInputTcpTempStruct*)((uint8_t*) dataRx.data() + strlen(stend_header));
        switch(cmd) {
        /* запрашиваем базовый пакет */
        case StendProperty::get_basic_param: {
            S_curves_data curves_data;
            dutInfoStruct.addr = inputTempStructInfo->info.addr;
            dutInfoStruct.power_3_3 = inputTempStructInfo->info.power_3_3;
            dutInfoStruct.power_input = inputTempStructInfo->info.power_input;
            dutInfoStruct.power_current = inputTempStructInfo->info.power_current / 10;
            dutInfoStruct.power_state = inputTempStructInfo->info.power_state;
            curves_data.power_3_3 = dutInfoStruct.power_3_3;
            curves_data.power_current = dutInfoStruct.power_current;
            curves_data.power_input = dutInfoStruct.power_input;
            curves_data.cnt = inputTempStructInfo->info.cnt;

            curves_data_vector.push_back(curves_data);
            if(curves_data_vector.size() > 500) {
                curves_data_vector.clear();
            }
            dutInfoStruct.cnt = inputTempStructInfo->info.cnt;
            if((inputTempStructInfo->info.serial_number[0]>'9')||(inputTempStructInfo->info.serial_number[0]<'0')) {
                strcpy(dutInfoStruct.serial_number, QString(tr("ТЕСТОВЫЙ_НОМЕР")).toStdString().data());
            } else {
                memcpy(&dutInfoStruct.serial_number, &inputTempStructInfo->info.serial_number, sizeof(inputTempStructInfo->info.serial_number));
            }
            memcpy(&dutInfoStruct.mcu_serial_number, &inputTempStructInfo->info.mcu_serial_number, sizeof(inputTempStructInfo->info.mcu_serial_number));
            memcpy(&dutInfoStruct.program_version, &inputTempStructInfo->info.program_version, sizeof(inputTempStructInfo->info.program_version));
            dutInfoStruct.dateTime = inputTempStructInfo->info.dateTime;
            dutInfoStruct.test = inputTempStructInfo->info.test;
            res = true;

            /* update fields */
            QJsonObject jsonObj;
            jsonObj.insert("cnt", (int)dutInfoStruct.cnt);
            jsonObj.insert("addr", dutInfoStruct.addr);
            jsonObj.insert("test.state", dutInfoStruct.test.state);
            jsonObj.insert("test.freq_state", dutInfoStruct.test.freq_state);;
            jsonObj.insert("test.rs232_state", dutInfoStruct.test.rs232_state);
            jsonObj.insert("test.rs485_state", dutInfoStruct.test.rs485_state);
            jsonObj.insert("test.cap_test_result[0]", (int)dutInfoStruct.test.cap_test_result[0]);
            jsonObj.insert("test.cap_test_result[1]", (int)dutInfoStruct.test.cap_test_result[1]);
            jsonObj.insert("test.cap_test_result[2]", (int)dutInfoStruct.test.cap_test_result[2]);
            jsonObj.insert("test.rs232_send_packet", dutInfoStruct.test.rs232_send_packet);
            jsonObj.insert("test.rs232_receive_packet", dutInfoStruct.test.rs232_receive_packet);
            jsonObj.insert("test.rs232_state", dutInfoStruct.test.rs232_state);
            jsonObj.insert("test.rs485_send_packet", dutInfoStruct.test.rs485_send_packet);
            jsonObj.insert("test.rs485_receive_packet", dutInfoStruct.test.rs485_receive_packet);
            jsonObj.insert("test.rs485_state", dutInfoStruct.test.rs485_state);
            jsonObj.insert("power_3_3", dutInfoStruct.power_3_3);
            jsonObj.insert("power_input", dutInfoStruct.power_input);
            jsonObj.insert("programming.serial_num", dutInfoStruct.programming.serial_num);
            jsonObj.insert("power_current", dutInfoStruct.power_current);

            /* cnt collect*/
            static QJsonArray powerVoltageCollect;
            static QJsonArray powerCurrentCollect;
            static QJsonArray cntCollect;
            powerVoltageCollect.append((int)dutInfoStruct.power_input);
            if(powerVoltageCollect.count() > maxCollectArrayData) {
                powerVoltageCollect.pop_front();
            }
            powerCurrentCollect.append((int)dutInfoStruct.power_current);
            if(powerCurrentCollect.count() > maxCollectArrayData) {
                powerCurrentCollect.pop_front();
            }
            cntCollect.append((int)dutInfoStruct.cnt);
            if(cntCollect.count() > maxCollectArrayData) {
                cntCollect.pop_front();
            }
            jsonObj.insert("powerCollect", powerVoltageCollect);
            jsonObj.insert("currentCollect", powerCurrentCollect);
            jsonObj.insert("cntCollect", cntCollect);

            QString factorySn;
            for(uint8_t i=0; i<sizeof(dutInfoStruct.serial_number); i++) {
                factorySn += QString::number(dutInfoStruct.serial_number[i], 16);
            }
            jsonObj.insert("serial_number", QString::fromUtf8(dutInfoStruct.serial_number, sizeof(dutInfoStruct.serial_number)));
            jsonObj.insert("program_version", dutInfoStruct.program_version);
            QString mcuSn;
            for(uint8_t i=0; i<sizeof(dutInfoStruct.mcu_serial_number); i++) {
                mcuSn += QString::number(dutInfoStruct.mcu_serial_number[i], 16);
            }
            jsonObj.insert("mcu_serial_number", mcuSn);
            jsonObj.insert("bootloader_version", dutInfoStruct.bootloader_version);

            emit updateRealTimeData(QJsonDocument(jsonObj).toJson());

            if((dutInfoStruct.test.rs232_state == test_ok)
                    && (dutInfoStruct.test.rs485_state == test_ok)
                    && (dutInfoStruct.test.freq_state == test_ok)) {
                jsonObj.insert("testResult", "ok");
                emit testFinihed(QJsonDocument(jsonObj).toJson());
            } else {
                jsonObj.insert("testResult", "fail");
                emit testFinihed(QJsonDocument(jsonObj).toJson());
            }
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
            emit powerDutDisabled();
            break;

        case StendProperty::dut_power_up: /* питания дут */
            res = true;
            emit powerDutEnabed();
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
