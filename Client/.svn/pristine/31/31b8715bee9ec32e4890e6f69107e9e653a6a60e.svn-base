
#include "stdafx.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITextEx.h>
#include "FileView.h"

#include "UITool.h"
#include "mainfrm.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEFAULT_X		(0)
#define DEFAULT_Y		(0)
#define DEFAULT_WIDTH	(100)
#define DEFAULT_HEIGHT	(100)


/////////////////////////////////////////////////////////////////////////////
// CFileView
class CmdClone : public Command
{
	// win7 64bit 에서 MsgProc() 에서 발생 시키는 exception 처리 문제로 인한 변경
	// MFC 메시지 프로시저에서 exception을 무시, 파일을 열지 못하고 assult가 일어나서 변경.
	// 메시지 큐에서 메시지 처리 하는 방식으로 변경함.
public:
	CmdClone() : m_pParent(NULL), m_pChild(NULL) {}
	void setData(CUIBase* pParent, CUIBase* pChild) {  m_pParent = pParent; m_pChild = pChild; }
	void execute()
	{
		if (m_pParent == NULL || m_pChild == NULL)
			return;

		m_pParent->addChild(m_pChild->Clone());
		
		// 갱신!
		if (CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd())
		{
			pMain->GetFileView()->FillFileView();

			pMain->OnSaveFile("auto_save");
		}
	}
private:
	CUIBase* m_pParent;
	CUIBase* m_pChild;
};

CFileView::CFileView()
	: m_pCopyUI(NULL)
{
}

CFileView::~CFileView()
{
	m_pCopyUI = NULL;
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 메시지 처리기

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;	// TVS_SHOWSELALWAYS 포커스를 잃어도 선택된 아이템 강조하기.

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("파일 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 뷰 이미지를 로드합니다.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
	//FillFileView();
	AdjustLayout();
	SetDragMode(TRUE);
	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	if( pUIMgr == NULL )
		return;

	int max = pUIMgr->getChildCount();

	m_wndFileView.DeleteAllItems();

	for (int i = 0; i < max; ++i)
	{
		CUIWindow* pWnd = (CUIWindow*)pUIMgr->getChildAt(i);
	
		if( pWnd )
		{
			std::string		str_control;
			str_control = str_ui_control[pWnd->getType()];
			str_control += " [";
			str_control += pWnd->getID();
			str_control += "]";
			HTREEITEM hRoot = m_wndFileView.InsertItem(str_control.c_str(), 0, 0);
			m_wndFileView.SetItemData(hRoot, (DWORD_PTR)pWnd);
			m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
			RootItem(pWnd, hRoot);
			m_wndFileView.Expand(hRoot, TVE_EXPAND);
		}
	}
}

void CFileView::RootItem(CUIWindow* pWnd, HTREEITEM hRoot)
{
	int			i, nMaxChild = pWnd->getChildCount();
	if( nMaxChild < 0 )
		return;

	CUIBase*	pChild = NULL;

	for( i = 0; i < nMaxChild; ++i )
	{
		pChild = pWnd->getChildAt(i);
		
		if( pChild == NULL )
		{
			LogOutput("CFileView::RootItem(...) : Control Not Found");
			continue;
		}

		if (pChild->getType() == eUI_CONTROL_TEXT_EX || 
			(pChild->getParent() != NULL && pChild->getParent()->getType() == eUI_CONTROL_TEXT_EX))
			continue;

		std::string		str_control;
		str_control = str_ui_control[pChild->getType()];
		str_control += " [";
		str_control += pChild->getID();
		str_control += "]";

		HTREEITEM hChildRoot = m_wndFileView.InsertItem(str_control.c_str(), 1, 1, hRoot);
		m_wndFileView.SetItemData(hChildRoot, (DWORD_PTR)pChild);
		
		RootItem((CUIWindow*)pChild, hChildRoot);

		if (( pChild->getType() == eUI_CONTROL_LIST_ITEM 
			|| pChild->getType() == eUI_CONTROL_ARRAY_ITEM ) && i > 0 )
			m_wndFileView.Expand(hChildRoot, TVE_COLLAPSE);
		else
			m_wndFileView.Expand(hChildRoot, TVE_EXPAND);
	}
}
void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("속성...."));

}

