#include "findStend/findstend.h"
#include <QNetworkInterface>
#include <QHostInfo>
#include <string>
#include <QDataStream>

FindStend::FindStend(QObject *parent) : QObject(parent) {
    mSearchTimer = new QTimer();
    mSearchTimer->setInterval(1500);
    mSearchTimer->start();

    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(QHostAddress::Any, Globals::network_port);
    mPacket_counter = 0;

    QObject::connect(mSearchTimer, &QTimer::timeout, this, [&]() {
        Globals::sConnectSettings dest;

        if(mSearch_ip_state != idle) {
            if(mSearch_ip_timeout < mSearch_ip_const_time) {
                mSearch_ip_timeout++;
                /* Отправляем запрос */
                QByteArray request(Globals::broadcast_call);
                // добавляем в запрос версию по
                request.push_back(Globals::stend_version);

                mUdpSocket->writeDatagram(request, request.length(), QHostAddress::Broadcast, Globals::network_port);
                mPacket_counter++;
            } else {
                mSearch_ip_timeout = 0;
                mSearch_ip_state = idle;
                mFindStendModel.getAll().empty() ? emit searchComplete("error") : emit searchComplete("normal");
            }
        }
    });

    mSearch_ip_timeout = 0;
    mSearch_ip_state = idle;

    QObject::connect(mUdpSocket, &QUdpSocket::readyRead, this, [&]() {
        QHostAddress hostAddr;
        quint16 hostPort;
        if(mUdpSocket != nullptr) {
            while (mUdpSocket->hasPendingDatagrams()) {
                QByteArray buf(mUdpSocket->pendingDatagramSize(), Qt::Uninitialized);
                QDataStream str(&buf, QIODevice::ReadOnly);
                mUdpSocket->readDatagram(buf.data(), buf.size(), &hostAddr, &hostPort);
                qDebug() << "udp from: " << hostAddr;
                qDebug() << "udp port: " << hostPort;

                QString dataStr(buf);
                if(dataStr.toUtf8().contains(QByteArray(Globals::broadcast_call_reply))) {
                    Globals::sConnectSettings dest;
                    dest.dateReply = QDateTime::currentDateTime();
                    quint32 addr32 = hostAddr.toIPv4Address();
                    dest.ip.ip_addr[3] = (addr32 & 0xFF);
                    dest.ip.ip_addr[2] = ((addr32 & 0xFF00)>>8);
                    dest.ip.ip_addr[1] = ((addr32 & 0xFF0000)>>16);
                    dest.ip.ip_addr[0] = ((addr32 & 0xFF000000)>>24);

                    /* ищем ответы раньше */
                    auto modelList = mFindStendModel.getAll();
                    for(auto i: modelList) {
                        // if it copy -> return
                        if((i->getValue().ip.ip_addr[0] == dest.ip.ip_addr[0])
                                && (i->getValue().ip.ip_addr[1] == dest.ip.ip_addr[1])
                                && (i->getValue().ip.ip_addr[2] == dest.ip.ip_addr[2])
                                && (i->getValue().ip.ip_addr[3] == dest.ip.ip_addr[3]) ) {
                            return;
                        }
                    }
                    mFindStendModel.addItem(dest);
                }

                if(mUdpSocket == nullptr) {
                    return;
                }
            }
        }
    });
}

FindStend::~FindStend() {
    delete mSearchTimer;
    delete mUdpSocket;
}

FindStendModel * FindStend::getModel() {
    return &mFindStendModel;
}

void FindStend::startfind() {
    /* get ip */
    QHostAddress address;
    QString address_str;
    QList <QHostAddress>list = QNetworkInterface::allAddresses();
    QVector <Globals::sIp_list>ip_list;

    for(int i=0; i<list.size(); i++) {
        address = list.at(i);
        address_str = address.toString();
        if(address_str.count('.')==3) {
            Globals::sIp_list ip;
            ip.ip_addr[0] = (uint8_t)address_str.section(".",0,0).toInt();
            ip.ip_addr[1] = (uint8_t)address_str.section(".",1,1).toInt();
            ip.ip_addr[2] = (uint8_t)address_str.section(".",2,2).toInt();
            ip.ip_addr[3] = (uint8_t)address_str.section(".",3,3).toInt();
            if(ip.ip_addr[0] != 127 && ip.ip_addr[1] != 0
                    && ip.ip_addr[2] != 0 && ip.ip_addr[3] != 1) {
                qDebug() << "Find stend: add adapter" << address_str;
                ip_list.push_back(ip);
            }
        }
    }

    if(!ip_list.isEmpty()) {
        mNetwork_intface_ip = ip_list.at(0);
        mBroadcast_ip = mNetwork_intface_ip;
        /* flush list stends */
        mFindStendModel.removeAll();
        mSearch_ip_state = on;
        mPacket_counter = 0;
    } else {
        emit searchComplete("error");
    }
}
