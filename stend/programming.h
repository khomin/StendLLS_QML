#ifndef PROGRAMMING_H
#define PROGRAMMING_H

#include <QObject>
#include "qprocess.h"
#include "qtimer.h"
#include <QVector>
#include <QFile>
#include <QThread>

class Programming : public QObject
{
    Q_OBJECT
public:
    explicit Programming(QObject *parent = 0);
    ~Programming();

    typedef enum {
        programming_ok,
        programming_ok_readp_on,
        programming_error
    }eStateProgramm;

    eStateProgramm state;

public:
    void startProgramm();
    void readSerialNumMcu();

public slots:
    QString programm_get_serialNumMcu();
    void programm_start_programming();
    QString writeCmdCommand(QString path_cmd, QStringList command);
    bool programm_clear_flash(QString path_cmd);
    bool programm_set_rom_protected(QString path_cmd);
    bool programm_set_rom_unprotected(QString path_cmd);
    bool programm_get_info(QString path_cmd, QString *serial_num_mcu);
    bool programm_write_bootloader(QString path_cmd);
    bool programm_write_main_programm(QString path_cmd);
    int programm_dut_reboot(QString path_cmd);
    bool programm_check_st_link(QString path_cmd);
    void initThread();

signals:
    void exeError(QString str);
    void programmFinished();
    void exeReadSerialNumMcu(QString str);
    void exeUpdate(int);

private:
    typedef enum {
        read_serial_mcu,
        start_programm
    }queue_type;

    QVector <queue_type> command;
    QProcess *process;
    QTimer *process_timer;
    QString process_command_output;
    QString mcu_serial_num;

    QString mPathCli;
    QString mPathLlsFirmware;
    QString mPathBootloaderFirmware;
};

#endif // PROGRAMMING_H
