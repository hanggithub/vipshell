// PictureWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "VipClient.h"
#include "PictureWnd.h"
#include ".\picturewnd.h"


// CPictureWnd

IMPLEMENT_DYNAMIC(CPictureWnd, CWnd)
CPictureWnd::CPictureWnd()
{
}

CPictureWnd::~CPictureWnd()
{
}


BEGIN_MESSAGE_MAP(CPictureWnd, CStatic)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CPictureWnd 消息处理程序

void CPictureWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nSBCode != SB_ENDSCROLL)	
	{
		SCROLLINFO vStructure;
		GetScrollInfo(SB_VERT, &vStructure);
		vStructure.fMask = SIF_ALL;
		vStructure.nPos = nPos;
		SetScrollInfo(SB_VERT, &vStructure);
	}
	Invalidate();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPictureWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nSBCode != SB_ENDSCROLL)	
	{
		SCROLLINFO hStructure;
		GetScrollInfo(SB_HORZ, &hStructure);
		hStructure.fMask = SIF_ALL;
		hStructure.nPos = nPos;
		SetScrollInfo(SB_HORZ, &hStructure);
	}
	Invalidate();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPictureWnd::SetBitmap(HBITMAP hBitmap)
{
	if( hBitmap == NULL)
		return ;
	if( m_hBitmap.m_hObject != NULL )
		m_hBitmap.DeleteObject();
	m_hBitmap.Attach(hBitmap);
	Invalidate();
}

void CPictureWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	if( m_hBitmap.m_hObject != NULL)
	{
		CDC hdc ;
		hdc.CreateCompatibleDC(&dc);
		CRect rc;
		GetClientRect(&rc);
		CBitmap* hOld = hdc.SelectObject(&m_hBitmap); 


		BITMAP              bitmap ;
		m_hBitmap.GetBitmap(&bitmap);

		SCROLLINFO hStructure, vStructure;
		GetScrollInfo(SB_HORZ, &hStructure);
		GetScrollInfo(SB_VERT, &vStructure);
		DWORD dwX = (int)bitmap.bmWidth - rc.right;
		if(dwX > (DWORD)hStructure.nPos)
			dwX = (DWORD)hStructure.nPos;
		DWORD dwY = (DWORD)bitmap.bmHeight - rc.bottom;
		if(dwY > (DWORD)vStructure.nPos)
			dwY = (DWORD)vStructure.nPos;		

		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &hdc, dwX, dwY, SRCCOPY);
		dc.SelectObject(hOld);

		hStructure.fMask = SIF_ALL;
		hStructure.nMin = 0;
		hStructure.nMax = bitmap.bmWidth ;
		hStructure.nPage = rc.right * rc.right / hStructure.nMax ;
		SetScrollInfo(SB_HORZ, &hStructure);

		vStructure.fMask = SIF_ALL;
		vStructure.nMin = 0;
		vStructure.nMax = bitmap.bmHeight ;
		vStructure.nPage = rc.bottom * rc.bottom / vStructure.nMax;
		SetScrollInfo(SB_VERT, &vStructure);
	}
}



void CPictureWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPictureWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
