#pragma once
#include <windows.h>
//#include "ThreadBase.h"
#pragma comment (lib, "Gdi32.lib")
class CGetScreenInfo
{
public:
	CGetScreenInfo(void);
	~CGetScreenInfo(void);
public:
	void LockData(bool bLock = true);
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	void SetBitCount(DWORD dwBitCount);
	DWORD GetBitCount();
	bool CopyScreenToData(LPRECT lpRect);
	void GetDataSource(BYTE** pBuff = NULL, DWORD* pdwLen = NULL, BYTE** pBuffLzw = NULL , DWORD* pdwLenLzw = NULL);
protected:
	bool _BitmapToData(HBITMAP hBitmap, WORD nwBitCount);
protected:
	HBITMAP __CreateCompatibleBitmap(HDC hdc, DWORD nWidth, DWORD nHeight);
	DWORD m_dwScreenX;
	DWORD m_dwScreenY;
	HBITMAP m_hBitmap;
	WORD* m_pHash;
	DWORD m_dwGetScreenDataThread;
	struct tagScreenData
	{
		BYTE* pScreenData; 
		DWORD dwBufLen;
	};
	bool m_bIsLock;
	DWORD m_dwBitCount;
	tagScreenData m_ScreenData;
	tagScreenData m_ScreenDataLzw;
};

class CGetColor : public  CGetScreenInfo
{
public:
	COLORREF GetPixel(int nXPos, int nYPos);
};
void LZW_Decode(BYTE* lzwBuf, BYTE* pOldScreenBuf);
