#pragma once
//#include "MsgData.h"
//#include "NormalMsg.h"
#include "NormaMsgCollection.h"
//typedef std::map<std::string, NormalMsg*>   MAP_STRING_PNORMALMSG;//OFFLINEMSGMAP;
class COfflineMsgs:public NormaMsgCollectionBase
{
public:
	COfflineMsgs(void);
	~COfflineMsgs(void);
	void  FromJson(const Json::Value& val);
	void Erase(const std::string& str);
	//void Insert(NormalMsg* p);
public:
	//MAP_STRING_PNORMALMSGCOLLECTION     m_normalmsgMap;
	
};