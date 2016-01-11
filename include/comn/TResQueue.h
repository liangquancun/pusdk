/********************************************************************
created:	2012/03/13
modify :    2012/03/13
file :      TQueue.h 
author:		lvll
purpose:    Thread Safe FIFO queue
*********************************************************************/
#ifndef TRES_QUEUE_H_
#define TRES_QUEUE_H_

#include "TConditionVariable.h"
#include <deque>


////////////////////////////////////////////////////////////////////////////
namespace comn
{
	template <class A>
	class CQueue
	{
	public:		
		CQueue();	
		~CQueue();

		///压入一个元素
		void push(const A &item);					//for producer
		bool pop(A &item);							//for consumer
		bool popTimeWait(A &item,int millisec);     //for consumer

		size_t  size();     //队列大小
		bool    empty();    ///判为空
	
	private:		
		std::deque<A> m_queue;    ///stl deque
		
		CriticalSection   m_cs ;   ///线程锁
		ConditionVariable m_cond;  ///条件变量
	};


	template <class A>
	CQueue<A>::CQueue()
	{
		AutoCritSec  tmpLock(m_cs);	
		m_queue.clear();
	}

	template <class A>
	CQueue<A>::~CQueue()
	{
		AutoCritSec  tmpLock(m_cs);	
		m_queue.clear();		
	}

	template <class A>
	void CQueue<A>::push(const A &item)
	{
		{
			AutoCritSec  tmpLock(m_cs);				
			m_queue.push_back(item);	///数据放入队列
		}		
		
		//唤醒一个线程
		m_cond.signal();    
		return ;
	}	


	template <class A>
	size_t CQueue<A>::size()
	{
		AutoCritSec  tmpLock(m_cs);
		return m_queue.size();	
	}

	template <class A>
	bool CQueue<A>::empty()
	{	
		AutoCritSec  tmpLock(m_cs);
		return m_queue.empty();
	}

	template <class A>
	bool CQueue<A>::pop(A &item)
	{	
		AutoCritSec  tmpLock(m_cs);

		while (m_queue.empty()) 
		{	
			//线程睡眠解锁，等待唤醒
			m_cond.wait(m_cs);  
		}			

		item = m_queue.front();  ///取头元素
		m_queue.pop_front();     ///弹出头元素			

		return true;
	}

	template <class A>
	bool CQueue<A>::popTimeWait(A &item,int millisec)
	{	
		AutoCritSec  tmpLock(m_cs);

		while (m_queue.empty()) 
		{		
			//线程睡眠解锁，等待唤醒或者超时
			int ret = m_cond.timedwait(m_cs,millisec);
			if (ret == CVariable_TimeOut)      ///超时了
			{				
				return false;
			}
		}		

		item = m_queue.front();  ///取头元素
		m_queue.pop_front();	
	
		return true;
	}

}
////////////////////////////////////////////////////////////////////////////
#endif /* TQUEUE_H_ */
