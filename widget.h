#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include "filedownloader.h"
#include "webmmodel.h"
#include "content.h"

#define DVACH "2ch.hk"
#define DVACH_FULL "https://2ch.hk"

class WebmModel;

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);
    ~Widget();

    void getJson();
    void readJson();

private:

    QGroupBox       *gb_controls;     // Групбокс настроек
    QTableView      *tv_webmList;  // Виджет списка видео

    QLabel          *lbl_greeting;
    QLineEdit       *le_linkInput;
    QPushButton     *pb_search;
    FileDownloader  *jsonDownLoader;
    WebmModel       *webmModel;

    QList<Content> contentList;

    void populateControls();
    void setDarkTheme();
    void setFonts();
    void setupTable();
    void resizeTable(int logicalIndex, int oldSize, int newSize);
};
#endif // WIDGET_H
