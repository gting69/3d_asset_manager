#include "componentmodel.h"

ComponentModel::ComponentModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void ComponentModel::addComponent(const ComponentData &component) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_components << component;
    endInsertRows();
}

void ComponentModel::clear() {
    beginResetModel();
    m_components.clear();
    endResetModel();
}

int ComponentModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_components.count();
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_components.count())
        return QVariant();

    const ComponentData &component = m_components[index.row()];

    if (role == IdRole)
        return component.id;
    else if (role == NameRole)
        return component.name;

    return QVariant();
}

QHash<int, QByteArray> ComponentModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    return roles;
}