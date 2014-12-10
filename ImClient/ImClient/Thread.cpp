#include "StdAfx.h"
#include "Thread.h"

// void InitializeCriticalSectionExEx(CRITICAL_SECTION_EX* pCsex)
// {
// 	//InitializeCriticalSection(&cs);
// 	if (!pCsex->bInit)
// 	{
// 		InitializeCriticalSection(&pCsex->cs);
// 		pCsex->bInit = TRUE;
// 	}
// }
// 
// void Lock(CRITICAL_SECTION_EX* pCsex)
// {
// 	EnterCriticalSection(&(pCsex->cs));
// }
// 
// void UnLock(CRITICAL_SECTION_EX* pCsex)
// {
// 	LeaveCriticalSection(&(pCsex->cs));
// }



CThread::CThread(void)
{
	m_hEvent = CreateEvent(0, TRUE, FALSE, NULL);
	m_bEventSignal = FALSE;
	m_bStatus = TRUE;
	m_pPara = NULL;
	//创建线程
	m_hThread = (HANDLE)_beginthreadex(NULL,0,CThread::start_address,this,0,&m_threadAddr);

}


CThread::~CThread(void)
{
	CloseHandle(m_hEvent);
	CloseHandle(m_hThread);
}

BOOL CThread::StartThread()
{
	//m_bEventSignal = TRUE;
	return SetEvent(m_hEvent);
}

BOOL CThread::SuspendThread()
{
	m_bEventSignal = FALSE;
	return ResetEvent(m_hEvent);
}

BOOL CThread::StopThread()
{
	m_bStatus = FALSE;
	StartThread(); //避免挂死在休眠处
	WaitForSingleObject(m_hThread, INFINITE); // 等待线程安全退出
	return TRUE;
}

BOOL CThread::Run()
{
	return TRUE;
}

void CThread::SetPara(UINT_PTR p)
{
	m_pPara = p;
}

UINT_PTR CThread::GetPara()
{
	return m_pPara;
}

unsigned _stdcall CThread::start_address(void* p)
{
	CThread* pThread = static_cast<CThread*>(p);
	while (pThread->m_bStatus)
	{
		//
		if (!pThread->m_bEventSignal)
		{
			//无信号，挂起线程
			//不加if判断一直在用户态和内核态切换会消耗不必要的时间
			WaitForSingleObject(pThread->m_hEvent, INFINITE);
			pThread->m_bEventSignal = TRUE; //此时变得 有信号
		}
		if (!pThread->Run())
		{
			break; 
		}
		
	}
	_endthreadex(0);
	return 0;
}

