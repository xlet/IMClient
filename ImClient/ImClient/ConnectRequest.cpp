#include "StdAfx.h"
#include "ConnectRequest.h"

extern const char* const_logid ;
extern const  char* const_token ;
extern const char* const_producttype;
ConnectRequest::ConnectRequest()
{
	m_strProductType = PRODUCT_TYPE;
}

ConnectRequest::~ConnectRequest()
{

}

void ConnectRequest::MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  
{
	//Json::FastWriter writer;
	Init(strtype, strMsgtype);
	AddBaseMsgInfoToJson(val/*, m_baseMsgInfo*/);
	val[/*const_logid*/"memberId"]  = m_strLoginId;
	val[const_token] = m_strToken;
	val[const_producttype] = m_strProductType;
}