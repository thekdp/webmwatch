#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);

    gb_controls = new QGroupBox;
    tv_webmList = new QTableView;

    hbox->addWidget(gb_controls);
    hbox->addWidget(tv_webmList);

    setDarkTheme();
    setFonts();
    populateControls();

    connect(pb_search, &QPushButton::clicked, this, &Widget::getJson);
}

Widget::~Widget()
{
}

void Widget::populateControls()
{
    QVBoxLayout *vbox_controls = new QVBoxLayout(gb_controls);
    QHBoxLayout *hbox = new QHBoxLayout;


//    vbox_controls->setAlignment(Qt::AlignCenter);
//    hbox->setAlignment(Qt::AlignCenter);

    lbl_greeting    = new QLabel;
    le_linkInput    = new QLineEdit;
    pb_search       = new QPushButton(tr("Search"));

    lbl_greeting->setText(QString(tr("Welcome back. Again.")));
    lbl_greeting->setAlignment(Qt::AlignHCenter);
    lbl_greeting->setFont(QFont("FuturaDemiC", 20));

    hbox->addWidget(le_linkInput, 1);
    hbox->addWidget(pb_search, 0);

    vbox_controls->addSpacing(100);
    vbox_controls->addWidget(lbl_greeting);
    vbox_controls->addSpacing(100);
    vbox_controls->addLayout(hbox);
    vbox_controls->addStretch(1);
}

void Widget::getJson()
{
    QString url = le_linkInput->text();

    if (url.isEmpty()) {
       QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Enter the link to a thread first"));
       msgBox.exec();
    } else {

        if (url.startsWith(DVACH))
            url.prepend("https://");

        if (!url.startsWith(DVACH_FULL) || !url.endsWith(".html")) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Enter a 2ch thread link"));
            msgBox.exec();
        } else {
            url.chop(5);
            url.append(".json");

            jsonDownLoader = new FileDownloader(url, this);
            connect(jsonDownLoader, &FileDownloader::downloaded, this, &Widget::readJson);
        }
    }
}

void Widget::readJson()
{
    QJsonObject jsonThread = QJsonDocument::fromJson(jsonDownLoader->downloadedData()).object();
    QJsonArray postsArray = jsonThread.value("threads").toArray().at(0).toObject().value("posts").toArray();

    for (auto post : postsArray) {
        QJsonArray fileArray = post.toObject().value("files").toArray();
        if (!fileArray.isEmpty()) {
            for (auto file : fileArray) {
                QString fullname = file.toObject().value("fullname").toString();
                if (fullname.endsWith("webm") || fullname.endsWith("mp4")) {
                    Content content;
                    content.fullname = fullname;
                    content.type = fullname.sliced(fullname.lastIndexOf('.'));
                    content.durationSec = file.toObject().value("duration_secs").toInt();
                    content.link = DVACH_FULL + file.toObject().value("path").toString();
                    contentList.append(content);
                }
            }
        }
    }
    webmModel   = new WebmModel;
    webmModel->applyContents(contentList);
    if (tv_webmList->model())
        delete tv_webmList->model();
    tv_webmList->setModel(webmModel);
    connect(tv_webmList->horizontalHeader(), &QHeaderView::sectionResized, this, &Widget::setupTable);

    delete sender();
    setupTable();
}

void Widget::setupTable()
{
    int tableWidth = tv_webmList->verticalHeader()->width();
    for (int i = 0; i < webmModel->columnCount(QModelIndex()); ++i) {
        tableWidth += tv_webmList->columnWidth(i);
    }
    if (tv_webmList->verticalScrollBar()) {
        tableWidth += 17;
    }
    tv_webmList->setFixedWidth(tableWidth);
}

void Widget::resizeTable(int logicalIndex, int oldSize, int newSize)
{

}

void Widget::setDarkTheme()
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

    qApp->setPalette(darkPalette);

}

void Widget::setFonts()
{
//    qApp->setStyleSheet("QLabel         { font: 20pt FuturaDemiC; }"
//                        "QPushButton    { font: 14pt FuturaBookC; }"
//                        "QLineEdit      { font: 14pt FuturaBookC; }"
//                        );
    QFontDatabase::addApplicationFont(":resources/FuturaBookC.ttf");
    QFontDatabase::addApplicationFont(":resources/FuturaDemiC.ttf");
    qApp->setFont(QFont("FuturaBookC", 12));
}

