#if !defined(AFX_DLGCEFFECTCONTROLPROPERTY_H__4D8AE77A_3B4A_4AEB_8960_1AD1BDA02808__INCLUDED_)
#define AFX_DLGCEFFECTCONTROLPROPERTY_H__4D8AE77A_3B4A_4AEB_8960_1AD1BDA02808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCEffectControlProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectControlProperty dialog
#include <Engine/Effect/CEffectControl.h>

class CDlgCEffectControlProperty : public CDialog
{
// Construction
public:
	CDlgCEffectControlProperty(CWnd* pParent = NULL);   // standard constructor

	const CEffectControl &GetEffectControl()
	{
		UpdateData(SAVETOOBJECT);
		m_effectControl.SetFadeTime(0, 0);
		m_effectControl.SetTwinklePeriod(m_fTwinklePeriod);
		if(m_effectControl.GetColorCount()) m_effectControl.EnableDynamicColor();
		else m_effectControl.DisableDynamicColor();
		if(m_effectControl.GetAlphaCount()) m_effectControl.EnableDynamicAlpha();
		else m_effectControl.DisableDynamicAlpha();
		if(m_effectControl.GetStretchCount()) m_effectControl.EnableDynamicStretch();
		else m_effectControl.DisableDynamicStretch();
		if(m_effectControl.GetPosCount()) m_effectControl.EnableDynamicPos();
		else m_effectControl.DisableDynamicPos();
		if(m_effectControl.GetAngleCount()) m_effectControl.EnableDynamicAngle();
		else m_effectControl.DisableDynamicAngle();
		return m_effectControl;
	}
	void SetEffectControl(CEffectControl &effectControl)
	{
		m_effectControl = effectControl;
		m_fTwinklePeriod = effectControl.GetTwinklePeriod();

		UpdateData(SAVETOFORM);
		Refresh(m_tcStateComponent.GetCurSel());
	}

	void Clear();
	void Refresh(int index);

// Dialog Data
	//{{AFX_DATA(CDlgCEffectControlProperty)
	enum { IDD = IDD_CEFFECTCONTROL_PROPERTY };
	CListBox	m_lbAddedState;
	CStatic	m_stDataName;
	CEdit	m_edValue3;
	CEdit	m_edValue2;
	CEdit	m_edValue1;
	CColoredButton	m_btnColor;
	CTabCtrl	m_tcStateComponent;
	float	m_fAddTime;
	float	m_fTwinklePeriod;
	float	m_fValue3;
	float	m_fValue2;
	float	m_fValue1;
	//}}AFX_DATA

	CEffectControl	m_effectControl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCEffectControlProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshPosSpline(BOOL bShow, SINT selPointIdx);

	// Generated message map functions
	//{{AFX_MSG(CDlgCEffectControlProperty)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnModify();
	afx_msg void OnSelchangeListAddedState();
	afx_msg void OnSelchangeTabStateComponent(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCEFFECTCONTROLPROPERTY_H__4D8AE77A_3B4A_4AEB_8960_1AD1BDA02808__INCLUDED_)
