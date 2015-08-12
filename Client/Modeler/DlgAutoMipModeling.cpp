// DlgAutoMipModeling.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoMipModeling dialog


CDlgAutoMipModeling::CDlgAutoMipModeling(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoMipModeling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoMipModeling)
	m_iVerticesToRemove = 0;
	m_iSurfacePreservingFactor = 0;
	//}}AFX_DATA_INIT

  m_iVerticesToRemove = theApp.GetProfileInt( "Modeler prefs", "Auto mip modeling verex removal rate", 10);
  m_iSurfacePreservingFactor = theApp.GetProfileInt( "Modeler prefs", "Surface preserving factor", 30);
}


void CDlgAutoMipModeling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoMipModeling)
	DDX_Text(pDX, IDC_VERTICES_TO_REMOVE, m_iVerticesToRemove);
	DDV_MinMaxInt(pDX, m_iVerticesToRemove, 1, 32767);
	DDX_Text(pDX, IDC_SURFACE_PRESERVING_FACTOR, m_iSurfacePreservingFactor);
	DDV_MinMaxInt(pDX, m_iSurfacePreservingFactor, 1, 99);
	//}}AFX_DATA_MAP

  // if dialog gives data
  if( pDX->m_bSaveAndValidate)
  {
    theApp.WriteProfileInt( "Modeler prefs", "Auto mip modeling verex removal rate", m_iVerticesToRemove);
    theApp.WriteProfileInt( "Modeler prefs", "Surface preserving factor", m_iSurfacePreservingFactor);
  }
}


BEGIN_MESSAGE_MAP(CDlgAutoMipModeling, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoMipModeling)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoMipModeling message handlers
