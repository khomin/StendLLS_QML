#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QClipboard>

class Clipboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text WRITE text NOTIFY textChanged)
public:
    explicit Clipboard(QClipboard*);
    void text(QString);

signals:
    void textChanged();

private:
    QClipboard* clipboard;
};

#endif // CLIPBOARD_H
