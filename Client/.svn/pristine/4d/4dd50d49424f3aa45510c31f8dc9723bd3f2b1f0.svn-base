
#include "stdafx.h"
#include "MainFrm.h"
#include "UITool.h"
#include "ViewTree.h"
#include <Engine/Interface/UIManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_AUTOEXPAND		(1000)		// 오토 익스팬드를 위한 타이머 ID 이자 시간.
#define ID_TIMER_AUTOSCROLL		(50)		// 오토 스크롤을 위한 타이머 ID 이자 시간.

class CmdCreateControl : public Command
{
public:
	void setData(const char* str_control)	{ str_control_ = str_control; }
	void execute()
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		pMain->AddUIControl(str_control_.c_str());
	}

private:
	std::string		str_control_;
};

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
	: m_bDblclkLoop(false)
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}
	
	return bRes;
}


void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hItem = GetSelectedItem();
	
	CUIBase* pUI = (CUIBase*)GetItemData(hItem);
	

	if( pUI )
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		pMain->SelectProperty(pUI, pUI->getType());
	}
	else
	{
		// 출력창에
		OutputDebugString(_T("해당 UI를 찾지 못했습니다.\n"));
	}
}


void CViewTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( m_bDblclkLoop == false )
		return;

	HTREEITEM hItem = GetSelectedItem();
	CString str = GetItemText(hItem);

	if( str.IsEmpty() == FALSE )
	{
		CmdCreateControl* pCmd = new CmdCreateControl;
		pCmd->setData((LPSTR)(LPCSTR)str);
		theApp.addQueue(pCmd);

	}
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}


void CViewTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CViewTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

// 드래그 처리를 위한 mfc 함수.
void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	HTREEITEM hSelectItem = pNMTreeView->itemNew.hItem;	// 최초 선택된 아이템.
	CPoint	  pt = pNMTreeView->ptDrag;

	CUIBase* pSelectUI = NULL;
	CUIBase* pDropUI   = NULL;
	if( hSelectItem )
		pSelectUI = (CUIBase*)GetItemData(hSelectItem);	// DWORD_PTR에 컨트롤의 포인터를 넘겨줬었음.

	HTREEITEM hDropItem = DragItem(hSelectItem, pt);	// 리턴값으로 드랍타겟의 핸들을 얻어옴. 드래그할 아이템의 핸들을 넘김.
	if( hDropItem )
		pDropUI = (CUIBase*)GetItemData(hDropItem);

	if( pSelectUI != NULL && pDropUI != NULL )
	{
		if( pSelectUI->getParent() != pDropUI && pSelectUI != pDropUI )
			pSelectUI->getParent()->VecMove(pSelectUI, pDropUI);

		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->GetFileView()->FillFileView();
	}
}

HTREEITEM CViewTree::DragItem( HTREEITEM hDragItem, CPoint pt )
{
	if( hDragItem == NULL || GetCapture() != NULL)	// 다른 윈도우가 마우스 캡춰중이면 리턴.
		return NULL;

	SetCapture();
	Invalidate(FALSE);

	CImageList* pDragImage = CreateDragImage(hDragItem);	// 드래그 이미지 생성.
	if(pDragImage == NULL)
		return NULL;

	CRect	rc;
	GetItemRect(hDragItem, &rc, TRUE);
	rc.left	-= GetIndent();
	pDragImage->BeginDrag(0, pt - rc.TopLeft());
	pDragImage->DragEnter(this, pt);			// 드래그 이미지를 마우스 위치에 그림.

	SelectDropTarget(NULL);
	HTREEITEM hDropTargetItem = NULL;
	MSG	msg;
	
	bool bBreak = FALSE;
	while( GetMessage(&msg, NULL, 0, 0) && !bBreak )	// 드래그 하는 동안 윈도우 메시지 처리.
	{
		if( GetCapture() != this )	// 캡춰된 윈도우가 다르다면 드래그를 포기함.
			break;

		switch( msg.message )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			{
				bBreak = true;	// 여기에 걸릴 경우 while문을 빠져 나가야 하기 때문에.
			}
			break;
		case WM_MOUSEMOVE:
			{
				pt.x = (short)LOWORD(msg.lParam);
				pt.y = (short)HIWORD(msg.lParam);

				hDropTargetItem = HitTest(pt);	// 드롭타겟 아이템을 얻어옴.

				if( hDropTargetItem == hDragItem && hDropTargetItem == GetParentItem(hDragItem))
					hDropTargetItem = NULL;
				else
				{
					HTREEITEM hItem = hDropTargetItem;
					while( (hItem = GetParentItem(hItem)) != NULL )
					{
						if( hItem == hDragItem )
						{
							hDropTargetItem = NULL;
							break;
						}
					}
				}

				if( hDropTargetItem != GetDropHilightItem() )
				{
					SetCursor(AfxGetApp()->LoadStandardCursor((hDropTargetItem == NULL) ? IDC_NO : IDC_ARROW ));
					pDragImage->DragShowNolock(FALSE);
					SelectDropTarget(hDropTargetItem);
					pDragImage->DragShowNolock(TRUE);

					SetTimer(ID_TIMER_AUTOEXPAND, ID_TIMER_AUTOEXPAND, NULL);	// 오토 익스펜드를 위한 타이머 ( 자동 펼치기 기능 )
				}
				GetClientRect(&rc);
				if( ((pt.x >= rc.left) && (pt.x <= rc.right))
					&& ((pt.y < rc.top + 10) || (pt.y > rc.bottom)) )
				{
					SetTimer(ID_TIMER_AUTOSCROLL, ID_TIMER_AUTOSCROLL, NULL);	// 오토 스크롤을 위한 타이머.
				}

				pDragImage->DragMove(pt);
			}
			break;

		case WM_TIMER:
			{
				if( msg.wParam == ID_TIMER_AUTOEXPAND )
				{
					hDropTargetItem = GetDropHilightItem();
					if( hDropTargetItem != NULL )
					{
						pDragImage->DragShowNolock(FALSE);
						Expand(hDropTargetItem, TVE_EXPAND);
						pDragImage->DragShowNolock(TRUE);
					}
					KillTimer(ID_TIMER_AUTOEXPAND);
				}
				else if( msg.wParam == ID_TIMER_AUTOSCROLL )
				{
					GetCursorPos(&pt);
					ScreenToClient(&pt);

					GetClientRect(&rc);
					BOOL bTestX = (pt.x >= rc.left) && (pt.x <= rc.right);
					if( bTestX && (pt.y < rc.top) )
						SendMessage(WM_VSCROLL, SB_LINEUP);
					else if(bTestX && (pt.y > rc.bottom))
						SendMessage(WM_VSCROLL, SB_LINEDOWN);
					else
						KillTimer(ID_TIMER_AUTOSCROLL);
				}
			}
			break;
		case WM_KEYDOWN:
			{
				switch(msg.wParam)
				{
				case VK_ESCAPE:
					{
						SelectDropTarget(NULL);	// 드래그중 esc키가 눌렸다면 드래그 포기.
						bBreak = true;
					}
					break;
				}
			}
			break;
		default:	// 내 msg 이외엔 관심 없음. 가져가서 일하세요.
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		}
	}

	KillTimer(ID_TIMER_AUTOSCROLL);
	KillTimer(ID_TIMER_AUTOEXPAND);

	pDragImage->DragLeave(this);
	pDragImage->EndDrag();
	delete pDragImage;

	Invalidate(FALSE);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	hDropTargetItem = GetDropHilightItem();
	SelectDropTarget(NULL);

	return hDropTargetItem;
}
