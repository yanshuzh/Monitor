#ifndef MYCODING_H
#define MYCODING_H
#include <stdint.h>
#include <iostream>
extern "C"
{
#include <x264.h>
}
typedef unsigned char uint8_t;
typedef struct {
    x264_param_t *param;
    x264_t *handle;
    x264_picture_t *picture;
    x264_nal_t *nal;
} Encoder;
class mycoding
{
public:
    mycoding(int width,int height);
     ~mycoding();
    void coding_init();
    int coding_frame(int type,uint8_t *in,uint8_t *out);
    void coding_end();
private:
    Encoder *en;
    int width;
    int height;
//	FILE *test1_fp;
};

#endif // MYCODING_H
