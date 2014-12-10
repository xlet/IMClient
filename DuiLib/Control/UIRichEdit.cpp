#include "stdafx.h"


// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0
#include "UIRichEditFun.h"

namespace DuiLib {

const LONG cInitTextMax = (32 * 1024) - 1;

EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};


#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#define MSG_LBUTTONDBCL_POST_GUID			(WM_USER + 226)
#define MSG_PASTE							(WM_USER + 227)
#define MSG_TRY_PASTE_IMG					(WM_USER + 228)
#define MSG_OLE_LBTNDOWN					(WM_USER + 229)
#define MSG_OLE_COPY						(WM_USER + 230)
#define TM_DROPFILE_MSG						(WM_USER + 240)	//文件拖放消息
#define UM_STOP_AUTO_REPLY					(WM_USER + 5129)
#define MSG_ADD_FACE						(WM_USER + 5131)
#define UM_CLICKED_USER_LINK				(WM_USER + 5132)        //点击了聊天窗口中具有超链接的用户名

#define LOAD_FAIL_PNG _T("png_recv_fail.png")
#define LOAD_WAIT_GIF _T("gif_wait_download.gif")
#define WIN_OWN_CLIP_TYPE _T("WINXUN_OWN_CLIP_TYPE")

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>

class CTxtWinHost : public ITextHost
{
public:
    CTxtWinHost();
    BOOL Init(CRichEditUI *re , const CREATESTRUCT *pcs);
    virtual ~CTxtWinHost();
	void setEventSink(RichEditEvent *pSink);
	RichEditEvent* getEventSink(void);

    ITextServices* GetTextServices(void) { return pserv; }
    void SetClientRect(RECT *prc);
    RECT* GetClientRect() { return &rcClient; }
    BOOL GetWordWrap(void) { return fWordWrap; }
    void SetWordWrap(BOOL fWordWrap);
    BOOL GetReadOnly();
    void SetReadOnly(BOOL fReadOnly);
    void SetFont(HFONT hFont);
    void SetColor(DWORD dwColor);
    SIZEL* GetExtent();
    void SetExtent(SIZEL *psizelExtent);
    void LimitText(LONG nChars);
    BOOL IsCaptured();

    BOOL GetAllowBeep();
    void SetAllowBeep(BOOL fAllowBeep);
    WORD GetDefaultAlign();
    void SetDefaultAlign(WORD wNewAlign);
    BOOL GetRichTextFlag();
    void SetRichTextFlag(BOOL fNew);
    LONG GetDefaultLeftIndent();
    void SetDefaultLeftIndent(LONG lNewIndent);
    BOOL SetSaveSelection(BOOL fSaveSelection);
    HRESULT OnTxInPlaceDeactivate();
    HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
    BOOL GetActiveState(void) { return fInplaceActive; }
    BOOL DoSetCursor(RECT *prc, POINT *pt);
    void SetTransparent(BOOL fTransparent);
    void GetControlRect(LPRECT prc);
    LONG SetAccelPos(LONG laccelpos);
    WCHAR SetPasswordChar(WCHAR chPasswordChar);
    void SetDisabled(BOOL fOn);
    LONG SetSelBarWidth(LONG lSelBarWidth);
    BOOL GetTimerState();

    void SetCharFormat(CHARFORMAT2W &c);
    void SetParaFormat(PARAFORMAT2 &p);

    // -----------------------------
    //	IUnknown interface
    // -----------------------------
    virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG _stdcall AddRef(void);
    virtual ULONG _stdcall Release(void);

    // -----------------------------
    //	ITextHost interface
    // -----------------------------
    virtual HDC TxGetDC();
    virtual INT TxReleaseDC(HDC hdc);
    virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
    virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
    virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
    virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
    virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
    virtual void TxViewChange(BOOL fUpdate);
    virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
    virtual BOOL TxShowCaret(BOOL fShow);
    virtual BOOL TxSetCaretPos(INT x, INT y);
    virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
    virtual void TxKillTimer(UINT idTimer);
    virtual void TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
    virtual void TxSetCapture(BOOL fCapture);
    virtual void TxSetFocus();
    virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
    virtual BOOL TxScreenToClient (LPPOINT lppt);
    virtual BOOL TxClientToScreen (LPPOINT lppt);
    virtual HRESULT TxActivate( LONG * plOldState );
    virtual HRESULT TxDeactivate( LONG lNewState );
    virtual HRESULT TxGetClientRect(LPRECT prc);
    virtual HRESULT TxGetViewInset(LPRECT prc);
    virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
    virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
    virtual COLORREF TxGetSysColor(int nIndex);
    virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
    virtual HRESULT TxGetMaxLength(DWORD *plength);
    virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
    virtual HRESULT TxGetPasswordChar(TCHAR *pch);
    virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
    virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
    virtual HRESULT OnTxCharFormatChange (const CHARFORMATW * pcf);
    virtual HRESULT OnTxParaFormatChange (const PARAFORMAT * ppf);
    virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
    virtual HRESULT TxNotify(DWORD iNotify, void *pv);
    virtual HIMC TxImmGetContext(void);
    virtual void TxImmReleaseContext(HIMC himc);
    virtual HRESULT TxGetSelectionBarWidth (LONG *lSelBarWidth);

public:
	ITextHost* GetTextHost()
	{
		AddRef();
		return this;
	}

	ITextServices * GetTextServices2()
	{
		if (NULL == pserv)
			return NULL;
		pserv->AddRef();
		return pserv;
	}

private:
	RichEditEvent	*m_pSink;
    CRichEditUI *m_re;
    ULONG	cRefs;					// Reference Count
    ITextServices	*pserv;		    // pointer to Text Services object
    // Properties

    DWORD		dwStyle;				// style bits

    unsigned	fEnableAutoWordSel	:1;	// enable Word style auto word selection?
    unsigned	fWordWrap			:1;	// Whether control should word wrap
    unsigned	fAllowBeep			:1;	// Whether beep is allowed
    unsigned	fRich				:1;	// Whether control is rich text
    unsigned	fSaveSelection		:1;	// Whether to save the selection when inactive
    unsigned	fInplaceActive		:1; // Whether control is inplace active
    unsigned	fTransparent		:1; // Whether control is transparent
    unsigned	fTimer				:1;	// A timer is set
    unsigned    fCaptured           :1;

    LONG		lSelBarWidth;			// Width of the selection bar
    LONG  		cchTextMost;			// maximum text size
    DWORD		dwEventMask;			// DoEvent mask to pass on to parent window
    LONG		icf;
    LONG		ipf;
    RECT		rcClient;				// Client Rect for this control
    SIZEL		sizelExtent;			// Extent array
    CHARFORMAT2W cf;					// Default character format
    PARAFORMAT2	pf;					    // Default paragraph format
    LONG		laccelpos;				// Accelerator position
    WCHAR		chPasswordChar;		    // Password character
};

void CTxtWinHost::setEventSink(RichEditEvent *pSink)
{
	m_pSink = pSink;
}
RichEditEvent* CTxtWinHost::getEventSink(void)
{
	return m_pSink;
}
// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
    return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
    return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}

HRESULT InitDefaultCharFormat(CRichEditUI* re, CHARFORMAT2W* pcf, HFONT hfont) 
{
    memset(pcf, 0, sizeof(CHARFORMAT2W));
    LOGFONT lf;
    if( !hfont )
        hfont = re->GetManager()->GetFont(re->GetFont());
    ::GetObject(hfont, sizeof(LOGFONT), &lf);

    DWORD dwColor = re->GetTextColor();
    pcf->cbSize = sizeof(CHARFORMAT2W);
    pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    LONG yPixPerInch = GetDeviceCaps(re->GetManager()->GetPaintDC(), LOGPIXELSY);
    pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    pcf->yOffset = 0;
    pcf->dwEffects = 0;
    pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
    if(lf.lfWeight >= FW_BOLD)
        pcf->dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        pcf->dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        pcf->dwEffects |= CFE_UNDERLINE;
    pcf->bCharSet = lf.lfCharSet;
    pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

    return S_OK;
}

HRESULT InitDefaultParaFormat(CRichEditUI* re, PARAFORMAT2* ppf) 
{	
    memset(ppf, 0, sizeof(PARAFORMAT2));
    ppf->cbSize = sizeof(PARAFORMAT2);
    ppf->dwMask = PFM_ALL;
    ppf->wAlignment = PFA_LEFT;
    ppf->cTabCount = 1;
    ppf->rgxTabs[0] = lDefaultTab;

    return S_OK;
}

HRESULT CreateHost(CRichEditUI *re, const CREATESTRUCT *pcs, CTxtWinHost **pptec)
{
    HRESULT hr = E_FAIL;
    //GdiSetBatchLimit(1);

    CTxtWinHost *phost = new CTxtWinHost();
    if(phost)
    {
        if (phost->Init(re, pcs))
        {
            *pptec = phost;
            hr = S_OK;
        }
    }

    if (FAILED(hr))
    {
        delete phost;
    }

    return TRUE;
}

CTxtWinHost::CTxtWinHost() : m_re(NULL)
{
    ::ZeroMemory(&cRefs, sizeof(CTxtWinHost) - offsetof(CTxtWinHost, cRefs));
    cchTextMost = cInitTextMax;
    laccelpos = -1;
	m_pSink = 0;
}

CTxtWinHost::~CTxtWinHost()
{
    pserv->OnTxInPlaceDeactivate();
    pserv->Release();
}

////////////////////// Create/Init/Destruct Commands ///////////////////////

BOOL CTxtWinHost::Init(CRichEditUI *re, const CREATESTRUCT *pcs)
{
    IUnknown *pUnk;
    HRESULT hr;

    m_re = re;
    // Initialize Reference count
    cRefs = 1;

    // Create and cache CHARFORMAT for this control
    if(FAILED(InitDefaultCharFormat(re, &cf, NULL)))
        goto err;

    // Create and cache PARAFORMAT for this control
    if(FAILED(InitDefaultParaFormat(re, &pf)))
        goto err;

    // edit controls created without a window are multiline by default
    // so that paragraph formats can be
    dwStyle = ES_MULTILINE;

    // edit controls are rich by default
    fRich = re->IsRich();

    cchTextMost = re->GetLimitText();

    if (pcs )
    {
        dwStyle = pcs->style;

        if ( !(dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)) )
        {
            fWordWrap = TRUE;
        }
    }

    if( !(dwStyle & ES_LEFT) )
    {
        if(dwStyle & ES_CENTER)
            pf.wAlignment = PFA_CENTER;
        else if(dwStyle & ES_RIGHT)
            pf.wAlignment = PFA_RIGHT;
    }

    fInplaceActive = TRUE;

    // Create Text Services component
    //if(FAILED(CreateTextServices(NULL, this, &pUnk)))
    //    goto err;

	PCreateTextServices TextServicesProc;
	HMODULE hmod = LoadLibrary(_T("msftedit.dll"));
	if (hmod)
	{
		TextServicesProc = (PCreateTextServices)GetProcAddress(hmod,"CreateTextServices");
	}

	if (TextServicesProc)
	{
		HRESULT hr = TextServicesProc(NULL, this, &pUnk);
	}

    hr = pUnk->QueryInterface(IID_ITextServices,(void **)&pserv);

    // Whether the previous call succeeded or failed we are done
    // with the private interface.
    pUnk->Release();

    if(FAILED(hr))
    {
        goto err;
    }

    // Set window text
    if(pcs && pcs->lpszName)
    {
#ifdef _UNICODE		
        if(FAILED(pserv->TxSetText((TCHAR *)pcs->lpszName)))
            goto err;
#else
        size_t iLen = _tcslen(pcs->lpszName);
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
        if(FAILED(pserv->TxSetText((LPWSTR)lpText))) {
            delete[] lpText;
            goto err;
        }
        delete[] lpText;
#endif
    }

    return TRUE;

err:
    return FALSE;
}

/////////////////////////////////  IUnknown ////////////////////////////////


HRESULT CTxtWinHost::QueryInterface(REFIID riid, void **ppvObject)
{
    HRESULT hr = E_NOINTERFACE;
    *ppvObject = NULL;

    if (IsEqualIID(riid, IID_IUnknown) 
        || IsEqualIID(riid, IID_ITextHost)) 
    {
        AddRef();
        *ppvObject = (ITextHost *) this;
        hr = S_OK;
    }

    return hr;
}

ULONG CTxtWinHost::AddRef(void)
{
    return ++cRefs;
}

ULONG CTxtWinHost::Release(void)
{
    ULONG c_Refs = --cRefs;

    if (c_Refs == 0)
    {
        delete this;
    }

    return c_Refs;
}

/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC CTxtWinHost::TxImmGetContext(void)
{
    return NULL;
}

void CTxtWinHost::TxImmReleaseContext(HIMC himc)
{
    //::ImmReleaseContext( hwnd, himc );
}

//////////////////////////// ITextHost Interface  ////////////////////////////

HDC CTxtWinHost::TxGetDC()
{
    return m_re->GetManager()->GetPaintDC();
}

int CTxtWinHost::TxReleaseDC(HDC hdc)
{
    return 1;
}

BOOL CTxtWinHost::TxShowScrollBar(INT fnBar, BOOL fShow)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_BOTH ) {
        if( pVerticalScrollBar ) pVerticalScrollBar->SetVisible(fShow == TRUE);
        if( pHorizontalScrollBar ) pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
{
    if( fuSBFlags == SB_VERT ) {
        m_re->EnableScrollBar(true, m_re->GetHorizontalScrollBar() != NULL);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_HORZ ) {
        m_re->EnableScrollBar(m_re->GetVerticalScrollBar() != NULL, true);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_BOTH ) {
        m_re->EnableScrollBar(true, true);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0 ) {
            pVerticalScrollBar->SetVisible(false);
        }
        else {
            pVerticalScrollBar->SetVisible(true);
			int nRange = nMaxPos - nMinPos - rcClient.bottom + rcClient.top;
            pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
        }
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0 ) {
            pHorizontalScrollBar->SetVisible(false);
        }
        else {
            pHorizontalScrollBar->SetVisible(true);
            pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
        }   
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetScrollPos(nPos);
		//int nRange1 =  m_re->GetVerticalScrollBar()->GetScrollRange();
		// add by zhaohaibo 
		// 修复richEdit 滚动条异常问题。 窗口隐藏或最小化 收到richEdit信息，再恢复正常会导致
		// 内容滚动的范围超出了滚动条的范围
		if (nPos > m_re->GetVerticalScrollBar()->GetScrollRange())
		{
			m_re->EndDown()
				;
		}
		// end 
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetScrollPos(nPos);
    }
    return TRUE;
}

void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
    if( prc == NULL ) {
        m_re->GetManager()->Invalidate(rcClient);
        return;
    }
    RECT rc = *prc;
    m_re->GetManager()->Invalidate(rc);
}

void CTxtWinHost::TxViewChange(BOOL fUpdate) 
{
    if( m_re->OnTxViewChanged() ) m_re->Invalidate();
}

BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
    return ::CreateCaret(m_re->GetManager()->GetPaintWindow(), hbmp, xWidth, yHeight);
}

BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
{
    if(fShow)
        return ::ShowCaret(m_re->GetManager()->GetPaintWindow());
    else
        return ::HideCaret(m_re->GetManager()->GetPaintWindow());
}

BOOL CTxtWinHost::TxSetCaretPos(INT x, INT y)
{
    return ::SetCaretPos(x, y);
}

BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
    fTimer = TRUE;
    return m_re->GetManager()->SetTimer(m_re, idTimer, uTimeout) == TRUE;
}

void CTxtWinHost::TxKillTimer(UINT idTimer)
{
    m_re->GetManager()->KillTimer(m_re, idTimer);
    fTimer = FALSE;
}

void CTxtWinHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
{
    return;
}

void CTxtWinHost::TxSetCapture(BOOL fCapture)
{
    if (fCapture) m_re->GetManager()->SetCapture();
    else m_re->GetManager()->ReleaseCapture();
    fCaptured = fCapture;
}

void CTxtWinHost::TxSetFocus()
{
    m_re->SetFocus();
}

void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
{
    ::SetCursor(hcur);
}

BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
{
    return ::ScreenToClient(m_re->GetManager()->GetPaintWindow(), lppt);	
}

BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
{
    return ::ClientToScreen(m_re->GetManager()->GetPaintWindow(), lppt);
}

HRESULT CTxtWinHost::TxActivate(LONG *plOldState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
{
    *prc = rcClient;
    GetControlRect(prc);
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc) 
{
    prc->left = prc->right = prc->top = prc->bottom = 0;
    return NOERROR;	
}

HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
    *ppCF = &cf;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
    *ppPF = &pf;
    return NOERROR;
}

