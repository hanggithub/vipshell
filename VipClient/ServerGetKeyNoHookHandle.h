#pragma once
#include "connectbase.h"

class CServerGetKeyNoHookHandle :
	public CConnectBase
{
public:
	CServerGetKeyNoHookHandle(void);
	~CServerGetKeyNoHookHandle(void);
public:
	bool GetString(WCHAR* szText);
	bool Statr();
};
