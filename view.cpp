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

//    connect(&stendApi, &StendApi::powerDutDisabled, this, &View::searchStendComplete);
//    connect(&stendApi, &StendApi::powerDutEnabed, this, &View::searchStendComplete);

    connect(&findStend, &FindStend::searchComplete, this, &View::searchStendComplete);
    connect(&stendApi, &StendApi::updateRealTimeData, this, &View::signalUpdateRealTimeData);
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

Settings* View::getSettings() {
    return &Settings::Instance();
}

void View::startFindStends() {
    log.insertLog(tr("Start find"), "grey");
    findStend.startfind();
}

FindStendModel* View::getFindStendModel() {
    return findStend.getModel();
}

void View::setStendActiveIp(int indexModel) {
    log.insertLog(tr("Set default stend"), "grey");
    auto model = findStend.getModel()->getAll();
    if(model.length() >= indexModel){
        Settings::Instance().setStendActiveIp(model.at(indexModel).get()->getIp());
    }
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