COLORREF CTxtWinHost::TxGetSysColor(int nIndex) 
{
    return ::GetSysColor(nIndex);
}

HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
    *pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetMaxLength(DWORD *pLength)
{
    *pLength = cchTextMost;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
    *pdwScrollBar =  dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
        ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

    return NOERROR;
}

HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef _UNICODE
    *pch = chPasswordChar;
#else
    ::WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG *pcp)
{
    *pcp = laccelpos;
    return S_OK;
} 										   

HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
    return S_OK;
}

HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
{
    DWORD dwProperties = 0;

    if (fRich)
    {
        dwProperties = TXTBIT_RICHTEXT;
    }

    if (dwStyle & ES_MULTILINE)
    {
        dwProperties |= TXTBIT_MULTILINE;
    }

    if (dwStyle & ES_READONLY)
    {
        dwProperties |= TXTBIT_READONLY;
    }

    if (dwStyle & ES_PASSWORD)
    {
        dwProperties |= TXTBIT_USEPASSWORD;
    }

    if (!(dwStyle & ES_NOHIDESEL))
    {
        dwProperties |= TXTBIT_HIDESELECTION;
    }

    if (fEnableAutoWordSel)
    {
        dwProperties |= TXTBIT_AUTOWORDSEL;
    }

    if (fWordWrap)
    {
        dwProperties |= TXTBIT_WORDWRAP;
    }

    if (fAllowBeep)
    {
        dwProperties |= TXTBIT_ALLOWBEEP;
    }

    if (fSaveSelection)
    {
        dwProperties |= TXTBIT_SAVESELECTION;
    }

    *pdwBits = dwProperties & dwMask; 
    return NOERROR;
}


HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void *pv)
{
    if( iNotify == EN_REQUESTRESIZE ) {
        RECT rc;
        REQRESIZE *preqsz = (REQRESIZE *)pv;
        GetControlRect(&rc);
        rc.bottom = rc.top + preqsz->rc.bottom;
        rc.right  = rc.left + preqsz->rc.right;
        SetClientRect(&rc);
    }
    m_re->OnTxNotify(iNotify, pv);
    return S_OK;
}

HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
{
    *lpExtent = sizelExtent;
    return S_OK;
}

HRESULT	CTxtWinHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
{
    *plSelBarWidth = lSelBarWidth;
    return S_OK;
}

void CTxtWinHost::SetWordWrap(BOOL fWordWrap)
{
    fWordWrap = fWordWrap;
    pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
}

BOOL CTxtWinHost::GetReadOnly()
{
    return (dwStyle & ES_READONLY) != 0;
}

void CTxtWinHost::SetReadOnly(BOOL fReadOnly)
{
    if (fReadOnly)
    {
        dwStyle |= ES_READONLY;
    }
    else
    {
        dwStyle &= ~ES_READONLY;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, 
        fReadOnly ? TXTBIT_READONLY : 0);
}

void CTxtWinHost::SetFont(HFONT hFont) 
{
    if( hFont == NULL ) return;
    LOGFONT lf;
    ::GetObject(hFont, sizeof(LOGFONT), &lf);
    LONG yPixPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY);
    cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    if(lf.lfWeight >= FW_BOLD)
        cf.dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        cf.dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        cf.dwEffects |= CFE_UNDERLINE;
    cf.bCharSet = lf.lfCharSet;
    cf.bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(cf.szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, cf.szFaceName, LF_FACESIZE) ;
#endif

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

void CTxtWinHost::SetColor(DWORD dwColor)
{
    cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

SIZEL* CTxtWinHost::GetExtent() 
{
    return &sizelExtent;
}

void CTxtWinHost::SetExtent(SIZEL *psizelExtent) 
{ 
    sizelExtent = *psizelExtent; 
    pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
}

void CTxtWinHost::LimitText(LONG nChars)
{
    cchTextMost = nChars;
    if( cchTextMost <= 0 ) cchTextMost = cInitTextMax;
    pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
}

BOOL CTxtWinHost::IsCaptured()
{
    return fCaptured;
}

BOOL CTxtWinHost::GetAllowBeep()
{
    return fAllowBeep;
}

void CTxtWinHost::SetAllowBeep(BOOL fAllowBeep)
{
    fAllowBeep = fAllowBeep;

    pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, 
        fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

WORD CTxtWinHost::GetDefaultAlign()
{
    return pf.wAlignment;
}

void CTxtWinHost::SetDefaultAlign(WORD wNewAlign)
{
    pf.wAlignment = wNewAlign;

    // Notify control of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL CTxtWinHost::GetRichTextFlag()
{
    return fRich;
}

void CTxtWinHost::SetRichTextFlag(BOOL fNew)
{
    fRich = fNew;

    pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, 
        fNew ? TXTBIT_RICHTEXT : 0);
}

LONG CTxtWinHost::GetDefaultLeftIndent()
{
    return pf.dxOffset;
}

void CTxtWinHost::SetDefaultLeftIndent(LONG lNewIndent)
{
    pf.dxOffset = lNewIndent;

    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void CTxtWinHost::SetClientRect(RECT *prc) 
{
    rcClient = *prc;

    LONG xPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSX); 
    LONG yPerInch =	::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY); 
    sizelExtent.cx = DXtoHimetricX(rcClient.right - rcClient.left, xPerInch);
    sizelExtent.cy = DYtoHimetricY(rcClient.bottom - rcClient.top, yPerInch);

    pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
}

BOOL CTxtWinHost::SetSaveSelection(BOOL f_SaveSelection)
{
    BOOL fResult = f_SaveSelection;

    fSaveSelection = f_SaveSelection;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, 
        fSaveSelection ? TXTBIT_SAVESELECTION : 0);

    return fResult;		
}

