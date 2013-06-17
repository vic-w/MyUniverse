// Ktmfc.h : Ktmfc DLL 的主头文件
//

#pragma once

//#ifndef __AFXWIN_H__
//	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
//#endif

#include "resource.h"		// 主符号

#ifdef KT_EXPORTS
#define KT_API __declspec(dllexport)
#else
#define KT_API __declspec(dllimport)
#endif

KT_API bool glbKtMFC(void);//Key test
