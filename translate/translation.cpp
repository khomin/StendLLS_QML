#include "translation.h"
#include "storeMisc.h"

Translation::Translation(QQmlApplicationEngine * engine) {
    translator1 = new QTranslator(this);
    translator2 = new QTranslator(this);
    translator3 = new QTranslator(this);
    this->engine = engine;
    currentLanguage = QString(Translation::defaultLanguage);
}

Translation::~Translation() {
    delete translator1;
    delete translator2;
    delete translator3;
    delete settings;
}

void Translation::qmlReady() {
    this->settings = new QSettings("qrc:/conf/language.conf", QSettings::IniFormat);
    if(settings->value("magic_word") == StoreMisc::magic_word) {
        currentLanguage = settings->value("language").toString();
    } else { // default settings
        settings->setValue("language", currentLanguage);
        settings->setValue("magic_word", StoreMisc::magic_word);
    }

    selectLanguage(currentLanguage);
}

void Translation::selectLanguage(QString language) {
    int langIndex = 0;
    if(language == QString("en")) {
        translator1->load("translate/Language_en", ".");
        qApp->installTranslator(translator1);
        langIndex = 0;
    }
    if(language == QString("ru")) {
        translator2->load("translate/Language_ru", ".");
        qApp->installTranslator(translator2);
        langIndex = 1;
    }
    if(language == QString("cn")) {
        translator3->load("translate/Language_cn", ".");
        qApp->installTranslator(translator3);
        langIndex = 2;
    }

    engine->retranslate();

    if(settings != nullptr) {
        settings->setValue("language", language);
    }

    emit signalLanguageChanged(langIndex);
}

QString Translation::getCurrentLanguage() {
    return currentLanguage;
}

QStringList Translation::getLanguageList() {
    QStringList res;
    res.append(tr("English"));
    res.append(tr("Русский"));
    res.append(tr("Chinese"));
    return  res;
}
