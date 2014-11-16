#pragma once
#include ".\serverfilemanagehandle.h"
#include "afxcmn.h"
#include "afxwin.h"


// CFileManageDlg 对话框

class CFileManageDlg : public CDialog, public CFileManageCallBack
{
	DECLARE_DYNAMIC(CFileManageDlg)

public:
	CFileManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileManageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILEMANAGE };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
public:
	struct strTransmitFile
	{
		CString strP;
		CString strR;
	};
	void __InitWorkVec();
	void __SatrtWork();
	std::vector<strTransmitFile> m_DownFileVec;
	std::vector<strTransmitFile> m_UpFileVec;
	std::vector<strTransmitFile> m_DownDirVec;
	std::vector<strTransmitFile> m_UpDirVec;
	bool m_bIsClose;
	void __OnDoWork();
	HANDLE m_hThread;
public:
	virtual void OnDownFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime);
	virtual void OnUpFileIng(LPCTSTR szPath, LPCTSTR szRemote, DWORD dwBig, DWORD dwLen, DWORD dwSTime);
protected:
	std::vector<CString> m_strFileType; //文件类型
	void _AddImgFileType(HICON hIcon, LPCTSTR strFile);
	void _AddImgFileType(UINT dwImgFile, LPCTSTR strFile);
	DWORD _GetFileImg(LPCTSTR sz);
	HICON __FindFileType(LPCTSTR szType);
	CImageList m_FileImgList;
	void __InitImgList();

	//排序
	static int CALLBACK SortCompareNumberFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	CServerFileManageHandle::File_Info_Vector m_FileInfo;
	int m_nHeadItem;
	bool m_bCompareType;
protected:
	CStatusBarCtrl m_wndStatusBar;
protected:
	//远程文件的目录
	CString m_strCurrentPath;
	void SetCurrentPath(LPCTSTR sz);
	CString GetCurrentPath();
	CString GetCurrentUpPath(LPCTSTR str);
	
	CString _GetNameByPath(LPCTSTR szPath);
protected:
	CString __MakeFileSizeString(DWORD dwSize);
	CString __MakeFileTimeString(FILETIME& fileTime);
	CString __MakeTimeString(DWORD dwTime);
public:
	tagVipShellFileInfo* __GetFileInfoByIndex(DWORD dwIndex);
	CString __GetFileNameByIndex(DWORD dwIndex);
protected:
	void __OnGetRoot();
	void __OnGetFileInfo(LPCTSTR szPath);
	
protected:
	DWORD _GetSelItem();
	
	DWORD _GetSelItemToVector(std::vector<DWORD>* pVector);
protected:
	CServerFileManageHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool IsRoot();
	void Layout();
	BOOL SelectSaveFilePath(CString& strPath);
	BOOL IsDir(WIN32_FIND_DATA& file);
	BOOL IsDir(LPCTSTR szFile);
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_ListFile;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileShow();
	afx_msg void OnFileHide();
	afx_msg void OnFileMove();
	afx_msg void OnFileDel();
	afx_msg void OnFileDown();
	afx_msg void OnLvnEndlabeleditListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_ComBoxFilePath;
protected:
	virtual void OnCancel();
public:
	afx_msg void OnHdnItemclickListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileRunat();
};