HRESULT	CTxtWinHost::OnTxInPlaceDeactivate()
{
    HRESULT hr = pserv->OnTxInPlaceDeactivate();

    if (SUCCEEDED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

HRESULT	CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
    fInplaceActive = TRUE;

    HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

    if (FAILED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

BOOL CTxtWinHost::DoSetCursor(RECT *prc, POINT *pt)
{
    RECT rc = prc ? *prc : rcClient;

    // Is this in our rectangle?
    if (PtInRect(&rc, *pt))
    {
        RECT *prcClient = (!fInplaceActive || prc) ? &rc : NULL;
        pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_re->GetManager()->GetPaintDC(),
            NULL, prcClient, pt->x, pt->y);

        return TRUE;
    }

    return FALSE;
}

void CTxtWinHost::GetControlRect(LPRECT prc)
{
    prc->top = rcClient.top;
    prc->bottom = rcClient.bottom;
    prc->left = rcClient.left;
    prc->right = rcClient.right;
}

void CTxtWinHost::SetTransparent(BOOL f_Transparent)
{
    fTransparent = f_Transparent;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG CTxtWinHost::SetAccelPos(LONG l_accelpos)
{
    LONG laccelposOld = l_accelpos;

    laccelpos = l_accelpos;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

    return laccelposOld;
}

WCHAR CTxtWinHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
    WCHAR chOldPasswordChar = chPasswordChar;

    chPasswordChar = ch_PasswordChar;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
        (chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

    return chOldPasswordChar;
}

void CTxtWinHost::SetDisabled(BOOL fOn)
{
    cf.dwMask	 |= CFM_COLOR | CFM_DISABLED;
    cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

    if( !fOn )
    {
        cf.dwEffects &= ~CFE_DISABLED;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

LONG CTxtWinHost::SetSelBarWidth(LONG l_SelBarWidth)
{
    LONG lOldSelBarWidth = lSelBarWidth;

    lSelBarWidth = l_SelBarWidth;

    if (lSelBarWidth)
    {
        dwStyle |= ES_SELECTIONBAR;
    }
    else
    {
        dwStyle &= (~ES_SELECTIONBAR);
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

    return lOldSelBarWidth;
}

BOOL CTxtWinHost::GetTimerState()
{
    return fTimer;
}

void CTxtWinHost::SetCharFormat(CHARFORMAT2W &c)
{
    cf = c;
}

void CTxtWinHost::SetParaFormat(PARAFORMAT2 &p)
{
    pf = p;
}





/////////////////////////////////////////////////////////////////////////////////////
//
//

//////////////////////////////////////////////////////////////////////////
//                 
// 
#if 1
CRichEditUI::IRichEditOleExCallback::IRichEditOleExCallback(CRichEditUI* _pRichEdit)
{
	pStorage = NULL;
	m_iNumStorages = 0 ;
	m_dwRef = 0 ;
	m_pOwner	= _pRichEdit;
	// set up OLE storage 
	HRESULT hResult = ::StgCreateDocfile( NULL,
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE /* | STGM_DELETEONRELEASE */ | STGM_CREATE ,
		0 , & pStorage );
	ASSERT( pStorage != NULL && hResult == S_OK && "Failed to setup OLE storage" );
	if ( pStorage == NULL ||
		hResult != S_OK )
	{
	}
	else
	{
		AddRef();
	}
}

CRichEditUI::IRichEditOleExCallback:: ~ IRichEditOleExCallback()
{
	Release();
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::GetNewStorage(LPSTORAGE * lplpstg)
{
	m_iNumStorages ++ ;
	CAtlStringW storageName;
	storageName.Format(L" REOLEStorage%d ", m_iNumStorages);
	HRESULT hResult = pStorage -> CreateStorage(  storageName.GetString(), 
		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
		0 , 0 , lplpstg );
	ASSERT( hResult == S_OK && "Failed to create storage" );

	return hResult;
}

EXTERN_C const IID IID_IRichEditOleExCallback = { 
	0x00020D03,
	0,
	0,
	{0xC0,0,0,0,0,0,0,0x46}
};

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::QueryInterface(REFIID iid, void ** ppvObject)
{
	HRESULT hr = S_OK;
	* ppvObject = NULL;

	if ( iid == IID_IUnknown || 
		iid == IID_IRichEditOleExCallback )
	{
		* ppvObject = this ;
		AddRef();
		hr = NOERROR;
	}
	else 
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}

ULONG STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::AddRef()
{
	return ++ m_dwRef;
}

ULONG STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::Release()
{
	if ( -- m_dwRef == 0 )
	{
		delete this ;
		return 0 ;
	}

	return m_dwRef;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR * lplpFrame,
	LPOLEINPLACEUIWINDOW FAR * lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::ShowContainerUI(BOOL fShow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp)
{
	if (!m_pOwner) return S_FALSE;

	IRichEditOle* pRichEdit = m_pOwner->GetIRichEditOle();
	pRichEdit->Release();
	int nCount = pRichEdit->GetObjectCount();
	if (nCount == 1)
	{
		//KillTimer(m_pOwner->GetManager()->GetPaintWindow(), m_pOwner->m_timer);
		//m_pOwner->m_timer = 0;
	}
	return S_OK;

}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::DeleteObject(LPOLEOBJECT lpoleobj)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat,
	DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
	if (!fReally || !lpdataobj)
	{
		OutputDebugString(_T("AcceptData Error!\n"));
		return S_OK;
	}


	bool bIsCopyFile = false;
	bool  bCopyOther = false;

	OutputDebugString(TEXT("AcceptData \n"));
	//tryDebug0(TEXT("QueryAcceptData"));
	bool bDragFile = false;

	//只处理粘贴  
	switch(reco)  
	{  
	case RECO_DRAG:
		{
			break;
		}
	case RECO_PASTE: 
		{
			HDROP       hdrop;
			STGMEDIUM stgmed; 
			ZeroMemory(&stgmed, sizeof(STGMEDIUM));
			FORMATETC tFormat={ 0xC007/*FileNameW*/, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };  

			HRESULT hr = lpdataobj->GetData(&tFormat, &stgmed);
			if (SUCCEEDED(hr))
			{

				//tryDebug0(TEXT("粘贴文件"));
				tFormat.cfFormat = CF_HDROP;
				bDragFile = true;
				hr = lpdataobj->GetData(&tFormat, &stgmed);
				if (SUCCEEDED(hr))
				{
					hdrop = reinterpret_cast<HDROP>(GlobalLock(stgmed.hGlobal));
					::SendMessage(m_pOwner->GetManager()->GetPaintWindow(),TM_DROPFILE_MSG,(WPARAM)hdrop,(LPARAM)bIsCopyFile);
					GlobalUnlock(stgmed.hGlobal);
					//lpdataobj->Release();
					return E_ABORT;
				}
			}
			bIsCopyFile = true;
			bCopyOther = true;
		}
	case RECO_DROP: 
		{


			//拖拽文件操作

			STGMEDIUM stgmed; 
			ZeroMemory(&stgmed, sizeof(STGMEDIUM));
			FORMATETC tFormat={ 0xC007/*FileNameW*/, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };  

			HRESULT hr = lpdataobj->GetData(&tFormat, &stgmed);
			if (SUCCEEDED(hr) && !bDragFile)
			{
				//tryDebug0(TEXT("拖拽文件操作"));
				tFormat.cfFormat = CF_HDROP;
				bDragFile = true;
				hr = lpdataobj->GetData(&tFormat, &stgmed);
				if (SUCCEEDED(hr))
				{

					HDROP       hdrop;
					hdrop = reinterpret_cast<HDROP>(GlobalLock(stgmed.hGlobal));
					//::SendMessage(m_pOwner->GetManager()->GetPaintWindow(),TM_DROPFILE_MSG,(WPARAM)hdrop,(LPARAM)bIsCopyFile);
					GlobalUnlock(stgmed.hGlobal);
					//lpdataobj->Release();
					return E_ABORT;
				}
				return S_OK;
			}

			//tryDebug0(TEXT("拖拽文字操作"));
			//tryOutputDebugString(_T("拖拽文字操作") );
			bool bIsDrop = false;
// 			if (g_pDropWindow != NULL&& !bCopyOther)
// 			{		//模拟拖拽行为清除上次拖拽 的文字信息
// 				//tryOutputDebugString(_T("开始拖拽行为") );
// 				//tryDebug0(TEXT("开始拖拽行为"));
// 				if ( g_pDropWindow->m_dropBegin != -1 && g_pDropWindow->m_dropEnd != -1 )
// 				{
// 					CRichEditUI::IRichEditOleExCallback::g_bDrop = false;
// 					bIsDrop = true;
// 					long  dropPos;
// 					POINT point;
// 					GetCursorPos(&point);
// 					ScreenToClient(m_pOwner->GetManager()->GetPaintWindow(), &point);
// 					//tryDebug0(TEXT("拖拽 1\n"));
// 					dropPos = g_pDropWindow->CharFromPos(point);
// 					g_pDropWindow->SetSel(g_pDropWindow->m_dropBegin , g_pDropWindow->m_dropEnd);
// 					if (g_pDropWindow->m_dropBegin < dropPos)
// 					{
// 						dropPos  = dropPos - (g_pDropWindow->m_dropEnd - g_pDropWindow->m_dropBegin);
// 					}
// 					if ( !g_pDropWindow->IsReadOnly())
// 					{
// 						//tryDebug0(_T("开始清除拖拽信息"));
// 						g_pDropWindow->ReplaceSel(TEXT(""), false);
// 					}
// 					g_pDropWindow->m_dropEnd = -1;
// 					g_pDropWindow->m_dropBegin = -1;
// 					g_pDropWindow->SetSel(dropPos, dropPos);
// 					g_pDropWindow = NULL;
// 
// 				}
// 				//tryDebug0( _T("拖拽行为结束") );
// 				//tryOutputDebugString(_T("拖拽行为结束") );
// 			}
	//		g_bDrop = false;
			bool bFlag  = false;
			if (bIsDrop)
			{
				POINT pt;
				RECT rect;
				GetCursorPos(&pt);
				ScreenToClient(m_pOwner->GetManager()->GetPaintWindow(), &pt);
				rect = m_pOwner->GetPos();
				if( ::PtInRect(&rect, pt) )
				{
					//bFlag = m_pOwner->DoClipboard();
				}
				else
					return E_NOINTERFACE;
			}
			else
			{
				//bFlag = m_pOwner->DoClipboard();
			}
			if (bFlag) 
			{
				return E_NOINTERFACE;
			}
			else
			{
				return S_OK;
			}
		} 



	default:  
		break;  
	}  
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::GetClipboardData(CHARRANGE FAR * lpchrg, DWORD reco, LPDATAOBJECT FAR * lplpdataobj)
{
	if (!m_pOwner) return S_FALSE;

 
 	if (reco == RECO_CUT || reco == RECO_DRAG || reco == RECO_DROP)
 	{
 		//m_pOwner->DoWriteClipData(true);
 		//OutputDebugString(TEXT("响应拖拽!\n"));
		return 0;
 	}
 	else  
 	{ 
		return 0;
 	}



	m_pOwner->Invalidate();
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!pdwEffect || !m_pOwner) return S_FALSE;

	*pdwEffect = TRUE;
// 	if (g_bDropSendFileDlg)
// 	{
// 		*pdwEffect = FALSE;
// 		return S_OK;
// 	}
// 	if (!g_bDrop)
// 		{
// 			CDuiString strTemp;
// 			
// 			strTemp.Format(TEXT("Key State!:%d\n"), grfKeyState);
// 			//tryDebug0(strTemp);
// 	
// 			if (!m_pOwner->IsReadOnly() && !grfKeyState)
// 			{
// 				//tryOutputDebugString(strTemp);
// 				m_pOwner->GetSel(m_pOwner->m_dropBegin,m_pOwner->m_dropEnd);
// 			//	g_pDropWindow = m_pOwner;
// 			//	g_bDrop  = true;
// 			}
// 		}
 		return E_NOTIMPL;
	
}

HRESULT STDMETHODCALLTYPE 
	CRichEditUI::IRichEditOleExCallback::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR * lpchrg,
	HMENU FAR * lphmenu)
{
	if (m_pOwner)
	{
		//m_pOwner->GetContextMenuInner(seltyp, lpoleobj, lpchrg);
	}
	return S_OK;
}

#endif
/////////////////////////////////////////////////////////////////////////////////////
//
//

CRichEditUI::CRichEditUI() : m_pTwh(NULL), m_bVScrollBarFixing(false), m_bWantTab(true), m_bWantReturn(true), 
    m_bWantCtrlReturn(true), m_bRich(true), m_bReadOnly(false), m_bWordWrap(false), m_dwTextColor(0), m_iFont(-1), 
    m_iLimitText(cInitTextMax), m_lTwhStyle(ES_MULTILINE), m_bInited(false)
{
	m_dropEnd = -1;
	m_bInit = false;
	m_dropBegin = -1;
}

CRichEditUI::~CRichEditUI()
{
    if( m_pTwh ) {
        m_pTwh->Release();
        m_pManager->RemoveMessageFilter(this);
    }
}

LPCTSTR CRichEditUI::GetClass() const
{
    return _T("RichEditUI");
}

LPVOID CRichEditUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_RICHEDIT) == 0 ) return static_cast<CRichEditUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CRichEditUI::GetControlFlags() const
{
    if( !IsEnabled() ) return CControlUI::GetControlFlags();

    return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

bool CRichEditUI::IsWantTab()
{
    return m_bWantTab;
}

void CRichEditUI::SetWantTab(bool bWantTab)
{
    m_bWantTab = bWantTab;
}

bool CRichEditUI::IsWantReturn()
{
    return m_bWantReturn;
}

void CRichEditUI::SetWantReturn(bool bWantReturn)
{
    m_bWantReturn = bWantReturn;
}

bool CRichEditUI::IsWantCtrlReturn()
{
    return m_bWantCtrlReturn;
}

void CRichEditUI::SetWantCtrlReturn(bool bWantCtrlReturn)
{
    m_bWantCtrlReturn = bWantCtrlReturn;
}

bool CRichEditUI::IsRich()
{
    return m_bRich;
}

void CRichEditUI::SetRich(bool bRich)
{
    m_bRich = bRich;
    if( m_pTwh ) m_pTwh->SetRichTextFlag(bRich);
}

bool CRichEditUI::IsReadOnly()
{
    return m_bReadOnly;
}

void CRichEditUI::SetReadOnly(bool bReadOnly)
{
    m_bReadOnly = bReadOnly;
    if( m_pTwh ) m_pTwh->SetReadOnly(bReadOnly);
}

bool CRichEditUI::GetWordWrap()
{
    return m_bWordWrap;
}

void CRichEditUI::SetWordWrap(bool bWordWrap)
{
    m_bWordWrap = bWordWrap;
    if( m_pTwh ) m_pTwh->SetWordWrap(bWordWrap);
}

int CRichEditUI::GetFont()
{
    return m_iFont;
}

void CRichEditUI::SetFont(int index)
{
    m_iFont = index;
    if( m_pTwh ) {
        m_pTwh->SetFont(GetManager()->GetFont(m_iFont));
    }
}

void CRichEditUI::SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
    if( m_pTwh ) {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        _tcsncpy(lf.lfFaceName, pStrFontName, LF_FACESIZE);
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -nSize;
        if( bBold ) lf.lfWeight += FW_BOLD;
        if( bUnderline ) lf.lfUnderline = TRUE;
        if( bItalic ) lf.lfItalic = TRUE;
        HFONT hFont = ::CreateFontIndirect(&lf);
        if( hFont == NULL ) return;
        m_pTwh->SetFont(hFont);
        ::DeleteObject(hFont);
    }
}

LONG CRichEditUI::GetWinStyle()
{
    return m_lTwhStyle;
}

void CRichEditUI::SetWinStyle(LONG lStyle)
{
    m_lTwhStyle = lStyle;
}

DWORD CRichEditUI::GetTextColor()
{
    return m_dwTextColor;
}

void CRichEditUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
    if( m_pTwh ) {
        m_pTwh->SetColor(dwTextColor);
    }
}

int CRichEditUI::GetLimitText()
{
    return m_iLimitText;
}

void CRichEditUI::SetLimitText(int iChars)
{
    m_iLimitText = iChars;
    if( m_pTwh ) {
        m_pTwh->LimitText(m_iLimitText);
    }
}

long CRichEditUI::GetTextLength(DWORD dwFlags) const
{
    GETTEXTLENGTHEX textLenEx;
    textLenEx.flags = dwFlags;
#ifdef _UNICODE
    textLenEx.codepage = 1200;
#else
    textLenEx.codepage = CP_ACP;
#endif
    LRESULT lResult;
    TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
    return (long)lResult;
}

CDuiString CRichEditUI::GetText() const
{
    long lLen = GetTextLength(GTL_DEFAULT);
    LPTSTR lpText = NULL;
    GETTEXTEX gt;
    gt.flags = GT_DEFAULT;
#ifdef _UNICODE
    gt.cb = sizeof(TCHAR) * (lLen + 1) ;
    gt.codepage = 1200;
    lpText = new TCHAR[lLen + 1];
    ::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
    gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
    gt.codepage = CP_ACP;
    lpText = new TCHAR[lLen * 2 + 1];
    ::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
    gt.lpDefaultChar = NULL;
    gt.lpUsedDefChar = NULL;
    TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
    CDuiString sText(lpText);
    delete[] lpText;
    return sText;
}

void CRichEditUI::SetText(LPCTSTR pstrText)
{
    m_sText = pstrText;
    if( !m_pTwh ) return;
    SetSel(0, -1);
    ReplaceSel(pstrText, FALSE);
}

bool CRichEditUI::GetModify() const
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

void CRichEditUI::SetModify(bool bModified) const
{ 
    TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
}

void CRichEditUI::GetSel(CHARRANGE &cr) const
{ 
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
}

void CRichEditUI::GetSel(long& nStartChar, long& nEndChar) const
{
    CHARRANGE cr;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
    nStartChar = cr.cpMin;
    nEndChar = cr.cpMax;
}

int CRichEditUI::SetSel(CHARRANGE &cr)
{ 
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

int CRichEditUI::SetSel(long nStartChar, long nEndChar)
{
    CHARRANGE cr;
    cr.cpMin = nStartChar;
    cr.cpMax = nEndChar;
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

void CRichEditUI::ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo)
{
#ifdef _UNICODE		
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
#else
    int iLen = _tcslen(lpszNewText);
    LPWSTR lpText = new WCHAR[iLen + 1];
    ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
    ::MultiByteToWideChar(CP_ACP, 0, lpszNewText, -1, (LPWSTR)lpText, iLen) ;
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpText, 0); 
    delete[] lpText;
#endif
}

void CRichEditUI::ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo)
{
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
}

CDuiString CRichEditUI::GetSelText() const
{
    if( !m_pTwh ) return CDuiString();
    CHARRANGE cr;
    cr.cpMin = cr.cpMax = 0;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
    LPWSTR lpText = NULL;
    lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
    ::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
    TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CRichEditUI::SetSelAll()
{
    return SetSel(0, -1);
}

int CRichEditUI::SetSelNone()
{
    return SetSel(-1, 0);
}

bool CRichEditUI::GetZoom(int& nNum, int& nDen) const
{
    LRESULT lResult;
    TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetZoom(int nNum, int nDen)
{
    if (nNum < 0 || nNum > 64) return false;
    if (nDen < 0 || nDen > 64) return false;
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetZoomOff()
{
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

WORD CRichEditUI::GetSelectionType() const
{
    LRESULT lResult;
    TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
    return (WORD)lResult;
}

bool CRichEditUI::GetAutoURLDetect() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetAutoURLDetect(bool bAutoDetect)
{
    LRESULT lResult;
    TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
    return (BOOL)lResult == FALSE;
}

DWORD CRichEditUI::GetEventMask() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
    return (DWORD)lResult;
}

DWORD CRichEditUI::SetEventMask(DWORD dwEventMask)
{
    LRESULT lResult;
    TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
    return (DWORD)lResult;
}

CDuiString CRichEditUI::GetTextRange(long nStartChar, long nEndChar) const
{
    TEXTRANGEW tr = { 0 };
    tr.chrg.cpMin = nStartChar;
    tr.chrg.cpMax = nEndChar;
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nEndChar - nStartChar + 1];
    ::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
    tr.lpstrText = lpText;
    TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

void CRichEditUI::HideSelection(bool bHide, bool bChangeStyle)
{
    TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
}

void CRichEditUI::ScrollCaret()
{
    TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
}

int CRichEditUI::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

int CRichEditUI::AppendText(LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(-1, -1);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

DWORD CRichEditUI::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetDefaultCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        CHARFORMAT2W cfw;
        cfw.cbSize = sizeof(CHARFORMAT2W);
        TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
        m_pTwh->SetCharFormat(cfw);
        return true;
    }
    return false;
}

DWORD CRichEditUI::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetWordCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION|SCF_WORD, (LPARAM)&cf, &lResult); 
    return (BOOL)lResult == TRUE;
}

DWORD CRichEditUI::GetParaFormat(PARAFORMAT2 &pf) const
{
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetParaFormat(PARAFORMAT2 &pf)
{
    if( !m_pTwh ) return false;
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        m_pTwh->SetParaFormat(pf);
        return true;
    }
    return false;
}

bool CRichEditUI::Redo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_REDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

bool CRichEditUI::Undo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_UNDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

void CRichEditUI::Clear()
{ 
    TxSendMessage(WM_CLEAR, 0, 0, 0); 
}

void CRichEditUI::Copy()
{ 
    TxSendMessage(WM_COPY, 0, 0, 0); 
}

void CRichEditUI::Cut()
{ 
    TxSendMessage(WM_CUT, 0, 0, 0); 
}

void CRichEditUI::Paste()
{ 
    TxSendMessage(WM_PASTE, 0, 0, 0); 
}

int CRichEditUI::GetLineCount() const
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
    return (int)lResult; 
}

CDuiString CRichEditUI::GetLine(int nIndex, int nMaxLength) const
{
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nMaxLength + 1];
    ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
    *(LPWORD)lpText = (WORD)nMaxLength;
    TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CRichEditUI::LineIndex(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
    return (int)lResult;
}

int CRichEditUI::LineLength(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
    return (int)lResult;
}

bool CRichEditUI::LineScroll(int nLines, int nChars)
{
    LRESULT lResult;
    TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
    return (BOOL)lResult == TRUE;
}

CPoint CRichEditUI::GetCharPos(long lChar) const
{ 
    CPoint pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar, 0); 
    return pt;
}

long CRichEditUI::LineFromChar(long nIndex) const
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
    return (long)lResult;
}

CPoint CRichEditUI::PosFromChar(UINT nChar) const
{ 
    POINTL pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0); 
    return CPoint(pt.x, pt.y); 
}

int CRichEditUI::CharFromPos(CPoint pt) const
{ 
    POINTL ptl = {pt.x, pt.y}; 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
    return (int)lResult; 
}

void CRichEditUI::EmptyUndoBuffer()
{ 
    TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0); 
}

UINT CRichEditUI::SetUndoLimit(UINT nLimit)
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
    return (UINT)lResult; 
}

long CRichEditUI::StreamIn(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult;
}

long CRichEditUI::StreamOut(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult; 
}

void CRichEditUI::DoInit()
{
	if(m_bInited)
		return ;

    CREATESTRUCT cs;
    cs.style = m_lTwhStyle;
    cs.x = 0;
    cs.y = 0;
    cs.cy = 0;
    cs.cx = 0;
    cs.lpszName = m_sText.GetData();
    CreateHost(this, &cs, &m_pTwh);
    if( m_pTwh ) {
        m_pTwh->SetTransparent(TRUE);
        LRESULT lResult;
        m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
        m_pTwh->OnTxInPlaceActivate(NULL);
        m_pManager->AddMessageFilter(this);
    }

	m_pIRichEditOleCallback = new IRichEditOleExCallback(this);
	SetOLECallback(m_pIRichEditOleCallback);
	m_pIRichEditOleCallback->AddRef();
	m_bInited= true;
}

HRESULT CRichEditUI::TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const
{
    if( m_pTwh ) {
        if( msg == WM_KEYDOWN && TCHAR(wparam) == VK_RETURN ) {
            if( !m_bWantReturn || (::GetKeyState(VK_CONTROL) < 0 && !m_bWantCtrlReturn) ) {
                if( m_pManager != NULL ) m_pManager->SendNotify((CControlUI*)this, DUI_MSGTYPE_RETURN);
                return S_OK;
            }
        }
        return m_pTwh->GetTextServices()->TxSendMessage(msg, wparam, lparam, plresult);
    }
    return S_FALSE;
}

IDropTarget* CRichEditUI::GetTxDropTarget()
{
    IDropTarget *pdt = NULL;
    if( m_pTwh->GetTextServices()->TxGetDropTarget(&pdt) == NOERROR ) return pdt;
    return NULL;
}

bool CRichEditUI::OnTxViewChanged()
{
    return true;
}

bool CRichEditUI::SetDropAcceptFile(bool bAccept) 
{
	LRESULT lResult;
	TxSendMessage(EM_SETEVENTMASK, 0,ENM_DROPFILES|ENM_LINK, // ENM_CHANGE| ENM_CORRECTTEXT | ENM_DRAGDROPDONE | ENM_DROPFILES | ENM_IMECHANGE | ENM_LINK | ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_REQUESTRESIZE | ENM_SCROLL | ENM_SELCHANGE | ENM_UPDATE,
		&lResult);
	return (BOOL)lResult == FALSE;
}

void CRichEditUI::OnTxNotify(DWORD iNotify, void *pv)
{
	switch(iNotify)
	{ 
	case EN_DROPFILES:   
	case EN_MSGFILTER:   
	case EN_OLEOPFAILED:   
	case EN_PROTECTED:   
	case EN_SAVECLIPBOARD:   
	case EN_SELCHANGE:   
	case EN_STOPNOUNDO:   
	case EN_LINK:   
		//add by zhaohaibo 2014-0407
		{
			if (m_pLinkNotifyCallback)
			{
				//if (iNotify == EN_LINK)
				//{
					ENLINK* pHeader = (ENLINK*)pv;
					if (pHeader->msg == WM_LBUTTONUP)
					{
						CDuiString sString = GetLinkText(pHeader->chrg.cpMin, pHeader->chrg.cpMax);
						m_pLinkNotifyCallback(m_pLinkNotify, sString, pHeader->chrg.cpMin, pHeader->chrg.cpMax);
					}
				//}
			}
		}
		//end 
	break;
	case EN_OBJECTPOSITIONS:   
	case EN_DRAGDROPDONE:   
		{
			if(pv)                        // Fill out NMHDR portion of pv   
			{   
				LONG nId =  GetWindowLong(this->GetManager()->GetPaintWindow(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = this->GetManager()->GetPaintWindow();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				if(SendMessage(this->GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))   
				{   
					//hr = S_FALSE;   
				}   
			}    
		}
		break;
	}
}

// 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
// 引入iPos就是为了修正这个bug
void CRichEditUI::SetScrollPos(SIZE szPos)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( cy != 0 ) {
        int iPos = 0;
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
            iPos = m_pVerticalScrollBar->GetScrollPos();
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pVerticalScrollBar->GetScrollPos());
        TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
            if( cy > 0 && m_pVerticalScrollBar->GetScrollPos() <= iPos )
                m_pVerticalScrollBar->SetScrollPos(iPos);
        }
    }
    if( cx != 0 ) {
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pHorizontalScrollBar->GetScrollPos());
        TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
    }
}

