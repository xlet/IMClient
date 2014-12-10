#include "StdAfx.h"
#include "BaseMember.h"
#include "SourceDownloadThread.h"

#include "Utils.h"
#include "Sqlite.h"
#include "FriendGroup.h"
#include "PictureProcess.h"

std::map<tString,HANDLE>  mapPicToThread;

std::set<tString>  setPicDownloaded;

CRITICAL_SECTION_EX  CSourceDownloadThread::s_cs;

CRITICAL_SECTION_EX PicDownloadThreadEx::s_cs;

//set<tString> Picset;

#define  USE_WININET_DOWNLOAD_

#ifdef USE_WININET_DOWNLOAD_

typedef bool  (/*_stdcall*/ *http_downloadfile) (const char* ,const char* ,const char* , bool, void(*)(unsigned long , unsigned long));

#else 
#include <UrlMon.h>
#pragma comment(lib, "urlmon.lib")
#endif



//查找文件是否存在
BOOL FindFile(LPCTSTR lptPath)
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
#ifdef _DEBUG
		int i = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE;
		//	printf ("The first file found is %s/n", FindFileData.cFileName);  
#endif 
		FindClose(hFind);  
		return TRUE;  
	}  
}

SrcPara::SrcPara()
{
	m_bFirstTimeToDownlod = FALSE;
}

SrcPara::~SrcPara()
{

}

PicDwonloadPara::PicDwonloadPara()
{
	memset(m_tszDir, MAX_PATH, 0x0);
}

PicDwonloadPara::~PicDwonloadPara()
{

}

PicDownloadThreadEx::PicDownloadThreadEx()
{

}

PicDownloadThreadEx::~PicDownloadThreadEx()
{

}

