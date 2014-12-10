#include "StdAfx.h"
#include "FriendUpdateThread.h"
#include "Sqlite.h"
#include "FriendGroup.h"
#include "Global.h"
#include "WOAMember.h"

CRITICAL_SECTION_EX CFriendUpdateThread::s_cs;

CFriendUpdateThread::CFriendUpdateThread(void)
{
}


CFriendUpdateThread::~CFriendUpdateThread(void)
{
}

//注意：
//在多线程更新本地数据库， 服务端如果发送的是好友及其组的变更， 那么
//在此处只需要将接收到的变更更新到本地数据库， 由于目前服务器每次都会
//发送所有的联系人、群组所以此处每次都会将接收的联系人和群组存储在sqlite
//数据库中
BOOL CFriendUpdateThread::Run()
{
	//Sqlite 目测不支持在一个事务中再开启一个事务

	s_cs.Lock();
	FriendGroup* p = (FriendGroup*)m_pPara;
	map<tString, BaseMember*>& memberMap = p->GetFriendGroupInfo().GetMemberMap();
	IMsgDb* pIMsgDb = CSqlite::GetIntstance()->GetMsgDbInterface();

	pIMsgDb->BeginTransaction();
	group g;
	g.id = p->GetFriendGroupInfo().m_strGroupId;
	g.name = p->GetFriendGroupInfo().m_strName;
	g.system = p->GetFriendGroupInfo().m_bSystem;
	pIMsgDb->AddGroup(g);
	friendsinfo f;
	// BaseMember* pMember = NULL;
	for (map<tString, BaseMember*>::iterator it = memberMap.begin(); it!=memberMap.end(); ++it)
	{
		//pMember = static_cast<WOAMember*>(it->second);
		it->second->GetFriendInfo(f);

// 		f.type = pMember->m_strType;
// 		f.membersource = pMember->m_strMemmberSource;
// 		f.id = pMember->m_strId;
// 		f.nickname = pMember->m_strNickName;
// 		f.sex = pMember->m_strSex;
// 		f.picurl = pMember->m_strPicUrl;
// 		f.signature = pMember->m_strSignature;
// 		f.tempmember = pMember->m_bTempmember;
// 		f.mobile = pMember->m_strMobile;
// 		f.telephone = pMember->m_strTelephone;
// 		f.address = pMember->m_strAddress;
// 		f.email = pMember->m_strEmail;
// 		f.init = pMember->m_bInit;
// 		f.post = pMember->m_strPost;
// 		f.department = pMember->m_strDepartment;
		pIMsgDb->AddFriend(f);
	}
	pIMsgDb->EndTransaction();
	s_cs.UnLock();

	delete this;
	return FALSE;
}