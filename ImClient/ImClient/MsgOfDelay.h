#pragma once

//#include "NormalMsg.h"

#include "NormaMsgCollection.h"
/************************************************************************/
/* 
	暂时滞留的消息

	仅限于客服版Im 
	当接收到一个消息的时候， 如果想联系人列表里面并没有当前的用户
	那么需要根据当前消息中用户的Id来获取用户的信息后， 将当前用户
	添加到联系人列表并在联系人列表中显示后， 才叫消息显示在聊天窗口
*/
/************************************************************************/
typedef std::map<std::string, NormalMsg*> MAP_STRING_PNORMALMSG ;



class CMsgOfDelay:public NormaMsgCollectionBase
{
public:
	~CMsgOfDelay(void);
	//MAP_STRING_PNORMALMSGCOLLECTION m_normalmsgMap;
	static CMsgOfDelay* GetInstance();
private:
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(CMsgOfDelay::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //清理资源
	CMsgOfDelay(void);
	static CMsgOfDelay* m_pInstance;
};

