// NL_QQwry.cpp: implementation of the NL_QQwry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NL_QQwry.h"
#include <atlbase.h>
#include <string>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NL_QQwry::NL_QQwry()
{
	m_bOpen=OpenQQwry("QQwry.dat");
	GetBE();
}

NL_QQwry::~NL_QQwry()
{
	CloseQQwry();
}

bool NL_QQwry::OpenQQwry(CString szFileName)
{
	if(!m_file.Open(szFileName,CFile::modeRead|CFile::typeBinary))
		return false;
	else
		return true;
}

void NL_QQwry::CloseQQwry()
{
	if(m_bOpen)m_file.Close();
}

bool NL_QQwry::GetBE()
{
	if(!m_bOpen)return false;
	m_file.Seek(0,CFile::begin);
	if(m_file.Read(&m_be,sizeof(BE))>0)
		return true;
	else
		return false;
}


int NL_QQwry::GetStartIPInfo(int iIndex)
{
	BYTE buf[MAXBUF];
	int ioff;
	if(!m_bOpen) return 0;
	ioff=m_be.uBOff+iIndex*7;
	if(ioff>m_be.uEOff) return 0;
	m_file.Seek(m_be.uBOff+iIndex*7,CFile::begin);
	m_file.Read(&m_ipoff,sizeof(IPOFF));
	ioff=(m_ipoff.off1+m_ipoff.off2*256+m_ipoff.off3*256*256);
	m_file.Seek(ioff,CFile::begin);
	m_file.Read(&m_ei,sizeof(EndInfo));

	if(m_ei.buf[0]!=1 &&  m_ei.buf[0]!=2)
	{
		m_ei.bMode=1;//没有跳
		for(int i=0;i<MAXBUF;i++)
		{
			if(m_ei.buf[i]==0)
			{
				if(m_ei.buf[i+1]==2)
				{
					m_ei.bMode=2;//Local 跳
					m_ei.offset1=m_ei.buf[i+2]+
						m_ei.buf[i+3]*256+
						m_ei.buf[i+4]*256*256;
				}
				break;
			}
		}
	}
	else if(m_ei.buf[0]==2)
	{
		m_ei.bMode=3;//Country 跳 local不跳
		m_ei.offset1=m_ei.buf[1]+m_ei.buf[2]*256+m_ei.buf[3]*256*256;
		if(m_ei.buf[4]!=2)
		{
			m_ei.bMode=3;
		}
		else
		{
			m_ei.bMode=4;//Country跳 local跳
			m_ei.offset2=m_ei.buf[5]+m_ei.buf[6]*256+m_ei.buf[7]*256*256;
		}
	}
	else if(m_ei.buf[0]==1)
	{
		m_ei.offset1=m_ei.buf[1]+m_ei.buf[2]*256+m_ei.buf[3]*256*256;
		m_file.Seek(m_ei.offset1,CFile::begin);
		m_file.Read(buf,MAXBUF);
		memcpy(m_ei.buf,buf,MAXBUF);
		m_ei.bMode=0;
		if(m_ei.buf[0]!=2)
		{
			for(int i=0;i<MAXBUF;i++)
			{
				if(m_ei.buf[i]==0)
				{
					if(m_ei.buf[i+1]!=2)
					{
						m_ei.bMode=5;//1 没有跳
					}
					else
					{
						m_ei.bMode=6;//1 Country不跳 Local 跳
						m_ei.offset2=m_ei.buf[i+2]+
							m_ei.buf[i+3]*256+
							m_ei.buf[i+4]*256*256;
					}
					break;
				}
			}
		}
		else
		{
			
			if(m_ei.buf[4]!=2)
			{
				m_ei.bMode=7;// 1 Country跳 Local不跳
				m_ei.offset2=m_ei.buf[1]+
							m_ei.buf[2]*256+
							m_ei.buf[3]*256*256;

			}
			else
			{
				m_ei.bMode=8;// 1 Country跳 Local跳
				m_ei.offset1=m_ei.buf[1]+
					m_ei.buf[2]*256+
					m_ei.buf[3]*256*256;
				m_ei.offset2=m_ei.buf[5]+
					m_ei.buf[6]*256+
					m_ei.buf[7]*256*256;
			}
		}
	}
	return ioff;
}


