#pragma once

#include <Winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#include <IPHlpApi.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Netapi32.lib")

#include <Nb30.h>

#include <string>
#include <sstream>
//#include <atlbase.h>



#define _IPHELPER_BEGIN		namespace iphelper {
#define _IPHELPER_END		}
#define _USING_IPHELPER		using namespace iphelper;




_IPHELPER_BEGIN


//type
struct MAC
	{ UCHAR v[6]; };
const MAC MAC_NONE = {0, 0, 0, 0, 0, 0};




namespace basic
{
	inline 
	ULONG Addr2IP(LPCWSTR addr)
	{
		ULONG ip = inet_addr(ATL::CW2A(addr));
		return ntohl(ip);
	}
	inline 
	std::wstring IP2Addr(ULONG ip)
	{
		in_addr inaddr;
		inaddr.s_addr = htonl(ip);
		return std::wstring(ATL::CA2W(inet_ntoa(inaddr)));
	}


	inline 
	MAC Str2Mac(LPCWSTR sz)
	{
		UINT fields[6];
		if (swscanf(sz, L"%x-%x-%x-%x-%x-%x", &fields[0], &fields[1], &fields[2], &fields[3], &fields[4], &fields[5]) != 6)
			return MAC_NONE;
		MAC mac;
		for (int i = 0; i < 6; ++i)
		{
			if (fields[i] > 0x00FF)
				return MAC_NONE;
			mac.v[i] = (UCHAR)fields[i];
		}
		return mac;
	}
	inline 
	std::wstring Mac2Str(const MAC& mac)
	{
		WCHAR buff[18];
		swprintf(buff, L"%02X-%02X-%02X-%02X-%02X-%02X", (UINT)mac.v[0], (UINT)mac.v[1], (UINT)mac.v[2], (UINT)mac.v[3], (UINT)mac.v[4], (UINT)mac.v[5]);
		return buff;
	}

} //namespace basic



namespace dns
{
	inline 
	ULONG Name2IP(LPCWSTR name)
	{
		hostent* pHE = gethostbyname(ATL::CW2A(name));
		if (pHE == NULL)
			return INADDR_NONE;
		if (pHE->h_addrtype != AF_INET)
			return INADDR_NONE;
		if (pHE->h_length < sizeof(in_addr))
			return INADDR_NONE;
		in_addr* pInAddr = (in_addr*)pHE->h_addr;
		if (pInAddr == NULL)
			return INADDR_NONE;
		return ntohl(pInAddr->s_addr);
	}

	inline 
	std::wstring IP2Name(ULONG ip)
	{
		ULONG ipN = htonl(ip);
		hostent* pHE = gethostbyaddr((const char*)&ipN, sizeof(ipN), AF_INET);
		if (pHE == NULL)
			return L"";
		if (pHE->h_addrtype != AF_INET)
			return L"";
		return std::wstring(ATL::CA2W(pHE->h_name));
	}

} //namespace dns




//支持Addr和Name两种格式
namespace advance
{
	inline 
	ULONG RetrieveIP(LPCWSTR AddrOrName)
	{
		ULONG ip = iphelper::basic::Addr2IP(AddrOrName);
		if (ip == INADDR_NONE)
			ip = iphelper::dns::Name2IP(AddrOrName);
		return ip;
	}

} //namespace adv




namespace adapter
{
	namespace
	{
		inline 
		bool _IsLoopbackIp(ULONG ip)
		{
			return (ip & 0xFF000000) == 0x7F000000;
		}

		inline 
		bool _IsValidIpAddrRow(const MIB_IPADDRROW& row, bool fIncludeLoopback)
		{
			if (row.dwMask == 0
					|| (row.dwAddr & 0xFF000000) == 0x00000000 || (row.dwAddr  & 0xFF000000) == 0xFF000000)
				return false;	//? Windows server 2003 中会有此情况
			if (!fIncludeLoopback)
			{
				if (_IsLoopbackIp(row.dwAddr))
					return false;
			}
			return true;
		}

