#include "settings.h"
#include <QFile>
#include <QDebug>

Settings::Settings() {
    QFile file("./conf/settings.json");
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QJsonDocument document = QJsonDocument::fromJson(in.readAll().toUtf8());
        settingsJson = document.object();
        loadValuesFromFileJson();
    } else {
        qDebug() << "Cannot read file" << file.errorString();
    }
}

Settings::~Settings() {}

void Settings::loadValuesFromFileJson() {
    mLanguage = settingsJson.value("language").toString();
    mActiveStend = settingsJson.value("activeStend").toString();
    mDatabaseName = settingsJson.value("databaseName").toString();
    mDatabaseHost = settingsJson.value("databaseHost").toString();
    mDatabaseUser = settingsJson.value("databaseUser").toString();
    mDatabasePassword = settingsJson.value("databasePassword").toString();
    mDatabasePort = settingsJson.value("databasePort").toString();
    mCap1Min = settingsJson.value("cap1_min").toString();
    mCap2Min = settingsJson.value("cap2_min").toString();
    mCap3Min = settingsJson.value("cap3_min").toString();
    mCap1Max = settingsJson.value("cap1_max").toString();
    mCap2Max = settingsJson.value("cap2_max").toString();
    mCap3Max = settingsJson.value("cap3_max").toString();
    mCurMin = settingsJson.value("cur_min").toString();
    mCurMax = settingsJson.value("cur_max").toString();
    mFimrwareStLinkPath = settingsJson.value("firmare_stlink_path").toString();
    mFirmwareDutPath = settingsJson.value("firmare_dut_path").toString();
    mFirmwareBootPath = settingsJson.value("firmare_bootloader_path").toString();
    mScanerPort = settingsJson.value("scanerPort").toString();
    mTempMax = settingsJson.value("tempMax").toString().toFloat();
    mTempMin = settingsJson.value("tempMin").toString().toFloat();

    emitAllSignals();
}

void Settings::emitAllSignals() {
    emit languageIsChanged();
    emit activeStendIsChanged();
    emit databaseNameIsChanged();
    emit databaseHostIsChanged();
    emit databaseUserIsChanged();
    emit databasePasswordIsChanged();
    emit databasePortIsChanged();
    emit cap1MinIsChanged();
    emit cap2MinIsChanged();
    emit cap3MinIsChanged();
    emit cap1MaxIsChanged();
    emit cap2MaxIsChanged();
    emit cap3MaxIsChanged();
    emit fimrwareStLinkPathIsChanged();
    emit firmwareDutPathIsChanged();
    emit frmwareBootPathIsChanged();
    emit curMinIsChanged();
    emit curMaxIsChanged();
    emit tempMaxChanged();
    emit tempMinChanged();
}

void Settings::saveSettingsToFile() {
    QFile file("./conf/settings.json");
    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream in(&file);
        QJsonDocument doc(settingsJson);
        in << doc.toJson(QJsonDocument::Indented) << endl;
    } else {
        qDebug() << "Cannot write file" << file.errorString();
    }
}
