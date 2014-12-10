#ifndef LOCALDATA_H
#define LOCALDATA_H
#define DB_OK 0
#define DB_FAILED 1
#define DB_EXIST  0x2

#define  USER_TYPE_WOA "woa"
#define  USER_TYPE_WCN "wcn"

#define NOSERVICETEST      1 //无服务状态下单机测试

#ifdef  DATASTORAGE_EXPORTS
#define DATADLL_API __declspec(dllexport)  
#else
#define DATADLL_API  __declspec(dllimport) 
#endif
//dll中接口函数参数使用了Vector，而没有使用map等一些较为复杂的stl的数据结构，因为对于一些较为复杂的数据结构做为入口或者出口参数可能导致某些异常

//内存释放宏
#define SAFE_ARRYDELETE(x) if(NULL!=x){delete[] x;x = NULL;}

#define SAFE_DELETE(x) if(NULL!=x){delete x; x =NULL;}

//由于Sqlite3中的没有外键约束。所以在删除的时候可以自
//己添加回调函数实现
#ifdef DATASTORAGE_EXPORTS
//EXE不需要包含
#define null 0x0
//#include <string.h>
size_t __strlen(const char* str);

#include <sqlite3.h>
#include <Windows.h>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(lib,  "Sqlite3_d.lib")
#else
#   ifdef _UNICODE
#pragma comment(lib,  "Sqlite3_U.lib")
#   endif
#endif

#endif

typedef int DB_FLAG;
#define DB_USERINFO 0x01
#define DB_MSG      0x02

//包含所有的数据库操作接口
//接口包含所有的本地文件数据库操作

#include <string>
#include <vector>
using namespace std;

#ifdef DATASTORAGE_EXPORTS

//void HandleString(string & str);

#endif

//以下数据结构对应表中数据

//当前登录到这个客户端的用户
typedef struct tag_userinfo
{
	string id;  //用户Id 唯一标示符
	string nickname;
	int  logintimes;
	string lastlogintime; 
	string res;
	string type;
	string     sex;   // Female 或者Male
    string loginid; //登录id
	string  pswd;   // 登录密码
} userinfo,*PUSERINFO;

typedef struct tag_userconfig
{
	string id;
	short  loginstatus;
	short  savepswd;
	short  autologin;
	short  autostart;
	short  exitconfirm;
	short  autohide;
	short  showintaskbar;
	short  upgrademode;
	short  mergewnd;
	short  autoshowmsg;
	short  leavestatusset;
	string  defaultderectory;
	short  criticalfilesize;
}userconfig, *PUSERCFG;

typedef struct tag_group
{
	string id;
	string name;
	bool   system;
} group, *PGROUP;

//此处只考虑到了OA用户， 没有考略TEMP用户和WCN用户，数据结构可重新设计
typedef struct tag_friendsinfo
{	
	string type;
	string membersource;
	string id;
	string nickname;
	string sex;
	string picurl;
	string signature;
	bool   tempmember;
	string mobile ;
	string telephone;
	string address;
	string email;
	bool   init;
	//OA 
	string post;
	string department;
	/*
	//WCN
	std::string   m_strContractor;
	std::string   m_strShopName;
	bool	      m_bMobilevalid;
	bool		  m_Realnamevalid;
	bool		  m_bMerchant;
	*/
}friendsinfo, *PFRIEND;;

typedef struct tag_msgfont
{
	string name;
	short  size;
	bool   bold;
	bool   italics;
	bool   underline;
	int    color;
} msgfont, *PMSGFONT;

typedef struct tag_msginfo 
{
	string  id;
	int msgid;
	short fromorto;
	string msgcontent;
	string msgres;
	string datetime;
	msgfont font;
}msginfo, *PMSGINFO;

enum e_msginfo_field
{
	e_msginfo_id = 0, 
	e_msginfo_msgid,
	e_msginfo_fromto,
	e_msginfo_msgcontent,
	e_msginfo_msgres,
	e_msginfo_date,
	e_msginfo_font 
};

enum e_comparison_operators
{
	e_biger = 0,
	e_less ,
	e_equal,
	e_bigerorequal,
	e_lessorequal
};

