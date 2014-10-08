#ifndef MYRTPSENDER_H
#define MYRTPSENDER_H

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
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#define MAX_RTP_PKT_LENGTH 1360
#define H264                96



class MyRTPSender:public RTPSession
{
public:
	MyRTPSender(uint16_t baseport);
	~MyRTPSender(void);

protected:
public:
	void SendH264(unsigned char*m_h264buf,int buflen);	
	void SetParamsForSendingH264();
	bool CheckError(int rtperr);
	void AddClientAddr(uint32_t destip,uint16_t destport);
	void DeleteClientAddr(uint32_t destip,uint16_t destport);
	int getclientnum();
private:
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	uint32_t destip;
	uint16_t destport;
	uint16_t baseport;
	int status;
	int clientnum;

};

#endif
