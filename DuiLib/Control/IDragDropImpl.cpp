#include "StdAfx.h"
#include "IDragDropImpl.h"
namespace DuiLib {


IDragSourceImpl::IDragSourceImpl(void)  
{  
	m_lRefCount = 1;  
} 

IDragSourceImpl::~IDragSourceImpl(void)  
{  
	m_lRefCount = 0;  
} 

STDMETHODIMP IDragSourceImpl::QueryInterface(REFIID riid, void **ppv)  
{  
	static const QITAB qit[] =  
	{  
		QITABENT(IDragSourceImpl, IDropSource),  
		{ 0 }  
	};  

	return QISearch(this, qit, riid, ppv);  
}  

STDMETHODIMP_(ULONG) IDragSourceImpl::AddRef()  
{  
	return InterlockedIncrement(&m_lRefCount);  
}  

STDMETHODIMP_(ULONG) IDragSourceImpl::Release()  
{  
	ULONG lRef = InterlockedDecrement(&m_lRefCount);  
	if (0 == lRef)  
	{  
		delete this;  
	}  
	return m_lRefCount;  
}  

STDMETHODIMP IDragSourceImpl::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)  
{  
	if( TRUE == fEscapePressed )  
	{  
		return DRAGDROP_S_CANCEL;  
	}  
 
	// If the left button of mouse is released  
	if( 0 == (grfKeyState & (MK_LBUTTON) )  )
	{  
		return DRAGDROP_S_DROP;  
	}  

	return S_OK;  
}  


STDMETHODIMP IDragSourceImpl::GiveFeedback(DWORD dwEffect)  
{  
	UNREFERENCED_PARAMETER(dwEffect);  
	return DRAGDROP_S_USEDEFAULTCURSORS;  
} 

//-----------------------------------------------------------------------------------------------------//

//
//	Helper function to perform a "deep" copy of a FORMATETC
//
static void DeepCopyFormatEtc(FORMATETC *dest, FORMATETC *source)
{
	// copy the source FORMATETC into dest
	*dest = *source;

	if(source->ptd)
	{
		// allocate memory for the DVTARGETDEVICE if necessary
		dest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));

		// copy the contents of the source DVTARGETDEVICE into dest->ptd
		*(dest->ptd) = *(source->ptd);
	}
}

//
//	Constructor 
//
CEnumFormatEtc::CEnumFormatEtc(FORMATETC *pFormatEtc, int nNumFormats)
{
	m_lRefCount   = 1;
	m_nIndex      = 0;
	m_nNumFormats = nNumFormats;
	m_pFormatEtc  = new FORMATETC[nNumFormats];

	// copy the FORMATETC structures
	for(int i = 0; i < nNumFormats; i++)
	{	
		DeepCopyFormatEtc(&m_pFormatEtc[i], &pFormatEtc[i]);
	}
}

//
//	Destructor
//
CEnumFormatEtc::~CEnumFormatEtc()
{
	if(m_pFormatEtc)
	{
		for(ULONG i = 0; i < m_nNumFormats; i++)
		{
			if(m_pFormatEtc[i].ptd)
				CoTaskMemFree(m_pFormatEtc[i].ptd);
		}

		delete[] m_pFormatEtc;
	}
}

//
//	IUnknown::AddRef
//
ULONG __stdcall CEnumFormatEtc::AddRef(void)
{
	// increment object reference count
	return InterlockedIncrement(&m_lRefCount);
}