void CRichEditUI::LineUp()
{
    TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
}

void CRichEditUI::LineDown()
{
    int iPos = 0;
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
        iPos = m_pVerticalScrollBar->GetScrollPos();
    TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        if( m_pVerticalScrollBar->GetScrollPos() <= iPos )
            m_pVerticalScrollBar->SetScrollPos(m_pVerticalScrollBar->GetScrollRange());
    }
}

void CRichEditUI::PageUp()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
}

void CRichEditUI::PageDown()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
}

void CRichEditUI::HomeUp()
{
    TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
}

void CRichEditUI::EndDown()
{
    TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
}

void CRichEditUI::LineLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
}

void CRichEditUI::LineRight()
{
    TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
}

void CRichEditUI::PageLeft()
{
    TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
}

void CRichEditUI::PageRight()
{
    TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
}

void CRichEditUI::HomeLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
}

void CRichEditUI::EndRight()
{
    TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
}

void CRichEditUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
    {
        if( m_pTwh && m_pTwh->DoSetCursor(NULL, &event.ptMouse) ) {
            return;
        }
    }
    if( event.Type == UIEVENT_SETFOCUS ) {
        if( m_pTwh ) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
        }
		m_bFocused = true;
		Invalidate();
		return;
    }
    if( event.Type == UIEVENT_KILLFOCUS )  {
        if( m_pTwh ) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
        }
		m_bFocused = false;
		Invalidate();
		return;
    }
    if( event.Type == UIEVENT_TIMER ) {
        if( m_pTwh ) {
            m_pTwh->GetTextServices()->TxSendMessage(WM_TIMER, event.wParam, event.lParam, 0);
        } 
		// add by zhaohaibo 
		// end
    }
    if( event.Type == UIEVENT_SCROLLWHEEL ) {
        if( (event.wKeyState & MK_CONTROL) != 0  ) {
            return;
        }
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE ) 
    {
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP ) 
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        return;
    }
    if( event.Type > UIEVENT__KEYBEGIN && event.Type < UIEVENT__KEYEND )
    {
        return;
    }
    CContainerUI::DoEvent(event);
}

SIZE CRichEditUI::EstimateSize(SIZE szAvailable)
{
    //return CSize(m_rcItem); // 这种方式在第一次设置大小之后就大小不变了
    return CContainerUI::EstimateSize(szAvailable);
}

void CRichEditUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;
    bool bVScrollBarVisiable = false;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        bVScrollBarVisiable = true;
        rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
    }

    if( m_pTwh ) {
        m_pTwh->SetClientRect(&rc);
        if( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsVisible() || m_bVScrollBarFixing) ) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetManager()->GetPaintDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if( lHeight > rc.bottom - rc.top ) {
                m_pVerticalScrollBar->SetVisible(true);
                m_pVerticalScrollBar->SetScrollPos(0);
                m_bVScrollBarFixing = true;
            }
            else {
                if( m_bVScrollBarFixing ) {
                    m_pVerticalScrollBar->SetVisible(false);
                    m_bVScrollBarFixing = false;
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom};
        m_pVerticalScrollBar->SetPos(rcScrollBarPos);
    }
    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
        m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
    }

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it);
        }
        else {
            pControl->SetPos(rc); // 所有非float子控件放大到整个客户区
        }
    }
}

void CRichEditUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    RECT rcTemp = { 0 };
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

    CRenderClip clip;
    CRenderClip::GenerateClip(hDC, rcTemp, clip);
    CControlUI::DoPaint(hDC, rcPaint);

    if( m_pTwh ) {
        RECT rc;
        m_pTwh->GetControlRect(&rc);
        // Remember wparam is actually the hdc and lparam is the update
        // rect because this message has been preprocessed by the window.
        m_pTwh->GetTextServices()->TxDraw(
            DVASPECT_CONTENT,  		// Draw Aspect
            /*-1*/0,				// Lindex
            NULL,					// Info for drawing optimazation
            NULL,					// target device information
            hDC,			        // Draw device HDC
            NULL, 				   	// Target device HDC
            (RECTL*)&rc,			// Bounding client rectangle
            NULL, 		            // Clipping rectangle for metafiles
            (RECT*)&rcPaint,		// Update rectangle
            NULL, 	   				// Call back function
            NULL,					// Call back parameter
            0);				        // What view of the object
        if( m_bVScrollBarFixing ) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetManager()->GetPaintDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if( lHeight <= rc.bottom - rc.top ) {
                NeedUpdate();
            }
        }
    }

    if( m_items.GetSize() > 0 ) {
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
        else {
            CRenderClip childClip;
            CRenderClip::GenerateClip(hDC, rcTemp, childClip);
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    CRenderClip::UseOldClipBegin(hDC, childClip);
                    pControl->DoPaint(hDC, rcPaint);
                    CRenderClip::UseOldClipEnd(hDC, childClip);
                }
                else {
                    if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
            m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
        }
    }

    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
            m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
        }
    }
}

void CRichEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
    }
    if( _tcscmp(pstrName, _T("autovscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOVSCROLL;
    }
    else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
    }
    if( _tcscmp(pstrName, _T("autohscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOHSCROLL;
    }
    else if( _tcscmp(pstrName, _T("wanttab")) == 0 ) {
        SetWantTab(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantreturn")) == 0 ) {
        SetWantReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantctrlreturn")) == 0 ) {
        SetWantCtrlReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("rich")) == 0 ) {
        SetRich(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("multiline")) == 0 ) {
        if( _tcscmp(pstrValue, _T("false")) == 0 ) m_lTwhStyle &= ~ES_MULTILINE;
    }
    else if( _tcscmp(pstrName, _T("readonly")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) { m_lTwhStyle |= ES_READONLY; m_bReadOnly = true; }
    }
    else if( _tcscmp(pstrName, _T("password")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_PASSWORD;
    }
    else if( _tcscmp(pstrName, _T("align")) == 0 ) {
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
            m_lTwhStyle |= ES_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
            m_lTwhStyle |= ES_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
            m_lTwhStyle |= ES_RIGHT;
        }
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

LRESULT CRichEditUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if( !IsVisible() || !IsEnabled() ) return 0;
    if( !IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) return 0;
    if( uMsg == WM_MOUSEWHEEL && (LOWORD(wParam) & MK_CONTROL) == 0 ) return 0;

    bool bWasHandled = true;
    if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
        if( !m_pTwh->IsCaptured() ) {
            switch (uMsg) {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                {
                    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                    CControlUI* pHover = GetManager()->FindControl(pt);
                    if(pHover != this) {
                        bWasHandled = false;
                        return 0;
                    }
                }
                break;
            }
        }
        // Mouse message only go when captured or inside rect
        DWORD dwHitResult = m_pTwh->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
        if( dwHitResult == HITRESULT_OUTSIDE ) {
            RECT rc;
            m_pTwh->GetControlRect(&rc);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if( uMsg == WM_MOUSEWHEEL ) ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
            if( ::PtInRect(&rc, pt) && !GetManager()->IsCaptured() ) dwHitResult = HITRESULT_HIT;
        }
        if( dwHitResult != HITRESULT_HIT ) return 0;
        if( uMsg == WM_SETCURSOR ) bWasHandled = false;
        else if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDOWN ) {
            SetFocus();
        }
    }
#ifdef _UNICODE
    else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
#else
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR ) {
#endif
        if( !IsFocused() ) return 0;
    }
    else if( uMsg == WM_CONTEXTMENU ) {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
        CControlUI* pHover = GetManager()->FindControl(pt);
        if(pHover != this) {
            bWasHandled = false;
            return 0;
        }
    }
    else
    {
        switch( uMsg ) {
        case WM_HELP:
            bWasHandled = false;
            break;
        default:
            return 0;
        }
    }
    LRESULT lResult = 0;
    HRESULT Hr = TxSendMessage(uMsg, wParam, lParam, &lResult);
    if( Hr == S_OK ) bHandled = bWasHandled;
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR )
        bHandled = bWasHandled;
    else if( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) {
        if( m_pTwh->IsCaptured() ) bHandled = bWasHandled;
    }
    return lResult;
}

//add by zhaohaibo 2014-04007
void CRichEditUI::SetLinkNotify(void* pNotify)
{
	m_pLinkNotify = pNotify;
}

void CRichEditUI::SetLinkNotifyCallback(ON_LINK_NOTIFY_EVENT pCallback)
{
	m_pLinkNotifyCallback = pCallback;
}

CDuiString CRichEditUI::GetLinkText(long lMin, long lMax)
{
	CHARRANGE cr;
	cr.cpMin = lMin;
	cr.cpMax = lMax;
	TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, 0);
	LPWSTR lpText = NULL;
	lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
	::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
	TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
	CDuiString sText;
	sText = (LPCWSTR)lpText;
	delete [] lpText;
	return sText;
}

//获取OLE接口 by zhaohaibo
IRichEditOle* CRichEditUI::GetIRichEditOle()
{
	IRichEditOle *pRichItem = NULL;
	TxSendMessage( EM_GETOLEINTERFACE, 0, (LPARAM)&pRichItem, 0);
	return pRichItem;
}

//为RichEdit 设置回调接口  add  by zhaohaibo
BOOL CRichEditUI::SetOLECallback(IRichEditOleCallback* pCallback)
{
	TxSendMessage(EM_SETOLECALLBACK, 0, (LPARAM)pCallback, 0);
	return TRUE;
}

//向RichEdit中添加非gif图片
bool CRichEditUI::InsertNotGIFImage(LPCTSTR szFilePath, DWORD dwUserData)
{
	if (szFilePath == NULL) return false;

	IRichEditOle *pRichEditOle = GetIRichEditOle();
	if (pRichEditOle)
	{
		CImageDataObject imageDataObject;
		imageDataObject.InsertNotGIFImage( pRichEditOle, szFilePath, dwUserData);
		pRichEditOle->Release();
		pRichEditOle = NULL;
		return true;
	}

	return false;
}


//向RichEidt中添加gif图片
bool CRichEditUI::InsertGIFImage(LPCTSTR szFilePath, DWORD dwUserData)
{
	IRichEditOle *pRichEditOle = GetIRichEditOle();
	if (pRichEditOle && szFilePath != NULL )
	{
		CGifOleObject::InsertGifRichEdit(this, GetManager()->GetPaintWindow(), pRichEditOle, szFilePath, dwUserData);
		pRichEditOle->Release();
		pRichEditOle = NULL;
		return true;
	}
	return false;
}

//by zhaohaibo
ITextServices *  CRichEditUI::GetTextServices()
{
	if (NULL == m_pTwh)
		return NULL;
	return m_pTwh->GetTextServices2();
}
// void CRichEditUI::OnTxNotify(DWORD iNotify, void *pv)
// {
// 	if (m_pLinkNotifyCallback)
// 	{
// 		if (iNotify == EN_LINK)
// 		{
// 			ENLINK* pHeader = (ENLINK*)pv;
// 			if (pHeader->msg == WM_LBUTTONUP)
// 			{
// 				//CDuiString sString = GetLinkText(pHeader->chrg.cpMin, pHeader->chrg.cpMax);
// 				//m_pLinkNotifyCallback(m_pLinkNotify, sString, pHeader->chrg.cpMin, pHeader->chrg.cpMax);
// 			}
// 		}
// 	}
// }

//end the code 


IDragDropEvent* CRichEditUI::getDragEvent()
{
	return this;
}

bool	CRichEditUI::EnableDropDrag( void )
{
	if( GetManager()->EnableDropTarget() )
	{	
		GetManager()->RegisterUI(this);
		return true;
	}
	return false;
}

HRESULT CRichEditUI::DragEnter(IDataObject *pDataObj,DWORD grfKeyState,
	POINTL pt,POINT ptClient,DWORD *pdwEffect)
{
	if ( NULL == pDataObj )  
	{  
		return E_INVALIDARG;  
	} 
	m_vDragDorpFile.clear();
	DuiLib::tString szFilePath;
	bool		 bAccept = false;
	if( false == getDropFilePath(pDataObj,m_vDragDorpFile) )
	{
		*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
		return 0;
	}
	RichEditEvent* pEvenSink = 0;
	if( m_pTwh )
	{
		if( pEvenSink = m_pTwh->getEventSink() )
		{
			bAccept = pEvenSink->OnDragDropOver(szFilePath);
			if( bAccept == false )
			{
				*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
				return 0;
			}
		}
	}
	return 0;
}
HRESULT CRichEditUI::DragOver(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,
	POINT ptClient,DWORD *pdwEffect)
{
	DuiLib::tString szFilePath;
	POINT		ptChar;
	EDIT_CHARPOS	tgCharPos;
	bool		bAccept = false;
// 	if( false == getDropFilePath(pDataObj,szFilePath) )
// 	{
// 		*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
// 		return 0;
// 	}
	tgCharPos.nVal = CharFromPos(ptClient);
	if( tgCharPos.nVal > 0 )
	{
		ptChar = PosFromChar(tgCharPos.wIndex);
		m_pTwh->TxShowCaret(false);
		m_pTwh->TxSetCaretPos(ptChar.x,ptChar.y);
		m_pTwh->TxShowCaret(true);
	}
	RichEditEvent* pEvenSink = 0;
	if( m_pTwh )
	{
		if( pEvenSink = m_pTwh->getEventSink() )
		{
			bAccept = pEvenSink->OnDragDropOver(szFilePath);
			if( bAccept == false )
			{
				*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
				return 0;
			}
		}
	}
	return 0;
}
HRESULT CRichEditUI::DragLeave(IDataObject *pDataObj)
{
	DuiLib::tString szFilePath;
	bool		bAccept = false;
	if( false == getDropFilePath(pDataObj,m_vDragDorpFile) )
	{
		return 0;
	}
	RichEditEvent* pEvenSink = 0;
	if( m_pTwh )
	{
		if( pEvenSink = m_pTwh->getEventSink() )
		{
			bAccept = pEvenSink->OnDropLeave(szFilePath);
			if( bAccept == false )
				return 0;
		}
	}
	return 0;
}
HRESULT CRichEditUI::Drop(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,
	POINT ptClient,DWORD *pdwEffect)
{
	DuiLib::tString szFilePath;
	bool		 bAccept = false;
	EDIT_CHARPOS	tgCharPos;
// 	if( false == getDropFilePath(pDataObj,m_vDragDorpFile) )
// 	{
// 		*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
// 		return 0;
// 	}
	for (int i = 0; i < m_vDragDorpFile.size(); i++)
	{
		tgCharPos.nVal = CharFromPos(ptClient);	
		RichEditEvent* pEvenSink = 0;
		if( m_pTwh )
		{
			if( pEvenSink = m_pTwh->getEventSink() )
			{
				bAccept = pEvenSink->OnDropDown(m_vDragDorpFile[i],tgCharPos.nVal);
				if( bAccept == false )
				{
					*pdwEffect = DROPEFFECT_NONE;	//拒绝接受该文件	
					return 0;
				}
			}
		}
	}
	return 0;
}

void CRichEditUI::RegisterEventSink(RichEditEvent* pEvenSink)
{
	if( m_pTwh )
		m_pTwh->setEventSink(pEvenSink);
}

