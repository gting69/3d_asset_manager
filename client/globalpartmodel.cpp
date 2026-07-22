#include "globalpartmodel.h"

GlobalPartModel::GlobalPartModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void GlobalPartModel::addGlobalPart(const GlobalPart &part) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_parts << part;
    endInsertRows();
}

void GlobalPartModel::clear() {
    beginResetModel();
    m_parts.clear();
    endResetModel();
}

int GlobalPartModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_parts.count();
}

QVariant GlobalPartModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_parts.count())
        return QVariant();

    const GlobalPart &part = m_parts[index.row()];

    if (role == IdRole)
        return part.id;
    else if (role == NameRole)
        return part.name;

    return QVariant();
}

QHash<int, QByteArray> GlobalPartModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    return roles;
}