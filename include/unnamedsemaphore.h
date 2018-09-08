#ifndef __unnamedsemaphore_h
#define __unnamedsemaphore_h

#ifdef WIN32
#include <WINDOWS.H>
#define MAX_SEMAPHORE_COUNT 100
#else
#include <semaphore.h>
#define SHARING_TYPE	0/* 0 means semaphores is shared between threads in same process */
#endif

namespace DAVIT_CLASSES
{
	class UnNamedSemaphore
	{

	public:
		UnNamedSemaphore( int Initial_Count = 0 );
		
		~UnNamedSemaphore();

		int Post();

		int Post( int a_nCount );

		int OpenAll();

		int Wait();

		int TimedWait(long int MaxWaitTime);

		//int SemInit( int a_nCount );

		//int SafeWait( class CriticalSection* pCritSect );

		int GetValue( int* SemValue );

		int GetWaitersCount()const;

	private:
		mutable int	m_nWaitersCount;
#ifdef WIN32
		HANDLE		m_Semaphore;
		mutable int	m_nSemCount;
#else
		sem_t		m_Semaphore;
#endif
	};
};

#endif
