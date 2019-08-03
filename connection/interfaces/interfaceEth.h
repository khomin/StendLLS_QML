#ifndef InterfaceEth_H
#define InterfaceEth_H

#include <QObject>
#include <memory>
#include "interfacesAbstract.h"
#include <QTimer>
#include <QJsonObject>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>

class InterfaceEth : public interfacesAbstract
{
    Q_OBJECT
public:
    InterfaceEth();
    ~InterfaceEth() override;

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

private:
    bool isManualClosed;
    QNetworkSession *networkSession;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QByteArray inputBuffer;
    QJsonObject parametersJson;
};

#endif // InterfaceEth_H
