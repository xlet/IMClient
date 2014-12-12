#include "stdafx.h"
#include "UIFriends.hpp"
#include "UIListCommonDefine.hpp"

#include "Global.h"

#define  TM_FRIENDINFODLG 2

#define TM_MONITOR        3  

namespace DuiLib
{

//const int kFriendListItemNormalHeight = 32;
#if CUSTOMSERV
	const int kFriendListItemNormalHeight = 37;
	const int kFriendListItemSelectedHeight = 37;
#else
	const int kFriendListItemNormalHeight = 50;
	const int kFriendListItemSelectedHeight = 50;
#endif




CFriendsUI::CFriendsUI(CPaintManagerUI& paint_manager)
	: root_node_(NULL)
	, delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, level_expand_image_(_T("<i list_icon_b.png>"))
	, level_collapse_image_(_T("<i list_icon_a.png>"))
	//, level_text_start_pos_(10)
	, level_text_start_pos_(5)
	, text_padding_(0, 0, 0, 0)
	, paint_manager_(paint_manager)
	, m_pFriendInfoDlg(NULL)
{
	SetItemShowHtml(true);
	SetItemFont(2);
	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}

CFriendsUI::~CFriendsUI()
{
	if (root_node_)
		delete root_node_;

	root_node_ = NULL;
}

bool CFriendsUI::OnListItemBtnEvent(void* p)
{
	TEventUI* pEvent = ((TEventUI*)p);
	if (pEvent->Type == UIEVENT_MOUSEHOVER)
	{
		OnItemBtnMouseOver(pEvent);
	}

	else if (pEvent->Type == UIEVENT_MOUSELEAVE)
	{
	 //	MessageBox(NULL, _T("LEAVE"), _T(""), MB_OK);
		OutputDebugString(_T("Leave HeadPic button area\n"));
		OnItemBtnMouseLeave(pEvent);
		pEvent->pSender->GetParent()->DoEvent(*pEvent);
	}
	else if (pEvent->Type == UIEVENT_MOUSEENTER)
	{
		//MessageBox(NULL, _T("Enter"), _T(""), MB_OK);
		OutputDebugString(_T("Enter HeadPic button area\n"));
		OnItemBtnMouseEnter(pEvent);
		pEvent->pSender->GetParent()->DoEvent(*pEvent);
	}
	else if (pEvent->Type == UIEVENT_SETCURSOR)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}
	else if (pEvent->Type == UIEVENT_TIMER)
	{
		//MessageBox(NULL, _T("LEAVE"), _T(""), MB_OKCANCEL);
		OnItemBtnTimer(pEvent);
	}
	else if(pEvent->Type == UIEVENT_DBLCLICK)
	{
		//CControlUI::DoEvent(*pEvent);
		pEvent->pSender->GetParent()->DoEvent(*pEvent);
		return false;
#if 0
 		 CListContainerElementUI* pList = (CListContainerElementUI*)(pEvent->pSender->GetTag());
 		 if( pList->IsEnabled() ) {
 			 pList->Activate();
 			 pList->Invalidate();
 		 }
		return  false;
#endif
	}

	else if (  pEvent->Type == UIEVENT_BUTTONDOWN || pEvent->Type == UIEVENT_RBUTTONDOWN )
	{
		//CControlUI::DoEvent(*pEvent);
		pEvent->pSender->GetParent()->DoEvent(*pEvent);
		return false;
	}
 	else
 		pEvent->pSender->DoEvent(*pEvent);

	/*
		注意 return false  表示不再交给上一级处理该事件通知到此结束
	*/
	return false;		 
}

