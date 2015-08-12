// DlgExtensionEdit.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgExtensionEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExtensionEdit dialog


CDlgExtensionEdit::CDlgExtensionEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExtensionEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExtensionEdit)
	m_fTotalTimeRatio = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgExtensionEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExtensionEdit)
	DDX_Control(pDX, IDC_BTN_TOTAL_TIME_RATIO_APPLY, m_btnTotalTimeRatioApply);
	DDX_Text(pDX, IDC_EDIT_TOTAL_TIME_RATIO, m_fTotalTimeRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExtensionEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgExtensionEdit)
	ON_BN_CLICKED(IDC_BTN_TOTAL_TIME_RATIO_APPLY, OnBtnTotalTimeRatioApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExtensionEdit message handlers
BOOL CDlgExtensionEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_iType != 0)
		m_btnTotalTimeRatioApply.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExtensionEdit::OnBtnTotalTimeRatioApply() 
{
	UpdateData(SAVETOOBJECT);
	int i = 0;
	int size = 0;
	m_fTotalTimeRatio;
	CEffectGroup *pEG = CEffectGroupManager::Instance().GetRegisteredMap().find(m_strName.GetBuffer(0))->second;
	if(pEG == NULL) return;

	CEffectGroup::vector_effect_key &ekeys = const_cast<CEffectGroup::vector_effect_key&>(pEG->GetEffectKeyVector());
	size = ekeys.size();
	for( i = 0; i < size; ++i)
	{
		ekeys[i].m_fStartTime *= m_fTotalTimeRatio;
	}
	CEffectGroup::vector_tag_key &tkeys = const_cast<CEffectGroup::vector_tag_key&>(pEG->GetTagKeyVector());
	size = tkeys.size();
	for( i = 0; i < size; ++i)
	{
		tkeys[i].m_fSettingTime *= m_fTotalTimeRatio;
	}
	theApp.m_dlgCEffectGroupProperty.EffectGroupSelectChangeNotify();

	m_fTotalTimeRatio = 0.0f;
	UpdateData(SAVETOFORM);
}

