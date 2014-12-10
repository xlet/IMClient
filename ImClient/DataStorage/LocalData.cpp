#include "LocalData.h"


/*
	sqlite数据库的创建语句都放在了代码中，后面
	如果创建语句较长的话可以放在文件中, 放在文件中会更加ling
*/


const char* const_userdb = "userinfo.db";

const char* const_msgdb = "msg.db";

const char* const_commondirectory = "Common";

TCHAR IPath::s_tszPath[MAX_PATH] = {0x0};

char IPath::s_szPath[MAX_PATH*2] = {0x0};

const char* const_userdb_sql =  "begin;\n  \
								CREATE TABLE tb_localuser(  \
								id varchar(20) not null,\
								nickname varchar(20),\
								logintime int not null, \
								lastlogintime varchar(20) not null,\
								res		varchar(20) not null  default '',\
								type varchar(20) not null  default '',\
								sex  varchar(20) not null, \
								loginid varchar(20) not null, \
								pswd  varchar(20) not null, \
								primary key(id)\
								);\
								CREATE TABLE tb_localconfig(\
								id  varchar(20) not null ,\
								loginstatus smallint not null  default 0,\
								savepswd smallint not null  default 0,\
								autologin smallint not null  default 0,\
								autostart smallint not null  default 0,\
								exitconfirm smallint not null  default 0,\
								autohide smallint not null  default 0,\
								showintaskbar smallint not null  default 0,\
								upgrademode  smallint not null  default 0,\
								mergewnd smallint  not null  default 0,\
								autoshowmsg smallint not null  default 0,\
								leavestatusset smallint not null  default 0,\
								defaultdirectory varchar(128)   default NULL,\
								criticalfilesize int not null  default 0,\
								constraint pk primary key (id)\
								);  \
								 create table tb_loginid (id, loginid, tm);;\
								commit;";


const char* const_msgdb_sql = "BEGIN;" \
	"create table tb_friends (type,memsrc,id,  nickname,   sex,picurl, signature, tempmember, mobile, telephone,address,email,init,post, department,constraint pk primary key (id) );" \
	"create table tb_groups (id, name, system default 1, primary key (id, name));" \
							 "CREATE TABLE tb_msg("\
							"id varchar(20) not null,"\
							"msgid INTEGER primary key autoincrement,"\
							 "fromorto smallint not null,"\
							 "msgcontent varchar(500),"\
							 "msgres   varchar(30) ," \
							 "datetime varchar(30),"\
							 "fontname  varchar(20),"\
							 "fontsize  smallint ,"\
							 "fontbold      smallint,"\
							 "fontitalics   smallint,"\
							 "fontunderline  smallint,"\
							 "fontcolor      INTEGER" \
							 ");"\
							"COMMIT;";


//多字节转化为宽字节
void C2W(WCHAR** dest, const char* src)
{
	if (src == NULL)
	{
		return ;
	}

	size_t alen = __strlen(src) + 1;
	size_t  ulen = (size_t)MultiByteToWideChar(CP_ACP, 0, src,alen,NULL, 0 )+1;

	*dest = new WCHAR[ulen];
	::MultiByteToWideChar(CP_ACP, 0, src, alen, *dest, ulen);
}

//宽字节转化为多字节
void W2C(char** dest, const WCHAR *src)
{
	if(src == NULL)
		return ;
	size_t len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0 , NULL, NULL);
	if (len == 0)
	{
		return;
	}
	*dest = new char[len];
	WideCharToMultiByte( CP_ACP, 0, src, -1, *dest, len, NULL, NULL );

}

void C2T(TCHAR** dest, const char* src)
{
#ifdef _UNICODE
	C2W(dest, src);
#else 
	//多字节TCHAR就是　ｃｈａｒ　
	int len = strlen(src)+1;
	*dest = new char[len];
	strcpy(*dest, src);
#endif
}

void T2C(char** dest, const TCHAR* src)
{
#ifdef _UNICODE
	W2C(dest, src);
#else
	int len = _tcslen(src) + 1;
	*dest = new TCHAR[len];
	strcpy(*dest, src);
#endif
}

//替换字符串中某个字符
//
void strreplace(char* lpStr, char src, char dest)
{
	while(*lpStr != '\0')
	{
		if (*lpStr == src)
		{
			*lpStr  = dest;
		}
		++lpStr;
	}
}

void _tstrreplace(TCHAR* lptStr, TCHAR src, TCHAR dest)
{
	while(*lptStr != _T('\0'))
	{
		if (*lptStr == src)
		{
			*lptStr  = dest;
		}
		++lptStr;
	}
}

void Replace(string& strText, const char * lpOldStr, const char * lpNewStr)
{
	if (NULL == lpOldStr || NULL == lpNewStr)
		return;

	int nOldStrLen = strlen(lpOldStr);
	int nNewStrLen = strlen(lpNewStr);

	std::wstring::size_type nPos = 0;
	while ((nPos = strText.find(lpOldStr, nPos)) != std::wstring::npos)
	{
		strText.replace(nPos, nOldStrLen, lpNewStr);
		nPos += nNewStrLen;
	}
}

//文件名中不能包含一些 \ / : * ? " < > | "
void EncodeUrl(string& strUrl)
{
	Replace(strUrl, ":", "&mh");
	Replace(strUrl, "\\", "&fxg");
	Replace(strUrl, "/", "&xg");
	Replace(strUrl, "*", "&xh");
	Replace(strUrl, "?", "&wh");
	Replace(strUrl, "<", "&xyh");
	Replace(strUrl, ">", "&dyh");
	Replace(strUrl, "|", "&sg");
	//Replace(tstrUrl, _T("\""), _T("&yh"))
}

void DecodeUrl(string& strUrl)
{
	Replace(strUrl, "&mh",  ":");
	Replace(strUrl, "&fxg", "\\");
	Replace(strUrl, "&xg", "/");
	Replace(strUrl, "&xh", "*");
	Replace(strUrl, "&wh", "?");
	Replace(strUrl, "&xyh", "<");
	Replace(strUrl, "&dyh", ">");
	Replace(strUrl, "&sg", "|");
	//Replace(tstrUrl, _T("&yh"), _T("\""))
}
// 
// //存进sqlite中的一些字符串需要加上转移字符
// void HandleString(string & str)
// {
// 	char* str1 = "'";
// 	char* str2 = "''";
// 	string::size_type  pos = 0;
// 	while((pos = str.find(str1, pos)) != string::npos)
// 	{
// 		str.replace(pos, __strlen(str1), str2 );
// 		pos += __strlen(str2);
// 	}
// }

