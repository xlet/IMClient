//-------------------------------------------------------------------------
//  GifRichEdit - RichEdit Plus Control
//--------------------------------------------------------------------------
#include "StdAfx.h"
#include "OleHelper.h"

//////////////////////////////////////////////////////////////////////////
#define DEFINE_GUIDXXX(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID CDECL name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_GUIDXXX(IID_ITextDocument,0x8CC497C0,0xA1DF,0x11CE,0x80,0x98,
			   0x00,0xAA,0x00,0x47,0xBE,0x5D);
//////////////////////////////////////////////////////////////////////////

namespace DuiLib{

//////////////////////////////////////////////////////////////////////////
//
//

CImageDataObject::CImageDataObject() :
m_ulRefCnt(0)
, m_bRelease(FALSE)
{
	ZeroMemory(&m_stgmed, sizeof(STGMEDIUM));
	ZeroMemory(&m_fromat, sizeof(FORMATETC));
}

CImageDataObject::~CImageDataObject()
{
	if (m_bRelease)
		::ReleaseStgMedium(&m_stgmed);
}

HRESULT WINAPI CImageDataObject::QueryInterface(REFIID iid, void ** ppvObject)
{
	if (iid == IID_IUnknown || iid == IID_IDataObject)
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}

ULONG WINAPI CImageDataObject::AddRef(void)
{
	m_ulRefCnt++;
	return m_ulRefCnt;
}

ULONG WINAPI CImageDataObject::Release(void)
{
	if (--m_ulRefCnt == 0)
	{
		delete this;
	}

	return m_ulRefCnt;
}

HRESULT WINAPI CImageDataObject::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
	HANDLE hDst;
	hDst = ::OleDuplicateData(m_stgmed.hBitmap, CF_BITMAP, NULL);
	if (hDst == NULL)
	{
		return E_HANDLE;
	}

	pmedium->tymed = TYMED_GDI;
	pmedium->hBitmap = (HBITMAP)hDst;
	pmedium->pUnkForRelease = NULL;

	return S_OK;
}

HRESULT WINAPI CImageDataObject::GetDataHere(FORMATETC* pformatetc, STGMEDIUM*  pmedium)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::QueryGetData(FORMATETC*  pformatetc)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::GetCanonicalFormatEtc(FORMATETC*  pformatectIn, FORMATETC* pformatetcOut)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM*  pmedium, BOOL  fRelease)
{
	m_fromat = *pformatetc;
	m_stgmed = *pmedium;
	return S_OK;
}

HRESULT WINAPI CImageDataObject::EnumFormatEtc(DWORD  dwDirection , IEnumFORMATETC** ppenumFormatEtc)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageDataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
	return E_NOTIMPL;
}

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	if (hBitmap == NULL)
		return;

	STGMEDIUM stgm;
	stgm.tymed          = TYMED_GDI;
	stgm.hBitmap        = hBitmap;
	stgm.pUnkForRelease = NULL;

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;
	fm.ptd      = NULL;
	fm.dwAspect = DVASPECT_CONTENT;
	fm.lindex   = -1;
	fm.tymed    = TYMED_GDI;

	this->SetData(&fm, &stgm, TRUE);		
}

IOleObject *CImageDataObject::GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage)
{
	SCODE sc;
	IOleObject *pOleObject;
	sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, 
		&m_fromat, pOleClientSite, pStorage, (void **)&pOleObject);
	if (sc != S_OK)
		return NULL;

	return pOleObject;
}

BOOL CImageDataObject::InsertNotGIFImage( IRichEditOle* pRichEditOle, LPCTSTR szFilePath, DWORD dwUserData)
{
	HBITMAP hBitmap = NULL;
	if( !CPaintManagerUI::GetResourcePath().IsEmpty() ){  // 文件路径访问
		CDuiString pathTemp;
		//pathTemp = CPaintManagerUI::GetResourcePath();
		pathTemp += szFilePath;
		CAtlStringW strFilePath( pathTemp.GetData() );
		Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile( strFilePath.GetString() );
		Gdiplus::Status status = pBitmap->GetLastStatus();
		if( Ok == status )
		{
			status = pBitmap->GetHBITMAP( Color( 0, 0, 0 ), &hBitmap );
			if( Ok != status )
			{
				return FALSE;
			}
		}
		else
			return FALSE;

		_tcsncpy_s(m_szNotGifFile, szFilePath, MAX_PATH);
	}		
	else{ // 资源中访问
	}

	SCODE sc;
	CImageDataObject *pods = new CImageDataObject;
	LPDATAOBJECT lpDataObject;
	pods->QueryInterface(IID_IDataObject, (void **)&lpDataObject);

	pods->SetBitmap(hBitmap);

	IOleClientSite *pOleClientSite = NULL;
	pRichEditOle->GetClientSite(&pOleClientSite);

	IStorage *pStorage = NULL;

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		return FALSE;

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		lpLockBytes->Release();
		lpLockBytes = NULL;
		return FALSE;
	}

	IOleObject *pOleObject = NULL;
	pOleObject = pods->GetOleObject(pOleClientSite, pStorage);

	::OleSetContainedObject(pOleObject, TRUE);

	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);

	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	if (sc != S_OK)
		return FALSE;

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.pstg = pStorage;
	reobject.dwUser = dwUserData;

	pRichEditOle->InsertObject(&reobject);

	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();
	lpDataObject->Release();
	DeleteObject( hBitmap );

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//

