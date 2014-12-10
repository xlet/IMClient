#include "StdAfx.h"
#include "FontInfo.h"


CFontInfo::CFontInfo(void)
{
	m_nSize = 9;
	m_clrText = RGB(0,0,0);
	m_tstrName = _T("Î¢ÈíÑÅºÚ");
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderLine = FALSE;
}


CFontInfo::~CFontInfo(void)
{
}