void  GetCurDir(LPTSTR lpStr, int nSize)
{
	GetModuleFileName(NULL, lpStr, nSize); 
	(_tcsrchr(lpStr, _T('\\')))[0] = 0;//删除文件名，只获得路径
}

#if 0
int db_create_db(sqlite3* pdb, DB_FLAG dbtype)
{
	char* pszSql = NULL;
	if (dbtype == DB_USERINFO)
	{
		pszSql = "begin;\n  \
					   CREATE TABLE tb_localuser(  \
					   id varchar(20) not null,\
					   nickname varchar(20),\
					   logintime int not null, \
					   lastlogintime varchar(20) not null,\
					   primary key(id)\
					   );\
					   CREATE TABLE tb_friends (\
					   id VARCHAR(20) NOT NULL,\
					   friendid VARCHAR(20) NOT NULL,\
					   nickname VARCHAR(20),\
					   datatime VARCHAR(30) NOT NULL,\
					   CONSTRAINT  fk \
					   foreign key (id) \
					   references tb_localuser (id),\
					   CONSTRAINT  pk\
					   primary key (id, friendid)\
					   );\
					   CREATE TABLE testtable(\
					   col_1 int ,\
					   col_2 int , col_3 int,\
					   constraint pk primary key (col_1, col_2)\
					   );\
					   \
					   CREATE TABLE tb_localconfig(\
					   id  varchar(20) not null ,\
					   loginstatus smallint not null  default 0,\
					   savepswd smallint not null  default 0,\
					   autologin smallint not null  default 0,\
					   autostart smallint not null  default 0,\
					   exitconfirm smallint not null  default 0,\
					   autohide smallint not null  default 0,\
					   showintaskbar smallint not null  default 0,\
					   upgrademode  smallint not null  default 0,\
					   mergewnd smallint  not null  default 0,\
					   autoshowmsg smallint not null  default 0,\
					   leavestatusset smallint not null  default 0,\
					   defaultdirectory varchar(128)   default NULL,\
					   criticalfilesize int not null  default 0,\
					   constraint pk primary key (id)\
					   ); commit;";
	}
	else if (dbtype == DB_MSG)
	{
		pszSql = "BEGIN TRANSACTION; \
			CREATE TABLE tb_msg_000000(\
			id varchar(20) not null,\
			msgid int not null,\
			fromorto smallint not null,\
			msgcontent varchar(500) ,\
			msgres   varchar(30) , \
			datetime varchar(30),\
			constraint pk primary  key (id, msgid)\
			);\
		COMMIT;";
	}
	char*  pszerrormsg = null;
	int res = sqlite3_exec(pdb, pszSql, null, null, &pszerrormsg);
	if (res != SQLITE_OK)
	{
		sqlite3_free(pszerrormsg);
		return 1;    //代表失败
	}
	return 0;
}