//userinfo.db的基本操作
class   IUserDb
{
public:
	IUserDb();
	virtual ~IUserDb();
	//打开或者创建本地数据库
	virtual int QueryUserInfo(vector<userinfo>& v) = 0;	
	virtual int AddUser(const userinfo& info)= 0;
	virtual int DeleteUser(const userinfo& info) =0;
	virtual int UpdateUser(const userinfo& info)=0;
	//virtual int QueryFriendsInfo(vector<friendsinfo>& v,const char* strId)=0;
	virtual int QueryCfgInfo(userconfig& cfginfo, const char* szId)=0;
	//必须保证调用此函数的之后不在调用类的其它成员
	virtual void FinalRelease()=0 ; 
};

//Msg.db的 基本操作
class IMsgDb
{
public:
	IMsgDb();
	virtual ~IMsgDb();
	virtual int BeginTransaction() = 0;
	virtual int EndTransaction()= 0;
	virtual int AddGroup(const group&	g) = 0;
	virtual int QueryGroupCollection(vector<group>& v) = 0;
	virtual int AddFriend(const friendsinfo& f) = 0;
	virtual int DeleteFriend(const friendsinfo& f) = 0;
	virtual int UpdateFriend(const friendsinfo& f) = 0;
	virtual int QueryFriendsCollection(vector<friendsinfo>& v, const string & groupid="", const string& groupname="") = 0;
	virtual int QueryMsg(msginfo& msg, const string & id, int msgid)=0;
//  virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id) = 0;
	virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id, int msgid, bool bLessComparison=true ,bool bAsc=false ,int num=50,int offset=0 ) = 0;
	virtual int QueryMsgCollection(vector<msginfo>& v, const string& id, const string& formDate,  const string & toDate ) = 0;
// 	virtual int AddTable(const string& id)=0;
// 	virtual int DropTable(const string& id)=0;
	virtual int AddMsg(const msginfo& msg, const string & id="")=0;
	virtual int DeleteMsg(int msgid, const string & id) =0;
	virtual int DeleteMsgCollection(int msgidfrom, int nCounts , const string & id ) = 0;
	virtual bool IsExist(const string& id, e_msginfo_field field, e_comparison_operators comparison, const string& value, int num = 1) = 0;
	virtual void FinalRelease()=0 ; 
};

class IIni
{
public:
	IIni();
	~IIni();
public:
	virtual void InitPath() = 0;
	virtual BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString) = 0;
	virtual DWORD ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR buff, DWORD nSize) = 0;
	virtual UINT ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault) = 0;
	virtual void SetPath(LPCTSTR lpPath) = 0;
	virtual void FinalRelease()=0 ; 
};

#ifdef DATASTORAGE_EXPORTS

//接口实现数据库的连接 释放，数据表的创建，基本sql语句操作
//对同时要操作多条exec，必须使用事务，大大加快执行速度
//多线程使用全局句柄时，必须建立pthread_mutex_t。除非可以保证多线程之间执行数据库prepare_v2操作时不冲突。否则，当一个线程prepare_v2分配好pStmt正在操作时，另一个线程sqlite3_finalize(pStmt),那么就会出现异常，导致整个程序down掉
class  IDBInit
{
	//注意用户可以操作
public:
	IDBInit();
	virtual ~IDBInit();
	int OpenDB(sqlite3** ppdb, DB_FLAG dbtype) ;
	virtual int CreateDB(sqlite3* pdb, DB_FLAG dbtype) ; 
	virtual int Init(DB_FLAG dbtype) = 0;

	virtual int ManipulateSql(sqlite3_stmt** ppstmt, const char* sql, bool bSelectSql = false);
	sqlite3* m_pConn;
	bool     m_bExist;		//当前数据库连接是否存在
	static char     strUid[30];	//当前登录用的ID， 也是对应的个人目录名
};


//文件路径接口
class IPath
{
public:
	IPath();
	~IPath();
	static TCHAR s_tszPath[MAX_PATH]; //IM文件存储路径，宽字节
	static char  s_szPath[MAX_PATH*2]; //IM文件存储路径,多字节
	static int SetDirectory(LPCTSTR lptStr);  //设置整个IM客户端存储路径
	static int SetUserDirectory(LPCTSTR lptStr, LPCTSTR lptType = NULL);
	//void	DeleteUserDirectory(LPCTSTR lptStr);
	BOOL  DeleteFolder(LPCTSTR lpstrFolder);
	//BOOL  DeleteFolder(LPCTSTR lpstrFolder);
	static BOOL   IsDirectoryExist(LPCTSTR lptStr);
private:
	//static BOOL   IsDirectoryExist(LPCTSTR lptStr);
};

