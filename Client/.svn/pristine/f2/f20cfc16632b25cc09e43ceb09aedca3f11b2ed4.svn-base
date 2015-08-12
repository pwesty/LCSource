// DlgEffectEditType.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgEffectEditType.h"

#include "DlgCEffectGroupProperty.h"
#include "DlgCEffectProperty.h"
#include "DlgCParticleGroupProperty.h"
#include "CmdSkaStudio.h"

#include <Engine/Base/Timer.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CParticleGroupManager.h>
#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CParticleEffect.h>
#include <Engine/Effect/CSoundEffect.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEffectEditType dialog


CDlgEffectEditType::CDlgEffectEditType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEffectEditType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEffectEditType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEffectEditType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEffectEditType)
	DDX_Control(pDX, IDC_BTN_EXTENSION, m_btnExtension);
	DDX_Control(pDX, IDC_BTN_TEST_EFFECTSYSTEM, m_btnTest);
	DDX_Control(pDX, IDC_CB_EFFECT_EDIT_TYPE, m_cbEffectEditType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEffectEditType, CDialog)
	//{{AFX_MSG_MAP(CDlgEffectEditType)
	ON_CBN_SELENDOK(IDC_CB_EFFECT_EDIT_TYPE, OnSelEndOkCbEffectEditType)
	ON_BN_CLICKED(IDC_BTN_TEST_EFFECTSYSTEM, OnBtnTestEffectsystem)
	ON_BN_CLICKED(IDC_BTN_EXTENSION, OnBtnExtension)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEffectEditType message handlers

void CDlgEffectEditType::OnSelEndOkCbEffectEditType() 
{
	// TODO: Add your control notification handler code here
	theApp.m_dlgCEffectProperty.ShowWindow(FALSE);
	theApp.m_dlgCEffectGroupProperty.ShowWindow(FALSE);
	theApp.m_dlgCParticleGroupProperty.ShowWindow(FALSE);

	m_btnTest.EnableWindow(FALSE);
	m_btnExtension.EnableWindow(FALSE);

	switch(m_cbEffectEditType.GetCurSel())
	{
	case 0:	theApp.m_dlgCEffectGroupProperty.ShowWindow(TRUE);
			m_btnTest.EnableWindow(TRUE);
			m_btnExtension.EnableWindow(TRUE);
			break;
	case 1:	theApp.m_dlgCEffectProperty.ShowWindow(TRUE);
			m_btnTest.EnableWindow(TRUE);
			m_btnExtension.EnableWindow(TRUE);
			break;
	case 2:	theApp.m_dlgCParticleGroupProperty.ShowWindow(TRUE);
			m_btnTest.EnableWindow(TRUE);
			m_btnExtension.EnableWindow(TRUE);
			break;
	}
}

BOOL CDlgEffectEditType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbEffectEditType.AddString("Group");
	m_cbEffectEditType.AddString("Effect");
	m_cbEffectEditType.AddString("Particle");

	m_dlgExtensionEdit.Create(IDD_DLG_EXTENSION, this);
	m_dlgExtensionEdit.ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEffectEditType::OnBtnTestEffectsystem() 
{
	CmdSkaTestEffect* pCmd = new CmdSkaTestEffect;
	if (pCmd)
		theApp.addQueue(pCmd);
}

