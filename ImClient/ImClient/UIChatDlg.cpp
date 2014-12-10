#include "StdAfx.h"
#include "UIChatDlg.h"
#include "MsgData.h"
#include "MsgOfArrival.h"
#include <commdlg.h>
#include "PictureProcess.h"

//const 变量定义
const TCHAR* const_fontswitchbtn = _T("fontbtn");
const TCHAR* const_fontbarcontainer = _T("fontbar");
const TCHAR* cosnt_rightviewcontainer = _T("rightview");
const TCHAR* const_rightviewswitchbtn = _T("viewctrlbtn");
const TCHAR* const_inputedit = _T("input_richedit");
const TCHAR* const_viewedit = _T("view_richedit");
const TCHAR* const_sendmsgbtn = _T("sendmsgbtn");
const TCHAR* const_closemsgbtn = _T("closemsgbtn");
const TCHAR* const_msgrecordbtn = _T("msgrecordbtn");
const TCHAR* const_msgrecordview = _T("msgrecordview");
const TCHAR* const_fontsizecmb = _T("cmb_font_size");
const TCHAR* const_fontnamecmb = _T("cmb_font_name");
//const TCHAR* const_defaultfontname = _T("")
CUIChatDlg::CUIChatDlg(void)
{
#if 0
	m_bShowMsgView = FALSE;
	m_bHideMsgView = FALSE;
	m_pInputView = NULL;
	m_bDraged = FALSE;
	m_pFontSizeCmb = NULL;
	m_pFontNameCmb = NULL;
	//m_pShowView	 = NULL;
	m_dwLinkTextRange = 0;
	m_pRecvEdit= NULL;
	m_pSendEdit = NULL;
	m_pMsgRecordView = NULL;
	MsgHeaderFontInfo info = {RGB(0x00,0x80,0x40), RGB(0x00,0x80,0x40),RGB(0x00,0x80,0x40), FALSE, FALSE, 9};
	m_msgoutFontinfo.SetMsgHeaderFontInfo(info);
	//m_msgoutFontinfo.
// 	info.nicknameClr = info.idClr = info.timeClr = RGB(0,0,255);
// 	info.bHasUnderline = TRUE;
// 	info.bIsLinked = TRUE;
	m_msginFontinfo.SetMsgHeaderFontInfo(info);
	m_msginFontinfo.nicknameClr = RGB(0x00, 0x35,0xFC);
	m_msginFontinfo.timeClr = RGB(0x00, 0x35, 0xFC);
	//m_strMsgRecordStartDate = "now";
	SetEachPageRecordNum(50);
#endif
}


CUIChatDlg::~CUIChatDlg(void)
{
	if( m_pInputView )
	{
		SAFE_DELETE(m_pInputView);
	}
// 	if( m_pShowView )
// 	{
// 		SAFE_DELETE(m_pShowView);
// 	}
}

void CUIChatDlg::OnFinalMessage(HWND hWnd)
{
	delete this;
}

LPCTSTR CUIChatDlg::GetWindowClassName() const 
{
	return _T("chatdlg");
}
CDuiString CUIChatDlg::GetSkinFile()
{
	//return _T("duilib.xml");
	return _T("chatDialog.xml");
	//return _T("QQXml\\main_frame.xml");
}

CDuiString CUIChatDlg::GetSkinFolder()
{
	return _T("");
}


void CUIChatDlg::Notify( TNotifyUI& msg )
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
		CRichEditUI* pRichEditView = static_cast<CRichEditUI*>(m_PaintManager.FindControl(const_viewedit));
		if (pRichEditView)
		{
			//EN_LINK回调设置
			pRichEditView->SetLinkNotify(this);
			pRichEditView->SetLinkNotifyCallback(CUIChatDlg::OnNotifyLinkCallback);
		}
		//m_pSendEdit->SetFocus();
		
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (IsStrEqual(msg.pSender->GetName(), _T("closebtn")))
		{
			OnBtn_Close(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_fontswitchbtn) == 0)
		{
			OnBtn_FontBar(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_rightviewswitchbtn) == 0)
		{
			OnBtn_ViewSwitch(msg);
		}
		else if(IsStrEqual(msg.pSender->GetName(), const_msgrecordbtn))
		{
			OnBtn_MsgRecord(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_sendmsgbtn) == 0)
		{
			OnBtn_SendMsg(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_closemsgbtn) == 0)
		{
			Close();
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("imagebtn")))
		{
			//OnBtn_SendImage(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("emotionbtn")))
		{
			OnBtn_Emotion(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("boldbtn")))
		{
			OnBtn_Bold(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("italicbtn")))
		{
			OnBtn_Italic(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("underlinebtn")))
		{
			OnBtn_UnderLine(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("colorbtn")))
		{
			OnBtn_Color(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("msgrecordbtn")))
		{
			OnBtn_MsgRecord(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("tolastpage") ))
		{
			TurnMsgRecordPage(E_TOLASTPAGE);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("toleftbtn")))
		{
			TurnMsgRecordPage(E_TOLEFT);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("torightbtn")))
		{
			TurnMsgRecordPage(E_TORIGHT);
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("tofirstpage")))
		{
			TurnMsgRecordPage(E_TOFIRSTPAGE);
		}
	}
	else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
	{
		if (_tcsicmp(msg.pSender->GetName(), const_inputedit) == 0)
		{
			SendMsg();
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender == m_pFontNameCmb)
		{
			OnCmb_Selchange_FontName(msg);
		}
		else if(msg.pSender == m_pFontSizeCmb)
		{
			OnCmb_SelChange_FontSize(msg);
		}

	}
	__super::Notify(msg);
}

void CUIChatDlg::Init()
{
	g_mapWnd.insert(make_pair(m_pFriendInfo->id, m_hWnd));

	m_flashwInfo.hwnd = m_hWnd;
	m_flashwInfo.cbSize = sizeof FLASHWINFO;
	m_flashwInfo.uCount = 2;
	m_flashwInfo.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
	m_flashwInfo.dwTimeout =  0; //500;

	tString strPath = ZYM::CPath::GetCurDir() + _T("Face\\FaceConfig.xml");
	m_FaceList.LoadConfigFile(strPath.c_str());
	m_pFontNameCmb = static_cast<CComboUI*>(m_PaintManager.FindControl(const_fontnamecmb));
	m_pFontSizeCmb = static_cast<CComboUI*>(m_PaintManager.FindControl(const_fontsizecmb));
	m_pSendEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(const_inputedit));
	m_pRecvEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(const_viewedit));

	m_pMsgRecordView = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("msgrecord_richedit")));
	


	if(m_pSendEdit != NULL)
	{
		m_pInputView = new InputViewEvent(m_pSendEdit,_T(""),0x0);
		m_pSendEdit->RegisterEventSink(m_pInputView);
		BOOL bret = m_pSendEdit->EnableDropDrag();

		//m_pSendEdit->DoMyDrag(m_hWnd);
	}
 	CControlUI* pLogo = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("Imlogo")));
	pLogo->SetBkImage(m_pFriendInfo->logo.c_str());

}
//相当于MFC的OninitDialg函数，窗口的初始化
void  CUIChatDlg::InitWindow()
{
	//FlashWindow(m_hWnd, FALSE);
	Init();
	SetWindowIcon();
	m_PaintManager.SetFocusNeeded(m_pSendEdit);
}

