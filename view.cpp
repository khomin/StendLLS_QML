#include "view.h"

View::View(QObject *parent) : QObject(parent) {
    validateQchValues = new ValidateQchValues(&selectLlsTestType);

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

    connect(&stendApi, &StendApi::updateLastData, validateQchValues, &ValidateQchValues::insertLlsData);

    connect(&stendApi, &StendApi::dataBaseError, this, &View::signalDataBaseError);

    connect(&stendApi, &StendApi::goodMessage, this, &View::goodMessage);
    connect(&stendApi, &StendApi::badMessage, this, &View::badMessage);
    connect(&stendApi, &StendApi::normalMessage, this, &View::normalMessage);

    connect(&selectLlsTestType, &SelectLlsTestType::dataBaseError, this, &View::signalDataBaseError);

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

ValidateQchValues* View::getStendQchDecision() {
    return validateQchValues;
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

SelectLlsTestType* View::getSelectLlsType() {
    return &selectLlsTestType;
}

void View::startTestStend() {
    stendApi.startTest();
}

void View::testDatabaseConnect() {
    stendApi.testDatabaseConnect();
}
