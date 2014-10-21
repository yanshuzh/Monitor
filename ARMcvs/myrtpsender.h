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

bool CheckError(int rtperr);

class MyRTPSender:public RTPSession
{
public:
	MyRTPSender(void);
	~MyRTPSender(void);

protected:
public:
	void SendH264(unsigned char*m_h264buf,int buflen);	
	void SetParamsForSendingH264();

private:
	char sendbuf[1430];

};

#endif
