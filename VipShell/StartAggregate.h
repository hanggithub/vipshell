#pragma once
#include <windows.h>
#include <string>
#include <atlbase.h>
class CStartAggregate
{
public:
	CStartAggregate(void);
	~CStartAggregate(void);
public:
	bool ReadRegisterKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpKey, LPCWSTR lpRet, DWORD* dwRet);
	HKEY OpenRegisterKey(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired);
	bool WriteRegisterKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpKey, LPCWSTR lpRet, DWORD dwRet);
public:
	bool SetCurrentVersionRun(LPCWSTR szKey, LPCWSTR szVel);
	bool GetCurrentVersionRun(LPCWSTR szKey, LPWSTR szVel, DWORD dwLen = MAX_PATH);

	bool SetCurrentWindowsLoad(LPCWSTR szVel);
	bool GetCurrentWindowsLoad(LPWSTR szVel, DWORD dwLen = MAX_PATH);

	bool SetCurrentVersionWinlogonShell(LPCWSTR szVel, bool bOverlay = false);
	bool GetCurrentVersionWinlogonShell(LPWSTR szVel, DWORD dwLen = MAX_PATH);
	
//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon
	bool SetCurrentVersionWinlogonUserinit(LPCWSTR szVel, bool bOverlay = false);
	bool GetCurrentVersionWinlogonUserinit(LPWSTR szVel, DWORD dwLen = MAX_PATH);

	SC_HANDLE _OpenSCManager();
	bool	_StopServer(SC_HANDLE hServer);
	bool	_StartServer(LPCWSTR szServiceName);
	bool CreateNtService(LPCWSTR szServiceName, LPCWSTR szPath);
public:
	std::wstring m_strWindowsPath;
	std::wstring m_strSystemPath;
	std::wstring m_strIePath;
	std::wstring m_strTempPath;
	std::wstring m_strExePath;
};
