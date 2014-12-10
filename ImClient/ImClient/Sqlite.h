#pragma once
#include "..\\DataStorage\LocalData.h"

#ifdef _DEBUG
#pragma comment(lib,  "DataStorage_d.lib")
#else
#   ifdef _UNICODE
#pragma comment(lib,  "DataStorage_u.lib")
#   endif
#endif

/*
	sqlite 在多线程操作的时候可能出现异常， 由于使用了单例模式只存在一个sqlite3结构指针，
*/

class CSqlite
{
private:
	CSqlite();
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(CSqlite::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //清理资源
public:
	~CSqlite(void);
	static CSqlite* GetIntstance();
	IUserDb* GetUserDbInterface() ;
	IMsgDb*  GetMsgDbInterface() ; 
	IIni*  GetIniInterface();
private:
	static  CSqlite*  m_pInstance;
	IUserDb*		       m_pIUserDb;
	IMsgDb*				   m_pIMsgDb;
	IIni*				   m_pIIni;
};

