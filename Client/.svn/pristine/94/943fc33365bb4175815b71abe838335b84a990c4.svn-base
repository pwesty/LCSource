// DlgCPPProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPProperty.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPProperty dialog


CDlgCPPProperty::CDlgCPPProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCPPProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPProperty)
	DDX_Control(pDX, IDC_PP_ADDED, m_lbAdded);
	DDX_Control(pDX, IDC_PP_TYPE, m_cbType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPProperty)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_CBN_SELENDOK(IDC_PP_TYPE, OnSelEndOkPpType)
	ON_LBN_SELCHANGE(IDC_PP_ADDED, OnSelChangePpAdded)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPProperty message handlers

void CDlgCPPProperty::Clear()
{
	m_lbAdded.ResetContent();
	m_cbType.SetCurSel(-1);

	for(int i=0; i<m_vectorPProcess.size(); ++i) delete m_vectorPProcess[i];
	m_vectorPProcess.clear();

	m_dlgCPPDynamicStateProperty.Clear();
	m_dlgCPPDynamicStateProperty.ShowWindow(FALSE);
	m_dlgCPPForceProperty.Clear();
	m_dlgCPPForceProperty.ShowWindow(FALSE);
	m_dlgCPPPointGoalProperty.Clear();
	m_dlgCPPPointGoalProperty.ShowWindow(FALSE);
	m_dlgCPPControlProperty.Clear();
	m_dlgCPPControlProperty.ShowWindow(FALSE);
	m_dlgCPPVelocityProperty.Clear();
	m_dlgCPPVelocityProperty.ShowWindow(FALSE);

	UpdateData(SAVETOFORM);
}

void CDlgCPPProperty::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAdded.GetCurSel() == -1) return;
	if(m_lbAdded.GetCurSel() == 0) return;
	
	const int idxSelected = m_lbAdded.GetCurSel()-1;
	CString strSelected;
	m_lbAdded.GetText(idxSelected+1, strSelected);
	m_lbAdded.InsertString(idxSelected, strSelected);
	m_lbAdded.DeleteString(idxSelected+2);
	
	CParticlesCommonProcess *pPP1 = m_vectorPProcess[idxSelected];
	CParticlesCommonProcess *pPP2 = m_vectorPProcess[idxSelected+1];
	m_vectorPProcess[idxSelected] = pPP2;
	m_vectorPProcess[idxSelected+1] = pPP1;

	m_lbAdded.SetCurSel(idxSelected);
}

void CDlgCPPProperty::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAdded.GetCurSel() == -1) return;
	if(m_lbAdded.GetCurSel() == m_lbAdded.GetCount()-1) return;
	
	const int idxSelected = m_lbAdded.GetCurSel();
	CString strSelected;
	m_lbAdded.GetText(idxSelected+1, strSelected);
	m_lbAdded.InsertString(idxSelected, strSelected);
	m_lbAdded.DeleteString(idxSelected+2);
	
	CParticlesCommonProcess *pPP1 = m_vectorPProcess[idxSelected];
	CParticlesCommonProcess *pPP2 = m_vectorPProcess[idxSelected+1];
	m_vectorPProcess[idxSelected] = pPP2;
	m_vectorPProcess[idxSelected+1] = pPP1;

	m_lbAdded.SetCurSel(idxSelected+1);
}

void CDlgCPPProperty::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	if(m_cbType.GetCurSel() == -1) return;
	switch(m_cbType.GetCurSel())
	{
	case 0:
		{
			CParticlesProcessDynamicState *pPPDS = new CParticlesProcessDynamicState;
			(*pPPDS) = m_dlgCPPDynamicStateProperty.GetParticlesProcess();
			m_vectorPProcess.push_back(pPPDS);
			m_lbAdded.AddString("DynamicState");
		}break;
	case 1:
		{
			CParticlesProcessForce *pPPF = new CParticlesProcessForce;
			(*pPPF) = m_dlgCPPForceProperty.GetParticlesProcess();
			m_vectorPProcess.push_back(pPPF);
			m_lbAdded.AddString("Force");
		}break;
	case 2:
		{
			CParticlesProcessPointGoal *pPPPG = new CParticlesProcessPointGoal;
			(*pPPPG) = m_dlgCPPPointGoalProperty.GetParticlesProcess();
			m_vectorPProcess.push_back(pPPPG);
			m_lbAdded.AddString("PointGoal");
		}break;
	case 3:
		{
			CParticlesProcessControl *pPPC = new CParticlesProcessControl;
			(*pPPC) = m_dlgCPPControlProperty.GetParticlesProcess();
			m_vectorPProcess.push_back(pPPC);
			m_lbAdded.AddString("Control");
		}break;
	case 4:
		{
			CParticlesProcessVelocity *pPPV = new CParticlesProcessVelocity;
			(*pPPV) = m_dlgCPPVelocityProperty.GetParticlesProcess();
			m_vectorPProcess.push_back(pPPV);
			m_lbAdded.AddString("Velocity");
		}break;
	}
	m_lbAdded.SetCurSel(m_lbAdded.GetCount()-1);
}

void CDlgCPPProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAdded.GetCurSel() == -1) return;

	int index = m_lbAdded.GetCurSel();
	delete m_vectorPProcess[ index ];
	m_vectorPProcess.erase(m_vectorPProcess.begin() + index);
	m_lbAdded.DeleteString(index);
	if(index >= m_lbAdded.GetCount()) index = m_lbAdded.GetCount() - 1;
	m_lbAdded.SetCurSel(index);

	OnSelChangePpAdded();
}

void CDlgCPPProperty::OnSelEndOkPpType() 
{
	// TODO: Add your control notification handler code here
	m_dlgCPPForceProperty.ShowWindow(FALSE);
	m_dlgCPPDynamicStateProperty.ShowWindow(FALSE);
	m_dlgCPPPointGoalProperty.ShowWindow(FALSE);
	m_dlgCPPControlProperty.ShowWindow(FALSE);
	m_dlgCPPVelocityProperty.ShowWindow(FALSE);
	m_lbAdded.SetCurSel(-1);

	if(m_cbType.GetCurSel() == -1) return;

	switch(m_cbType.GetCurSel())
	{
	case 0:	m_dlgCPPDynamicStateProperty.ShowWindow(TRUE);	break;
	case 1:	m_dlgCPPForceProperty.ShowWindow(TRUE);			break;
	case 2:	m_dlgCPPPointGoalProperty.ShowWindow(TRUE);		break;
	case 3:	m_dlgCPPControlProperty.ShowWindow(TRUE);		break;
	case 4:	m_dlgCPPVelocityProperty.ShowWindow(TRUE);		break;
	}
}

BOOL CDlgCPPProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dlgCPPDynamicStateProperty.Create(IDD_CPP_DYNAMICSTATE_PROPERTY, this);
	m_dlgCPPForceProperty.Create(IDD_CPP_FORCE_PROCESS_PROPERTY, this);
	m_dlgCPPPointGoalProperty.Create(IDD_CPP_POINTGOAL_PROPERTY, this);
	m_dlgCPPControlProperty.Create(IDD_CPP_CONTROL_PROPERTY, this);
	m_dlgCPPVelocityProperty.Create(IDD_CPP_VELOCITY_PROCESS_PROPERTY, this);

	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	m_dlgCPPDynamicStateProperty.GetClientRect(&rectChild);
	m_dlgCPPDynamicStateProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPDynamicStateProperty.ShowWindow(FALSE);
	m_dlgCPPForceProperty.GetClientRect(&rectChild);
	m_dlgCPPForceProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPForceProperty.ShowWindow(FALSE);
	m_dlgCPPPointGoalProperty.GetClientRect(&rectChild);
	m_dlgCPPPointGoalProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPPointGoalProperty.ShowWindow(FALSE);
	m_dlgCPPControlProperty.GetClientRect(&rectChild);
	m_dlgCPPControlProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPControlProperty.ShowWindow(FALSE);
	m_dlgCPPVelocityProperty.GetClientRect(&rectChild);
	m_dlgCPPVelocityProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPVelocityProperty.ShowWindow(FALSE);

	// TODO: Add extra initialization here
	m_cbType.AddString("Dynamic State");
	m_cbType.AddString("Force");
	m_cbType.AddString("Point Goal");
	m_cbType.AddString("Control");
	m_cbType.AddString("Velocity");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgCPPProperty::SetPropertyTo(CParticleGroup &pg)
{
	for(int i=0; i<m_vectorPProcess.size(); ++i)
	{
		pg.AddCommonProcess(m_vectorPProcess[i]);
	}
	return TRUE;
}