void CFriendsUI::OnItemBtnTimer(TEventUI* pEvent)
{
	if (pEvent->wParam == TM_FRIENDINFODLG)
	{
		if (IsWindowVisible(m_pFriendInfoDlg->GetHWND()))
		{
			//m_pManager->SetTimer(pEvent->pSender, TM_FRIENDINFODLG, 300);
			//::ShowWindow(m_pFriendInfoDlg->GetHWND(), SW_HIDE);
			//
			/*m_pFriendInfoDlg->ShowWindow(false);*/
			m_pManager->KillTimer(pEvent->pSender, TM_FRIENDINFODLG);
			POINT curPos;
			GetCursorPos(&curPos);
			RECT rc;
			GetWindowRect(m_pFriendInfoDlg->GetHWND(), &rc);
			if (PtInRect(&rc, curPos))
			{
				// 鼠标在个人信息窗口内
					//m_pManager->KillTimer(pEvent->pSender, TM_FRIENDINFODLG);
					m_pManager->SetTimer(this, TM_MONITOR, 50);
			}
			else
			{
				::ShowWindow(m_pFriendInfoDlg->GetHWND(), SW_HIDE);
			}
		}
	}
}

void CFriendsUI::OnItemBtnMouseEnter(TEventUI* pEvent)
{
	CListContainerElementUI* pListItem = (CListContainerElementUI*)(pEvent->pSender->GetTag());
	Node* pNode = (Node*)pListItem->GetTag();

	BOOL bNeedShow = TRUE;
	if (!m_pFriendInfoDlg)
	{
		m_pFriendInfoDlg = new CUIFriendInfoDlg();
		m_pFriendInfoDlg->Create(NULL,_T("好友信息"), WS_POPUPWINDOW /*| WS_CAPTION */| WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_EX_APPWINDOW  , WS_EX_TOOLWINDOW );
	}

	if (IsWindowVisible(m_pFriendInfoDlg->GetHWND()))
	{
		m_pManager->KillTimer(m_pFriendInfoDlg->GetOwner(), TM_FRIENDINFODLG);
		if (m_pFriendInfoDlg->GetItemInfo() == pNode->data().pItemData)
			bNeedShow =FALSE;
		else
			m_pManager->KillTimer(m_pFriendInfoDlg->GetOwner(), TM_FRIENDINFODLG);
		bNeedShow = TRUE;

	}

	if (bNeedShow)
	{
		RECT rc;
		GetClientRect(m_pFriendInfoDlg->GetHWND(), &rc);
		RECT rcBtn =  pEvent->pSender->GetPos();
		POINT pos  = {rcBtn.left, rcBtn.top};
		ClientToScreen(m_pManager->GetPaintWindow(), &pos);
		pos.x -= rc.right - rc.left + 15;

		m_pFriendInfoDlg->SetOwner(pEvent->pSender);
		m_pFriendInfoDlg->SetItemInfo(pNode->data().pItemData);

		::SetWindowPos(m_pFriendInfoDlg->GetHWND(), NULL, pos.x,  pos.y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		::ShowWindow(m_pFriendInfoDlg->GetHWND(), SW_SHOW);
	}
}

void CFriendsUI::OnItemBtnMouseOver(TEventUI* pEvent)
{
	
}

void CFriendsUI::OnItemBtnMouseLeave(TEventUI* pEvent)
{
	if( IsEnabled() ) {
		m_uButtonState &= ~UISTATE_HOT;
		Invalidate();
	}
	if (IsWindowVisible(m_pFriendInfoDlg->GetHWND()))
	{
		m_pManager->SetTimer(pEvent->pSender, TM_FRIENDINFODLG, 300);
		//::ShowWindow(m_pFriendInfoDlg->GetHWND(), SW_HIDE);
		//m_pFriendInfoDlg->ShowWindow(false);
	}
}

/*
	功能：列表中添加一行
	参数：pControl行指针
	返回：
		True:操作成功
		FALSE:操作失败
*/
bool CFriendsUI::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CFriendsUI::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CFriendsUI::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::Remove(pControl);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

bool CFriendsUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::RemoveAt(iIndex);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

void CFriendsUI::RemoveAll()
{
	CListUI::RemoveAll();
// 	for (int i = 0; i < root_node_->num_children(); ++i)
// 	{
// 		Node* child = root_node_->child(i);
// 		RemoveNode(child);
// 	}
	Node* child = NULL;
	for (Children::iterator it = root_node_->get_children().begin(); it != root_node_->get_children().end(); ++it)
	{
		child = it->second;
		RemoveNode(child);
	}
	for (Children::iterator it = root_node_->get_children_().begin(); it != root_node_->get_children_().end(); ++it)
	{
		child = it->second;
		RemoveNode(child);
	}

	delete root_node_;

	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}

void CFriendsUI::DoEvent(TEventUI& event) 
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
			m_pParent->DoEvent(event);
		else
			CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_TIMER)
	{
		if (event.wParam == SCROLL_TIMERID)
		{
			if (delay_left_ > 0)
			{
				--delay_left_;
				SIZE sz = GetScrollPos();
				LONG lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
				if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
				{
					sz.cy -= lDeltaY;
					SetScrollPos(sz);
					return;
				}
			}
			delay_deltaY_ = 0;
			delay_number_ = 0;
			delay_left_ = 0;
			m_pManager->KillTimer(this, SCROLL_TIMERID);
		}
		else if (event.wParam == TM_MONITOR)
		{
			POINT curPos;
			RECT rc;
			GetCursorPos(&curPos);
			GetWindowRect(m_pFriendInfoDlg->GetHWND(), &rc);
			if (!PtInRect(&rc, curPos))
			{
				::ShowWindow(m_pFriendInfoDlg->GetHWND(), SW_HIDE);
				m_pManager->KillTimer(this, TM_MONITOR);
				
			}
		}
		return;
	}   
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 8;
			else
				delay_deltaY_ = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 8;
			else
				delay_deltaY_ = lDeltaY - 12;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 20U);
		return;
	}

	CListUI::DoEvent(event);
}