CGifOleObject::CGifOleObject()
{
	m_ulRef = 0;
	m_pGifImage = NULL;
	m_puFrameElapse = NULL;
	m_nFrameCount = 0;
	m_nFramePosition = 0;
	m_pOleClientSite = NULL;
	m_hWndRichEdit = NULL;
	m_pOwner = NULL;

	ZeroMemory(m_szGifFile, MAX_PATH * sizeof(TCHAR));
}

CGifOleObject::~CGifOleObject()
{
	DeleteGif();
}

HRESULT WINAPI CGifOleObject::QueryInterface(REFIID iid, void ** ppvObject)
{
	if (iid == IID_IUnknown || iid == IID_IOleObject)
	{
		*ppvObject = (IOleObject *)this;
		((IOleObject *)(*ppvObject))->AddRef();
		return S_OK;
	}
	else if (iid == IID_IViewObject || iid == IID_IViewObject2)
	{
		*ppvObject = (IViewObject2 *)this;
		((IViewObject2 *)(*ppvObject))->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG WINAPI CGifOleObject::AddRef(void)
{
	m_ulRef++;
	return m_ulRef;
}

ULONG WINAPI CGifOleObject::Release(void)
{
	if (--m_ulRef == 0)
	{
		delete this;
		return 0;
	}

	return m_ulRef;
}

HRESULT WINAPI CGifOleObject::SetClientSite(IOleClientSite *pClientSite)
{
	m_pOleClientSite = pClientSite;
	return S_OK;
}

HRESULT WINAPI CGifOleObject::GetClientSite(IOleClientSite **ppClientSite)
{
	*ppClientSite = m_pOleClientSite;
	return S_OK;
}

HRESULT WINAPI CGifOleObject::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj)
{
	return S_OK;
}

HRESULT WINAPI CGifOleObject::Close(DWORD dwSaveOption)
{
	::KillTimer(m_hWndRichEdit, (UINT_PTR)this);

	DeleteGif();
	return S_OK;
}

HRESULT WINAPI CGifOleObject::SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::InitFromData(IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
{
	return S_OK;
}

HRESULT WINAPI CGifOleObject::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Update(void)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::IsUpToDate(void)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetUserClassID(CLSID *pClsid)
{
	*pClsid = IID_NULL;
	return S_OK;
}

HRESULT WINAPI CGifOleObject::GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::SetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Unadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::EnumAdvise(IEnumSTATDATA **ppenumAdvise)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::SetColorScheme(LOGPALETTE *pLogpal)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
							HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
							BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue)
{
	if (lindex != -1)
		return S_FALSE;

	RECT rcOleGif;
	rcOleGif.left   = lprcBounds->left;
	rcOleGif.top    = lprcBounds->top;
	rcOleGif.right  = lprcBounds->right;
	rcOleGif.bottom = lprcBounds->bottom;

	DrawFrame(hdcDraw, &rcOleGif);
	return S_OK;
}

HRESULT WINAPI CGifOleObject::GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
								   DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::Unfreeze(DWORD dwFreeze)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink)
{
	if (m_puFrameElapse)
	{
		long lElapse = m_puFrameElapse[m_nFramePosition];
		::SetTimer(m_hWndRichEdit, (UINT_PTR)this, lElapse, CGifOleObject::TimerProc);
	}

	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CGifOleObject::GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel)
{
	SIZE size;
	GetGifSize(&size);

	HDC hDC = ::GetDC(NULL);
	lpsizel->cx = ::MulDiv(size.cx + 2, 2540, GetDeviceCaps(hDC, LOGPIXELSX));
	lpsizel->cy = ::MulDiv(size.cy + 2, 2540, GetDeviceCaps(hDC, LOGPIXELSY));
	::ReleaseDC(NULL, hDC);

	return S_OK;
}

void CGifOleObject::SetRichEditHwnd(HWND hWnd)
{
	m_hWndRichEdit = hWnd;
}

HWND CGifOleObject::GetRichEditHwnd()
{
	return m_hWndRichEdit;
}

