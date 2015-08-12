// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CB4D7FB8_8B58_475B_B738_4E33848EFF64__INCLUDED_)
#define AFX_MAINFRM_H__CB4D7FB8_8B58_475B_B738_4E33848EFF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MDIClientWnd.h"
#include "SplitterFrame.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
  CTString CreateTexture();

public:
  // mdi client
  CMDIClientWnd m_wndMDIClient;
  // status bar
	CStatusBar  m_wndStatusBar;
  // toolbars
	CToolBar    m_wndToolBar;
  CToolBar    m_wndNavigationToolBar;
	CToolBar    m_wndToolBarManage;
  // edit control for model instance stretch
  CEdit m_ctrlMIStretch;
//안태훈 수정 시작	//(Open beta)(2004-11-30)
  CEdit m_ctrlMIAlpha;
//안태훈 수정 끝	//(Open beta)(2004-11-30)

#if _MSC_VER>=1300
#define DWORD_OR_HTASK DWORD
#else 
#define DWORD_OR_HTASK HTASK
#endif
// Generated message map functions
private:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivateApp(BOOL bActive, DWORD_OR_HTASK hTask);
	afx_msg void OnViewTreeview();
	afx_msg void OnUpdateViewTreeview(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnFileCreateTexture();
	afx_msg void OnBtClose();
	afx_msg void OnBtClear();
	afx_msg void OnViewErrorlist();
	afx_msg void OnUpdateViewErrorlist(CCmdUI* pCmdUI);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	afx_msg void OnViewEffectEdit();
	afx_msg void OnUpdateViewEffectEdit(CCmdUI* pCmdUI);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CB4D7FB8_8B58_475B_B738_4E33848EFF64__INCLUDED_)
