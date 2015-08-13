// DlgCShockWaveEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCShockWaveEffectProperty.h"

#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CShockWaveEffect.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCShockWaveEffectProperty dialog


CDlgCShockWaveEffectProperty::CDlgCShockWaveEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCShockWaveEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCShockWaveEffectProperty)
	m_bOuterBasis = FALSE;
	m_fAngleValue = 0.0f;
	m_fRadiusValue = 0.0f;
	m_strFileName = _T("");
	m_fAngleTime = 0.0f;
	m_fRadiusTime = 0.0f;
	m_fWidthTime = 0.0f;
	m_uiSplitCount = 0;
	m_fWidthValue = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCShockWaveEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCShockWaveEffectProperty)
	DDX_Control(pDX, IDC_BLEND_TYPE, m_cbBlendType);
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_comboRotationType);
	DDX_Control(pDX, IDC_COMBO_POSITION_TYPE, m_comboPositionType);
	DDX_Control(pDX, IDC_SHOCKWAVE_WIDTHLIST, m_listWidth);
	DDX_Control(pDX, IDC_SHOCKWAVE_RADIUSLIST, m_listRadius);
	DDX_Control(pDX, IDC_SHOCKWAVE_ANGLELIST, m_listAngle);
	DDX_Check(pDX, IDC_CHECK_OUTERBASIS, m_bOuterBasis);
	DDX_Text(pDX, IDC_SHOCKWAVE_ANGLEVALUE, m_fAngleValue);
	DDX_Text(pDX, IDC_SHOCKWAVE_RADIUSVALUE, m_fRadiusValue);
	DDX_Text(pDX, IDC_SHOCKWAVE_FILENAME, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, 255);
	DDX_Text(pDX, IDC_SHOCKWAVE_ANGLETIME, m_fAngleTime);
	DDX_Text(pDX, IDC_SHOCKWAVE_RADIUSTIME, m_fRadiusTime);
	DDX_Text(pDX, IDC_SHOCKWAVE_WIDTHTIME, m_fWidthTime);
	DDX_Text(pDX, IDC_SHOCKWAVE_SPLITCOUNT, m_uiSplitCount);
	DDX_Text(pDX, IDC_SHOCKWAVE_WIDTHVALUE, m_fWidthValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCShockWaveEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCShockWaveEffectProperty)
	ON_BN_CLICKED(IDC_SHOCKWAVE_ANGLEADD, OnShockwaveAngleadd)
	ON_BN_CLICKED(IDC_SHOCKWAVE_ANGLEDELETE, OnShockwaveAngledelete)
	ON_BN_CLICKED(IDC_SHOCKWAVE_ANGLEMODIFY, OnShockwaveAnglemodify)
	ON_LBN_SELCHANGE(IDC_SHOCKWAVE_ANGLELIST, OnSelchangeShockwaveAnglelist)
	ON_BN_CLICKED(IDC_SHOCKWAVE_FILEFIND, OnShockwaveFilefind)
	ON_BN_CLICKED(IDC_SHOCKWAVE_RADIUSADD, OnShockwaveRadiusadd)
	ON_BN_CLICKED(IDC_SHOCKWAVE_RADIUSDELETE, OnShockwaveRadiusdelete)
	ON_BN_CLICKED(IDC_SHOCKWAVE_RADIUSMODIFY, OnShockwaveRadiusmodify)
	ON_LBN_SELCHANGE(IDC_SHOCKWAVE_RADIUSLIST, OnSelchangeShockwaveRadiuslist)
	ON_BN_CLICKED(IDC_SHOCKWAVE_WIDTHADD, OnShockwaveWidthadd)
	ON_BN_CLICKED(IDC_SHOCKWAVE_WIDTHDELETE, OnShockwaveWidthdelete)
	ON_LBN_SELCHANGE(IDC_SHOCKWAVE_WIDTHLIST, OnSelchangeShockwaveWidthlist)
	ON_BN_CLICKED(IDC_SHOCKWAVE_WIDTHMODIFY, OnShockwaveWidthmodify)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCShockWaveEffectProperty message handlers

