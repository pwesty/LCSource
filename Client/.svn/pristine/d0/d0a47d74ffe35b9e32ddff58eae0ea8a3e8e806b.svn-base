// DlgCPPPointGoalProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPPointGoalProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPPointGoalProperty dialog


CDlgCPPPointGoalProperty::CDlgCPPPointGoalProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPPointGoalProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPPointGoalProperty)
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fSpeed = 0.0f;
	m_fLerpSpeed = 0.0f;
	m_fLerpRatio = 0.0f;
	m_strGoalTagName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCPPPointGoalProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPPointGoalProperty)
	DDX_Text(pDX, IDC_POINTGOAL_X, m_fX);
	DDX_Text(pDX, IDC_POINTGOAL_Y, m_fY);
	DDX_Text(pDX, IDC_POINTGOAL_Z, m_fZ);
	DDX_Text(pDX, IDC_POINTGOAL_SPEED, m_fSpeed);
	DDX_Text(pDX, IDC_POINTGOAL_LERPSPEED, m_fLerpSpeed);
	DDV_MinMaxFloat(pDX, m_fLerpSpeed, 0.f, 99999.f);
	DDX_Text(pDX, IDC_POINTGOAL_LERPRATIO, m_fLerpRatio);
	DDV_MinMaxFloat(pDX, m_fLerpRatio, 0.f, 1.f);
	DDX_Text(pDX, IDC_POINTGOAL_GOALTAGNAME, m_strGoalTagName);
	DDV_MaxChars(pDX, m_strGoalTagName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPPointGoalProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPPointGoalProperty)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPPointGoalProperty message handlers

void CDlgCPPPointGoalProperty::Clear()
{
	m_fSpeed = 0.0f;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fLerpSpeed = 0.1f;
	m_fLerpRatio = 0.5f;
	m_strGoalTagName = _T("");

	UpdateData(SAVETOFORM);
}