int  db_open_database(sqlite3** ppdb, DB_FLAG dbtype)
{
	const char* pszCreateTable = NULL;
	if (dbtype == DB_MSG)
		pszCreateTable = const_msgdb;
	else if (dbtype == DB_USERINFO)
		pszCreateTable = const_userdb;
	
	*ppdb = null;
	if (  sqlite3_open(pszCreateTable, ppdb) != SQLITE_OK)
	{
		//打开或者创建数据库失败
		sqlite3_close(*ppdb);
		*ppdb = null;
		return 1;
	}
	else
	{
		//打开或者创建成功
		//判断数据库是否为空
		const char* pszTablenum = "select count(*) from  sqlite_master where type='table';";
		sqlite3_stmt* stmt = NULL;
		if (sqlite3_prepare_v2(*ppdb, pszTablenum, __strlen(pszTablenum), &stmt,null)  != SQLITE_OK)
		{
			if(stmt)
				sqlite3_finalize(stmt);
			sqlite3_close(*ppdb);
			*ppdb = null;
			return 1;
		}

		int r = sqlite3_step(stmt);
		int tablenum = 0;
		tablenum = sqlite3_column_int(stmt, 0);
		if (tablenum <= 1)
		{
			//表示数据库是空的 创建数据库
			int res = db_create_db(*ppdb, dbtype);
			if (res != 0)
			{
				//创建失败
				sqlite3_close(*ppdb);
				*ppdb = NULL;
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

int db_create_msgdb(sqlite3* pdb)
{
	return 0;
}

int  Test()
{
	sqlite3*  pdb1 = NULL;
	int res =   db_open_database(&pdb1, DB_USERINFO);
		sqlite3*  pdb2 = NULL;
	res = db_open_database(&pdb2,  DB_MSG);
	return res;
}
#endif

size_t __strlen(const char* str)
{
	const char* t = str;
		while(*t++);
		t--;
		return (t - str);
}
char IDBInit::strUid[30] = {0x0};

IDBInit::IDBInit()
{
	m_pConn = null;
	m_bExist = false;
// 	for (int i=0; i < 30; i++)
// 	{
// 		strUid[i] = 0x0;
// 	}
}

 IDBInit::~IDBInit()
{
	if(m_pConn != NULL)
	{
		sqlite3_close(m_pConn);
		m_pConn = NULL;
	}
}

 void UnicodeToUtf8(char** dest , const WCHAR* src)
 {
//	 ASSERT(dest!= NULL || src != NULL);
	 int len = -1;
	 len = WideCharToMultiByte(CP_UTF8, 0, src, -1, 0, 0, 0, 0)+1;
	 *dest = new char[len+1];
	 ::WideCharToMultiByte(CP_UTF8, 0, src, -1,*dest, len, 0, 0);
 }

 void AnsiToUtf8(char** dest, const char* src)
 {
	// ASSERT(dest!= NULL || src != NULL);
	 WCHAR* pwszStr = NULL;
	 C2W(&pwszStr, src);
	 UnicodeToUtf8(dest, pwszStr);
	 SAFE_ARRYDELETE(pwszStr);
 }


 //打开或者创建一个数据库
int IDBInit::OpenDB(sqlite3** ppdb, DB_FLAG dbtype)
{
	char szCreateTable[MAX_PATH*2] = {0x0};

	
	if (dbtype == DB_MSG)
	{
		
		sprintf_s(szCreateTable, "%s\\%s\\%s",IPath::s_szPath,strUid, const_msgdb);
	}
	else if (dbtype == DB_USERINFO)
	{
		sprintf_s(szCreateTable, "%s\\%s\\%s",IPath::s_szPath,const_commondirectory, const_userdb);
	}
	
	*ppdb = null;

	////////////  修改
	char* pszCreateSql = NULL;
	AnsiToUtf8(&pszCreateSql, szCreateTable);

	///////////

	if (  sqlite3_open(/*szCreateTable*/pszCreateSql, ppdb) != SQLITE_OK)
	{  
		//打开或者创建数据库失败
		sqlite3_close(*ppdb);
		*ppdb = null;
		return DB_FAILED;
	}
	else
	{
		//打开或者创建成功
		//判断数据库是否为空
		if (!m_bExist)
		{
			const char* pszTablenum = "select count(*) from  sqlite_master where type='table';";
			sqlite3_stmt* stmt = NULL;
			if (sqlite3_prepare_v2(*ppdb, pszTablenum, __strlen(pszTablenum), &stmt,null)  != SQLITE_OK)
			{
				if(stmt)
					sqlite3_finalize(stmt);
				sqlite3_close(*ppdb);
				*ppdb = null;
				return DB_FAILED;
			}

			int r = sqlite3_step(stmt);
			int tablenum = 0;
			tablenum = sqlite3_column_int(stmt, 0);
			sqlite3_finalize(stmt);
			if (tablenum <= 1)
			{
				//表示数据库是空的 创建数据库
				int res = CreateDB(*ppdb, dbtype);
				if (res != 0)
				{
					//创建失败
					sqlite3_close(*ppdb);
					*ppdb = NULL;
					return DB_FAILED;
				}
				m_bExist = true;
				return DB_OK;
			}
		}
	
	}
	SAFE_ARRYDELETE(pszCreateSql);
	return DB_OK;
}

//建表
int IDBInit::CreateDB(sqlite3* pdb, DB_FLAG dbtype)
{
	const char* pszSql = NULL;
	if (dbtype == DB_USERINFO)
	{
		pszSql = const_userdb_sql;
	}
	else if (dbtype == DB_MSG)
	{
		pszSql = const_msgdb_sql;
	}
	char*  pszerrormsg = null;
	int res = sqlite3_exec(pdb, pszSql, null, null, &pszerrormsg);
	if (res != SQLITE_OK)
	{
		OutputDebugStringA(pszerrormsg);
		OutputDebugStringA("\n");
		sqlite3_free(pszerrormsg);
		return DB_FAILED;    //代表失败
	}
	return DB_OK;
}


int IDBInit::ManipulateSql(sqlite3_stmt** ppstmt, const char* sql,bool bSelectSql)
{
	if (m_pConn == null)
	{
		return DB_FAILED;
	}
	

	if (bSelectSql) //select语句 有结果返回的
	{
		if (sqlite3_prepare_v2(m_pConn, sql, __strlen(sql),ppstmt, null) != SQLITE_OK)
		{
			if (*ppstmt)
			{
				sqlite3_finalize(*ppstmt);
			}
			return DB_FAILED;
		}

	}
	else
	{//无结果返回的

		char* errmsg = null;
		//int ret = sqlite3_exec(m_pConn, sql, NULL, NULL, &errmsg);
		if ( sqlite3_exec(m_pConn, sql, NULL, NULL, &errmsg) != SQLITE_OK) 
		{
			//打印出错误信息
			OutputDebugStringA(errmsg);
			OutputDebugStringA("\n");
			sqlite3_free(errmsg);
			sqlite3_exec(m_pConn, "rollback", NULL, NULL,&errmsg);
			return DB_FAILED;
		}
		//sqlite3_finalize(*ppstmt);
	}
	return DB_OK;
}

IUserDb::IUserDb()
{

}

 IUserDb:: ~IUserDb()
{
	int i = 1;
}

// userdb implementation
UserDb::UserDb()
{
	Init(DB_USERINFO);
}

UserDb::~UserDb()
{
	int i = 1;
}

int UserDb::QueryUserInfo(vector<userinfo>& v)
{
	userinfo u;

	const char* sql = "select * from tb_localuser  order by lastlogintime desc;";
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, sql, true) != 0)
	{
		return  DB_FAILED;
	}

	int nCols  = sqlite3_column_count(stmt);
	do 
	{
		int ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW)
		{
			u.id = (const char*)sqlite3_column_text(stmt, 0);
			u.nickname = (const char*)sqlite3_column_text(stmt, 1);
			u.logintimes = sqlite3_column_int(stmt, 2);
			u.lastlogintime= (const char*)sqlite3_column_text(stmt, 3);
			u.res = (const char*)sqlite3_column_text(stmt, 4);
			u.type = (const char*)sqlite3_column_text(stmt, 5);
			u.sex = (const char*)sqlite3_column_text(stmt, 6);
			u.loginid = (const char*)sqlite3_column_text(stmt, 7);
			u.pswd = (const char*)sqlite3_column_text(stmt, 8);
			v.push_back(u);
		}
		else if (ret == SQLITE_DONE)
		{
			//查找结束
			break;
		}
		else
		{
			//FAILED TO SELECT
			sqlite3_finalize(stmt);
			return DB_FAILED;
		}
	} while (1);
	sqlite3_finalize(stmt);

	return 0;
}

/*
	函数名：
	功能：
	入口参数
	返回：
	其他：判断当前新登录的用户是否存在
*/
bool UserDb::IsUserExist(const char* szId)
{
	char szSql[256] = {0};
	sprintf_s(szSql, "select count(*) from tb_localuser where id='%s';", szId);
	//const char* sql = "select count(*) from tb_localuser;";
	sqlite3_stmt* stmt = NULL;
	if (ManipulateSql(&stmt, szSql, true) != DB_OK)
	{
		return  /*DB_FAILED*/false;
	}

	int ret = sqlite3_step(stmt);
	if ( sqlite3_column_int(stmt, 0)!= 1)
	{
		//该用户不存在
		sqlite3_finalize(stmt);
		return false;
	}
	sqlite3_finalize(stmt);
	return true;
}

/*
	
	函数名： AddUser
	功能：当一个新用户登录到当前客户端的时候，添加一个用户， 此时
		  需要做的事情有 ：
			1）在 user.db里面添加相当前用的基本信息
			2）给当前用户创建个目录， 用于存贮当前用户的
				聊天信息， 文件图片等内容
	入口参数：info， 添加的用户的基本信息
	出口参数：
	返回：
	其它：新增加一个用用户之前需要给该用户先添加一个文件目录
	当前用户目录信息保存在
*/
int UserDb::AddUser(const userinfo& info)
{
	//添加一个文件夹目录
	TCHAR * tszId= NULL;
	char szId[30] = {0x0};
	strcpy(szId, info.id.c_str());
	//ID中可能存在':'， 需要进行下转换, 文件中不允许存在:
	strreplace(szId, ':', ' ');
	strcpy(strUid, szId);
	C2T(&tszId, szId);

#if NOSERVICETEST
	SAFE_ARRYDELETE(tszId);
	return DB_OK;
#endif
	char strSql[1024] = {0};
	if (IsUserExist(info.id.c_str()/*szId*/))
	{
 		//该用户存在, 更新下该用户的信息
 		SAFE_ARRYDELETE(tszId);
 		OutputDebugStringA("USER EXIST， Update It\n");
		
		sprintf_s(strSql,"update tb_localuser set nickname='%s', logintime='0', lastlogintime=datetime('now'), res='%s', type='', sex='%s', loginid='%s' ,pswd='%s' where id='%s'",
			info.nickname.c_str(), info.res.c_str(), info.sex.c_str(), info.loginid.c_str() ,info.pswd.c_str(), info.id.c_str());
 		//return DB_EXIST; 
 	}
	else
	{
		//为该用户创建相应个人的文件目录
		if (SetUserDirectory(tszId) == DB_FAILED)
		{
			SAFE_ARRYDELETE(tszId);
			return DB_FAILED;
		}
		SAFE_ARRYDELETE(tszId);
		//文件目录添加成功后需要 可以再其下添加对应的头像目录

		//文件目录添加成功后需要 可以再其下添加db数据库
		sqlite3* conn;
		if (OpenDB(&conn, DB_MSG) != SQLITE_OK)
		{
			//创建失败
			return DB_FAILED;
		}
		sqlite3_close(conn);

		sprintf_s(strSql, "begin;CREATE TABLE if not exists tb_localuser(id varchar(20) not null,nickname varchar(20),logintime int not null, lastlogintime varchar(20) not null,res		varchar(20) not null  default '',type varchar(20) not null, sex  varchar(20) not null, loginid varchar(20) not null, primary key(id));\
						  insert into tb_localuser values ('%s','%s','%d',datetime('now'), '%s','%s', '%s', '%s','%s');\
						  insert into  tb_localconfig (id) values ('%s');\
						  insert into tb_loginid values ('%s', '%s',  datetime('now')) ;\
						  commit;" ,info.id.c_str() , info.nickname.c_str(),  info.logintimes, /*info.lastlogintime.c_str(),*/info.res.c_str(), info.type.c_str(),info.sex.c_str(),info.loginid.c_str(), info.pswd.c_str(), info.id.c_str(), info.id.c_str(), info.loginid.c_str()); 
	}
	sqlite3_stmt* stmt1 = null;
	if (ManipulateSql(&stmt1, strSql) != DB_OK)
	{
		return DB_FAILED;
	}              
	return DB_OK;
}

//删除用户，删除用户信息以及其所对应的文件夹
//实际上此处的删除只是根据用户ID进行的删除， 但是函数中
//参数仍旧使用了userinfo 结构体。
int UserDb::DeleteUser(const userinfo& info)
{
	char strSql[256] = {0};
	sprintf_s(strSql, "begin;\ndelete from tb_localuser where id='%s';\n delete from tb_localconfig where id='%s';\ncommit",info.id.c_str(), info.id.c_str());
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, strSql) != DB_OK)
	{
		return DB_FAILED;
	}
	//sqlite3_finalize(stmt);
	//删除该用户对应的文件夹
	TCHAR tszPath[MAX_PATH] = {0x0};
	WCHAR* wstrUid = NULL;
	//C2W(&wstrUid, strUid);
	C2W(&wstrUid, info.id.c_str());
	wsprintf(tszPath, _T("%s\\%s"), IPath::s_tszPath, wstrUid);
	DeleteFolder(tszPath);
	SAFE_ARRYDELETE(wstrUid)
	return DB_OK;
}

