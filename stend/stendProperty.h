#ifndef StendProperty_H
#define StendProperty_H

#include <QObject>
#include "globals.h"

class StendProperty : public QObject {
    Q_OBJECT
public:
    explicit StendProperty();

    /* stend commands */
    typedef enum {
        get_basic_param     = 0x01,
        //disconnect          = 0x02, //-- not used
        reboot              = 0x03,
        write_serial_dut 	= 0x04,
        dut_power_down		= 0x05,
        dut_power_up        = 0x06,
        start_test			= 0x07,
        reset_test			= 0x08
    }eTypeCommand;

    /* states */
    typedef enum {
        disconnected,
        connected
    }eConnetState;

    /* data */
    typedef struct {
        quint16 serial_num;
        quint8 mac[6];
        qint32 ip;
        struct tm dateReply;
        quint16 version;
    }sSettings;

    typedef struct {
        quint32 min[3];
        quint32 max[3];
    }sCapTestValues;

    /* data input */
#pragma pack(1)
    typedef struct {
        quint8 version;
        sDutBaseStruct info;
    }sInputTcpTempStruct;
#pragma pack()

    /* data output */
#pragma pack(1)
    typedef struct {
        qint8 stend_version;
        eTypeCommand command;
        char writed_serial_num[12];
        sCapTestValues capValues;
    }sOutputTcpTempStruct;
#pragma pack(0)
};

#endif // StendProperty_H
