#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include "connection/connection.h"
#include "connection/log.h"
#include "connection/logModel/logModel.h"
#include "settings.h"
#include "findStend/findStendModel.h"
#include "findStend/findstend.h"
#include "stend/stendApi.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE Settings* getSettings();
    Q_INVOKABLE void startFindStends();
    Q_INVOKABLE FindStendModel* getFindStendModel();

    Q_INVOKABLE void setStendActiveIp(int indexModel);

    Q_INVOKABLE bool addConnection(QString name, const QString & parameters);
    Q_INVOKABLE void closeConnection();

    Q_INVOKABLE QString getListConnections();
    Q_INVOKABLE bool isConnected();

    Q_INVOKABLE LogModel* getStendLogModel();

signals:
    void signalStendLost();
    void signalStendReady();

    void searchStendComplete(QString status);

    void signalInterfaceError(QString error);
    void signalInterfaceClosed();
    void signalInterfaceReady();

    void signalTestComplete(QString json);

    void signalAppendLog(QString text);

    void signalUpdateRealTimeData(QString json);

private:
    Connection connection;
    StendApi stendApi;
    Log log;
    Settings settings;
    FindStend findStend;
};

#endif // VIEW_H
