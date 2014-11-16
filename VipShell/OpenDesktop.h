#pragma once
#include <Windows.h>
#include <tchar.h>
class COpenDesktop
{
public:
	COpenDesktop();
	COpenDesktop(LPCWSTR szName);
	~COpenDesktop(void);
public:
	BOOL _OpenDesktop(LPCWSTR szName = NULL);
	BOOL _CloseDesktop();
protected:

	HDESK   m_hdeskCurrent;
    HDESK   m_hdesk;
    HWINSTA m_hwinstaCurrent;
    HWINSTA m_hwinsta;

};
