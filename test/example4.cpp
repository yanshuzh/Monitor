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
#define H264 96
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
public:
	long m_lLength;
	long m_current_size;
	unsigned char *recv_buffer;
	FILE *fp;
	
	
};
MyRTPSession::MyRTPSession()
{
	m_lLength=0;	

	m_current_size=0;
	recv_buffer=(unsigned char *)malloc(640*480*3*sizeof(unsigned char));
	fp=fopen("recv.h264","wa+");
	
}
MyRTPSession::~MyRTPSession()
{

	free(recv_buffer);
	fclose(fp);
	
}
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


void MyRTPSession::ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack)
{
    // You can inspect the packet and the source's info here
    //std::cout<<"Packet type: "<<rtppack.GetPayloadType()<<std::endl;
    //std::cout<<"Packet says: "<<(char *)rtppack.GetPayloadData()<<std::endl;
    //test RTCP packet
    /*int status = this->SendRTCPAPPPacket(0,(uint8_t*)&("123"),(void*)&("hel"),4);
    checkerror(status);*/

    if(rtppack.GetPayloadType() == H264)
    {
        //std::cout<<"Got H264 packet：êo " << rtppack.GetExtendedSequenceNumber() << " from SSRC " << srcdat.GetSSRC() <<std::endl;
        if(rtppack.HasMarker())//如果是最后一包则进行组包
        {
            m_lLength = m_current_size + rtppack.GetPayloadLength();//得到数据包总的长度
            //memcpy(m_pVideoData->m_pBuffer,recv_buffer,m_current_size);
            memcpy(recv_buffer + m_current_size ,rtppack.GetPayloadData(),rtppack.GetPayloadLength());
            std::cout << "Got packet the length is :"<< m_lLength<< " from SSRC " << std::endl;
	   fwrite(recv_buffer,m_lLength,1,fp);
//////////////m_ReceiveArray.Add(m_pVideoData);//添加到接收队列
           // my_decoder->start_decoder();
            memset(recv_buffer,0,m_current_size);//清空缓存，为下次做准备
            m_current_size = 0;
        }
        else//放入缓冲区，在此必须确保有序
        {
            unsigned char* p = rtppack.GetPayloadData();
            memcpy(recv_buffer + m_current_size,rtppack.GetPayloadData(),rtppack.GetPayloadLength());
            m_current_size += rtppack.GetPayloadLength();
        }
    }

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
//    unsigned char *data_buffer = (unsigned char *)malloc(640*480*3*sizeof(unsigned char));
//    sess.recv_buffer=data_buffer;
//    sess.m_lLength = 0;
//    sess.m_current_size = 0;
        // First, we'll ask for the necessary information
		
	std::cout << "Enter local portbase:" << std::endl;
	std::cin >> portbase;
	std::cout << std::endl;
	
	std::cout << std::endl;
	std::cout << "Number of seconds you wish to wait:" << std::endl;
	std::cin >> num;
	
	// Now, we'll create a RTP session, set the destination
	// and poll for incoming data.
	
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	
	// IMPORTANT: The local timestamp unit MUST be set, otherwise
	//            RTCP Sender Report info will be calculated wrong
	// In this case, we'll be just use 8000 samples per second.
	sessparams.SetOwnTimestampUnit(1.0/8000.0);		
	
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

