#pragma once
#include "Thread.h"
#include <map>
#include "Global.h"
#include  <set>

typedef std::map<std::string, tString>  MAP_STRING_TSTRING ;

typedef std::map<tString, tString>  MAP_TSTRING_TSTRING ;



#define  MUTITHREAD_DWOONLOAD_NUM  5 // 每个线程中下载数

//#define  MAX_WAITTHREAD       64  //等待线程最大数值





class PicInfoEx
{
public:
	//在当前处理的多个member中，必须先区分头像是下载还是未下载
	// <memberid, picUrl>
	MAP_TSTRING_TSTRING m_mapHasPic;  //有头像不用下载
	//MAP_TSTRING_TSTRING m_mapNotHasPic;  //无头像需要下载
	std::map<tString, vector<tString>> m_mapPicToId;  //需要下载头像的map<url, vector<好友ID>>
};

// 图片-好友ID映射


//PicDownloadThreadEx 线程参数
class PicDwonloadPara
{
public:
	PicDwonloadPara();
	~PicDwonloadPara();
	HWND m_hWnd;
	MAP_TSTRING_TSTRING m_mapDownInfo;
	std::map<tString, vector<tString>> m_mapPicInfo;
	TCHAR   m_tszDir[MAX_PATH + 1];  
	E_MEMBER_TYPE type;
	tString m_tstrGroupId;
};

//图片下载线程
class PicDownloadThreadEx: public CThread
{
public:	
	PicDownloadThreadEx();
	~PicDownloadThreadEx();
	//HANDLE GetHandle(){return m_hThread;}
private:
	static CRITICAL_SECTION_EX s_cs;
	BOOL Run();
	BOOL URLDownloadPicture(LPCTSTR lptUrl , LPCTSTR  lptPicPath, HINSTANCE hDll);
};

//CSourceDownloadThread 线程参数
class SrcPara
{
public:
	SrcPara();
	~SrcPara();

	tString     m_tstrId;  // GoupId or userself ID

	BOOL          m_bFirstTimeToDownlod;
	HWND          m_hWnd;

	E_MEMBER_TYPE type;

	///////
	PicInfoEx     m_picInfo;
};




//开启多个下载线程， 并等待下载结束
class CSourceDownloadThread:public CThread
{
public:
	CSourceDownloadThread();
	~CSourceDownloadThread(void);
private:
	BOOL IsMemberHasPic(BaseMember* pMember, LPCTSTR lptDir);
	void GetPicDownloadThreadPara(SrcPara* pSrcPara);
	BOOL Run();
	//下载图片存放路径
	std::vector<PicDownloadThreadEx*>       m_vThread;
	TCHAR     m_tszDir[MAX_PATH + 1];  
	//void GetPicDir(LPTSTR lptStr, int nSize, E_MEMBER_TYPE type,  LPCTSTR lptId= NULL);
	static CRITICAL_SECTION_EX s_cs;;
};

