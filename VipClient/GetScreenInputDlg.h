#pragma once
#include "picturewnd.h"
#include ".\serverscreeninfohandle.h"
#include ".\serverconnecthandle.h"
#include ".\screentools.h"
#include "afxwin.h"


// CGetScreenInputDlg 对话框

class CGetScreenInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CGetScreenInputDlg)

public:
	CGetScreenInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGetScreenInputDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCREEN };
public:
	void BindSocket(SOCKET s, SOCKET sIn)
	{
		m_hInput.Attach(sIn);
		m_hServer.Attach(s);
	}
public:
	void Layout();
protected:
	CServerConnectHandle m_hInput;
	CServerScreenInfoHandle m_hServer;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	void __GetScreenInfo();
protected:
	bool b_IsStop;
	CScreenTools m_Tools;
	HANDLE m_hThread;
public:
	void GetPicture();
	CPictureWnd m_PicWnd;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