BOOL PicDownloadThreadEx::URLDownloadPicture(LPCTSTR lptUrl, LPCTSTR lptPicPath, HINSTANCE hDll)
{
#ifdef USE_WININET_DOWNLOAD_

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
	if (FindFile(lptPicPath))
	{
		return TRUE;
	}
	
	


	bool bRet =   downloadfile(strUrl.c_str(), NULL, strFileName.c_str(), false, NULL);
	return bRet?TRUE:FALSE;
	
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

BOOL PicDownloadThreadEx::Run()
{
//	InitializeCriticalSectionExEx(&s_cs);
	CPictureProcess grayProcess;
	PicDwonloadPara* p = (PicDwonloadPara*)m_pPara;
	HINSTANCE hDll = NULL;
#ifdef USE_WININET_DOWNLOAD_
	#ifdef _DEBUG
		hDll = LoadLibrary(_T("HttpDownload_d.dll"));
	#else
		hDll = LoadLibrary(_T("HttpDownload.dll"));
	#endif
#endif

		for (std::map<tString, vector<tString>>::iterator it = p->m_mapPicInfo.begin(); it != p->m_mapPicInfo.end(); ++it)
		{
			tString tstrPicPath;
			//GetMemberPicPath(tstrPicPath, p->m_tszDir, it->second.c_str(), it->first.c_str());
			GetMemberPicPath(tstrPicPath, p->m_tszDir,it->first.c_str(), NULL);

			BOOL bRet  = FALSE;
			//Lock(&s_cs);
			s_cs.Lock();
			set<tString>::iterator itFind = setPicDownloaded.find(tstrPicPath);
			if (itFind != setPicDownloaded.end())
			{
				bRet = TRUE;
				if (!FindFile(GetGrayPicName(tstrPicPath.c_str()).c_str()))
				{
					bRet = FALSE;
				}
				
				//UnLock(&s_cs);
				s_cs.UnLock();
			}
			else
			{
				setPicDownloaded.insert(tstrPicPath);
				//UnLock(&s_cs);
				s_cs.UnLock();
				bRet = URLDownloadPicture(it->first.c_str(), tstrPicPath.c_str(), hDll);
				if (bRet)
				{
					//变灰
#ifdef _UNICODE
					grayProcess.ToGray(tstrPicPath.c_str());
#else
					WCHAR* wstrFileName = NULL;
					C2W(&wstrFileName, tstrPicPath.c_str());
					grayProcess.ToGray(wstrFileName);
					SAFE_ARRYDELETE(wstrFileName);
#endif
				}
				

			}

			if (bRet)
			{
// 				Lock(&s_cs);
// 				setPicDownloaded.insert(tstrPicPath);
// 				UnLock(&s_cs);


				// 下载成功
				//it->second=tstrPicPath;
			for (vector<tString>::iterator itId = it->second.begin(); itId != it->second.end(); ++itId)
				{
					//map<id, 图片路径>
					p->m_mapDownInfo.insert(make_pair(*itId , tstrPicPath));
				}
			}
			else
			{
				//setPicDownloaded.erase(tstrPicPath);
				//下载失败
				//it->second = _T("");
				for (vector<tString>::iterator itId = it->second.begin(); itId != it->second.end(); ++itId)
				{
					// 男性or女性
					string str = FriendList::GetInstance()->GetFriendList().at(p->m_tstrGroupId)->GetFriendGroupInfo().GetMemberMap().at(*itId)->m_strSex;
					BOOL bFemale = FriendList::GetInstance()->GetFriendList().at(p->m_tstrGroupId)->GetFriendGroupInfo().GetMemberMap().at(*itId)->m_strSex == "Female";
					if (bFemale)
						p->m_mapDownInfo.insert(make_pair(*itId, /*it->first*/_T("female.png")));
					else
						p->m_mapDownInfo.insert(make_pair(*itId, /*it->first*/_T("male.png")));
				}
			}
		}
#ifdef USE_WININET_DOWNLOAD_
		FreeLibrary(hDll);
#endif
		// 下载完了通知界面
		SendMessage(p->m_hWnd, WM_SETDOWNLOADPICTURE, /*E_NORMALMEMBER*/p->type, (LPARAM)this);
		
	return FALSE;
}

CSourceDownloadThread::CSourceDownloadThread()
{

}


CSourceDownloadThread::~CSourceDownloadThread(void)
{
	
}

//遍历文件夹，是否存在当前Member的图片文件
BOOL CSourceDownloadThread::IsMemberHasPic(BaseMember* pMember, LPCTSTR lptDir)
{
// 	TCHAR tszFind[MAX_PATH + 1] = {0x0};
// 	_tcscpy_s(tszFind, MAX_PATH, lptDir);
// 	_tcscat_s(tszFind, _T("\\*.*"));
// 	WIN32_FIND_DATA wfd;
// 	BOOL bRet = FALSE;
// 	HANDLE hFind = FindFirstFile(tszFind, &wfd);
// 	if (INVALID_HANDLE_VALUE == hFind)
// 	{
// 		OutputDebugString(_T("Cant't find file, Error:INVALID_HANDLE_VALUE\n"));
// 		return;
// 	}
// 	tString tstrNewUrl;
// 	bRet = TRUE;
// 
// 	//遍历当前文件夹中所有文件
// 	while (bRet)
// 	{
// 
// 	}
	return TRUE;
}

// void CSourceDownloadThread::GetPicDir(LPTSTR lptStr, int nSize, E_MEMBER_TYPE type, LPCTSTR lptId)
// {
// 	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
// 	TCHAR tszDir[MAX_PATH] = {0x0};
// 	BOOL bRet = pIni->ReadString(_T("file Directory"), _T("path"), NULL, tszDir, MAX_PATH);
// 
// 
// 	TCHAR tszPath[MAX_PATH] = {0x0};
// 	tString tstrUserId;
// 	if (lptId == NULL)
// 		StringTotString(tstrUserId, g_user.UerSelf()->m_strId);
// 	else
// 		tstrUserId =  lptId;
// 
// 	
// 	Replace(tstrUserId, _T(":"), _T(" "));
// 	if(type == E_NORMALMEMBER)
// 		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("FriendsFace"));
// 	else
// 		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("user"));
// }


void  CSourceDownloadThread::GetPicDownloadThreadPara(SrcPara* pSrcPara)
{
	//获取当前图片的存储路径,好友和用户在不同的文件目录
	//TCHAR tszDir[MAX_PATH + 1] = {0x0};
	GetPicDir(m_tszDir, MAX_PATH, pSrcPara->type);
	if (pSrcPara->type == E_USERSELF)
	{	
		//用户自己的头像需要下载
		//PicDwonloadPara* pPara = new PicDwonloadPara;
		pSrcPara->type = E_USERSELF;

		tString tstrId;
		StringTotString(tstrId, g_user.UerSelf()/*m_pConnResponse->m_pUserSelf*/->m_strId);

		tString tstrUrl;
		StringTotString(tstrUrl, g_user./*m_pConnResponse->m_pUserSelf*/UerSelf()->m_strPicUrl);
		tString tstrPath;
		GetMemberPicPath(tstrPath, m_tszDir, tstrUrl.c_str(), NULL);
		if (tstrPath == _T(""))
		{
			//男性女性，
			if (g_user./*m_pConnResponse->m_pUserSelf*/UerSelf()->m_strSex == "Female")
				pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(tstrId, _T("female.png")));
			else
				pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(tstrId, _T("male.png")));
			//pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(tstrId, tstrPath));
		}
		else if (FindFile(tstrPath.c_str()))
		{
			//TODO:
			pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(tstrId, tstrPath));
		}
		else
		{
			vector<tString> v;
		
			v.push_back(tstrId);
			pSrcPara->m_picInfo.m_mapPicToId.insert(make_pair(tstrUrl, v));
		}
	}
	else
	{
		//获取对应组member的Map
		map<tString, BaseMember*>& mapMember = /*FriendList::GetInstance()->GetFriendList().at(pSrcPara->m_tstrId)->GetFriendGroupInfo().GetMemberMap()*/FriendList::GetInstance()->GetMemberMap(pSrcPara->m_tstrId);

		//假设当前目录中存在所有好友的头像
		for (map<tString, BaseMember*>::iterator it = mapMember.begin(); it != mapMember.end(); ++it)
		{
			tString tstrURl;
			StringTotString(tstrURl, it->second->m_strPicUrl);
			pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(it->first, tstrURl));
		}

		tString tstrPicName;
		for (MAP_TSTRING_TSTRING::iterator it = pSrcPara->m_picInfo.m_mapHasPic.begin();
			it != pSrcPara->m_picInfo.m_mapHasPic.end(); )
		{
			GetMemberPicPath(tstrPicName, m_tszDir, it->second.c_str(), it->first.c_str());
			BOOL bFemale = FALSE;
			if (FriendList::GetInstance()->GetFriendList().at(pSrcPara->m_tstrId)->GetFriendGroupInfo().GetMemberMap().at(it->first)->m_strSex == "Female")
				bFemale = TRUE;
			//tstrPicName   为空说明url 给的地址为空或者不对 
			if (tstrPicName == _T(""))
			{
#if 0
				//男性女性，
				if (FriendList::GetInstance()->GetFriendList().at(pSrcPara->m_tstrId)->GetFriendGroupInfo().GetMemberMap().at(it->first)->m_strSex == "Female")
				//	pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(it->first, _T("female.png")));
					it->second = _T("female.png");
				else
					//pSrcPara->m_picInfo.m_mapHasPic.insert(make_pair(it->first, _T("male.png")));
					it->second = _T("male.png");
				//从m_mapHasPic中删除（排除法）
				//pSrcPara->m_picInfo.m_mapHasPic.erase(it++);
#endif
#if 1
				bFemale?it->second = _T("female.png"):it->second = _T("male.png");
#endif
				++it;
				continue;
			}

			if (FindFile(tstrPicName.c_str()))
			{
				if (!FindFile(GetGrayPicName(tstrPicName.c_str()).c_str()))
					bFemale?it->second = _T("female.png"):it->second = _T("male.png");
				else
				{
					//找到当前的URL对应的图片
					//将URL改变为图片路径
					tString tstrPath;
					GetMemberPicPath(tstrPath, m_tszDir, it->second.c_str(), it->first.c_str());
					it->second = tstrPath;
					
				}
				++it;
			}
			else
			{
				//没有找到当前Url对应的图片,需要下载
				//TODO:需要判断是否删除旧图片
				map<tString, vector<tString>>& picToId = pSrcPara->m_picInfo.m_mapPicToId;

				map<tString, vector<tString>>::iterator itFind =picToId.find(it->second);
//				set<tString>::iterator itFind1 = Picset.find(it->second);

 				if (itFind != picToId.end())
 				{//说明已经有好友的头像是该图片URL, 该图片URL被重复使用
 					itFind->second.push_back(it->first);
 				}
 				else 
 				{
 					vector<tString> v;
 					v.push_back(it->first);
 					picToId.insert(make_pair(it->second, v));
//  					Lock(&g_cs);
//  					Picset.insert(it->second);
//  					UnLock(&g_cs);
 				}

				pSrcPara->m_picInfo.m_mapHasPic.erase(it++);
							
				// //没有找到当前URL对用的图片
				// pSrcPara->m_picInfo.m_mapNotHasPic.insert(*it);
				// //从m_mapHasPic中删除（排除法）
				// 	pSrcPara->m_picInfo.m_mapHasPic.erase(it++);
			}
			tstrPicName = _T("");

		}
	}
	
	//向界面发送消息更换图片
	if (pSrcPara->m_picInfo.m_mapHasPic.size() > 0 )
		SendMessage(pSrcPara->m_hWnd, WM_SETEXISTINGPICTURE, pSrcPara->type,  (LPARAM)this);

 //// 开启多个线程进行下载
 ////  初始化多线程的一些参数
	
	
	//如果存在多个好友使用同一张图片的情况那么有可能在多线程中多个线程同时打开了一个文件并往里写内容，
	//这样会导致文件内容出错所以使用：映射<图片URL, 好友ID>
	

	std::map<tString, vector<tString>>::iterator it =pSrcPara->m_picInfo.m_mapPicToId.begin();

	// 注意MAXIMUM_WAIT_OBJECTS 为最大等待线程数
	// 控制线程数在MAXIMUM_WAIT_OBJECTS 以内， 默认每个线程处理的task为MUTITHREAD_DWOONLOAD_NUM（5）
	int nTasks = MUTITHREAD_DWOONLOAD_NUM; 
	int nSize = pSrcPara->m_picInfo.m_mapPicToId.size();
	if (nSize / MUTITHREAD_DWOONLOAD_NUM > MAXIMUM_WAIT_OBJECTS)
		nTasks = nSize/MAXIMUM_WAIT_OBJECTS + 1; 
	for (int i = 0; it!=pSrcPara->m_picInfo.m_mapPicToId.end() ; ++i)
	{
		PicDwonloadPara* pPara = new PicDwonloadPara;
		for (int j = 0; j < nTasks &&it!=pSrcPara->m_picInfo.m_mapPicToId.end(); ++j)
		{
			pPara->m_mapPicInfo.insert(make_pair(it->first, it->second));
			_tcscpy(pPara->m_tszDir, m_tszDir);
			++it;
		}
		pPara->m_hWnd = pSrcPara->m_hWnd;
		pPara->m_tstrGroupId = pSrcPara->m_tstrId;
		PicDownloadThreadEx* pThread = new PicDownloadThreadEx;
		pPara->type = pSrcPara->type;
		pThread->SetPara((UINT_PTR)pPara);
		m_vThread.push_back(pThread);
	}	
}