void CFileView::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnFileOpenWith()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnDummyCompile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::CreateControl(CUIBase* pParent, CString strControl )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	if (pParent == NULL)
	{
		CUIWindow* pWnd = new CUIWindow;
		pWnd->InitPos(DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT);
		pUIManager->addChild(pWnd);
		pParent = pUIManager->getChildAt(0);
	}
	if (pParent == NULL) 
	{
		MessageBox(NULL, "BaseWindow가 생성 되지 않았습니다.", MB_OK);
		return;
	}

	CUIBase* pUI = NULL;

	if (strControl == str_ui_control[eUI_CONTROL_BASE])
	{
		pUI = new CUIBase;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_IMAGE] )
	{
		pUI = new CUIImage;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_BUTTON] )
	{
		pUI = new CUIButton;		
	}
	else if( strControl == str_ui_control[eUI_CONTROL_TEXT] )
	{
		pUI = new CUIText;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_TEXTBOX] )
	{
		pUI = new CUITextBox;
	}
	else if (strControl == str_ui_control[eUI_CONTROL_TEXTBOX_EX])
	{
		pUI = new CUITextBoxEx;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_EDIT] )
	{
		pUI = new CUIEditBox;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_SCROLL] )
	{
		pUI = new CUIScrollBar;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_CHECKBOX] )
	{
		pUI = new CUICheckButton;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_COMBOBOX] )
	{
		pUI = new CUIComboBox;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_SLIDEBAR] )
	{
		pUI = new CUISlideBar;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_PROGRESSBAR] )
	{
		pUI = new CUIProgressBar;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_LIST] )
	{
		pUI = new CUIList;

		// UIList가 생성되면 UIListItem을 자동생성해서 list의 child로 관리.
		CUIListItem* pListItem = new CUIListItem;
		((CUIList*)pUI)->SetPreCreate(true);
		((CUIList*)pUI)->SetItemCount(1);
		pListItem->InitPos(DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2);
		((CUIList*)pUI)->AddListItem(pListItem);
	}
	else if( strControl == str_ui_control[eUI_CONTROL_TAB] )
	{
		pUI = new CUITab;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_TABPAGE] )
	{
		pUI = new CUITabPage;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_SPINBUTTON] )
	{
		pUI = new CUISpinButton;
		// 스핀버튼 생성시 업다운 버튼을 연결해 준다.
		CUIButton* pUIBtn0 = new CUIButton;
		pUIBtn0->InitPos(DEFAULT_X, DEFAULT_Y, 8, 9);
		pUIBtn0->setID("btn_up");
		CUIButton* pUIBtn1 = new CUIButton;
		pUIBtn1->InitPos(DEFAULT_X, DEFAULT_Y, 8, 9);
		pUIBtn1->setID("btn_down");
		static_cast<CUISpinButton*>(pUI)->SetSpinButton(0, pUIBtn0);
		static_cast<CUISpinButton*>(pUI)->SetSpinButton(1, pUIBtn1);
	}
	else if( strControl == str_ui_control[eUI_CONTROL_ICON] )
	{
		pUI	= new CUIIcon;
		((CUIIcon*)pUI)->setInfo(UBET_ITEM, 0, 0, 0);
	}
	else if( strControl == str_ui_control[eUI_CONTROL_IMAGEFONT] )
	{
		pUI = new CUIImageFont;
	}
	else if( strControl == str_ui_control[eUI_CONTROL_ARRAY] )
	{
		pUI = new CUIArray;
		
		// CUIArray가 생성되면 UIArrayItem을 자동생성해서 Array의 child로 관리.
		CUIArrayItem* pArrItem = new CUIArrayItem;
		
		pArrItem->InitPos(DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2);
		
		pUI->addChild(pArrItem);
	}
	else if( strControl == str_ui_control[eUI_CONTROL_SPRITEANI] )
	{
		pUI = new CUISpriteAni;
	}
	else if (strControl == str_ui_control[eUI_CONTROL_IMAGE_ARRAY])
	{
		pUI = new CUIImageArray;
	}
	else if (strControl == str_ui_control[eUI_CONTROL_IMAGE_SPLIT])
	{
		pUI = new CUIImageSplit;
	}
	else if (strControl == str_ui_control[eUI_CONTROL_TREE])
	{
		pUI = new CUITree;

		UIRectUV uv;
		CUICheckButton* pCheck = new CUICheckButton;
		pCheck->setID("tree_collapse");
		pCheck->InitPos(DEFAULT_X, DEFAULT_Y, 11, 11);
		pCheck->setTexString("quest.tex");

		uv.SetUV(240, 895, 251, 906);
		pCheck->SetUVTex(UCBS_NONE, uv);

		uv.SetUV(227, 895, 238, 906);
		pCheck->SetUVTex(UCBS_CHECK, uv);

		static_cast<CUITree*>(pUI)->setCheck(pCheck);
	}
	else if (strControl == str_ui_control[eUI_CONTROL_TREEITEM])
	{
		pUI = new CUITreeItem;
	}
	else if (strControl == str_ui_control[eUI_CONTROL_SPIN_CONTROL])
	{
		pUI = new CUISpinControl;

		CUIButton* pUIBtn0 = new CUIButton;
		pUIBtn0->InitPos(DEFAULT_X, DEFAULT_Y, 8, 9);
		pUIBtn0->setID("btn_up");
		CUIButton* pUIBtn1 = new CUIButton;
		pUIBtn1->InitPos(DEFAULT_X, DEFAULT_Y, 8, 9);
		pUIBtn1->setID("btn_down");
		static_cast<CUISpinButton*>(pUI)->SetSpinButton(0, pUIBtn0);
		static_cast<CUISpinButton*>(pUI)->SetSpinButton(1, pUIBtn1);
	}
	
	if( pUI )
	{
		pUI->InitPos(DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT);
		pUI->initialize();
		pParent->addChild(pUI);

		FillFileView();

		HTREEITEM hTreeItem = m_wndFileView.GetRootItem();
		FindTreeItem(hTreeItem, (LPARAM)pUI);

		m_wndFileView.SetFocus();
	}
}