//更新用户信息
int UserDb::UpdateUser(const userinfo& info)
{
 	char strSql[256] = {0};
 	sprintf_s(strSql, "update tb_localuser set nickname='%s',logintime='%d', lastlogintime='%s',res='%s', type='%s' where id='%s';", info.nickname, info.logintimes,info.lastlogintime,info.res,info.id,info.type.c_str());
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt,strSql) != DB_OK)
	{
		OutputDebugStringA("Update User Failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

//查询当前用户配置信息
int UserDb::QueryCfgInfo(userconfig& cfginfo, const char* szId)
{
	//userconfig ucfg;
	
	sqlite3_stmt*  stmt = null;
	char szSql[64] = {0};
	// SELECT * FROM  TB_LOCALCONFIG WHERE ID = '11'
	sprintf_s(szSql,"select * from tb_localconfig where id='%s';", szId );
	if (ManipulateSql(&stmt,szSql,true ) != DB_OK)
	{
		sqlite3_finalize(stmt);
		return DB_FAILED;
	}
	int ret =sqlite3_step(stmt);

	cfginfo.id  = (const char*)sqlite3_column_text(stmt, 0);
	cfginfo.loginstatus = sqlite3_column_int(stmt, 1);
	cfginfo.savepswd = sqlite3_column_int(stmt, 2);
	cfginfo.autologin = sqlite3_column_int(stmt, 3);
	cfginfo.autostart = sqlite3_column_int(stmt, 4);
	cfginfo.exitconfirm = sqlite3_column_int(stmt, 5);
	cfginfo.autohide = sqlite3_column_int(stmt, 6);
	cfginfo.showintaskbar= sqlite3_column_int(stmt, 7);
	cfginfo.upgrademode = sqlite3_column_int(stmt, 8);
	cfginfo.mergewnd = sqlite3_column_int(stmt, 9);
	cfginfo.autoshowmsg = sqlite3_column_int(stmt,10);
	cfginfo.leavestatusset = sqlite3_column_int(stmt, 11);
	cfginfo.defaultderectory = (const char*)sqlite3_column_text(stmt, 12);
	cfginfo.criticalfilesize = sqlite3_column_int(stmt, 13);
	sqlite3_finalize(stmt);
	return DB_OK;
}

int UserDb::Init(DB_FLAG dbtype) 
{
	return	OpenDB(&m_pConn, dbtype);
}

void UserDb::FinalRelease()
{
	delete this ;
}

IUserDb* CreateUserDb()
{
	return new UserDb();
}


//MsgDb Implementation 

IMsgDb::IMsgDb()
{

}

IMsgDb::~IMsgDb()
{

}

int MsgDb::Init(DB_FLAG dbtype) 
{
	return OpenDB(&m_pConn, dbtype);
}

MsgDb::MsgDb()
{
	Init(DB_MSG);
}

MsgDb::~MsgDb()
{

}

int MsgDb::BeginTransaction()
{
	char* strSql="BEGIN";
	sqlite3_stmt* stmt1 = null;
	if (ManipulateSql(&stmt1, strSql) != DB_OK)
	{
		return DB_FAILED;
	}   
	return DB_OK;
}

int MsgDb::EndTransaction()
{
	char* strSql="COMMIT";
	sqlite3_stmt* stmt1 = null;
	if (ManipulateSql(&stmt1, strSql) != DB_OK)
	{
		return DB_FAILED;
	}   
	return DB_OK;
}

int MsgDb::QueryMsgCollection(vector<msginfo>&v, const char* sql)
{
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, sql,true) != DB_OK)
	{
		OutputDebugStringA("QueryMsg Failed \n");
		return DB_FAILED;
	}

	msginfo m;
	int ret = -1;
	do 
	{
		int nCols = sqlite3_column_count(stmt);
		ret = sqlite3_step(stmt);
		if (ret== SQLITE_ROW)
		{
			m.id = (const char*)sqlite3_column_text(stmt, 0);
			m.msgid = sqlite3_column_int(stmt, 1);
			m.fromorto = sqlite3_column_int(stmt,2);
			m.msgcontent = (const char*)sqlite3_column_text(stmt, 3);
			m.msgres = (const char*)sqlite3_column_text(stmt, 4);
			m.datetime = (const char*)sqlite3_column_text(stmt, 5);
			m.font.name = (const char*)sqlite3_column_text(stmt, 6);
			m.font.size = sqlite3_column_int(stmt, 7);
			m.font.bold = sqlite3_column_int(stmt, 8)==1?true:false;
			m.font.italics = sqlite3_column_int(stmt, 9)==1?true:false;
			m.font.underline = sqlite3_column_int(stmt, 10)==1?true:false;
			m.font.color = sqlite3_column_int(stmt, 11);
			v.push_back(m);
		}
		else if (ret == SQLITE_DONE)
		{
			break;
		}
		else
		{
			//Failed Select
			OutputDebugStringA("Failed select rows in  msg_table \n");
			if (stmt != null)
			{
				sqlite3_finalize(stmt);
				return DB_FAILED;
			}
		}

	} while (1);
	sqlite3_finalize(stmt);
	return DB_OK;
}

