#include "vehiclemodel.h"
#include <QJsonDocument>

VehicleModel::VehicleModel(QObject *parent) : QAbstractListModel(parent) {}

int VehicleModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_vehicles.size();
}

QVariant VehicleModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    const auto &vehicle = m_vehicles[index.row()];
    if (role == NameRole) return vehicle.name;
    if (role == IdRole) return vehicle.id;
    return QVariant();
}

QHash<int, QByteArray> VehicleModel::roleNames() const {
    return { {NameRole, "name"}, {IdRole, "id"} };
}

void VehicleModel::updateData(const QByteArray &jsonData) {
    beginResetModel();
    m_vehicles.clear();
    QJsonArray array = QJsonDocument::fromJson(jsonData).array();
    for (const auto &val : array) {
        QJsonObject obj = val.toObject();
        m_vehicles.append({obj["id"].toInt(), obj["name"].toString()});
    }
    endResetModel();
}