//
//	IUnknown::Release
//
ULONG __stdcall CEnumFormatEtc::Release(void)
{
	// decrement object reference count
	LONG count = InterlockedDecrement(&m_lRefCount);

	if(count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

//
//	IUnknown::QueryInterface
//
HRESULT __stdcall CEnumFormatEtc::QueryInterface(REFIID iid, void **ppvObject)
{
	// check to see what interface has been requested
	if(iid == IID_IEnumFORMATETC || iid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
}

//
//	IEnumFORMATETC::Next
//
//	If the returned FORMATETC structure contains a non-null "ptd" member, then
//  the caller must free this using CoTaskMemFree (stated in the COM documentation)
//
HRESULT __stdcall CEnumFormatEtc::Next(ULONG celt, FORMATETC *pFormatEtc, ULONG * pceltFetched)
{
	ULONG copied  = 0;

	// validate arguments
	if(celt == 0 || pFormatEtc == 0)
		return E_INVALIDARG;

	// copy FORMATETC structures into caller's buffer
	while(m_nIndex < m_nNumFormats && copied < celt)
	{
		DeepCopyFormatEtc(&pFormatEtc[copied], &m_pFormatEtc[m_nIndex]);
		copied++;
		m_nIndex++;
	}

	// store result
	if(pceltFetched != 0) 
		*pceltFetched = copied;

	// did we copy all that was requested?
	return (copied == celt) ? S_OK : S_FALSE;
}

//
//	IEnumFORMATETC::Skip
//
HRESULT __stdcall CEnumFormatEtc::Skip(ULONG celt)
{
	m_nIndex += celt;
	return (m_nIndex <= m_nNumFormats) ? S_OK : S_FALSE;
}

//
//	IEnumFORMATETC::Reset
//
HRESULT __stdcall CEnumFormatEtc::Reset(void)
{
	m_nIndex = 0;
	return S_OK;
}

//
//	IEnumFORMATETC::Clone
//
HRESULT __stdcall CEnumFormatEtc::Clone(IEnumFORMATETC ** ppEnumFormatEtc)
{
	HRESULT hResult;

	// make a duplicate enumerator
	hResult = CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);

	if(hResult == S_OK)
	{
		// manually set the index state
		((CEnumFormatEtc *) *ppEnumFormatEtc)->m_nIndex = m_nIndex;
	}

	return hResult;
}


//////////////////////////////////////////////////////////////////////////

//
//	"Drop-in" replacement for SHCreateStdEnumFmtEtc. Called by CDataObject::EnumFormatEtc
//
HRESULT CreateEnumFormatEtc(UINT nNumFormats, FORMATETC *pFormatEtc, IEnumFORMATETC **ppEnumFormatEtc)
{
	if(nNumFormats == 0 || pFormatEtc == 0 || ppEnumFormatEtc == 0)
		return E_INVALIDARG;

	*ppEnumFormatEtc = new CEnumFormatEtc(pFormatEtc, nNumFormats);

	return (*ppEnumFormatEtc) ? S_OK : E_OUTOFMEMORY;
}

IDropTargetImpl::IDropTargetImpl(HWND hParent)
	:m_hParent(hParent),cRefs(0),m_pDataObj(0)
{

}

IDropTargetImpl::~IDropTargetImpl()
{
	m_hParent = 0;
	cRefs	  = 0;
	m_vtUIs.clear();
}

void IDropTargetImpl::Register(CControlUI* pEvent)
{
	VT_UI::iterator vtItem = std::find(m_vtUIs.begin(),m_vtUIs.end(),pEvent);
	if( vtItem != m_vtUIs.end() )
		*vtItem = pEvent;
	else
		m_vtUIs.push_back(pEvent);
}


CControlUI* IDropTargetImpl::IsOverUI( POINTL* pt,POINT *pClient )
{
	RECT	tgRc;
	int nCount = m_vtUIs.size();
	if( pt == 0 )	return 0;
	POINT tgPt = {pt->x,pt->y};
	::ScreenToClient(m_hParent,&tgPt);
	for(int n=0;n < nCount;n++)
	{
		if( m_vtUIs[n] )
		{
			tgRc = m_vtUIs[n]->GetPos();
			if( PtInRect(&tgRc,tgPt) )
			{
				if( pClient )
					memcpy(pClient,&tgPt,sizeof(POINT));
				return m_vtUIs[n];
			}
		}
	}
	return 0;
}


HRESULT STDMETHODCALLTYPE IDropTargetImpl::DragEnter(IDataObject *pDataObj,
	DWORD grfKeyState,POINTL pt,
	DWORD *pdwEffect)
{
	POINT ptClient;
	CControlUI* pUI = IsOverUI(&pt,&ptClient);
	m_pDataObj = pDataObj;
	memcpy(&m_pt,&pt,sizeof(POINTL));
	if( pUI )
	{
		IDragDropEvent *pEvent = pUI->getDragEvent();
		if( pEvent )
			return pEvent->DragEnter(pDataObj,grfKeyState,pt,ptClient,pdwEffect);
	}
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IDropTargetImpl::DragOver(DWORD grfKeyState,POINTL pt,
	DWORD *pdwEffect)
{
	POINT ptClient;
	CControlUI* pUI = IsOverUI(&pt,&ptClient);
	memcpy(&m_pt,&pt,sizeof(POINTL));
	if( pUI )
	{
		IDragDropEvent *pEvent = pUI->getDragEvent();
		if( pEvent )
			return pEvent->DragOver(m_pDataObj,grfKeyState,pt,ptClient,pdwEffect);
	}
	return S_OK;
}


HRESULT STDMETHODCALLTYPE IDropTargetImpl::DragLeave(void)
{
	POINT ptClient;
	CControlUI* pUI = IsOverUI(&m_pt,&ptClient);
	if( pUI )
	{
		IDragDropEvent *pEvent = pUI->getDragEvent();
		if( pEvent )
			return pEvent->DragLeave(m_pDataObj);
	}
	m_pDataObj = 0;
	memset(&m_pt,0,sizeof(POINTL));	
	return S_OK;
}


HRESULT STDMETHODCALLTYPE IDropTargetImpl::Drop(IDataObject *pDataObj,
	DWORD grfKeyState,POINTL pt,
	DWORD *pdwEffect)
{
	POINT ptClient;
	CControlUI* pUI = IsOverUI(&pt,&ptClient);
	memcpy(&m_pt,&pt,sizeof(POINTL));
	if( pUI )
	{
		IDragDropEvent *pEvent = pUI->getDragEvent();
		if( pEvent )
			return pEvent->Drop(pDataObj,grfKeyState,pt,ptClient,pdwEffect);
	}
	return S_OK;
}


HRESULT  IDropTargetImpl::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject = NULL;
	if(IsEqualIID(riid,IID_IUnknown)||IsEqualIID(riid,IID_IDropTarget)) 
	{
		AddRef();
		*ppvObject = (IDropTarget *) this;
		hr = S_OK;
	}
	return hr;
}
ULONG	 IDropTargetImpl::AddRef(void)
{
	return ++cRefs;
}
ULONG    IDropTargetImpl::Release(void)
{
	ULONG c_Refs = --cRefs;
	if (c_Refs == 0)
	{
		delete this;
	}
	return c_Refs;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//
//	Constructor
//
CDataObject::CDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmed, int count) 
{
	m_lRefCount  = 1;
	m_nNumFormats = count;

	m_pFormatEtc  = new FORMATETC[count];
	m_pStgMedium  = new STGMEDIUM[count];

	for(int i = 0; i < count; i++)
	{
		m_pFormatEtc[i] = fmtetc[i];
		m_pStgMedium[i] = stgmed[i];
	}
}

CDataObject::~CDataObject()
{
	if(m_pFormatEtc) delete[] m_pFormatEtc;
	if(m_pStgMedium) delete[] m_pStgMedium;
}

ULONG __stdcall CDataObject::AddRef(void)
{
	// increment object reference count
	return InterlockedIncrement(&m_lRefCount);
}

ULONG __stdcall CDataObject::Release(void)
{
	// decrement object reference count
	LONG count = InterlockedDecrement(&m_lRefCount);

	if(count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

//
//	IUnknown::QueryInterface
//
HRESULT __stdcall CDataObject::QueryInterface(REFIID iid, void **ppvObject)
{
	// 
	if(iid == IID_IDataObject || iid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
}

HGLOBAL DupMem(HGLOBAL hMem)
{
	// lock the source memory object
	DWORD   len    = GlobalSize(hMem);
	PVOID   source = GlobalLock(hMem);

	// create a fixed "global" block - i.e. just
	// a regular lump of our process heap
	PVOID   dest   = GlobalAlloc(GMEM_FIXED, len);

	memcpy(dest, source, len);

	GlobalUnlock(hMem);

	return dest;
}


int CDataObject::LookupFormatEtc(FORMATETC *pFormatEtc)
{
	for(int i = 0; i < m_nNumFormats; i++)
	{
		if((pFormatEtc->tymed    &  m_pFormatEtc[i].tymed)   &&
			pFormatEtc->cfFormat == m_pFormatEtc[i].cfFormat && 
			pFormatEtc->dwAspect == m_pFormatEtc[i].dwAspect)
		{
			return i;
		}
	}

	return -1;

}


//
//	IDataObject::GetData
//
HRESULT __stdcall CDataObject::GetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
	int idx;

	//
	// try to match the requested FORMATETC with one of our supported formats
	//
	if((idx = LookupFormatEtc(pFormatEtc)) == -1)
	{
		return DV_E_FORMATETC;
	}

	//
	// found a match! transfer the data into the supplied storage-medium
	//
	pMedium->tymed			 = m_pFormatEtc[idx].tymed;
	pMedium->pUnkForRelease  = 0;

	switch(m_pFormatEtc[idx].tymed)
	{
	case TYMED_HGLOBAL:

		pMedium->hGlobal = DupMem(m_pStgMedium[idx].hGlobal);
		//return S_OK;
		break;

	default:
		return DV_E_FORMATETC;
	}

	return S_OK;
}


//
//	IDataObject::GetDataHere
//
HRESULT __stdcall CDataObject::GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
	// GetDataHere is only required for IStream and IStorage mediums
	// It is an error to call GetDataHere for things like HGLOBAL and other clipboard formats
	//
	//	OleFlushClipboard 
	//
	return DATA_E_FORMATETC;
}


//
//	IDataObject::QueryGetData
//
//	Called to see if the IDataObject supports the specified format of data
//
HRESULT __stdcall CDataObject::QueryGetData (FORMATETC *pFormatEtc)
{
	return (LookupFormatEtc(pFormatEtc) == -1) ? DV_E_FORMATETC : S_OK;
}

//
//	IDataObject::GetCanonicalFormatEtc
//
HRESULT __stdcall CDataObject::GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut)
{
	// Apparently we have to set this field to NULL even though we don't do anything else
	pFormatEtcOut->ptd = NULL;
	return E_NOTIMPL;
}


//
//	IDataObject::SetData
//
HRESULT __stdcall CDataObject::SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease)
{
	return E_NOTIMPL;
}
//
//	IDataObject::EnumFormatEtc
//
HRESULT __stdcall CDataObject::EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
	if(dwDirection == DATADIR_GET)
	{
		// for Win2k+ you can use the SHCreateStdEnumFmtEtc API call, however
		// to support all Windows platforms we need to implement IEnumFormatEtc ourselves.
		return CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);
	}
	else
	{
		// the direction specified is not support for drag+drop
		return E_NOTIMPL;
	}
}


