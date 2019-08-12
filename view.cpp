#include "view.h"

View::View(QObject *parent) : QObject(parent) {
    stendConnection.setType(Connection::ConnectionEth);

    qrScanerConnection.setType(Connection::ConnectionSerial);

    connect(&stendConnection, &Connection::signalReadyReadNewData, &stendApi, &StendApi::insertDataFromInterface);

    connect(&stendApi, &StendApi::readyWriteDataToInterface, &stendConnection, &Connection::writeData);

    connect(&stendApi, &StendApi::testFinihed, this, &View::signalTestFinished);
    connect(&stendApi, &StendApi::testError, this, &View::signalTestError);
    connect(&stendApi, &StendApi::testUpdateStep, this, &View::signalTestUpdateStatus);

    connect(&stendConnection, &Connection::signalOpened, &stendApi, &StendApi::setStatusConnected);
    connect(&stendConnection, &Connection::signalClosed, &stendApi, &StendApi::setStatusDisconnected);

    connect(&findStend, &FindStend::searchComplete, this, &View::searchStendComplete);
    connect(&stendApi, &StendApi::updateRealTimeData, this, &View::signalUpdateRealTimeData);

    connect(&qrScanerConnection, &Connection::signalReadyReadNewData, this, [&](QByteArray data) {
        qrScaner.insertQrData(data);
    });
}

FindStend* View::getFindStend() {
    return &findStend;
}

Connection* View::getStendInterface() {
    return &stendConnection;
}

StendApi* View::getStendProp() {
    return &stendApi;
}

Connection* View::getQrScanerInterface() {
    return &qrScanerConnection;
}

LogModel* View::getStendLogModel() {
    return log.getLogModel();
}

QrScaner* View::getQrScanerProp() {
    return &qrScaner;
}

void View::startTestStend() {
    stendApi.startTest();
}
bool View::testDatabaseConnect() {
    return stendApi.testDatabaseConnect();
}

void View::writeSerialNumToLls(QString llsMcuNumber) {
    //    stendApi.
}

void View::markLlsAsDefected(QString llsMcuNumber) {

}
