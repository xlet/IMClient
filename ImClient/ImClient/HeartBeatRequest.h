#pragma once
#include "MsgData.h"

class HeartBeatRequest:public IMsgJson
{
public:
	HeartBeatRequest(void);
	~HeartBeatRequest(void);
//	virtual void MakeJson(Json::Value& val);
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  ;
	void AddBaseMsgInfoToJson(Json::Value& val, const BaseMsgInfo& basemsginfo);
public:
	bool   m_bIsReply;
	int    m_nSeq;
};