void CUIChatDlg::SetWindowIcon()
{
	CPictureProcess grayProcess;
	HICON hIcon = grayProcess.GetIcon(m_pFriendInfo->logo.c_str());
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
	hIcon = grayProcess.GetIcon(m_pFriendInfo->logo.c_str());
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);
		
}

//字体栏的隐藏和显示
void CUIChatDlg::OnBtn_FontBar(TNotifyUI& msg)
{
	CContainerUI* pFontbarContainer = static_cast<CContainerUI*>(m_PaintManager.FindControl(const_fontbarcontainer));
	if(pFontbarContainer != NULL)
	{
		pFontbarContainer->SetVisible(!pFontbarContainer->IsVisible());
	}
}

void CUIChatDlg::OnBtn_Close(TNotifyUI& msg)
{


}

void CUIChatDlg::OnBtn_Bold(TNotifyUI& msg)
{
	 int xx = m_pRecvEdit->GetVerticalScrollBar()->GetScrollRange();
	 int yy = m_pRecvEdit->GetLineCount();
	 m_pRecvEdit->LineScroll(yy  + 20);
	//SIZE sz = {m_pRecvEdit->GetScrollPos() + }
//	m_pRecvEdit->SetScrollPos();
	//m_pRecvEdit->SetScrollPos()
	//m_pRecvEdit->EndDown();
	//
	m_pRecvEdit->Invalidate();
	COptionUI* pFontOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("boldbtn")));
	if (pFontOption == NULL)
	{
		return;
	}

	BOOL bBold = !pFontOption->IsSelected();
	if (m_fontInfo.m_bBold != bBold)
	{
		m_fontInfo.m_bBold = bBold; 
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		//g_buddyFontInfo.m_bBold = m_fontInfo.m_bBold;
	}
}

void CUIChatDlg::OnBtn_UnderLine(TNotifyUI& msg)
{
	COptionUI* pUnderLineOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("underlinebtn")));
	if (pUnderLineOption == NULL)
	{
		return;
	}
	BOOL bUnderLine = !pUnderLineOption->IsSelected();
	if (m_fontInfo.m_bUnderLine != bUnderLine)
	{
		m_fontInfo.m_bUnderLine = bUnderLine;
		ITextServices* pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
	}
}

void CUIChatDlg::OnBtn_Italic(TNotifyUI& msg)
{
	COptionUI* pItalicOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("italicbtn")));
	if (pItalicOption == NULL)
	{
		return;
	}

	BOOL bItalic = !pItalicOption->IsSelected();
	if (m_fontInfo.m_bItalic != bItalic)
	{
		m_fontInfo.m_bItalic = bItalic;
		ITextServices* pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
	}
}

void CUIChatDlg::OnBtn_ViewSwitch(TNotifyUI& msg)
{
	//点击隐藏或者显示右边的区域
	BOOL bExtend = FALSE;
	if (!m_bShowMsgView)
	{//消息框没有显示
		if (m_bHideMsgView)
		{//上一次隐藏的是消息框
			//那么此次就显示消息框
			CContainerUI* pMsgRecordContainer= static_cast<CContainerUI*>(m_PaintManager.FindControl(const_msgrecordview));
			if (pMsgRecordContainer != NULL)
			{
				m_PaintManager.SetMinInfo(m_MinInfo.cx+ m_nExtendedWidth,m_MinInfo.cy);
				RECT rc = {0};
				::GetWindowRect(m_hWnd, &rc);
				ResizeWindow(rc.right - rc.left + m_nExtendedWidth, -1);
				pMsgRecordContainer->SetVisible(!pMsgRecordContainer->IsVisible());

				bExtend = TRUE;
			}
		}
		else
		{//隐藏的不是消息框
			CContainerUI* pRightViewContainer = static_cast<CContainerUI*>(m_PaintManager.FindControl(cosnt_rightviewcontainer));
			if(pRightViewContainer != NULL)
			{
				pRightViewContainer->SetVisible(!pRightViewContainer->IsVisible());
			}
			bExtend = !pRightViewContainer->IsVisible();
		}

	}
	else 
	{ //消息记录显示， 那么 点击中间收缩按钮，窗口恢复正常大小， 同时隐藏消息记录框
		CContainerUI* pMsgRecordContainer= static_cast<CContainerUI*>(m_PaintManager.FindControl(const_msgrecordview));
		if (pMsgRecordContainer != NULL)
		{
			m_PaintManager.SetMinInfo(m_MinInfo.cx, m_MinInfo.cy);
			m_bShowMsgView = FALSE;
			RECT rc = {0};
			::GetWindowRect(m_hWnd,  &rc);
			ResizeWindow(rc.right - rc.left - m_nExtendedWidth, -1);
			pMsgRecordContainer->SetVisible(FALSE);
			m_bHideMsgView = TRUE;
			bExtend = TRUE;
		}
	}

	CButtonUI* pViewSwitchBtn = static_cast<CButtonUI*>(msg.pSender);
	if (bExtend)
	{
		pViewSwitchBtn->SetHotImage(_T("rightviewhidebtn.png"));
	}
	else
	{
		pViewSwitchBtn->SetHotImage(_T("rightviewshowbtn.png"));
	}


	
}

void CUIChatDlg::SetTurnPageBtnState(bool bToFirstPage, bool bToLastPage)
{
	CButtonUI* pBtnToRight = NULL;
	CButtonUI* pBtnToFirstPage = NULL;
	CButtonUI* pBtnToLeft = NULL;
	CButtonUI* pBtnToLastPage = NULL;
	if (bToLastPage != m_bToLastPage)
	{
		pBtnToLastPage = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("tolastpage")));
		pBtnToLeft = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("toleftbtn")));
		if (pBtnToLeft != NULL && pBtnToLastPage != NULL)
		{
			m_bToLastPage = bToLastPage;
			pBtnToLastPage->SetEnabled(!m_bToLastPage);
			pBtnToLeft->SetEnabled(!m_bToLastPage);
			pBtnToLastPage->SetBkImage(m_bToLastPage?_T("file='lastpagebtn_.png' dest='2,2,13,13'"):_T("file='lastpagebtn.png' dest='2,2,13,13'"));
			pBtnToLeft->SetBkImage(m_bToLastPage?_T("file='toleftbtn_.png' dest='2,2,13,13'"):_T("file='toleftbtn.png' dest='2,2,13,13'"));
		}
		
	}

	if (bToFirstPage != m_bToFristPage)
	{
		pBtnToFirstPage = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("tofirstpage")));
		pBtnToRight = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("torightbtn")));
		if (pBtnToFirstPage != NULL && pBtnToRight != NULL)
		{
			m_bToFristPage = bToFirstPage;
			pBtnToFirstPage->SetEnabled(!m_bToFristPage);
			pBtnToRight->SetEnabled(!m_bToFristPage);
			pBtnToFirstPage->SetBkImage(m_bToFristPage?_T("file='firstpagebtn_.png' dest='2,2,13,13'"):_T("file='firstpagebtn.png' dest='2,2,13,13'"));
			pBtnToRight->SetBkImage(m_bToFristPage?_T("file='torightbtn_.png' dest='2,2,13,13'"):_T("file='torightbtn.png' dest='2,2,13,13'"));
		}
	}

		
}

void CUIChatDlg::TurnMsgRecordPage(E_CHECKMSGRECORD type)
{
	ShowMsgRecord(type);
}