void CDlgCShockWaveEffectProperty::OnClearForm()
{
	m_ssRadius.Clear();
	m_ssWidth.Clear();
	m_ssAngle.Clear();
	m_listWidth.ResetContent();
	m_listRadius.ResetContent();
	m_listAngle.ResetContent();
	m_bOuterBasis = FALSE;
	m_fAngleValue = 0;
	m_fRadiusValue = 0;
	m_fWidthValue = 0;
	m_strFileName = "";
	m_uiSplitCount = 8;//minimum

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCShockWaveEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_strFileName == "") return 0;
	CShockWaveEffect swe;
	CTFileName filename = m_strFileName;
	swe.SetTexture(filename);
	swe.SetInnerBasis(!m_bOuterBasis);
	swe.SetPositionType((EFFECT_OF_TAG_TYPE)m_comboPositionType.GetCurSel());
	swe.SetRotationType((EFFECT_OF_TAG_TYPE)m_comboRotationType.GetCurSel());
	swe.SetSplitCount(m_uiSplitCount);
	if(m_cbBlendType.GetCurSel() == CB_ERR) return 0;
	swe.SetBlendType( PredefinedBlendType(201 + m_cbBlendType.GetCurSel()) );

	UINT i = 0;
	int count = m_ssAngle.GetSampleCount();
	for( i = 0; i < count; ++i)
	{
		swe.AddHeight(m_ssAngle.GetKey(i), m_ssAngle.GetValue(i));
	}

	count = m_ssRadius.GetSampleCount();
	for( i = 0; i < count; ++i)
	{
		swe.AddRadius(m_ssRadius.GetKey(i), m_ssRadius.GetValue(i));
	}

	count = m_ssWidth.GetSampleCount();
	for( i = 0; i < count; ++i)
	{
		swe.AddWidth(m_ssWidth.GetKey(i), m_ssWidth.GetValue(i));
	}

	ep.RegisterEffect(swe);
	return 0;
}

LRESULT CDlgCShockWaveEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index == CB_ERR) return 0;

	ep.m_cbRegisteredName.GetLBText( index, strName );
	CShockWaveEffect *pSWE = (CShockWaveEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
	if(pSWE == NULL) return 0;

	this->m_bOuterBasis = !pSWE->GetInnerBasis();
	this->m_comboPositionType.SetCurSel(pSWE->GetPositionType());
	this->m_comboRotationType.SetCurSel(pSWE->GetRotationType());
	this->m_uiSplitCount = pSWE->GetSplitCount();
	char buf[256] = {0};
	m_ssAngle.Clear();
	m_listAngle.ResetContent();

	UINT i = 0;
	int count = pSWE->GetHeightCount();
	for( i = 0; i < count; ++i)
	{
		this->m_ssAngle.AddSample(pSWE->GetHeightTime(i), pSWE->GetHeightValue(i));
		sprintf(buf, "%4.2f/%4.2f", m_ssAngle.GetKey(i), m_ssAngle.GetValue(i));
		this->m_listAngle.AddString(buf);
	}
	m_ssRadius.Clear();
	m_listRadius.ResetContent();
	
	count = pSWE->GetRadiusCount();
	for( i = 0; i < count; ++i)
	{
		this->m_ssRadius.AddSample(pSWE->GetRadiusTime(i), pSWE->GetRadiusValue(i));
		sprintf(buf, "%4.2f/%4.2f", m_ssRadius.GetKey(i), m_ssRadius.GetValue(i));
		this->m_listRadius.AddString(buf);
	}
	m_ssWidth.Clear();
	m_listWidth.ResetContent();

	count = pSWE->GetWidthCount();
	for( i = 0; i < count; ++i)
	{
		this->m_ssWidth.AddSample(pSWE->GetWidthTime(i), pSWE->GetWidthValue(i));
		sprintf(buf, "%4.2f/%4.2f", m_ssWidth.GetKey(i), m_ssWidth.GetValue(i));
		this->m_listWidth.AddString(buf);
	}
	this->m_strFileName = pSWE->GetTexFileName().str_String;
	this->UpdateData(SAVETOFORM);
	this->m_cbBlendType.SetCurSel( int(pSWE->GetBlendType()) - 201 );

	ep.SetEffectInfo(*pSWE);
	CEffectManager::Instance().Destroy((CEffect *&)pSWE);

	return 0;
}

