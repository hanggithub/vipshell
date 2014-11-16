// ProcessListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "ProcessListDlg.h"
#include ".\processlistdlg.h"


// CProcessListDlg �Ի���

IMPLEMENT_DYNAMIC(CProcessListDlg, CDialog)
CProcessListDlg::CProcessListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessListDlg::IDD, pParent)
{
}

CProcessListDlg::~CProcessListDlg()
{
}

void CProcessListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ListProcess);
}


BEGIN_MESSAGE_MAP(CProcessListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_KILL, OnBnClickedButtonKill)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListProcess)
END_MESSAGE_MAP()


// CProcessListDlg ��Ϣ�������

void CProcessListDlg::OnBnClickedButtonGet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n = m_hServer.GetProcessList(m_pVecTor);
	if(n != -1)
		OnInitList(m_pVecTor);
}
DWORD CProcessListDlg::_GetSelItem()
{
	POSITION posti = m_ListProcess.GetFirstSelectedItemPosition();
	if(posti == NULL)
		return -1;
	DWORD dwSel = m_ListProcess.GetNextSelectedItem(posti);
	return dwSel;
}
void CProcessListDlg::OnBnClickedButtonKill()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwPid = _GetSelItem();
	if(dwPid == -1)
		return ;
	if ( IDYES != MessageBox(L"�Ƿ��������?", NULL, MB_YESNO) )
		return ;

	DWORD nIndex = m_ListProcess.GetItemData(dwPid);
	m_hServer.KillProcess(m_pVecTor->at(nIndex).dwPid);
}
void CProcessListDlg::OnInitList(std::vector<tagVipShellProcess>* pVecTor)
{
	m_ListProcess.DeleteAllItems();
	for(DWORD i = 0; i < pVecTor->size(); i++)
	{
		DWORD daCount = m_ListProcess.GetItemCount();
		int nRet = m_ListProcess.InsertItem(daCount, pVecTor->at(i).cFileName, 0);
		m_ListProcess.SetItemData(nRet, i);
	}
	
}
BOOL CProcessListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	WCHAR szIp[100] =L"";
	DWORD dwPort = m_hServer.GetPeer(szIp);
	CString strS; strS.Format(L"%s:%d", szIp, dwPort);
	strS += L"���̹���";
	SetWindowText(strS);

	m_ListProcess.InsertColumn(0, L"����·��", LVCFMT_LEFT, 400,  -1);
	OnInitList(m_pVecTor);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProcessListDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_hServer.Detach();
	CDialog::OnCancel();
}
int CALLBACK CProcessListDlg::SortCompareStringFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CProcessListDlg* pThis = (CProcessListDlg*)lParamSort;
	int c = 0;
	if( pThis->m_dwClickItem == 0)
	{
		CString strItem1 = pThis->m_pVecTor->at(lParam1).cFileName;
		CString strItem2 = pThis->m_pVecTor->at(lParam2).cFileName;
		c = strItem1.CompareNoCase(strItem2);
	}else
		c = 0;
	if(pThis->m_dwCmpType)
		c = -c;
	return c;
}
void CProcessListDlg::OnHdnItemclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_dwCmpType = ! m_dwCmpType;
	m_dwClickItem = phdr->iItem;
	m_ListProcess.SortItems(SortCompareStringFunc,(DWORD_PTR) this);
}
