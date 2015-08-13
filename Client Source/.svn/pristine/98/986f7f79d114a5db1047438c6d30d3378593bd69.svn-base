#if !defined(AFX_MOBDATADIALOG_H__072CC631_B379_4211_8F55_3DB3B7F28C25__INCLUDED_)
#define AFX_MOBDATADIALOG_H__072CC631_B379_4211_8F55_3DB3B7F28C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MobDataDialog.h : header file
//
#include "resource.h"
#include "dbcmd.h"

/////////////////////////////////////////////////////////////////////////////
// CMobDataDialog dialog

class CMobDataDialog : public CDialog
{
// Construction
public:
	CMobDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMobDataDialog)
	enum { IDD = IDD_MOB_DATA_DIALOG };
	CComboBox	m_ctlcbFindMobByType;
	CListCtrl	m_ctlMobCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMobDataDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeMobFindCombo();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMakeZoneFlag();
	afx_msg void OnDblclkMobGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownMobGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL IsValidShop(const int iNpcIndex, int& iSellRate, int& iBuyRate);
	void RefreshData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBDATADIALOG_H__072CC631_B379_4211_8F55_3DB3B7F28C25__INCLUDED_)
