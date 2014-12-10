#pragma once
//////////////////////////////////////////////////////////////////////////
// 
// class LoginRequest 
//    登录登录服务器数据结构
//   
//
//
//////////////////////////////////////////////////////////////////////////

#include "MsgData.h"
class LoginRequest:public IMsgJson
{
public:
	LoginRequest();
	~LoginRequest();
public:
	std::string m_strLoginId; //登陆ID
	std::string m_strPswd;   //登陆密码
	std::string m_strProductType; // 产品类型 
	//virtual void MakeJson(Json::Value& val);
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  ;
};