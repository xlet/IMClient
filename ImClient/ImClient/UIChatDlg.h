#pragma once
#include "UIFriends.hpp"
#include "UIFaceSelDlg.h"
#include "FontInfo.h"
#include "RichEditUtil.h"
#include "IMSocket.h"
#include "UIListCommonDefine.hpp"
enum E_PICSUFFIX
{
	E_PIC_GIF = 0,
	E_PIC_PNG,
	E_PIC_BMP,
	E_PIC_JPG,
	E_PIC_JPEG,
	E_PIC_EXIF,
	E_PIC_TIFF
};

enum E_FILEFORMAT
{
	E_PICTURE = 0,
	E_NORMALFILE
};

class InputViewEvent : public RichEditEvent
{
public:
	InputViewEvent(CRichEditUI *pReUI,const tString& bgimage, DWORD bkcolor);
	~InputViewEvent();
	//DragDrop
	virtual bool OnDragDropOver(const tString& pFilePath );
	virtual bool OnDropDown(const  tString& pFilePath,int nPos );
	virtual bool OnDropLeave(const tString& pFilePath );	
	/*---------------------------------------------------------------------*\
			decription : 鼠标或键盘点击ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
					bRight	—— true表示鼠标右键，false表示鼠标左键
	\*---------------------------------------------------------------------*/
	virtual void OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight );
	/*---------------------------------------------------------------------*\
			decription : 删除ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
	\*---------------------------------------------------------------------*/
	virtual void OnDeleteOleObject(void* pObj,DWORD dwType);

	virtual void OnUrl(void* pObj,DWORD dwType,long lPos);
	 void GetFileType(const tString& strFilename, E_FILEFORMAT* format, int* suffix );
private:
	CRichEditUI *m_pReUI;
	tString bgimage_;
	DWORD	bkcolor_;
};

#if 0
class ShowViewEvent : public RichEditEvent
{
public:
	ShowViewEvent(CRichEditUI *pReUI,const tString& bgimage, DWORD bkcolor);
	~ShowViewEvent();
	//DragDrop
	virtual bool OnDragDropOver( const DuiLib::string& pFilePath );
	virtual bool OnDropDown( const DuiLib::string& pFilePath,int nPos );
	virtual bool OnDropLeave( const DuiLib::string& pFilePath );	
	/*---------------------------------------------------------------------*\
			decription : 鼠标或键盘点击ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
					bRight	—— true表示鼠标右键，false表示鼠标左键
	\*---------------------------------------------------------------------*/
	virtual void OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight );
	/*---------------------------------------------------------------------*\
			decription : 删除ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
	\*---------------------------------------------------------------------*/
	virtual void OnDeleteOleObject(void* pObj,DWORD dwType);

	virtual void OnUrl(void* pObj,DWORD dwType,long lPos);
private:
	CRichEditUI *m_pReUI;
	tString bgimage_;
	DWORD	bkcolor_;
};

#endif

struct  MsgHeaderFontInfo
{
	COLORREF nicknameClr; //用户昵称颜色
	COLORREF idClr;   //用户id颜色
	COLORREF timeClr ; // 时间颜色
	BOOL	 bHasUnderline;   //Id是否有下划线
	BOOL     bIsLinked ;		//点击ID是否有超链接
	int      fontSize;        //字体的大小
	void SetMsgHeaderFontInfo(const MsgHeaderFontInfo & info)
	{
		nicknameClr = info.nicknameClr;
		idClr = info.idClr;
		timeClr = info.timeClr;
		bHasUnderline = info.bHasUnderline;
		bIsLinked = info.bIsLinked;
		fontSize = info.fontSize;
	}

} ;

typedef MsgHeaderFontInfo*  PMSGHEADERFONTINFO;

enum E_MSG_TYPE 
{
	MSG_NULL = -1,
	MSG_IN ,   //接收消息
	MSG_OUT      //发送消息
};

class CUIChatDlg:public WindowImplBase
{
public:
	
