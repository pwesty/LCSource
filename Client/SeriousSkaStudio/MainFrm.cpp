#include "stdafx.h"
#include "SeriousSkaStudio.h"
#include "CmdSkaStudio.h"
#include "MainFrm.h"
#include "resource.h"

//안태훈 수정 시작	//(Remake Effect)(0.1)
#include "DlgCEffectProperty.h"
//안태훈 수정 끝	//(Remake Effect)(0.1)

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOAD_BAR_STATE( WName, HName, bar, dx, dy)                                      \
	bar.m_Size.cx = (AfxGetApp()->GetProfileInt(_T("General"),_T(WName),dx));             \
	bar.m_Size.cy = (AfxGetApp()->GetProfileInt(_T("General"),_T(HName),dy));             \
	bar.CalcDynamicLayout(0, LM_HORZDOCK)

#define SAVE_BAR_STATE( WName, HName, bar)                                              \
	AfxGetApp()->WriteProfileInt( _T("General"),_T(WName), bar.m_Size.cx);                \
	AfxGetApp()->WriteProfileInt( _T("General"),_T(HName), bar.m_Size.cy)

#define STD_TREEVIEW_WIDTH 230
#define STD_TREEVIEW_HEIGHT 550

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_VIEW_TREEVIEW, OnViewTreeview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREEVIEW, OnUpdateViewTreeview)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_COMMAND(ID_FILE_CREATE_TEXTURE, OnFileCreateTexture)
	ON_BN_CLICKED(IDC_BT_CLOSE, OnBtClose)
	ON_BN_CLICKED(IDC_BT_CLEAR, OnBtClear)
	ON_COMMAND(ID_VIEW_ERRORLIST, OnViewErrorlist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ERRORLIST, OnUpdateViewErrorlist)
//안태훈 수정 시작	//(Remake Effect)(0.1)
	ON_COMMAND(ID_VIEW_EFFECT_EDIT, OnViewEffectEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EFFECT_EDIT, OnUpdateViewEffectEdit)
//안태훈 수정 끝	//(Remake Effect)(0.1)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_STATUS_BAR_TEXT,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

#define STD_DRAGBAR_WIDTH  230
#define STD_DRAGBAR_HEIGHT 230

CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
}

BOOL _bApplicationActive = TRUE;
void CMainFrame::OnActivateApp(BOOL bActive, DWORD_OR_HTASK hTask) 
{
	_bApplicationActive = bActive;
	CMDIFrameWnd::OnActivateApp(bActive, hTask);
}

// create main frame
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// set same styles for use with all toolbars
	CRect rectDummy(0,0,0,0);
	DWORD dwToolBarStyles = TBSTYLE_FLAT | WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC | 
													CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_GRIPPER;

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// subclass mdiclient
	if (!m_wndMDIClient.SubclassWindow(m_hWndMDIClient)) {
			TRACE ("Failed to subclass MDI client window\n");
			return (-1);
	}                                                       
	// create toolbar IDR_MAINFRAME
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, dwToolBarStyles, rectDummy, IDW_TOOLBAR_MAINFRAME) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// create toolbar IDR_NAVIGATION
	if( (!m_wndNavigationToolBar.CreateEx(this, TBSTYLE_FLAT, dwToolBarStyles, rectDummy,IDW_TOOLBAR_NAVIGATION)) ||
		  (!m_wndNavigationToolBar.LoadToolBar(IDR_NAVIGATION)) )
	{
		TRACE0("Failed to create FX toolbar\n");
		return -1;      // fail to create fx tool bar
	}
	// create toolbar IDR_MANAGE
	if (!m_wndToolBarManage.CreateEx(this, TBSTYLE_FLAT, dwToolBarStyles, rectDummy,IDW_TOOLBAR_MANAGE) ||
	!m_wndToolBarManage.LoadToolBar(IDR_MANAGE))
	{
	  TRACE0("Failed to create toolbar\n");
	  return -1;      // fail to create
	}
	// create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	EnableDocking(CBRS_ALIGN_ANY);
	// enable docking for toolbars
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndNavigationToolBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBarManage.EnableDocking(CBRS_ALIGN_TOP);
	// dock toolbars
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndNavigationToolBar);
	DockControlBar(&m_wndToolBarManage);


	// Set model instance stretch edit ctrl
	m_wndNavigationToolBar.SetButtonInfo(STRETCH_BUTTON_INDEX, ID_MI_STRETCH, TBBS_SEPARATOR, 30);
	CRect rcEditStretch;
	m_wndNavigationToolBar.GetItemRect(STRETCH_BUTTON_INDEX, &rcEditStretch);
	rcEditStretch.left += 25;
	rcEditStretch.top = 2;
	rcEditStretch.right += 25;
	rcEditStretch.right -= 2;
	rcEditStretch.bottom = rcEditStretch.top + 18;
	if (!m_ctrlMIStretch.Create( WS_VISIBLE|WS_BORDER,
		rcEditStretch, &m_wndNavigationToolBar, ID_MI_STRETCH) )
	{
		TRACE0("Failed to create model instance stretch edit control\n");
		return FALSE;
	}
	m_ctrlMIStretch.SetWindowText("1");
