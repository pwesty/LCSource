#if !defined(AFX_DLGSETPRIORITY_H__B66A4C5F_FA80_4593_A212_712A249588A2__INCLUDED_)
#define AFX_DLGSETPRIORITY_H__B66A4C5F_FA80_4593_A212_712A249588A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetPriority.h : header file
//
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CDlgSetPriority dialog

class CDlgSetPriority : public CDialog
{
// Construction
public:
	CDlgSetPriority(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetPriority)
	enum { IDD = IDD_SET_PRIORITY };
	CSpinButtonCtrl	m_ctlSpinPriority;
	CListBox	m_lbPriority;
	CFont		m_Font;
	//}}AFX_DATA
	std::vector<int>	m_vectorPriority;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetPriority)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void	SetPriorityList(std::vector<int> vectorList);
	void	GetPriorityList(std::vector<int>& vectorList);

// Implementation
protected:
	void	RefreshPriorityList();
	void	SwapPriority( int iOld, int iNew );

	// Generated message map functions
	//{{AFX_MSG(CDlgSetPriority)
	afx_msg void OnSelchangeListPriority();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposPrioritySpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPRIORITY_H__B66A4C5F_FA80_4593_A212_712A249588A2__INCLUDED_)