//
//	IDataObject::DAdvise
//
HRESULT __stdcall CDataObject::DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
//	IDataObject::DUnadvise
//
HRESULT __stdcall CDataObject::DUnadvise (DWORD dwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}


//
//	IDataObject::EnumDAdvise
//
HRESULT __stdcall CDataObject::EnumDAdvise (IEnumSTATDATA **ppEnumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
//	Helper function
//
HRESULT CreateDataObject (FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject)
{
	if(ppDataObject == 0)
		return E_INVALIDARG;

	*ppDataObject = new CDataObject(fmtetc, stgmeds, count);

	return (*ppDataObject) ? S_OK : E_OUTOFMEMORY;
}

//--------------------------------------------------------------------------------------------------------------------//

//
//	Constructor
//
CDropSource::CDropSource() 
{
	m_lRefCount = 1;
}

//
//	Destructor
//
CDropSource::~CDropSource()
{
}

//
//	IUnknown::AddRef
//
ULONG __stdcall CDropSource::AddRef(void)
{
	// increment object reference count
	return InterlockedIncrement(&m_lRefCount);
}

//
//	IUnknown::Release
//
ULONG __stdcall CDropSource::Release(void)
{
	// decrement object reference count
	LONG count = InterlockedDecrement(&m_lRefCount);

	if(count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

//
//	IUnknown::QueryInterface
//
HRESULT __stdcall CDropSource::QueryInterface(REFIID iid, void **ppvObject)
{
	// check to see what interface has been requested
	if(iid == IID_IDropSource || iid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
}

//
//	CDropSource::QueryContinueDrag
//
//	Called by OLE whenever Escape/Control/Shift/Mouse buttons have changed
//
HRESULT __stdcall CDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	// if the <Escape> key has been pressed since the last call, cancel the drop
	if(fEscapePressed == TRUE)
		return DRAGDROP_S_CANCEL;	

	// if the <LeftMouse> button has been released, then do the drop!
	if((grfKeyState & MK_LBUTTON) == 0)
		return DRAGDROP_S_DROP;

	// continue with the drag-drop
	return S_OK;
}

//
//	CDropSource::GiveFeedback
//
//	Return either S_OK, or DRAGDROP_S_USEDEFAULTCURSORS to instruct OLE to use the
//  default mouse cursor images
//
HRESULT __stdcall CDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

//
//	Helper routine to create an IDropSource object
//	
HRESULT CreateDropSource(IDropSource **ppDropSource)
{
	if(ppDropSource == 0)
		return E_INVALIDARG;

	*ppDropSource = new CDropSource();

	return (*ppDropSource) ? S_OK : E_OUTOFMEMORY;

}
}