void CUIChatDlg::ShowOneMsgRecord(ITextServices* pTextServices, const msginfo& info)
{
	E_MSG_TYPE type;
	if (info.fromorto == 0) // 收到消息
	{

		type = MSG_IN;
	}
	else //发送消息
	{
		type = MSG_OUT;
	}
	ShowMsgHeader(pTextServices, type, info);
	AddMsg(pTextServices, m_pMsgRecordView, info);
	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	m_pRecvEdit->EndDown();
}

void CUIChatDlg::ShowMsgRecord(E_CHECKMSGRECORD type)
{
	m_pMsgRecordView->SetText(_T(""));
	std::string strId;
	tStringToString(strId, m_pFriendInfo->id);
	//vector<msginfo> vMsgRecrd;
	bool bAsc;  //按升序查询
	
// 	if (type == E_TOFIRSTPAGE)
// 		bAsc = FALSE;
// 	else if (type == E_TORIGHT)
// 		bAsc = TRUE;
// 	else if (type == E_TOLASTPAGE)
// 		bAsc = TRUE;
// 	else if (type == E_TOLEFT)
// 		bAsc = FALSE;
	bAsc = type == E_TORIGHT ||type == E_TOLASTPAGE;

// 	if (type == E_TOFIRSTPAGE || type == E_TOLASTPAGE)
// 	     m_strMsgRecordStartDate = "now";
// 	else if (type == E_TOLEFT)
// 		m_strMsgRecordStartDate = m_vMsgRecrd[];
	m_vMsgRecrd.clear();

	 int  strQueryMsgId;
	if (type == E_TOFIRSTPAGE || type == E_TOLASTPAGE)
	{
		//"<" or ">"
		strQueryMsgId = INT_MAX;
		m_bLessComparison = true;
	}
	else  if (type == E_TOLEFT)
	{
		strQueryMsgId = m_nMinMsgId;
		m_bLessComparison = true;
	}
	else if (type == E_TORIGHT)
	{
		strQueryMsgId = m_nMaxMsgId;
		m_bLessComparison = false;
	}
	CSqlite::GetIntstance()->GetMsgDbInterface()->QueryMsgCollection(m_vMsgRecrd,strId,strQueryMsgId,m_bLessComparison, bAsc ,m_nEachPageRecordNum + 1); // 此处多查询一条是用来判断是否有下一页或者上一页,实际上需要显示的是m_nEachPageRecordNum条

	int nSize = m_vMsgRecrd.size();

	if (nSize == 0)
	{
		if (type == E_TOLASTPAGE || type == E_TOLEFT)
			SetTurnPageBtnState(m_bToFristPage, true);
		else
			SetTurnPageBtnState(true, m_bToLastPage);
		return;
	}

	//查询时候是否已经查询到了尽头
	bool bHasMore = nSize > m_nEachPageRecordNum;

	bool bToFirstPage, bToLastPage;

	if (type == E_TOFIRSTPAGE)
	{// 降序
		bToFirstPage = true;
		bToLastPage = !bHasMore;
		m_nMaxMsgId = m_vMsgRecrd[0].msgid;
		m_nMinMsgId = m_vMsgRecrd[nSize<m_nEachPageRecordNum+1?nSize-1:nSize-2].msgid;
	}
	else if (type == E_TORIGHT)
	{//升序
		bToFirstPage = !bHasMore;
		bToLastPage = false;// 保持不变
		m_nMinMsgId = m_vMsgRecrd[0].msgid;
		m_nMaxMsgId = m_vMsgRecrd[nSize<m_nEachPageRecordNum+1?nSize-1:nSize-2].msgid;
	}
	else if (type == E_TOLASTPAGE)
	{//升序
		bToLastPage = true;
		bToFirstPage = !bHasMore;
		m_nMinMsgId = m_vMsgRecrd[0].msgid;
		m_nMaxMsgId = m_vMsgRecrd[nSize<m_nEachPageRecordNum+1?nSize-1:nSize-2].msgid;
	}
	else if (type == E_TOLEFT)
	{//降序
		bToLastPage = !bHasMore;
		bToFirstPage = false; // 保持不变
		m_nMaxMsgId = m_vMsgRecrd[0].msgid;
		m_nMinMsgId = m_vMsgRecrd[nSize<m_nEachPageRecordNum+1?nSize-1:nSize-2].msgid;
	}

	SetTurnPageBtnState(bToFirstPage, bToLastPage);

	ITextServices* pTextServices = m_pMsgRecordView->GetTextServices();


	int nMsgShowNum = bHasMore?m_nEachPageRecordNum:nSize;

	if (bAsc) //升序
	{
// 		for (vector<msginfo>::iterator it = m_vMsgRecrd.begin(); i < nMsgShowNum; ++it, ++i)
// 		{
// 			ShowOneMsgRecord(pTextServices, *it);
// 		}
		for (int i = 0;  i < nMsgShowNum; ++i)
		{
			ShowOneMsgRecord(pTextServices, m_vMsgRecrd[i]);
		}
	
	}
	else
	{//降序	
// 		for (vector<msginfo>::reverse_iterator it = m_vMsgRecrd.rbegin(); i < nMsgShowNum; ++it, ++i)
// 		{
// 			ShowOneMsgRecord(pTextServices, *it);
// 		}
		//int i=nSize-nMsgShowNum
		for (int i = nMsgShowNum -1 ; i >= 0; --i)
		{
			ShowOneMsgRecord(pTextServices, m_vMsgRecrd[i]);
		}
	}
	


	pTextServices->Release();
}

//显示某一天的聊天记录
void CUIChatDlg::ShowMsgRecord(const char* date)
{
	std::string strFromDate(date) , strToDate(date);
	strFromDate  += "00:00:00";
	strToDate += "24:00:00";


	std::string strId;
	tStringToString(strId, m_pFriendInfo->id);
	m_vMsgRecrd.clear();
	// 按升序排序
	CSqlite::GetIntstance()->GetMsgDbInterface()->QueryMsgCollection(m_vMsgRecrd,strId, strFromDate, strToDate);
	if (m_vMsgRecrd.empty())
		return ;
	m_pMsgRecordView->SetText(_T(""));
	ITextServices* pTextServices = m_pMsgRecordView->GetTextServices();

	m_nMaxMsgId = m_vMsgRecrd[m_vMsgRecrd.size() - 1].msgid;
	m_nMinMsgId = m_vMsgRecrd[0].msgid;
	for (vector<msginfo>::iterator it = m_vMsgRecrd.begin(); it != m_vMsgRecrd.end(); ++it)
	{
		ShowOneMsgRecord(pTextServices, *it);
	}

	bool bToFirstPage, bToLastPage;
// 	std::string strMaxMsgId;
// 	std::string strMinMsgId;
	char szMaxMsgId[16] ={0x0};
	char szMinMsgId[16] = {0x0};
	_itoa(m_nMinMsgId, szMinMsgId, 10);
	_itoa(m_nMaxMsgId, szMaxMsgId, 10);
	
	bToFirstPage = !CSqlite::GetIntstance()->GetMsgDbInterface()->IsExist(strId, e_msginfo_msgid, e_biger, szMaxMsgId);
	bToLastPage = !CSqlite::GetIntstance()->GetMsgDbInterface()->IsExist(strId, e_msginfo_msgid, e_less, szMinMsgId);
	SetTurnPageBtnState(bToFirstPage, bToLastPage);
	pTextServices->Release();
}

