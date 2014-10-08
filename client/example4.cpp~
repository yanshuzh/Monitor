/*
   This IPv4 example uses the background thread itself to process all packets.
   You can use example one to send data to the session that's created in this
   example.
*/

#include "rtpsession.h"
#include "rtppacket.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include "rtpsourcedata.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "mydecoder.h"
#include "mysdl.h"
#define H264 96
#define MY_VIDEO_WIDTH 352
#define MY_VIDEO_HEIGHT 288
#ifdef RTP_SUPPORT_THREAD

//
// This function checks if there was a RTP error. If so, it displays an error
// message and exists.
//

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

//
// The new class routine
//

class MyRTPSession : public RTPSession
{
public:
	MyRTPSession();
	~MyRTPSession();
protected:
	void OnPollThreadStep();
	void ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack);
	int findheader(unsigned char *databuf,int len);
private:
	long m_lLength;
	long m_current_size;
	unsigned char *recv_buffer;
	unsigned char *out_buffer;
	FILE *fp;
	FILE *fpyuv;
	Mydecoder *my_decoder;
	MySDL *my_sdl;
	bool i_flag;//是否开始解码的标志
	
	
};
MyRTPSession::MyRTPSession()
{
	m_lLength=0;	
	i_flag = false;
	m_current_size=0;
	recv_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));
	out_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2*sizeof(unsigned char));	
	fp=fopen("recv.h264","wa+");
	fpyuv=fopen("recv.yuv","wa+");
//	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);
//	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);

	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);
	my_sdl = new MySDL(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT,out_buffer);
	my_decoder->init_decoder();
	my_sdl->InitSDL();
	
}
MyRTPSession::~MyRTPSession()
{
	my_decoder->end_decoder();
	delete my_decoder;
	delete my_sdl;
	free(recv_buffer);
	fclose(fp);
	
}
/*
void MyRTPSession::OnPollThreadStep()
{
	BeginDataAccess();
		
	// check incoming packets
	if (GotoFirstSourceWithData())
	{
		do
		{
			RTPPacket *pack;
			RTPSourceData *srcdat;
			
			srcdat = GetCurrentSourceInfo();
			
			while ((pack = GetNextPacket()) != NULL)
			{
				ProcessRTPPacket(*srcdat,*pack);
				DeletePacket(pack);
			}
		} while (GotoNextSourceWithData());
	}
		
	EndDataAccess();
}
*/
void MyRTPSession::ReceiveDada()
{
	BeginDataAccess();
		
	// check incoming packets
	if (GotoFirstSourceWithData())
	{
		do
		{
			RTPPacket *pack;
			RTPSourceData *srcdat;
			
			srcdat = GetCurrentSourceInfo();
			
			while ((pack = GetNextPacket()) != NULL)
			{
				ProcessRTPPacket(*srcdat,*pack);
				DeletePacket(pack);
			}
		} while (GotoNextSourceWithData());
	}
		
	EndDataAccess();
}

void MyRTPSession::ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack)
{

    if(rtppack.GetPayloadType() == H264)
    {
        if(rtppack.HasMarker())//如果是最后一包则进行组包
        {
            m_lLength = m_current_size + rtppack.GetPayloadLength();//得到数据包总的长度
            //memcpy(m_pVideoData->m_pBuffer,recv_buffer,m_current_size);
            memcpy(recv_buffer + m_current_size ,rtppack.GetPayloadData(),rtppack.GetPayloadLength());
            std::cout << "Got packet the length is :"<< m_lLength<< " from SSRC " << std::endl;
	   fwrite(recv_buffer,m_lLength,1,fp);
	   if(i_flag==false)
	   {
		int ret=findheader(recv_buffer,m_lLength);
            	if(ret<m_lLength)
            	{
               		my_decoder->start_decoder(recv_buffer,m_lLength,out_buffer);
                	i_flag=true;
            	}
           }
	   else
	   {
		my_decoder->start_decoder(recv_buffer,m_lLength,out_buffer);
	   ////fwrite(out_buffer,MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2,1,fpyuv);
	    	my_sdl->SDLshow();
	   }

//////////////m_ReceiveArray.Add(m_pVideoData);//添加到接收队列
            memset(recv_buffer,0,m_current_size);//清空缓存，为下次做准备
            m_current_size = 0;
        }
        else//放入缓冲区，在此必须确保有序
        {
            //unsigned char* p = rtppack.GetPayloadData();
            memcpy(recv_buffer + m_current_size,rtppack.GetPayloadData(),rtppack.GetPayloadLength());
            m_current_size += rtppack.GetPayloadLength();
        }
    }

}
//如果数据包不是以I帧开头则会解码不成功，用以下函数找到I帧的位置，从那个地方开始解码。
int MyRTPSession::findheader(unsigned char *databuf,int len)
{
    int offset=0;
    for(offset=0;offset<len-4;offset++,databuf++)
    {
        if(databuf[0]==0x00&&databuf[1]==0x00&&databuf[2]==0x00&&databuf[3]==0x01&&databuf[4]==0x67)
        {
            return offset;
        }
        if(databuf[0]==0x00&&databuf[1]==0x00&&databuf[2]==0x00&&databuf[3]==0x01&&databuf[4]==0x65)
        {
            return offset;
        }
        if(databuf[0]==0x00&&databuf[1]==0x00&&databuf[2]==0x00&&databuf[3]==0x01&&databuf[4]==0x68)
        {
            return offset;
        }
    }
    return len;
}
//
// The main routine
// 

int main(void)
{
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
		
	MyRTPSession sess;
	uint16_t portbase;
	std::string ipstr;
	int status,num;
		
	std::cout << "Enter local portbase:" << std::endl;
	std::cin >> portbase;
	std::cout << std::endl;
	
	std::cout << std::endl;
	std::cout << "Number of seconds you wish to wait:" << std::endl;
	std::cin >> num;
	
	
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	
	sessparams.SetOwnTimestampUnit(1.0/90000.0);		
	
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams,&transparams);	
	checkerror(status);
	
	// Wait a number of seconds
	RTPTime::Wait(RTPTime(num,0));

	sess.BYEDestroy(RTPTime(10,0),0,0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 0;
}

#else

int main(void)
{
	std::cerr << "Thread support is required for this example" << std::endl;
	return 0;
}

#endif // RTP_SUPPORT_THREAD

