// DownFileRun.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "DownFileRun.h"
#include ".\downfilerun.h"


// CDownFileRun 对话框

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


// CDownFileRun 消息处理程序

void CDownFileRun::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strUrl ; GetDlgItemText(IDC_EDIT_DOWNDFILERUN, strUrl);
	strUrl.Replace(L"\\", L"/");
	bool r = m_hServer.DownFileAndRun(strUrl);
	OnOK();
}
