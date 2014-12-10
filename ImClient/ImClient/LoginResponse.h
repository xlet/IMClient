////////////////////////////////////////////////////
//
//  class LoginResponse
 //     登录登录服务器后返回
///
//

#pragma once
#include "MsgData.h"

//对应旧版本的有个AllocateServer对象
class AllocateServer
{
public:	 
	AllocateServer();
	~AllocateServer();
//	std::string m_strType;
	int         m_nPort;
//	bool        m_bStart;
// 	double      m_dbStartDateTime;
// 	std::string m_strNodeId;
// 	std::string m_strServerType;
	std::string m_strHost;
};

class LoginResponse
{
public:
	LoginResponse(void);
	~LoginResponse(void);
	void FromJson(const Json::Value& val);
	//注释的这些选项对于IM客户端来说都没用处
// 	std::string  m_strType;
// 	std::string  m_strMsgType;
// 	double       m_dbSendTime;
// 	double       m_dbReceivedTime;
// 	bool		 m_bSuccess;
//	std::string  m_strLoggedOtherPalce;
	int			 m_nErrorCode;
	std::string  m_strErrorMsg;
	std::string  m_strToken;
	std::string  m_strMemberId;
	//AllocateServer  m_allocateServer;
	int          m_nPort;
	std::string  m_strHost;
};

