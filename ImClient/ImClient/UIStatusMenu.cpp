#include "StdAfx.h"
#include "UIStatusMenu.h"
namespace DuiLib {
const TCHAR* const const_statusmenutype = _T("StatusMenuItem");

CUIStatusMenu::CUIStatusMenu(void):m_pOwner(NULL)
{
}


CUIStatusMenu::~CUIStatusMenu(void)
{
}

void CUIStatusMenu::Init(CControlUI* pOwner, POINT pt, BOOL bHasParaent /*= TRUE*/)
{
	if (pOwner == NULL) return;
	m_pOwner = pOwner;
	m_ptPos = pt;
	HWND hWnd = NULL;
	if (bHasParaent)
		hWnd = pOwner->GetManager()->GetPaintWindow(); 
	   Create(hWnd, NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
        HWND hWndParent = m_hWnd;
        while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CUIStatusMenu::Init(HWND hWnd, POINT pt)
{
	m_ptPos = pt;
	Create(NULL, NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

//当窗口处于边缘的时候需要调整菜单的显示位置
void CUIStatusMenu::AdjustPostion() 
{
	CDuiRect rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();

	rcWnd.left = m_ptPos.x;
	rcWnd.top = m_ptPos.y;
	rcWnd.right = rcWnd.left + nWidth;
	rcWnd.bottom = rcWnd.top + nHeight;

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	CDuiRect rcScreen = oMonitor.rcMonitor;
	int  nTaskBarHeight = oMonitor.rcMonitor.bottom - rcWork.bottom;
#if 0
// 	if( rcWnd.bottom > rcWork.bottom ) {
// 		if( nHeight >= rcWork.GetHeight() ) {
// 			rcWnd.top = 0;
// 			rcWnd.bottom = nHeight;
// 		}
// 		else {
// 			rcWnd.bottom = rcWork.bottom;
// 			rcWnd.top = rcWnd.bottom - nHeight;
// 		}
// 	}
// 	if( rcWnd.right > rcWork.right ) {
// 		if( nWidth >= rcWork.GetWidth() ) {
// 			rcWnd.left = 0;
// 			rcWnd.right = nWidth;
// 		}
// 		else {
// 			rcWnd.right = rcWork.right;
// 			rcWnd.left = rcWnd.right - nWidth;
// 		}
// 	}
// 	::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

#endif
	//重新计算位置
	if( rcWnd.bottom > rcScreen.bottom ) 
	{//菜单超出下边界
		if( nHeight >= rcScreen.GetHeight() ) 
		{
			rcWnd.top = 0;
			rcWnd.bottom = nHeight;
		}
		else 
		{
			rcWnd.bottom = m_ptPos.y;
			rcWnd.top = m_ptPos.y - nHeight;
			rcWnd.left = m_ptPos.x;
			rcWnd.right = m_ptPos.x + nWidth;
		}
	}
	if( rcWnd.right > rcScreen.right )
	{
		if( nWidth >= rcScreen.GetWidth() )
		{
			rcWnd.left = 0;
			rcWnd.right = nWidth;
		}
		else 
		{
			rcWnd.right = rcWnd.left;
			rcWnd.left = rcWnd.left - nWidth;
		}
	}
	::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

 void CUIStatusMenu::OnFinalMessage(HWND hWnd)
 { 
	 delete this; 
 }

 void CUIStatusMenu::Notify(TNotifyUI& msg)
 {
	 if( msg.sType == _T("itemselect") ) 
	 {
		// ::MessageBox(NULL, _T("fdfd"), _T(""), NULL);
	 }
	 else if( msg.sType == _T("lclick") )   //点击菜单给父窗口发送通知
	 {
		 if (m_pOwner != NULL)
		 {
			 m_pOwner->GetManager()->SendNotify(m_pOwner, msg.pSender->GetName() , 0, 0, true);
		 }
		 Close();
	 }
 }

   HWND CUIStatusMenu::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x , int y , int cx , int cy, HMENU hMenu )
   {
	   dwExStyle |= WS_EX_TOOLWINDOW;
	   return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
   }

   //处理WM_OnCreate
   LRESULT CUIStatusMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   m_pm.Init(m_hWnd);
	   CDialogBuilder builder;
	   CStatusMenuBuilderCallBack menuCallback;
	   CControlUI* pRoot = builder.Create(_T("menu.xml"), (UINT)0, &menuCallback , &m_pm);
	   ASSERT(pRoot && "Failed to parse XML");
	   m_pm.AttachDialog(pRoot);
	   m_pm.AddNotifier(this);
	   m_pm.SetRoundCorner(3, 3);

// 	   CListUI* pList = static_cast<CListUI*>(m_pOwner);
// 	   int nSel = pList->GetCurSel();
// 	   if( nSel < 0 ) {
// 		   CControlUI* pControl = m_pm.FindControl(_T("menu_Delete"));
// 		   if( pControl ) pControl->SetEnabled(false);
// 	   }

	   AdjustPostion();
	   return 0;
   }

   LRESULT CUIStatusMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	  HWND hwnd =  GetForegroundWindow();
	   if( (HWND)wParam == m_hWnd )
		   bHandled = TRUE;
	   else {
		   Close();
		   bHandled = FALSE;
	   }
	   return 0;
   }

   LRESULT CUIStatusMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
   {
	   LRESULT lRes = 0;
	   BOOL bHandled = TRUE;
	   switch( uMsg ) {
	   case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	   case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break; 
	   case WM_MOUSEWHEEL:    break;
	   default:
		   bHandled = FALSE;
	   }
	   if( bHandled ) return lRes;
	   if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	   return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
   }
}