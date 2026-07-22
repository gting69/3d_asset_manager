#include "searchproxymodel.h"

SearchProxyModel::SearchProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
    // Налаштовуємо пошук без урахування регістру (щоб "audi" знаходило "Audi")
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void SearchProxyModel::setSearchQuery(const QString &query) {
    // Встановлюємо ключове слово для фільтрації
    setFilterFixedString(query);
}