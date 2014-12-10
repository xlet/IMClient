#pragma once

/*
	到达的消息， 可以显示在界面上的消息

	对于客服版本而言


*/
#include "NormaMsgCollection.h"

class MsgOfArrival:public NormaMsgCollectionBase
{
public:
	~MsgOfArrival(void);
	static MsgOfArrival* GetInstance();
private:
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			OutputDebugString(_T("MsgOfArrival release \n"));
			SAFE_DELETE(MsgOfArrival::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //清理资源
	MsgOfArrival(void);
	static MsgOfArrival* m_pInstance;
};