void CUIChatDlg::OnBtn_MsgRecord(TNotifyUI& msg)
{
	//点击消息记录按钮
	CContainerUI* pMsgRecordContainer = static_cast<CContainerUI*>(m_PaintManager.FindControl(const_msgrecordview));
	CContainerUI* pRightViewContainer = static_cast<CContainerUI*>(m_PaintManager.FindControl(cosnt_rightviewcontainer));
	if (pRightViewContainer == NULL || pMsgRecordContainer == NULL)
	{
		return;
	}
	//	if (pMsgRecordContainer != NULL)
	//	{
	RECT  rc = {0};
	::GetClientRect(m_hWnd, &rc);
	if (!m_bShowMsgView)
	{//当前没显示消息记录，那么点击按钮后显示
		ResizeWindow(rc.right - rc.left + m_nExtendedWidth, rc.bottom - rc.top);
		if (pRightViewContainer->IsVisible())
		{
			pRightViewContainer->SetVisible(FALSE);
		}

		pMsgRecordContainer->SetVisible(TRUE);
		m_pMsgRecordView->SetText(_T(""));
		m_bShowMsgView = TRUE;
		m_bHideMsgView  = FALSE;
		m_PaintManager.SetMinInfo(m_MinInfo.cx+m_nExtendedWidth, m_MinInfo.cy );
		ShowMsgRecord(E_TOFIRSTPAGE);
	}
	else 
	{//当前已经显示消息记录， 那么点击按钮后取消显示消息记录

		m_PaintManager.SetMinInfo(m_MinInfo.cx, m_MinInfo.cy );

		ResizeWindow(rc.right - rc.left - m_nExtendedWidth, rc.bottom - rc.top);
		pMsgRecordContainer->SetVisible(FALSE);
		m_pMsgRecordView->SetText(_T(""));
		pRightViewContainer->SetVisible(TRUE);
		RECT rc = {0};
		::GetWindowRect(m_hWnd, &rc);
		m_bShowMsgView = FALSE;
	}

	//	}
}

void CUIChatDlg::OnBtn_SendMsg(TNotifyUI& msg)
{
	SendMsg();
}

void CUIChatDlg::OnBtn_SendImage(TNotifyUI& msg)
{
	//Test
	//CRichEditUI* pInputEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(const_inputedit));
	//pInputEdit->InsertGIFImage(_T("D:\\imClient\\ImClient\\Debug\\ImClient\\04.gif"),0x22);
	//pInputEdit->InsertNotGIFImage(_T("D:\\imClient\\ImClient\\Debug\\ImClient\\png.png"),0x33);

}

void CUIChatDlg::OnBtn_Emotion(TNotifyUI& msg)
{
	m_FaceDlg.SetFaceList(&m_FaceList);
	if (!::IsWindow((HWND)m_FaceDlg))
	{
		HWND hwd =m_FaceDlg.Create(m_hWnd,NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);
		RECT rcBtn = msg.pSender->GetPos();
		::ClientToScreen(m_hWnd, (LPPOINT)&rcBtn);

		int cx = 432;
		int cy = 236;
		int x = rcBtn.left - cx / 2;
		int y = rcBtn.top - cy;

		BOOL b = ::SetWindowPos((HWND)m_FaceDlg, NULL, x, y, cx, cy, NULL);
		b = ::ShowWindow((HWND)m_FaceDlg, SW_SHOW);
	}
}

//CUIChatDlg::CUIChatDlg(const FriendListItemInfo& friend_info):m_friendInfo(friend_info)
CUIChatDlg::CUIChatDlg(FriendListItemInfo* friend_info):m_vMsgRecrd(60)
{
	m_pFriendInfo = friend_info;
	m_bShowMsgView = FALSE;
	m_bHideMsgView = FALSE;

	//当显示消息记录的时候
	m_nExtendedWidth = 156; 
	m_MinInfo.cx = 492;
	m_MinInfo.cy = 504;
	m_bDraged = FALSE;
	m_pIDragObj = NULL;
	m_pFontNameCmb = NULL;
	m_pFontSizeCmb = NULL;
	MsgHeaderFontInfo info = {RGB(88, 135,65), RGB(88, 135,65),RGB(88, 135,65), FALSE, FALSE, 9};
	m_msgoutFontinfo.SetMsgHeaderFontInfo(info);
	info.nicknameClr = info.idClr = info.timeClr = RGB(0,0,255);
	info.bHasUnderline = FALSE;
	info.bIsLinked = FALSE;
	m_msginFontinfo.SetMsgHeaderFontInfo(info);
	m_pMsgRecordView = NULL;
	m_pRecvEdit= NULL;
	m_pSendEdit = NULL;
	//m_strMsgRecordStartDate = "now";
	SetEachPageRecordNum(20);
	m_bToFristPage = true;
	m_bToLastPage = false;//	m_bHasUnreadMsgAndFlash = FALSE;
	m_bLessComparison = false;
//	m_bFocus = FALSE;
}

void CUIChatDlg::OnPrepare(TNotifyUI& msg)
{
	CLabelUI* title = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("chatdlgtitle")));
	if (title != NULL)
	{
		//改变窗口上面标题
		TCHAR szTitle[MAX_PATH] ;
		int len = _tcslen(m_pFriendInfo->nick_name.c_str());
		wsprintf(szTitle, _T("与%s聊天中"),m_pFriendInfo->nick_name.c_str());
		title->SetText(szTitle);
	}
	BOOL bret = FALSE;

}

/*
	功能：点击发送按钮发送消息
	输入： void
	输出： void
	其他：注意限定一次性发送的字符串的长度
*/
void CUIChatDlg::SendMsg()
{
	ITextServices* pTextServices =m_pSendEdit->GetTextServices();
	tString tstrText;  //发送窗口的字符串
	//RichEdit_GetText(pTextServices, tstrText );
	_RichEdit_GetText(pTextServices, tstrText );


	if (tstrText.empty())
	{
		return;
	}
	if (tstrText.length() > 1024)
	{
		// 此处处理下， 消息长度过长是需要分段发送
	}
	pTextServices->Release();
	


	Json::Value val;
	Msg2Json(val, tstrText.c_str());
	
	//存储该消息, 存进当前消息集合里面
	NormalMsg* pNormalMsg = new NormalMsg;
	pNormalMsg->FromJson(val);
	//MsgOfArrival::GetInstance()->Insert(pNormalMsg);

	G_IMSOCKET->SetMsgfont(m_fontInfo);
	G_IMSOCKET->SendData(val);
	ShowMsgInRecvEdit(pNormalMsg, MSG_OUT);
	// 发送消息改变最近联系人的状态
	::SendMessage(GetOwner(),WM_RECENTCONTACTS,  0 , (LPARAM)pNormalMsg);
	
	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();
	SAFE_DELETE(pNormalMsg)
#if 0
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
	cf.dwEffects = CFE_LINK;
	//cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(220, 0, 0);

	lSelEnd = lSelBegin = m_pRecvEdit->GetTextLength();
	m_pRecvEdit->SetSel(lSelEnd, lSelEnd);
	m_pRecvEdit->ReplaceSel(_T("just_bg"), false);

	lSelEnd = m_pRecvEdit->GetTextLength();
	m_pRecvEdit->SetSel(lSelBegin, lSelEnd);
	m_pRecvEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = m_pRecvEdit->GetTextLength();
	m_pRecvEdit->SetSel(lSelEnd, lSelEnd);
	m_pRecvEdit->ReplaceSel(GetCurrentTimeString(), false);
	cf.dwMask = CFM_COLOR;//#7983c0
	cf.crTextColor = RGB(0x79, 0x83, 0xc0);
	cf.dwEffects = 0;
	lSelEnd = m_pRecvEdit->GetTextLength();
	m_pRecvEdit->SetSel(lSelBegin, lSelEnd);
	m_pRecvEdit->SetSelectionCharFormat(cf);


	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	m_pRecvEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = m_pRecvEdit->GetTextLength();

	m_pRecvEdit->SetSel(-1, -1);
	m_pRecvEdit->ReplaceSel(tstrText.c_str(), false);

	m_pRecvEdit->SetSel(-1, -1);
	m_pRecvEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = m_pRecvEdit->GetTextLength();
	m_pRecvEdit->SetSel(lSelBegin, lSelEnd);
	m_pRecvEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	m_pRecvEdit->SetParaFormat(pf);

	m_pRecvEdit->SetEventMask(ENM_LINK);
	m_pRecvEdit->EndDown();
#endif
}

