#pragma once
#include "BaseMember.h"
class WOAMember:public BaseMember 
{
public:
	WOAMember(void);
	~WOAMember(void);
	virtual void  FromJson(const Json::Value& val);
	virtual void FromFriendInfo(const friendsinfo& f);
	virtual void GetFriendInfo(friendsinfo& f);
	std::string m_strPost;
	std::string m_strDepartment;
	virtual LPCSTR GetMemberType();
};

