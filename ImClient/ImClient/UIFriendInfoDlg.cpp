#include "StdAfx.h"
#include "UIFriendInfoDlg.h"


CUIFriendInfoDlg::CUIFriendInfoDlg():m_pItemInfo(NULL), m_pOwner(NULL)
{
	m_pNameLabel = NULL;
	m_pGroupLabel = NULL;
	m_pMailLabel = NULL;
	m_pBtnDetail = NULL;
	m_pMobileLabel = NULL;
}


CUIFriendInfoDlg::~CUIFriendInfoDlg(void)
{
}

LPCTSTR CUIFriendInfoDlg::GetWindowClassName()const
{
	return _T("FriendInfo");
}

CDuiString  CUIFriendInfoDlg::GetSkinFile()
{
	return _T("friendinfo.xml");
}

CDuiString CUIFriendInfoDlg::GetSkinFolder()
{
	return _T("");
}

void CUIFriendInfoDlg::Notify( TNotifyUI& msg )
{

	return __super::Notify(msg);
}

void CUIFriendInfoDlg::InitWindow()
{
	m_pUserPicCtrl = m_PaintManager.FindControl(_T("friendhead"));
	m_pNameLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("friendname")));
	m_pGroupLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("group")));
	m_pMailLabel =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mail")));
	m_pMobileLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("mobile")));
	m_pBtnDetail = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("detail")));
}

void CUIFriendInfoDlg::SetItemInfo(FriendListItemInfo* pItem)
{
	m_pItemInfo = pItem;
	
	m_pUserPicCtrl->SetBkImage(m_pItemInfo->logo.c_str());
	//<c #333333>穆凤香</c><c #666666>（人事专员）</c>" 
	TCHAR tszStr[512] = {0x0};
	_stprintf_s(tszStr, _T("<c #333333>%s</c><c #666666>（%s）</c>"), m_pItemInfo->nick_name.c_str(), m_pItemInfo->post.c_str());
	m_pNameLabel->SetText(tszStr);
	m_pGroupLabel->SetText(m_pItemInfo->goupName.c_str());
	m_pMailLabel->SetText(m_pItemInfo->mail.c_str());
	m_pMobileLabel->SetText(m_pItemInfo->mobile.c_str());
}

FriendListItemInfo* CUIFriendInfoDlg::GetItemInfo()
{
	return m_pItemInfo;
}
// 
// void CUIFriendInfoDlg::ShowWindow(bool bShow/* = true*/, bool bTakeFocus /*= true*/)
// {
// 
// }

void CUIFriendInfoDlg::SetOwner(CControlUI* pOwner)
{
	m_pOwner = pOwner;
}

CControlUI* CUIFriendInfoDlg::GetOwner()
{
	return m_pOwner;
}

LRESULT CUIFriendInfoDlg::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return __super::OnMouseHover(uMsg,wParam,lParam, bHandled);
}