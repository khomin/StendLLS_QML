#include "view.h"

View::View(QObject *parent) : QObject(parent) {
    connect(&connection, &Connection::signalClosed, this, &View::signalInterfaceClosed);
    connect(&connection, &Connection::signalOpened, this, &View::signalInterfaceReady);
    connect(&connection, &Connection::signalError, this, &View::signalInterfaceError);


    connect(&connection, &Connection::signalReadyReadNewData, &stendApi, &StendApi::insertDataFromInterface);
    connect(&stendApi, &StendApi::readyWriteDataToInterface, &connection, &Connection::writeData);
    connect(&stendApi, &StendApi::stendNotReply, this, &View::signalStendLost);
    connect(&stendApi, &StendApi::testError, this, &View::signalTestComplete);
    connect(&stendApi, &StendApi::testFinihed, this, &View::signalTestComplete);

    connect(&connection, &Connection::signalOpened, &stendApi, &StendApi::setStatusConnected);
    connect(&connection, &Connection::signalClosed, &stendApi, &StendApi::setStatusDisconnected);

//    connect(&stendApi, &StendApi::powerDutDisabled, this, &View::searchStendComplete);
//    connect(&stendApi, &StendApi::powerDutEnabed, this, &View::searchStendComplete);

    connect(&findStend, &FindStend::searchComplete, this, &View::searchStendComplete);

    connect(&stendApi, &StendApi::updateRealTimeData, this, &View::signalUpdateRealTimeData);
}


bool View::addConnection(QString name, const QString & parameters) {
    bool res = connection.addConnection(name, parameters);
    log.insertLog("Open interface: " + name, res ? "grey" : "red");
    return res;
}

void View::closeConnection() {
    log.insertLog("Close interface", "grey");
    connection.close();
}

Settings* View::getSettings() {
    return &settings;
}

void View::startFindStends() {
    log.insertLog("Start find", "grey");
    findStend.startfind();
}

FindStendModel* View::getFindStendModel() {
    return findStend.getModel();
}

void View::setStendActiveIp(int indexModel) {
    log.insertLog("Set default stend", "grey");
    auto model = findStend.getModel()->getAll();
    if(model.length() >= indexModel){
        settings.setStendActiveIp(model.at(indexModel).get()->getIp());
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
