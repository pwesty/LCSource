#if !defined(AFX_DLGCPECYLINDERPROPERTY_H__D6D344D7_8DEF_4306_9C22_5169776F3D71__INCLUDED_)
#define AFX_DLGCPECYLINDERPROPERTY_H__D6D344D7_8DEF_4306_9C22_5169776F3D71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPECylinderProperty.h : header file
//
#include <Engine/Effect/CCylinderSpace.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCPECylinderProperty dialog

class CDlgCPECylinderProperty : public CDialog
{
// Construction
public:
	CDlgCPECylinderProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

// Dialog Data
	//{{AFX_DATA(CDlgCPECylinderProperty)
	enum { IDD = IDD_CPE_CYLINDER_PROPERTY };
	BOOL	m_bUsePos;
	BOOL	m_bUseSpeed;
	float	m_fSpeedLower;
	float	m_fSpeedUpper;
	BOOL	m_bEmitAllDir;
	//}}AFX_DATA

	CCylinderDoubleSpace	m_cylinder;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPECylinderProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPECylinderProperty)
	afx_msg void OnPecylBtnCylinder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPECYLINDERPROPERTY_H__D6D344D7_8DEF_4306_9C22_5169776F3D71__INCLUDED_)
