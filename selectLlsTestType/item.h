#ifndef SelectLlsTestItem_H
#define SelectLlsTestItem_H

#include <QObject>
#include "QColor"
#include <QPair>
#include "globals.h"

class SelectLlsTestItem : public QObject
{
    Q_OBJECT
public:

    explicit SelectLlsTestItem(QString name) {
        mTypeName = name;
    }

    Q_PROPERTY(QString naem READ getTypeName NOTIFY typeNameIsChanged)

    QString getTypeName() {
        return mTypeName;
    }

signals:
    void typeNameIsChanged();

private:
    QString mTypeName;
};

#endif // SelectLlsTestItem_H
