#include "stdafx.h"
#include ".\threadbase.h"





struct tagNewThreadInfo
{
	CThreadBase* pThis;
	LPVOID lpParameter;
	DWORD dwThreadId;
	DWORD dwCreateMarker;
};
CThreadBase::CThreadBase(void)
{
	m_dwCreateThreadIdBase = 0;
	m_ThreadInfo.clear();
}

CThreadBase::~CThreadBase(void)
{
	for(DWORD i = 0; i < m_ThreadInfo.size(); i++)
	{
		SetThreadStop(m_ThreadInfo[i].dwThreadId);
		WaitForSingleObject(m_ThreadInfo[i].hEventStop, INFINITE);
		CloseHandle(m_ThreadInfo[i].hEventStop);
		m_ThreadInfo[i].hEventStop = NULL;
	}
}

DWORD WINAPI NewThread(LPVOID p)
{
	tagNewThreadInfo* phThis = (tagNewThreadInfo*)p;
	tagNewThreadInfo hThis;
	hThis.dwCreateMarker = phThis->dwCreateMarker;
	hThis.dwThreadId = phThis->dwThreadId;
	hThis.lpParameter = phThis->lpParameter;
	hThis.pThis = phThis->pThis;
	delete phThis;
	hThis.pThis->OnNewThread(hThis.dwCreateMarker, hThis.dwThreadId, hThis.lpParameter);
	return 0;
}
HANDLE CThreadBase::NewThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId, LPDWORD lpThisThreadId, DWORD dwCreateMarker)
{
	CMySingleLock Event(&m_Event);
	m_dwCreateThreadIdBase++;
	tagNewThreadInfo* pInfo = new tagNewThreadInfo;
	pInfo->lpParameter = lpParameter;
	pInfo->pThis = this;
	pInfo->dwThreadId = m_dwCreateThreadIdBase;
	pInfo->dwCreateMarker = dwCreateMarker;

	HANDLE hRet = CreateThread(lpThreadAttributes, dwStackSize, ::NewThread, pInfo, dwCreationFlags, lpThreadId);
	if( lpThisThreadId )
		*lpThisThreadId = m_dwCreateThreadIdBase;
	if(!hRet)
	{
		delete pInfo;
		
	}
	else
	{
		tagThreadInfo ThreadInfo;
		ThreadInfo.hEventStop = CreateEvent(NULL, FALSE, FALSE, NULL);
		ThreadInfo.dwThreadId = m_dwCreateThreadIdBase;
		ThreadInfo.hThread = hRet;
		m_ThreadInfo.push_back(ThreadInfo);
	}

	return hRet;
}
HANDLE CThreadBase::NewThread(LPVOID lpParameter ,LPDWORD lpThreadId , LPDWORD lpThisThreadId , DWORD dwCreateMarker)
{
	return NewThread(NULL, 0, lpParameter, 0, lpThreadId, lpThisThreadId, dwCreateMarker);
}

bool CThreadBase::IsStop(DWORD dwThreadId,  DWORD dwTime)
{
	CMySingleLock Event(&m_Event);
	for(DWORD i = 0; i < m_ThreadInfo.size(); i++)
	{
		if(dwThreadId == m_ThreadInfo[i].dwThreadId)
		{
			DWORD dwSTime = GetTickCount();
			while(true)
			{
				if((GetTickCount() - dwSTime) > dwTime)
					return false;
				if( WaitForSingleObject(m_ThreadInfo[i].hEventStop, 1) == WAIT_TIMEOUT)
					;
				else
					return true;
			}

		}
	}
	return false;
}
bool CThreadBase::SetThreadStop(DWORD dwThread)
{
	CMySingleLock Event(&m_Event);
	for(DWORD i = 0; i < m_ThreadInfo.size(); i++)
	{
		if(dwThread == m_ThreadInfo[i].dwThreadId)
		{
			tagThreadInfo* pThreadInfo = &m_ThreadInfo[i];
			if( SetEvent(m_ThreadInfo[i].hEventStop))
			{
				return true;
			}
			else
				return false;
		}
	}
	return false;
}