Node* CFriendsUI::GetRoot()
{
	return root_node_;
}

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kLogoContainerControlName = _T("logo_container");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");
const TCHAR* const kOperatorPannelControlName = _T("operation");

static bool OnLogoButtonEvent(void* event) {
    if( ((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN ) {
        CControlUI* pButton = ((TEventUI*)event)->pSender;
        if( pButton != NULL ) {
            CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
            if( pListElement != NULL ) pListElement->DoEvent(*(TEventUI*)event);
        }
    }
    return true;
}

Node* CFriendsUI::AddNode( FriendListItemInfo* item, Node* parent, int nIndex/*=-1*/)
{
	item->id;
	int nItemHeight= kFriendListItemNormalHeight;
	if (!parent)
		parent = root_node_;

	TCHAR szBuf[/*MAX_PATH*/1024] = {0};

    CListContainerElementUI* pListElement = NULL;

	//判断xml文件是否已经初始化
    if( !m_dlgBuilder.GetMarkup()->IsValid() )
	{
#if CUSTOMSERV
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("qiye_friend_list_item.xml"), (UINT)0, NULL, &paint_manager_));
#else 
		 pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("friend_list_item.xml"), (UINT)0, NULL, &paint_manager_));
#endif 
      
    }
    else {//已经加载了xml文件
        pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
    }
    if (pListElement == NULL)
        return NULL;

	Node* node = new Node;  //每个节点代表一个item

	//level表示节点级别， 在父节点基础上加1
	node->data().level_ = parent->data().level_ + 1;
	if (item->folder)  //折叠或者展开
		node->data().has_child_ = !item->empty;
	else
		node->data().has_child_ = false;

	node->data().pItemData = item;
	node->data().folder_ = item->folder;

	node->data().child_visible_ = (node->data().level_ == 0);
	node->data().child_visible_ = false;


	node->data().list_elment_ = pListElement;

	if (!parent->data().child_visible_)
		pListElement->SetVisible(false);

	//该节点有父节点并且父节点下子节点是可见的
	//那么该节点添加后也是可见的 反之亦然
	if (parent != root_node_ && !parent->data().list_elment_->IsVisible())
		pListElement->SetVisible(false);

	//Item 对应的偏移
	CDuiRect rcPadding = text_padding_; 
	//for (int i = 0; i < node->data().level_; ++i)
	//{
	//	rcPadding.left += level_text_start_pos_;		
	//}
	pListElement->SetPadding(rcPadding); //设置padding

	//logo按钮 
	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));

	//左侧logo图片
	if (log_button != NULL)
	{

		if (!item->folder && !item->logo.empty())
		{
		//节点是展开的并且logo不为空
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("%s"), item.logo.c_str());
#else
			_stprintf_s(szBuf, 1024 - 1, _T("%s"), item->logo.c_str());
#endif
			// 图片的设置
			if (item->status == FRIEND_OFFLINE)
				log_button->SetNormalImage(GetGrayPicName(szBuf).c_str());
			else
				log_button->SetNormalImage(szBuf);  //设置logo图片
	
			//log_button->SetNormalImage(szBuf);  //设置logo图片
		}
		else
		{
			//当前节点是分组 不显示头像 将其隐藏掉
			CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
			if (logo_container != NULL)
				logo_container->SetVisible(false);  //整个容器设为隐藏的
		}
        log_button->SetTag((UINT_PTR)pListElement);
       // log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent); //
		log_button->OnEvent += MakeDelegate(this, &CFriendsUI::OnListItemBtnEvent);
	}

	/*CDuiString*/ tString html_text;
	//当前节点不是好友而是分组
	//折叠图标
	if (node->data().has_child_)
	{
		nItemHeight = kFriendListItemNormalHeight/2;  //为分组Item
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

		_stprintf_s(szBuf, 1024 - 1, _T("<x %d>"), level_text_start_pos_);
		html_text += szBuf;
	}

	//节点是折叠
	if (item->folder)
	{
		html_text += (node->data().pItemData)->nick_name;
	}
	else
	{
		tString tStr = item->nick_name;
		if (item->nick_name.empty())
		{
			tStr = item->id;
		}

		_stprintf_s(szBuf, 1024 - 1, _T("%s"), tStr.c_str());

		html_text += szBuf;
	}

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
	{
		if (item->folder)
			nick_name->SetFixedWidth(0);

		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text.c_str());
	}

	if (!item->folder && !item->description.empty())
	{
		CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kDescriptionControlName));
		if (description != NULL)
		{
			description->SetToolTip(item->description.c_str());
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("<x 20><c #808080>%s</c>"), item.description);
#else
			_stprintf_s(szBuf, 1024 - 1, _T("<x 0><c #808080>%s</c>"), item->description.c_str());
