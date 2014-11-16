#pragma once
#include <windows.h>
#include <vector>

class CMyCriticalSection
{
public:
	CMyCriticalSection()
	{
		m_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);    
	}
	~CMyCriticalSection()
	{
		CloseHandle(m_hEvent);
	}
	operator HANDLE()
	{
		return m_hEvent;
	}
protected:
	HANDLE m_hEvent;
private:
};

class CMySingleLock
{
public:
	CMySingleLock(CMyCriticalSection* InEvent)
	{
		m_Event = (HANDLE)*InEvent;
//		WaitForSingleObject(m_Event, INFINITE);
		if( WaitForSingleObject(m_Event, 3*1000) == WAIT_TIMEOUT)
		{
			MessageBoxA(NULL, "ÐÅºÅ³¬Ê±", "NULL", MB_OK);
			int i;
			i = 0;
		}
		
	}
	~CMySingleLock()
	{
		SetEvent(m_Event);
	}
protected:
	HANDLE m_Event;
private:
};

class CThreadBase
{
public:
	CThreadBase(void);
	~CThreadBase(void);
private:
	struct tagThreadInfo
	{
		HANDLE hThread;
		HANDLE hEventStop;
		DWORD dwThreadId;
	};
	DWORD m_dwCreateThreadIdBase;
	std::vector<tagThreadInfo> m_ThreadInfo;
	CMyCriticalSection m_Event;
public:
	virtual void OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter) = 0;
	bool IsStop(DWORD dwThreadId, DWORD dwTime);
	bool SetThreadStop(DWORD dwThread);
	HANDLE NewThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId, LPDWORD lpThisThreadId, DWORD dwCreateMarker);
	HANDLE NewThread(LPVOID lpParameter = NULL,LPDWORD lpThreadId = NULL, LPDWORD lpThisThreadId = NULL, DWORD dwCreateMarker =0);
protected:
};
