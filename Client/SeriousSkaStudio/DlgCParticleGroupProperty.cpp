// DlgCParticleGroupProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCParticleGroupProperty.h"
#include <Engine/Effect/CParticleGroup.h>
#include <Engine/Effect/CParticleGroupManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleGroupProperty dialog


CDlgCParticleGroupProperty::CDlgCParticleGroupProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCParticleGroupProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCParticleGroupProperty)
	m_strTexFileName = _T("");
	m_strName = _T("");
	m_uiMexWidth = 512;
	m_uiMexHeight = 512;
	m_uiColumn = 0;
	m_uiRow = 0;
	//}}AFX_DATA_INIT
}


void CDlgCParticleGroupProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCParticleGroupProperty)
	DDX_Control(pDX, IDC_TAB_PG_COMPONENT, m_tcComponent);
	DDX_Control(pDX, IDC_PG_REGISTERED_NAME, m_cbRegisteredName);
	DDX_Control(pDX, IDC_PG_RENDER_TYPE, m_cbRenderType);
	DDX_Control(pDX, IDC_PG_BLEND_TYPE, m_cbBlendType);
	DDX_Text(pDX, IDC_PG_TEX_FILENAME, m_strTexFileName);
	DDV_MaxChars(pDX, m_strTexFileName, 256);
	DDX_Text(pDX, IDC_PG_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	DDX_Text(pDX, IDC_PG_MEXWIDTH, m_uiMexWidth);
	DDV_MinMaxUInt(pDX, m_uiMexWidth, 0, 99999);
	DDX_Text(pDX, IDC_PG_MEXHEIGHT, m_uiMexHeight);
	DDV_MinMaxUInt(pDX, m_uiMexHeight, 0, 99999);
	DDX_Text(pDX, IDC_PG_COLUMN, m_uiColumn);
	DDV_MinMaxUInt(pDX, m_uiColumn, 0, 256);
	DDX_Text(pDX, IDC_PG_ROW, m_uiRow);
	DDV_MinMaxUInt(pDX, m_uiRow, 0, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCParticleGroupProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCParticleGroupProperty)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PG_COMPONENT, OnSelChangeTabPgComponent)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_PG_TEX_FILEFIND, OnBtnPgTexFilefind)
	ON_CBN_SELENDOK(IDC_PG_REGISTERED_NAME, OnSelEndOkPgRegisteredName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCParticleGroupProperty message handlers

void CDlgCParticleGroupProperty::Clear()
{
	m_cbRegisteredName.ResetContent();
	m_cbRegisteredName.SetCurSel(-1);
	m_cbRenderType.SetCurSel(-1);
	m_cbBlendType.SetCurSel(-1);
	m_strTexFileName = "";
	m_strName = "";
	m_uiMexWidth = 512;
	m_uiMexHeight = 512;
	m_uiColumn = 0;
	m_uiRow = 0;

	m_dlgCPEProperty.Clear();
	m_dlgCPAProperty.Clear();
	m_dlgCPPProperty.Clear();

	UpdateData(SAVETOFORM);
}

void CDlgCParticleGroupProperty::Refresh()
{
	Clear();

	CParticleGroupManager::my_map::iterator iterBegin = CParticleGroupManager::Instance().GetRegisteredMap().begin();
	CParticleGroupManager::my_map::iterator iterEnd = CParticleGroupManager::Instance().GetRegisteredMap().end();
	CParticleGroupManager::my_map::iterator iter;
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		m_cbRegisteredName.AddString( (*iter).second->GetName().c_str() );
	}
}

BOOL CDlgCParticleGroupProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	RECT rectParent, rectChild;
	GetClientRect(&rectParent);
	//emitter
	m_dlgCPEProperty.Create(IDD_CPARTICLEEMITTER_PROPERTY, this);
	m_dlgCPEProperty.GetClientRect(&rectChild);
	m_dlgCPEProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPEProperty.ShowWindow(TRUE);
	
	//absorption
	m_dlgCPAProperty.Create(IDD_CPARTICLEABSORPTION_PROPERTY, this);
	m_dlgCPAProperty.GetClientRect(&rectChild);
	m_dlgCPAProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPAProperty.ShowWindow(FALSE);
	
	//process
	m_dlgCPPProperty.Create(IDD_CPARTICLEPROCESS_PROPERTY, this);
	m_dlgCPPProperty.GetClientRect(&rectChild);
	m_dlgCPPProperty.SetWindowPos(NULL, 0, rectParent.bottom - rectChild.bottom, 0, 0, SWP_NOSIZE);
	m_dlgCPPProperty.ShowWindow(FALSE);
	
	// TODO: Add extra initialization here
	m_tcComponent.InsertItem(0, "Emitter");
	m_tcComponent.InsertItem(1, "Absor...");
	m_tcComponent.InsertItem(2, "Process");

	m_cbRenderType.AddString("Quad");	//0
	m_cbRenderType.AddString("Line");	//1
	m_cbRenderType.AddString("Trail");	//2
	m_cbRenderType.AddString("TrailVelocity");	//3

	m_cbBlendType.AddString("Opaque");		//201
	m_cbBlendType.AddString("Transparent");
	m_cbBlendType.AddString("Blend");
	m_cbBlendType.AddString("Shade");
	m_cbBlendType.AddString("Add");
	m_cbBlendType.AddString("AddAlpha");
	m_cbBlendType.AddString("Multiply");
	m_cbBlendType.AddString("InvMultiply");	//208
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCParticleGroupProperty::OnSelChangeTabPgComponent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_dlgCPEProperty.ShowWindow(FALSE);
	m_dlgCPAProperty.ShowWindow(FALSE);
	m_dlgCPPProperty.ShowWindow(FALSE);
	
	int index = m_tcComponent.GetCurSel();
	switch(index)
	{
	case 0:	m_dlgCPEProperty.ShowWindow(TRUE);	break;
	case 1:	m_dlgCPAProperty.ShowWindow(TRUE);	break;
	case 2:	m_dlgCPPProperty.ShowWindow(TRUE);	break;
	}
	*pResult = 0;
}

void CDlgCParticleGroupProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(IDNO == MessageBox("정말 지우시겠습니까?", "주의", MB_YESNO)) return;
	if(m_cbRegisteredName.GetCurSel() == -1) return;
	CString strPGName;
	m_cbRegisteredName.GetLBText(m_cbRegisteredName.GetCurSel(), strPGName);
	CParticleGroupManager::Instance().Unregister(strPGName.GetBuffer(0));
	m_cbRegisteredName.DeleteString(m_cbRegisteredName.GetCurSel());

	this->Refresh();
	CPrintF("Particle Group delete : %s\n", strPGName);
}

void CDlgCParticleGroupProperty::OnSelEndOkPgRegisteredName() 
{
	// TODO: Add your control notification handler code here
	if(m_cbRegisteredName.GetCurSel() == -1) return;

	CString strPG;
	m_cbRegisteredName.GetLBText( m_cbRegisteredName.GetCurSel(), strPG );
	CParticleGroup &pg = *CParticleGroupManager::Instance().GetRegisteredMap()[std::string(strPG.GetBuffer(0))];
	//기본 particle group 정보
	m_strName = pg.GetName().c_str();
	m_cbBlendType.SetCurSel( int(pg.GetBlendType())-201 );
	m_cbRenderType.SetCurSel( int(pg.GetRenderType()) );
	m_strTexFileName = pg.GetTexFileName().str_String;
	m_uiMexWidth = pg.GetTexInfoMexWidth();
	m_uiMexHeight = pg.GetTexInfoMexHeight();
	m_uiColumn = pg.GetTexInfoColumn();
	m_uiRow = pg.GetTexInfoRow();
	m_strParticleGroupNameOld = m_strName;
	UpdateData(SAVETOFORM);
	//particle emitter 정보
	m_dlgCPEProperty.SetPropertyFrom(pg);

	//particle absorption 정보
	m_dlgCPAProperty.SetPropertyFrom(pg);

	//particle process 정보
	m_dlgCPPProperty.SetPropertyFrom(pg);

	theApp.OnExecuteEffect( TRUE );
}

void CDlgCParticleGroupProperty::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(SAVETOOBJECT);
	if(m_cbBlendType.GetCurSel() == -1) return;
	if(m_cbRenderType.GetCurSel() == -1) return;
	if(m_strTexFileName == "") return;

	if(m_strParticleGroupNameOld != m_strName && m_strParticleGroupNameOld != ""
		&& m_cbRegisteredName.FindStringExact(-1, m_strName) != CB_ERR)
	{
		if(IDNO == MessageBox("같은 이름을 가진 파티클그룹이 있습니다. 덮어 쓰시겠습니까?", "주의", MB_YESNO))
		{
			UpdateData(SAVETOFORM);
			return;
		}
	}
	m_strParticleGroupNameOld = m_strName;
	CParticleGroup pg;
	//기본 particle group 정보
	pg.SetName( m_strName.GetBuffer(0) );
	pg.SetBlendType( PredefinedBlendType(201 + m_cbBlendType.GetCurSel()) );
	pg.SetRenderType( PARTICLE_RENDER_TYPE(m_cbRenderType.GetCurSel()) );
	CTFileName filename = m_strTexFileName.GetBuffer(0);
	pg.SetTexture( filename );
	pg.SetTextureInfo( MEX(m_uiMexWidth), MEX(m_uiMexHeight), INDEX(m_uiColumn), INDEX(m_uiRow) );

	//particle emitter 정보
	if( !m_dlgCPEProperty.SetPropertyTo(pg) ) return;	//저장 실패
	pg.GetEmitter()->GetParticlePrototype().SetTexPos((UBYTE)m_uiRow, (UBYTE)m_uiColumn);

	//particle absorption 정보
	if( !m_dlgCPAProperty.SetPropertyTo(pg) ) return;	//저장 실패

	//particle process 정보
	if( !m_dlgCPPProperty.SetPropertyTo(pg) ) return;	//저장 실패

	//set and complete
	if(m_cbRegisteredName.GetCurSel() != -1)
	{
		CString strPGName;
		m_cbRegisteredName.GetLBText(m_cbRegisteredName.GetCurSel(), strPGName);
		if( m_strName == strPGName )
		{
			m_cbRegisteredName.DeleteString(m_cbRegisteredName.GetCurSel());
			CParticleGroupManager::Instance().Unregister(m_strName.GetBuffer(0));
		}
		else
		{
			int index = m_cbRegisteredName.FindString(-1, m_strName.GetBuffer(0));
			if(index != -1) m_cbRegisteredName.DeleteString(index);
		}
		CParticleGroupManager::Instance().Unregister(m_strName.GetBuffer(0));
	}
	if( CParticleGroupManager::Instance().Register(pg.Copy()) )
	{
		int index = m_cbRegisteredName.AddString(m_strName);
		m_cbRegisteredName.SetCurSel(index);
	}

	CPrintF("Particle Group save : %s\n", m_strName);
}

void CDlgCParticleGroupProperty::OnBtnPgTexFilefind() 
{
	// TODO: Add your control notification handler code here
	TEXSELECTDLG(m_strTexFileName);
}

