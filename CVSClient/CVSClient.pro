#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T23:35:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH+=/usr/local/2.95.3/jthread/include/jthread
INCLUDEPATH+=/usr/local/2.95.3/jrtplib/include/jrtplib3
LIBS+=-L/usr/local/2.95.3/jthread/lib -ljthread -L/usr/local/2.95.3/jrtplib/lib -ljrtp
INCLUDEPATH +=/home/zhuang/ffmpeg/include
#(两种加库的方法都行)

#LIBS += -L/home/zhuang/lib/libavcodec.a\
#        /home/zhuang/lib/libavdevice.a\
#        /home/zhuang/lib/libavformat.a\
#        /home/zhuang/lib/libavfilter.a\
#        /home/zhuang/lib/libavutil.a\
#        /home/zhuang/lib/libswscale.a\
#        /home/zhuang/lib/libswresample.a
LIBS += -L/home/zhuang/ffmpeg/lib/ -lavcodec -lavformat -lavfilter -lswresample -lswscale -lavdevice -lavutil

TARGET = CVSClient
TEMPLATE = app


SOURCES += main.cpp\
        cvsclient.cpp \
    mydecoder.cpp \
    myrtpreceiver.cpp

HEADERS  += cvsclient.h \
    mydecoder.h \
    myrtpreceiver.h

FORMS    += cvsclient.ui
