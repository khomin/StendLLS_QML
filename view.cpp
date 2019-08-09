#include "view.h"

View::View(QObject *parent) : QObject(parent) {
    connect(&connection, &Connection::signalClosed, this, &View::signalInterfaceClosed);
    connect(&connection, &Connection::signalOpened, this, &View::signalInterfaceReady);
    connect(&connection, &Connection::signalError, this, &View::signalInterfaceError);
    connect(&connection, &Connection::signalReadyReadNewData, &stendApi, &StendApi::insertDataFromInterface);

    connect(&stendApi, &StendApi::readyWriteDataToInterface, &connection, &Connection::writeData);
    connect(&stendApi, &StendApi::stendNotReply, this, &View::signalStendLost);

    connect(&stendApi, &StendApi::testFinihed, this, &View::signalTestFinished);
    connect(&stendApi, &StendApi::testError, this, &View::signalTestError);
    connect(&stendApi, &StendApi::testUpdateStep, this, &View::signalTestUpdateStatus);

    connect(&connection, &Connection::signalOpened, &stendApi, &StendApi::setStatusConnected);
    connect(&connection, &Connection::signalClosed, &stendApi, &StendApi::setStatusDisconnected);

    connect(&findStend, &FindStend::searchComplete, this, &View::searchStendComplete);
    connect(&stendApi, &StendApi::updateRealTimeData, this, &View::signalUpdateRealTimeData);

    connect(&scanerConnection, &Connection::signalError, this, &View::scanerError);
    connect(&scanerConnection, &Connection::signalError, this, &View::scanerOpened);
    connect(&scanerConnection, &Connection::signalError, this, &View::scanerClosed);
    connect(&scanerConnection, &Connection::signalReadyReadNewData, this, [&](QByteArray data) {
        qrScaner.insertQrData(data);
    });
    connect(&qrScaner, &QrScaner::qrCodeUpdateSerialNum, this, &View::scanerUpdateNumber);
    connect(&qrScaner, &QrScaner::qrCodeUpdateSerialNum, this, &View::scanerErrorReading);
}

bool View::addConnection(QString name, const QString & parameters) {
    bool res = connection.addConnection(name, parameters);
    log.insertLog(tr("Open interface:") + QString("\r\n") + name, res ? "grey" : "red");
    return res;
}

void View::closeConnection() {
    log.insertLog(tr("Close interface"), "grey");
    connection.close();
}

void View::startFindStends() {
    log.insertLog(tr("Start find"), "grey");
    findStend.startfind();
}

FindStendModel* View::getFindStendModel() {
    return findStend.getModel();
}

QString View::getListConnections() {
    return connection.getListConnections();
}

bool View::isConnected() {
    return connection.isOpened();
}

LogModel* View::getStendLogModel() {
    return log.getLogModel();
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

bool View::addScanerConnection(QString name, const QString & parameters) {
    return scanerConnection.addConnection(name, parameters);
}

void View::closeScanerConnection() {
    log.insertLog(tr("Close scaner interface"), "grey");
    scanerConnection.close();
}

QString View::getScanersList() {
    return scanerConnection.getListConnections();
}

bool View::isScanerConnected() {
    return scanerConnection.isOpened();
}
