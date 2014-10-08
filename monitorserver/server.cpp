#include <pthread.h>
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <string> 
#include <sys/epoll.h>
#include <iostream>

#include "fcntl.h"
#include <cassert>
#include <unistd.h>
#include "threadpool.h"
#include "myrtpreceiver.h"
#include "myrtpsender.h"
#include "MySQLManager.h"   
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
*/
#define MAX_EVENT_NUMBER 1000 //最大事件数
#define HELLO_WORLD_SERVER_PORT    7766 
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#define MAX_FD 65536 //最大的socket连接数
using namespace std;
struct MessageData{
	char flag;//标识位，1为注册信息，2为登录信息，3为离线信息,4为心跳包。
	char username[10];//用户名
	char userpasswd[20];//密码
	int userport;
};
pthread_mutex_t rs_pthreadMutex;	
MyRTPSender *sender;
unsigned char *recv2out_buffer;
long length=0;//一帧数据的长度
int g_flag=0;//标志是否该更新接受数据


class WorkTask: public Task
{
public:
	WorkTask()
	{
		bzero(buffer, BUFFER_SIZE);
	}
	void initmessage()
	{
		message.flag = 0;
		message.userport = 0;
		memset(message.username,0,10);
		memset(message.userpasswd,0,20);
	}
	int Run()
	{
		int length;
		initmessage();
		cout<<"read the data from socket:"<<endl;
        	while((length = recv(m_socket,buffer,BUFFER_SIZE,0))>0)
		{
			cout<<"read the total data :"<<length<<endl;
			memcpy(&message,buffer,sizeof(message));
			cout<<"read the flag is :"<<message.flag<<endl;
			cout<<"read the massage data is:"<<message.username<<endl;
			cout<<"read the massage data is:"<<message.userpasswd<<endl;
			cout<<"read the massage data is:"<<message.userport<<endl;
		}
		struct sockaddr_in sa;
		int len = sizeof(sa);
		char sql_str[512];
		//获取退出客户端的ip地址
		getpeername(m_socket, (struct sockaddr *)&sa, (socklen_t *)&len);
		//客户端退出处理
		if(length==0){
			//std::string destip_str = "127.0.0.1";
			uint32_t dest_ip = inet_addr(inet_ntoa(sa.sin_addr));
			int ret1 = sprintf(sql_str,
				"SELECT user_port  FROM user_data  WHERE user_ip='%s' and tcp_port=%d", 
				inet_ntoa(sa.sin_addr),
				ntohs(sa.sin_port));
			char* temp_port = mysql->get_user_port(sql_str);//获取视频传输端口
			sender->DeleteClientAddr(dest_ip,atoi(temp_port));//客户端断开连接，将对应的目的视频端口去除
			//设置所退出客户端的状态为离线状态
			ret1 = sprintf(sql_str,
				"UPDATE user_data SET user_status=0 WHERE user_ip='%s' and tcp_port=%d", 
				inet_ntoa(sa.sin_addr),
				ntohs(sa.sin_port));
			int tmp = mysql->runSQLCommand(sql_str);
			close(m_socket);
			cout<<"one client leave!!"<<endl;
		}
		//登录消息包
		if(message.flag==1){
			//获取用户名和密码，查询数据库判断是否为合法用户
			 mysql = new MySQLManager("127.0.0.1", "root", "0715", "userdatabase", (unsigned int)3306);
		         mysql->initConnection();
			 	
			 int ret = sprintf(sql_str,
				"SELECT * FROM  user_data WHERE user_name='%s' and user_passwd='%s'",
				message.username,
				message.userpasswd);
			 cout<<sql_str<<endl;
			 bool temp = mysql->search_user(sql_str);//查找登录用户密码是否正确
                         //mysql->show_result(sql_str); 
			 if(!temp){
				delete mysql;
				cout<<"bad user"<<endl;
				return 1;
			 }
			 cout<<"good user"<<endl;
			//将客户ip地址和视频传输所用端口号存储在数据库中
			 ret = sprintf(sql_str,
				"UPDATE user_data SET user_ip='%s', user_port=%d,tcp_port=%d,user_status=%d WHERE user_name='%s' and user_passwd='%s'",
				inet_ntoa(sa.sin_addr),
				message.userport,
				ntohs(sa.sin_port),
				1,
				message.username,
				message.userpasswd);
			 //cout<<sql_str<<endl;
			 temp = mysql->runSQLCommand(sql_str);	
			//std::string destip_str = "127.0.0.1";
			uint32_t dest_ip = inet_addr(inet_ntoa(sa.sin_addr));
			sender->AddClientAddr(dest_ip ,message.userport);//将客户端ip和视频传输所使用端口号添加到RTP传输目的地址
							
		}
		else if(message.flag==2)
		return 0;
	}
private:
	char buffer[BUFFER_SIZE];
	MessageData message;
	MySQLManager *mysql;
	
};
int setnonblocking( int fd )//将fd设置为非阻塞  
{  
    int old_option = fcntl( fd, F_GETFL );  
    int new_option = old_option | O_NONBLOCK;  
    fcntl( fd, F_SETFL, new_option );  
    return old_option;  
}  
void addfd( int epollfd, int fd, bool one_shot )//将fd添加到事件表epollfd  
{  
    epoll_event event;  
    event.data.fd = fd;  
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;  
    if( one_shot )//采用EPOLLONESHOT事件避免了同一事件被多次触发，因为一个事件只被触发一次且需要重置事件才能侦听下次是否发生  
    {  
        event.events |= EPOLLONESHOT;  
    }  
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );  
    setnonblocking( fd );  
}  
//接收线程，接收ARM板节点发送过来的视频数据
void *recv_thread(void *arg)
{
	//long *m_lLength;
	long m_current_size;
	unsigned char *recv_buffer;
	//unsigned char *recv2out_buffer;
	unsigned char *out_buffer;
	FILE *fp;
	//FILE *fpyuv;
	Mydecoder *my_decoder;
	MySDL *my_sdl;
	bool i_flag;//是否开始解码的标志
	recv_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));
	out_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2*sizeof(unsigned char));	
	fp=fopen("recv.h264","wa+");
	//fpyuv=fopen("recv.yuv","wa+");
	my_decoder = new Mydecoder(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT);
	my_sdl = new MySDL(MY_VIDEO_WIDTH,MY_VIDEO_HEIGHT,out_buffer);
	my_decoder->init_decoder();
	my_sdl->InitSDL();

	cout<<"into recv_thread"<<endl;
