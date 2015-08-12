#if !defined(AFX_DLGCSHOCKWAVEEFFECTPROPERTY_H__A99B7593_E7CB_4576_B5C3_D99594B7A156__INCLUDED_)
#define AFX_DLGCSHOCKWAVEEFFECTPROPERTY_H__A99B7593_E7CB_4576_B5C3_D99594B7A156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCShockWaveEffectProperty.h : header file
//
#include <Engine/Effect/CSampleSpline.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgCShockWaveEffectProperty dialog

class CDlgCShockWaveEffectProperty : public CDialog
{
// Construction
public:
	CDlgCShockWaveEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCShockWaveEffectProperty)
	enum { IDD = IDD_CSHOCKWAVEEFFECT_PROPERTY };
	CComboBox	m_cbBlendType;
	CComboBox	m_comboRotationType;
	CComboBox	m_comboPositionType;
	CListBox	m_listWidth;
	CListBox	m_listRadius;
	CListBox	m_listAngle;
	BOOL	m_bOuterBasis;
	float	m_fAngleValue;
	float	m_fRadiusValue;
	CString	m_strFileName;
	float	m_fAngleTime;
	float	m_fRadiusTime;
	float	m_fWidthTime;
	UINT	m_uiSplitCount;
	float	m_fWidthValue;
	//}}AFX_DATA

	CSampleSpline<FLOAT> m_ssRadius;
	CSampleSpline<FLOAT> m_ssWidth;
	CSampleSpline<FLOAT> m_ssAngle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCShockWaveEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCShockWaveEffectProperty)
	afx_msg void OnShockwaveAngleadd();
	afx_msg void OnShockwaveAngledelete();
	afx_msg void OnShockwaveAnglemodify();
	afx_msg void OnSelchangeShockwaveAnglelist();
	afx_msg void OnShockwaveFilefind();
	afx_msg void OnShockwaveRadiusadd();
	afx_msg void OnShockwaveRadiusdelete();
	afx_msg void OnShockwaveRadiusmodify();
	afx_msg void OnSelchangeShockwaveRadiuslist();
	afx_msg void OnShockwaveWidthadd();
	afx_msg void OnShockwaveWidthdelete();
	afx_msg void OnSelchangeShockwaveWidthlist();
	afx_msg void OnShockwaveWidthmodify();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSHOCKWAVEEFFECTPROPERTY_H__A99B7593_E7CB_4576_B5C3_D99594B7A156__INCLUDED_)
