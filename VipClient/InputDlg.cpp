// InputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "InputDlg.h"
#include ".\inputdlg.h"


// CInputDlg 对话框

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)
CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
}
CInputDlg::CInputDlg(CWnd* pParent, LPCTSTR szTitle, LPCTSTR szDef )
: CDialog(CInputDlg::IDD, pParent)
{
	CInputDlg::CInputDlg(pParent);
	m_strTitle = szTitle;
	m_strText = szDef;
}
CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDlg 消息处理程序

BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(m_strTitle);
	SetDlgItemText(IDC_EDIT_INPUT, m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CInputDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_INPUT, m_strText);
	OnOK();
}