BOOL CFileView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	static bool bCtrl = false;
	
	switch(pMsg->message)
	{
	case WM_KEYUP:
		switch( pMsg->wParam )
		{
		case VK_DELETE:
			{
				if(	pMain->GetSelectControl() != NULL )
				{
					if( m_pCopyUI == pMain->GetSelectControl() )
						m_pCopyUI = NULL;

					pMain->GetSelectControl()->getParent()->deleteChild(pMain->GetSelectControl());
					pMain->ResetSelectUI();
					FillFileView();
				}
			}
			break;
		case VK_OEM_MINUS:
			{
				if( pMain->GetSelectControl()->getParent() != NULL )
				{
					int idx = pMain->GetSelectControl()->getParent()->GetChildIndex(pMain->GetSelectControl());
					if( idx < pMain->GetSelectControl()->getParent()->getChildCount() )
					{
						if( pMain->GetSelectControl()->getParent()->Swap(idx, idx+1) )
						{
							// 선택된 아이템을 다시 그리게 되므로 LParam 에 있는 UI 포인터 비교로 선택된 아이템을 찾는다.
							HTREEITEM hItem = m_wndFileView.GetSelectedItem();

							LPARAM param = NULL;
							if (hItem != NULL)
							{
								TV_ITEM	item;
								item.mask = TVIF_PARAM;
								item.hItem = hItem;
								m_wndFileView.GetItem(&item);

								param = item.lParam;
							}

							// 트리를 다시 그린다.
							FillFileView();

							// 트리를 탐색하여, 아이템을 찾는다.
							HTREEITEM hFind = m_wndFileView.GetRootItem();
							FindTreeItem(hFind, param);
						}
					}
				}
			}
			break;
		case VK_OEM_PLUS:
			{
				if( pMain->GetSelectControl()->getParent() != NULL )
				{
					int idx = pMain->GetSelectControl()->getParent()->GetChildIndex(pMain->GetSelectControl());
					if( idx > 0 )
					{
						if( pMain->GetSelectControl()->getParent()->Swap(idx, idx-1) )
						{
							// 선택된 아이템을 다시 그리게 되므로 LParam 에 있는 UI 포인터 비교로 선택된 아이템을 찾는다.
							HTREEITEM hItem = m_wndFileView.GetSelectedItem();

							LPARAM param = NULL;
							if (hItem != NULL)
							{
								TV_ITEM	item;
								item.mask = TVIF_PARAM;
								item.hItem = hItem;
								m_wndFileView.GetItem(&item);

								param = item.lParam;
							}

							// 트리를 다시 그린다.
							FillFileView();

							// 트리를 탐색하여, 아이템을 찾는다.
							HTREEITEM hFind = m_wndFileView.GetRootItem();
							FindTreeItem(hFind, param);
						}
					}
				}
			}
			break;
		case VK_INSERT:
			{
				if( pMain->GetSelectControl() == NULL )
					break;

				int nIdx = pMain->GetSelectControl()->getParent()->GetChildIndex( pMain->GetSelectControl() );
				// 현재 선택된 아이템의 바로 위에 인덱스를 가져와서 그 컨트롤의 차일드로 넣어줘야 하기 때문에 -1을 해줘야함.
				nIdx -= 1;
				
				CUIBase* p = pMain->GetSelectControl()->getParent()->getChildAt(nIdx);
				if( p == NULL )
					break;

				pMain->GetSelectControl()->getParent()->VecMove( pMain->GetSelectControl(), p );
				FillFileView();
			}
			break;
		}
		break;
	case WM_KEYDOWN:
		{
			BOOL bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );

			if ( bCtrl )
			{
				if (GetKeyState('C') & 0x8000)
				{
					if( pMain->GetSelectControl() == NULL )
						break;

					m_pCopyUI = NULL;

					m_pCopyUI = pMain->GetSelectControl();
				}
				else if (GetKeyState('V') & 0x8000)
				{
					if( m_pCopyUI != NULL && pMain->GetSelectControl() != NULL )
					{
						CmdClone* pCmd = new CmdClone;
						
						if( m_pCopyUI == pMain->GetSelectControl() )
							pCmd->setData(pMain->GetSelectControl()->getParent(), m_pCopyUI);
						else
							pCmd->setData(pMain->GetSelectControl(), m_pCopyUI);

						theApp.addQueue(pCmd);
					}
				}
			}

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
						return TRUE;
					}
				}
			}
		}
		break;
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}

