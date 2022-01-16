#include "webmmodel.h"

WebmModel::WebmModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int WebmModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return contents.size();
}

int WebmModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant WebmModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return QVariant();
        case 1:
            return contents.at(index.row()).fullname;
        case 2:
            return contents.at(index.row()).type;
        case 3:
        {
            quint16 minutes = contents.at(index.row()).durationSec / 60;
            quint16 seconds = contents.at(index.row()).durationSec % 60;
            if (seconds < 10)
                return QString("%1:0%2").arg(minutes).arg(seconds);
            else
                return QString("%1:%2").arg(minutes).arg(seconds);
        }
        }
    }
    return QVariant();
}

QVariant WebmModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QVariant();
            case 1:
                return tr("Name");
            case 2:
                return tr("Type");
            case 3:
                return tr("Duration");
            }
        } else if (orientation == Qt::Vertical) {
            return section;
        }
    }
    return QVariant();
}
