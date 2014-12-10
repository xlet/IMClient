#include "StdAfx.h"
#include "IMSocket.h"

//#include "MsgData.h"
#include "MsgOfDelay.h"
#include "ProfileRequest.h"
#include "ProfileResponse.h"
#include "MsgOfArrival.h"
#include "LoginResponse.h"
#include "ConnectResponse.h"
#include "FriendGroup.h"
#include "SourceDownloadThread.h"
extern const char* const_type;
extern const char* const_msgtype ;
extern const char* const_type_normal_msgval ;
extern const char* const_type_login_responseval;
extern const char* const_type_logout_responseval;
extern const char* const_token;
extern const char* const_type_connect_requestval;
extern const char* const_msgtype_connect_requestval;
extern const char* const_type_connect_responseval;
extern const char* const_allocatedMsgServ ;
extern const char* const_host ;
extern const char* const_port ;
extern const char* const_msgcontent ;
extern const char* cosnt_errorcode ;
extern const char* const_errorMsg ;
extern const char* const_fromid;
extern const char* const_toid ;
extern const char* const_memberid ;
extern CRITICAL_SECTION g_sendcs ;

// 客户端尽量少用多线程

//全局变量消息缓冲区 
MsgBuf  msgBuf;
MsgBuf  msgBugSend;

//将数据存到sqlite之前 都需要处理
// void HandleString(std::string & str)
// {
// 	char* str1 =  "'";
// 	char* str2 = "''";
// 	std::string::size_type  pos = 0;
// 	while((pos = str.find(str1, pos)) != std::string::npos)
// 	{
// 		str.replace(pos, strlen(str1), str2 );
// 		pos += strlen(str2);
// 	}
// }

//将消息json存进缓冲结构体
 //需要在界面线程中加锁
void  CIMSocket::MakeMsgBuf(const Json::Value & val, E_FROMORTOTYPE fromorto)
{
#if 0 
	if (val[const_type].asString() != const_type_normal_msgval)
	{
		OutputDebugString(_T("Not MsgJson \n"));
		return;
	}

	msginfo*  pMsgBug = NULL;
	//pMsgBug = fromorto==E_TOFRIEND? &msgBugSend:&msgBufRecv;
	if (fromorto == E_FROMFRIEND) 
	{
		//收到 消息
		//对于接收到消息， m_recvBufStatus 不会为为BUF_IDLE 
		//原因在于， 若接收的时候缓冲区繁忙，则一直接收0 个字节
		//也就是不继续想sockect缓冲区读取字节
		if (m_pThread->m_recvBufStatus != BUF_IDLE)
		{
			return ;
		}
		pMsgBug = &(msgBuf.recvmsg);
	}
	else  //E_TOFRIEND
	{
		//发送到消息，消息缓冲区的填充放在
		//临界区， 保证其原子性
		pMsgBug = &(msgBuf.sendmsg);
		while(m_pThread->m_sendBufStatus != BUF_IDLE)
		{
			//将时间片交给工作线程
			//不等于BUF_IDLE 表示缓冲去还没空闲
			Sleep(0);
		}
		EnterCriticalSection(&g_sendcs); 
	
	}
	
	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	


	if (fromorto == E_FROMFRIEND)
		m_pThread->m_recvBufStatus = BUF_WRITING;
	else
		m_pThread->m_sendBufStatus = BUF_WRITING;
	
	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //当期用户发出去的消息
	{
		pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
		pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //消息ID在数据库中自增长

	//此时将缓冲区标志设为可读的

	if (fromorto == E_FROMFRIEND)
		m_pThread->m_recvBufStatus = BUF_READYTOREAD;
	else
		m_pThread->m_sendBufStatus = BUF_READYTOREAD;
	if (fromorto == E_TOFRIEND)
	{
		LeaveCriticalSection(&g_sendcs);
	}
	
	Sleep(0);
#endif
	//正常聊天时候将消息存储在队列中
	if (val[const_type].asString() != const_type_normal_msgval)
	{
		OutputDebugString(_T("Not MsgJson \n"));
		return;
	}

	msginfo*  pMsgBug =  new msginfo;
	//pMsgBug = fromorto==E_TOFRIEND? &msgBugSend:&msgBufRecv;
	if (fromorto == E_FROMFRIEND) 
	{
		//收到好友或者联系人的消息
	}
	else  //E_TOFRIEND
	{
		//发送到好友或者联系人的信息
	}

	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //当前用户发出去的消息
	{
		pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
		pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //消息ID在数据库中自增长
	pMsgBug->font = GetMsgfont();
	EnterCriticalSection(&g_sendcs); 
	m_pThread->m_normalmsgQueue.push(pMsgBug);
	LeaveCriticalSection(&g_sendcs);

	//此时将缓冲区标志设为可读的
}

//////////////////////////////////////////////////////////////////////////////////////////////

CIMSocket::CIMSocket(void)
{
	memset(m_sendBuf, 0x0, sizeof(m_sendBuf));
	memset(m_recvBuf, 0x0, sizeof(m_recvBuf));
	memset(m_tempBuf, 0x0, sizeof(m_tempBuf));
	m_bConnecting = FALSE;
	m_nLength  = 0;
	m_nTempLen = 0;
	m_bFinished = TRUE;
	m_hWnd = NULL;
	m_pszJson = NULL;
	m_nPos = 0;
	m_serverType =E_NOSERVERCONNECT;
	//m_pThread = NULL;
	m_nReply = 0;  //心跳返回 初始化为0 
	m_pHeartBeatThread = NULL;
	m_pThread = NULL;
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszServ[24] = {0x0};
	pIni->ReadString(_T("loginserver"), _T("value"), NULL, tszServ, 23);
	char* szServ = NULL;
	T2C(&szServ, tszServ);
	strcpy_s(m_szLoginServerIp, szServ);
	SAFE_ARRYDELETE(szServ);
	m_nLoginPort = pIni->ReadInt(_T("loginport"), _T("value"), NULL);
	m_nRecvLen = 0;
	m_bContinue = FALSE;
	SetMsgfont(CFontInfo());
}


CIMSocket::~CIMSocket(void)
{
	m_pThread->StopThread();
	m_pHeartBeatThread->StopThread();
	//m_pFriendUpdateThread->StopThread();
	SAFE_ARRYDELETE(m_pszJson);
	SAFE_DELETE(m_pThread);
	SAFE_DELETE(m_pHeartBeatThread);
}

void CIMSocket::Create(HWND hwnd)
{
	m_hWnd = hwnd;
	InitDBWirterThread();
}

BOOL CIMSocket::InitSocket(E_SERVERTPE type, int port/* = SERVERPORT*/, LPCSTR lpcStr/* = SERVERIP*/)
{
	m_serverType = type;
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2,2), &wsData) != 0)
	{
		return FALSE;
	}
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(INVALID_SOCKET == m_sock)
	{
		return FALSE;
	}

	sockaddr_in  serverAddr;
	serverAddr.sin_family = AF_INET;
	if (type== E_LOGINSERVER)
	{
		serverAddr.sin_port = htons(m_nLoginPort);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(m_szLoginServerIp);
	}
	else
	{
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(lpcStr);
	}
	
	if (WSAAsyncSelect(m_sock,m_hWnd, WM_SOCK_CONNECT, FD_CONNECT) == SOCKET_ERROR)
	{
		OutputDebugString(_T("SOCKET ERROR \n"));
		return FALSE;
	}

	//此时连接是异步的， 需要在WM_SOCK_CONNECT中判断是否已经连接
	connect(m_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	return TRUE;
}

