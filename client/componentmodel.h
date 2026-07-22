#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QList>

struct ComponentData { // Назвемо ComponentData, щоб не конфліктувало з іншими бібліотеками
    int id;
    QString name;
};

class ComponentModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ComponentRoles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    explicit ComponentModel(QObject *parent = nullptr);

    void addComponent(const ComponentData &component);
    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<ComponentData> m_components;
};

#endif // COMPONENTMODEL_H