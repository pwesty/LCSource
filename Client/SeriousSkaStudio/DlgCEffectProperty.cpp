// DlgCEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"

#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectManager.h>

#include "DlgCEffectProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectProperty dialog


CDlgCEffectProperty::CDlgCEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCEffectProperty)
	m_strEffectName = _T("");
	m_fFadeInTime = 0.0f;
	m_fFadeOutTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fBoundingSphereRadius = 1.0f;
	m_slRepeatCount = 0;
	//}}AFX_DATA_INIT
	m_eEffectType = ET_NOTHING;
}

void CDlgCEffectProperty::ClearForm()
{
	m_strEffectName = _T("");
	m_fFadeInTime = 0.0f;
	m_fFadeOutTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_slRepeatCount = 0;
	m_fBoundingSphereRadius = 1.0f;

	m_eEffectType = ET_NOTHING;

	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCEffectProperty)
	DDX_Control(pDX, IDC_EE_TYPE, m_cbEffectType);
	DDX_Control(pDX, IDC_EE_REGISTERED_NAME, m_cbRegisteredName);
	DDX_Text(pDX, IDC_EFFECT_NAME, m_strEffectName);
	DDV_MaxChars(pDX, m_strEffectName, 32);
	DDX_Text(pDX, IDC_FADE_IN_TIME, m_fFadeInTime);
	DDV_MinMaxFloat(pDX, m_fFadeInTime, 0.f, 999999.f);
	DDX_Text(pDX, IDC_FADE_OUT_TIME, m_fFadeOutTime);
	DDV_MinMaxFloat(pDX, m_fFadeOutTime, 0.f, 999999.f);
	DDX_Text(pDX, IDC_LIFE_TIME, m_fLifeTime);
	DDV_MinMaxFloat(pDX, m_fLifeTime, 0.f, 999999.f);
	DDX_Text(pDX, IDC_BOUNDING_SPHERE_RADIUS, m_fBoundingSphereRadius);
	DDV_MinMaxFloat(pDX, m_fBoundingSphereRadius, 1.e-006f, 99999.f);
	DDX_Text(pDX, IDC_REPEAT_COUNT, m_slRepeatCount);
	DDV_MinMaxLong(pDX, m_slRepeatCount, -1, 9999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCEffectProperty)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_CBN_SELENDOK(IDC_EE_TYPE, OnSelEndOkEeType)
	ON_CBN_SELENDOK(IDC_EE_REGISTERED_NAME, OnSelEndOkEeRegisteredName)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectProperty message handlers

void CDlgCEffectProperty::Clear()
{
	m_cbEffectType.SetCurSel(-1);
	m_cbRegisteredName.SetCurSel(-1);
	m_strEffectName = "";
	m_fFadeInTime = 0;
	m_fFadeOutTime = 0;
	m_fBoundingSphereRadius = 1;
	m_fLifeTime = 0;
	m_slRepeatCount = 0;
	m_eEffectType = ET_NOTHING;

	m_pConcreteEffectProperty = NULL;
	m_dlgCLightEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCMdlEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCSkaEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCTerrainEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCSoundEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCTraceEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCParticleEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCSplineBillboardEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCOrbitEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCShockWaveEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCSplinePathEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCCameraEffectProperty.SendMessage(WM_CLEAR_FORM);
	m_dlgCEntityEffectProperty.SendMessage(WM_CLEAR_FORM);

	UpdateData(SAVETOFORM);
}

void CDlgCEffectProperty::Refresh()
{
	Clear();
/*
	CEffectManager::my_map::iterator iterBegin = CEffectManager::Instance().GetRegisteredMap().begin();
	CEffectManager::my_map::iterator iterEnd = CEffectManager::Instance().GetRegisteredMap().end();
	CEffectManager::my_map::iterator iter;
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		m_cbRegisteredName.AddString( (*iter).second->GetName().c_str() );
	}
*/
}

