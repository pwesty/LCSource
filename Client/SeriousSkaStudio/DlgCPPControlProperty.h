#if !defined(AFX_DLGCPPCONTROLPROPERTY_H__DE1A8299_496A_4A6E_BA39_5E820475D926__INCLUDED_)
#define AFX_DLGCPPCONTROLPROPERTY_H__DE1A8299_496A_4A6E_BA39_5E820475D926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPControlProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPControlProperty dialog
#include <Engine/Effect/CParticlesProcessControl.h>

class CDlgCPPControlProperty : public CDialog
{
// Construction
public:
	CDlgCPPControlProperty(CWnd* pParent = NULL);   // standard constructor

	const CParticlesProcessControl &GetParticlesProcess()
	{
		UpdateData(SAVETOOBJECT);
		if(m_cbControlType.GetCurSel() == CT_CIRCLE)
		{
			m_ppcontrol.SetCircleInfo(m_bUsePosition, m_fAngleSpeed);
		}
		else if(m_cbControlType.GetCurSel() == CT_SPIRAL)
		{
			m_ppcontrol.SetSpiralInfo(m_bUsePosition, m_fAngleSpeed, m_fHeightSpeed);
		}
		return m_ppcontrol;
	}
	void SetParticlesProcess(CParticlesProcessControl &ppc)
	{
		m_ppcontrol = ppc;
		UpdateData(SAVETOFORM);
		Refresh();
	}
	void Clear();
	void Refresh();

	// Dialog Data
	//{{AFX_DATA(CDlgCPPControlProperty)
	enum { IDD = IDD_CPP_CONTROL_PROPERTY };
	CStatic	m_stHeightSpeed;
	CComboBox	m_cbControlType;
	CEdit	m_edHeightSpeed;
	float	m_fAngleSpeed;
	float	m_fHeightSpeed;
	BOOL	m_bUsePosition;
	//}}AFX_DATA

	CParticlesProcessControl	m_ppcontrol;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPControlProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPControlProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokControlType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPCONTROLPROPERTY_H__DE1A8299_496A_4A6E_BA39_5E820475D926__INCLUDED_)
