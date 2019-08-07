#include "devdata.h"
#include "QString"
#include "crc.h"
#include "globals.h"
#include <QtDebug>
#include <QTimer>
#include "llscomparevalue.h"

const QString stend_dev_header_tcp = "stend_dev_data_";

DevData::DevData(bool enabled,
                 QString *ip_addr, int port_485, int port_232,
                 uint32_t *comp_value_min_correct_full,
                 uint32_t *comp_value_min_correct_empty,
                 float *tollerance) {

    dest_ip_addr = *ip_addr;
    dest_port_232 = port_232;
    dest_port_485 = port_485;

    this->rs485_socket = new QTcpSocket();
    this->rs232_socket = new QTcpSocket();
    this->dev_value_empty = comp_value_min_correct_empty;
    this->dev_value_full = comp_value_min_correct_full;
    this->tolerence = tollerance;
    connect(rs232_socket, SIGNAL(readyRead()), this, SLOT(readySocket232Read()));
    connect(rs485_socket, SIGNAL(readyRead()), this, SLOT(readySocket485Read()));
    connect(rs232_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
    connect(rs485_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));

    compare_lls = new llsCompareValue(*dev_value_empty, *dev_value_full, *tollerance);

    if(enabled) {
        QTimer *timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(queueFunc()));
        timer->start(100);
    }
    this->connect_state = false;
    this->test_is_finished  = false;
}

bool DevData::connectTcp() {
    bool res = false;
    rs232_socket->connectToHost(QHostAddress(dest_ip_addr), dest_port_232);
    rs485_socket->connectToHost(QHostAddress(dest_ip_addr), dest_port_485);
    qDebug() << ("While tcp socket connecting\r");
    rs232_socket->waitForConnected(3000);
    rs485_socket->waitForConnected(3000);
    //
    QAbstractSocket::SocketState rs232_conn_state = rs232_socket->state();
    QAbstractSocket::SocketState rs485_conn_state = rs232_socket->state();
    if(((rs232_conn_state == QAbstractSocket::ConnectedState)
        || (rs232_conn_state == QAbstractSocket::ConnectingState))
            && ((rs232_conn_state == QAbstractSocket::ConnectedState)
                || (rs232_conn_state == QAbstractSocket::ConnectingState))
            )
    {
        connect_state = connected;
        res = true;
    } else {
        emit connectLost();
    }
    return res;
}

void DevData::tcpError(QAbstractSocket::SocketError error) {
    connect_state = connected;
    emit connectLost();
}

bool DevData::closeTcp() {
    connect_state = disconnected;
    return true;
}

void DevData::readySocket232Read() {
    QByteArray dataRx;
    uint32_t size = rs232_socket->bytesAvailable();
    if(size < sizeof(S_inTcpDevTemp)) {
        qDebug() << "Avalible < sizeof(S_inTcpDevTemp)\n";
        return;
    }
    dataRx = rs232_socket->readAll();
    if(!parsceRxData(&dataRx)) {
        rs232_socket->flush();
    }
}

void DevData::readySocket485Read() {
    QByteArray dataRx;
    uint32_t size = rs485_socket->bytesAvailable();
    if(size < sizeof(S_inTcpDevTemp)) {
        qDebug() << "Avalible < sizeof(S_inTcpDevTemp)\n";
        return;
    }
    dataRx = rs485_socket->readAll();
    if(!parsceRxData(&dataRx)) {
        rs485_socket->flush();
    }
}

