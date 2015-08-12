// DlgCCone.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCCone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCCone dialog


CDlgCCone::CDlgCCone(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCCone::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCCone)
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_strHotspot = _T("");
	m_fHotspot = 0.0f;
	m_fFalloff = 30.0f;
	m_fHeightLower = 0.0f;
	m_fHeightUpper = 1.0f;
	m_fLerpRatio = 0.0f;
	m_fCloseRange = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCCone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCCone)
	DDX_Control(pDX, IDC_CONE_FALLOFF_ANGLE, m_edFalloff);
	DDX_Control(pDX, IDC_STATIC_FALLOFF, m_stFalloff);
	DDX_Text(pDX, IDC_CONE_X, m_fX);
	DDX_Text(pDX, IDC_CONE_Y, m_fY);
	DDX_Text(pDX, IDC_CONE_Z, m_fZ);
	DDX_Text(pDX, IDC_STATIC_HOTSPOT, m_strHotspot);
	DDX_Text(pDX, IDC_CONE_HOTSPOT_ANGLE, m_fHotspot);
	DDX_Text(pDX, IDC_CONE_FALLOFF_ANGLE, m_fFalloff);
	DDX_Text(pDX, IDC_CONE_HEIGHT_LOWER, m_fHeightLower);
	DDX_Text(pDX, IDC_CONE_HEIGHT_UPPER, m_fHeightUpper);
	DDX_Text(pDX, IDC_CONE_LERP_RATIO, m_fLerpRatio);
	DDV_MinMaxFloat(pDX, m_fLerpRatio, 0.f, 1.f);
	DDX_Text(pDX, IDC_CONE_CLOSE_RANGE, m_fCloseRange);
	DDV_MinMaxFloat(pDX, m_fCloseRange, 0.f, 99999.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCCone, CDialog)
	//{{AFX_MSG_MAP(CDlgCCone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCCone message handlers

BOOL CDlgCCone::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bDoubleSpace)
	{
		m_strHotspot = "Hotspot";
		m_edFalloff.ShowWindow(TRUE);
		m_stFalloff.ShowWindow(TRUE);
	}
	else
	{
		m_strHotspot = "Angle";
		m_edFalloff.ShowWindow(FALSE);
		m_stFalloff.ShowWindow(FALSE);
	}
	UpdateData(SAVETOFORM);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