int NL_QQwry::GetRecordCount(void)
{
	if(!m_bOpen) return 0;
	if((m_be.uEOff-m_be.uBOff)<0) return 0;
	return (m_be.uEOff-m_be.uBOff)/7+1;
}


CString NL_QQwry::GetStr(int ioffset)
{
	if(ioffset>m_be.uEOff) return _T("");
	BYTE ch;
	std::string buf;
//	CString buf=_T("");
	m_file.Seek(ioffset,CFile::begin);
	int i=0;
	while(1)
	{
		m_file.Read(&ch,1);
		if(ch==0)
			break;
		buf+=ch;
		i++;
		if(i>50)break;
	}
	CString bufRet = ATL::CA2T(buf.c_str());
	return bufRet;
}

CString NL_QQwry::GetCountryLocal(int index)
{
	if(index<0 || index>GetRecordCount()-1)
		return _T("");
	return GetCountryLocal(m_ei.bMode,GetStartIPInfo(index)+4);
}

CString NL_QQwry::GetCountryLocal(BYTE bMode,int ioffset)
{
	CString buf=_T("");
	if(bMode==1)//X 没有跳
	{
		buf=GetStr(ioffset);
		buf+=_T(" ");
		buf+=GetStr();
	}
	if(bMode==2)//X Country不跳 Local 跳
	{
		buf=GetStr(ioffset);
		buf+=_T(" ");
		buf+=GetStr(m_ei.offset1);
	}
	if(bMode==3)//2 Country跳 local不跳
	{
		buf=GetStr(m_ei.offset1);
		buf+=_T(" ");
		buf+=GetStr(ioffset+4);
	}

	if(bMode==4)//2 Country跳 local跳
	{
		buf=GetStr(m_ei.offset1);
		buf+=_T(" ");
		buf+=GetStr(m_ei.offset2);
	}

	if(bMode==5)//1 没有跳
	{
		buf=GetStr(m_ei.offset1);
		buf+=_T(" ");
		buf+=GetStr();
	}

	if(bMode==6)//1 Country不跳 Local 跳
	{
		buf=GetStr(m_ei.offset1);
		buf+=_T(" ");
		buf+=GetStr(m_ei.offset2);
	}
	if(bMode==7)//1 Country跳 Local 不跳
	{
		buf=GetStr(m_ei.offset2);
		buf+=_T(" ");
		buf+=GetStr(m_ei.offset1+4);
	}
	if(bMode==8)//1 Country跳 Local跳
	{
		buf=GetStr(m_ei.offset1);
		buf+=_T(" ");
		buf+=GetStr(m_ei.offset2);
	}
	return buf;
}

CString NL_QQwry::GetStr()
{
	BYTE ch;
	std::string buf ;
	int i=0;
	while(1)
	{ 
		m_file.Read(&ch,1);
		if(ch==0)
			break;
		buf+=ch;
		i++;
		if(i>50)break;
	}
	CString bufRet = ATL::CA2T(buf.c_str());
	return bufRet;
}

CString NL_QQwry::IP2Add(CString szIP)
{
	if(szIP==_T("")) return _T("");
	return GetCountryLocal(GetIndex(szIP));
}

int NL_QQwry::GetIndex(CString szIP)
{
	int index=-1;
	DWORD dwInputIP;
	DWORD dwStartIP;
	dwInputIP=IP2DWORD(szIP);
	//
	//利用半跳方法速度快一些
	//
	int iT;
	int iB,iE;
	iB=0;
	iE=GetRecordCount()-1;
	iT=iE/2;
	
	while(iB<iE)
	{
		dwStartIP=GetSIP(iT);
		if(dwInputIP==dwStartIP)
		{
			index =iT;
			break;
		}
		if((iE-iB)<=1)
		{
			/*
			CString s;
			s.Format("%d %d %d %d in:%08X s:%08X",iE-iB,iB,iT,iE,dwInputIP,dwStartIP);
			if(MessageBox(0,s,0,MB_YESNO)==IDNO);
			//*/
			for(int i=iB;i<=iE;i++)
			{
				dwStartIP=GetSIP(i);
				if(dwStartIP<=dwInputIP && dwInputIP<=m_dwLastIP)
				{
					index=i;
					break;
				}
			}
			break;
		}

		if(dwInputIP>dwStartIP)
		{
			iB=iT;
		}
		else
		{
			iE=iT;
		}
		iT=iB+(iE-iB)/2;
	}
	return index;
}


