
// UIToolView.cpp : CUIToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "UIToolView.h"
#include "MainFrm.h"

#include <Engine/Interface/UIManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUIToolView

IMPLEMENT_DYNCREATE(CUIToolView, CView)

BEGIN_MESSAGE_MAP(CUIToolView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CUIToolView 생성/소멸

CUIToolView::CUIToolView()
	: m_pUIMgr(NULL)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CUIToolView::~CUIToolView()
{
}

BOOL CUIToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CView::PreCreateWindow(cs);
}

// CUIToolView 그리기

void CUIToolView::OnDraw(CDC* /*pDC*/)
{
	CUIToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CUIToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	theApp.initDraw(m_hWnd);
	 
	m_pUIMgr = CUIManager::getSingleton();
}

void CUIToolView::OnDestroy()
{
	CView::OnDestroy();
}

void CUIToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUIToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUIToolView 진단

#ifdef _DEBUG
void CUIToolView::AssertValid() const
{
	CView::AssertValid();
}

void CUIToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIToolDoc* CUIToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIToolDoc)));
	return (CUIToolDoc*)m_pDocument;
}

#endif //_DEBUG


// CUIToolView 메시지 처리기


LRESULT CUIToolView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	static bool bCtrl = false;
	static bool bDrag = false;
	static bool bDraging = false;
	static POINT Oldpt;

	POINT	pt;

	pt.x = (short)LOWORD( lParam );
	pt.y = (short)HIWORD( lParam );

	DWORD dwPram = MAKELPARAM(pt.x, pt.y);

	int nX = (short)LOWORD( dwPram );
	int nY = (short)HIWORD( dwPram );

	switch(message)
	{
	case WM_MOUSEMOVE:
		{
			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				pMain->SetMousePosition(dwPram);
				
				if (( GetKeyState(VK_SHIFT) & 0x8000 ) && bDrag == true)
				{
					CUIBase* pControl = pMain->GetSelectControl();
	
					if (pControl != NULL)
					{
						int x, y;
						x = nX - Oldpt.x;
						y = nY - Oldpt.y;

						pControl->SetPos(pControl->GetOrigX() + x, pControl->GetOrigY() + y);
						pControl->updatePosition(true);
						pMain->GetPropWnd()->UpdatePos( pControl->GetOrigX(), pControl->GetOrigY());

						bDraging = true;
					}
				}
				else
				{
					bDraging = false;
				}
			}
			
			Oldpt.x = nX;
			Oldpt.y = nY;

			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );
		}
		break;
	case WM_LBUTTONDOWN:
		{
			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );

			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				if (pMain->GetSelectControl() && pMain->GetSelectControl()->IsInside(nX, nY))
					bDrag = true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if (bDraging == false)
			{
				if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
				{
					if (pMain->GetFileView() != NULL)
					{
						if (pUIManager != NULL)
						{	// RFindControl은 차일드부터 검사하기 때문에 UIManager를 넣어준다.
							pMain->GetFileView()->RFindControl(bCtrl, (CUIBase*)pUIManager, nX, nY);
						}
					}
				}
			}
			
			bDrag = false;
		}
		break;
	case WM_KEYDOWN:
		{
			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );

			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				CUIBase* pControl = pMain->GetSelectControl();

				if (pControl != NULL)
				{
					if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || 
						GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
					{
						int nX = 0, nY = 0;

						if (GetKeyState('W') & 0x8000)
							nY -= 1;
						if (GetKeyState('S') & 0x8000)
							nY += 1;
						if (GetKeyState('A') & 0x8000)
							nX -= 1;
						if (GetKeyState('D') & 0x8000)
							nX += 1;

						pControl->SetPos(pControl->GetOrigX() + nX, pControl->GetOrigY() + nY);
						pControl->updatePosition(true);
						pMain->GetPropWnd()->UpdatePos( pControl->GetOrigX(), pControl->GetOrigY());
					}
				}
			}
		}
		break;
	case WM_KEYUP:
		{
			bCtrl = FALSE;
		}
		break;
	}

	return CView::DefWindowProc(message, wParam, lParam);
}


BOOL CUIToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pUIMgr)
	{
		m_pUIMgr->MsgSimpleProc(pMsg);
	}
	return CView::PreTranslateMessage(pMsg);
}


void CUIToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pUIMgr)
	{
		_pGfx->ResetDisplayMode();

		m_pUIMgr->SetSize(cx, cy, true);
	}
}
