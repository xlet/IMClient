#pragma once
class CCaptionWnd:public WindowImplBase
{
public:
	CCaptionWnd(void);
	virtual ~CCaptionWnd(void);

	//virtual LPCTSTR GetWindowClassName( ) const{return _T("loginWnd");} 
	//virtual CDuiString GetSkinFile(){return m_dstrXml;/*_T("login.xml")*//*_T("sysset.xml")*/;}
	//UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	//virtual CDuiString GetSkinFolder(){return _T("");}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void       InitWindow(){}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	BOOL IsInStaticControl(CControlUI *pControl);
	LRESULT OnNcButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	//void Notify(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg){Close();};
};

