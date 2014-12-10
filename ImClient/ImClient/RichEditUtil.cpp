#include "StdAfx.h"
#include "RichEditUtil.h"
#include "Utils.h"

void RichEdit_SetDefFont(ITextServices * pTextServices, LPCTSTR lpFontName,		// 设置默认字体
	int nFontSize,	COLORREF clrText, BOOL bBold, 
	BOOL bItalic, BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(pTextServices, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetDefaultCharFormat(pTextServices, cf);
}

DWORD RichEdit_GetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lRes);
	return (DWORD)lRes;
}


BOOL RichEdit_SetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lRes);
	return (BOOL)lRes;
}

//获取richedit的OLE接口
IRichEditOle* RichEdit_GetOleInterface(ITextServices * pTextServices)
{
	IRichEditOle *pRichEditOle = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, NULL);
	return pRichEditOle;
}

//获取文本
void RichEdit_GetText(ITextServices * pTextServices, tString& strText)
{
	REOBJECT reobject;
	LONG nFaceId = 0;
	LONG  nPos = 0;  //记录OLE对象的位置
	tString strOrgText, strTemp;
	IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(pTextServices);
	if (NULL == pRichEditOle)
		return;
	
	CHARRANGE chrg = {0, RichEdit_GetWindowTextLength(pTextServices)};
	RichEdit_GetTextRange(pTextServices, &chrg, strOrgText);

	for (LONG i = 0; i < (int)strOrgText.size(); i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.cp = i;
		HRESULT hr = pRichEditOle->GetObject(REO_IOB_USE_CP, &reobject, REO_GETOBJ_POLEOBJ);
		if (SUCCEEDED(hr))
		{
			if (reobject.cp > 0 && reobject.cp > nPos)
			{
				strTemp = strOrgText.substr(nPos, reobject.cp-nPos);
				Replace(strTemp, _T("/"), _T("//"));
				strText += strTemp;
			}
			nPos = reobject.cp + 1;
			if (NULL == reobject.poleobj)
				continue;
			else
			{
				if (1/*CLSID_ImageOle == reobject.clsid*/)
				{
					//ImageOLE对象
				}
				nFaceId = reobject.dwUser;
				TCHAR tstr[24] = {0x0};
				wsprintf(tstr, _T("/f['%03d\']"), nFaceId);
				strText += tstr;
			}
		
		}
	}
	if (nPos < (int)strOrgText.size())
	{
		strTemp = strOrgText.substr(nPos);
		Replace(strTemp, _T("/"), _T("//"));
		strText += strTemp;
	}
	pRichEditOle->Release();
}

int RichEdit_GetWindowTextLength(ITextServices * pTextServices)
{
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(WM_GETTEXTLENGTH, 0, 0, &lRes);
	return (int)lRes;
}

int RichEdit_GetTextRange(ITextServices * pTextServices, CHARRANGE * lpchrg, tString& strText)
{
	strText = _T("");

	if (NULL == lpchrg || lpchrg->cpMax <= lpchrg->cpMin)
		return 0;

	LONG nLen = lpchrg->cpMax - lpchrg->cpMin;
	TCHAR * pText = new TCHAR[nLen+1];
	if (NULL == pText)
		return 0;

	memset(pText, 0, (nLen+1)*sizeof(TCHAR));

	TEXTRANGE tr = { 0 };
	tr.chrg = *lpchrg;
	tr.lpstrText = pText;

	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, &lRes);

	strText = pText;
	delete []pText;

	return (int)lRes;
}

int RichEdit_SetSel(ITextServices * pTextServices, LONG nStartChar, LONG nEndChar)
{
	CHARRANGE cr = { nStartChar, nEndChar };
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lRes);
	return (int)lRes;
}

// 替换选中文本
void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText,	
	LPCTSTR lpFontName, int nFontSize,	COLORREF clrText, 
	BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink, 
	int nStartIndent, BOOL bCanUndo/* = FALSE*/)
{
	long lStartChar = 0, lEndChar = 0;
	RichEdit_GetSel(pTextServices, lStartChar, lEndChar);
	RichEdit_ReplaceSel(pTextServices, lpszNewText,bCanUndo);
	lEndChar = lStartChar + _tcslen(lpszNewText);
	RichEdit_SetSel(pTextServices, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServices, lpFontName, nFontSize, clrText, bBold, bItalic, bUnderLine, bIsLink);
	RichEdit_SetStartIndent(pTextServices, nStartIndent);
	RichEdit_SetSel(pTextServices, lEndChar, lEndChar);
}

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText, BOOL bCanUndo/* = FALSE*/)
{
	pTextServices->TxSendMessage(EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText, NULL);
}


void RichEdit_GetSel(ITextServices * pTextServices, LONG& nStartChar, LONG& nEndChar)
{
	CHARRANGE cr = { 0, 0 };
	pTextServices->TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, NULL);
	nStartChar = cr.cpMin;
	nEndChar = cr.cpMax;
}

// 设置字体
void RichEdit_SetFont(ITextServices * pTextServices, LPCTSTR lpFontName, int nFontSize,
	COLORREF clrText, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(pTextServices, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetSelectionCharFormat(pTextServices, cf);
}

// 设置左缩进(单位:缇)
BOOL RichEdit_SetStartIndent(ITextServices * pTextServices, int nSize)
{
	PARAFORMAT2 pf2;
	memset(&pf2, 0, sizeof(pf2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = nSize;
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf2, &lRes);
	return (BOOL)lRes;
}

/*
	根据实际测试来看，bLineSpacingRule=5时，可以设置dyLineSpacing=30来设置1.5倍行距，所以最终决定用这个设置1倍以上的非整数行间距。

	CRichEditCtrl m_RichEdit;
	LONG lineSpace=(LONG)(X*20);//X为要设置的行间距

	PARAFORMAT2    pf;
	ZeroMemory(&pf, sizeof(pf));
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask|=PFM_LINESPACING;
	pf.bLineSpacingRule =5;  
	pf.dyLineSpacing  = lineSpace; 
	m_CRichEdit.SetParaFormat(pf);
*/
BOOL RichEdit_SetLinePacing(ITextServices* pTextServices, int nLinePace)
{
// 	PARAFORMAT2 pf2;
//  	memset(&pf2, 0x0 , sizeof(pf2));
//  	pf2.cbSize = sizeof(PARAFORMAT2);
//  	pf2.dwMask|=PFM_LINESPACING | PFM_SPACEAFTER;
//  	pf2.bLineSpacingRule = 3; //1.5倍行距 //bLineSpacingRule 设为3、4、5，dyLineSpacing才有效 
//  	pf2.dyLineSpacing = 1200;
// 	
// 	PARAFORMAT2 MyFormat; 
// 	ZeroMemory(&MyFormat, sizeof(MyFormat)); 
// 	MyFormat.cbSize=sizeof(MyFormat); 
// 	MyFormat.dwMask=PFM_LINESPACING; 
// 	MyFormat.dyLineSpacing=300; 
// 	MyFormat.bLineSpacingRule=LineSpace; 
	PARAFORMAT2 pf;
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_NUMBERING | PFM_OFFSET;
	pf.wNumbering = PFN_BULLET;//注意PFM_NUMBERING 
	pf.dxOffset = 100;
	//VERIFY(SetParaFormat(pf)); 
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETPARAFORMAT, SCF_SELECTION, (LPARAM)&pf, &lRes);
	return lRes;
}

DWORD RichEdit_GetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	LRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lRes);
	return (DWORD)lRes;
}


BOOL RichEdit_SetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	LRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lRes);
	return (BOOL)lRes;
}

