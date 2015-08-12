#if !defined(AFX_DLGCCAMERAEFFECTPROPERTY_H__DCD3CB1A_DCB2_4669_B061_4D35A2883CAE__INCLUDED_)
#define AFX_DLGCCAMERAEFFECTPROPERTY_H__DCD3CB1A_DCB2_4669_B061_4D35A2883CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCCameraEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCCameraEffectProperty dialog
#include <Engine/Effect/CSampleSpline.h>
#include <Engine/Effect/CCameraEffect.h>

class CDlgCCameraEffectProperty : public CDialog
{
// Construction
public:
	CDlgCCameraEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCCameraEffectProperty)
	enum { IDD = IDD_CCAMERAEFFECT_PROPERTY };
	CListBox	m_lbCameraShake;
	float	m_fHeight;
	float	m_fTwist;
	float	m_fTime;
	float	m_fWidth;
	float	m_fZoom;
	float	m_fMoveX;
	float	m_fMoveY;
	float	m_fMoveZ;
	//}}AFX_DATA

	CSampleSpline<CCameraEffect::CCameraValue> m_ssCameraShake;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCCameraEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCCameraEffectProperty)
	afx_msg void OnBtnListAddmod();
	afx_msg void OnBtnListDel();
	afx_msg void OnSelchangeList();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCCAMERAEFFECTPROPERTY_H__DCD3CB1A_DCB2_4669_B061_4D35A2883CAE__INCLUDED_)
