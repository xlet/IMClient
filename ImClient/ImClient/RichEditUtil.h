/*
	richedi中一些常用的函数
*/


void RichEdit_SetDefFont(ITextServices * pTextServices, LPCTSTR lpFontName,		// 设置默认字体
	int nFontSize,	COLORREF clrText, BOOL bBold, 
	BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);

DWORD RichEdit_GetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

IRichEditOle* RichEdit_GetOleInterface(ITextServices * pTextServices);

void RichEdit_GetText(ITextServices * pTextServices, tString& strText);	

int RichEdit_GetWindowTextLength(ITextServices * pTextServices);

int RichEdit_GetTextRange(ITextServices * pTextServices, CHARRANGE * lpchrg, tString& strText);

int RichEdit_SetSel(ITextServices * pTextServices, LONG nStartChar, LONG nEndChar);

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText,	// 替换选中文本
	LPCTSTR lpFontName, int nFontSize,	COLORREF clrText, 
	BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink, 
	int nStartIndent, BOOL bCanUndo = FALSE);

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);

void RichEdit_GetSel(ITextServices * pTextServices, LONG& nStartChar, LONG& nEndChar);

void RichEdit_SetFont(ITextServices * pTextServices, LPCTSTR lpFontName, int nFontSize,	// 设置字体
	COLORREF clrText, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);

BOOL RichEdit_SetStartIndent(ITextServices * pTextServices, int nSize);			// 设置左缩进(单位:缇)

DWORD RichEdit_GetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);

BOOL RichEdit_SetLinePacing(ITextServices* pTextServices, int nLinePace);