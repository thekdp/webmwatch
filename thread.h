#ifndef THREAD_H
#define THREAD_H

#include <QtCore>

struct Thread
{
    QString fullname = "none";
    QString link = "none";
    quint32 posts = 0;
    quint32 files = 0;
};

#endif // THREAD_H
