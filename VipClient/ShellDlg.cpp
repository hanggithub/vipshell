// ShellDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "ShellDlg.h"
#include ".\shelldlg.h"


// CShellDlg �Ի���

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


// CShellDlg ��Ϣ�������

void CShellDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CShellDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ˴������Ϣ����������
}

HBRUSH CShellDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		COLORREF COLOR = RGB(0, 0, 0);
		HBRUSH brush = CreateSolidBrush(COLOR); //COLOR���������õ���ɫ 
		pDC->SetTextColor(RGB(255, 255, 255));
	 	return (HBRUSH) brush;	
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CShellDlg::OnEnUpdateEditCmdtext()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// �������� EM_SETEVENTMASK ��Ϣ���͵��ؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

