#include "StdAfx.h"
#include "BaseMember.h"


BaseMember::BaseMember(void)
{
	m_pTag = NULL;
//	m_pTag_ = NULL;
}


BaseMember::~BaseMember(void)
{
}

LPCSTR BaseMember::GetMemberType()
{
	return BASEMEMBERSRC;
}

void BaseMember::FromJson(const Json::Value& val)
{
	m_strType = val["@type"].asString();
	m_strMemmberSource = val["memberSource"].asString();
	m_strId = val["id"].asString();
	m_strNickName = val["nickname"].asString();
	m_strSex = val["sex"].asString();
	m_strPicUrl = val["picUrl"].asString();
	m_strSignature = val["signature"].asString();
	m_bTempmember = val["tempMember"].asBool();
	m_strMobile = val["mobile"].asString();
	m_strTelephone =val["telephone"].asString();
	m_strAddress = val["address"].asString();
	m_strEmail = val["email"].asString();
	m_bInit  = val["init"].asBool();
}

void  BaseMember::FromFriendInfo(const friendsinfo& f)
{
	//BaseMember通用的一些字段
	//这些操作写在一个函数中更好
	m_strType = f.type;
	m_strMemmberSource = f.membersource;
	m_strId = f.id;
	m_strNickName = f.nickname;
	m_strSex = f.sex;
	m_strPicUrl = f.picurl;
	m_strSignature = f.signature;
	m_bTempmember = f.tempmember;
	m_strMobile = f.mobile;
	m_strTelephone = f.telephone;
	m_strAddress = f.address;
	m_strEmail = f.email;
	m_bInit = f.init;
}

void BaseMember::GetFriendInfo(friendsinfo& f)
{
	f.type = m_strType;
	f.membersource = m_strMemmberSource;
	f.id = m_strId;
	f.nickname = m_strNickName;
	f.sex = m_strSex;
	f.picurl = m_strPicUrl;
	f.signature = m_strSignature;
	f.tempmember = m_bTempmember;
	f.mobile = m_strMobile;
	f.telephone = m_strTelephone;
	f.address = m_strAddress;
	f.email = m_strEmail;
	f.init = m_bInit;
}

void BaseMember::SetTag(UINT_PTR tag)
{
	m_pTag = tag;
}

UINT_PTR BaseMember::GetTag()
{
	return m_pTag;
}
// 
// void BaseMember::SetTag_(UINT_PTR tag_)
// {
// 	m_pTag_ = tag_;
// }
// 
// UINT_PTR BaseMember::GetTag_()
// {
// 	return m_pTag_;
// }