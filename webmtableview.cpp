#include "webmtableview.h"

WebmTableView::WebmTableView()
{

}

TableMode WebmTableView::getMode()
{
    return currentMode;
}

void WebmTableView::setMode(TableMode newMode)
{
    currentMode = newMode;
}
