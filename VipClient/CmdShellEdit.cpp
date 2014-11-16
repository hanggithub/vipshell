// CmdShellEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipClient.h"
#include "CmdShellEdit.h"
#include ".\cmdshelledit.h"



// CCmdShellEdit

IMPLEMENT_DYNAMIC(CCmdShellEdit, CEdit)
CCmdShellEdit::CCmdShellEdit()
{
	m_TextdwLen = 0;
	m_hServer = NULL;
}

CCmdShellEdit::~CCmdShellEdit()
{
}


BEGIN_MESSAGE_MAP(CCmdShellEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CCmdShellEdit ��Ϣ�������


BOOL CCmdShellEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�õ�Edit�ؼ�����򣬼���������
	 CEdit::OnEraseBkgnd(pDC);
	RECT updatarect; 
	GetClientRect(&updatarect);
	//���������һ�����һ����ɫ�ľ���
	CBrush newBrush;
	newBrush.CreateSolidBrush(RGB(0,0,0));
	CBrush * oldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(&updatarect);
	pDC->SelectObject(oldBrush);
	return TRUE;
	
}

UINT CCmdShellEdit::OnGetDlgCode()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE ); 
	return CEdit::OnGetDlgCode();
}

void CCmdShellEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nStartChar;
	int nEndChar ;
	GetSel(nStartChar, nEndChar);
	int nTextLen = GetWindowTextLength();

	if(nChar == 8)
	{
		if(nStartChar <= m_TextdwLen - 1 )
			nStartChar = m_TextdwLen - 1;
		if(nEndChar <= m_TextdwLen - 1 )
			nEndChar = m_TextdwLen - 1;
		SetSel(nStartChar, nEndChar);
	//	if(m_TextdwLen == nTextLen)
		if(nEndChar < m_TextdwLen )
			return ;
	//	nEndChar--;
///		
	}
	if(nEndChar < m_TextdwLen - 1)
		return ;
	if(nStartChar < m_TextdwLen - 1)
		return ;
	if(nChar == 13)
	{
		SetSel(nTextLen, -1);

		CEdit::OnChar(nChar, nRepCnt, nFlags);
		CString strText; GetWindowText(strText);
		CString strCmd = strText.Mid(m_TextdwLen - 1);
		if(m_hServer != NULL)
			m_hServer->SendCmd(strCmd);
		return ;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CCmdShellEdit::AddText(LPCWSTR szText)
{
	int nLen = GetWindowTextLength();
	CString str ;
	this->GetWindowText(str);
	str += szText;
	SetWindowText(str);
	LineScroll(GetLineCount());
	SetFocus();
	SetSel(str.GetLength(), -1);
	m_TextdwLen = str.GetLength() + 1;
}
void CCmdShellEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE ); 
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CCmdShellEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE ); 
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}
