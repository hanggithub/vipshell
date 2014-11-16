#include "stdafx.h"
#include "atlbase.h"
#include ".\servershellhandle.h"

CServerShellHandle::CServerShellHandle(void)
{
	
}

CServerShellHandle::~CServerShellHandle(void)
{
}
void CServerShellHandle::_AddBuf(char* buf)
{
	CMySingleLock Event(&m_CriticalSectionBuf);
	m_RecvBuf += buf;
}

DWORD CServerShellHandle::ReadBuf(WCHAR* szBuf)
{
	CMySingleLock Event(&m_CriticalSectionBuf);
	CA2W wRet(m_RecvBuf.c_str());
	m_RecvBuf.clear();
	lstrcpyW(szBuf, wRet);
	return lstrlenW(wRet);
}

void CServerShellHandle::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{	
	char szBuf[8000];
	int nRet;
	while(true)
	{
		if( IsStop(dwThreadId, 100) )
			return ;
		if( !m_hServer.IsOpened() )
			continue;
		memset(szBuf, 0, sizeof(szBuf));
		nRet = m_hServer.Recv(szBuf, sizeof(szBuf));
		if( nRet == SOCKET_ERROR)
			return ;
		_AddBuf(szBuf);
	}
}
bool CServerShellHandle::SendCmd(LPCWSTR szBuf)
{
	std::string strCmd = CW2A(szBuf);
	const char* pBuf = strCmd.c_str();
	int nRet = m_hServer.Send(pBuf, strCmd.length());
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}


bool CServerShellHandle::_GetShell()
{
	tagServerSystemInfo Info;
	tagServerSystemInfo* pInfo = &Info;
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETSHELL;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
/*
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	memcpy(pInfo, cmd.pBuff, sizeof(tagServerSystemInfo));
//	SetServerInfoAtThis(pInfo);
*/
	return true;
}

bool CServerShellHandle::Begin()
{
	if( !_GetShell() )
		return false;
	NewThread(NULL, NULL, NULL, 0);    //循环读取数据
	return true;
}