void CUIChatDlg::HandleSendString(string& str)
{

}

void CUIChatDlg::RecvMsg(NormalMsg* pNormalMsg)
{
	//MsgOfArrival::GetInstance()->Insert(pNormalMsg);


	TCHAR* ptszId = NULL;
	C2T(&ptszId, pNormalMsg->m_strFromId.c_str());
	if (_tcsicmp(ptszId, m_pFriendInfo->id.c_str()) == 0)
	{
		ShowMsgInRecvEdit(pNormalMsg, MSG_IN);
	}
	else
	{
#if CUSTOMSERV
		Node*  pNode = (Node*)((linkmaninfoEx*)(CGlobalData::GetInstance()->m_recentLinkmen.m_linkmenMap.at(pNormalMsg->m_strFromId)))->m_pTag;
		CVerticalLayoutUI*  pVLayout = (CVerticalLayoutUI*)(pNode->data().list_elment_->FindSubControl(_T("msgtipview")));
		pVLayout->SetVisible(1);

#endif	
	}

	SAFE_ARRYDELETE(ptszId);
}

void CUIChatDlg::AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit, LPCTSTR lptszText,const msgfont* pMsgFont /*= NULL*/)
{
	tString tstrText;
	//设置缩进
	if (pRichEdit == m_pRecvEdit || pRichEdit == m_pMsgRecordView)
	{
		_RichEdit_ReplaceSel(pTextServices,pRichEdit, _T(""), pMsgFont);
	}


	for (LPCTSTR p = lptszText; *p != _T('\0'); p++)
	{
		if (*p == _T('['))
			if(HandleSysFaceId(pTextServices, pRichEdit, p, tstrText, pMsgFont))
				continue;
		tstrText += *p;
	}

	if (!tstrText.empty())
		_RichEdit_ReplaceSel(pTextServices, pRichEdit, tstrText.c_str(), pMsgFont);
}

void CUIChatDlg::AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  const NormalMsg* pMsg)
{
	// 	if (NULL == pRichEdit || NULL == lpText || lpText == 0x0)
	// 		return;

	TCHAR* lptszText = NULL;
	C2T(&lptszText, pMsg->m_strContent.c_str());
#if 0
	//旧代码 /f['face']
	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				tstrText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if(HandleSysFaceId(pRichEdit, p, tstrText))
					continue;
			}
		}
		tstrText += *p;
	}
#endif
#if 1
	AddMsg(pTextServices,pRichEdit, lptszText);
#endif
	SAFE_ARRYDELETE(lptszText);
}

void CUIChatDlg::AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit, const msginfo& msg)
{
	TCHAR* lptszText = NULL;
	C2T(&lptszText, msg.msgcontent.c_str());
	AddMsg(pTextServices,pRichEdit, lptszText, &(msg.font));
	SAFE_ARRYDELETE(lptszText);

}
BOOL CUIChatDlg::HandleSysFaceId(ITextServices *pTextServices, CRichEditUI * pRichEdit, LPCTSTR& p, tString& strText, const msgfont* pMsgFont/* = NULL*/)
{
#if 0
	int nFaceId = GetBetweenInt(p+2, _T("['"),_T("']"), -1);
	CFaceInfo* lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if(!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		pRichEdit->InsertGIFImage(lpFaceInfo->m_strFileName.c_str(), 0);
		p = _tcsstr(p+2, _T("']"));
		p++;
		return TRUE;
	}
#endif
#if 1
	tString tstrFaceName = GetBetweenString(p, _T("["), _T("]"));
	CFaceInfo* lpFaceInfo = m_FaceList.GetFaceInfoByFaceTip(tstrFaceName.c_str());
	if (lpFaceInfo != NULL)
	{
		if(!strText.empty())
		{
			_RichEdit_ReplaceSel(pTextServices, pRichEdit, strText.c_str(), pMsgFont); 
			strText = _T("");
		}
		pRichEdit->InsertGIFImage(lpFaceInfo->m_strFileName.c_str(), lpFaceInfo->m_nId);
		p = _tcsstr(p, _T("]"));
		return TRUE;
	}
#endif
	return FALSE;
}

void CUIChatDlg::ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type, const NormalMsg* pMsg)
{
	//显示当前用户自己发送的信息
	//昵称加自己的Im号 
#if 0 
	PMSGHEADERFONTINFO pInfo = NULL;
	TCHAR tszText[512] = {0};
	tString tstrId ;
	if (type == MSG_IN)
	{
		pInfo = &m_msginFontinfo;
		wsprintf(tszText,_T("%s("), /*_T("just_bg")*/m_friendInfo.nick_name.c_str()); //"("
		tstrId = m_friendInfo.id;
	}
	else
	{
		pInfo = &m_msgoutFontinfo;
		wsprintf(tszText,_T("%s("),g_user.m_userInfo.nickname.c_str()); //"("
		StringTotString(tstrId,  g_user.m_userInfo.id);
	}

	

	m_pRecvEdit->SetSel(-1, -1);
	RichEdit_ReplaceSel(pTextServices,tszText, _T("微软雅黑"), pInfo->fontSize, pInfo->nicknameClr, FALSE, FALSE, pInfo->bHasUnderline, pInfo->bIsLinked, 0 );
	LONG  linkStart ;
	LONG  linkEnd;
	RichEdit_GetSel(pTextServices, linkStart, linkStart);
	wsprintf(tszText, _T("%s"), tstrId.c_str()/*m_friendInfo.id.c_str()*/);  //用户Im号
	RichEdit_ReplaceSel(pTextServices, tszText, 
		_T("微软雅黑"), pInfo->fontSize, pInfo->idClr, FALSE, FALSE,TRUE/* pInfo->bHasUnderline*/, 1/*pInfo->bIsLinked*/, 0);	
	RichEdit_GetSel(pTextServices, linkEnd, linkEnd);

	m_dwLinkTextRange =  MAKELPARAM(linkStart, linkEnd);
	TCHAR tsztime[24] = {0} ;
	TimestampToLocalTime(tsztime, sizeof(tsztime), (int)(pMsg->m_baseMsgInfo.m_dbSendTime/1000));
	//tstrTime = FormatTime(time(NULL), _T("%H:%M:%S"));
	
	wsprintf(tszText, _T(")  %s\r\n"), tsztime);
	RichEdit_ReplaceSel(pTextServices, tszText, _T("微软雅黑"), 9, RGB(/*0, 0, 255*/88, 135,65), FALSE, FALSE, FALSE, FALSE, 0);
#endif
	TCHAR tsztime[24] = {0} ;
	TimestampToLocalTime(tsztime, sizeof(tsztime), (int)(pMsg->m_baseMsgInfo.m_dbSendTime/1000));
	ShowMsgHeader(pTextServices, type, tsztime);
}

