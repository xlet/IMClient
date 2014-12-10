#pragma once

#include "Global.h"
#define CLIENT_DLL_REEOR	0x01	//调用Windows socket dll失败
#define CLIENT_EXIT_OK		0x0		//客户端正常退出
#define MAX_NUM_BUF			1024		//接收缓冲区的最大长度
#define MAX_SEND_BUF		1024
#define SERVERPORT			17041	//5556////通信端口
#define SERVERIP			/*"127.0.0.1"	*/"10.0.41.102"
// #define SERVERPORT			5556////通信端口
// #define SERVERIP			"127.0.0.1"
//#include <json/json.h>
#include "FontInfo.h" 
#include "DBWriterThread.h"
#include "HeartBeatThread.h"
//#include "FriendUpdateThread.h"
#include<WinSock2.h>
#include "ConnectRequest.h"
#pragma comment(lib, "ws2_32.lib")

void HandleString(std::string & str);

class CIMSocket
{
	
public:
	CIMSocket(void);
	~CIMSocket(void);
	void Create(HWND hwnd);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	enum E_SERVERTPE
	{
		E_NOSERVERCONNECT = -1,
		E_LOGINSERVER,
		E_MSGSERVER
	};
	enum E_FROMORTOTYPE
	{
		E_FROMFRIEND = 0,   //收到 消息
		E_TOFRIEND			//发送到当前的用户
	};

private:
	SOCKET  m_sock;			//客户端连接SOCKET
	HWND    m_hWnd;     //消息接收窗口句柄
	
	CHAR	m_sendBuf[MAX_SEND_BUF]; //发送缓冲区
	CHAR    m_recvBuf[MAX_NUM_BUF];  //接收缓冲区
	CHAR    m_tempBuf[MAX_NUM_BUF]; // 临时缓冲区,存储一个Json接收完后可能黏在后面多余的字节
	int     m_nTempLen;				//临时缓冲区存储的字节长度
	BOOL    m_bConnecting;		//是否连接
	BOOL    m_bFinished;       // 接收一个包是否完成
	int     m_nLength;			 //包长度
	int     m_nPos;				//合包是上一次合包结束的位置
	char*	m_pszJson;    //接收到的json包
	E_SERVERTPE m_serverType;  //消息服务器类型
	//std::string m_strToken; //
	CDBWriterThread*  m_pThread; //写消息线程类指针
	CHeartBeatThread*  m_pHeartBeatThread;  // 心跳线程
	//CFriendUpdateThread* m_pFriendUpdateThread;
	int					m_nReply;  //心跳返回
	std::string  m_strLoginJson;    //登陆json 
	char			m_szLoginServerIp[24];
	int          m_nLoginPort;  
	//std::string   m_strLoginId; // 为login Response 返回的memberid 
	ConnectRequest m_connnectRequest;
	int m_nRecvLen; // 已经接收到的长度
	BOOL m_bContinue ;  // 一个Json分多次接收是否继续
public:
	void SendData(const Json::Value& val);
	void SendData(const std::string & str);

	BOOL InitLoginServer();
	void  SendLoginMsg(const Json::Value& val);
	void ExitSocket(void);

	void SetMsgfont(const CFontInfo& f);
private:
	
	BOOL InitSocket(E_SERVERTPE type, int port = SERVERPORT, LPCSTR lpcStr = SERVERIP);
	BOOL InitMsgSock(int port, const char* strip);
	void InitDBWirterThread();
	void InitHeartBeatThread();
	void ReceiveData(void);
	void HandleStickPackeage(BOOL bWhenNewRecv = TRUE);
	void Close(void);
	void Pack(Json::Value& val, const char*  str);
	void HandleJson(const Json::Value& val);
	void HandleNormalMsg(const Json::Value& val);
	void HandleLoginResponseMsg(const Json::Value& val);
	void HandleLogoutResponseMsg(const Json::Value & val);
	void HandleConnectResponseMsg(const Json::Value& val);
	//void HandleSelfInfo(const J)
	void HandleHeartBeatResponse(const Json::Value& val);
	void HandleMsgOfDelay(const Json::Value& val);
	void HandleProfileResponse(const Json::Value& val);
	void HandleOfflineMsgs(const Json::Value& val);
	void HandleFriendGroup(const Json::Value& val);
	void HandleMemberStatus(const Json::Value& val);
	void HandleRecentContacts(const Json::Value& val);
	inline void ByteToInt(int* pVal,char* psz, int size = 4); 
	inline void IntToByte(char* pDest, int* pSrc, int size = 4);
	//将时间戳（精确到秒）转为当前的时间字符串
	//void TimestampToLocalTime(WCHAR** dest, long tick);
	//void TimestampToLocalTime(char* dest, int len,  long tick);
	void MakeMsgBuf(const Json::Value & val, E_FROMORTOTYPE fromorto);
	void MakeMsgBuf(NormaMsgCollection* pMsgCollection);

	msgfont   m_msgfont;
	msgfont& GetMsgfont();
};

