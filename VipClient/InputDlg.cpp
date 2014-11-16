// InputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "InputDlg.h"
#include ".\inputdlg.h"


// CInputDlg �Ի���

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


// CInputDlg ��Ϣ�������

BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_strTitle);
	SetDlgItemText(IDC_EDIT_INPUT, m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CInputDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT_INPUT, m_strText);
	OnOK();
}
