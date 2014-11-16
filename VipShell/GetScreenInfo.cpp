#include ".\getscreeninfo.h"
#include <windows.h>
#include <tchar.h>
#include "..\VipShellInfo\Lzw.h"

void LZW_Decode(BYTE* lzwBuf, BYTE* pOldScreenBuf)
{
	FCLzw lzw;
	lzw.LZW_Decode(lzwBuf, pOldScreenBuf);
}


CGetScreenInfo::CGetScreenInfo(void)
{
	m_ScreenData.pScreenData = NULL;
	m_ScreenDataLzw.pScreenData = NULL;
	m_ScreenData.dwBufLen = 0;
	m_ScreenDataLzw.dwBufLen = 0;
	m_dwBitCount = 8;
	m_bIsLock = false;
	m_pHash = new WORD [LZW_MAX_HASH_SIZE] ;
	m_hBitmap = NULL;
	m_dwScreenX = 0;
	m_dwScreenY = 0;
}

CGetScreenInfo::~CGetScreenInfo(void)
{
	if(m_hBitmap != NULL)
		DeleteObject(m_hBitmap);
	delete m_pHash;
	if(m_ScreenData.pScreenData != NULL)
		delete m_ScreenData.pScreenData;
	if(m_ScreenDataLzw.pScreenData != NULL)
		delete m_ScreenDataLzw.pScreenData;

}
DWORD CGetScreenInfo::GetBitCount()
{
	return m_dwBitCount;
}
void CGetScreenInfo::SetBitCount(DWORD dwBitCount)
{
	m_dwBitCount = dwBitCount;
}

void CGetScreenInfo::LockData(bool bLock)
{
	m_bIsLock = bLock;
}

void CGetScreenInfo::GetDataSource(BYTE** pBuff, DWORD* pdwLen, BYTE** pBuffLzw, DWORD* pdwLenLzw)
{
	LockData();
	DWORD dwRLzwLen = 0;
	if(pBuff)
		*pBuff = m_ScreenData.pScreenData;
	if(pdwLen)
		*pdwLen = m_ScreenData.dwBufLen;
	if(pBuffLzw)
	{
		if(m_ScreenData.dwBufLen > m_ScreenDataLzw.dwBufLen)
		{
			if(m_ScreenDataLzw.pScreenData != NULL)
				delete m_ScreenDataLzw.pScreenData;
			m_ScreenDataLzw.pScreenData = new BYTE[m_ScreenData.dwBufLen];
			if( !m_ScreenDataLzw.pScreenData )
			{
				m_ScreenDataLzw.dwBufLen = 0;
				return ;
			}
		}
		m_ScreenDataLzw.dwBufLen = m_ScreenData.dwBufLen;
		//ѹ��
		FCLzw lzw;
		memset(m_ScreenDataLzw.pScreenData, 0, m_ScreenData.dwBufLen);
		dwRLzwLen =  lzw.LZW_Encode(m_ScreenData.pScreenData, m_ScreenData.dwBufLen, m_ScreenDataLzw.pScreenData, m_pHash);
		*pBuffLzw = m_ScreenDataLzw.pScreenData;
	}
	if(pdwLenLzw)
			*pdwLenLzw = dwRLzwLen;
}