BOOL CIMSocket::InitLoginServer()
{
	if (m_bConnecting == TRUE)
	{
		return FALSE;
	}
	if (!InitSocket(E_LOGINSERVER))
	{
		ExitSocket();
		return FALSE;
	}
	return TRUE;
}

BOOL  CIMSocket::InitMsgSock(int port, const char* strip)
{
	return 0;
}

//关闭套接字， 清除资源
void CIMSocket::ExitSocket(void)
{
#if _DEBUG
	if (m_serverType == E_LOGINSERVER)
	{
		OutputDebugString(_T("LOGIN　SERVER DISCONNECTED ,CLOSE  SOCKET\n"));
	}
	else
	{
		OutputDebugString(_T("MSG　SERVER DISCONNECTED , CLOSE  SOCKET\n"));
	}
#endif
	m_serverType = E_NOSERVERCONNECT;
	closesocket(m_sock);
	m_bConnecting = FALSE;
	WSACleanup();
}

LRESULT CIMSocket::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//网络错误
	bHandled = TRUE;
	int nErrorCode = WSAGETSELECTERROR(lParam);
	if (WSAGETSELECTERROR(lParam))
	{
		//WSAEHOSTDOWN
		if (nErrorCode ==WSAEHOSTDOWN)
		{
			OutputDebugString(_T(" SOCKET CLOSED \n"));
			//网络断开
			//m_bConnecting = FALSE;
			//closesocket()
		}
		ExitSocket();
		return 0;
	}
	int wPEvent = WSAGETSELECTEVENT(lParam);
	switch(uMsg)
	{
	case WM_SOCK_CONNECT:
		{
// 			if (FD_WRITE ==wPEvent)
// 			{
// 				//SendData();
// 			}
			if(FD_CONNECT == wPEvent)
			{
				//成功连接， 
				
#if 1
				if (m_serverType == E_LOGINSERVER)
				{
					OutputDebugString(_T("LOGIN SERVER CONNECT SUCCEDDED\n"));
					m_bConnecting = TRUE;
					SendData(m_strLoginJson);
				}
				else
				{
					OutputDebugString(_T("MSG SERVER CONNECT SUCCEDDED\n"));
					//必须确连接上了消息服务器
					//才有必要初始化数据库写线程
					InitDBWirterThread() ; 
					InitHeartBeatThread();
					//以下是测试
// 					ConnectRequest request;
// 					//request.Init(const_type_connect_requestval, const_msgtype_connect_requestval);
// 					request.m_strLoginId = m_strLoginId;//g_user.m_userInfo.id/*"13622882929"*/;
// 					request.m_strToken = m_strToken;
					
					Json::Value val;
					m_connnectRequest.MakeJson(val,const_type_connect_requestval,const_msgtype_connect_requestval);
					SendData(val);
				}
				
#endif
				//重新启动了一个WSAAsyncSelect先前设置作废
				if (WSAAsyncSelect(m_sock, m_hWnd, WM_SOCK_RWORCLOSE,FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
				{
					ExitSocket();
					OutputDebugString(_T("WSAAsyncSelect ERROR \n"));
				}
			}
			break;
		}
	case WM_SOCK_RWORCLOSE:
		{
			if (FD_WRITE == wPEvent)
			{
				//准备好发送
			}
			else if (FD_READ == wPEvent)
			{
				//准备好接收数据
				ReceiveData();
			}

			else if (FD_CLOSE == wPEvent)
			{
				//关闭
				Close();
			}
			break;
		}
	default: 
			bHandled = FALSE;
			break;
	
	}
	return 0;
}

void CIMSocket::InitDBWirterThread()
{
	//初始化线程类
	m_pThread = new CDBWriterThread();
	//设置参数, 传入缓冲区结构体指针
	m_pThread->SetPara((UINT_PTR)(&msgBuf));
	m_pThread->StartThread();
}

void CIMSocket::InitHeartBeatThread()
{
	m_pHeartBeatThread = new CHeartBeatThread;
	//m_pHeartBeatThread->StartThread();
}

#ifdef _DEBUG

void PrintSendOrRecvStr(const char* buf , int len, const TCHAR* type)
{

	char* pOut = new char[len+1];
	memset(pOut, 0x0, len+1);
	memcpy(pOut, buf, len);
	TCHAR* ptOut = NULL;
	
	C2T(&ptOut, pOut);
	OutputDebugString(type);
	OutputDebugString(ptOut);
	SAFE_ARRYDELETE(pOut);
	SAFE_ARRYDELETE(ptOut);
}
#endif

void CIMSocket::HandleStickPackeage(BOOL bWhenNewRecv /* = TRUE */)
{
	if (m_nTempLen > 0)
	{
		// 出现断包需要处理
		int nLen1 = 0;// 已经处理过字节长度

		int nRemain = m_nTempLen; // 断包剩余长度
		while (nLen1 < m_nTempLen)
		{
			int nLen2 = 0;   // 每个Json包长度

			if (m_nTempLen-nLen1 < 4)
			{
				//剩下小于4个字节无法计算长度
				char* pTempBuf = new char[m_nTempLen - nLen1];
				memcpy(pTempBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
				memset(m_tempBuf, 0x0, MAX_NUM_BUF);
				m_nTempLen = m_nTempLen - nLen1;
				memcpy(m_tempBuf, pTempBuf, m_nTempLen);
				SAFE_ARRYDELETE(pTempBuf);
				break;
			}
			else
			{
				ByteToInt(&nLen2, m_tempBuf + nLen1);

				//JSon包 加上4个字节的长度
				if (nLen1 + nLen2+4 > m_nTempLen)
				{
					//断包
					char* pTempBuf = new char[m_nTempLen - nLen1];
					memcpy(pTempBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); //
					memset(m_tempBuf, 0x0, MAX_NUM_BUF);
					m_nTempLen = m_nTempLen - nLen1;
					memcpy(m_tempBuf, pTempBuf,m_nTempLen );
					SAFE_ARRYDELETE(pTempBuf);
					break;
				}
				else
				{
					m_pszJson = new char[nLen2 + 1];
					memset(m_pszJson, 0, nLen2 + 1);
					memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
					Json::Value val;
					if (!m_pszJson)
					{

					}
					Pack(val, m_pszJson);
					HandleJson(val);
	
					//m_nTempLen = 0;
				}
				nLen1  += nLen2 +4;
			}
		}
		if(nLen1 == m_nTempLen)
			m_nTempLen = 0;
	}
}


/*
	
*/
void CIMSocket::ReceiveData()
{
	memset(m_recvBuf, 0, MAX_NUM_BUF);
	//int nRecvLen = 0;
	int retVal = -1;
	//不需要循环接收，未接收完的系统会自动通知
	if (m_bConnecting)
	{
		if (m_nTempLen > 0)
		{
			// 出现断包需要处理
			int nLen1 = 0;// 已经处理过字节长度
			
			int nRemain = m_nTempLen; // 断包剩余长度
			while (nLen1 < m_nTempLen)
			{
				int nLen2 = 0;   // 每个Json包长度

				if (m_nTempLen-nLen1 < 4)
				{
					//剩下小于4个字节无法计算长度
					memcpy(m_recvBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
					m_nPos = m_nTempLen - nLen1;
					break;
				}
				else
				{
					ByteToInt(&nLen2, m_tempBuf + nLen1);

					//JSon包 加上4个字节的长度
					if (nLen1 + nLen2+4 > m_nTempLen)
					{
						//断包
						memcpy(m_recvBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); // 写进接收buffer
						m_nPos = m_nTempLen-nLen1 ; // 标志位置
					}
					else
					{
						m_pszJson = new char[nLen2 + 1];
						memset(m_pszJson, 0, nLen2 + 1);
						memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
						Json::Value val;
						if (!m_pszJson)
						{

						}
						Pack(val, m_pszJson);
						HandleJson(val);
					}
					nLen1  += nLen2 +4;
				}
			}

			m_nTempLen = 0;	
		}
		else
			m_nPos = 0;

		retVal = recv(m_sock, m_recvBuf + m_nPos, MAX_NUM_BUF - m_nPos, 0);

		if (SOCKET_ERROR == retVal)
		{
			int nErrCode = WSAGetLastError();
			return;
		}
		else if (0 == retVal)
		{
	
			//服务器关闭连接
			return;
		}
		else if (retVal > 0 )
		{
			//接收到的数据大小
		}
	}
	////////////////////////

	if (retVal >0)
	{
		if (m_bContinue)
		{ // 一个Json  没接收完， 继续接收
			if (m_nLength > m_nRecvLen + retVal  + m_nPos)
			{
				memcpy(m_pszJson + m_nRecvLen, m_recvBuf, retVal);
				m_nRecvLen += retVal;
			}
			else
			{
				memcpy(m_pszJson + m_nRecvLen,m_recvBuf, m_nLength - m_nRecvLen);
				Json::Value val;
				Pack(val, m_pszJson);
				HandleJson(val);
				m_nTempLen = m_nRecvLen + retVal - m_nLength;
				memcpy(m_tempBuf, m_recvBuf + m_nLength - m_nRecvLen, m_nTempLen);
				
				//TODO:  bug here 
				//int x ;
				//ByteToInt(&x, m_tempBuf);
				///////////// Begin 
				HandleStickPackeage();

				/////////////////end
				//
				m_bContinue = FALSE;
				m_nRecvLen = 0;
			}
		}
		else
		{
			//获取当前接收的包的长度，前四个字节为长度
			if (retVal + m_nPos < 4)
			{
				//m_nPos += retVal;
				m_nTempLen = m_nPos + retVal;
				memcpy(m_tempBuf, m_recvBuf, m_nTempLen);
			}
			else
			{
				ByteToInt(&m_nLength, m_recvBuf);
				//为将要接收的json包分配内存空间
				if (!m_pszJson)
				{
					m_pszJson = new char[m_nLength + 1];
					memset(m_pszJson, 0, m_nLength + 1);
				}

				//小于MAX_BUF的情况
				if (m_nLength <= retVal + m_nPos  -4)
				{
					memcpy(m_pszJson, m_recvBuf + 4,m_nLength);
					Json::Value val;
					Pack(val, m_pszJson);
					HandleJson(val);
					m_nTempLen = retVal + m_nPos -m_nLength -4;
					memcpy(m_tempBuf, m_recvBuf + m_nLength + 4, m_nTempLen);
				}
				else 
				{	//包未接收完
					memcpy(m_pszJson + m_nRecvLen,  m_recvBuf + 4,  retVal + m_nPos -4 );
					m_nRecvLen  +=  retVal + m_nPos - 4;
					m_bContinue = TRUE;
				}
			}
		}
	
		
	}
}

#if 0

void CIMSocket::ReceiveData()
{
	memset(m_recvBuf, 0, MAX_NUM_BUF);
	//int nRecvLen = 0;
	int retVal = -1;
	//不需要循环接收，未接收完的系统会自动通知
	if (m_bConnecting)
	{
		if (m_nTempLen > 0)
		{
			// 出现断包需要处理
			int nLen1 = 0;// 已经处理过字节长度

			int nRemain = m_nTempLen; // 断包剩余长度
			while (nLen1 < m_nTempLen)
			{
				int nLen2 = 0;   // 每个Json包长度

				if (m_nTempLen-nLen1 < 4)
				{
					//剩下小于4个字节无法计算长度
					memcpy(m_recvBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
					m_nPos = m_nTempLen - nLen1;
					break;
				}
				else
				{
					ByteToInt(&nLen2, m_tempBuf + nLen1);

					//JSon包 加上4个字节的长度
					if (nLen1 + nLen2+4 > m_nTempLen)
					{
						//断包
						memcpy(m_recvBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); // 写进接收buffer
						m_nPos = m_nTempLen-nLen1 ; // 标志位置
					}
					else
					{
						m_pszJson = new char[nLen2 + 1];
						memset(m_pszJson, 0, m_nLength + 1);
						memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
						Json::Value val;
						Pack(val, m_pszJson);
						HandleJson(val);
					}
					nLen1  += nLen2 +4;
				}
			}

			m_nTempLen = 0;	
		}
		else
			m_nPos = 0;

		retVal = recv(m_sock, m_recvBuf + m_nPos, MAX_NUM_BUF - m_nPos, 0);

		if (SOCKET_ERROR == retVal)
		{
			int nErrCode = WSAGetLastError();
			return;
		}
		else if (0 == retVal)
		{

			//服务器关闭连接
			return;
		}
		else if (retVal > 0 )
		{
			//接收到的数据大小
		}
	}
	////////////////////////

	if (retVal >0)
	{
		if (m_bContinue)
		{ // 一个Json  没接收完， 继续接收
			if (m_nLength > m_nRecvLen + retVal  + m_nPos)
			{
				memcpy(m_pszJson + m_nRecvLen, m_recvBuf, retVal);
				m_nRecvLen += retVal;
			}
			else
			{
				memcpy(m_pszJson + m_nRecvLen,m_recvBuf, m_nLength - m_nRecvLen);
				Json::Value val;
				Pack(val, m_pszJson);
				HandleJson(val);
				m_nTempLen = m_nRecvLen + retVal - m_nLength;
				memcpy(m_tempBuf, m_recvBuf + m_nLength - m_nRecvLen, m_nTempLen);
				m_bContinue = FALSE;
				m_nRecvLen = 0;
			}
		}
		else
		{
			//获取当前接收的包的长度，前四个字节为长度
			if (retVal + m_nPos < 4)
			{
				//m_nPos += retVal;
				m_nTempLen = m_nPos + retVal;
				memcpy(m_tempBuf, m_recvBuf, m_nTempLen);
			}
			else
			{
				ByteToInt(&m_nLength, m_recvBuf);
				//为将要接收的json包分配内存空间
				if (!m_pszJson)
				{
					m_pszJson = new char[m_nLength + 1];
					memset(m_pszJson, 0, m_nLength + 1);
				}

				//小于MAX_BUF的情况
				if (m_nLength <= retVal + m_nPos  -4)
				{
					memcpy(m_pszJson, m_recvBuf + 4,m_nLength);
					Json::Value val;
					Pack(val, m_pszJson);
					HandleJson(val);
					m_nTempLen = retVal + m_nPos -m_nLength -4;
					memcpy(m_tempBuf, m_recvBuf + m_nLength + 4, m_nTempLen);
				}
				else 
				{	//包未接收完
					memcpy(m_pszJson + m_nRecvLen,  m_recvBuf + 4,  retVal + m_nPos -4 );
					m_nRecvLen  +=  retVal + m_nPos - 4;
					m_bContinue = TRUE;
				}
			}
		}


	}
	//////////////


#if 0

	if (m_bFinished)
	{	
		//开始一个新的包的接收
		//获取报长度
		if(strlen(m_recvBuf) < 4)
		{
			//继续接收，计算接收时间，超时重新连接
		}

		//获取当前接收的包的长度，前四个字节为长度
		ByteToInt(&m_nLength, m_recvBuf);

		//为将要接收的json包分配内存空间
		m_pszJson = new char[m_nLength + 1];
		memset(m_pszJson, 0, m_nLength + 1);
		m_bFinished = FALSE;


		if (m_nLength <= retVal -4)
		{
			//需要接收长度小于实际接收到的长度
			//此时会产生一个问题， 多余出来的字符串应该属于下一次的接收，
			//需要考虑的是将多余的字符存储

			m_bFinished = TRUE;

			memcpy(m_pszJson,m_recvBuf+4, m_nLength);
			if (m_nLength < retVal -4)
			{
				//多接收的字节存储在临时缓冲区
				m_nTempLen = retVal - 4 - m_nLength;
				memcpy(m_tempBuf, m_recvBuf+4+m_nLength, m_nTempLen);
			}

			Json::Value val;
#ifdef _DEBUG
			PrintSendOrRecvStr(m_pszJson, m_nLength,  _T("recv message\n") );
#endif
			Pack(val, m_pszJson);
			HandleJson(val);
		}
		else
		{
			memcpy(m_pszJson,m_recvBuf+4, retVal-4);
			//记录第一次接包在缓冲区结束位置
			m_nPos = retVal-4;
		}
	}
	else
	{
		//继续接收，直到收完为止
		if (m_nPos + retVal > m_nLength)
			memcpy(m_pszJson+m_nPos, m_recvBuf, m_nLength - m_nPos);
		//memcpy(m_tempBuf, m_recvBuf+m_nLength - m_nPos +1, m_nTempLen);
		else
			memcpy(m_pszJson+m_nPos, m_recvBuf, retVal);

		m_nPos += retVal;

		if (m_nPos >= m_nLength)
		{
			m_bFinished = TRUE;

			//说明已经接受完毕m_nPos > m_nLength
			if (m_nPos > m_nLength)
			{
				//多接收的字节存储在临时缓冲区，下次此接受先读取m_tempBuf， 注意前四个字节是长度
				m_nTempLen = m_nPos - m_nLength ;   // 多余的字节长度

				// 这时可能存在一个包中前一部分是一个Json的结尾， 另一部分是一个Json的头
				//memcpy(m_tempBuf, m_recvBuf+4+retVal, m_nTempLen);
				memcpy(m_tempBuf, m_recvBuf + retVal -  m_nTempLen , m_nTempLen);
				ByteToInt( &m_nLength, m_recvBuf + retVal -  m_nTempLen);
				//m_bFinished = FALSE;
				m_nPos = 0;
			}

			Json::Value val;
#ifdef _DEBUG
			//PrintSendOrRecvStr(m_pszJson, m_nLength,  _T("recv message") );
#endif
			Pack(val, m_pszJson);
			HandleJson(val);
		}
	}
#endif
}

#endif

//客户端发送数据
void CIMSocket::SendData(const Json::Value& val)
{
	//序列化后在发送
#if NOSERVICETEST
#else
	if (!m_bConnecting)
	{
		return;
	}
#endif
	

	//将消息放到消息队列
	MakeMsgBuf(val, E_TOFRIEND); 
	//将消息放到消息队列
	Json::FastWriter   writer;
	std::string  strOut = writer.write(val);
	 SendData(strOut);
}



void CIMSocket::SendData(const std::string & str)
{
	// m_serverType 可以判断当前连接的服务器

// 	int len = str.length();
// 	IntToByte(m_sendBuf, &len);
// 	memcpy(m_sendBuf+4, str.c_str(),len);
	char* pszVal = NULL;
	AnsiToUtf8(&pszVal, str.c_str());
	int len =  strlen(pszVal);
	IntToByte(m_sendBuf, &len);
	
#if 0
	int ret  = 0;
	memcpy(m_sendBuf+4, pszVal,len);

	ret = send(m_sock, m_sendBuf, len+4, 0);
#endif

#if 1
	int ret  = 0;
	if (len+4 <= MAX_SEND_BUF)
	{
		memcpy(m_sendBuf+4, pszVal,len);
		ret = send(m_sock, m_sendBuf, len+4, 0);
#ifdef _DEBUG

		PrintSendOrRecvStr(m_sendBuf+4, len, _T("send message"));
#endif
		
	}
	else
	{
		//超出缓冲区长度循环发送
		int nCout =  (len + 4) / MAX_SEND_BUF;
		memcpy(m_sendBuf+4, pszVal,MAX_SEND_BUF-4);

#ifdef _DEBUG
		PrintSendOrRecvStr(m_sendBuf+4, MAX_SEND_BUF-4, _T("send message"));
#endif
		ret = send(m_sock, m_sendBuf, MAX_SEND_BUF, 0);

		for (int i=1; i<nCout; i++)
		{
			memcpy(m_sendBuf, pszVal+i*MAX_SEND_BUF -4, MAX_SEND_BUF);
#ifdef _DEBUG
			PrintSendOrRecvStr(m_sendBuf, MAX_SEND_BUF, _T("send message"));
#endif
			ret = send(m_sock, m_sendBuf, MAX_SEND_BUF, 0);
		}
		if ((len+4)%MAX_SEND_BUF != 0)
		{
			memcpy(m_sendBuf, pszVal+nCout*MAX_SEND_BUF-4, (len + 4)%MAX_SEND_BUF);
#ifdef _DEBUG
			PrintSendOrRecvStr(m_sendBuf, (len + 4)%MAX_SEND_BUF,_T("send message"));
#endif 
			ret = send(m_sock, m_sendBuf, (len + 4)%MAX_SEND_BUF, 0);
		}
	}
#endif
	

	if(SOCKET_ERROR == ret)
	{ 
		int nErrorCode =  WSAGetLastError();
		if (WSAEWOULDBLOCK == nErrorCode)
		{
			//此时缓冲已满应该进行处理 等待FD_WRITE
			OutputDebugString(_T("Send Block ,Please wait \n"));
		}
	}
	else
	{

	}
	SAFE_ARRYDELETE(pszVal);
}

void CIMSocket::SendLoginMsg(const Json::Value& val)
{
	if (m_bConnecting)
	{
		SendData(val);
	}
	else
	{
		Json::FastWriter   writer;
		m_strLoginJson = writer.write(val);
	}
}


//关闭socket接收到消息
void CIMSocket::Close()
{
	//ExitSocket();
}

//将接收到的包合并成json格式
void CIMSocket::Pack(Json::Value& val, const char*  str)
{
	Json::Reader reader;
	//Json::Value root;
	std::string strMsg;
	Utf8ToAnsi(strMsg, str);
	if (reader.parse(strMsg.c_str(), val))
	{
		//获取里面内容
		OutputDebugString(_T("STRING TO JSON \n"));
#if _DEBUG
		std::string str1 = val[const_msgtype].asString();
		long  tmstamp = ((long long)(val["sendTime"].asDouble()))/1000;
		TCHAR tstr[24] = {0};
		TimestampToLocalTime(tstr, sizeof(tstr), tmstamp);
#endif 
	}
	SAFE_ARRYDELETE(m_pszJson);
}

/*
	函数名：
	功能：
	输入：
	输出：
	其他：函数处理客户端接收到的消息json
*/
void CIMSocket::HandleJson(const Json::Value& val)
{
	std::string &  strType = val[const_type].asString();
	//const char*  lpcStr = (char*)((val[const_type].asString()).c_str()); //直接如此得到的值lpcStr是空字符串，原因不详
#ifdef _DEBUG
	Json::FastWriter writer;
	string strDeug   = writer.write(val);
	OutputDebugStringA((strDeug + "\n").c_str());
#endif

	if (strcmp(strType.c_str(), const_type_normal_msgval) == 0)
	{
		HandleNormalMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_login_responseval) == 0)
	{//登陆返回, 开始连接消息服务器
		HandleLoginResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_logout_responseval) == 0)
	{//退出应答
		HandleLogoutResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_connect_responseval) == 0)
	{
		//连接消息服务器返回
		HandleConnectResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), "HeartbeatResponse") == 0)
	{
		HandleHeartBeatResponse(val);
	}
	else if (strcmp(strType.c_str(), "GetProfileResponse") == 0)
	{
		HandleProfileResponse(val);
	}
	else if (strcmp(strType.c_str(), "FriendGroup") == 0)
	{
		HandleFriendGroup(val);
	}
	else if(strcmp(strType.c_str(), "Offline") == 0)
	{
		//MessageBox(m_hWnd, _T("Offline"), _T("d"), MB_OK);
		HandleOfflineMsgs(val);
	}
	else if (strcmp(strType.c_str(), "Status") == 0)
	{
		HandleMemberStatus(val);
	}
	else if (strcmp(strType.c_str(), "RecentContacts") == 0)
	{
		HandleRecentContacts(val);
	}
}


