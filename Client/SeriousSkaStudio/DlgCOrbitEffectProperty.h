#if !defined(AFX_DLGCORBITEFFECTPROPERTY_H__8FD8BFD7_8D4E_4A7A_885F_83A07321F6A2__INCLUDED_)
#define AFX_DLGCORBITEFFECTPROPERTY_H__8FD8BFD7_8D4E_4A7A_885F_83A07321F6A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgCOrbitEffectProperty.h : header file
//

#include <Engine/Effect/COrbitEffect.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCOrbitEffectProperty dialog

class CDlgCOrbitEffectProperty : public CDialog
{
// Construction
public:
	CDlgCOrbitEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCOrbitEffectProperty)
	enum { IDD = IDD_CORBITEFFECT_PROPERTY };
	CComboBox	m_cbRotationType;
	CComboBox	m_cbPositionType;
	CListBox	m_lbFadeOutFlag;
	CListBox	m_lbFadeInFlag;
	CComboBox	m_comboEffectList;
	CComboBox	m_comboOrbitList;
	float	m_fPhaseMulX;
	float	m_fPhaseMulY;
	float	m_fPhaseMulZ;
	float	m_fPhaseOffsetX;
	float	m_fPhaseOffsetY;
	float	m_fPhaseOffsetZ;
	float	m_fRadiusX;
	float	m_fRadiusY;
	float	m_fRadiusZ;
	float	m_fTimeBeforeStart;
	float	m_fFadeInDestRadiusMul;
	float	m_fFadeInDestPhaseMul;
	float	m_fFadeOutDestRadiusMul;
	float	m_fFadeOutDestPhaseMul;
	CString	m_strGoalTag;
	float	m_fOffsetX;
	float	m_fOffsetY;
	float	m_fOffsetZ;
	//}}AFX_DATA

	COrbitEffect m_orbitEffect;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCOrbitEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCOrbitEffectProperty)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnSelendokComboOrbitList();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnModify();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCORBITEFFECTPROPERTY_H__8FD8BFD7_8D4E_4A7A_885F_83A07321F6A2__INCLUDED_)
