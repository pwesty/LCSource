// DlgUpdateTerrainShadow.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgUpdateTerrainShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateTerrainShadow dialog


CDlgUpdateTerrainShadow::CDlgUpdateTerrainShadow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdateTerrainShadow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUpdateTerrainShadow)
	m_strBlurRadius = _T("0.0 pixel");
	m_bObjShadowSka = FALSE;
	m_bObjShadowMdl = FALSE;
	m_bObjShadowBrush = FALSE;
	m_bObjShadowTerrain = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgUpdateTerrainShadow::DoDataExchange(CDataExchange* pDX)
{
	CTerrain*	ptrTerrain = GetTerrain();
	if( ptrTerrain == NULL )
		return;

	if( pDX->m_bSaveAndValidate == FALSE && IsWindow( m_ctrlBlurRadius ) )
	{
		INDEX	iPos = m_ctrlBlurRadius.GetPos();
		FLOAT	fRadius = iPos / 10.0f;
		m_strBlurRadius.Format( _T("%.1f pixel"), fRadius );
	}

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpdateTerrainShadow)
	DDX_Control(pDX, IDC_BLUR_RADIUS_SLIDER, m_ctrlBlurRadius);
	DDX_Text(pDX, IDC_BLUR_RADIUS, m_strBlurRadius);
	DDX_Check(pDX, IDC_OBJ_SHADOW_SKA, m_bObjShadowSka);
	DDX_Check(pDX, IDC_OBJ_SHADOW_MDL, m_bObjShadowMdl);
	DDX_Check(pDX, IDC_OBJ_SHADOW_BRUSH, m_bObjShadowBrush);
	DDX_Check(pDX, IDC_OBJ_SHADOW_TERRAIN, m_bObjShadowTerrain);
	//}}AFX_DATA_MAP

	if( pDX->m_bSaveAndValidate != FALSE )
	{
		m_iSliderPos = m_ctrlBlurRadius.GetPos();
	}
}


BEGIN_MESSAGE_MAP(CDlgUpdateTerrainShadow, CDialog)
	//{{AFX_MSG_MAP(CDlgUpdateTerrainShadow)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OBJ_SHADOW_COLOR, OnObjShadowColor)
	ON_BN_CLICKED(IDC_OBJ_SHADOW_SKA, OnObjShadowSka)
	ON_BN_CLICKED(IDC_OBJ_SHADOW_MDL, OnObjShadowMdl)
	ON_BN_CLICKED(IDC_OBJ_SHADOW_BRUSH, OnObjShadowBrush)
	ON_BN_CLICKED(IDC_OBJ_SHADOW_TERRAIN, OnObjShadowTerrain)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateTerrainShadow message handlers

BOOL CDlgUpdateTerrainShadow::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlBlurRadius.SetRange( 0, 100, TRUE );

	CTerrain*	ptrTerrain = GetTerrain();
	if( ptrTerrain == NULL )
	{
		m_iSliderPos = 0;
		m_ctrlBlurRadius.SetPos( 0 );
		m_colObjShadow.abgr = 0xFF000000;
	}
	else
	{
		INDEX	iSelection = GetShadowMapIndex();
		FLOAT	fRadius = ptrTerrain->tr_ptrTerrain->tr_afShadowBlurRadius[iSelection];
		INDEX	iPos = fRadius * 10;
		m_iSliderPos = iPos;
		m_ctrlBlurRadius.SetPos( iPos );
		m_strBlurRadius.Format( _T("%.1f pixel"), fRadius );

		m_colObjShadow = ptrTerrain->tr_ptrTerrain->tr_acolObjShadowColor[iSelection];

		UpdateData( FALSE );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUpdateTerrainShadow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	
	UpdateData( FALSE );
}

void CDlgUpdateTerrainShadow::OnOK()
{
	// TODO: Add extra validation here
	UpdateData( TRUE );

	CDialog::OnOK();
}

void CDlgUpdateTerrainShadow::OnObjShadowColor()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	COLORREF		color = ( m_colObjShadow.b << 16 ) | ( m_colObjShadow.g << 8 ) | m_colObjShadow.r;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_colObjShadow.abgr = 0xFF000000 | ColorDlg.GetColor();

		UpdateData( FALSE );
		Invalidate( TRUE );
	}
}

void CDlgUpdateTerrainShadow::OnObjShadowSka()
{
	UpdateData( TRUE );	
}

void CDlgUpdateTerrainShadow::OnObjShadowMdl()
{
	UpdateData( TRUE );
}

void CDlgUpdateTerrainShadow::OnObjShadowBrush()
{
	UpdateData( TRUE );
}

void CDlgUpdateTerrainShadow::OnObjShadowTerrain()
{
	UpdateData( TRUE );
}

void CDlgUpdateTerrainShadow::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect		rc;
	CBrush		black( RGB( 0, 0, 0 ) );

	COLORREF	crObjShadow = ( m_colObjShadow.b << 16 ) | ( m_colObjShadow.g << 8 ) | m_colObjShadow.r;
	CBrush		brObjShadow( crObjShadow );
	CPaintDC	dcObjShadow( GetDlgItem( IDC_OBJ_SHADOW_COLOR ) );
	GetDlgItem( IDC_OBJ_SHADOW_COLOR )->GetClientRect( rc );
	dcObjShadow.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcObjShadow.FillRect( &rc, &brObjShadow );
}
