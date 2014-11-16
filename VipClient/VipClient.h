// VipClient.h : main header file for the VIPCLIENT application
//

#if !defined(AFX_VIPCLIENT_H__BC81C2A8_BBC8_423B_A6DA_81F7496F0BAD__INCLUDED_)
#define AFX_VIPCLIENT_H__BC81C2A8_BBC8_423B_A6DA_81F7496F0BAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <Winsock2.h>
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CVipClientApp:
// See VipClient.cpp for the implementation of this class
//

class CVipClientApp : public CWinApp
{
public:
	std::vector<SOCKET> m_UserSocketVecTor;
	void AddSocket(SOCKET s)
	{
		m_UserSocketVecTor.push_back(s);
	}
	void ReMoveSocket(SOCKET s)
	{
		for(DWORD i = 0; i < m_UserSocketVecTor.size(); i++)
		{
			if(m_UserSocketVecTor[i] == s)
			{
				m_UserSocketVecTor.erase(m_UserSocketVecTor.begin() + i);
				break;
			}
		}
	}
public:
	CVipClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVipClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVipClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CVipClientApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIPCLIENT_H__BC81C2A8_BBC8_423B_A6DA_81F7496F0BAD__INCLUDED_)
