// DlgExportBspAttributeMap.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgExportBspAttributeMap.h"
#include "DlgSetPriority.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspAttributeMap dialog

// Default ¼ø¼­ : 0, 4, 5, 6, 7, 8, 2, 1
static int _aDefaultAttributeType[ATTC_TOTAL_COUNT] = 
{ 0, 4, 5, 6, 7, 8, 2, 3, 1 };

CDlgExportBspAttributeMap::CDlgExportBspAttributeMap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportBspAttributeMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportBspAttributeMap)
	m_nHeight = 0;
	m_nWidth = 0;
	m_nSelFloor = 0;
	m_strFileName = _T("");
	m_bFieldAttrMap = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgExportBspAttributeMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportBspAttributeMap)
	DDX_Text(pDX, IDC_BSPATTRIBUTEMAP_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 0, 10000);
	DDX_Text(pDX, IDC_BSPATTRIBUTEMAP_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 10000);
	DDX_Radio(pDX, IDC_SELECT_BSPFLOOR1, m_nSelFloor);
	DDX_Text(pDX, IDC_BSPATTRIBUTEMAP_NAME, m_strFileName);
	DDX_Check(pDX, IDC_CHECK_BSPFIELD, m_bFieldAttrMap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportBspAttributeMap, CDialog)
	//{{AFX_MSG_MAP(CDlgExportBspAttributeMap)
	ON_BN_CLICKED(IDC_BSPATTRIBUTEMAP_OPEN, OnBspattributemapOpen)
	ON_BN_CLICKED(IDC_BUTTON_PRIORITY, OnButtonPriority)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspAttributeMap message handlers

void CDlgExportBspAttributeMap::OnBspattributemapOpen() 
{
	UpdateData( TRUE );

	CTFileName fnAttrinuteMap=_EngineGUI.FileRequester(
		"Export bsp attribute map", FILTER_SAT FILTER_ALL FILTER_END,
		"Bsp attribute map directory", "Data\\World\\" );
	if( fnAttrinuteMap == "" ) return;
	try
	{
		m_fnFileName = fnAttrinuteMap;
		m_strFileName = fnAttrinuteMap;
		CMainFrame* pMainFrame = STATIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
		pMainFrame->Invalidate( FALSE );
	}
	catch(char *strError)
	{
		AfxMessageBox( strError );
	}

	UpdateData( FALSE );
}

void CDlgExportBspAttributeMap::OnOK() 
{
	UpdateData( TRUE );
	
	CDialog::OnOK();
}

void CDlgExportBspAttributeMap::OnButtonPriority() 
{
	// TODO: Add your control notification handler code here
	CDlgSetPriority Dlg;
	Dlg.SetPriorityList(m_vectorPriority);
	if( Dlg.DoModal() == IDOK )
	{
		Dlg.GetPriorityList(m_vectorPriority);		
	}
}

BOOL CDlgExportBspAttributeMap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i = 0; i < ATTC_TOTAL_COUNT; ++i )
	{
		m_vectorPriority.push_back(_aDefaultAttributeType[i]);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
