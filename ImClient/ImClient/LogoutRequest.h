#pragma once

#include "MsgData.h"
/*
	logout request 
*/
class LogoutRequest:public IMsgJson
{
public:
	LogoutRequest();
	~LogoutRequest();
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype);
public:
	std::string  m_strLoginId;
};

