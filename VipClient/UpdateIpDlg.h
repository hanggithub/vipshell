#pragma once


// CUpdateIpDlg �Ի���

class CUpdateIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateIpDlg)

public:
	CUpdateIpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateIpDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATEIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