//안태훈 수정 시작	//(Open beta)(2004-11-30)
	m_wndNavigationToolBar.SetButtonInfo(ALPHA_BUTTON_INDEX, ID_MI_ALPHA, TBBS_SEPARATOR, 30);
	m_wndNavigationToolBar.GetItemRect(ALPHA_BUTTON_INDEX, &rcEditStretch);
	rcEditStretch.top = 2;
	rcEditStretch.left += 25;
	rcEditStretch.right -= 2;
	rcEditStretch.right += 25;
	rcEditStretch.bottom = rcEditStretch.top + 18;
	if (!m_ctrlMIAlpha.Create( WS_VISIBLE|WS_BORDER,
		rcEditStretch, &m_wndNavigationToolBar, ID_MI_ALPHA) )
	{
		TRACE0("Failed to create model instance alpha edit control\n");
		return FALSE;
	}
	m_ctrlMIAlpha.SetWindowText("255");
//안태훈 수정 끝	//(Open beta)(2004-11-30)


	// Initialize dialog bar m_dlgBarTreeView
	if (!theApp.m_dlgBarTreeView.Create(this, IDD_TREEBAR,
		CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC,
		ID_VIEW_TREEVIEW))
	{
		TRACE0("Failed to create dialog bar m_dlgBarTreeView\n");
		return -1;		// fail to create
	}

//안태훈 수정 시작	//(Remake Effect)(0.1)
	if(!theApp.m_dlgEffectEdit.Create(this,IDD_EFFECT_EDIT,
		CBRS_RIGHT | CBRS_HIDE_INPLACE/* | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC*/,
		ID_VIEW_EFFECT_EDIT)) {
		TRACE0("Failed to create dialog bar m_dlgEffectEdit\n");
		return -1;		// fail to create
	}
	theApp.m_dlgEffectEditType.Create(IDD_EFFECT_EDIT_TYPE, &(theApp.m_dlgEffectEdit));
	theApp.m_dlgEffectEditType.ShowWindow(SW_SHOW);
	RECT rect;
	theApp.m_dlgEffectEditType.GetClientRect(&rect);
	theApp.m_dlgCEffectProperty.Create(IDD_CEFFECT_PROPERTY, &(theApp.m_dlgEffectEdit));
	theApp.m_dlgCEffectProperty.SetWindowPos(NULL, 1, rect.bottom + 2, 0, 0, SWP_NOSIZE);
	theApp.m_dlgCEffectProperty.ShowWindow(FALSE);
	theApp.m_dlgCEffectGroupProperty.Create(IDD_CEFFECTGROUP_PROPERTY, &(theApp.m_dlgEffectEdit));
	theApp.m_dlgCEffectGroupProperty.SetWindowPos(NULL, 1, rect.bottom + 2, 0, 0, SWP_NOSIZE);
	theApp.m_dlgCEffectGroupProperty.ShowWindow(FALSE);
	theApp.m_dlgCParticleGroupProperty.Create(IDD_CPARTICLEGROUP_PROPERTY, &(theApp.m_dlgEffectEdit));
	theApp.m_dlgCParticleGroupProperty.SetWindowPos(NULL, 1, rect.bottom + 2, 0, 0, SWP_NOSIZE);
	theApp.m_dlgCParticleGroupProperty.ShowWindow(FALSE);
