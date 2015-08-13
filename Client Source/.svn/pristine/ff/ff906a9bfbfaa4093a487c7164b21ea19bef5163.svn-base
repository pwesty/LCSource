// DlgCPPDSOffsetPosAutoSetting.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPDSOffsetPosAutoSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDSOffsetPosAutoSetting dialog


CDlgCPPDSOffsetPosAutoSetting::CDlgCPPDSOffsetPosAutoSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPDSOffsetPosAutoSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPDSOffsetPosAutoSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCPPDSOffsetPosAutoSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPDSOffsetPosAutoSetting)
	DDX_Control(pDX, IDC_CB_TYPE, m_cbType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPDSOffsetPosAutoSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPDSOffsetPosAutoSetting)
	ON_CBN_SELENDOK(IDC_CB_TYPE, OnSelendokCbType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDSOffsetPosAutoSetting message handlers
enum OFFSET_AUTOSETTING_TYPE
{
	OAT_FEATHER_FALL = 0,
};

void CDlgCPPDSOffsetPosAutoSetting::OnSelendokCbType() 
{
	// TODO: Add your control notification handler code here
	m_dlgAutosettingFeatherFall.ShowWindow(FALSE);

	switch(m_cbType.GetCurSel())
	{
	case OAT_FEATHER_FALL:
		{
			m_dlgAutosettingFeatherFall.ShowWindow(TRUE);
		} break;
	}
}

BOOL CDlgCPPDSOffsetPosAutoSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbType.AddString("Feather Fall");

	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	//feather fall
	m_dlgAutosettingFeatherFall.Create(IDD_AUTOSETTING_FEATHER_FALL, this);
	m_dlgAutosettingFeatherFall.GetClientRect(&rectChild);
	m_dlgAutosettingFeatherFall.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgAutosettingFeatherFall.ShowWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCPPDSOffsetPosAutoSetting::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pPPDS == NULL)
	{
		CDialog::OnOK();
		return;
	}

	m_dlgAutosettingFeatherFall.FillSettingInfo(*m_pPPDS);

	CDialog::OnOK();
}