		inline 
		void _TidyIpAddrTable(MIB_IPADDRTABLE* pIpAddrTable, bool fIncludeLoopback)
		{
			DWORD dwNumEntries = pIpAddrTable->dwNumEntries;
			pIpAddrTable->dwNumEntries = 0;

			for (DWORD i = 0; i < dwNumEntries; ++i)
			{
				pIpAddrTable->table[i].dwAddr = ntohl(pIpAddrTable->table[i].dwAddr);
				pIpAddrTable->table[i].dwMask = ntohl(pIpAddrTable->table[i].dwMask);

				if (!_IsValidIpAddrRow(pIpAddrTable->table[i], fIncludeLoopback))
					continue;

				if (pIpAddrTable->dwNumEntries < i)
					pIpAddrTable->table[pIpAddrTable->dwNumEntries] = pIpAddrTable->table[i];

				++ pIpAddrTable->dwNumEntries;
			}
		}

		inline 
		MIB_IPADDRTABLE* _GetIpAddrTable(bool fIncludeLoopback, bool fSort)
		{
			ULONG cbIpAddrTable = 0;
			if (::GetIpAddrTable(NULL, &cbIpAddrTable, false) != ERROR_INSUFFICIENT_BUFFER)
				return NULL;
			if (cbIpAddrTable == 0)
				return NULL;
			BYTE* pRawMem = new BYTE[cbIpAddrTable];
			MIB_IPADDRTABLE* pIpAddrTable = (MIB_IPADDRTABLE*)pRawMem;

			if (::GetIpAddrTable(pIpAddrTable, &cbIpAddrTable, fSort) != NO_ERROR)
			{
				delete []pRawMem;
				return NULL;
			}

			_TidyIpAddrTable(pIpAddrTable, fIncludeLoopback);

			if (pIpAddrTable->dwNumEntries == 0)
			{
				delete []pRawMem;
				return NULL;
			}

			return pIpAddrTable;
		}

		inline 
		void _FreeIpAddrTable(MIB_IPADDRTABLE* pIpAddrTable)
		{
			if (pIpAddrTable != NULL)
			{
				BYTE* pRawMem = (BYTE*)pIpAddrTable;
				delete []pRawMem;
			}
		}

	} //namespace


	inline 
	ULONG GetSelfAddressCount(bool fIncludeLoopback = false)
	{
		//DWORD InterfaceCount = 0;
		//if (::GetNumberOfInterfaces(&InterfaceCount) != NO_ERROR)
		//	return 0;
		//
		//return InterfaceCount;


		//使用GetNumberOfInterfaces()不够准确，因为有部分Ip需要剔除，改成GetIpAddrTable()
		MIB_IPADDRTABLE* pIpAddrTable = _GetIpAddrTable(fIncludeLoopback, false);
		if (pIpAddrTable == NULL)
			return 0;

		ULONG AddressCountRet = pIpAddrTable->dwNumEntries;

		_FreeIpAddrTable(pIpAddrTable);

		return AddressCountRet;
	}

	inline 
	ULONG GetSelfAddresses(ULONG* pAddresses, ULONG AddressCount, bool fIncludeLoopback = false, bool fSort = false)
	{
		MIB_IPADDRTABLE* pIpAddrTable = _GetIpAddrTable(fIncludeLoopback, fSort);
		if (pIpAddrTable == NULL)
			return 0;

		ULONG AddressCountRet = 0;

		for (DWORD i = 0; i < pIpAddrTable->dwNumEntries; ++i)
		{
			if (AddressCountRet >= AddressCount)
				break;

			pAddresses[AddressCountRet] = pIpAddrTable->table[i].dwAddr;
			++ AddressCountRet;
		}

		_FreeIpAddrTable(pIpAddrTable);

		return AddressCountRet;
	}


	inline 
	bool IsLoopbackIp(ULONG ip)
	{
		return _IsLoopbackIp(ip);
	}

	inline 
	bool IsSelfAddress(ULONG ip)
	{
		if (ip == INADDR_NONE)
			return false;
		if (_IsLoopbackIp(ip))	//127.0.0.1
			return true;

		ULONG nAddrCount = GetSelfAddressCount();
		if (nAddrCount == 0)
			return false;


		MIB_IPADDRTABLE* pIpAddrTable = _GetIpAddrTable(false, false);
		if (pIpAddrTable == NULL)
			return false;

		bool fSelf = false;

		for (ULONG i = 0; i < nAddrCount; ++i)
		{
			if (pIpAddrTable->table[i].dwAddr == ip)
			{
				fSelf = true;
				break;
			}
		}

		_FreeIpAddrTable(pIpAddrTable);

		return fSelf;
	}

