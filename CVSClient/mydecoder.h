#ifndef MYDECODER_H
#define MYDECODER_H

#include <QObject>
#include<iostream>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
using namespace std;
class mydecoder:public QObject
{
    Q_OBJECT
public:
    explicit mydecoder(QObject *parent = 0);
    int init_decoder();
    void start_decoder(unsigned char *buf,int buf_length,unsigned char *outbuffer);
    void end_decoder();
signals:
    void toshow();
private:
    AVCodecContext *my_avcodecontext;
    AVCodec *my_avcodec;
    AVFrame *my_avframe;
    AVFrame *my_avframeRGB;
    AVPacket my_avpacket;
    unsigned char *out_buffer;
    FILE *avfp;
    FILE *outputrgb;
};

#endif // MYDECODER_H