BOOL CGifOleObject::LoadGifFromFile(const TCHAR *lpszGifFile, const LPCTSTR sType)
{
	if (lpszGifFile == NULL || *lpszGifFile == NULL)
		return FALSE;

	DeleteGif();

	if(sType==NULL){	
		if( !CPaintManagerUI::GetResourcePath().IsEmpty() ){  // 文件路径访问
// 			CDuiString pathTemp;
// 			pathTemp = CPaintManagerUI::GetInstancePath();
// 			pathTemp += lpszGifFile;
// 			CAtlStringW strFilePath( pathTemp.GetData() );
// 			m_pGifImage = new Gdiplus::Image( strFilePath.GetString() );
// 			if (m_pGifImage == NULL)
// 				return FALSE;
// 
// 			_tcsncpy_s(m_szGifFile, lpszGifFile, MAX_PATH);
		}		
		else{ // 资源中访问
			
		}
		m_pGifImage = new Gdiplus::Image( lpszGifFile );
		if (m_pGifImage == NULL)
			return FALSE;
		_tcsncpy_s(m_szGifFile, lpszGifFile, MAX_PATH);
	}
	else{ // 从资源直接添加		
	}
	
	UINT nCount = 0;
	nCount = m_pGifImage->GetFrameDimensionsCount();
	
	GUID* pDimensionIDs = new GUID[nCount];
	m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
	
	m_nFrameCount = m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
	
	int nSize = m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
	Gdiplus::PropertyItem *pPropertyItem = (Gdiplus::PropertyItem *)malloc(nSize);
	m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, pPropertyItem);
	
	//实际上每两帧图像之间的时间间隔不一定相同
	m_puFrameElapse = new UINT[nSize];

	unsigned int i = 0;
	int temp = 0;
	for (i = 0;i < m_nFrameCount; i++)
	{
		  m_puFrameElapse[i] = ((long*)pPropertyItem->value)[i] * 10;
		  if ( m_puFrameElapse[i] <= 100)
		  {
			  m_puFrameElapse[i] = 100;
		  }
	}

	free(pPropertyItem);
	delete pDimensionIDs;

	return TRUE;
}

void CGifOleObject::GetGifSize(LPSIZE lpSize)
{
	if (lpSize == NULL || m_pGifImage == NULL)
		return;

	lpSize->cx = m_pGifImage->GetWidth();
	lpSize->cy = m_pGifImage->GetHeight();
}

UINT CGifOleObject::GetFrameCount()
{
	return m_nFrameCount;
}

void CGifOleObject::DeleteGif()
{
	::KillTimer(m_hWndRichEdit, (UINT_PTR)this);
	if (m_pGifImage)
	{
		
		delete m_pGifImage;
		m_pGifImage = NULL;
	}

	ZeroMemory(m_szGifFile, MAX_PATH * sizeof(TCHAR));
	if (m_puFrameElapse)
	{
		delete m_puFrameElapse;
		m_puFrameElapse = NULL;
	}

	m_nFrameCount = 0;
	m_nFramePosition = 0;
}

BOOL CGifOleObject::IsAnimatedGIF()
{
	return m_nFrameCount > 1;
}

void CGifOleObject::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != (UINT_PTR)this)
		return;

	::KillTimer(m_hWndRichEdit, (UINT_PTR)this);

	RECT rcOleObject;
	::SetRectEmpty(&rcOleObject);
	GetOleObjectRect(&rcOleObject);

	m_nFramePosition++;
	if (m_nFramePosition == m_nFrameCount)
		m_nFramePosition = 0;

	if (NULL != m_pOwner)
	{
		//光标会闪烁
		//	m_pOwner->Invalidate();
	}

	//方法二 光标会闪烁
	//InvalidateRect(m_hWndRichEdit, &rcOleObject, FALSE);
	//::PostMessage(m_hWndRichEdit, WM_PAINT, 0,0);

	//此种方法动态显示gif光标不会会闪烁
 	HDC hDC = ::GetDC(m_hWndRichEdit);
 	DrawFrame(hDC, &rcOleObject);
 	::ReleaseDC(m_hWndRichEdit, hDC);

	if (m_puFrameElapse)
	{
		long lElapse = m_puFrameElapse[m_nFramePosition];
		::SetTimer(m_hWndRichEdit, (UINT_PTR)this, lElapse, CGifOleObject::TimerProc);
	}
}

