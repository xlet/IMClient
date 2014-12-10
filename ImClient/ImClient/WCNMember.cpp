#include "StdAfx.h"
#include "WCNMember.h"


WCNMember::WCNMember(void)
{
}


WCNMember::~WCNMember(void)
{
}

void WCNMember::FromJson(const Json::Value& val)
{
// 	BaseMember::FromJson(val);
// 	m_strContractor = val[]
// 	
// 	m_strShopName;
// 	bool	      m_bMobilevalid;
// 	bool		  m_Realnamevalid;
// 	bool		  m_bMerchant;
}

LPCSTR WCNMember::GetMemberType()
{
	return WCNMEMBERSRC;
}