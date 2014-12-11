#ifndef	 MAINFRAME_HPP_
#define  MAINFRAME_HPP_ 
#include "UIFriends.hpp"
#include "UISysSetDlg.h"
#include "IMSocket.h"
#include "LogoutRequest.h"
#include "shellapi.h"
#include "FriendGroup.h"
#include  <hash_map>
#include  <list>
//#include "UIListCommonDefine.hpp"
#include "UIFriends.hpp"
#include "FriendUpdateThread.h"
class CMainFrame: public WindowImplBase
{
public:	
	//对应三个标签选项
	enum E_OPTION
	{
		NONE =0,
		FRIENDS,
		RECENTCONTACTS,
		APPLICATION
	};
	struct TabState
	{
		E_OPTION select ;
		E_OPTION mousevoer;
	};
	CMainFrame::CMainFrame(void);

	CMainFrame::~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFile();
	void UpdateGroup(FriendGroup* pGroup);
	void UpdateFriendsList();  //更新好友列表
	void UpdateRecentList(NormalMsg* pMsg = NULL); //更新最近联系人列表
	virtual CDuiString GetSkinFolder();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND  Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle,CIMSocket *sock);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void       InitWindow();
	LRESULT OnTrayMsg(WPARAM wParam , LPARAM lParam);
	LRESULT  OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT  OnFriendGoup(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetExistingPicture(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetDownloadPicture(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetUserInfo(WPARAM wParam, LPARAM lParam);
	LRESULT  OnOfflineMessages(WPARAM wParam, LPARAM lParam);
	LRESULT  OnChatMessagesOfNoWnd(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetMemberStatus(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetUserStatus(WPARAM wParam, LPARAM lParam);
	LRESULT  OnRecentContacts(WPARAM wParam, LPARAM lParam);

	// Use zip Resource 
  	UILIB_RESOURCETYPE GetResourceType() const
  	{
  		return UILIB_ZIP;
  	}
  
  	virtual CDuiString GetZIPFileName() const
  	{
  		return _T("skin.zip");
  	}
protected:
	void Close();
	void Notify(TNotifyUI& msg);
	void OnTab_SelChanged(TNotifyUI& msg);
	void OnWindowInit(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void HideSerchEdit(CControlUI* pEdit);
	void OnBtn_SearchTip(TNotifyUI& msg);
	void OnBtn_SysSet(TNotifyUI& msg);
	void OnBtn_Signature(TNotifyUI& msg);
	void OnBtn_UserStatus(TNotifyUI& msg);
	void OnKillFocus_SerchEdit(TNotifyUI& msg);
	void OnKillFocus_SignatureEdit(TNotifyUI& msg);
	void OnDbItemClick(TNotifyUI& msg);
	void OnLItemClick(TNotifyUI& msg);
	void OnRItemClick(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);
	void OnMenu_Online(TNotifyUI& msg);
	void OnMenu_Leave(TNotifyUI& msg);
	void OnMenu_Busy(TNotifyUI& msg);
	void OnMenu_UnDisturb(TNotifyUI& msg);
	void OnMenu_Stealth(TNotifyUI& msg);
	void OnMenu_Offline(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg){}
	void OnExit(TNotifyUI& msg){Close();}
	void OnBtn_Close(TNotifyUI& msg);
	void OnBtn_Minimize(TNotifyUI& msg);
	void OnBtn_Serach(TNotifyUI& msg);
	void OnTextChanged(TNotifyUI& msg);
	void ToTray();
	void DeleteTray();
	void SetMemberLogo(Node* pNode/*, LPCTSTR lptPic*/);
	//控制托盘光标闪烁
	HICON   m_hTrayIcon;
	BOOL	m_bIsIcon;     
	void    TrayFlash();
	void    BeginFlash();
	void    EndFlash();
	void    OnTrayDbButtonDown();
	void    OnTrayRButtonDown();
	//////////////////////////////
	//tString GetGrayPicName(LPCTSTR lptNormalName);
	void SetUserHeadPicture(LPCTSTR lptPicName);
	void  SetFriendListItemPicture(LPCTSTR lptGroupId, map<tString, tString>& mapIdToPic);
	BOOL  ShowChatWindow(const tString&  tstrId, BOOL bSureOfNoWnd = FALSE);
	void UpdateRecentList(RecentContact& recentContact, int nIndex = -1);
	void UpdateRecentList(FriendListItemInfo* pItem);
	void Search(LPCTSTR lptStr );
	void SetSearchBtnState(BOOL bSeachState);
	void  DisplaySearchView(BOOL bShow);
protected:
	typedef hash_map<tString, FriendListItemInfo*>    MAP;
	//std::list<FriendListItemInfo> m_vcFriendInfo;  //keshan 
	/*std::hash_map<tString, FriendListItemInfo*>*/MAP m_FriendItemInfo; // 对应的每个人的Item信息
	FriendListItemInfo myself_info_;
	// 用户头像 <ID, 图片路径>
//	std::map<tString, tString> m_mapMemberPic;
	TabState				 m_tabState;    //通过鼠标事件的处理模拟tab控件的实现
	CUISysSetDlg*			 m_pUISyssetDlg;
	CIMSocket*				 m_pIMSock;     //
	NOTIFYICONDATA			 m_nid;        
	tString					 m_tstrUserPic;   //用户头像路径
	CButtonUI*				 m_pBtnUserHead ;
	CTextUI*				 m_pTxtNickName;
	NormaMsgCollectionBase   m_notReadMessages; //未读的消息集合
	list<std::string>        m_listActiveFriend; // 当前未读消息的好友	
	std::map<tString, Node*> m_mapGroupNode;   // 每个好友组节点
	CFriendsUI*				 m_pFriendsList ;    // 好友列表
	CFriendsUI*				 m_pRecentContactsList;  // 
	int					     m_nGroupId ;           // 自定义的GroupId， 按照	接收顺序		
	int						 m_nCurSel;          // 当前tab 标签对应的选项
	std::map<string , RecentContact*>	 m_mapRecentContatct;
	CEditUI*				 m_pSerchEdit;
	CVerticalLayoutUI*		 m_pMainview;
	CVerticalLayoutUI*		 m_pSearchView;

	//CFriendUpdateThread* m_pFriendUpdateThread;
	////////////////////////////////////////////
	// 存储好友非离线状况= 
	
};

#endif