	inline 
	ULONG GetBestInterface(ULONG ipTo)
	{
		if (ipTo == INADDR_NONE)
			return INADDR_NONE;
		if (_IsLoopbackIp(ipTo))
			return ipTo;

		MIB_IPADDRTABLE* pIpAddrTable = _GetIpAddrTable(false, false);
		if (pIpAddrTable == NULL)
			return INADDR_NONE;


		ULONG ipBest = INADDR_NONE;

		//check self
		for (DWORD i = 0; i < pIpAddrTable->dwNumEntries; ++i)
		{
			if (pIpAddrTable->table[i].dwAddr == ipTo)
			{
				ipBest = ipTo;	//self
				goto over_check;
			}
		}

		//check mask
		for (DWORD i = 0; i < pIpAddrTable->dwNumEntries; ++i)
		{
			if (pIpAddrTable->table[i].dwAddr == 0 || pIpAddrTable->table[i].dwMask == 0)
				continue;	//? Windows server 2003 中会有此情况
			if ((pIpAddrTable->table[i].dwAddr & pIpAddrTable->table[i].dwMask)
					== (ipTo & pIpAddrTable->table[i].dwMask))
			{
				ipBest = pIpAddrTable->table[i].dwAddr;
				goto over_check;
			}
		}

		//best interface
		DWORD BestInterfaceIndex = 0;
		if (::GetBestInterface(ipTo, &BestInterfaceIndex) == NO_ERROR)
		{
			for (DWORD i = 0; i < pIpAddrTable->dwNumEntries; ++i)
			{
				if (pIpAddrTable->table[i].dwIndex == BestInterfaceIndex)
				{
					ipBest = pIpAddrTable->table[i].dwAddr;
					goto over_check;
				}
			}
		}

	over_check:
		_FreeIpAddrTable(pIpAddrTable);

		return ipBest;
	}



	inline 
	ULONG GetLanAdapterCount()
	{
		LANA_ENUM lana_enum;

		NCB ncbEnum = {0};
		ncbEnum.ncb_command = NCBENUM;
		ncbEnum.ncb_buffer = (PUCHAR)&lana_enum;
		ncbEnum.ncb_length = sizeof(lana_enum);
		if (::Netbios(&ncbEnum) != NRC_GOODRET)
			return 0;

		return lana_enum.length;
	}


	inline 
	ULONG GetLanAdapterMACs(MAC* pMacAddresses, ULONG MacAddressCount)
	{
#pragma pack(push, 1)
		struct _ASTAT
		{
			ADAPTER_STATUS status;
			NAME_BUFFER name[30];
		};
#pragma pack(pop)


		LANA_ENUM lana_enum;
		{{
			NCB ncbEnum = {0};
			ncbEnum.ncb_command = NCBENUM;
			ncbEnum.ncb_buffer = (PUCHAR)&lana_enum;
			ncbEnum.ncb_length = sizeof(lana_enum);
			if (::Netbios(&ncbEnum) != NRC_GOODRET)
				return 0;
		}}


		ULONG MacAddressCountRet = 0;

		for (UCHAR i = 0; i < lana_enum.length; ++i)
		{
			if (MacAddressCountRet >= MacAddressCount)
				break;

			{{
				NCB ncbReset = {0};
				ncbReset.ncb_command = NCBRESET;
				ncbReset.ncb_lana_num = lana_enum.lana[i];    
				if (::Netbios(&ncbReset) != NRC_GOODRET)
					break;
			}}

			_ASTAT astat;
			{{
				NCB ncbAStat = {0};
				ncbAStat.ncb_command = NCBASTAT;
				ncbAStat.ncb_lana_num = lana_enum.lana[i];
				strcpy((char*)ncbAStat.ncb_callname, "*");
				ncbAStat.ncb_buffer = (PUCHAR)&astat;
				ncbAStat.ncb_length = sizeof(astat);
				if (::Netbios( &ncbAStat) != NRC_GOODRET)
					break;
			}}

			memcpy(pMacAddresses[MacAddressCountRet].v, astat.status.adapter_address, 6);
			++ MacAddressCountRet;
		}

		return MacAddressCountRet;
	}


} //namespace adapter




_IPHELPER_END
