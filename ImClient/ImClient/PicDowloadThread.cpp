#include "StdAfx.h"
#include "PicDowloadThread.h"

#include "Sqlite.h"
#include "Global.h"
#include "Utils.h"
#define USE_WININET_DOWNLOAD   //是否使用Winnet下载

#ifdef USE_WININET_DOWNLOAD

	typedef BOOL  (*http_downloadfile) (const char* ,const char* ,const char* , bool, void(*)(unsigned long , unsigned long));

#else 
	#include <UrlMon.h>
	#pragma comment(lib, "urlmon.lib")
#endif

CPicDowloadThread::CPicDowloadThread(E_SETPIC_TYPE type)
{
	memset(m_tszDir, MAX_PATH, 0x0);
	//if (type == E_LINKMANPIC)
	GetPicDir(m_tszDir, MAX_PATH, type);
	m_downloadType = type;

}


CPicDowloadThread::~CPicDowloadThread(void)
{
}

BOOL CPicDowloadThread::Run()
{
	//tString tstrUrl;
	//tString tstrFriendId;
	PicInfo* pInfo = (PicInfo*)m_pPara;
	//TCHAR  tszPicPath[MAX_PATH] = {0x0};

	GetDownloadUrl();

	// 向 界面发送消息将已经有的头像图片贴上去
	PostMessage(pInfo->m_hWnd, WM_SETHEADPICTURE, MAKELPARAM(m_downloadType, 0), (LPARAM)this);

	HINSTANCE hDll = NULL;
#ifdef USE_WININET_DOWNLOAD
	#ifdef _DEBUG
		hDll = LoadLibrary(_T("HtttpDownload_d.dll"));
	#else
		hDll = LoadLibrary(_T("HtttpDownload.dll"));
	#endif
#endif
	for (map<std::string, tString>::iterator it = pInfo->m_mapIdNotHadPic.begin(); it != pInfo->m_mapIdNotHadPic.end(); it++)
	{
		TCHAR tszPicPath[MAX_PATH] = {0x0};
		LPTSTR lptId = NULL;
		C2T(&lptId, it->first.c_str());
		GetLinkmanPicPath(tszPicPath, MAX_PATH,it->second.c_str(), lptId);

		if (URLDownloadPicture(it->second.c_str(), tszPicPath, hDll))
		
			it->second = tszPicPath;
		else
			//下载失败
			it->second = _T("");
		SAFE_ARRYDELETE(lptId);
	}	
#ifdef USE_WININET_DOWNLOAD
	FreeLibrary(hDll);
#endif
	//下载的图片
	PostMessage(pInfo->m_hWnd, WM_SETHEADPICTURE, MAKELPARAM(m_downloadType, 1), (LPARAM)this);
	return FALSE;
}

//图片命名格式：[联系人ID]#编码后的图片URL路径#

void CPicDowloadThread::GetLinkmanPicPath(LPTSTR tszPicName, int nSize,LPCTSTR lptUrl,  LPCTSTR lptFriendId)
{
//	GetFriendFacePicDir(tszPicName, MAX_PATH);
//  获取url后缀，即图片格式
// 	int len = _tcslen(lptUrl);
// 
// 	int suffixlen =0;
// 	LPCTSTR p;
// 
// 	for (p=lptUrl+len-1; p!=lptUrl; p--)
// 	{
// 		suffixlen++;
// 		if (*p == _T('.'))
// 		{
// 			break;
// 		}
// 	}
// 
// 	TCHAR* tstrSuff = new TCHAR[suffixlen+1];
// 	_tcscpy(tstrSuff,p);
	tString tstrUrl = lptUrl;
	EncodeUrl(tstrUrl);
	wsprintf(tszPicName, _T("%s\\[%s]#%s"), m_tszDir, lptFriendId, tstrUrl.c_str());

	//SAFE_ARRYDELETE(tstrSuff);
}

