#if !defined(AFX_DLGCPPDSOFFSETPOSAUTOSETTING_H__244A4522_1F3E_4E9E_B97E_F616B302FCFB__INCLUDED_)
#define AFX_DLGCPPDSOFFSETPOSAUTOSETTING_H__244A4522_1F3E_4E9E_B97E_F616B302FCFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPDSOffsetPosAutoSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDSOffsetPosAutoSetting dialog
#include "DlgAutosettingFeatherFall.h"
#include <Engine/Effect/CParticlesProcessDynamicState.h>

class CDlgCPPDSOffsetPosAutoSetting : public CDialog
{
// Construction
public:
	CDlgCPPDSOffsetPosAutoSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCPPDSOffsetPosAutoSetting)
	enum { IDD = IDD_CPP_DS_OFFSETPOS_AUTOSETTING };
	CComboBox	m_cbType;
	//}}AFX_DATA

	CDlgAutosettingFeatherFall m_dlgAutosettingFeatherFall;
	CParticlesProcessDynamicState *m_pPPDS;

	inline void SetDynamicState(CParticlesProcessDynamicState *pPPDS)
	{
		m_pPPDS = pPPDS;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPDSOffsetPosAutoSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPDSOffsetPosAutoSetting)
	afx_msg void OnSelendokCbType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPDSOFFSETPOSAUTOSETTING_H__244A4522_1F3E_4E9E_B97E_F616B302FCFB__INCLUDED_)
