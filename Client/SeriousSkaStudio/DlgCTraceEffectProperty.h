#if !defined(AFX_DLGCTRACEEFFECTPROPERTY_H__706EB0D3_969D_4D3E_BE62_1464B694DD7C__INCLUDED_)
#define AFX_DLGCTRACEEFFECTPROPERTY_H__706EB0D3_969D_4D3E_BE62_1464B694DD7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCTraceEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCTraceEffectProperty dialog

class CDlgCTraceEffectProperty : public CDialog
{
// Construction
public:
	CDlgCTraceEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCTraceEffectProperty)
	enum { IDD = IDD_CTRACEEFFECT_PROPERTY };
	CComboBox	m_cbBlendType;
	CColoredButton	m_btnColorEnd;
	CColoredButton	m_btnColorBegin;
	CString	m_strTexFileName;
	float	m_fTraceInterval;
	float	m_fTraceTime;
	float	m_fTwinklePeriod;
	BOOL	m_bCapEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCTraceEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCTraceEffectProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnTraceFilefind();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCTRACEEFFECTPROPERTY_H__706EB0D3_969D_4D3E_BE62_1464B694DD7C__INCLUDED_)
