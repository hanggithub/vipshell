#pragma once
#include "connectbase.h"

class CServerScreenInfoHandle :
	public CConnectBase
{
public:
	CServerScreenInfoHandle(void);
	~CServerScreenInfoHandle(void);
public:
	DWORD GetConnectId();
	void SetConnectId(DWORD dwId);
public:
	HBITMAP GetScreenInfo(DWORD dwBitCount);

protected:
	DWORD m_dwConnectId;
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
private:
	bool __NewBufLzw(DWORD dwLen);
	bool __NewBufBitmap(DWORD dwLen);
	HBITMAP __GetDataToBitmap(BYTE* pDate);
};
