// DlgCEffectGroupProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCEffectGroupProperty.h"
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline CString MakeTimeName(float time, const CString &name)
{
	//99초대까지 표시
	CString strTime;
	if(time > 10) strTime.Format("%2.2f/", time);
	else strTime.Format("0%1.2f/", time);
	strTime += name;
	return strTime;
}
/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectGroupProperty dialog


CDlgCEffectGroupProperty::CDlgCEffectGroupProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCEffectGroupProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCEffectGroupProperty)
	m_strEffectGroupName = _T("");
	m_fTagSettingTime = 0.0f;
	m_fEffectStartTime = 0.0f;
	m_strTagName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCEffectGroupProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCEffectGroupProperty)
	DDX_Control(pDX, IDC_REGISTERED_TAG_NAME, m_cbTagName);
	DDX_Control(pDX, IDC_EFFECT_TYPE, m_cbEffectType);
	DDX_Control(pDX, IDC_REGISTERED_TAG_KEY, m_lstRegisteredTagKey);
	DDX_Control(pDX, IDC_REGISTERED_EFFECT_KEY, m_lstRegisteredEffectKey);
	DDX_Control(pDX, IDC_REGISTERED_EFFECT_FOR_EK, m_cbRegisteredEffect);
	DDX_Control(pDX, IDC_EG_REGISTERED_NAME, m_cbRegisteredEGName);
	DDX_Text(pDX, IDC_EG_NAME, m_strEffectGroupName);
	DDV_MaxChars(pDX, m_strEffectGroupName, 32);
	DDX_Text(pDX, IDC_EK_SETTING_TIME, m_fTagSettingTime);
	DDV_MinMaxFloat(pDX, m_fTagSettingTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_EK_START_TIME, m_fEffectStartTime);
	DDV_MinMaxFloat(pDX, m_fEffectStartTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TAG_NAME_FOR_TAG_KEY, m_strTagName);
	DDV_MaxChars(pDX, m_strTagName, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCEffectGroupProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCEffectGroupProperty)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EK_ADD, OnBtnEkAdd)
	ON_BN_CLICKED(IDC_BTN_EK_DEL, OnBtnEkDel)
	ON_BN_CLICKED(IDC_BTN_TK_ADD, OnBtnTkAdd)
	ON_BN_CLICKED(IDC_BTN_TK_DEL, OnBtnTkDel)
	ON_LBN_SELCHANGE(IDC_REGISTERED_EFFECT_KEY, OnSelChangeRegisteredEffectKey)
	ON_LBN_SELCHANGE(IDC_REGISTERED_TAG_KEY, OnSelChangeRegisteredTagKey)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_TK_ADDALL, OnBtnTkAddall)
	ON_CBN_SELENDOK(IDC_EG_REGISTERED_NAME, OnSelEndOkEgRegisteredName)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELENDOK(IDC_EFFECT_TYPE, OnSelendokEffectType)
	ON_CBN_SELENDOK(IDC_REGISTERED_TAG_NAME, OnSelendokRegisteredTagName)
	ON_EN_MAXTEXT(IDC_EK_START_TIME, OnMaxtextEkStartTime)
	ON_LBN_DBLCLK(IDC_REGISTERED_EFFECT_KEY, OnDblclkRegisteredEffectKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectGroupProperty message handlers

void CDlgCEffectGroupProperty::Clear()
{
	m_strEffectGroupName = _T("");
	m_fTagSettingTime = 0.0f;
	m_fEffectStartTime = 0.0f;
	m_strTagName = _T("");

	m_lstRegisteredTagKey.ResetContent();
	m_lstRegisteredEffectKey.ResetContent();
	m_cbRegisteredEGName.ResetContent();

	m_vectorTagVector.clear();

	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectGroupProperty::ClearTagkey()
{
	m_strTagName = _T("");
	m_fTagSettingTime = 0.0f;
	m_lstRegisteredTagKey.ResetContent();
	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectGroupProperty::RefreshEffect()
{
	CEffectManager::my_map::iterator iterBegin = CEffectManager::Instance().GetRegisteredMap().begin();
	CEffectManager::my_map::iterator iterEnd = CEffectManager::Instance().GetRegisteredMap().end();
	CEffectManager::my_map::iterator iter;

	m_cbRegisteredEffect.ResetContent();
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{

		CEffect &effect = *((*iter).second);
		
		if( m_cbEffectType.GetCurSel() > 0 )
		{
			if( effect.GetType() != m_cbEffectType.GetCurSel() ) continue;
		}
		
		m_cbRegisteredEffect.AddString( effect.GetName().c_str() );
		
	}
}

void CDlgCEffectGroupProperty::Refresh()
{
	Clear();

	RefreshEffect();

	CEffectGroupManager::my_map::iterator iterBegin = CEffectGroupManager::Instance().GetRegisteredMap().begin();
	CEffectGroupManager::my_map::iterator iterEnd = CEffectGroupManager::Instance().GetRegisteredMap().end();
	CEffectGroupManager::my_map::iterator iter;

	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		CEffectGroup &eg = *((*iter).second);
		m_cbRegisteredEGName.AddString(eg.GetName().c_str());
	}
}

void CDlgCEffectGroupProperty::SelectEffectKey(int index)
{
	if(index == -1) return;

	ClearTagkey();
	std::vector<TagKeyInfo> &vectorTKI = m_vectorTagVector[index];
	for(int i=0; i<vectorTKI.size(); ++i)
	{
		m_lstRegisteredTagKey.AddString( MakeTimeName(vectorTKI[i].m_fSettingTime, vectorTKI[i].m_strTagName.c_str()) );
	}
	m_lstRegisteredTagKey.SetCurSel(-1);
}

void CDlgCEffectGroupProperty::OnBtnEkAdd() 
{
	// TODO: Add your control notification handler code here
	if(m_cbRegisteredEffect.GetCurSel() == -1) return;

	CString strSelEffect;
	m_cbRegisteredEffect.GetLBText(m_cbRegisteredEffect.GetCurSel(), strSelEffect);

	this->UpdateData(SAVETOOBJECT);
	int index = m_lstRegisteredEffectKey.AddString( MakeTimeName(m_fEffectStartTime, strSelEffect) );
	m_vectorTagVector.insert( m_vectorTagVector.begin()+index, std::vector<TagKeyInfo>() );

	m_lstRegisteredEffectKey.SetCurSel(index);
	SelectEffectKey(index);
}

void CDlgCEffectGroupProperty::OnBtnEkDel() 
{
	// TODO: Add your control notification handler code here
	if(m_lstRegisteredEffectKey.GetCurSel() == -1) return;

	ClearTagkey();

	std::vector<TagKeyInfo> &vectorTI = m_vectorTagVector[ m_lstRegisteredEffectKey.GetCurSel() ];
	m_vectorTagVector.erase(m_vectorTagVector.begin() + m_lstRegisteredEffectKey.GetCurSel());

	int curSel = m_lstRegisteredEffectKey.GetCurSel();
	m_lstRegisteredEffectKey.DeleteString( curSel );
	if(curSel >= m_lstRegisteredEffectKey.GetCount()) curSel = m_lstRegisteredEffectKey.GetCount() - 1;
	m_lstRegisteredEffectKey.SetCurSel( curSel );
}

void CDlgCEffectGroupProperty::OnBtnTkAdd() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(SAVETOOBJECT);

	if(m_strTagName == "") return;
	if(m_lstRegisteredEffectKey.GetCurSel() == -1) return;
	m_strTagName.MakeUpper();

	int index = m_lstRegisteredTagKey.AddString( MakeTimeName(m_fTagSettingTime, m_strTagName) );
	m_lstRegisteredTagKey.SetCurSel(index);

	std::vector<TagKeyInfo> &vectorTI = m_vectorTagVector[ m_lstRegisteredEffectKey.GetCurSel() ];
	TagKeyInfo tki;
	tki.m_fSettingTime = m_fTagSettingTime;
	tki.m_strTagName = m_strTagName.GetBuffer(0);
	vectorTI.insert( vectorTI.begin() + index, tki );
}

void CDlgCEffectGroupProperty::OnBtnTkAddall() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(SAVETOOBJECT);

	if(m_strTagName == "") return;
	if(m_lstRegisteredEffectKey.GetCurSel() == -1) return;

	int index = m_lstRegisteredTagKey.AddString( MakeTimeName(m_fTagSettingTime, m_strTagName) );
	m_lstRegisteredTagKey.SetCurSel(index);

	for(int i=0; i<m_lstRegisteredEffectKey.GetCount(); ++i)
	{
		std::vector<TagKeyInfo> &vectorTI = m_vectorTagVector[ i ];
		TagKeyInfo tki;
		tki.m_fSettingTime = m_fTagSettingTime;
		tki.m_strTagName = m_strTagName.GetBuffer(0);
		vectorTI.insert( vectorTI.begin() + index, tki );
	}
}

