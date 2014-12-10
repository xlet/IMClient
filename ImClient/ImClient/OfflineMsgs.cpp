#include "StdAfx.h"
#include "OfflineMsgs.h"

extern const char* const_type ;
extern const char* const_msgtype ;
extern const char* const_sendtime;
extern const char* const_recvtime ;
extern const char* const_fromid ;
extern const char* const_toid ;
extern const char* const_msgcontent ;
extern const char* const_clienttype ;


COfflineMsgs::COfflineMsgs(void)
{
}


COfflineMsgs::~COfflineMsgs(void)
{
	OutputDebugString(_T("COfflineMsgs destructor \n"));
}

void COfflineMsgs::Erase(const std::string& str)
{
	//SAFE_ARRYDELETE(m_offlinemsgMap.at(str));
	
}

// void COfflineMsgs::Insert(NormalMsg* p)
// {
// 	MAP_STRING_PNORMALMSGCOLLECTION::iterator it = m_normalmsgMap.find(p->m_strFromId);
// 	if (it != m_normalmsgMap.end())
// 	{
// 		//ÕÒµ½
// 		it->second->m_vcNormalMsg.push_back(p);
// 	}
// 	else
// 	{
// 		NormaMsgCollection* pCollection = new NormaMsgCollection;
// 		pCollection->m_vcNormalMsg.push_back(p);
// 		m_normalmsgMap.insert(make_pair(p->m_strFromId, pCollection));
// 	}
// }

void COfflineMsgs::FromJson(const Json::Value& val)
{
	const Json::Value& offlineMsg = val["offlineMessages"];
	if (offlineMsg.isNull() || !offlineMsg.isArray())
	{
		return;
	}
	

	int nSize = offlineMsg.size();
#ifdef _DEBUG
	if (nSize >= 0)
	{
		OutputDebugString(_T("Connectresponse has offlinemsg \n"));
	}
#endif 
	for (int i = 0; i < nSize; i++)
	{
		NormalMsg*  pNormalMsg = new NormalMsg();
		pNormalMsg->m_baseMsgInfo.m_strType = offlineMsg[i][const_type].asString().c_str();
		pNormalMsg->m_baseMsgInfo.m_strMsgType = offlineMsg[i][const_msgtype].asString().c_str();
		pNormalMsg->m_baseMsgInfo.m_dbSendTime = offlineMsg[i][const_sendtime].asDouble();
		pNormalMsg->m_baseMsgInfo.m_dbreceivedTime = offlineMsg[i][const_recvtime].asDouble();
		pNormalMsg->m_baseMsgInfo.m_strClientType = offlineMsg[i][const_clienttype].asString().c_str();
		pNormalMsg->m_strFromId = offlineMsg[i][const_fromid].asString().c_str();
		pNormalMsg->m_strtoId = offlineMsg[i][const_toid].asString().c_str();
		pNormalMsg->m_strContent = offlineMsg[i][const_msgcontent].asString().c_str();
		
		//m_offlinemsgMap.insert(make_pair(pNormalMsg->m_strFromId, pNormalMsg));
		Insert(pNormalMsg);

	}
}