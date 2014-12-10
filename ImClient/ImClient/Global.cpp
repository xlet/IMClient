#include "StdAfx.h"
#include "Global.h"
#include <time.h>
#include "Sqlite.h"
#include "Utils.h"
CIMSocket* G_IMSOCKET = NULL;

CUser  g_user; 
//std::string   g_strtoken = "";

//登录窗口的key值是0x0,其余对应的聊天
//窗口的key的值是当前聊天对象的ID值
MAP_TSTRING_HWND g_mapWnd;

const tString  g_loginwndKey = _T("LoginWnd");



const CGlobalData* CGlobalData::m_pInstance = new CGlobalData;

void Utf8ToUnicode(WCHAR** dest,const char* src)
{
	ASSERT(dest!= NULL || src != NULL);
	int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 ) + 1;  
	
	*dest = new WCHAR[unicodeLen];
	//memset(*dest, 0x0, (unicodeLen + 1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, src, -1, *dest, unicodeLen);

}

#ifdef _UNICODE
void Utf8ToUnicode(tString &strDest, const char* src)
{
	WCHAR* str = NULL;
	Utf8ToUnicode(&str, src);
	strDest =  str;
	SAFE_ARRYDELETE(str);
}

#endif

void UnicodeToAnsi(std::string & strDest, const WCHAR* src)
{
	char* str = NULL;
	W2C(&str, src);
	strDest = str;
	SAFE_ARRYDELETE(str);
}

 void UnicodeToAnsi(char** dest,  const WCHAR* src)
 {
 	ASSERT(dest!= NULL || src != NULL);
	W2C(dest, src);
 }

 void UnicodeToUtf8(char** dest , const WCHAR* src)
 {
	  ASSERT(dest!= NULL || src != NULL);
	  int len = -1;
	  len = WideCharToMultiByte(CP_UTF8, 0, src, -1, 0, 0, 0, 0)+1;
	  *dest = new char[len+1];
	  ::WideCharToMultiByte(CP_UTF8, 0, src, -1,*dest, len, 0, 0);
 }


 void AnsiToUtf8(char** dest, const char* src)
 {
	ASSERT(dest!= NULL || src != NULL);
	WCHAR* pwszStr = NULL;
	C2W(&pwszStr, src);
	UnicodeToUtf8(dest, pwszStr);
	SAFE_ARRYDELETE(pwszStr);
 }

 void Utf8ToAnsi(char** dest, const char* src)
 {
	 ASSERT(dest!= NULL || src != NULL);
	 WCHAR* str = NULL;
	 Utf8ToUnicode(&str, src);
	 W2C(dest, str);
	 SAFE_ARRYDELETE(str);
 }

 void Utf8ToAnsi(std::string& strDest, const char* src)
 {
	  ASSERT(src != NULL);
	  char* str = NULL;
	  Utf8ToAnsi(&str, src);
	  strDest = str;
	  SAFE_ARRYDELETE(str);

 }

 void StringTotString(tString& tstrDest, const std::string& strSrc)
 {
	 TCHAR* pwszStr = NULL;
	 C2T(&pwszStr, strSrc.c_str());
	 tstrDest = pwszStr;
	 SAFE_ARRYDELETE(pwszStr);
 }

 void StringTotString(tString& tstrDest, LPCSTR lpcSrc)
 {
	 TCHAR* pwszStr = NULL;
	 C2T(&pwszStr, lpcSrc);
	 tstrDest = pwszStr;
	 SAFE_ARRYDELETE(pwszStr);
 }

 void tStringToString(std::string& strDest, const tString& tstrSrc)
 {
	 char* pszStr = NULL;
	 T2C(&pszStr, tstrSrc.c_str());
	 strDest = pszStr;
	 SAFE_ARRYDELETE(pszStr);
 }

 void tStringToString(std::string& strDest, LPCTSTR lpctSrc)
 {
	  char* pszStr = NULL;
	 T2C(&pszStr, lpctSrc);
	 strDest = pszStr;
	 SAFE_ARRYDELETE(pszStr);
 }

 void Utf8ToTChar(TCHAR** dest, const char* src)
 {
#ifdef _UNICODE
	Utf8ToUnicode(dest, src);
#else
	 //多字节
	 Utf8ToAnsi(dest, src);
#endif 
 }

 void TcharToUtf8(char** dest, const TCHAR* src)
 {
#ifdef _UNICODE
	 UnicodeToUtf8(dest, src);
#else
	 AnsiToUtf8(dest, src);
#endif
 }
 

void TimestampToLocalTime(TCHAR* dest,int len, long tick)
{
//*dest = new WCHAR[24];
// 	if (dest == NULL)
// 	{
// 		return ;
// 	}
// 	time_t tm_t = tick;
// 	char str[24];
// 	struct tm tm_;
// 	tm_ = *localtime((time_t*)&tm_t);
// 	
// 	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm_);
	
	//char str[24];
	//TimestampToLocalTime(str, 24, tick);
	//C2W(dest, str);
	time_t tm_t = tick;
	//char str[24];
	struct tm tm_;
	tm_ = *localtime((time_t*)&tm_t);
	//_tcsftime(dest, len, _T("%Y-%m-%d %H:%M:%S"), &tm_);
	_tcsftime(dest, len, _T("%H:%M:%S"), &tm_);
}