//普通的聊天消息
void CIMSocket::HandleNormalMsg(const Json::Value& val)
{
	//接收到的是普通的聊天消息
	//std::string strMsg = val["content"].asString();
	OutputDebugString(_T("Receive Normal message \n"));

	//发送到对应的窗口
	//std::string strFriendId= val[const_fromid].asString().c_str();
	TCHAR* ptszId = NULL;
	C2T(&ptszId, val[const_fromid].asString().c_str());
	tString tstrFriendId = ptszId;
	SAFE_ARRYDELETE(ptszId);
	MAP_TSTRING_HWND::iterator it = g_mapWnd.find(tstrFriendId);

	//先存储
	MakeMsgBuf(val,E_FROMFRIEND);

	NormalMsg* pNormalMsg = new NormalMsg;
	pNormalMsg->FromJson(val);
	
	//更新最近联系人列表
	SendMessage(m_hWnd, WM_RECENTCONTACTS, 0 , (LPARAM)pNormalMsg);

	if (it == g_mapWnd.end())
	{
		//没找到这个ID，需要重新创建一个窗口并添加
		//如果是客服的话只有一个聊天的主窗口， 所以不需要
		//重新创建窗口
		//对于客服版本，如果好友是第一次发起聊天， 那么需要向服务器请求
		//联系人资料后，将联系人添加到好友列表
		//g_mapWnd.insert(make_pair(tstrFriendId, g_mapWnd.at(tstrFriendId)));
	
		//HandleMsgOfDelay(val);
		SendMessage(m_hWnd, WM_CHATMSGWHENNOWND,  0, (LPARAM)pNormalMsg);
		return;
	}
	else
	
		//找到了, 已经存在就不再添加了
		//直接将消息发过去，
		//接收到的普通的聊天消息
		//MakeMsgBuf(val,E_FROMFRIEND);
		//像改对应的窗口发送消息
		SendMessage(g_mapWnd[tstrFriendId], WM_REICEIVEMSG, 0, (LPARAM)(pNormalMsg));
	

	
}

