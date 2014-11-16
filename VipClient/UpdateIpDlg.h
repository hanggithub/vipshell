#pragma once


// CUpdateIpDlg 对话框

class CUpdateIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateIpDlg)

public:
	CUpdateIpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateIpDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATEIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strFtpServer;
	DWORD m_dwPortFtp;
	DWORD m_dwPortMy;
	CString m_strMyIp;
	CString m_strFile;
	CString m_strUser;
	CString m_strPass;
	virtual BOOL OnInitDialog();
};
