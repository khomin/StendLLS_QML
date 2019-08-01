#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QHash>
#include <QAbstractTableModel>
#include "logModelItem.h"
#include <memory>
#include <QVariant>

class LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LogModel(int maxCountElements, QObject *parent = nullptr);

    enum Roles {
        ModelRoleText = Qt::UserRole + 1,
        ModelRoleColor,
    };

    bool addItem(QString log, QString color);

    void removeAll();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QHash<int,QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QVector<std::shared_ptr<LogModelItem>> tableItems;
    QHash<int, QByteArray> roleNameMapping;
    int m_maxCountElements;
};

#endif // TABLEMODEL_H