void CDlgEffectEditType::TestEffectSystem()
{
	// TODO: Add your control notification handler code here
	CSeriousSkaStudioView *pView = (CSeriousSkaStudioView*)theApp.GetActiveView();
	if(pView == NULL)
	{
		AfxMessageBox("적어도 창이 하나는 열려있어야 합니다.");
		return;
	}
	if(m_cbEffectEditType.GetCurSel() == -1) return;

	//Test용 Tag 추가.
	CSeriousSkaStudioDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL) return;
	if(pDoc->m_ModelInstance == NULL) return;
	CTagManager &tagmgr = (*pDoc->m_ModelInstance).m_tmSkaTagManager;
	if( !tagmgr.IsRegister("TEST") )
	{
		CWorldTag tag;
		tag.SetOffsetPos(0,1,0);
		tag.SetName("TEST");
		tagmgr.Register(&tag);
	}
	if( !tagmgr.IsRegister("TEST-HIT") )
	{
		CWorldTag tag;
		tag.SetName("TEST-HIT");
		tag.SetOffsetPos(0,1,-1);
		tag.SetOffsetRot(ANGLE3D(0,90,0));
		tagmgr.Register(&tag);
	}

	extern CTimer *_pTimer;
	switch(m_cbEffectEditType.GetCurSel())
	{
	case 0:	//EffectGroup
		{
			//확인
			if(theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEGName.GetCurSel() == -1)
			{
				CPrintF("Must select one registered effect group.\n");
				return;
			}
			CString strEffectGroupName;
			theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEGName.GetLBText(
				theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEGName.GetCurSel(),
				strEffectGroupName);
			//Test
			CEffectGroup *pEG = CreateEffectGroup(strEffectGroupName);
			if(pEG == NULL) { ASSERTALWAYS("Create Effect Group Fail."); return; }
			pEG->SetTagManager(&tagmgr);
			pEG->Start(_pTimer->GetLerpedCurrentTick());
			for(int i=0; i<pEG->GetEffectCount(); ++i)
			{
				CEffect *pE = pEG->GetEffectKeyVector()[i].m_pCreatedEffect;
				if(pE != NULL && pE->GetType() == ET_SOUND)
				{
					((CSoundEffect *)pE)->SetSoundPlayFlag( (((CSoundEffect *)pE)->GetSoundPlayFlag() & ~SOF_3D) | SOF_NONGAME );
				}
			}
		} break;
	case 1:	//Effect
		{
			//확인
			if(theApp.m_dlgCEffectProperty.m_cbRegisteredName.GetCurSel() == -1) return;
			CString strEffectName;
			theApp.m_dlgCEffectProperty.m_cbRegisteredName.GetLBText(
				theApp.m_dlgCEffectProperty.m_cbRegisteredName.GetCurSel(), 
				strEffectName);
			//EffectGroup 등록
			CEffectGroup eg;
			CString strEGTest;
			strEGTest.Format("__EG_TEST__%4d", rand()%10000);
			eg.SetName(strEGTest);
			eg.AddEffect(0, strEffectName.GetBuffer(0));
			eg.AddTagForAllEffect(0, "TEST");
			if( !CEffectGroupManager::Instance().Register(eg.Copy()) )
			{
				ASSERTALWAYS("Temporary Effect Group Register Fail.");
				return;
			}
			//Test
			CEffectGroup *pEG = CreateEffectGroup(strEGTest.GetBuffer(0));
			if(pEG == NULL) { ASSERTALWAYS("Create Effect Group Fail."); return; }
			pEG->SetTagManager(&tagmgr);
			pEG->Start(_pTimer->GetLerpedCurrentTick());
			for(int i=0; i<pEG->GetEffectCount(); ++i)
			{
				CEffect *pE = pEG->GetEffectKeyVector()[i].m_pCreatedEffect;
				if(pE != NULL && pE->GetType() == ET_SOUND)
				{
					((CSoundEffect *)pE)->SetSoundPlayFlag( (((CSoundEffect *)pE)->GetSoundPlayFlag() & ~SOF_3D) | SOF_NONGAME );
				}
			}
			//등록 해제
			CEffectGroupManager::Instance().Unregister(strEGTest.GetBuffer(0));
		} break;
	case 2:	//ParticleGroup
		{
			//확인
			if(theApp.m_dlgCParticleGroupProperty.m_cbRegisteredName.GetCurSel() == -1) return;
			CString strParticleGroupName;
			theApp.m_dlgCParticleGroupProperty.m_cbRegisteredName.GetLBText(
				theApp.m_dlgCParticleGroupProperty.m_cbRegisteredName.GetCurSel(), 
				strParticleGroupName);
			//ParticleEffect 등록
			CString strPETest;
			strPETest.Format("__PE_TEST__%4d", rand()%10000);
			CParticleEffect pe;
			pe.SetName(strPETest.GetBuffer(0));
			pe.SetFadeTime(0, 60, 0);
			pe.SetRepeatCount(0);
			pe.SetBoundingSphereRadius(100.0f);
			pe.SetParticleGroup(strParticleGroupName.GetBuffer(0));
			if( !CEffectManager::Instance().Register(pe.Copy()) )
			{
				ASSERTALWAYS("Temporary Particle Effect Register Fail.");
				return;
			}
			//EffectGroup 등록
			CEffectGroup eg;
			CString strEGTest;
			strEGTest.Format("__EG_TEST__%4d", rand()%10000);
			eg.SetName(strEGTest.GetBuffer(0));
			eg.AddEffect(0, strPETest.GetBuffer(0));
			eg.AddTagForAllEffect(0, "TEST");
			if( !CEffectGroupManager::Instance().Register(eg.Copy()) )
			{
				ASSERTALWAYS("Temporary Effect Group Register Fail.");
				return;
			}
			//Test
			CEffectGroup *pEG = CreateEffectGroup(strEGTest.GetBuffer(0));
			if(pEG == NULL) { ASSERTALWAYS("Create Effect Group Fail."); return; }
			pEG->SetTagManager(&tagmgr);
			pEG->Start(_pTimer->GetLerpedCurrentTick());
			//등록 해제
			CEffectGroupManager::Instance().Unregister(strEGTest.GetBuffer(0));
			CEffectManager::Instance().Unregister(strPETest.GetBuffer(0));
		} break;
	}
}

