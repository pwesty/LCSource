// CreateWaterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "CreateWaterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateWaterDialog dialog


CCreateWaterDialog::CCreateWaterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateWaterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateWaterDialog)
	m_fWaterHeight	= 145.0f;
	m_fTileSize		= 6.0f;
	m_strFileName = _T("");
	//}}AFX_DATA_INIT
}


void CCreateWaterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateWaterDialog)
	DDX_Text(pDX, IDC_EDIT_WATER_HEIGHT, m_fWaterHeight);
	DDV_MinMaxFloat(pDX, m_fWaterHeight, 10.f, 300.f);
	DDX_Text(pDX, IDC_EDIT_WATER_TILESIZE, m_fTileSize);
	DDV_MinMaxFloat(pDX, m_fTileSize, 1.f, 512.f);
	DDX_Text(pDX, IDC_WATER_SMC_FILE, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateWaterDialog, CDialog)
	//{{AFX_MSG_MAP(CCreateWaterDialog)
	ON_BN_CLICKED(IDC_FIND_FILE, OnFindFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateWaterDialog message handlers

BOOL CCreateWaterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateWaterDialog::OnFindFile() 
{
	// TODO: Add your control notification handler code here
	
}
