// DlgCLightEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCLightEffectProperty.h"

#include <Engine/Effect/CLightEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCLightEffectProperty dialog


CDlgCLightEffectProperty::CDlgCLightEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCLightEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCLightEffectProperty)
	m_bAmbientMode = FALSE;
	m_fFallOff = 0.0f;
	//}}AFX_DATA_INIT
}

void CDlgCLightEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCLightEffectProperty)
	DDX_Control(pDX, IDC_BTN_LIGHT_COLOR, m_btnColor);
	DDX_Check(pDX, IDC_CHECK_AMBIENT_MODE, m_bAmbientMode);
	DDX_Text(pDX, IDC_FALL_OFF, m_fFallOff);
	DDV_MinMaxFloat(pDX, m_fFallOff, 0.f, 100.f);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCLightEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCLightEffectProperty)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCLightEffectProperty message handlers

BOOL CDlgCLightEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCLightEffectProperty::OnClearForm()
{
	m_bAmbientMode = FALSE;
	m_fFallOff = 0.0f;
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCLightEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	CLightEffect le;
	le.SetLightColor( this->m_btnColor.GetColor() );
	le.SetAmbientMode( this->m_bAmbientMode );
	le.SetMaxFallOff( this->m_fFallOff );

	ep.RegisterEffect(le);
	return 0;
}

LRESULT CDlgCLightEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CLightEffect *pLE = (CLightEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pLE == NULL) return 0;

		this->m_btnColor.SetColor( pLE->GetLightColor() );
		this->m_bAmbientMode = pLE->GetAmbientMode();
		this->m_fFallOff = pLE->GetMaxFallOff();
		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pLE);
		CEffectManager::Instance().Destroy((CEffect *&)pLE);
	}
	return 0;
}
