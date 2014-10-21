#ifndef MYRTPRECEIVER_H
#define MYRTPRECEIVER_H

#include "rtpsession.h"
#include "rtppacket.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"

#include <netinet/in.h>
#include <arpa/inet.h>

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
void checkerror(int rtperr);
class MyRTPRecvSession : public RTPSession
{
public:
	//MyRTPRecvSession(unsigned char * recv2out_buffer,long *length);
	MyRTPRecvSession();
	//MyRTPRecvSession(unsigned char * recv2out_buffer);
	~MyRTPRecvSession();
public:
//	void OnPollThreadStep();
	void ReceiveData();
	void ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack);
	int findheader(unsigned char *databuf,int len);
private:
	long *m_lLength;
	long m_current_size;
	unsigned char *recv_buffer;
	unsigned char *recv2out_buffer;
	unsigned char *out_buffer;
	FILE *fp;
	FILE *fpyuv;
	Mydecoder *my_decoder;
	MySDL *my_sdl;
	bool i_flag;//是否开始解码的标志
	
	
};

#endif
