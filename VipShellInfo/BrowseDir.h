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
	//���������ļ�
	virtual void EndProcessDir(LPCTSTR szFilePath){return ;}
	//����Ŀ¼dir����filespecָ�����ļ�
	//������Ŀ¼,���õ����ķ���
	//�������false,��ʾ��ֹ�����ļ�
	virtual BOOL ProcessDir(LPCTSTR szFilePath, const WIN32_FIND_DATA *filename){return TRUE;}

	//����BrowseDirÿ�ҵ�һ���ļ�,�͵���ProcessFile
	//�����ļ�����Ϊ�������ݹ�ȥ
	//�������false,��ʾ��ֹ�����ļ�
	//�û����Ը�д�ú���,�����Լ��Ĵ������
	virtual BOOL ProcessFile(LPCTSTR szFilePath, const WIN32_FIND_DATA *filename){return TRUE;}

private:
};

