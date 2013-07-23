// FlashPlayer_vc6.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FlashPlayer_vc6.h"
#include "FlashPlayer_vc6Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlashPlayer_vc6App

BEGIN_MESSAGE_MAP(CFlashPlayer_vc6App, CWinApp)
	//{{AFX_MSG_MAP(CFlashPlayer_vc6App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashPlayer_vc6App construction

CFlashPlayer_vc6App::CFlashPlayer_vc6App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFlashPlayer_vc6App object

CFlashPlayer_vc6App theApp;

/////////////////////////////////////////////////////////////////////////////
// CFlashPlayer_vc6App initialization

BOOL CFlashPlayer_vc6App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFlashPlayer_vc6Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
