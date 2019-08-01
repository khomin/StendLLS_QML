#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QObject>
#include "QColor"
#include <QPair>

class LogModelItem : public QObject
{
    Q_OBJECT
public:
    explicit LogModelItem(QString log, QString color) {
        this->logText = log;
        this->logColor = color;
    }

    Q_PROPERTY(QString logText READ getText NOTIFY signalTextIsChanged)
    Q_PROPERTY(QColor logColor READ getColor NOTIFY signalColorIsChanged)

    QString getText() { return logText; }
    QColor getColor() { return logColor; }

signals:
    void signalTextIsChanged();
    void signalColorIsChanged();

private:
    QString logText;
    QColor logColor;
};

#endif // TABLEITEM_H
