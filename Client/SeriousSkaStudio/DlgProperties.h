#if !defined(AFX_DLGPROPERTIES_H__9073CE25_D67E_43D0_B2C8_482C6C681BAF__INCLUDED_)
#define AFX_DLGPROPERTIES_H__9073CE25_D67E_43D0_B2C8_482C6C681BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties dialog

class CDlgProperties : public CDialog
{
// Construction
public:
	CDlgProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProperties)
	enum { IDD = IDD_PROPERTIES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProperties)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTIES_H__9073CE25_D67E_43D0_B2C8_482C6C681BAF__INCLUDED_)
