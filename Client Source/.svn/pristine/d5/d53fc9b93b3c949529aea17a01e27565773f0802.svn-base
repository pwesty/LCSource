#if !defined(AFX_DLGCSPLINEPATHEFFECTPROPERTY_H__E953E250_CDA1_470F_BA65_C38BE1982767__INCLUDED_)
#define AFX_DLGCSPLINEPATHEFFECTPROPERTY_H__E953E250_CDA1_470F_BA65_C38BE1982767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSplinePathEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplinePathEffectProperty dialog
#include <Engine/Effect/CSplinePathEffect.h>

class CDlgCSplinePathEffectProperty : public CDialog
{
// Construction
public:
	CDlgCSplinePathEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCSplinePathEffectProperty)
	enum { IDD = IDD_CSPLINEPATHEFFECT_PROPERTY };
	CComboBox	m_cbRotationType;
	CComboBox	m_cbPositionType;
	CListBox	m_lbPosList;
	CComboBox	m_cbPathList;
	CComboBox	m_cbEffectList;
	float	m_fOffsetX;
	float	m_fOffsetY;
	float	m_fOffsetZ;
	//}}AFX_DATA

	CSplinePathEffect *m_pSplinePathEffect;
	CSplinePathEffect::sSplinePathInfo m_splinePathInfo;
	BOOL m_bDirectApply;

	void RefreshPosSpline();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSplinePathEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshPosList();
	void SelectPosListRow(INDEX i);
	// Generated message map functions
	//{{AFX_MSG(CDlgCSplinePathEffectProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelendokComboList();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnModify();
	afx_msg void OnSelchangeListPos();
	afx_msg void OnBtnPoslistAdd();
	afx_msg void OnBtnPoslistDel();
	afx_msg void OnCheckDirectapply();
	afx_msg void OnChangeEd();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSPLINEPATHEFFECTPROPERTY_H__E953E250_CDA1_470F_BA65_C38BE1982767__INCLUDED_)
