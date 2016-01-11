#ifndef  RWLOCK_H
#define  RWLOCK_H

////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#else //
#include <pthread.h>
#endif //WIN32

/////////////////////////////////////////////////////////////////////////////
namespace comn
{

#ifndef WIN32

class CRWLock
{
public:
    CRWLock()
    {
		create();       
    }

    ~CRWLock()
    {
		destroy();       
    }

	bool create()
	{
		return  pthread_rwlock_init(&m_rwlock,NULL) == 0;
	}

	void destroy()
	{
		 pthread_rwlock_destroy(&m_rwlock);
	}

    bool rlock()
    {
        return pthread_rwlock_rdlock(&m_rwlock) == 0;
    }

	bool tryrlock()
	{
		return pthread_rwlock_tryrdlock(&m_rwlock) == 0;
	}

	bool wlock()
	{
		return pthread_rwlock_wrlock(&m_rwlock) == 0;
	}

	bool trywlock()
	{
		return pthread_rwlock_trywrlock(&m_rwlock) == 0;
	}

    bool unlock()
    {
        return pthread_rwlock_unlock(&m_rwlock) == 0;
    }
   
private:
     pthread_rwlock_t m_rwlock;

};

#else 

#define OS_START_ERROR      20000
#define OS_ERRSPACE_SIZE    50000
#define ERROR_NOT_OWNER     288L
#define OS_START_STATUS    (OS_START_ERROR  + OS_ERRSPACE_SIZE)
#define OS_START_USERERR   (OS_START_STATUS + OS_ERRSPACE_SIZE)
#define OS_START_CANONERR  (OS_START_USERERR+ (OS_ERRSPACE_SIZE * 10))
#define OS_START_EAIERR    (OS_START_CANONERR + OS_ERRSPACE_SIZE)
#define OS_START_SYSERR    (OS_START_EAIERR + OS_ERRSPACE_SIZE)
#define FROM_OS_ERROR(e)   (e == 0 ? 0 : e + OS_START_SYSERR)
#define	get_os_error()     (FROM_OS_ERROR(GetLastError()))



class CRWLock
{
public:
	CRWLock()
	{
		create();       
	}

	~CRWLock()
	{
		destroy();       
	}

	bool create()
	{
		m_readers = 0;
		if ( !(m_readEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 
		{
			return false;
		}
		if (! (m_writeMutex = CreateMutex(NULL, FALSE, NULL)) ) 
		{
			CloseHandle(m_readEvent);
			return false;
		}
		return true;
	}

	void destroy()
	{
		CloseHandle(m_readEvent);	
		CloseHandle(m_writeMutex);			
		m_readers = 0;
	}

	bool rlock()
	{
		return rlockCore(INFINITE);
	}

	bool tryrlock()
	{
		return rlockCore(0);
	}

	bool wlock()
	{
		return wlockCore(INFINITE);
	}

	bool trywlock()
	{
		return wlockCore(0);
	}

	bool unlock()
	{
		int ret = 0;

		/* First, guess that we're unlocking a writer */
		if (! ReleaseMutex(m_writeMutex) )
		{
			ret = get_os_error();
		}	

		if (ret == FROM_OS_ERROR(ERROR_NOT_OWNER)) 
		{
			/* Nope, we must have a read lock */
			if (m_readers &&
				! InterlockedDecrement(&m_readers) &&
				! SetEvent(m_readEvent)) 
			{
					ret = get_os_error();
			}
			else 
			{
				ret = 0;
			}
		}

		return ret == 0;
	}

private:
	bool rlockCore(DWORD  milliseconds)
	{
		DWORD   code = WaitForSingleObject(m_writeMutex, milliseconds);
		if (code == WAIT_FAILED || code == WAIT_TIMEOUT)
		{
			return false;
		}
		/* We've successfully acquired the writer mutex, we can't be locked
		 * for write, so it's OK to add the reader lock.  The writer mutex
		 * doubles as race condition protection for the readers counter.   
		 */
		InterlockedIncrement(&m_readers);
	    
		if (! ResetEvent(m_readEvent) )
		{
			return false;
		}	    
		if (! ReleaseMutex(m_writeMutex) )
		{
			return false;
		}	    
		return true;
	}

	bool wlockCore(DWORD milliseconds)
	{
		DWORD   code = WaitForSingleObject(m_writeMutex, milliseconds);
		if (code == WAIT_FAILED || code == WAIT_TIMEOUT)
		{
			return false;
		}
		
		/* We've got the writer lock but we have to wait for all readers to
		 * unlock before it's ok to use it.
		 */
		if (m_readers) 
		{
			/* Must wait for readers to finish before returning, unless this
			 * is an trywrlock (milliseconds == 0):
			 */
			code = milliseconds? WaitForSingleObject(m_readEvent, milliseconds) : WAIT_TIMEOUT;	        
			if (code == WAIT_FAILED || code == WAIT_TIMEOUT)
			{
				/* Unable to wait for readers to finish, release write lock: */
				ReleaseMutex(m_writeMutex);					            
				return false;
			}
		}
		return true;
	}

private:
	HANDLE      m_writeMutex;
	HANDLE      m_readEvent;
	long        m_readers;
};



#endif 
/////////////////////////////////////////////////////////////////////////////


class AutoRWLock
{
public:
    explicit AutoRWLock(CRWLock& cs,bool isRLock = true) :
        m_locked(false), m_cs(cs)
    {
		if (isRLock)
		{
			m_cs.rlock();
		}
		else
		{
			m_cs.wlock();
		}        
        m_locked = true;
    }

    ~AutoRWLock()
    {
        unlock();
    }

    void unlock()
    {
        if (m_locked)
        {
            m_cs.unlock();
            m_locked = false;
        }
    }

private:
    bool m_locked;
    CRWLock& m_cs;

    AutoRWLock(const AutoRWLock& mtx);
    AutoRWLock& operator =(const AutoRWLock& mtx);

};

} // end of namespace
////////////////////////////////////////////////////////////////////////////
#endif //


