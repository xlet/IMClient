#pragma once
#include "WCNMember.h"
#include "WOAMember.h"
#include "TempMember.h"


class MemberFactory
{
public:

	~MemberFactory(void);
	BaseMember* CreateMember(LPCSTR  strMemSrc);
	static MemberFactory& GetInstance();
private:
	MemberFactory(void);
	MemberFactory(const MemberFactory& );
	MemberFactory& operator = (const MemberFactory&);

};

