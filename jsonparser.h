#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include "thread.h"
#include "board.h"
#include "content.h"

enum class JsonTargetData
{
    Boards,
    Threads,
    Contents
};

class JsonParser : public QObject
{
    Q_OBJECT
public:
    JsonParser();
    virtual ~JsonParser();

    void parseBoards(QJsonDocument jsonDocument);
    void parseThreads(QJsonDocument jsonDocument);
    void parseContents(QJsonDocument jsonDocument);

signals:
    void boardsAreReady(QList<Board> boards);
    void threadsAreReady(QList<Thread> threads);
    void contentsAreReady(QList<Content> contents);
};

#endif // JSONPARSER_H
