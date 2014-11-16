#pragma once
#include "..\VipShellInfo\vipshellcommand.h"
#include "..\VipShellInfo\IPSocket.h"

class CShellTcpConnect : public CTcpConnect
{
public:
	int Send(const void* buf, int len, int flags = 0, bool block = true)
	{
		return CTcpConnect::Send(buf, len, flags, block);
	}
	int Recv(void* buf, int len, int flags = 0, bool block = true)
	{
		return CTcpConnect::Recv(buf, len, flags, block);
	}
	bool Accept(CTcpServer* pTcpServer, bool block = true)
	{
		return CTcpConnect::Accept(pTcpServer, block);
	}
//	bool IsRecvTimeOut(DWORD dwTime = 1000);
	bool IsRecvTimeOut(DWORD dwTime = 1000)
	{
		DWORD dwS = GetTickCount();
		while(!_CanRead())
		{ 
			Sleep(1);
			if( GetTickCount() - dwS >= dwTime )
			{
				Close();
				return false;
			}
		}
		return true;
	}
	SOCKET Detach()
	{
		SOCKET s = m_sock;
		m_sock = INVALID_SOCKET;
		return s;
	}
	SOCKET Attach(SOCKET s)
	{
		SOCKET sRet = Detach();
		m_sock = s;
		return sRet;
	}
	SOCKET GetSocket()
	{
		return m_sock;
	}
};

class CConnectBase
{
public:
	CConnectBase(void);
	~CConnectBase(void);
protected:
	CShellTcpConnect m_hServer;
public:
	ULONG GetPeer(LPWSTR szIp)
	{		
		ULONG Ip;
		USHORT sPort;
		m_hServer.GetPeer(&Ip, (USHORT *)&sPort);
		struct in_addr sa;
		sa.S_un.S_addr = ntohl(Ip);
		lstrcpyW(szIp, CA2W(inet_ntoa(sa)));
		return sPort;
	}
	ULONG GetPeer(DWORD& dwIp)
	{
		ULONG Ip;
		USHORT sPort;
		m_hServer.GetPeer(&Ip, (USHORT *)&sPort);
		dwIp = Ip;
		return sPort;
	}
	SOCKET Detach()
	{
		return m_hServer.Detach();
	}
	SOCKET Attach(SOCKET s)
	{
		return m_hServer.Attach(s);
	}
	SOCKET GetSocket()
	{
		return m_hServer.GetSocket();
	}
	bool Close()
	{
		if(  !m_hServer.Close() )
			m_hServer.Detach();
		return true;
	}
protected:
	int _RecvBuffData(BYTE* pData, DWORD dwlen)
	{
		DWORD dwSendBuflen = 16 * 1024;
		DWORD dwSendLen = 0;
	//	DWORD dwSendTime = dwlen / dwSendBuflen;
		DWORD dwSended;
		DWORD dwRbuf = dwlen;
		while(dwSendLen < dwlen)
		{
			dwRbuf = (dwlen - dwSendLen);
			if( dwRbuf >=  dwSendBuflen)
			{
				dwSended = m_hServer.Recv(pData + dwSendLen, dwSendBuflen);
				if( dwSended == SOCKET_ERROR)
		    		return dwSended;
			}
			else
			{
				dwSended = m_hServer.Recv(pData + dwSendLen, dwRbuf);
				if( dwSended == SOCKET_ERROR)
		    		return dwSended;
			}
			dwSendLen += dwSended;
		}
		return dwlen;
	}
	int _SendBuffData(BYTE* pData, DWORD dwlen)
	{
		DWORD dwSendBuflen = 16 * 1024;
		DWORD dwSendLen = 0;
	//	DWORD dwSendTime = dwlen / dwSendBuflen;
		DWORD dwSended;
		DWORD dwRbuf = dwlen;
		while(dwSendLen < dwlen)
		{
			
			dwRbuf = (dwlen - dwSendLen);
			if( dwRbuf >=  dwSendBuflen)
			{
				dwSended = m_hServer.Send(pData + dwSendLen, dwSendBuflen);
				if( dwSended == SOCKET_ERROR)
		    		return dwSended;
			}
			else
			{
				dwSended = m_hServer.Send(pData + dwSendLen, dwRbuf);
				if( dwSended == SOCKET_ERROR)
		    		return dwSended;
			}
			dwSendLen += dwSended;
		}
		return dwlen;
	}
};
