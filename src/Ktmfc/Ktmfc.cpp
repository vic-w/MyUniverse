// Ktmfc.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Ktmfc.h"
#include <wbemidl.h> 
#include <Wbemcli.h>
#pragma comment(lib,"Wbemuuid.lib")   
#include <iostream>
#include "IDtest.h"
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



KT_API bool glbKtMFC(void)
{
    static bool first = 1;
    if(!first)
    {
        return true;
    }
    else
    {
        first = false;
    }

    fstream file;
    file.open("license.dat");
    char BiosID[256];
    file >> BiosID; 

	return ID_test(BiosID);
}