void CDlgCShockWaveEffectProperty::OnShockwaveFilefind() 
{
	TEXSELECTDLG(m_strFileName);
}

/*
#define ON_SHOCKWAVE_ADD(type) \
void CDlgCShockWaveEffectProperty::OnShockwave##type##add() \
{\
	this->UpdateData(SAVETOOBJECT);\
	if(-1 == this->m_ss##type.AddSample(m_f##type##Time, m_f##type##Value)) return;\
	char buf[256] = {0};\
	sprintf(buf, "%4.2f", m_f##type##Time);\
	int ret = this->m_list##type.AddString(buf);\
	this->m_list##type.SetCurSel(ret);\
}
ON_SHOCKWAVE_ADD(Angle)
ON_SHOCKWAVE_ADD(Radius)
ON_SHOCKWAVE_ADD(Width)
*/
void CDlgCShockWaveEffectProperty::OnShockwaveAngleadd() 
{
	int ret1;
	this->UpdateData(SAVETOOBJECT);
	if(-1 == (ret1 = this->m_ssAngle.AddSample(m_fAngleTime, m_fAngleValue))) return;
	char buf[256] = {0};
	sprintf(buf, "%4.2f/%4.2f", m_fAngleTime, m_fAngleValue);
	int ret = this->m_listAngle.AddString(buf);
	ASSERT(ret == ret1);
	this->m_listAngle.SetCurSel(ret);
}
void CDlgCShockWaveEffectProperty::OnShockwaveRadiusadd()
{
	int ret1;
	this->UpdateData(SAVETOOBJECT);
	if(-1 == (ret1 = this->m_ssRadius.AddSample(m_fRadiusTime, m_fRadiusValue))) return;
	char buf[256] = {0};
	sprintf(buf, "%4.2f/%4.2f", m_fRadiusTime, m_fRadiusValue);
	int ret = this->m_listRadius.AddString(buf);
	ASSERT(ret == ret1);
	this->m_listRadius.SetCurSel(ret);
}
void CDlgCShockWaveEffectProperty::OnShockwaveWidthadd()
{
	int ret1;
	this->UpdateData(SAVETOOBJECT);
	if(-1 == (ret1 = this->m_ssWidth.AddSample(m_fWidthTime, m_fWidthValue))) return;
	char buf[256] = {0};
	sprintf(buf, "%4.2f/%4.2f", m_fWidthTime, m_fWidthValue);
	int ret = this->m_listWidth.AddString(buf);
	ASSERT(ret == ret1);
	this->m_listWidth.SetCurSel(ret);
}

/*
#define ON_SHOCKWAVE_DELETE(type) \
void CDlgCShockWaveEffectProperty::OnShockwave##type##delete() \
{\
	int ret;\
	if( LB_ERR == (ret = m_list##type.GetCurSel()) ) return;\
	m_list##type.DeleteString(ret);\
	m_ss##type##.RemoveSample(ret);\
	m_list##type.SetCurSel(ret);\
}
ON_SHOCKWAVE_DELETE(Angle);
ON_SHOCKWAVE_DELETE(Radius);
ON_SHOCKWAVE_DELETE(Width);
*/
void CDlgCShockWaveEffectProperty::OnShockwaveAngledelete()
{
	int ret;
	if( LB_ERR == (ret = m_listAngle.GetCurSel()) ) return;
	m_listAngle.DeleteString(ret);
	m_ssAngle.RemoveSample(ret);
	m_listAngle.SetCurSel(ret);
}

void CDlgCShockWaveEffectProperty::OnShockwaveRadiusdelete()
{
	int ret;
	if( LB_ERR == (ret = m_listRadius.GetCurSel()) ) return;
	m_listRadius.DeleteString(ret);
	m_ssRadius.RemoveSample(ret);
	m_listRadius.SetCurSel(ret);
}

