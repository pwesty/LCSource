// ShopDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "ShopDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShopDialog dialog


CShopDialog::CShopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShopDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShopDialog)
	//}}AFX_DATA_INIT
}


void CShopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShopDialog)
	DDX_Control(pDX, IDC_SHOP_GRID, m_ctlShopList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShopDialog, CDialog)
	//{{AFX_MSG_MAP(CShopDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShopDialog message handlers

BOOL CShopDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//-------------------------------------------------------------------
	// Grid 형태 설정.
	//-------------------------------------------------------------------
	CString aStrTitle[] = 
	{ _T("NPC Type"), _T("X"), _T("Y"), _T("Z"), _T("LV"), _T("Speed"), _T("Attack Area"), _T("Count Total") };

	int aiColWidth[] =
	{ 50, 50, 50, 50, 50, 50, 50, 50};

	//--------------------------------------------------------
	// 전체 리스트 목록 출력창.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	
	// 컬럼 헤더를 중앙에 출력.
	lvcolumn.fmt = LVCFMT_CENTER;
	
	for(int i = 0; i < sizeof(aiColWidth)/sizeof(int); ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlShopList.InsertColumn(i, &lvcolumn);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
