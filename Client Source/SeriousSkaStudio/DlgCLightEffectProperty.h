#if !defined(AFX_DLGCLIGHTEFFECTPROPERTY_H__2E6AB261_8095_4B20_AAFF_FA4B540DE46B__INCLUDED_)
#define AFX_DLGCLIGHTEFFECTPROPERTY_H__2E6AB261_8095_4B20_AAFF_FA4B540DE46B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCLightEffectProperty.h : header file
//
#include "ColoredButton.h"
#include "DlgCEffectProperty.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCLightEffectProperty dialog

class CDlgCLightEffectProperty : public CDialog
{
// Construction
public:
	CDlgCLightEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCLightEffectProperty)
	enum { IDD = IDD_CLIGHTEFFECT_PROPERTY };
	CColoredButton	m_btnColor;
	BOOL	m_bAmbientMode;
	float	m_fFallOff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCLightEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCLightEffectProperty)
	virtual BOOL OnInitDialog();
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCLIGHTEFFECTPROPERTY_H__2E6AB261_8095_4B20_AAFF_FA4B540DE46B__INCLUDED_)
