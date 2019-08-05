#include "findStendModel.h"
#include <QDebug>

FindStendModel::FindStendModel(int maxCountElements, QObject *parent) {
    Q_UNUSED(parent);
    this->m_maxCountElements = maxCountElements;
}

bool FindStendModel::addItem(sConnectSettings value) {
    if(tableItems.size() +1 < m_maxCountElements) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        tableItems << std::make_shared<FindStendItem>(value);
        endInsertRows();
        return true;
    }
    return false;
}

void FindStendModel::removeAll() {
    int row = rowCount();
    if(row != 0) {
        beginRemoveRows(QModelIndex(), 0, row-1);
        endRemoveRows();
        tableItems.clear();
    }
}

QVector<std::shared_ptr<FindStendItem>> FindStendModel::getAll() {
    return tableItems;
}

int FindStendModel::rowCount(const QModelIndex & index) const {
    Q_UNUSED(index);
    return tableItems.count();
}

Qt::ItemFlags FindStendModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant FindStendModel::data(const QModelIndex &index, int role) const {
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();
    auto item = tableItems.at(index.row());
    if (role == Roles::ModelRoleIp) return item->getIp();
    if (role == Roles::ModelRoleMac) return item->getMac();
    return QVariant();
}

QHash<int, QByteArray> FindStendModel::roleNames() const {
    return {
        { Roles::ModelRoleIp, "ip" },
        { Roles::ModelRoleMac, "mac" },
    };
}

