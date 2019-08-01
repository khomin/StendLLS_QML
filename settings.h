#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

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

signals:
    void languageIsChanged();
    void activeStendIsChanged();
    void databaseNameIsChanged();
    void databaseHostIsChanged();
    void databaseUserIsChanged();
    void databasePasswordIsChanged();
    void databasePortIsChanged();

private:
//    QString mLanguage;
//    QString mActiveStend;
//    QString mDatabaseName;
//    QString mDatabaseHost;
//    QString mDatabaseUser;
//    QString mDatabasePassword;
//    QString mDatabasePort;

    QJsonObject settingsJson;

};

#endif // SETTINGS_H