BOOL CDlgCPPProperty::SetPropertyFrom(CParticleGroup &pg)
{
	int i = 0;
	int size = m_vectorPProcess.size();
	int count = pg.GetCommonProcessCount();
	for( i = 0; i < size; ++i) delete m_vectorPProcess[i];
	m_vectorPProcess.clear();

	for( i = 0; i < count; ++i)
	{
		m_vectorPProcess.push_back(pg.GetCommonProcess(i)->Copy());
	}
	Refresh();
	return TRUE;
}


void CDlgCPPProperty::OnSelChangePpAdded() 
{
	// TODO: Add your control notification handler code here
	m_dlgCPPForceProperty.ShowWindow(FALSE);
	m_dlgCPPDynamicStateProperty.ShowWindow(FALSE);
	m_dlgCPPPointGoalProperty.ShowWindow(FALSE);
	m_dlgCPPControlProperty.ShowWindow(FALSE);
	m_dlgCPPVelocityProperty.ShowWindow(FALSE);
	m_cbType.SetCurSel(-1);
	
	if(m_lbAdded.GetCurSel() == -1) return;

	switch( m_vectorPProcess[m_lbAdded.GetCurSel()]->GetType() )
	{
	case PCPT_DYNAMIC_STATE:
		{
			m_cbType.SetCurSel(0);
			m_dlgCPPDynamicStateProperty.ShowWindow(TRUE);
			m_dlgCPPDynamicStateProperty.SetParticlesProcess(*(CParticlesProcessDynamicState*)m_vectorPProcess[m_lbAdded.GetCurSel()]);
		} break;
	case PCPT_FORCE:
		{
			m_cbType.SetCurSel(1);
			m_dlgCPPForceProperty.ShowWindow(TRUE);
			m_dlgCPPForceProperty.SetParticlesProcess(*(CParticlesProcessForce*)m_vectorPProcess[m_lbAdded.GetCurSel()]);
		} break;
	case PCPT_POINT_GOAL:
		{
			m_cbType.SetCurSel(2);
			m_dlgCPPPointGoalProperty.ShowWindow(TRUE);
			m_dlgCPPPointGoalProperty.SetParticlesProcess(*(CParticlesProcessPointGoal*)m_vectorPProcess[m_lbAdded.GetCurSel()]);
		} break;
	case PCPT_CONTROL:
		{
			m_cbType.SetCurSel(3);
			m_dlgCPPControlProperty.ShowWindow(TRUE);
			m_dlgCPPControlProperty.SetParticlesProcess(*(CParticlesProcessControl*)m_vectorPProcess[m_lbAdded.GetCurSel()]);
		} break;
	case PCPT_VELOCITY:
		{
			m_cbType.SetCurSel(4);
			m_dlgCPPVelocityProperty.ShowWindow(TRUE);
			m_dlgCPPVelocityProperty.SetParticlesProcess(*(CParticlesProcessVelocity*)m_vectorPProcess[m_lbAdded.GetCurSel()]);
		} break;
	}
}

void CDlgCPPProperty::Refresh()
{
	m_lbAdded.ResetContent();
	m_cbType.SetCurSel(-1);

	for(int i=0; i<m_vectorPProcess.size(); ++i)
	{
		switch(m_vectorPProcess[i]->GetType())
		{
		case PCPT_DYNAMIC_STATE:
			{
				m_lbAdded.AddString("DynamicState");
			} break;
		case PCPT_FORCE:
			{
				m_lbAdded.AddString("Force");
			} break;
		case PCPT_POINT_GOAL:
			{
				m_lbAdded.AddString("PointGoal");
			} break;
		case PCPT_CONTROL:
			{
				m_lbAdded.AddString("Control");
			} break;
		case PCPT_VELOCITY:
			{
				m_lbAdded.AddString("Velocity");
			} break;
		}
	}
}


void CDlgCPPProperty::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	int curSel = m_lbAdded.GetCurSel();
	OnBtnAdd();
	m_lbAdded.SetCurSel(curSel);
	OnBtnDel();
}
