#include "StdAfx.h"
#include "RecentContacts.h"
extern const char* const_type ;

RecentContact::RecentContact()
{

}

RecentContact::~RecentContact()
{

}
// 
// RecentContactData& RecentContact::data()
// {
// 	return m_data;
// }

RecentContact::RecentContact(const RecentContact& obj)
{
	//m_data = obj.m_data;
	m_strId = obj.m_strId;
	m_strContent = obj.m_strContent;
	m_eLastMsgTye = obj.m_eLastMsgTye;
	m_dbLastTime = obj.m_dbLastTime;
	m_pTag = obj.m_pTag;
}

bool RecentContact::operator==(const RecentContact& obj) const
{
    return strcmp(m_strId.c_str(), obj.m_strId.c_str())==0?true:false;
}

bool RecentContact::operator!=(const RecentContact& obj) const
{
	return !(*this == obj);
}
 
 bool RecentContact::operator<(const RecentContact& obj) const
 {
 // 	if(strcmp(m_data.m_strId.c_str(), obj.m_data.m_strId.c_str())==0)
 // 		return false;
 	
 	return m_dbLastTime > obj.m_dbLastTime;
 }

bool RecentContact::operator>(const RecentContact& obj) const
{
// 	if(strcmp(m_data.m_strId.c_str(), obj.m_data.m_strId.c_str())==0)
// 		return false;
	return m_dbLastTime < obj.m_dbLastTime;
}

void RecentContact::SetTag(UINT_PTR tag)
{
	m_pTag =tag;
}

UINT_PTR RecentContact::GetTag()
{
	return m_pTag;
}

RecentContactCollection::RecentContactCollection(void)
{
}

RecentContactCollection::~RecentContactCollection(void)
{
// 	for (MAP_STRING_PLINKMANINFO::iterator it = m_linkmenMap.begin();it !=m_linkmenMap.end(); ++it)
// 	{
// 		PLINKMANINFO p = it->second;
// 		SAFE_DELETE(p);
// 	}
// 	m_linkmenMap.clear();
}

void RecentContactCollection::Erase(const std::string& str)
{
// 	PLINKMANINFO p = m_linkmenMap.at(str);
// 	SAFE_DELETE(p);
// 	m_linkmenMap.erase(str);
}

/*
	最近联系人
	{
		"@type": "RecentContactCollection", 
		"messageType": "RecentContactCollection", 
		"receivedTime": 0, 
		"RecentContactCollection": 
		[
			{
			"contactId": "wdemo1:w005", 
			"lastContent": "dwd", 
			"lastTime": 1416360547906
			}
		], 
		"sendTime": 1416360650859
	}s
*/
void RecentContactCollection::FromJson(const Json::Value& val)
{
	Json::Value contactsArry = val["recentContacts"];

	if (contactsArry.isNull() || !contactsArry.isArray())
	{
		return;
	}

	int nSize = contactsArry.size();
	RecentContact contact;
	contact.m_eLastMsgTye = E_RECEIVE;
	for (int i = 0; i < nSize; ++i)
	{
		contact.m_strId = contactsArry[i]["contactId"].asString();
		contact.m_strContent = contactsArry[i]["lastContent"].asString();
		contact.m_dbLastTime = contactsArry[i]["lastTime"].asDouble();
		m_setRecentContacts.insert(contact);
	}
// 	std::string strArray[13] = {"status","temp","id", "nickName","address", "mobile","contractor","shopName","mobileValid","realNameValid","thumb","email","merchant" };
// 	for (int i = 0; i < nSize; ++i)
// 	{
// 		linkmaninfo* pInfo = new linkmaninfoEx();
// 		pInfo->status = contactsArry[i][strArray[0]].asInt();
// 		pInfo->temp = contactsArry[i][strArray[1]].asString().c_str();
// 		pInfo->id = contactsArry[i][strArray[2]].asString().c_str();
// 		pInfo->nickname = contactsArry[i][strArray[3]].asString().c_str();
// 		pInfo->address = contactsArry[i][strArray[4]].asString().c_str();
// 		pInfo->mobile = contactsArry[i][strArray[5]].asString().c_str();
// 		pInfo->contractor = contactsArry[i][strArray[6]].asString().c_str();
// 		pInfo->shopname = contactsArry[i][strArray[7]].asString().c_str();
// 		pInfo->mobilevalid = contactsArry[i][strArray[8]].asBool();
// 		pInfo->realnamevalid = contactsArry[i][strArray[9]].asBool();
// 		pInfo->thumb = contactsArry[i][strArray[10]].asString().c_str();
// 		pInfo->email = contactsArry[i][strArray[11]].asString().c_str();
// 		pInfo->merchant = contactsArry[i][strArray[12]].asBool();
// 		m_linkmenMap.insert(make_pair(pInfo->id, pInfo));
// 	}
}

RecentContactSet& RecentContactCollection::RecentContacts()
{
	return m_setRecentContacts;
}
