#if !defined(AFX_DLGCEFFECTGROUPPROPERTY_H__C0E7DB3B_BC04_49B3_B882_058ACC8E494D__INCLUDED_)
#define AFX_DLGCEFFECTGROUPPROPERTY_H__C0E7DB3B_BC04_49B3_B882_058ACC8E494D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#include <vector>
#include <string>
// DlgCEffectGroupProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectGroupProperty dialog

class CDlgCEffectGroupProperty : public CDialog
{
// Construction
public:
	CDlgCEffectGroupProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void ClearTagkey();
	void Refresh();
	void RefreshEffect();
	void SelectEffectKey(int index);
	
	void EffectGroupSelectChangeNotify()
	{
		OnSelEndOkEgRegisteredName();
	}

	struct TagKeyInfo
	{
		float m_fSettingTime;
		std::string m_strTagName;
	};
	std::vector< std::vector<TagKeyInfo> > m_vectorTagVector;
	CString m_strEffectGroupNameOld;

// Dialog Data
	//{{AFX_DATA(CDlgCEffectGroupProperty)
	enum { IDD = IDD_CEFFECTGROUP_PROPERTY };
	CComboBox	m_cbTagName;
	CComboBox	m_cbEffectType;
	CListBox	m_lstRegisteredTagKey;
	CListBox	m_lstRegisteredEffectKey;
	CComboBox	m_cbRegisteredEffect;
	CComboBox	m_cbRegisteredEGName;
	CString	m_strEffectGroupName;
	float	m_fTagSettingTime;
	float	m_fEffectStartTime;
	CString	m_strTagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCEffectGroupProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCEffectGroupProperty)
	afx_msg void OnBtnSave();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnEkAdd();
	afx_msg void OnBtnEkDel();
	afx_msg void OnBtnTkAdd();
	afx_msg void OnBtnTkDel();
	afx_msg void OnSelChangeRegisteredEffectKey();
	afx_msg void OnSelChangeRegisteredTagKey();
	afx_msg void OnBtnRefresh();
	afx_msg void OnBtnTkAddall();
	afx_msg void OnSelEndOkEgRegisteredName();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokEffectType();
	afx_msg void OnSelendokRegisteredTagName();
	afx_msg void OnMaxtextEkStartTime();
	afx_msg void OnDblclkRegisteredEffectKey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCEFFECTGROUPPROPERTY_H__C0E7DB3B_BC04_49B3_B882_058ACC8E494D__INCLUDED_)