BOOL CPicDowloadThread::URLDownloadPicture(LPCTSTR lptUrl, LPCTSTR lptPicPath, HINSTANCE hDll)
{
#ifdef USE_WININET_DOWNLOAD
	
	//hDll = LoadLibrary(_T("HtttpDownload.dll"));
	if (hDll == 0 )
	{
		return FALSE;
	}
	http_downloadfile downloadfile = (http_downloadfile)(GetProcAddress(hDll, "http_downloadfile"));
	if (downloadfile == NULL)
	{
		return FALSE;
	}
	std::string strUrl, strFileName;
	tStringToString(strUrl, lptUrl);
	tStringToString(strFileName, lptPicPath);
	return downloadfile(strUrl.c_str(), NULL, strFileName.c_str(), FALSE, NULL);
#else 
	HRESULT	hr = URLDownloadToFile(NULL, lptUrl, lptPicPath, 0, 0);
	if (hr != S_OK)
	{
		OutputDebugString(lptUrl);
		OutputDebugString(_T("Picture download failed \n"));
		return FALSE;
	}
	else
		return TRUE;
#endif
	
	
}

BOOL CPicDowloadThread::FindFile(LPCTSTR lptPath)
{
	WIN32_FIND_DATA FindFileData;  
	HANDLE hFind;  

	hFind = FindFirstFile(lptPath, &FindFileData);  
	if (hFind == INVALID_HANDLE_VALUE )  
	{  
		//printf ("Invalid File Handle. GetLastError reports %d/n", GetLastError ());  
		return FALSE;  
	}  
	else  
	{ 
		int i = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE;
		//	printf ("The first file found is %s/n", FindFileData.cFileName);  
		FindClose(hFind);  
		return TRUE;  
	}  
}

#if 0
void CPicDowloadThread::GetDownloadUrlofUser()
{
	TCHAR tszFind[MAX_PATH] = {0x0};
	_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
	_tcscat_s(tszFind, _T("\\*.*"));
	WIN32_FIND_DATA wfd;
	BOOL bRet = FALSE;
	PicInfo* pPicInfo = (PicInfo*)m_pPara;
	HANDLE hFind = FindFirstFile(tszFind, &wfd);

	tString tstrNewUrl ;
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	StringTotString(tstrNewUrl, g_user.m_selfInfo.thumb);

		pPicInfo->m_mapIdNotHadPic.insert(make_pair(g_user.m_selfInfo.id, tstrNewUrl));
	while (bRet)
	{
		bRet = FindNextFile(hFind, &wfd);
		if (!bRet)
		{
			break;
		}

		_tcscpy_s(tszFind, wfd.cFileName);
		//获取该图片的联系人Id
		tString& tstrId = GetBetweenString(tszFind, _T('['), _T(']'));
		std::string userId ;
		tStringToString(userId, tstrId);
		if (userId == g_user.m_selfInfo.id)
		{
			//图片路径中解析出来的路径
			tString tstrOldUrl = GetBetweenString(tszFind, _T('#'), _T('\0'));
			tString tstrPicPath;
			DecodeUrl(tstrOldUrl);

			if (tstrOldUrl == tstrNewUrl)
			{
				_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
				_tcscat_s(tszFind, _T("\\"));
				_tcscat_s(tszFind, wfd.cFileName);
				pPicInfo->m_mapIdHasPic.insert(make_pair(g_user.m_selfInfo.id, tszFind));
				pPicInfo->m_mapIdNotHadPic.clear();
			}
			else
			{

			}
			break;
		}
	}
	

}
#endif

