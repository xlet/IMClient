#ifndef MSGDATA_H_
#define  MSGDATA_H_

//客户端向服务器发送时候时所带类型
#define   PRODUCT_TYPE    "OA" //"Wcn"
/************************************************************************/
/* 
	客户端往外发送的数据结构

*/
/************************************************************************/

class BaseMsgInfo
{
public:
	BaseMsgInfo();
	~BaseMsgInfo();
public:
	std::string m_strType;
	std::string m_strMsgType;
	double      m_dbSendTime;
	double		m_dbreceivedTime;
	std::string m_strClientType;
};

class IMsgJson
{
public:
	IMsgJson();
	virtual ~IMsgJson();
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  = 0;
	//void AddBaseMsgInfoToJson(Json::Value& val, const BaseMsgInfo& basemsginfo);
	void AddBaseMsgInfoToJson(Json::Value& val);
	
public:
	void Init(const std::string& strtype, const std::string& strMsgtype);
	BaseMsgInfo  m_baseMsgInfo;
};




/*
	logout response 
*/





/*

=============login success=================
{
"@type" : "LoginResponse",
"messageType" : "LoginResponse",
"sendTime" : 1404205317073,
"receivedTime" : 0,
"success" : true,
"errorCode" : 0,
"errorMessage" : null,
"token" : {
"clientHost" : "10.0.40.18",
"loginId" : "username",
"loginDate" : 1404205317072,
"token" : "ec22044b13aa4a2492e8aca4cd0c42ec",
"allocatedMessageServer" : {
"@type" : "ServerBasic",
"host" : "10.0.41.104",
"port" : 17021,
"start" : true,
"startDateTime" : 1404205317072,
"nodeId" : "1",
"serverType" : "LoginServer"
}
},
"loggedOtherPlace" : null
}

=============login fail=================
{
"@type" : "LoginResponse",
"messageType" : "LoginResponse",
"sendTime" : 1404205317073,
"receivedTime" : 0,
"success" : false,
"errorCode" : 1001,
"errorMessage" : "IDPASSWORDERRORCODE",
"token" : null,
"loggedOtherPlace" : ""
}


=============logout request=================
{
"@type" : "Logout",
"messageType" : "Logout",
"sendTime" : 1404205881458,
"receivedTime" : 0,
"clientType" : "WinForm",
"loginId" : "username"
}

=============logout response=================
{
"@type" : "LogoutResponse",
"messageType" : "LogoutResponse",
"sendTime" : 1404205881522,
"receivedTime" : 0,
"success" : true,
"errorCode" : 0,
"errorMessage" : null
}

=============normal message=================
{
"@type" : "Normal",
"messageType" : "Normal",
"sendTime" : 1404206489818,
"receivedTime" : 0,
"from" : "one",
"to" : "another",
"content" : "消息内容",
"clientType" : "WinForm"
}
*/

#endif