void DevData::queueFunc() {
    if(connect_state == connected) {
        //--- если ничео нет, читаем базу
        if(command.isEmpty()) {
            S_dev_struct t_command;
            //--- читаем данные по rs232
            t_command.uart_type = interface_232;
            if(makeDataBaseRawBuf(t_command.data, &t_command.len)) {
                t_command.delay_reply_value_ms = 50; //--- задержка перед ответом
                command.push_back(t_command);
            }
            t_command.uart_type = interface_232;
            if(makeDataCntRawBuf(t_command.data, &t_command.len)) {
                t_command.delay_reply_value_ms = 50; //--- задержка перед ответом
                command.push_back(t_command);
            }
            t_command.uart_type = interface_232;
            if(makeDataSettingsRawBuf(t_command.data, &t_command.len)) {
                t_command.delay_reply_value_ms = 50; //--- задержка перед ответом
                command.push_back(t_command);
            }
            t_command.uart_type = interface_232;
            if(makeDataSerNumRawBuf(t_command.data, &t_command.len)) {
                t_command.delay_reply_value_ms = 50; //--- задержка перед ответом
                command.push_back(t_command);
            }
            //--- читаем данные по rs485
            t_command.uart_type = interface_485;
            if(makeDataBaseRawBuf(t_command.data, &t_command.len)) {
                t_command.delay_reply_value_ms = 50; //--- задержка перед ответом
                command.push_back(t_command);
            }
        }
        if(sendCommand(command.first())) {
            connect_state = connected;
        } else { //--- высылаем ошибку
            connect_state = disconnected;
            closeTcp();
            emit sendError();
            emit connectLost();
        }
    }
}

//-- отправка запороса, получение ответа
bool DevData::sendCommand(S_dev_struct cmd) {
    bool res = false;
    QByteArray data_tx;
    //--- временая структура для перадачи
    S_outTcpDevTemp out_tcp_struct;
    out_tcp_struct.stend_version = stend_version;;
    out_tcp_struct.dev = cmd;
    //---
    packetInsertHeader(out_tcp_struct, &data_tx, (QString*)&stend_dev_header_tcp);
    //---
    switch(cmd.uart_type) {
    case interface_232:
        res = sendSocket(rs232_socket, &data_tx);
        break;
    case interface_485:
        res = sendSocket(rs485_socket, &data_tx);
        break;
    }
    return res;
}

//--- упаковывает raw пакет перед отпаравкой
void DevData::packetInsertHeader(S_outTcpDevTemp p_out,
                                 QByteArray *p_array,
                                 QString *pack_header)
{
    p_array->setRawData((char*)&p_out, sizeof(p_out));
    /* вставляем заголовок */
    p_array->push_front(pack_header->toUtf8());
}

bool DevData::sendSocket(QTcpSocket *psocket, QByteArray *dataTx) {
    psocket->write(*dataTx);
    if(!command.isEmpty()) {
        command.removeFirst();
    }
    return true;
}

