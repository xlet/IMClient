#include "StdAfx.h"
#include "MainFrame.h"
#include "UIChatDlg.h"
#include "UIStatusMenu.h"
#include "UIMenu.h"
//#include "UIEnterpriseWnd.h"
#include "resource.h"
#include "FriendGroup.h"
#include "SourceDownloadThread.h"
#include "WOAMember.h"
//控件名称，根据其获取控件的指针
const TCHAR*  const_userstatusbtn = _T("userstatusbtn");
const TCHAR*  const_searchtipbtn = _T("searchtip");
const TCHAR*  const_searchedit = _T("searchedit");
const TCHAR*  const_signaturebtn =  _T("signaturetip");
const TCHAR*  const_signatureedit = _T("signature");
const TCHAR*  const_friendsoption = _T("friendsoption");
const TCHAR*  const_recentcontactoption = _T("recentoption");
const TCHAR*  const_appoption     = _T("appoption");
const TCHAR*  const_listtablayout =  _T("listtab");
const TCHAR*  const_friendlistlayout = _T("friendlisthlayout");
const TCHAR*  const_recentcontactlistlayout = _T("recentcontactslistlayout");
const TCHAR*  const_applistlayout = _T("applayout");
const TCHAR*  const_friendlogocontrol  = _T("friendlogo");
const TCHAR*  const_recentlogocontrol = _T("recentlogo");
const TCHAR*  const_applogocontrol = _T("applogo");
const TCHAR*  const_friendarrowcontrol =  _T("friendsarrow");
const TCHAR*  const_recentarrowcontrol = _T("recentarrow");
const TCHAR*  const_apparrowcontrol = _T("apparrow");

const TCHAR*  const_friendlist = _T("friendlist");
const TCHAR*  const_recentcontactlist = _T("rencentlist");
const TCHAR*  const_applicationlist = _T("applicationlist");

const TCHAR*  const_syssetbtn = _T("sysIcon1_btn");
const TCHAR*  const_closebtn = _T("closebtn");
const TCHAR*  const_minbtn = _T("minbtn");

extern  const char* const_type_logout_requestval ;
extern const char* const_msgtype_logout_response;

CMainFrame::CMainFrame(void)
{
	m_tabState.mousevoer = NONE;
	m_tabState.select = FRIENDS;
	m_pUISyssetDlg = NULL;
	m_bIsIcon  = TRUE;
	m_pBtnUserHead = NULL;
	m_pFriendsList = NULL;
	m_nGroupId = 0;
	m_nCurSel = 0; // 默认刚开始选中的是第一个标签
	m_pSerchEdit = NULL;
	m_pMainview = NULL;
	m_pSearchView = NULL;
//	m_pFriendUpdateThread = NULL;
}

CMainFrame::~CMainFrame(void)
{
	//PostQuitMessage(0);
	for (hash_map<tString, FriendListItemInfo*>::iterator it = m_FriendItemInfo.begin(); it!=m_FriendItemInfo.end(); ++it)
	{
		FriendListItemInfo* p = it->second;
		SAFE_DELETE(p);
	}
	m_FriendItemInfo.clear();
// Test
	int nSize1 = m_notReadMessages.GetMesageCollectionMap().size();
	int nSize2 = CGlobalData::GetInstance()->OfflinMessages().GetMesageCollectionMap().size();
}

 LPCTSTR CMainFrame::GetWindowClassName() const 
{
	return _T("mainWnd");
}

CDuiString CMainFrame::GetSkinFile()
{
	//return _T("duilib.xml");
	return _T("mainframe.xml");
	//return _T("DiuTest2_bk.xml");
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL  bHandled = TRUE;
	if (uMsg >= WM_IMSOCKET)
	{
		m_pIMSock->HandleMessage(uMsg,wParam,lParam,bHandled);
		if(bHandled)
		{
			return 0;
		}
	}
	else
	{
		switch(uMsg)
		{
		case WM_TRAYMSG:
			return OnTrayMsg(wParam, lParam);
		case WM_TIMER:
			OnTimer(wParam, lParam);break;
		case WM_FRIENDGROUP:
			return OnFriendGoup(wParam, lParam);
		case WM_SETEXISTINGPICTURE:
			return OnSetExistingPicture(wParam, lParam);
		case WM_SETDOWNLOADPICTURE:
			return OnSetDownloadPicture(wParam, lParam);
		case WM_SETUSERINFO:
			return OnSetUserInfo(wParam, lParam);
		case WM_OFFLINEMESSAGES:
			return OnOfflineMessages(wParam,lParam);
		case WM_CHATMSGWHENNOWND:
			return OnChatMessagesOfNoWnd(wParam, lParam);
		case WM_SETMEMBERSTATUS:
			return OnSetMemberStatus(wParam, lParam);
		case WM_RECENTCONTACTS:
			return OnRecentContacts(wParam, lParam);
		default:  break;

		}
	}
#if 0
	else if (uMsg == WM_TRAYMSG)
	{
			return OnTrayMsg(wParam, lParam);
	}
	else if (uMsg == WM_TIMER)
	{
		OnTimer(wParam, lParam);
	}
	else if (uMsg == WM_FRIENDGROUP)
	{
		return OnFriendGoup(wParam, lParam);
	}
	else if (uMsg == WM_SETEXISTINGPICTURE)
	{
		return OnSetExistingPicture(wParam, lParam);
	}
	else if (uMsg == WM_SETDOWNLOADPICTURE)
	{
		return OnSetDownloadPicture(wParam, lParam);
	}
	else if (uMsg == WM_SETUSERINFO)
	{
		return OnSetUserInfo(wParam, lParam);
	}
#endif
	return __super::HandleMessage(uMsg, wParam, lParam);
}

HWND  CMainFrame::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle,CIMSocket *pSock)
{
	HWND hwnd = __super::Create(hwndParent,pstrName, dwStyle, dwExStyle);
	pSock->Create(hwnd);
	G_IMSOCKET = m_pIMSock = pSock;
	return hwnd;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
#if 1
	if(msg.sType == _T("selectchanged"))
	{
		OnTab_SelChanged(msg);
	}

	else if (_tcsicmp(msg.sType, _T("windowinit"))==0)
	{
		OnWindowInit(msg);
	}
	else if (IsStrEqual(msg.sType, _T("click")))
	{
		if (_tcsicmp(msg.pSender->GetName(), const_searchtipbtn) == 0)
		{
			OnBtn_SearchTip(msg);
		}
		//显示系统设置
		else if (IsStrEqual(msg.pSender->GetName(),const_syssetbtn))
		{
			OnBtn_SysSet(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_searchedit) == 0)
		{
		
		}
		else if (IsStrEqual(msg.pSender->GetName(), _T("searchbtn")))
		{
			OnBtn_Serach(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(),const_signaturebtn) == 0)
		{
			OnBtn_Signature(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), const_userstatusbtn))
		{
			OnBtn_UserStatus(msg);
		}
		else if (IsStrEqual(msg.pSender->GetName(), const_closebtn))
		{
			OnBtn_Close(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_minbtn) == 0)
		{
			OnBtn_Minimize(msg);
		}
	}
	else if (IsStrEqual(msg.sType, _T("return")))
	{

		OnReturn(msg);//MessageBox(NULL, _T("RETURN"), _T(""), MB_OK);;
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		if (IsStrEqual(msg.pSender->GetName(), const_searchedit))
		{
			OnKillFocus_SerchEdit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), const_signatureedit) == 0)
		{
			OnKillFocus_SignatureEdit(msg);
		}
	}
	else if (IsStrEqual(msg.sType, _T("textchanged")))
	{
		//OnTextChanged(TNotifyUI& );
	}
