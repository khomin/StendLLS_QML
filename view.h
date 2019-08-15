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
#include "stend/validateQchValues.h"
#include "selectLlsTestType/selectLlsTesttype.h"

class View : public QObject
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);

    /* stend property */
    Q_PROPERTY(QString stendRole READ getStendRole WRITE setStendRole NOTIFY stendRoleIsChanged)

public slots:

    /* stend testing */
    Q_INVOKABLE void startTestStend();
    Q_INVOKABLE void testDatabaseConnect();

public:
    //
    /* used in main.cpp */
    //

    /* stend */
    Q_INVOKABLE Connection* getStendInterface();
    Q_INVOKABLE StendApi* getStendProp();
    Q_INVOKABLE ValidateQchValues* getStendQchDecision();

    /* qr scaner */
    Q_INVOKABLE Connection* getQrScanerInterface();
    Q_INVOKABLE QrScaner* getQrScanerProp();

    /* qch value validate */
    SelectLlsTestType* getSelectLlsType();

    FindStend* getFindStend();

    Q_INVOKABLE LogModel* getStendLogModel();

signals:
    void searchStendComplete(QString status);

    void signalTestFinished(QString json);
    void signalTestError(QString json);
    void signalTestUpdateStatus(QString json);

    void signalAppendLog(QString text);

    void signalUpdateRealTimeData(QString json);

    void stendRoleIsChanged();

    void signalDataBaseError(QString err);

    void goodMessage(QString text);
    void badMessage(QString text);
    void normalMessage(QString text);

private:
    void setStendRole(QString role) { stendRole = role; emit stendRoleIsChanged(); }
    QString getStendRole() { return stendRole; }
    bool getScanerIsAvailable();

private:
    QString stendRole = "undefined";
    Connection stendConnection;
    StendApi stendApi;
    Log log;
    FindStend findStend;
    Connection qrScanerConnection;
    QrScaner qrScaner;
    SelectLlsTestType selectLlsTestType;
    ValidateQchValues* validateQchValues;
};

#endif // VIEW_H
