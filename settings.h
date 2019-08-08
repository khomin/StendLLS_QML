#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlEngine>

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings & Instance() {
        static Settings myInstance;
        return myInstance;
    }

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);
        return new Settings;
    }

    Settings(Settings const &) = delete;
    Settings(Settings &&) = delete;
    Settings& operator=(Settings const &) = delete;
    Settings& operator=(Settings &&) = delete;

    QJsonObject getSettings();
    void saveSettingsToFile();

    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageIsChanged)
    Q_PROPERTY(QString activeStend READ getActiveStend WRITE setActiveStend NOTIFY activeStendIsChanged)

    Q_PROPERTY(QString databaseName READ getDatabaseName WRITE setDatabaseName NOTIFY databaseNameIsChanged)
    Q_PROPERTY(QString databaseHost READ getDatabaseHost WRITE setDatabaseHost NOTIFY databaseHostIsChanged)
    Q_PROPERTY(QString databaseUser READ getDatabaseUser WRITE setDatabaseUser NOTIFY databaseUserIsChanged)
    Q_PROPERTY(QString databasePassword READ getDatabasePassword WRITE setDatabasePassword NOTIFY databasePasswordIsChanged)
    Q_PROPERTY(QString databasePort READ getDatabasePort WRITE setDatabasePort NOTIFY databasePortIsChanged)

    Q_PROPERTY(QString curMax READ getCurMin WRITE setCurMin NOTIFY curMinIsChanged)
    Q_PROPERTY(QString curMin READ getCurMax WRITE setCurMax NOTIFY curMaxIsChanged)

    Q_PROPERTY(QString cap1Min READ getCap1Min WRITE setCap1Min NOTIFY cap1MinIsChanged)
    Q_PROPERTY(QString cap2Min READ getCap2Min WRITE setCap2Min NOTIFY cap2MinIsChanged)
    Q_PROPERTY(QString cap3Min READ getCap3Min WRITE setCap3Min NOTIFY cap3MinIsChanged)
    Q_PROPERTY(QString cap1Max READ getCap1Max WRITE setCap1Max NOTIFY cap1MaxIsChanged)
    Q_PROPERTY(QString cap2Max READ getCap2Max WRITE setCap2Max NOTIFY cap2MaxIsChanged)
    Q_PROPERTY(QString cap3Max READ getCap3Max WRITE setCap3Max NOTIFY cap3MaxIsChanged)
    Q_PROPERTY(QString fimrwareStLinkPath READ getFimrwareStLinkPath WRITE setFimrwareStLinkPath NOTIFY fimrwareStLinkPathIsChanged)
    Q_PROPERTY(QString firmwareDutPath READ getFirmwareDutPath WRITE setFirmwareDutPath NOTIFY firmwareDutPathIsChanged)
    Q_PROPERTY(QString frmwareBootPath READ getFirmwareBootPath WRITE setFirmwareBootPath NOTIFY frmwareBootPathIsChanged)

    QString getLanguage() { return settingsJson.value("language").toString(); }
    void setLanguage(QString value) { settingsJson.insert("language", value); saveSettingsToFile(); languageIsChanged(); }

    QString getActiveStend() { return settingsJson.value("activeStend").toString(); }
    void setActiveStend(QString value) { settingsJson.insert("activeStend", value); saveSettingsToFile(); activeStendIsChanged(); }

    QString getDatabaseName() { return settingsJson.value("databaseName").toString(); }
    void setDatabaseName(QString value) { settingsJson.insert("databaseName", value); saveSettingsToFile(); databaseNameIsChanged(); }

    QString getDatabaseHost() { return settingsJson.value("databaseHost").toString(); }
    void setDatabaseHost(QString value) { settingsJson.insert("databaseHost", value); saveSettingsToFile(); databaseHostIsChanged(); }

    QString getDatabaseUser() { return settingsJson.value("databaseUser").toString(); }
    void setDatabaseUser(QString value) { settingsJson.insert("databaseUser", value); saveSettingsToFile(); databaseUserIsChanged(); }

    QString getDatabasePassword() { return settingsJson.value("databasePassword").toString(); }
    void setDatabasePassword(QString value) { settingsJson.insert("databasePassword", value); saveSettingsToFile(); databasePasswordIsChanged(); }

    QString getDatabasePort() { return settingsJson.value("databasePort").toString(); }
    void setDatabasePort(QString value) { settingsJson.insert("databasePort", value); saveSettingsToFile(); databasePortIsChanged(); }

    QString getCap1Min() { return settingsJson.value("cap1_min").toString(); }
    void setCap1Min(QString value) { settingsJson.insert("cap1_min", value); saveSettingsToFile(); cap1MinIsChanged(); }

    QString getCap2Min() { return settingsJson.value("cap2_min").toString(); }
    void setCap2Min(QString value) { settingsJson.insert("cap2_min", value); saveSettingsToFile(); cap2MinIsChanged(); }

    QString getCap3Min() { return settingsJson.value("cap3_min").toString(); }
    void setCap3Min(QString value) { settingsJson.insert("cap3_min", value); saveSettingsToFile(); cap3MinIsChanged(); }

    QString getCap1Max() { return settingsJson.value("cap1_max").toString(); }
    void setCap1Max(QString value) { settingsJson.insert("cap1_max", value); saveSettingsToFile(); cap1MaxIsChanged(); }

    QString getCap2Max() { return settingsJson.value("cap2_max").toString(); }
    void setCap2Max(QString value) { settingsJson.insert("cap2_max", value); saveSettingsToFile(); cap2MaxIsChanged(); }

    QString getCap3Max() { return settingsJson.value("cap3_max").toString(); }
    void setCap3Max(QString value) { settingsJson.insert("cap3_max", value); saveSettingsToFile(); cap3MaxIsChanged(); }

    QString getCurMin() { return settingsJson.value("cur_min").toString(); }
    void setCurMin(QString value) { settingsJson.insert("cur_min", value); saveSettingsToFile(); }

    QString getCurMax() { return settingsJson.value("cur_max").toString(); }
    void setCurMax(QString value) { settingsJson.insert("cur_max", value); saveSettingsToFile(); }

    QString getFimrwareStLinkPath() { return settingsJson.value("firmare_stlink_path").toString(); }
    void setFimrwareStLinkPath(QString value) { settingsJson.insert("firmare_stlink_path", value); saveSettingsToFile(); fimrwareStLinkPathIsChanged(); }

    QString getFirmwareDutPath() { return settingsJson.value("firmare_dut_path").toString(); }
    void setFirmwareDutPath(QString value) { settingsJson.insert("firmare_dut_path", value); saveSettingsToFile(); firmwareDutPathIsChanged(); }

    QString getFirmwareBootPath() { return settingsJson.value("firmare_bootloader_path").toString(); }
    void setFirmwareBootPath(QString value) { settingsJson.insert("firmare_bootloader_path", value); saveSettingsToFile(); frmwareBootPathIsChanged(); }

protected:
    Settings();
    virtual ~Settings();

signals:
    void languageIsChanged();
    void activeStendIsChanged();
    void databaseNameIsChanged();
    void databaseHostIsChanged();
    void databaseUserIsChanged();
    void databasePasswordIsChanged();
    void databasePortIsChanged();
    void cap1MinIsChanged();
    void cap2MinIsChanged();
    void cap3MinIsChanged();
    void cap1MaxIsChanged();
    void cap2MaxIsChanged();
    void cap3MaxIsChanged();
    void fimrwareStLinkPathIsChanged();
    void firmwareDutPathIsChanged();
    void frmwareBootPathIsChanged();
    void curMinIsChanged();
    void curMaxIsChanged();

private:
    QJsonObject settingsJson;
};

#endif // SETTINGS_H
