#ifndef DATABASEPROPERTY_H
#define DATABASEPROPERTY_H

#include <QObject>
#include "QDateTime"
#include "globals.h"

class DataBaseProperty : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseProperty(QObject *parent = nullptr);


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
};

#endif // DATABASEPROPERTY_H
