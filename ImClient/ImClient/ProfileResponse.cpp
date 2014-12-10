#include "StdAfx.h"
#include "ProfileResponse.h"
extern const char* const_type;

ProfileResponse* ProfileResponse::m_pInstance = NULL;

ProfileResponse::CGarbo ProfileResponse::Garbo;

ProfileResponse::ProfileResponse(void)
{
}


ProfileResponse::~ProfileResponse(void)
{
	for(MAP_STRING_PLINKMANINFO::iterator it = m_linkmenMap.begin(); it != m_linkmenMap.end(); it++)
	{
		linkmaninfo* p = it->second;
		SAFE_DELETE(p);
	}
	m_linkmenMap.clear();
}

ProfileResponse* ProfileResponse::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new ProfileResponse;
	}
	return m_pInstance;
}

void ProfileResponse::EraseAndRelease(MAP_STRING_PLINKMANINFO::iterator it)
{
	linkmaninfo* p = it->second;
	SAFE_DELETE(p);
	m_linkmenMap.erase(it);
}

void  ProfileResponse::FromJson(const Json::Value& val)
{
	const Json::Value& contactsArry =  val["members"];


	//Json::Value nearlyLinkman = val["nearlyLinkmen"];
	if (contactsArry.isNull() || !contactsArry.isArray())
	{
		return;
	}

	linkmaninfo* pInfo = new linkmaninfoEx();
	int nSize = contactsArry.size();
	std::string strArray[13] = {"status","temp","id", "nickName","address", "mobile","contractor","shopName","mobileValid","realNameValid","thumb","email","merchant" };
	for (int i = 0; i < nSize; ++i)
	{
		pInfo->status = contactsArry[i][strArray[0]].asInt();
		pInfo->temp = contactsArry[i][strArray[1]].asString().c_str();
		pInfo->id = contactsArry[i][strArray[2]].asString().c_str();
		pInfo->nickname = contactsArry[i][strArray[3]].asString().c_str();
		pInfo->address = contactsArry[i][strArray[4]].asString().c_str();
		pInfo->mobile = contactsArry[i][strArray[5]].asString().c_str();
		pInfo->contractor = contactsArry[i][strArray[6]].asString().c_str();
		pInfo->shopname = contactsArry[i][strArray[7]].asString().c_str();
		pInfo->mobilevalid = contactsArry[i][strArray[8]].asBool();
		pInfo->realnamevalid = contactsArry[i][strArray[9]].asBool();
		pInfo->thumb = contactsArry[i][strArray[10]].asString().c_str();
		pInfo->email = contactsArry[i][strArray[11]].asString().c_str();
		pInfo->merchant = contactsArry[i][strArray[12]].asBool();
		m_linkmenMap.insert(make_pair(pInfo->id, pInfo));
	}
}