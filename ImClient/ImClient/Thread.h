#pragma once


class CRITICAL_SECTION_EX
{
public:
	CRITICAL_SECTION_EX()
	{
		//InitializeCriticalSection(&cs);
		bInit = FALSE;
		if (!bInit)
		{
			InitializeCriticalSection(&cs);
			bInit = TRUE;
		}
	}
	~CRITICAL_SECTION_EX()
	{
		if(bInit)
			DeleteCriticalSection(&cs);
	}
	void Lock()
	{
		if(bInit)
			EnterCriticalSection(&cs);
	}

	void UnLock()
	{
		LeaveCriticalSection(&cs);
	}
private:

	CRITICAL_SECTION  cs;	
	BOOL bInit;
};
/*
class CRITICAL_SECTION_EX
{
public:
CRITICAL_SECTION_EX()
{
//InitializeCriticalSection(&cs);
bInit = FALSE;
}
~CRITICAL_SECTION_EX()
{
if(bInit)
DeleteCriticalSection(&cs);
}
CRITICAL_SECTION  cs;	
BOOL bInit;
};
*/
// 也可以将这些函数放在CRITICAL_SECTION_EX类中
// void InitializeCriticalSectionExEx(CRITICAL_SECTION_EX* pCsex);
// 
// void Lock(CRITICAL_SECTION_EX* pCsex);
// 
// void UnLock(CRITICAL_SECTION_EX* pCsex);

// 所有线程类的父类

class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
	
	BOOL StartThread();
	BOOL SuspendThread();
	BOOL StopThread();
	void SetPara(UINT_PTR p);
	UINT_PTR GetPara();
    HANDLE GetHandle(){return m_hThread;}
protected:
	virtual BOOL Run();
private:  
	static unsigned  _stdcall  start_address(void* p);
private:
	HANDLE m_hThread; 
	HANDLE m_hEvent;
	BOOL   m_bEventSignal;
	unsigned m_threadAddr;
	BOOL   m_bStatus;
protected:	 
	UINT_PTR m_pPara; //参数指针
	
};

