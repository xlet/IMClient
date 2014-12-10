#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include  <iostream> 
#include <string>

#include <tchar.h>
#include <Windows.h>
using namespace std;
#ifdef _DEBUG
#	pragma  comment (lib, "Sqlite3_d.lib")
#else
#	pragma  comment(lib, "Sqlite3.lib")
#endif
 int callback(void*,int,char**,char**);
//sqlite ceshi 
void dotest2()
{
	sqlite3* db  = NULL;
   if (sqlite3_open("mytest1.db", &db) != SQLITE_OK)
   {
	   printf("打开数据库失败");
	   return ;
   }
   else
	   printf("打开数据库成功");
    char* errmsg = NULL;
	
	int res = sqlite3_exec(db, "create table fuck(id integer primary key autoincrement,name varchar(100))",NULL,NULL,&errmsg);
	if (res != SQLITE_OK)
	{
		sqlite3_close(db);
		printf(errmsg);
		sqlite3_free(errmsg);
		return ;
	}
	string strSql;
	strSql += "begin;\n";
	for(int i=0; i < 100; i++)
	{
		strSql += "insert into fuck values(null,'heh');\n";

	}
	  strSql+="commit;";
	  res = sqlite3_exec(db, strSql.c_str(), NULL, NULL, &errmsg);
	  if (res != SQLITE_OK)
	  {
		  sqlite3_close(db);
		  printf(errmsg);
		  sqlite3_free(errmsg);
		 return;
	  }

	  strSql = "select *  from fuck";
	  res = sqlite3_exec(db, strSql.c_str(),callback,NULL,&errmsg) ;
	  if (res != SQLITE_OK)
	  {
		  sqlite3_close(db);
		 printf(errmsg);
		 sqlite3_free(errmsg);
	  }
	  sqlite3_close(db);
}

int callback(void* ,int nCount,char** pValue,char** pName)
{
		string s;
		for(int i=0;i<nCount;i++)
		{
				s+=pName[i];
				s+=":";
				s+=pValue[i];
				s+="\n";  
		}
	printf(s.c_str());
	return 0;
	}

void doTest()
{
	sqlite3* conn = NULL;
	//1 打开数据库
	int result = sqlite3_open("D:\\vs2010\\mytest.db", &conn);
	if (result != SQLITE_OK)
	{                                                                                                                                                                                                                                                                                                                                     
		sqlite3_close(conn);
		return;
	}

	const char* createTableSQL = "CREATE TABLE TESTTABLE(int_col INT, float_col REAL, string_col TEXT)";
	sqlite3_stmt* stmt = NULL;
	int len = strlen(createTableSQL);
	//2、创建数据表，如果创建失败需要用Sqlite3_finalize释放sqlite3_stmt对象， 防止内存泄露
	if(sqlite3_prepare_v2(conn,createTableSQL, len, &stmt, NULL)!= SQLITE_OK)
	{
		if (stmt != NULL)
		{
			sqlite3_finalize(stmt);
		}
		sqlite3_close(conn);
		return;

	}
	//3、
	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		sqlite3_close(conn);
		return;
	}
	//4、释放创建表语句对象资源
	sqlite3_finalize(stmt);
	printf("succeed to create test table now.\n");

	//5、为后面的查询操作插入测试数据
	sqlite3_stmt* stmt2 = NULL;
	const char* insertSQL = "INSERT INTO TESTTABLE VALUES(20,21.0,'this is a test')";
	if (sqlite3_prepare_v2(conn, insertSQL, strlen(insertSQL),&stmt2, NULL) != SQLITE_OK)
	{
		if (stmt2)
		{
			sqlite3_finalize(stmt2);
		}
		sqlite3_close(conn);
		return;
	}
	if (sqlite3_step(stmt2) != SQLITE_DONE)
	{
		sqlite3_finalize(stmt2);
		sqlite3_close(conn);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	printf("succeed to insert test data \n");
	sqlite3_finalize(stmt2);
	//6 执行select 语句
	const char* selectSQL = "SELECT * FROM TESTTABLE";
	sqlite3_stmt* stmt3  = NULL;
	if (sqlite3_prepare_v2(conn, selectSQL,strlen(selectSQL),&stmt3, NULL) != SQLITE_OK)
	{
		if (stmt3)
		{
			sqlite3_finalize(stmt3);
		}
		sqlite3_close(conn);
		return;
	}
	//读取列数
	int filedCount = sqlite3_column_count(stmt3);
	do 
	{
		int r = sqlite3_step(stmt3);
		if (r== SQLITE_ROW)
		{
		
				for (int i=0; i < filedCount; i++)
				{
					int vtype = sqlite3_column_type(stmt3,i);
					if (vtype == SQLITE_INTEGER)
					{
						int v= sqlite3_column_int(stmt3, i);
						printf("the integer value is %d.lib.\n" ,v);
					}else if (vtype == SQLITE_FLOAT)
					{
						double v = sqlite3_column_double(stmt3, i);
						printf("the double value is %f.\n", v);
					} 
					else if (vtype == SQLITE_TEXT)
					{
						const char*  v = (const char*)sqlite3_column_text(stmt3, i);
						printf("the text value is %s.\n", v);
					}
					else if (vtype == SQLITE_NULL)
					{
						printf("the value is null.\n");
					}
				}
	
			
		}
		else if ( r== SQLITE_DONE)
		{
			printf("select finished .\n");
			break;
		}
		else 
		{
			printf("Failed to select .\n");
			sqlite3_finalize(stmt3);
			sqlite3_close(conn);
			return ;
		}
	} while (true);

	sqlite3_finalize(stmt3);
	//为了方便调试删除该函数创建的数据表，否则下此无法创建
	const char* dropSQL = "DROP TABLE TESTTABLE";
	sqlite3_stmt* stmt4  = NULL;
	if (sqlite3_prepare_v2(conn, dropSQL, strlen(dropSQL), &stmt4, NULL) != SQLITE_OK)
	{
		if (stmt4)
		{
			sqlite3_finalize(stmt4);
		}
		sqlite3_close(conn);
		return;
	}
	if (sqlite3_step(stmt4) == SQLITE_DONE)
	{
		printf("the testtable has been dropped.\n");
	}
	sqlite3_finalize(stmt4);
	sqlite3_close(conn);
}

