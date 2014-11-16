#pragma once
#include "cmdshelledit.h"
#include ".\servershellhandle.h"

// CCmdShellEdit

class CCmdShellEdit : public CEdit
{
	DECLARE_DYNAMIC(CCmdShellEdit)

public:
	void AddText(LPCWSTR szText);
	CCmdShellEdit();
	virtual ~CCmdShellEdit();
	void BindSocket(CServerShellHandle* h)
	{
		m_hServer = h;
	}
protected:
	CServerShellHandle* m_hServer;
	int m_TextdwLen;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


