#if !defined(AFX_DLGUPDATETERRAINSHADOW_H__BD4A0C2C_3415_4913_AC48_23526C60E575__INCLUDED_)
#define AFX_DLGUPDATETERRAINSHADOW_H__BD4A0C2C_3415_4913_AC48_23526C60E575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUpdateTerrainShadow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateTerrainShadow dialog

class CDlgUpdateTerrainShadow : public CDialog
{
// Construction
public:
	int			m_iSliderPos;
	GFXColor	m_colObjShadow;
	CDlgUpdateTerrainShadow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUpdateTerrainShadow)
	enum { IDD = IDD_UPDATE_TERRAIN_SHADOW };
	CSliderCtrl	m_ctrlBlurRadius;
	CString	m_strBlurRadius;
	BOOL	m_bObjShadowSka;
	BOOL	m_bObjShadowMdl;
	BOOL	m_bObjShadowBrush;
	BOOL	m_bObjShadowTerrain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUpdateTerrainShadow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUpdateTerrainShadow)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnObjShadowColor();
	afx_msg void OnObjShadowSka();
	afx_msg void OnObjShadowMdl();
	afx_msg void OnObjShadowBrush();
	afx_msg void OnObjShadowTerrain();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUPDATETERRAINSHADOW_H__BD4A0C2C_3415_4913_AC48_23526C60E575__INCLUDED_)
