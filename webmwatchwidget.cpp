#include "webmwatchwidget.h"
#include "jsonparser.h"
#include "proxymodel.h"

WebmWatchWidget::WebmWatchWidget(QWidget *parent)
    : QMainWindow(parent)
{

    gb_search = new QGroupBox;
    gb_table = new QGroupBox;

    auto dw_webmList = new QDockWidget;
    auto dw_search = new QDockWidget;

    dw_webmList->setWidget(gb_table);
    dw_search->setWidget(gb_search);

    addDockWidget(Qt::LeftDockWidgetArea, dw_webmList, Qt::Vertical);
    addDockWidget(Qt::TopDockWidgetArea, dw_search, Qt::Horizontal);

    setCentralWidget(new QWidget());

    setDarkTheme();
    setFonts();
    populateSearch();
    populateResults();

    getBoardsJson();

    connect(tv_searchResults, &QTableView::doubleClicked, this, [=](const QModelIndex &index)
    {
        accessContent(index);
    });
}

WebmWatchWidget::~WebmWatchWidget()
{
}

void WebmWatchWidget::populateSearch()
{
    auto vbox_main = new QVBoxLayout(gb_search);
    auto hbox = new QHBoxLayout;


//    vbox_controls->setAlignment(Qt::AlignCenter);
//    hbox->setAlignment(Qt::AlignCenter);

    lbl_greeting    = new QLabel;
    le_linkInput    = new QLineEdit;
    pb_search       = new QPushButton(tr("Search"));

    lbl_greeting->setText(QString(tr("Welcome back. Again.")));
    lbl_greeting->setAlignment(Qt::AlignHCenter);
    lbl_greeting->setFont(QFont("FuturaDemiC", 20));

    le_linkInput->setPlaceholderText(tr("Thread URL"));

    hbox->addWidget(le_linkInput, 1);
    hbox->addWidget(pb_search, 0);
    vbox_main->addWidget(lbl_greeting);
    vbox_main->addLayout(hbox);

    connect(pb_search, &QPushButton::clicked, this, &WebmWatchWidget::getContentsJson);
}

void WebmWatchWidget::populateResults()
{
    auto vbox_main = new QVBoxLayout(gb_table);
    auto hbox = new QHBoxLayout;

    le_filter = new QLineEdit;
    cb_boards = new QComboBox;
    pb_return = new QPushButton(tr("Back to boards"));
    tv_searchResults = new WebmTableView;

    pb_return->setHidden(true);

    hbox->addWidget(cb_boards, 0);
    hbox->addWidget(pb_return, 1);

    vbox_main->addLayout(hbox, 0);
    vbox_main->addWidget(le_filter, 0);
    vbox_main->addWidget(tv_searchResults, 1);

    connect(cb_boards, &QComboBox::currentIndexChanged, [=](quint32 index)
    {
        QString boardUrl = boardMap.value(index).link;
        getThreadsJson(boardUrl);
    });
    connect(pb_return, &QPushButton::clicked, [=]()
    {
         QString boardUrl = boardMap.value(cb_boards->currentIndex()).link;
         getThreadsJson(boardUrl);
    });
    connect(le_filter, &QLineEdit::textChanged, [=](const QString input)
    {
        if (!proxyModel)
            return;

        proxyModel->setUserRequest(input);
    });

    cb_boards->setCurrentIndex(0);
    le_filter->setPlaceholderText(tr("Search"));
}

void WebmWatchWidget::getBoardsJson()
{
    jsonDownLoader = new FileDownloader(boardsUrl, this);
    connect(jsonDownLoader, &FileDownloader::downloaded, [=]()
    {
        readJson(JsonTargetData::Boards);
    });
}

void WebmWatchWidget::getContentsJson()
{
    QString url = le_linkInput->text();

    if (url.isEmpty()) {
       QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Enter the link to a thread first"));
       msgBox.exec();
    } else {

        if (url.startsWith(dvachUrl))
            url.prepend("https://");

        if (!url.startsWith(dvachUrlFull) || !url.endsWith(".html")) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Enter a 2ch thread link"));
            msgBox.exec();
        } else {
            url.chop(5);
            url.append(".json");

            jsonDownLoader = new FileDownloader(url, this);
            connect(jsonDownLoader, &FileDownloader::downloaded, [=]()
            {
                readJson(JsonTargetData::Contents);
            });
        }
    }
}

void WebmWatchWidget::getThreadsJson(QString boardUrl)
{
    boardUrl.append("/catalog.json");

    jsonDownLoader = new FileDownloader(boardUrl, this);
    connect(jsonDownLoader, &FileDownloader::downloaded, [=]()
    {
        readJson(JsonTargetData::Threads);
    });
}

