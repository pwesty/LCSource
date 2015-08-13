// DlgCTraceEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCTraceEffectProperty.h"
#include <Engine/Effect/CTraceEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCTraceEffectProperty dialog


CDlgCTraceEffectProperty::CDlgCTraceEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCTraceEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCTraceEffectProperty)
	m_strTexFileName = _T("");
	m_fTraceInterval = 0.0f;
	m_fTraceTime = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_bCapEnd = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCTraceEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCTraceEffectProperty)
	DDX_Control(pDX, IDC_BLEND_TYPE, m_cbBlendType);
	DDX_Control(pDX, IDC_BTN_TRACE_END_COLOR, m_btnColorEnd);
	DDX_Control(pDX, IDC_BTN_TRACE_BEGIN_COLOR, m_btnColorBegin);
	DDX_Text(pDX, IDC_TRACE_FILENAME, m_strTexFileName);
	DDX_Text(pDX, IDC_TRACE_INTERVAL, m_fTraceInterval);
	DDV_MinMaxFloat(pDX, m_fTraceInterval, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TRACE_TIME, m_fTraceTime);
	DDV_MinMaxFloat(pDX, m_fTraceTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TRACE_TWINKLE_PERIOD, m_fTwinklePeriod);
	DDV_MinMaxFloat(pDX, m_fTwinklePeriod, 0.f, 99999.f);
	DDX_Check(pDX, IDC_CHK_CAPEND, m_bCapEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCTraceEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCTraceEffectProperty)
	ON_BN_CLICKED(IDC_BTN_TRACE_FILEFIND, OnBtnTraceFilefind)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCTraceEffectProperty message handlers

void CDlgCTraceEffectProperty::OnBtnTraceFilefind() 
{
	// TODO: Add your control notification handler code here
	TEXSELECTDLG(m_strTexFileName);
}

BOOL CDlgCTraceEffectProperty::OnInitDialog() 
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

void CDlgCTraceEffectProperty::OnClearForm()
{
	m_strTexFileName = _T("");
	m_fTraceInterval = 0.0f;
	m_fTraceTime = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_btnColorBegin.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColorEnd.SetColor(C_WHITE|CT_OPAQUE);
	m_bCapEnd = FALSE;

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCTraceEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	CTraceEffect tce;
	tce.SetTraceTime(m_fTraceTime);
	tce.SetTimeInterval(m_fTraceInterval);
	tce.SetColor(m_btnColorBegin.GetColor(), m_btnColorEnd.GetColor());
	tce.SetTwinklePeriod(m_fTwinklePeriod);
	tce.SetCapEnd(m_bCapEnd);
	if(m_cbBlendType.GetCurSel() == CB_ERR) return 0;
	tce.SetBlendType( PredefinedBlendType(201 + m_cbBlendType.GetCurSel()) );

	if(m_strTexFileName != "")
	{
		CTFileName filename = m_strTexFileName;
		tce.SetTexture(filename);
	}

	ep.RegisterEffect(tce);
	return 0;
}

LRESULT CDlgCTraceEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CTraceEffect *pTCE = (CTraceEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pTCE == NULL) return 0;

		this->m_btnColorBegin.SetColor(pTCE->GetColorBegin());
		this->m_btnColorEnd.SetColor(pTCE->GetColorEnd());
		this->m_fTraceTime = pTCE->GetTraceTime();
		this->m_fTraceInterval = pTCE->GetTimeInterval();
		this->m_strTexFileName = pTCE->GetTexFileName().str_String;
		this->m_fTwinklePeriod = pTCE->GetTwinklePeriod();
		this->m_bCapEnd = pTCE->GetCapEnd();
		this->UpdateData(SAVETOFORM);
		this->m_cbBlendType.SetCurSel( int(pTCE->GetBlendType()) - 201 );

		ep.SetEffectInfo(*pTCE);
		CEffectManager::Instance().Destroy((CEffect *&)pTCE);
	}
	return 0;
}

