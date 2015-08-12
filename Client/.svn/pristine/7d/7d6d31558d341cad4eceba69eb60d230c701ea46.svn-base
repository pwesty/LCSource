#if !defined(AFX_DLGCPPPOINTGOALPROPERTY_H__206E8137_6828_4F1D_AB65_F60F4DE863D1__INCLUDED_)
#define AFX_DLGCPPPOINTGOALPROPERTY_H__206E8137_6828_4F1D_AB65_F60F4DE863D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPPointGoalProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPPointGoalProperty dialog
#include <Engine/Effect/CParticlesProcessPointGoal.h>

class CDlgCPPPointGoalProperty : public CDialog
{
// Construction
public:
	CDlgCPPPointGoalProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void Refresh() {}

	CParticlesProcessPointGoal GetParticlesProcess()
	{
		UpdateData(SAVETOOBJECT);
		
		CParticlesProcessPointGoal pppg;
		pppg.SetPointGoal(FLOAT3D(m_fX, m_fY, m_fZ));
		pppg.SetSpeed(m_fSpeed);
		pppg.SetLerpValue(m_fLerpRatio, m_fLerpSpeed);
		pppg.SetGoalTagName(m_strGoalTagName.GetBuffer(0));
		return pppg;
	}
	void SetParticlesProcess(CParticlesProcessPointGoal &pppg)
	{
		m_fSpeed = pppg.GetSpeed();
		m_fX = pppg.GetPointGoal()(1);
		m_fY = pppg.GetPointGoal()(2);
		m_fZ = pppg.GetPointGoal()(3);
		m_fLerpRatio = pppg.GetLerpRatio();
		m_fLerpSpeed = pppg.GetLerpSpeed();
		m_strGoalTagName = pppg.GetGoalTagName();

		UpdateData(SAVETOFORM);
	}
// Dialog Data
	//{{AFX_DATA(CDlgCPPPointGoalProperty)
	enum { IDD = IDD_CPP_POINTGOAL_PROPERTY };
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fSpeed;
	float	m_fLerpSpeed;
	float	m_fLerpRatio;
	CString	m_strGoalTagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPPointGoalProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPPointGoalProperty)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPPOINTGOALPROPERTY_H__206E8137_6828_4F1D_AB65_F60F4DE863D1__INCLUDED_)
