// DlgCParticleEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "CmdSkaStudio.h"
#include "DlgCParticleEffectProperty.h"
#include <Engine/Effect/CParticleEffect.h>
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CParticleGroup.h>
#include <Engine/Effect/CParticleGroupManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleEffectProperty dialog


CDlgCParticleEffectProperty::CDlgCParticleEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCParticleEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCParticleEffectProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCParticleEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCParticleEffectProperty)
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_cbRotationType);
	DDX_Control(pDX, IDC_PE_REGISTERED_PARTICLE_GROUP, m_cbRegisteredParticleGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCParticleEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCParticleEffectProperty)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleEffectProperty message handlers

void CDlgCParticleEffectProperty::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	m_cbRegisteredParticleGroup.ResetContent();
	m_cbRegisteredParticleGroup.SetCurSel(-1);

	CParticleGroupManager::my_map::iterator iterBegin = CParticleGroupManager::Instance().GetRegisteredMap().begin();
	CParticleGroupManager::my_map::iterator iterEnd = CParticleGroupManager::Instance().GetRegisteredMap().end();
	CParticleGroupManager::my_map::iterator iter;

	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		CParticleGroup &pg = *((*iter).second);
		m_cbRegisteredParticleGroup.AddString( pg.GetName().c_str() );
	}
}

void CDlgCParticleEffectProperty::OnClearForm()
{
	OnBtnRefresh();

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCParticleEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_cbRegisteredParticleGroup.GetCurSel() == -1) return 0;

	CParticleEffect pe;
	CString strPGName;
	m_cbRegisteredParticleGroup.GetLBText( m_cbRegisteredParticleGroup.GetCurSel(), strPGName );
	pe.SetParticleGroup( strPGName.GetBuffer(0) );
	pe.SetRotationType(EFFECT_OF_TAG_TYPE(m_cbRotationType.GetCurSel()));

	ep.RegisterEffect(pe);
	return 0;
}

LRESULT CDlgCParticleEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;

	CmdSkaSelectParticleEffect* pCmd = new CmdSkaSelectParticleEffect;
	if (pCmd)
	{
		pCmd->setData( wp, lp );
		theApp.addQueue( pCmd );
	}
	
	return 0;
}

void CDlgCParticleEffectProperty::SelectEffect( WPARAM wp, LPARAM lp )
{
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CParticleEffect *pPE = (CParticleEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pPE == NULL) return;

		int curPGIndex = m_cbRegisteredParticleGroup.FindString(-1, pPE->GetParticleGroup().c_str());
		if(curPGIndex == CB_ERR)
		{
			CString strWarning;
			strWarning.Format("Not exist Particle Group \"%s\".", pPE->GetParticleGroup().c_str());
			AfxMessageBox(strWarning, MB_OK | MB_ICONWARNING);
		}
		this->m_cbRegisteredParticleGroup.SetCurSel(curPGIndex);
		this->m_cbRotationType.SetCurSel((SINT)pPE->GetRotationType());
		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pPE);
		CEffectManager::Instance().Destroy((CEffect *&)pPE);
	}
}

void CDlgCParticleEffectProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow && nStatus == 0)
	{
		CString strPGName;
		int curSel = m_cbRegisteredParticleGroup.GetCurSel();
		if(curSel != CB_ERR) m_cbRegisteredParticleGroup.GetLBText(curSel, strPGName);
		this->OnBtnRefresh();
		curSel = m_cbRegisteredParticleGroup.FindString(-1, strPGName);
		m_cbRegisteredParticleGroup.SetCurSel(curSel);
	}
}

BOOL CDlgCParticleEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbRotationType.AddString("None");
	m_cbRotationType.AddString("Start Once");
	m_cbRotationType.AddString("Always");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
