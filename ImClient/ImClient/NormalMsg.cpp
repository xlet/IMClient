#include "StdAfx.h"
#include "NormalMsg.h"

extern const char* const_fromid;
extern const char* const_toid ;
extern const char* const_msgcontent;
extern const char* const_type;
extern const char* const_type_normal_msgval ;
extern const char* const_msgtype ;
extern const char* const_sendtime ;
extern const char* const_recvtime ;
extern const char* const_clienttype ;
extern const char* const_fromid ;
extern const char* const_toid ;
extern const char* const_msgcontent ;
NormalMsg::NormalMsg()
{

}

NormalMsg::~NormalMsg()
{

}

void NormalMsg::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  
{
	//Json::FastWriter writer;
	Init(strtype, strMsgtype);
	AddBaseMsgInfoToJson(val/*, m_baseMsgInfo*/);
	val[const_fromid] = m_strFromId;
	val[const_toid] = m_strtoId;
	val[const_msgcontent] = m_strContent;
}

void NormalMsg::FromJson(const Json::Value& val)
{
	if (val[const_type].asString() != const_type_normal_msgval)
	{
		OutputDebugString(_T("Not Normalmsg json \n"));
		return;
	}


	m_baseMsgInfo.m_strType =  val[const_type].asString().c_str();
	m_baseMsgInfo.m_strMsgType = val[const_msgtype].asString().c_str();
	m_baseMsgInfo.m_dbSendTime = val[const_sendtime].asDouble();
	m_baseMsgInfo.m_dbreceivedTime  = val[const_recvtime].asDouble();
	m_baseMsgInfo.m_strMsgType = val[const_clienttype].asString().c_str();
	m_strtoId = val[const_toid].asString().c_str();
	m_strFromId = val[const_fromid].asString().c_str();
	m_strContent = val[const_msgcontent].asString().c_str();
}
