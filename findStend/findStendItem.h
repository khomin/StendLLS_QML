#ifndef FINDSTENDITEM_H
#define FINDSTENDITEM_H

#include <QObject>
#include "QColor"
#include <QPair>
#include "globals.h"

class FindStendItem : public QObject
{
    Q_OBJECT
public:

    explicit FindStendItem(sConnectSettings value) {
        this->value = value;
        ip = QString("%1.%2.%3.%4")
                .arg(value.ip.ip_addr[0])
                .arg(value.ip.ip_addr[1])
                .arg(value.ip.ip_addr[2])
                .arg(value.ip.ip_addr[3]);
        mac = QString("%1.%2.%3.%4")
                .arg(value.mac[0])
                .arg(value.mac[1])
                .arg(value.mac[2])
                .arg(value.mac[3]);
    }

    Q_PROPERTY(QString ip READ getIp NOTIFY signalIpIsChanged)
    Q_PROPERTY(QString mac READ getMac NOTIFY signalMacIsChanged)

    QString getIp() {
        return ip;
    }
    QString getMac() {
        return mac;
    }
    sConnectSettings getValue() {
        return value;
    }

signals:
    void signalIpIsChanged();
    void signalMacIsChanged();

private:
    QString ip;
    QString mac;
    sConnectSettings value;
};

#endif // FINDSTENDITEM_H
