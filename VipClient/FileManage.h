#pragma once
#include "connectbase.h"
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