//登录登录服务器时候的返回， 若成功连接消息服务器
void CIMSocket::HandleLoginResponseMsg(const Json::Value& val)
{
	bool bSuccess = val["success"].asBool();
	LoginResponse loginResponse;
#ifdef  _DEBUG
	Json::FastWriter writer; 
	string strDebug = writer.write(val);
#endif
	loginResponse.FromJson(val);
	if (bSuccess)
	{
		const Json::Value&  servObj = val[const_allocatedMsgServ];
		std::string strServerAddr = servObj[const_host].asString();
		//m_strToken = val[const_token].asString();
		int port = servObj[const_port].asInt();
		//m_strLoginId = servObj[const_memberid].asString();
		m_connnectRequest.m_strToken = val[const_token].asString();
		m_connnectRequest.m_strLoginId = val[const_memberid].asString();

		closesocket(m_sock);
		//"10.0.40.18"
		InitSocket(E_MSGSERVER, loginResponse.m_nPort, loginResponse.m_strHost.c_str()/*"10.0.40.38"*/);
		//InitSocket(E_MSGSERVER, 17041, "10.0.40.10");
	}
	else
	{
		int nErrorCode =   loginResponse.m_nErrorCode;//val["errorCode" ].asInt();
		const string& strError =   loginResponse.m_strErrorMsg;//val["errorMessage"].asString();
		OutputDebugStringA(strError.c_str());
		tString tstrError ;

		if (nErrorCode == 1001)
			tstrError = _T("用户名或者密码错误");
		else if (nErrorCode == 1002)
			tstrError  = _T("已经登录");
		TCHAR* lptError = NULL;
		C2T(&lptError, strError.c_str());
		SendMessage(g_mapWnd[g_loginwndKey],WM_ONLOGINERROR, 0, (LPARAM)tstrError.c_str());
		SAFE_ARRYDELETE(lptError);
#if _DEBUG

		TCHAR  tszError[48] = {0x0};
		wsprintf(tszError, _T("Error code: %d\n"), nErrorCode);
		OutputDebugString(tszError);
#endif
	}
}

