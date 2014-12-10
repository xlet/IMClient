#include "StdAfx.h"
#include "PictureProcess.h"


CPictureProcess::CPictureProcess(void)
{
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}


CPictureProcess::~CPictureProcess(void)
{
	GdiplusShutdown(m_gdiplusToken);
}

int CPictureProcess::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0) {
		return -1;
	}
	pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL) {
		return -1;
	}
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j=0; j< num; ++j) {
		if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

HICON CPictureProcess::GetIcon(const WCHAR*  filename)
{
	//
	TImageInfo* p = CRenderEngine::LoadImage(filename);
	Bitmap* pBmp  = Bitmap::FromHBITMAP(p->hBitmap, NULL);
	HICON hIcon;
	pBmp->GetHICON(&hIcon);
	//
// 	Bitmap* pBmp = new Bitmap(filename);
// 	HICON hIcon;
// 	pBmp->GetHICON(&hIcon);
	return hIcon;
}

bool CPictureProcess::ToGray(const WCHAR* filename)
{
	Bitmap* pBmp = new Bitmap(filename);
	/* ****  *
	/********* */
	int width = pBmp->GetWidth();
	int height = pBmp->GetHeight();
	Rect rect(0, 0, width, height);
	BitmapData data;
	Status status ;
	status = pBmp->LockBits(&rect, ImageLockModeWrite, pBmp->GetPixelFormat(), &data);
	if (status != 0)
	{
		pBmp->UnlockBits(&data);
		return false;
	}
	unsigned char*   p = (unsigned char*)(data.Scan0);
	int offset = data.Stride - width*3;
	int i = 0, j = 0;
	BYTE r, g, b;
	for (i=0; i < height; ++i)
	{
		for (j=0; j <width; ++j)
		{
			b= p[0];
			g= p[1];
			r = p[2];
			p[0] = p[1] = p[2] = (BYTE)(0.299f*r + 0.578f*g + 0.114*b);
			//p[0] = p[1] = p[2] = (BYTE)(r/3.0f + g/3.0f + b/3.0f); //均值法
			p+=3;
		}
		p+=offset;
	}
	pBmp->UnlockBits(&data);


	CLSID encoderClsid;
	WCHAR wstrFormat[20] = {0x0};
	WCHAR wstrSuffix[5] = {0x0};
	wcscpy(wstrSuffix, wcsrchr(filename,L'.')+1);
	if (_wcsicmp(wstrSuffix, L"jpg") == 0)
	{
		wcscpy(wstrSuffix, L"jpeg");
	}
	swprintf_s(wstrFormat, L"%s/%s", L"image", wstrSuffix);
	GetEncoderClsid(/*L"image/jpeg"*/wstrFormat, &encoderClsid);

	WCHAR wstrNewFilePath[513] = {0x0};

	const WCHAR* pname = wcsrchr(filename, L'\\') +1; // 不带路径文件名称
	//int len = pname-filename;//wcsrchr(filename, L'\\')-(filename);
	wcsncpy(wstrNewFilePath , filename ,(pname-filename) );
	//swprintf_s(wstrNewFilePath, L"g%s", filename);
	wcscat(wstrNewFilePath, L"gray.");
	wcscat(wstrNewFilePath,  pname);
	pBmp->Save(wstrNewFilePath, &encoderClsid, NULL); 
	delete pBmp;
	return true;
}