void CDlgCEffectProperty::OnSelEndOkEeType() 
{
	// TODO: Add your control notification handler code here
	m_cbRegisteredName.ResetContent();
	//선택된 effect 타입이 없음.
	if(m_cbEffectType.GetCurSel() == CB_ERR) return;

	ClearForm();
	m_eEffectType = EFFECT_TYPE(m_cbEffectType.GetCurSel()+1);
	int cnt = CEffectManager::Instance().GetRegisteredMap().size();
	for(CEffectManager::my_map::iterator iter = CEffectManager::Instance().GetRegisteredMap().begin();
	iter != CEffectManager::Instance().GetRegisteredMap().end(); ++iter)
	{
		if((*iter).second->GetType() == m_eEffectType)
			m_cbRegisteredName.AddString( (*iter).first.c_str() );
	}
	if(m_pConcreteEffectProperty) m_pConcreteEffectProperty->SendMessage(WM_CLEAR_FORM);
	m_pConcreteEffectProperty = NULL;
	m_dlgCLightEffectProperty.ShowWindow(FALSE);
	m_dlgCMdlEffectProperty.ShowWindow(FALSE);
	m_dlgCSkaEffectProperty.ShowWindow(FALSE);
	m_dlgCTerrainEffectProperty.ShowWindow(FALSE);
	m_dlgCSoundEffectProperty.ShowWindow(FALSE);
	m_dlgCTraceEffectProperty.ShowWindow(FALSE);
	m_dlgCParticleEffectProperty.ShowWindow(FALSE);
	m_dlgCSplineBillboardEffectProperty.ShowWindow(FALSE);
	m_dlgCOrbitEffectProperty.ShowWindow(FALSE);
	m_dlgCShockWaveEffectProperty.ShowWindow(FALSE);
	m_dlgCSplinePathEffectProperty.ShowWindow(FALSE);
	m_dlgCCameraEffectProperty.ShowWindow(FALSE);
	m_dlgCEntityEffectProperty.ShowWindow(FALSE);
	
	switch(m_eEffectType)
	{
	case ET_TERRAIN:	m_pConcreteEffectProperty = &m_dlgCTerrainEffectProperty;			break;
	case ET_LIGHT:		m_pConcreteEffectProperty = &m_dlgCLightEffectProperty;				break;
	case ET_PARTICLE:	m_pConcreteEffectProperty = &m_dlgCParticleEffectProperty;			break;
	case ET_SKA:		m_pConcreteEffectProperty = &m_dlgCSkaEffectProperty;				break;
	case ET_MDL:		m_pConcreteEffectProperty = &m_dlgCMdlEffectProperty;				break;
	case ET_TRACE:		m_pConcreteEffectProperty = &m_dlgCTraceEffectProperty;				break;
	case ET_SOUND:		m_pConcreteEffectProperty = &m_dlgCSoundEffectProperty;				break;
	case ET_SPLINEBILLBOARD:
						m_pConcreteEffectProperty = &m_dlgCSplineBillboardEffectProperty;	break;
	case ET_ORBIT:		m_pConcreteEffectProperty = &m_dlgCOrbitEffectProperty;				break;
	case ET_SHOCKWAVE:	m_pConcreteEffectProperty = &m_dlgCShockWaveEffectProperty;			break;
	case ET_SPLINEPATH:	m_pConcreteEffectProperty = &m_dlgCSplinePathEffectProperty;		break;
	case ET_CAMERA:		m_pConcreteEffectProperty = &m_dlgCCameraEffectProperty;			break;
	case ET_ENTITY:		m_pConcreteEffectProperty = &m_dlgCEntityEffectProperty;			break;
	default: ASSERTALWAYS("알 수 없는 이펙트 타입임."); break;
	}
	if(m_pConcreteEffectProperty) m_pConcreteEffectProperty->ShowWindow(TRUE);
}

