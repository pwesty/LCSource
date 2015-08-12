#if !defined(AFX_DLGCSOUNDEFFECTPROPERTY_H__33C97C02_5A5C_4DF1_A8D9_C45ACE338C67__INCLUDED_)
#define AFX_DLGCSOUNDEFFECTPROPERTY_H__33C97C02_5A5C_4DF1_A8D9_C45ACE338C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSoundEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSoundEffectProperty dialog

class CDlgCSoundEffectProperty : public CDialog
{
// Construction
public:
	CDlgCSoundEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCSoundEffectProperty)
	enum { IDD = IDD_CSOUNDEFFECT_PROPERTY };
	CListBox	m_listSndFlag;
	float	m_fFallOff;
	CString	m_strSndFileName;
	float	m_fHotSpot;
	float	m_fVolume;
	float	m_fPitch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSoundEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCSoundEffectProperty)
	afx_msg void OnBtnSndFilefind();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSOUNDEFFECTPROPERTY_H__33C97C02_5A5C_4DF1_A8D9_C45ACE338C67__INCLUDED_)
