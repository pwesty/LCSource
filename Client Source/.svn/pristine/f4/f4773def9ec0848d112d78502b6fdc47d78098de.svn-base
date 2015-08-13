// DlgCMdlEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCMdlEffectProperty.h"
#include <Engine/Effect/CMdlEffect.h>
#include <Engine/Effect/CEffectManager.h>
#include <Afxdlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCMdlEffectProperty dialog


CDlgCMdlEffectProperty::CDlgCMdlEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCMdlEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCMdlEffectProperty)
	m_strMdlFileName = _T("");
	m_strMdlTexFileName = _T("");
	m_fX = 1.0f;
	m_fY = 1.0f;
	m_fZ = 1.0f;
	m_bEnableEffectControl = FALSE;
	m_bOverDraw = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCMdlEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCMdlEffectProperty)
	DDX_Control(pDX, IDC_CB_POSITION_TYPE, m_cbPositionType);
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_cbRotationType);
	DDX_Control(pDX, IDC_BTN_MDL_COLOR, m_btnColor);
	DDX_Text(pDX, IDC_MDL_FILENAME, m_strMdlFileName);
	DDV_MaxChars(pDX, m_strMdlFileName, 256);
	DDX_Text(pDX, IDC_MDL_TEX_FILENAME, m_strMdlTexFileName);
	DDV_MaxChars(pDX, m_strMdlTexFileName, 256);
	DDX_Text(pDX, IDC_MDL_STRETCH_X, m_fX);
	DDX_Text(pDX, IDC_MDL_STRETCH_Y, m_fY);
	DDX_Text(pDX, IDC_MDL_STRETCH_Z, m_fZ);
	DDX_Check(pDX, IDC_CHK_EFFECT_CONTROL, m_bEnableEffectControl);
	DDX_Check(pDX, IDC_CHK_OVERDRAW, m_bOverDraw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCMdlEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCMdlEffectProperty)
	ON_BN_CLICKED(IDC_BTN_MDL_FILEFIND, OnBtnMdlFilefind)
	ON_BN_CLICKED(IDC_BTN_MDL_TEX_FILEFIND, OnBtnMdlTexFilefind)
	ON_BN_CLICKED(IDC_CHK_EFFECT_CONTROL, OnChkEffectControl)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCMdlEffectProperty message handlers

void CDlgCMdlEffectProperty::OnBtnMdlFilefind() 
{
	// TODO: Add your control notification handler code here
	FILESELECTDLG(m_strMdlFileName, "mdl");
}

void CDlgCMdlEffectProperty::OnBtnMdlTexFilefind() 
{
	// TODO: Add your control notification handler code here
	TEXSELECTDLG(m_strMdlTexFileName);
}

void CDlgCMdlEffectProperty::OnClearForm()
{
	m_strMdlFileName = "";
	m_strMdlTexFileName = "";
	m_bEnableEffectControl = FALSE;
	m_bOverDraw = FALSE;

	m_dlgEffectControl.Clear();
	m_dlgEffectControl.ShowWindow(FALSE);

	this->UpdateData(SAVETOFORM);
	OnChkEffectControl();
}

LRESULT CDlgCMdlEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_strMdlFileName == "" || m_strMdlTexFileName == "") return 0;
	CMdlEffect me;
	CTFileName fnmMdl = m_strMdlFileName;
	CTFileName fnmMdlTex = m_strMdlTexFileName;
	me.SetModelObject(fnmMdl, fnmMdlTex);
	me.SetColor(m_btnColor.GetColor());
	me.SetStretch(FLOAT3D(m_fX, m_fY, m_fZ));
	me.SetRotationType(EFFECT_OF_TAG_TYPE(m_cbRotationType.GetCurSel()));
	me.SetPositionType(EFFECT_OF_TAG_TYPE(m_cbPositionType.GetCurSel()));
	if(m_bEnableEffectControl)
	{
		me.SetEffectControl( m_dlgEffectControl.GetEffectControl() );
	}
	me.SetOverDraw(m_bOverDraw);

	ep.RegisterEffect(me);

	return 0;
}

LRESULT CDlgCMdlEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CMdlEffect *pME = (CMdlEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pME == NULL) return 0;

		this->m_strMdlFileName = pME->GetMdlFileName().str_String;
		this->m_strMdlTexFileName = pME->GetMdlTexFileName().str_String;
		this->m_btnColor.SetColor(pME->GetColor());
		this->m_fX = pME->GetStretch()(1);
		this->m_fY = pME->GetStretch()(2);
		this->m_fZ = pME->GetStretch()(3);
		this->m_cbRotationType.SetCurSel((SINT)pME->GetRotationType());
		this->m_cbPositionType.SetCurSel((SINT)pME->GetPositionType());
		this->m_bEnableEffectControl = (pME->GetEffectControl() != NULL);
		if(m_bEnableEffectControl)
		{
			m_dlgEffectControl.ShowWindow(TRUE);
			m_dlgEffectControl.SetEffectControl( *(pME->GetEffectControl()) );
		}
		this->m_bOverDraw = pME->GetOverDraw();
		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pME);
		CEffectManager::Instance().Destroy((CEffect *&)pME);
	}
	return 0;
}

void CDlgCMdlEffectProperty::OnChkEffectControl() 
{
	UpdateData(SAVETOOBJECT);

	if(m_bEnableEffectControl)
	{
		m_dlgEffectControl.ShowWindow(TRUE);
	}
	else
	{
		m_dlgEffectControl.ShowWindow(FALSE);
		m_dlgEffectControl.Clear();
	}
}

BOOL CDlgCMdlEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	//light
	m_dlgEffectControl.Create(IDD_CEFFECTCONTROL_PROPERTY, this);
	m_dlgEffectControl.GetClientRect(&rectChild);
	m_dlgEffectControl.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgEffectControl.ShowWindow(FALSE);

	m_cbRotationType.AddString("None");
	m_cbRotationType.AddString("Start Once");
	m_cbRotationType.AddString("Always");
	
	m_cbPositionType.AddString("None");
	m_cbPositionType.AddString("Start Once");
	m_cbPositionType.AddString("Always");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
