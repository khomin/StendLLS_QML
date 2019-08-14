#include "settings.h"
#include <QFile>
#include <QDebug>

Settings::Settings() { getSettings(); }

Settings::~Settings() {}

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
    } else {
        qDebug() << "Cannot read file" << file.errorString();
    }
    return settingsJson;
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
