#if !defined(AFX_DLGCPARTICLEEFFECTPROPERTY_H__61B5D2FF_3480_449E_8CB9_E6A95E1428E5__INCLUDED_)
#define AFX_DLGCPARTICLEEFFECTPROPERTY_H__61B5D2FF_3480_449E_8CB9_E6A95E1428E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCParticleEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleEffectProperty dialog

class CDlgCParticleEffectProperty : public CDialog
{
// Construction
public:
	CDlgCParticleEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Cmd Interface
	void SelectEffect(WPARAM wp, LPARAM lp);

// Dialog Data
	//{{AFX_DATA(CDlgCParticleEffectProperty)
	enum { IDD = IDD_CPARTICLEEFFECT_PROPERTY };
	CComboBox	m_cbRotationType;
	CComboBox	m_cbRegisteredParticleGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCParticleEffectProperty)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCParticleEffectProperty)
	afx_msg void OnBtnRefresh();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPARTICLEEFFECTPROPERTY_H__61B5D2FF_3480_449E_8CB9_E6A95E1428E5__INCLUDED_)
