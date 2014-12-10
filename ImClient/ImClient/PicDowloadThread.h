#pragma once
#include "Thread.h"
#include "linkmaninfo.h"
typedef std::map<std::string, tString>  MAP_STRING_TSTRING ;

class PicInfo
{
public:
	MAP_STRING_TSTRING m_mapIdHasPic;
	MAP_STRING_TSTRING m_mapIdNotHadPic;
	HWND m_hWnd;
	//需要下载头像的联系人，对于客服新接受聊天并添加了联系人有用
	MAP_STRING_PLINKMANINFO m_linkmenMap;
};

enum E_SETPIC_TYPE
{
	E_USERPIC = 0,
	E_LINKMANPIC
};

class CPicDowloadThread:public CThread
{
public:
	CPicDowloadThread(E_SETPIC_TYPE type);
	~CPicDowloadThread(void);
private:
	CPicDowloadThread();
	BOOL Run();
	BOOL URLDownloadPicture(LPCTSTR lptUrl , LPCTSTR  lptPicPath, HINSTANCE hDll);
	void GetLinkmanPicPath(LPTSTR lptStr, int nSize, LPCTSTR lptUrl,  LPCTSTR lptFriendId);
	void GetUserPicDir(LPTSTR, int nSize);
	void GetDownloadUrl(/*MAP_STRING_TSTRING& mapUrl*/);
	//void GetDownloadUrlofUser();
	BOOL FindFile(LPCTSTR lptPath);
	void EncodeUrl(tString& tstrUrl);
	void DecodeUrl(tString& tstrUrl);
	E_SETPIC_TYPE   m_downloadType;
	TCHAR     m_tszDir[MAX_PATH];

};

void GetPicDir(LPTSTR lptStr, int nSize, E_SETPIC_TYPE type, LPCTSTR lptId = NULL);
BOOL GetLinkmanPicPath(tString & tstrPath, LPCTSTR lptId, LPCTSTR lptDir);
