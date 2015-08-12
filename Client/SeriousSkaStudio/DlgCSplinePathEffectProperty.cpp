// DlgCSplinePathEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCSplinePathEffectProperty.h"
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CSplinePathEffect.h>

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplinePathEffectProperty dialog


CDlgCSplinePathEffectProperty::CDlgCSplinePathEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCSplinePathEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSplinePathEffectProperty)
	m_fOffsetX = 0.0f;
	m_fOffsetY = 0.0f;
	m_fOffsetZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCSplinePathEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSplinePathEffectProperty)
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_cbRotationType);
	DDX_Control(pDX, IDC_COMBO_POSITION_TYPE, m_cbPositionType);
	DDX_Control(pDX, IDC_LIST_POS, m_lbPosList);
	DDX_Control(pDX, IDC_COMBO_LIST, m_cbPathList);
	DDX_Control(pDX, IDC_COMBO_EFFECT_LIST, m_cbEffectList);
	DDX_Text(pDX, IDC_OFFSET_X, m_fOffsetX);
	DDX_Text(pDX, IDC_OFFSET_Y, m_fOffsetY);
	DDX_Text(pDX, IDC_OFFSET_Z, m_fOffsetZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSplinePathEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCSplinePathEffectProperty)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELENDOK(IDC_COMBO_LIST, OnSelendokComboList)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_LBN_SELCHANGE(IDC_LIST_POS, OnSelchangeListPos)
	ON_BN_CLICKED(IDC_BTN_POSLIST_ADD, OnBtnPoslistAdd)
	ON_BN_CLICKED(IDC_BTN_POSLIST_DEL, OnBtnPoslistDel)
	ON_BN_CLICKED(IDC_CHECK_DIRECTAPPLY, OnCheckDirectapply)
	ON_EN_CHANGE(IDC_ED_POSX, OnChangeEd)
	ON_EN_CHANGE(IDC_ED_POSY, OnChangeEd)
	ON_EN_CHANGE(IDC_ED_POSZ, OnChangeEd)
	ON_EN_CHANGE(IDC_ED_TIME, OnChangeEd)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplinePathEffectProperty message handlers

LRESULT CDlgCSplinePathEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);
	if(m_cbPathList.GetCount() == 0) return 0;

	m_pSplinePathEffect->SetPositionType((EFFECT_OF_TAG_TYPE)m_cbPositionType.GetCurSel());
	m_pSplinePathEffect->SetRotationType((EFFECT_OF_TAG_TYPE)m_cbRotationType.GetCurSel());
	m_pSplinePathEffect->SetOffsetPos(FLOAT3D(m_fOffsetX, m_fOffsetY, m_fOffsetZ));
	ep.RegisterEffect(*m_pSplinePathEffect);
	return 0;
}

LRESULT CDlgCSplinePathEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CSplinePathEffect *pSPE = (CSplinePathEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pSPE == NULL) return 0;
		OnClearForm();
		if(m_pSplinePathEffect) delete m_pSplinePathEffect;
		m_pSplinePathEffect = (CSplinePathEffect*)pSPE->Copy();

		m_cbPositionType.SetCurSel(INDEX(pSPE->GetPositionType()));
		m_cbRotationType.SetCurSel(INDEX(pSPE->GetRotationType()));
		m_fOffsetX = pSPE->GetOffsetPos()(1);
		m_fOffsetY = pSPE->GetOffsetPos()(2);
		m_fOffsetZ = pSPE->GetOffsetPos()(3);
		char buf[2] = {0};
		for(INDEX i=0; i<pSPE->GetSplinePathCount(); ++i)
		{
			sprintf(buf, "%c", 'a' + i);
			m_cbPathList.AddString(buf);
		}
		if(m_cbPathList.GetCount() > 0)
		{
			m_cbPathList.SetCurSel(0);
			OnSelendokComboList();
		}

		ep.SetEffectInfo(*pSPE);
		CEffectManager::Instance().Destroy((CEffect *&)pSPE);
	}
	this->UpdateData(SAVETOFORM);
	return 0;
}

void CDlgCSplinePathEffectProperty::OnClearForm()
{
	m_cbPathList.ResetContent();
	m_lbPosList.ResetContent();
	SetDlgItemText(IDC_ED_POSX, "0");
	SetDlgItemText(IDC_ED_POSY, "0");
	SetDlgItemText(IDC_ED_POSZ, "0");
	SetDlgItemText(IDC_ED_TIME, "0");
	m_cbEffectList.SetCurSel(-1);
	m_cbPositionType.SetCurSel(0);
	m_cbRotationType.SetCurSel(0);
	m_fOffsetX = m_fOffsetY = m_fOffsetZ = 0;

	if(m_pSplinePathEffect) delete m_pSplinePathEffect;
	m_pSplinePathEffect = new CSplinePathEffect;
	m_splinePathInfo = CSplinePathEffect::sSplinePathInfo();
}