void CUIChatDlg::ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type, const msginfo& msg)
{
	TCHAR* pttime;
	C2T(&pttime, msg.datetime.c_str());
	ShowMsgHeader(pTextServices, type, pttime);
	SAFE_ARRYDELETE(pttime);
}

void CUIChatDlg::ShowMsgHeader(ITextServices* pTextServices, E_MSG_TYPE type, LPCTSTR lptTime)
{

#if 1
	PMSGHEADERFONTINFO pInfo = NULL;
	TCHAR tszText[512] = {0};
	tString tStrName;  //前面显示的用户昵称或者Im号 
	if (type == MSG_IN)
	{
		pInfo = &m_msginFontinfo;
		if (m_pFriendInfo->nick_name.empty())
			tStrName = m_pFriendInfo->id;
		else
			tStrName = m_pFriendInfo->nick_name;
		wsprintf(tszText,_T("%s"), /*_T("just_bg")*/tStrName.c_str()); //"("

	}
	else
	{
		pInfo = &m_msgoutFontinfo;
#if NOSERVICETEST
		if (g_user.m_userInfo.nickname.empty())
			StringTotString(tStrName, g_user.m_userInfo.id);
#else 
		if (g_user.UerSelf()->m_strNickName.empty())
			StringTotString(tStrName,  g_user.UerSelf()->m_strId);
#endif
		else
			StringTotString(tStrName,   g_user.UerSelf()->m_strNickName);
		wsprintf(tszText,_T("%s"),  tStrName.c_str()); //"("
	}



	m_pRecvEdit->SetSel(-1, -1);
	RichEdit_ReplaceSel(pTextServices,tszText, _T("微软雅黑"), pInfo->fontSize, pInfo->nicknameClr, FALSE, FALSE, pInfo->bHasUnderline, pInfo->bIsLinked, 0 );

	//TCHAR tsztime[24] = {0} ;
	//TimestampToLocalTime(tsztime, sizeof(tsztime), (int)(pMsg->m_baseMsgInfo.m_dbSendTime/1000));
	//tstrTime = FormatTime(time(NULL), _T("%H:%M:%S"));

	wsprintf(tszText, _T("  %s\r\n"), lptTime);
	RichEdit_ReplaceSel(pTextServices, tszText, _T("微软雅黑"), pInfo->fontSize, pInfo->timeClr, FALSE, FALSE, FALSE, FALSE, 0);
#endif
}

//再接收框中显示消息
void CUIChatDlg::ShowMsgInRecvEdit(const NormalMsg* pMsg, E_MSG_TYPE type)
{
	if (pMsg == NULL)
	{
		return;
	}

	ITextServices* pTextServices = m_pRecvEdit->GetTextServices();
	ShowMsgHeader(pTextServices, type, pMsg);
	//消息内容
	//_RichEdit_ReplaceSel(m_pRecvEdit, lpText);
	AddMsg(pTextServices, m_pRecvEdit,  pMsg);
	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	m_pRecvEdit->SetEventMask(ENM_LINK);
	
	m_pRecvEdit->EndDown();
	pTextServices->Release();
	//m_pRecvEdit->SetEventMask(ENM_LINK);

}

//将发送出去的消息转化为json对象
void CUIChatDlg::Msg2Json(Json::Value & val, const TCHAR* lptsz)
{
#ifdef _UNICODE
	if (lptsz == NULL || *lptsz == 0x0)
	{
		return;
	}
	char* lpstr = NULL;
	W2C(&lpstr, lptsz);
	//UnicodeToUtf8()
	std::string str(lpstr);
	NormalMsg msg;
	msg.m_strContent = lpstr;
#if NOSERVICETEST
	msg.m_strFromId = g_user.m_userInfo.id;
#else
	msg.m_strFromId =  g_user.UerSelf()->m_strId;//g_user.m_userInfo.id;//"1004462060";
#endif
	
	SAFE_ARRYDELETE(lpstr);
	W2C(&lpstr , m_pFriendInfo->id.c_str());
	msg.m_strtoId = lpstr;
	SAFE_ARRYDELETE(lpstr);
	msg.MakeJson(val,"Normal", "Normal");
#else 
#endif
}

void CUIChatDlg::_RichEdit_ReplaceSel(ITextServices * pTextServices, CRichEditUI * pRichEdit, LPCTSTR lpszNewText,  const msgfont* pMsgFont/* = NULL*/)
{
	//ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit)
	{//是显示聊天内容的RichEdit

		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			m_fontInfo.m_tstrName.c_str(), m_fontInfo.m_nSize, 
			m_fontInfo.m_clrText, m_fontInfo.m_bBold, m_fontInfo.m_bItalic, 
			m_fontInfo.m_bUnderLine, FALSE, 300);
	}
	else if ( pRichEdit == m_pMsgRecordView)
	{
		tString tstrName;
		StringTotString(tstrName, pMsgFont->name);
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			m_fontInfo.m_tstrName.c_str(), pMsgFont->size, 
			pMsgFont->color, pMsgFont->bold, pMsgFont->italics, 
			pMsgFont->underline, FALSE, 300);
	}
	else
	{//发送的RichEdit
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
}

void CUIChatDlg::_RichEdit_GetText(ITextServices * pTextServices, tString& strText)
{
	REOBJECT reobject;
	LONG nFaceId = 0;
	LONG  nPos = 0;  //记录OLE对象的位置
	tString strOrgText, strTemp;
	IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(pTextServices);
	if (NULL == pRichEditOle)
		return;

	CHARRANGE chrg = {0, RichEdit_GetWindowTextLength(pTextServices)};
	RichEdit_GetTextRange(pTextServices, &chrg, strOrgText);

	for (LONG i = 0; i < (int)strOrgText.size(); i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.cp = i;
		HRESULT hr = pRichEditOle->GetObject(REO_IOB_USE_CP, &reobject, REO_GETOBJ_POLEOBJ);
		if (SUCCEEDED(hr))
		{
			//当前位置上是OLE对象
			if (reobject.cp > 0 && reobject.cp > nPos)
			{
				strTemp = strOrgText.substr(nPos, reobject.cp-nPos);
			//	Replace(strTemp, _T("/"), _T("//"));
				
				strText += strTemp;
			}
			nPos = reobject.cp + 1;
			if (NULL == reobject.poleobj)
				continue;
			else
			{
				nFaceId = reobject.dwUser;
				TCHAR tstr[24] = {0x0};
				//wsprintf(tstr, _T("/f['%03d\']"), nFaceId);
				wsprintf(tstr, _T("[%s]"), m_FaceList.GetFaceInfoById(nFaceId)->m_strTip.c_str());
				strText += tstr;
			}

		}
	}
	if (nPos < (int)strOrgText.size())
	{
		strTemp = strOrgText.substr(nPos);
		strText += strTemp;
	}
	pRichEditOle->Release();
}

