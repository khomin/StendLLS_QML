#include "settings.h"
#include <QFile>
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent) {
    getSettings();
}

QJsonObject Settings::getSettings() {
    QFile file("./conf/settings.json");
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QJsonDocument document = QJsonDocument::fromJson(in.readAll().toUtf8());
        settingsJson = document.object();

        emit languageIsChanged();
        emit activeStendIsChanged();
        emit databaseNameIsChanged();
        emit databaseHostIsChanged();
        emit databaseUserIsChanged();
        emit databasePasswordIsChanged();
        emit databasePortIsChanged();
    } else {
        qDebug() << "Cannot read file" << file.errorString();
    }
    return settingsJson;
}

void Settings::setStendActiveIp(QString ip) {
    settingsJson.insert("activeStend", ip);

    QFile file("./conf/settings.json");
    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream in(&file);
        QJsonDocument doc(settingsJson);
        in << doc.toJson(QJsonDocument::Compact) << endl;
        getSettings();
    } else {
        qDebug() << "Cannot write file" << file.errorString();
    }
}
