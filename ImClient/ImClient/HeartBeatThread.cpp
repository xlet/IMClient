#include "StdAfx.h"
#include "IMSocket.h"
#include "HeartBeatThread.h"
#include "HeartBeatRequest.h"
extern const char*  const_type_heartbeat_request;
extern const char* const_msgtype_heartbeat_request;
CHeartBeatThread::CHeartBeatThread(void)
{
	//m_heartbeatRequest.Init(const_type_heartbeat_request,const_msgtype_heartbeat_request);
}


CHeartBeatThread::~CHeartBeatThread(void)
{
}

BOOL CHeartBeatThread::Run()
{
	Json::Value val;
	m_heartbeatRequest.m_bIsReply = true;
	m_heartbeatRequest.MakeJson(val,const_type_heartbeat_request,const_msgtype_heartbeat_request);
	G_IMSOCKET->SendData(val);
	m_heartbeatRequest.m_nSeq++;
	Sleep(30000);
	return TRUE;
}
