#pragma once
#include "Thread.h"
class FriendsInfoLoadThreadPara
{
public:
	HWND  m_hWnd;
	std::string  m_strUserid;
	std::string  m_strPswd;
};

class CFriendsInfoLoadThread:public CThread
{
public:
	CFriendsInfoLoadThread(void);
	~CFriendsInfoLoadThread(void);
private:
	virtual BOOL Run();
};

