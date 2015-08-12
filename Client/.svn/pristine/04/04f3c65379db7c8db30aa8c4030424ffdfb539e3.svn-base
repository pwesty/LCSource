// DlgCCylinder.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCCylinder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCCylinder dialog


CDlgCCylinder::CDlgCCylinder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCCylinder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCCylinder)
	m_fHeight = 0.0f;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_strInner = _T("");
	m_fInner = 0.0f;
	m_fOuter = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCCylinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCCylinder)
	DDX_Control(pDX, IDC_STATIC_OUTER, m_stOuter);
	DDX_Control(pDX, IDC_CYLINDER_OUTER_RADIUS, m_edOuter);
	DDX_Text(pDX, IDC_CYLINDER_HEIGHT, m_fHeight);
	DDX_Text(pDX, IDC_CYLINDER_X, m_fX);
	DDX_Text(pDX, IDC_CYLINDER_Y, m_fY);
	DDX_Text(pDX, IDC_CYLINDER_Z, m_fZ);
	DDX_Text(pDX, IDC_STATIC_INNER, m_strInner);
	DDX_Text(pDX, IDC_CYLINDER_INNER_RADIUS, m_fInner);
	DDX_Text(pDX, IDC_CYLINDER_OUTER_RADIUS, m_fOuter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCCylinder, CDialog)
	//{{AFX_MSG_MAP(CDlgCCylinder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCCylinder message handlers

BOOL CDlgCCylinder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bDoubleSpace)
	{
		m_strInner = "InnerRadius";
		m_edOuter.ShowWindow(TRUE);
		m_stOuter.ShowWindow(TRUE);
	}
	else
	{
		m_strInner = "Radius";
		m_edOuter.ShowWindow(FALSE);
		m_stOuter.ShowWindow(FALSE);
	}
	UpdateData(SAVETOFORM);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
