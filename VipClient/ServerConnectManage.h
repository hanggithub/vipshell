#pragma once
#include "..\vipshellinfo\threadbase.h"	
#include ".\serverconnecthandle.h"
#include ".\vipshellclientserver.h"

class CServerConnectManage :
	public CThreadBase
{
public:
	CServerConnectManage(void);
	~CServerConnectManage(void);
protected:
	CVipShellClientServer* m_hServer; 
public:
	void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
	void SetBindServer(CVipShellClientServer* server);
};