void dotest3()
{
	string str = "fdfdfdf"\
		"fdsfd"\
		"fdfsdf都是";
		cout<<str;
}

typedef  bool(*pCreateDirectory)(LPCTSTR , void*);
void dotest4()
{
 	SECURITY_ATTRIBUTES attribute;
 	attribute.nLength = sizeof(attribute);
 	attribute.lpSecurityDescriptor = NULL;
 	attribute.bInheritHandle = FALSE;

	bool b = ::CreateDirectory(_T("D:"), &attribute);
	//b = ::(_T("D:\\My Documents\\IM2014\\IMDDD"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		int i = 0;
	}
// 	HINSTANCE hdll;
// 	hdll = LoadLibrary(_T("kernel32.dll"));
// 	if (hdll == NULL)
// 	{
// 		//ShowMessage(SysErrorMessage(GetLastError()));
// 		return;
// 	}
// 	pCreateDirectory CreateDirectory_;
// 	CreateDirectory_ = (pCreateDirectory)GetProcAddress(hdll, "CreateDirectory");
// 
// 	(*CreateDirectory_)(_T("D:\\My Documents\\IM2014\\IMDDD"), 0);
}

int main(int argc, char **argv)
{
#if 0
	int rc, i, ncols;


	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *sql;
	const char *tail;
	//打开数据
	rc = sqlite3_open("foods.db", &db);

	if(rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	sql = "select * from episodes";
	//预处理 转化sql语句3
	rc = sqlite3_prepare(db, sql, (int)strlen(sql), &stmt, &tail);

	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
	}

	rc = sqlite3_step(stmt);
	ncols = sqlite3_column_count(stmt); 

	while(rc == SQLITE_ROW) {

		for(i=0; i < ncols; i++) {
			fprintf(stderr, "'%s' ", sqlite3_column_text(stmt, i));
		}

		fprintf(stderr, "\n");

		rc = sqlite3_step(stmt);
	}
	//释放statement
	sqlite3_finalize(stmt);
	//关闭数据库
	sqlite3_close(db);
#endif
	doTest();
	//dotest2();
	//dotest3();
	//dotest4();
	std::string str = "卧槽尼玛aaa";
	int len = 0;
	len = str.length();
	std::cin.get();
	
	return 0;    
}
