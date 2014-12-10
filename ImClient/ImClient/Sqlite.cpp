#include "StdAfx.h"
#include "Sqlite.h"
//静态常量的初始化
//能保证现场的安全型， 但是若是sqlite
//e断开连接则无法回复
CSqlite* CSqlite::m_pInstance = NULL;

CSqlite::CGarbo CSqlite::Garbo ;

CSqlite::CSqlite(void)
{
	m_pIIni =NULL;
	m_pIMsgDb = NULL;
	m_pIUserDb = NULL;
	m_pIIni = CreateIni();
	m_pIIni->InitPath();
	m_pIUserDb = CreateUserDb();
	//m_pIMsgDb = CreateMsgDb();
	
	//这面这一步不能少，先要初始化当前Im客户端
	//用户的路径
}


CSqlite::~CSqlite(void)
{
	if (m_pIMsgDb != NULL)
	{
		m_pIMsgDb->FinalRelease();
	}
	if (m_pIUserDb != NULL)
	{
		m_pIUserDb->FinalRelease();
	}
	
	if (m_pIIni != NULL)
	{
		m_pIIni->FinalRelease();
	}
}

IUserDb* CSqlite::GetUserDbInterface() 
{
	return m_pIUserDb;
}


 IMsgDb* CSqlite::GetMsgDbInterface() 
{
	if (m_pIMsgDb == NULL)
	{
		m_pIMsgDb = CreateMsgDb();
	}
	return m_pIMsgDb;
}

IIni* CSqlite::GetIniInterface()
{
	if (m_pIIni == NULL)
	{
		m_pIIni = CreateIni();
	}
	return m_pIIni;
}

 CSqlite* CSqlite::GetIntstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CSqlite();
	}
	
	return m_pInstance;
}

