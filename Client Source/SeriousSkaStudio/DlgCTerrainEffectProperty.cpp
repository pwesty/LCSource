// DlgCTerrainEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCTerrainEffectProperty.h"
#include <Engine/Effect/CTerrainEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCTerrainEffectProperty dialog


BOOL CDlgCTerrainEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);
	
	m_cbBlendType.AddString("Opaque");		//201
	m_cbBlendType.AddString("Transparent");
	m_cbBlendType.AddString("Blend");
	m_cbBlendType.AddString("Shade");
	m_cbBlendType.AddString("Add");
	m_cbBlendType.AddString("AddAlpha");
	m_cbBlendType.AddString("Multiply");
	m_cbBlendType.AddString("InvMultiply");	//208
	
	m_comboPositionType.AddString("None");
	m_comboPositionType.AddString("Start Once");
	m_comboPositionType.AddString("Always");

	m_comboRotationType.AddString("None");
	m_comboRotationType.AddString("Start Once");
	m_comboRotationType.AddString("Always");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CDlgCTerrainEffectProperty::CDlgCTerrainEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCTerrainEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCTerrainEffectProperty)
	m_fTRAreaBottom = 0.0f;
	m_fTRAreaLeft = 0.0f;
	m_fTRAreaRight = 0.0f;
	m_fTRAreaTop = 0.0f;
	m_strTerrainTextureFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCTerrainEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCTerrainEffectProperty)
	DDX_Control(pDX, IDC_COMBO_ROTATION_TYPE, m_comboRotationType);
	DDX_Control(pDX, IDC_COMBO_POSITION_TYPE, m_comboPositionType);
	DDX_Control(pDX, IDC_TE_BLEND_TYPE, m_cbBlendType);
	DDX_Control(pDX, IDC_BTN_TRCOLOR, m_btnColor);
	DDX_Text(pDX, IDC_TRAREA_BOTTOM, m_fTRAreaBottom);
	DDX_Text(pDX, IDC_TRAREA_LEFT, m_fTRAreaLeft);
	DDX_Text(pDX, IDC_TRAREA_RIGHT, m_fTRAreaRight);
	DDX_Text(pDX, IDC_TRAREA_TOP, m_fTRAreaTop);
	DDX_Text(pDX, IDC_TERRAIN_FILENAME, m_strTerrainTextureFileName);
	DDV_MaxChars(pDX, m_strTerrainTextureFileName, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCTerrainEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCTerrainEffectProperty)
	ON_BN_CLICKED(IDC_BTN_TERRAIN_FILEFIND, OnBtnTerrainFilefind)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCTerrainEffectProperty message handlers

void CDlgCTerrainEffectProperty::OnClearForm()
{
	m_fTRAreaBottom = 0.0f;
	m_fTRAreaLeft = 0.0f;
	m_fTRAreaRight = 0.0f;
	m_fTRAreaTop = 0.0f;
	m_strTerrainTextureFileName = _T("");
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);
	m_comboPositionType.SetCurSel(1);
	m_comboRotationType.SetCurSel(0);

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCTerrainEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_strTerrainTextureFileName == "") return 0;
	CTerrainEffect te;
	te.SetArea( CRectFlt(m_fTRAreaLeft, m_fTRAreaTop, m_fTRAreaRight, m_fTRAreaBottom) );
	te.SetColor(m_btnColor.GetColor());
	CTFileName filename = m_strTerrainTextureFileName;
	te.SetTexture(filename);
	if(m_cbBlendType.GetCurSel() == CB_ERR) return 0;
	te.SetBlendType( PredefinedBlendType(201 + m_cbBlendType.GetCurSel()) );
	te.SetPositionType((EFFECT_OF_TAG_TYPE)m_comboPositionType.GetCurSel());
	te.SetRotationType((EFFECT_OF_TAG_TYPE)m_comboRotationType.GetCurSel());

	ep.RegisterEffect(te);
	return 0;
}

LRESULT CDlgCTerrainEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CTerrainEffect *pTE = (CTerrainEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pTE == NULL) return 0;

		CRectFlt area = pTE->GetArea();
		this->m_fTRAreaTop = area.m_fTop;
		this->m_fTRAreaBottom = area.m_fBottom;
		this->m_fTRAreaLeft = area.m_fLeft;
		this->m_fTRAreaRight = area.m_fRight;
		this->m_btnColor.SetColor(pTE->GetColor());
		this->m_strTerrainTextureFileName = pTE->GetTexFileName().str_String;
		this->UpdateData(SAVETOFORM);
		this->m_cbBlendType.SetCurSel( int(pTE->GetBlendType()) - 201 );
		this->m_comboPositionType.SetCurSel( int(pTE->GetPositionType()) );
		this->m_comboRotationType.SetCurSel( int(pTE->GetRotationType()) );

		ep.SetEffectInfo(*pTE);
		CEffectManager::Instance().Destroy((CEffect *&)pTE);
	}
	return 0;
}

void CDlgCTerrainEffectProperty::OnBtnTerrainFilefind() 
{
	// TODO: Add your control notification handler code here
	TEXSELECTDLG(m_strTerrainTextureFileName);
}
