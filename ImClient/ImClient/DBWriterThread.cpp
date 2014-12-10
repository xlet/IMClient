#include "StdAfx.h"
#include "DBWriterThread.h"


//临界区 保证 发送消息时 存储消息的过程
CRITICAL_SECTION g_sendcs ;

//存进sqlite中的一些字符串需要加上转移字符
void HandleString(string & str)
{
	char* str1 = "'";
	char* str2 = "''";
	string::size_type  pos = 0;
	while((pos = str.find(str1, pos)) != string::npos)
	{
		str.replace(pos, strlen(str1), str2 );
		pos += strlen(str2);
	}
}


CDBWriterThread::CDBWriterThread(void)
{
	m_recvBufStatus = BUF_IDLE;
	m_sendBufStatus = BUF_IDLE;
	m_bQueueWritinng = FALSE;
	InitializeCriticalSection(&g_sendcs);
}


CDBWriterThread::~CDBWriterThread(void)
{
	DeleteCriticalSection(&g_sendcs);
	if (!m_normalmsgQueue.empty())
	{
		for (int i=0; i < m_normalmsgQueue.size(); i++)
		{
			msginfo* p =m_normalmsgQueue.front();
			SAFE_DELETE(p);
			m_normalmsgQueue.pop();
		}
	}
}

BOOL CDBWriterThread::Run()
{
// 	if (m_recvBufStatus != BUF_READYTOREAD)
// 	{
// 		return TRUE;
// 	}
#if 0
	CSqlite*   p = CSqlite::GetIntstance();
	if (m_recvBufStatus == BUF_READYTOREAD)
	{
		m_recvBufStatus  = BUF_READING;   //此时进入写状态
		PMSGBUF pMsgBuf = (PMSGBUF)m_pPara;
		p->GetMsgDbInterface()->AddMsg(pMsgBuf->recvmsg);
		//写完
		m_recvBufStatus = BUF_IDLE; //将缓冲区标志置为空闲
		Sleep(1);
	}
	if (m_sendBufStatus == BUF_READYTOREAD)
	{
		EnterCriticalSection(&g_sendcs);
		m_sendBufStatus =  BUF_READING;
		PMSGBUF pMsgBuf = (PMSGBUF)m_pPara;
		p->GetMsgDbInterface()->AddMsg(pMsgBuf->sendmsg);
		m_sendBufStatus = BUF_IDLE;
		LeaveCriticalSection(&g_sendcs);
	}

	Sleep(1);
	return TRUE;
#endif 
	msginfo* pMsgInfo = NULL;
	EnterCriticalSection(&g_sendcs);
	if (m_normalmsgQueue.size() != 0)
	{
		pMsgInfo = m_normalmsgQueue.front();
		HandleString(pMsgInfo->msgcontent);
		m_normalmsgQueue.pop();
		LeaveCriticalSection(&g_sendcs);

		CSqlite::GetIntstance()->GetMsgDbInterface()->AddMsg(*pMsgInfo);
		SAFE_DELETE(pMsgInfo);
		Sleep(20);
	}
	else
	{
		LeaveCriticalSection(&g_sendcs);
		Sleep(100);//Sleep(50);
	}
	return TRUE;
}