LRESULT CUIChatDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	switch(uMsg)
	{
	case WM_CHAR:	
			 break;
	case WM_NOTIFY:
		/*
		NMHDR			*notify;
		notify = (NMHDR*)lParam;
		if (notify->code == EM_SETEVENTMASK)
		{
		//::MessageBox(NULL, _T("fff"), _T("dd"),MB_OK);;
		}
		*/
		{
			NMHDR			*notify;
			notify = (NMHDR*)lParam;
			if (notify->code == DTN_DATETIMECHANGE   )
			{
				//LPNMDATETIMECHANGE lpChage=(LPNMDATETIMECHANGE)lParam; 
				return OnDateChange((LPNMDATETIMECHANGE)lParam);
			}
			break;
		}
		
	case  FACE_CTRL_SEL:
		OnFaceCtrlSel(uMsg, wParam, lParam);
		return 0;
	case  WM_TIMER:
		//::MessageBox(NULL, _T("fff"), _T("dd"),MB_OK);
		 OnTimer(uMsg,wParam, lParam);
		 break;
	case  WM_REICEIVEMSG:
		OnReceiveMsg( uMsg,  wParam, lParam);
			return 0;
	case  WM_SYSCOMMAND:
		{
			if(wParam == SC_MINIMIZE)
			{
				//MessageBox(NULL, _T("最小化"), _T("NULL"), MB_OK);
			}
			else if (wParam == SC_RESTORE)
			{
				//MessageBox(NULL, _T("Restore"), _T("NULL"), MB_OK);
			}
			break;
		}
	case WM_SETFOCUS:
		//MessageBox(NULL, _T("SetFocus"), _T(""), MB_OK);
//		return OnSetFocus(uMsg, wParam, lParam, bHandled);
		break;
	case WM_KILLFOCUS:
		//MessageBox(NULL, _T("KillFocus"), _T(""), MB_OK);
		return OnKillFocus(uMsg, wParam,lParam, bHandled);
		//break;
	default: break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CUIChatDlg::StatrtFlashwindow()
{
	//SetTimer(m_hWnd, 2, 500, NULL);
	FlashWindowEx(&m_flashwInfo);
}

void CUIChatDlg::StopFlashWindow()
{
	//KillTimer(m_hWnd, 2);
}

LRESULT CUIChatDlg::OnDateChange(LPNMDATETIMECHANGE lpCHange)
{
	char  szDate[48] = {0x0};
	//%4d-%02d-%02d %02d:%02d:%02d
	sprintf_s(szDate, "%4d-%02d-%02d ", lpCHange->st.wYear, lpCHange->st.wMonth, lpCHange->st.wDay);
		
	ShowMsgRecord(szDate);
	
	return 0;
}

LRESULT CUIChatDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
// 	//bHandled = false;
// 	if (wParam == 2)
// 	{
// 		if (i%2 == 0)
// 		   //	FlashWindow(m_hWnd, TRUE);
// 		   return 0;
// 		else
// 			FlashWindow(m_hWnd, TRUE);
// 	//	KillTimer(m_hWnd, 2);
// 	}
// 	i++;
	return 0;
}

void CUIChatDlg::OnReceiveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 注意：wParam == TRUE 说明是消息集合
	// FALSE 说明是单条消息记录

	StatrtFlashwindow();
	if ((BOOL)wParam) // 
	{
		NormaMsgCollection* pMsgCollection = (NormaMsgCollection*)lParam;
 		for (vector<NormalMsg*>::iterator it = pMsgCollection->GetMessageCollection().begin(); it != pMsgCollection->GetMessageCollection().end(); ++it)
 		{
			RecvMsg(*it);
 		}
		//::SendMessage(GetOwner(), WM_RECENTCONTACTS, 0 , (LPARAM)(*pMsgCollection->GetMessageCollection().rbegin()));
		delete pMsgCollection;
	}
	else
	{
		NormalMsg* pMsg = (NormalMsg*)lParam;
		RecvMsg(pMsg);
		//
		SAFE_DELETE(pMsg);
	}
	//更新最近联系人
}

LRESULT CUIChatDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	g_mapWnd.erase(m_pFriendInfo->id);
	::RevokeDragDrop(m_hWnd);
	m_FaceList.Reset();
	bHandled = FALSE;
	return 0;
}

LRESULT CUIChatDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//m_bFocus = FALSE;
	__super::OnKillFocus(uMsg, wParam, lParam , bHandled);
	return 0;
}

LRESULT CUIChatDlg::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
// 	bHandled = FALSE;
// 	m_bFocus = TRUE;
// 	if (m_bHasUnreadMsgAndFlash)
// 	{
// 		m_bHasUnreadMsgAndFlash = FALSE;
// 		StopFlashWindow();
// 	}
	return 0;
}


void CUIChatDlg::OnNotifyLinkCallback(void* p, CDuiString linkText, LONG cpMin, LONG cpMax)
{
	CUIChatDlg* pThis = (CUIChatDlg*)p;
	int start = LOWORD(pThis->m_dwLinkTextRange);
	int end = HIWORD(pThis->m_dwLinkTextRange);
	if (cpMin>=start && cpMax <=end)
	{
			::MessageBox(NULL, linkText,_T("LINK"),  MB_OK);
	}
//	::MessageBox(NULL, linkText,_T("LINK"),  MB_OK);
}

void CUIChatDlg::OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COptionUI* pEmotionOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("emotionbtn")));
	if (pEmotionOption != NULL)
	{
		pEmotionOption->Selected(false);
	}
	int nSelFaceId = m_FaceDlg.GetSelFaceId();
	int nFaceIndex = m_FaceDlg.GetSelFaceIndex();
	tString strFileName = m_FaceDlg.GetSelFaceFileName();
	if (!strFileName.empty())
	{
		m_pSendEdit->InsertGIFImage(strFileName.c_str(), nSelFaceId);
	}
	return ;
}

LRESULT CUIChatDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE; 
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(*this, &pt);
	//获取
	CControlUI* pControl = m_PaintManager.FindControl(pt);
	if (pControl == NULL)
	{
		return  0;
	}
	if (pControl == m_pSendEdit)
	{
		ITextServices* pTextServices = m_pSendEdit->GetTextServices();
	}
	else 
		return 0;

	IRichEditOle* pRichEditOle;
	REOBJECT reobject;
	HRESULT hr;

	pRichEditOle = m_pSendEdit->GetIRichEditOle();
	int nCout = pRichEditOle->GetObjectCount();

	for (int i = 0; i < nCout; i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		hr = pRichEditOle->GetObject(i, &reobject,  REO_GETOBJ_POLEOBJ);
		SIZEL szl = reobject.sizel ;
		if (S_OK == hr)
		{
			//IDataObject * pDataObject = NULL;
// 			if (IID_IDataObject == reobject.clsid)
// 			{
// 				IDataObject* pIdataObject = NULL;
// 			}
			IDataObject* pIdataObject = NULL;
			hr = reobject.poleobj->QueryInterface(IID_IDataObject, (void **)&pIdataObject);
			if(hr == S_OK)
			{
				FORMATETC	fmte = 	{ CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
				STGMEDIUM		medium = { 0 };
				//hr = pIdataObject->GetData(&fm, &stgMed);
#if 0
				TCHAR* tstr = _T("c:/wwww/sfs/s.png");

				HGLOBAL hgl = GlobalAlloc(GHND, (_tcslen(tstr)*2)+1);
				medium.hGlobal = GlobalLock(hgl);
				memcpy(medium.hGlobal, tstr
					, _tcslen(tstr));
				hr = CreateDataObject(&fmte, &medium,1, &m_pIDragObj);
				GlobalUnlock(hgl);
#endif
 				//m_bDraged = TRUE;
				return 0;
			}
			else
			{
				//TODO: 
				CGifOleObject* pGif = (CGifOleObject*)(reobject.poleobj);
				
				return 0;
			}
		}
	}
	return __super::OnLButtonDown(uMsg,wParam,lParam, bHandled);
}