BOOL CRichEditUI::DoMyDrag(HWND hwnd)
{
	IDropTarget* pdt = NULL;
	m_pTwh->GetTextServices2()->TxGetDropTarget(&pdt);
	::RegisterDragDrop(hwnd, pdt);
	return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------//
# if 0
bool CRichEditUI::DoClipboard()
{
	//UINT format = RegisterClipboardFormat("WIN_CLIP_DATA");
	//tryTrace0(_T("粘贴版本:"));
	if(!OpenClipboard(GetManager()->GetPaintWindow()))
	{
		return false;
	}
	DWORD   dwError;

	if (m_typeOfRichEdit == MSN_EDIT&& !EnumClipboardFormats(CF_TEXT))
	{
		CloseClipboard();
		::SendMessage(GetManager()->GetPaintWindow(), MSG_TRY_PASTE_IMG, NULL, NULL);
		return true;
	}

	if ( !m_bCanRecvPastePictrue && !EnumClipboardFormats(CF_TEXT))
	{
		CloseClipboard();
		return true;
	}

	if (EnumClipboardFormats(m_uOwnOleClipboardFormat) && m_bCanRecvPastePictrue)
	{//处理Win 粘贴格式
		//tryTrace0(_T("处理Win 粘贴格式"));
		CString data;
		HANDLE hData =GetClipboardData(m_uOwnOleClipboardFormat);
		if (hData)
		{
			char* buffer = (char*)GlobalLock(hData);
			std::string stdBuff = buffer;
			tString tsBuff = Utf8ToTString(stdBuff);
			data = tsBuff.c_str();
			CDuiString strClipText;
			std::list<IMG_CLIP_MSG *> list;
			ParseClipMsg(strClipText,list, data);
			InsertClipMsg(strClipText,list);
			GlobalUnlock(hData);
			ClearClipList(list);
			::CloseClipboard();

			long lBegin = 0, lEnd = 0;
			GetSel(lBegin, lEnd);
			SetSel(0, -1);
			SetSelectionCharFormat(m_cf);
			SetSel(lBegin, lEnd);
			::RedrawWindow(GetManager()->GetPaintWindow(), NULL, NULL,RDW_INVALIDATE);
			return true;
		}
	}


	UINT cfid = RegisterClipboardFormat( _T("HTML Format")); 
	UINT nRet = EnumClipboardFormats(cfid);
	if (nRet&& m_bCanRecvPastePictrue)
	{
		//处理HTML的粘贴格式
		//tryTrace0(_T("处理HTML的粘贴格式"));
		HANDLE hData =GetClipboardData(cfid);
		if (hData)
		{
			char* buffer = (char*)GlobalLock(hData);
			if( S_OK == InsertHtmlClipData( buffer) )
			{
				::CloseClipboard();
				long lBegin = 0, lEnd = 0;
				GetSel(lBegin, lEnd);
				SetSel(0, -1);
				SetSelectionCharFormat(m_cf);
				SetSel(lBegin, lEnd);

				return true;
			}
			//OutputDebugString(buffer);
		}
	}

	nRet = EnumClipboardFormats(CF_BITMAP);
	if (nRet&& m_bCanRecvPastePictrue)
	{
		//处理HBITMAP的粘贴格式
		//tryTrace0(_T("处理HBITMAP的粘贴格式"));
		HANDLE hBitmap = GetClipboardData(CF_BITMAP); 
		if ( hBitmap != NULL )
		{
			HBITMAP bitmap = (HBITMAP)CopyImage(hBitmap,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION ); 
			DWORD dwError  = GetLastError();
			if ( bitmap == NULL)
			{

			}
			if (bitmap)
			{
				InsertNotGifImage(bitmap, PICTURE_OTHER, USERPIC);
				DeleteObject(bitmap);
				::CloseClipboard();
				return true;
			}
			DeleteObject(bitmap);
		}
	}

	if (IsClipboardFormatAvailable(CF_METAFILEPICT) && nRet && m_bCanRecvPastePictrue )
	{

		//tryTrace0(_T("处理CF_METAFILEPICT的粘贴格式"));
		GLOBALHANDLE    hGMem ;
		LPMETAFILEPICT    lpMFP ;
		hGMem = GetClipboardData(CF_METAFILEPICT) ;
		lpMFP = (LPMETAFILEPICT)GlobalLock(hGMem) ;  

		HANDLE   hClip=::GetClipboardData(CF_ENHMETAFILE);
		HENHMETAFILE hEnhMetaFile;
		hEnhMetaFile=(HENHMETAFILE)hClip; 		 
		dwError=GetLastError();   //   dwError=6，无效的句柄。   
		CTime time = CTime::GetCurrentTime();
		CString str = _T("d:\\aaa.emf");
		//str.Format("d:\\aaa.emf", time.Format("%H%M%S"));		

		HENHMETAFILE   hMetaFile=CopyEnhMetaFile(hEnhMetaFile, str);//保存到文件 

		//关闭CMetafileDC并获得它的句柄


		DeleteEnhMetaFile(hMetaFile);
		dwError=GetLastError();   //   dwError=6，无效的句柄。 
		HDC dc = GetDC(GetManager()->GetPaintWindow());
		// 		CClientDC dc(this);
		// 		CDuiRect client(0,0,600,400);
		// 		dc.PlayMetaFile(hEnhMetaFile,client);
		PlayMetaFile( dc,lpMFP->hMF) ;
		GUID guid;
		::CoCreateGuid(&guid);
		TCHAR* szGuid = new TCHAR[128];//delete [] sz
		memset(szGuid, 0, 128);
		_stprintf( szGuid, TEXT("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"),
			guid.Data1,  guid.Data2,   guid.Data3 ,
			guid.Data4[0],   guid.Data4[1],
			guid.Data4[2],   guid.Data4[3],
			guid.Data4[4],   guid.Data4[5],
			guid.Data4[6],   guid.Data4[7] );

		CString file_name;
		file_name = GetMyImageDir() + szGuid + _T(".jpg");
		ConvertEMFToBMP(str, file_name);
		//InsertNotGIFImage(file_name.GetBuffer(), PICTURE_OTHER, CUTPIC);
		ReleaseDC( GetManager()->GetPaintWindow(),dc);
		GlobalUnlock(hGMem) ;
		delete[] szGuid;
		return true;
	}

	nRet = EnumClipboardFormats(CF_UNICODETEXT);
	if (nRet)
	{
		//tryTrace0(_T("处理CF_UNICODETEXT的粘贴格式"));
		HANDLE hData = GetClipboardData( CF_UNICODETEXT );
		TCHAR * buffer = (TCHAR*)GlobalLock( hData );
		DWORD dwMaxLenght;
		CString strBuff = buffer;
		long lBegin = 0, lEnd = 0;

		m_pTwh->TxGetMaxLength(&dwMaxLenght);
		ReplaceSel(_T(""), true);
		GetSel(lBegin, lEnd);
		SetSel( 0, -1);
		CString strWidnowText = GetSelText().GetData();
		SetSel(lBegin,lEnd);
		DWORD dwCurLenght = GetTextCount( strWidnowText );
		DWORD dwDrawCount = dwMaxLenght - dwCurLenght;
		CString strTemp  = strBuff;
		if (dwDrawCount > 0)
		{

			strTemp = GetTextByCount(strTemp, dwDrawCount);//复制的长度 - 当前长度 + 选中的长度
			ReplaceSel(strTemp, true);
		}
		GlobalUnlock( hData );
		::CloseClipboard();

		GetSel(lBegin, lEnd);
		SetSel(0, -1);
		SetSelectionCharFormat(m_cf);
		SetSel(lBegin, lEnd);
		return true;
	}

	nRet = EnumClipboardFormats(CF_TEXT);
	if (nRet)
	{

		//tryTrace0(_T("处理CF_TEXT的粘贴格式"));
		HANDLE hData = GetClipboardData( CF_TEXT );
		char * buffer = (char*)GlobalLock( hData );
		DWORD dwMaxLenght;
		m_pTwh->TxGetMaxLength(&dwMaxLenght);
		CString strBuff = StringToTString(buffer).c_str();
		long lBegin = 0,lEnd = 0;
		ReplaceSel(_T(""), true);
		GetSel(lBegin, lEnd);
		SetSel( 0, -1);
		CString strWidnowText = GetSelText().GetData();
		SetSel(lBegin,lEnd);
		DWORD dwCurLenght = GetTextCount( strWidnowText );
		DWORD dwDrawCount = dwMaxLenght - dwCurLenght;
		CString strTemp  = strBuff;
		if (dwDrawCount > 0)
		{

			strTemp = GetTextByCount(strTemp, dwDrawCount);//复制的长度 - 当前长度 + 选中的长度
			ReplaceSel(strTemp, true);
		}
		GlobalUnlock( hData );
		::CloseClipboard();

		GetSel(lBegin, lEnd);
		SetSel(0, -1);
		SetSelectionCharFormat(m_cf);
		SetSel(lBegin, lEnd);
		return true;
	}

	::CloseClipboard();
	return false;
}

bool CRichEditUI::DoWriteClipData( HINSTANCE hIstance,HWND hWnd,::CHAT_IMAGE& chat,CDuiRect rect)
{
	bool bRet = false;
	do 
	{
		if ( !hWnd || !hIstance)
		{
			break;
		}
		if( OpenClipboard(hWnd))
		{
			EmptyClipboard();

			//粘贴格式：Win 粘贴格式
			CDuiString strClipWord = TEXT("<message>");
			CString strBodyMsg;
			strBodyMsg.Format(TEXT("<body>%s</body>"), chat.strChatMsg );
			std::list<img_info>::iterator it = chat.img.begin();
			for (;it != chat.img.end(); it++)
			{
				CString strItemMsg;
				if ( it->strFileType == SYSPIC || USERPIC == it->strFileType || CUTPIC == it->strFileType)
				{
					strItemMsg.Format(TEXT("<item><id>%d</id><name>%s</name><pos>%d</pos><type>%d</type></item>"),
						0,
						it->strFilePath,
						it->strFilePos,
						USERPIC
						);
				}
				else if(it->strFileType != SYSFACE)
				{
					strItemMsg.Format(TEXT("<item><id>%d</id><name>%s</name><pos>%d</pos><type>%d</type></item>"),
						0,
						it->strFilePath,
						it->strFilePos,
						USERGIF
						);
				}
				else
				{
					strItemMsg.Format(TEXT("<item><id>%d</id><name>%s</name><pos>%d</pos><type>%d</type></item>"),
						0,
						it->strFilePath,
						it->strFilePos,
						it->strFileType
						);
				}

				strClipWord += strItemMsg;
			}
			strClipWord += strBodyMsg + TEXT("</message>");
			std::string stdClipWord = TStringToUtf8(strClipWord.GetData());
			HGLOBAL clipbuffer;
			clipbuffer = GlobalAlloc(GHND | GMEM_SHARE, stdClipWord.size()+4);
			char * buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, stdClipWord.c_str() );
			GlobalUnlock(clipbuffer);
			SetClipboardData(m_uOwnOleClipboardFormat,clipbuffer);

			//粘贴格式：HBITMAP
			if ( ( _tcslen( chat.strChatMsg) == 0 ) && ( chat.img.size() == 1 ) )
			{
				it = chat.img.begin();
				DoWriteHBitmapClip( hIstance,hWnd,it->strFilePath, rect );
				CloseClipboard();
				bRet = true;
				break;
			}
			else
			{
				if (chat.img.size() > 0)
				{
					CRichEditUI::DoWriteHTMLWord(chat);
				}
			}

			stdClipWord = TStringToString(chat.strChatMsg);
			HGLOBAL hClip;
			char * TextBuffer = NULL;
			hClip = GlobalAlloc( GMEM_MOVEABLE,stdClipWord.size() + 4 );
			TextBuffer = (char*)GlobalLock(hClip);
			strcpy( TextBuffer,  stdClipWord.c_str());
			GlobalUnlock(hClip);
			SetClipboardData(CF_TEXT, hClip);

			::CloseClipboard();
		}

	} while (0);

	return true;
}

bool CRichEditUI::DoWriteClipData(bool bIsCut)
{
	if (m_typeOfRichEdit == MSN_EDIT)
	{
		::SendMessage(GetManager()->GetPaintWindow(),MSG_OLE_COPY, (WPARAM)this, NULL );
		return true;
	}

	if( OpenClipboard(GetManager()->GetPaintWindow()) )
	{
		EmptyClipboard();
		CDuiString data,strSelText;
		std::list<IMG_CLIP_MSG *> listOfImg;
		long lBegin = 0, lEnd = 0;
		GetSel(lBegin, lEnd);

		if( !GetSelClipData(strSelText, listOfImg) ) 
		{
			CloseClipboard();
			return false;
		}

		CDuiString strText = GetSelText();
		tString tsCopyText;
		std::string stdCopyText;
		strSelText = strText;
		//tryDebug(_T("选中的 文字信息为：%s"),strText.GetData());
		GetClipWord(strSelText,listOfImg, data);
		if (bIsCut)
		{
			ReplaceSel(_T(""), false);
		}
		tsCopyText = data.GetData();
		stdCopyText = TStringToUtf8(tsCopyText);
		HGLOBAL clipbuffer;
		clipbuffer = GlobalAlloc(GHND | GMEM_SHARE, (stdCopyText.size()+1)*sizeof(TCHAR));
		char * buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer,stdCopyText.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(m_uOwnOleClipboardFormat,clipbuffer);


		if ( ( strText.GetLength() == 1 ) && ( listOfImg.size() == 1 ) )
		{
			WriteHBitmapClip(listOfImg);
			ClearClipList(listOfImg);

			CloseClipboard();
			return TRUE; 
		}
		if( listOfImg.size() > 0)
		{
			WriteHTMLClip(strSelText,listOfImg);
		}


		tsCopyText = strText.GetData();
		stdCopyText = TStringToString(tsCopyText);
		//ConvertUtf8ToGBK( str);
		HGLOBAL hClip;
		char * TextBuffer = NULL;
		hClip = GlobalAlloc( GMEM_MOVEABLE, (stdCopyText.size() + 1) );
		TextBuffer = (char*)GlobalLock(hClip);
		strcpy( TextBuffer, stdCopyText.c_str());
		GlobalUnlock(hClip);

		SetClipboardData(CF_TEXT, hClip);

		ClearClipList(listOfImg);

		CloseClipboard();

	}
	return TRUE;
}

bool CRichEditUI::DoReplace()
{
	long lBegin = 0, lEnd = 0;
	//保存现场
	GetSel(lBegin, lEnd);
	//获取负责数据
	SetSel(0, -1 );
	DoWriteClipData(true);
	//粘贴
	DoClipboard();
	//还原现场
	SetSel(lBegin, lEnd);
	//清空粘贴板信息
	if( OpenClipboard(GetManager()->GetPaintWindow()) )
	{
		EmptyClipboard();
		CloseClipboard();
	}
	return true;
}


void CRichEditUI::DoWriteHBitmapClip( HINSTANCE hInstance ,HWND hWnd, CDuiString strImagePath, CDuiRect Rect)
{
	USES_CONVERSION;
	HDC	dc = GetDC(hWnd);
	//内存设备文本对象
	HDC 		memDC;
	HBITMAP hBitmap = (HBITMAP)::LoadImage( hInstance, strImagePath.GetData(),IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE |LR_SHARED
		);

	if(hBitmap == NULL)
	{
		CLSID  encoderClsid;
#ifdef _UNICODE
		Image* pImage = Gdiplus::Image::FromFile(strImagePath.GetData());
#else 
		Image* pImage = Gdiplus::Image::FromFile(T2W(strImagePath.GetData()));
#endif
		Color color(RGB(255,255,255));
		if (-1 != GetEncoderPigClsid(L"image/jpeg", &encoderClsid))
		{

			pImage->Save(L"D:\\Pigtemp.png", &encoderClsid);
		}
		hBitmap = (HBITMAP)::LoadImage(hInstance, _T("D:\\Pigtemp.png"),IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE |LR_SHARED
			);
		delete pImage;
	}

	SelectObject(dc,hBitmap);
	//初始化空白位图
	//bitmap.CreateCompatibleBitmap(dc, Rect.Width(),Rect.Height() );
	//bitmap.Attach(hBitmap);
	//创建内存设备文本对象
	memDC = CreateCompatibleDC(dc); 

	//将位图选入内存设备文本对象
	HBITMAP pOldBitmap = (HBITMAP)SelectObject( memDC, &hBitmap);
	//将客户区中的位图绘制到内存位图对象中
	::SetBkColor(memDC, RGB(255,255,255));
	BitBlt( memDC,0,0, Rect.GetWidth(),Rect.GetHeight(), dc, Rect.left,Rect.top, SRCCOPY); 
	//打开剪贴板

	//拷贝位图到剪贴板
	if( NULL == SetClipboardData(CF_BITMAP, hBitmap ))
	{
		DWORD dwError =  GetLastError();
		CDuiString strTemp;
		strTemp.Format(_T("设置粘贴板 格式 失败:%d!\n"), dwError);
		OutputDebugString(strTemp);

	}

	//关闭剪贴板
	//恢复默认位图对象
	SelectObject( memDC,pOldBitmap);
	//分离位图
	//bitmap.Detach();
	//重绘视图区
	DeleteDC(memDC);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, dc);
}

