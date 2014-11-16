// ScreenTools.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "ScreenTools.h"
#include ".\screentools.h"
#include ".\getscreeninputdlg.h"

// CScreenTools �Ի���

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


// CScreenTools ��Ϣ�������

BOOL CScreenTools::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListBitCount.AddString(_T("��ɫ"));
	m_ListBitCount.AddString(_T("16ɫ"));
	m_ListBitCount.AddString(_T("256ɫ"));
	m_ListBitCount.AddString(_T("16����ɫ"));
	m_ListBitCount.AddString(_T("24���ɫ"));
	m_ListBitCount.AddString(_T("32��ǿɫ"));
	m_ListBitCount.SelectString(0, _T("256ɫ"));
	m_BitCount = 8;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CScreenTools::OnCbnSelchangeComboBitcount()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText;
	GetDlgItemText(IDC_COMBO_BITCOUNT, strText);
	if ( strText == _T("��ɫ"))
		m_BitCount = 1;
	if ( strText == _T("16ɫ"))
		m_BitCount = 4;
	if ( strText == _T("256ɫ"))
		m_BitCount = 8;
	if ( strText == _T("16����ɫ"))
		m_BitCount = 16;
	if ( strText == _T("24���ɫ"))
		m_BitCount = 24;
	if ( strText == _T("32��ǿɫ"))
		m_BitCount = 32;
//	GetDlgItem(IDC_CHECK_SCREEN)->SetFocus();
}

void CScreenTools::OnBnClickedCheckScreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CGetScreenInputDlg* pWnd = (CGetScreenInputDlg*)GetParent();
	pWnd->GetPicture();
//	pWnd->OnGet
//	OnOK();
}

void CScreenTools::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	ShowWindow(SW_HIDE);
}
