#if !defined(AFX_DLGCEFFECTPROPERTY_H__9F70A76C_26A5_4787_A1D9_5FA1ED4B3757__INCLUDED_)
#define AFX_DLGCEFFECTPROPERTY_H__9F70A76C_26A5_4787_A1D9_5FA1ED4B3757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCEffectProperty.h : header file
//

#include <Engine/Effect/CEffect.h>
#include "DlgCLightEffectProperty.h"
#include "DlgCMdlEffectProperty.h"
#include "DlgCSkaEffectProperty.h"
#include "DlgCTerrainEffectProperty.h"
#include "DlgCSoundEffectProperty.h"
#include "DlgCTraceEffectProperty.h"
#include "DlgCParticleEffectProperty.h"
#include "DlgCSplineBillboardEffectProperty.h"
#include "DlgCOrbitEffectProperty.h"
#include "DlgCShockWaveEffectProperty.h"
#include "DlgCSplinePathEffectProperty.h"
#include "DlgCCameraEffectProperty.h"
#include "DlgCEntityEffectProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectProperty dialog
#define WM_CLEAR_FORM			(WM_APP+200)
#define ON_WM_CLEAR_FORM() \
	{ WM_CLEAR_FORM, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnClearForm },
#define WM_REGISTER_EFFECT		(WM_APP+100)
#define ON_WM_REGISTER_EFFECT() \
	{ WM_REGISTER_EFFECT, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM,LPARAM))&OnRegisterEffect },
#define WM_SELECT_EFFECT		(WM_APP+102)
#define ON_WM_SELECT_EFFECT() \
	{ WM_SELECT_EFFECT, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM,LPARAM))&OnSelectEffect },

class CDlgCEffectProperty : public CDialog
{
// Construction
public:
	CDlgCEffectProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void ClearForm();
	void Refresh();

// Dialog Data
	//{{AFX_DATA(CDlgCEffectProperty)
	enum { IDD = IDD_CEFFECT_PROPERTY };
	CComboBox	m_cbEffectType;
	CComboBox	m_cbRegisteredName;
	CString	m_strEffectName;
	float	m_fFadeInTime;
	float	m_fFadeOutTime;
	float	m_fLifeTime;
	float	m_fBoundingSphereRadius;
	long	m_slRepeatCount;
	//}}AFX_DATA

	EFFECT_TYPE m_eEffectType;
	CDialog		*m_pConcreteEffectProperty;
	CDlgCLightEffectProperty			m_dlgCLightEffectProperty;
	CDlgCMdlEffectProperty				m_dlgCMdlEffectProperty;
	CDlgCSkaEffectProperty				m_dlgCSkaEffectProperty;
	CDlgCTerrainEffectProperty			m_dlgCTerrainEffectProperty;
	CDlgCSoundEffectProperty			m_dlgCSoundEffectProperty;
	CDlgCTraceEffectProperty			m_dlgCTraceEffectProperty;
	CDlgCParticleEffectProperty			m_dlgCParticleEffectProperty;
	CDlgCSplineBillboardEffectProperty	m_dlgCSplineBillboardEffectProperty;
	CDlgCOrbitEffectProperty			m_dlgCOrbitEffectProperty;
	CDlgCShockWaveEffectProperty		m_dlgCShockWaveEffectProperty;
	CDlgCSplinePathEffectProperty		m_dlgCSplinePathEffectProperty;
	CDlgCCameraEffectProperty			m_dlgCCameraEffectProperty;
	CDlgCEntityEffectProperty			m_dlgCEntityEffectProperty;
	CString m_strEffectNameOld;

	void RegisterEffect(CEffect &effect);
	void SetEffectInfo(CEffect &effect);
	void UnregisterEffect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCEffectProperty)
	afx_msg void OnBtnSave();
	afx_msg void OnBtnDel();
	afx_msg void OnSelEndOkEeType();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkEeRegisteredName();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCEFFECTPROPERTY_H__9F70A76C_26A5_4787_A1D9_5FA1ED4B3757__INCLUDED_)
