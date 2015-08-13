#if !defined(AFX_DLGCPAPROPERTY_H__FC4DF0EB_93F5_43E4_8113_58EBFEF27F75__INCLUDED_)
#define AFX_DLGCPAPROPERTY_H__FC4DF0EB_93F5_43E4_8113_58EBFEF27F75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPAProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPAProperty dialog
#include "DlgCPADefaultProperty.h"
#include "DlgCPASphereProperty.h"

class CDlgCPAProperty : public CDialog
{
// Construction
public:
	CDlgCPAProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

// Dialog Data
	//{{AFX_DATA(CDlgCPAProperty)
	enum { IDD = IDD_CPARTICLEABSORPTION_PROPERTY };
	CComboBox	m_cbParticleMoveType;
	CComboBox	m_cbAbsorptionType;
	BOOL	m_bDependLife;
	//}}AFX_DATA

	BOOL SetPropertyTo(CParticleGroup &pg);
	BOOL SetPropertyFrom(CParticleGroup &pg);

	CDlgCPADefaultProperty m_dlgCPADefaultProperty;
	CDlgCPASphereProperty m_dlgCPASphereProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPAProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPAProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkAbsorptionType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPAPROPERTY_H__FC4DF0EB_93F5_43E4_8113_58EBFEF27F75__INCLUDED_)
