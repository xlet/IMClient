#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once

#include "OleHelper.h"
#include  <stdio.h>
#include  <textserv.h>
#include "IDragDropImpl.h"
#include <list>
namespace DuiLib {


	typedef void (*ON_LINK_NOTIFY_EVENT)(void*, CDuiString, LONG cpMin, LONG cpMax);

//类声明
class CTxtWinHost;
class CImageDataObject;
class CGifOleObject;
#include <stdio.h>
#include <Richole.h>
class CTxtWinHost;
class RichEditEvent
{
public:
	//DragDrop
	virtual bool OnDragDropOver( const tString& pFilePath ) = 0;
	virtual bool OnDropDown( const tString& pFilePath,int nPos ) = 0;
	virtual bool OnDropLeave( const tString& pFilePath ) = 0 ;	
	/*---------------------------------------------------------------------*\
			decription : 鼠标或键盘点击ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
					bRight	—— true表示鼠标右键，false表示鼠标左键
	\*---------------------------------------------------------------------*/
	virtual void OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight ) = 0;
	/*---------------------------------------------------------------------*\
			decription : 删除ole对象
			parament:
					pObj	—— 文件路径 或 ControlUI
					dwType	—— 值为1时pObj指向文件路径，值为2时指向ControlUI
	\*---------------------------------------------------------------------*/
	virtual void OnDeleteOleObject(void* pObj,DWORD dwType) = 0;

	virtual void OnUrl(void* pObj,DWORD dwType,long lPos) = 0;
};

#if 0
class CHAT_IMAGE
{
public:
	int nType;
	TCHAR strGroupJID[JID_BUFFER_LEN];
	TCHAR strFont[1024];
	TCHAR strTime[CHAT_TIME_LEN];
	TCHAR strFromJID[JID_BUFFER_LEN];
	TCHAR strChatMsg[MACRO_DB_MSGCONTEXT];
	std::list<img_info> img;
	CHAT_IMAGE()
	{

		ZeroMemory(strGroupJID, sizeof(strGroupJID));
		ZeroMemory(strFont, sizeof(strFont));
		ZeroMemory(strTime, sizeof(strTime));
		ZeroMemory(strFromJID, sizeof(strFromJID));
		ZeroMemory(strChatMsg, sizeof(strChatMsg));
		nType = -1;
	}
};



#endif
class UILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI, public IDragDropEvent
{
public:
#if 1
	class IRichEditOleExCallback : public IRichEditOleCallback
	{
	public :
		IRichEditOleExCallback(CRichEditUI* _pRichEdit);
		virtual ~ IRichEditOleExCallback();
		int m_iNumStorages;
		IStorage * pStorage;
		DWORD m_dwRef;
		CRichEditUI*	m_pOwner;
		//static bool	g_bDrop;
		//static bool g_bDropSendFileDlg;
		//static CRichEditUI* g_pDropWindow;
	public:
		virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE * lplpstg);
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR * lplpFrame,
			LPOLEINPLACEUIWINDOW FAR * lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
		virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
		virtual HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
		virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
		virtual HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat,
			DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
		virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
		virtual HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR * lpchrg, DWORD reco, LPDATAOBJECT FAR * lplpdataobj);
		virtual HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR * lpchrg, HMENU FAR * lphmenu);
	};
#endif
public:
	//RichEidt OLE对象回调类

	BOOL DoMyDrag(HWND hwnd);
	//
    CRichEditUI();
    ~CRichEditUI();

	//add by zhaohaibo 2014-14
	//EN_LINK回调
	void* m_pLinkNotify;
	ON_LINK_NOTIFY_EVENT m_pLinkNotifyCallback;
	void SetLinkNotifyCallback(ON_LINK_NOTIFY_EVENT pCallback);
	CDuiString GetLinkText(long lMin, long lMax);
	void SetLinkNotify(void* pNotify);
	//void OnTxNotify(DWORD iNotify, void *pv);


	//end the code 
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    bool IsWantTab();
    void SetWantTab(bool bWantTab = true);
    bool IsWantReturn();
    void SetWantReturn(bool bWantReturn = true);
    bool IsWantCtrlReturn();
    void SetWantCtrlReturn(bool bWantCtrlReturn = true);
    bool IsRich();
    void SetRich(bool bRich = true);
    bool IsReadOnly();
    void SetReadOnly(bool bReadOnly = true);
    bool GetWordWrap();
    void SetWordWrap(bool bWordWrap = true);
    int GetFont();
    void SetFont(int index);
    void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    LONG GetWinStyle();
    void SetWinStyle(LONG lStyle);
    DWORD GetTextColor();
    void SetTextColor(DWORD dwTextColor);
    int GetLimitText();
    void SetLimitText(int iChars);
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
    CDuiString GetText() const;
    void SetText(LPCTSTR pstrText);
    bool GetModify() const;
    void SetModify(bool bModified = true) const;
    void GetSel(CHARRANGE &cr) const;
    void GetSel(long& nStartChar, long& nEndChar) const;
    int SetSel(CHARRANGE &cr);
    int SetSel(long nStartChar, long nEndChar);
    void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
    CDuiString GetSelText() const;
    int SetSelAll();
    int SetSelNone();
    WORD GetSelectionType() const;
    bool GetZoom(int& nNum, int& nDen) const;
    bool SetZoom(int nNum, int nDen);
    bool SetZoomOff();
    bool GetAutoURLDetect() const;
    bool SetAutoURLDetect(bool bAutoDetect = true);
    DWORD GetEventMask() const;
    DWORD SetEventMask(DWORD dwEventMask);
    CDuiString GetTextRange(long nStartChar, long nEndChar) const;
    void HideSelection(bool bHide = true, bool bChangeStyle = false);
    void ScrollCaret();
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
    int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);
    bool SetWordCharFormat(CHARFORMAT2 &cf);
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;
    bool SetParaFormat(PARAFORMAT2 &pf);
    bool Redo();
    bool Undo();
    void Clear();
    void Copy();
    void Cut();
    void Paste();
    int GetLineCount() const;
    CDuiString GetLine(int nIndex, int nMaxLength) const;
    int LineIndex(int nLine = -1) const;
    int LineLength(int nLine = -1) const;
    bool LineScroll(int nLines, int nChars = 0);
	CPoint GetCharPos(long lChar) const;
    long LineFromChar(long nIndex) const;
    CPoint PosFromChar(UINT nChar) const;
    int CharFromPos(CPoint pt) const;
    void EmptyUndoBuffer();
    UINT SetUndoLimit(UINT nLimit);
    long StreamIn(int nFormat, EDITSTREAM &es);
    long StreamOut(int nFormat, EDITSTREAM &es);

    void DoInit();
    // 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
    // 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
	bool SetDropAcceptFile(bool bAccept);
    virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
    IDropTarget* GetTxDropTarget();
    virtual bool OnTxViewChanged();
    virtual void OnTxNotify(DWORD iNotify, void *pv);

    void SetScrollPos(SIZE szPos);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//for OLE Object 
	//add  by zhaohaibo

	//ITextHost * GetTextHost();
	ITextServices * GetTextServices();


	IRichEditOle* GetIRichEditOle();
	BOOL SetOLECallback(IRichEditOleCallback* pCallback);

	bool InsertNotGIFImage(LPCTSTR szFilePath, DWORD dwUserData);
	bool InsertGIFImage(LPCTSTR szFilePath, DWORD dwUserData);

	virtual IDragDropEvent* getDragEvent();
	bool EnableDropDrag( void );

	void RegisterEventSink(RichEditEvent* pEvenSink);
	//end 
