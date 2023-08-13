#ifndef THREADMODEL_H
#define THREADMODEL_H

#include <QAbstractTableModel>
#include "thread.h"

class ThreadModel : public QAbstractTableModel
{
public:
    ThreadModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void applyThreads(QList<Thread> _threads) {threads = _threads;}

private:
    QList<Thread> threads;
};

#endif // THREADMODEL_H