void CGifOleObject::GetOleObjectRect(LPRECT lpRect)
{
	if (lpRect == NULL || m_hWndRichEdit == NULL)
		return;

	IRichEditOle *pRichEditOle = NULL;
	CRichEditUI* pRichEdit = (CRichEditUI*)m_pOwner;
	//::SendMessage(m_hWndRichEdit, EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle);
	pRichEditOle =  pRichEdit->GetIRichEditOle();
	if (pRichEditOle == NULL)
		return;

	int nObjCount = pRichEditOle->GetObjectCount();
	int i = 0;
	for (i = 0;i < nObjCount;i++)
	{	
		REOBJECT reo;
		ZeroMemory(&reo, sizeof(REOBJECT));
		reo.cbStruct = sizeof(REOBJECT);
	
		HRESULT hr = pRichEditOle->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);
		if (hr != S_OK)
			continue;

		if (reo.poleobj)
			reo.poleobj->Release();

		if (reo.polesite)
			reo.polesite->Release();

		if (reo.pstg)
			reo.pstg->Release();

		if (reo.poleobj == (IOleObject *)this)
		{
			SIZE size;
			GetGifSize(&size);

			ITextDocument *pTextDocument = NULL;
			ITextRange *pTextRange = NULL;

			pRichEditOle->QueryInterface(IID_ITextDocument, (void **)&pTextDocument);
			if (!pTextDocument)
				return;

			long nLeft = 0;
			long nBottom = 0;
			pTextDocument->Range(reo.cp, reo.cp, &pTextRange);
			if (reo.dwFlags & REO_BELOWBASELINE)
				hr = pTextRange->GetPoint(TA_BOTTOM|TA_LEFT, &nLeft, &nBottom);
			else
				hr = pTextRange->GetPoint(TA_BASELINE|TA_LEFT, &nLeft, &nBottom);

			pTextDocument->Release();
			pTextRange->Release();

			RECT rectWindow;
			::GetWindowRect(m_hWndRichEdit, &rectWindow);

			lpRect->left   = nLeft - rectWindow.left;
			lpRect->bottom = nBottom - rectWindow.top;
			lpRect->right  = lpRect->left + size.cx + 2;
			lpRect->top    = lpRect->bottom - size.cy - 2;

			break;
		}
	}

	pRichEditOle->Release();
}

void CGifOleObject::DrawFrame(HDC hDC, LPRECT lpRect)
{
	if (m_pGifImage == NULL)
	{
		return;
	}
	GUID pageGuid = Gdiplus::FrameDimensionTime;

	long hmWidth = m_pGifImage->GetWidth();
	long hmHeight = m_pGifImage->GetHeight();

	Gdiplus::Graphics graphics(hDC);
	graphics.DrawImage(m_pGifImage, lpRect->left+1, lpRect->top+1, lpRect->right-lpRect->left-2, 
		lpRect->bottom-lpRect->top-2);

	m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
}

/* InsertGifRichEdit

Routine Description:

	在RICHEDIT编辑框控件中插入GIF图片

Arguments:

	hWnd           - RICHEDIT控件句柄
	pRichEditOle   - RICHEDIT OLE对象指针
	lpGifPath      - GIF图片文件位置

Return Value:
	
	如果成功,返回真(TRUE).如果失败,返回假(FALSE).
*/
BOOL CGifOleObject::InsertGifRichEdit(CControlUI* owner,HWND hWnd, IRichEditOle* pRichEditOle, 
	const TCHAR *lpGifPath, DWORD dwUserData, LPCTSTR sType)
{
	SCODE sc;

	IOleClientSite *pOleClientSite = NULL;
	pRichEditOle->GetClientSite(&pOleClientSite);

	IStorage *pStorage = NULL;

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		return FALSE;

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		lpLockBytes->Release();
		lpLockBytes = NULL;
		return FALSE;
	}

	CGifOleObject *pGifOleObject = new CGifOleObject;
	pGifOleObject->SetRichEditHwnd(hWnd);
	pGifOleObject->LoadGifFromFile(lpGifPath);
	pGifOleObject->m_pOwner = owner;

	IOleObject *pOleObject = NULL;
	pGifOleObject->QueryInterface(IID_IOleObject, (void **)&pOleObject);
	pOleObject->SetClientSite(pOleClientSite);

	HRESULT hr = ::OleSetContainedObject(pOleObject, TRUE);

	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);

	reobject.clsid    = CLSID_NULL;
	reobject.cp       = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj  = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.pstg     = pStorage;
	reobject.dwUser   = dwUserData;

	pRichEditOle->InsertObject(&reobject);

	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();

	return TRUE;
}

/*TimerProc

Routine Description:

	GIF动画定时器回调函数

Arguments:

	hWnd           - RICHEDIT控件句柄.
	uMsg           - WM_TIMER消息.
	idEvent        - 定时器标识,这里为CGifOleObject指针.
	dwTime         - 未使用
*/
VOID CALLBACK CGifOleObject::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CGifOleObject *pGifOleObject = (CGifOleObject *)idEvent;
	if (pGifOleObject)
		pGifOleObject->OnTimer(idEvent);
}

} // namespace DuiLib