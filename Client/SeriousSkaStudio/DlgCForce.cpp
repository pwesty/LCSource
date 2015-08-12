// DlgCForce.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCForce.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCForce dialog


CDlgCForce::CDlgCForce(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCForce::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCForce)
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fPower = 0.0f;
	m_iType = -1;
	//}}AFX_DATA_INIT
}


void CDlgCForce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCForce)
	DDX_Control(pDX, IDC_FORCE_TYPE, m_cbType);
	DDX_Text(pDX, IDC_FORCE_X, m_fX);
	DDX_Text(pDX, IDC_FORCE_Y, m_fY);
	DDX_Text(pDX, IDC_FORCE_Z, m_fZ);
	DDX_Text(pDX, IDC_FORCE_POWER, m_fPower);
	DDX_CBIndex(pDX, IDC_FORCE_TYPE, m_iType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCForce, CDialog)
	//{{AFX_MSG_MAP(CDlgCForce)
	ON_BN_CLICKED(IDC_FORCE_NORMALIZE, OnForceNormalize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCForce message handlers

void CDlgCForce::OnForceNormalize() 
{
	// TODO: Add your control notification handler code here
	UpdateData(SAVETOOBJECT);
	FLOAT3D float3d(m_fX, m_fY, m_fZ);
	float3d.Normalize();
	m_fX = float3d(1);
	m_fY = float3d(2);
	m_fZ = float3d(3);
	UpdateData(SAVETOFORM);
}

BOOL CDlgCForce::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbType.AddString("Zero");
	m_cbType.AddString("Direction");
	m_cbType.AddString("Point");
	m_cbType.AddString("GravityPoint");
	m_cbType.AddString("InverseGravityPoint");
	m_cbType.SetCurSel(m_iType);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCForce::SetPropertyTo(CForce &force)
{
	switch(m_iType)
	{
	case 0:	force.SetZeroForce();													break;
	case 1:	force.SetDirectionalForce(m_fPower, FLOAT3D(m_fX, m_fY, m_fZ));			break;
	case 2:	force.SetPointForce(m_fPower, FLOAT3D(m_fX, m_fY, m_fZ));				break;
	case 3:	force.SetGravityPointForce(m_fPower, FLOAT3D(m_fX, m_fY, m_fZ));		break;
	case 4:	force.SetInverseGravityPointForce(m_fPower, FLOAT3D(m_fX, m_fY, m_fZ));	break;
	}
}

void CDlgCForce::SetPropertyFrom(CForce &force)
{
	m_iType = (int)force.GetType();
	m_fPower = force.GetPower();
	if(m_iType == 0)//zero force
	{
		m_fX = 0;
		m_fY = 0;
		m_fZ = 0;
	}
	if(m_iType == 1)//directional
	{
		m_fX = force.GetDirection()(1);
		m_fY = force.GetDirection()(2);
		m_fZ = force.GetDirection()(3);
	}
	else	//other
	{
		m_fX = force.GetPosition()(1);
		m_fY = force.GetPosition()(2);
		m_fZ = force.GetPosition()(3);
	}
}
