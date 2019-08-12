#include "connection.h"
#include <QDebug>
#include "exception"

Connection::Connection() {}

void Connection::setType(Connection::eConnectionType connectionType) {
    if(connectionType == Connection::ConnectionSerial) {
        m_interface = std::make_shared<InterfaceSerial>();
    } else if(connectionType == Connection::ConnectionEth) {
        m_interface = std::make_shared<InterfaceEth>();
    } else throw ("Not valid type interface");
}

bool Connection::addConnection(QString name, const QString & parameters) {
    bool res = false;
    if(m_interface != nullptr) {
        res  = m_interface->openInterface(name, parameters);
        if(res) {
            connect(m_interface.get(), &interfacesAbstract::signalError, this,
                    [&](QString message) {
                emit signalError(message);
            });
            connect(m_interface.get(), &interfacesAbstract::signalReadyReadNewData, this, [&](QByteArray data) {
                emit signalReadyReadNewData(data);
            });
            emit signalOpened();
            emit interfaceNameIsChanged();
        } else {
            emit signalError("not open");
            emit interfaceNameIsChanged();
        }
    }
    return res;
}

bool Connection::writeData(QByteArray data) {
    return m_interface != nullptr ? m_interface->sendData(data) : 0;
}

void Connection::close() {
    if(m_interface.get() != nullptr) {
        m_interface->closeInterface();
    }
    emit signalClosed();
}

QString Connection::getListConnections() {
    return m_interface != nullptr ? m_interface->getAvailableList() : "";
}

bool Connection::isOpened() {
    return m_interface != nullptr ? m_interface->isOpen() : false;
}
