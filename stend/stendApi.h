#ifndef STENDAPI_H
#define STENDAPI_H

#include <QObject>
#include <QNetworkSession>
#include "globals.h"
#include "stendProperty.h"
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>

class StendApi : public QObject
{
    Q_OBJECT
public:
    explicit StendApi(QObject *parent = nullptr);
    ~StendApi();

public slots:
    /* send command to execute */
    void sendCommand(StendProperty::eTypeCommand cmd);

    /* status */
    void setStatusConnected();
    void setStatusDisconnected();

    /* insert data from tcp */
    void insertDataFromInterface(QByteArray data);
    /* insert error from tcp */
    void insertErrorFromInterface(QString data);

signals:
    /* send data through tcp */
    void readyWriteDataToInterface(QByteArray data);
    void stendNotReply(QString text);
    /* test or update state */
    void testError(QString json);
    void testFinihed(QString json);
    void updateRealTimeData(QString json);
    void powerDutDisabled();
    void powerDutEnabed();

private slots:
    bool parsinReply(StendProperty::eTypeCommand cmd, QByteArray & dataRx);
    bool headerIsValid(QByteArray & dataRx);
    void packetInsert(StendProperty::sOutputTcpTempStruct pOut, QByteArray & pArray, QByteArray packHeader);

private:
    sIp_list ip;
    int interval_read_info_counter;
    QTimer *handlerStendTimer;
    QTimer *timeoutCommandTimer;

    int transferTimeoutId;
    StendProperty::eConnetState connect_state;
    sDutBaseStruct dutInfoStruct;
    /* указатель на структуру из настроек тестирования */
    /* настройки теста частоты отправляются в каждом пакете */
    StendProperty::sCapTestValues *capValues;
    float curLlsMinValue;
    float curLlsMaxValue;

    QVector <StendProperty::eTypeCommand> command;
    QVector <S_curves_data>curves_data_vector;

    static constexpr int maxCollectArrayData = 30;
};

#endif // STENDAPI_H
