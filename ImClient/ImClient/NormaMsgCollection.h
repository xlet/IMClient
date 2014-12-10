#pragma once
#include "NormalMsg.h"
//将一个Vectore或者Map写在类中便于指针的管理
//
class NormaMsgCollection
{
public:
	NormaMsgCollection(void);
	~NormaMsgCollection(void);
	vector <NormalMsg*>& GetMessageCollection();
	vector <NormalMsg*>  m_vcNormalMsg ;
};

typedef std::map<std::string , NormaMsgCollection*> MAP_STRING_PNORMALMSGCOLLECTION ;

class NormaMsgCollectionBase
{
public:
	NormaMsgCollectionBase();
	virtual ~NormaMsgCollectionBase();
	MAP_STRING_PNORMALMSGCOLLECTION& GetMesageCollectionMap();
public:
	virtual void Insert(NormalMsg* p);
	void EraseAndRelease(MAP_STRING_PNORMALMSGCOLLECTION::iterator it);
	MAP_STRING_PNORMALMSGCOLLECTION m_normalmsgMap;
};

