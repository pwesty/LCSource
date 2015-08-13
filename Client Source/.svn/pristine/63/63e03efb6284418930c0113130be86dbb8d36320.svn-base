#if !defined(AFX_DLGCFORCE_H__99DA6777_CF94_4E85_9659_5309967A31AC__INCLUDED_)
#define AFX_DLGCFORCE_H__99DA6777_CF94_4E85_9659_5309967A31AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCForce.h : header file
//
#include <Engine/Effect/CForce.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCForce dialog

class CDlgCForce : public CDialog
{
// Construction
public:
	CDlgCForce(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCForce)
	enum { IDD = IDD_FORCE };
	CComboBox	m_cbType;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fPower;
	int		m_iType;
	//}}AFX_DATA

	void SetPropertyTo(CForce &force);
	void SetPropertyFrom(CForce &force);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCForce)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCForce)
	afx_msg void OnForceNormalize();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFORCE_H__99DA6777_CF94_4E85_9659_5309967A31AC__INCLUDED_)
