#include "myrtpsender.h"
#include "mycoding.h"
#include "videocapture.h"
//#include "time.h"
#define SSRC  100
#define DEST_IP_STR "192.168.1.223"
#define DEST_PORT 1234
#define BASE_PORT 2222

void SetRTPParams(MyRTPSender& sess,uint32_t destip,uint16_t destport,uint16_t baseport)
{
	int status;
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	sessparams.SetOwnTimestampUnit(1.0/90000.0);
	sessparams.SetAcceptOwnPackets(true);
//	sessparams.SetUsePredefinedSSRC(true);
//	sessparams.SetPredefinedSSRC(SSRC);

	transparams.SetPortbase(baseport);
	status = sess.Create(sessparams,&transparams);
	CheckError(status);

	destip = ntohl(destip);
	RTPIPv4Address addr(destip,destport);
	status = sess.AddDestination(addr);
	CheckError(status);
}

int main(int argc,char**agrv)
{
	MyRTPSender sender;
	std::string destip_str = DEST_IP_STR;
	uint32_t dest_ip = inet_addr(destip_str.c_str());
	SetRTPParams(sender,dest_ip,DEST_PORT,BASE_PORT);
	sender.SetParamsForSendingH264();

	VideoCapture myvideocapture;
	myvideocapture.startcapture();
	unsigned char *data_buffer = (unsigned char*)malloc(VIDEO_WIDTH*VIDEO_HEIGHT*3*sizeof(unsigned char));
	//int count=0;
	//clock_t start = clock();	
	while(true)
	{
			int frame_length = myvideocapture.read_and_code(data_buffer);
			sender.SendH264(data_buffer,frame_length);
			//RTPTime::Wait(RTPTime(1,0));
			//count++;
	}
	//clock_t end = clock();
	//cout<<"the total time is "<<(end-start)/CLOCKS_PER_SEC<<endl;
	free(data_buffer);
	sender.BYEDestroy(RTPTime(10,0),0,0);
	return 0;

}
