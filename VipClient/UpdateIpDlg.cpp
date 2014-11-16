// UpdateIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "UpdateIpDlg.h"
#include ".\updateipdlg.h"
#include <afxinet.h>
#include "iphelper.h"
#include "stdio.h"

// CUpdateIpDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateIpDlg, CDialog)
CUpdateIpDlg::CUpdateIpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateIpDlg::IDD, pParent)
	, m_strFtpServer(_T(""))
	, m_dwPortFtp(0)
	, m_dwPortMy(0)
	, m_strMyIp(_T(""))
	, m_strFile(_T(""))
	, m_strUser(_T(""))
	, m_strPass(_T(""))
{
}

CUpdateIpDlg::~CUpdateIpDlg()
{
}

void CUpdateIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FTPSERVER, m_strFtpServer);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPortFtp);
	DDX_Text(pDX, IDC_EDIT_PORTMY, m_dwPortMy);
	DDX_CBString(pDX, IDC_COMBO_IP, m_strMyIp);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
}


BEGIN_MESSAGE_MAP(CUpdateIpDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUpdateIpDlg 消息处理程序

void CUpdateIpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CInternetSession ses;
	CFtpConnection* hFTP = ses.GetFtpConnection(m_strFtpServer, m_strUser, m_strPass, (INTERNET_PORT)m_dwPortFtp,FALSE);
	if( !hFTP )
	{
		MessageBox(L"登陆失败");
		return ;
	}
	CString strText; strText.Format(L"%s:%d", m_strMyIp, m_dwPortMy);
	char szTmpPath[MAX_PATH];
	memset(szTmpPath, 0, sizeof(szTmpPath));
	GetTempPathA(MAX_PATH, szTmpPath);
	strcat(szTmpPath, "ip.txt");
	FILE* fp = fopen(szTmpPath, "w");
	fwrite((LPCSTR)ATL::CT2A(strText), 1, lstrlen(strText), fp);
	fclose(fp);
	BOOL bIsOK = hFTP->PutFile(ATL::CA2T(szTmpPath), m_strFile);
	if(!bIsOK)
	{
		MessageBox(L"传输失败");
		return ;
	}
	hFTP->Close();
	ses.Close();

	OnOK();
}

BOOL CUpdateIpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	WCHAR szIp[256] = {0};
	_tcscpy(szIp, iphelper::basic::IP2Addr(iphelper::adapter::GetBestInterface(0)).c_str());
//	CComboBox* pList = (CComboBox*)GetDlgItem(IDC_COMBO_IP);
//	pList->AddString(szIp);
//	pList->SelectString(0, szIp);
	m_strMyIp = szIp;
	m_strFile = L"/ip.txt";
	m_dwPortFtp = 21;
	m_dwPortMy = 52044;
	m_strUser = L"zhuifengjian";
	m_strPass = L"2004";
	m_strFtpServer = L"www.zhuifengjian.net";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