void CRichEditUI::DoWriteHTMLWord( ::CHAT_IMAGE& chat)
{
	TCHAR* szInsertBuff;
	int nLen = CHATIMG2char( chat,&szInsertBuff);
	TCHAR *buf = new TCHAR[512 + nLen]; 
	if(!buf)
	{
		delete[] szInsertBuff;
		return; 
	}
	static int cfid = 0; 
	if(!cfid) cfid = RegisterClipboardFormat( _T("HTML Format")); 
	// Create a template string for the HTML header... 
	_tcscpy(buf, 
		_T("Version:0.9\r\n") 
		_T("StartHTML:00000000\r\n" )
		_T("EndHTML:00000000\r\n") 
		_T("StartFragment:00000000\r\n") 
		_T("EndFragment:00000000\r\n") 
		_T("<html><body>\r\n") 
		_T("<!--StartFragment-->\r\n")); 
	// Append the HTML... 
	_tcscat(buf, _T("<DIV>\r\n"));
	_tcscat( buf, szInsertBuff ); 
	_tcscat(buf, _T("</DIV>\r\n"));

	_tcscat( buf, _T("\r\n") ); 
	// Finish up the HTML format... 
	_tcscat(buf, 
		_T("<!--EndFragment-->\r\n" )
		_T("</body>\r\n" )
		_T("</html>")); 
	// Now go back, calculate all the lengths, and write out the 
	// necessary header information. Note, wsprintf() truncates the 
	// string when you overwrite it so you follow up with code to replace 
	// the 0 appended at the end with a '\r'... 
	TCHAR *pTemp = _tcsstr( buf, _T("StartHTML" )); 
	_stprintf( pTemp + 10, _T("%08u"), _tcsstr(buf,_T("<html>")) - buf ); 
	*( pTemp + 10 + 8 ) = '\r'; 
	pTemp = _tcsstr( buf, _T("EndHTML") ); 
	_stprintf( pTemp + 8, _T("%08u"), _tcslen(buf) ); 
	*( pTemp + 8 + 8 ) = '\r'; 
	pTemp = _tcsstr(buf,_T("StartFragment")); 
	_stprintf( pTemp + 14, _T("%08u"), _tcsstr( buf, _T("<!--StartFrag")) - buf ); 
	*( pTemp + 14 + 8 ) = '\r'; 
	pTemp = _tcsstr( buf, _T("EndFragment") ); 
	_stprintf( pTemp + 12, _T("%08u"), _tcsstr( buf, _T("<!--EndFrag") ) - buf ); 
	*( pTemp + 12 + 8 ) = '\r'; 
	int l = _tcslen(buf);
	// 	char* utf8 = NULL;
	// 	int c = ConvertGBKToUtf8( buf, 1024, &utf8);
	// 	if (c == -1)
	// 	{
	// 		delete[] buf; 
	// 		delete[] szInsertBuff;
	// 		return;
	// 	}

	std::string stdHtml = TStringToUtf8(buf);
	//   Allocate   global   memory   for   transfer... 
	HGLOBAL hHtml = GlobalAlloc( GMEM_MOVEABLE|GMEM_DDESHARE, stdHtml.size() +4 ); 

	//   Put   your   string   in   the   global   memory... 
	char *ptr = (char*)GlobalLock(hHtml); 
	if (buf != NULL )
	{
		strcpy(ptr, stdHtml.c_str() );

	}
	//delete p;
	GlobalUnlock(hHtml); 
	::SetClipboardData(cfid, hHtml);
	delete[] buf; 
	delete[] szInsertBuff;
}

int CRichEditUI::CHATIMG2char(::CHAT_IMAGE& chat,TCHAR** szBuff)
{
	if (szBuff == NULL)
	{
		return 0;
	}
	CString strWord = chat.strChatMsg;
	std::list<img_info> list = chat.img;
	int beginWord = 0;
	CString strHtmlWord;
	CString strLastWord;
	std::list<img_info>::iterator it = list.begin();
	for (; it != list.end(); it++)
	{
		CString strTemp = strWord.Left( it->strFilePos );
		strLastWord = strWord.Right(strWord.GetLength() - it->strFilePos);
		int nCount = FindAndReplace(strTemp);
		beginWord = it->strFilePos;
		if (nCount > 0)
		{
			std::list<img_info>::iterator it_f = it;
			for (; it_f != list.end(); it_f++)
			{
				it_f->strFilePos += nCount;
			}
		}
		else
			break;
		strWord = strTemp + strLastWord;
		strHtmlWord += strTemp;
	}


	int nBegin = 0;
	int nIndex = 0;

	USES_CONVERSION;
	it = list.begin();
	for (;it != list.end(); it++)
	{
		int nPos = it->strFilePos;
		strWord.Delete(nPos, 1);
		CString strTemp;
		strTemp.Format(_T("<IMG SRC=\"file:///%s\">"), it->strFilePath);
		strWord.Insert(nPos, strTemp);
		std::list<img_info>::iterator it_f = it;
		it_f++;
		for(; it_f != list.end(); it_f++ )
		{
			it_f->strFilePos += strTemp.GetLength() - 1;
		}
	}

	*szBuff = new TCHAR[strWord.GetLength()  + 1];
	strWord.ReleaseBuffer();
	ZeroMemory(*szBuff, sizeof(strWord.GetLength() + 1));
	_tcsncpy_s(*szBuff, strWord.GetLength()  + 1, strWord.GetBuffer(), _TRUNCATE);
	strWord.ReleaseBuffer();

	return strWord.GetLength() + 1;
}


int CRichEditUI::CHATIMG2char(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list,TCHAR** szBuff)
{
	if (szBuff == NULL)
	{
		return 0;
	}
	std::list<int> listOfIndex;

	CStringW strTemp, strBuffer, strChat;
	CStringW Temp= strWord.GetData();
#ifndef _UNICODE
	GetWordCount(strWord, list, listOfIndex);

	while (1)
	{
		int nPos = Temp.Find('\r');
		if (nPos == -1)
		{
			break;
		}		
		std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
		CStringW szLeft = strWord.Left((*it)->nPos);
		OutputDebugString(_T("输出字符串:"));
		OutputDebugStringW(szLeft);
		OutputDebugString(_T("\n"));
		szLeft.Format(L"第一个图片的位置:%d, 找到 回车的位置:%d\n", (*it)->nPos, nPos);
		OutputDebugStringW(szLeft);
		for (;it != list.end(); it++)
		{
			if ((*it)->nPos+1 >= nPos)
			{
				(*it)->nPos += 1;
			}
		}
		Temp.Delete( nPos, 1);
		Temp.Insert(nPos,L"");
	}
	Temp= strWord.GetData();

	std::list<int >::iterator it_index = listOfIndex.begin();
	std::list<IMG_CLIP_MSG *>::iterator it_begin  = list.begin();
	for (;it_begin != list.end() && it_index != listOfIndex.end(); it_begin++, it_index++)
	{
		(*it_begin)->nPos -= *it_index;
	}
	OutputDebugString(_T("=======================我是分割线==================\n"));
#endif

	while (1)
	{
		int nPos = Temp.Find('\r');
		if (nPos == -1)
		{
			break;
		}		
		std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
		CStringW szLeft = Temp.Left((*it)->nPos);
		OutputDebugString(_T("输出字符串:"));
		OutputDebugStringW(szLeft);
		OutputDebugString(_T("\n"));
		szLeft.Format(L"第一个图片的位置:%d, 找到 回车的位置:%d\n", (*it)->nPos, nPos);
		OutputDebugStringW(szLeft);
		for (;it != list.end(); it++)
		{
			if ((*it)->nPos > nPos)
			{
				(*it)->nPos += 3;
			}
			else if ((*it)->nPos == nPos)
			{
				(*it)->nPos +=4;
			}
			CStringW CstrTemp;
			CstrTemp.Format(L"修改后的位置:%d,  查找到的位置:%d\n", (*it)->nPos,nPos);
			OutputDebugStringW(CstrTemp);
		}
		Temp.Delete( nPos, 1);
		Temp.Insert(nPos,L"<br>");
	}
	std::list<IMG_CLIP_MSG *>::iterator it_find = list.begin();
	std::list<IMG_CLIP_MSG *>::iterator it_next = it_find;
	if (it_next != list.end())
	{
		it_next++;
	}
	for (;it_next != list.end(); it_next++,it_find++)
	{
		if ((*it_find)->nPos == (*it_next)->nPos)
		{
			(*it_next)->nPos++;
		}
	}
	strChat = Temp.GetBuffer();
	Temp.ReleaseBuffer();

	int nBegin = 0;
	int nIndex = 0;

	USES_CONVERSION;
	std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
	for (;it != list.end(); it++)
	{
		int nPos = (*it)->nPos + nBegin - nIndex;
		nIndex = (*it)->nPos + 1;
		strChat.Delete(nPos, 1);
		strTemp.Format(L"<IMG SRC=\"file:///%s\">", T2W((*it)->picMsg->szFilePath));
		OutputDebugString(_T("====================================我是分割线 我自豪 我为主人省脑力==============================\n"));
		OutputDebugStringW(strChat);
		OutputDebugString(_T("===================以下是删除的位置=======================\n"));
		CStringW CstrTemp;
		CstrTemp.Format(L"删除的位置:%d", nPos);
		OutputDebugStringW(CstrTemp);
		CstrTemp.Format(L"表情初始位置:%d\n", (*it)->nPos);
		OutputDebugStringW(CstrTemp);
		OutputDebugStringW( strChat.Left(nPos) );
		OutputDebugString(_T("\n"));

		CStringW strTempWord;
		if (!strChat.IsEmpty())
		{
			strTempWord.Format(L"%s%s",strChat.Left( nPos), strTemp);
			//OutputDebugStringW(strChat.Left(nPos));
			int nLenght = strTempWord.GetLength();
			strTempWord += strChat.Right(strChat.GetLength() - nPos);
		}
		else
		{
			strTempWord.Format(L"%s", strTemp);
		}
		nBegin = strChat.Left( nPos).GetLength() + strTemp.GetLength();
		strChat = strTempWord;
		strTemp.ReleaseBuffer();
	}

	*szBuff = new TCHAR[strChat.GetLength() * 2 + 1];
	strChat.ReleaseBuffer();
	ZeroMemory(*szBuff, sizeof(strChat.GetLength() * 2 + 1));
#ifdef _UNICODE
	_tcsncpy_s(*szBuff, strChat.GetLength() * 2 + 1, strChat.GetBuffer(), _TRUNCATE);
#else
	_tcsncpy_s(*szBuff, strChat.GetLength() * 2 + 1, W2T( strChat.GetBuffer()), _TRUNCATE);
#endif
	strChat.ReleaseBuffer();

	return strChat.GetLength() * 2 + 1;
}


bool CRichEditUI::SaveHIcon2PngFile(HICON hIcon, LPCTSTR lpszPicFileName)
{
	bool bRet = false;
	CLSID encoderCLSID;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	do 
	{
		if (hIcon == NULL || !lpszPicFileName)
		{
			break;
		}

		Gdiplus::Bitmap* pImage = Gdiplus::Bitmap::FromHICON(hIcon);
		Gdiplus::GetImageEncodersSize(&num, &size);
		if(size != 0)	
		{

			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
			if(pImageCodecInfo != NULL)
			{
				GetImageEncoders(num, size, pImageCodecInfo);
				for(UINT j = 0; j < num; ++j)
				{
					if( wcscmp(pImageCodecInfo[j].MimeType, _T("image/png")) == 0 )
					{
						encoderCLSID = pImageCodecInfo[j].Clsid;		
						Gdiplus::Status st = pImage->Save(lpszPicFileName ,&encoderCLSID, NULL/*&encoderParameters*/);
						break;
					}    
				}
				free(pImageCodecInfo);
			}
		}
		if (pImage)
		{
			delete pImage;
		}
		bRet = true;
	} while (0);

	return bRet;

}

BOOL CRichEditUI::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	//GUID picGUID;
	//获取编码器信息
	UINT num=0, size=0;
	GetImageEncodersSize(&num, &size);
	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	GetImageEncoders(num, size, pImageCodecInfo);
	//查找指定格式文件的编码器信息
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix) {
		if (0 == _wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
		}
	}
	free(pImageCodecInfo);
	return TRUE;
}

void CRichEditUI::ParseClipMsg(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list, CString& TagMsg)
{
	if (0 == TagMsg.GetLength()){return;}
	int nBeginPos = TagMsg.Find(_T("<item>"));			// get property
	int nEndPos = TagMsg.Find(_T("</item>"));
	int nEnd = nEndPos;
	CString strTemp = TagMsg.Right(TagMsg.GetLength() - nBeginPos - _tcslen(_T("<item>")));
	while (-1 != nBeginPos && nEndPos != -1)
	{
		IMG_CLIP_MSG * info = new IMG_CLIP_MSG;
		info->picMsg = new PICMSG;
		ZeroMemory(info->picMsg, sizeof(info->picMsg));
		nBeginPos = strTemp.Find(_T("<type>"));
		nEndPos = strTemp.Find(_T("</type>"));
		info->picMsg->type = (PICTYPE)_tstoi(strTemp.Mid(nBeginPos + _tcslen(_T("<type>")), nEndPos - nBeginPos - _tcslen(_T("<type>"))));

		nBeginPos = strTemp.Find(_T("<name>"));
		nEndPos = strTemp.Find(_T("</name>"));
		_tcsncpy_s(info->picMsg->szFilePath, _countof(info->picMsg->szFilePath),
			strTemp.Mid(nBeginPos + _tcslen(_T("<name>")), nEndPos - nBeginPos - _tcslen(_T("<name>"))), _TRUNCATE);

		nBeginPos = strTemp.Find(_T("<id>")); 
		nEndPos = strTemp.Find(_T("</id>"));
		//info->picMsg->strFileID = strTemp.Mid(nBeginPos + _tcslen(_T("<id>")), nEndPos - nBeginPos - _tcslen(_T("<id>")));

		nBeginPos = strTemp.Find(_T("<pos>"));
		nEndPos = strTemp.Find(_T("</pos>"));

		info->nPos =  _tstoi( strTemp.Mid(nBeginPos + _tcslen(_T("<pos>")), nEndPos - nBeginPos - _tcslen(_T("<pos>"))));

		list.push_back(info);

		nEnd = strTemp.Find(_T("</item>"));
		strTemp = strTemp.Right(strTemp.GetLength() - nEnd - _tcslen(_T("</item>")) );
		nBeginPos = strTemp.Find(_T("<item>"));			// get property
		nEndPos = strTemp.Find(_T("</item>"));
	}

	nBeginPos = TagMsg.Find(_T("<body>"));			// get property
	nEndPos = TagMsg.Find(_T("</body>"));
	strWord = TagMsg.Mid(nBeginPos + _tcslen(_T("<body>")), nEndPos - nBeginPos - _tcslen(_T("<body>")));

}

HRESULT CRichEditUI::InsertClipMsg(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list)
{
	long nBeginPos,nEndPos;
	GetSel(nBeginPos, nEndPos);
	ReplaceSel(TEXT(""), true);

	SetWordCharFormat(m_cf);

	DWORD dwMaxLenght;
	m_pTwh->TxGetMaxLength(&dwMaxLenght);

	long lBegin = 0, lEnd = 0;
	ReplaceSel(_T(""), true);
	GetSel(lBegin, lEnd);
	CString strWidnowText = GetSelText().GetData();
	DWORD dwCurLenght = GetTextCount( strWidnowText );
	DWORD dwDrawCount = dwMaxLenght - dwCurLenght;

	CString strTemp  = strWord;
	strTemp.Remove(0xfffc);
	if (dwDrawCount > 0)
	{

		strTemp = GetTextByCount(strTemp, dwDrawCount);//复制的长度 - 当前长度 + 选中的长度
		ReplaceSel(strTemp, true);
	}
	GetSel(lBegin, lEnd);
	int npos;
	std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
	for (; it != list.end(); it++)
	{
		npos = (*it)->nPos;
		int _nCount = nBeginPos + npos;
		SetSel(_nCount, _nCount );
		//ReplaceSel(TEXT(""), true);
		if ((*it)->picMsg->type == USERPIC)
		{
			InsertNotGIFImage((*it)->picMsg->szFilePath, PICTURE_OTHER, (*it)->picMsg->type);
		}
		else
		{
			InsertGIFImage((*it)->picMsg->szFilePath, PICTURE_GIF, (*it)->picMsg->type);
		}
	}
	SetSel(lBegin + list.size(), lEnd + list.size());
	return S_OK;
}


