#include "stdafx.h"
#include "IDtest.h"
#include <windows.h>
#include "md5.h"
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

CString _getWmiInfo(IWbemClassObject *pClassObject, LPCTSTR lpszName)
{
	CComVariant varValue ;
	CComBSTR bstrName(lpszName);
	CString str ;
	if( pClassObject->Get( bstrName , 0 , &varValue , NULL , 0 ) == S_OK )
	{
		// 对不同的数据类型分别处理
		if( varValue.vt == VT_BSTR )	//字符串
		{
			str = CString(varValue.bstrVal) ;
		}
		else 
			if( varValue.vt == VT_ARRAY ) //数组
			{
				long iLowBound = 0 , iUpBound = 0 ;
				SafeArrayGetLBound( varValue.parray , 1 , &iLowBound ) ;
				SafeArrayGetUBound( varValue.parray , 1 , &iUpBound ) ;
				for( long j = iLowBound ; j <= iUpBound ; j ++ )
				{
					VARIANT *pvar = NULL ;
					long temp = j ;
					if( SafeArrayGetElement( varValue.parray , &temp , pvar ) == S_OK && pvar )
					{
						CComVariant varTemp ;
						if( varTemp.ChangeType( VT_BSTR , pvar ) == S_OK )
						{
							if( !str.IsEmpty() )
								str += _T(",") ;
							str += varTemp.bstrVal ;
						} //if( varTemp...
					} //if( SafeArrayGet...
				} //for( long j=iLowBound;...
			} //if (varValue.vt ...
			else
			{
				switch( varValue.vt)
				{
				case VT_I4:
					str.Format("%d", varValue.lVal);
					break;
				case VT_I8:
					str.Format("%l", varValue.llVal);
					break;
				case VT_I2:
					str.Format("%d", varValue.iVal);
					break;
				case VT_UI1:
					str.Format("%uc", varValue.bVal);
					break;
				case VT_R4:
					str.Format("%f", varValue.fltVal);
					break;
				case VT_R8:
					str.Format("%lf", varValue.dblVal);
					break;
				case VT_I1:
					str.Format("%c", varValue.cVal);
					break;
				case VT_UI2:
					str.Format("%ud", varValue.uiVal);
					break;
				case VT_UI4:
					str.Format("%ud", varValue.ulVal);
					break;
				case VT_UI8:
					str.Format("%ul", varValue.ullVal);
					break;
				case VT_BOOL:
					cout<<"bool***************\n";
					break;
				case VT_NULL:
					str = "NULL";
					break;
				default:
					if( varValue.ChangeType( VT_BSTR ) == S_OK )
						str = varValue.bstrVal ;
					else
						cout<<"Cannot handle the the data type of "<<CString(bstrName)<<": "<<varValue.vt<<endl;
				}
			}

			// 打印出硬件信息
			//cout<<CString(bstrName)<<" = "<<str<<endl;
	}
	else
		str = "";

	return str;
}

BOOL bCoInitializeSecurityCalled = FALSE; 
CString GetSeiralNumberOrID( LPCTSTR lpszClass, LPCTSTR lpszName)
{
	CString str("");
	// 初始化COM模块
	CoUninitialize();
	HRESULT hres = CoInitializeEx( NULL, COINIT_MULTITHREADED );
	if ( hres != S_OK)
		return str;	

	// 获取访问 WMI 权限
	if (!bCoInitializeSecurityCalled)
	{
		hres = CoInitializeSecurity( 
			NULL, 
			-1, 
			NULL, 
			NULL, 
			RPC_C_AUTHN_LEVEL_DEFAULT, 
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL, 
			EOAC_NONE, 
			0 );

		if (hres  == S_OK)
			bCoInitializeSecurityCalled = TRUE;
	}
	if(bCoInitializeSecurityCalled)
	{
		// 通过 IWbemLocator 和 IWbemServices 这两个 COM 接口访问 WMI, 获取系统信息

		CComPtr<IWbemLocator> spWbemLocator ;
		if( spWbemLocator.CoCreateInstance( CLSID_WbemAdministrativeLocator , 0 , CLSCTX_INPROC_SERVER| CLSCTX_LOCAL_SERVER ) == S_OK )
		{
			CComPtr<IWbemServices> spWbemServices ;
			if( spWbemLocator->ConnectServer( L"root\\cimv2" , NULL, NULL, NULL, 0, NULL, NULL, &spWbemServices ) == S_OK )
			{
				// 检索指定信息

				USES_CONVERSION ;
				CComPtr<IEnumWbemClassObject> spEnumWbemClassObject ;
				CComBSTR bstrQuery ( L"Select * from " ) ;
				bstrQuery += T2OLE(lpszClass) ;
				if( spWbemServices->ExecQuery( L"WQL" , bstrQuery , WBEM_FLAG_RETURN_IMMEDIATELY , NULL, &spEnumWbemClassObject ) == S_OK )
				{
					ULONG uCount = 1, uReturned;
					CComPtr<IWbemClassObject> spClassObject ;
					if( spEnumWbemClassObject->Reset()  == S_OK )
					{
						int iEnumIdx = 0;
						while( spEnumWbemClassObject->Next( WBEM_INFINITE,uCount, &spClassObject, &uReturned) == S_OK )
						{
							str = _getWmiInfo( spClassObject, lpszName) ;
							spClassObject.Release() ;
						}
					}
				}
				spWbemServices.Release();
			}
			spWbemLocator.Release();
		}
	}

	CoUninitialize();

	return str;
}



char * CreateKey(char *pSRC)
{
	return MD5String(pSRC);
}

bool ID_test(const char* license)
{
	CString SystemModel = GetSeiralNumberOrID("Win32_ComputerSystem", "Model");
	CString BoardID = GetSeiralNumberOrID("Win32_BaseBoard", "SerialNumber");
    CString ProcessorID = GetSeiralNumberOrID("Win32_Processor", "ProcessorID");
	cout << "SystemModel: " << SystemModel << endl; //机型
	cout << "BoardID: " << BoardID << endl;	// 底板
	cout << "ProcessorID: " << ProcessorID << endl;	// CPU
	if (SystemModel.Trim() == "")
		SystemModel = "DefaultModel";
	if (BoardID.Trim() == "")
		BoardID = "UnidentifiedBoard";
	
	if (ProcessorID.Trim() != "")
	{
		CString MachineID = SystemModel + BoardID + ProcessorID; 
		char *ID_CODE = CreateKey(MachineID.GetBuffer());
		ID_CODE[10] = '\0';
		char *LICENSE_KEY = CreateKey(ID_CODE);
		bool ret = (strcmp(LICENSE_KEY,license) == 0);
		if (!ret)
		{
			//TODO: save ID_CODE to desktop\MyUniverseKey.dat
			TCHAR path[MAX_PATH] = {0};

			BOOL ok = SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOP, TRUE);
			PathAppend(path, _T("MyUniverseKey.dat"));
			
			FILE* file = fopen(path, _T("w"));	
			fwrite( ID_CODE, sizeof( char ), strlen(ID_CODE), file );
			fclose(file);
		}
		delete ID_CODE;
		delete LICENSE_KEY;
		return ret;	
	}
	else
	{
		MessageBox(NULL, "软件无法在此机器上正常运行，CODE：13", "错误", MB_OK|MB_ICONEXCLAMATION);
		exit(0);
	}
}