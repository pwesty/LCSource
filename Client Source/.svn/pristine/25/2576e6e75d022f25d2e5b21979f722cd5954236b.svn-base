// DlgCPEProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPEProperty.h"

#include <Engine/Effect/CParticlesEmitter.h>
#include <Engine/Effect/CParticlesEmitterSphere.h>
#include <Engine/Effect/CParticlesEmitterCone.h>
#include <Engine/Effect/CParticlesEmitterCylinder.h>
#include <Engine/Effect/CParticle.h>
#include <Engine/Math/AdditionalFunction.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPEProperty dialog


CDlgCPEProperty::CDlgCPEProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPEProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPEProperty)
	m_uiMaxCount = 0;
	m_fEmitSpeed = 0.0f;
	m_fLifeTime1 = 0.0f;
	m_fLifeTime2 = 0.0f;
	m_fWidth2 = 0.0f;
	m_fWidth1 = 0.0f;
	m_fHeight1 = 0.0f;
	m_fHeight2 = 0.0f;
	m_fMass1 = 1.0f;
	m_fMass2 = 1.0f;
	m_fAngleX1 = 0.0f;
	m_fAngleY1 = 0.0f;
	m_fAngleZ1 = 0.0f;
	m_fAngleX2 = 0.0f;
	m_fAngleY2 = 0.0f;
	m_fAngleZ2 = 0.0f;
	m_bLocalType = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCPEProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPEProperty)
	DDX_Control(pDX, IDC_BTN_COLOR2, m_btnColor2);
	DDX_Control(pDX, IDC_BTN_COLOR1, m_btnColor1);
	DDX_Control(pDX, IDC_EMITTER_TYPE, m_cbEmitterType);
	DDX_Text(pDX, IDC_EMITTER_MAXCOUNT, m_uiMaxCount);
	DDV_MinMaxUInt(pDX, m_uiMaxCount, 0, 999999);
	DDX_Text(pDX, IDC_EMITTER_EMITSPEED, m_fEmitSpeed);
	DDX_Text(pDX, IDC_EMITTER_LIFETIME1, m_fLifeTime1);
	DDV_MinMaxFloat(pDX, m_fLifeTime1, 0.f, 999999.f);
	DDX_Text(pDX, IDC_EMITTER_LIFETIME2, m_fLifeTime2);
	DDV_MinMaxFloat(pDX, m_fLifeTime2, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_WIDTH2, m_fWidth2);
	DDV_MinMaxFloat(pDX, m_fWidth2, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_WIDTH1, m_fWidth1);
	DDV_MinMaxFloat(pDX, m_fWidth1, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_HEIGHT1, m_fHeight1);
	DDV_MinMaxFloat(pDX, m_fHeight1, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_HEIGHT2, m_fHeight2);
	DDV_MinMaxFloat(pDX, m_fHeight2, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_MASS1, m_fMass1);
	DDV_MinMaxFloat(pDX, m_fMass1, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_MASS2, m_fMass2);
	DDV_MinMaxFloat(pDX, m_fMass2, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EMITTER_ANGLEX1, m_fAngleX1);
	DDX_Text(pDX, IDC_EMITTER_ANGLEY1, m_fAngleY1);
	DDX_Text(pDX, IDC_EMITTER_ANGLEZ1, m_fAngleZ1);
	DDX_Text(pDX, IDC_EMITTER_ANGLEX2, m_fAngleX2);
	DDX_Text(pDX, IDC_EMITTER_ANGLEY2, m_fAngleY2);
	DDX_Text(pDX, IDC_EMITTER_ANGLEZ2, m_fAngleZ2);
	DDX_Check(pDX, IDC_CHECK_LOCALTYPE, m_bLocalType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPEProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPEProperty)
	ON_CBN_SELENDOK(IDC_EMITTER_TYPE, OnSelEndOkEmitterType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPEProperty message handlers

void CDlgCPEProperty::Clear()
{
	m_btnColor2.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColor1.SetColor(C_WHITE|CT_OPAQUE);
	m_cbEmitterType.SetCurSel(-1);
	m_uiMaxCount = 0;
	m_fEmitSpeed = 0;
	m_fLifeTime1 = 0;
	m_fLifeTime2 = 0;
	m_fWidth2 = 0;
	m_fWidth1 = 0;
	m_fHeight1 = 0;
	m_fHeight2 = 0;
	m_fMass1 = 1;
	m_fMass2 = 1;
	m_fAngleX1 = 0.0f;
	m_fAngleY1 = 0.0f;
	m_fAngleZ1 = 0.0f;
	m_fAngleX2 = 0.0f;
	m_fAngleY2 = 0.0f;
	m_fAngleZ2 = 0.0f;
	m_bLocalType = FALSE;
	UpdateData(SAVETOFORM);

	m_dlgCPESphereProperty.Clear();
	m_dlgCPEConeProperty.Clear();
	m_dlgCPECylinderProperty.Clear();
}

BOOL CDlgCPEProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	m_dlgCPESphereProperty.Create(IDD_CPE_SPHERE_PROPERTY, this);
	m_dlgCPESphereProperty.GetClientRect(&rectChild);
	m_dlgCPESphereProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPESphereProperty.ShowWindow(FALSE);
	
	m_dlgCPEConeProperty.Create(IDD_CPE_CONE_PROPERTY, this);
	m_dlgCPEConeProperty.GetClientRect(&rectChild);
	m_dlgCPEConeProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPEConeProperty.ShowWindow(FALSE);
	
	m_dlgCPECylinderProperty.Create(IDD_CPE_CYLINDER_PROPERTY, this);
	m_dlgCPECylinderProperty.GetClientRect(&rectChild);
	m_dlgCPECylinderProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPECylinderProperty.ShowWindow(FALSE);
	
	// TODO: Add extra initialization here
	m_btnColor1.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColor2.SetColor(C_WHITE|CT_OPAQUE);

	m_cbEmitterType.AddString("Sphere");
	m_cbEmitterType.AddString("Cone");
	m_cbEmitterType.AddString("Cylinder");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCPEProperty::OnSelEndOkEmitterType() 
{
	// TODO: Add your control notification handler code here
	if(m_cbEmitterType.GetCurSel() == -1) return;

	m_dlgCPESphereProperty.ShowWindow(FALSE);
	m_dlgCPEConeProperty.ShowWindow(FALSE);
	m_dlgCPECylinderProperty.ShowWindow(FALSE);

	int index = m_cbEmitterType.GetCurSel();
	switch(index)
	{
	case 0: m_dlgCPESphereProperty.ShowWindow(TRUE); break;
	case 1: m_dlgCPEConeProperty.ShowWindow(TRUE); break;
	case 2: m_dlgCPECylinderProperty.ShowWindow(TRUE); break;
	}
}

BOOL CDlgCPEProperty::SetPropertyFrom(CParticleGroup &pg)
{
	if(pg.GetEmitter() == NULL) return FALSE;

	CParticlesEmitter *pPE = pg.GetEmitter();
	//emitter
	m_uiMaxCount = pPE->GetTotalCount();
	m_fEmitSpeed = pPE->GetEmitSpeed();
	CParticle p1, p2;
	pPE->GetParticlePrototype().CopyLowerTo(p1);
	pPE->GetParticlePrototype().CopyUpperTo(p2);
	m_fLifeTime1 = p1.GetLifeTime();
	m_fLifeTime2 = p2.GetLifeTime();
	m_fWidth1 = p1.GetWidth();
	m_fWidth2 = p2.GetWidth();
	m_fHeight1 = p1.GetHeight();
	m_fHeight2 = p2.GetHeight();
	m_fMass1 = p1.GetMass();
	m_fMass2 = p2.GetMass();
	m_btnColor1.SetColor( p1.GetColor() );
	m_btnColor2.SetColor( p2.GetColor() );
	ANGLE3D angle1 = GetEulerAngleFromQuaternion(p1.GetDirection());
	ANGLE3D angle2 = GetEulerAngleFromQuaternion(p2.GetDirection());
	m_fAngleX1 = angle1(1);
	m_fAngleY1 = angle1(2);
	m_fAngleZ1 = angle1(3);
	m_fAngleX2 = angle2(1);
	m_fAngleY2 = angle2(2);
	m_fAngleZ2 = angle2(3);
	m_bLocalType = pg.GetEmitter()->IsLocalType();
	
	switch(pg.GetEmitter()->GetType())
	{
	case PET_SPHERE:	//CParticlesEmitterSphere
		{
			m_cbEmitterType.SetCurSel(0);
			CParticlesEmitterSphere &pes = *(CParticlesEmitterSphere*)pg.GetEmitter();
			//emitter force, sphere
			m_dlgCPESphereProperty.m_force = pes.GetForce();
			m_dlgCPESphereProperty.m_fDelayTime = pes.GetDelayTime();
			m_dlgCPESphereProperty.m_sphere = pes.GetShape();
			//set and complete
			m_dlgCPESphereProperty.UpdateData(SAVETOFORM);
		} break;
	case PET_CONE:	//CParticlesEmitterCone
		{
			//emitter
			m_cbEmitterType.SetCurSel(1);
			CParticlesEmitterCone &pec = *(CParticlesEmitterCone*)pg.GetEmitter();
			//emitter force, cone
			m_dlgCPEConeProperty.m_bUsePos = pec.GetUsePos();
			m_dlgCPEConeProperty.m_bUseSpeed = pec.GetUseSpeed();
			m_dlgCPEConeProperty.m_fSpeedLower = pec.GetSpeedLower();
			m_dlgCPEConeProperty.m_fSpeedUpper = pec.GetSpeedUpper();
			m_dlgCPEConeProperty.m_cone = pec.GetShape();
			//set and complete
			m_dlgCPEConeProperty.UpdateData(SAVETOFORM);
		} break;
	case PET_CYLINDER:	//CParticlesEmitterCylinder
		{
			//emitter
			m_cbEmitterType.SetCurSel(2);
			CParticlesEmitterCylinder &pec = *(CParticlesEmitterCylinder*)pg.GetEmitter();
			//emitter force, cylinder
			m_dlgCPECylinderProperty.m_bUsePos = pec.GetUsePos();
			m_dlgCPECylinderProperty.m_bUseSpeed = pec.GetUseSpeed();
			m_dlgCPECylinderProperty.m_bEmitAllDir = pec.GetEmitAllDir();
			m_dlgCPECylinderProperty.m_fSpeedLower = pec.GetSpeedLower();
			m_dlgCPECylinderProperty.m_fSpeedUpper = pec.GetSpeedUpper();
			m_dlgCPECylinderProperty.m_cylinder = pec.GetShape();
			//set and complete
			m_dlgCPECylinderProperty.UpdateData(SAVETOFORM);
		} break;
	}

	OnSelEndOkEmitterType();
	UpdateData(SAVETOFORM);
	return TRUE;
}

BOOL CDlgCPEProperty::SetPropertyTo(CParticleGroup &pg)
{
	if(m_cbEmitterType.GetCurSel() == -1) return FALSE;

	UpdateData(SAVETOOBJECT);
	CParticle p1, p2;
	p1.SetLifeTime( m_fLifeTime1 );
	p2.SetLifeTime( m_fLifeTime2 );
	p1.SetWidth( m_fWidth1 );
	p2.SetWidth( m_fWidth2 );
	p1.SetHeight( m_fHeight1 );
	p2.SetHeight( m_fHeight2 );
	p1.SetMass( m_fMass1 );
	p2.SetMass( m_fMass2 );
	p1.SetColor( m_btnColor1.GetColor() );
	p2.SetColor( m_btnColor2.GetColor() );
	FLOATquat3D quat1, quat2;
	quat1.FromEuler(ANGLE3D(m_fAngleX1, m_fAngleY1, m_fAngleZ1));
	quat2.FromEuler(ANGLE3D(m_fAngleX2, m_fAngleY2, m_fAngleZ2));
	p1.SetDirection(quat1);
	p2.SetDirection(quat2);
	CParticlePrototype ppt;
	ppt.SetLower(p1);
	ppt.SetUpper(p2);

	switch(m_cbEmitterType.GetCurSel())
	{
	case 0:	//CParticlesEmitterSphere
		{
			m_dlgCPESphereProperty.UpdateData(SAVETOOBJECT);
			//emitter
			CParticlesEmitterSphere pes;
			pes.SetTotalCount(m_uiMaxCount);
			pes.SetEmitSpeed(m_fEmitSpeed);
			pes.SetParticlePrototype(ppt);
			//emitter force, sphere
			pes.SetForce(m_dlgCPESphereProperty.m_force, m_dlgCPESphereProperty.m_fDelayTime);
			pes.SetShape(m_dlgCPESphereProperty.m_sphere);
			pes.SetLocalType(m_bLocalType);
			//set and complete
			pg.SetEmitter(&pes);
		} break;
	case 1:	//CParticlesEmitterCone
		{
			m_dlgCPEConeProperty.UpdateData(SAVETOOBJECT);
			//emitter
			CParticlesEmitterCone pec;
			pec.SetTotalCount(m_uiMaxCount);
			pec.SetEmitSpeed(m_fEmitSpeed);
			pec.SetParticlePrototype(ppt);
			//emitter force, cone
			pec.SetUsePos(m_dlgCPEConeProperty.m_bUsePos);
			pec.SetUseSpeed(m_dlgCPEConeProperty.m_bUseSpeed);
			pec.SetSpeed(m_dlgCPEConeProperty.m_fSpeedLower, m_dlgCPEConeProperty.m_fSpeedUpper);
			pec.SetShape(m_dlgCPEConeProperty.m_cone);
			pec.SetLocalType(m_bLocalType);
			//set and complete
			pg.SetEmitter(&pec);
		} break;
	case 2:	//CParticlesEmitterCylinder
		{
			m_dlgCPECylinderProperty.UpdateData(SAVETOOBJECT);
			//emitter
			CParticlesEmitterCylinder pec;
			pec.SetTotalCount(m_uiMaxCount);
			pec.SetEmitSpeed(m_fEmitSpeed);
			pec.SetParticlePrototype(ppt);
			//emitter force, cylinder
			pec.SetUsePos(m_dlgCPECylinderProperty.m_bUsePos);
			pec.SetUseSpeed(m_dlgCPECylinderProperty.m_bUseSpeed);
			pec.SetEmitAllDir(m_dlgCPECylinderProperty.m_bEmitAllDir);
			pec.SetSpeed(m_dlgCPECylinderProperty.m_fSpeedLower, m_dlgCPECylinderProperty.m_fSpeedUpper);
			pec.SetShape(m_dlgCPECylinderProperty.m_cylinder);
			pec.SetLocalType(m_bLocalType);
			//set and complete
			pg.SetEmitter(&pec);
		} break;
	}

	return TRUE;
}