void CDlgCEffectGroupProperty::OnBtnTkDel() 
{
	// TODO: Add your control notification handler code here
	if(m_lstRegisteredTagKey.GetCurSel() == -1) return;
	if(m_lstRegisteredEffectKey.GetCurSel() == -1) return;

	std::vector<TagKeyInfo> &vectorTI = m_vectorTagVector[ m_lstRegisteredEffectKey.GetCurSel() ];
	vectorTI.erase( vectorTI.begin() + m_lstRegisteredTagKey.GetCurSel() );

	int curSel = m_lstRegisteredTagKey.GetCurSel();
	m_lstRegisteredTagKey.DeleteString( curSel );
	if(curSel >= m_lstRegisteredTagKey.GetCount()) curSel = m_lstRegisteredTagKey.GetCount() - 1;
	m_lstRegisteredTagKey.SetCurSel( curSel );
}

void CDlgCEffectGroupProperty::OnSelChangeRegisteredEffectKey() 
{
	// TODO: Add your control notification handler code here
	CString strEffectKey;
	m_lstRegisteredEffectKey.GetText(m_lstRegisteredEffectKey.GetCurSel(), strEffectKey);
	int index = strEffectKey.FindOneOf("/");
	m_fEffectStartTime = atof( strEffectKey.Left( index ).GetBuffer(0) );
	CString strEffectName = strEffectKey.Right( strEffectKey.GetLength() - (index+1) );
	m_cbRegisteredEffect.SetCurSel( m_cbRegisteredEffect.FindString(-1, strEffectName) );

	SelectEffectKey(m_lstRegisteredEffectKey.GetCurSel());

	// Date : 2005-11-08(오후 1:58:34), By Lee Ki-hwan
	m_cbTagName.ResetContent();
	m_cbTagName.AddString( "USER" );
	m_cbTagName.AddString( "__ROOT" );
	m_cbTagName.AddString( "HEAD" );
	m_cbTagName.AddString( "FACE" );
	m_cbTagName.AddString( "LHAND" );
	m_cbTagName.AddString( "RHAND" );
	m_cbTagName.AddString( "CENTER" );
	m_cbTagName.AddString( "LFOOT" );
	m_cbTagName.AddString( "RFOOT" );
	m_cbTagName.SetCurSel( 0 );


	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectGroupProperty::OnSelChangeRegisteredTagKey() 
{
	// TODO: Add your control notification handler code here
	CString strTagKey;
	m_lstRegisteredTagKey.GetText(m_lstRegisteredTagKey.GetCurSel(), strTagKey);
	int index = strTagKey.FindOneOf("/");
	m_fTagSettingTime = atof( strTagKey.Left( index ).GetBuffer(0) );
	m_strTagName = strTagKey.Right( strTagKey.GetLength() - (index+1) );

	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectGroupProperty::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}

void CDlgCEffectGroupProperty::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(SAVETOOBJECT);
	if(m_strEffectGroupName == "") return;
	if(m_lstRegisteredEffectKey.GetCount() == 0) return;

	if(m_strEffectGroupNameOld != m_strEffectGroupName && m_strEffectGroupName != ""
		&& m_cbRegisteredEGName.FindStringExact(-1, m_strEffectGroupName) != CB_ERR)
	{
		if(IDNO == MessageBox("같은 이름을 가진 이펙트그룹이 있습니다. 덮어 쓰시겠습니까?", "주의", MB_YESNO))
		{
			UpdateData(SAVETOFORM);
			return;
		}
	}
	m_strEffectGroupNameOld = m_strEffectGroupName;
	CEffectGroup eg;
	eg.SetName( m_strEffectGroupName.GetBuffer(0) );

	for(int i=0; i < m_lstRegisteredEffectKey.GetCount(); ++i)
	{
		CString strEffectKey;
		m_lstRegisteredEffectKey.GetText(i, strEffectKey);
		int index = strEffectKey.FindOneOf("/");
		float startTime = atof( strEffectKey.Left( index ).GetBuffer(0) );
		CString strEffectName = strEffectKey.Right( strEffectKey.GetLength() - (index+1) );
		
		INDEX iAddedEffect = eg.AddEffect(startTime, strEffectName.GetBuffer(0));
		std::vector<TagKeyInfo> &vectorTKI = m_vectorTagVector[i];
		for(int j = 0; j < vectorTKI.size(); ++j)
		{
			eg.AddTagForEffect(vectorTKI[j].m_fSettingTime, iAddedEffect, vectorTKI[j].m_strTagName);
		}
	}
	if( CEffectGroupManager::Instance().IsRegistered(eg.GetName()) )
	{
		CEffectGroupManager::Instance().Unregister(eg.GetName());
		m_cbRegisteredEGName.DeleteString( m_cbRegisteredEGName.FindString(-1, eg.GetName().c_str()) );
	}
	if( CEffectGroupManager::Instance().Register(eg.Copy()) )
	{
		int index = m_cbRegisteredEGName.AddString(eg.GetName().c_str());
		m_cbRegisteredEGName.SetCurSel(index);
	}
	CPrintF("Effect Group save : %s\n", eg.GetName().c_str());
}

void CDlgCEffectGroupProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(m_cbRegisteredEGName.GetCurSel() == -1) return;
	if(IDNO == MessageBox("정말 지우시겠습니까?", "주의", MB_YESNO)) return;

	CString strEGName;
	m_cbRegisteredEGName.GetLBText(m_cbRegisteredEGName.GetCurSel(), strEGName);
	CEffectGroupManager::Instance().Unregister(strEGName.GetBuffer(0));
	m_vectorTagVector.clear();

	m_cbRegisteredEGName.DeleteString(m_cbRegisteredEGName.GetCurSel());
	m_cbRegisteredEGName.SetCurSel(-1);
	
	Refresh();

	CPrintF("Effect Group delete : %s\n", strEGName);
}


void CDlgCEffectGroupProperty::OnSelEndOkEgRegisteredName() 
{
	// TODO: Add your control notification handler code here
	if(m_cbRegisteredEGName.GetCurSel() == -1) return;

	int index = m_cbRegisteredEGName.GetCurSel();
	Refresh();
	m_cbRegisteredEGName.SetCurSel(index);

	CString strEGName;
	m_cbRegisteredEGName.GetLBText(index, strEGName);
	CEffectGroup *pEG = CEffectGroupManager::Instance().GetRegisteredMap()[ std::string(strEGName.GetBuffer(0)) ];
	if(pEG == NULL) return;
	m_strEffectGroupNameOld = strEGName;

	m_strEffectGroupName = strEGName;
	m_vectorTagVector.clear();
	m_vectorTagVector.resize(pEG->GetEffectCount());
	for(int i=0; i<pEG->GetEffectCount(); ++i)
	{
		float fStartTime;
		std::string strEffectName;
		pEG->GetEffectKeyInfo(i, fStartTime, strEffectName);
		int index = m_lstRegisteredEffectKey.AddString( MakeTimeName(fStartTime, strEffectName.c_str()) );
		INDEX iEKV = pEG->FindEffectKeyValue(i);

		std::vector<TagKeyInfo> &vectorTKI = m_vectorTagVector[iEKV];
		vectorTKI.clear();
		for(int j=0; j<pEG->GetTagCount(); ++j)
		{
			INDEX iKV = pEG->GetTagKeyVector()[j].m_iEffectKeyValue;
			if(pEG->GetTagKeyVector()[j].m_iEffectKeyValue == iEKV)
			{
				TagKeyInfo tki;
				tki.m_fSettingTime = pEG->GetTagKeyVector()[j].m_fSettingTime;
				tki.m_strTagName = pEG->GetTagKeyVector()[j].m_strTagName;
				vectorTKI.push_back(tki);
			}
		}
	}
	this->UpdateData(SAVETOFORM);

	// Date : 2005-11-08(오후 2:01:58), By Lee Ki-hwan
	theApp.OnExecuteEffect( TRUE );
}

void CDlgCEffectGroupProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// ShowWindow일때 nStatus가 0임.
	if(bShow && nStatus == 0)
	{
		CString strEName;
		int curSel = m_cbRegisteredEffect.GetCurSel();
		if(curSel != CB_ERR) m_cbRegisteredEffect.GetLBText(curSel, strEName);
		this->RefreshEffect();
		curSel = m_cbRegisteredEffect.FindString(-1, strEName);
		m_cbRegisteredEffect.SetCurSel(curSel);
	}
}

BOOL CDlgCEffectGroupProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lstRegisteredEffectKey.SetHorizontalExtent(512);
	
	m_cbEffectType.AddString("All");
	m_cbEffectType.AddString("Terrain");
	m_cbEffectType.AddString("Light");
	m_cbEffectType.AddString("Particle");
	m_cbEffectType.AddString("Ska");
	m_cbEffectType.AddString("Mdl");
	m_cbEffectType.AddString("Trace");
	m_cbEffectType.AddString("Sound");
	m_cbEffectType.AddString("SplineBillboard");
	m_cbEffectType.AddString("Orbit");
	m_cbEffectType.AddString("ShockWave");
	m_cbEffectType.AddString("SplinePath");
	m_cbEffectType.AddString("Camera");
	m_cbEffectType.AddString("Entity");
	m_cbEffectType.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCEffectGroupProperty::OnSelendokEffectType() 
{
	// TODO: Add your control notification handler code here
	RefreshEffect();
}

