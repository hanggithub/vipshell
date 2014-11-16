#pragma once
#include "connectbase.h"
#include "..\vipshellinfo\threadbase.h"	

class CCapPictureCallBack
{
public:
	virtual void OnPicture(HBITMAP hBitmap)  = 0 ;
};

class CServerGetCapPicture :
	public CConnectBase, public CThreadBase
{
public:
	CServerGetCapPicture(void);
	~CServerGetCapPicture(void);
public:
	virtual void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
	bool Start(CCapPictureCallBack* e);
	bool Stop();
private:
	struct tagLZW_STRING
	{
		WORD	wPrefix ;	// Îª Old
		WORD	wSuffix ;	// Îª GetFirstChar (Old) »ò GetFirstChar (Code)
	};
	tagLZW_STRING m_lzw_STR[65535 + 32];
	DWORD m_dwBufLenLzw;
	BYTE* m_pDataLzw;

	DWORD m_dwBufBitmaplen;
	BYTE* m_pDataBitmap;

	HBITMAP m_hBitmap;
	bool m_bIsStop;
	CMyCriticalSection m_CriticalSectionStop;
private:
	CCapPictureCallBack* m_pCall;
	bool __NewBufLzw(DWORD dwLen);
	bool __NewBufBitmap(DWORD dwLen);
	HBITMAP __GetDataToBitmap(BYTE* pDate);
	HBITMAP ___GetPicture();
};


class CServerGetCapPictureHandle : public CServerGetCapPicture
{

};