void CIMSocket::HandleLogoutResponseMsg(const Json::Value & val)
{
	
}

//处理连接消息服务器返回消息
//返回消息包括：当前用户自己的基本信息“self”及
void CIMSocket::HandleConnectResponseMsg(const Json::Value& val)
{
	//ConnectResponse connectresponse;
	//connectresponse.FromJson(val);
	if (val["success"].asBool())
	{
		OutputDebugString(_T("LOGIN MSG SERVER SUCCEDD \n"));
		//通知登陆窗口已经已经成功登陆
		
		//const Json::Value&  self = val["self"];
		g_user.InitUserInfoFromJson(val); 
		
		//CGlobalData::GetInstance()->m_recentLinkmen.FromJson(val); //客服版本返回的最近联系人在connect response 中
		//登录 成功

		SendMessage(g_mapWnd[g_loginwndKey],WM_LOGINRET, TRUE, 0);
	//HandleOfflineMsgs(val);
	    PostMessage(m_hWnd,WM_SETUSERINFO, TRUE, 0);
		CSourceDownloadThread* pThread = new CSourceDownloadThread;
		SrcPara* pSrcPara = new SrcPara;
		pSrcPara->m_bFirstTimeToDownlod = TRUE;
		pSrcPara->m_hWnd = m_hWnd;
		pSrcPara->type = E_USERSELF;
		//pSrcPara->m_tstrId ;//_T("产品部"); //
		pThread->SetPara((UINT_PTR)pSrcPara);
		pThread->StartThread();
	}
}

