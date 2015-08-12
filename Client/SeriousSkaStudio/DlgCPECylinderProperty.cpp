// DlgCPECylinderProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPECylinderProperty.h"
#include "DlgCCylinder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPECylinderProperty dialog


CDlgCPECylinderProperty::CDlgCPECylinderProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPECylinderProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPECylinderProperty)
	m_bUsePos = FALSE;
	m_bUseSpeed = FALSE;
	m_fSpeedLower = 0.0f;
	m_fSpeedUpper = 0.0f;
	m_bEmitAllDir = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCPECylinderProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPECylinderProperty)
	DDX_Check(pDX, IDC_PECYL_CHK_POS, m_bUsePos);
	DDX_Check(pDX, IDC_PECYL_CHK_SPEED, m_bUseSpeed);
	DDX_Text(pDX, IDC_PECYL_SPEED_LOWER, m_fSpeedLower);
	DDX_Text(pDX, IDC_PECYL_SPEED_UPPER, m_fSpeedUpper);
	DDX_Check(pDX, IDC_PECYL_CHK_EMITALLDIR, m_bEmitAllDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPECylinderProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPECylinderProperty)
	ON_BN_CLICKED(IDC_PECYL_BTN_CYLINDER, OnPecylBtnCylinder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPECylinderProperty message handlers

void CDlgCPECylinderProperty::OnPecylBtnCylinder() 
{
	// TODO: Add your control notification handler code here
	DLGCYLINDER(m_cylinder);
}

void CDlgCPECylinderProperty::Clear()
{
	m_bEmitAllDir = FALSE;
	m_bUsePos = FALSE;
	m_bUseSpeed = TRUE;
	m_fSpeedLower = 0;
	m_fSpeedUpper = 0;
	m_cylinder = CCylinderDoubleSpace();
	m_cylinder.SetCenter(FLOAT3D(0,0,0));
	m_cylinder.SetHeight(0);
	m_cylinder.SetRadius(0,0);

	UpdateData(SAVETOFORM);
}

