// DlgCSplineBillboardEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCSplineBillboardEffectProperty.h"
#include <Engine/Effect/CSplineBillboardEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplineBillboardEffectProperty dialog


CDlgCSplineBillboardEffectProperty::CDlgCSplineBillboardEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCSplineBillboardEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSplineBillboardEffectProperty)
	m_strTexFileName = _T("");
	m_fInterval = 0.0f;
	m_fTraceTime = 0.0f;
	m_fWidth = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_fVelocity = 0.0f;
	//}}AFX_DATA_INIT
}

void CDlgCSplineBillboardEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSplineBillboardEffectProperty)
	DDX_Control(pDX, IDC_BLEND_TYPE, m_cbBlendType);
	DDX_Control(pDX, IDC_BTN_COLOR_END, m_btnColorEnd);
	DDX_Control(pDX, IDC_BTN_COLOR_BEGIN, m_btnColorBegin);
	DDX_Text(pDX, IDC_FILENAME, m_strTexFileName);
	DDX_Text(pDX, IDC_INTERVAL, m_fInterval);
	DDV_MinMaxFloat(pDX, m_fInterval, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TRACE_TIME, m_fTraceTime);
	DDV_MinMaxFloat(pDX, m_fTraceTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_WIDTH, m_fWidth);
	DDV_MinMaxFloat(pDX, m_fWidth, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TWINKLE_PERIOD, m_fTwinklePeriod);
	DDV_MinMaxFloat(pDX, m_fTwinklePeriod, 0.f, 99999.f);
	DDX_Text(pDX, IDC_VELOCITY, m_fVelocity);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCSplineBillboardEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCSplineBillboardEffectProperty)
	ON_BN_CLICKED(IDC_BTN_FILEFIND, OnBtnFilefind)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSplineBillboardEffectProperty message handlers

void CDlgCSplineBillboardEffectProperty::OnBtnFilefind() 
{
	// TODO: Add your control notification handler code here
	TEXSELECTDLG(m_strTexFileName);
}

BOOL CDlgCSplineBillboardEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnColorBegin.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColorEnd.SetColor(C_WHITE|CT_OPAQUE);
	
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

void CDlgCSplineBillboardEffectProperty::OnClearForm()
{
	m_strTexFileName = _T("");
	m_fInterval = 0.0f;
	m_fTraceTime = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_fWidth = 0.0f;
	m_btnColorBegin.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColorEnd.SetColor(C_WHITE|CT_OPAQUE);
	m_fVelocity = 0.0f;

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCSplineBillboardEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	CSplineBillboardEffect sbe;
	sbe.SetWidth(m_fWidth);
	sbe.SetTraceTime(m_fTraceTime);
	sbe.SetTimeInterval(m_fInterval);
	sbe.SetColor(m_btnColorBegin.GetColor(), m_btnColorEnd.GetColor());
	sbe.SetTwinklePeriod(m_fTwinklePeriod);
	sbe.SetVelocity(m_fVelocity);
	if(m_cbBlendType.GetCurSel() == CB_ERR) return 0;
	sbe.SetBlendType( PredefinedBlendType(201 + m_cbBlendType.GetCurSel()) );

	if(m_strTexFileName != "")
	{
		CTFileName filename = m_strTexFileName;
		sbe.SetTexture(filename);
	}

	ep.RegisterEffect(sbe);
	return 0;
}

LRESULT CDlgCSplineBillboardEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CSplineBillboardEffect *pSBE = (CSplineBillboardEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pSBE == NULL) return 0;

		this->m_fWidth = pSBE->GetWidth();
		this->m_btnColorBegin.SetColor(pSBE->GetColorBegin());
		this->m_btnColorEnd.SetColor(pSBE->GetColorEnd());
		this->m_fTraceTime = pSBE->GetTraceTime();
		this->m_fInterval = pSBE->GetTimeInterval();
		this->m_strTexFileName = pSBE->GetTexFileName().str_String;
		this->m_fTwinklePeriod = pSBE->GetTwinklePeriod();
		this->m_fVelocity = pSBE->GetVelocity();
		this->UpdateData(SAVETOFORM);
		this->m_cbBlendType.SetCurSel( int(pSBE->GetBlendType()) - 201 );

		ep.SetEffectInfo(*pSBE);
		CEffectManager::Instance().Destroy((CEffect *&)pSBE);
	}
	return 0;
}