//好友组，
void CIMSocket::HandleFriendGroup(const Json::Value& val)
{
	//好友组及其信息存储在FriendList单例指针中
	FriendList::GetInstance()->FromFriendGroup(val);
	//像界面发送消息在界面添加当前组
	tString tstrGroupName ;
	StringTotString( tstrGroupName, val["friendGroup"]["name"].asString());
	SendMessage(m_hWnd, WM_FRIENDGROUP, 0 ,(LPARAM)tstrGroupName.c_str());

	//开启线程，线程中判断是否需要下载，需要下载的话在多线程中进行下载
//#ifdef  _DEBUG
	CSourceDownloadThread* pThread = new CSourceDownloadThread;
	SrcPara* pSrcPara = new SrcPara;
	pSrcPara->m_bFirstTimeToDownlod = TRUE;
	pSrcPara->m_hWnd = m_hWnd;
	pSrcPara->type = E_NORMALMEMBER;
	pSrcPara->m_tstrId = tstrGroupName;//_T("产品部");
	pThread->SetPara((UINT_PTR)pSrcPara);
	pThread->StartThread();
//#endif
}

//当接收到多条消息， 如离线消息，
void CIMSocket::MakeMsgBuf(NormaMsgCollection* pMsgCollection)
{
	/*

	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //当前用户发出去的消息
	{
	pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
	pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //消息ID在数据库中自增长
	*/
#if 1
	int nSize = pMsgCollection->m_vcNormalMsg.size();
	msginfo** MsgInfoArray = new msginfo*[nSize];
	int nIndex = 0;

	char strTime[24] = {0x0};
	for (vector<NormalMsg*>::iterator itMsg = pMsgCollection->m_vcNormalMsg.begin(); itMsg != pMsgCollection->m_vcNormalMsg.end(); ++itMsg, ++nIndex)
	{
	
 		msginfo* pMsg   = new msginfo;
  		TimestampToLocalTime(strTime, 23, (long)((*itMsg)->m_baseMsgInfo.m_dbSendTime/1000));
  
  		pMsg->datetime = strTime;
  		pMsg->id = (*itMsg)->m_strFromId;
  		pMsg->fromorto = E_FROMFRIEND;
  		pMsg->msgcontent = (*itMsg)->m_strContent;
  		pMsg->msgres = "";
  		pMsg->msgid = 0;//消息ID在数据库中自增长
		pMsg->font = GetMsgfont();
  		MsgInfoArray[nIndex] = pMsg;
	}

	EnterCriticalSection(&g_sendcs);
	for (int i = 0; i < nSize; i++)
	{
		m_pThread->m_normalmsgQueue.push(MsgInfoArray[i]);
	}
	LeaveCriticalSection(&g_sendcs);
	
#endif
}

