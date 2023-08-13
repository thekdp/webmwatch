#include "threadmodel.h"

ThreadModel::ThreadModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ThreadModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return threads.size();
}

int ThreadModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ThreadModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return threads.at(index.row()).fullname;
        case 1:
            return threads.at(index.row()).posts;
        case 2:
            return threads.at(index.row()).files;
        }
    }
    return QVariant();
}

QVariant ThreadModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Posts");
            case 2:
                return tr("Files");
            }
        } else if (orientation == Qt::Vertical) {
            return section;
        }
    }
    return QVariant();
}
