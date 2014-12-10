#include "StdAfx.h"
#include "UISysSetDlg.h"
const TCHAR* const_syssettablout = _T("sysseytab");
const TCHAR* const_normalsetoption = _T("normalsetoption");
const TCHAR* const_upgradesetoption = _T("upgradesetoption");
const TCHAR* const_chatinfosetoption = _T("chatinfosetoption");
const TCHAR* const_voicechatsetoption = _T("voicechatsetoption");
const TCHAR* const_filesetoption = _T("filesetoption");
CUISysSetDlg::CUISysSetDlg(void)
{
}


CUISysSetDlg::~CUISysSetDlg(void)
{
}

void CUISysSetDlg::OnFinalMessage(HWND hWnd)
{
	//delete this;
}

void CUISysSetDlg::Notify(TNotifyUI& msg)
{
	if(IsStrEqual(msg.sType,_T("selectchanged")))
	{
		CDuiString    dstrName     = msg.pSender->GetName();
		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(const_syssettablout));

		if (pTabLayout != NULL)
		{
			if(IsStrEqual(dstrName, const_normalsetoption))
				pTabLayout->SelectItem(0);
			else if(IsStrEqual(dstrName, const_upgradesetoption))
				pTabLayout->SelectItem(1);
			else if(IsStrEqual(dstrName, const_chatinfosetoption))
				pTabLayout->SelectItem(2);
			else if (IsStrEqual(dstrName, const_voicechatsetoption))
				pTabLayout->SelectItem(3);
			else if(IsStrEqual(dstrName, const_filesetoption))
				pTabLayout->SelectItem(4);
		}
		return;
	}
	__super::Notify(msg);
}