// PersistentSymbolTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PersistentSymbolTool.h"
#include "PersistentSymbolToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolApp

BEGIN_MESSAGE_MAP(CPersistentSymbolToolApp, CWinApp)
	//{{AFX_MSG_MAP(CPersistentSymbolToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolApp construction

CPersistentSymbolToolApp::CPersistentSymbolToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPersistentSymbolToolApp object

CPersistentSymbolToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolApp initialization

BOOL CPersistentSymbolToolApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	int argc = __argc;
	CHAR** argv = __argv;
	CFileStatus rStatus;
	if (argc == 3 && CFile::GetStatus(argv[1], rStatus) && rStatus.m_size > 0)
	{
		CPersistentSymbolToolDlg::EncodeFile(argv[1], argv[2]);
		return FALSE;
	}

	CPersistentSymbolToolDlg dlg;
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
