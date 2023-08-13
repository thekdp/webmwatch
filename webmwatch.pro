QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filedownloader.cpp \
    jsonparser.cpp \
    main.cpp \
    proxymodel.cpp \
    threadmodel.cpp \
    webmmodel.cpp \
    webmtableview.cpp \
    webmwatchwidget.cpp

HEADERS += \
    board.h \
    content.h \
    filedownloader.h \
    jsonparser.h \
    proxymodel.h \
    thread.h \
    threadmodel.h \
    webmmodel.h \
    webmtableview.h \
    webmwatchwidget.h

RESOURCES += res.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
