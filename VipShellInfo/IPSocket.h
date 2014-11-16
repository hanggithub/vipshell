#pragma once

#include <Winsock.h>
#pragma comment(lib, "Ws2_32.lib")


class CRawSocket
{
protected:
	SOCKET m_sock;

	CRawSocket() : m_sock(INVALID_SOCKET) {}
	~CRawSocket() {Close();}

	static void _SetSockAddrIn(ULONG ip, USHORT port, sockaddr_in* pAddr)
	{
		if (pAddr != NULL)
		{
			pAddr->sin_family = AF_INET;
			pAddr->sin_port = htons(port);
			pAddr->sin_addr.s_addr = htonl(ip);
			memset(pAddr->sin_zero, 0, sizeof(pAddr->sin_zero));
		}
	}
	static void _GetSockAddrIn(const sockaddr_in& addr, ULONG* pIp, USHORT* pPort)
	{
		//ASSERT(addr.sin_family == AF_INET);
		if (pPort != NULL)
			*pPort = ntohs(addr.sin_port);
		if (pIp != NULL)
			*pIp = ntohl(addr.sin_addr.s_addr);
	}

	bool _CanRead()
	{
		if (m_sock == INVALID_SOCKET)
			return false;
		fd_set fsR;
		fsR.fd_count = 1;
		fsR.fd_array[0] = m_sock;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		int ir = select(0, &fsR, NULL, NULL, &timeout);
		if (ir != 1)
			return false;
		return true;
	}
	bool _CanWrite()
	{
		if (m_sock == INVALID_SOCKET)
			return false;
		fd_set fsW;
		fsW.fd_count = 1;
		fsW.fd_array[0] = m_sock;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		int ir = select(0, NULL, &fsW, NULL, &timeout);
		if (ir != 1)
			return false;
		return true;
	}

	bool _Attach(SOCKET s)
	{
		if (m_sock != INVALID_SOCKET)
			return false;	//already created
		if (s == INVALID_SOCKET)
			return false;
		m_sock = s;
		return true;
	}

public:
	bool IsOpened() const
	{
		return m_sock != INVALID_SOCKET;
	}
	bool Close()
	{
		if (m_sock == INVALID_SOCKET)
			return false;	//already closed
		int ir = closesocket(m_sock);
		if (ir != 0)
			return false;	//fail to close
		m_sock = INVALID_SOCKET;
		return true;
	}
	bool GetMe(ULONG* pIp, USHORT* pPort) const
	{
		if (m_sock == INVALID_SOCKET)
			return false;
		sockaddr_in addr;
		int cb = sizeof(addr);
		int ir = getsockname(m_sock, (sockaddr*)&addr, &cb);
		if (ir == SOCKET_ERROR)
			return false;
		_GetSockAddrIn(addr, pIp, pPort);
		return true;
	}
	bool GetPeer(ULONG* pIp, USHORT* pPort) const
	{
		if (m_sock == INVALID_SOCKET)
			return false;
		sockaddr_in addr;
		int cb = sizeof(addr);
		int ir = getpeername(m_sock, (sockaddr*)&addr, &cb);
		if (ir == SOCKET_ERROR)
			return false;
		_GetSockAddrIn(addr, pIp, pPort);
		return true;
	}
};



class CUdpSocket : public CRawSocket
{
public:
	bool Create(ULONG ip = 0, USHORT port = 0)
	{
		if (m_sock != INVALID_SOCKET)
			return false;	//already created
		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s == INVALID_SOCKET)
			return false;	//fail to create
		if (ip != 0 || port != 0)
		{
			sockaddr_in addr;
			_SetSockAddrIn(ip, port, &addr);
			int ir = bind(s, (const sockaddr*)&addr, sizeof(addr));
			if (ir != 0)
			{//error occur
				closesocket(s);
				return false;
			}
		}
		return _Attach(s);
	}

	int SendTo(const void* buf, int len, ULONG addr, USHORT port, int flags = 0, bool block = true)
	{
		if (m_sock == INVALID_SOCKET)
			return SOCKET_ERROR;
		if (!block)
		{
			if (!_CanWrite())
				return 0;
		}
		sockaddr_in to;
		_SetSockAddrIn(addr, port, &to);
		return sendto(m_sock, (const char*)buf, len, flags, (const sockaddr*)&to, sizeof(to));
	}
	int RecvFrom(void* buf, int len, ULONG* pAddr = NULL, USHORT* pPort = NULL, int flags = 0, bool block = true)
	{
		if (m_sock == INVALID_SOCKET)
			return SOCKET_ERROR;
		if (!block)
		{
			if (!_CanRead())
				return 0;
		}
		sockaddr_in from;
		int fromlen = sizeof(from);
		int ir = recvfrom(m_sock, (char*)buf, len, flags, (sockaddr*)&from, &fromlen);
		if (ir != 0 && ir != SOCKET_ERROR)
		{
			//ASSERT(fromlen >= sizeof(from) - sizeof(from.sin_zero));
			_GetSockAddrIn(from, pAddr, pPort);
		}
		return ir;
	}
};



class CTcpRawConnect : public CRawSocket
{
public:
	int Send(const void* buf, int len, int flags = 0, bool block = true)
	{
		if (m_sock == INVALID_SOCKET)
			return SOCKET_ERROR;
		if (!block)
		{
			if (!_CanWrite())
				return 0;
		}
		return send(m_sock, (const char*)buf, len, flags);
	}
	int Recv(void* buf, int len, int flags = 0, bool block = true)
	{
		if (m_sock == INVALID_SOCKET)
			return SOCKET_ERROR;
		if (!block)
		{
			if (!_CanRead())
				return 0;
		}
		return recv(m_sock, (char*)buf, len, flags);
	}
};

class CTcpClient : public CTcpRawConnect
{
public:
	bool Create(ULONG ipTo, USHORT portTo)
	{
		if (m_sock != INVALID_SOCKET)
			return false;	//already created
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
			return false;	//fail to create
		sockaddr_in addr;
		_SetSockAddrIn(ipTo, portTo, &addr);
		int ir = connect(s, (const sockaddr*)&addr, sizeof(addr));
		if (ir != 0)
		{//error occur
			closesocket(s);
			return false;
		}
		return _Attach(s);
	}
};

class CTcpServer : public CRawSocket
{
public:
	bool Create(ULONG ipListen, USHORT portListen, int backlog = SOMAXCONN)
	{
		if (m_sock != INVALID_SOCKET)
			return false;	//already created
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
			return false;	//fail to create
		sockaddr_in addr;
		_SetSockAddrIn(ipListen, portListen, &addr);
		int ir = bind(s, (sockaddr*)&addr, sizeof(addr));
		if (ir != 0)
		{
			closesocket(s);
			return false;
		}
		ir = listen(s, backlog);
		if (ir != 0)
		{
			closesocket(s);
			return false;
		}
		return _Attach(s);
	}

private:
	friend class CTcpConnect;

	SOCKET _RawAccept(bool block)
	{
		if (m_sock == INVALID_SOCKET)
			return INVALID_SOCKET;
		if (!block)
		{
			if (!_CanRead())
				return INVALID_SOCKET;
		}
		sockaddr_in conn;
		int connlen = sizeof(conn);
		return accept(m_sock, (sockaddr*)&conn, &connlen);
	}
};

class CTcpConnect : public CTcpRawConnect
{
public:
	bool Accept(CTcpServer* pTcpServer, bool block = true)
	{
		if (m_sock != INVALID_SOCKET)
			return false;
		m_sock = pTcpServer->_RawAccept(block);
		if (m_sock == INVALID_SOCKET)
			return false;
		return true;
	}
};