// 	else if (_tcsicmp(msg.sType, _T("menu"))==0)
// 	{
// 		//::MessageBox(NULL, _T("OK"),_T("Menu"),MB_OK);
// 		return;
// 	}
	else if (_tcscmp(msg.sType, _T("rclick")) == 0)
	{
		//::MessageBox(NULL, _T("menu"), _T(""), MB_OK);
		OnRItemClick(msg);
	}
	else if (_tcsicmp(msg.sType, _T("lclick")/*_T("itemclick")*/ )== 0)
	{
		OnLItemClick(msg);
	}

	else if( IsStrEqual(msg.sType, _T("itemactivate")) ) 
	{
		OnDbItemClick(msg);
	}
	else if (IsStrEqual(msg.sType, _T("Online")))
	{
		OnMenu_Online(msg);
	}
	else if (IsStrEqual(msg.sType, _T("leave")))
	{
		OnMenu_Leave(msg);
	}
	else if (IsStrEqual(msg.sType, _T("busy")))
	{
		OnMenu_Busy(msg);
	}
	else if (IsStrEqual(msg.sType, _T("undisturb")))
	{
		OnMenu_UnDisturb(msg);
	}
	else if(IsStrEqual(msg.sType, _T("stealth")))
	{
		OnMenu_Stealth(msg);
	}
	else if (IsStrEqual(msg.sType, _T("offline")))
	{
		OnMenu_Offline(msg);
	}
#endif
	
	else
		__super::Notify(msg);
}

void  CMainFrame::InitWindow()
{	
	m_pBtnUserHead = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("bigpersonbtn")));
	m_pTxtNickName = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("nicknametext")));
    m_pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_friendlist));
	m_pRecentContactsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_recentcontactlist));
	m_pSerchEdit =  static_cast<CEditUI*>(m_PaintManager.FindControl(_T("searchedit")));
	m_pMainview = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("mainview")));
	m_pSearchView = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("sercchShowView")));
}

void CMainFrame::OnWindowInit(TNotifyUI& msg)
{
	ToTray();
	//BeginFlash();
	OnPrepare(msg);
}

void CMainFrame::OnTab_SelChanged(TNotifyUI& msg)
{
	CDuiString    strName     = msg.pSender->GetName();
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(const_listtablayout));

	if(strName == const_friendsoption)
		pControl->SelectItem(0);
	else if(strName == const_recentcontactoption)
		pControl->SelectItem(1);
	else if(strName == const_appoption)
		pControl->SelectItem(2);
	m_nCurSel = pControl->GetCurSel();
}

void CMainFrame::OnPrepare(TNotifyUI& msg)
{
	//UpdateFriendsList();
	//UpdateRecentList();
#if NOSERVICETEST
	std::map<tString, FriendGroup*>& friendList = FriendList::GetInstance()->GetFriendList();
	for (std::map<tString, FriendGroup*>::iterator it = friendList.begin(); it!=friendList.end(); ++it)
	{
		UpdateGroup(it->second);
	}
#endif
	
}

void CMainFrame::HideSerchEdit(CControlUI* pEdit)
{
	pEdit->GetParent()->SetVisible(false);
	CControlUI* searchtip_btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_searchtipbtn));
	if (searchtip_btn != NULL)
	{
		CEditUI* search_edit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_searchedit));
		if (search_edit != NULL)
		{
			searchtip_btn->SetText(search_edit->GetText());
		}
		searchtip_btn->GetParent()->SetVisible(true);
	}
}

//关闭主窗口，退出程序
void CMainFrame::Close()
{
	//G_IMSOCKET->SendData()
	LogoutRequest requst;
	//requst.Init(const_type_logout_requestval, const_msgtype_logout_response);
	requst.m_strLoginId = g_user.m_userInfo.id ;
	Json::Value val;
	requst.MakeJson(val,const_type_logout_requestval,const_msgtype_logout_response);
	//发送退出消息
	G_IMSOCKET->SendData(val);
	//////////////////////////////////////////////////////////////////////////
	DeleteTray();
	::PostQuitMessage(0);
}

void CMainFrame::OnBtn_SearchTip(TNotifyUI& msg)
{
	msg.pSender->GetParent()->SetVisible(false);
	CEditUI* search_edit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_searchedit));
	if (search_edit != NULL)
	{
		search_edit->SetFocus();
		search_edit->GetParent()->SetVisible(true);
		search_edit->SetVisible(true);
		search_edit->SetFocus();  //必须将编辑框设为焦点
		search_edit->SetText(msg.pSender->GetText());
		int nStrLen = _tcslen(msg.pSender->GetText().GetData());
 		search_edit->SetSel(nStrLen, nStrLen);	

		//搜索框获得焦点
		DisplaySearchView(TRUE);
		SetSearchBtnState(TRUE);
	}
}

void CMainFrame::OnBtn_SysSet(TNotifyUI& msg)
{
	if (m_pUISyssetDlg  == NULL || !::IsWindow(*m_pUISyssetDlg))
	{
		m_pUISyssetDlg = new CUISysSetDlg();
		if (m_pUISyssetDlg != NULL)
		{
			m_pUISyssetDlg->Create(NULL, _T("系统设置界面"),UI_WNDSTYLE_DIALOG, 0);
		}
		m_pUISyssetDlg->CenterWindow();
		::ShowWindow(*m_pUISyssetDlg,TRUE);
	}
}

void CMainFrame::OnBtn_Signature(TNotifyUI& msg)
{
	msg.pSender->GetParent()->SetVisible(false);
	CEditUI* signature_edit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_signatureedit));
	if (signature_edit != NULL)
	{
		signature_edit->SetText(msg.pSender->GetText());
		signature_edit->GetParent()->SetVisible(true);
		signature_edit->SetFocus();
		signature_edit->SetSelAll();
	}
}

void CMainFrame::OnBtn_UserStatus(TNotifyUI& msg)
{
	CUIStatusMenu* pMenu = new CUIStatusMenu();
	if(pMenu == NULL)
		return;
	POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
	::ClientToScreen(m_hWnd, &pt);
	pMenu->Init(msg.pSender, pt);
}

void CMainFrame::OnKillFocus_SerchEdit(TNotifyUI& msg)
{
	HideSerchEdit(msg.pSender);
	DisplaySearchView(FALSE);
	SetSearchBtnState(FALSE);
}

void CMainFrame::OnKillFocus_SignatureEdit(TNotifyUI& msg)
{
	msg.pSender->GetParent()->SetVisible(false);
	CControlUI*  signature_btn = m_PaintManager.FindControl(const_signaturebtn) ;
	if (signature_btn != NULL)
	{
		CEditUI* signature_edit = static_cast<CEditUI*>(msg.pSender);
		signature_btn->SetText(signature_edit->GetText());
		signature_btn->GetParent()->SetVisible(true);
	}
}

