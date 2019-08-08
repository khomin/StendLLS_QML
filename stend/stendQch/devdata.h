#ifndef DEVDATA_H
#define DEVDATA_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include "globals.h"
#include "llscomparevalue.h"

class DevData : public QObject
{
    Q_OBJECT
public:
    DevData(bool enabled,
            QString *ip_addr, int port_485, int port_232,
            uint32_t *comp_value_min_correct_full,
            uint32_t *comp_value_min_correct_empty,
            float *tollerance);

public:
    void testFunc(int id);

    llsCompareValue *compare_lls;

    bool test_is_finished;

#pragma pack(1)
    typedef struct {
        uint32_t cntEmpty;
        uint32_t cntFull;
        uint8_t netAddress;
        uint8_t periodicSendType; // T_DataSend
        uint8_t periodicSendTime;
        uint16_t minLevel;
        uint16_t maxLevel;
        uint8_t outputValue;
        uint8_t filterType;
        uint8_t avarageLength;
        uint8_t medianLength;
        float q;
        float r;
        uint8_t thermoCompensationType;
        float k1;
        float k2;
        uint8_t interpolationType;
        uint8_t rs232Speed;
        uint8_t rs485Speed;
        uint8_t masterMode; // T_MasterMode
        uint8_t slaveCount;
        uint8_t slaveAddr[4];
        uint8_t waterMode;
        uint8_t reserved;
    }T_settings;
#pragma pack()

private:

    typedef enum {
        lls_read_lvl_once =         0x06,
        lls_send_data_enable =      0x07,
        lls_set_send_time =         0x13,
        lls_send_data_default =     0x17,
        lls_read_cnt =              0xf7,
        lls_read_lvl_all =          0x46,
        lls_read_settings =         0x47,
        lls_write_settings =        0x48,
        lls_read_cal_table =        0x26,
        lls_write_cal_table =       0x27,
        lls_calibrate_min =         0x08,
        lls_calibrate_max =         0x09,
        lls_read_errors =           0x30,
        lls_set_serial_number =     0x33,
        lls_read_serial_number =    0x32,
        lls_set_personal =          0x43,
        lls_read_personal =         0x42,
        lls_set_new_password =      0x16,
        lls_check_address_and_pass= 0x74,
        lls_run_bootloader =        0x67,
        lls_download_application = 	0x68,
        lls_upload_application	=	0x69,
        lls_run_application		=	0x6A,
        lls_download_settings	=	0x6B,
        lls_upload_settings		=	0x6C,
        lls_clear_settings		=	0x6D,
        lls_flash_unprotect		=	0x6F
    }E_type_operation;

#pragma pack(1)

    //--- типы интерфейсов для данных (232/485)
    typedef enum {
        interface_232,
        interface_485,
    }E_typeInterface;

    //--- данные посылаемые в rs232/485
    typedef struct {
        E_typeInterface uart_type;
        uint8_t data[100];
        int delay_reply_value_ms;
        uint8_t len;
    }S_dev_struct;

    //--- для отправки на железку
    typedef struct {
        uint8_t stend_version;
        S_dev_struct dev;
    }S_outTcpDevTemp;

    //--- для приема с железки
    typedef struct {
        uint8_t version;
        S_dev_struct dev;
    }S_inTcpDevTemp;
#pragma pack()

    QVector <S_dev_struct> command;

    QTcpSocket *rs485_socket;
    QTcpSocket *rs232_socket;

    uint32_t *dev_value_empty;
    uint32_t *dev_value_full;
    float *tolerence;

    QString dest_ip_addr;
    int dest_port_485;
    int dest_port_232;
    bool connect_state;
    bool rs232_is_active = false;
    bool rs485_is_active = false;

    const uint8_t dev_id = 1;
    QString cpu_id;
    uint32_t lls_level;
    uint8_t sn[12];
    char firmware_version[8];
    char type_lls[10];
    uint32_t max_level;
    T_settings settings;
    //---
    typedef enum {
        type_lls_tmk24 =  1,
        type_lls_tmk2u1 = 2,
        type_lls_tmk4ux = 13,
    }E_type_lls;

public slots:
    void queueFunc();
    bool connectTcp();
    bool closeTcp();
    void tcpError(QAbstractSocket::SocketError);
    void readySocket232Read();
    void readySocket485Read();
    QString getFirmware();
    QString getTypeDev();
    uint32_t getMaxLevel();
    QString getMcuNumber();
    uint32_t getCurrnetLevel();
    void getLevelValues(uint32_t *pfull, uint32_t *pempty);
    bool getDevState();
    QString getSerialNum();
    bool setSerialNumber(QString serial_number);
    bool serialNumIsEmpty();
    void clearLevelTrig();
private slots:
    bool sendSocket(QTcpSocket *psocket, QByteArray *dataTx);
    bool sendCommand(S_dev_struct cmd);
    void packetInsertHeader(S_outTcpDevTemp p_out,
                            QByteArray *p_array,
                            QString *pack_header);
    //--- создаем запросы
    bool makeDataBaseRawBuf(uint8_t *data, uint8_t *len);
    bool makeDataCntRawBuf(uint8_t *data, uint8_t *len);
    bool makeDataSettingsRawBuf(uint8_t *data, uint8_t *len);
    bool makeDataWriteSerialNumberRawBuf(uint8_t *data, char *p_sn, uint8_t *len);
    bool makeDataSerNumRawBuf(uint8_t *data, uint8_t *len);
    //--- разбираем ответы
    bool parsceRxData(QByteArray *dataRx);

signals:
    void sendError();
    void devReadLvl(int8_t temp, uint16_t fuel_level, uint16_t freq);
    void devReadCnt(uint32_t cnt);
    void devReadSettings(char *firm, char *type_dev, DevData::T_settings psettings);
    void devReadSn(uint8_t *serial_number, QString cpu_sn);
    void devState232_485(bool state232, bool state485);
    void devEmptyValueChangedStatus(uint32_t value, bool state);
    void devFullValueChangedStatus(uint32_t value, bool state);
    void devSetSerialNumber();
    void connectLost();

};

#endif // DEVDATA_H
