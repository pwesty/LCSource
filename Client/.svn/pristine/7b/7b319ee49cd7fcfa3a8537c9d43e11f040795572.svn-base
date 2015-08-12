// DlgCOrbitEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCOrbitEffectProperty.h"
#include <Engine/Effect/COrbitEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCOrbitEffectProperty dialog


CDlgCOrbitEffectProperty::CDlgCOrbitEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCOrbitEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCOrbitEffectProperty)
	m_fPhaseMulX = 0.0f;
	m_fPhaseMulY = 0.0f;
	m_fPhaseMulZ = 0.0f;
	m_fPhaseOffsetX = 0.0f;
	m_fPhaseOffsetY = 0.0f;
	m_fPhaseOffsetZ = 0.0f;
	m_fRadiusX = 0.0f;
	m_fRadiusY = 0.0f;
	m_fRadiusZ = 0.0f;
	m_fTimeBeforeStart = 0.0f;
	m_fFadeInDestRadiusMul = 0.0f;
	m_fFadeInDestPhaseMul = 0.0f;
	m_fFadeOutDestRadiusMul = 0.0f;
	m_fFadeOutDestPhaseMul = 0.0f;
	m_strGoalTag = _T("");
	m_fOffsetX = 0.0f;
	m_fOffsetY = 0.0f;
	m_fOffsetZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCOrbitEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCOrbitEffectProperty)
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_cbRotationType);
	DDX_Control(pDX, IDC_COMBO_POSITION_TYPE, m_cbPositionType);
	DDX_Control(pDX, IDC_LIST_FADEOUT_FLAG, m_lbFadeOutFlag);
	DDX_Control(pDX, IDC_LIST_FADEIN_FLAG, m_lbFadeInFlag);
	DDX_Control(pDX, IDC_COMBO_SPLINE_LIST, m_comboEffectList);
	DDX_Control(pDX, IDC_COMBO_ORBIT_LIST, m_comboOrbitList);
	DDX_Text(pDX, IDC_ED_PHASEMULX, m_fPhaseMulX);
	DDX_Text(pDX, IDC_ED_PHASEMULY, m_fPhaseMulY);
	DDX_Text(pDX, IDC_ED_PHASEMULZ, m_fPhaseMulZ);
	DDX_Text(pDX, IDC_ED_PHASEOFFSETX, m_fPhaseOffsetX);
	DDX_Text(pDX, IDC_ED_PHASEOFFSETY, m_fPhaseOffsetY);
	DDX_Text(pDX, IDC_ED_PHASEOFFSETZ, m_fPhaseOffsetZ);
	DDX_Text(pDX, IDC_ED_RADIUSX, m_fRadiusX);
	DDX_Text(pDX, IDC_ED_RADIUSY, m_fRadiusY);
	DDX_Text(pDX, IDC_ED_RADIUSZ, m_fRadiusZ);
	DDX_Text(pDX, IDC_ED_TIMEBEFORESTART, m_fTimeBeforeStart);
	DDV_MinMaxFloat(pDX, m_fTimeBeforeStart, 0.f, 99999.f);
	DDX_Text(pDX, IDC_ED_FADEIN_RMUL, m_fFadeInDestRadiusMul);
	DDX_Text(pDX, IDC_ED_FADEIN_PMUL, m_fFadeInDestPhaseMul);
	DDX_Text(pDX, IDC_ED_FADEOUT_RMUL, m_fFadeOutDestRadiusMul);
	DDX_Text(pDX, IDC_ED_FADEOUT_PMUL, m_fFadeOutDestPhaseMul);
	DDX_Text(pDX, IDC_ED_GOALTAG, m_strGoalTag);
	DDV_MaxChars(pDX, m_strGoalTag, 32);
	DDX_Text(pDX, IDC_ED_OFFSETX, m_fOffsetX);
	DDX_Text(pDX, IDC_ED_OFFSETY, m_fOffsetY);
	DDX_Text(pDX, IDC_ED_OFFSETZ, m_fOffsetZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCOrbitEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCOrbitEffectProperty)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_CBN_SELENDOK(IDC_COMBO_ORBIT_LIST, OnSelendokComboOrbitList)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCOrbitEffectProperty message handlers

extern ULONG GetFlagsByLB(CListBox &lb, int bitOffset);
extern void SetFlagsToLB(ULONG flags, CListBox &lb, int bitOffset);

