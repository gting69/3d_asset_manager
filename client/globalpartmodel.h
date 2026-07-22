#ifndef GLOBALPARTMODEL_H
#define GLOBALPARTMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QList>

// Структура для зберігання даних однієї "Глобальної частини"
struct GlobalPart {
    int id;
    QString name;
};

class GlobalPartModel : public QAbstractListModel {
    Q_OBJECT

public:
    // Ролі дозволяють QML розуміти, як називаються змінні (model.id, model.name)
    enum GlobalPartRoles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    explicit GlobalPartModel(QObject *parent = nullptr);

    // Методи для управління даними
    void addGlobalPart(const GlobalPart &part);
    void clear();

    // Обов'язкові методи QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<GlobalPart> m_parts;
};

#endif // GLOBALPARTMODEL_H