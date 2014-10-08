#include "cvsclient.h"
#include "ui_cvsclient.h"


void checkerror(int rtperr)
{
    if (rtperr < 0)
    {
        std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        exit(-1);
    }
}

CvsClient::CvsClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CvsClient)
{
    ui->setupUi(this);
    my_decoder = new mydecoder;
    connect(my_decoder,SIGNAL(toshow()),this,SLOT(show_image()));
    show_buffer=(unsigned char *)malloc(480*640*3*sizeof(uchar)+0x36);
    this->sess = new MyRTPReceiver(show_buffer,my_decoder);
    uint16_t portbase=1234;
    int status;
    int num=800;
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;
    transparams.SetPortbase(portbase);
    sessparams.SetOwnTimestampUnit(1.0/8000.0);
    status = sess->Create(sessparams,&transparams);
    checkerror(status);

    // Wait a number of seconds
    RTPTime::Wait(RTPTime(num,0));

    sess->BYEDestroy(RTPTime(10,0),0,0);
}

CvsClient::~CvsClient()
{
    delete ui;
    delete sess;
}
void CvsClient::show_image()
{
    QPixmap p;
    change_order(show_buffer+0x36);
    memcpy(show_buffer,bmpFileHead,0x36);//bmp信息头数据写入buffer1中
    p.loadFromData(show_buffer,640 * 480 * 3 * sizeof(uchar)+0x36,"BMP");//在label中显示buffer1中的图像
    ui->label->setPixmap(p);
}

void CvsClient::change_order(unsigned char *buffer)
{
    unsigned char RGB_tmp=0;
    unsigned long int i,j;
    for(j=0;j<240;j++)
        for(i=0;i<640*3;i++)
        {
            RGB_tmp=buffer[i+j*640*3];
            buffer[i+j*640*3]=buffer[i+(479-j)*640*3];
            buffer[i+(479-j)*640*3]=RGB_tmp;
        }
}
