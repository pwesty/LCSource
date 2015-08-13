#if !defined(AFX_DLGRECURSIVEFILEBROWSER_H__733C94E3_4798_49A2_BDDB_3069BE8559E9__INCLUDED_)
#define AFX_DLGRECURSIVEFILEBROWSER_H__733C94E3_4798_49A2_BDDB_3069BE8559E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecursiveFileBrowser.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgRecursiveFileBrowser dialog

class CDlgRecursiveFileBrowser : public CDialog
{
// Construction
public:
	CDlgRecursiveFileBrowser(CWnd* pParent = NULL);   // standard constructor
  void SetMessage(INDEX iControlID, const char *strFormat, ...);
  void Update(void);

// Dialog Data
	//{{AFX_DATA(CDlgRecursiveFileBrowser)
	enum { IDD = IDD_RECURSIVE_FILE_BROWSER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecursiveFileBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecursiveFileBrowser)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECURSIVEFILEBROWSER_H__733C94E3_4798_49A2_BDDB_3069BE8559E9__INCLUDED_)
