// DlgTerrainShadowTime.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgTerrainShadowTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainShadowTime dialog


CDlgTerrainShadowTime::CDlgTerrainShadowTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTerrainShadowTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTerrainShadowTime)
	m_strResultTime = _T("");
	m_nHour = 0;
	m_nMinute = 0;
	m_nSecond = 0;
	//}}AFX_DATA_INIT
}


void CDlgTerrainShadowTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTerrainShadowTime)
	DDX_Text(pDX, IDC_TERRAIN_RESULT_TIME, m_strResultTime);
	DDX_Text(pDX, IDC_TERRAIN_SHADOW_HOUR, m_nHour);
	DDV_MinMaxInt(pDX, m_nHour, 0, 29);
	DDX_Text(pDX, IDC_TERRAIN_SHADOW_MIN, m_nMinute);
	DDV_MinMaxInt(pDX, m_nMinute, 0, 59);
	DDX_Text(pDX, IDC_TERRAIN_SHADOW_SEC, m_nSecond);
	DDV_MinMaxInt(pDX, m_nSecond, 0, 59);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTerrainShadowTime, CDialog)
	//{{AFX_MSG_MAP(CDlgTerrainShadowTime)
	ON_EN_CHANGE(IDC_TERRAIN_SHADOW_HOUR, OnChangeTerrainShadowHour)
	ON_EN_CHANGE(IDC_TERRAIN_SHADOW_MIN, OnChangeTerrainShadowMin)
	ON_EN_CHANGE(IDC_TERRAIN_SHADOW_SEC, OnChangeTerrainShadowSec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainShadowTime message handlers

BOOL CDlgTerrainShadowTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strResultTime = _T("0 seconds");
	m_nHour = 0;
	m_nMinute = 0;
	m_nSecond = 0;
	m_ulWorldTime = 0;

	CTerrain*	ptrTerrain = GetTerrain();
	if( ptrTerrain != NULL )
	{
		INDEX	iSelection = GetShadowMapIndex();
		ULONG	ulShadowTime = ptrTerrain->tr_ptrTerrain->tr_aulShadowTimes[iSelection];
		m_ulWorldTime = ulShadowTime;
		m_nHour = ulShadowTime / 3600;
		ulShadowTime -= m_nHour * 3600;
		m_nMinute = ulShadowTime / 60;
		ulShadowTime -= m_nMinute * 60;
		m_nSecond = ulShadowTime;
		m_strResultTime.Format( _T("%d seconds"), m_ulWorldTime );
	}

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTerrainShadowTime::OnChangeTerrainShadowHour() 
{
	int	nOldHour = m_nHour;

	UpdateData( TRUE );	
	if( m_nHour < 0 || m_nHour > 29 )
		m_nHour = nOldHour;

	m_ulWorldTime = m_nHour * 3600 + m_nMinute * 60 + m_nSecond;
	m_strResultTime.Format( _T("%d seconds"), m_ulWorldTime );

	UpdateData( FALSE );
}

void CDlgTerrainShadowTime::OnChangeTerrainShadowMin() 
{
	int	nOldMinute = m_nMinute;

	UpdateData( TRUE );	
	if( m_nMinute < 0 || m_nMinute > 59 )
		m_nMinute = nOldMinute;

	m_ulWorldTime = m_nHour * 3600 + m_nMinute * 60 + m_nSecond;
	m_strResultTime.Format( _T("%d seconds"), m_ulWorldTime );

	UpdateData( FALSE );
}

void CDlgTerrainShadowTime::OnChangeTerrainShadowSec() 
{
	int	nOldSecond = m_nSecond;

	UpdateData( TRUE );	
	if( m_nSecond < 0 || m_nSecond > 59 )
		m_nSecond = nOldSecond;

	m_ulWorldTime = m_nHour * 3600 + m_nMinute * 60 + m_nSecond;
	m_strResultTime.Format( _T("%d seconds"), m_ulWorldTime );

	UpdateData( FALSE );
}
