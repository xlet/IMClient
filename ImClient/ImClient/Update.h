#pragma once
#include "XmlDocument.h"
class CUpdate
{
public:
	CUpdate(void);
	~CUpdate(void);
	BOOL CheckUpdate();
private:
	tString m_tstrPackUrl;
	TCHAR    m_tszVersion[8];
	BOOL CheckUpdate(const char* url, tString& tstrPackageUrl);
	BOOL GetFileVersion(TCHAR* lptVersion, int size) ;

};

