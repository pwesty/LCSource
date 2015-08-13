// DlgCPPControlProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPControlProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPControlProperty dialog


CDlgCPPControlProperty::CDlgCPPControlProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPControlProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPControlProperty)
	m_fAngleSpeed = 0.0f;
	m_fHeightSpeed = 0.0f;
	m_bUsePosition = TRUE;
	//}}AFX_DATA_INIT
}


void CDlgCPPControlProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPControlProperty)
	DDX_Control(pDX, IDC_STATIC_HEIGHTSPEED, m_stHeightSpeed);
	DDX_Control(pDX, IDC_CONTROL_TYPE, m_cbControlType);
	DDX_Control(pDX, IDC_CONTROL_HEIGHTSPEED, m_edHeightSpeed);
	DDX_Text(pDX, IDC_CONTROL_ANGLESPEED, m_fAngleSpeed);
	DDX_Text(pDX, IDC_CONTROL_HEIGHTSPEED, m_fHeightSpeed);
	DDX_Check(pDX, IDC_CHK_CONTROL_USE_POSITION, m_bUsePosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPControlProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPControlProperty)
	ON_CBN_SELENDOK(IDC_CONTROL_TYPE, OnSelendokControlType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPControlProperty message handlers

BOOL CDlgCPPControlProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbControlType.AddString("Circle");
	m_cbControlType.AddString("Spiral");

	m_cbControlType.SetCurSel(0);
	m_stHeightSpeed.ShowWindow(FALSE);
	m_edHeightSpeed.ShowWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCPPControlProperty::OnSelendokControlType() 
{
	// TODO: Add your control notification handler code here
	if(m_cbControlType.GetCurSel() == CT_CIRCLE)
	{
		m_stHeightSpeed.ShowWindow(FALSE);
		m_edHeightSpeed.ShowWindow(FALSE);
	}
	else if(m_cbControlType.GetCurSel() == CT_SPIRAL)
	{
		m_stHeightSpeed.ShowWindow(TRUE);
		m_edHeightSpeed.ShowWindow(TRUE);
	}
}

void CDlgCPPControlProperty::Clear()
{
	m_fAngleSpeed = 0.0f;
	m_fHeightSpeed = 0.0f;
	m_bUsePosition = TRUE;

	m_cbControlType.SetCurSel(0);
	m_stHeightSpeed.ShowWindow(FALSE);
	m_edHeightSpeed.ShowWindow(FALSE);
	
	UpdateData(SAVETOFORM);
}

void CDlgCPPControlProperty::Refresh()
{
	m_fAngleSpeed = m_ppcontrol.GetAngleSpeed();
	m_fHeightSpeed = m_ppcontrol.GetHeightSpeed();
	m_bUsePosition = m_ppcontrol.IsUseParticlePos();

	m_cbControlType.SetCurSel( (int)m_ppcontrol.GetControlType());
	OnSelendokControlType();

	UpdateData(SAVETOFORM);
}

