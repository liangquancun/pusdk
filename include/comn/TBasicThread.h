/*    file: TBasicThread.h
 *    desc:
 *   
 * created: 2009-09-02 09:44:38
 *  author: 
 * version: 1.0
 * company: 
 */

#if !defined TBASICTHREAD__
#define TBASICTHREAD__

////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#include <process.h>
#else //
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/syscall.h>
#endif //WIN32
#include <assert.h>
/////////////////////////////////////////////////////////////////////////////
namespace comn
{

typedef int int32_t;
typedef unsigned int uint32_t;

#ifdef WIN32

class BasicThread
{
public:
    typedef HANDLE ThreadHandle;
    typedef uint32_t ThreadID;
#define INVALID_COUNT	(unsigned int)(-1)
#define	INVALID_HANDLE 	NULL

    BasicThread()
    {
        m_handle = INVALID_HANDLE;
        m_canExit = false;
        m_threadID = 0;
        m_suspendCount = 0;

        initResource();
        createThread( 0 );
    }

    explicit BasicThread( uint32_t stackSize )
    {
        m_handle = INVALID_HANDLE;
        m_canExit = false;
        m_threadID = 0;
        m_suspendCount = 0;

        initResource();
        createThread( stackSize );
    }

    virtual ~BasicThread()
    {
        stop();
    }

    void start()
    {
        doStart();

        resumeThread();
    }

    void stop()
    {
        tryStop();
        wait( -1 );
    }

    void tryStop()
    {
        InterlockedExchange( (LPLONG)&m_canExit,true );
        if ( isHandleValid() )
        {
            while( m_suspendCount > 0 )
            {
                if( resumeThread() == INVALID_COUNT )
                break;
            }
        }

        doStop();
    }

    bool wait( int32_t millisecond = -1 )
    {
        bool ok = true;
        if ( isHandleValid() )
        {
            DWORD dwMilliseconds = ( millisecond >= 0 )?millisecond:INFINITE;
            DWORD res = ::WaitForSingleObject( m_handle, dwMilliseconds );
            ok = ( WAIT_OBJECT_0 == res );
        }
        return ok;
    }

    ThreadHandle getHandle() const
    {
        return m_handle;
    }

    bool isRunning() const
    {
        return ( m_suspendCount == 0 );
    }

    bool isHandleValid() const
    {
        return ( m_handle != INVALID_HANDLE );
    }

    ThreadID getThreadID() const
    {
        return m_threadID;
    }

    int32_t getPriority() const
    {
        assert( isHandleValid() );
        return ::GetThreadPriority( m_handle );
    }

    bool setPriority( int32_t priority )
    {
        assert( isHandleValid() );
        return ( 0 != ::SetThreadPriority( m_handle, priority ) );
    }

    uint32_t resumeThread()
    {
        uint32_t prevCount = INVALID_COUNT;
        if( isHandleValid() )
        {
            prevCount = m_suspendCount;
            if(m_suspendCount > 0)
            {
                if((prevCount = ::ResumeThread(m_handle)) != INVALID_COUNT)
                {
                    InterlockedDecrement( (LPLONG)&m_suspendCount );
                }
            }
        }
        return prevCount;
    }

    uint32_t suspendThread()
    {
        DWORD prevCount = m_suspendCount;
        if((prevCount = ::SuspendThread(m_handle)) != INVALID_COUNT)
        {
            InterlockedIncrement((LPLONG)&m_suspendCount);
        }
        return prevCount;
    }

private:
    /// disable copy and assign
    BasicThread( const BasicThread& ref );
    BasicThread& operator = ( const BasicThread& ref );

protected:
    ThreadHandle m_handle;
    bool m_canExit;
    ThreadID m_threadID;
    uint32_t m_suspendCount;

protected:
    virtual void run() = 0;
    virtual void init()
    {};
    virtual void unInit()
    {};
    virtual void doStop()
    {};
    virtual void doStart()
    {};

    static unsigned WINAPI wrapper( LPVOID param )
    {
        BasicThread* pThread = (BasicThread*)param;
        if ( !pThread->m_canExit )
        {
            pThread->init();
            pThread->run();
            pThread->unInit();
        }
		::CloseHandle(pThread->m_handle);
        pThread->m_handle = INVALID_HANDLE;
        _endthreadex(0);
        return 0;
    }

    bool createThread( uint32_t stackSize )
    {
        uint32_t flag = CREATE_SUSPENDED;
        LPVOID param = (LPVOID)this;
        m_handle = (HANDLE)_beginthreadex( NULL,
                stackSize, wrapper,
                param, flag, &m_threadID );
        m_suspendCount = ( flag | CREATE_SUSPENDED)?1:0;
        return isHandleValid();
    }

    void initResource()
    {
        //pass
    }

};

#else

/// basic thread class
class BasicThread
{
public:
    typedef pthread_t ThreadHandle;
    typedef uint32_t ThreadID;
    const static ThreadHandle INVALID_HANDLE = (ThreadHandle) (-1);

    BasicThread()
    {
        m_handle = INVALID_HANDLE;
        m_canExit = false;
        m_threadID = 0;
        ::pthread_mutex_init(&m_mutexRun, NULL);
        ::pthread_cond_init(&m_condRun, NULL);
        m_status = STATUS_CREATE;
        ::pthread_mutex_init(&m_mutexStatus, NULL);

        initResource();
        createThread(0);
    }

