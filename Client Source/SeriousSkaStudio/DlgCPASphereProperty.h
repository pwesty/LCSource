#if !defined(AFX_DLGCPASPHEREPROPERTY_H__D095AE25_F02C_4D9D_B8E8_1D1434AEF300__INCLUDED_)
#define AFX_DLGCPASPHEREPROPERTY_H__D095AE25_F02C_4D9D_B8E8_1D1434AEF300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPASphereProperty.h : header file
//
#include <Engine/Effect/CParticlesAbsorptionSphere.h>
#include <Engine/Effect/CParticleGroup.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgCPASphereProperty dialog

class CDlgCPASphereProperty : public CDialog
{
// Construction
public:
	CDlgCPASphereProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

// Dialog Data
	//{{AFX_DATA(CDlgCPASphereProperty)
	enum { IDD = IDD_CPA_SPHERE_PROPERTY };
	//}}AFX_DATA

	CSphereDoubleSpace m_sphere;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPASphereProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPASphereProperty)
	afx_msg void OnBtnSphere();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPASPHEREPROPERTY_H__D095AE25_F02C_4D9D_B8E8_1D1434AEF300__INCLUDED_)