bool MsgDb::IsExist(const string& id, e_msginfo_field field, e_comparison_operators comparison, const string& value, int num /*= 1*/)
{
	string strField;
	string strComparison;
	switch (field)
	{
	case e_msginfo_id:
		strField ="id";
		break;
	case e_msginfo_msgid:
		strField = "msgid";
		break;
	case e_msginfo_fromto:
		strField = "fromorto";
		break;
	case e_msginfo_msgcontent: // 一般不会根据这个匹配， 根据这个一般是用like匹配查找内容
		strField = "msgcontent";
		break;
	case e_msginfo_msgres:
		strField = "msgres";
		break;
	case e_msginfo_date:
		strField = "datetime";
		break;
	case e_msginfo_font:  // 一般也不会根据字体匹配
		strField ="" ;
		break;
	default:  break;
	}
	switch(comparison)
	{
	case e_biger:
		strComparison = ">";
		break;
	case e_less:
		strComparison = "<";
		break;
	case e_bigerorequal:
		strComparison = ">=";
		break;
	case e_lessorequal:
		strComparison = "<=";
		break;
	}
	/*
	select count(*) from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-11-30 24:00:00') order by msgid asc limit
	*/
	char szSql[512]= {0x0};
	sprintf_s(szSql,  "select count(*) from tb_msg where id='%s' and %s %s '%s' order by msgid asc limit %d", id.c_str(), strField.c_str(), strComparison.c_str(), value.c_str(), num);


	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql,true) != DB_OK)
	{
		OutputDebugStringA("QueryMsg Failed \n");
		return false;
	}

	int ret = -1;
	int nCout = -1;
	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW)
		nCout =sqlite3_column_int(stmt,0);

	if (nCout >=  num)
		return true;
	return false;
}

int MsgDb::QueryMsgCollection(vector<msginfo>& v, const string& id, const string& formDate, const string & toDate )
{
	/*
	select * from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-11-30 24:00:00') 
	and datetime >datetime('2014-11-30 00:00:00')  order by msgid asc
	*/
	char szSql[512] = {0x0};
	sprintf_s(szSql, "select * from tb_msg where id='wdemo1:w148' and datetime < datetime('%s') and datetime >datetime('%s') order by msgid asc", toDate.c_str(), formDate.c_str());
	return QueryMsgCollection(v, szSql);
}
//消息（聊天记录）集合查询

int MsgDb::QueryMsgCollection(vector<msginfo>& v, const string&  id, int msgid , bool bLessComparison/*=true*/,  bool bAsc/*=FALSE */, int num/*=50*/,int offset/*=0*/ )
{
	//此处id为好友Id
	char szSql[512] = {0x0};
	//select * from tb_msg where id='wdemo1:w148' and  msgid < 1000 order by  msgid  desc limit 51 offset 0
# if 0 
	//  old code query all message from tb_msg
	sprintf_s(szSql, "select * from tb_msg where id='%s' order by msgid asc;", id.c_str());
#else
	
	  
	sprintf_s(szSql,"select * from tb_msg where id='%s' and msgid %s %d order by msgid  %s limit %d offset %d", id.c_str(), bLessComparison?"<":">",msgid, bAsc?"asc":"desc", num, offset );
#endif
	return QueryMsgCollection(v, szSql);
	
}
 

