#include "StdAfx.h"
#include "MemberFactory.h"


MemberFactory::MemberFactory(void)
{
}


MemberFactory::~MemberFactory(void)
{
}

BaseMember* MemberFactory::CreateMember(LPCSTR strMemSrc)
{
	if (strcmp(strMemSrc, OAMEMBERSRC) == 0)
		return new WOAMember();
	else if (strcmp(strMemSrc, WCNMEMBERSRC) == 0)
		return new WCNMember;
	else if(strcmp(strMemSrc, TEMPMEMBERSRC) == 0)
		return  new TempMember;
	return NULL;
}

MemberFactory& MemberFactory::GetInstance()
{
	static MemberFactory instance;
	return instance;
}