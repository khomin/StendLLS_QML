#ifndef FINDSTEND_MODEL_H
#define FINDSTEND_MODEL_H

#include <QObject>
#include <QHash>
#include <QAbstractTableModel>
#include "findStendItem.h"
#include <memory>
#include <QVariant>
#include "globals.h"

class FindStendModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FindStendModel(QObject *parent = nullptr);

    enum Roles {
        ModelRoleIp = Qt::UserRole + 1,
        ModelRoleMac,
    };

    bool addItem(Globals::sConnectSettings value);

    void removeAll();

    QVector<std::shared_ptr<FindStendItem>> getAll();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QHash<int,QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QVector<std::shared_ptr<FindStendItem>> tableItems;
    QHash<int, QByteArray> roleNameMapping;
    int m_maxCountElements;
};

#endif // FINDSTEND_MODEL_H
