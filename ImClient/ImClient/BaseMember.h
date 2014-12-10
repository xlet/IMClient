#pragma once
//联系人基类使用继承体系更易于扩展
#include "..\\DataStorage\LocalData.h"

#define  OAMEMBERSRC "OA"
#define  WCNMEMBERSRC "WCN"
#define  TEMPMEMBERSRC "TEMP"
#define  BASEMEMBERSRC  "BASE"
class IMember
{
	virtual void FromJson(const Json::Value& val) = 0;
};

//friendsinfo结构包含一个好友的信息， 不论是OA用户还是WCN用户信息都包含在这个结构中
// 可以冲设计DataStorage中数据结构
class BaseMember:public IMember
{
public:
	BaseMember(void);
	virtual ~BaseMember(void);
	virtual LPCSTR GetMemberType();
public:

	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();
	//void SetTag_(UINT_PTR tag_);
	//UINT_PTR GetTag_();
	virtual void FromJson(const Json::Value& val);
	//从数据库表对应结构获取
	virtual void FromFriendInfo(const friendsinfo& f);
	//转化为对应的friendsinfo结构
	virtual void GetFriendInfo(friendsinfo& f);
	std::string  m_strType;
	std::string m_strMemmberSource;
	std::string m_strId;
	std::string m_strNickName;
	std::string m_strSex;
	std::string m_strPicUrl;
	std::string m_strSignature;
	bool		 m_bTempmember;
	std::string m_strMobile;
	std::string m_strTelephone;
	std::string m_strAddress;
	std::string m_strEmail;
	bool        m_bInit;
protected: 	
	UINT_PTR m_pTag;
};

