#if !defined(AFX_DLGCPECONEPROPERTY_H__1EE45D81_60F6_4288_B215_EA12DA82CB3D__INCLUDED_)
#define AFX_DLGCPECONEPROPERTY_H__1EE45D81_60F6_4288_B215_EA12DA82CB3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPEConeProperty.h : header file
//
#include <Engine/Effect/CConeSpace.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCPEConeProperty dialog

class CDlgCPEConeProperty : public CDialog
{
// Construction
public:
	CDlgCPEConeProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

// Dialog Data
	//{{AFX_DATA(CDlgCPEConeProperty)
	enum { IDD = IDD_CPE_CONE_PROPERTY };
	BOOL	m_bUsePos;
	BOOL	m_bUseSpeed;
	float	m_fSpeedLower;
	float	m_fSpeedUpper;
	//}}AFX_DATA

	CConeDoubleSpace	m_cone;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPEConeProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPEConeProperty)
	afx_msg void OnPeconeBtnCone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPECONEPROPERTY_H__1EE45D81_60F6_4288_B215_EA12DA82CB3D__INCLUDED_)
