#if !defined(AFX_DLGCENTITYEFFECTPROPERTY_H__F3A478A8_B4B5_4CC3_8D9F_BE74FD98918D__INCLUDED_)
#define AFX_DLGCENTITYEFFECTPROPERTY_H__F3A478A8_B4B5_4CC3_8D9F_BE74FD98918D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCEntityEffectProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCEntityEffectProperty dialog

class CDlgCEntityEffectProperty : public CDialog
{
// Construction
public:
	CDlgCEntityEffectProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCEntityEffectProperty)
	enum { IDD = IDD_CENTITYEFFECT_PROPERTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CDlgCEffectControlProperty	m_dlgEffectControl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCEntityEffectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCEntityEffectProperty)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual LRESULT OnRegisterEffect(WPARAM wp, LPARAM lp);
	virtual LRESULT OnSelectEffect(WPARAM wp, LPARAM lp);
	virtual void OnClearForm();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCENTITYEFFECTPROPERTY_H__F3A478A8_B4B5_4CC3_8D9F_BE74FD98918D__INCLUDED_)