void CMainFrame::OnLItemClick(TNotifyUI& msg)
{
#if 1
	//::MessageBox(NULL, _T("OK"),_T("WWWWWW"),MB_OK);
/*	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_friendlist));*/
	if ( m_pFriendsList->GetItemIndex(msg.pSender) != -1)
	{

		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();

			if (m_pFriendsList->CanExpand(node))
			{
				msg.pSender->SetBkColor(0xFFFFFFFF);
				m_pFriendsList->SetChildVisible(node, !node->data().child_visible_);
			}
		}
	}
	return;
#endif
}

void CMainFrame::OnBtn_Close(TNotifyUI& msg)
{
	Close();
}

void CMainFrame::OnBtn_Minimize(TNotifyUI& msg)
{
	ShowWindow(false);
}

void CMainFrame::OnRItemClick(TNotifyUI& msg)
{
	CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
	CPoint point = msg.ptMouse;
	ClientToScreen(m_hWnd, &point);
	STRINGorID xml(_T("menutest.xml"));
	pMenu->Init(NULL, xml, _T("xml"), point);
}

void CMainFrame::OnDbItemClick(TNotifyUI& msg)
{
	//双击Item弹出聊天框
	CTabLayoutUI* pTabControl= static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(const_listtablayout));
	if (pTabControl != NULL)
	{
		int nSel = pTabControl->GetCurSel() ;
		CFriendsUI* pFriendList = NULL;
		if (nSel == 0)
			 pFriendList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_friendlist));
		else if (nSel == 1)
			pFriendList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_recentcontactlist));
		//if (nSel ==0 )
		//{
			if ((pFriendList != NULL) &&  pFriendList->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag(); //获取节点相关信息
					//CControlUI* bkground =m_PaintManager.FindControl(kBackgroundControlName); 
					//判断Item是否可以伸展，可以伸展的表示是分组节点
					if (!pFriendList->CanExpand(node) /*&& (background != NULL)*/)
					{
						//双击的时候存贮当前选择的好友的信息
					//	FriendListItemInfo  friend_info; 

// 						
// 						for (std::list<FriendListItemInfo>::const_iterator citer = m_vcFriendInfo.begin(); citer != m_vcFriendInfo.end(); ++citer)
// 						{
// 							if (_tcsicmp(citer->id.c_str(), node->data().value.c_str()) == 0)
// 							{
// 								friend_info = *citer;
// 								break;
// 							}
// 						}

// 						//测试 使用
// 						if(IsStrEqual(friend_info.id.c_str() , _T("4")))
// 						{
// #if 0
// 							CUIEnterpriseWnd *pEnterprisDlg = new CUIEnterpriseWnd;
// 							if (pEnterprisDlg == NULL)
// 							{
// 								return;
// 							}
// 							pEnterprisDlg->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
// 
// 							//pChatDialog->Create(NULL, _T("ChatDialog"),UI_WNDSTYLE_DIALOG,  NULL, 0, 0, 0, 0);
// 							pEnterprisDlg->CenterWindow();
// 							::ShowWindow(*pEnterprisDlg, SW_SHOW);
// #endif
// 						}
// 
// 						//此处注意需要将指针释放掉
// 						else{
							ShowChatWindow(node->data().pItemData->id);
//						}
					}
				}
			}
		//}
	}
	return;
}

void CMainFrame::OnReturn(TNotifyUI& msg)
{
	if (_tcsicmp(msg.pSender->GetName(), const_searchedit) == 0)
	{
		//按下回车键后搜索
		::SetFocus(m_hWnd);
		//Search(m_pSerchEdit->GetText().GetData());
		//HideSerchEdit(msg.pSender);
	}
	else if (_tcsicmp(msg.pSender->GetName(), const_signatureedit) == 0)
	{
		//
	}
}

void CMainFrame::OnMenu_Online(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status1.png' dest='36,4,47,15'"));
		//pButtonUI->SetBkImage()
		//pButtonUI->SetNormalImage(_T("file='icon_status1.png' dest='36,4,47,15"));
	}
}

void CMainFrame::OnMenu_Leave(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status2.png' dest='36,4,47,15'"));
	}
}

void CMainFrame::OnMenu_Busy(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status3.png' dest='36,4,47,15'"));
	}
}

void CMainFrame::OnMenu_UnDisturb(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status4.png' dest='36,4,47,15'"));
	}
}

void CMainFrame::OnMenu_Stealth(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status5.png' dest='36,4,47,15'"));
	}
}

void CMainFrame::OnMenu_Offline(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status6.png' dest='36,4,47,15'"));
	}
}
//屏蔽Enter和Esc键
LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) {
		if( wParam == VK_RETURN ) {

			return true;
		}
		else if( wParam == VK_ESCAPE ) {
			return true;
		}

	}
	return false;
}

//  如果服务器是每次登陆发送的是好友变更,那么UpdateGroup中实际上也应该是加载
void CMainFrame::UpdateGroup( FriendGroup* pGroup)
{
	if (m_pFriendsList == NULL)
		return;
	
	FriendListItemInfo* pItemRoot = new FriendListItemInfo;
	pItemRoot->folder =true;
	pItemRoot->empty = false;
	TCHAR tszGroupId[8]; 
	_itot(m_nGroupId, tszGroupId, 10);
	//StringTotString(pItemRoot->id, (pGroup->GetFriendGroupInfo())./*m_strGroupId *//*服务器返回为空的ID*/);
	pItemRoot->id = tszGroupId;
	StringTotString(pItemRoot->nick_name,  (pGroup->GetFriendGroupInfo()).m_strName);
	
	Node* root_parent = m_pFriendsList->AddNode(pItemRoot, NULL);
	//root_parent->data().child_visible_ = true;
	  // 对于组组来说组名就是唯一的表示符， 不存在重复的组， 
	m_mapGroupNode.insert(make_pair(pItemRoot->nick_name, root_parent)) ;
	//m_vcFriendInfo.push_back(item);
	map<tString, BaseMember*>& memberMap = pGroup->GetFriendGroupInfo().GetMemberMap();
	
	pGroup->SetTag((UINT_PTR)root_parent);

	//因为memberMap插入是默认按id 排序的所以在好友列表中显示的时候是按照列表ID从上往下排的
	UINT_PTR tag = NULL;
	for (map<tString, BaseMember*>::iterator it=memberMap.begin(); it!=memberMap.end(); ++it)
	{
		WOAMember* pMember = (WOAMember*)(it->second);
		FriendListItemInfo* pItem = new FriendListItemInfo;
		pItem->id = it->first;
		pItem->folder = false;
		pItem->status = FRIEND_OFFLINE;   //初始状态都是
		pItem->logo = _T("defaultuserHead.png");
		StringTotString(pItem->nick_name, pMember->m_strNickName);
		StringTotString(pItem->description , pMember->m_strSignature);
		StringTotString(pItem->groupId,  pGroup->GetFriendGroupInfo().m_strGroupId);
		StringTotString(pItem->mail, pMember->m_strEmail);
		StringTotString(pItem->mobile, pMember->m_strMobile);
		StringTotString(pItem->goupName, pGroup->GetFriendGroupInfo().m_strName);
		StringTotString(pItem->post, pMember->m_strPost);

		tag = (UINT_PTR)(m_pFriendsList->AddNode(pItem, root_parent));
		pMember->SetTag(tag);
		pItem->SetTag(tag);
		m_FriendItemInfo.insert(make_pair(it->first, pItem));
	}
	m_nGroupId++;
}

