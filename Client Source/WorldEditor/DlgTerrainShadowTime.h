#if !defined(AFX_DLGTERRAINSHADOWTIME_H__184AA6FC_189C_49ED_883D_EFF4694F1584__INCLUDED_)
#define AFX_DLGTERRAINSHADOWTIME_H__184AA6FC_189C_49ED_883D_EFF4694F1584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTerrainShadowTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainShadowTime dialog

class CDlgTerrainShadowTime : public CDialog
{
// Construction
public:
	ULONG	m_ulWorldTime;
	CDlgTerrainShadowTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTerrainShadowTime)
	enum { IDD = IDD_EDIT_TERRAIN_TIME };
	CString	m_strResultTime;
	int		m_nHour;
	int		m_nMinute;
	int		m_nSecond;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTerrainShadowTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTerrainShadowTime)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTerrainShadowHour();
	afx_msg void OnChangeTerrainShadowMin();
	afx_msg void OnChangeTerrainShadowSec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTERRAINSHADOWTIME_H__184AA6FC_189C_49ED_883D_EFF4694F1584__INCLUDED_)
