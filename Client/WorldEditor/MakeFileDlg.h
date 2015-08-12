#if !defined(AFX_MAKEFILEDLG_H__9A42B17F_E728_49BB_B283_A0232D1EF75D__INCLUDED_)
#define AFX_MAKEFILEDLG_H__9A42B17F_E728_49BB_B283_A0232D1EF75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeFileDlg dialog

class CMakeFileDlg : public CDialog
{
// Construction
public:
	CMakeFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeFileDlg)
	enum { IDD = IDD_MAKEFILE };
	CComboBox	m_ctlZoneNo;
	CComboBox	m_ctlLang;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void			LoadShopData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKEFILEDLG_H__9A42B17F_E728_49BB_B283_A0232D1EF75D__INCLUDED_)
