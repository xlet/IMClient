#include "StdAfx.h"
#include "ProfileRequest.h"
extern const char* const_ids;
extern const char* const_type ;
extern const char* const_msgtype;
extern const char* const_sendtime;
extern const char* const_recvtime;

ProfileRequest::ProfileRequest(void)
{
}


ProfileRequest::~ProfileRequest(void)
{
}

void ProfileRequest::AddBaseMsgInfoToJson(Json::Value& val)
{
	val[const_type] = m_baseMsgInfo.m_strType;
	val[const_msgtype] =  m_baseMsgInfo.m_strMsgType;
	val[const_sendtime] = m_baseMsgInfo.m_dbSendTime;
	val[const_recvtime] = m_baseMsgInfo.m_dbreceivedTime;
}

void ProfileRequest::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)
{
	Init(strtype, strMsgtype);
	AddBaseMsgInfoToJson(val/*, m_baseMsgInfo*/);

	//m_vcId
	for (std::vector<std::string>::iterator it = m_vcId.begin(); it != m_vcId.end(); it++)
	{
		val[const_ids].append(*it);
	}
	
}

void ProfileRequest::FromJson(const Json::Value& val)
{
// 	if (strcmp (val[const_type].asString().c_str(), "GetProfileResponse") != 0)
// 	{
// 		OutputDebugString(_T("Method: FromJson;\n Error: not Profile response \n"));
// 		return;
// 	}
	
}