//	MyRTPRecvSession sess(recv2out_buffer,&length);
	MyRTPRecvSession sess;
//	MyRTPRecvSession *sess = (MyRTPRecvSession *)arg;
	uint16_t portbase=1234;
	std::string ipstr;
	int status;
	//int num=800000;
		
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	
	sessparams.SetOwnTimestampUnit(1.0/90000.0);		
	
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams,&transparams);	
	checkerror(status);
	g_flag=1;
	while(1);
	while(true)
	{

		//cout<<"into receive thread"<<endl;
	   	//sess.ReceiveData();
		sess.BeginDataAccess();		
		// check incoming packets
		if (sess.GotoFirstSourceWithData())
		{
			do
			{
			RTPPacket *rtppack;
			RTPSourceData *srcdat;
			
			srcdat = sess.GetCurrentSourceInfo();
			
			while (((rtppack = sess.GetNextPacket()) != NULL))
			{
				//pthread_mutex_lock(&rs_pthreadMutex);
				//sess.ProcessRTPPacket(*srcdat,*pack);
					//g_flag=0;
				if(rtppack->GetPayloadType() == H264)
    				{
        				if(rtppack->HasMarker())//如果是最后一包则进行组包
        				{
            				length = m_current_size + rtppack->GetPayloadLength();//得到数据包总的长度
            			//memcpy(m_pVideoData->m_pBuffer,recv_buffer,m_current_size);
            				memcpy(recv_buffer + m_current_size ,rtppack->GetPayloadData(),rtppack->GetPayloadLength());
            				std::cout << "Got packet the length is :"<< length<< " from SSRC " << std::endl;
	   				fwrite(recv_buffer,length,1,fp);

	   				pthread_mutex_lock(&rs_pthreadMutex);
	   				memcpy(recv2out_buffer,recv_buffer,length);
					g_flag=1;
	   				pthread_mutex_unlock(&rs_pthreadMutex);

	   				if(i_flag==false)
	   				{
						int ret=sess.findheader(recv_buffer,length);
            					if(ret<length)
            					{
               						my_decoder->start_decoder(recv_buffer,length,out_buffer);
                					i_flag=true;
            					}
           				}
	  			 	else
	   				{
						my_decoder->start_decoder(recv_buffer,length,out_buffer);
	   					////fwrite(out_buffer,MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3/2,1,fpyuv);
	    					my_sdl->SDLshow();
	   				}

					//m_ReceiveArray.Add(m_pVideoData);//添加到接收队列
            				memset(recv_buffer,0,m_current_size);//清空缓存，为下次做准备
            				m_current_size = 0;
        			}
        			else//放入缓冲区，在此必须确保有序
        			{
            				//unsigned char* p = rtppack.GetPayloadData();
            				memcpy(recv_buffer + m_current_size,rtppack->GetPayloadData(),rtppack->GetPayloadLength());
            				m_current_size += rtppack->GetPayloadLength();
        			}
    			}
			//pthread_mutex_unlock(&rs_pthreadMutex);	
			sess.DeletePacket(rtppack);
					
			}
			} while (sess.GotoNextSourceWithData());
		}	


		sess.EndDataAccess();
		//sleep(1);
	}
	
	// Wait a number of seconds
	//RTPTime::Wait(RTPTime(num,0));
	free(recv_buffer);	
	free(out_buffer);
	delete my_decoder;
	delete my_sdl;
	sess.BYEDestroy(RTPTime(10,0),0,0);
	return 0;
	
}
//发送线程，当有客户连接到服务器时就唤醒该线程，将视频数据转发给客户
void *send_thread(void *arg)
{

	//unsigned char *data_buffer = (unsigned char*)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));
	cout<<"into send_thread!"<<endl;

	while(true)
	{
		//if(sender->getclientnum()==0){
		//	continue;
		//}
	  	pthread_mutex_lock(&rs_pthreadMutex);
		if(g_flag==1)
		{
	   		//memcpy(data_buffer,recv2out_buffer,length);
			cout<<"into send thread"<<endl;
			sender->SendH264(recv2out_buffer,length);
			g_flag=0;
		}
		//g_flag=1;
	   	pthread_mutex_unlock(&rs_pthreadMutex);	
		//sleep(1);
		
	}
	
	return 0;

}
int main(int argc, char **argv)
{
	recv2out_buffer=(unsigned char *)malloc(MY_VIDEO_WIDTH*MY_VIDEO_HEIGHT*3*sizeof(unsigned char));//数据临界区
	rs_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
	sender = new MyRTPSender(2211);
	pthread_t recv_id,send_id;
	int ret = 0;  
      	ret=pthread_create(&recv_id, NULL, recv_thread, NULL);  
    	if(ret!=0){  
        	cout<<"Create receive pthread error!"<<endl;  
        	return 0;  
    	}
      	ret=pthread_create(&send_id, NULL, send_thread, NULL);  
    	if(ret!=0){  
        	cout<<"Create send pthread error!"<<endl;  
        	return 0;  
    	}

	
	Threadpool thread_pool(10,1000);
	WorkTask *work_task = new WorkTask[MAX_FD];//先分配好每个客户连接需要的任务类空间
//设置一个socket地址结构server_addr,代表服务器internet地址, 端口
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
	server_addr.sin_family = AF_INET;
    	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
        struct sockaddr_in client_address;
        socklen_t client_addrlength = sizeof(client_address);
//创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    	int server_socket = socket(PF_INET,SOCK_STREAM,0);
    	if( server_socket < 0){
       		printf("Create Socket Failed!");
       		exit(1);
    	}
	{ 
   		int opt =1;
   		setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	}
//把socket和socket地址结构联系起来
    	if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        	printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT); 
        	exit(1);
    	}
