#include "StdAfx.h"
#include "WOAMember.h"


WOAMember::WOAMember(void)
{
}


WOAMember::~WOAMember(void)
{
}

void WOAMember::FromJson(const Json::Value& val)
{
	BaseMember::FromJson(val);
	 
	 //////////////////////////////////////////////////////////////////////////
	 m_strPost = val["post"].asString();
	 m_strDepartment = val["department"].asString();
}

void WOAMember::FromFriendInfo(const friendsinfo& f)
{
	BaseMember::FromFriendInfo(f);
	m_strPost = f.post;
	m_strDepartment = f.department;
}

LPCSTR WOAMember::GetMemberType()
{
	return OAMEMBERSRC;
}

void WOAMember::GetFriendInfo(friendsinfo& f)
{
	BaseMember::GetFriendInfo(f);
	f.post = m_strPost;
	f.department = m_strDepartment;
}
