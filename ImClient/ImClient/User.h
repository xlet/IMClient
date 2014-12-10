#pragma once
#include "..\\DataStorage\LocalData.h"
#include "linkmaninfo.h"
#include "ConnectResponse.h"

typedef ConnectResponse*  SELF;
/*
	由于数据库的设计与服务器数据库设计不一致
	导致发送或者接收消息的时候，在存储在本地的
	过程中，需要转化
*/
class CUser
{
public:
	CUser(void);
	~CUser(void);
public:
	BaseMember*	         UerSelf();
	userinfo			 m_userInfo;   //基于Sqlite数据库
	//userconfig			 m_userConfig; //基于sqlite数据库	
	linkmaninfo          m_selfInfo;   //用户个人信息   // 基于客服版本


	void SetValue(const userinfo& val1, const userconfig& val2);
	void  InitUserInfoFromJson(const Json::Value val);  
private:
	ConnectResponse*   m_pConnResponse;
};

