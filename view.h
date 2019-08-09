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
#include "scanerQr/interfaces/interfaceSerial.h"
#include "scanerQr/qrScaner.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);

    /* stend property */
    Q_PROPERTY(QString stendRole READ getStendRole WRITE setStendRole NOTIFY stendRoleIsChanged)

public slots:

    /* stend find */
    Q_INVOKABLE void startFindStends();

    /* stend connections */
    Q_INVOKABLE bool addConnection(QString name, const QString & parameters);
    Q_INVOKABLE void closeConnection();
    Q_INVOKABLE QString getListConnections();
    Q_INVOKABLE bool isConnected();

    /* stend testing */
    Q_INVOKABLE void startTestStend();
    Q_INVOKABLE bool testDatabaseConnect();

    /* stend write sn to lls */
    Q_INVOKABLE void writeSerialNumToLls(QString llsMcuNumber);
    Q_INVOKABLE void markLlsAsDefected(QString llsMcuNumber);

    /* scaner */
    Q_INVOKABLE bool addScanerConnection(QString name, const QString & parameters);
    Q_INVOKABLE void closeScanerConnection();
    Q_INVOKABLE QString getScanersList();
    Q_INVOKABLE bool isScanerConnected();

public:
    //
    /* used in main.cpp */
    //
    FindStendModel* getFindStendModel();

    Q_INVOKABLE LogModel* getStendLogModel();

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

    /* scaner */
    void scanerError(QString error);
    void scanerOpened();
    void scanerClosed();
    void scanerUpdateNumber(QString qrNumber);
    void scanerErrorReading(QString codeMessage);

    void signalError(QString conTypeName, QString errorMessage);
    void signalOpened();
    void signalClosed();
    void signalReadyReadNewData(const QByteArray & data);

private:
    void setStendRole(QString role) { stendRole = role; emit stendRoleIsChanged(); }
    QString getStendRole() { return stendRole; }
    bool getScanerIsAvailable();

private:
    QString stendRole;
    Connection connection;
    StendApi stendApi;
    Log log;
    FindStend findStend;
    Connection scanerConnection;
    QrScaner qrScaner;
};

#endif // VIEW_H
