// DlgCPESphereProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPESphereProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPESphereProperty dialog
#include "DlgCForce.h"
#include "DlgCSphere.h"

CDlgCPESphereProperty::CDlgCPESphereProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPESphereProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPESphereProperty)
	m_fDelayTime = 0.0f;
	//}}AFX_DATA_INIT
	m_force.SetZeroForce();
	m_sphere.SetRadius(FLOAT3D(0,0,0), 0, 0);
}


void CDlgCPESphereProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPESphereProperty)
	DDX_Text(pDX, IDC_EMITTER_DELAYTIME, m_fDelayTime);
	DDV_MinMaxFloat(pDX, m_fDelayTime, 0.f, 99999.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPESphereProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPESphereProperty)
	ON_BN_CLICKED(IDC_BTN_FORCE, OnBtnForce)
	ON_BN_CLICKED(IDC_BTN_SPHERE, OnBtnSphere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPESphereProperty message handlers

void CDlgCPESphereProperty::Clear()
{
	m_fDelayTime = 0;
	m_force = CForce();
	m_force.SetZeroForce();
	m_sphere = CSphereDoubleSpace();
	m_sphere.SetRadius(FLOAT3D(0,0,0), 0, 0);

	UpdateData(SAVETOFORM);
}

void CDlgCPESphereProperty::OnBtnForce() 
{
	// TODO: Add your control notification handler code here
	CDlgCForce dlgForce(this);
	dlgForce.SetPropertyFrom(m_force);

	if( dlgForce.DoModal() == IDOK )
	{
		dlgForce.SetPropertyTo(m_force);
	}
}

void CDlgCPESphereProperty::OnBtnSphere() 
{
	// TODO: Add your control notification handler code here
	DLGSPHERE(m_sphere);
}
