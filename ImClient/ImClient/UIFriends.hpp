#ifndef UIFRIENDS_HPP
#define UIFRIENDS_HPP

#include "UIListCommonDefine.hpp"

#include "UIFriendInfoDlg.h"

namespace DuiLib
{
	//class Node;


class CFriendsUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CFriendsUI(CPaintManagerUI& paint_manager);

	~CFriendsUI();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	Node* GetRoot();
	//默认nIndex为-1时在列表最后添加， 
	Node* AddNode(/*const*/ FriendListItemInfo* item, Node* parent = NULL, int nIndex=-1);

	bool RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);
	void SetChildVisible(Children& children, bool visible);

	bool CanExpand(Node* node) const;

	bool SelectItem(int iIndex, bool bTakeFocus = false);

	bool OnListItemBtnEvent(void* pEvent);

	void OnItemBtnMouseOver(TEventUI* pEvent);
	void OnItemBtnMouseLeave(TEventUI* pEvent);
	void OnItemBtnMouseEnter(TEventUI* pEvent);
	void OnItemBtnTimer(TEventUI* pEvent);
private:
	//鼠标移动到头像图标显示好友的信息
	CUIFriendInfoDlg*   m_pFriendInfoDlg;
	Node*	root_node_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CDuiRect	text_padding_;
	int level_text_start_pos_;
	CDuiString level_expand_image_;
	CDuiString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

    CDialogBuilder m_dlgBuilder;
	//tString        m_tstr
};

} // DuiLib

#endif // UIFRIENDS_HPP