protected:
	//by zhaohaibo
	HRESULT DragEnter(IDataObject *pDataObj,DWORD grfKeyState,
		POINTL pt,POINT ptClient,DWORD *pdwEffect);

	HRESULT DragOver(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,POINT ptClient,
		DWORD *pdwEffect);

	HRESULT DragLeave(IDataObject *pDataObj);

	HRESULT Drop(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,POINT ptClient,
		DWORD *pdwEffect);
	//end
	
public:
	long m_dropBegin;
	long m_dropEnd;
	bool				m_bInit;
	static UINT m_uOwnOleClipboardFormat;
	UINT_PTR			m_timer;
public: //黏贴板操作 by zhaohaibo
#if 0
	bool DoClipboard();
	bool DoWriteClipData(bool bIsCut = false);
	static bool DoWriteClipData(HINSTANCE hIstance,HWND hWnd,::CHAT_IMAGE& chat, CDuiRect rect = CDuiRect(0, 0, 0, 0));
	bool DoReplace();
	static void DoWriteHBitmapClip( HINSTANCE hInstance ,HWND hWnd,CDuiString strImagePath, CDuiRect Rect );
	static void DoWriteHTMLWord( ::CHAT_IMAGE& chat);
	static int CHATIMG2char(::CHAT_IMAGE& chat,TCHAR** szBuff);
	static bool SaveHIcon2PngFile(HICON hIcon, LPCTSTR lpszPicFileName);
	static BOOL GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void ParseClipMsg( CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list, CString& TagMsg);
	HRESULT InsertClipMsg( CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list );
	BOOL GetSelClipData(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list);
	void GetClipWord(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list, CDuiString& strClipWord);
	void WriteHTMLClip( CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list);
	void WriteHBitmapClip( std::list<IMG_CLIP_MSG *>& list);

	HRESULT InsertHTMLPicture( CDuiString& strHtml);
	HRESULT InsertHTMLWord(CDuiString& strHtml);
	HRESULT ChangeCharEntities(CDuiString& strHtml);

	//html字符串转换成CHAT_IMG
	HRESULT InsertHtmlClipData(char* szData);
	//CHAT_IMG转换成html字符串
	int CHATIMG2char(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list,TCHAR** szBuff);

	HBITMAP DIBToDDB( HANDLE hDIB);


	//粘贴板转成png文件
	HANDLE DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE pPal );
	BOOL WriteDIB( LPCTSTR szFile, HANDLE hDIB);
	BOOL ConvertToBMP(const TCHAR * strFileName,const TCHAR* strBMPFile,BOOL bScaleImage);
	BOOL SetBackColorToWhite(HDC pDC);
	BOOL ConvertEMFToBMP(const TCHAR * pszEMFFile,const TCHAR* pszBMPFile,BOOL bScaleImage=false);

	void ClearClipList(std::list<IMG_CLIP_MSG *>& list);
	int GetTextCount( CString  strText );
	CString GetTextByCount(CString sText, int nCount );
#endif
protected:
	//add by zhaohaibo
	//IRichEditOleCallback* m_pIRichEditOleCallback;
	vector<DuiLib::tString> m_vDragDorpFile;
	/*IRichEditOleExCallback*/IRichEditOleCallback* m_pIRichEditOleCallback;


	//end

   CTxtWinHost* m_pTwh;
    bool m_bVScrollBarFixing;
    bool m_bWantTab;
    bool m_bWantReturn;
    bool m_bWantCtrlReturn;
    bool m_bRich;
    bool m_bReadOnly;
    bool m_bWordWrap;
    DWORD m_dwTextColor;
    int m_iFont;
    int m_iLimitText;
    LONG m_lTwhStyle;
	bool m_bInited;


	//add by zhaohaibo 
	//end
};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
