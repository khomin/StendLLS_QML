#ifndef GLOBALS_H
#define GLOBALS_H

#include <QObject>
#include <stdint.h>
#include "qdatetime.h"

typedef struct sIp_list {
    quint8 ip_addr[4];
}sIp_list;

typedef struct {
    float power_input;
    float power_3_3;
    float power_current;
    uint32_t cnt;
}S_curves_data;

typedef struct sConnectSettings {
    sIp_list ip; // ip железки
    sIp_list net_interface_ip; // ip сетевого интерфейса
    quint8 mac[6];
    QDateTime dateReply;
    quint16 serial_num;
}sConnectSettings;

enum eTestState {
    idle,
    process,
    test_err,
    test_ok
};

#pragma pack(1)
typedef struct {
    uint32_t HourFormat;
    uint32_t AsynchPrediv;
    uint32_t SynchPrediv;
    uint32_t OutPut;
    uint32_t OutPutPolarity;
    uint32_t OutPutType;
}RTC_InitTypeDef;
#pragma pack()

#pragma pack(1)
typedef struct {
    quint8 Hours;
    quint8 Minutes;
    quint8 Seconds;
    quint8 TimeFormat;
    quint32 SubSeconds;
    quint32 SecondFraction;
    quint32 DayLightSaving;
    quint32 StoreOperation;
}RTC_TimeTypeDef;
#pragma pack()

#pragma pack(1)
typedef struct {
    quint8 WeekDay;
    quint8 Month;
    quint8 Date;
    quint8 Year;
}RTC_DateTypeDef;
#pragma pack()

#pragma pack(1)
typedef struct {
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
}sDateTime;
#pragma pack(0)

#pragma pack(1)
typedef struct {
    quint16 typeAck;
    quint32 code;
}sPacketAckStruct;
#pragma pack(0)

#pragma pack(1)
typedef struct{
    bool write_num;
    char serial_num[12];
}sDutProgramming;
#pragma pack(0)

#pragma pack(1)
typedef struct {
    eTestState state;
    eTestState freq_state;
    eTestState rs232_state;
    eTestState rs485_state;
    eTestState oneWire_state;
    uint32_t cap_test_result[3];
    quint16 rs232_send_packet;
    quint16 rs232_receive_packet;
    quint16 rs485_send_packet;
    quint16 rs485_receive_packet;
    quint16 oneWire_value;
    sDateTime dateTimeEndTest;
}sDutTestStruct;
#pragma pack(0)

#pragma pack(1)
typedef struct {
    int8_t temperature;
    uint16_t level;
    uint16_t freq;
}sLlsCurrentData;
#pragma pack(0)

#pragma pack(1)
typedef struct {
    quint8 addr;
    uint32_t cnt;
    float power_input;
    float power_3_3;
    float power_current;
    char serial_number[12];
    char program_version[8];
    char bootloader_version[8];
    char mcu_serial_number[12];
    sDutTestStruct test;
    sDutProgramming programming;
    bool power_state;
    sLlsCurrentData currentDataRs232;
    sLlsCurrentData currentDataRs485;
    sDateTime dateTime;
}sDutBaseStruct;
#pragma pack(0)

static constexpr int network_port = 45454;
static constexpr char stend_header[] = "stend_locus_";
static constexpr int stend_version = 2;
static constexpr char broadcast_call[] = "Who lives on the ocean floor?";
static constexpr char broadcast_call_reply[] = "SpongeBob Squarepants!";

#endif // GLOBALS_H