void CDlgEffectEditType::OnBtnExtension() 
{
	CDlgExtensionEdit &dlg = m_dlgExtensionEdit;
	switch(m_cbEffectEditType.GetCurSel())
	{
	case 0:	//EffectGroup
		{
			CString strEffectGroupName;
			theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEGName.GetLBText(
				theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEGName.GetCurSel(),
				strEffectGroupName);
			dlg.m_strName = strEffectGroupName;
			dlg.m_iType = 0;
		} break;
	case 1: //Effect
		{
			CString strEffectName;
			theApp.m_dlgCEffectProperty.m_cbRegisteredName.GetLBText(
				theApp.m_dlgCEffectProperty.m_cbRegisteredName.GetCurSel(), 
				strEffectName);
			dlg.m_strName = strEffectName;
			dlg.m_iType = 1;
		} break;
	case 2: //Particle
		{
			CString strParticleGroupName;
			theApp.m_dlgCParticleGroupProperty.m_cbRegisteredName.GetLBText(
				theApp.m_dlgCParticleGroupProperty.m_cbRegisteredName.GetCurSel(), 
				strParticleGroupName);
			dlg.m_strName = strParticleGroupName;
			dlg.m_iType = 2;
		} break;
	default://nothing
		{
		} break;
	}
	dlg.ShowWindow(TRUE);
}


void CDlgEffectEditType::ExcuteSubEffect()
{
	// TODO: Add your control notification handler code here
	CSeriousSkaStudioView *pView = (CSeriousSkaStudioView*)theApp.GetActiveView();
	
	if(pView == NULL)
	{
		AfxMessageBox("적어도 창이 하나는 열려있어야 합니다.");
		return;
	}

	if(m_cbEffectEditType.GetCurSel() == -1) return;

	//Test용 Tag 추가.
	CSeriousSkaStudioDoc *pDoc = pView->GetDocument();

	if(pDoc == NULL) return;
	if(pDoc->m_ModelInstance == NULL) return;

	CTagManager& tagmgr = (*pDoc->m_ModelInstance).m_tmSkaTagManager;
	
	if( !tagmgr.IsRegister("TEST") )
	{
		CWorldTag tag;
		tag.SetOffsetPos(0,1,0);
		tag.SetName("TEST");
		tagmgr.Register(&tag);
	}
	if( !tagmgr.IsRegister("TEST-HIT") )
	{
		CWorldTag tag;
		tag.SetName("TEST-HIT");
		tag.SetOffsetPos(0,1,-1);
		tag.SetOffsetRot(ANGLE3D(0,90,0));
		tagmgr.Register(&tag);
	}


	extern CTimer *_pTimer;
	
	//확인
	if(theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEffect.GetCurSel() == -1) return;
	CString strEffectName;
	theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEffect.GetLBText(
		theApp.m_dlgCEffectGroupProperty.m_cbRegisteredEffect.GetCurSel(), 
		strEffectName);
	//EffectGroup 등록
	CEffectGroup eg;
	CString strEGTest;
	strEGTest.Format("__EG_TEST__%4d", rand()%10000);
	eg.SetName(strEGTest);
	eg.AddEffect(0, strEffectName.GetBuffer(0));
	eg.AddTagForAllEffect(0, "TEST");
	if( !CEffectGroupManager::Instance().Register(eg.Copy()) )
	{
		ASSERTALWAYS("Temporary Effect Group Register Fail.");
		return;
	}
	//Test
	CEffectGroup *pEG = CreateEffectGroup(strEGTest.GetBuffer(0));
	if(pEG == NULL) { ASSERTALWAYS("Create Effect Group Fail."); return; }
	pEG->SetTagManager(&tagmgr);
	pEG->Start(_pTimer->GetLerpedCurrentTick());
	for(int i=0; i<pEG->GetEffectCount(); ++i)
	{
		CEffect *pE = pEG->GetEffectKeyVector()[i].m_pCreatedEffect;
		if(pE != NULL && pE->GetType() == ET_SOUND)
		{
			((CSoundEffect *)pE)->SetSoundPlayFlag( (((CSoundEffect *)pE)->GetSoundPlayFlag() & ~SOF_3D) | SOF_NONGAME );
		}
	}
	//등록 해제
	CEffectGroupManager::Instance().Unregister(strEGTest.GetBuffer(0));

}