void CDlgCShockWaveEffectProperty::OnShockwaveWidthdelete()
{
	int ret;
	if( LB_ERR == (ret = m_listWidth.GetCurSel()) ) return;
	m_listWidth.DeleteString(ret);
	m_ssWidth.RemoveSample(ret);
	m_listWidth.SetCurSel(ret);
}

/*
#define ON_SHOCKWAVE_MODIFY(type) \
void CDlgCShockWaveEffectProperty::OnShockwave##type##modify() \
{\
	if(LB_ERR == m_list##type.GetCurSel()) return;\
	OnShockwave##type##delete();\
	OnShockwave##type##add();\
}
ON_SHOCKWAVE_MODIFY(Angle);
ON_SHOCKWAVE_MODIFY(Radius);
ON_SHOCKWAVE_MODIFY(Width);
*/
void CDlgCShockWaveEffectProperty::OnShockwaveAnglemodify()
{
	if(LB_ERR == m_listAngle.GetCurSel()) return;
	OnShockwaveAngledelete();
	OnShockwaveAngleadd();
}

void CDlgCShockWaveEffectProperty::OnShockwaveRadiusmodify()
{
	if(LB_ERR == m_listRadius.GetCurSel()) return;
	OnShockwaveRadiusdelete();
	OnShockwaveRadiusadd();
}

void CDlgCShockWaveEffectProperty::OnShockwaveWidthmodify()
{
	if(LB_ERR == m_listWidth.GetCurSel()) return;
	OnShockwaveWidthdelete();
	OnShockwaveWidthadd();
}

/*
#define ON_SHOCKWAVE_SELCHANGE(type) \
void CDlgCShockWaveEffectProperty::OnSelchangeShockwave##type##list() \
{\
	int ret;\
	if(LB_ERR == (ret = m_list##type##.GetCurSel())) return;\
	m_f##type##Time = m_ss##type.GetKey(ret);\
	m_f##type##Value = m_ss##type.GetValue(ret);\
	UpdateData(SAVETOFORM);\
}
ON_SHOCKWAVE_SELCHANGE(Angle);
ON_SHOCKWAVE_SELCHANGE(Radius);
ON_SHOCKWAVE_SELCHANGE(Width);
*/
void CDlgCShockWaveEffectProperty::OnSelchangeShockwaveAnglelist()
{
	int ret;
	if(LB_ERR == (ret = m_listAngle.GetCurSel())) return;
	m_fAngleTime = m_ssAngle.GetKey(ret);
	m_fAngleValue = m_ssAngle.GetValue(ret);
	UpdateData(SAVETOFORM);
}
void CDlgCShockWaveEffectProperty::OnSelchangeShockwaveRadiuslist()
{
	int ret;
	if(LB_ERR == (ret = m_listRadius.GetCurSel())) return;
	m_fRadiusTime = m_ssRadius.GetKey(ret);
	m_fRadiusValue = m_ssRadius.GetValue(ret);
	UpdateData(SAVETOFORM);
}
void CDlgCShockWaveEffectProperty::OnSelchangeShockwaveWidthlist()
{
	int ret;
	if(LB_ERR == (ret = m_listWidth.GetCurSel())) return;
	m_fWidthTime = m_ssWidth.GetKey(ret);
	m_fWidthValue = m_ssWidth.GetValue(ret);
	UpdateData(SAVETOFORM);
}

BOOL CDlgCShockWaveEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboPositionType.AddString("None");
	m_comboPositionType.AddString("Start Once");
	m_comboPositionType.AddString("Always");

	m_comboRotationType.AddString("None");
	m_comboRotationType.AddString("Start Once");
	m_comboRotationType.AddString("Always");
	
	m_cbBlendType.AddString("Opaque");		//201
	m_cbBlendType.AddString("Transparent");
	m_cbBlendType.AddString("Blend");
	m_cbBlendType.AddString("Shade");
	m_cbBlendType.AddString("Add");
	m_cbBlendType.AddString("AddAlpha");
	m_cbBlendType.AddString("Multiply");
	m_cbBlendType.AddString("InvMultiply");	//208

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
