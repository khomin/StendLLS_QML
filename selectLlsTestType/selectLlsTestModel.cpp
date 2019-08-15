#include "selectLlsTestType/selectLlsTestModel.h"
#include <QDebug>

SelectLlsTestModel::SelectLlsTestModel(QObject *parent) {
    Q_UNUSED(parent);
    this->m_maxCountElements = 100;
}

bool SelectLlsTestModel::addItem(QString typeName, Globals::sTypeDevicesList propertyValidate) {
    if(items.size() +1 < m_maxCountElements) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        items << modelItemProperty(std::make_shared<SelectLlsTestItem>(typeName), propertyValidate);
        endInsertRows();
        return true;
    }
    return false;
}

void SelectLlsTestModel::removeAll() {
    int row = rowCount();
    if(row != 0) {
        beginRemoveRows(QModelIndex(), 0, row-1);
        items.clear();
        endRemoveRows();
    }
}

QVector<SelectLlsTestModel::modelItemProperty> SelectLlsTestModel::getAll() {
    return items;
}

int SelectLlsTestModel::rowCount(const QModelIndex & index) const {
    Q_UNUSED(index);
    return items.count();
}

Qt::ItemFlags SelectLlsTestModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant SelectLlsTestModel::data(const QModelIndex &index, int role) const {
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();
    auto item = items.at(index.row());
    if (role == Roles::ModelRoleText) return item.first->getTypeName();
    return QVariant();
}

QHash<int, QByteArray> SelectLlsTestModel::roleNames() const {
    return {
        { Roles::ModelRoleText, "text" }
    };
}

