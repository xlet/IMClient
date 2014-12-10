#pragma once
#include "CaptionWnd.h"
class CUISysSetDlg:public CCaptionWnd
{
public:
	CUISysSetDlg(void);
	~CUISysSetDlg(void);
	virtual LPCTSTR GetWindowClassName( ) const{return _T("SyssetWnd");} 
	virtual CDuiString GetSkinFile(){return _T("sysset.xml");}
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; }
	virtual CDuiString GetSkinFolder(){return _T("");}
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND hWnd);
};