void CDlgCOrbitEffectProperty::OnBtnAdd() 
{
	if(m_orbitEffect.GetOrbitCount() > INDEX('z' - 'a')) return;
	this->UpdateData(SAVETOOBJECT);

	COrbitEffect::sOrbitInfo info;
	info.fPhaseMulX = m_fPhaseMulX;
	info.fPhaseMulY = m_fPhaseMulY;
	info.fPhaseMulZ = m_fPhaseMulZ;
	info.fPhaseOffsetX = m_fPhaseOffsetX;
	info.fPhaseOffsetY = m_fPhaseOffsetY;
	info.fPhaseOffsetZ = m_fPhaseOffsetZ;
	info.fRadiusX = m_fRadiusX;
	info.fRadiusY = m_fRadiusY;
	info.fRadiusZ = m_fRadiusZ;
	info.fOffsetX = m_fOffsetX;
	info.fOffsetY = m_fOffsetY;
	info.fOffsetZ = m_fOffsetZ;
	info.fTimeBeforeStart = m_fTimeBeforeStart;
	INDEX iEffect = m_comboEffectList.GetCurSel();
	if(iEffect == CB_ERR) return;
	CString strTemp;
	m_comboEffectList.GetLBText(iEffect, strTemp);
	info.strEffectName = strTemp.GetBuffer(0);
	INDEX ret = m_orbitEffect.AddOrbit(info);
	char buf[2] = {0};
	buf[0] = 'a' + ret;
	INDEX iOrbit = m_comboOrbitList.AddString(buf);
	m_comboOrbitList.SetCurSel(iOrbit);
}

void CDlgCOrbitEffectProperty::OnBtnModify() 
{
	this->UpdateData(SAVETOOBJECT);

	COrbitEffect::sOrbitInfo info;
	info.fPhaseMulX = m_fPhaseMulX;
	info.fPhaseMulY = m_fPhaseMulY;
	info.fPhaseMulZ = m_fPhaseMulZ;
	info.fPhaseOffsetX = m_fPhaseOffsetX;
	info.fPhaseOffsetY = m_fPhaseOffsetY;
	info.fPhaseOffsetZ = m_fPhaseOffsetZ;
	info.fRadiusX = m_fRadiusX;
	info.fRadiusY = m_fRadiusY;
	info.fRadiusZ = m_fRadiusZ;
	info.fOffsetX = m_fOffsetX;
	info.fOffsetY = m_fOffsetY;
	info.fOffsetZ = m_fOffsetZ;
	info.fTimeBeforeStart = m_fTimeBeforeStart;
	INDEX iEffect = m_comboEffectList.GetCurSel();
	INDEX iOrbit = m_comboOrbitList.GetCurSel();
	if(iEffect != CB_ERR && iOrbit != CB_ERR)
	{
		CString strEffect;
		m_comboEffectList.GetLBText(iEffect, strEffect);
		info.strEffectName = strEffect.GetBuffer(0);
		CString strOrbit;
		m_comboOrbitList.GetLBText(iOrbit, strOrbit);
		*m_orbitEffect.GetOrbit(strOrbit.GetAt(0) - 'a') = info;
	}
}

void CDlgCOrbitEffectProperty::OnBtnDel() 
{
	if(m_comboOrbitList.GetCurSel() == CB_ERR) return;
	CString strTemp;
	m_comboOrbitList.GetLBText(m_comboOrbitList.GetCurSel(), strTemp);
	m_comboOrbitList.DeleteString(m_comboOrbitList.GetCount() - 1);
	COrbitEffect::sOrbitInfo *pInfo = m_orbitEffect.GetOrbit(INDEX(strTemp.GetAt(0) - 'a'));
	if(pInfo == NULL) return;
	m_orbitEffect.DeleteOrbit(INDEX(strTemp.GetAt(0) - 'a'));
	m_fPhaseMulX = 0.0f;
	m_fPhaseMulY = 0.0f;
	m_fPhaseMulZ = 0.0f;
	m_fPhaseOffsetX = 0.0f;
	m_fPhaseOffsetY = 0.0f;
	m_fPhaseOffsetZ = 0.0f;
	m_fOffsetX = 0.0f;
	m_fOffsetY = 0.0f;
	m_fOffsetZ = 0.0f;
	m_fRadiusX = 0.0f;
	m_fRadiusY = 0.0f;
	m_fRadiusZ = 0.0f;
	m_fTimeBeforeStart = 0.0f;
	m_comboEffectList.SetCurSel(-1);
	this->UpdateData(SAVETOFORM);
}

