#if !defined(AFX_CREATEWATERDIALOG_H__50160B27_E9EA_4F11_BCD2_A04AA11DE03C__INCLUDED_)
#define AFX_CREATEWATERDIALOG_H__50160B27_E9EA_4F11_BCD2_A04AA11DE03C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateWaterDialog.h : header file
#include "resource.h"
//

/////////////////////////////////////////////////////////////////////////////
// CCreateWaterDialog dialog

class CCreateWaterDialog : public CDialog
{
// Construction
public:
	CCreateWaterDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateWaterDialog)
	enum { IDD = IDD_CREATE_WATER };
	float	m_fWaterHeight;
	float	m_fTileSize;
	CString	m_strFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateWaterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateWaterDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnFindFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEWATERDIALOG_H__50160B27_E9EA_4F11_BCD2_A04AA11DE03C__INCLUDED_)
