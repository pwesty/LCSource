
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree 창입니다.
class CUIBase;

class CViewTree : public CTreeCtrl
{
// 생성입니다.
public:
	CViewTree();
	void	setDblClkLoop(bool b) { m_bDblclkLoop = b; }
// 재정의입니다.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	HTREEITEM	DragItem(HTREEITEM hDragItem, CPoint pt);

// 구현입니다.
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()

private:
	bool m_bDblclkLoop;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};
