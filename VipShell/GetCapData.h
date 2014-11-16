#pragma once
#include "..\vipshellinfo\threadbase.h"
#include "Vfw.h"
class CGetCapData :
	public CThreadBase
{
	WORD* m_pHash;
	HWND m_hwndMain;
	HWND m_hwndVideo;
	bool b_IsOpen;
	BYTE* m_pDataLzw;
	DWORD m_dwLzwBuflen;
public:
	CGetCapData(void);
	~CGetCapData(void);
public:
	void __NewLzwBuff(DWORD dwLen);
	static BYTE*		__NewBuff(DWORD dwLen);
	static LRESULT CALLBACK FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr);
	static LRESULT PASCAL WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
public:
	void Start(LPCTSTR szText, RECT* rc);
	bool GetCapDataBuff(BYTE** pBuff = NULL, DWORD* dwLen = NULL, BYTE** pBufflzw = NULL, DWORD* dwlzw = NULL );
	bool IsOpen();
	bool Stop();
};

