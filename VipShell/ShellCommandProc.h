#pragma once
#include "defcommandproc.h"

class CShellCommandProc :
	public CDefCommandProc
{
public:
	CShellCommandProc(void);
	~CShellCommandProc(void);
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnCmd_GetShell(tagVipShellCmd* info);
};
