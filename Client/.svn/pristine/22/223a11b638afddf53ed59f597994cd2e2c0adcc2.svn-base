#if !defined(AFX_DLGCPARTICLEGROUPPROPERTY_H__07699CC2_6B15_4175_BB9E_ACBD2B2BE6B8__INCLUDED_)
#define AFX_DLGCPARTICLEGROUPPROPERTY_H__07699CC2_6B15_4175_BB9E_ACBD2B2BE6B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCParticleGroupProperty.h : header file
//

#include "DlgCPEProperty.h"
#include "DlgCPAProperty.h"
#include "DlgCPPProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleGroupProperty dialog

class CDlgCParticleGroupProperty : public CDialog
{
// Construction
public:
	CDlgCParticleGroupProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void Refresh();

// Dialog Data
	//{{AFX_DATA(CDlgCParticleGroupProperty)
	enum { IDD = IDD_CPARTICLEGROUP_PROPERTY };
	CTabCtrl	m_tcComponent;
	CComboBox	m_cbRegisteredName;
	CComboBox	m_cbRenderType;
	CComboBox	m_cbBlendType;
	CString	m_strTexFileName;
	CString	m_strName;
	UINT	m_uiMexWidth;
	UINT	m_uiMexHeight;
	UINT	m_uiColumn;
	UINT	m_uiRow;
	//}}AFX_DATA
	
	CString m_strParticleGroupNameOld;

	CDlgCPEProperty m_dlgCPEProperty;
	CDlgCPAProperty m_dlgCPAProperty;
	CDlgCPPProperty m_dlgCPPProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCParticleGroupProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCParticleGroupProperty)
	afx_msg void OnSelChangeTabPgComponent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnDel();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnPgTexFilefind();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkPgRegisteredName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPARTICLEGROUPPROPERTY_H__07699CC2_6B15_4175_BB9E_ACBD2B2BE6B8__INCLUDED_)
