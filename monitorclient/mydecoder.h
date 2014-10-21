#ifndef MYDECODER_H
#define MYDECODER_H

#include<iostream>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
using namespace std;
class Mydecoder
{

public:
    explicit Mydecoder(int width,int height);
    int init_decoder();
    void start_decoder(unsigned char *buf,int buf_length,unsigned char *outbuffer);
    void end_decoder();

private:
    int width;
    int height;
    AVCodecContext *my_avcodecontext;
    AVCodec *my_avcodec;
    AVFrame *my_avframe;
    AVFrame *my_avframeYUV;
    AVPacket my_avpacket;
    unsigned char *out_buffer;
    FILE *avfp;
    FILE *outputrgb;
};

#endif // MYDECODER_H
