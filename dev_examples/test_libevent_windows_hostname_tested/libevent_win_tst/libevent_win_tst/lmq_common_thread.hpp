#ifndef _HEADER_FILE_LMQ_COMMON_THREAD_HPP_
#define _HEADER_FILE_LMQ_COMMON_THREAD_HPP_

#ifdef WIN32
#include <windows.h> 
#else
#include <pthread.h>
#endif 

typedef int (*LmqThreadFunc) (int & exit, void * arg);

class LmqThread
{
public:
    LmqThread(LmqThreadFunc thread_func,void * arg):
        _thread_func(thread_func),
#ifdef WIN32
			_handle(NULL),
#else
			_handle(-1),
#endif
			_exit(0),
			_arg(arg)
    {
#ifdef WIN32
        _handle = CreateThread(NULL, 0, thread_run, this, 0, NULL);  
#else
		if(0!=pthread_create(&_handle, NULL, thread_run, this))
            printf("pthread_create error! \n");
#endif 
    }
    
    
    ~LmqThread()
    {
#ifdef WIN32
        WaitForSingleObject(_handle, INFINITE); 
#else
        pthread_join(_handle, NULL);
#endif  
    }
    
    void set_exit()
    {
        _exit =1;
    }
    
    int & get_exit()
    {
        return _exit;
    }
	
    void * get_arg()
    {
        return _arg;
    }

#ifdef WIN32
    static DWORD WINAPI thread_run(LPVOID self)
#else
    static void * thread_run(void * self)
#endif  
    {  
		LmqThread * obj =(LmqThread*)self;
		if(NULL==obj)
		{
			printf("LmqThread::thread_run, NULL==obj \n");
            return 0;
		}
        obj->_thread_func(obj->get_exit(), obj->get_arg());
        return 0;  
    }
    

private:
    LmqThreadFunc _thread_func;
#ifdef WIN32
    HANDLE _handle;
#else
    pthread_t _handle;
#endif 
    int _exit;
	void * _arg;
};

#endif // _HEADER_FILE_LMQ_COMMON_THREAD_HPP_

