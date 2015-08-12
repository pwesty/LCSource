// DlgCCameraEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCCameraEffectProperty.h"

#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCCameraEffectProperty dialog


CDlgCCameraEffectProperty::CDlgCCameraEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCCameraEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCCameraEffectProperty)
	m_fHeight = 0.0f;
	m_fTwist = 0.0f;
	m_fTime = 0.0f;
	m_fWidth = 0.0f;
	m_fZoom = 0.0f;
	m_fMoveX = 0.0f;
	m_fMoveY = 0.0f;
	m_fMoveZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCCameraEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCCameraEffectProperty)
	DDX_Control(pDX, IDC_LIST, m_lbCameraShake);
	DDX_Text(pDX, IDC_ED_HEIGHT, m_fHeight);
	DDX_Text(pDX, IDC_ED_TWIST, m_fTwist);
	DDX_Text(pDX, IDC_ED_TIME, m_fTime);
	DDX_Text(pDX, IDC_ED_WIDTH, m_fWidth);
	DDX_Text(pDX, IDC_ED_ZOOM, m_fZoom);
	DDX_Text(pDX, IDC_ED_MOVEX, m_fMoveX);
	DDX_Text(pDX, IDC_ED_MOVEY, m_fMoveY);
	DDX_Text(pDX, IDC_ED_MOVEZ, m_fMoveZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCCameraEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCCameraEffectProperty)
	ON_BN_CLICKED(IDC_BTN_LIST_ADDMOD, OnBtnListAddmod)
	ON_BN_CLICKED(IDC_BTN_LIST_DEL, OnBtnListDel)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCCameraEffectProperty message handlers
void CDlgCCameraEffectProperty::OnClearForm()
{
	m_fHeight = 0.0f;
	m_fTwist = 0.0f;
	m_fTime = 0.0f;
	m_fWidth = 0.0f;
	m_fZoom = 0.0f;
	m_fMoveX = 0.0f;
	m_fMoveY = 0.0f;
	m_fMoveZ = 0.0f;
	m_ssCameraShake.Clear();
	m_lbCameraShake.ResetContent();
	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCCameraEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	CCameraEffect ce;
	for(INDEX i=0; i<m_ssCameraShake.GetSampleCount(); ++i)
	{
		ce.AddShakeValue(m_ssCameraShake.GetKey(i), m_ssCameraShake.GetValue(i));
	}

	ep.RegisterEffect(ce);

	return 0;
}

static char *szFormat = "%1.3f//(%2.2f, %2.2f)/ %3.3f/ %1.2f/(%2.2f, %2.2f, %2.2f)";
LRESULT CDlgCCameraEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	OnClearForm();

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CCameraEffect *pCE = (CCameraEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pCE == NULL) return 0;

		char buf[256];
		for(INDEX i=0; i<pCE->GetShakeValueCount(); ++i)
		{
			m_ssCameraShake.AddSample(pCE->GetShakeValueTime(i), pCE->GetShakeValueValue(i));
			sprintf(buf, szFormat
						, pCE->GetShakeValueTime(i)
						, pCE->GetShakeValueValue(i).m_fHorizonal
						, pCE->GetShakeValueValue(i).m_fVertical
						, pCE->GetShakeValueValue(i).m_fTwist
						, pCE->GetShakeValueValue(i).m_fZoom
						, pCE->GetShakeValueValue(i).m_vMoveByCharCoord(1)
						, pCE->GetShakeValueValue(i).m_vMoveByCharCoord(2)
						, pCE->GetShakeValueValue(i).m_vMoveByCharCoord(3)
						);
			m_lbCameraShake.AddString(buf);
		}

		ep.SetEffectInfo(*pCE);
		CEffectManager::Instance().Destroy((CEffect *&)pCE);
	}
	UpdateData(SAVETOFORM);
	return 0;
}

void CDlgCCameraEffectProperty::OnBtnListAddmod() 
{
	UpdateData(SAVETOOBJECT);

	CCameraEffect::CCameraValue cv;
	cv.m_fHorizonal = m_fWidth;
	cv.m_fVertical = m_fHeight;
	cv.m_fTwist = m_fTwist;
	cv.m_fZoom = m_fZoom;
	cv.m_vMoveByCharCoord = FLOAT3D(m_fMoveX, m_fMoveY, m_fMoveZ);
	int index = m_ssCameraShake.AddSample(m_fTime, cv);
	if(index == -1)	//modify
	{
		int i = 0;
		int count = m_ssCameraShake.GetSampleCount();
		for( i = 0; i < count; ++i)
		{
			if(m_ssCameraShake.GetKey(i) == m_fTime) break;
		}
		index = i;
		m_ssCameraShake.RemoveSample(index);
		m_lbCameraShake.DeleteString(index);
		m_ssCameraShake.AddSample(m_fTime, cv);
	}

	char buf[256];
	sprintf(buf, szFormat
				, m_fTime
				, m_fWidth
				, m_fHeight
				, m_fTwist
				, m_fZoom
				, m_fMoveX
				, m_fMoveY
				, m_fMoveZ
				);
	index = m_lbCameraShake.InsertString(index, buf);
	
	m_lbCameraShake.SetCurSel(index);
	OnSelchangeList();
}

void CDlgCCameraEffectProperty::OnBtnListDel() 
{
	int index = m_lbCameraShake.GetCurSel();
	if(index == LB_ERR) return;
	
	m_ssCameraShake.RemoveSample(index);
	int cnt = m_lbCameraShake.DeleteString(index);
	if(index >= cnt) index = cnt - 1;

	m_lbCameraShake.SetCurSel(index);
	OnSelchangeList();
}

void CDlgCCameraEffectProperty::OnSelchangeList() 
{
	int index = m_lbCameraShake.GetCurSel();
	if(index == LB_ERR) return;
	m_fTime = m_ssCameraShake.GetKey(index);
	m_fWidth = m_ssCameraShake.GetValue(index).m_fHorizonal;
	m_fHeight = m_ssCameraShake.GetValue(index).m_fVertical;
	m_fTwist = m_ssCameraShake.GetValue(index).m_fTwist;
	m_fZoom = m_ssCameraShake.GetValue(index).m_fZoom;
	m_fMoveX = m_ssCameraShake.GetValue(index).m_vMoveByCharCoord(1);
	m_fMoveY = m_ssCameraShake.GetValue(index).m_vMoveByCharCoord(2);
	m_fMoveZ = m_ssCameraShake.GetValue(index).m_vMoveByCharCoord(3);

	UpdateData(SAVETOFORM);
}

BOOL CDlgCCameraEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lbCameraShake.SetHorizontalExtent(320);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
