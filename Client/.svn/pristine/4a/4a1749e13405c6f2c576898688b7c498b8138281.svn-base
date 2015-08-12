// DlgSetPriority.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "DlgSetPriority.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString _aStrFieldType[ATTC_TOTAL_COUNT] = 
{ _T("Walkable"), _T("Unwalkable"), _T("Stair"), _T("Wall of stair"), _T("Peace"), 
_T("Product Public"), _T("Product Private"), _T("War Area"), _T("Free PK") };

/////////////////////////////////////////////////////////////////////////////
// CDlgSetPriority dialog

CDlgSetPriority::CDlgSetPriority(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetPriority::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetPriority)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSetPriority::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetPriority)
	DDX_Control(pDX, IDC_PRIORITY_SPIN, m_ctlSpinPriority);
	DDX_Control(pDX, IDC_LIST_PRIORITY, m_lbPriority);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetPriority, CDialog)
	//{{AFX_MSG_MAP(CDlgSetPriority)
	ON_LBN_SELCHANGE(IDC_LIST_PRIORITY, OnSelchangeListPriority)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRIORITY_SPIN, OnDeltaposPrioritySpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetPriority message handlers

void CDlgSetPriority::OnSelchangeListPriority() 
{
	// TODO: Add your control notification handler code here
	int iCurSel = m_lbPriority.GetCurSel();
	CRect rcItemRect;
	CRect rcListRect;
	CRect rcSpinRect;
	m_lbPriority.GetWindowRect(rcListRect);
	ScreenToClient(rcListRect);
	m_lbPriority.GetItemRect(iCurSel, rcItemRect);
	m_ctlSpinPriority.GetWindowRect(rcSpinRect);	
	m_ctlSpinPriority.SetWindowPos(NULL, rcListRect.right , rcListRect.top + rcItemRect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
	m_ctlSpinPriority.ShowWindow(SW_SHOW);
}

BOOL CDlgSetPriority::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Font.CreatePointFont(150, "±¼¸²");
	m_lbPriority.SetFont(&m_Font);
	
	RefreshPriorityList();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetPriority::SetPriorityList(std::vector<int> vectorList)
{
	m_vectorPriority = vectorList;
}

void CDlgSetPriority::GetPriorityList(std::vector<int>& vectorList)
{
	vectorList = m_vectorPriority;
}

void CDlgSetPriority::RefreshPriorityList()
{
	m_lbPriority.ResetContent();	
	std::vector<int>::iterator it;
	std::vector<int>::iterator itend = m_vectorPriority.end();
	int i = 0;
	for( it = m_vectorPriority.begin() ; it != itend; ++it, ++i )
	{		
		m_lbPriority.InsertString(i, _aStrFieldType[(*it)]);
	}
}

void CDlgSetPriority::OnDeltaposPrioritySpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	int iOldPos = m_lbPriority.GetCurSel();
	int iNewPos = iOldPos;
	if( 0 < pNMUpDown->iDelta )
	{
		iNewPos = iOldPos+1;
	}
	else
	{
		iNewPos = iOldPos-1;		
	}
	if( iNewPos < 0 || iNewPos >= m_vectorPriority.size())
		return;
	SwapPriority( iOldPos, iNewPos );
	
	RefreshPriorityList();
	m_lbPriority.SetCurSel(iNewPos);
	OnSelchangeListPriority();		
	
	*pResult = 0;
}

void CDlgSetPriority::SwapPriority( int iOld, int iNew )
{
	int iTemp = m_vectorPriority[iOld];
	m_vectorPriority[iOld] = m_vectorPriority[iNew];
	m_vectorPriority[iNew] = iTemp;
}