bool DevData::parsceRxData(QByteArray *dataRx) {
    bool res = false;
    QByteArray dev_data;
    int8_t temp = 0;
    uint16_t fuel_level = 0;
    uint16_t freq = 0;
    uint32_t cnt = 0;

    /* проверка заголовка */
    QString header = dataRx[0];
    if((header[0] != stend_dev_header_tcp[0])
            || (header[3] != stend_dev_header_tcp[3])) {
        return false;
    }
    S_inTcpDevTemp *inp_struct;
    inp_struct = (S_inTcpDevTemp*)(dataRx->data() + stend_dev_header_tcp.length());
    dev_data = QByteArray::fromRawData((char*)inp_struct->dev.data, sizeof(inp_struct->dev.data));

    bool *is_serial_active = NULL;
    switch(inp_struct->dev.uart_type) {
    case interface_232:
        is_serial_active = &rs232_is_active;
        break;
    case interface_485:
        is_serial_active = &rs485_is_active;
        break;
    default :
        qDebug() << "Error packet!\n";
        return false;
    }

    switch(dev_data.at(2) & 0xFF)
    {
    case lls_read_lvl_once:
    {
        temp = (int8_t)(0xFF & dev_data.at(3));
        fuel_level = 0xFF & dev_data.at(5);
        fuel_level = fuel_level << 8;
        fuel_level |= 0xFF & dev_data.at(4);
        freq = 0xFF & dev_data.at(7);
        freq = freq << 8;
        freq |= 0xFF & dev_data.at(6);
        *is_serial_active = true;
        emit devReadLvl(temp, fuel_level, freq);
        lls_level = fuel_level;

        if(getDevState()) {
            if((freq >= *dev_value_empty) && (freq < 0xFFFA)) {
                *dev_value_empty = freq;
            }
            if((freq <= *dev_value_full) || (*dev_value_full == 0)) {
                *dev_value_full = freq;
            }
            if(compare_lls->valuesEmptyIsNormal(*dev_value_empty)) {
                emit devEmptyValueChangedStatus(*dev_value_empty, true);
            } else {
                emit devEmptyValueChangedStatus(*dev_value_empty, false);
            }
            if(compare_lls->valuesFullIsNormal(*dev_value_full)) {
                emit devFullValueChangedStatus(*dev_value_full, true);
            } else {
                emit devFullValueChangedStatus(*dev_value_full, false);
            }
        }

        res = true;
    }
        break;
    case lls_send_data_enable: break;
    case lls_set_send_time: break;
    case lls_send_data_default: break;
    case lls_read_cnt:
    {
        if(dev_data.size() >= 8) {
            cnt = 0xFF & dev_data.at(7);
            cnt = cnt << 8;
            cnt |= 0xFF & dev_data.at(6);
            cnt = cnt << 8;
            cnt |= 0xFF & dev_data.at(5);
            cnt = cnt << 8;
            cnt |= 0xFF & dev_data.at(4);
            *is_serial_active = true;
            emit devReadCnt(cnt);
            res = true;
        }
    }
        break;
    case lls_read_lvl_all: { res = true; }
        break;

    case lls_read_settings:
        if(dev_data.size() > 22) {
            char *pbuf = (dev_data.data() + 4);
            memcpy((char*)sn, pbuf, sizeof(sn));
            pbuf = (dev_data.data() + 4 + sizeof(sn));
            memcpy((char*)firmware_version, pbuf, sizeof(firmware_version));
            switch(dev_data.at(3)) {
            case type_lls_tmk24:
                strcpy((char*)type_lls, "ТМК.24");
                break;
            case type_lls_tmk4ux:
                strcpy((char*)type_lls, "ТМК.4UX");
                break;
            case type_lls_tmk2u1:
                strcpy((char*)type_lls, "ТМК.2И1");
            default:
                strcpy((char*)type_lls, "Не определен");
                break;
            }
            uint8_t traw_u8[256] = {0};
            for(uint8_t i=0; i<dev_data.size(); i++) {
                traw_u8[i] = dev_data.at(i);
            }
            T_settings *psettings = (T_settings*)&traw_u8[34];
            settings = *psettings;
            *is_serial_active = true;
            emit devReadSettings((char*)firmware_version, (char*)type_lls, settings);
            res = true;
        }
        break;

    case lls_write_settings:
    case lls_read_cal_table:
    case lls_write_cal_table:
    case lls_calibrate_min:
    case lls_calibrate_max:
    case lls_read_errors:
        break;
    case lls_set_serial_number:
        emit devSetSerialNumber();
        break;
    case lls_read_serial_number:
    {
        if(dev_data.size() > 22) {
            char tcpu_id[12] = {0};
            char *pbuf = (dev_data.data() + 3);
            memcpy((char*)sn, pbuf, sizeof(sn));
            //
            pbuf = (dev_data.data() + 3 + sizeof(sn) + sizeof(firmware_version));
            //
            memcpy((char*)tcpu_id, pbuf, sizeof(tcpu_id));
            //
            *is_serial_active = true;
            cpu_id.clear();
            for(uint8_t count=0; count<sizeof(tcpu_id); count++) {
                QString hex = QString::number(tcpu_id[count], 16);
                cpu_id += hex;
                if(hex.length() == 1) {
                    cpu_id += "0";
                }

            }
            emit devReadSn(sn, cpu_id);
            res = true;
        }
    }
        break;

    case lls_set_personal: break;
    case lls_read_personal: break;
    case lls_set_new_password:
        break;

    case lls_check_address_and_pass:
        break;

    case lls_run_bootloader: break;
    default :
        *is_serial_active = false;
        break;
    }
    emit devState232_485(rs232_is_active, rs485_is_active);
    return res;
}

