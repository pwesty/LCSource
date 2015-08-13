#if !defined(AFX_DLGCSKAEFFECTPROPERTY_H__2AF5EF25_AA35_4B25_BDD2_8D3D8C14EFFB__INCLUDED_)
#define AFX_DLGCSKAEFFECTPROPERTY_H__2AF5EF25_AA35_4B25_BDD2_8D3D8C14EFFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSkaEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSkaEffectProperty dialog
#include "DlgCEffectControlProperty.h"

class CDlgCSkaEffectProperty : public CDialog
{
// Construction
public:
	CDlgCSkaEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCSkaEffectProperty)
	enum { IDD = IDD_CSKAEFFECT_PROPERTY };
	CComboBox	m_cbPositionType;
	CComboBox	m_cbRotationType;
	CColoredButton	m_btnColor;
	CString	m_strSkaFileName;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	BOOL	m_bEnableEffectControl;
	//}}AFX_DATA

	CDlgCEffectControlProperty	m_dlgEffectControl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSkaEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCSkaEffectProperty)
	afx_msg void OnBtnSkaFilefind();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkEffectControl();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSKAEFFECTPROPERTY_H__2AF5EF25_AA35_4B25_BDD2_8D3D8C14EFFB__INCLUDED_)