LRESULT CUIChatDlg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
// 	DWORD dbEffect  = DROPEFFECT_NONE;
// 	if (m_bDraged)
// 	{
// 
// 		IDropSource* pDragSource ;
// 		HRESULT hr = S_FALSE;
// 
// 		CreateDropSource(&pDragSource);
// 
// 	hr = DoDragDrop(m_pIDragObj, pDragSource, DROPEFFECT_COPY| DROPEFFECT_MOVE, &dbEffect);
// 		m_pIDragObj->Release();
// 		pDragSource->Release();
// 		m_bDraged = FALSE;
// 	}
	
	

// 	switch(dbEffect)
// 	{
// 
// 	}
	return __super::OnMouseMove(uMsg,wParam, lParam, bHandled);
}

LRESULT CUIChatDlg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if(m_bDraged)
	{
		m_bDraged = FALSE;
		bHandled = TRUE;
	}
	return 0;
}

//下拉框改变字体大小
void CUIChatDlg::OnCmb_SelChange_FontSize(TNotifyUI& msg)
{
	//tString tStrSize = m_pFontSizeCmb->GetText().GetData();
	int nSize = _ttol(m_pFontSizeCmb->GetText().GetData());
	if (m_fontInfo.m_nSize != nSize)
	{
		m_fontInfo.m_nSize = nSize;
		ITextServices* pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
	}
}

//下拉框改变字体类型
void CUIChatDlg::OnCmb_Selchange_FontName(TNotifyUI& msg)
{

	tString tstrText = (TCHAR*)m_pFontNameCmb->GetText().GetData();
	if (m_fontInfo.m_tstrName != tstrText)
	{
		m_fontInfo.m_tstrName = tstrText;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),m_fontInfo.m_nSize,m_fontInfo.m_clrText,m_fontInfo.m_bBold,m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
	}
}

void CUIChatDlg::OnBtn_Color(TNotifyUI& msg)
{
	static COLORREF rgbCustomColors[16] =
	{
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
	};

	CHOOSECOLOR cc = {0};
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = rgbCustomColors;
	cc.hwndOwner = m_hWnd;
	cc.Flags = CC_RGBINIT;

	BOOL bRet = ::ChooseColor(&cc);
	if (bRet)
	{
		COptionUI* pColroOption = static_cast<COptionUI*>(msg.pSender);
		pColroOption->Selected(!pColroOption->IsSelected());
		COLORREF clrText = cc.rgbResult;
		if (m_fontInfo.m_clrText != clrText)
		{
			m_fontInfo.m_clrText = clrText;
			ITextServices * pTextServices = m_pSendEdit->GetTextServices();
			RichEdit_SetDefFont(pTextServices, m_fontInfo.m_tstrName.c_str(),
				m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
				m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
			pTextServices->Release();
		}
	}
}

void CUIChatDlg::SetEachPageRecordNum(int num)
{
	m_nEachPageRecordNum = num;
}

int CUIChatDlg::GetEachPageRecordNum()
{
	return m_nEachPageRecordNum;
}

void CUIChatDlg::SetOwner(HWND hwnd)
{
	m_hMainFrame = hwnd;
}

HWND CUIChatDlg::GetOwner()
{
	return m_hMainFrame;
}

/*
-----------------------------------------------------------------------------------------------------------------*/



void InputViewEvent::GetFileType(const tString& strFilename, E_FILEFORMAT* format, int* suffix )
{
	//TCHAR  tDot = 0x2e;
	int nPos = -1;
	int nLen = strFilename.length();
	for (int i = nLen - 1; i >= 0 ; i--)
	{
		if (strFilename[i] == 0x2e)
		{
			nPos = i;
			break;
		}
	}
	/*E_PIC_GIF = 0,
	E_PIC_PNG,
	E_PIC_BMP,
	E_PIC_PNG,
	E_PIC_JPG,
	E_PIC_JPEG,
	E_PIC_EXIF,
	E_PIC_TIFF	*/
	tString tstrSuffix = strFilename.substr(nPos+1, nLen);
	tString tstrPicSuffixArry[7]= {_T("gif"), _T("png"), _T("bmp"), _T("jpg"), _T("jpeg"), _T("exif"), _T("tiff")};
	tString tstrLowSuffix = tstrSuffix;
	ToLower(tstrLowSuffix);
	for (int i = 0; i < 7; i++)
	{
		if (tstrLowSuffix == tstrPicSuffixArry[i])
		{
			*format = E_PICTURE;
			*suffix = i;
			return;
		}
	}
	*format = E_NORMALFILE;
}

InputViewEvent::InputViewEvent(CRichEditUI *pReUI,const tString& bgimage,DWORD bkcolor)
{
	m_pReUI = pReUI;
	bgimage_ = bgimage;
	bkcolor_ = bkcolor;
}

InputViewEvent::~InputViewEvent()
{

}

bool InputViewEvent::OnDragDropOver( const tString& pFilePath )
{

	return true;
}
bool InputViewEvent::OnDropDown(const tString& pFilePath,int nPos )
{
	if( m_pReUI != NULL)
	{
		//m_pReUI->InsertNotGIFImage()
		
		E_FILEFORMAT  efileformat ;
		int epicsuffix;
		GetFileType(pFilePath, &efileformat, &epicsuffix);

		if (efileformat == E_PICTURE)
		{
			if (epicsuffix == E_PIC_GIF)
			{
				return m_pReUI->InsertGIFImage(pFilePath .c_str(), 0x0);
			}
			else
				return m_pReUI->InsertNotGIFImage(pFilePath.c_str(), 0x0);
		}
	}
		
	return false; 
}

bool InputViewEvent::OnDropLeave( const tString& pFilePath )
{

	return true;
}

void InputViewEvent::OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight )
{
#if 0
	if( dwType == 1 )
	{
		LPCTSTR pPath = (LPCTSTR)pObj;
		::MessageBox(NULL,pPath,"OnClickOleObject",MB_OK);
	}
	if( dwType == 2 )
	{
		CHorizontalLayoutUI* new_h_lay = (CHorizontalLayoutUI*)pObj;
		if( new_h_lay )
		{
			CProgressUI* pProgress = (CProgressUI*)new_h_lay->FindSubControl("down_progress");
			if( pProgress )
				pProgress->SetValue((rand()%100));
		}
	}
#endif
}

void InputViewEvent::OnDeleteOleObject(void* pObj,DWORD dwType)
{
	if( dwType == 1 )
	{
		LPCTSTR pPath = (LPCTSTR)pObj;
		::MessageBox(NULL,pPath,_T("OnDeleteOleObject"),MB_OK);
	}
	if( dwType == 2 )
	{
		::MessageBox(NULL,_T("ControlUI"),_T("OnDeleteOleObject"),MB_OK);
	}
}

void InputViewEvent::OnUrl(void* pObj,DWORD dwType,long lPos)
{
#if 0
	if( dwType == 1 )
	{
		ViewImageDialog* pViewDialog = new ViewImageDialog(bgimage_,bkcolor_,(LPCTSTR)pObj);
		pViewDialog->Create(NULL, _T("ViewImageDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
		pViewDialog->CenterWindow();
		::ShowWindow(*pViewDialog, SW_SHOW);
	}
	if( dwType == 2 )
	{
		CHorizontalLayoutUI* new_h_lay = (CHorizontalLayoutUI*)pObj;
		if( new_h_lay )
		{
			CProgressUI* pProgress = (CProgressUI*)new_h_lay->FindSubControl("down_progress");
			if( pProgress )
				pProgress->SetValue((rand()%100));
		}
	}
#endif
}
