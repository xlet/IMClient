#pragma once

#include "CaptionWnd.h"

class CComboItemElement:public CListContainerElementUI
{
public:
	CComboItemElement();
	~CComboItemElement();
	void DoEvent(TEventUI& event);
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
};

struct ComboItemInfo
{
	tString tstrId;
	tString tstrNickname;
	tString tstrPicPath;
	tString tstrPswd;
};

class CUILoginDlg:public CCaptionWnd
{
public:
	CUILoginDlg(void);
	~CUILoginDlg(void);
// 	UILIB_RESOURCETYPE GetResourceType() const
// 	{
// 		return UILIB_ZIP;
// 	}
// 	virtual CDuiString GetZIPFileName() const
// 	{
// 		return _T("skin.zip");
// 	}
	virtual LPCTSTR GetWindowClassName( ) const{return _T("loginWnd");} 
	virtual CDuiString GetSkinFile(){return /*_T("qiye.xml")*/_T("login.xml")/*_T("sysset.xml")*/;}
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	virtual CDuiString GetSkinFolder(){return _T("");}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void       InitWindow();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnLoginRet(WPARAM wParam, LPARAM lParam);
// 	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 	BOOL IsInStaticControl(CControlUI *pControl);
//	LRESULT OnNcButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void    OnReceiveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);         //接收服务器发送来的消息
	LRESULT  OnLoginError1(UINT uMsg, WPARAM wParam, LPARAM lParam); 
	LRESULT  OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LRESULT  OnDeleteItem(UINT uMsg, WPARAM wParam, LPARAM lParam);//
	CControlUI* CreateControl(LPCTSTR pstrClass);
protected:
	void InitItemInfoList();
	void IniCombo();
	void OnDropdown(TNotifyUI& msg);
	void OnSetFocus(TNotifyUI& msg);
	void OnKillFocus(TNotifyUI& msg);
	void OnBtnClick(TNotifyUI& msg);
	void Menu_Online(TNotifyUI& msg);
	void Menu_Leave(TNotifyUI& msg);
	void Menu_Busy(TNotifyUI& msg);
	void Menu_UnDisturb(TNotifyUI& msg);
	void Menu_Stealth(TNotifyUI& msg);
	void Menu_Offline(TNotifyUI& msg);
	void OnItemSelect(TNotifyUI& msg);
	void SwitchView();
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg){Close();}
	BOOL LogIn();
	void RecvMsg(LPCSTR lpcStr);
protected:
	//CControlUI*				 m_pAccountContainer;
	CComboUI*				m_pCombo ;
	//CEditUI*                m_pPswdEdit;
	std::list<ComboItemInfo> m_listItemInfo;
	int						 m_nTimeout ;  //超时时间
	//BOOL					 m_bComboDropdown;
};

