#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <memory.h>
#include "interfaces/interfacesAbstract.h"
#include "interfaces/interfaceEth.h"
#include <QJsonObject>

class Connection : public QObject {
    Q_OBJECT
public:
    explicit Connection();

public slots:
    bool addConnection(QString name, const QString & parameters);
    bool writeData(QByteArray data);
    void close();
    QString getListConnections();
    bool isOpened();

signals:
    void signalError(const QString message);
    void signalReadyReadNewData(QByteArray data);
    void signalOpened();
    void signalClosed();

private:
    std::shared_ptr<interfacesAbstract> m_interface;
};

#endif // CONNECTION_H