	//CUIChatDlg(const FriendListItemInfo& friend_info);
	CUIChatDlg(FriendListItemInfo* friend_info);
	~CUIChatDlg(void);
	virtual LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void Notify( TNotifyUI& msg );
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//相当于MFC的OninitDialg函数，窗口的初始化
	virtual void  InitWindow();
	//void memberFxn( NMHDR * pNotifyStruct, LRESULT * result );
	static void OnNotifyLinkCallback(void*, CDuiString, LONG cpMin, LONG cpMax);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void    OnReceiveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT   OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void SetOwner(HWND hwnd);
	HWND GetOwner();
protected:
	enum E_CHECKMSGRECORD
	{
		E_TORIGHT = 0,  // 向右边翻页
		E_TOFIRSTPAGE , // 第一页 最新消息记录
		E_TOLEFT,       // 向右边翻
		E_TOLASTPAGE    // 最后一页， 最早的消息记录
	};
	CUIChatDlg(void);
	void Init();
	void Msg2Json(Json::Value & val,const TCHAR*  lptsz);
	void OnPrepare(TNotifyUI& msg);
	void SendMsg();
	void HandleSendString(string& str);
	virtual void RecvMsg(NormalMsg* pNormMsg);
	//CDuiString GetCurrentTimeString();
	void OnFinalMessage(HWND hWnd);
	void OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnBtn_Emotion(TNotifyUI& msg);
	void OnBtn_FontBar(TNotifyUI& msg);
	void OnBtn_ViewSwitch(TNotifyUI& msg);
	void OnBtn_MsgRecord(TNotifyUI& msg);
	void OnBtn_SendMsg(TNotifyUI& msg);
	virtual void OnBtn_Close(TNotifyUI& msg);
	void OnBtn_Bold(TNotifyUI& msg);
	void OnBtn_UnderLine(TNotifyUI& msg);
	void OnBtn_Italic(TNotifyUI& msg);
	void OnBtn_SendImage(TNotifyUI& msg);
	void OnCmb_SelChange_FontSize(TNotifyUI& msg);
	void OnCmb_Selchange_FontName(TNotifyUI& msg);
	void OnBtn_Color(TNotifyUI& msg);
	void _RichEdit_ReplaceSel(ITextServices * pTextServices, CRichEditUI * pRichEdit, LPCTSTR lpszNewText, const msgfont* pMsgFont = NULL);
	void _RichEdit_GetText(ITextServices * pTextServices, tString& strText);	
	void ShowMsgInRecvEdit(const NormalMsg* pMsg, E_MSG_TYPE type);
	void ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type,  const NormalMsg* pMsg);
	void ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type,  const msginfo&   msg);
	void ShowMsgHeader(ITextServices* pTextServices, E_MSG_TYPE type, LPCTSTR lptTime);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  const NormalMsg* pMsg);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  LPCTSTR lptszText, const msgfont* pMsgFont = NULL);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  const msginfo& msg);
	BOOL HandleSysFaceId(ITextServices *pTextServices, CRichEditUI * pRichEdit, LPCTSTR& p, tString& strText, const msgfont* pMsgFont = NULL);
	void SetMsgHeaderFontInfo(const MsgHeaderFontInfo & info);
	//点击向左、向右、最后一页、第一页按钮显示消息记录
	/*virtual*/ void ShowMsgRecord(E_CHECKMSGRECORD type);  
	// 根据日期搜索显示消息记录
	void ShowMsgRecord(const char* date);
	void ShowOneMsgRecord(ITextServices* pTextServices, const msginfo& info);
	void SetWindowIcon();
	void StatrtFlashwindow();
	void StopFlashWindow();
	//初始化消息记录下方的有一些按钮
	void InitMsgRecordController();   
	
	void SetEachPageRecordNum(int num);
	int  GetEachPageRecordNum();
//	void OnBtn_CheckMsgRecord(TNotifyUI& msg);
	void TurnMsgRecordPage(E_CHECKMSGRECORD type); 
	void SetTurnPageBtnState(bool bToFirstPage, bool bToLastPage);
	LRESULT OnDateChange(LPNMDATETIMECHANGE lpCHange);
protected:
	HWND               m_hMainFrame;
	FLASHWINFO          m_flashwInfo;
	//BOOL                m_bFocus;    // 窗口是否获得了焦点
	//BOOL                m_bHasUnreadMsgAndFlash; // 有未读消息
	InputViewEvent*		m_pInputView;
	//ShowViewEvent*	m_pShowView;

	//tString bgimage_;
	//DWORD bkcolor_;
	//tString				m_tsrFriendId;  //当前聊天的好友的ID 
	CFontInfo			 m_fontInfo;  //当前聊天窗口的字体信息类
	CRichEditUI*		m_pSendEdit, * m_pRecvEdit, *m_pMsgRecordView;
	CComboUI*			m_pFontNameCmb, *m_pFontSizeCmb;
	DWORD			    m_dwLinkTextRange; //用户有超链接的IM号
	FriendListItemInfo*  m_pFriendInfo;
	//FriendListItemInfo	m_friendInfo; //好友信息5
	BOOL				m_bShowMsgView;		//消息记录栏是否隐藏，状态
	BOOL                m_bHideMsgView;		//隐藏的是否是消息记录栏，动作
	int					m_nExtendedWidth;   //窗口扩展额外增加的宽度
	SIZE				m_MinInfo;
	MsgHeaderFontInfo   m_msginFontinfo;

	MsgHeaderFontInfo   m_msgoutFontinfo;

	CUIFaceSelDlg		m_FaceDlg ;  //表情框
	CFaceList			m_FaceList;  //表情链表类
	BOOL				m_bDraged;    //正在被拖拽
	IDataObject*		m_pIDragObj; // 拖拽源

	//std::string         m_strMsgRecordStartDate;
	// 每一页的消息记录的显示的条数
	int                 m_nEachPageRecordNum;
	// 查看消息记录的方式， 翻页
	E_CHECKMSGRECORD    m_checkMsgRecordType;
	// 聊天记录是否到第一页
	bool                m_bToFristPage;
	// 聊天记录是否到最后一页
	bool				m_bToLastPage;

	bool                m_bLessComparison  ;
	vector<msginfo>     m_vMsgRecrd;
	//当前消息记录中最大日期
	int			m_nMaxMsgId;
	//当前消息记录中最小日期
	int 		m_nMinMsgId;
};

