#pragma once


// CInputDlg �Ի���

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CInputDlg(CWnd* pParent , LPCTSTR szTitle, LPCTSTR szDef = NULL);
	virtual ~CInputDlg();

	CString m_strTitle;
	CString m_strText;
// �Ի�������
	enum { IDD = IDD_DIALOG_INPUTMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
