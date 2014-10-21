#ifndef __THREADPOOL
#define __THREADPOOL
#include <string>
#include <pthread.h>
#include <list>
using namespace std;

class Task
{
protected:
	int m_socket;
	string Taskname;
public:
	Task(){}
	//~Task(){}
	Task(string taskname)
	{
		this->Taskname=taskname;
		m_socket=-1;
	}
	virtual int Run()=0;
	void SetData(int socket);

};

class Threadpool
{
private:
	std::list<Task*> TaskList;
	int threadnum;
	int max_requests;
	bool m_stop;
	pthread_t *m_threads;
	pthread_mutex_t m_pthreadMutex;
        pthread_cond_t m_pthreadCond;
private:
	static void *worker(void *arg);
	void ThreadFunc();
public:
	Threadpool(int threadnum,int max_requests);
	bool AddTask(Task *task);
	~Threadpool();
};
#endif
