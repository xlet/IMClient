#include "StdAfx.h"
#include "CaptionWnd.h"


CCaptionWnd::CCaptionWnd(void)
{
}


CCaptionWnd::~CCaptionWnd(void)
{
}



LRESULT CCaptionWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{    
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	//if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

//屏蔽Enter和Esc键
LRESULT CCaptionWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{

	if( uMsg == WM_KEYDOWN ) {
		if( wParam == VK_RETURN ) {

			return true;
		}
		else if( wParam == VK_ESCAPE ) {
			return true;
		}

	}
	return __super::MessageHandler(uMsg, wParam, lParam,bHandled);
}


LRESULT CCaptionWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CCaptionWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CCaptionWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

//类似MFC中的OnNcHitTest， 重写该函数，使得在整个窗口可以拖动的时候，控件不至于无效
LRESULT CCaptionWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);


	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (-1 == rcCaption.bottom)
	{
		rcCaption.bottom = rcClient.bottom;
	}

	//一些静态的“控件” 像 ControlUI  ContainerUI Label等点击在上面是可以拖动的
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = m_PaintManager.FindControl(pt);
		if (IsInStaticControl(pControl))
		{
			return HTCAPTION;
		}
	}


	return HTCLIENT;

}

//类似MFC中的OnSize
LRESULT CCaptionWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

//
BOOL CCaptionWnd::IsInStaticControl(CControlUI *pControl)
{
	BOOL bRet = FALSE;
	if (! pControl)
	{
		return bRet;
	}

	CDuiString strClassName;
	std::vector<CDuiString> vctStaticName;

	strClassName = pControl->GetClass();
	strClassName.MakeLower();
	vctStaticName.push_back(_T("controlui"));
	vctStaticName.push_back(_T("textui"));
	vctStaticName.push_back(_T("labelui"));
	vctStaticName.push_back(_T("containerui"));
	vctStaticName.push_back(_T("horizontallayoutui"));
	vctStaticName.push_back(_T("verticallayoutui"));
	vctStaticName.push_back(_T("tablayoutui"));
	vctStaticName.push_back(_T("childlayoutui"));
	vctStaticName.push_back(_T("dialoglayoutui"));

	std::vector<CDuiString>::iterator it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
	if (vctStaticName.end() != it)
	{
		CControlUI* pParent = pControl->GetParent();
		while (pParent)
		{
			strClassName = pParent->GetClass();
			strClassName.MakeLower();
			it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
			if (vctStaticName.end() == it)
			{
				return bRet;
			}

			pParent = pParent->GetParent();
		}

		bRet = TRUE;
	}

	return bRet;
}

LRESULT CCaptionWnd::OnNcButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CControlUI* pLogoControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("imlogo")));
	//点击下转移焦点，否则由于除了几个按钮盒输入框之外， 其他地方都是非客户区
	//点击空白的地方原来获得焦点的控件不会KillFocus
	if (pLogoControl != NULL && !pLogoControl->IsFocused())
	{
		pLogoControl->SetFocus();
	}
	bHandled = FALSE; //此处要置为false
	return 0;
}

