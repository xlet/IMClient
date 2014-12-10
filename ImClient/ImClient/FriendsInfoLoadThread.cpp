#include "StdAfx.h"
#include "FriendsInfoLoadThread.h"
#include "FriendGroup.h"
CFriendsInfoLoadThread::CFriendsInfoLoadThread(void)
{
}


CFriendsInfoLoadThread::~CFriendsInfoLoadThread(void)
{
}

BOOL CFriendsInfoLoadThread::Run()
{
	FriendsInfoLoadThreadPara* p = (FriendsInfoLoadThreadPara*)(m_pPara);
	// 此处可以根据用户名和密码跟本地数据库中进行匹配
    if (FriendList::GetInstance()->FromMsgdb())
	{
		PostMessage(p->m_hWnd, WM_LOGINRET, WPARAM(TRUE), 0);
	}
	return FALSE;
}