/*
* Copyright (c) 2004,NLSoft 
* Http://newnan.8u8.com
* All rights reserved. At �Ӽ�
* 
* �ļ����ƣ�NL_QQwry.h
* �ļ���ʶ��NL_QQwry Class
* ժ    Ҫ��QQIP�� QQWry.dat�Ĳ���
* 
* ��ǰ�汾��1.0
* ��    �ߣ�����
* ������ڣ�2004��9��3��
*
* ԭ����  ��QQwry.dat �ļ���ʽ˵������cnss��blog����
* ժ    Ҫ��http://blog.csdn.net/cnss/archive/2004/08/18/77628.aspx
*/

//                
// NL_QQwry.h: interface for the NL_QQwry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NL_QQWRY_H__A4517C27_CB7E_4BA6_BCBA_2335AB8EC71B__INCLUDED_)
#define AFX_NL_QQWRY_H__A4517C27_CB7E_4BA6_BCBA_2335AB8EC71B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXBUF 50 
typedef struct _tagEndInfo
{
	BYTE b0;
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE buf[MAXBUF];
	BYTE bMode;
	int offset1;
	int offset2;
}EndInfo,PEndInfo;

typedef struct _tagIPOFF
{
	BYTE b0;
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE off1;
	BYTE off2;
	BYTE off3;
}IPOFF,*PIPOFF;

typedef struct _tagBE
{
	int uBOff;
	int uEOff;
}BE,*PBE;

class NL_QQwry  
{
public:
	DWORD m_dwLastIP;
	CString IP2Add(CString szIP);
	CString GetCountryLocal(int index);
	DWORD GetSIP(int index);
	DWORD IP2DWORD(CString szIP);
	int GetIndex(CString szIP);
	void SaveToFile();
	CString GetStr(void);
	CString GetCountryLocal(BYTE bMode,int ioffset);
	CString GetStr(int ioffset);
	int GetRecordCount(void);
	int m_i;
	int GetStartIPInfo(int iIndex);
	CString m_buf;
	CString Test(void);
	bool GetBE(void);
	bool OpenQQwry(CString szFileName);
	void CloseQQwry(void);
	BE m_be;
	IPOFF m_ipoff;
	EndInfo m_ei;
	NL_QQwry();
	virtual ~NL_QQwry();
private:
	bool m_bOpen;
	CFile m_file;
};

#endif // !defined(AFX_NL_QQWRY_H__A4517C27_CB7E_4BA6_BCBA_2335AB8EC71B__INCLUDED_)
