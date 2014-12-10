#include "StdAfx.h"
#include "MsgOfArrival.h"


MsgOfArrival* MsgOfArrival::m_pInstance = NULL;
MsgOfArrival::CGarbo MsgOfArrival:: Garbo;  //清理资源
MsgOfArrival::MsgOfArrival(void)
{
}


MsgOfArrival::~MsgOfArrival(void)
{
// 	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = m_normalmsgMap.begin();
// 		it != m_normalmsgMap.end(); ++it)
// 	{
// 		NormaMsgCollection*  p = it->second;
// 		SAFE_DELETE(p);
// 	}
// 	m_normalmsgMap.clear();
	OutputDebugString(_T("MsgofArrival destructor \n"));
}

MsgOfArrival* MsgOfArrival::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new MsgOfArrival;
	}
	return m_pInstance;
}