#if !defined(AFX_DLGCMDLEFFECTPROPERTY_H__9897E2BB_87D2_4A0E_B73C_DBD7D8A83C01__INCLUDED_)
#define AFX_DLGCMDLEFFECTPROPERTY_H__9897E2BB_87D2_4A0E_B73C_DBD7D8A83C01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCMdlEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCMdlEffectProperty dialog
#include "DlgCEffectControlProperty.h"

class CDlgCMdlEffectProperty : public CDialog
{
// Construction
public:
	CDlgCMdlEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCMdlEffectProperty)
	enum { IDD = IDD_CMDLEFFECT_PROPERTY };
	CComboBox	m_cbPositionType;
	CComboBox	m_cbRotationType;
	CColoredButton	m_btnColor;
	CString	m_strMdlFileName;
	CString	m_strMdlTexFileName;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	BOOL	m_bEnableEffectControl;
	BOOL	m_bOverDraw;
	//}}AFX_DATA

	CDlgCEffectControlProperty	m_dlgEffectControl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCMdlEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCMdlEffectProperty)
	afx_msg void OnBtnMdlFilefind();
	afx_msg void OnBtnMdlTexFilefind();
	afx_msg void OnChkEffectControl();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCMDLEFFECTPROPERTY_H__9897E2BB_87D2_4A0E_B73C_DBD7D8A83C01__INCLUDED_)
