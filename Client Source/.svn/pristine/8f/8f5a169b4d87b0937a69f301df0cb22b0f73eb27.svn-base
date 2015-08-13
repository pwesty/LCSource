// DlgCPEConeProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPEConeProperty.h"
#include "DlgCCone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPEConeProperty dialog


CDlgCPEConeProperty::CDlgCPEConeProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPEConeProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPEConeProperty)
	m_bUsePos = FALSE;
	m_bUseSpeed = FALSE;
	m_fSpeedLower = 0.0f;
	m_fSpeedUpper = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCPEConeProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPEConeProperty)
	DDX_Check(pDX, IDC_PECONE_CHK_POS, m_bUsePos);
	DDX_Check(pDX, IDC_PECONE_CHK_SPEED, m_bUseSpeed);
	DDX_Text(pDX, IDC_PECONE_SPEED_LOWER, m_fSpeedLower);
	DDX_Text(pDX, IDC_PECONE_SPEED_UPPER, m_fSpeedUpper);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPEConeProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPEConeProperty)
	ON_BN_CLICKED(IDC_PECONE_BTN_CONE, OnPeconeBtnCone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPEConeProperty message handlers

void CDlgCPEConeProperty::OnPeconeBtnCone() 
{
	// TODO: Add your control notification handler code here
	DLGCONE(m_cone);
}

void CDlgCPEConeProperty::Clear()
{
	m_bUsePos = FALSE;
	m_bUseSpeed = TRUE;
	m_fSpeedLower = 0;
	m_fSpeedUpper = 0;
	m_cone = CConeDoubleSpace();
	m_cone.SetAngle(0,0);
	m_cone.SetCenter(FLOAT3D(0,0,0));
	m_cone.SetHeight(0,0);

	UpdateData(SAVETOFORM);
}

