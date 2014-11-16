#pragma once
#include ".\servergetcappicture.h"
#include "afxmt.h"
// CGetCapPictureDlg 对话框

class CGetCapPictureDlg : public CDialog , public CCapPictureCallBack
{
	DECLARE_DYNAMIC(CGetCapPictureDlg)

public:
	CGetCapPictureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGetCapPictureDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GETCAP };
public:
	void BindSocket(SOCKET s)
	{
		m_hServer.Attach(s);
	}
protected:
	void SetBitmap(HBITMAP hBitmap);
	CBitmap m_hBitmap;
	CCriticalSection m_CriticalSectionhBitmap;
protected:
	bool m_bIsFull;
	bool b_IsStop;
	HANDLE m_hThread;
	CServerGetCapPictureHandle m_hServer;
public:
	BOOL DrawBitmap();
	void Layout();
public:
	virtual void OnPicture(HBITMAP hBitmap);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
