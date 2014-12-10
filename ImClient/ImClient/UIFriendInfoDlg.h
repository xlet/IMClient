#pragma once

#include "UIListCommonDefine.hpp"

class CUIFriendInfoDlg:public WindowImplBase
{
public:
	CUIFriendInfoDlg();
	~CUIFriendInfoDlg(void);
	virtual LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void Notify( TNotifyUI& msg );
	virtual void       InitWindow();	
	void SetItemInfo(FriendListItemInfo* pItem);
	FriendListItemInfo* GetItemInfo();
	//void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	void SetOwner(CControlUI* pOwner);
	CControlUI* GetOwner();
	//此函数中主要处理tab标签选项在鼠标移动岛上面和移开的效果
	LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	FriendListItemInfo* m_pItemInfo;
	CControlUI*         m_pOwner;
	CControlUI*			m_pUserPicCtrl;
	CLabelUI*			m_pNameLabel;
	CLabelUI*			m_pGroupLabel;
	CLabelUI*           m_pMailLabel;
	CButtonUI*			m_pBtnDetail;
	CLabelUI*			m_pMobileLabel;

};

