#include "interfaceSerial.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <regex>
#include "QDebug"
#include <QRegExp>

InterfaceSerial::InterfaceSerial() {
    this->portHandler = std::make_unique<QSerialPort>();
    this->isManualClosed = false;
}
void InterfaceSerial::initInterface()  {}

InterfaceSerial::~InterfaceSerial() {
    if(portHandler->isOpen()) {
        portHandler->close();
    }
    portHandler.release();
}

bool InterfaceSerial::openInterface(QString name, const QString & parameters) {
    bool res = false;
    portHandler->setPortName(name);
    parametersJson = QJsonDocument::fromJson(parameters.toUtf8()).object();
    auto baudrate = parametersJson.find("baudrate");
    if(baudrate != parametersJson.end()) {
        res  = portHandler->open(QIODevice::ReadWrite);
        if(res) {
            portHandler->setBaudRate(baudrate.value().toInt());
            portHandler->setDataBits(QSerialPort::Data8);
            portHandler->setParity(QSerialPort::NoParity);
            portHandler->setStopBits(QSerialPort::OneStop);
            portHandler->setFlowControl(QSerialPort::NoFlowControl);
            connect(portHandler.get(), SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(errorHanler(QSerialPort::SerialPortError)));
            connect(portHandler.get(), &QSerialPort::readyRead, this, [&]() {
                QTimer::singleShot(100, Qt::CoarseTimer, [&] {
                    emit signalReadyReadNewData(portHandler->readAll());
                });
            });
            isManualClosed = false;
        } else {
            qDebug() << "openInterface: error";
        }
        qDebug() << "openInterface = " << (res ? QString(" normal") : QString(" error"));
    } else {
        qDebug() << "openInterface: invalid json";
    }
    return res;
}

bool InterfaceSerial::isOpen() {
    return portHandler->isOpen();
}

void InterfaceSerial::closeInterface() {
    if(portHandler != nullptr) {
        if(portHandler->isOpen()) {
            isManualClosed = true;
            portHandler->close();
            disconnect(portHandler.get(), SIGNAL(error(QSerialPort::SerialPortError)), this, nullptr);
        }
    }
}

bool InterfaceSerial::sendData(QByteArray data)  {
    bool res = false;
    if(portHandler != nullptr) {
        if(portHandler->isOpen()) {
            res = portHandler->write(data);
        }
    }
    return res;
}

bool InterfaceSerial::readData(QByteArray &pData) {
    if(!portHandler->isOpen()) {
        return false;
    }
    pData = portHandler->readAll();
    return !pData.isEmpty();
}

QString InterfaceSerial::getInterfaceName() {
    return portHandler->portName();
}

QPair<QStringList,QStringList> InterfaceSerial::getInterfaceProperty() {
    QPair<QStringList,QStringList> res;
    QString description;
    QString manufacturer;
    QString serialNumber;
    QString baudrate;
    if(portHandler->isOpen()) {
        const auto infos = QSerialPortInfo::availablePorts();
        for(const QSerialPortInfo &info : infos) {
            if(info.portName().contains(portHandler->portName())) {
                description = info.description();
                manufacturer = info.manufacturer();
                serialNumber = info.serialNumber();
                if(portHandler->isOpen()) {
                    baudrate = QString::number(portHandler->baudRate());
                }
                res.first.push_back("info.portName()");
                res.second.push_back(info.portName());
                res.first.push_back("baudrate");
                res.second.push_back(baudrate);
                res.first.push_back("description");
                res.second.push_back(description);
                res.first.push_back("manufacturer");
                res.second.push_back(manufacturer);
                res.first.push_back("serialNumber");
                res.second.push_back(serialNumber);
                break;
            }
        }
    }
    return res;
}

QString InterfaceSerial::getAvailableList() {
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos) {
        if(portHandler != nullptr) {
            if(!portHandler->isOpen()) {
                jsonArray.append(QJsonValue(info.portName()));
            }
        }
    }
    jsonObj.insert("portName", jsonArray);
    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Compact);
}

void InterfaceSerial::errorHanler(QSerialPort::SerialPortError err) {
    qDebug() << "InterfaseSerial -ERR=" << err;
    if(err != QSerialPort::NoError) {
        disconnect(portHandler.get(), SIGNAL(error(QSerialPort::SerialPortError)), this, nullptr);
        portHandler->close();
        emit signalError(tr("Interface error"));
    }
}

QString InterfaceSerial::getType() {
    return QString::fromLocal8Bit(typeName, strlen(typeName));
}
