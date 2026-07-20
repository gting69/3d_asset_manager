#include "versionmodel.h"
#include <QJsonDocument>

VersionModel::VersionModel(QObject *parent) : QAbstractListModel(parent) {}

int VersionModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_versions.size();
}

QVariant VersionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    const auto &ver = m_versions[index.row()];
    if (role == NameRole) return ver.versionName;
    if (role == IdRole) return ver.id;
    return QVariant();
}

QHash<int, QByteArray> VersionModel::roleNames() const {
    return { {NameRole, "name"}, {IdRole, "id"} };
}

void VersionModel::updateData(const QByteArray &jsonData) {
    beginResetModel();
    m_versions.clear();
    QJsonArray array = QJsonDocument::fromJson(jsonData).array();
    for (const auto &val : array) {
        QJsonObject obj = val.toObject();
        m_versions.append({obj["id"].toInt(), obj["version_name"].toString()});
    }
    endResetModel();
}