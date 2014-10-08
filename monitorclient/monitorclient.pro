#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T00:37:19
#
#-------------------------------------------------

QT       += core gui
QT +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = monitorclient
TEMPLATE = app
INCLUDEPATH +=/usr/include/SDL
INCLUDEPATH +=/home/zhuang/ffmpeg/include
INCLUDEPATH +=/usr/local/2.95.3/jrtplib/include/jrtplib3
INCLUDEPATH +=/usr/local/2.95.3/jthread/include/jthread

LIBS +=-L/usr/local/2.95.3/jthread/lib -ljthread
LIBS +=-L/usr/local/2.95.3/jrtplib/lib -ljrtp
LIBS +=-L/home/zhuang/ffmpeg/lib -lavcodec -lavformat -lavfilter -lswresample -lswscale -lavdevice -lavutil -lSDL

SOURCES += main.cpp\
        monitorclient.cpp \
    mysdl.cpp \
    mydecoder.cpp \
    myrtprecv.cpp \
    myrecvthread.cpp

HEADERS  += monitorclient.h \
    mysdl.h \
    mydecoder.h \
    myrtprecv.h \
    myrecvthread.h

FORMS    += monitorclient.ui
