#include "StdAfx.h"
#include "UIFaceSelDlg.h"


CUIFaceSelDlg::CUIFaceSelDlg(void)
{
	m_lpFaceList = NULL;
	m_nSelFaceId = -1;
	m_nSelFaceIndex = -1;
	m_strSelFaceFileName = _T("");
}


CUIFaceSelDlg::~CUIFaceSelDlg(void)
{
}


LRESULT CUIFaceSelDlg::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

LPCTSTR CUIFaceSelDlg::GetWindowClassName() const
{
	return _T("FaceDlg");
}
void  CUIFaceSelDlg::InitWindow()
{
	m_pFaceCtrl = static_cast<CFaceCtrl*>(m_PaintManager.FindControl(_T("FaceCtrl1")));
	m_pFaceCtrl->SetLineColor(RGB(223, 230, 246));
	m_pFaceCtrl->SetFocusBorderColor(RGB(0, 0, 255));
	m_pFaceCtrl->SetZoomBorderColor(RGB(0, 138, 255));
	m_pFaceCtrl->SetRowAndCol(8, 15);
	m_pFaceCtrl->SetItemSize(28, 28);
	m_pFaceCtrl->SetZoomSize(84, 84);
	m_pFaceCtrl->SetFaceList(m_lpFaceList);
	m_pFaceCtrl->SetCurPage(0);
	m_nSelFaceId = -1;
	m_nSelFaceIndex = -1;
	m_strSelFaceFileName = _T("");
}


LRESULT CUIFaceSelDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

CDuiString CUIFaceSelDlg::GetSkinFile()
{
	return _T("FaceSelDlg.xml");
}

CDuiString CUIFaceSelDlg::GetSkinFolder()
{
	return _T("");
}
CControlUI* CUIFaceSelDlg::CreateControl(LPCTSTR pstrClass)
{
	if (IsStrEqual(pstrClass, _T("FaceCtrl")))
	{
		return new CFaceCtrl;
	}
	return 0;
}

LRESULT CUIFaceSelDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATE)
	{
		if (WA_INACTIVE == (UINT)LOWORD(wParam))
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
	}
	else if (uMsg == WM_CLOSE)
	{
		::PostMessage(::GetParent(m_hWnd), FACE_CTRL_SEL, NULL, NULL);
		::DestroyWindow(m_hWnd);
		return 0;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CUIFaceSelDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this
}

void CUIFaceSelDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pFaceCtrl)
		{
			int nSelIndex = (int)msg.lParam;
			CFaceInfo * lpFaceInfo = m_pFaceCtrl->GetFaceInfo(nSelIndex);
			if (lpFaceInfo != NULL)
			{
				m_nSelFaceId = lpFaceInfo->m_nId;
				m_nSelFaceIndex = lpFaceInfo->m_nIndex;
				m_strSelFaceFileName = lpFaceInfo->m_strFileName;
				m_tstrFaceTips = lpFaceInfo->m_strTip;
			}
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		//	GetParent(m_hWnd)
		}
		return;
	}
	return __super::Notify(msg);
}


void CUIFaceSelDlg::SetFaceList(CFaceList * lpFaceList)
{
	m_lpFaceList = lpFaceList;
}

tString CUIFaceSelDlg::GetSelFaceFileName()
{
	return m_strSelFaceFileName;
}

int CUIFaceSelDlg::GetSelFaceId()
{
	return m_nSelFaceId;
}

int CUIFaceSelDlg::GetSelFaceIndex()
{
	return m_nSelFaceIndex;
}
