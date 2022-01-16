#ifndef WEBMMODEL_H
#define WEBMMODEL_H

#include <QAbstractTableModel>
#include <widget.h>
#include "content.h"

class WebmModel : public QAbstractTableModel
{
public:
    WebmModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void applyContents(QList<Content> _contents) {contents = _contents;}


private:

    QList<Content> contents;
};

#endif // WEBMMODEL_H
