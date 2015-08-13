// DlgCSphere.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCSphere.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCSphere dialog


CDlgCSphere::CDlgCSphere(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCSphere::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSphere)
	m_fOuterRadius = 0.0f;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fInnerRadius = 0.0f;
	m_strInner = _T("");
	//}}AFX_DATA_INIT
	m_bDoubleSpace = FALSE;
}


void CDlgCSphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSphere)
	DDX_Control(pDX, IDC_SPHERE_OUTER_RADIUS, m_edOuter);
	DDX_Control(pDX, IDC_STATIC_OUTER, m_stOuter);
	DDX_Text(pDX, IDC_SPHERE_OUTER_RADIUS, m_fOuterRadius);
	DDX_Text(pDX, IDC_SPHERE_X, m_fX);
	DDX_Text(pDX, IDC_SPHERE_Y, m_fY);
	DDX_Text(pDX, IDC_SPHERE_Z, m_fZ);
	DDX_Text(pDX, IDC_SPHERE_INNER_RADIUS, m_fInnerRadius);
	DDX_Text(pDX, IDC_STATIC_INNER, m_strInner);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSphere, CDialog)
	//{{AFX_MSG_MAP(CDlgCSphere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSphere message handlers

BOOL CDlgCSphere::OnInitDialog() 
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