BOOL CSourceDownloadThread::Run()
{
	//	PicDwonloadPara* pPara
	//m_pPara是当前组联系人指针
//	InitializeCriticalSectionExEx(&s_cs);
	SrcPara* p = (SrcPara*)m_pPara;

	GetPicDownloadThreadPara(p);
	for (vector<PicDownloadThreadEx*>::iterator it = m_vThread.begin(); it!=m_vThread.end(); ++it)
	{
// 		PicDwonloadPara* pPara = (PicDwonloadPara*)((*it)->GetPara());
// 		Lock(&s_cs);
// 		for (std::map<tString, vector<tString>>::iterator itPicToId = pPara->m_mapPicInfo.begin(); itPicToId != pPara->m_mapPicInfo.end(); ++itPicToId)
// 		{
// 			// insert 并不能插入重复的
// 			mapPicToThread.insert(make_pair(itPicToId->first, (*it)->GetHandle()));
// 		}
// 		UnLock(&s_cs);
		(*it)->StartThread();
	}

	int nThreadSize = m_vThread.size();
	HANDLE* pHandle = new HANDLE[nThreadSize];
	for (int i = 0; i < nThreadSize; ++i)
	{
		pHandle[i] = m_vThread[i]->GetHandle();
	}

	// 等待所有线程结束此时可以对线程资源进行释放
	//WaitForMultipleObjects(nThreadSize, pHandle, TRUE, WSA_INFINITE );
// 	for (vector<PicDownloadThreadEx*>::iterator it = m_vThread.begin(); it != m_vThread.end(); ++it)
// 	{
// 		PicDownloadThreadEx* p = *it;
// 		SAFE_DELETE(p);
// 	}

	return FALSE;
}

