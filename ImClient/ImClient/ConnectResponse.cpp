#include "StdAfx.h"
#include "ConnectResponse.h"
#include "MemberFactory.h"
// UserSelf::UserSelf()
// {
// 
// }
// 
// UserSelf::~UserSelf()
// {
// 
// }

ConnectResponse::ConnectResponse(void):m_pUserSelf(NULL)
{
}


ConnectResponse::~ConnectResponse(void)
{
	SAFE_DELETE(m_pUserSelf);
}

void ConnectResponse::FromJson(const Json::Value& val)
{
	
	const Json::Value&  self = val["self"];
	m_nErrorCode = val["errorCode"].asBool();
	m_strErrorMsg = val["errorMessage"].asString();
	if (!m_pUserSelf)
	{
		const string& strSrc = self["memberSource"].asString();
		m_pUserSelf = MemberFactory::GetInstance().CreateMember(strSrc.c_str());
// 		if (strcmp(strSrc.c_str(), OAMEMBERSRC) == 0)
// 			m_pUserSelf = new WOAMember();
// 		else if (strcmp(strSrc.c_str(), WCNMEMBERSRC) == 0)
// 			m_pUserSelf = new WCNMember;
// 		else if(strcmp(strSrc.c_str(), TEMPMEMBERSRC) == 0)
// 			m_pUserSelf = new TempMember;
	}
	
	m_pUserSelf->FromJson(self);
}