void TimestampToLocalTime(char* dest, int len, long tick)
{
	if (dest == NULL)
	{
		return ;
	}
	time_t tm_t = tick;
	//char str[24];
	struct tm tm_;
	tm_ = *localtime((time_t*)&tm_t);
	
	strftime(dest, len, "%Y-%m-%d %H:%M:%S", &tm_);

}


 //存进sqlite中的一些字符串需要加上转移字符
//  void HandleString(string & str)
//  {
// 	 char* str1 = "'";
// 	 char* str2 = "''";
// 	 string::size_type  pos = 0;
// 	 while((pos = str.find(str1, pos)) != string::npos)
// 	 {
// 		 str.replace(pos, __strlen(str1), str2 );
// 		 pos += __strlen(str2);
// 	 }
//  }
CGlobalData::CGarbo CGlobalData:: Garbo;  //清理资源

  CGlobalData::CGlobalData()
  {
	  Init();
  }

  CGlobalData::~CGlobalData()
  {

  }

  //一些需要用到的全局变量的初始化
  void CGlobalData::Init()
  {
	    m_tstrCustomerServKey = _T("CustomerserviceWnd");
  }

   CGlobalData* CGlobalData::GetInstance()
  {
	  return  (CGlobalData*)m_pInstance;
  }

  COfflineMsgs& CGlobalData::OfflinMessages()
  {
		return m_offlineMsgs;
  }

  RecentContactCollection& CGlobalData::RecentContacts()
  {
	  return m_recentContacts;
  }
  
// 根据用户Id ， URL获取当前用户pic的路径, 注意id中如果存在":"， 替换成空格
void GetMemberPicPath(tString & tstrPath, LPCTSTR lptDir, LPCTSTR lptUrl, LPCTSTR lptId)
{
#if 0 // 根据url id 方式命名
	tString tstrUrl = lptUrl;
	EncodeUrl(tstrUrl);
	TCHAR tszId[30] = {0x0};
	tString tstrId = lptId;
	Replace(tstrId, _T(":"), _T(" "));
	TCHAR tszPicName[MAX_FILENAME_SIZE + 1] = {0x0};
	_stprintf_s(tszPicName, _T("%s\\[%s]#%s"), lptDir, tstrId.c_str(), tstrUrl.c_str());
	tstrPath  = tszPicName;
#endif 
	
	//tstrPath = _T("");
  LPCTSTR lptFind = _tcsrchr(lptUrl, _T('/')) ;

  if (lptFind != NULL)
  {
	  tString tstrId =  lptFind + 1;
	  Replace(tstrId, _T(":"), _T(" "));
	  TCHAR tszPicName[MAX_FILENAME_SIZE + 1] = {0x0};
	  _stprintf_s(tszPicName, _T("%s\\%s"), lptDir,tstrId.c_str() );
	  tstrPath = tszPicName;
  }
}

void GetPicDir(LPTSTR lptStr, int nSize, E_MEMBER_TYPE type, LPCTSTR lptId)
{
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszDir[MAX_PATH] = {0x0};
	BOOL bRet = pIni->ReadString(_T("file Directory"), _T("path"), NULL, tszDir, MAX_PATH);


	TCHAR tszPath[MAX_PATH] = {0x0};
	tString tstrUserId;
	if (lptId == NULL)
		StringTotString(tstrUserId, g_user.UerSelf()->m_strId);
	else
		tstrUserId =  lptId;


	Replace(tstrUserId, _T(":"), _T(" "));
	if(type == E_NORMALMEMBER)
		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("FriendsFace"));
	else
		wsprintf(lptStr, _T("%s\\%s\\%s"), tszDir,tstrUserId.c_str() , _T("user"));
}


tString GetGrayPicName(LPCTSTR lptNormalName)
{
	//	LPCTSTR lptFind = _tcsrchr(tstrNormalName.c_str(), _T('/')) +1;
	// 譬如图片名称为 D:\www\ww\afdfdfgdg.jpg, 那么对应灰度图为D:\www\ww
	//D:\imClient\ImClient\Debug\ImClient\IMFileRecv\wdemo1 w060\user\ddb8fa42c4f14e1688e1b80f120383ba.jpg
	//D:\imClient\ImClient\Debug\ImClient\IMFileRecv\wdemo1 w060\user\gray.ddb8fa42c4f14e1688e1b80f120383ba.jpg
	tString tstrRet = lptNormalName;
	LPCTSTR lptFind = _tcsrchr(lptNormalName, _T('\\'));
	if (lptFind == NULL)
		tstrRet.insert(0, _T("gray."));
	else
	{
		//tstrRet = lptNormalName;
		int x = lptFind+1 - lptNormalName;
		tstrRet.insert(lptFind+1 - lptNormalName, _T("gray."));
		return tstrRet;
	}
	return tstrRet;
}