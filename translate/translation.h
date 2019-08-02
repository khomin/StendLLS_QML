#ifndef TRANSLATIONTEST_H
#define TRANSLATIONTEST_H

#include <QtGui>
#include <QQmlApplicationEngine>

class Translation : public QObject
{
    Q_OBJECT
public:
    Translation(QQmlApplicationEngine * engine);
    ~Translation();

    Q_INVOKABLE void qmlReady();

    Q_INVOKABLE void selectLanguage(QString language);

    Q_INVOKABLE QString getCurrentLanguage();

    Q_PROPERTY(QString currentLanguage READ getCurrentLanguage NOTIFY signalLanguageChanged)

    Q_INVOKABLE QStringList getLanguageList();

signals:
    void signalLanguageChanged(int langIndex);

private:
    QTranslator *translator1;
    QTranslator *translator2;
    QTranslator *translator3;
    QQmlApplicationEngine * engine;
    QString currentLanguage;

    static constexpr char const * defaultLanguage = "en";
};

#endif // TRANSLATIONTEST_H
