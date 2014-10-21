#include "myrtpsender.h"

bool CheckError(int rtperr)
{
	if(rtperr<0)
	{
		std::cout<<"ERROR:"<<RTPGetErrorString(rtperr)<<std::endl;
		return false;	
	}
	return true;
}

MyRTPSender::MyRTPSender()
{

}

MyRTPSender::~MyRTPSender()
{

}

void MyRTPSender::SendH264(unsigned char*m_h264buf,int buflen)
{
	unsigned char *pSendbuf;
	pSendbuf = m_h264buf;
	//char sendbuf[1430];
	memset(sendbuf,0,1430);
	int status;
	std::cout<<"send packet length:"<<buflen<<std::endl;
	if(buflen<=MAX_RTP_PKT_LENGTH)
	{
		memcpy(sendbuf,pSendbuf,buflen);
		status = this->SendPacket((void*)sendbuf,buflen);
		CheckError(status);
	}
	else if(buflen>MAX_RTP_PKT_LENGTH)
	{
		this->SetDefaultMark(false);
		int k=0,l=0;
		k = buflen / MAX_RTP_PKT_LENGTH;
		l = buflen % MAX_RTP_PKT_LENGTH;
		int t=0;
		//char nalHeader = pSendbuf[0];
		while(t<k||(t==k && l>0))
		{
			if((0==t)||(t<k &&t!=0))
			{
				memcpy(sendbuf,&pSendbuf[t*MAX_RTP_PKT_LENGTH],MAX_RTP_PKT_LENGTH);
				status = this->SendPacket((void*)sendbuf,MAX_RTP_PKT_LENGTH);
				CheckError(status);
				t++;
			}
			else if((k==t && l>0)||(t==(k-1) &&l==0))
			{
				this->SetDefaultMark(true);
				int iSendLen;
				if(l>0)
				{
					iSendLen = buflen-t*MAX_RTP_PKT_LENGTH;
				}
				else
					iSendLen = MAX_RTP_PKT_LENGTH;
				memcpy(sendbuf,&pSendbuf[t*MAX_RTP_PKT_LENGTH],iSendLen);
				status = this->SendPacket((void*)sendbuf,iSendLen);	
				CheckError(status);
				t++;
			}
		}

	}
}

void MyRTPSender::SetParamsForSendingH264()
{
	this->SetDefaultPayloadType(H264);
	this->SetDefaultMark(true);
	this->SetTimestampUnit(1.0/90000.0);
	this->SetDefaultTimestampIncrement(3600);
}

