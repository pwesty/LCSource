// NpcManagerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "NpcManagerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcManagerDialog dialog
CNpcManagerDialog::CNpcManagerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcManagerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcManagerDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pNpcDlg			= NULL;
	m_pSpawnerDlg		= NULL;
	m_pShopDlg			= NULL;	
	m_ptPos.x			= 4;
	m_ptPos.y			= 28;

	for(int i = 0; i < TOTAL_TAB; ++i)
	{
		m_pWndTab[i]	= NULL;
	}
}


void CNpcManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcManagerDialog)
	DDX_Control(pDX, IDC_NPC_TAB, m_ctlNpcTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcManagerDialog, CDialog)
	//{{AFX_MSG_MAP(CNpcManagerDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_NPC_TAB, OnSelchangeNpcTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcManagerDialog message handlers

BOOL CNpcManagerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pNpcDlg		= new CNpcDataDialog;
	m_pSpawnerDlg	= new CEnemySpawnerDialog;
	m_pShopDlg		= new CShopDialog;

	m_pNpcDlg->Create(CNpcDataDialog::IDD, &m_ctlNpcTab);
	m_pSpawnerDlg->Create(CEnemySpawnerDialog::IDD, &m_ctlNpcTab);
	m_pShopDlg->Create(CShopDialog::IDD, &m_ctlNpcTab);

	m_pWndTab[NPC_TAB]		= m_pNpcDlg;
	m_pWndTab[SPAWNER_TAB]	= m_pSpawnerDlg;
	m_pWndTab[SHOP_TAB]		= m_pShopDlg;

	m_pWndTab[NPC_TAB]->ShowWindow(TRUE);
	
	CString aStrTitle[] = 
	{ _T("NPC"), _T("Spawner"), _T("SHOP")};

	TCITEM item;
	item.mask		= TCIF_TEXT|TCIF_PARAM;
	for(int i = 0; i < TOTAL_TAB; ++i)
	{
		item.lParam		= (LPARAM) m_pWndTab[i];
		item.pszText	= (LPSTR)(LPCTSTR)(aStrTitle[i]);
		m_ctlNpcTab.InsertItem(i, &item);
		m_pWndTab[i]->SetWindowPos(NULL, m_ptPos.x, m_ptPos.y, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);		
	}

	// 접속중인 DB명을 표시해줍니다.
	CWnd* pWnd = GetDlgItem(IDC_DB_NAME);
	if(pWnd)
	{		
		pWnd->SetWindowText(((CWorldEditorApp*)AfxGetApp())->m_strDBName);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcManagerDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	for(int i = 0; i < TOTAL_TAB; ++i)
	{
		if(m_pWndTab[i])
		{
			delete m_pWndTab[i];
			m_pWndTab[i]	= NULL;
		}
	}

	((CMainFrame*)AfxGetMainWnd())->m_pNpcManagerDialog = NULL;
	delete this;

	CDialog::PostNcDestroy();
}

void CNpcManagerDialog::OnSelchangeNpcTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ActivateTab(m_ctlNpcTab.GetCurSel());
	
	*pResult = 0;
}

void CNpcManagerDialog::ActivateTab(int iTab)
{
	for(int i = 0; i < TOTAL_TAB; ++i)
	{
		m_pWndTab[i]->EnableWindow((i == iTab) ? TRUE : FALSE);
		m_pWndTab[i]->ShowWindow((i == iTab) ? TRUE : FALSE);
	}
}
