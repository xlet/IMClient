#pragma once
#include "Thread.h"
#include "Sqlite.h"
#include <queue>
typedef struct  MSGBUF_Tag
{
	msginfo recvmsg;
	msginfo sendmsg;
}MsgBuf, * PMSGBUF;

#define  BUF_WRITING 1  //写
#define  BUF_READING 2   //读
#define  BUF_IDLE 3   //没有写也没有读
#define  BUF_READYTOREAD  4 // 缓冲区写进 准备读

/*
	聊天消息存储线程类
	将消息指针放在队列中， 不断循环存储在sqlite中
*/

class CDBWriterThread:public CThread
{
public:
	CDBWriterThread(void);
	~CDBWriterThread(void);
	volatile int     m_recvBufStatus;  //缓冲区的状态 1、写 2、读、 3、没有写也没有读
	volatile int	 m_sendBufStatus; // 同上
	
	//队列用来存储聊天的消息
	std::queue<msginfo*>  m_normalmsgQueue;
	BOOL             m_bQueueWritinng;
private:
	BOOL Run();
};