bool CGetScreenInfo::_BitmapToData(HBITMAP hBitmap, WORD nwBitCount)
{
	if(m_bIsLock)
		return false;
	HDC hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���
	int iBits;
	//λͼ��ÿ������ռ�ֽ���
	WORD wBitCount;
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	//λͼ���Խṹ
	BITMAP Bitmap;
	//λͼ�ļ�ͷ�ṹ
	BITMAPFILEHEADER bmfHdr;
	//λͼ��Ϣͷ�ṹ
	BITMAPINFOHEADER bi;
	//ָ��λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;
	//�����ļ��������ڴ�������ɫ����
	HANDLE  /*hDib,*/ hPal,hOldPal=NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���
//	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	hDC = GetDC(GetDesktopWindow());
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
//	DeleteDC(hDC);
	ReleaseDC(GetDesktopWindow(), hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	wBitCount = nwBitCount;
	if (wBitCount <= 8) 
		dwPaletteSize = (1 <<  wBitCount) *  sizeof(RGBQUAD);

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	//	     dwBmBitsSize=          Bitmap.bmWidthBytes*Bitmap.bmHeight;
	dwBmBitsSize= ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ� 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	if(m_ScreenData.dwBufLen < dwDIBSize)
	{
		if(m_ScreenData.pScreenData != NULL)
			delete m_ScreenData.pScreenData;
		m_ScreenData.pScreenData = new BYTE[dwDIBSize];
		
		if( !m_ScreenData.pScreenData )
		{
			m_ScreenData.dwBufLen = 0;
			return false;
		}
		m_ScreenData.dwBufLen = dwDIBSize ;

	}
	lpbi = (LPBITMAPINFOHEADER)(m_ScreenData.pScreenData + sizeof(BITMAPFILEHEADER));
	*lpbi = bi;
	// �����ɫ��  
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{ 
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	//�ָ���ɫ��
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	// ����λͼ�ļ�ͷ 
	bmfHdr.bfType = 0x4D42; // "BM"
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	memcpy(m_ScreenData.pScreenData, &bmfHdr, sizeof(BITMAPFILEHEADER));


	return true;
}
HBITMAP CGetScreenInfo::__CreateCompatibleBitmap(HDC hdc, DWORD nWidth, DWORD nHeight)
{
	if( m_dwScreenX != nWidth || nHeight != m_dwScreenY)
	{
		if(!m_hBitmap)
			DeleteObject(m_hBitmap);
		m_hBitmap = CreateCompatibleBitmap(hdc, nWidth, nHeight);
		m_dwScreenX = nWidth;
		m_dwScreenY = nHeight;
	}
	return m_hBitmap;
}
HBITMAP CGetScreenInfo::CopyScreenToBitmap(LPRECT lpRect)
{
//	DeleteObject(m_hBitmap);
	HICON hIcon = GetCursor();
	HDC       hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;      
	// λͼ��Ⱥ͸߶�
	int       xScrn, yScrn;         
	// ��Ļ�ֱ���

	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
//	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	hScrDC = GetDC(GetDesktopWindow());
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = __CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap =(struct HBITMAP__ *) SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
		hScrDC, nX, nY, SRCCOPY);
//	POINT ptMouse;
//	GetCursorPos(&ptMouse);
	
//	DrawIcon(hMemDC, ptMouse.x, ptMouse.y, hIcon);
	//�õ���Ļλͼ�ľ��
	hBitmap =(struct HBITMAP__ *) SelectObject(hMemDC, hOldBitmap);
	//��� 
//	DeleteDC(hScrDC);
	ReleaseDC(GetDesktopWindow(), hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���

//	m_hBitmap = hBitmap;
	return hBitmap;
}

bool CGetScreenInfo::CopyScreenToData(LPRECT lpRect)
{
	if(!m_bIsLock)
	{
		HBITMAP hBitmap;
		hBitmap =  CopyScreenToBitmap(lpRect);
		if(hBitmap)
		{
			WORD dwBitCount = (WORD)GetBitCount();
			_BitmapToData(hBitmap, dwBitCount);
		//	DeleteObject(hBitmap);
			return true;
		}
		else
			return false;
	}
	return false;
}



COLORREF CGetColor::GetPixel(int nXPos, int nYPos)
{
	COLORREF cr = {0};
	RECT rcScreen;
	rcScreen.left = nXPos;
	rcScreen.top = nYPos;
	rcScreen.right = nXPos + 1;
	rcScreen.bottom = nYPos + 1;
	if(CopyScreenToData(&rcScreen))
	{
		BYTE* pData;
		DWORD dwLen;
		GetDataSource(&pData, &dwLen);
		DWORD dwBitCount = GetBitCount();
		DWORD dwPaletteSize = 0;
		if (dwBitCount <= 8) 
			dwPaletteSize = (1 <<  dwBitCount) *  sizeof(RGBQUAD);
		BITMAPFILEHEADER bmpFileHred;
		BITMAPINFOHEADER bi;
		memcpy(&bmpFileHred, pData, sizeof(BITMAPFILEHEADER));
		memcpy(&bi, pData + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
		
		if(dwBitCount == 8)
		{
			DWORD DataSizePerLine = ((bi.biWidth * dwBitCount + 31) / 32) * 4;
			BYTE* pColor = pData +  sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER) + dwPaletteSize + DataSizePerLine * ( bi.biHeight - 1) + 0;
			BYTE bColor; memcpy(&bColor, pColor, sizeof(BYTE));
			int nColor = (int)bColor;
			RGBQUAD rgb;
			memcpy(&rgb, pData +  sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*nColor) , sizeof(RGBQUAD));
			cr = RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
		}
		if(dwBitCount == 24)
		{
			DWORD DataSizePerLine = ((bi.biWidth * dwBitCount + 31) / 32) * 4;
			BYTE* pColor = pData +  sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER) + 0 + DataSizePerLine * ( bi.biHeight - 1) + 0;
		//	BYTE bColor; memcpy(&bColor, pColor, sizeof(BYTE));
		//	int nColor = (int)bColor;
			RGBQUAD rgb;
			memcpy(&rgb, pColor , sizeof(RGBQUAD));
			cr = RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
		}
		LockData(false);
	}
	return cr;
}