#endif
			description->SetShowHtml(true);
			description->SetText(szBuf);
		}
	}

	pListElement->SetFixedHeight(/*kFriendListItemNormalHeight*/nItemHeight);
	pListElement->SetTag((UINT_PTR)node);  //存储当前Node节点指针


	int index = 0;
	if (nIndex == -1)
	{
		if (parent->has_children())
		{
			//找到添加当前节点的前一个节点
			Node* prev = parent->get_last_child();
			index = prev->data().list_elment_->GetIndex() + 1;
		}
		else 
		{
			//父节点如果是最高层根节点
			if (parent == root_node_)
				index = 0;
			else
				index = parent->data().list_elment_->GetIndex() + 1;
		}
	}
	else
		index = nIndex;
	
	
	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);  //在父节点中保存子节点
	return node;
}

bool CFriendsUI::RemoveNode(Node* node)
{
	if (!node || node == root_node_) return false;

// 	for (int i = 0; i < node->num_children(); ++i)
// 	{
// 		Node* child = node->child(i);
// 		RemoveNode(child);
// 	}
// 
// 	CListUI::Remove(node->data().list_elment_);
// 	node->parent()->remove_child(node);
// 	delete node;
	//移除该节点的子节点
	for (Children::iterator it=node->get_children().begin();  it != node->get_children().end(); ++it)
	{
		RemoveNode(it->second);
	}
	for (Children::iterator it = node->get_children_().begin(); it != node->get_children_().end(); ++it)
	{
		RemoveNode(it->second);
	}
	CListUI::Remove(node->data().list_elment_);

	//从父节点集合中移除
	node->parent()->remove_child(node);
	delete node;
	return true;
}

