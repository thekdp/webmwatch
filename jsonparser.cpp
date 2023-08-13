#include "jsonparser.h"
#include "webmwatchwidget.h"

JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}

void JsonParser::parseBoards(QJsonDocument jsonDocument)
{
    QJsonArray boardsArray = jsonDocument.array();
    QList<Board> boardList;

    for (auto board : boardsArray) {
        QString name = board.toObject().value("name").toString();
        QString id = board.toObject().value("id").toString();
        QString link = dvachUrlFull + QString("/%1").arg(id);
        QString boardName = QString("/%1/ - %2").arg(id).arg(name);

        Board boardData;
        boardData.name = boardName;
        boardData.link = link;

        boardList.append(boardData);
    }

    emit boardsAreReady(boardList);
}

void JsonParser::parseThreads(QJsonDocument jsonDocument)
{
    QJsonArray threadsArray = jsonDocument.object().value("threads").toArray();
    QList<Thread> threadList;

    for (auto thread : threadsArray) {

        QString boardId = thread.toObject().value("board").toString();
        QString number = thread.toObject().value("num").toString();

        Thread threadData;
        threadData.fullname = thread.toObject().value("subject").toString();
        threadData.files = thread.toObject().value("files_count").toInt();
        threadData.link = dvachUrlFull + QString("/%1/res/%2.html").arg(boardId).arg(number);
        threadData.posts = thread.toObject().value("posts_count").toInt();

        threadList.append(threadData);
    }

    emit threadsAreReady(threadList);
}

void JsonParser::parseContents(QJsonDocument jsonDocument)
{
    QJsonArray postsArray = jsonDocument.object().value("threads").toArray().at(0).toObject().value("posts").toArray();
    QList<Content> contentList;

    for (auto post : postsArray) {
        QJsonArray fileArray = post.toObject().value("files").toArray();

        if (fileArray.isEmpty())
            continue;

        for (auto file : fileArray) {
            QString fullname = file.toObject().value("fullname").toString();

            if (!(fullname.endsWith("webm") || fullname.endsWith("mp4")))
                continue;

            Content content;
            content.fullname = fullname;
            content.type = fullname.sliced(fullname.lastIndexOf('.'));
            content.durationSec = file.toObject().value("duration_secs").toInt();
            content.link = dvachUrlFull + file.toObject().value("path").toString();
            contentList.append(content);

        }

    }
    emit contentsAreReady(contentList);
}
