#if !defined(AFX_DLGCTERRAINEFFECTPROPERTY_H__ACE37077_2B15_4BE5_8D75_E37FA88DEF37__INCLUDED_)
#define AFX_DLGCTERRAINEFFECTPROPERTY_H__ACE37077_2B15_4BE5_8D75_E37FA88DEF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCTerrainEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCTerrainEffectProperty dialog

class CDlgCTerrainEffectProperty : public CDialog
{
// Construction
public:
	CDlgCTerrainEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCTerrainEffectProperty)
	enum { IDD = IDD_CTERRAINEFFECT_PROPERTY };
	CComboBox	m_comboRotationType;
	CComboBox	m_comboPositionType;
	CComboBox	m_cbBlendType;
	CColoredButton	m_btnColor;
	float	m_fTRAreaBottom;
	float	m_fTRAreaLeft;
	float	m_fTRAreaRight;
	float	m_fTRAreaTop;
	CString	m_strTerrainTextureFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCTerrainEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCTerrainEffectProperty)
	virtual BOOL OnInitDialog();
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	afx_msg void OnBtnTerrainFilefind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCTERRAINEFFECTPROPERTY_H__ACE37077_2B15_4BE5_8D75_E37FA88DEF37__INCLUDED_)