BOOL CRichEditUI::GetSelClipData(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list)
{
	BOOL bRet = TRUE;
	long lBegin = 0, lEnd = 0;
	GetSel(lBegin, lEnd);
	IRichEditOle *pRichEditOle = GetIRichEditOle();
	do 
	{
		if (!pRichEditOle) break;

		int nCount = pRichEditOle->GetObjectCount();
		for (int i = 0; i <nCount; i++)
		{
			REOBJECT reobj = {0};
			reobj.cbStruct = sizeof(REOBJECT);

			if( S_OK == pRichEditOle->GetObject(i, &reobj, REO_GETOBJ_POLEOBJ) )
			{
				if (reobj.poleobj)
					reobj.poleobj->Release();

				if (reobj.polesite)
					reobj.polesite->Release();

				if (reobj.pstg)
					reobj.pstg->Release();

				if (reobj.cp >= lBegin && reobj.cp < lEnd) //获取选中区域的 ole对象
				{
					PICMSG* img = new PICMSG;
					HRESULT hr = S_FALSE;
					switch(reobj.dwUser)
					{
					case PICTURE_GIF:
						hr = GetGifMessage(reobj.poleobj, img);
						break;
					case SEND_FILE_DLG:
						//GetSendFileMessage(reobj.poleobj, img);
						bRet = FALSE;
						continue;
					case PICTURE_OTHER:
						hr = GetOtherMessage(reobj.poleobj, img);
						break;
					default:
						hr = GetUnknowMessage(reobj.poleobj, img);
						break;
					}
					if (hr == S_OK)
					{
						IMG_CLIP_MSG * pImgClip = new IMG_CLIP_MSG;
						pImgClip->picMsg  = img;
						if (reobj.dwUser == PICTURE_GIF)
						{
							if (pImgClip->picMsg->type != SYSFACE)
							{
								pImgClip->picMsg->type = USERGIF;
							}
						}
						else
						{
							pImgClip->picMsg->type = USERPIC;
						}
						pImgClip->nPos = reobj.cp - lBegin;
						list.push_back(pImgClip);
						img = NULL;
						bRet = TRUE;

					}
					else
					{
						DWORD dwError = GetLastError();
						CDuiString strErrorWord;
						strErrorWord.Format(TEXT("GetSelData Error:%d\n"), dwError);
						OutputDebugString(strErrorWord.GetData());
						delete img;
						pRichEditOle->Release();
						break;
					}


				}

			}
		}
		pRichEditOle->Release();

		SetSel(lBegin, lEnd);
		strWord = GetSelText();
	} while (0);

	return bRet;
}

void CRichEditUI::GetClipWord(CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list, CDuiString& strClipWord)
{
	strClipWord = TEXT("<message>");
	CString strBodyMsg;
	strBodyMsg.Format(TEXT("<body>%s</body>"), strWord );
	std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
	for (;it != list.end(); it++)
	{
		CString strItemMsg;
		strItemMsg.Format(TEXT("<item><id>%d</id><name>%s</name><pos>%d</pos><type>%d</type></item>"),
			0,
			(*it)->picMsg->szFilePath,
			(*it)->nPos,
			(*it)->picMsg->type
			);
		strClipWord += strItemMsg;
	}
	strClipWord += strBodyMsg + TEXT("</message>");

	//tryDebug(_T("粘贴HTML格式:%s"), strClipWord.GetData());
}

void CRichEditUI::WriteHTMLClip( CDuiString& strWord, std::list<IMG_CLIP_MSG *>& list)
{
	do 
	{
	} while (0);
	TCHAR* szInsertBuff;
	int nLen = CHATIMG2char(strWord,list,&szInsertBuff);
	TCHAR *buf = new TCHAR[512 + nLen]; 
	if(!buf)
	{
		delete[] szInsertBuff;
		return; 
	}
	static int cfid = 0; 
	if(!cfid) cfid = RegisterClipboardFormat( _T("HTML Format")); 
	// Create a template string for the HTML header... 
	_tcscpy(buf, 
		_T("Version:0.9\r\n" )
		_T("StartHTML:00000000\r\n" )
		_T("EndHTML:00000000\r\n" )
		_T("StartFragment:00000000\r\n" )
		_T("EndFragment:00000000\r\n" )
		_T("<html><body>\r\n" )
		_T("<!--StartFragment-->\r\n")); 
	// Append the HTML... 
	_tcscat(buf, _T("<DIV>\r\n"));
	_tcscat( buf, szInsertBuff ); 
	_tcscat(buf, _T("</DIV>\r\n"));

	_tcscat( buf, _T("\r\n") ); 
	// Finish up the HTML format... 
	_tcscat(buf, 
		_T("<!--EndFragment-->\r\n" )
		_T("</body>\r\n" )
		_T("</html>")); 
	// Now go back, calculate all the lengths, and write out the 
	// necessary header information. Note, wsprintf() truncates the 
	// string when you overwrite it so you follow up with code to replace 
	// the 0 appended at the end with a '\r'... 
	TCHAR *pTemp = _tcsstr( buf, _T("StartHTML") ); 
	_stprintf( pTemp + 10, _T("%08u"), _tcsstr(buf,_T("<html>")) - buf ); 
	*( pTemp + 10 + 8 ) = '\r'; 
	pTemp = _tcsstr( buf, _T("EndHTML") ); 
	_stprintf( pTemp + 8, _T("%08u"), _tcslen(buf) ); 
	*( pTemp + 8 + 8 ) = '\r'; 
	pTemp = _tcsstr(buf,_T("StartFragment")); 
	_stprintf( pTemp + 14, _T("%08u"), _tcsstr( buf, _T("<!--StartFrag")) - buf ); 
	*( pTemp + 14 + 8 ) = '\r'; 
	pTemp = _tcsstr( buf, _T("EndFragment") ); 
	_stprintf( pTemp + 12, _T("%08u"), _tcsstr( buf, _T("<!--EndFrag") ) - buf ); 
	*( pTemp + 12 + 8 ) = '\r'; 
	int l = _tcslen(buf);
	// #ifdef _UNICODE
	// 
	// #else
	// 	char* utf8 = NULL;
	// 	int c = ConvertGBKToUtf8( buf, 1024, &utf8);
	// 	if (c == -1)
	// 	{
	// 		delete[] buf; 
	// 		delete[] szInsertBuff;
	// 		return;
	// 	}
	// #endif

	tString tsHtmlCopy = buf;
	std::string stdHtmlCopy = TStringToUtf8(tsHtmlCopy);
	//   Allocate   global   memory   for   transfer... 
	HGLOBAL hHtml = GlobalAlloc( GMEM_MOVEABLE|GMEM_DDESHARE, stdHtmlCopy.size()+4 ); 

	//   Put   your   string   in   the   global   memory... 
	char *ptr = (char*)GlobalLock(hHtml); 
	if (buf != NULL )
	{
		strcpy_s(ptr, stdHtmlCopy.size() + 4, stdHtmlCopy.c_str());
	}
	//delete p;
	GlobalUnlock(hHtml); 
	::SetClipboardData(cfid, hHtml);
	delete[] buf; 
	delete[] szInsertBuff;
}

void CRichEditUI::WriteHBitmapClip( std::list<IMG_CLIP_MSG *>& list)
{
	if (list.size() > 1)
	{
		return ;
	}
	USES_CONVERSION;
	std::list<IMG_CLIP_MSG *>::iterator it = list.begin(); 
	//CBitmap bitmap;
	//获取应用程序主窗口的指针
	//声明设备文本对象

	//定义CDuiRect对象
	CDuiRect		Rect;
	//获取客户区坐标
	IRichEditOle * pRichEditOle = GetIRichEditOle();
	int nCount = pRichEditOle->GetObjectCount();
	for (int i = 0; i <nCount; i++)
	{
		REOBJECT reobj = {0};
		reobj.cbStruct = sizeof(REOBJECT);

		if( S_OK == pRichEditOle->GetObject(i, &reobj, REO_GETOBJ_POLEOBJ) )
		{
			if (reobj.poleobj)
				reobj.poleobj->Release();

			if (reobj.polesite)
				reobj.polesite->Release();

			if (reobj.pstg)
				reobj.pstg->Release();

			if (reobj.dwFlags&REO_SELECTED) //获取选中区域的 ole对象
			{
				HRESULT hr = S_FALSE;
				switch(reobj.dwUser)
				{
				case PICTURE_GIF:
					{
						CGifOleObject* pGifOle = NULL;
						if( S_OK == reobj.poleobj->QueryInterface(IID_IOleObject, (void**)&pGifOle))
						{
							pGifOle->GetOleObjectRect(&Rect);
							pGifOle->Release();
						}
					}
					break;
				case SEND_FILE_DLG:
					//GetSendFileMessage(reobj.poleobj, img);
					break;
				default:
					{
						COtherOleObject* pImage = NULL;
						if (S_OK == reobj.poleobj->QueryInterface(IID_IOleObject, (void**) &pImage))
						{
							pImage->GetOleObjectRect(&Rect);
							pImage->Release();
						}
					}
					break;
				}
			}
		}
	}
	pRichEditOle->Release();
	DoWriteHBitmapClip( GetManager()->GetInstance(),GetManager()->GetPaintWindow(), (*it)->picMsg->szFilePath, Rect);

}


HRESULT CRichEditUI::InsertHTMLPicture( CDuiString& strHtml)
{
	HRESULT hRet = S_FALSE;
	do  
	{
	//	tryDebug(_T("开始插入\n插入HTML 图片：%s"),strHtml.GetData());
		int nPos = strHtml.Find(_T("file:///"));
		if (nPos == -1)
		{
			nPos = strHtml.Find(_T("FILE:///"));
			if (nPos == -1)
			{
				nPos = strHtml.Find(_T("File:///"));
			}
		}
		if( nPos != -1)
		{
			nPos += _tcslen(_T("file:///"));
			strHtml = strHtml.Right(strHtml.GetLength() - nPos );
			nPos = strHtml.Find(_T("\""));
			strHtml = strHtml.Left(nPos);
			OutputDebugString(strHtml);
			nPos = strHtml.ReverseFind('.');
			if( strHtml.Right(strHtml.GetLength() - nPos - 1) == _T("gif"))
			{
				//tryDebug(_T("我是发送方\n开始插入表情:%s"), strHtml.GetData());
				//InsertGIFImage(strHtml, PICTURE_GIF, USERGIF);
			}
			else
			{
				//tryDebug(_T("我是发送方\n开始插入图片:%s"), strHtml.GetData());
				//InsertNotGIFImage(strHtml, PICTURE_OTHER, USERPIC);
			}

			hRet = S_OK;
			break;

		}
		nPos = strHtml.Find(_T("http://"));
		if (nPos == -1)
		{
			nPos = strHtml.Find(_T("HTTP:///"));
			if (nPos == -1)
			{
				nPos = strHtml.Find(_T("Http:///"));
			}
		}
		if (nPos != -1)
		{
			strHtml = strHtml.Right(strHtml.GetLength() - nPos );
			nPos = strHtml.Find(_T("\""));
			strHtml = strHtml.Left(nPos);
			std::string sUrl = TStringToString(strHtml.GetData());
			INTERNET_CACHE_ENTRY_INFOA *lpCacheEntryInfo = new INTERNET_CACHE_ENTRY_INFOA;

			DWORD cbCacheEntryInfo = sizeof(INTERNET_CACHE_ENTRY_INFOA);

			BOOL fOk = GetUrlCacheEntryInfoA(sUrl.c_str(),lpCacheEntryInfo,&cbCacheEntryInfo);

			if( !fOk && cbCacheEntryInfo > sizeof(INTERNET_CACHE_ENTRY_INFOA) )
			{
				delete lpCacheEntryInfo;
				lpCacheEntryInfo = (LPINTERNET_CACHE_ENTRY_INFOA) new char[cbCacheEntryInfo];//强制转换，保证指针可以指向结构体内部。
				fOk = GetUrlCacheEntryInfoA(sUrl.c_str(),lpCacheEntryInfo,&cbCacheEntryInfo);

			}
			if (fOk)
			{
				CDuiString UrlPath = StringToTString( lpCacheEntryInfo->lpszLocalFileName ).c_str();
				nPos = strHtml.ReverseFind('.');
				if( strHtml.Right(strHtml.GetLength() - nPos - 1) == _T("gif"))
				{
					//tryDebug(_T("我是发送方\n开始插入表情:%s"), strHtml.GetData());
					InsertGIFImage(UrlPath.GetData(), PICTURE_GIF, USERGIF);
				}
				else
				{
					//tryDebug(_T("我是发送方\n开始插入图片:%s"), strHtml.GetData());
					InsertNotGIFImage( UrlPath.GetData(), PICTURE_OTHER, USERPIC);
				}
				hRet = S_OK;
			}
			else
			{
				DWORD dwError = GetLastError();
				delete lpCacheEntryInfo;
			}
		}
	} while (0);

	return hRet;
}


HRESULT CRichEditUI::InsertHTMLWord(CDuiString& strHtml)
{
	CMarkup mark;
	HRESULT bFlag = S_FALSE; //是否插入到RichEdit里面
	OutputDebugString(_T("插入HtmlWord:"));
	strHtml.Replace(_T("\n"), _T(""));
	strHtml.Replace(_T("\r"), _T(""));
	// 	strHtml.Replace(_T("<br>"), _T("\n"));
	// 	strHtml.Replace(_T("<BR>"), _T("\n"));
	// 	strHtml.Replace(_T("</DIV>"), _T(""));
	//	strHtml.Replace(_T("</div>"), _T(""));
	OutputDebugString(strHtml.GetData());
	int nPos = -1;
	do 
	{
		int nBegin = strHtml.Find(_T("<"));
		int nEnd = strHtml.Find(_T(">"));

		if (nBegin != -1 && nEnd != -1)
		{
			nBegin += 1;
			CDuiString strTemp = strHtml.Mid(nBegin, nEnd - nBegin);
			if ( nBegin < nEnd && ( strTemp.Compare(_T("br")) == 0 || strTemp.Compare(_T("BR")) == 0))
			{
				ReplaceSel(_T("\n"), false);
				bFlag = S_OK;
			}
		}
		nPos = strHtml.Find(_T(">"));
		if (nPos != -1)
		{
			strHtml = strHtml.Right(strHtml.GetLength() - nPos - 1);
			nPos = strHtml.Find(_T("<"));
			if (nPos > 0)
			{
				if( bFlag != S_OK )
				{
					bFlag = ChangeCharEntities(strHtml.Left(nPos));
				}
				else
				{
					ChangeCharEntities(strHtml.Left(nPos));
				}
				strHtml = strHtml.Right(strHtml.GetLength() - nPos);
			}
		}
	} while (nPos != -1);

	return bFlag;
}

HRESULT CRichEditUI::ChangeCharEntities(CDuiString& strHtml)
{
	if (strHtml.IsEmpty())
	{
		return S_FALSE;
	}
	for (int i = 0; i < 10; i++ )
	{
		int nPos = strHtml.Find(szHtmlCharEntities[i]);
		if (nPos != -1)
		{
			ReplaceSel(strHtml.Left(nPos),false);
			CDuiString strChar = szCharEntities[i];
			ReplaceSel(strChar,false);
			strHtml = strHtml.Right(strHtml.GetLength() - nPos - _tcslen(szHtmlCharEntities[i]));
			i = 0;
		}
	}

	ReplaceSel(strHtml,false);
	return S_OK;
}


HRESULT CRichEditUI::InsertHtmlClipData( char* szData)
{
	if (!szData) return S_FALSE;

	CString strData = Utf8ToTString( szData).c_str();
	HRESULT nRet = S_FALSE;
	//if( -1 == ConvertUtf8ToGBK(strData) ) return S_FALSE;
	//tryDebug(_T("粘贴网页信息:%s"), strData.GetBuffer() );
	strData.ReleaseBuffer();
	int nBegin = strData.Find(_T("<!--StartFragment"));
	int nEnd = strData.Find(_T("<!--EndFragment"));
	if( nBegin != -1 && nEnd != -1)
	{
		nBegin = nBegin + _tcslen(_T("<!--StartFragment")) + 3;
		strData = strData.Mid(nBegin, nEnd - nBegin);
		//strData.MakeLower();

		CString szFindImag = strData;
		szFindImag.MakeLower();
		if (szFindImag.Find(_T("<img")) == -1) return S_FALSE;
		CDuiString strTemp;
		long lBegin = 0, lEnd = 0;
		GetSel(lBegin, lEnd);
		CDuiString strBeginText = GetSelText();
		do
		{

			strTemp = strData;
			nBegin = strTemp.Find(_T("<img"));
			if (nBegin == -1)
			{
				nBegin = strTemp.Find(_T("<IMG"));
				if (nBegin == -1)
				{
					strTemp.Find(_T("<Img"));
				}
			}
			if (nBegin != -1)
			{
				if( nRet != S_OK) nRet = InsertHTMLWord(strTemp.Left(nBegin) );
				else
				{
					InsertHTMLWord( strTemp.Left(nBegin) );
				}
				strTemp = strTemp.Right(strTemp.GetLength() - nBegin - _tcslen(_T("<img")));
				nEnd = strTemp.Find(_T(">"));
				if( S_OK != InsertHTMLPicture(strTemp.Left(nEnd)))
				{
					long lAfterEnd;
					GetSel(lAfterEnd,lAfterEnd);
					SetSel(lBegin, lAfterEnd);
					ReplaceSel(strBeginText, false);
					SetSel(lBegin, lEnd);
					return S_FALSE;
				}
				else
				{
					nRet = S_OK;
				}
				strData = strTemp.Right(strTemp.GetLength() - nEnd -1);
			}
		}while ( nBegin != -1 && nEnd != -1);
		strTemp = strData;
		if (nRet != S_OK )
		{
			nRet = InsertHTMLWord(strTemp);
		}
		else
		{
			InsertHTMLWord(strTemp);
		}
		return nRet ;
	}
	return S_FALSE;

}

