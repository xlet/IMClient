#include "StdAfx.h"
#include "FriendGroup.h"
#include "MemberFactory.h"
#include "Global.h"
#include "Sqlite.h"
FriendList* FriendList::m_pInstance = NULL;
FriendGroupInfo::FriendGroupInfo()
{
	
}

FriendGroupInfo::~FriendGroupInfo()
{
	for (map<tString, BaseMember*>::iterator it=m_mapContracter.begin(); it!=m_mapContracter.end();++it)
	{
		BaseMember* p=it->second;
		SAFE_DELETE(p);
	}
	m_mapContracter.clear();
}

void FriendGroupInfo::FromContactsArrayJson(const Json::Value& val)
{
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(val);


	if (!val.isArray())
	{
		OutputDebugString(_T("FriendGroupInfo:FromContactsArrayJson, param is not Json Array\n"));
		return;
	}
	int nSize = val.size();
	MemberFactory& memberFactory = MemberFactory::GetInstance();
	//LPCSTR strMemberSrc = val["memberSource"].asString().c_str();
	BaseMember* pMember = NULL;
	tString tstrId;
	for (int i = 0; i < nSize;  ++i)
	{
		pMember = memberFactory.CreateMember(val[i]["memberSource"].asString().c_str());
		pMember->FromJson(val[i]);
		StringTotString(tstrId, pMember->m_strId);
		m_mapContracter.insert(make_pair(tstrId, pMember));
	}

}

// void FriendGroupInfo::SetTag(UINT_PTR tag)
// {
// 	m_pTag = tag;
// }
// 
// UINT_PTR FriendGroupInfo::GetTag()
// {
// 	return m_pTag;
// }

//////////////////////////////////////////////////
FriendGroup::FriendGroup(void)
{

}


FriendGroup::~FriendGroup(void)
{

}

void FriendGroup::SetTag(UINT_PTR tag)
{
	m_pTag = tag;
}

UINT_PTR FriendGroup::GetTag()
{
	return m_pTag;
}

//////////////////////////
FriendGroupInfo& FriendGroup::GetFriendGroupInfo()
{
	return m_friendGroupInfo;
}

FriendList::FriendList()
{

}

FriendList::~FriendList()
{
	for (map<tString, FriendGroup*>::iterator it=m_mapFriendList.begin();it != m_mapFriendList.end(); ++it)
	{
		FriendGroup * p = it->second;
		SAFE_DELETE(p);
	}
	m_mapFriendList.clear();
}

FriendList* FriendList::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new FriendList;
	}
	return m_pInstance;
}

//  "@type": "FriendGroup"
//
void FriendList::FromFriendGroup(const Json::Value& val)
{
	//@type FriendGroup
	const Json::Value& group = val["friendGroup"];
	FriendGroup* pGroup = new FriendGroup;
	pGroup->GetFriendGroupInfo().m_bSystem = group["system"].asBool();
	pGroup->GetFriendGroupInfo().m_strGroupId = group["id"].asString();
	std::string strName = pGroup->GetFriendGroupInfo().m_strName = group["name"].asString();
	pGroup->GetFriendGroupInfo().FromContactsArrayJson(group["contacts"]);

	tString tstrName;
	StringTotString(tstrName, strName);
	GetFriendList().insert(make_pair(tstrName, pGroup));
	//pGroup->GetFriendGroupInfo().m_mapContracter
	//m_mapFriendList.insert
}

BOOL FriendList::FromMsgdb()
{
	IMsgDb* pIMsgDb = CSqlite::GetIntstance()->GetMsgDbInterface();
	std::vector<friendsinfo> vFriends;
	std::vector<group>  vGroup;
	if (pIMsgDb->QueryFriendsCollection(vFriends) != DB_OK)
		return FALSE;
	if (pIMsgDb->QueryGroupCollection(vGroup) != DB_OK)
		return FALSE;
	for (std::vector<group>::iterator it=vGroup.begin(); it != vGroup.end(); ++it)
	{
		FriendGroup* pGroup = new FriendGroup;
		pGroup->GetFriendGroupInfo().m_strGroupId = it->id;
		pGroup->GetFriendGroupInfo().m_strName = it->name;
		pGroup->GetFriendGroupInfo().m_bSystem = it->system;
		tString tstrName;
		StringTotString(tstrName, it->name);
		m_mapFriendList.insert(make_pair(tstrName, pGroup));
	}

	tString tstrGroupName;
	for (std::vector<friendsinfo>::iterator it=vFriends.begin(); it != vFriends.end(); ++it)
	{
		BaseMember* pMember = NULL;
		if (it->membersource == OAMEMBERSRC)   //如果除了OA的其他用户可以再此处判断再添加
		{
			 pMember = new WOAMember;
			pMember->FromFriendInfo(*it);
			WOAMember* p = static_cast<WOAMember*>(pMember);
		}
#if 0
		else if (it->membersource == WCNMEMBERSRC)
			pMember = new WCNMEMBERSRC;
		else if (it->membersource == TEMPMEMBERSRC)
			pMember = new TempMember;
		else  if (it->membersource == BASEMEMBERSRC)
			pMember = new BaseMember;
		if(pMember == NULL)
			return FALSE;
#endif
		StringTotString(tstrGroupName, it->department);
		std::map<tString, FriendGroup*>::iterator itFind = m_mapFriendList.find(tstrGroupName);
		if (itFind != m_mapFriendList.end())
		{
			tString tstrId;
			StringTotString(tstrId, it->id);
			itFind->second->GetFriendGroupInfo().GetMemberMap().insert(make_pair(tstrId, pMember));
		}
	}
	return TRUE;
}

map<tString, FriendGroup*>& FriendList::GetFriendList()
{
	return m_mapFriendList;
}

map<tString, BaseMember*>&  FriendList::GetMemberMap(const tString tstrId)
{
	return  GetFriendList().at(tstrId)->GetFriendGroupInfo().GetMemberMap();
}