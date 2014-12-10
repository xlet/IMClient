#ifndef __IDRAGDROP_H__
#define __IDRAGDROP_H__
#pragma  once
// #include <ShlObj.h>
// #include <Shlwapi.h>
#include <algorithm>
namespace DuiLib
{
	class CControlUI;

	class  UILIB_API IDragSourceImpl:public IDropSource  
	{
	public:
		IDragSourceImpl();
		~IDragSourceImpl();

		// Methods of IUnknown  
		IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);  
		IFACEMETHODIMP_(ULONG) AddRef(void);  
		IFACEMETHODIMP_(ULONG) Release(void);  

		// Methods of IDropSource  
		IFACEMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);  
		IFACEMETHODIMP GiveFeedback(DWORD dwEffect);  
	private:
		 volatile LONG   m_lRefCount;        //!< The reference count  
	};


	class IDragDropEvent
	{
	public:
		virtual HRESULT DragEnter(IDataObject *pDataObj,DWORD grfKeyState,
			POINTL pt,POINT ptClient,DWORD *pdwEffect) = 0;

		virtual HRESULT DragOver(IDataObject *pDataObj,DWORD grfKeyState,
			POINTL pt,POINT ptClient,DWORD *pdwEffect) = 0;

		virtual HRESULT DragLeave(IDataObject *pDataObj) = 0;

		virtual HRESULT Drop(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,
			POINT ptClient,DWORD *pdwEffect) = 0;
	};	


	class IDropTargetImpl:public IDropTarget
	{
	public:
		typedef std::vector<CControlUI*>	VT_UI;
		IDropTargetImpl(HWND hParent);
		~IDropTargetImpl();
		void Register(CControlUI* pEvent);
		HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj,DWORD grfKeyState,
			POINTL pt,DWORD *pdwEffect);
		HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState,POINTL pt,
			DWORD *pdwEffect);
		HRESULT STDMETHODCALLTYPE DragLeave(void);
		HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj,DWORD grfKeyState,
			POINTL pt,DWORD *pdwEffect);
		CControlUI*	IsOverUI( POINTL* pt,POINT *ptClient );
		virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
		virtual ULONG _stdcall AddRef(void);
		virtual ULONG _stdcall Release(void);
	private:
		IDataObject*	m_pDataObj;
		HWND	m_hParent;
		VT_UI	m_vtUIs;
		ULONG	cRefs;
		POINTL	m_pt;
	};


	class CEnumFormatEtc : public IEnumFORMATETC
	{
	public:

		//
		// IUnknown members
		//
		HRESULT __stdcall  QueryInterface (REFIID iid, void ** ppvObject);
		ULONG	__stdcall  AddRef (void);
		ULONG	__stdcall  Release (void);

		//
		// IEnumFormatEtc members
		//
		HRESULT __stdcall  Next  (ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
		HRESULT __stdcall  Skip  (ULONG celt); 
		HRESULT __stdcall  Reset (void);
		HRESULT __stdcall  Clone (IEnumFORMATETC ** ppEnumFormatEtc);

		//
		// Construction / Destruction
		//
		CEnumFormatEtc(FORMATETC *pFormatEtc, int nNumFormats);
		~CEnumFormatEtc();

	private:

		LONG		m_lRefCount;		// Reference count for this COM interface
		ULONG		m_nIndex;			// current enumerator index
		ULONG		m_nNumFormats;		// number of FORMATETC members
		FORMATETC * m_pFormatEtc;		// array of FORMATETC objects
	};


	class UILIB_API CDataObject: public IDataObject
	{
		//
		// IUnknown members
		//
	public:
		HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject);
		ULONG   __stdcall AddRef (void);
		ULONG   __stdcall Release (void);

		//
		// IDataObject members
		//
		HRESULT __stdcall GetData				(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium);
		HRESULT __stdcall GetDataHere			(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium);
		HRESULT __stdcall QueryGetData			(FORMATETC *pFormatEtc);
		HRESULT __stdcall GetCanonicalFormatEtc (FORMATETC *pFormatEct,  FORMATETC *pFormatEtcOut);
		HRESULT __stdcall SetData				(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium,  BOOL fRelease);
		HRESULT __stdcall EnumFormatEtc			(DWORD      dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
		HRESULT __stdcall DAdvise				(FORMATETC *pFormatEtc,  DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
		HRESULT __stdcall DUnadvise				(DWORD      dwConnection);
		HRESULT __stdcall EnumDAdvise			(IEnumSTATDATA **ppEnumAdvise);

		//
		// Constructor / Destructor
		//
		CDataObject(FORMATETC *fmt, STGMEDIUM *stgmed, int count);
		~CDataObject();

	private:

		int LookupFormatEtc(FORMATETC *pFormatEtc);

		//
		// any private members and functions
		//
		LONG	   m_lRefCount;

		FORMATETC *m_pFormatEtc;
		STGMEDIUM *m_pStgMedium;
		LONG	   m_nNumFormats;

	};


	class UILIB_API CDropSource : public IDropSource
	{
	public:
		//
		// IUnknown members
		//
		HRESULT __stdcall QueryInterface	(REFIID iid, void ** ppvObject);
		ULONG   __stdcall AddRef			(void);
		ULONG   __stdcall Release			(void);

		//
		// IDropSource members
		//
		HRESULT __stdcall QueryContinueDrag	(BOOL fEscapePressed, DWORD grfKeyState);
		HRESULT __stdcall GiveFeedback		(DWORD dwEffect);

		//
		// Constructor / Destructor
		//
		CDropSource();
		~CDropSource();

	private:

		//
		// private members and functions
		//
		LONG	   m_lRefCount;
	};


	 HRESULT CreateEnumFormatEtc(UINT nNumFormats, FORMATETC *pFormatEtc, IEnumFORMATETC **ppEnumFormatEtc);
	extern"C" UILIB_API  HRESULT  CreateDropSource(IDropSource **ppDropSource);
	extern"C" UILIB_API  HRESULT CreateDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject);
}

#endif