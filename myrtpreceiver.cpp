#include "myrtpreceiver.h"


void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

/*
MyRTPRecvSession::MyRTPRecvSession(unsigned char * recv2out_buffer,long *length)
{
	this->m_lLength=length;	
	i_flag = false;
	m_current_size=0;
	recv_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));
	this->recv2out_buffer = recv2out_buffer;
	out_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2*sizeof(unsigned char));	
	fp=fopen("recv.h264","wa+");
	fpyuv=fopen("recv.yuv","wa+");
	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);
	my_sdl = new MySDL(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT,out_buffer);
	my_decoder->init_decoder();
	my_sdl->InitSDL();
	
}*/
MyRTPRecvSession::MyRTPRecvSession()
{
/*	i_flag = false;
	m_current_size=0;
	recv_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));
	this->recv2out_buffer = recv2out_buffer;
	out_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2*sizeof(unsigned char));	
	fp=fopen("recv.h264","wa+");
	fpyuv=fopen("recv.yuv","wa+");
	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);
	my_sdl = new MySDL(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT,out_buffer);
	my_decoder->init_decoder();
	my_sdl->InitSDL();*/
	
}
MyRTPRecvSession::~MyRTPRecvSession()
{
	my_decoder->end_decoder();
	delete my_decoder;
	delete my_sdl;
	free(recv_buffer);
	fclose(fp);
	
}
/*
void MyRTPRecvSession::OnPollThreadStep()
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
void MyRTPRecvSession::ReceiveData()
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


void MyRTPRecvSession::ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack)
{

    if(rtppack.GetPayloadType() == H264)
    {
        if(rtppack.HasMarker())//如果是最后一包则进行组包
        {
            (*m_lLength) = m_current_size + rtppack.GetPayloadLength();//得到数据包总的长度
            //memcpy(m_pVideoData->m_pBuffer,recv_buffer,m_current_size);
            memcpy(recv_buffer + m_current_size ,rtppack.GetPayloadData(),rtppack.GetPayloadLength());
            std::cout << "Got packet the length is :"<< *m_lLength<< " from SSRC " << std::endl;
	   fwrite(recv_buffer,(*m_lLength),1,fp);

	   //pthread_mutex_lock(&rs_pthreadMutex);
	   memcpy(recv2out_buffer,recv_buffer,(*m_lLength));
	   //pthread_mutex_unlock(&rs_pthreadMutex);

	   if(i_flag==false)
	   {
		int ret=findheader(recv_buffer,(*m_lLength));
            	if(ret<(*m_lLength))
            	{
               		my_decoder->start_decoder(recv_buffer,(*m_lLength),out_buffer);
                	i_flag=true;
            	}
           }
	   else
	   {
		my_decoder->start_decoder(recv_buffer,(*m_lLength),out_buffer);
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
int MyRTPRecvSession::findheader(unsigned char *databuf,int len)
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





