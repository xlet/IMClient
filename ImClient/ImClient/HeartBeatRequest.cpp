#include "StdAfx.h"
#include "HeartBeatRequest.h"

extern const char*  const_reply ;

extern const char*  const_seq ;
extern const char* const_type;
extern const char* const_msgtype;
extern const char* const_sendtime;
extern const char* const_recvtime;

HeartBeatRequest::HeartBeatRequest(void)
{
	m_nSeq = 1;  //³õÊ¼»¯Îª1
}


HeartBeatRequest::~HeartBeatRequest(void)
{
}

void HeartBeatRequest::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) 
{
	//Json::FastWriter writer;
	Init(strtype, strMsgtype);
	AddBaseMsgInfoToJson(val, m_baseMsgInfo);
	val[const_reply] = m_bIsReply;
	val[const_seq] = m_nSeq;
}


void HeartBeatRequest::AddBaseMsgInfoToJson(Json::Value& val, const BaseMsgInfo& basemsginfo)
{
	val[const_type] = basemsginfo.m_strType;
	val[const_msgtype] =  basemsginfo.m_strMsgType;
	val[const_sendtime] = basemsginfo.m_dbSendTime;
	val[const_recvtime] = basemsginfo.m_dbreceivedTime;
}