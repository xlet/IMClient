#pragma once

namespace DuiLib
{
//用户状态菜单类
extern const TCHAR* const const_statusmenutype;
class CListContainerElementUI;
class CStatusMenuElement:public CListContainerElementUI
{
public:
	CStatusMenuElement(){}
	~CStatusMenuElement(){}
	void DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN )
		{
			if( IsEnabled() )
			{

				if(event.Type== UIEVENT_BUTTONDOWN){
					//鼠标左击
					m_pManager->SendNotify(this, DUI_MSGTYPE_LCLICK);
					//此处需要区分左击选中和右击选中
					//
					Select();
				Invalidate();
				}
				else 
				{
					m_pManager->SendNotify(this, DUI_MSGTYPE_RCLICK);
				}
		//Select();
			
			}
			return;
		}

		
		if( m_pOwner != NULL ) CListContainerElementUI::DoEvent(event);
	}
	LPCTSTR GetClass() const{return const_statusmenutype;}
	LPVOID GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, const_statusmenutype) == 0 ) 
			return static_cast<CStatusMenuElement*>(this);    
		return CListContainerElementUI::GetInterface(pstrName);
	}
private:
};

class CUIStatusMenu: public CWindowWnd, public INotifyUI
{
public:
	CUIStatusMenu(void);
	~CUIStatusMenu(void);
	virtual LPCTSTR GetWindowClassName() const{return _T("statusMenu");}
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//当菜单出现在屏幕边缘的时候调整菜单位置
	void AdjustPostion();
	void Init(CControlUI* pOwner, POINT pt, BOOL bHasParaent = TRUE);
	void Init(HWND hWnd, POINT pt);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT  uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void CUIStatusMenu::OnFinalMessage(HWND hWnd);
public:	 
	CPaintManagerUI m_pm;
	CControlUI* m_pOwner;//菜单父窗口
	POINT m_ptPos;    //当前鼠标点击的坐标
};

class CStatusMenuBuilderCallBack: public IDialogBuilderCallback
{
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (IsStrEqual(pstrClass, const_statusmenutype))
		{
			return new CStatusMenuElement();
		}
		return NULL;
	};
};

}