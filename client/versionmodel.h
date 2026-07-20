#ifndef VERSIONMODEL_H
#define VERSIONMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>

struct VersionData {
    int id;
    QString versionName;
};

class VersionModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum VersionRoles { NameRole = Qt::UserRole + 1, IdRole };

    explicit VersionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void updateData(const QByteArray &jsonData);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<VersionData> m_versions;
};

#endif