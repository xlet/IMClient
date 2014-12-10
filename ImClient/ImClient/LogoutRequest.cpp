#include "StdAfx.h"
#include "LogoutRequest.h"

extern const char* const_memberid;

extern const char* const_logid;


LogoutRequest::LogoutRequest()
{

}

LogoutRequest::~LogoutRequest()
{

}

void LogoutRequest::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)
{
	Init(strtype, strMsgtype);
	//Json::FastWriter writer;
	AddBaseMsgInfoToJson(val/*, m_baseMsgInfo*/);
	val[/*const_logid*/const_memberid] = m_strLoginId;
}
