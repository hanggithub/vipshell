// GetCapPictureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "GetCapPictureDlg.h"
#include ".\getcappicturedlg.h"


// CGetCapPictureDlg 对话框

IMPLEMENT_DYNAMIC(CGetCapPictureDlg, CDialog)
CGetCapPictureDlg::CGetCapPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetCapPictureDlg::IDD, pParent)
{
	m_hThread = NULL;
	b_IsStop = false;
	m_bIsFull = false;
}

CGetCapPictureDlg::~CGetCapPictureDlg()
{
	if( m_hBitmap.m_hObject != NULL )
		m_hBitmap.DeleteObject();
}

void CGetCapPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGetCapPictureDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CGetCapPictureDlg 消息处理程序



BOOL CGetCapPictureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CMenu* pSysMenu = GetSystemMenu(FALSE); 
	if (pSysMenu != NULL) 
	{ 
		pSysMenu->AppendMenu(MF_STRING, 0xF00011, L"自动调整大小"); 
		pSysMenu->AppendMenu(MF_STRING, 0xF00012, L"取消自动调整大小"); 
	}
	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"视频信息";
	SetWindowText(strS);
//	m_hThread = CreateThread(NULL, 0, ThreadGetCapData, this, 0, NULL);
	SetTimer(0x1, 100, NULL);
	m_hServer.Start(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CGetCapPictureDlg::SetBitmap(HBITMAP hBitmap)
{
	CSingleLock lock(&m_CriticalSectionhBitmap, TRUE);
	if( hBitmap == NULL)
		return ;
	if( m_hBitmap.m_hObject != NULL )
		m_hBitmap.DeleteObject();
	m_hBitmap.Attach(hBitmap);
}

void CGetCapPictureDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	m_hServer.Close();
	m_hServer.Stop();
	Sleep(2000);
	CDialog::OnCancel();
}

BOOL CGetCapPictureDlg::DrawBitmap()
{
	CSingleLock lock(&m_CriticalSectionhBitmap, TRUE);
	if( m_hBitmap.m_hObject != NULL)
	{
		CClientDC dc(this);
		CDC hdc ;
		hdc.CreateCompatibleDC(&dc);
		CRect rc;
		GetClientRect(&rc);
		CBitmap* hOld = hdc.SelectObject(&m_hBitmap); 
		BITMAP              bitmap ;
		m_hBitmap.GetBitmap(&bitmap);
		if(m_bIsFull)
		if( rc.Width() != bitmap.bmWidth || rc.Height() != bitmap.bmHeight)
		{
			CRect rcWnd;
			GetWindowRect(&rcWnd);
			SetWindowPos(NULL, 0, 0,( rcWnd.Width() - rc.Width() ) + bitmap.bmWidth,  rcWnd.Height()  - rc.Height() + bitmap.bmHeight , SWP_NOZORDER | SWP_NOMOVE );
		}
		if(!m_bIsFull)
			dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		else
			dc.BitBlt(0, 0, rc.Width(), rc.Height(), &hdc, 0, 0, SRCCOPY);
	//	
		dc.SelectObject(hOld);
	}
	return TRUE;
}

void CGetCapPictureDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DrawBitmap();
	CDialog::OnTimer(nIDEvent);
}

void CGetCapPictureDlg::OnPicture(HBITMAP hBitmap)
{
	SetBitmap(hBitmap);
}

void CGetCapPictureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID == 0xF00011)
		m_bIsFull = true;
	else if (0xF00012 == nID)
		m_bIsFull = false;
	__super::OnSysCommand(nID, lParam);
}
