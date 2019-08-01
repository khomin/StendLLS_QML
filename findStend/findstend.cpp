#include "findStend/findstend.h"
#include <QNetworkInterface>
#include <QHostInfo>
#include <string>

FindStend::FindStend(QObject *parent) : QObject(parent) {
    this->mFindStendModel = new FindStendModel(100);

    mSearchTimer = new QTimer();
    mSearchTimer->setInterval(300);
    mSearchTimer->start();

    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(QHostAddress::Any, network_port);
    mPacket_counter = 0;

    QObject::connect(mSearchTimer, &QTimer::timeout, this, [&]() {
        sConnectSettings dest;

        if(mSearch_ip_state != idle) {
            if(mSearch_ip_timeout < mSearch_ip_const_time) {
                mSearch_ip_timeout++;
                /* Отправляем запрос */
                QByteArray request(broadcast_call);
                // добавляем в запрос версию по
                request.push_back(stend_version);

                mUdpSocket->writeDatagram(request, request.length(), QHostAddress::Broadcast, network_port);
                mPacket_counter++;
            } else {
                mSearch_ip_timeout = 0;
                mSearch_ip_state = idle;

                //--- test
                sConnectSettings connt;
                connt.ip.ip_addr[0] = 192;
                connt.ip.ip_addr[1] = 168;
                connt.ip.ip_addr[2] = 15;
                connt.ip.ip_addr[3] = 22;
                mFindStendModel->addItem(connt);
                //--- test

                mFindStendModel->getAll().empty() ? emit searchComplete("error") : emit searchComplete("normal");
            }
        }
    });

    mSearch_ip_timeout = 0;
    mSearch_ip_state = idle;

    QObject::connect(mUdpSocket, &QUdpSocket::readyRead, this, [&]() {
        QHostAddress address;
        quint16 port;
        while(mUdpSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(mUdpSocket->pendingDatagramSize());
            mUdpSocket->readDatagram(datagram.data(), datagram.size(), &address, &port);

            QString dataStr = datagram.data();
            if(dataStr.toUtf8().contains(QByteArray(broadcast_call_reply))) {
                sConnectSettings dest;
                dest.dateReply = QDateTime::currentDateTime();
                quint32 addr32 = address.toIPv4Address();
                dest.ip.ip_addr[3] = (addr32 & 0xFF);
                dest.ip.ip_addr[2] = ((addr32 & 0xFF00)>>8);
                dest.ip.ip_addr[1] = ((addr32 & 0xFF0000)>>16);
                dest.ip.ip_addr[0] = ((addr32 & 0xFF000000)>>24);

                QHostInfo hostInfo = QHostInfo::fromName(address.toString());
                QString mac = hostInfo.hostName();

                /* ищем ответы раньше */
                auto modelList = mFindStendModel->getAll();
                for(auto i: modelList) {
                    // if it copy -> return
                    if(memcmp(i->getValue().ip.ip_addr, dest.ip.ip_addr, sizeof(dest.ip.ip_addr)) == 0) {
                        return;
                    }
                }
                mFindStendModel->addItem(dest);
            }
        }
    });
}

FindStend::~FindStend() {
    delete mFindStendModel;
    delete mSearchTimer;
    delete mUdpSocket;
}

FindStendModel * FindStend::getModel() {
    return mFindStendModel;
}

void FindStend::startfind() {
    /* get ip */
    QHostAddress address;
    QString address_str;
    QList <QHostAddress>list = QNetworkInterface::allAddresses();
    QVector <sIp_list>ip_list;

    for(int i=0; i<list.size(); i++) {
        address = list.at(i);
        address_str = address.toString();
        if(address_str.count('.')==3) {
            sIp_list ip;
            ip.ip_addr[0] = address_str.section(".",0,0).toShort();
            ip.ip_addr[1] = address_str.section(".",1,1).toShort();
            ip.ip_addr[2] = address_str.section(".",2,2).toShort();
            ip.ip_addr[3] = address_str.section(".",3,3).toShort();
            if(ip.ip_addr[0] != 127 && ip.ip_addr[1] != 0
                    && ip.ip_addr[2] != 0 && ip.ip_addr[3] != 1) {
                ip_list.push_back(ip);
            }
        }
    }

    if(!ip_list.isEmpty()) {
        mNetwork_intface_ip = ip_list.at(0);
        mBroadcast_ip = mNetwork_intface_ip;
        /* flush list stends */
        mFindStendModel->removeAll();
        mSearch_ip_state = on;
        mPacket_counter = 0;
    } else {
        emit searchComplete("error");
    }
}
