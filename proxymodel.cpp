#include "proxymodel.h"

ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void ProxyModel::setUserRequest(QString _request)
{
    request = _request;
    invalidateFilter();
}

bool ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (request.isEmpty())
        return true;
    return sourceModel()->index(sourceRow, 0, sourceParent).data(Qt::DisplayRole).toString().contains(request, Qt::CaseInsensitive);

}
