#include "connection.h"
#include <QDebug>

Connection::Connection() {
    m_interface = std::make_shared<InterfaceEth>();
}

bool Connection::addConnection(QString name, const QString & parameters) {
    bool res = false;
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
    } else {
        emit signalError("not open");
    }
    return res;
}

bool Connection::writeData(QByteArray data) {
    return m_interface->sendData(data);
}

void Connection::close() {
    if(m_interface.get() != nullptr) {
        m_interface->closeInterface();
    }
    emit signalClosed();
}

QString Connection::getListConnections() {
    return m_interface->getAvailableList();
}

bool Connection::isOpened() {
    return m_interface->isOpen();
}
