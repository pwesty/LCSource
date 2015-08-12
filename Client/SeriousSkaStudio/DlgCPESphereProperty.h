#if !defined(AFX_DLGCPESPHEREPROPERTY_H__50272F6D_3147_48E2_93DF_959299A01C83__INCLUDED_)
#define AFX_DLGCPESPHEREPROPERTY_H__50272F6D_3147_48E2_93DF_959299A01C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPESphereProperty.h : header file
//
#include <Engine/Effect/CForce.h>
#include <Engine/Effect/CSphereSpace.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCPESphereProperty dialog

class CDlgCPESphereProperty : public CDialog
{
// Construction
public:
	CDlgCPESphereProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

// Dialog Data
	//{{AFX_DATA(CDlgCPESphereProperty)
	enum { IDD = IDD_CPE_SPHERE_PROPERTY };
	float	m_fDelayTime;
	//}}AFX_DATA

	CForce m_force;
	CSphereDoubleSpace m_sphere;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPESphereProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPESphereProperty)
	afx_msg void OnBtnForce();
	afx_msg void OnBtnSphere();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPESPHEREPROPERTY_H__50272F6D_3147_48E2_93DF_959299A01C83__INCLUDED_)