//server_socket用于监听
    	if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE)){
        	printf("Server Listen Failed!"); 
        	exit(1);
    	}
	epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);//创建事件表
	assert(epollfd !=-1);
	addfd( epollfd, server_socket, false );//将监听端口添加到事件表，false表示不注册EPOLLONESHOT事件，注意不能将监听端口注册为EPOLLONESHOT事件因为该事件每次发生只触发一次，而accept每次只能连接一个客户，那么多个客户连接请求到来，则必然丢失客户连接请求
	while(true)
	{
		cout<<"into main thread to listen!"<<endl;
		int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );//无限期等待sockfd上的注册事件
	        if ( ( number < 0 ) && ( errno != EINTR ) )//若epoll_wait不是因中断EINTR是出错  
        	{  
            		printf( "epoll failure\n" );  
            		break;  
        	}
		for(int i=0;i<number;i++)
		{
			int socketfd = events[i].data.fd;//获取就绪事件描述符 
			if(socketfd == server_socket)//表示有新的连接请求
			{
				int connfd = accept( server_socket, ( struct sockaddr* )&client_address, &client_addrlength );//建立客户连接 
				cout<<"test1"<<endl;
				if ( connfd < 0 )  
                		{  
                    			printf( "errno is: %d\n", errno );  
                    			continue;  
               			}
				addfd( epollfd, connfd, false );//将连接的sokcet添加到监听事件表中
				 
			} 
			else if(events[i].events & EPOLLIN)//有客户数据,把可读sokcet放入队列中，选取线程池中的一个线程来处理
			{
				
				work_task[socketfd].SetData(socketfd);				
				thread_pool.AddTask(&work_task[socketfd]);

			}

			
		}  	
	}
    //关闭监听用的socket
    close(server_socket);
    delete  [] work_task;
    return 0;
}
