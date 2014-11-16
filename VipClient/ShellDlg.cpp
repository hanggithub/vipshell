// ShellDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "ShellDlg.h"
#include ".\shelldlg.h"


// CShellDlg 对话框

IMPLEMENT_DYNAMIC(CShellDlg, CDialog)
CShellDlg::CShellDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShellDlg::IDD, pParent)
{
}

CShellDlg::~CShellDlg()
{
}

void CShellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CMDTEXT, m_CmdEdit);
}


BEGIN_MESSAGE_MAP(CShellDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_EN_UPDATE(IDC_EDIT_CMDTEXT, OnEnUpdateEditCmdtext)
END_MESSAGE_MAP()


// CShellDlg 消息处理程序

void CShellDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CShellDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"CmdShell";
	SetWindowText(strS);

	m_hServer.Begin();
	SetTimer(0x1, 100, NULL);
	m_CmdEdit.BindSocket(&m_hServer);
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CShellDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WCHAR szBuf[60000];
	m_hServer.ReadBuf(szBuf);
	if(lstrlenW(szBuf) == 0)
		return ;
	m_CmdEdit.AddText(szBuf);
	CDialog::OnTimer(nIDEvent);
}

void CShellDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_hServer.Close();
	// TODO: 在此处添加消息处理程序代码
}

HBRUSH CShellDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		COLORREF COLOR = RGB(0, 0, 0);
		HBRUSH brush = CreateSolidBrush(COLOR); //COLOR是你想设置的颜色 
		pDC->SetTextColor(RGB(255, 255, 255));
	 	return (HBRUSH) brush;	
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CShellDlg::OnEnUpdateEditCmdtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数，将 EM_SETEVENTMASK 消息发送到控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。
	// TODO:  在此添加控件通知处理程序代码
}

