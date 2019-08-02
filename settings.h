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
    void setStendActiveIp(QString ip);

    Q_PROPERTY(QString language READ getLanguage NOTIFY languageIsChanged)
    Q_PROPERTY(QString activeStend READ getActiveStend NOTIFY activeStendIsChanged)
    Q_PROPERTY(QString databaseName READ getDatabaseName NOTIFY databaseNameIsChanged)
    Q_PROPERTY(QString databaseHost READ getDatabaseHost NOTIFY databaseHostIsChanged)
    Q_PROPERTY(QString databaseUser READ getDatabaseUser NOTIFY databaseUserIsChanged)
    Q_PROPERTY(QString databasePassword READ getDatabasePassword NOTIFY databasePasswordIsChanged)
    Q_PROPERTY(QString databasePort READ getDatabasePort NOTIFY databasePortIsChanged)

    QString getLanguage() { return settingsJson.value("language").toString(); }
    QString getActiveStend() { return settingsJson.value("activeStend").toString(); }
    QString getDatabaseName() { return settingsJson.value("databaseName").toString(); }
    QString getDatabaseHost() { return settingsJson.value("databaseHost").toString(); }
    QString getDatabaseUser() { return settingsJson.value("databaseUser").toString(); }
    QString getDatabasePassword() { return settingsJson.value("databasePassword").toString(); }
    QString getDatabasePort() { return settingsJson.value("databasePort").toString(); }
    QString getCap1Min() { return settingsJson.value("cap1_min").toString(); }
    QString getCap2Min() { return settingsJson.value("cap2_min").toString(); }
    QString getCap3Min() { return settingsJson.value("cap3_min").toString(); }
    QString getCap1Max() { return settingsJson.value("cap1_max").toString(); }
    QString getCap2Max() { return settingsJson.value("cap2_max").toString(); }
    QString getCap3Max() { return settingsJson.value("cap3_max").toString(); }
    QString getCurMin() { return settingsJson.value("cur_min").toString(); }
    QString getCurMax() { return settingsJson.value("cur_max").toString(); }
    QString getFimrwareStLinkPath() { return settingsJson.value("firmare_stlink_path").toString(); }
    QString getFirmwareDutPath() { return settingsJson.value("firmare_dut_path").toString(); }
    QString getFirmwareBootPath() { return settingsJson.value("firmare_bootloader_path").toString(); }

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

private:
    QJsonObject settingsJson;
};

#endif // SETTINGS_H
