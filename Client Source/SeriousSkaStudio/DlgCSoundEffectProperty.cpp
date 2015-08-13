// DlgCSoundEffectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCSoundEffectProperty.h"
#include <Engine/Effect/CSoundEffect.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ULONG GetFlagsByLB(CListBox &lb, int bitOffset = 0)
{
	ULONG ulRet = 0;
	for(int i=0; i<lb.GetCount(); ++i)
	{
		ulRet |= (lb.GetSel(i) > 0) << (i + bitOffset);
	}
	return ulRet;
}

void SetFlagsToLB(ULONG flags, CListBox &lb, int bitOffset = 0)
{
	for(int i=bitOffset; i<32; ++i)
	{
		if( (1 << i) & flags )
		{
			lb.SetSel(i - bitOffset, TRUE);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDlgCSoundEffectProperty dialog


CDlgCSoundEffectProperty::CDlgCSoundEffectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCSoundEffectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSoundEffectProperty)
	m_fFallOff = 0.0f;
	m_strSndFileName = _T("");
	m_fHotSpot = 0.0f;
	m_fVolume = 0.0f;
	m_fPitch = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCSoundEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSoundEffectProperty)
	DDX_Control(pDX, IDC_SND_FLAGS, m_listSndFlag);
	DDX_Text(pDX, IDC_SND_FALLOFF, m_fFallOff);
	DDX_Text(pDX, IDC_SND_FILENAME, m_strSndFileName);
	DDX_Text(pDX, IDC_SND_HOTSPOT, m_fHotSpot);
	DDX_Text(pDX, IDC_SND_VOLUME, m_fVolume);
	DDX_Text(pDX, IDC_SND_PITCH, m_fPitch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSoundEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCSoundEffectProperty)
	ON_BN_CLICKED(IDC_BTN_SND_FILEFIND, OnBtnSndFilefind)
	//}}AFX_MSG_MAP
	ON_WM_REGISTER_EFFECT()
	ON_WM_SELECT_EFFECT()
	ON_WM_CLEAR_FORM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSoundEffectProperty message handlers

#define SND_FLAG_CNT 7

BOOL CDlgCSoundEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listSndFlag.AddString("Loop");
	m_listSndFlag.AddString("3D");
	m_listSndFlag.AddString("Volumetric");
	m_listSndFlag.AddString("Local");
	m_listSndFlag.AddString("Music");
	m_listSndFlag.AddString("Non-Game");
	m_listSndFlag.AddString("Voice");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSoundEffectProperty::OnBtnSndFilefind() 
{
	// TODO: Add your control notification handler code here
	FILESELECTDLG(m_strSndFileName, "wav");
}

void CDlgCSoundEffectProperty::OnClearForm()
{
	m_fFallOff = 0.0f;
	m_strSndFileName = _T("");
	m_fHotSpot = 0.0f;
	m_fVolume = 0.0f;
	m_fPitch = 0.0f;
	SetFlagsToLB(0, m_listSndFlag);

	this->UpdateData(SAVETOFORM);
}

LRESULT CDlgCSoundEffectProperty::OnRegisterEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;
	this->UpdateData(SAVETOOBJECT);

	if(m_strSndFileName == "") return 0;

	ULONG ulFlag = GetFlagsByLB(m_listSndFlag);

	CSoundEffect se;
	CTFileName fnmSnd = m_strSndFileName;
	se.SetSound(fnmSnd);
	se.Set3DParameter(m_fFallOff, m_fHotSpot, m_fVolume, m_fPitch);
	se.SetSoundPlayFlag(ulFlag);

	ep.RegisterEffect(se);
	return 0;
}

LRESULT CDlgCSoundEffectProperty::OnSelectEffect(WPARAM wp, LPARAM lp)
{
	if(wp == 0) return 0;
	CDlgCEffectProperty &ep = *(CDlgCEffectProperty*)wp;

	CString strName;
	int index = ep.m_cbRegisteredName.GetCurSel();
	if(index != CB_ERR)
	{
		ep.m_cbRegisteredName.GetLBText( index, strName );
		CSoundEffect *pSE = (CSoundEffect*)CEffectManager::Instance().Create( strName.GetBuffer(0) );
		if(pSE == NULL) return 0;

		this->m_fFallOff = pSE->GetFallOff();
		this->m_fHotSpot = pSE->GetHotSpot();
		this->m_fVolume = pSE->GetVolume();
		this->m_fPitch = pSE->GetPitch();
		this->m_strSndFileName = pSE->GetSoundFileName().str_String;
		ULONG ulFlag = pSE->GetSoundPlayFlag();
		SetFlagsToLB(ulFlag, m_listSndFlag);

		this->UpdateData(SAVETOFORM);

		ep.SetEffectInfo(*pSE);
		CEffectManager::Instance().Destroy((CEffect *&)pSE);
	}
	return 0;
}