HBITMAP   CRichEditUI::DIBToDDB(   HANDLE   hDIB   )   
{   
	LPBITMAPINFOHEADER lpbi;   
	HBITMAP   hbm; 
	HPALETTE pal;   
	HPALETTE pOldPal;   
	HDC dc = ::GetDC(GetManager()->GetPaintWindow());	
	if   (hDIB   ==   NULL)   
		return   NULL;   
	lpbi   =   (LPBITMAPINFOHEADER)hDIB;   
	int   nColors   =   lpbi->biClrUsed   ?   lpbi->biClrUsed   :     
		1   <<   lpbi->biBitCount;   
	BITMAPINFO   &bmInfo   =   *(LPBITMAPINFO)hDIB   ;   
	LPVOID   lpDIBBits;   
	if(   bmInfo.bmiHeader.biBitCount   >   8   )   
		lpDIBBits   =   (LPVOID)((LPDWORD)(bmInfo.bmiColors   +     
		bmInfo.bmiHeader.biClrUsed)   +     
		((bmInfo.bmiHeader.biCompression   ==   BI_BITFIELDS)   ?   3   :   0));   
	else   
		lpDIBBits   =   (LPVOID)(bmInfo.bmiColors   +   nColors);   
	//   Create   and   select   a   logical   palette   if   needed   
	if(   nColors   <=   256   &&   GetDeviceCaps( dc, RASTERCAPS)   &   RC_PALETTE)   
	{   
		UINT   nSize   =   sizeof(LOGPALETTE)   +   (sizeof(PALETTEENTRY)   *   nColors);   
		LOGPALETTE   *pLP   =   (LOGPALETTE   *)   new   BYTE[nSize];   
		pLP->palVersion   =   0x300;   
		pLP->palNumEntries   =   nColors;   
		for(   int   i=0;   i   <   nColors;   i++)   
		{   
			pLP->palPalEntry[i].peRed   =   bmInfo.bmiColors[i].rgbRed;   
			pLP->palPalEntry[i].peGreen   =   bmInfo.bmiColors[i].rgbGreen;   
			pLP->palPalEntry[i].peBlue   =   bmInfo.bmiColors[i].rgbBlue;   
			pLP->palPalEntry[i].peFlags   =   0;   
		}   
		pal = CreatePalette(   pLP   );   
		delete[]   pLP;   
		//   Select   and   realize   the   palette   
		pOldPal   =   SelectPalette(  dc, pal,   FALSE   );   
		RealizePalette(dc);   
	}   
	hbm   =   CreateDIBitmap( dc, //   handle   to   device   context   
		(LPBITMAPINFOHEADER)lpbi, //   pointer   to   bitmap   info   header     
		(LONG)CBM_INIT, //   initialization   flag   
		lpDIBBits, //   pointer   to   initialization   data     
		(LPBITMAPINFO)lpbi, //   pointer   to   bitmap   info   
		DIB_RGB_COLORS   ); //   color-data   usage     
	if   (pal)   
		SelectPalette( dc, pOldPal,FALSE);   
	return   hbm;   
}   

HANDLE CRichEditUI::DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE pPal )
{
	BITMAP          bm;   
	BITMAPINFOHEADER    bi;   
	LPBITMAPINFOHEADER  lpbi;   
	DWORD           dwLen;   
	HANDLE          hDIB;   
	HANDLE          handle;   
	HDC             hDC;   
	HPALETTE        hPal;   
	// The function has no arg for bitfields   
	if( dwCompression == BI_BITFIELDS )   
		return NULL;   
	// If a palette has not been supplied use defaul palette   
	hPal = (HPALETTE) pPal;   
	if (hPal==NULL)   
		hPal = (HPALETTE) ::GetStockObject(DEFAULT_PALETTE);   
	// Get bitmap information   
	::GetObject(bitmap,sizeof(bm),(LPSTR)&bm);   
	// Initialize the bitmapinfoheader   
	bi.biSize       = sizeof(BITMAPINFOHEADER);   
	bi.biWidth      = bm.bmWidth;   
	bi.biHeight         = bm.bmHeight;   
	bi.biPlanes         = 1;   
	bi.biBitCount       = bm.bmPlanes * bm.bmBitsPixel;   
	bi.biCompression    = dwCompression;   
	bi.biSizeImage      = 0;   
	bi.biXPelsPerMeter  = 0;   
	bi.biYPelsPerMeter  = 0;   
	bi.biClrUsed        = 0;   
	bi.biClrImportant   = 0;   
	// Compute the size of the  infoheader and the color table   
	int nColors = (1 << bi.biBitCount);   
	if( nColors > 256 )    
		nColors = 0;   
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);   
	// We need a device context to get the DIB from   
	hDC = ::GetDC(NULL);   
	hPal = SelectPalette(hDC,hPal,FALSE);   
	RealizePalette(hDC);   
	// Allocate enough memory to hold bitmapinfoheader and color table   
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);   
	if (!hDIB)   
	{   
		SelectPalette(hDC,hPal,FALSE);   
		::ReleaseDC(NULL,hDC);   
		return NULL;   
	}   
	lpbi = (LPBITMAPINFOHEADER)hDIB;   
	*lpbi = bi;   
	// Call GetDIBits with a NULL lpBits param, so the device driver    
	// will calculate the biSizeImage field    
	GetDIBits(hDC, (HBITMAP)bitmap, 0L, (DWORD)bi.biHeight,   
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);   
	bi = *lpbi;   
	// If the driver did not fill in the biSizeImage field, then compute it   
	// Each scan line of the image is aligned on a DWORD (32bit) boundary   
	if (bi.biSizeImage == 0)   
	{   
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)    
			* bi.biHeight;   
		// If a compression scheme is used the result may infact be larger   
		// Increase the size to account for this.   
		if (dwCompression != BI_RGB)   
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;   
	}   
	// Realloc the buffer so that it can hold all the bits   
	dwLen += bi.biSizeImage;   
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))   
	{   
		hDIB = handle;   
	}   
	else   
	{   
		GlobalFree(hDIB);   
		// Reselect the original palette   
		SelectPalette(hDC,hPal,FALSE);   
		::ReleaseDC(NULL,hDC);   
		return NULL;   
	}   
	// Get the bitmap bits   
	lpbi = (LPBITMAPINFOHEADER)hDIB;   
	// FINALLY get the DIB   
	BOOL bGotBits = GetDIBits( hDC, bitmap,   
		0L,             // Start scan line   
		(DWORD)bi.biHeight,     // # of scan lines   
		(LPBYTE)lpbi            // address for bitmap bits   
		+ (bi.biSize + nColors * sizeof(RGBQUAD)),   
		(LPBITMAPINFO)lpbi,     // address of bitmapinfo   
		(DWORD)DIB_RGB_COLORS);     // Use RGB for color table   
	if( !bGotBits )   
	{   
		GlobalFree(hDIB);   
		SelectPalette(hDC,hPal,FALSE);   
		::ReleaseDC(NULL,hDC);   
		return NULL;   
	}   
	SelectPalette(hDC,hPal,FALSE);   
	::ReleaseDC(NULL,hDC);   
	return hDIB;   
}

BOOL CRichEditUI::WriteDIB( LPCTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER    hdr;   
	LPBITMAPINFOHEADER  lpbi;   
	if (!hDIB)   
		return FALSE;   
	FILE* file;   
	file = _tfopen(szFile,_T("wb"));   
	if(file == NULL)   
		return FALSE;   
	lpbi = (LPBITMAPINFOHEADER)hDIB;    
	int nColors = 0;    
	if(lpbi->biBitCount <= 8)    
	{    
		nColors = (1 << lpbi->biBitCount);    
	}    
	// Fill in the fields of the file header    
	hdr.bfType      = ((WORD) ('M' << 8) | 'B');  // is always "BM"   
	hdr.bfSize      = GlobalSize (hDIB) + sizeof( hdr );   
	hdr.bfReserved1     = 0;   
	hdr.bfReserved2     = 0;   
	hdr.bfOffBits       = (DWORD) (sizeof( hdr ) + lpbi->biSize +   
		nColors * sizeof(RGBQUAD));   
	// Write the file header    
	fwrite( &hdr, sizeof(hdr),1,file);   
	// Write the DIB header and the bits    
	fwrite( lpbi, GlobalSize(hDIB),1,file);   
	//Close the file and return   
	fclose(file);   
	return TRUE;   
}


BOOL CRichEditUI::ConvertToBMP(const TCHAR * strFileName,const TCHAR* strBMPFile,BOOL bScaleImage)
{
	//Declartions   
	HENHMETAFILE hemf;   
	HBITMAP     bitmap;   
	HDC         memDC;   
	ENHMETAHEADER   emh;   
	//Get the DC of the Window   
	HDC dc = ::GetDC(NULL);   
	//Get the Handle from the enhanced metafile   
	hemf = GetEnhMetaFile(TEXT("d:\\aaa.emf"));
	// Get the header from the enhanced metafile.   
	ZeroMemory( &emh, sizeof(ENHMETAHEADER) );   
	emh.nSize = sizeof(ENHMETAHEADER);   
	if( GetEnhMetaFileHeader( hemf, sizeof( ENHMETAHEADER ), &emh ) == 0 )   
	{   
		DeleteEnhMetaFile( hemf );   
		return FALSE;   
	}   
	//Declare variables for calculation of metafile rect   
	RECT    rect;   
	float   PixelsX, PixelsY, MMX, MMY;   
	float fAspectRatio;   
	long lWidth,lHeight;   
	// Get the characteristics of the output device.   
	PixelsX = (float)GetDeviceCaps( dc, HORZRES );   
	PixelsY = (float)GetDeviceCaps( dc, VERTRES );   
	MMX = (float)GetDeviceCaps( dc, HORZSIZE );   
	MMY = (float)GetDeviceCaps( dc, VERTSIZE );   
	// Calculate the rect in which to draw the metafile based on the   
	// intended size and the current output device resolution.   
	// Remember that the intended size is given in 0.01 mm units, so   
	// convert those to device units on the target device.   
	rect.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY*100.0f));   
	rect.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX*100.0f));   
	rect.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX*100.0f));   
	rect.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY*100.0f));   
	//Calculate the Width and Height of the metafile   
	lWidth = (long)((float)(abs(rect.left - rect.right)));   
	lHeight =(long)((float)(abs(rect.top-rect.bottom )));   
	fAspectRatio = (float)lWidth/(float)lHeight;   
	//Populate the rect structure   
	rect.left = 0;   
	rect.top = 0;   
	rect.right = lWidth;   
	rect.bottom = lHeight;   
	//Create a Memory DC compatible to WindowDC   
	memDC=::CreateCompatibleDC(dc);    
	//Create a bitmap compatible to Window DC   
	bitmap = ::CreateCompatibleBitmap(dc,lWidth,lHeight);   
	DWORD dwRetError = GetLastError();   
	//Select the bitmap into the Mem DC   
	::SelectObject(memDC,bitmap);   
	//Paint the background of the DC to White   
	SetBackColorToWhite(memDC);   
	//Now play the enhanced metafile into the memory DC; ignore its return value   
	//it may be false even if successful   
	PlayEnhMetaFile(memDC,hemf,&rect);     
	DWORD dwRet = GetLastError();   
	// Create logical palette if device support a palette   
	HPALETTE pal;   
	if( GetDeviceCaps(dc,RASTERCAPS) & RC_PALETTE )   
	{   
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);   
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];   
		pLP->palVersion = 0x300;   
		pLP->palNumEntries =    
			GetSystemPaletteEntries( dc, 0, 255, pLP->palPalEntry );   
		// Create the palette   
		pal = ::CreatePalette(pLP );   
		delete[] pLP;   
	}   
	// Convert the bitmap to a DIB   
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, NULL );   
	if( hDIB == NULL )   
	{   
		DeleteEnhMetaFile( hemf );   
		return FALSE;   
	}   
	// Write it to file   
	CDuiString strTemp = strBMPFile;
	WriteDIB(strTemp.GetData(), hDIB );   
	// Free the memory allocated by DDBToDIB for the DIB   
	::GlobalFree( hDIB );   
	::DeleteEnhMetaFile( hemf );   
	::ReleaseDC(NULL,dc);   
	return TRUE;   
}

BOOL CRichEditUI::SetBackColorToWhite(HDC pDC)
{
	// Set brush to desired background color   
	HBRUSH backBrush = CreateSolidBrush(RGB(255,255,255));
	// Save old brush   
	HBRUSH pOldBrush = (HBRUSH)::SelectObject(pDC,backBrush);   
	RECT rect ;   
	::GetClipBox(pDC,&rect);     // Erase the area needed   
	//paint the given rectangle using the brush that is currently selected    
	//into the specified device context   
	::PatBlt(pDC,rect.left, rect.top, abs(rect.left - rect.right),abs(rect.top-rect.bottom ),PATCOPY);   
	//Select back the old brush   
	::SelectObject(pDC,pOldBrush);   
	return TRUE;       
}

BOOL CRichEditUI::ConvertEMFToBMP(const TCHAR * pszEMFFile,const TCHAR* pszBMPFile,BOOL bScaleImage)
{
	//Prepare the BMP file name   
	TCHAR szBMPFile[255] = _T("");   
	_tcsncpy_s(szBMPFile, _countof(szBMPFile), pszBMPFile, _TRUNCATE);
	//Convert the EMF file to BMP File   
	BOOL bRet = ConvertToBMP(pszEMFFile,(const TCHAR*)szBMPFile,bScaleImage);   
	//if BMP conversion failed return false   
	if(bRet == FALSE)   
	{   
		return FALSE;   
	}else
	{
		CImage m_img;
		m_img.Load(_T("d:\\aaa.bmp"));
		m_img.Save(_T("d:\\aaa.jpg"));
	}
	return bRet ? TRUE:FALSE;   
}


void CRichEditUI::ClearClipList(std::list<IMG_CLIP_MSG *>& list)
{
	std::list<IMG_CLIP_MSG *>::iterator it = list.begin();
	while (it != list.end())
	{
		IMG_CLIP_MSG * pImg = *it;
		it=list.erase(it);
		delete pImg->picMsg;
		delete pImg;
	}
}

int CRichEditUI::GetTextCount( CString  sText )
{
	int nCountOfWord = 0, nCount = 0;  
	int i=0;  
	std::string strText = TStringToString(sText.GetBuffer(0));
	for(i=0;i<strText.size();i++)    
	{   
		if((BYTE)strText[i]>0x80)//if((BYTE)str[i]>= 0xa1 && (BYTE)str[i]<= 0xfe)  
		{  
			nCountOfWord++;  
			i++;  
		}
		else
		{
			nCount++;
		}
	}

	return nCount + nCountOfWord * 2;
}

CString CRichEditUI::GetTextByCount(CString sText, int nCount )
{
	CString strReText = sText;
	do 
	{
		if (nCount < 0) break;
		if (sText.IsEmpty()) break;
		int len = nCount;
		//取 setLength 个 字符的大小 纯汉字的话 就是nCount /2 这么大 纯字符的话 就是 nCount这么大
		int nLenght = 0;   //获取的总字符串长度
		int nGetLenght = 0; //已经获取的字符长度
		bool bIsFrist = true; //判断是否 第一个 字符
		int nCountOfWord = 0;
		int nCountOfChar = 0;

		std::string strText = TStringToString(sText.GetBuffer(0));
		for(int i=0;i<strText.size();i++)    
		{   
			if (nCountOfWord*2 + nCountOfChar == nCount )
			{
				strReText = sText.Left(nCountOfWord + nCountOfChar);
				break;
			}
			else if (nCountOfWord*2 + nCountOfChar > nCount)
			{
				strReText = sText.Left(nCountOfChar + nCountOfWord - 1);
				break;
			}
			if((BYTE)strText[i]>0x80)//if((BYTE)str[i]>= 0xa1 && (BYTE)str[i]<= 0xfe)  
			{  
				nCountOfWord++;  
				i++;  
			}
			else
			{
				nCountOfChar++;
			}
		}

	} while (0); 


	return strReText;
}

#endif
} // namespace DuiLib