void CFriendsUI::SetChildVisible(Node* node, bool visible)
{
	if (!node || node == root_node_)
		return;

	if (node->data().child_visible_ == visible)
		return;

	node->data().child_visible_ = visible;

	TCHAR szBuf[MAX_PATH] = {0};
	/*CDuiString*/tString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("<x %d>"), level_text_start_pos_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
#endif
		html_text += szBuf;

		html_text += node->data().pItemData->nick_name;

		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kNickNameControlName));
		if (nick_name != NULL)
		{
			nick_name->SetShowHtml(true);
			nick_name->SetText(html_text.c_str());
		}
	}


	if (!node->data().list_elment_->IsVisible())
		return;

	if (!node->has_children())
		return;

// 	Node* begin = node->child(0);
// 	Node* end = node->get_last_child();
// 	for (int i = begin->data().list_elment_->GetIndex(); i <= end->data().list_elment_->GetIndex(); ++i)
// 	{
//		CControlUI* control = GetItemAt(i);
// 		if (_tcsicmp(control->GetClass(), _T("ListContainerElementUI")) == 0)
// 		{
// 			if (visible) 
// 			{
// 				Node* local_parent = ((Node*)control->GetTag())->parent();
// 				if (local_parent->data().child_visible_ && local_parent->data().list_elment_->IsVisible())
// 				{
// 					control->SetVisible(true);
// 				}
// 			}
// 			else
// 			{
// 				control->SetVisible(false);
// 			}
// 		}
// 	}
	SetChildVisible(node->get_children(), visible);
	SetChildVisible(node->get_children_(), visible);
#if 1
	
#endif 

}

void CFriendsUI::SetChildVisible(Children& children, bool visible)
{
	for (Children::iterator it = children.begin(); it != children.end(); ++it)
	{
		 CListContainerElementUI* pItem = ((it->second)->data()).list_elment_;
		 if (visible)
		 {
			 Node* local_parent = ((Node*)pItem->GetTag())->parent();
			 if (local_parent->data().child_visible_ && local_parent->data().list_elment_->IsVisible())
				 {
					 pItem->SetVisible(true);
				 }
		 }
		 else
		 {
			 pItem->SetVisible(false);
		 }
	}
}

bool CFriendsUI::CanExpand(Node* node) const
{
	if (!node || node == root_node_)
		return false;

	return node->data().has_child_;
}

// xuan
bool CFriendsUI::SelectItem(int iIndex, bool bTakeFocus)
{
    if( iIndex == m_iCurSel ) return true;

    // We should first unselect the currently selected item
    if( m_iCurSel >= 0 ) {
        CControlUI* pControl = GetItemAt(m_iCurSel);
        if( pControl != NULL) {
            IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
            if( pListItem != NULL )
			{
				CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
				Node* node = (Node*)pControl->GetTag();
				if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
				{
					
					pFriendListItem->SetFixedHeight(kFriendListItemNormalHeight);
// 					if (!node->folder())
// 					{
// 						MessageBox(NULL, _T("FFF"), _T(""), MB_OK);
// 					}
					
					
// 					CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
// 					if (pOperatorPannel != NULL)
// 					{
// 						pOperatorPannel->SetVisible(false);
// 					}
				}
				pListItem->Select(false);
			}
        }

        m_iCurSel = -1;
    }

    if( iIndex < 0 )
		return false;

	if (!__super::SelectItem(iIndex, bTakeFocus))
		return false;


	CControlUI* pControl = GetItemAt(m_iCurSel);
	if( pControl != NULL) {
		CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
		Node* node = (Node*)pControl->GetTag();
		if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
		{
			pFriendListItem->SetFixedHeight(kFriendListItemSelectedHeight);
			CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
			if (pOperatorPannel != NULL)
			{
			//	pOperatorPannel->SetVisible(true);
			}
		}
	}
	return true;
}

} // namespace DuiLib