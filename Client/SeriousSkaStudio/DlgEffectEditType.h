#if !defined(AFX_DLGEFFECTEDITTYPE_H__0991F4EC_E998_44A7_B77E_0AD44BEA69B7__INCLUDED_)
#define AFX_DLGEFFECTEDITTYPE_H__0991F4EC_E998_44A7_B77E_0AD44BEA69B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEffectEditType.h : header file
//
#include "DlgExtensionEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEffectEditType dialog

class CDlgEffectEditType : public CDialog
{
// Construction
public:
	CDlgEffectEditType(CWnd* pParent = NULL);   // standard constructor
	void ExcuteSubEffect();
	
// Dialog Data
	//{{AFX_DATA(CDlgEffectEditType)
	enum { IDD = IDD_EFFECT_EDIT_TYPE };
	CButton	m_btnExtension;
	CButton	m_btnTest;
	CComboBox	m_cbEffectEditType;
	//}}AFX_DATA

	CDlgExtensionEdit m_dlgExtensionEdit;

//Cmd Interface
	void TestEffectSystem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEffectEditType)
	afx_msg void OnBtnTestEffectsystem();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEffectEditType)
	afx_msg void OnSelEndOkCbEffectEditType();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBtnExtension();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEFFECTEDITTYPE_H__0991F4EC_E998_44A7_B77E_0AD44BEA69B7__INCLUDED_)
