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
//#include "stendProperty.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase();
    ~DataBase();

    typedef struct sParties {
        quint64 id;
        int number;
        int size;
        int type;
        QDateTime created;
        bool finished;
    }sParties;

    typedef struct sPcb {
        quint64 id;
        int number;
        quint64 id_parties;
        struct {
            QString serial_num_cpu;
            QString serial_num_dut;
            quint8 addr;
            QString addr_onewire;
        }dut;
    }sPcb;

    typedef struct sResult
    {
        quint64 id;
        quint64 id_pcb;
        QDateTime date;
        QString json_field;
        int type_test;
    }sResult;

    typedef struct sDataBase {
        sParties parties;
        sPcb pcb;
        sResult result;
    }sDataBase;

    QSqlDatabase base;
    sDataBase dataBaseStruct;

    bool openConnection(QString *error);
    bool closeConnection();
    bool testCconnect();
    bool insertTestData(QString mcu, QString dev_test_state, sDutTestStruct res_data_test, bool res_test);

private:
    static constexpr int USER_ID = 1;
};

#endif // DATABASE_H
