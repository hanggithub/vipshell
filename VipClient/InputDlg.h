#pragma once


// CInputDlg 对话框

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	CInputDlg(CWnd* pParent , LPCTSTR szTitle, LPCTSTR szDef = NULL);
	virtual ~CInputDlg();

	CString m_strTitle;
	CString m_strText;
// 对话框数据
	enum { IDD = IDD_DIALOG_INPUTMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
