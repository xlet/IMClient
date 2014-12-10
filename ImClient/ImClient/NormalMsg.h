#pragma once
#include "MsgData.h"


/*
	momal message
*/
class NormalMsg:public IMsgJson
{
public:
	NormalMsg();
	~NormalMsg();
	//virtual void MakeJson(Json::Value& val);
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) ;
	void FromJson(const Json::Value& val);
	int GetLength();
public:
	std::string  m_strFromId;
	std::string  m_strtoId;
	std::string  m_strContent;
};