void CFileView::FindTreeItem(HTREEITEM hFind, LPARAM param)
{
	if(hFind == NULL)
		return;

	hFind = m_wndFileView.GetNextItem(hFind, TVGN_CHILD);
	
	{	// 최상위 컨트롤 검사
		TV_ITEM	findRootItem;
		findRootItem.mask = TVIF_PARAM;
		findRootItem.hItem = m_wndFileView.GetRootItem();

		m_wndFileView.GetItem(&findRootItem);

		if (findRootItem.lParam == param)
		{
			m_wndFileView.SelectItem( findRootItem.hItem );
			return;
		}
	}

	TV_ITEM	findItem;

	while (hFind)
	{
		findItem.mask = TVIF_PARAM;
		findItem.hItem = hFind;
		m_wndFileView.GetItem(&findItem);

		if (findItem.lParam == param)
		{
			m_wndFileView.SelectItem( findItem.hItem );
			break;
		}

		FindTreeItem(hFind, param);
		hFind = m_wndFileView.GetNextItem(hFind, TVGN_NEXT);	
	}
}

void CFileView::RFindControl( BOOL bCtrl, CUIBase* pBase, int x, int y )
{
	if (pBase == NULL)
		return;

	if (pBase->GetHide() == TRUE)
		return;

	int max = pBase->getChildCount();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	CUIBase* pFind = NULL;

	if (pMain == NULL)
		return;

	if (bCtrl == TRUE && pMain->GetSelectControl() != NULL)	
	{	// 컨트롤이 눌려있고 현재 선택된 컨트롤이 있다면.
		CUIBase* pParent = NULL;
		
		pParent = pMain->GetSelectControl()->getParent();

		if (pParent != NULL && pParent != (CUIBase*)CUIManager::getSingleton())	// UI매니저가 선택되면 안됨.!
		{	// 현재 선택된 컨트롤부터 검사 시작.
			pFind = RFindCurIndexControl(pParent, x, y);	

			if (pFind != NULL)
			{
				SelectControl(pFind);
				return;
			}
		}
	}

	pFind = RFindChildControl(pBase, x, y);	// 위에서 리턴이 안됐다면 차일드 검사.

	if (pFind)
		SelectControl(pFind);
}

