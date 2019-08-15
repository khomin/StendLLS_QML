#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlEngine>
#include <QDebug>

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
        return &Instance();
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

    Q_PROPERTY(QString tempMax READ getTempMax NOTIFY tempMaxChanged)
    Q_PROPERTY(QString tempMin READ getTempMin NOTIFY tempMinChanged)

    Q_PROPERTY(QString cap1Min READ getCap1Min WRITE setCap1Min NOTIFY cap1MinIsChanged)
    Q_PROPERTY(QString cap2Min READ getCap2Min WRITE setCap2Min NOTIFY cap2MinIsChanged)
    Q_PROPERTY(QString cap3Min READ getCap3Min WRITE setCap3Min NOTIFY cap3MinIsChanged)
    Q_PROPERTY(QString cap1Max READ getCap1Max WRITE setCap1Max NOTIFY cap1MaxIsChanged)
    Q_PROPERTY(QString cap2Max READ getCap2Max WRITE setCap2Max NOTIFY cap2MaxIsChanged)
    Q_PROPERTY(QString cap3Max READ getCap3Max WRITE setCap3Max NOTIFY cap3MaxIsChanged)
    Q_PROPERTY(QString fimrwareStLinkPath READ getFimrwareStLinkPath WRITE setFimrwareStLinkPath NOTIFY fimrwareStLinkPathIsChanged)
    Q_PROPERTY(QString firmwareDutPath READ getFirmwareDutPath WRITE setFirmwareDutPath NOTIFY firmwareDutPathIsChanged)
    Q_PROPERTY(QString frmwareBootPath READ getFirmwareBootPath WRITE setFirmwareBootPath NOTIFY frmwareBootPathIsChanged)

    /* scaner */
    Q_PROPERTY(QString scanerPort READ getScanerPort WRITE setScanerPort NOTIFY scanerPortIsChanged)

    QString getLanguage() { return mLanguage; }
    void setLanguage(QString value) {
        mLanguage = value;
        settingsJson.insert("language", value); saveSettingsToFile(); languageIsChanged(); }

    QString getActiveStend() { return mActiveStend; }
    void setActiveStend(QString value) {
        mActiveStend = value;
        settingsJson.insert("activeStend", value); saveSettingsToFile(); activeStendIsChanged(); }

    QString getDatabaseName() { return mDatabaseName; }
    void setDatabaseName(QString value) {
        mDatabaseName = value;
        settingsJson.insert("databaseName", value); saveSettingsToFile(); databaseNameIsChanged(); }

    QString getDatabaseHost() { return mDatabaseHost; }
    void setDatabaseHost(QString value) {
        mDatabaseHost = value;
        settingsJson.insert("databaseHost", value); saveSettingsToFile(); databaseHostIsChanged(); }

    QString getDatabaseUser() { return mDatabaseUser; }
    void setDatabaseUser(QString value) {
        mDatabaseUser = value;
        settingsJson.insert("databaseUser", value); saveSettingsToFile(); databaseUserIsChanged(); }

    QString getDatabasePassword() { return mDatabasePassword; }
    void setDatabasePassword(QString value) {
        mDatabasePassword = value;
        settingsJson.insert("databasePassword", value); saveSettingsToFile(); databasePasswordIsChanged(); }

    QString getDatabasePort() { return mDatabasePort; }
    void setDatabasePort(QString value) {
        mDatabasePort = value;
        settingsJson.insert("databasePort", value); saveSettingsToFile(); databasePortIsChanged(); }

    QString getCap1Min() { return mCap1Min; }
    void setCap1Min(QString value) {
        mCap1Min = value;
        settingsJson.insert("cap1_min", value); saveSettingsToFile(); cap1MinIsChanged(); }

    QString getCap2Min() { return mCap2Min; }
    void setCap2Min(QString value) {
        mCap2Min = value;
        settingsJson.insert("cap2_min", value); saveSettingsToFile(); cap2MinIsChanged(); }

    QString getCap3Min() { return mCap3Min; }
    void setCap3Min(QString value) {
        mCap3Min = value;
        settingsJson.insert("cap3_min", value); saveSettingsToFile(); cap3MinIsChanged(); }

    QString getCap1Max() { return mCap1Max; }
    void setCap1Max(QString value) {
        mCap1Max = value;
        settingsJson.insert("cap1_max", value); saveSettingsToFile(); cap1MaxIsChanged(); }

    QString getCap2Max() { return mCap2Max; }
    void setCap2Max(QString value) {
        mCap2Max = value;
        settingsJson.insert("cap2_max", value); saveSettingsToFile(); cap2MaxIsChanged(); }

    QString getCap3Max() { return mCap3Max; }
    void setCap3Max(QString value) {
        mCap3Max = value;
        settingsJson.insert("cap3_max", value); saveSettingsToFile(); cap3MaxIsChanged(); }

    QString getCurMin() { return mCurMin; }
    void setCurMin(QString value) {
        mCurMin = value;
        settingsJson.insert("cur_min", value); saveSettingsToFile(); }

    QString getCurMax() { return mCurMax; }
    void setCurMax(QString value) {
        mCurMax = value;
        settingsJson.insert("cur_max", value); saveSettingsToFile(); }

    QString getFimrwareStLinkPath() { return mFimrwareStLinkPath; }
    void setFimrwareStLinkPath(QString value) {
        mFimrwareStLinkPath = value;
        settingsJson.insert("firmare_stlink_path", value); saveSettingsToFile(); fimrwareStLinkPathIsChanged(); }

    QString getFirmwareDutPath() { return mFirmwareDutPath; }
    void setFirmwareDutPath(QString value) {
        mFirmwareDutPath = value;
        settingsJson.insert("firmare_dut_path", value); saveSettingsToFile(); firmwareDutPathIsChanged(); }

    QString getFirmwareBootPath() { return mFirmwareBootPath; }
    void setFirmwareBootPath(QString value) {
        mFirmwareBootPath = value;
        settingsJson.insert("firmare_bootloader_path", value); saveSettingsToFile(); frmwareBootPathIsChanged(); }

    QString getScanerPort() { return mScanerPort; }
    void setScanerPort(QString value) {
        mScanerPort = value;
        settingsJson.insert("scanerPort", value); saveSettingsToFile(); scanerPortIsChanged(); }

    float getTempMax() { return mTempMax; }
    float getTempMin() { return mTempMin; }

    float getTempMax() { return settingsJson.value("tempMax").toString().toFloat(); }
    float getTempMin() { return settingsJson.value("tempMin").toString().toFloat(); }

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
    void scanerPortIsChanged();
    void tempMaxChanged();
    void tempMinChanged();

private:
    void loadValuesFromFileJson();
    void emitAllSignals();

    QString mLanguage;
    QString mActiveStend;
    QString mDatabaseName;
    QString mDatabaseHost;
    QString mDatabaseUser;
    QString mDatabasePassword;
    QString mDatabasePort;
    QString mCap1Min;
    QString mCap2Min;
    QString mCap3Min;
    QString mCap1Max;
    QString mCap2Max;
    QString mCap3Max;
    QString mCurMin;
    QString mCurMax;
    QString mFimrwareStLinkPath;
    QString mFirmwareDutPath;
    QString mFirmwareBootPath;
    QString mScanerPort;
    float mTempMax;
    float mTempMin;

    QJsonObject settingsJson;
};

#endif // SETTINGS_H
