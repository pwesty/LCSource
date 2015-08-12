#if !defined(AFX_DLGCPPDYNAMICSTATEPROPERTY_H__F1323239_352A_4492_ABEB_D609CE41E513__INCLUDED_)
#define AFX_DLGCPPDYNAMICSTATEPROPERTY_H__F1323239_352A_4492_ABEB_D609CE41E513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPDynamicStateProperty.h : header file
//
#include <Engine/EFfect/CParticlesProcessDynamicState.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDynamicStateProperty dialog

class CDlgCPPDynamicStateProperty : public CDialog
{
// Construction
public:
	CDlgCPPDynamicStateProperty(CWnd* pParent = NULL);   // standard constructor

	const CParticlesProcessDynamicState &GetParticlesProcess()
	{
		UpdateData(SAVETOOBJECT);
		m_ppdstate.SetFadeTime(m_fFadeInTime, m_fFadeOutTime);
		m_ppdstate.SetTwinklePeriod(m_fTwinklePeriod);
		if(m_ppdstate.GetColorCount()) m_ppdstate.EnableDynamicColor();
		else m_ppdstate.DisableDynamicColor();
		if(m_ppdstate.GetAlphaCount()) m_ppdstate.EnableDynamicAlpha();
		else m_ppdstate.DisableDynamicAlpha();
		if(m_ppdstate.GetTexPosCount()) m_ppdstate.EnableDynamicTexPos();
		else m_ppdstate.DisableDynamicTexPos();
		if(m_ppdstate.GetSizeCount()) m_ppdstate.EnableDynamicSize();
		else m_ppdstate.DisableDynamicSize();
		if(m_ppdstate.GetMassCount()) m_ppdstate.EnableDynamicMass();
		else m_ppdstate.DisableDynamicMass();
		if(m_ppdstate.GetDeltaPosCount()) m_ppdstate.EnableDynamicDeltaPos();
		else m_ppdstate.DisableDynamicDeltaPos();
		if(m_ppdstate.GetAngleCount()) m_ppdstate.EnableDynamicAngle();
		else m_ppdstate.DisableDynamicAngle();
		return m_ppdstate;
	}
	void SetParticlesProcess(CParticlesProcessDynamicState &ppds)
	{
		m_ppdstate = ppds;
		m_fTwinklePeriod = ppds.GetTwinklePeriod();
		m_fFadeInTime = ppds.GetFadeInTime();
		m_fFadeOutTime = ppds.GetFadeOutTime();

		UpdateData(SAVETOFORM);
		Refresh(m_tcStateComponent.GetCurSel());
	}
	void Clear();
	void Refresh(int index);

// Dialog Data
	//{{AFX_DATA(CDlgCPPDynamicStateProperty)
	enum { IDD = IDD_CPP_DYNAMICSTATE_PROPERTY };
	CEdit	m_edValue3;
	CStatic	m_stDataName;
	CEdit	m_edValue2;
	CEdit	m_edValue1;
	CColoredButton	m_btnColor;
	CTabCtrl	m_tcStateComponent;
	CListBox	m_lbAddedState;
	float	m_fFadeInTime;
	float	m_fFadeOutTime;
	float	m_fValue1;
	float	m_fValue2;
	float	m_fAddTime;
	float	m_fValue3;
	float	m_fTwinklePeriod;
	//}}AFX_DATA

	CParticlesProcessDynamicState m_ppdstate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPDynamicStateProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPDynamicStateProperty)
	afx_msg void OnSelChangeTabStateComponent(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnModify();
	afx_msg void OnBtnDel();
	afx_msg void OnSelChangeListAddedState();
	afx_msg void OnBtnAutosetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPDYNAMICSTATEPROPERTY_H__F1323239_352A_4492_ABEB_D609CE41E513__INCLUDED_)
