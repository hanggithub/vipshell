#pragma once


struct CDllModuleControlInterface
{
	virtual bool StartServer() = 0;
};
extern "C" __declspec(dllexport) bool StartServer(LPVOID lp);
extern "C" __declspec(dllexport) bool WaitServer(LPVOID lp);
extern "C" __declspec(dllexport) bool DownCtrlAltDel(LPVOID lp);
extern "C" __declspec(dllexport) bool GetDllModuleControl(CDllModuleControlInterface** p);   
