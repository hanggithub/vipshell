#pragma once
#include "CmdShellEdit.h"
#include "afxwin.h"


// CShellDlg 对话框

class CShellDlg : public CDialog  
{
	DECLARE_DYNAMIC(CShellDlg)

public:
	CShellDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShellDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GETSHELL };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CServerShellHandle m_hServer;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	CCmdShellEdit m_CmdEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnUpdateEditCmdtext();
};