//查询单条消息
int MsgDb::QueryMsg(msginfo& msg, const string & id, int msgid)
{
	char szSql[128] = {0x0};
	sprintf_s(szSql, "select * from tb_msg where msgid='%d' order by msgid desc;", msgid);
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql, true) != DB_OK)
	{
		OutputDebugStringA("Query msg  form tb_msg failed \n");
			return DB_FAILED;
	}
	
	int ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW)
	{
		msg.id = (const char*)sqlite3_column_text(stmt, 0);
		msg.msgid = sqlite3_column_int(stmt, 1);
		msg.fromorto = sqlite3_column_int(stmt,2);
		msg.msgcontent = (const char*)sqlite3_column_text(stmt, 3);
		msg.msgres = (const char*)sqlite3_column_text(stmt, 4);
		msg.datetime = (const char*)sqlite3_column_text(stmt, 5);
		msg.font.name = (const char*)sqlite3_column_text(stmt, 6);
		msg.font.size = sqlite3_column_int(stmt, 7);
		msg.font.bold = sqlite3_column_int(stmt, 8)==1?true:false;
		msg.font.italics = sqlite3_column_int(stmt, 9)==1?true:false;
		msg.font.underline = sqlite3_column_int(stmt, 10)==1?true:false;
		msg.font.color = sqlite3_column_int(stmt, 11);
	}
	sqlite3_finalize(stmt);
	return 0;
}

int MsgDb::AddGroup(const group& g)
{
	char szSql[1024] = {0x0};
	sprintf_s(szSql, "insert into tb_groups values('%s', '%s','%d');", g.id.c_str(), g.name.c_str(), g.system);
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql) == DB_FAILED)
	{
		OutputDebugStringA("Add Group Failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

int MsgDb::QueryGroupCollection(vector<group>& v)
{
	char* strSql = "select * from tb_groups;";
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, strSql,true) != DB_OK)
	{
		OutputDebugStringA("QueryGroupCollection Method Failed \n");
		return DB_FAILED;
	}

	group g;
	int ret = -1;
	do 
	{
		int nCols = sqlite3_column_count(stmt);
		ret = sqlite3_step(stmt);
		if (ret== SQLITE_ROW)
		{
			g.id  = (const char*)sqlite3_column_text(stmt, 0);
			g.name = (const char*)sqlite3_column_text(stmt, 1);
			g.system = sqlite3_column_int(stmt, 2)==1?true:false;
			v.push_back(g);
		}
		else if (ret == SQLITE_DONE)
		{
			break;
		}
		else
		{
			//Failed Select
			OutputDebugStringA("Failed select rows in  msg_table \n");
			if (stmt != null)
			{
				sqlite3_finalize(stmt);
				return DB_FAILED;
			}
		}

	} while (1);
	return DB_OK;
}

int MsgDb::AddFriend(const friendsinfo& f)
{
	char szSql[1024] = {0x0};
	sprintf_s(szSql, "insert into tb_friends values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s', '%s', '%s','%s','%d','%s', '%s');", f.type.c_str(), f.membersource.c_str(), f.id.c_str(), f.nickname.c_str(), f.sex.c_str(), f.picurl.c_str(), f.signature.c_str(), f.tempmember, f.mobile.c_str(), f.telephone.c_str(), f.address.c_str(), f.email.c_str(), f.init, f.post.c_str(), f.department.c_str());

	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql) == DB_FAILED)
	{
		OutputDebugStringA("Add Friend Failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

int MsgDb::DeleteFriend(const friendsinfo& f)
{
	char szSql[256] = {0x0};
	sprintf_s(szSql, "delete from tb_friends where id='%s';", f.id.c_str());
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql) != DB_OK)
	{
		return DB_FAILED;
	}
	return DB_OK;
}

int MsgDb::UpdateFriend(const friendsinfo& f)
{
	char szSql[1024] = {0x0};
	sprintf_s(szSql, "update tb_friends set type='%s',memsrc='%s',nickname='%s',sex='%s',picurl='%s', signature='%s',tempmember='%d',mobile='%s', telephone='%s',address='%s',email='%s',init='%d',post='%s',department='%s' where id='%s';",f.type.c_str(), f.membersource.c_str(), f.nickname.c_str(),f.sex.c_str(),f.picurl.c_str(),f.signature.c_str(), f.tempmember, f.mobile.c_str(), f.telephone.c_str(), f.address.c_str(), f.email.c_str(), f.init,f.post.c_str(), f.department.c_str(), f.id.c_str());;
	if (ManipulateSql(null, szSql) != DB_OK)
	{
		OutputDebugStringA("Update friendinfo failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

//获取当前用户的好友信息，
//goupid 若为空， 则获取所有好友的信息， 不为空则获取对应的组的信息
int MsgDb::QueryFriendsCollection(vector<friendsinfo>& v, const string & goupid/*=""*/, const string& groupname/*=""*/)
{
	char szSql[1024] = {0x0};
	if (goupid == "" && groupname=="")
		strcpy (szSql, "select * from tb_friends;");
	else
		// 此处可以根据goupid判断是否存在这个组 , 需要更改数据表（懒得改）
		sprintf_s(szSql, "select * from tb_friends where  department='%s';");
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql,true) != DB_OK)
	{
		OutputDebugStringA("Query Friendsinfo Failed \n");
		return DB_FAILED;
	}

	friendsinfo f;
	int ret = -1;
	do 
	{
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW)
		{
			f.type = (const char*)sqlite3_column_text(stmt, 0);
			f.membersource = (const char*)sqlite3_column_text(stmt, 1);
			f.id = (const char*)sqlite3_column_text(stmt, 2);
			f.nickname = (const char*)sqlite3_column_text(stmt, 3);
			f.sex = (const char*)sqlite3_column_text(stmt, 4);
			f.picurl = (const char*)sqlite3_column_text(stmt, 5);
			f.signature = (const char*)sqlite3_column_text(stmt, 6);
			f.tempmember = sqlite3_column_int(stmt, 7);
			f.mobile = (const char*)sqlite3_column_text(stmt, 8);
			f.telephone = (const char*)sqlite3_column_text(stmt, 9);
			f.address = (const char*)sqlite3_column_text(stmt, 10);
			f.email = (const char*)sqlite3_column_text(stmt, 11);
			f.init = sqlite3_column_int(stmt, 12);
			f.post = (const char*)sqlite3_column_text(stmt, 13);
			f.department = (const char*)sqlite3_column_text(stmt, 14);
			v.push_back(f);
		}
		else if (ret == SQLITE_DONE)
		{
			break;
		}
		else
		{
			if (stmt != null)
			{
				sqlite3_finalize(stmt);
			}
			OutputDebugStringA("Query Friendsinfo Failed \n");
			return DB_FAILED;
		}
	} while (1);
	
	sqlite3_finalize(stmt);
	return DB_OK;
}


