// TriangularisationCombo.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriangularisationCombo

CTriangularisationCombo::CTriangularisationCombo()
{
}

CTriangularisationCombo::~CTriangularisationCombo()
{
}


BEGIN_MESSAGE_MAP(CTriangularisationCombo, CComboBox)
	//{{AFX_MSG_MAP(CTriangularisationCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriangularisationCombo message handlers

BOOL CTriangularisationCombo::OnIdle(LONG lCount)
{
  // get document ptr
  CWorldEditorDoc* pDoc = theApp.GetActiveDocument();

  if( (pDoc == NULL) ||
      (pDoc->GetEditingMode() != CSG_MODE) ||
      (!pDoc->m_bPrimitiveMode) )
  {
    // we should disable triangularisation combo
    if(GetCount() == 1) return TRUE; // we allready have "not available" in combo
    // remove all combo entries
    ResetContent();
    // set none available message
    AddString( "None Available");
    SetCurSel( 0);
  }
  // we should enable triangularisation combo 
  else
  {
    // if it is allready enabled
    if(GetCount() != 1) return TRUE;
    ResetContent();
    // add all possible triangularisation types
    AddString( "None");
    AddString( "Center");
    AddString( "Vertex 1");
    AddString( "Vertex 2");
    AddString( "Vertex 3");
    AddString( "Vertex 4");
    AddString( "Vertex 5");
    AddString( "Vertex 6");
    AddString( "Vertex 7");
    AddString( "Vertex 8");
    AddString( "Vertex 9");
    AddString( "Vertex 10");
    AddString( "Vertex 11");
    AddString( "Vertex 12");
    AddString( "Vertex 13");
    AddString( "Vertex 14");
    AddString( "Vertex 15");
    AddString( "Vertex 16");
    // select currently selected triangularisation type
    SetCurSel( (int)theApp.m_vfpCurrent.vfp_ttTriangularisationType);
  }
  return TRUE;
}

void CTriangularisationCombo::OnSelchange() 
{
  // get document ptr
  CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
  if( pDoc == NULL) return;

  int iSelected = GetCurSel();
  theApp.m_vfpCurrent.vfp_ttTriangularisationType = (enum TriangularisationType) iSelected;
  pDoc->CreatePrimitive();
  pDoc->UpdateAllViews(NULL);
}

void CTriangularisationCombo::OnDropdown() 
{
  INDEX ctItems = GetCount();
  if( ctItems == CB_ERR) return;
  
  CRect rectCombo;
  GetWindowRect( &rectCombo);
  
  PIX pixScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
  PIX pixMaxHeight = pixScreenHeight - rectCombo.top;

  CWnd *pwndParent = GetParent();
  if( pwndParent == NULL) return;
  pwndParent->ScreenToClient( &rectCombo);
  PIX pixNewHeight = GetItemHeight(0)*(ctItems+2);
  rectCombo.bottom = rectCombo.top + ClampUp( pixNewHeight, pixMaxHeight);
  MoveWindow( rectCombo);
}
