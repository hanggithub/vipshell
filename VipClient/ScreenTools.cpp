// ScreenTools.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "ScreenTools.h"
#include ".\screentools.h"
#include ".\getscreeninputdlg.h"

// CScreenTools 对话框

IMPLEMENT_DYNAMIC(CScreenTools, CDialog)
CScreenTools::CScreenTools(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenTools::IDD, pParent)
{
	
	b_IsGet = false;
	b_IsInput = false;
}

CScreenTools::~CScreenTools()
{
}

void CScreenTools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BITCOUNT, m_ListBitCount);
}


BEGIN_MESSAGE_MAP(CScreenTools, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_BITCOUNT, OnCbnSelchangeComboBitcount)
	ON_BN_CLICKED(IDC_CHECK_SCREEN, OnBnClickedCheckScreen)
	ON_BN_CLICKED(IDC_CHECK_INPUT, OnBnClickedCheckInput)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CScreenTools 消息处理程序

BOOL CScreenTools::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListBitCount.AddString(_T("两色"));
	m_ListBitCount.AddString(_T("16色"));
	m_ListBitCount.AddString(_T("256色"));
	m_ListBitCount.AddString(_T("16基本色"));
	m_ListBitCount.AddString(_T("24真彩色"));
	m_ListBitCount.AddString(_T("32增强色"));
	m_ListBitCount.SelectString(0, _T("256色"));
	m_BitCount = 8;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CScreenTools::OnCbnSelchangeComboBitcount()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItemText(IDC_COMBO_BITCOUNT, strText);
	if ( strText == _T("两色"))
		m_BitCount = 1;
	if ( strText == _T("16色"))
		m_BitCount = 4;
	if ( strText == _T("256色"))
		m_BitCount = 8;
	if ( strText == _T("16基本色"))
		m_BitCount = 16;
	if ( strText == _T("24真彩色"))
		m_BitCount = 24;
	if ( strText == _T("32增强色"))
		m_BitCount = 32;
//	GetDlgItem(IDC_CHECK_SCREEN)->SetFocus();
}

void CScreenTools::OnBnClickedCheckScreen()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBut = (CButton*)GetDlgItem(IDC_CHECK_SCREEN);
	if( pBut->GetCheck()  == 0)
	{
		GetDlgItem(IDOK)->EnableWindow();
		b_IsGet = false;
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		b_IsGet = true;
	}
}

void CScreenTools::OnBnClickedCheckInput()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBut = (CButton*)GetDlgItem(IDC_CHECK_INPUT);
	if( pBut->GetCheck()  == 0)
	{
	//	GetDlgItem(IDOK)->EnableWindow();
		b_IsInput = false;
	}
	else
	{
	//	GetDlgItem(IDOK)->EnableWindow(FALSE);
		b_IsInput = true;
	}
}

void CScreenTools::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CGetScreenInputDlg* pWnd = (CGetScreenInputDlg*)GetParent();
	pWnd->GetPicture();
//	pWnd->OnGet
//	OnOK();
}

void CScreenTools::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
}
