#pragma once
#include <Windows.h>
class CBrowseDir
{
public:
	CBrowseDir(void){}
	~CBrowseDir(void){}
public:
	BOOL BrowseDir(LPCTSTR szPath)
	{
		BOOL bIsOk = TRUE;
		TCHAR szFilePath[MAX_PATH] = {0};
		lstrcpy(szFilePath, szPath);
		lstrcat(szFilePath, _T("*.*"));
		WIN32_FIND_DATA fileinfo;
		HANDLE hFile = FindFirstFile( szFilePath, &fileinfo );
		if(hFile == INVALID_HANDLE_VALUE )
			return FALSE;
		do 
		{
			if(fileinfo.cFileName[0] == _T('.'))
				continue;
			if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!ProcessDir(szPath, &fileinfo))
				{
					FindClose(hFile);
					bIsOk = FALSE;
					break;
				}
				TCHAR szFilePathDir[MAX_PATH] = {0};
				wsprintf(szFilePathDir, _T("%s%s\\"), szPath, fileinfo.cFileName);
				if(!BrowseDir(szFilePathDir))
				{
					FindClose(hFile);
					bIsOk = FALSE;
					break;
				}
			}
			else
			{
				if(!ProcessFile(szPath, &fileinfo))
				{
					FindClose(hFile);
					bIsOk = FALSE;
					break;
				}		
			}
		} while(FindNextFile(hFile, &fileinfo));
		FindClose(hFile);
		EndProcessDir(szPath);
		return bIsOk;
	}
protected:
	//结束遍历文件
	virtual void EndProcessDir(LPCTSTR szFilePath){return ;}
	//遍历目录dir下由filespec指定的文件
	//对于子目录,采用迭代的方法
	//如果返回false,表示中止遍历文件
	virtual BOOL ProcessDir(LPCTSTR szFilePath, const WIN32_FIND_DATA *filename){return TRUE;}

	//函数BrowseDir每找到一个文件,就调用ProcessFile
	//并把文件名作为参数传递过去
	//如果返回false,表示中止遍历文件
	//用户可以覆写该函数,加入自己的处理代码
	virtual BOOL ProcessFile(LPCTSTR szFilePath, const WIN32_FIND_DATA *filename){return TRUE;}

private:
};

