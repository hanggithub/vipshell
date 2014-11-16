// GetScreenInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "GetScreenInputDlg.h"
#include ".\getscreeninputdlg.h"


// CGetScreenInputDlg 对话框

IMPLEMENT_DYNAMIC(CGetScreenInputDlg, CDialog)
CGetScreenInputDlg::CGetScreenInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetScreenInputDlg::IDD, pParent)
{
	m_hThread = NULL;
	b_IsStop = false;

}

CGetScreenInputDlg::~CGetScreenInputDlg()
{
}

void CGetScreenInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGetScreenInputDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CGetScreenInputDlg 消息处理程序


void CGetScreenInputDlg::Layout()
{
	if( m_PicWnd.m_hWnd == NULL)
		return ;
	CRect rc;
	GetClientRect(&rc);
	CRect rcPic = rc;
//	rcPic.top += 50; 
	m_PicWnd.MoveWindow(&rcPic);
}
DWORD WINAPI ThreadGetScreen(LPVOID lp)
{
	CGetScreenInputDlg* pThis = (CGetScreenInputDlg*)lp;
	pThis->__GetScreenInfo();
	return 0;
}

BOOL CGetScreenInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_PicWnd.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL, CRect(0, 0, 200, 200), this, 0);
	m_PicWnd.ModifyStyle(NULL, WS_HSCROLL | WS_VSCROLL);


	theApp.AddSocket(m_hInput.GetSocket());
	m_Tools.Create(IDD_DIALOG_SCREENTOOLS, this);
	m_Tools.ShowWindow(SW_SHOW);
	GetPicture();

	CMenu* pSysMenu = GetSystemMenu(FALSE); 
	if (pSysMenu != NULL) 
	{ 
		pSysMenu->AppendMenu(MF_STRING, 0xF00011, L"显示工具条"); 
		pSysMenu->AppendMenu(MF_STRING, 0xF00012, L"发送Ctrl + Alt + Delete"); 

	}
	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"屏幕信息";
	SetWindowText(strS);
	Layout();
	m_hThread = CreateThread(NULL, 0, ThreadGetScreen, this, 0, NULL);
	m_PicWnd.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGetScreenInputDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Layout();
	// TODO: 在此处添加消息处理程序代码
}

void CGetScreenInputDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
}

void CGetScreenInputDlg::OnDestroy()
{

	CDialog::OnDestroy();
	b_IsStop = true;
	m_hServer.Close();
	theApp.ReMoveSocket(m_hInput.Detach());
	if(m_hThread != NULL)
		WaitForSingleObject(m_hThread, INFINITE);
	// TODO: 在此处添加消息处理程序代码
}

void CGetScreenInputDlg::__GetScreenInfo()
{
	while(true)
	{
		if(b_IsStop)
			break;
		if(m_Tools.b_IsGet)
		{
			HBITMAP hBitmap = m_hServer.GetScreenInfo(m_Tools.m_BitCount);
			if( hBitmap != NULL)
				m_PicWnd.SetBitmap(hBitmap);
		}
		else
			Sleep(100);
	}
}



BOOL CGetScreenInputDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	bool b_IsInput = m_Tools.b_IsInput;
	if(pMsg->message == WM_KEYDOWN && b_IsInput)
	{
		m_hInput.SendKeyDown(pMsg->wParam);
		return true;
	}
	if(pMsg->message == WM_KEYUP && b_IsInput)
	{
		m_hInput.SendKeyUp(pMsg->wParam);
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE && b_IsInput)
	{

		SCROLLINFO hStructure, vStructure;
		m_PicWnd.GetScrollInfo(SB_HORZ, &hStructure);
		m_PicWnd.GetScrollInfo(SB_VERT, &vStructure);
		CRect rc;
		m_PicWnd.GetClientRect(&rc);
		int nXPos = hStructure.nPos;
		int nYPos = vStructure.nPos;
		if( nXPos + rc.Width() > hStructure.nMax)
			nXPos =  hStructure.nMax - rc.Width();
		if (nYPos + rc.Height() >  vStructure.nMax)
			nYPos = vStructure.nMax - rc.Height();
		int dwX = nXPos + LOWORD(pMsg->lParam);
		int dwY =  nYPos + HIWORD(pMsg->lParam);
		if( rc.Width() >= hStructure.nMax)
			dwX = LOWORD(pMsg->lParam);
		if( rc.Height() >= vStructure.nMax)
			dwY = HIWORD(pMsg->lParam);
		POINT pt;
		pt.x = dwX;
		pt.y = dwY;

		CString str;
		str.Format(L"%d  %d", dwX, dwY);
		SetWindowText(str);

		m_hInput.SendMouseMove(&pt);
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN && b_IsInput)
	{
		m_hInput.SendLButtonDown();
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_LBUTTONUP && b_IsInput)
	{
		m_hInput.SendLButtonUp();
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_LBUTTONDBLCLK && b_IsInput)
	{
		m_hInput.SendLButtonDblclk();
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN && b_IsInput)
	{
		m_hInput.SendRButtonDown();
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_RBUTTONUP && b_IsInput)
	{
		m_hInput.SendRButtonUp();
		return true;
	}
	if(pMsg->hwnd == m_PicWnd.GetSafeHwnd() && pMsg->message == WM_RBUTTONDBLCLK && b_IsInput)
	{
		m_hInput.SendRButtonDblclk();
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CGetScreenInputDlg::GetPicture()
{
	HBITMAP hBitmap = m_hServer.GetScreenInfo(m_Tools.m_BitCount);
	if( hBitmap != NULL)
		m_PicWnd.SetBitmap(hBitmap);
}


void CGetScreenInputDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( 0xF00011 == nID)
		m_Tools.ShowWindow(SW_SHOW);
	else if ( 0xF00012 == nID)
		m_hInput.SendKeyCtrlAltDel();
	CDialog::OnSysCommand(nID, lParam);
}
