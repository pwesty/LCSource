#if !defined(AFX_DLGCPEPROPERTY_H__26986F5E_F4B9_448F_A23A_D1F2FF09AFE9__INCLUDED_)
#define AFX_DLGCPEPROPERTY_H__26986F5E_F4B9_448F_A23A_D1F2FF09AFE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPEProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPEProperty dialog
#include "DlgCPESphereProperty.h"
#include "DlgCPEConeProperty.h"
#include "DlgCPECylinderProperty.h"
#include <Engine/Effect/CParticleGroup.h>

class CDlgCPEProperty : public CDialog
{
// Construction
public:
	CDlgCPEProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();

	BOOL SetPropertyTo(CParticleGroup &pg);
	BOOL SetPropertyFrom(CParticleGroup &pg);

// Dialog Data
	//{{AFX_DATA(CDlgCPEProperty)
	enum { IDD = IDD_CPARTICLEEMITTER_PROPERTY };
	CColoredButton	m_btnColor2;
	CColoredButton	m_btnColor1;
	CComboBox	m_cbEmitterType;
	UINT	m_uiMaxCount;
	float	m_fEmitSpeed;
	float	m_fLifeTime1;
	float	m_fLifeTime2;
	float	m_fWidth2;
	float	m_fWidth1;
	float	m_fHeight1;
	float	m_fHeight2;
	float	m_fMass1;
	float	m_fMass2;
	float	m_fAngleX1;
	float	m_fAngleY1;
	float	m_fAngleZ1;
	float	m_fAngleX2;
	float	m_fAngleY2;
	float	m_fAngleZ2;
	BOOL	m_bLocalType;
	//}}AFX_DATA

	CDlgCPESphereProperty	m_dlgCPESphereProperty;
	CDlgCPEConeProperty		m_dlgCPEConeProperty;
	CDlgCPECylinderProperty	m_dlgCPECylinderProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPEProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPEProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkEmitterType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPEPROPERTY_H__26986F5E_F4B9_448F_A23A_D1F2FF09AFE9__INCLUDED_)
