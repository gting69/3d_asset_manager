#ifndef SEARCHPROXYMODEL_H
#define SEARCHPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

class SearchProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit SearchProxyModel(QObject *parent = nullptr);

    // Метод, який ми викликатимемо з QML при кожному натисканні клавіші
    Q_INVOKABLE void setSearchQuery(const QString &query);
};

#endif // SEARCHPROXYMODEL_H