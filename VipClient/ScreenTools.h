#pragma once
#include "afxwin.h"


// CScreenTools 对话框

class CScreenTools : public CDialog
{
	DECLARE_DYNAMIC(CScreenTools)
public:
	bool b_IsInput;
	bool b_IsStop;
	bool b_IsGet;
	
	DWORD m_BitCount;
public:
	CScreenTools(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenTools();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCREENTOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ListBitCount;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboBitcount();
	afx_msg void OnBnClickedCheckScreen();
	afx_msg void OnBnClickedCheckInput();
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnCancel();
};
