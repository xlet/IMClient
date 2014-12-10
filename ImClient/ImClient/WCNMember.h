#pragma once
#include "BaseMember.h"

class WCNMember:public BaseMember
{
public:
	WCNMember(void);
	~WCNMember(void);
	std::string   m_strContractor;
	std::string   m_strShopName;
	bool	      m_bMobilevalid;
	bool		  m_Realnamevalid;
	bool		  m_bMerchant;
	virtual void FromJson(const Json::Value& val);
	virtual void FromFriendInfo(const friendsinfo& f){}
	virtual void GetFriendInfo(friendsinfo& f){}
	virtual LPCSTR GetMemberType();
};

