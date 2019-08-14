#ifndef SELECTLLS_TEST__MODEL_H
#define SELECTLLS_TEST__MODEL_H

#include <QObject>
#include <QHash>
#include <QAbstractTableModel>
#include "selectLlsTestType/item.h"
#include <memory>
#include <QVariant>
#include "globals.h"

class SelectLlsTestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SelectLlsTestModel(QObject *parent = nullptr);

    bool addItem(QString typeName, Globals::sTypeDevicesList propertyValidate);
    void removeAll();


    enum Roles { ModelRoleText = Qt::UserRole + 1 };

    typedef QPair<std::shared_ptr<SelectLlsTestItem>, Globals::sTypeDevicesList> modelItemProperty;

    QVector<modelItemProperty> getAll();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QHash<int,QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QVector<modelItemProperty> items;
    QHash<int, QByteArray> roleNameMapping;
    int m_maxCountElements;
};

#endif // SELECTLLS_TEST__MODEL_H
