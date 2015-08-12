// DlgPgAttribute.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPgAttribute.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPgAttribute property page

IMPLEMENT_DYNCREATE(CDlgPgAttribute, CPropertyPage)

CDlgPgAttribute::CDlgPgAttribute() : CPropertyPage(CDlgPgAttribute::IDD)
{
	//{{AFX_DATA_INIT(CDlgPgAttribute)
	m_nAttribute = 0;
	//}}AFX_DATA_INIT
}

CDlgPgAttribute::~CDlgPgAttribute()
{
}

void CDlgPgAttribute::DoDataExchange(CDataExchange* pDX)
{
	if( theApp.m_bDisableDataExchange ) return ;

	CPropertyPage::DoDataExchange( pDX );
	 // mark that property page has been modified
	SetModified( TRUE );

	// obtain document
	CWorldEditorDoc*	pDoc = theApp.GetDocument();
	if( pDoc == NULL ) return ;
	// polygon mode must be on
	if( pDoc->GetEditingMode() != POLYGON_MODE )  return;
	// get flags of control activity
	BOOL	bSelectionExists = pDoc->m_selPolygonSelection.Count() != 0;

	// if dialog is recieving data and control window is valid
	if( pDX->m_bSaveAndValidate == FALSE )
	{
		GetDlgItem( IDC_PG_ATTRIBUTE0 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE1 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE2 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE3 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE4 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE5 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE6 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE7 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE8 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE9 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE10 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE11 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE12 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE13 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE14 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE15 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE16 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE17 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE18 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE19 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE20 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE21 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE22 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE23 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE24 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE25 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE26 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE27 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE28 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE29 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE30 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE31 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE32 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE33 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE34 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE35 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE36 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE37 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE38 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE39 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE40 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE41 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE42 )->EnableWindow( bSelectionExists );
		GetDlgItem( IDC_PG_ATTRIBUTE43 )->EnableWindow( bSelectionExists );

		// if selection exists
		if( bSelectionExists )
		{
			UBYTE	ubPolygonAttribute = pDoc->m_selPolygonSelection[0].bpo_ubPolygonAttribute;
			if( ubPolygonAttribute >= 81 )
				ubPolygonAttribute -= 42;
			else if( ubPolygonAttribute >= 71 )
				ubPolygonAttribute -= 37;
			else if( ubPolygonAttribute >= 61 )
				ubPolygonAttribute -= 32;
			else if( ubPolygonAttribute >= 51 )
				ubPolygonAttribute -= 27;
			else if( ubPolygonAttribute >= 41 )
				ubPolygonAttribute -= 22;
			else if( ubPolygonAttribute >= 31 )
				ubPolygonAttribute -= 16;
			else if( ubPolygonAttribute >= 21 )
				ubPolygonAttribute -= 10;
			else if( ubPolygonAttribute >= 11 )
				ubPolygonAttribute -= 5;

			m_nAttribute = ubPolygonAttribute;
		}

		// mark that page is updated
		m_udPolygonSelection.MarkUpdated();
	}

	//{{AFX_DATA_MAP(CDlgPgAttribute)
	DDX_Radio(pDX, IDC_PG_ATTRIBUTE0, m_nAttribute);
	//}}AFX_DATA_MAP

	// if dialog is giving data
	if( pDX->m_bSaveAndValidate != FALSE )
	{
		UBYTE	ubPolygonAttribute;
		if( m_nAttribute > 38 ) 
			ubPolygonAttribute = m_nAttribute + 42;
		else if( m_nAttribute > 33 ) 
			ubPolygonAttribute = m_nAttribute + 37;
		else if( m_nAttribute > 28 )
			ubPolygonAttribute = m_nAttribute + 32;
		else if( m_nAttribute > 23 )
			ubPolygonAttribute = m_nAttribute + 27;
		else if( m_nAttribute > 18 )
			ubPolygonAttribute = m_nAttribute + 22;
		else if( m_nAttribute > 14 )
			ubPolygonAttribute = m_nAttribute + 16;
		else if( m_nAttribute > 10 )
			ubPolygonAttribute = m_nAttribute + 10;
		else if( m_nAttribute > 5 )
			ubPolygonAttribute = m_nAttribute + 5;
		else
			ubPolygonAttribute = m_nAttribute;

		FOREACHINDYNAMICCONTAINER( pDoc->m_selPolygonSelection, CBrushPolygon, itbpo )
		{
			itbpo->bpo_ubPolygonAttribute = ubPolygonAttribute;
		}

		// mark that document is changed
		theApp.GetDocument()->SetModifiedFlag( TRUE );
		// redraw to show changes
		pDoc->UpdateAllViews( NULL );
	}
}


