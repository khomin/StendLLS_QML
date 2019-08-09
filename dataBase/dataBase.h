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

class DataBase : public QObject
{
    Q_OBJECT
public:
    static DataBase & Instance() {
        static DataBase mIntance;
        return mIntance;
    }
    DataBase(DataBase const &) = delete;
    DataBase(DataBase &&) = delete;
    DataBase& operator=(DataBase const &) = delete;
    DataBase& operator=(DataBase &&) = delete;

protected:
    DataBase();
    virtual ~DataBase();

public:
    bool openConnection(QString *error);
    bool closeConnection();
    bool testCconnect();
    bool insertTestData(QString mcu, QString dev_test_state, sDutTestStruct res_data_test, bool res_test);
    bool checkDeviceQrCode(QString qr_code);
    void addNewDeviceByMcuSn(QString mcuSn);

private:
    static constexpr int USER_ID = 1;
    QSqlDatabase base;
    DataBaseProperty::sDataBase dataBaseStruct;
};

#endif // DATABASE_H
