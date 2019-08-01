#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>
#include <memory>

class interfacesAbstract : public QObject
{
    Q_OBJECT
public:
    explicit interfacesAbstract(QObject *parent = nullptr);

public slots:
    virtual void initInterface() = 0;
    virtual bool openInterface(QString name, const QString & parameters) = 0;
    virtual bool isOpen() = 0;
    virtual void closeInterface() = 0;
    virtual bool sendData(QByteArray pData) = 0;
    virtual bool readData(QByteArray &pData) = 0;
    virtual QString getType() = 0;
    virtual QString getInterfaceName() = 0;
    virtual QPair<QStringList,QStringList> getInterfaceProperty() = 0;
    virtual QString getAvailableList() = 0;

signals:
    void signalError(QString conTypeName, QString errorMessage);
    void signalOpened();
    void signalClosed();
    void signalReadyReadNewData(const QByteArray & data);
};

#endif // INTERFACES_H