void CPicDowloadThread::GetDownloadUrl(/*MAP_STRING_TSTRING& mapUrl*/)
{
// 	MAP_STRING_PLINKMANINFO& mapLinkmen = CGlobalData::GetInstance()->m_recentLinkmen.m_linkmenMap;
// 	int nSize = mapLinkmen.size();
// 	TCHAR tszFind[MAX_PATH] = {0x0};
// 	_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
// 	_tcscat_s(tszFind, _T("\\*.*"));
// 	WIN32_FIND_DATA wfd;
// 	BOOL bRet = FALSE;
// 	HANDLE hFind = FindFirstFile(tszFind, &wfd);
// 	if (INVALID_HANDLE_VALUE == hFind)
// 	{
// 		OutputDebugString(_T("Cant't find file , Error:INVALID_HANDLE_VALUE\n"));
// 		return;
// 	}
// 
// 	tString tstrNewUrl ;
// 	bRet = TRUE;
// 	PicInfo* pPicInfo = (PicInfo*)m_pPara;
// 	//----
// 	if (m_downloadType == E_LINKMANPIC)
// 	{
// 		for (MAP_STRING_PLINKMANINFO::iterator it = mapLinkmen.begin(); it != mapLinkmen.end(); ++it)
// 		{
// 			StringTotString(tstrNewUrl, it->second->thumb);
// 			pPicInfo->m_mapIdNotHadPic.insert(make_pair(it->first, tstrNewUrl));
// 		}
// 
// 		while (bRet)
// 		{
// 			bRet = FindNextFile(hFind, &wfd);
// 			if (!bRet)
// 			{
// 				break;
// 			}
// 			// 		_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
// 			// 		_tcscat_s(tszFind, _T("\\"));
// 			_tcscpy_s(tszFind, wfd.cFileName);
// 			//tstrFileName = tszFind;
// 
// 			//获取该图片的联系人Id
// 			tString& tstrId = GetBetweenString(tszFind, _T('['), _T(']'));
// 			std::string strLinkmanId ;
// 			tStringToString(strLinkmanId, tstrId);
// 
// 			//在联系人中进行查找
// 			MAP_STRING_PLINKMANINFO::iterator itLinkman = mapLinkmen.find(strLinkmanId);
// 			if (itLinkman != mapLinkmen.end())
// 			{
// 				//图片路径中解析出来的路径
// 				tString tstrOldUrl = GetBetweenString(tszFind, _T('#'), _T('\0'));
// 
// 				DecodeUrl(tstrOldUrl);
// 
// 				StringTotString(tstrNewUrl, itLinkman->second->thumb);
// 
// 				_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
// 				_tcscat_s(tszFind, _T("\\"));
// 				_tcscat_s(tszFind, wfd.cFileName);
// 
// 				//URL对比
// 				if (tstrNewUrl == tstrOldUrl)
// 				{
// 					//图片没发生变化
// 					pPicInfo->m_mapIdNotHadPic.erase(strLinkmanId);
// 
// // 					_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
// // 					_tcscat_s(tszFind, _T("\\"));
// // 					_tcscat_s(tszFind, wfd.cFileName);
// 					pPicInfo->m_mapIdHasPic.insert(make_pair(strLinkmanId, tszFind));
// 				}
// 				else  //tstrNewUrl != tstrOldUrl 
// 				{
// 					//在新下载图片前删掉已经有的图片
// 					DeleteFile(tszFind);
// 				}
// 
// 			}
// 
// 		}
// 	}
// 	else if (m_downloadType == E_USERPIC)
// 	{
// 		StringTotString(tstrNewUrl, g_user.m_selfInfo.thumb);
// 
// 		pPicInfo->m_mapIdNotHadPic.insert(make_pair(g_user.m_selfInfo.id , tstrNewUrl));
// 
// 		while (bRet)
// 		{
// 			bRet = FindNextFile(hFind, &wfd);
// 			if (!bRet)
// 			{
// 				break;
// 			}
// 
// 			_tcscpy_s(tszFind, wfd.cFileName);
// 			//获取该图片的联系人Id
// 			tString& tstrId = GetBetweenString(tszFind, _T('['), _T(']'));
// 			std::string userId ;
// 			tStringToString(userId, tstrId);
// 			if (userId == g_user.m_selfInfo.id)
// 			{
// 				//图片路径中解析出来的路径
// 				tString tstrOldUrl = GetBetweenString(tszFind, _T('#'), _T('\0'));
// 				tString tstrPicPath;
// 				DecodeUrl(tstrOldUrl);
// 
// 				if (tstrOldUrl == tstrNewUrl)
// 				{
// 					_tcscpy_s(tszFind, MAX_PATH, m_tszDir);
// 					_tcscat_s(tszFind, _T("\\"));
// 					_tcscat_s(tszFind, wfd.cFileName);
// 					pPicInfo->m_mapIdHasPic.insert(make_pair(g_user.m_selfInfo.id, tszFind));
// 					pPicInfo->m_mapIdNotHadPic.clear();
// 				}
// 				else
// 				{
// 
// 				}
// 				break;
// 			}
// 		}
// 	}
	

}

