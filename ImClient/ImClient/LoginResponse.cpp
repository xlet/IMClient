#include "StdAfx.h"
#include "LoginResponse.h"

extern const char*  const_allocatedMsgServ;

AllocateServer::AllocateServer()
{

}


AllocateServer::~AllocateServer()
{

}

LoginResponse::LoginResponse(void)
{
}


LoginResponse::~LoginResponse(void)
{
}

void LoginResponse::FromJson(const Json::Value& val)
{
	bool m_bSuccess = val["success"].asBool();
	if (!m_bSuccess)
	{
		OutputDebugString(_T("LoginResponse FromJson Method Failed"));

	}
	//const Json::Value& allocateServer = val[const_allocatedMsgServ];
// 	m_strType = val["@type"].asString();
// 	m_strMsgType = val["messageType"].asString();
// 	m_dbSendTime = val["sendTime"].asDouble();
// 	m_dbReceivedTime = val["receivedTime"].asDouble();
	m_nErrorCode = val["errorCode"].asInt();
	m_strErrorMsg = val["errorMessage"].asString();
	m_strToken = val["token"].asString();
	m_strMemberId = val["memberId"].asString();
//	m_strLoggedOtherPalce = val["loggedOtherPlace"].asString();
//	m_allocateServer.m_strType = allocateServer["@type"].asString();
	m_nPort  = val["messagePort"].asInt();
// 	m_allocateServer.m_bStart = allocateServer["start"].asBool();
// 	m_allocateServer.m_dbStartDateTime = allocateServer["startDateTime"].asDouble();
// 	m_allocateServer.m_strServerType = allocateServer["serverType"].asString();
// 	m_allocateServer.m_strNodeId = allocateServer["nodeId"].asString();
	m_strHost = val["messageHost"].asString();
}