BOOL CDlgCSplinePathEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cbPositionType.AddString("None");
	m_cbPositionType.AddString("Start Once");
	m_cbPositionType.AddString("Always");
	m_cbPositionType.SetCurSel(2);

	m_cbRotationType.AddString("None");
	m_cbRotationType.AddString("Start Once");
	m_cbRotationType.AddString("Always");
	m_cbRotationType.SetCurSel(1);

	m_bDirectApply = FALSE;

	m_pSplinePathEffect = new CSplinePathEffect;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSplinePathEffectProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		m_cbEffectList.Clear();
		m_cbEffectList.ResetContent();
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
				m_cbEffectList.AddString((*iter).second->GetName().c_str());
			}
		}
	}
	else
	{
		((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_bShowSpline = FALSE;
	}
}

void CDlgCSplinePathEffectProperty::RefreshPosList()
{
	m_lbPosList.ResetContent();

	INDEX iPath = m_cbPathList.GetCurSel();
	CSplinePathEffect::sSplinePathInfo *pSPath = &m_splinePathInfo;

	for(INDEX iPos=0; iPos<pSPath->ssPath.GetSampleCount(); ++iPos)
	{
		const FLOAT3D &pos = pSPath->ssPath.GetValue(iPos);
		FLOAT time = pSPath->ssPath.GetKey(iPos);
		CString strTemp;
		strTemp.Format("%4.2f / ( %4.2f, %4.2f, %4.2f )"
			, time, pos(1), pos(2), pos(3));
		m_lbPosList.AddString(strTemp);
	}
	INDEX iEffect = m_cbEffectList.FindStringExact(0, pSPath->strEffectName.c_str());
	if(iEffect == CB_ERR) m_cbEffectList.SetCurSel(-1);
	else m_cbEffectList.SetCurSel(iEffect);
	RefreshPosSpline();
}

void CDlgCSplinePathEffectProperty::SelectPosListRow(INDEX iPos)
{
	if(m_lbPosList.GetCount() <= iPos) return;

	INDEX iPath = m_cbPathList.GetCurSel();
	CSplinePathEffect::sSplinePathInfo *pSPath = &m_splinePathInfo;

	m_lbPosList.SetCurSel(iPos);
	CString strTemp;
	strTemp.Format("%f", pSPath->ssPath.GetKey(iPos));
	this->SetDlgItemText(IDC_ED_TIME, strTemp);
	FLOAT3D pos = pSPath->ssPath.GetValue(iPos);
	strTemp.Format("%f", pos(1));
	this->SetDlgItemText(IDC_ED_POSX, strTemp);
	strTemp.Format("%f", pos(2));
	this->SetDlgItemText(IDC_ED_POSY, strTemp);
	strTemp.Format("%f", pos(3));
	this->SetDlgItemText(IDC_ED_POSZ, strTemp);
}

void CDlgCSplinePathEffectProperty::OnSelendokComboList() 
{
	INDEX iPath = m_cbPathList.GetCurSel();
	m_splinePathInfo = *m_pSplinePathEffect->GetSplinePath(iPath);
	RefreshPosList();
	UpdateData(SAVETOFORM);
	SelectPosListRow(0);
}

void CDlgCSplinePathEffectProperty::OnBtnAdd() 
{
	if(m_pSplinePathEffect->GetSplinePathCount() > INDEX('z' - 'a')) return;

	INDEX iEffect = m_cbEffectList.GetCurSel();
	if(iEffect == CB_ERR) return;
	CString strTemp;
	m_cbEffectList.GetLBText(iEffect, strTemp);
	m_splinePathInfo.strEffectName = strTemp.GetBuffer(0);
	INDEX ret = m_pSplinePathEffect->AddSplinePath(m_splinePathInfo);
	char buf[2] = {0};
	buf[0] = 'a' + ret;
	INDEX iPath = m_cbPathList.AddString(buf);
	m_cbPathList.SetCurSel(iPath);
	UpdateData(SAVETOFORM);
}

void CDlgCSplinePathEffectProperty::OnBtnDel() 
{
	if(m_cbPathList.GetCurSel() == CB_ERR) return;
	CString strTemp;
	m_cbPathList.GetLBText(m_cbPathList.GetCurSel(), strTemp);
	m_cbPathList.DeleteString(m_cbPathList.GetCount() - 1);
	INDEX iPath = INDEX(strTemp.GetAt(0) - 'a');
	CSplinePathEffect::sSplinePathInfo *pInfo
		= m_pSplinePathEffect->GetSplinePath(iPath);
	if(pInfo == NULL) return;
	m_pSplinePathEffect->DeleteSplinePath(iPath);
	if(m_cbPathList.GetCount() > 0)
	{
		m_cbPathList.SetCurSel(0);
		OnSelendokComboList();
	}
	UpdateData(SAVETOFORM);
}

void CDlgCSplinePathEffectProperty::OnBtnModify() 
{
	if(m_cbPathList.GetCurSel() == CB_ERR) return;
	CString strTemp;
	m_cbPathList.GetLBText(m_cbPathList.GetCurSel(), strTemp);
	INDEX iPath = INDEX(strTemp.GetAt(0) - 'a');
	CSplinePathEffect::sSplinePathInfo *pInfo
		= m_pSplinePathEffect->GetSplinePath(iPath);
	if(pInfo == NULL) return;
	m_cbEffectList.GetLBText(m_cbEffectList.GetCurSel(), strTemp);
	m_splinePathInfo.strEffectName = strTemp.GetBuffer(0);
	*pInfo = m_splinePathInfo;
	UpdateData(SAVETOFORM);
}

void CDlgCSplinePathEffectProperty::OnSelchangeListPos() 
{
	BOOL old = m_bDirectApply;
	m_bDirectApply = FALSE;
	if(m_lbPosList.GetCurSel() == LB_ERR) return;
	SelectPosListRow(m_lbPosList.GetCurSel());
	m_bDirectApply = old;
	RefreshPosSpline();
}

void CDlgCSplinePathEffectProperty::OnBtnPoslistAdd() 
{
	CString strTemp;
	this->GetDlgItemText(IDC_ED_TIME, strTemp);
	FLOAT time = atof(strTemp.GetBuffer(0));
	FLOAT3D pos;
	this->GetDlgItemText(IDC_ED_POSX, strTemp);
	pos(1) = atof(strTemp.GetBuffer(0));
	this->GetDlgItemText(IDC_ED_POSY, strTemp);
	pos(2) = atof(strTemp.GetBuffer(0));
	this->GetDlgItemText(IDC_ED_POSZ, strTemp);
	pos(3) = atof(strTemp.GetBuffer(0));
	int ret = m_splinePathInfo.ssPath.AddSample(time, pos);
	if(ret == -1) return;

	strTemp.Format("%4.2f / ( %4.2f, %4.2f, %4.2f )"
		, time, pos(1), pos(2), pos(3));
	m_lbPosList.InsertString(ret, strTemp);
	m_lbPosList.SetCurSel(ret);

	m_splinePathInfo.ssPath.Prepare();
	RefreshPosSpline();
}

void CDlgCSplinePathEffectProperty::OnBtnPoslistDel() 
{
	if(LB_ERR == m_lbPosList.GetCurSel()) return;
	m_splinePathInfo.ssPath.RemoveSample(m_lbPosList.GetCurSel());
	m_splinePathInfo.ssPath.Prepare();
	m_lbPosList.DeleteString(m_lbPosList.GetCurSel());
	if(m_lbPosList.GetCount() > 0)
	{
		SelectPosListRow(0);
	}
	RefreshPosSpline();
}

void CDlgCSplinePathEffectProperty::OnCheckDirectapply() 
{
	m_bDirectApply = !m_bDirectApply;
}

void CDlgCSplinePathEffectProperty::OnChangeEd() 
{
	if(!m_bDirectApply) return;
	if(LB_ERR == m_lbPosList.GetCurSel()) return;
	m_splinePathInfo.ssPath.RemoveSample(m_lbPosList.GetCurSel());
	m_splinePathInfo.ssPath.Prepare();
	m_lbPosList.DeleteString(m_lbPosList.GetCurSel());
	OnBtnPoslistAdd();
}

void CDlgCSplinePathEffectProperty::RefreshPosSpline()
{
	if(m_splinePathInfo.ssPath.GetSampleCount() < 2) return;

	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_bShowSpline = TRUE;
	SINT sel = 0;
	if(m_lbPosList.GetCurSel() != LB_ERR) sel
		= SINT( m_splinePathInfo.ssPath.GetKey(m_lbPosList.GetCurSel()) / _pTimer->TickQuantum );
	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_siCurrentPoint = sel;
		
	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_vectorSplinePoint.clear();
	m_splinePathInfo.ssPath.Prepare();
	FLOAT timeBegin = m_splinePathInfo.ssPath.GetKey(0);
	FLOAT timeEnd = m_splinePathInfo.ssPath.GetKey(m_splinePathInfo.ssPath.GetSampleCount()-1);
	for(FLOAT time=timeBegin; time<=timeEnd; time+=_pTimer->TickQuantum)
	{
		((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_vectorSplinePoint.push_back(
			m_splinePathInfo.ssPath.Value(time) );
	}
}
