#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
public:
    ProxyModel(QObject *parent = nullptr);
    void setUserRequest(QString _request);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString request = "";
};

#endif // PROXYMODEL_H