void CDlgCOrbitEffectProperty::OnSelendokComboOrbitList() 
{
	if(m_comboOrbitList.GetCurSel() == CB_ERR) return;
	CString strTemp;
	m_comboOrbitList.GetLBText(m_comboOrbitList.GetCurSel(), strTemp);
	COrbitEffect::sOrbitInfo *pInfo = m_orbitEffect.GetOrbit(INDEX(strTemp.GetAt(0) - 'a'));
	if(pInfo == NULL) return;
	m_fPhaseMulX = pInfo->fPhaseMulX;
	m_fPhaseMulY = pInfo->fPhaseMulY;
	m_fPhaseMulZ = pInfo->fPhaseMulZ;
	m_fPhaseOffsetX = pInfo->fPhaseOffsetX;
	m_fPhaseOffsetY = pInfo->fPhaseOffsetY;
	m_fPhaseOffsetZ = pInfo->fPhaseOffsetZ;
	m_fRadiusX = pInfo->fRadiusX;
	m_fRadiusY = pInfo->fRadiusY;
	m_fRadiusZ = pInfo->fRadiusZ;
	m_fOffsetX = pInfo->fOffsetX;
	m_fOffsetY = pInfo->fOffsetY;
	m_fOffsetZ = pInfo->fOffsetZ;
	m_fTimeBeforeStart = pInfo->fTimeBeforeStart;
	INDEX iEffect = m_comboEffectList.FindStringExact(0, pInfo->strEffectName.c_str());
	if(iEffect == CB_ERR) m_comboEffectList.SetCurSel(-1);
	else m_comboEffectList.SetCurSel(iEffect);
	this->UpdateData(SAVETOFORM);
}

void CDlgCOrbitEffectProperty::OnClearForm()
{
	m_orbitEffect.ClearOrbit();

	m_comboOrbitList.Clear();
	m_comboOrbitList.ResetContent();

	m_fPhaseMulX = 0.0f;
	m_fPhaseMulY = 0.0f;
	m_fPhaseMulZ = 0.0f;
	m_fPhaseOffsetX = 0.0f;
	m_fPhaseOffsetY = 0.0f;
	m_fPhaseOffsetZ = 0.0f;
	m_fOffsetX = 0.0f;
	m_fOffsetY = 0.0f;
	m_fOffsetZ = 0.0f;
	m_fRadiusX = 0.0f;
	m_fRadiusY = 0.0f;
	m_fRadiusZ = 0.0f;
	m_fTimeBeforeStart = 0.0f;
	m_fFadeInDestRadiusMul = 0.0f;
	m_fFadeInDestPhaseMul = 0.0f;
	m_fFadeOutDestRadiusMul = 0.0f;
	m_fFadeOutDestPhaseMul = 0.0f;
	m_lbFadeInFlag.SetSel(-1, FALSE);
	m_lbFadeInFlag.SetSel(-1, FALSE);
	m_strGoalTag = _T("");
	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCOrbitEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	COrbitEffect::sOrbitInfo info;
	info.fPhaseMulX = m_fPhaseMulX;
	info.fPhaseMulY = m_fPhaseMulY;
	info.fPhaseMulZ = m_fPhaseMulZ;
	info.fPhaseOffsetX = m_fPhaseOffsetX;
	info.fPhaseOffsetY = m_fPhaseOffsetY;
	info.fPhaseOffsetZ = m_fPhaseOffsetZ;
	info.fRadiusX = m_fRadiusX;
	info.fRadiusY = m_fRadiusY;
	info.fRadiusZ = m_fRadiusZ;
	info.fOffsetX = m_fOffsetX;
	info.fOffsetY = m_fOffsetY;
	info.fOffsetZ = m_fOffsetZ;
	info.fTimeBeforeStart = m_fTimeBeforeStart;
	INDEX iEffect = m_comboEffectList.GetCurSel();
	INDEX iOrbit = m_comboOrbitList.GetCurSel();
	if(iEffect != CB_ERR && iOrbit != CB_ERR)
	{
		CString strEffect;
		m_comboEffectList.GetLBText(iEffect, strEffect);
		info.strEffectName = strEffect.GetBuffer(0);
		CString strOrbit;
		m_comboOrbitList.GetLBText(iOrbit, strOrbit);
		*m_orbitEffect.GetOrbit(strOrbit.GetAt(0) - 'a') = info;
	}
	m_orbitEffect.SetFadeInFlag(GetFlagsByLB(m_lbFadeInFlag, FADEIN_ORBIT_BEGIN));
	m_orbitEffect.SetFadeOutFlag(GetFlagsByLB(m_lbFadeOutFlag, FADEOUT_ORBIT_BEGIN));
	m_orbitEffect.SetFadeInDest(m_fFadeInDestRadiusMul, m_fFadeInDestPhaseMul);
	m_orbitEffect.SetFadeOutDest(m_fFadeOutDestRadiusMul, m_fFadeOutDestPhaseMul);
	m_orbitEffect.SetGoalTagName(m_strGoalTag.GetBuffer(0));
	m_orbitEffect.SetPositionType((EFFECT_OF_TAG_TYPE)m_cbPositionType.GetCurSel());
	m_orbitEffect.SetRotationType((EFFECT_OF_TAG_TYPE)m_cbRotationType.GetCurSel());

	ep.RegisterEffect(m_orbitEffect);
	return 0;
}