//안태훈 수정 끝	//(Remake Effect)(0.1)

	if(!theApp.m_dlgErrorList.Create(this,IDD_ERROR_LIST,
		CBRS_BOTTOM/* | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC*/,
		ID_VIEW_ERRORLIST)) {
		TRACE0("Failed to create dialog bar m_dlgErrorList\n");
		return -1;		// fail to create
	}
	theApp.GetErrorList()->InsertColumn(0,"Error");
	theApp.GetErrorList()->SetImageList( &theApp.m_dlgBarTreeView.m_IconsImageList, LVSIL_SMALL);
	// theApp.m_dlgErrorList.m_Size = theApp.m_dlgErrorList.m_sizeDefault;  
	// theApp.m_dlgErrorList.SetSplitterControlID(IDC_SPLITER_LOG_FRAME);

	theApp.m_dlgBarTreeView.SetWindowText("Tree view");
	theApp.m_dlgErrorList.SetWindowText("Log");

	theApp.m_dlgBarTreeView.EnableDockingSides(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	// theApp.m_dlgErrorList.EnableDockingSides(CBRS_ALIGN_BOTTOM);
	//theApp.m_dlgErrorList.dlg_ulEnabledDockingSides = CBRS_ALIGN_BOTTOM;

	theApp.m_dlgBarTreeView.DockCtrlBar();
	// theApp.m_dlgErrorList.DockCtrlBar();
	// DockControlBar(&theApp.m_dlgErrorList);

	// set size of panels in status bar
	m_wndStatusBar.SetPaneInfo(0,ID_STATUS_BAR_TEXT,SBPS_NORMAL,600);
	m_wndStatusBar.SetPaneInfo(1,ID_SEPARATOR,SBPS_STRETCH,100);

	// try to load dialog width and height from reg.
	LOAD_BAR_STATE("TreeView width", "TreeView height", theApp.m_dlgBarTreeView,
		STD_TREEVIEW_WIDTH, STD_TREEVIEW_HEIGHT);
	// try to load toolbars positions
	LoadBarState(_T("General"));

	theApp.bAppInitialized = TRUE;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

// Show/hide treeview
void CMainFrame::OnViewTreeview() 
{
	// OnBarCheck();

	if(theApp.m_dlgBarTreeView.IsWindowVisible() )
	{
		ShowControlBar(&theApp.m_dlgBarTreeView, FALSE, FALSE);
		theApp.m_dlgBarTreeView.SetFocus();
	}
	else
	{
		ShowControlBar(&theApp.m_dlgBarTreeView, TRUE, FALSE);
		theApp.m_dlgBarTreeView.m_TreeCtrl.SetFocus();
	}
}
// Update treeview check
void CMainFrame::OnUpdateViewTreeview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.m_dlgBarTreeView.IsWindowVisible());
}

// Show/hide error dlg
void CMainFrame::OnViewErrorlist() 
{
	theApp.ShowErrorDlg(!theApp.IsErrorDlgVisible());
}
// Update error dlg
void CMainFrame::OnUpdateViewErrorlist(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.IsErrorDlgVisible());
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
// Show/hide error dlg
void CMainFrame::OnViewEffectEdit() 
{
	if( theApp.GetActiveView() != NULL )
		theApp.ShowEffectEditDlg(!theApp.IsEffectEditDlgVisible());
}
// Update error dlg
void CMainFrame::OnUpdateViewEffectEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.IsEffectEditDlgVisible());
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

// close main frame
void CMainFrame::OnClose() 
{
	// try to save all documents before closing them
	POSITION pos = theApp.m_pdtDocTemplate->GetFirstDocPosition();
	while (pos!=NULL)
	{
		CSeriousSkaStudioDoc *pmdCurrent = (CSeriousSkaStudioDoc *)theApp.m_pdtDocTemplate->GetNextDoc(pos);
		if(!pmdCurrent->BeforeDocumentClose()) {
			 return;
		}
	}

	theApp.ShowEffectEditDlg(FALSE);
	// save toolbars positions
	SaveBarState(_T("General"));
	// savedialog width and height in reg
	SAVE_BAR_STATE("TreeView width", "TreeView height", theApp.m_dlgBarTreeView);
	REG_SAVE_INDEX("GAPI",_pGfx->gl_eCurrentAPI);
	FLOAT fFOVDefault = 60.0f;
	REG_SAVE_INDEX("FOV",*(ULONG*)&theApp.m_fFOV);
	CMDIFrameWnd::OnClose();
}
// resize main frame
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	extern BOOL _bApplicationActive;
	if(nType==SIZE_MINIMIZED) {
		_bApplicationActive = FALSE;
	} else {
		_bApplicationActive = TRUE;
	}
	CMDIFrameWnd::OnSize(nType, cx, cy);
	m_wndStatusBar.SetPaneInfo(0,ID_STATUS_BAR_TEXT,SBPS_NORMAL,cx/1.5);  
}

// show/hide toolbars
void CMainFrame::OnViewToolbar() 
{
	BOOL bVisible = ((m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
	bVisible &= ((m_wndNavigationToolBar.GetStyle() & WS_VISIBLE) != 0);
	bVisible &= ((m_wndToolBarManage.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndToolBar, !bVisible, FALSE);
	ShowControlBar(&m_wndNavigationToolBar, !bVisible, FALSE);
	ShowControlBar(&m_wndToolBarManage, !bVisible, FALSE);
}
// update toolbars check
void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	BOOL bVisible = ((m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
	bVisible &= ((m_wndNavigationToolBar.GetStyle() & WS_VISIBLE) != 0);
	bVisible &= ((m_wndToolBarManage.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

CTString CMainFrame::CreateTexture()
{
	// call create texture dialog
	CTFileName fnCreated = _EngineGUI.CreateTexture();
	return fnCreated;
}

void CMainFrame::OnFileCreateTexture() 
{	
	CmdSkaMainFrmCreateTexture* pCmd = new CmdSkaMainFrmCreateTexture;
	if (pCmd)
		theApp.addQueue(pCmd);
}

void CMainFrame::OnBtClose() 
{
	theApp.ShowErrorDlg(FALSE);
}

void CMainFrame::OnBtClear() 
{
	theApp.GetErrorList()->DeleteAllItems();
}
