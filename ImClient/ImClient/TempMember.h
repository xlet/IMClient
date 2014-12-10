#pragma once
#include "BaseMember.h"
class TempMember:public BaseMember
{
public:
	TempMember(void);
	~TempMember(void);
	std::string m_strSource;
	virtual LPCSTR GetMemberType();
};