LRESULT CDlgCOrbitEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		COrbitEffect *pOE = (COrbitEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pOE == NULL) return 0;

		OnClearForm();
		m_cbPositionType.SetCurSel(pOE->GetPositionType());
		m_cbRotationType.SetCurSel(pOE->GetRotationType());
		char buf[2] = {0};
		for(INDEX i=0; i<pOE->GetOrbitCount(); ++i)
		{
			sprintf(buf, "%c", 'a' + i);
			m_orbitEffect.AddOrbit(*(pOE->GetOrbit(i)));
			m_comboOrbitList.AddString(buf);
		}
		m_lbFadeInFlag.SetSel(-1, FALSE);
		SetFlagsToLB(pOE->GetFadeInFlag(), m_lbFadeInFlag, FADEIN_ORBIT_BEGIN);
		m_lbFadeOutFlag.SetSel(-1, FALSE);
		SetFlagsToLB(pOE->GetFadeOutFlag(), m_lbFadeOutFlag, FADEOUT_ORBIT_BEGIN);
		m_fFadeInDestRadiusMul = pOE->GetFadeInDestRadiusMul();
		m_fFadeInDestPhaseMul = pOE->GetFadeInDestPhaseMul();
		m_fFadeOutDestRadiusMul = pOE->GetFadeOutDestRadiusMul();
		m_fFadeOutDestPhaseMul = pOE->GetFadeOutDestPhaseMul();
		m_strGoalTag = pOE->GetGoalTagName();

		ep.SetEffectInfo(*pOE);
		CEffectManager::Instance().Destroy((CEffect *&)pOE);
	}
	this->UpdateData(SAVETOFORM);
	return 0;
}

void CDlgCOrbitEffectProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)
	{
		m_comboEffectList.Clear();
		m_comboEffectList.ResetContent();
		CEffectManager::my_map &map = CEffectManager::Instance().GetRegisteredMap();
		CEffectManager::my_map::iterator iterBegin = map.begin();
		CEffectManager::my_map::iterator iterEnd = map.end();
		CEffectManager::my_map::iterator iter;
		for(iter=iterBegin; iter!=iterEnd; ++iter)
		{
			//문제가 생길 소지가 있음. 무한 반복 이펙트랄까?
			//주석 풀면 그런 문제는 안생기지만 제약이 있음.
			/*
			if( (*iter).second->GetType() != ET_SPLINEPATH
			|| (*iter).second->GetType() != ET_ORBIT )
			*/
			{
				m_comboEffectList.AddString((*iter).second->GetName().c_str());
			}
		}
	}
}

BOOL CDlgCOrbitEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lbFadeInFlag.AddString("Radius X");
	m_lbFadeInFlag.AddString("Phase X");
	m_lbFadeInFlag.AddString("Radius Y");
	m_lbFadeInFlag.AddString("Phase Y");
	m_lbFadeInFlag.AddString("Radius Z");
	m_lbFadeInFlag.AddString("Phase Z");
	m_lbFadeInFlag.AddString("Offset");
	
	m_lbFadeOutFlag.AddString("Radius X");
	m_lbFadeOutFlag.AddString("Phase X");
	m_lbFadeOutFlag.AddString("Radius Y");
	m_lbFadeOutFlag.AddString("Phase Y");
	m_lbFadeOutFlag.AddString("Radius Z");
	m_lbFadeOutFlag.AddString("Phase Z");
	m_lbFadeOutFlag.AddString("Offset");

	m_cbPositionType.AddString("None");
	m_cbPositionType.AddString("Start Once");
	m_cbPositionType.AddString("Always");

	m_cbRotationType.AddString("None");
	m_cbRotationType.AddString("Start Once");
	m_cbRotationType.AddString("Always");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
