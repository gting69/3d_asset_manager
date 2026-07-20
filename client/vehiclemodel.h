#ifndef VEHICLEMODEL_H
#define VEHICLEMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>

struct VehicleData {
    int id;
    QString name;
};

class VehicleModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum VehicleRoles { NameRole = Qt::UserRole + 1, IdRole };

    explicit VehicleModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void updateData(const QByteArray &jsonData);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<VehicleData> m_vehicles;
};

#endif