// 
// // 根据用户Id ， URL获取当前用户pic的路径, 注意id中如果存在":"， 替换成空格
// void GetMemberPicPath(tString & tstrPath, LPCTSTR lptDir, LPCTSTR lptUrl, LPCTSTR lptId)
// {
// #if 0 // 根据url id 方式命名
// 	tString tstrUrl = lptUrl;
// 	EncodeUrl(tstrUrl);
// 	TCHAR tszId[30] = {0x0};
// 	tString tstrId = lptId;
// 	Replace(tstrId, _T(":"), _T(" "));
// 	TCHAR tszPicName[MAX_FILENAME_SIZE + 1] = {0x0};
// 	_stprintf_s(tszPicName, _T("%s\\[%s]#%s"), lptDir, tstrId.c_str(), tstrUrl.c_str());
// 	tstrPath  = tszPicName;
// #endif 
// 	
// 	//tstrPath = _T("");
//   LPCTSTR lptFind = _tcsrchr(lptUrl, _T('/')) ;
// 
//   if (lptFind != NULL)
//   {
// 	  tString tstrId =  lptFind + 1;
// 	  Replace(tstrId, _T(":"), _T(" "));
// 	  TCHAR tszPicName[MAX_FILENAME_SIZE + 1] = {0x0};
// 	  _stprintf_s(tszPicName, _T("%s\\%s"), lptDir,tstrId.c_str() );
// 	  tstrPath = tszPicName;
//   }
// }
