// DlgCPASphereProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPASphereProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPASphereProperty dialog
#include "DlgCSphere.h"

CDlgCPASphereProperty::CDlgCPASphereProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPASphereProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPASphereProperty)
	//}}AFX_DATA_INIT
}


void CDlgCPASphereProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPASphereProperty)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPASphereProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPASphereProperty)
	ON_BN_CLICKED(IDC_BTN_SPHERE, OnBtnSphere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPASphereProperty message handlers
void CDlgCPASphereProperty::Clear()
{
	m_sphere.SetRadius(FLOAT3D(0,0,0),0,0);
	UpdateData(SAVETOFORM);
}

void CDlgCPASphereProperty::OnBtnSphere() 
{
	// TODO: Add your control notification handler code here
	DLGSPHERE(m_sphere);
}
