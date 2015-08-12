#if !defined(AFX_DLGPGTEXTURE_H__00278253_91BC_11D2_8478_004095812ACC__INCLUDED_)
#define AFX_DLGPGTEXTURE_H__00278253_91BC_11D2_8478_004095812ACC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPgTexture.h : header file
// 

/////////////////////////////////////////////////////////////////////////////
// CDlgPgTexture dialog

class CDlgPgTexture : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPgTexture)

// Construction
public:
	CDlgPgTexture();
	~CDlgPgTexture();

  BOOL OnIdle(LONG lCount);

  CViewTexture m_wndViewTexture;
  CUpdateableRT m_udTextureData;

// Dialog Data
	//{{AFX_DATA(CDlgPgTexture)
	enum { IDD = IDD_PG_TEXTURE };
	CCtrlEditBoolean	m_checkTexTrans3;
	CCtrlEditBoolean	m_checkTexTrans2;
	CCtrlEditBoolean	m_checkTexTrans1;
	CCtrlEditBoolean	m_checkAfterShadow;
	CColoredButton	m_ctrlCombineColor;
	CComboBox	m_comboBlend;
	CCtrlEditBoolean	m_checkReflective;
	CCtrlEditBoolean	m_checkClampV;
	CCtrlEditBoolean	m_checkClampU;
	CComboBox	m_comboScroll;
	CString	m_strTextureFile;
	float	m_fOffsetV;
	float	m_fOffsetU;
	int		m_radioTexture;
	float	m_fRotationU;
	float	m_fRotationV;
	float	m_fStrength;
	float	m_fStretchU;
	float	m_fStretchV;
	CString	m_strTextureDim;
	//}}AFX_DATA
	
  BOOL m_bOffsetV;
	BOOL m_bOffsetU;
	BOOL m_bRotationU;
	BOOL m_bRotationV;
	BOOL m_bStrength;
	BOOL m_bStretchU;
	BOOL m_bStretchV;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgPgTexture)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgPgTexture)
	afx_msg void OnBrowseTexture();
	afx_msg void OnRadioTexture();
	afx_msg void OnTexture2();
	afx_msg void OnTexture3();
	afx_msg void OnRemoveTexture();
	afx_msg void OnSelchangeScrollCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSelchangeBlendType();
	afx_msg void OnDropdownScrollCombo();
	afx_msg void OnDropdownBlendType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPGTEXTURE_H__00278253_91BC_11D2_8478_004095812ACC__INCLUDED_)