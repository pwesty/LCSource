// DlgExportBspHeightMap.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "DlgExportBspHeightMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspHeightMap dialog


CDlgExportBspHeightMap::CDlgExportBspHeightMap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportBspHeightMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportBspHeightMap)
	m_strFileName = _T("");
	m_nWidth = 0;
	m_nHeight = 0;
	m_nSelFloor = 0;
	m_bFieldAttrMap = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgExportBspHeightMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportBspHeightMap)
	DDX_Text(pDX, IDC_BSPHEIGHTMAP_NAME, m_strFileName);
	DDX_Text(pDX, IDC_BSPHEIGHTMAP_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 10000);
	DDX_Text(pDX, IDC_BSPHEIGHTMAP_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 0, 10000);
	DDX_Radio(pDX, IDC_SELECT_BSPHMFLOOR1, m_nSelFloor);
	DDX_Check(pDX, IDC_CHECK_BSPHMFIELD, m_bFieldAttrMap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportBspHeightMap, CDialog)
	//{{AFX_MSG_MAP(CDlgExportBspHeightMap)
	ON_BN_CLICKED(IDC_BSPHEIGHTMAP_OPEN, OnBspheightmapOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspHeightMap message handlers

void CDlgExportBspHeightMap::OnBspheightmapOpen() 
{
	UpdateData( TRUE );

	CTFileName fnHeightMap=_EngineGUI.FileRequester(
		"Export bsp server height map", FILTER_SHT FILTER_ALL FILTER_END,
		"Bsp attribute map directory", "Data\\World\\" );
	if( fnHeightMap == "" ) return;
	try
	{
		m_fnFileName = fnHeightMap;
		m_strFileName = fnHeightMap;
		CMainFrame* pMainFrame = STATIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
		pMainFrame->Invalidate( FALSE );
	}
	catch(char *strError)
	{
		AfxMessageBox( strError );
	}

	UpdateData( FALSE );
}

void CDlgExportBspHeightMap::OnOK() 
{
	UpdateData( TRUE );
	
	CDialog::OnOK();
}
