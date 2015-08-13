#if !defined(AFX_DLGCPPPROPERTY_H__9708E5C3_3BB0_4327_811C_5B60925306E8__INCLUDED_)
#define AFX_DLGCPPPROPERTY_H__9708E5C3_3BB0_4327_811C_5B60925306E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPProperty.h : header file
//

#include <vector>
#include <Engine/Effect/CParticlesCommonProcess.h>
#include "DlgCPPDynamicStateProperty.h"
#include "DlgCPPForceProcessProperty.h"
#include "DlgCPPPointGoalProperty.h"
#include "DlgCPPControlProperty.h"
#include "DlgCPPVelocityProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPProperty dialog

class CDlgCPPProperty : public CDialog
{
// Construction
public:
	CDlgCPPProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void Refresh();

	BOOL SetPropertyTo(CParticleGroup &pg);
	BOOL SetPropertyFrom(CParticleGroup &pg);

// Dialog Data
	//{{AFX_DATA(CDlgCPPProperty)
	enum { IDD = IDD_CPARTICLEPROCESS_PROPERTY };
	CListBox	m_lbAdded;
	CComboBox	m_cbType;
	//}}AFX_DATA

	std::vector<CParticlesCommonProcess *> m_vectorPProcess;

	CDlgCPPDynamicStateProperty m_dlgCPPDynamicStateProperty;
	CDlgCPPForceProcessProperty m_dlgCPPForceProperty;
	CDlgCPPPointGoalProperty	m_dlgCPPPointGoalProperty;
	CDlgCPPControlProperty		m_dlgCPPControlProperty;
	CDlgCPPVelocityProperty		m_dlgCPPVelocityProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPProperty)
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnSelEndOkPpType();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangePpAdded();
	afx_msg void OnBtnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPPROPERTY_H__9708E5C3_3BB0_4327_811C_5B60925306E8__INCLUDED_)
