#include "threadpool.h"
#include <string>
#include <iostream>

using namespace std;
void Task::SetData(int socket)
{
	m_socket=socket;
}

//pthread_mutex_t Threadpool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;  
//pthread_cond_t Threadpool::m_pthreadCond = PTHREAD_COND_INITIALIZER; 

Threadpool::Threadpool(int threadnum,int max_requests)  
{ 
 this->m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;  
 this->m_pthreadCond = PTHREAD_COND_INITIALIZER;
 this->threadnum = threadnum;
 this->max_requests=max_requests;
 m_threads = new pthread_t[threadnum]; 
 for(int i=0;i<threadnum;i++)
 {
   
   if(pthread_create(m_threads+i,NULL,worker,this)!=0)
   {
	delete [] m_threads;
        exit;	
   }
   cout<<"creat the "<<i<<"thread"<<endl;	
 } 
}  

Threadpool::~Threadpool()  
{  
  delete [] m_threads;
  m_stop=true;
 
}  
 
void *Threadpool::worker(void *arg)
{
  Threadpool *pool=(Threadpool *)arg;
  pool->ThreadFunc();
  return pool;
}
void Threadpool::ThreadFunc()  
{  
 pthread_t tid = pthread_self();
 while(!m_stop)
 {
	pthread_mutex_lock(&m_pthreadMutex);
        if(TaskList.empty())
	{
		//pthread_mutex_unlock(&m_pthreadMutex);
		pthread_cond_wait(&m_pthreadCond,&m_pthreadMutex);
		//continue;
	}
	Task* task = TaskList.front();
	TaskList.pop_front();
	pthread_mutex_unlock(&m_pthreadMutex);
	if(!task)
	{
		continue;
	}
	task->Run();
	cout<<"the current thread is"<<tid<<endl; 
 } 
} 
bool Threadpool::AddTask(Task *task)  
{ 
 pthread_mutex_lock(&m_pthreadMutex);
 if(TaskList.size()>max_requests)
 {
	pthread_mutex_unlock(&m_pthreadMutex); 
	return false;
 }   
 this->TaskList.push_back(task);
 pthread_cond_signal(&m_pthreadCond);
 pthread_mutex_unlock(&m_pthreadMutex);  
 return true;  
}  

  