void CDlgCEffectGroupProperty::OnSelendokRegisteredTagName() 
{
	// TODO: Add your control notification handler code here
	if( m_cbTagName.GetCurSel() <= 0 ) 
	{
		m_strTagName.Empty();
	}
	else
	{
		CString strTagName;
		m_cbTagName.GetLBText( m_cbTagName.GetCurSel(), strTagName );
		m_strTagName = strTagName;
		m_strTagName.MakeUpper();
	}
	this->UpdateData(SAVETOFORM);
}

void CDlgCEffectGroupProperty::OnMaxtextEkStartTime() 
{
	// TODO: Add your control notification handler code here
	if(m_cbRegisteredEffect.GetCurSel() == -1) return;
	
	CString strSelEffect;
	m_cbRegisteredEffect.GetLBText(m_cbRegisteredEffect.GetCurSel(), strSelEffect);

	this->UpdateData(SAVETOOBJECT);

	OnBtnEkDel();

	int index = m_lstRegisteredEffectKey.AddString( MakeTimeName(m_fEffectStartTime, strSelEffect) );

	m_vectorTagVector.insert( m_vectorTagVector.begin()+index, std::vector<TagKeyInfo>() );
		
	m_lstRegisteredEffectKey.SetCurSel(index);
	SelectEffectKey(index);
}

void CDlgCEffectGroupProperty::OnDblclkRegisteredEffectKey() 
{
	// TODO: Add your control notification handler code here
	theApp.m_dlgEffectEditType.ExcuteSubEffect();
}
