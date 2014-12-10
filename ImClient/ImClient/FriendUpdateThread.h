#pragma once
#include "Thread.h"
/*
	目前服务端无法做到每次都发送好友列表的变更，而是每次都发送了所有的好友信息。

	如果服务端实现了每次发送好友变更那么CFriendUpdateThread中每次也只需要根据服务端发送的变更改变本地数据库即可。

	目前CFriendUpdateThread每次都存储服务端发送的所有的联系人
*/
class CFriendUpdateThread:public CThread
{
public:
	CFriendUpdateThread(void);
	~CFriendUpdateThread(void);
private:
	virtual BOOL Run();
	static CRITICAL_SECTION_EX s_cs;
};