BEGIN_MESSAGE_MAP(CDlgPgAttribute, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPgAttribute)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE0, OnPgAttribute0)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE1, OnPgAttribute1)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE2, OnPgAttribute2)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE3, OnPgAttribute3)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE4, OnPgAttribute4)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE5, OnPgAttribute5)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE6, OnPgAttribute6)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE7, OnPgAttribute7)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE8, OnPgAttribute8)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE9, OnPgAttribute9)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE10, OnPgAttribute10)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE11, OnPgAttribute11)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE12, OnPgAttribute12)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE13, OnPgAttribute13)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE14, OnPgAttribute14)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE15, OnPgAttribute15)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE16, OnPgAttribute16)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE17, OnPgAttribute17)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE18, OnPgAttribute18)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE19, OnPgAttribute19)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE20, OnPgAttribute20)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE21, OnPgAttribute21)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE22, OnPgAttribute22)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE23, OnPgAttribute23)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE24, OnPgAttribute24)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE25, OnPgAttribute25)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE26, OnPgAttribute26)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE27, OnPgAttribute27)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE28, OnPgAttribute28)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE29, OnPgAttribute29)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE30, OnPgAttribute30)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE31, OnPgAttribute31)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE32, OnPgAttribute32)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE33, OnPgAttribute33)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE34, OnPgAttribute34)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE35, OnPgAttribute35)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE36, OnPgAttribute36)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE37, OnPgAttribute37)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE38, OnPgAttribute38)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE39, OnPgAttribute39)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE40, OnPgAttribute40)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE41, OnPgAttribute41)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE42, OnPgAttribute42)
	ON_BN_CLICKED(IDC_PG_ATTRIBUTE43, OnPgAttribute43)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPgAttribute message handlers

BOOL CDlgPgAttribute::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;
}

BOOL CDlgPgAttribute::OnIdle( LONG lCount )
{
	CWorldEditorDoc*	pDoc = theApp.GetDocument();
	if( ( pDoc == NULL ) || !IsWindow( m_hWnd ) )
	{
		return TRUE;
	}
  
	// if selections have been changed (they are not up to date)
	if( !pDoc->m_chSelections.IsUpToDate( m_udPolygonSelection ) )
	{
		// update dialog data
		UpdateData( FALSE );
	}
	return TRUE;
}

BOOL CDlgPgAttribute::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		// move data from page to polygon
		UpdateData( TRUE);
		// the message is handled
		return TRUE;
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CDlgPgAttribute::OnPgAttribute0() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute1() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute2() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute3() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute4() 
{
	UpdateData( TRUE );	
}
void CDlgPgAttribute::OnPgAttribute5() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute6() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute7() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute8() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute9() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute10() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute11() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute12() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute13() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute14() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute15() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute16() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute17() 
{
	UpdateData( TRUE );
}
void CDlgPgAttribute::OnPgAttribute18() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute19() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute20() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute21() 
{
	UpdateData( TRUE );	
}

void CDlgPgAttribute::OnPgAttribute22() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute23() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute24() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute25() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute26() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute27() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute28() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute29() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute30() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute31() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute32() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute33() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute34() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute35() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute36() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute37() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute38() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute39() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute40() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute41() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute42() 
{
	UpdateData( TRUE );
}

void CDlgPgAttribute::OnPgAttribute43() 
{
	UpdateData( TRUE );
}
