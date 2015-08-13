// DlgTerrainSound.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "DlgTerrainSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainSound dialog


CDlgTerrainSound::CDlgTerrainSound(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTerrainSound::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTerrainSound)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSoundIndex = 0;
}


void CDlgTerrainSound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTerrainSound)
	DDX_Control(pDX, IDC_TERRAIN_SURFACE_TYPE, m_ctrlSurfaceType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTerrainSound, CDialog)
	//{{AFX_MSG_MAP(CDlgTerrainSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainSound message handlers

BOOL CDlgTerrainSound::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTerrain*	ptrTerrain = GetTerrain();
	if( ptrTerrain != NULL )
	{
		CTerrainLayer	*ptlLayer = GetLayer();
		if( ptlLayer!= NULL )
		{
			m_ctrlSurfaceType.AddString( "Dirt" );
			m_ctrlSurfaceType.AddString( "Grass" );
			m_ctrlSurfaceType.AddString( "Rock" );
			m_ctrlSurfaceType.AddString( "Block" );

			m_nSoundIndex = ptlLayer->tl_iSoundIndex;
			m_ctrlSurfaceType.SetCurSel( m_nSoundIndex );
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTerrainSound::OnOK() 
{
	m_nSoundIndex = m_ctrlSurfaceType.GetCurSel();
	
	CDialog::OnOK();
}
