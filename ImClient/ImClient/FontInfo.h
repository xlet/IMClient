#pragma once
// 字体信息类
class CFontInfo
{
public:
	CFontInfo(void);
	~CFontInfo(void);
public:
	int			 m_nSize;
	COLORREF     m_clrText;
	tString      m_tstrName;
	BOOL         m_bBold;
	BOOL		 m_bItalic;
	BOOL		 m_bUnderLine;
};