void CMainFrame::UpdateFriendsList()
{
#if 0
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(const_friendlist));
	if (pFriendsList != NULL)
	{//if (!m_vcFriendInfo.empty())
//	m_vcFriendInfo.clear();
	if (pFriendsList->GetCount() > 0)
		pFriendsList->RemoveAll();

	//以下的代码是添加好友及其分组 
	// all note by zhaohaibo
	//添加我的好友列表
	FriendListItemInfo item;
	item.id = _T("0");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("我的好友");

	//我的好友
	Node* root_parent = pFriendsList->AddNode(item, NULL);
	//m_vcFriendInfo.push_back(item);

	item.id = _T("1");
	item.folder = false;		
	item.logo = _T("defaultuserHead.png");
	item.nick_name = _T("白雪公猪");
	item.description = _T("1004462060@qq.com");
	myself_info_ = item;
	//在我的好友下添加一个节点也就是“tojen”自己
	//root_parent 为父节点（"我的好友"）
	//note by zhaohaibo 2014-0321
	pFriendsList->AddNode(item, root_parent);
	//m_vcFriendInfo.push_back(item);

	item.id = _T("2");
	item.folder = false;
	item.logo = _T("bigpersonbtn.png");
	item.nick_name = _T("梦入神鸡");
	item.description = _T("1004462060@qq.com");
	pFriendsList->AddNode(item, root_parent);
	//m_vcFriendInfo.push_back(item);

	item.id = _T("2");
	item.folder = false;
	item.logo = _T("bigpersonbtn.png");
	item.nick_name = _T("菊花好香");
	item.description = _T("1004462060@qq.com");
	pFriendsList->AddNode(item, root_parent);
	//m_vcFriendInfo.push_back(item);


	for( int i = 0; i < 100; ++i )
	{
		item.id = _T("2");
		item.folder = false;
		item.logo = _T("defaultuserHead.png");
		item.nick_name = _T("Buddy");
		item.description = _T("who's your daddy  who's your daddy who's your daddy who's your daddy who's your daddy who's your daddy  !");
		pFriendsList->AddNode(item, root_parent);
	//	m_vcFriendInfo.push_back(item);
	}

	//注意AddNod的第二个参数为NULL的时候是添加根节点 note by zhaohaibo
	item.id = _T("3");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("企业好友");
	Node* root_parent2 = pFriendsList->AddNode(item, NULL);
//	m_vcFriendInfo.push_back(item);

	///////
	item.id = _T("4");
	item.folder = false;
	item.logo = _T("bigpersonbtn.png");
	item.nick_name = _T("w信息技术有限公司");
	item.description = _T("");
	pFriendsList->AddNode(item, root_parent2);
//	m_vcFriendInfo.push_back(item);

	item.id = _T("5");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("陌生人");
	Node* root_parent3 = pFriendsList->AddNode(item, NULL);
//	m_vcFriendInfo.push_back(item);


	item.id = _T("6");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("黑名单");
	Node* root_parent4 = pFriendsList->AddNode(item, NULL);
	//m_vcFriendInfo.push_back(item);
	}
#endif
}

void CMainFrame::UpdateRecentList(RecentContact& recentContact, int nIndex/* = -1*/)
{
	/*
		nIndex = -1  默认在列表中添加
		nIndex = -2  列表Item顺序置顶
		nIdex >=0    添加在nIndex处
	*/
	if (nIndex == -2)
	{
		FriendListItemInfo  *pItem = (FriendListItemInfo*)recentContact.GetTag();
		StringTotString(pItem->description, recentContact.m_strContent);
		CListContainerElementUI* pList = ((Node*)pItem->GetTag())->data().list_elment_;
		m_pRecentContactsList->SetItemIndex(pList, 0); 
		CLabelUI* description = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pList, _T("description")));
		description->SetText(pItem->description.c_str());
		description->SetToolTip(pItem->description.c_str());
	}
	else
	{
		tString tstrId;          // 发送消息的好友
		tString tstrContent;
		//tString tstrNickName;   // 发送消息的好友姓名
		
		StringTotString(tstrId, /*const_cast<RecentContact&>(*it)*/recentContact.m_strId);
		MAP::iterator itFind = m_FriendItemInfo.find(tstrId);
		if (itFind != m_FriendItemInfo.end())
		{// 理论上在m_FriendItemInfo中都能找到

			FriendListItemInfo* pItem_ = itFind->second;   //好友列表中的item信息
			FriendListItemInfo* pItem = pItem_->Clone(); 

			StringTotString(tstrContent, /*const_cast<RecentContact&>(*it)*/recentContact.m_strContent);
// 			if (recentContact.m_eLastMsgTye == E_RECEIVE)
// 				pItem->description = pItem->nick_name+_T(":")+ tstrContent;
// 			else
// 			{
// 				tString tsreUserName;
// 				StringTotString(tsreUserName, g_user.UerSelf()->m_strNickName);
// 				pItem->description = tsreUserName+_T(":")+ tstrContent;
// 			}
			pItem->description =  tstrContent;
			Node* pNode_ = (Node*)pItem_->GetTag();
			Node* pNode = m_pRecentContactsList->AddNode(pItem, NULL, nIndex);

		
			// 二者实际上是同一个member的节点SetlinkNode让二者知道彼此
			pNode_->SetlinkNode(pNode);
			pNode->SetlinkNode(pNode_); 
			pItem->SetTag(UINT_PTR(pNode));
			recentContact.SetTag((UINT_PTR)pItem); // 因为自定义了比较运算符所以此处修改不会导致set中的元素排序发生错乱
			///
			m_mapRecentContatct.insert(make_pair(recentContact.m_strId, &recentContact));
		}
		else
		{
			// 说明出错了， 理论上不存在这种qingkuang
			OutputDebugString(_T("UpdateRecentList Error.Can't find member in m_FriendItemInfo \n"));
		}
	}
	
}

void CMainFrame::UpdateRecentList(FriendListItemInfo* pItem)
{
// 	CListContainerElementUI* pList = ((Node*)pItem->GetTag())->data().list_elment_;
// 	m_pFriendsList->SetItemIndex(pList, 0); 
// 	CLabelUI* description = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pList, _T("description")));
// 	description->SetText(pItem->description.c_str());

}