    /// constructor with stack size
    explicit BasicThread(uint32_t stackSize)
    {
        m_handle = INVALID_HANDLE;
        m_canExit = false;
        m_threadID = 0;
        ::pthread_mutex_init(&m_mutexRun, NULL);
        ::pthread_cond_init(&m_condRun, NULL);
        m_status = STATUS_CREATE;
        ::pthread_mutex_init(&m_mutexStatus, NULL);

        initResource();
        createThread(stackSize);
    }

    virtual ~BasicThread()
    {
        stop();
        ::pthread_mutex_destroy(&m_mutexRun);
        ::pthread_cond_destroy(&m_condRun);
        ::pthread_mutex_destroy(&m_mutexStatus);
    }

    /** wait for thread exit
     * @param millisecond if millisecond equals -1, it will wait until thread exit
     * @return return true if thread exit
     */
    bool wait(int32_t millisecond = -1)
    {
        bool ok = true;
        if (isHandleValid())
        {
            if (millisecond <= 0)
            {
                ok = (0 == pthread_join(m_handle, NULL));
				 m_handle = INVALID_HANDLE;
            }
            else
            {
                struct timespec ts;
                getTimeout(&ts, millisecond);

                ::pthread_mutex_lock(&m_mutexRun);
                while (m_status != STATUS_STOP)
                {
                    int ret = ::pthread_cond_timedwait(&m_condRun, &m_mutexRun,
                            &ts);
                    if (ETIMEDOUT == ret)
                    {
                        ok = false;
                        break;
                    }
                }
                ::pthread_mutex_unlock(&m_mutexRun);
            }
        }
        
        return ok;
    }

    /// start thread
    void start()
    {
        doStart();

        changeStatus(STATUS_START);
        ::pthread_mutex_lock(&m_mutexRun);
        ::pthread_cond_signal(&m_condRun);
        ::pthread_mutex_unlock(&m_mutexRun);
    }

    void stop()
    {
        tryStop();
        wait(-1);
    }

    /// stop thread
    void tryStop()
    {
        m_canExit = true;
        /// resume the thread
        if (STATUS_CREATE == m_status)
        {
            changeStatus(STATUS_START);
            ::pthread_mutex_lock(&m_mutexRun);
            ::pthread_cond_signal(&m_condRun);
            ::pthread_mutex_unlock(&m_mutexRun);
        }

        doStop();
    }

    /// get thread handle
    ThreadHandle getHandle() const
    {
        return m_handle;
    }

    /// return true if thread is running
    bool isRunning() const
    {
        return (STATUS_START == m_status) && isHandleValid();
    }

    /// return true if the thread handle is valid
    bool isHandleValid() const
    {
        return (m_handle != INVALID_HANDLE);
    }

    /// get thread ID
    ThreadID getThreadID() const
    {
        return m_threadID;
    }

    /** send signal to thread
     * @return 0 if succeeded
     */
    int kill(int sig)
    {
        int ret = 0;
        if (isHandleValid())
        {
            ret = pthread_kill(m_handle, sig);
        }
        return ret;
    }

    static void getTimeout(struct timespec *spec, int timer)
    {
        struct timeval current;
        gettimeofday(&current, NULL);
        TIMEVAL_TO_TIMESPEC( &current, spec );
        spec->tv_sec = current.tv_sec + ((timer + current.tv_usec / 1000)
                / 1000);
        spec->tv_nsec = ((current.tv_usec / 1000 + timer) % 1000) * 1000000;
    }

private:
    BasicThread(const BasicThread& ref);
    BasicThread& operator =(const BasicThread& ref);

protected:
    ThreadHandle m_handle;
    bool m_canExit;
    ThreadID m_threadID;

    pthread_mutex_t m_mutexRun;
    pthread_cond_t m_condRun;
    int32_t m_status;
    pthread_mutex_t m_mutexStatus;

    enum ThreadStatus
    {
        STATUS_CREATE = 1, STATUS_START = 2, STATUS_STOP = 3
    };
protected:
    virtual void run() = 0;
    virtual void init()
    {
    }
    ;
    virtual void unInit()
    {
    }
    ;
    virtual void doStop()
    {
    }
    ;
    virtual void doStart()
    {
    }
    ;

    bool createThread(uint32_t stackSize)
    {
        pthread_attr_t attr;
        ::pthread_attr_init(&attr);
        pthread_attr_t* pAttr = NULL;
        if (0 != stackSize)
        {
            ::pthread_attr_setstacksize(&attr, stackSize);
            pAttr = &attr;
        }
        int ret = ::pthread_create(&m_handle, pAttr, wrapper, (void*) this);
        assert( ret == 0 );
        ::pthread_attr_destroy(&attr);
        return (0 == ret);
    }

    static void* wrapper(void* param)
    {
        BasicThread* pThread = (BasicThread*) param;
        pThread->m_threadID = syscall(SYS_gettid);
        pThread->waitForRun();
        if (!pThread->m_canExit)
        {
            pThread->init();
            pThread->run();
            pThread->unInit();
        }
       
        pThread->changeStatus(STATUS_STOP);
        pthread_exit(NULL);
        return NULL;
    }

    void initResource()
    {
        //pass
    }

    void waitForRun()
    {
        ::pthread_mutex_lock(&m_mutexRun);
        while (m_status != STATUS_START)
        {
            ::pthread_cond_wait(&m_condRun, &m_mutexRun);
        }
        ::pthread_mutex_unlock(&m_mutexRun);
    }

    void changeStatus(int32_t status)
    {
        ::pthread_mutex_lock(&m_mutexStatus);
        m_status = status;
        ::pthread_mutex_unlock(&m_mutexStatus);
    }

};

#endif //WIN32

} // end of namespace
////////////////////////////////////////////////////////////////////////////
#endif //TBASICTHREAD__
