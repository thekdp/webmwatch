#ifndef WEBMWATCHWIDGET_H
#define WEBMWATCHWIDGET_H

#include <QtWidgets>

#include "filedownloader.h"
#include "jsonparser.h"
#include "proxymodel.h"
#include "webmtableview.h"
#include "webmmodel.h"
#include "threadmodel.h"

#include "content.h"
#include "thread.h"
#include "board.h"

const QString dvachUrl = "2ch.hk";
const QString dvachUrlFull = "https://2ch.hk";
const QString boardsUrl = "https://2ch.hk/api/mobile/v2/boards";

class WebmModel;

class WebmWatchWidget : public QMainWindow
{
    Q_OBJECT

public:

    WebmWatchWidget(QWidget *parent = nullptr);
    ~WebmWatchWidget();

private:

    //==========WIDGETS================
    QGroupBox       *gb_search = nullptr;
    QGroupBox       *gb_table = nullptr;

    WebmTableView   *tv_searchResults = nullptr;
    QLineEdit       *le_filter = nullptr;
    QComboBox       *cb_boards = nullptr;
    QPushButton     *pb_return = nullptr;

    QLabel          *lbl_greeting = nullptr;
    QLineEdit       *le_linkInput = nullptr;
    QPushButton     *pb_search = nullptr;

    //==========UTILITY================
    FileDownloader  *jsonDownLoader = nullptr;
    WebmModel       *webmModel = nullptr;
    ThreadModel     *threadModel = nullptr;
    ProxyModel      *proxyModel = nullptr;

    QMap<quint32, Board> boardMap;


    void getContentsJson();
    void getThreadsJson(QString boardUrl);
    void getBoardsJson();
    void readJson(JsonTargetData target);

    void populateSearch();
    void populateResults();
    void setDarkTheme();
    void setFonts();
    void setupTable();
    void resizeTable(int logicalIndex, int oldSize, int newSize);
    void accessContent(const QModelIndex &index);

    void applyNewContents(QList<Content> contentList);
    void applyNewBoards(QList<Board> boardList);
    void applyNewThreads(QList<Thread> threadList);

    void clearModels();

signals:
    void boardsApplied();
    void contentsApplied();
    void threadsApplied();
};
#endif // WEBMWATCHWIDGET_H
