#ifndef INTERFACESERIAL_H
#define INTERFACESERIAL_H

#include <QObject>
#include <memory>
#include "connection/interfaces/interfacesAbstract.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QJsonObject>

class InterfaceSerial : public interfacesAbstract
{
    Q_OBJECT
public:
    InterfaceSerial();
    ~InterfaceSerial() override;

public slots:
    void initInterface() override;
    bool openInterface(QString name, const QString & parameters) override;
    bool isOpen() override;
    void closeInterface() override;
    bool sendData(QByteArray pData) override;
    bool readData(QByteArray &pData) override;
    QString getInterfaceName() override;
    QPair<QStringList,QStringList> getInterfaceProperty() override;
    QString getAvailableList() override;
    QString getType() override;

private slots:
    void errorHanler(QSerialPort::SerialPortError err);
private:
    static constexpr char const * typeName = "serial";
    bool isManualClosed;
    std::unique_ptr<QSerialPort> portHandler = nullptr;
    QByteArray inputBuffer;
    QJsonObject parametersJson;
};

#endif // INTERFACESERIAL_H
