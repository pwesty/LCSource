// DlgCPPVelocityProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPVelocityProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPVelocityProperty dialog


CDlgCPPVelocityProperty::CDlgCPPVelocityProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPVelocityProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPVelocityProperty)
	m_fVelocityDirX = 0.0f;
	m_fVelocityDirY = 0.0f;
	m_fVelocityDirZ = 0.0f;
	m_fSpeed = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCPPVelocityProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPVelocityProperty)
	DDX_Text(pDX, IDC_VELOCITY_DIRX, m_fVelocityDirX);
	DDX_Text(pDX, IDC_VELOCITY_DIRZ, m_fVelocityDirZ);
	DDX_Text(pDX, IDC_VELOCITY_SPEED, m_fSpeed);
	DDX_Text(pDX, IDC_VELOCITY_DIRY, m_fVelocityDirY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPVelocityProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPVelocityProperty)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPVelocityProperty message handlers

void CDlgCPPVelocityProperty::Clear()
{
	m_fVelocityDirX = 0;
	m_fVelocityDirY = 0;
	m_fVelocityDirZ = 0;
	m_fSpeed = 0;
	UpdateData(SAVETOFORM);
}

void CDlgCPPVelocityProperty::Refresh()
{
	UpdateData(SAVETOOBJECT);
}

