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

    Q_PROPERTY(QString stendRole READ getStendRole WRITE setStendRole NOTIFY stendRoleIsChanged)

public slots:
    Q_INVOKABLE Settings* getSettings();
    Q_INVOKABLE void startFindStends();
    Q_INVOKABLE FindStendModel* getFindStendModel();

    Q_INVOKABLE bool addConnection(QString name, const QString & parameters);
    Q_INVOKABLE void closeConnection();

    Q_INVOKABLE QString getListConnections();
    Q_INVOKABLE bool isConnected();

    Q_INVOKABLE LogModel* getStendLogModel();

    Q_INVOKABLE void startTestStend();

    Q_INVOKABLE bool testDatabaseConnect();

    Q_INVOKABLE void writeSerialNumToLls(QString llsMcuNumber);
    Q_INVOKABLE void markLlsAsDefected(QString llsMcuNumber);

signals:
    void signalStendLost();
    void signalStendReady();

    void searchStendComplete(QString status);

    void signalInterfaceError(QString error);
    void signalInterfaceClosed();
    void signalInterfaceReady();

    void signalTestFinished(QString json);
    void signalTestError(QString json);
    void signalTestUpdateStatus(QString json);

    void signalAppendLog(QString text);

    void signalUpdateRealTimeData(QString json);

    void stendRoleIsChanged();

private:
    void setStendRole(QString role) { stendRole = role; emit stendRoleIsChanged(); }
    QString getStendRole() { return stendRole; }

private:
    QString stendRole;
    Connection connection;
    StendApi stendApi;
    Log log;
    FindStend findStend;
};

#endif // VIEW_H
