#include "interfaceEth.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <regex>
#include "QDebug"
#include <QRegExp>
#include "globals.h"

InterfaceEth::InterfaceEth() {
    this->isManualClosed = false;
}

void InterfaceEth::initInterface()  {}

InterfaceEth::~InterfaceEth() {
    if(isOpen()) {
        closeInterface();
    }
    if(tcpSocket != nullptr) {
        delete tcpSocket;
        tcpSocket = nullptr;
    }
    if(tcpServer != nullptr) {
        delete tcpServer;
        tcpServer = nullptr;
    }
}

bool InterfaceEth::openInterface(QString name, const QString & parameters) {
    bool res = false;
    tcpSocket = new QTcpSocket();
    tcpServer = new QTcpServer();
    this->parametersJson = QJsonDocument::fromJson(parameters.toUtf8()).object();
    tcpSocket->connectToHost(QHostAddress(name), parametersJson.value("port").toInt());
    tcpSocket->waitForConnected(1000);
    res = tcpSocket->state() == QAbstractSocket::ConnectedState;
    if(res) {
        connect(tcpSocket, &QTcpSocket::readyRead, this, [&]() {
            emit signalReadyReadNewData(tcpSocket->readAll());
        });
        connect(tcpSocket, &QTcpSocket::aboutToClose, this, &InterfaceEth::signalClosed);
        tcpServer->listen(QHostAddress::AnyIPv4, parametersJson.value("port").toInt());
        isManualClosed = false;
        emit signalOpened();
    } else {
        emit signalError(name, "not open");
        qDebug() << "openInterface: error";
    }
    return res;
}

bool InterfaceEth::isOpen() {
    if(tcpSocket != nullptr) {
        return tcpSocket->state() == QAbstractSocket::ConnectedState;
    }
    return false;
}

void InterfaceEth::closeInterface() {
    if(isOpen()) {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        isManualClosed = true;
        if(tcpSocket != nullptr) {
            delete tcpSocket;
            tcpSocket = nullptr;
        }
        if(tcpServer != nullptr) {
            delete tcpServer;
            tcpServer = nullptr;
        }
    }
}

bool InterfaceEth::sendData(QByteArray data)  {
    bool res = false;
    if(isOpen()) {
        res = tcpSocket->write(data);
    }
    return res;
}

bool InterfaceEth::readData(QByteArray &pData) {
    if(isOpen()) {
        pData = tcpSocket->readAll();
    }
    return !pData.isEmpty();
}

QString InterfaceEth::getInterfaceName() {
    return "undefined";
}

QPair<QStringList,QStringList> InterfaceEth::getInterfaceProperty() {
    return QPair<QStringList,QStringList>();
}

QString InterfaceEth::getAvailableList() {
    QJsonObject jsonObj;
    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Compact);
}

QString InterfaceEth::getType() {
    return "";
}