BOOL GetLinkmanPicPath(tString & tstrPath,  LPCTSTR lptId, LPCTSTR lptDir)
{

	TCHAR tszFind[MAX_PATH] = {0x0};
	_tcscpy_s(tszFind, MAX_PATH, lptDir);
	_tcscat_s(tszFind, _T("\\*.*"));
	WIN32_FIND_DATA wfd;
	BOOL bRet = FALSE;
	HANDLE hFind = FindFirstFile(tszFind, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		OutputDebugString(_T("Cant't find file , Error:INVALID_HANDLE_VALUE\n"));
		return FALSE;
	}

	bRet = TRUE;
	while (bRet)
	{
		bRet = FindNextFile(hFind, &wfd);
		if (!bRet)
		{
			break;
		}

		_tcscpy_s(tszFind, wfd.cFileName);
		//获取该图片的联系人Id
		tString& tstrId = GetBetweenString(tszFind, _T('['), _T(']'));

		if (_tcsicmp(lptId, tstrId.c_str()) == 0)
		{
			_tcscpy_s(tszFind, MAX_PATH, lptDir);
			_tcscat_s(tszFind, _T("\\"));
			_tcscat_s(tszFind, wfd.cFileName);
			tstrPath = tszFind;
			return TRUE;
		}
	}
	FindClose(hFind);
	return FALSE;
}

void /*CPicDowloadThread::*/GetPicDir(LPTSTR lptStr, int nSize, E_SETPIC_TYPE type,  LPCTSTR lptId/* = NULL*/)
{
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszDir[MAX_PATH] = {0x0};
	BOOL bRet = pIni->ReadString(_T("file Directory"), _T("path"), NULL, tszDir, MAX_PATH);


	TCHAR tszPath[MAX_PATH] = {0x0};
	tString tstrUserId;
	if (lptId == NULL)
		StringTotString(tstrUserId, g_user.m_selfInfo.id);
	else
		tstrUserId =  lptId;
	if(type == E_LINKMANPIC)
		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("FriendsFace"));
	else
		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("user"));
}

void CPicDowloadThread::GetUserPicDir(LPTSTR, int nSize)
{
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszDir[MAX_PATH] = {0x0};
	BOOL bRet = pIni->ReadString(_T("file Directory"), _T("path"), NULL, tszDir, MAX_PATH);

}

//文件名中不能包含一些 \ / : * ? " < > | "
void CPicDowloadThread::EncodeUrl(tString& tstrUrl)
{
	Replace(tstrUrl, _T(":"), _T("&mh"));
	Replace(tstrUrl, _T("\\"), _T("&fxg"));
	Replace(tstrUrl, _T("/"), _T("&xg"));
	Replace(tstrUrl, _T("*"), _T("&xh"));
	Replace(tstrUrl, _T("?"), _T("&wh"));
	Replace(tstrUrl, _T("<"), _T("&xyh"));
	Replace(tstrUrl, _T(">"), _T("&dyh"));
	Replace(tstrUrl, _T("|"), _T("&sg"));
	//Replace(tstrUrl, _T("\""), _T("&yh"))
}

void CPicDowloadThread::DecodeUrl(tString& tstrUrl)
{
	Replace(tstrUrl, _T("&mh"),  _T(":"));
	Replace(tstrUrl, _T("&fxg"), _T("\\"));
	Replace(tstrUrl, _T("&xg"), _T("/"));
	Replace(tstrUrl, _T("&xh"), _T("*"));
	Replace(tstrUrl, _T("&wh"), _T("?"));
	Replace(tstrUrl, _T("&xyh"), _T("<"));
	Replace(tstrUrl, _T("&dyh"), _T(">"));
	Replace(tstrUrl, _T("&sg"), _T("|"));
	//Replace(tstrUrl, _T("&yh"), _T("\""))
}