void CMainFrame::UpdateRecentList(NormalMsg* pMsg/* = NULL*/)
{
	//pMsg为NULL的时候， 表示处理初始化时服务端发送的最近联系人
	RecentContactSet& contacts = CGlobalData::GetInstance()->RecentContacts().RecentContacts();

	//先清除列表
#if 1
	//m_pRecentContactsList->RemoveAll();
	
	// 收到服务器的最近联系人列表
	if (pMsg == NULL)
	{
		int nIndex=0;
		for (RecentContactSet::iterator it = contacts.begin(); it != contacts.end(); ++it)
		{
			UpdateRecentList(const_cast<RecentContact&>(*it), nIndex++);
		}
	}
	// 用户接收到或者发送消息时候更新最经联系人列表
	else
	{
		//存储最近联系人需要更新的信息
		RecentContact contact;
		contact.m_dbLastTime = pMsg->m_baseMsgInfo.m_dbSendTime;
		contact.m_strContent = pMsg->m_strContent;
#if NOSERVICETEST 
		if(pMsg->m_strFromId==g_user.m_userInfo.id/*?pMsg->m_strtoId:pMsg->m_strFromId*/)
#else
		if(pMsg->m_strFromId==g_user.UerSelf()->m_strId/*?pMsg->m_strtoId:pMsg->m_strFromId*/)
#endif
		{
			contact.m_strId = pMsg->m_strtoId;  // 当前的联系人
			contact.m_eLastMsgTye = E_SENDOUT;
		}
		else
		{
			contact.m_strId = pMsg->m_strFromId;  // 当前的联系人
			contact.m_eLastMsgTye = E_RECEIVE;
		}
		
		//contact.data().m_eLastMsgTye
		RecentContactSet& recentContacts = CGlobalData::GetInstance()->RecentContacts().RecentContacts();
		map<string, RecentContact*>::iterator it_ = m_mapRecentContatct.find(contact.m_strId);
		if (it_ != m_mapRecentContatct.end())
		{
			//RecentContact& contact_ = *(m_mapRecentContatct.at(contact.m_strId));
			RecentContactSet::iterator it = recentContacts.find(*(it_->second));

			if (it != recentContacts.end())
			{
				//最经联系人存在
				//UpdateRecentList()

				contact.SetTag((UINT_PTR)((const_cast<RecentContact&>(*it)).GetTag()));
				UpdateRecentList(contact, -2);
				recentContacts.erase(it);
				pair<RecentContactSet::iterator, bool> ret = recentContacts.insert(contact);
				m_mapRecentContatct.at(contact.m_strId) = &(const_cast<RecentContact&>(*ret.first));
			}
		}
		
		else
		{// 不存在这个最近联系人
// 			recentContacts.insert(contact);
// 			StringTotString(tstrId, contact.data().m_strId);
// 			MAP::iterator itFind = m_FriendItemInfo.find(tstrId);
// 			if (itFind != m_FriendItemInfo.end())
// 			{
// 				FriendListItemInfo* pItem = itFind->second->Clone(); 
// 			}
			UpdateRecentList(contact, 0);
			recentContacts.insert(contact);
		}

	}
	
#if 0
		item->id = 1;
		item->folder = FALSE;
		item->logo = _T("defaultuserHead.png");
		item->nick_name = _T("告别青春");
		item->description = _T("青春没来得及挥霍，就已经悄悄流逝");
		m_pRecentContactsList->AddNode(item, NULL);

		item = new FriendListItemInfo;
		item->folder = FALSE;
		item->logo = _T("defaultuserHead.png");
		item->nick_name = _T("Zebreo");
		item->description = _T("春天去哪儿了呢？");
		m_pRecentContactsList->AddNode(item, NULL);

		item = new FriendListItemInfo;
		item->logo = _T("defaultuserHead.png");
		item->folder = FALSE;
		item->nick_name = _T("菊花不是花");
		item->description = _T("周末到了， 睡个痛快");
		m_pRecentContactsList->AddNode(item, NULL);
#endif

#endif
}

CDuiString CMainFrame::GetSkinFolder()
{
	return _T("");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	//自定义控件初始化
	if (IsStrEqual(pstrClass, _T("FriendList")))
	{
		//OutputDebugString(_T("FriendList"));
		return new CFriendsUI(m_PaintManager); 

	}
	if (IsStrEqual(pstrClass, _T("RecentList")))
	{
		return new CFriendsUI(m_PaintManager);
	}
	if (IsStrEqual(pstrClass, _T("SearchResultList")))
	{
		return new CFriendsUI(m_PaintManager);
	}
	return NULL;
}

//客户区鼠标事件捕获，用来处理tab标签切换
LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE; 
	POINT pt;
	::GetCursorPos(&pt);
	//pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam) -32;
	::ScreenToClient(*this, &pt);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	CControlUI* pControl = m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		CDuiString dstrName = pControl->GetParent()->GetName();

		//点击好友标签
		if ((IsStrEqual(dstrName ,const_friendlistlayout) ||
			IsStrEqual(dstrName, const_friendlogocontrol)) && m_tabState.select != FRIENDS )
		{
			COptionUI* pOptionUI = static_cast<COptionUI*>(m_PaintManager.FindControl(const_friendsoption));
			if (pOptionUI)
			{
				CControlUI* pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendarrowcontrol));
				if(pArrowControl != NULL)
					pArrowControl->SetVisible(TRUE);
				if (m_tabState.select== RECENTCONTACTS )
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentarrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}
				else if (m_tabState.select == APPLICATION)
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_apparrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}
				pOptionUI->GetManager()->SendNotify(pOptionUI, _T("selectchanged") , 0, 0, true);
				m_tabState.select  = FRIENDS;
			}
			bHandled = TRUE;
		}
		//点击最近联系人标签
		else if ((IsStrEqual(dstrName ,const_recentcontactlistlayout) ||IsStrEqual(dstrName, _T("recnentlogo"))) && m_tabState.select != RECENTCONTACTS)
		{
			COptionUI* pOptionUI = static_cast<COptionUI*>(m_PaintManager.FindControl(const_recentcontactoption));
			if (pOptionUI)
			{
					CControlUI* pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentarrowcontrol));
				if(pArrowControl != NULL)
					pArrowControl->SetVisible(TRUE);
				if (m_tabState.select== FRIENDS )
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendarrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}
				else if (m_tabState.select == APPLICATION)
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_apparrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}

				pOptionUI->GetManager()->SendNotify(pOptionUI, _T("selectchanged") , 0, 0, true);
				m_tabState.select = RECENTCONTACTS;
			}
			bHandled = TRUE;
		}
		
		else if ((IsStrEqual(dstrName, const_applistlayout)||IsStrEqual(dstrName, const_applogocontrol)) && m_tabState.select != APPLICATION)
		{
			COptionUI* pOptionUI = static_cast<COptionUI*>(m_PaintManager.FindControl(const_appoption));
			if (pOptionUI)
			{
				CControlUI* pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_apparrowcontrol));
				if(pArrowControl != NULL)
					pArrowControl->SetVisible(TRUE);
				if (m_tabState.select== FRIENDS )
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendarrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}
				else if (m_tabState.select == RECENTCONTACTS)
				{
					pArrowControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentarrowcontrol));
					if(pArrowControl != NULL)
						pArrowControl->SetVisible(FALSE);
				}
				pOptionUI->GetManager()->SendNotify(pOptionUI, _T("selectchanged") , 0, 0, true);
				m_tabState.select = APPLICATION;
			}
			bHandled = TRUE;
		}
	}

	return 0;
}

