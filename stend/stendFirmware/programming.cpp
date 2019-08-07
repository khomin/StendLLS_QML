#include "programming.h"
#include "settings.h"

const QStringList command_connect_SWD       =  QStringList() << "-c" << "SWD" << "UR";
const QStringList command_clear_flash       =  QStringList() << "-ME";
const QStringList command_read_mcu_num      =  QStringList() << "-r8" << "0x1FFFF7AC" << "0x0C";
const QStringList command_write_bootloader  =  QStringList() << "-P" << "%1" << "0x08000000" << "-V";
const QStringList command_write_dut         =  QStringList() << "-P" << "%1" << "0x08002000" << "-V";
const QStringList command_hard_reset        =  QStringList() << "-HardRst";
const QStringList command_protect_rom       =  QStringList() << "-OB" << "RDP=1";
const QStringList command_unprotect_rom     =  QStringList() << "-OB" << "RDP=0";
const QStringList command_run_dut           =  QStringList() << "-c" << "SWD" << "-run" << "0x08000000";

Programming::Programming(QObject *parent) {}

Programming::~Programming() {}

void Programming::initThread() {
    process_timer = new QTimer();
    connect(process_timer, &QTimer::timeout, this, [&]() {
        if(!command.isEmpty()) {
            switch(command.first()) {
            case read_serial_mcu:
                programm_get_serialNumMcu();
                break;
            case start_programm:
                programm_start_programming();
                break;
            }
            command.removeFirst();
        }
    });
    process_timer->start(100);

    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [&]() {
        process_command_output = QString(process->readAllStandardOutput());
    });

    mcu_serial_num.clear();

    mPathCli = Settings::Instance().getFimrwareStLinkPath();
    mPathLlsFirmware = Settings::Instance().getFirmwareDutPath();
    mPathBootloaderFirmware = Settings::Instance().getFirmwareBootPath();
}

void Programming::startProgramm() {
    command.push_back(start_programm);
}

void Programming::readSerialNumMcu() {
    command.push_back(read_serial_mcu);
}

void Programming::programm_start_programming() {
    /* если настройки корректы */
    if ( (QFile::exists(mPathCli)
          && (QFile::exists(mPathBootloaderFirmware)))
         && (QFile::exists(mPathLlsFirmware)) )
    {
        QString path = mPathCli;
        path.replace("/", "\\");

        int progress = 10;
        emit exeUpdate(progress);

        /* проверить налицие линка */
        if((programm_check_st_link(path)) != true){
            state = programming_error;
            emit exeError(tr("No programmer or remote control board detected"));
            return;
        }
        progress = 30;
        emit exeUpdate(progress);

        /* снять защиту */
        if((programm_set_rom_unprotected(path)) != true){
            emit exeError(tr("Failed to unprotect"));
            state = programming_error;
            return;
        }

        progress = 40;
        emit exeUpdate(progress);

        /* стереть флеш */
        if(programm_clear_flash(path) != true){
            emit exeError(tr("Failed to clear flash"));
            state = programming_error;
            return;
        }
        progress = 50;
        emit exeUpdate(progress);

        /* зашить бутлоадер */
        if(programm_write_bootloader(path) != true){
            emit exeError(tr("Failed to clear flash"));
            state = programming_error;
            return;
        }
        progress = 60;
        emit exeUpdate(progress);

        /* зашить программу */
        if(programm_write_main_programm(path) != true){
            emit exeError(tr("Failed to write bootloader"));
            state = programming_error;
            return;
        }
        progress = 70;
        emit exeUpdate(progress);

        /* выставить защиту */
        if(programm_set_rom_protected(path) != true){
            emit exeError(tr("Failed to record main program"));
            state = programming_error;
            return;
        }
        progress = 100;
        emit exeUpdate(progress);

        state = programming_ok_readp_on;
        emit programmFinished();
    } else {
        emit exeError(tr("Programming failed") + "\r\n" + tr("Please check path"));
        state = programming_error;
        return;
    }
}

QString Programming::writeCmdCommand(QString path_cmd, QStringList command) {
    process_command_output.clear();
    process->start(path_cmd, command);
    process->waitForFinished();
    process->waitForReadyRead();
    return process_command_output;
}

int Programming::programm_dut_reboot(QString path_cmd) {
    QString ret = writeCmdCommand(path_cmd, command_run_dut);
    if(ret.contains("Application started")) {
        return 0;
    }else{
        if(ret.contains("Could not run application while the read out protection is activated")) {
            return 1;
        }else{
            return -1;
        }
    }
}

bool Programming::programm_get_info(QString path_cmd, QString *serial_num_mcu)
{
    QString ret = writeCmdCommand(path_cmd, command_read_mcu_num);
    if(ret.contains("0x1FFFF7AC :") == false){
        return false;
    }
    int i = ret.lastIndexOf("0x1FFFF7AC :");
    QString serial = ret.mid(i + sizeof("0x1FFFF7AC :"));
    serial.replace(" ", "");
    serial.remove("\r");
    serial.remove("\n");
    *serial_num_mcu = serial;
    return true;
}

bool Programming::programm_clear_flash(QString path_cmd){
    QString ret = writeCmdCommand(path_cmd, command_clear_flash);
    return ret.contains(tr("Flash memory erased"));
}

bool Programming::programm_set_rom_protected(QString path_cmd) {
    QString ret = writeCmdCommand(path_cmd, command_protect_rom);
    return ret.contains(tr("Option bytes updated successfully"));
}

bool Programming::programm_set_rom_unprotected(QString path_cmd) {
    QString ret = writeCmdCommand(path_cmd, command_unprotect_rom);
    return ret.contains(tr("Option bytes updated successfully"));
}

bool Programming::programm_check_st_link(QString path_cmd) {
    QString ret = writeCmdCommand(path_cmd, command_connect_SWD);
    return ret.contains("ST-LINK SN");
}

bool Programming::programm_write_bootloader(QString path_cmd){
    QStringList command = command_write_bootloader;
    QString temp_command = command[1];
    QString temp_command_part2 = temp_command.arg(mPathBootloaderFirmware);
    command[1] = temp_command_part2;
    QString ret = writeCmdCommand(path_cmd, command);
    return ret.contains(tr("Programming Complete"));
}

bool Programming::programm_write_main_programm(QString path_cmd){
    QStringList command = command_write_dut;
    QString temp_command = command[1];
    QString temp_command_part2 = temp_command.arg(mPathLlsFirmware);
    command[1] = temp_command_part2;
    QString ret = writeCmdCommand(path_cmd, command);
    return ret.contains(tr("Programming Complete"));
}

QString Programming::programm_get_serialNumMcu() {
    QString serial_num;
    QString path = mPathCli;
    if(programm_get_info(path, &serial_num) == true) {
        if(serial_num != "303030303030303030303030") {
            mcu_serial_num = serial_num;
        }
    }
    emit exeReadSerialNumMcu(mcu_serial_num);
    return mcu_serial_num;
}