BOOL CDlgCEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	//light
	m_dlgCLightEffectProperty.Create(IDD_CLIGHTEFFECT_PROPERTY, this);
	m_dlgCLightEffectProperty.GetClientRect(&rectChild);
	m_dlgCLightEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCLightEffectProperty.ShowWindow(FALSE);

	//mdl
	m_dlgCMdlEffectProperty.Create(IDD_CMDLEFFECT_PROPERTY, this);
	m_dlgCMdlEffectProperty.GetClientRect(&rectChild);
	m_dlgCMdlEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCMdlEffectProperty.ShowWindow(FALSE);

	//ska
	m_dlgCSkaEffectProperty.Create(IDD_CSKAEFFECT_PROPERTY, this);
	m_dlgCSkaEffectProperty.GetClientRect(&rectChild);
	m_dlgCSkaEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCSkaEffectProperty.ShowWindow(FALSE);

	//terrain
	m_dlgCTerrainEffectProperty.Create(IDD_CTERRAINEFFECT_PROPERTY, this);
	m_dlgCTerrainEffectProperty.GetClientRect(&rectChild);
	m_dlgCTerrainEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCTerrainEffectProperty.ShowWindow(FALSE);

	//sound
	m_dlgCSoundEffectProperty.Create(IDD_CSOUNDEFFECT_PROPERTY, this);
	m_dlgCSoundEffectProperty.GetClientRect(&rectChild);
	m_dlgCSoundEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCSoundEffectProperty.ShowWindow(FALSE);

	//trace
	m_dlgCTraceEffectProperty.Create(IDD_CTRACEEFFECT_PROPERTY, this);
	m_dlgCTraceEffectProperty.GetClientRect(&rectChild);
	m_dlgCTraceEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCTraceEffectProperty.ShowWindow(FALSE);

	//particle
	m_dlgCParticleEffectProperty.Create(IDD_CPARTICLEEFFECT_PROPERTY, this);
	m_dlgCParticleEffectProperty.GetClientRect(&rectChild);
	m_dlgCParticleEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCParticleEffectProperty.ShowWindow(FALSE);

	//spline billboard
	m_dlgCSplineBillboardEffectProperty.Create(IDD_CSPLINEBILLBOARDEFFECT_PROPERTY, this);
	m_dlgCSplineBillboardEffectProperty.GetClientRect(&rectChild);
	m_dlgCSplineBillboardEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCSplineBillboardEffectProperty.ShowWindow(FALSE);

	//orbit
	m_dlgCOrbitEffectProperty.Create(IDD_CORBITEFFECT_PROPERTY, this);
	m_dlgCOrbitEffectProperty.GetClientRect(&rectChild);
	m_dlgCOrbitEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCOrbitEffectProperty.ShowWindow(FALSE);

	//shock wave
	m_dlgCShockWaveEffectProperty.Create(IDD_CSHOCKWAVEEFFECT_PROPERTY, this);
	m_dlgCShockWaveEffectProperty.GetClientRect(&rectChild);
	m_dlgCShockWaveEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCShockWaveEffectProperty.ShowWindow(FALSE);

	//spline path
	m_dlgCSplinePathEffectProperty.Create(IDD_CSPLINEPATHEFFECT_PROPERTY, this);
	m_dlgCSplinePathEffectProperty.GetClientRect(&rectChild);
	m_dlgCSplinePathEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCSplinePathEffectProperty.ShowWindow(FALSE);

	//camera
	m_dlgCCameraEffectProperty.Create(IDD_CCAMERAEFFECT_PROPERTY, this);
	m_dlgCCameraEffectProperty.GetClientRect(&rectChild);
	m_dlgCCameraEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCCameraEffectProperty.ShowWindow(FALSE);

	//entity
	m_dlgCEntityEffectProperty.Create(IDD_CENTITYEFFECT_PROPERTY, this);
	m_dlgCEntityEffectProperty.GetClientRect(&rectChild);
	m_dlgCEntityEffectProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCEntityEffectProperty.ShowWindow(FALSE);

	// TODO: Add extra initialization here
	m_cbEffectType.AddString("Terrain");
	m_cbEffectType.AddString("Light");
	m_cbEffectType.AddString("Particle");
	m_cbEffectType.AddString("Ska");
	m_cbEffectType.AddString("Mdl");
	m_cbEffectType.AddString("Trace");
	m_cbEffectType.AddString("Sound");
	m_cbEffectType.AddString("SplineBillboard");
	m_cbEffectType.AddString("Orbit");
	m_cbEffectType.AddString("ShockWave");
	m_cbEffectType.AddString("SplinePath");
	m_cbEffectType.AddString("Camera");
	m_cbEffectType.AddString("Entity");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCEffectProperty::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	if(m_pConcreteEffectProperty == NULL) return;
	if(m_cbEffectType.GetCurSel() == -1) return;
	m_pConcreteEffectProperty->SendMessage(WM_REGISTER_EFFECT, (WPARAM)this, (LPARAM)0);
}

void CDlgCEffectProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(m_cbEffectType.GetCurSel() == -1) return;
	this->UnregisterEffect();
}

void CDlgCEffectProperty::OnSelEndOkEeRegisteredName() 
{
	// TODO: Add your control notification handler code here

	if(m_pConcreteEffectProperty == NULL) return;
	m_pConcreteEffectProperty->SendMessage(WM_SELECT_EFFECT, (WPARAM)this, (LPARAM)0);
	m_cbRegisteredName.GetLBText(m_cbRegisteredName.GetCurSel(), m_strEffectNameOld);

	if( m_cbRegisteredName.GetCurSel() == -1 ) return;
	theApp.OnExecuteEffect( TRUE );
}

void CDlgCEffectProperty::RegisterEffect(CEffect &effect)
{
	UpdateData(SAVETOOBJECT);

	m_strEffectName.Replace('/', '_');
	if(m_strEffectName != m_strEffectNameOld && m_strEffectNameOld != ""
		&& m_cbRegisteredName.FindStringExact(-1, m_strEffectName) != CB_ERR)
	{
		if(IDNO == MessageBox("같은 이름을 가진 이펙트가 있습니다. 덮어 쓰시겠습니까?", "주의", MB_YESNO))
		{
			UpdateData(SAVETOFORM);
			return;
		}
	}
	m_strEffectNameOld = m_strEffectName;
	effect.SetName( m_strEffectName.GetBuffer(0) );
	effect.SetFadeTime( m_fFadeInTime, m_fLifeTime - m_fFadeInTime - m_fFadeOutTime, m_fFadeOutTime );
	effect.SetBoundingSphereRadius(m_fBoundingSphereRadius);
	effect.SetRepeatCount( m_slRepeatCount );

	CString strCBName;
	int index = m_cbRegisteredName.GetCurSel();
	if(index != -1)
	{
		m_cbRegisteredName.GetLBText( index, strCBName );
		if(strCBName == m_strEffectName)
		{
			m_cbRegisteredName.DeleteString( index );
		}
		else
		{
			int index = m_cbRegisteredName.FindString(-1, m_strEffectName.GetBuffer(0));
			if(index != -1) m_cbRegisteredName.DeleteString(index);
		}
		CEffectManager::Instance().Unregister( effect.GetName() );
	}
	if( CEffectManager::Instance().Register( effect.Copy() ) )
	{
		m_cbRegisteredName.AddString( effect.GetName().c_str() );
		int index = m_cbRegisteredName.FindString( -1, effect.GetName().c_str() );
		m_cbRegisteredName.SetCurSel( index );
		CPrintF("Effect save : %s\n", effect.GetName().c_str());
	}
	else
	{
		AfxMessageBox("등록에 실패했습니다.", MB_OK|MB_ICONWARNING);
	}

	return;
}

void CDlgCEffectProperty::UnregisterEffect()
{
	if(IDNO == MessageBox("정말 지우시겠습니까?", "주의", MB_YESNO)) return;

	UpdateData(SAVETOOBJECT);

	CString strName;
	int index = m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		m_cbRegisteredName.GetLBText( index, strName );
		CEffectManager::Instance().Unregister( strName.GetBuffer(0) );

		m_cbRegisteredName.DeleteString(index);
		m_cbRegisteredName.SetCurSel(-1);

		CPrintF("Effect delete : %s\n", strName);
	}
	return;
}

void CDlgCEffectProperty::SetEffectInfo(CEffect &effect)
{
	m_strEffectName = effect.GetName().c_str();
	m_fFadeInTime = effect.GetFadeInTime();
	m_fFadeOutTime = effect.GetFadeOutTime();
	m_fBoundingSphereRadius = effect.GetBoundingSphereRadius();
	m_fLifeTime = effect.GetLifeTime();
	m_slRepeatCount = effect.GetRepeatCount();
	UpdateData(SAVETOFORM);
	
	return;
}

void CDlgCEffectProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(m_pConcreteEffectProperty)
	{
		m_pConcreteEffectProperty->ShowWindow(bShow);
	}
}
