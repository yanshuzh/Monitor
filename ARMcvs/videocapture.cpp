#include "videocapture.h"

VideoCapture::VideoCapture()
{
 //   my_coding=new mycoding;
}

/*
在linux下设备都是以文件的形式进行管理的
ioctl是设备驱动程序中对设备的I/O通道进行管理的函数int ioctl(int fd,int cmd,...)?
成功返回0，出错返回-1
其中fd--就是用户程序打开设备使用open函数返回的文件标识符
    cmd--就是用户程序对设备的控制命令，至于后面都省略号，有或没有和cmd的意义相关
*/
int VideoCapture::open_device()
{
    fd = open(CAMERA_DEVICE, O_RDWR, 0);
    if (fd < 0) {
        cout<<"Open "<<CAMERA_DEVICE<<" failed"<<endl;
        return -1;
    }
    return fd;
}

/*
控制命令VIDIOC_QUERYCAP
功能：查询设备驱动的功能;
参数说明：参数类型为V4L2的能力描述类型struct v4l2_capability;
struct v4l2_capability {
        __u8    driver[16];     //i.e. "bttv"            //驱动名称,
        __u8    card[32];       // i.e. "Hauppauge WinTV"         //
        __u8    bus_info[32];   // "PCI:" + pci_name(pci_dev)     //PCI总线信息
        __u32   version;        // should use KERNEL_VERSION()
        __u32   capabilities;   // Device capabilities         //设备能力
        __u32   reserved[4];
};
返回值说明： 执行成功时，函数返回值为 0;
函数执行成功后，struct v4l2_capability 结构体变量中的返回当前视频设备所支持的功能
例如支持视频捕获功能V4L2_CAP_VIDEO_CAPTURE或V4L2_CAP_STREAMING
*/
void VideoCapture::get_capability()
{

    int ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        cout<<"VIDIOC_QUERYCAP failed" <<ret<<endl;
        return;
    }
    // Print capability infomations
    cout<<"------------VIDIOC_QUERYCAP-----------"<<endl;
    cout<<"Capability Informations:"<<endl;
    cout<<"driver:"<<cap.driver<<endl;
    cout<<"card:"<<cap.card<<endl;
    cout<<"bus_info:"<<cap.bus_info<<endl;
    cout<<"version:"<<cap.version<<endl;
    cout<<"capabilities:"<<cap.capabilities<<endl;
    return;
}
/*
控制命令 VIDIOC_ENUM_FMT
功能： 获取当前视频设备支持的视频格式 。
参数说明：参数类型为V4L2的视频格式描述符类型 struct v4l2_fmtdesc
struct v4l2_fmtdesc {
        __u32               index;             // Format number
        enum v4l2_buf_type  type;              // buffer type
        __u32               flags;
        __u8                description[32];   // Description string
        __u32               pixelformat;       // Format fourcc
        __u32               reserved[4];
};
返回值说明： 执行成功时，函数返回值为 0;
*/
void VideoCapture::get_format()
{

    int ret;
    fmtdesc.index=0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret=ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
    while (ret != 0)
    {
        fmtdesc.index++;
        ret=ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
    }
    cout<<"--------VIDIOC_ENUM_FMT---------"<<endl;
    cout<<"get the format what the device support" <<endl;
    //cout"pixelformat ="<<(fmtdesc.pixelformat & 0xFF)<<((fmtdesc.pixelformat >> 8) & 0xFF)<<((fmtdesc.pixelformat >> 16) & 0xFF)<<((fmtdesc.pixelformat >> 24) & 0xFF)<<"description ="<<(fmtdesc.description)<<endl;
    return;
}
/*
控制命令VIDIOC_S_FMT
功能：设置视频设备的视频数据格式，例如设置视频图像数据的长、宽，图像格式JPEG、YUYV格式);
参数说明：参数类型为V4L2的视频数据格式类型struct v4l2_format;
struct v4l2_format {
        enum v4l2_buf_type type;    //数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
        union {
                struct v4l2_pix_format          pix;     // V4L2_BUF_TYPE_VIDEO_CAPTURE
                struct v4l2_window              win;     // V4L2_BUF_TYPE_VIDEO_OVERLAY
                struct v4l2_vbi_format          vbi;     // V4L2_BUF_TYPE_VBI_CAPTURE
                struct v4l2_sliced_vbi_format   sliced;  // V4L2_BUF_TYPE_SLICED_VBI_CAPTURE
                __u8    raw_data[200];                   // user-defined
        } fmt;
};
struct v4l2_pix_format {
        __u32                   width;         // 宽，必须是16的倍数
        __u32                   height;        // 高，必须是16的倍数
        __u32                   pixelformat;   // 视频数据存储类型，例如是YUV4:2:2还是RGB
        enum v4l2_field       field;
        __u32                   bytesperline;
        __u32                   sizeimage;
        enum v4l2_colorspace colorspace;
        __u32                   priv;
};
返回值说明： 执行成功时，函数返回值为 0;
注意：如果该视频设备驱动不支持你所设定的图像格式，视频驱动会重新修改struct v4l2_format结构体变量的值为该视频设备所支持的图像格式，所以在程序设计中，设定完所有的视频格式后，要获取实际的视频格式，要重新读取 struct v4l2_format结构体变量。
使用VIDIOC_G_FMT设置视频设备的视频数据格式，VIDIOC_TRY_FMT验证视频设备的视频数据格式
*/
int VideoCapture::set_format()
{

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = VIDEO_WIDTH;
    fmt.fmt.pix.height      = VIDEO_HEIGHT;
    fmt.fmt.pix.pixelformat = fmtdesc.pixelformat;//V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    int ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
    if (ret < 0) {
        cout<<"VIDIOC_S_FMT failed "<<ret<<endl;
        return ret;
    }
  /*  // 设置视频格式VIDIOC_G_FMT和VIDIOC_S_FMT相同
    ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
    if (ret < 0) {
        printf("VIDIOC_G_FMT failed (%d)\n", ret);
        return ret;
    }*/
    // Print Stream Format
    cout<<"------------VIDIOC_S_FMT---------------"<<endl;
    cout<<"Stream Format Informations:"<<endl;
    cout<<"type:"<<fmt.type<<endl;
    cout<<"width:"<<fmt.fmt.pix.width<<endl;
    cout<<"height:"<<fmt.fmt.pix.height<<endl;
    char fmtstr[8];
    memset(fmtstr, 0, 8);
/*
void *memcpy(void *dest, const void *src, size_t n);
从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
所需头文件include <string.h>
*/
    memcpy(fmtstr, &fmt.fmt.pix.pixelformat, 4);
    cout<<" pixelformat: "<<fmtstr<<endl;
    cout<<" field: "<<fmt.fmt.pix.field<<endl;
    cout<<" bytesperline: "<<fmt.fmt.pix.bytesperline<<endl;
    cout<<" sizeimage: "<<fmt.fmt.pix.sizeimage<<endl;
    cout<<" colorspace: "<<fmt.fmt.pix.colorspace<<endl;
    cout<<" priv: "<<fmt.fmt.pix.priv<<endl;
    cout<<" raw_date: "<<fmt.fmt.raw_data<<endl;
    return ret;
}
/************************设置视频采集的帧率*******************************/
int VideoCapture::set_streamparm(unsigned int Frames)
{

    struct v4l2_streamparm *parm = new struct v4l2_streamparm;
    memset(parm,0,sizeof(struct v4l2_streamparm));
    parm->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm->parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;
    parm->parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    parm->parm.capture.timeperframe.denominator = Frames ;//时间间隔分母
    parm->parm.capture.timeperframe.numerator = 1;//分子
    if(-1 == ioctl(fd,VIDIOC_S_PARM,parm))
    {
        cout<<"Set parm failed "<<endl;
            return 0;
    }
    return 1;
}
/*
控制命令VIDIOC_REQBUFS
功能： 请求V4L2驱动分配视频缓冲区(申请V4L2视频驱动分配内存)，V4L2是视频设备的驱动层，位于内核空间，所以通过VIDIOC_REQBUFS控制命令字申请的内存位于内核空间，应用程序不能直接访问，需要通过调用mmap内存映射函数把内核空间内存映射到用户空间后，应用程序通过访问用户空间地址来访问内核空间。
参数说明：参数类型为V4L2的申请缓冲区数据结构体类型struct v4l2_requestbuffers;
struct v4l2_requestbuffers {
        u32                   count;        //缓存数量,也就是说在缓存队列里保持多少张照片
        enum v4l2_buf_type    type;         //数据流类型,必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
        enum v4l2_memory      memory;       //V4L2_MEMORY_MMAP或V4L2_MEMORY_USERPTR
        u32                   reserved[2];
};
返回值说明： 执行成功时，函数返回值为 0，V4L2驱动层分配好了视频缓冲区;
注意：VIDIOC_REQBUFS会修改tV4L2_reqbuf的count值，tV4L2_reqbuf的count值返回实际申请成功的视频缓冲区数目;
*/
void VideoCapture::request_buf()
{
    reqbuf.count = BUFFER_COUNT;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    int ret = ioctl(fd , VIDIOC_REQBUFS, &reqbuf);
    if(ret < 0) {
        cout<<"VIDIOC_REQBUFS failed"<<ret<<endl;
        return;
    }
    cout<<"the buffer has been assigned successfully!"<<endl;
    return;
}
/*
控制命令VIDIOC_QUERYBUF
功能：查询已经分配的V4L2的视频缓冲区的相关信息，包括视频缓冲区的使用状态、在内核空间的偏移地址、缓冲区长度等。在应用程序设计中通过调VIDIOC_QUERYBUF来获取内核空间的视频缓冲区信息，然后调用函数mmap把内核空间地址映射到用户空间，这样应用程序才能够访问位于内核空间的视频缓冲区
参数说明：参数类型为V4L2缓冲区数据结构类型struct v4l2_buffer;
struct v4l2_buffer {
        __u32                   index;
        enum v4l2_buf_type      type;
        __u32                   bytesused;
        __u32                   flags;
        enum v4l2_field         field;
        struct timeval          timestamp;
        struct v4l2_timecode    timecode;
        __u32                   sequence;
        ////////// memory location ////////
        enum v4l2_memory        memory;
        union {
                __u32           offset;
                unsigned long   userptr;
        } m;
        __u32                   length;
        __u32                   input;
        __u32                   reserved;
};
返回值说明： 执行成功时，函数返回值为 0;
struct v4l2_buffer结构体变量中保存了指令的缓冲区的相关信息;一般情况下，应用程序中调用VIDIOC_QUERYBUF取得了内核缓冲区信息后，紧接着调用mmap函数把内核空间地址映射到用户空间方便用户空间应用程序的访问
*/
void VideoCapture::query_map_qbuf()
{
    int i,ret;

    for (i = 0; i < reqbuf.count; i++)
    {
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(fd , VIDIOC_QUERYBUF, &buf);//buf取得内存缓冲区的信息
        if(ret < 0) {
            cout<<"VIDIOC_QUERYBUF" <<i<< "failed "<<ret<<endl;
            return;
        }
        // mmap buffer
        framebuf[i].length = buf.length;//framebuf是程序最前面定义的一个结构体类型的数据
        framebuf[i].start = (char *) mmap(0, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (framebuf[i].start == MAP_FAILED) {
            cout<<"mmap"<<i<<"failed:"<<strerror(errno)<<endl;
            return;
        }
/*
控制命令VIDIOC_QBUF
功能：投放一个空的视频缓冲区到视频缓冲区输入队列中
参数说明：参数类型为V4L2缓冲区数据结构类型struct v4l2_buffer;
返回值说明： 执行成功时，函数返回值为 0;
函数执行成功后，指令(指定)的视频缓冲区进入视频输入队列，在启动视频设备拍摄图像时，相应的视频数据被保存到视频输入队列相应的视频缓冲区中
*/
        ret = ioctl(fd , VIDIOC_QBUF, &buf);
        if (ret < 0) {
            cout<<"VIDIOC_QBUF"<<i<<"failed "<<ret<<endl;
            return;
        }
        cout<<"Frame buffer"<<i<< "address= "<<"address="<<(unsigned int)framebuf[i].start<<"length="<<framebuf[i].length<<endl;
    }//空的视频缓冲区都已经在视频缓冲的输入队列中了
    return;
}
void VideoCapture::yuyv2rgb(unsigned char * buf_data)
{


    unsigned char YUYV[4],RGB[6];
    int j,k,i;
    unsigned int location0=0;

    j=0;
    for(i=0;i < framebuf[buf.index].length;i+=4)
    {
        YUYV[0]=starter[i];//Y0
        YUYV[1]=starter[i+1];//U
        YUYV[2]=starter[i+2];//Y1
        YUYV[3]=starter[i+3];//V
        if(YUYV[0]<1)
        {
            RGB[0]=0;
            RGB[1]=0;
            RGB[2]=0;
        }
        else
        {

        RGB[0]=YUYV[0]+1.772*(YUYV[1]-128);//b
            RGB[1]=YUYV[0]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g
            RGB[2]=YUYV[0]+1.402*(YUYV[3]-128);//r
        }
        if(YUYV[2]<0)
        {
            RGB[3]=0;
            RGB[4]=0;
            RGB[5]=0;
        }
        else
        {

            RGB[3]=YUYV[2]+1.772*(YUYV[1]-128);//b
            RGB[4]=YUYV[2]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g
            RGB[5]=YUYV[2]+1.402*(YUYV[3]-128);//r
        }

        for(k=0;k<6;k++)
        {
            if(RGB[k]<0)
                RGB[k]=0;
            if(RGB[k]>255)
                RGB[k]=255;
        }
 //请记住：扫描行在位图文件中是反向存储的！
/*       if(j%(VIDEO_WIDTH*3)==0)//定位存储位置
        {
            location0=(VIDEO_HEIGHT-j/(VIDEO_WIDTH*3))*(VIDEO_WIDTH*3);
        }
        bcopy(RGB,buf_data+location0+(j%(VIDEO_WIDTH*3)),sizeof(RGB));


        j+=6; */
       if(j%(VIDEO_WIDTH*3)==0)//定位存储位置
        {
            location0=(j/(VIDEO_WIDTH*3))*(VIDEO_WIDTH*3);
        }
        bcopy(RGB,buf_data+location0+(j%(VIDEO_WIDTH*3)),sizeof(RGB));
        j+=6;
    }
    return;
}
int VideoCapture::read_aframe(unsigned char * data_buffer)
{
    int ret;
    unsigned char * data_tmp;
    data_tmp=data_buffer;
    ret = ioctl(fd, VIDIOC_DQBUF, &buf);//VIDIOC_DQBUF命令结果, 使从队列删除的缓冲帧信息传给了此buf
    if (ret < 0) {
        cout<<"VIDIOC_DQBUF failed"<<ret<<endl;
        return ret;
    }

    starter=(unsigned char *)framebuf[buf.index].start;
    yuyv2rgb(data_tmp); // 循环读取buf内存段的内容，进行转换
    return 0;
}

int VideoCapture::code_frame(unsigned char *yuv_frame, size_t yuv_length,unsigned char *h264_buf)
{

    int h264_length=0;
//    char str[]="Good";
    h264_length = my_coding->coding_frame(-1, yuv_frame, h264_buf);
    if (h264_length > 0) {
        //写h264文件
//        fwrite(h264_buf, h264_length, 1, h264_fp);
        
//	fwrite(str,sizeof(char),sizeof(str),test_fp);
    }
    
//    fwrite(yuv_frame,yuv_length,1,yuv_fp);
    return h264_length;
}
int VideoCapture::read_and_code(unsigned char *data_buffer)
{
    int ret,length;

    ret = ioctl(fd, VIDIOC_DQBUF, &buf);//VIDIOC_DQBUF命令结果, 使从队列删除的缓冲帧信息传给了此buf
    if (ret < 0) {
        cout<<"VIDIOC_DQBUF failed"<<ret<<endl;
        return ret;
    }
    starter=(unsigned char *)framebuf[buf.index].start;
    length=code_frame(starter,buf.length,data_buffer);
    requeen();
    return length;
}

int VideoCapture::requeen()
{
    int ret;
    ret = ioctl(fd, VIDIOC_QBUF, &buf);
    if (ret < 0) {
        cout<<"VIDIOC_QBUF failed"<<ret<<endl;
        return ret;
    }
    return 0;
}

int VideoCapture::startcapture()
{
    cout<<"into startcapture function"<<endl;
    int ret;
  //  char h264_file_name[100]="zys1.h264";
  //  char yuv_file_name[100]="zys.yuv";
  //  char test_file_name[100]="zys.test";
    fd=open_device();                    // 打开设备
 //   h264_fp=fopen(h264_file_name,"wa+");
  //  yuv_fp=fopen(yuv_file_name,"wa+");
  //  test_fp=fopen(test_file_name,"wa+");
    my_coding =new mycoding(VIDEO_WIDTH,VIDEO_HEIGHT);
 //   my_coding =new mycoding(352,288);
    my_coding->coding_init();
    get_capability();                    // 获取驱动信息struct v4l2_capability cap;
    memset(&fmtdesc,0,sizeof(fmtdesc));  //memset在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快的方法
    get_format();                        //获取当前视频设备支持的视频格式struct v4l2_fmtdesc fmtdesc;
    memset(&fmt, 0, sizeof(fmt));        //将fmt中的前sizeof(fmt)字节用0替换并返回fmt
    set_format();                        // 设置视频格式struct v4l2_format fmt;
    set_streamparm(25);
    request_buf();                       // 请求分配内存struct v4l2_requestbuffers reqbuf;
    query_map_qbuf();                    // 获取空间，并将其映射到用户空间，然后投放到视频输入队列st
    // 开始录制
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret < 0) {
        cout<<"VIDIOC_STREAMON failed"<<ret<<endl;
        return ret;
    }
    return 0;
}

void VideoCapture::stopcapture()
{
    int i=0;
    for (i=0; i< 4; i++)
    {

        munmap(framebuf[i].start, framebuf[i].length);
    }
   // fclose(h264_fp);
  //  fclose(test_fp);
  //  fclose(yuv_fp);
 //   my_coding->coding_end();
    close(fd);
    cout<<"Stop Capture!!"<<endl;

}
