#ifndef FINDSTEND_H
#define FINDSTEND_H

#include <QObject>
#include "findStend/findStendModel.h"
#include "QUdpSocket"
#include "globals.h"
#include "QTimer"

class FindStend : public QObject
{
    Q_OBJECT
public:
    explicit FindStend(QObject *parent = nullptr);
    ~FindStend();

    typedef enum eSearch_ip_State { idle, on, abort, ok }eSearch_ip_State;

signals:
    void searchComplete(QString status);

public slots:
    void startfind();
    FindStendModel * getModel();

private:
    FindStendModel* mFindStendModel;
    QTimer *mSearchTimer;
    sConnectSettings mSelected_dest;
    sIp_list mBroadcast_ip;
    sIp_list mNetwork_intface_ip;
    QUdpSocket* mUdpSocket;
    eSearch_ip_State mSearch_ip_state;
    int mSearch_ip_timeout;
    int mPacket_counter;
    const int mSearch_ip_const_time = 10;
};

#endif // FINDSTEND_H
