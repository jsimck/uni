#ifndef __MULTITHREADING_H_
#define __MULTITHREADING_H_

#include <assert.h>
#include <pthread.h>
#include <signal.h>

#define NUMBER_OF_THREADS 2

enum ENUM_THREADS
{
	COMPUTATION_THREAD = 0,
	GUI_THREAD = 1
};

class ThreadObject;

struct MultiThreading{

	int argc;
	char **argv;

	pthread_mutex_t mutex;
	pthread_cond_t condition;
	
	HDC hdc;
	HGLRC contexts[NUMBER_OF_THREADS];				//e.g. Context 0 - Cuda OpenGL - shared context, Context 1 - OpenGL
	
	pthread_t threads[NUMBER_OF_THREADS];
	unsigned long long tids[NUMBER_OF_THREADS];
	pthread_attr_t threadAttributes[NUMBER_OF_THREADS];

	ThreadObject *threadObject;
	
	MultiThreading()
	{
		argc = 0;
		argv = 0;

		mutex = PTHREAD_MUTEX_INITIALIZER;
		condition = PTHREAD_COND_INITIALIZER;
		
		if(pthread_mutex_init(&mutex, NULL)) { printf("Unable to initialize a mutex\n"); return; }
		if(pthread_cond_init(&condition, NULL)) { printf("Unable to initialize a condition\n");return;	}
		
		hdc = 0;

		for (unsigned int i=0; i< NUMBER_OF_THREADS; i++)
		{
			contexts[i] = nullptr;
			threads[i] = pthread_t();
			tids[i] = 0;
			threadAttributes[i] = nullptr;
		}

		threadObject = nullptr;
	}

	~MultiThreading()
	{
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&condition);
	}

	inline void setPthread(ENUM_THREADS id, pthread_t t) 
	{  
		threads[id] = t;
		tids[id] = pthread_getunique_np(threads[id]);
	}

	inline void updateTID(ENUM_THREADS id) 
	{  
		tids[id] = pthread_getunique_np(threads[id]);
	}

	inline void resetPthread(ENUM_THREADS id) 
	{
		threads[id] = pthread_t();
		tids[id] = 0;
		threadAttributes[id] = nullptr;
	}
};


class ThreadObject
{

protected:
	MultiThreading *mt;

public:
	ThreadObject() : mt(0) {}
	virtual ~ThreadObject() {}

public:
	inline void setSharedThreadData(MultiThreading *_mt) { mt = _mt; }
};

#endif