#include "StdAfx.h"

#include "MsgData.h"
#include <sys/timeb.h>
//key of the json
const char* const_producttype ="productType";
const char* const_memberid = "memberId";
const char* const_type = "@type" ;
const char* const_msgtype = "messageType";
const char* const_sendtime = "sendTime";
const char* const_recvtime = "receivedTime" ;
const char* const_clienttype = "clientType" ;
const char* const_logid = "loginId" ;
const char* const_pswd = "password" ;
const char* const_fromid = "from" ;
const char* const_toid = "to" ;
const char* const_msgcontent = "content" ;
const char* const_token = "token";
const char* const_allocatedMsgServ = "allocateServer" ;
const char* const_host = "host";
const char* const_port = "port";
const char* const_startdatetime  = "startDateTime";
const char* cosnt_errorcode = "errorCode";
const char* const_errorMsg = "errorMessage";
const char* const_reply = "reply" ;
const char* const_seq = "seq"; 
const char* const_ids = "ids";
//
const char* const_type_login_requestval = "Login";
const char* const_type_login_responseval = "LoginResponse";
const char* const_type_logout_requestval = "Logout";
const char* const_type_logout_responseval = "LogoutResponse";
const char* const_type_connect_requestval = "Connect";
const char* const_type_connect_responseval = "ConnectResponse";
const char* const_type_normal_msgval = "Normal";
const char* const_type_heartbeat_request = "Heartbeat";

const char* const_msgtype_logout_response = "Logout";
const char* const_msgtype_connect_requestval = "Connect";
const char* const_msgtype_connect_responseval = "ConnectResponse";
const char* const_msgtype_login_requestval = "Login";
const char* const_msgtype_login_responseval = "LoginResponse";
const char* cosnt_msgtype_normal_msgval = "Normal";
const char* const_clienttype_name = "WinForm";
const char* const_msgtype_heartbeat_request = "Heartbeat";
IMsgJson::IMsgJson()
{

}

IMsgJson::~IMsgJson()
{

}
// 
// void IMsgJson::AddBaseMsgInfoToJson(Json::Value& val, const BaseMsgInfo& basemsginfo)
// {
// 	val[const_type] = basemsginfo.m_strType;
// 	val[const_msgtype] =  basemsginfo.m_strMsgType;
// 	val[const_sendtime] = basemsginfo.m_dbSendTime;
// 	val[const_recvtime] = basemsginfo.m_dbreceivedTime;
// 	val[const_clienttype] = basemsginfo.m_strClientType;
// }


void IMsgJson::AddBaseMsgInfoToJson(Json::Value& val)
{
	val[const_type] = m_baseMsgInfo.m_strType;
	val[const_msgtype] =  m_baseMsgInfo.m_strMsgType;
	val[const_sendtime] = m_baseMsgInfo.m_dbSendTime;
	val[const_recvtime] = m_baseMsgInfo.m_dbreceivedTime;
	val[const_clienttype] = m_baseMsgInfo.m_strClientType;
}

void IMsgJson::Init(const std::string& strtype, const std::string& strMsgtype)
{
	m_baseMsgInfo.m_strType = strtype;
	m_baseMsgInfo.m_strMsgType = strMsgtype;
	struct timeb tmb;
	ftime(&tmb);
	m_baseMsgInfo.m_dbSendTime = (double)(tmb.time*1000) + (double)(tmb.millitm);
	m_baseMsgInfo.m_dbreceivedTime = 0.0;
	m_baseMsgInfo.m_strClientType =  const_clienttype_name;
}
BaseMsgInfo::BaseMsgInfo()
{
}

BaseMsgInfo::~BaseMsgInfo()
{

}