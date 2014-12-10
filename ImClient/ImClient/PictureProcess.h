#pragma once
#include  <tchar.h>
#include <windows.h>
#include <GdiPlus.h>
#include  <GdiPlusInit.h>
#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;

class CPictureProcess
{
public:
	CPictureProcess(void);
	~CPictureProcess(void);
public:

	bool ToGray(const WCHAR*  filename);
	HICON GetIcon(const WCHAR*  filename);
private:
	int CPictureProcess::GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	GdiplusStartupInput   m_gdiplusStartupInput;
	ULONG_PTR               m_gdiplusToken;
};

