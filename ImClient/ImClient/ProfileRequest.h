#pragma once
#include "MsgData.h"

/************************************************************************/
/* 
	功能： 联系人信息请求

*/
/************************************************************************/
class ProfileRequest:public IMsgJson
{
public:
	ProfileRequest(void);
	~ProfileRequest(void);
	void AddBaseMsgInfoToJson(Json::Value& val);
public:
	//联系人的Id数组
	std::vector<std::string> m_vcId;
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) ;
	void FromJson(const Json::Value& val);
};

