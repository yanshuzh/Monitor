#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include <iostream>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<linux/fb.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <arpa/inet.h>
#include "mycoding.h"
using namespace std;
#define CAMERA_DEVICE "/dev/video15"
#define VIDEO_WIDTH 352
#define VIDEO_HEIGHT 288
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV
#define BUFFER_COUNT 4
typedef struct VideoBuffer {
    void   *start; //视频缓冲区的起始地址
    size_t  length;//缓冲区的长度
} VideoBuffer;
class VideoCapture
{
public:
    VideoCapture();
    int open_device();
    void get_capability();
    void get_format();
    int set_format();
    int set_streamparm(unsigned int Frames);
    void request_buf();
    void query_map_qbuf();
    void yuyv2rgb(unsigned char * buf_data);
    int read_aframe(unsigned char * data_buffer);
    int code_frame(unsigned char *yuv_frame, size_t yuv_length,unsigned char *h264_buf);
    int read_and_code(unsigned char *data_buffer);
    int requeen();
    int  startcapture();
    void stopcapture();
private:
    int fd;
    struct v4l2_capability cap;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers reqbuf;
    struct v4l2_buffer buf;
    VideoBuffer framebuf[BUFFER_COUNT];
    unsigned char *starter;
    mycoding *my_coding;
//	FILE *h264_fp;
//	FILE *yuv_fp;
//	FILE *test_fp;

};

#endif // VIDEOCAPTURE_H
