#ifndef LOCALDATA_H
#define LOCALDATA_H
#define DB_OK 0
#define DB_FAILED 1
#ifdef  DATASTORAGE_EXPORTS
#define DATADLL_API __declspec(dllexport)  
#else
#define DATADLL_API  __declspec(dllimport) 
#endif


//不适用标准库中的东西

#ifdef DATASTORAGE_EXPORTS
//不需要包含
#define null 0x0
#include <string.h>
#include <sqlite3.h>
#endif

typedef int DB_FLAG;
#define DB_USERINFO 0x01
#define DB_MSG      0x02

//包含所有的数据库操作接口
//接口包含所有的本地文件数据库操作

#include <string>
#include <vector>
using namespace std;

typedef struct tag_userinfo
{
	string id;
	string nickname;
	string logintimes;
	string lastlogintime; 
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
	short  defaultderectory;
	short  criticalfilesize;
}userconfig, *PUSERCFG;

typedef struct tag_friendsinfo
{
	string id;
	string friendid;
	string nickname;
	string datetime;
}friendsinfo, *PFRIEND;;

class   IUserDb
{
public:
	IUserDb();
	virtual ~IUserDb();
	//打开或者创建本地数据库
	virtual int QueryUserInfo(vector<userinfo>&) = 0;	
	virtual int AddUser(const userinfo& info)= 0;
	virtual int DeleteUser(const userinfo& info) =0;
	virtual int UpdateUser(const userinfo& info)=0;
	virtual int QueryFriendsInfo(vector<userinfo>&)=0;
	virtual int QueryCfgInfo(userconfig& cfginfo)=0;
	//必须保证调用此函数的之后不在调用类的其它成员
	virtual void FinalRelease()=0 ; 
};

#ifdef DATASTORAGE_EXPORTS
class  IDBInit
{
public:
	IDBInit();
	virtual ~IDBInit();
	int OpenDB(sqlite3** ppdb, DB_FLAG dbtype) ;
	virtual int CreateDB(sqlite3* pdb, DB_FLAG dbtype) ;
	virtual int Init(DB_FLAG dbtype) = 0;
	sqlite3* m_pSql;
};

class UserDb:public IDBInit, public IUserDb
{
public:
	UserDb();
	~UserDb();
public:
	virtual int Init(DB_FLAG dbtype) ;
	//virtual int OpenDB(sqlite3** ppdb, DB_FLAG dbtype) const ;
	//virtual int CreateDB(sqlite3* pdb, DB_FLAG dbtype) const;
public:
	virtual int QueryUserInfo(vector<userinfo>& v) ;	
	virtual int AddUser(const userinfo& info);
	virtual int DeleteUser(const userinfo& info);
	virtual int UpdateUser(const userinfo& info);
	virtual int QueryFriendsInfo(vector<userinfo>& v);
	virtual int QueryCfgInfo(userconfig& cfginfo);
	virtual void FinalRelease();
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
#ifdef __cplusplus
}
#endif

//
#endif