DWORD NL_QQwry::GetSIP(int index)
{
	DWORD ip;
	BYTE b[3];
	int ioff;
	if(!m_bOpen)return -1;
	if(index>GetRecordCount()-1)return -1;
	if(index<0)return -1;
	ioff=m_be.uBOff+index*7;
	m_file.Seek(ioff,CFile::begin);
	m_file.Read(&ip,4);
	m_file.Read(b,3);
	ioff=b[0]+b[1]*256+b[2]*256*256;
	m_file.Seek(ioff,CFile::begin);
	m_file.Read(&m_dwLastIP,4);
	return ip;
}


DWORD NL_QQwry::IP2DWORD(CString szIP)
{
	DWORD iIP;
	BYTE b[4];
	CString szTemp;
	char ch;
	int iLen;
	int iXB;
	szIP+=_T(".");
	memset(b,0,4);
	iLen=szIP.GetLength();
	iXB=0;
	iIP=0;
	for(int i=0;i<iLen;i++)
	{
		ch=ATL::CT2A(szIP)[i];
		szTemp+=ch;
		if(ch==_T('.'))
		{
			b[iXB]=atoi(ATL::CT2A(szTemp));
			szTemp=_T("");
			iXB++;
		}
	}
	iIP=b[0]*256*256*256+b[1]*256*256+b[2]*256+b[3];
	return iIP;
}

/*

CString NL_QQwry::Test()
{
	int ioff;
	CString str;
	//m_buf.Format("%08X %08X",m_be.uBOff,m_be.uEOff);
	m_buf.Format(_T("Total %d\r\n"),GetRecordCount());
	
	//for(m_i=GetRecordCount()-200;m_i<GetRecordCount();m_i++)
	for(m_i=0;m_i<1000;m_i++)
	{
		ioff=GetStartIPInfo(m_i);

		if(m_ei.bMode>=1 && m_ei.bMode<=8)
		{
			str.Format(_T("%6d %03d.%03d.%03d.%03d ")
				_T("%03d.%03d.%03d.%03d  %d  %06X "),
			m_i,
			m_ipoff.b3,
			m_ipoff.b2,
			m_ipoff.b1,
			m_ipoff.b0,
			m_ei.b3,
			m_ei.b2,
			m_ei.b1,
			m_ei.b0,
			m_ei.bMode,
			ioff);
		m_buf+=str;

			str=GetCountryLocal(m_ei.bMode,ioff+4);
			str+="\r\n";
		}
		else
		{
		str.Format(_T("%6d %03d.%03d.%03d.%03d ")
			_T("%03d.%03d.%03d.%03d (%d) %06X "),
			m_i,
			m_ipoff.b3,
			m_ipoff.b2,
			m_ipoff.b1,
			m_ipoff.b0,
			m_ei.b3,
			m_ei.b2,
			m_ei.b1,
			m_ei.b0,
			m_ei.bMode,
			ioff);
		m_buf+=str;

		str.Format(_T("%02X-%02X-%02X-%02X-%02X ")
			_T("%02X-%02X-%02X-%02X-%02X ")
			_T("%02X-%02X-%02X-%02X-%02X ")
			_T("%02X-%02X-%02X-%02X-%02X\r\n"),

			m_ei.buf[0],
			m_ei.buf[1],
			m_ei.buf[2],
			m_ei.buf[3],
			m_ei.buf[4],
			m_ei.buf[5],
			m_ei.buf[6],
			m_ei.buf[7],
			m_ei.buf[8],
			m_ei.buf[9],
			m_ei.buf[10],
			m_ei.buf[11],
			m_ei.buf[12],
			m_ei.buf[13],
			m_ei.buf[14],
			m_ei.buf[15],
			m_ei.buf[16],
			m_ei.buf[17],
			m_ei.buf[18],
			m_ei.buf[19]);
		}
		m_buf+=str;
	}
	return m_buf;
}
*/
