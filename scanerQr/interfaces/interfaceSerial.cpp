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
    this->portHandler->setPortName(name);
    this->parametersJson = QJsonDocument::fromJson(parameters.toUtf8()).object();
    auto baudrate = parametersJson.find("baudrate");
    if(baudrate != parametersJson.end()) {
        portHandler->setBaudRate(baudrate.value().toInt());
        portHandler->setDataBits(QSerialPort::Data8);
        portHandler->setParity(QSerialPort::NoParity);
        portHandler->setStopBits(QSerialPort::OneStop);
        portHandler->setFlowControl(QSerialPort::NoFlowControl);
        res  = portHandler->open(QIODevice::ReadWrite);
        if(res) {
            connect(portHandler.get(), SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(errorHanler(QSerialPort::SerialPortError)));
            connect(portHandler.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
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
        emit signalError(portHandler->portName(), tr("Ошибка интерфейса\nПроверьте соединение"));
    }
}

void InterfaceSerial::readyRead() {
    if(portHandler.get() != nullptr) {
        QTimer::singleShot(100, Qt::CoarseTimer, [&] {
            auto data = portHandler->readAll();
            if(!data.isEmpty()) {
                qDebug() << "inRaw append: "<< data;
                inputBuffer.append(data);
            }
            if(inputBuffer.length() > 24) {
                QRegExp rx("@@([\\d]{1,16})");
                int pos = 0;
                while ((pos = rx.indexIn(inputBuffer, pos)) != -1) {
                    qDebug() << "Parce1: "<< inputBuffer;
                    int packetLen = rx.cap(1).toInt();
                    if(packetLen <= inputBuffer.size()) {
                        QByteArray outArray(inputBuffer.data() + pos, packetLen);
                        emit signalReadyReadNewData(outArray);
                        inputBuffer.remove(pos, packetLen);
                        qDebug() << "Parce2: ready :" << outArray;
                    } else {
                        qDebug() << "Parce2: size not full";
                    }
                    pos += rx.matchedLength();
                }
                qDebug() << "inRaw: "<< inputBuffer;
            }
        });
    }

    if(inputBuffer.length() > 20128) {
        inputBuffer.clear();
    }
}

QString InterfaceSerial::getType() {
    return QString::fromLocal8Bit(typeName, strlen(typeName));
}