//--- создаем запросы
bool DevData::makeDataBaseRawBuf(uint8_t *data, uint8_t *len) {
    uint8_t i=0;
    Crc *crc = new Crc();
    data[i++] = 0x31;
    data[i++] = dev_id;
    data[i++] = lls_read_lvl_once;
    uint8_t t_crc = crc->crc8_dallas((char*)data, i);
    data[i++] = t_crc;
    *len = i;
    return true;
}

bool DevData::makeDataCntRawBuf(uint8_t *data, uint8_t *len) {
    uint8_t i=0;
    Crc *crc = new Crc();
    data[i++] = 0x31;
    data[i++] = dev_id;
    data[i++] = lls_read_cnt;
    uint8_t t_crc = crc->crc8_dallas((char*)data, i);
    data[i++] = t_crc;
    *len = i;
    return true;
}

bool DevData::makeDataSettingsRawBuf(uint8_t *data, uint8_t *len) {
    uint8_t i=0;
    Crc *crc = new Crc();
    data[i++] = 0x31;
    data[i++] = dev_id;
    data[i++] = lls_read_settings;
    uint8_t t_crc = crc->crc8_dallas((char*)data, i);
    data[i++] = t_crc;
    *len = i;
    return true;
}

bool DevData::makeDataWriteSerialNumberRawBuf(uint8_t *data, char *p_sn, uint8_t *len) {
    uint8_t i=0;
    Crc *crc = new Crc();
    data[i++] = 0x31;
    data[i++] = dev_id;
    data[i++] = lls_set_serial_number;
    for(uint8_t count=0; count<sizeof(sn); count++) {
        data[i++] = p_sn[count];
    }
    uint8_t t_crc = crc->crc8_dallas((char*)data, i);
    data[i++] = t_crc;
    *len = i;
    return true;
}

bool DevData::makeDataSerNumRawBuf(uint8_t *data, uint8_t *len) {
    uint8_t i=0;
    Crc *crc = new Crc();
    data[i++] = 0x31;
    data[i++] = dev_id;
    data[i++] = lls_read_serial_number;
    uint8_t t_crc = crc->crc8_dallas((char*)data, i);
    data[i++] = t_crc;
    *len = i;
    return true;
}

QString DevData::getFirmware() {
    return QString::fromUtf8((char*)firmware_version, strlen(firmware_version));
}

QString DevData::getTypeDev() {
    return QString::fromUtf8(type_lls);
}

uint32_t DevData::getMaxLevel() {
    return max_level;
}

bool DevData::setSerialNumber(QString serial_number) {
    bool res = false;
    if(connect_state == connected) {
        char t_sn[12] = {0};
        strcpy(t_sn, serial_number.toUtf8());
        S_dev_struct t_command;
        t_command.uart_type = interface_232;
        if(makeDataWriteSerialNumberRawBuf(t_command.data, t_sn, &t_command.len)) {
            t_command.delay_reply_value_ms = 150; //--- задержка перед ответом
            command.push_back(t_command);
            res = true;
        }
    }
    return res;
}

bool DevData::serialNumIsEmpty() {
    bool res = false;
    if((sn[0] == 0xFF) && (sn[0] == 0xFF)) {
        res = true;
    }
    return res;
}

QString DevData::getSerialNum() {
    QString res;
    if(!serialNumIsEmpty()) {
        res = QString::fromLocal8Bit((char*)sn);
    }
    return res;
}

uint32_t DevData::getCurrnetLevel() {
    return lls_level;
}

bool DevData::getDevState() {
    bool res = false;
    if((rs232_is_active) && (rs485_is_active)) {
        res = true;
    }
    return res;
}

void DevData::getLevelValues(uint32_t *pfull, uint32_t *pempty) {
    *pfull = *dev_value_full;
    *pempty = *dev_value_empty;
}

void DevData::clearLevelTrig() {
    *dev_value_empty = 0;
    *dev_value_full = 0;
    test_is_finished = false;
}

QString DevData::getMcuNumber() {
    QString ret;
    if(cpu_id.length()) {
        ret = cpu_id;
    }
    return ret;
}