CUIBase* CFileView::RFindCurIndexControl( CUIBase* pBase, int x, int y )
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

 	int idx = pBase->GetChildIndex(pMain->GetSelectControl());
// 	
// 	CUIBase* pCurChild = pBase->getChildAt(idx);	// 현재 선택된 자기 자신의 차일드만 검사.
// 
// 	if (pCurChild != NULL)
// 	{
// 		CUIBase* pFind = RFindChildControl(pCurChild, x, y);
// 		if (pFind != NULL)
// 			return pFind;
// 	}

	while(idx != 0)
	{
		CUIBase* pChild = pBase->getChildAt(--idx);	// 위에서 리턴이 안됐다면 현재 선택된 컨트롤은 검사목록에서 제외.

		if (pChild == NULL)
			return NULL;

		if (pChild->GetHide() == TRUE)
			continue;

		CUIBase* pFind = RFindChildControl(pChild, x, y);

		if (pFind != NULL)
			return pFind;

		if (pChild->IsInside(x, y) == TRUE)
			return pChild;
	}

	if (pBase->IsInside(x, y) == TRUE)
		return pBase;

	return NULL;
}

CUIBase* CFileView::RFindChildControl( CUIBase* pBase, int x, int y )
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	int max = pBase->getChildCount();

	while(max != 0)
	{
		CUIBase* pChild = pBase->getChildAt(--max);

		if (pChild == NULL)
			return NULL;

		if (pChild->GetHide() == TRUE)
			continue;

		CUIBase* pFindUI = RFindChildControl(pChild, x, y);

		if (pFindUI != NULL)
			return pFindUI;

		if (pChild->IsInside(x, y) == TRUE)
			return pChild;
	}
	return NULL;
}

void CFileView::SelectControl( CUIBase* pBase )
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	if (pMain == NULL || pBase == NULL)
		return;

	pMain->SelectProperty(pBase, pBase->getType());

	CUIBase* pParent = pBase->getParent();
	if (pBase->getType() == eUI_CONTROL_TEXT_EX ||
		(pParent != NULL && pParent->getType() == eUI_CONTROL_TEXT_EX))
		return;

	HTREEITEM hFind = m_wndFileView.GetRootItem();

	FindTreeItem(hFind, (LPARAM)pBase);

	m_wndFileView.SetFocus();
}
