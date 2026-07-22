#include "searchproxymodel.h"

SearchProxyModel::SearchProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void SearchProxyModel::setSearchQuery(const QString &query) {
    setFilterFixedString(query);
}