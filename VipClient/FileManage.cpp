#include "stdafx.h"
#include ".\filemanage.h"




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