//此函数中主要处理tab标签选项在鼠标移动岛上面和移开的效果
LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt ;//= { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	//CControlUI* pHover = m_PaintManager.FindControl(pt);
	bHandled = FALSE; 
	//POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(*this, &pt);
	E_OPTION mouseover = NONE;
	CControlUI* pControl = m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		CDuiString dstrName = pControl->GetParent()->GetName();
		if(IsStrEqual(dstrName ,const_friendlistlayout) ||IsStrEqual(dstrName, const_friendlogocontrol))
		{
			mouseover = FRIENDS;
		}
		else if (IsStrEqual(dstrName, const_recentcontactlistlayout) || IsStrEqual(dstrName, const_recentlogocontrol))
		{
			mouseover = RECENTCONTACTS;
		}
		else if (IsStrEqual(dstrName,const_applistlayout) || IsStrEqual(dstrName, const_applogocontrol))
		{
			mouseover = APPLICATION;
		}
	}

	if (mouseover != NONE)
	{
		bHandled = TRUE;  //不用再交给上一级处理了
	}

	if (mouseover == m_tabState.mousevoer)
	{
		return 0;
	}
	else
	{
		switch (mouseover)
		{
		case NONE:
			{
				CControlUI* pBkControl = NULL;
				if (m_tabState.mousevoer == FRIENDS)
				{
					pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendlistlayout));
					if (pBkControl != NULL)
					{
						pBkControl->SetBkImage(_T("tabnormal.jpg"));
					}
				}
				else if (m_tabState.mousevoer == RECENTCONTACTS)
				{
					pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentcontactlistlayout));
					pBkControl->SetBkImage(_T("tabnormal.jpg"));
				}
				else if (m_tabState.mousevoer == APPLICATION)
				{
					pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_applistlayout));
					pBkControl->SetBkImage(_T("tabnormal.jpg"));
				}
				break;
			}
		case FRIENDS:
			{
				CControlUI* pBkControl = NULL;
				if (m_tabState.mousevoer == NONE)
				{

				}
				else if (m_tabState.mousevoer == RECENTCONTACTS)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentcontactlistlayout));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}
				else if (m_tabState.mousevoer == APPLICATION)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_applistlayout));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}

				pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendlistlayout));
				if (pBkControl != NULL)
					pBkControl->SetBkImage(_T("tabover.png"));
				break;
			}
		case RECENTCONTACTS:
			{
				CControlUI* pBkControl = NULL;
				if (m_tabState.mousevoer == NONE)
				{
// 					pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_appoption));
// 					if (pBkControl != NULL)
// 					{
// 						pBkControl->SetBkImage(_T("tabover.png"));
// 					}
				}
				else if (m_tabState.mousevoer == FRIENDS)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_friendlistlayout));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}
				else if (m_tabState.mousevoer == APPLICATION)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_applistlayout));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}

				pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentcontactlistlayout));
				if (pBkControl != NULL)
					pBkControl->SetBkImage(_T("tabover.png"));
				break;
			}
		case  APPLICATION:
			{
					CControlUI* pBkControl = NULL;
				if (m_tabState.mousevoer == NONE)
				{
// 					pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_appoption));
// 					if (pBkControl != NULL)
// 					{
// 						pBkControl->SetBkImage(_T("tabover.png"));
// 					}
				}
				else if (m_tabState.mousevoer == FRIENDS)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_appoption));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}
				else if (m_tabState.mousevoer == RECENTCONTACTS)
				{
					CControlUI* pOldBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_recentcontactlistlayout));
					if (pOldBkControl != NULL)
						pOldBkControl->SetBkImage(_T("tabnormal.jpg"));
				}

				pBkControl = static_cast<CControlUI*>(m_PaintManager.FindControl(const_applistlayout));
				if (pBkControl != NULL)
					pBkControl->SetBkImage(_T("tabover.png"));
				break;;
			}
		default: break;
		}
		m_tabState.mousevoer = mouseover;
	}
	//if( pControl == NULL ) return 0;
	return 0;
}

//给主窗口添加系统托盘图标
void CMainFrame::ToTray()
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDI_IMCLIENT;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE  |NIF_TIP ;
	m_nid.uCallbackMessage = WM_TRAYMSG;
	m_nid.hIcon  = m_hTrayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_IMCLIENT));
	_tcscpy(m_nid.szTip, _T("liuliu"));
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}

//删除托盘图标
void CMainFrame::DeleteTray()
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDI_IMCLIENT;
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

//托盘图标上接收到的消息处理哦
LRESULT CMainFrame::OnTrayMsg(WPARAM wParam , LPARAM lParam)
{
	if (wParam != IDI_IMCLIENT)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONDOWN:
		{
			OnTrayRButtonDown();
			break;
		}
	case  WM_LBUTTONDBLCLK:  //双击托盘图标左键
		{
			OnTrayDbButtonDown();
			break;
		}
	default:
		break;
	}
	return 0;
}

void CMainFrame::OnTrayRButtonDown()
{
	
//	ShowWindow(false);
	CButtonUI* pBtnUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(const_userstatusbtn));
	POINT point;
	::GetCursorPos(&point);
	//SetFocus(m_hWnd);
	//pBtnUI->SetFocus();
	
	
	CUIStatusMenu* pMenu = new CUIStatusMenu();
	if(pMenu == NULL)
		return ;
	pMenu->Init(pBtnUI, point, FALSE);
	SetForegroundWindow(pMenu->GetHWND());
	//ShowWindow(SW_RESTORE);
}

// 双击托盘图标事件响应函数
void CMainFrame::OnTrayDbButtonDown()
{
	
	if (!m_listActiveFriend.empty())
	{
		//m_notReadMessages.GetMesageCollectionMap().find(m_listActiveFriend.front());
		tString tstrId;
		StringTotString(tstrId, m_listActiveFriend.front());
		ShowChatWindow(tstrId, TRUE);
		m_nid.hIcon=m_hTrayIcon;
		Shell_NotifyIcon(NIM_MODIFY,&m_nid);
		m_bIsIcon = !m_bIsIcon;
		return ;
	}

	SetForegroundWindow(m_hWnd);

	if (!IsWindowVisible(m_hWnd))
	{
		ShowWindow(true);
	}
	//SetFocus(m_hWnd);
	//ShowWindow(true);
}

void CMainFrame::TrayFlash()
{
#if 0 //此种状况下系统托盘会一会儿有一会儿无
	if (m_bIsIcon)
	{
		m_nid.uFlags |= NIF_STATE;
 		m_nid.dwState = NIS_HIDDEN ;
 		m_nid.dwStateMask = NIS_HIDDEN /*| NIS_SHAREDICON*/;
		Shell_NotifyIcon(NIM_MODIFY, &m_nid);
 		m_bIsIcon = FALSE;
	}
	else
	{
		m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		m_nid.dwState = NIS_SHAREDICON ;
		m_nid.dwStateMask = NIS_HIDDEN /*| NIS_SHAREDICON*/;  //注意不是NIS_SHAREDICON
		m_nid.hWnd = this->m_hWnd;
		m_nid.uID = IDI_IMCLIENT;
		m_nid.uFlags = NIF_STATE;// NIF_ICON | NIF_MESSAGE  |NIF_TIP ;
		m_nid.uCallbackMessage = WM_TRAYMSG;
		Shell_NotifyIcon(NIM_MODIFY,&m_nid);
		m_bIsIcon = TRUE;
	}
#endif
	m_bIsIcon?m_nid.hIcon=NULL:m_nid.hIcon=m_hTrayIcon;
	Shell_NotifyIcon(NIM_MODIFY,&m_nid);
	m_bIsIcon = !m_bIsIcon;
}

LRESULT CMainFrame::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if (wParam  == 1)
	{
		TrayFlash();
	}

	return 0;
}

void CMainFrame::BeginFlash()
{
	SetTimer(m_hWnd, 1, 500, NULL);	
}

void CMainFrame::EndFlash()
{
	KillTimer(m_hWnd,1);
}

LRESULT CMainFrame::OnFriendGoup(WPARAM wParam, LPARAM lParam)
{
	tString tstrGroup = (LPCTSTR)lParam;
	FriendGroup* pFriendGroup = FriendList::GetInstance()->GetFriendList().at(tstrGroup);
	UpdateGroup(pFriendGroup);

	//存储到数据库
	
	CFriendUpdateThread* pFriendUpdateThread= new CFriendUpdateThread;
	pFriendUpdateThread->SetPara((UINT_PTR)pFriendGroup);
	pFriendUpdateThread->StartThread();
	return 0;
}

