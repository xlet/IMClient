#pragma once
#include "Path.h"

#include "Utils.h"

#include <vector>

#include "XmlDocument.h"

class CFaceInfo
{
public:
	CFaceInfo(void);
	~CFaceInfo(void);

public:
	int m_nId; //xml中ID
	int m_nIndex; //gif名前缀
	tString m_strTip;
	tString m_strFileName;
};

// class CFaceInfoKey
// {
// public:
// 	CFaceInfoKey();
// 	~CFaceInfoKey();
// 
// 	int  m_nId;   // 默认按照m_nId排序
// 	tString m_tstr; // 表情名字
// 
// 	
// };


//表情是按xml中顺序顺序排列的

class CFaceList
{
public:
	CFaceList(void);
	~CFaceList(void);

public:
	void Reset();
	BOOL LoadConfigFile(LPCTSTR lpszFileName);
	CFaceInfo * GetFaceInfo(int nIndex);   //根据表情列表中索引
	CFaceInfo * GetFaceInfoById(int nFaceId); //根据表情xml中id查找
	CFaceInfo * GetFaceInfoByFaceTip(LPCTSTR lptStr);
	CFaceInfo * GetFaceInfoByIndex(int nFaceIndex);

public:
	int m_nItemWidth, m_nItemHeight;
	int m_nZoomWidth, m_nZoomHeight;
	int m_nRow, m_nCol;
	//注意下面vector 和 map里面的都是同一个指针释放的释放时候一次就行
	std::vector<CFaceInfo *> m_arrFaceInfo;
	//std::map<int, CFaceInfo*> m_mapFaceinfo;
	
};