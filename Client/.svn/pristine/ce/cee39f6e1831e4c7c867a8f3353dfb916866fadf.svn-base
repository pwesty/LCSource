// DlgCPPForceProcessProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPForceProcessProperty.h"

#include <Engine/Effect/CForce.h>
#include "DlgCForce.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPForceProcessProperty dialog


CDlgCPPForceProcessProperty::CDlgCPPForceProcessProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPForceProcessProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPForceProcessProperty)
	//}}AFX_DATA_INIT
}


void CDlgCPPForceProcessProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPForceProcessProperty)
	DDX_Control(pDX, IDC_LIST_FORCE, m_lbAddedForce);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPForceProcessProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPForceProcessProperty)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPForceProcessProperty message handlers

void CDlgCPPForceProcessProperty::Clear()
{
	m_lbAddedForce.ResetContent();
	m_vectorForce.clear();

	UpdateData(SAVETOFORM);
}

void CDlgCPPForceProcessProperty::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgCForce dlgForce(this);
	CForce force;
	if(dlgForce.DoModal() == IDOK)
	{
		dlgForce.SetPropertyTo(force);
		m_vectorForce.push_back(force);
		int index = -1;
		index = m_lbAddedForce.AddString(GetForceName(force));
		m_lbAddedForce.SetCurSel(index);
	}
}

void CDlgCPPForceProcessProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAddedForce.GetCurSel() == -1) return;

	m_vectorForce.erase( m_vectorForce.begin() + m_lbAddedForce.GetCurSel() );
	m_lbAddedForce.DeleteString( m_lbAddedForce.GetCurSel() );
	m_lbAddedForce.SetCurSel(-1);
}

void CDlgCPPForceProcessProperty::OnBtnModify() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAddedForce.GetCurSel() == -1) return;

	int index = m_lbAddedForce.GetCurSel();
	CDlgCForce dlgForce(this);
	dlgForce.SetPropertyFrom(m_vectorForce[index]);

	if(dlgForce.DoModal() == IDOK)
	{
		dlgForce.SetPropertyTo(m_vectorForce[index]);
	}
	m_lbAddedForce.InsertString(index, GetForceName(m_vectorForce[index]));
	m_lbAddedForce.DeleteString(index+1);
	m_lbAddedForce.SetCurSel(index);
}

void CDlgCPPForceProcessProperty::Refresh()
{
	m_lbAddedForce.ResetContent();

	for(int i=0; i<m_vectorForce.size(); ++i)
	{
		m_lbAddedForce.AddString(GetForceName(m_vectorForce[i]));
	}
	m_lbAddedForce.SetCurSel(-1);
}
