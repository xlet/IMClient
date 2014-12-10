#include "StdAfx.h"
#include "Path.h"

namespace ZYM
{

CPath::CPath(void)
{
}

CPath::~CPath(void)
{
}

// 获取应用程序执行路径
tString CPath::GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = {0};

	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		tString strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}
	return szPath;
}

// 获取应用程序当前目录
tString CPath::GetCurDir()
{
	TCHAR szCurDir[MAX_PATH] = {0};
	//::GetCurrentDirectory(MAX_PATH, szCurDir);

	//if (szCurDir[_tcslen(szCurDir) - 1] != _T('\\'))
	//	_tcscat(szCurDir, _T("\\"));

	_tcscat(szCurDir, CPaintManagerUI::GetInstancePath());
	return szCurDir;
}

// 获取当前系统的临时文件夹的路径
tString CPath::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = {0};

	::GetTempPath(MAX_PATH, szTempPath);

	if (szTempPath[_tcslen(szTempPath) - 1] != _T('\\'))
		_tcscat(szTempPath, _T("\\"));

	return szTempPath;
}

// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
tString CPath::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
}

// 获取随机文件名(全路径)
tString CPath::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	tString strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = {0};

	if (!IsDirectoryExist(lpszPath))
		strPath = GetCurDir();
	else
		strPath = lpszPath;

	strFileName = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (int i = 2; i < 10000; i++)
	{
		if (strExtFileName.empty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}
		
		if (!IsFileExist(strFullPath.c_str()))
			return strFullPath;
	}

	return _T("");
}

// 检测指定路径是否目录
BOOL CPath::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

// 检测指定文件是否存在
BOOL CPath::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	if(::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)
		return TRUE;
	else
		return FALSE;
}

// 检测指定目录是否存在
BOOL CPath::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

BOOL CPath::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR cPath[MAX_PATH] = {0};
	TCHAR cTmpPath[MAX_PATH] = {0};
	TCHAR * lpPos = NULL;
	TCHAR cTmp = _T('\0');

	if (NULL == lpPathName || NULL == *lpPathName)
		return FALSE;

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (int i = 0; i < (int)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
			cPath[i] = _T('/');
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (lpPos != NULL)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp = *lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}

	return TRUE;
}

// 获取指定路径的根目录信息
tString CPath::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.find(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// 返回指定路径字符串的目录信息
tString CPath::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// 返回指定路径字符串的文件名和扩展名
tString CPath::GetFileName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// 返回不具有扩展名的路径字符串的文件名
tString CPath::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	nPos = strPath.rfind(_T('.'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos);

	return strPath;
}

// 返回指定的路径字符串的扩展名
tString CPath::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::size_type nPos = strPath.rfind(_T('.'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// 根据指定的相对路径获取绝对路径
tString CPath::GetFullPath(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	TCHAR cFirstChar = strPath.at(0);
	TCHAR cSecondChar = strPath.at(1);

	if (cFirstChar == _T('/'))
	{
		tString strCurDir = GetAppPath();
		tString strRootPath = GetPathRoot(strCurDir.c_str());
		return strRootPath + strPath;
	}
	else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
	{
		return strPath;
	}
	else
	{
		tString strCurDir = GetAppPath();
		return strCurDir + strPath;
	}
}

}