void CIMSocket::HandleOfflineMsgs(const Json::Value& val)
{
	COfflineMsgs & OfflineMsg = CGlobalData::GetInstance()->OfflinMessages();
	//CRecentLinkmen& Recentlinlmen = CGlobalData::GetInstance()->m_recentLinkmen;
	//先读取离线消息
	//PostMessage()
#ifdef _DEBUG
 	Json::FastWriter writer;
 	string strDwug   = writer.write(val);
#endif
	OfflineMsg.FromJson(val);
	tString tstrId;
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = OfflineMsg.m_normalmsgMap.begin(); it != OfflineMsg.m_normalmsgMap.end(); )
	{
		MakeMsgBuf(it->second);
		StringTotString(tstrId, it->first);
		// 更伤心最近联系人
		SendMessage(m_hWnd, WM_RECENTCONTACTS, 0 , (LPARAM)(*(it->second->GetMessageCollection().rbegin())));
		//窗口是否存在
		MAP_TSTRING_HWND::iterator itFind = g_mapWnd.find(tstrId);
		if (itFind != g_mapWnd.end())
		{
			//PostMessage()
			SendMessage(itFind->second, WM_REICEIVEMSG, 1, (LPARAM)(it->second));
			OfflineMsg.m_normalmsgMap.erase(it++);
		}
		else
			++it;
	}

	PostMessage(m_hWnd, WM_OFFLINEMESSAGES, 0, 0);
#if 0
	ProfileRequest profileRequest ;
	

	//在显示消息之前， 先判断该用户 是否存在最近联系人当中
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = OfflineMsg.m_normalmsgMap.begin(); it != OfflineMsg.m_normalmsgMap.end(); )
	{
		MAP_STRING_PLINKMANINFO::iterator itlinkman = Recentlinlmen.m_linkmenMap.find(it->first);
		
		//写进队列
		MakeMsgBuf(it->second);

		if (itlinkman != Recentlinlmen.m_linkmenMap.end())
		{
			// 联系人存在
			// 将消息移进到当前的消息中去同事记得存进消息记录
			
			for (vector<NormalMsg*>::iterator itMsg = it->second->m_vcNormalMsg.begin(); itMsg != it->second->m_vcNormalMsg.end(); ++itMsg)
			{
				MsgOfArrival::GetInstance()->Insert(*itMsg);
			}
			OfflineMsg.m_normalmsgMap.erase(it++);
			
		}
		else
		{
			//联系人不存在
			//消息存在滞留消息对象中
			CMsgOfDelay::GetInstance()->m_normalmsgMap.insert(make_pair(it->first, it->second));
			profileRequest.m_vcId.push_back(it->first);
			OfflineMsg.m_normalmsgMap.erase(it++);
			//向服务发送请求联系人信息
			Json::Value profileRequestJson;
			profileRequest.MakeJson(profileRequestJson , "GetProfileRequest", "GetProfileRequest");
			SendData(profileRequestJson);
		}
	}