//配置文件写入类
class CIni:public IIni
{
public:
	CIni();
	~CIni();
public:
	virtual void InitPath();
	virtual BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	virtual DWORD ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR buff, DWORD nSize);
	virtual UINT ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault);
	virtual void SetPath(LPCTSTR lpPath);
	virtual void FinalRelease() ; 
private:
	TCHAR m_tszFilePath[MAX_PATH];
};

class UserDb:public IDBInit, public IUserDb, public IPath
{
public:
	UserDb();
	~UserDb();
public:
	virtual int Init(DB_FLAG dbtype) ;
public:
	//获取所有的当前客户端用户信息
	virtual int QueryUserInfo(vector<userinfo>& v) ;	
	//添加用户若用户不存在添加， 若存在不添加根据返回值判断
	virtual int AddUser(const userinfo& info);
	//删除一个用户，除了在数据库中删除之外还需要删除该用户的本地目录等
	virtual int DeleteUser(const userinfo& info);
	//更新一个用户
	virtual int UpdateUser(const userinfo& info);
	//获取当前用户的各项配置信息
	virtual int QueryCfgInfo(userconfig& cfginfo,const char* szId);
	//资源的释放
	bool IsUserExist(const char* szId);
	virtual void FinalRelease();
};

//好友信息以及聊天记录都存贮在MsgDb里面
class MsgDb:public IDBInit, public IMsgDb
{
public:	
	MsgDb();
	~MsgDb();
	virtual int Init(DB_FLAG dbtype) ;
	//BeginTransaction和EndTransaction需要配对使用，操作失败会进行回滚处理
	virtual int BeginTransaction();
	virtual int EndTransaction();
	virtual int AddFriend(const friendsinfo& f);
	virtual int AddGroup(const group&	g);
	virtual int QueryGroupCollection(vector<group>& v);
	virtual int UpdateFriend(const friendsinfo& f);
	virtual int DeleteFriend(const friendsinfo& f);
	// 实际操作中由于groupid 为空，组唯一标示是根据groupname
	virtual int QueryFriendsCollection(vector<friendsinfo>& v, const string & groupid="", const string& groupname="");
	virtual int QueryMsg(msginfo& msg, const string & id, int msgid);
	virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id, int msgid, bool bLessComparison=true,  bool bAsc=false , int num=50,int offset=0   );
	virtual int QueryMsgCollection(vector<msginfo>& v, const string& id, const string& formDate,  const string & toDate );
	//virtual int QueryMsgCollection(vector<msginfo>& v, const string & id)
// 	virtual int AddTable(const string& id);
// 	virtual int DropTable(const string& id);
	virtual int AddMsg(const msginfo& msg, const string & id="");
	virtual int DeleteMsg(int msgid, const string & id);
	virtual int DeleteMsgCollection(int msgidfrom, int nCounts , const string & id );
	virtual bool IsExist(const string& id, e_msginfo_field field, e_comparison_operators comparison, const string& value, int num = 1);
	virtual void FinalRelease();
private:
	int QueryMsgCollection(vector<msginfo>&v,  const char* sql);
};
#endif



#ifdef __cplusplus
extern "C"{
#endif

#ifdef DATASTORAGE_EXPORTS

	//int db_open_database(sqlite3** ppdb, DB_FLAG dbtype);
	//int db_create_db(sqlite3* pdb, DB_FLAG dbtype);

#endif
	//DATADLL_API int  Test();
	DATADLL_API IUserDb* CreateUserDb();
	DATADLL_API IMsgDb* CreateMsgDb();
	DATADLL_API IIni*  CreateIni();
	//如在系统设置中改变该用户的目录设置调用此接口
	DATADLL_API void SeDirectory(LPCTSTR  lptStr);
#ifdef __cplusplus
}
#endif

//
#endif