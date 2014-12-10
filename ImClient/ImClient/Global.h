#ifndef GLOBAL_H_
#define  GLOBAL_H_
//部分 全局变量的声明
#include "User.h"

#include "RecentContacts.h"

#include "OfflineMsgs.h"

#include <map>

class CIMSocket;

extern CIMSocket* G_IMSOCKET;

extern CUser  g_user; 

//extern std::string   g_strtoken ;

typedef std::map<tString, HWND> MAP_TSTRING_HWND;
extern MAP_TSTRING_HWND g_mapWnd;

const extern tString  g_loginwndKey;

enum E_MEMBER_TYPE
{
	E_USERSELF = 0,   //  用户自己
	E_NORMALMEMBER    //  好友
};

void Utf8ToUnicode(WCHAR** dest,const char* src);

void Utf8ToUnicode(tString &strDest, const char* src);

void UnicodeToAnsi(char** dest,  const WCHAR* src);

void UnicodeToUtf8(char** dest , const WCHAR* src);

void AnsiToUtf8(char** dest, const char* src);

void Utf8ToAnsi(std::string& strDest, const char* src);

void Utf8ToAnsi(char** dest, const char* src);

void StringTotString(tString& tstrDest, const std::string& strSrc);

void StringTotString(tString& tstrDest, LPCSTR lpcSrc);

void tStringToString(std::string& strDest, const tString& tstrSrc);

void tStringToString(std::string& strDest, LPCTSTR lpctSrc);

void Utf8ToTChar(TCHAR** dest, const char* src);

void TcharToUtf8(char** dest, const TCHAR* src);

void TimestampToLocalTime(TCHAR* dest,int len, long tick);

void TimestampToLocalTime(char* dest, int len, long tick);

void GetMemberPicPath(tString & tstrPath, LPCTSTR lptDir, LPCTSTR lptUrl, LPCTSTR lptId);

void GetPicDir(LPTSTR lptStr, int nSize, E_MEMBER_TYPE type,  LPCTSTR lptId= NULL);
//void HandleString(string & str);


tString GetGrayPicName(LPCTSTR lptNormalName);

class CGlobalData
{
public:
	~CGlobalData();
private:
	CGlobalData();
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(CGlobalData::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //清理资源
	//防止被复制
	CGlobalData(const CGlobalData& );
	CGlobalData& operator=(const CGlobalData&);
	void Init();
	static const  CGlobalData*  m_pInstance;
	COfflineMsgs				m_offlineMsgs;
	RecentContactCollection     m_recentContacts;
public:
	//CRecentLinkmen    m_recentLinkmen;
	RecentContactCollection&  RecentContacts();
	COfflineMsgs& OfflinMessages();
	tString			  m_tstrCustomerServKey ;//= _T("CustomerserviceWnd");
	static   CGlobalData* GetInstance();
};

#endif