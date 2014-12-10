#pragma once
#include "BaseMember.h"
// class UserSelf
// {
// public:
// 	UserSelf();
// 	~UserSelf();
// public:
// 	std::string  m_strType;
// 	std::string  m_strMemberSrc;
// 	std::string  m_strId;
// 	std::string  m_strNickName;
// 	std::string  m_strPicUrl;
// 	std::string  m_strSignature;
// 	std::string  m_strtempMember;
// 	std::string  m_strMobile;
// 	std::string  m_strTelephone;
// 	std::string  m_strAddress;
// 	std::string  m_strEmail;
// 	bool         m_bInit;
// };

class ConnectResponse:IMember
{
public:
	ConnectResponse(void);
	~ConnectResponse(void);

// 服务端发送过来的无用数据
// 	std::string  m_strType;
// 	std::string  m_strMsgType;
// 	double       m_dbSendTime;
// 	double       m_dbReceivedTime;
// 	bool		 m_bSuccess;
	int			 m_nErrorCode;
	std::string  m_strErrorMsg;
	BaseMember*  m_pUserSelf;
	virtual void FromJson(const Json::Value& val);
};

