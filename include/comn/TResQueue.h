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

		///ѹ��һ��Ԫ��
		void push(const A &item);					//for producer
		bool pop(A &item);							//for consumer
		bool popTimeWait(A &item,int millisec);     //for consumer

		size_t  size();     //���д�С
		bool    empty();    ///��Ϊ��
	
	private:		
		std::deque<A> m_queue;    ///stl deque
		
		CriticalSection   m_cs ;   ///�߳���
		ConditionVariable m_cond;  ///��������
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
			m_queue.push_back(item);	///���ݷ������
		}		
		
		//����һ���߳�
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
			//�߳�˯�߽������ȴ�����
			m_cond.wait(m_cs);  
		}			

		item = m_queue.front();  ///ȡͷԪ��
		m_queue.pop_front();     ///����ͷԪ��			

		return true;
	}

	template <class A>
	bool CQueue<A>::popTimeWait(A &item,int millisec)
	{	
		AutoCritSec  tmpLock(m_cs);

		while (m_queue.empty()) 
		{		
			//�߳�˯�߽������ȴ����ѻ��߳�ʱ
			int ret = m_cond.timedwait(m_cs,millisec);
			if (ret == CVariable_TimeOut)      ///��ʱ��
			{				
				return false;
			}
		}		

		item = m_queue.front();  ///ȡͷԪ��
		m_queue.pop_front();	
	
		return true;
	}

}
////////////////////////////////////////////////////////////////////////////
#endif /* TQUEUE_H_ */
