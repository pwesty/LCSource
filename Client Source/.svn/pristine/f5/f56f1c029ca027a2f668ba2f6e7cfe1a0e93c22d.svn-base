
#pragma once

#include "ViewTree.h"

class CUIBase;
class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 생성입니다.
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 특성입니다.
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

//protected:
public:
	void FillFileView();
	void CreateControl(CUIBase* pParent, CString strControl);

	void RFindControl(BOOL bCtrl, CUIBase* pBase, int x, int y);
	bool TestFindUI(CUIBase* pBase, int x, int y);
protected:
	void FindTreeItem(HTREEITEM hFind, LPARAM param);
	void RootItem(CUIWindow* pWnd, HTREEITEM hRoot);
	CUIBase* RFindCurIndexControl(CUIBase* pBase, int x, int y);
	CUIBase* RFindChildControl(CUIBase* pBase, int x, int y);
	void SelectControl(CUIBase* pBase);

// 구현입니다.
public:
	virtual ~CFileView();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CUIBase* m_pCopyUI;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};

