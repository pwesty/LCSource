#if !defined(AFX_ENEMYSPAWNERDIALOG_H__7838F08C_E8D9_4E1D_AFF7_8CBE08AA7072__INCLUDED_)
#define AFX_ENEMYSPAWNERDIALOG_H__7838F08C_E8D9_4E1D_AFF7_8CBE08AA7072__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnemySpawnerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnemySpawnerDialog dialog

class CEnemySpawnerDialog : public CDialog
{
// Construction
public:
	CEnemySpawnerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnemySpawnerDialog)
	enum { IDD = IDD_NPC_SPAWNER };
	CListCtrl	m_ctlSpawnerList;
	UINT	m_uiTotalCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnemySpawnerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL		InsertDataToDB(int iWorldNum, int iMobIndex, float fXPos, float fYPos, float fZPos, float fRot, int iCount);
	BOOL		DelDataFromDB(int iWorldNum);
	BOOL		UpdateDataToDB(int iWorldNum);

	// Generated message map functions
	//{{AFX_MSG(CEnemySpawnerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnemyEdit();
	afx_msg void OnNpcDataDelete();
	afx_msg void OnSpawnerRescan();
	afx_msg void OnSpawnerSelect();
	afx_msg void OnEnemyScan();
	afx_msg void OnEnemyUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENEMYSPAWNERDIALOG_H__7838F08C_E8D9_4E1D_AFF7_8CBE08AA7072__INCLUDED_)
