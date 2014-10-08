#include "mydecoder.h"

Mydecoder::Mydecoder(int width,int height)
{
    this->width = width;
    this->height = height;
}

int Mydecoder::init_decoder()
{
    avcodec_register_all();
    my_avcodec=avcodec_find_decoder(CODEC_ID_H264);
    my_avcodecontext=avcodec_alloc_context();
    my_avcodecontext->extradata = new uint8_t[32];//给extradata成员参数分配内存
    my_avcodecontext->extradata_size = 32;//extradata成员参数分配内存大小
   //给extradata成员参数设置值
    //00 00 00 01
    my_avcodecontext->extradata[0] = 0x00;
    my_avcodecontext->extradata[1] = 0x00;
    my_avcodecontext->extradata[2] = 0x00;
    my_avcodecontext->extradata[3] = 0x01;
    //67 42 80 1e
    my_avcodecontext->extradata[4] = 0x67;
    my_avcodecontext->extradata[5] = 0x42;
    my_avcodecontext->extradata[6] = 0x80;
    my_avcodecontext->extradata[7] = 0x1e;
    //88 8b 40 50
    my_avcodecontext->extradata[8] = 0x88;
    my_avcodecontext->extradata[9] = 0x8b;
    my_avcodecontext->extradata[10] = 0x40;
    my_avcodecontext->extradata[11] = 0x50;
    //1e d0 80 00
    my_avcodecontext->extradata[12] = 0x1e;
    my_avcodecontext->extradata[13] = 0xd0;
    my_avcodecontext->extradata[14] = 0x80;
    my_avcodecontext->extradata[15] = 0x00;
    //03 84 00 00
    my_avcodecontext->extradata[16] = 0x03;
    my_avcodecontext->extradata[17] = 0x84;
    my_avcodecontext->extradata[18] = 0x00;
    my_avcodecontext->extradata[19] = 0x00;
    //af c8 02 00
    my_avcodecontext->extradata[20] = 0xaf;
    my_avcodecontext->extradata[21] = 0xc8;
    my_avcodecontext->extradata[22] = 0x02;
    my_avcodecontext->extradata[23] = 0x00;
    //00 00 00 01
    my_avcodecontext->extradata[24] = 0x00;
    my_avcodecontext->extradata[25] = 0x00;
    my_avcodecontext->extradata[26] = 0x00;
    my_avcodecontext->extradata[27] = 0x01;
    //68 ce 38 80
    my_avcodecontext->extradata[28] = 0x68;
    my_avcodecontext->extradata[29] = 0xce;
    my_avcodecontext->extradata[30] = 0x38;
    my_avcodecontext->extradata[31] = 0x80;
    if(avcodec_open(my_avcodecontext,my_avcodec))
    {
        cout<<"Error:open H264 decoder err"<<endl;
        return -1;
    }
    my_avframe=avcodec_alloc_frame();
    my_avframeYUV=avcodec_alloc_frame();
    out_buffer=new unsigned char[avpicture_get_size(PIX_FMT_YUV420P,width,height)];
    avpicture_fill((AVPicture *)my_avframeYUV,out_buffer,PIX_FMT_YUV420P,width,height);
    return 1;
}

void Mydecoder::start_decoder(unsigned char *buf,int buf_length,unsigned char *outbuffer)
{

    if(buf_length>0)
    {
        int flag;//判断是否加码成功的标志
        my_avpacket.data=buf;
        my_avpacket.size=buf_length;
        int ret=avcodec_decode_video2(my_avcodecontext,my_avframe,&flag,&my_avpacket);
        if(ret>0 && flag)//解码成功
        {
            struct SwsContext *img_convert_ctx =NULL;
            img_convert_ctx=sws_getContext(my_avcodecontext->width,
                                           my_avcodecontext->height,
                                           my_avcodecontext->pix_fmt,
                                           my_avcodecontext->width,
                                           my_avcodecontext->height,
                                           PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);
            sws_scale(img_convert_ctx,
                      (const unsigned char* const*)my_avframe->data,
                      my_avframe->linesize,0,
                      my_avcodecontext->height,
                      my_avframeYUV->data,my_avframeYUV->linesize);

            memcpy(outbuffer,my_avframeYUV->data[0],height*width);
            memcpy(outbuffer+height*width,my_avframeYUV->data[1],height*width/4);
	    memcpy(outbuffer+height*width*5/4,my_avframeYUV->data[2],height*width/4);
        }
	else if(flag==0) std::cout<<"cmiss a frame!!!!"<<endl;
    }


}
void Mydecoder::end_decoder()
{
    avcodec_close(my_avcodecontext);
    av_free(my_avcodecontext);
    av_free(my_avframe);
    av_free(my_avframeYUV);
}