#endif
}

void CIMSocket::HandleHeartBeatResponse(const Json::Value& val)
{
	if (val["success"].asBool())
	{
		int nSeq = val["seq"].asInt();
		if (nSeq != m_nReply+1)
		{
			//心跳出错
			OutputDebugString(_T("HeartBeat Missing  error \n"));
		}
		m_nReply=nSeq;
	}
	else
	{
		int nErrorCode = val[cosnt_errorcode].asInt();
#if _DEBUG
		TCHAR tszError[128] = {0x0};
		wsprintf(tszError, _T("HeartBeat error , error code:%d\n"), nErrorCode);
		OutputDebugString(tszError);
#endif
	}
}

//向服务器请求联系人信息返回
//完成后向聊天窗体发送消息开始显示
//商家版才有此流程
void CIMSocket::HandleProfileResponse(const Json::Value& val)
{
	const Json::Value& membersArry = val["members"];
	if (membersArry.isNull() || !membersArry.isArray())
	{
		OutputDebugString(_T("Method:HandleProfileResponse \n;  Error: member is null or not array \n"));
		//return;
	}

	//存到最近联系人中
	//CGlobalData::GetInstance()->m_recentLinkmen.FromJson(val);
	ProfileResponse::GetInstance()->FromJson(val);
	//客服版本对应一个聊天窗口所以对应的窗口句柄都是m_hwnd
	SendMessage(m_hWnd, WM_PROFILERESPONSE, 0, 0);
}

//每次 只处理一个Json., 所以在聊天的过程中中，不会出现多个聊天记录
//在 第一次连接的时候返回的是离线消息可能 会包含多个消息
void CIMSocket::HandleMsgOfDelay(const Json::Value& val)
{
	NormalMsg*  pNormaMsg = new NormalMsg();
	pNormaMsg->FromJson(val);
	MAP_STRING_PNORMALMSGCOLLECTION& NormalMsgMap  = CMsgOfDelay::GetInstance()->m_normalmsgMap;
	//暂时存储延迟的消息（无该用户消息记录， 需要获取信息后在显示）
	if (NormalMsgMap.find(pNormaMsg->m_strFromId)!= NormalMsgMap.end())
	{
		NormalMsgMap.at(pNormaMsg->m_strFromId)->m_vcNormalMsg.push_back(pNormaMsg);
	}
	else
	{
		//无该用户消息记录
		NormaMsgCollection* pCollection = new NormaMsgCollection();
		pCollection->m_vcNormalMsg.push_back(pNormaMsg);
		NormalMsgMap.insert(make_pair(pNormaMsg->m_strFromId, pCollection));
	}
	//NormalMsgMap.insert(make_pair(pNormaMsg->m_strFromId, pNormaMsg));
	//向服务器发送消息获取当前用户信息
	ProfileRequest profileRequest;
	profileRequest.m_vcId.push_back(val[const_fromid].asString());

	Json::Value profileRequestJson;
	profileRequest.MakeJson(profileRequestJson, "GetProfileRequest", "GetProfileRequest");
	SendData(profileRequestJson);
}


void CIMSocket::HandleMemberStatus(const Json::Value& val)
{
 	#ifdef _DEBUG
 	Json::FastWriter writer;
 	string strDwug   = writer.write(val);
 	#endif
 	tString tstrMemberId;
 	int status;
 	StringTotString(tstrMemberId, val["memberId"].asString());
 	status = val["status"].asInt();
	SendMessage(m_hWnd, WM_SETMEMBERSTATUS, status, (LPARAM)tstrMemberId.c_str());
}

void CIMSocket::HandleRecentContacts(const Json::Value& val)
{
	CGlobalData::GetInstance()->RecentContacts().FromJson(val);
	SendMessage(m_hWnd, WM_RECENTCONTACTS, 0, 0);
}

//服务器端采用的是大端存储和发送， 本地内存中采用的

//是小端， 需要进行转化
void CIMSocket::ByteToInt(int* pVal,char* psz,int size )
{
	//int 在32位上是4个字节
	ASSERT(size == 4);
	char* p = (char*)pVal;
	for (int i = 0; i < size; i++)
	{
		*(p+i) = *(psz+size-1-i);
	}
}

void CIMSocket::IntToByte(char* pDest, int* pSrc, int size /* = 4 */)
{
	//服务器端是大端，客户端是小端，所以int的高位放在低地址位
	//
	ASSERT(size == 4);
	char* pSz = (char*)pSrc;
	for (int i = 0; i < size; i++)
	{
		*(pDest + i) = *(pSz+size-1-i);
	}
}

// 
// void CIMSocket::TimestampToLocalTime(WCHAR** dest, long tick)
// {
// //*dest = new WCHAR[24];
// // 	if (dest == NULL)
// // 	{
// // 		return ;
// // 	}
// // 	time_t tm_t = tick;
// // 	char str[24];
// // 	struct tm tm_;
// // 	tm_ = *localtime((time_t*)&tm_t);
// // 	
// // 	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm_);
// 	
// 	char str[24];
// 	TimestampToLocalTime(str, 24, tick);
// 	C2W(dest, str);
// }
// 
// void CIMSocket::TimestampToLocalTime(char* dest, int len, long tick)
// {
// 	if (dest == NULL)
// 	{
// 		return ;
// 	}
// 	time_t tm_t = tick;
// 	//char str[24];
// 	struct tm tm_;
// 	tm_ = *localtime((time_t*)&tm_t);
// 	
// 	strftime(dest, len, "%Y-%m-%d %H:%M:%S", &tm_);
// 
// }


void CIMSocket::SetMsgfont(const CFontInfo& f)
{
	tStringToString(m_msgfont.name,  f.m_tstrName);
	m_msgfont.size  = f.m_nSize;
	m_msgfont.bold = f.m_bBold?true:false;
	m_msgfont.italics = f.m_bItalic?true:false;
	m_msgfont.underline = f.m_bUnderLine?true:false;
	m_msgfont.color = f.m_clrText;
}

msgfont& CIMSocket::GetMsgfont()
{
	return m_msgfont;
}