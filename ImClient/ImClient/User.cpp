#include "StdAfx.h"
#include "User.h"


CUser::CUser(void)
{
	m_pConnResponse = NULL;
}


CUser::~CUser(void)
{
	SAFE_DELETE(m_pConnResponse);
}

BaseMember* CUser::UerSelf()
{
	return m_pConnResponse->m_pUserSelf;
}

void CUser::SetValue(const userinfo& val1, const userconfig& val2)
{
	m_userInfo = val1;
	//m_userConfig = val2;
}

void CUser::InitUserInfoFromJson(const Json::Value val)
{
#if 0
	if (val.isNull())
	{
		return;
	}
	std::string strArray[13] = {"status","temp","id", "nickName","address", "mobile","contractor","shopName","mobileValid","realNameValid","thumb","email","merchant" };
	linkmaninfo* pInfo = new linkmaninfo();
	m_selfInfo.status = val[strArray[0]].asInt();
	m_selfInfo.temp = val[strArray[1]].asString().c_str();
	m_selfInfo.id = val[strArray[2]].asString().c_str();
	m_selfInfo.nickname = val[strArray[3]].asString().c_str();
	m_selfInfo.address = val[strArray[4]].asString().c_str();
	m_selfInfo.mobile = val[strArray[5]].asString().c_str();
	m_selfInfo.contractor = val[strArray[6]].asString().c_str();
	m_selfInfo.shopname = val[strArray[7]].asString().c_str();
	m_selfInfo.mobilevalid = val[strArray[8]].asBool();
	m_selfInfo.realnamevalid = val[strArray[9]].asBool();
	m_selfInfo.thumb = val[strArray[10]].asString().c_str();
	m_selfInfo.email = val[strArray[11]].asString().c_str();
	m_selfInfo.merchant = val[strArray[12]].asBool();
#endif 
	if (!m_pConnResponse)
		m_pConnResponse = new ConnectResponse;
	m_pConnResponse->FromJson(val);
}