#ifndef WEBMTABLEVIEW_H
#define WEBMTABLEVIEW_H

#include <QTableView>

enum class TableMode
{
    Threads,
    Contents
};

class WebmTableView : public QTableView
{
public:
    WebmTableView();
    TableMode getMode();
    void setMode(TableMode newMode);
private:
    TableMode currentMode = TableMode::Threads;
};

#endif // WEBMTABLEVIEW_H
