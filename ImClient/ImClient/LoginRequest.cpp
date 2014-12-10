#include "StdAfx.h"
#include "LoginRequest.h"

extern const char* const_logid;
extern const char* const_pswd ;
extern const char* const_producttype;
LoginRequest::LoginRequest()
{
	m_strProductType = PRODUCT_TYPE;
}

LoginRequest::~LoginRequest()
{

}

void LoginRequest::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  
{
	//Json::FastWriter writer;
	Init(strtype, strMsgtype);
	AddBaseMsgInfoToJson(val/*, m_baseMsgInfo*/);
	val[const_logid] = m_strLoginId;
	val[const_pswd] = m_strPswd;
	val[const_producttype] = m_strProductType;
	//val.end();
}