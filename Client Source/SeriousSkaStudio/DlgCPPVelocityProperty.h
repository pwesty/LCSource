#if !defined(AFX_DLGCPPVELOCITYPROPERTY_H__35D900A0_2BAA_4856_BF1A_2C3A93325F34__INCLUDED_)
#define AFX_DLGCPPVELOCITYPROPERTY_H__35D900A0_2BAA_4856_BF1A_2C3A93325F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPVelocityProperty.h : header file
//
#include <Engine/Effect/CParticlesProcessVelocity.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPVelocityProperty dialog

class CDlgCPPVelocityProperty : public CDialog
{
// Construction
public:
	CDlgCPPVelocityProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void Refresh();

	CParticlesProcessVelocity GetParticlesProcess()
	{
		UpdateData(SAVETOOBJECT);
		CParticlesProcessVelocity ppv;
		ppv.SetVelocityDir(FLOAT3D(m_fVelocityDirX, m_fVelocityDirY, m_fVelocityDirZ));
		ppv.SetVelocitySpd(m_fSpeed);
		return ppv;
	}
	void SetParticlesProcess(CParticlesProcessVelocity &ppv)
	{
		m_fVelocityDirX = ppv.GetVelocityDir()(1);
		m_fVelocityDirY = ppv.GetVelocityDir()(2);
		m_fVelocityDirZ = ppv.GetVelocityDir()(3);
		m_fSpeed = ppv.GetVelocitySpd();
		UpdateData(SAVETOFORM);
	}

// Dialog Data
	//{{AFX_DATA(CDlgCPPVelocityProperty)
	enum { IDD = IDD_CPP_VELOCITY_PROCESS_PROPERTY };
	float	m_fVelocityDirX;
	float	m_fVelocityDirY;
	float	m_fVelocityDirZ;
	float	m_fSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPVelocityProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPVelocityProperty)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPVELOCITYPROPERTY_H__35D900A0_2BAA_4856_BF1A_2C3A93325F34__INCLUDED_)
