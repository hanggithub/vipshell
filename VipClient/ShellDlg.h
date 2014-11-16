#pragma once
#include "CmdShellEdit.h"
#include "afxwin.h"


// CShellDlg �Ի���

class CShellDlg : public CDialog  
{
	DECLARE_DYNAMIC(CShellDlg)

public:
	CShellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShellDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GETSHELL };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