void CMainFrame::SetUserHeadPicture(LPCTSTR lptPicName)
{
	//m_tstrUserPic = it->second.c_str();
	//dest='2,2,62,62'
	TCHAR  tszPath[MAX_FILENAME_SIZE + 1] = {0x0};
	_stprintf_s(tszPath, _T("file='%s' dest='2,2,62,62'"),/*GetGrayPicName(lptPicName).c_str()*/lptPicName);
	m_pBtnUserHead->SetBkImage(tszPath);
}

LRESULT CMainFrame::OnSetExistingPicture(WPARAM wParam, LPARAM lParam)
{
	//向界面发送消息更换图片
	//SendMessage(pSrcPara->m_hWnd, pSrcPara->type,  pSrcPara);
	SrcPara* p = (SrcPara*)(((CSourceDownloadThread*)lParam)->GetPara());
	if ((E_MEMBER_TYPE)wParam == E_USERSELF)
	{
		// 设置用户自己头像
 		MAP_TSTRING_TSTRING::iterator it = p->m_picInfo.m_mapHasPic.begin();
 		SetUserHeadPicture(it->second.c_str());

	}
	else if((E_MEMBER_TYPE)wParam == E_NORMALMEMBER)
	{
// 		map<tString, BaseMember*>& memberMap = /*FriendList::GetInstance()->GetFriendList().at(p->m_tstrId)->GetFriendGroupInfo().GetMemberMap();*/ FriendList::GetInstance()->GetMemberMap(p->m_tstrId);
// 
// 		Node* pNode= NULL;
// 		for (MAP_TSTRING_TSTRING::iterator it = p->m_picInfo.m_mapHasPic.begin(); it != p->m_picInfo.m_mapHasPic.end(); ++it)
// 		{
// 			m_mapMemberPic.insert(make_pair(it->first, it->second));
// 			pNode = (Node*)(memberMap.at(it->first)->GetTag());
// 			CButtonUI* pLogoBtn = static_cast<CButtonUI*>(pNode->data().list_elment_->FindSubControl(_T("logo")));
// 			pLogoBtn->SetNormalImage(it->second.c_str());
// 		}
		SetFriendListItemPicture(p->m_tstrId.c_str(), p->m_picInfo.m_mapHasPic);
	}
	return 0;
}

void CMainFrame::SetFriendListItemPicture(LPCTSTR lptGroupId, MAP_TSTRING_TSTRING& mapIdToPic)
{
	map<tString, BaseMember*>& memberMap = /*FriendList::GetInstance()->GetFriendList().at(p->m_tstrId)->GetFriendGroupInfo().GetMemberMap();*/ FriendList::GetInstance()->GetMemberMap(lptGroupId);

	Node* pNode= NULL;
	for (MAP_TSTRING_TSTRING::iterator it = mapIdToPic.begin(); it != mapIdToPic.end(); ++it)
	{
		//m_mapMemberPic.insert(make_pair(it->first, it->second));
		m_FriendItemInfo.at(it->first)->logo = it->second;
		//BaseMember* pMember = memberMap.at(it->first);
		Node* pNode = (Node*)(memberMap.at(it->first)->GetTag());
		SetMemberLogo(pNode);
		SetMemberLogo(pNode->GetlinkNode());
	}
}

void CMainFrame::SetMemberLogo(Node* pNode/*, LPCTSTR lptPic*/)
{
	if (pNode)
	{
		CButtonUI* pLogoBtn = static_cast<CButtonUI*>(pNode->data().list_elment_->FindSubControl(_T("logo")));


		if(pNode->data().pItemData->status == FRIEND_OFFLINE)
		{
			pLogoBtn->SetNormalImage(GetGrayPicName(pNode->data().pItemData->logo.c_str()).c_str());
		}
		else
			pLogoBtn->SetNormalImage(pNode->data().pItemData->logo.c_str());
	}
}
// 
// tString CMainFrame::GetGrayPicName(LPCTSTR lptNormalName)
// {
// //	LPCTSTR lptFind = _tcsrchr(tstrNormalName.c_str(), _T('/')) +1;
// // 譬如图片名称为 D:\www\ww\afdfdfgdg.jpg, 那么对应灰度图为D:\www\ww
// 	//D:\imClient\ImClient\Debug\ImClient\IMFileRecv\wdemo1 w060\user\ddb8fa42c4f14e1688e1b80f120383ba.jpg
// 	//D:\imClient\ImClient\Debug\ImClient\IMFileRecv\wdemo1 w060\user\gray.ddb8fa42c4f14e1688e1b80f120383ba.jpg
// 	tString tstrRet = lptNormalName;
// 	LPCTSTR lptFind = _tcsrchr(lptNormalName, _T('\\'));
// 	if (lptFind == NULL)
// 		tstrRet.insert(0, _T("gray."));
// 	else
// 	{
// 		//tstrRet = lptNormalName;
// 		int x = lptFind+1 - lptNormalName;
// 		tstrRet.insert(lptFind+1 - lptNormalName, _T("gray."));
// 		return tstrRet;
// 	}
// 	return tstrRet;
// }

LRESULT CMainFrame::OnSetDownloadPicture(WPARAM wParam, LPARAM lParam)
{
	PicDwonloadPara* p  = (PicDwonloadPara*)(((PicDownloadThreadEx*)lParam)->GetPara());
	if (E_MEMBER_TYPE(wParam == E_USERSELF))
	{
		SetUserHeadPicture((p->m_mapDownInfo.begin()->second).c_str());
	}
	else
	{
		SetFriendListItemPicture(p->m_tstrGroupId.c_str(), p->m_mapDownInfo);
	}
	return 0;
}

LRESULT CMainFrame::OnSetUserInfo(WPARAM wParam, LPARAM lParam)
{
	tString tstrNickName;
	StringTotString(tstrNickName, g_user.UerSelf()->m_strNickName);
	m_pTxtNickName->SetText(tstrNickName.c_str());
	return 0;
}

LRESULT CMainFrame::OnOfflineMessages(WPARAM wParam, LPARAM lParam)
{
	// 登陆的是否收到离线消息
	COfflineMsgs & OfflineMsg = CGlobalData::GetInstance()->OfflinMessages();
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = OfflineMsg.m_normalmsgMap.begin(); it != OfflineMsg.m_normalmsgMap.end(); ++it)
	{
		m_notReadMessages.GetMesageCollectionMap().insert(*it);
		m_listActiveFriend.remove(it->first);
		m_listActiveFriend.push_front(it->first);
	}
	OfflineMsg.GetMesageCollectionMap().clear();
	BeginFlash();
	return 0; 
}

//当没有窗口的时候, 只有收到消息才能到达此处 
LRESULT CMainFrame::OnChatMessagesOfNoWnd(WPARAM wParam, LPARAM lParam)
{
	NormalMsg* pMsg = (NormalMsg*)lParam;
	m_notReadMessages.Insert(pMsg);
	// 此时FromId是好友ID
	m_listActiveFriend.remove(pMsg->m_strFromId);
	m_listActiveFriend.push_front(pMsg->m_strFromId);
	BeginFlash();
	return 0;
}

