#pragma once


// CMakeFileDlg �Ի���

typedef struct tagVipShellConfig
{
	WCHAR szUrl[256];
	WCHAR szExplain[100];
	WCHAR szCfgFile[100];
	WCHAR szServerName[100];
}VIPSHELLCONFIG, *LPVIPSHELLCONFIG;


class CMakeFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CMakeFileDlg)

public:
	CMakeFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMakeFileDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAKEFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool MakeDll(LPCTSTR szDllFile);
	bool SetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg1);
	afx_msg void OnBnClickedOk();
	CString m_strUrl;
	CString m_strExplain;
	CString m_strServerName;
	CString m_strCFfile;
};
