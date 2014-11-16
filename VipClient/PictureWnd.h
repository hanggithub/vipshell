#pragma once


// CPictureWnd

class CPictureWnd : public CWnd
{
	DECLARE_DYNAMIC(CPictureWnd)

public:
	CPictureWnd();
	virtual ~CPictureWnd();
public:
	void SetBitmap(HBITMAP hBitmap);
protected:
	CBitmap m_hBitmap;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