void WebmWatchWidget::readJson(JsonTargetData target)
{
    auto parser = new JsonParser;
    auto parserThread = new QThread;

    parser->moveToThread(parserThread);

    connect(parserThread, &QThread::started, [=]()
    {
        auto document = QJsonDocument::fromJson(jsonDownLoader->downloadedData());
        switch (target) {
        case JsonTargetData::Boards:
            parser->parseBoards(document);
            break;
        case JsonTargetData::Contents:
            parser->parseContents(document);
            break;
        case JsonTargetData::Threads:
            parser->parseThreads(document);
            break;
        }
    });

    connect(parser,         &JsonParser::contentsAreReady,      this,           &WebmWatchWidget::applyNewContents);
    connect(parser,         &JsonParser::threadsAreReady,       this,           &WebmWatchWidget::applyNewThreads);
    connect(parser,         &JsonParser::boardsAreReady,        this,           &WebmWatchWidget::applyNewBoards);
    connect(this,           &WebmWatchWidget::contentsApplied,  jsonDownLoader, &QObject::deleteLater);
    connect(this,           &WebmWatchWidget::contentsApplied,  parserThread,   &QThread::quit);
    connect(this,           &WebmWatchWidget::contentsApplied,  parser,         &QThread::deleteLater);
    connect(parserThread,   &QThread::finished,                 parserThread,   &QThread::deleteLater);


    parserThread->start();
}

void WebmWatchWidget::setupTable()
{
    qDebug() << "Setup table";
    int tableWidth = tv_searchResults->verticalHeader()->width();
    auto model = tv_searchResults->model();
    if (!model) {
        return;
    }
    for (int i = 0; i < model->columnCount(QModelIndex()); ++i) {
        tableWidth += tv_searchResults->columnWidth(i);
    }
    if (tv_searchResults->verticalScrollBar()) {
        tableWidth += 17;
    }
    tv_searchResults->setFixedWidth(tableWidth);
}

void WebmWatchWidget::resizeTable(int logicalIndex, int oldSize, int newSize)
{

}

void WebmWatchWidget::accessContent(const QModelIndex &index)
{

}

void WebmWatchWidget::applyNewContents(QList<Content> contentList)
{
    clearModels();

    webmModel = new WebmModel;
    webmModel->applyContents(contentList);

    proxyModel = new ProxyModel;
    proxyModel->setSourceModel(webmModel);
    proxyModel->setDynamicSortFilter(true);

    tv_searchResults->setModel(proxyModel);
    tv_searchResults->setMode(TableMode::Contents);
    pb_return->setHidden(false);
    cb_boards->setHidden(true);

    connect(tv_searchResults->horizontalHeader(), &QHeaderView::sectionResized, this, &WebmWatchWidget::setupTable);
    setupTable();

    emit contentsApplied();
}

void WebmWatchWidget::applyNewBoards(QList<Board> boardList)
{
    cb_boards->clear();
    boardMap.clear();

    for (quint32 i = 0; i < boardList.size(); i++) {
        boardMap.insert(i, boardList[i]);
        cb_boards->addItem(boardList[i].name);
    }

    emit boardsApplied();
}

void WebmWatchWidget::applyNewThreads(QList<Thread> threadList)
{
    clearModels();

    threadModel = new ThreadModel;
    threadModel->applyThreads(threadList);

    proxyModel = new ProxyModel;
    proxyModel->setSourceModel(threadModel);
    proxyModel->setDynamicSortFilter(true);

    tv_searchResults->setModel(proxyModel);
    tv_searchResults->setMode(TableMode::Threads);
    pb_return->setHidden(true);
    cb_boards->setHidden(false);

    connect(tv_searchResults->horizontalHeader(), &QHeaderView::sectionResized, this, &WebmWatchWidget::setupTable);
    setupTable();

    emit threadsApplied();
}

void WebmWatchWidget::clearModels()
{
    if (!tv_searchResults->model()) {
        return;
    }
    if (threadModel) {
        delete threadModel;
        threadModel = nullptr;
    }
    if (webmModel) {
        delete webmModel;
        webmModel = nullptr;
    }
    if (proxyModel) {
        delete proxyModel;
        proxyModel = nullptr;
    }
}

void WebmWatchWidget::setDarkTheme()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::PlaceholderText, QColor(70, 70, 70));

    qApp->setPalette(darkPalette);
}

void WebmWatchWidget::setFonts()
{
    QFontDatabase::addApplicationFont(":resources/FuturaBookC.ttf");
    QFontDatabase::addApplicationFont(":resources/FuturaDemiC.ttf");
    qApp->setFont(QFont("FuturaBookC", 12));
}

