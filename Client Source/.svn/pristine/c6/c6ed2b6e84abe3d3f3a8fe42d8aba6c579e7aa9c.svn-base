// DlgCSkaEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCSkaEffectProperty.h"
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCSkaEffectProperty dialog


CDlgCSkaEffectProperty::CDlgCSkaEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCSkaEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSkaEffectProperty)
	m_strSkaFileName = _T("");
	m_fX = 1.0f;
	m_fY = 1.0f;
	m_fZ = 1.0f;
	m_bEnableEffectControl = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCSkaEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSkaEffectProperty)
	DDX_Control(pDX, IDC_COMBO_POSITION_TYPE, m_cbPositionType);
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_cbRotationType);
	DDX_Control(pDX, IDC_BTN_SKA_COLOR, m_btnColor);
	DDX_Text(pDX, IDC_SKA_FILENAME, m_strSkaFileName);
	DDV_MaxChars(pDX, m_strSkaFileName, 256);
	DDX_Text(pDX, IDC_SKA_STRETCH_X, m_fX);
	DDX_Text(pDX, IDC_SKA_STRETCH_Y, m_fY);
	DDX_Text(pDX, IDC_SKA_STRETCH_Z, m_fZ);
	DDX_Check(pDX, IDC_CHK_EFFECT_CONTROL, m_bEnableEffectControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSkaEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCSkaEffectProperty)
	ON_BN_CLICKED(IDC_BTN_SKA_FILEFIND, OnBtnSkaFilefind)
	ON_BN_CLICKED(IDC_CHK_EFFECT_CONTROL, OnChkEffectControl)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSkaEffectProperty message handlers

void CDlgCSkaEffectProperty::OnBtnSkaFilefind() 
{
	// TODO: Add your control notification handler code here
	FILESELECTDLG(m_strSkaFileName, "smc");
}

void CDlgCSkaEffectProperty::OnClearForm()
{
	m_strSkaFileName = "";
	m_bEnableEffectControl = FALSE;

	this->UpdateData(SAVETOFORM);
	OnChkEffectControl();
}

LRESULT CDlgCSkaEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_strSkaFileName == "") return 0;
	CSkaEffect se;
	CTFileName fnmSka = m_strSkaFileName;
	se.SetModelInstance(fnmSka);
	se.SetColor(m_btnColor.GetColor());
	se.SetStretch(FLOAT3D(m_fX, m_fY, m_fZ));
	se.SetRotationType(EFFECT_OF_TAG_TYPE(m_cbRotationType.GetCurSel()));
	se.SetPositionType(EFFECT_OF_TAG_TYPE(m_cbPositionType.GetCurSel()));
	if(m_bEnableEffectControl)
	{
		se.SetEffectControl( m_dlgEffectControl.GetEffectControl() );
	}

	ep.RegisterEffect(se);

	return 0;
}

LRESULT CDlgCSkaEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CSkaEffect *pSE = (CSkaEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pSE == NULL) return 0;

		this->m_strSkaFileName = pSE->GetSkaFileName().str_String;
		this->m_btnColor.SetColor(pSE->GetColor());
		this->m_fX = pSE->GetStretch()(1);
		this->m_fY = pSE->GetStretch()(2);
		this->m_fZ = pSE->GetStretch()(3);
		this->m_bEnableEffectControl = (pSE->GetEffectControl() != NULL);
		this->m_cbRotationType.SetCurSel((SINT)pSE->GetRotationType());
		this->m_cbPositionType.SetCurSel((SINT)pSE->GetPositionType());
		if(m_bEnableEffectControl)
		{
			m_dlgEffectControl.ShowWindow(TRUE);
			m_dlgEffectControl.SetEffectControl( *(pSE->GetEffectControl()) );
		}
		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pSE);
		CEffectManager::Instance().Destroy((CEffect *&)pSE);
	}
	return 0;
}

void CDlgCSkaEffectProperty::OnChkEffectControl() 
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

BOOL CDlgCSkaEffectProperty::OnInitDialog() 
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

