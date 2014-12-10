//////////////////////////////////////////////////////////////////////////
// class ConnectRequest
//	
// 
//
//
////
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "MsgData.h"

class ConnectRequest: public IMsgJson
{
public:
	ConnectRequest();
	~ConnectRequest();
public:
	std::string  m_strLoginId ;// µÇÂ¼
	std::string   m_strToken;  // tokenÖµ
	//virtual void MakeJson(Json::Value& val);
	std::string   m_strProductType;
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) ;
};
