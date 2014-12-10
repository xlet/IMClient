#pragma once
#include "HeartBeatRequest.h"
#include "Thread.h"

class CHeartBeatThread:public CThread
{
public:
	CHeartBeatThread(void);
	~CHeartBeatThread(void);

private:
	HeartBeatRequest  m_heartbeatRequest;
	virtual BOOL Run();
};

