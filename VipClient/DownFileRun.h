#pragma once


// CDownFileRun 对话框
#include ".\vipshellclientserver.h"

class CDownFileRun : public CDialog
{
	DECLARE_DYNAMIC(CDownFileRun)

public:
	CDownFileRun(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownFileRun();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNFILERUN };
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	CServerConnectHandle m_hServer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
