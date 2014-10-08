#ifndef MYRTPRECEIVER_H
#define MYRTPRECEIVER_H

#include "rtpsession.h"
#include "rtppacket.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtpsourcedata.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "mydecoder.h"

#define H264 96
class MyRTPReceiver:public RTPSession
{
public:
    MyRTPReceiver(unsigned char*showbuffer,mydecoder *decoder);
    ~MyRTPReceiver();
protected:
    void OnPollThreadStep();
    void ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack);
    void OnRTCPCompoundPacket(RTCPCompoundPacket *pack, const RTPTime &receivetime, const RTPAddress *senderaddress);
private:
    unsigned char *recv_buffer;
    long m_current_size;
    long m_lLength;
    unsigned char *s_buffer;
    mydecoder *my_decoder;
};

#endif // MYRTPRECEIVER_H
