// DlgCPAProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPAProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPAProperty dialog


CDlgCPAProperty::CDlgCPAProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPAProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPAProperty)
	m_bDependLife = TRUE;
	//}}AFX_DATA_INIT
}


void CDlgCPAProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPAProperty)
	DDX_Control(pDX, IDC_PARTICLE_MOVE_TYPE, m_cbParticleMoveType);
	DDX_Control(pDX, IDC_ABSORPTION_TYPE, m_cbAbsorptionType);
	DDX_Check(pDX, IDC_CHK_DEPEND_LIFE, m_bDependLife);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPAProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPAProperty)
	ON_CBN_SELENDOK(IDC_ABSORPTION_TYPE, OnSelEndOkAbsorptionType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPAProperty message handlers

void CDlgCPAProperty::Clear()
{
	m_cbAbsorptionType.SetCurSel(-1);
	m_cbParticleMoveType.SetCurSel(0);
	m_bDependLife = TRUE;

	m_dlgCPADefaultProperty.Clear();
	m_dlgCPASphereProperty.Clear();

	UpdateData(SAVETOFORM);
}

BOOL CDlgCPAProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	RECT rectParent, rectChild;
	m_dlgCPADefaultProperty.Create(IDD_CPA_DEFAULT_PROPERTY, this);
	GetClientRect(&rectParent);
	m_dlgCPADefaultProperty.GetClientRect(&rectChild);
	m_dlgCPADefaultProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPADefaultProperty.ShowWindow(FALSE);
	
	m_dlgCPASphereProperty.Create(IDD_CPA_SPHERE_PROPERTY, this);
	GetClientRect(&rectParent);
	m_dlgCPASphereProperty.GetClientRect(&rectChild);
	m_dlgCPASphereProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPASphereProperty.ShowWindow(FALSE);
	
	// TODO: Add extra initialization here
	m_cbAbsorptionType.AddString("Default");
	m_cbAbsorptionType.AddString("Sphere");

	m_cbParticleMoveType.AddString("No-Move");
	m_cbParticleMoveType.AddString("Velocity");
	m_cbParticleMoveType.AddString("Acceleration");
	m_cbParticleMoveType.AddString("Local,No-Move");
	m_cbParticleMoveType.AddString("Local,Velocity");
	m_cbParticleMoveType.AddString("Local,Acceleration");
	m_cbParticleMoveType.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCPAProperty::OnSelEndOkAbsorptionType() 
{
	// TODO: Add your control notification handler code here
	if(m_cbAbsorptionType.GetCurSel() == -1) return;
	
	m_dlgCPADefaultProperty.ShowWindow(FALSE);
	m_dlgCPASphereProperty.ShowWindow(FALSE);

	switch(m_cbAbsorptionType.GetCurSel())
	{
	case 0:	m_dlgCPADefaultProperty.ShowWindow(TRUE);	break;
	case 1: m_dlgCPASphereProperty.ShowWindow(TRUE);	break;
	}
}

BOOL CDlgCPAProperty::SetPropertyFrom(CParticleGroup &pg)
{
	if(pg.GetAbsorption() == NULL) return FALSE;

	m_dlgCPADefaultProperty.ShowWindow(FALSE);
	m_dlgCPASphereProperty.ShowWindow(FALSE);

	m_cbAbsorptionType.SetCurSel( int(pg.GetAbsorption()->GetType()) - 1 );

	m_cbParticleMoveType.SetCurSel( int(pg.GetAbsorption()->GetMoveType()) );
	m_bDependLife = pg.GetAbsorption()->GetDependLife();

	switch(pg.GetAbsorption()->GetType())
	{
	case PAT_DEFAULT:	//CParticlesAbsorptionDefault
		{
			//CParticlesAbsorptionDefault &pa = *(CParticlesAbsorptionDefault*)pg.GetAbsorption();
			//set and complete
			//m_dlgCPADefaultProperty.UpdateData(SAVETOFORM);
			m_dlgCPADefaultProperty.ShowWindow(TRUE);
		} break;
	case PAT_SPHERE:
		{
			//sphere
			CParticlesAbsorptionSphere &pa = *(CParticlesAbsorptionSphere*)pg.GetAbsorption();
			m_dlgCPASphereProperty.m_sphere = pa.GetSphere();
			//set and complete
			m_dlgCPASphereProperty.UpdateData(SAVETOFORM);
			m_dlgCPASphereProperty.ShowWindow(TRUE);
		} break;
	}
	UpdateData(SAVETOFORM);

	return TRUE;
}

BOOL CDlgCPAProperty::SetPropertyTo(CParticleGroup &pg)
{
	if(m_cbAbsorptionType.GetCurSel() == -1) return FALSE;

	UpdateData(SAVETOOBJECT);

	switch(m_cbAbsorptionType.GetCurSel())
	{
	case 0:	//CParticlesAbsorption
		{
			//Absorption
			CParticlesAbsorptionDefault pa;
			pa.SetDependLife( m_bDependLife );
			pa.SetMoveType( (PARTICLE_MOVE_TYPE)m_cbParticleMoveType.GetCurSel() );
			//set and complete
			pg.SetAbsorption(&pa);
		} break;
	case 1:
		{
			m_dlgCPASphereProperty.UpdateData(SAVETOOBJECT);
			//Absorption
			CParticlesAbsorptionSphere pa;
			pa.SetDependLife( m_bDependLife );
			pa.SetMoveType( (PARTICLE_MOVE_TYPE)m_cbParticleMoveType.GetCurSel() );
			pa.SetSphere( m_dlgCPASphereProperty.m_sphere );
			//set and complete
			pg.SetAbsorption(&pa);
		} break;
	}

	return TRUE;
}
