#include "stdafx.h"
#include ".\serverconnectmanage.h"

CServerConnectManage::CServerConnectManage(void)
{
	m_hServer = NULL;
	NewThread(NULL, NULL, NULL, 0);    //启动 判断是否有用户 已经连接进来
}

CServerConnectManage::~CServerConnectManage(void)
{

}

void CServerConnectManage::SetBindServer(CVipShellClientServer* server)
{
	m_hServer = server;
}

void CServerConnectManage::OnNewThread(DWORD dwCreateMarker, DWORD dwThreadId, LPVOID lpParameter)
{
	while(true)
	{
		if ( IsStop(dwThreadId, 100) )
			return ;
		if(m_hServer != NULL)
		{
			CServerConnectHandle handle;
			if( m_hServer->GetConnectHandle(&handle) )
			{
				
			}
		}
	}
}