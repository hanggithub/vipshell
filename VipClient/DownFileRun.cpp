// DownFileRun.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "DownFileRun.h"
#include ".\downfilerun.h"


// CDownFileRun �Ի���

IMPLEMENT_DYNAMIC(CDownFileRun, CDialog)
CDownFileRun::CDownFileRun(CWnd* pParent /*=NULL*/)
	: CDialog(CDownFileRun::IDD, pParent)
{
}

CDownFileRun::~CDownFileRun()
{
}

void CDownFileRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDownFileRun, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDownFileRun ��Ϣ�������

void CDownFileRun::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strUrl ; GetDlgItemText(IDC_EDIT_DOWNDFILERUN, strUrl);
	strUrl.Replace(L"\\", L"/");
	bool r = m_hServer.DownFileAndRun(strUrl);
	OnOK();
}
