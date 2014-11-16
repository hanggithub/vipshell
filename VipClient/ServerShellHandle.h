#pragma once
#include "connectbase.h"
#include "..\vipshellinfo\threadbase.h"	

class CServerShellHandle :
	public CConnectBase ,public CThreadBase
{
public:
	CServerShellHandle(void);
	~CServerShellHandle(void);
public :
	virtual void CServerShellHandle::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
protected:
	void _AddBuf(char* buf);
	bool _GetShell();
public:
	bool Begin();
	DWORD ReadBuf(WCHAR* szBuf);
	bool SendCmd(LPCWSTR szBuf);
protected:
	CMyCriticalSection m_CriticalSectionBuf;
	std::string m_RecvBuf;	
};
