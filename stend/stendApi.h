#ifndef STENDAPI_H
#define STENDAPI_H

#include <QObject>
#include <QNetworkSession>
#include "globals.h"
#include "stendProperty.h"
#include <QTimer>
#include <tuple>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "stendFirmware/programming.h"
#include "dataBase/dataBase.h"
#include <QJsonObject>

class StendApi : public QObject
{
    Q_OBJECT
public:
    explicit StendApi(QObject *parent = nullptr);
    ~StendApi();

    Q_PROPERTY(bool isConnected READ getStendIsConnected WRITE setStendIsConnected NOTIFY stendIsConnectedChanged)
    Q_PROPERTY(bool isProcessed READ getStendIsProcessed WRITE setStendIsisProcessed NOTIFY stendIsProcessedChanged)

public slots:
    void startTest();

    /* status */
    void setStatusConnected();
    void setStatusDisconnected();

    /* insert data from tcp */
    void insertDataFromInterface(QByteArray data);
    /* insert error from tcp */
    void insertErrorFromInterface(QString data);

    void testDatabaseConnect();

    bool isAvailableLlsRangeValues();

    /* qml property */
    bool getStendIsConnected() { return mStendIsConnected; }
    void setStendIsConnected(bool value) { mStendIsConnected = value; emit stendIsConnectedChanged(); }

    bool getStendIsProcessed() { return mStendIsProcessed; }
    void setStendIsisProcessed(bool value) { mStendIsProcessed = value; emit stendIsProcessedChanged(); }

    void saveAssemblyTestLlsToDb(QString mcuNum);
    void writeSerialNumToLls(QString sn);
    void markLlsAsDefective(QString mcuSn, QString qrCode, QString jsonData);

signals:
    /* send data through tcp */
    void readyWriteDataToInterface(QByteArray data);
    void stendNotReply(QString text);

    /* test or update state (qml) */
    void testError(QString json);
    void testFinihed(QString json);
    void testUpdateStep(QString json);
    void updateRealTimeData(QString json);

    /* qml property */
    void stendIsConnectedChanged();
    void stendIsProcessedChanged();

    void dataBaseError(QString err);

    void goodMessage(QString text);
    void badMessage(QString text);
    void normalMessage(QString text);

    /* update last data */
    void updateLastData(Globals::sDutBaseStruct llsLastData);

private slots:
    /* send command to execute */
    void sendCommand(StendProperty::eTypeCommand cmd, QJsonObject jsonArgs);
    bool parsinReply(StendProperty::eTypeCommand cmd, QByteArray & dataRx);
    bool headerIsValid(QByteArray & dataRx);
    void packetInsert(StendProperty::sOutputTcpTempStruct pOut, QByteArray & pArray, QByteArray packHeader);
    QJsonObject evaluateTestStatus(Globals::sDutTestStruct & tests);
    QJsonObject convertDataToJson(Globals::sDutBaseStruct & dutStruct);
    void copyInputData(Globals::sDutBaseStruct & dutStruct, const StendProperty::sInputTcpTempStruct* pinputTemp);
    QString convertTestStateToString(Globals::eTestState state);
    bool testIsFinished(Globals::sDutTestStruct & tests);

private:
    Globals::sIp_list ip;
    int interval_read_info_counter;
    QTimer *handlerStendTimer;
    QTimer *timeoutCommandTimer;

    int transferTimeoutId;
    bool mStendIsConnected = false;
    bool mStendIsProcessed = false;
    bool mTestIsFinished = false;

    Globals::sDutBaseStruct llsInfoStruct;
    /* указатель на структуру из настроек тестирования */
    /* настройки теста частоты отправляются в каждом пакете */
    StendProperty::sCapTestValues capValues;
    float curLlsMinValue;
    float curLlsMaxValue;

    QString mMcuNumber;

    QVector <QPair<StendProperty::eTypeCommand, QJsonObject>> command;
    QVector <Globals::S_curves_data>curves_data_vector;

    Programming* programming;
    QThread * programmingThread;

    static constexpr int maxCollectArrayData = 30;
};

#endif // STENDAPI_H
