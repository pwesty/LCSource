#if !defined(AFX_NPCMANAGERDIALOG_H__7CBFBB21_7961_4B19_A152_916DA32B9168__INCLUDED_)
#define AFX_NPCMANAGERDIALOG_H__7CBFBB21_7961_4B19_A152_916DA32B9168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NpcManagerDialog.h : header file
//
#include "NpcDataDialog.h"
#include "EnemySpawnerDialog.h"
#include "ShopDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNpcManagerDialog dialog

class CNpcManagerDialog : public CDialog
{
	enum eTabs
	{
		NPC_TAB		= 0,
		SPAWNER_TAB	= 1,
		SHOP_TAB	= 2,
		TOTAL_TAB	= 3,
	};
	
// Construction
public:
	CNpcManagerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNpcManagerDialog)
	enum { IDD = IDD_NPC_MANAGER };	
	CNpcDataDialog			*m_pNpcDlg;
	CEnemySpawnerDialog		*m_pSpawnerDlg;
	CShopDialog				*m_pShopDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNpcManagerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPoint					m_ptPos;
	CTabCtrl				m_ctlNpcTab;
	CWnd*					m_pWndTab[TOTAL_TAB];

	void		ActivateTab(int iTab);

	// Generated message map functions
	//{{AFX_MSG(CNpcManagerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNpcTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPCMANAGERDIALOG_H__7CBFBB21_7961_4B19_A152_916DA32B9168__INCLUDED_)
