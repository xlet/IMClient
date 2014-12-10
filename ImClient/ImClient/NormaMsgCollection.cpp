#include "StdAfx.h"
#include "NormaMsgCollection.h"
#include "Global.h"

NormaMsgCollection::NormaMsgCollection(void)
{
}


NormaMsgCollection::~NormaMsgCollection(void)
{
	for (vector<NormalMsg*>::iterator it = m_vcNormalMsg.begin(); it
		!= m_vcNormalMsg.end();  ++it)
	{
		NormalMsg* p = *it;
		SAFE_DELETE(p);
	}
	m_vcNormalMsg.clear();
}

vector <NormalMsg*>& NormaMsgCollection::GetMessageCollection()
{
	return m_vcNormalMsg;
}

NormaMsgCollectionBase::NormaMsgCollectionBase()
{
	
}

NormaMsgCollectionBase::~NormaMsgCollectionBase()
{
	OutputDebugString(_T("NormaMsgCollectionBase destructor \n"));
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = m_normalmsgMap.begin(); it != m_normalmsgMap.end(); it++)
	{
		NormaMsgCollection* p = it->second;
		SAFE_DELETE(p);
	}
	m_normalmsgMap.clear();
}

MAP_STRING_PNORMALMSGCOLLECTION& NormaMsgCollectionBase::GetMesageCollectionMap()
{
	return m_normalmsgMap;
}

void NormaMsgCollectionBase::Insert(NormalMsg* p)
{
	string  strKeyId;
	if (p->m_strFromId == /*g_user.m_userInfo*/g_user.UerSelf()->m_strId)
	{
		strKeyId = p->m_strtoId;
	}
	else
	{
		strKeyId = p->m_strFromId;
	}
	MAP_STRING_PNORMALMSGCOLLECTION::iterator it = m_normalmsgMap.find(strKeyId);
	if (it != m_normalmsgMap.end())
	{
		//ÕÒµ½
		it->second->m_vcNormalMsg.push_back(p);
	}
	else
	{
		NormaMsgCollection* pCollection = new NormaMsgCollection;
		pCollection->m_vcNormalMsg.push_back(p);
		m_normalmsgMap.insert(make_pair(strKeyId, pCollection));
	}
}

void NormaMsgCollectionBase::EraseAndRelease(MAP_STRING_PNORMALMSGCOLLECTION::iterator it)
{
	NormaMsgCollection* p = it->second;
	SAFE_DELETE(p);
	m_normalmsgMap.erase(it);
}