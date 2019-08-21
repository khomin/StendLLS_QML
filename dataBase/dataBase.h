#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "QDateTime"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSql>
#include "QtSql/QSqlDriver"
#include "QtSql/QSqlError"
#include "QtSql/QSqlQuery"
#include "QtSql/QSqlTableModel"
#include "QtSql/QSqlRecord"
#include "QtSql/QSqlResult"
#include "QDebug"
#include "globals.h"
#include "dataBaseProperty.h"
#include <QQmlEngine>

class DataBase : public QObject
{
    Q_OBJECT
public:
    static DataBase & Instance() {
        static DataBase mIntance;
        return mIntance;
    }

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);
        return &Instance();
    }

    DataBase(DataBase const &) = delete;
    DataBase(DataBase &&) = delete;
    DataBase& operator=(DataBase const &) = delete;
    DataBase& operator=(DataBase &&) = delete;

    Q_PROPERTY(bool isOpened READ getIsOpened NOTIFY isOpenedChanged)

protected:
    DataBase();
    virtual ~DataBase();

public:
    bool openConnection(QString *error);
    bool isConnected();
    bool closeConnection();
    bool testCconnect();
    bool insertPcbTestData(QString mcu, QString dev_test_state, Globals::sDutTestStruct res_data_test, bool res_test);
    bool insertAsseblyTestResult(QString mcu, QString dev_test_state, Globals::sDutTestStruct res_data_test, bool res_test);
    bool checkDeviceQrCode(QString qr_code);
    bool placeResult(bool res_bool, QString qr_code, QString mcu, QString dev_status, QString resultJson);
    bool getLlsMaxMinValues(int dts_id, float *cnt_value_min, float *cnt_value_max,
                            float *voltage_value_min, float *voltage_value_max,
                            float *freq_value_min, float *freq_value_max,
                            float *nominal_current,
                            float *tolerance_level,
                            uint32_t *empty_value_minumum_correct,
                            uint32_t *full_value_minumum_correct,
                            float *tolerance_current);
    Q_INVOKABLE void checkUserPermission(QString login, QString password);

    QVector <Globals::sTypeDevicesList> getDeviceFullDeviceList();

signals:
    void isOpenedChanged();
    void userPermissionReadyRead(QString permission);

private:
    bool getIsOpened() {
        mIsConnected = DataBase::Instance().isConnected();
        return mIsConnected;
    }

    bool mIsConnected = false;

    static constexpr int USER_ID = 1;
    QSqlDatabase base;
    DataBaseProperty::sDataBase dataBaseStruct;
};

#endif // DATABASE_H