void MsgDb::FinalRelease()
{
	delete this;
}

//此函数后期可以进行优化，当持续插入多条数据的时候可以使用
//事务，提高存储效率
int MsgDb::AddMsg(const msginfo& msg, const string & id/*=""*/)
{
	//此处实际上使用不到id 参数
	char szSql[4096] = {0x0};
	sprintf_s(szSql,"insert into tb_msg values ('%s',null, '%d', '%s', '%s','%s', '%s', '%d', '%d', '%d', '%d', '%d');",msg.id.c_str(), msg.fromorto, msg.msgcontent.c_str(), msg.msgres.c_str(), msg.datetime.c_str(), msg.font.name.c_str(), msg.font.size, msg.font.bold, msg.font.italics, msg.font.underline, msg.font.color);
	sqlite3_stmt* stmt = null;
	if (ManipulateSql(&stmt, szSql) != DB_OK)
	{
		OutputDebugStringA("Insert Msg Failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

int MsgDb::DeleteMsg(int  msgid, const string & id)
{
	//因为Msgid 是唯一的所以此处用不到用户Id
	char szSql[128]= {0x0};
	sprintf_s(szSql, "delete from tb_msg where msgid = '%d';", msgid);
	if (ManipulateSql(null, szSql) != DB_OK)
	{
		OutputDebugStringA("Delete msg  Failed \n ");
		return DB_FAILED;
	}
	return DB_OK;
}

int MsgDb::DeleteMsgCollection(int msgidfrom, int nCounts , const string & id )
{
	char szSql[256] = {0x0};
	sprintf_s(szSql, "delete from tb_msg where msgid in (select msgid from tb_msg  where (id = '%s'  and msgid >='%d') order by msgid limit '%d');" , id.c_str(), msgidfrom, nCounts);
	if (ManipulateSql(null, szSql) != DB_OK)
	{
		OutputDebugStringA("Delete Msg Collection Failed \n");
		return DB_FAILED;
	}
	return DB_OK;
}

IMsgDb* CreateMsgDb()
{
	return new MsgDb();
}

IPath::IPath()
{

}

IPath::~IPath()
{

}

//创建文件目录， 一般而言文件目录都不会创建在c盘中
//一般而言此目录为整个Im客户端保存信息的根文件目录
//此方法可以用来随意创建目录
int IPath::SetDirectory(LPCTSTR lptStr)
{

	_stprintf_s(IPath::s_tszPath, _T("%s"), lptStr );
	char* szPath=NULL;
	W2C(&szPath, lptStr);
	strcpy_s(IPath::s_szPath, szPath);
	SAFE_ARRYDELETE(szPath);

	//TCHAR* tszPath = IPath::s_tszPath;
	TCHAR tszPath[MAX_PATH] = {0x0};
	_tcscpy_s(tszPath, IPath::s_tszPath);
	TCHAR* ptszTok = _tcstok(tszPath, _T("\\"));

	TCHAR tszTempPath[MAX_PATH] = {'\0'};

	while(ptszTok != NULL)
	{
		int pos = 0; //记录当前文件上一次目录结束位置（字符串中）
		if (tszTempPath[0] == '\0')
		{
			_tcscpy_s(tszTempPath, ptszTok);
		}
		else
		{
			pos = _tcslen(tszTempPath);
			_stprintf_s(tszTempPath, _T("%s\\%s"), tszTempPath, ptszTok);
		}
	

		//res 0: 文件存在，
		//	  1：创建成功 
		//	  -1：创建失败
		int res = 0; 
		if (!CreateDirectory(tszTempPath, NULL))
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				//文件夹已经存在
				res = 0;
			else
				//创建失败
				res = -1;
		}
		else
			res = 1;

		if (res >= 0)
		{
			//创建成功 或者文件存在
			ptszTok =  _tcstok(NULL, _T("\\"));
		}
		else
		{
			//创建失败后 处理可以 删除 之前已经创建的
			tszTempPath[pos] = '\0';
			RemoveDirectory(tszTempPath);
			return DB_FAILED;
			
		}
	}

	return DB_OK;

}


//	当客户端端新添加一个用户的时候， 需要给他分配一个
//	单独的文件夹来存储他的图片本地数据库等等
int IPath::SetUserDirectory(LPCTSTR lptStr, LPCTSTR lptType/* = NULL */)
{
	//lptstr 为 用户户的ID, 此处根据用户的ID来创建一个文件夹

	//先判断当前的Im存储文件夹是否存在， 不存在
	//则先创建目录， 然后再在其中创建子当前用户的子目录
	//父目录不存在 重新创建

	if (!IPath::IsDirectoryExist(IPath::s_tszPath))
	{
		if (IPath::SetDirectory(IPath::s_tszPath) != DB_OK)
			return DB_FAILED;
	}

	//父目录存在或者 重新创建成功， 创建当前用户目录

	TCHAR tszPath[MAX_PATH] = {0x0};
	_stprintf_s(tszPath,_T("%s\\%s"),IPath::s_tszPath, lptStr);

	DWORD dbError = 0;
	if (!CreateDirectory(tszPath, NULL))
	{
		dbError= GetLastError();
#ifdef _DEBUG
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dbError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		OutputDebugString((LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
#endif
	}
	
	if (dbError == 183 || dbError == 0)
	{
		//给用户创建好友头像目录
		TCHAR tszSubDir[MAX_PATH] = {0};
		_stprintf_s(tszSubDir, _T("%s\\%s"), tszPath, _T("FriendsFace"));
		if (!CreateDirectory(tszSubDir,NULL))
		{
			OutputDebugString(_T("FriendsFace directory create failed \n"));
			//return DB_FAILED;
		}

		_stprintf_s(tszSubDir, _T("%s\\%s"), tszPath,_T("user"));
		if (!CreateDirectory(tszSubDir, NULL))
		{
			OutputDebugString(_T("User directory create failed \n"));
			//return DB_FAILED;
		}
	}
	else
		return DB_FAILED;

	return DB_OK;
}

/*
	删除一个文件夹中的子文件和文件夹	
*/
BOOL IPath::DeleteFolder(LPCTSTR lpstrFolder)
{
	TCHAR tszFind[MAX_PATH] = {0x0};
	WIN32_FIND_DATA wfd;
	BOOL bRet ;
	_tcscpy_s(tszFind, MAX_PATH, lpstrFolder);
	_tcscat_s(tszFind, _T("\\*.*"));

	HANDLE hFind  = FindFirstFile(tszFind, &wfd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	bRet = TRUE;
	//遍历文件夹进行删除
	while (bRet)
	{
		bRet = FindNextFile(hFind, &wfd);
		//不是当前目录或者父目录
		if (!bRet)
		{
			break;
		}
		_tcscpy_s(tszFind, MAX_PATH, lpstrFolder);
		_tcscat_s(tszFind, _T("\\"));
		_tcscat_s(tszFind, wfd.cFileName);

		if (wfd.cFileName[0] != _T('.'))
		{
#if _DEBUG
			TCHAR tszPath[MAX_PATH] = {0x0};
			wsprintf(tszPath,_T("%s\\%s\n"), lpstrFolder, wfd.cFileName);
			OutputDebugString(tszPath);
#endif	
			if (wfd.dwFileAttributes &  FILE_ATTRIBUTE_DIRECTORY)
			{
				//普通目录
				DeleteFolder(tszFind);
			}
			else
			{
				if (!DeleteFile(tszFind))
				{
					OutputDebugStringA("Delete fie failed \n");
					FindClose(hFind);
					return FALSE;
				}
				//文件
			}
		}
		else
			continue;
	}
	FindClose(hFind);
	BOOL bb =   RemoveDirectory(lpstrFolder);
	if (!bb)
	{
		DWORD dbError = GetLastError();
#ifdef _DEBUG
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dbError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		OutputDebugString((LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
#endif
	}
	return bb;
}

// BOOL IPath::DeleteFolder(LPCTSTR lpstrFolder)
// {
// 	if (DeleteFolderContent(lpstrFolder))
// 	{
// 		BOOL b = RemoveDirectory(lpstrFolder);
// 		return b;
// 	}
// 	return FALSE;
// }

//判断当前目录是否存在
BOOL IPath::IsDirectoryExist(LPCTSTR lptStr)
{
	WIN32_FIND_DATA wfd;
	BOOL b = FALSE;

//	LPCWSTR lpwStrPath = IPath::s_tszPath;
	HANDLE hFind  = FindFirstFile(lptStr, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) 
	{
		b = TRUE;
	}
	FindClose(hFind);
	return b;
}


//////////////////////////////////////////////////////////////////////
IIni::IIni()
{

}

IIni::~IIni()
{

}

CIni::CIni()
{
	//默认的路径是exe安装路径下的路径
	GetCurDir(m_tszFilePath,MAX_PATH);
	_tcscat_s(m_tszFilePath, _T("\\im.ini"));
}

CIni::~CIni()
{
	
}

void CIni::SetPath(LPCTSTR lpPath)
{
	_tcscpy_s(m_tszFilePath, lpPath);
}

void CIni::InitPath()
{
	// 	TCHAR tszCurDir[MAX_PATH] = {0x0};
	// 	GetCurDir(tszCurDir, MAX_PATH);
	TCHAR tszDir[MAX_PATH] = {0x0};
	BOOL bRet = ReadString(_T("file Directory"), _T("path"), NULL, tszDir, MAX_PATH);
	if (!bRet)
	{
		//配置文件中jiushi 没有读到，写一个默认路径
		OutputDebugStringA("No Path value in the Ini file \n" );
		memset(tszDir, 0x0, sizeof(TCHAR)*MAX_PATH);
		GetCurDir(tszDir, MAX_PATH);
		_tcscat_s(tszDir, _T("\\IMFileRecv"));
		WriteString(_T("file Directory"), _T("path"), tszDir);
	}

	if (IPath::SetDirectory(tszDir) == DB_FAILED)
		return;
	//IM目录中通用文件夹
	TCHAR tszPath[MAX_PATH] = {0x0};
	TCHAR* tstrCommon = _T("Common");
	_stprintf_s(tszPath,_T("%s\\%s"),  IPath::s_tszPath, tstrCommon );
	if (IPath::IsDirectoryExist(tszPath))
	{
		return;
	}
	CreateDirectory(tszPath, NULL);
	WriteString(_T("common file"), _T("path"), tszPath);
	//IPath::SetUserDirectory(_T("1004462060"));
}

BOOL CIni::WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	return WritePrivateProfileString(lpAppName, lpKeyName,lpString, m_tszFilePath);
}

DWORD CIni::ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR buff, DWORD nSize)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buff,nSize,m_tszFilePath);
}

UINT CIni::ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault)
{
	return  GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_tszFilePath) ;
}

void CIni::FinalRelease()
{
	delete this;
}

IIni*  CreateIni()
{
	return new CIni();
}

//////////////////////////////////////////////////////////////
//接口函数IM目录的设置
void SeDirectory(LPCTSTR  lptStr)
{
// 	TCHAR tszCurDir[MAX_PATH] = {0x0};
// 	GetCurDir(tszCurDir, MAX_PATH);
	TCHAR tszStr[30] = {0x0};
	_tcscpy(tszStr, lptStr);
	_tstrreplace(tszStr,_T(':'), _T(' '));
	CIni* pIni = new CIni();
	if (IPath::SetDirectory(tszStr) == DB_FAILED)
		return;
	//IM目录中通用文件夹
	BOOL bRet = pIni->WriteString(_T("file Directory"), _T("path"), tszStr);
	TCHAR tszPath[MAX_PATH] = {0x0};
	TCHAR* tszCommon = _T("Common");
	_stprintf_s(tszPath,_T("%s\\%s"),  IPath::s_tszPath, tszCommon );

	

	CreateDirectory(tszPath, NULL);
	//IPath::SetUserDirectory(_T("1004462060"));
}