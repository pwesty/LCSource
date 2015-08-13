#if !defined(AFX_DLGCSPLINEBILLBOARDEFFECTPROPERTY_H__6AFABAFF_8155_44DE_9667_7D5323B9F482__INCLUDED_)
#define AFX_DLGCSPLINEBILLBOARDEFFECTPROPERTY_H__6AFABAFF_8155_44DE_9667_7D5323B9F482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSplineBillboardEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplineBillboardEffectProperty dialog

class CDlgCSplineBillboardEffectProperty : public CDialog
{
// Construction
public:
	CDlgCSplineBillboardEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCSplineBillboardEffectProperty)
	enum { IDD = IDD_CSPLINEBILLBOARDEFFECT_PROPERTY };
	CComboBox	m_cbBlendType;
	CColoredButton	m_btnColorEnd;
	CColoredButton	m_btnColorBegin;
	CString	m_strTexFileName;
	float	m_fInterval;
	float	m_fTraceTime;
	float	m_fWidth;
	float	m_fTwinklePeriod;
	float	m_fVelocity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSplineBillboardEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCSplineBillboardEffectProperty)
	afx_msg void OnBtnFilefind();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSPLINEBILLBOARDEFFECTPROPERTY_H__6AFABAFF_8155_44DE_9667_7D5323B9F482__INCLUDED_)
