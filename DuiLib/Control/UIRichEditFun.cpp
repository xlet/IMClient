#include "StdAfx.h"
#include "UIRichEditFun.h"

LPSTORAGE	CreateStorage()
{
	LPLOCKBYTES		lpLockBytes	= NULL;
	LPSTORAGE		lpStorage	= NULL;
	SCODE			sc;
	//Create lockbytes
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)	return 0;
	if( lpLockBytes == 0 )	return 0;
	//use lockbytes to create storage
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,0,&lpStorage);
	if (sc != S_OK)
	{
		if(lpLockBytes->Release() == 0)	return 0;
		return false;
	}
	return lpStorage;
}

void CreateNewGUID(LPTSTR *pOut)
{
	//static char buf[64] = {0};
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		*pOut = (LPTSTR)malloc(86*sizeof(TCHAR));
		if( *pOut )
		{
			snprintf(*pOut,86*sizeof(TCHAR)
				, _T("<picture:%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X>")
				, guid.Data1
				, guid.Data2
				, guid.Data3
				, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
		}
	}
}
bool		IsPictureGuid(LPTSTR pPic)
{
	if( pPic == 0 )	return false;
#if !defined(UNICODE)
	return strstr(pPic,_T("<picture:")) ? true : false;
#else
	return wcsstr(pPic,_T("<picture:")) ? true : false;
#endif
}

DWORD CALLBACK StreamOutRtf(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	StreamOutResult* pOutResult = static_cast<StreamOutResult*>((void*)dwCookie);
	if( pOutResult )
		pOutResult->appendValue((LPCTSTR)pbBuff,cb);
	*pcb=cb;
	return 0;
}

bool	getDropFilePath(IDataObject *pDataObj,vector<DuiLib::tString>& v)
{
#if 1
	FORMATETC		fm = { CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL };
	STGMEDIUM		stgMed = { TYMED_HGLOBAL, {0}, 0 };
	HRESULT hr = 0;
	HDROP hDrop = 0;
	LPTSTR pFile = 0;
	hr = pDataObj->GetData(&fm,&stgMed);
	if(FAILED(hr))	return false;
	hDrop = (HDROP)stgMed.hGlobal;

	
	//获取文件个数

	int nCount = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);

	for (int i = 0; i < nCount; i++)
	{
		//此时hr为文件长度
		int len = DragQueryFile(hDrop,i,0,0);
		//char pp[MAX_PATH] ={ 0};
		//memcpy(pp, hDrop, len);
	 
		if( (len == 0) || (len == 0xFFFFFFFF) )
		{
			if( stgMed.pUnkForRelease == 0)
				GlobalFree(stgMed.hGlobal);
			return 0;
		}
		pFile = new TCHAR[len+2];
		if( pFile == 0 )	
		{
			if( stgMed.pUnkForRelease == 0)
				GlobalFree(stgMed.hGlobal);
			return 0;
		}
		ZeroMemory(pFile,len+2);
		hr = DragQueryFile(hDrop,i,pFile,len+1);
		v.push_back(DuiLib::tString(pFile));

		delete[] pFile;
	}
	
	if( stgMed.pUnkForRelease == 0)
		GlobalFree(stgMed.hGlobal);
#endif
	return true;
}
bool	OleDataFormatPicEtc(DuiLib::CRichEditUI *pRe,LPCTSTR lpFilePath,LPTSTR pictureInfo,
	long lPos,DuiLib::atlCString& outVal)
{
	DuiLib::atlCString szType;
	LPTSTR token = strtok(pictureInfo,_T(" :<>"));
	int nNum = 0;
	while( token )
	{
		if(nNum % 2 == 0)
		{
			szType.Format(_T("%s"),token);
		}
		else if(nNum % 2 == 1)
		{
			if( szType == _T("picture") )
			{
				DuiLib::atlCString	OleInfo;
				OleInfo.Format(_T("<picture|%d|%s>"),lPos,token);
				outVal += OleInfo;
				pRe->SetSel(lPos,lPos+1);			//OLE对象占一个字符
				pRe->ReplaceSel(_T("   "),true);	//此操作相当于删除OLE对象	
			}
		}
		token = strtok(NULL,_T(" :<>"));
		nNum++;
	}
	return true;
}
void	EnumAllOleObject(IRichEditOle* pRe,EnumOleObjectByRange *pEnum)
{
	if( pRe == 0 )		return ;
	if( pEnum == 0 )	return ;
	long  lCount = pRe->GetObjectCount();
	HRESULT hr = 0;
	bool bContinued = false;
	for(int n=0;n < lCount;n++)
	{
		REOBJECT tgObj;
		tgObj.cbStruct = sizeof(REOBJECT);
		hr = pRe->GetObject(n,&tgObj,REO_GETOBJ_ALL_INTERFACES);
		if( hr != S_OK )	continue;
		bContinued = pEnum->OnOleObject(tgObj.poleobj,tgObj.clsid,tgObj.cp);
		if( tgObj.poleobj )
			tgObj.poleobj->Release();
		if( tgObj.polesite )
			tgObj.polesite->Release();
		if( tgObj.pstg )
			tgObj.pstg->Release();
		if( bContinued == false)
			break;
	}
}
void EnumOleObjectByPos(IRichEditOle* pRe,CHARRANGE& pRange,EnumOleObjectByRange *pEnum)
{
	if( pRe == 0 )		return ;
	if( pEnum == 0 )	return ;
	long  lCount = pRe->GetObjectCount();
	HRESULT hr = 0;
	bool bContinued = false;
	for(int n=0;n < lCount;n++)
	{
		REOBJECT tgObj;
		tgObj.cbStruct = sizeof(REOBJECT);
		hr = pRe->GetObject(n,&tgObj,REO_GETOBJ_ALL_INTERFACES);
		if( hr != S_OK )	continue;
		if( (pRange.cpMin <= tgObj.cp) && (pRange.cpMax >= tgObj.cp) )
		{
			bContinued = pEnum->OnOleObject(tgObj.poleobj,tgObj.clsid,tgObj.cp);
			if( tgObj.poleobj )
				tgObj.poleobj->Release();
			if( tgObj.polesite )
				tgObj.polesite->Release();
			if( tgObj.pstg )
				tgObj.pstg->Release();
			if( bContinued == false)
				break;
		}
	}
}