//bSureOfNowWnd TRUE 确定没有窗口， FALSE不确定有没有窗口
BOOL CMainFrame::ShowChatWindow(const tString&  tstrId, BOOL bSureOfNoWnd/* = FALSE*/)
{
	if (!bSureOfNoWnd)
	{ // 有窗口
		MAP_TSTRING_HWND::iterator itFindWnd = g_mapWnd.find(tstrId);
		if (itFindWnd != g_mapWnd.end())
		{
			if (IsIconic(itFindWnd->second))
			{
				//::SendMessage(itFind->second, WM_SYSCOMMAND, SC_RESTORE, (LPARAM)0);
				::ShowWindow(itFindWnd->second, SW_SHOWNORMAL);
			}
			SetFocus(itFindWnd->second);
			return FALSE;
		}
	}

	CUIChatDlg* pChatDialog = new CUIChatDlg(m_FriendItemInfo.at(tstrId));
	if( pChatDialog == NULL )
		return FALSE;
	pChatDialog->SetOwner(m_hWnd);
	HWND hwnd = pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME /*UI_WNDSTYLE_DIALOG*/| WS_POPUP,  NULL, 0, 0, 0, 0);
	 
	//pChatDialog->Create(NULL, _T("ChatDialog"),UI_WNDSTYLE_DIALOG,  NULL, 0, 0, 0, 0);							
	pChatDialog->CenterWindow();
	::ShowWindow(*pChatDialog, SW_SHOW);
	//::SetForegroundWindow(hwnd);
  //::ShowWindow(itFindWnd->second, SW_SHOWNORMAL);

	//m_notReadMessages 处理
	std::string strId;
	tStringToString(strId, tstrId);
	MAP_STRING_PNORMALMSGCOLLECTION::iterator itFindMsg = m_notReadMessages.GetMesageCollectionMap().find(strId);
	if (itFindMsg !=  m_notReadMessages.GetMesageCollectionMap().end())
	{
			// 现将消息显示在窗口中

		::PostMessage(hwnd, WM_REICEIVEMSG, (WPARAM)TRUE, (LPARAM)itFindMsg->second);
		m_listActiveFriend.remove(itFindMsg->first);
		m_notReadMessages.GetMesageCollectionMap().erase(itFindMsg);
		if (m_notReadMessages.GetMesageCollectionMap().size() == 0)
		{
			EndFlash();
		}
	}
	return TRUE;
}

// 设置当前好友的状态 设置好友的状态
LRESULT CMainFrame::OnSetMemberStatus(WPARAM wParam, LPARAM lParam)
{
 	int status = wParam;
 	tString tstrId = (LPCTSTR)lParam;
 	MAP::iterator itFind = m_FriendItemInfo.find(tstrId);
 	m_FriendItemInfo;
   	if (itFind != m_FriendItemInfo.end())
   	{
   		//存在
   		Node* pNode = (Node*)(itFind->second->GetTag());
  		//pNode->parent()->
		Node* prev = NULL;
 		if (pNode->data().pItemData->status != status)
 		{ // 转改发生改变
 			//pNode->data().pItemData->status = status;
			pair<Children::iterator, bool> ret; 

			if (pNode->GetlinkNode())
				pNode->GetlinkNode()->data().pItemData->status = status;

 			if (status != FRIEND_OFFLINE)
 			{  // 改变后状态不是离线状态
				if (pNode->data().pItemData->status == FRIEND_OFFLINE)
				{	
					// 之前状态值离线状态 
					// 从FRIEND_OFFLINE =》 非FRIEND_OFFLINE状态
					pNode->data().pItemData->status  = status;
					
					Children::iterator itMember  = pNode->parent()->get_children_().find(tstrId);
					ret = pNode->parent()->get_children().insert(*itMember);
					if (ret.second)
					{
						pNode->parent()->get_children_().erase(itMember);
						Children::iterator itNew = ret.first;
						if (pNode->parent()->get_children().begin() != itNew)
						{
							itNew--;
							prev = itNew->second;
						}
						else
							prev  = pNode->parent();
						int nInsertPos =  m_pFriendsList->GetItemIndex(prev->data().list_elment_) + 1; 
						m_pFriendsList->SetItemIndex(pNode->data().list_elment_, nInsertPos);
						//pNode->data().list_elment_->SetBkImage((pNode->data().pItemData->logo).c_str());
						SetMemberLogo(pNode);
						SetMemberLogo(pNode->GetlinkNode());
					}
				}
				else
				{
					//从Not FRIEND_OFFLINE =》 NOT FRIEND_OFFLINE状态， 
					//非离线状态到非离线状态 , 列表排序不发生变化

				}
 			}
 			else// if ( status == FRIEND_OFFLINE)
 			{ 
				//表示member不在线
				// member状态从在线改变到不在线的状态

				pNode->data().pItemData->status = status;

				Children::iterator itMember = pNode->parent()->get_children().find(tstrId);
				if (itMember != pNode->parent()->get_children().end())
				{
					ret = pNode->parent()->get_children_().insert(*itMember);
					if (ret.second)
					{
						pNode->parent()->get_children().erase(itMember);
						Children::iterator  itNew = ret.first;
						if (pNode->parent()->get_children_().begin() != itNew)
						{//不在集合中
							itNew -- ;
							prev = itNew->second;
						}
						else
						{ //
							if(pNode->parent()->get_children().empty())
								prev = pNode->parent();
							else
								prev = pNode->parent()->get_children().rend()->second;
						}
						int nInsertPos = m_pFriendsList->GetItemIndex(prev->data().list_elment_) + 1;
						m_pFriendsList->SetItemIndex(pNode->data().list_elment_, nInsertPos);
					} // end if
					
				} // end if
				SetMemberLogo(pNode->GetlinkNode());
				SetMemberLogo(pNode);
				return 0 ;
 			} //end else 
   		}
		else
			return 0;
 	}//end if 
 	
	return 0 ;
}

// 收到最近联系人， 客户端进行更新
LRESULT CMainFrame::OnRecentContacts(WPARAM wParam, LPARAM lParam)
{
	UpdateRecentList((NormalMsg*)lParam);
	return 0;
}

void CMainFrame::OnBtn_Serach(TNotifyUI& msg)
{
	if (m_pSearchView->IsVisible())
	{//如果时刻将的
		m_pSearchView->SetVisible(false);
	
	}
}

//好友搜素
void CMainFrame::Search(LPCTSTR lptStr )
{
  // TODO:
}

void  CMainFrame::SetSearchBtnState(BOOL bSeachState)
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("searchbtn")));
	if(!pBtn)
		return;
	if (!bSeachState) 
	{
		pBtn->SetNormalImage(_T("file='search_btn.png' dest='4,6,20,23'"));
		pBtn->SetEnabled(false);
		
	}
	else
	{
		pBtn->SetText(_T(""));
		pBtn->SetNormalImage(_T("file='serachclose_btn1.png' dest='4,6,20,23'"));
		pBtn->SetHotImage(_T("file='serachclose_btn2.png' dest='4,6,20,23'"));
		pBtn->SetPushedImage(_T("file='serachclose_btn3.png' dest='4,6,20,23'"));
		pBtn->SetEnabled(TRUE);
	}
}

void CMainFrame::OnTextChanged(TNotifyUI& msg)
{
	if (IsStrEqual(msg.pSender->GetName(), _T("searchedit")))
	{

	}
}

void CMainFrame::DisplaySearchView(BOOL bShow)
{
	m_pMainview->SetVisible(!bShow?true:false);
	m_pSearchView->SetVisible(bShow?true:false);
}
