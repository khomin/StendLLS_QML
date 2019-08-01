#include "logModel.h"
#include <QDebug>

LogModel::LogModel(int maxCountElements, QObject *parent) {
    Q_UNUSED(parent);
    this->m_maxCountElements = maxCountElements;
}

bool LogModel::addItem(QString log, QString color) {
    if(tableItems.size() +1 < m_maxCountElements) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        tableItems << std::make_shared<LogModelItem>(log, color);
        endInsertRows();
        return true;
    }
    return false;
}

void LogModel::removeAll() {
    for(int index = 0; index<tableItems.size(); index++) {
        beginRemoveRows( QModelIndex(), index, index+1);
        endRemoveRows();
    }
    tableItems.clear();
}

int LogModel::rowCount(const QModelIndex & index) const {
    Q_UNUSED(index);
    return tableItems.count();
}

Qt::ItemFlags LogModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant LogModel::data(const QModelIndex &index, int role) const {
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();
    auto item = tableItems.at(index.row());
    if (role == Roles::ModelRoleText) return item->getText();
    if (role == Roles::ModelRoleColor) return item->getColor();
    return QVariant();
}

QHash<int, QByteArray> LogModel::roleNames() const {
    return {
        { Roles::ModelRoleText, "text" },
        { Roles::ModelRoleColor, "color" },
    };
}

