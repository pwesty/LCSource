// ZoneFlagDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "ZoneFlagDialog.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// FIXME : 리소스 ID가 연속적이지 않음.
int _aiZoneResourceID[] =
{
	IDC_ZONE01,
	IDC_ZONE02,
	IDC_ZONE03,
	IDC_ZONE04,
	IDC_ZONE05,
	IDC_ZONE06,
	IDC_ZONE07,
	IDC_ZONE08,
	IDC_ZONE09,
	IDC_ZONE10,
	IDC_ZONE11,
	IDC_ZONE12,
	IDC_ZONE13,
	IDC_ZONE14,
	IDC_ZONE15,
	IDC_ZONE16,
	IDC_ZONE17,
	IDC_ZONE18,
	IDC_ZONE19,
	IDC_ZONE20,
	IDC_ZONE21,
	IDC_ZONE22,
	IDC_ZONE23,
	IDC_ZONE24,
	IDC_ZONE25,
	IDC_ZONE26,
	IDC_ZONE27,
	IDC_ZONE28,
	IDC_ZONE29,
	IDC_ZONE30,
	IDC_ZONE31,
	IDC_ZONE32,
	IDC_ZONE33,
	IDC_ZONE34,
	IDC_ZONE35,
	IDC_ZONE36,
	IDC_ZONE37,
	IDC_ZONE38,
	IDC_ZONE39,
	IDC_ZONE40,
	IDC_ZONE41,
	IDC_ZONE42,
	IDC_ZONE43,
	IDC_ZONE44,
	IDC_ZONE45,
	IDC_ZONE46,
	IDC_ZONE47,
	IDC_ZONE48,
	IDC_ZONE49,
	IDC_ZONE50,
	IDC_ZONE51,
	IDC_ZONE52,
	IDC_ZONE53,
	IDC_ZONE54,
	IDC_ZONE55,
	IDC_ZONE56,
	IDC_ZONE57,
	IDC_ZONE58,
	IDC_ZONE59,
	IDC_ZONE60,
	IDC_ZONE61,
	IDC_ZONE62,
	IDC_ZONE63,
	IDC_ZONE64,
};

int _aiExtraResourceID[] =
{
	IDC_EXTRA01,
	IDC_EXTRA02,
	IDC_EXTRA03,
	IDC_EXTRA04,
	IDC_EXTRA05,
	IDC_EXTRA06,
	IDC_EXTRA07,
	IDC_EXTRA08,
	IDC_EXTRA09,
	IDC_EXTRA10,
	IDC_EXTRA11,
	IDC_EXTRA12,
	IDC_EXTRA13,
	IDC_EXTRA14,
	IDC_EXTRA15,
	IDC_EXTRA16,
	IDC_EXTRA17,
	IDC_EXTRA18,
	IDC_EXTRA19,
	IDC_EXTRA20,
	IDC_EXTRA21,
	IDC_EXTRA22,
	IDC_EXTRA23,
	IDC_EXTRA24,
	IDC_EXTRA25,
	IDC_EXTRA26,
	IDC_EXTRA27,
	IDC_EXTRA28,
	IDC_EXTRA29,
	IDC_EXTRA30,
	IDC_EXTRA31,
	IDC_EXTRA32,
	IDC_EXTRA33,
	IDC_EXTRA34,
	IDC_EXTRA35,
	IDC_EXTRA36,
	IDC_EXTRA37,
	IDC_EXTRA38,
	IDC_EXTRA39,
	IDC_EXTRA40,
	IDC_EXTRA41,
	IDC_EXTRA42,
	IDC_EXTRA43,
	IDC_EXTRA44,
	IDC_EXTRA45,
	IDC_EXTRA46,
	IDC_EXTRA47,
	IDC_EXTRA48,
	IDC_EXTRA49,
	IDC_EXTRA50,
	IDC_EXTRA51,
	IDC_EXTRA52,
	IDC_EXTRA53,
	IDC_EXTRA54,
	IDC_EXTRA55,
	IDC_EXTRA56,
	IDC_EXTRA57,
	IDC_EXTRA58,
	IDC_EXTRA59,
	IDC_EXTRA60,
	IDC_EXTRA61,
	IDC_EXTRA62,
	IDC_EXTRA63,
	IDC_EXTRA64,
};

/////////////////////////////////////////////////////////////////////////////
// CZoneFlagDialog dialog
CZoneFlagDialog::CZoneFlagDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CZoneFlagDialog::IDD, pParent), m_ulZoneFlag(0), m_ulExtraFlag(0)
{
	//{{AFX_DATA_INIT(CZoneFlagDialog)
	//}}AFX_DATA_INIT
}


void CZoneFlagDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoneFlagDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoneFlagDialog, CDialog)
	//{{AFX_MSG_MAP(CZoneFlagDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoneFlagDialog message handlers

BOOL CZoneFlagDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetZoneFlag(m_ulZoneFlag);
	SetExtraFlag(m_ulExtraFlag);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : ULONG
//-----------------------------------------------------------------------------
ZONEFLAGS CZoneFlagDialog::GetZoneFlag()
{
	return m_ulZoneFlag;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : ulZoneFlag - 
//-----------------------------------------------------------------------------
void CZoneFlagDialog::SetZoneFlag(ZONEFLAGS ulZoneFlag)
{
	const int iNumOfID = sizeof(_aiZoneResourceID) / sizeof(int);
	for(int i = 0; i < iNumOfID; ++i)
	{
		CButton* pBtn	= static_cast<CButton*>(GetDlgItem(_aiZoneResourceID[i]));
		BOOL bChecked	= (m_ulZoneFlag & (((ZONEFLAGS)1) << i))>((ZONEFLAGS)0)? 1 : 0;
		pBtn->SetCheck(bChecked);
	}
}

void CZoneFlagDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_ulZoneFlag	= 0;
	m_ulExtraFlag	= 0;

	{
		const int iNumOfID = sizeof(_aiZoneResourceID) / sizeof(int);
		for(int i = 0; i < iNumOfID; ++i)
		{
			CButton* pBtn		= static_cast<CButton*>(GetDlgItem(_aiZoneResourceID[i]));
			ZONEFLAGS ulValue	= (ZONEFLAGS)(((ZONEFLAGS)pBtn->GetCheck()) << i);
			m_ulZoneFlag		|= ulValue;		
		}
	}
	{
		const int iNumOfID = sizeof(_aiExtraResourceID) / sizeof(int);
		for(int i = 0; i < iNumOfID; ++i)
		{
			CButton* pBtn		= static_cast<CButton*>(GetDlgItem(_aiExtraResourceID[i]));
			ZONEFLAGS ulValue	= (ZONEFLAGS)(((ZONEFLAGS)pBtn->GetCheck()) << i);
			m_ulExtraFlag		|= ulValue;		
		}
	}

	CDialog::OnOK();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : ULONG
//-----------------------------------------------------------------------------
ZONEFLAGS CZoneFlagDialog::GetExtraFlag()
{
	return m_ulExtraFlag;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : ulExtraFlag - 
//-----------------------------------------------------------------------------
void CZoneFlagDialog::SetExtraFlag(ZONEFLAGS ulExtraFlag)
{
	const int iNumOfID = sizeof(_aiExtraResourceID) / sizeof(int);
	for(int i = 0; i < iNumOfID; ++i)
	{
		CButton* pBtn	= static_cast<CButton*>(GetDlgItem(_aiExtraResourceID[i]));
		BOOL bChecked	= (m_ulExtraFlag & (((ZONEFLAGS)1) << i))>((ZONEFLAGS)0)? 1 : 0;
		pBtn->SetCheck(bChecked);
	}
}