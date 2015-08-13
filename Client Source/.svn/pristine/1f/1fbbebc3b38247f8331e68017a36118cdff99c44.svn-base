// DlgCEntityEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCEntityEffectProperty.h"
#include <Engine/Effect/CEntityEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCEntityEffectProperty dialog


CDlgCEntityEffectProperty::CDlgCEntityEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCEntityEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCEntityEffectProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCEntityEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCEntityEffectProperty)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCEntityEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCEntityEffectProperty)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCEntityEffectProperty message handlers

BOOL CDlgCEntityEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	m_dlgEffectControl.Create(IDD_CEFFECTCONTROL_PROPERTY, this);
	m_dlgEffectControl.GetClientRect(&rectChild);
	m_dlgEffectControl.SetWindowPos(NULL, 0, 0/*rectParent.bottom - rectChild.bottom*/, 0, 0, SWP_NOSIZE);
	m_dlgEffectControl.ShowWindow(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCEntityEffectProperty::OnClearForm()
{
	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCEntityEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	CEntityEffect ee;
	ee.SetEffectControl( m_dlgEffectControl.GetEffectControl() );

	ep.RegisterEffect(ee);

	return 0;
}

LRESULT CDlgCEntityEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CEntityEffect *pEE = (CEntityEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pEE == NULL) return 0;

		m_dlgEffectControl.ShowWindow(TRUE);
		m_dlgEffectControl.SetEffectControl( *(pEE->GetEffectControl()) );
		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pEE);
		CEffectManager::Instance().Destroy((CEffect *&)pEE);
	}
	return 0;
}
