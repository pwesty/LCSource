// DlgCPPDynamicStateProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCPPDynamicStateProperty.h"

#include <Engine/Effect/CParticlesProcessDynamicState.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgCPPDSOffsetPosAutoSetting.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDynamicStateProperty dialog


CDlgCPPDynamicStateProperty::CDlgCPPDynamicStateProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCPPDynamicStateProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCPPDynamicStateProperty)
	m_fFadeInTime = 0.0f;
	m_fFadeOutTime = 0.0f;
	m_fValue1 = 0.0f;
	m_fValue2 = 0.0f;
	m_fAddTime = 0.0f;
	m_fValue3 = 0.0f;
	m_fTwinklePeriod = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCPPDynamicStateProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCPPDynamicStateProperty)
	DDX_Control(pDX, IDC_VALUE3, m_edValue3);
	DDX_Control(pDX, IDC_STATIC_STATE_NAME, m_stDataName);
	DDX_Control(pDX, IDC_VALUE2, m_edValue2);
	DDX_Control(pDX, IDC_VALUE1, m_edValue1);
	DDX_Control(pDX, IDC_BTN_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_TAB_STATE_COMPONENT, m_tcStateComponent);
	DDX_Control(pDX, IDC_LIST_ADDED_STATE, m_lbAddedState);
	DDX_Text(pDX, IDC_FADE_IN_TIME, m_fFadeInTime);
	DDV_MinMaxFloat(pDX, m_fFadeInTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_FADE_OUT_TIME, m_fFadeOutTime);
	DDV_MinMaxFloat(pDX, m_fFadeOutTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_VALUE1, m_fValue1);
	DDX_Text(pDX, IDC_VALUE2, m_fValue2);
	DDX_Text(pDX, IDC_ADD_TIME, m_fAddTime);
	DDV_MinMaxFloat(pDX, m_fAddTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_VALUE3, m_fValue3);
	DDX_Text(pDX, IDC_TWINKLE_PERIOD, m_fTwinklePeriod);
	DDV_MinMaxFloat(pDX, m_fTwinklePeriod, 0.f, 99999.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCPPDynamicStateProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCPPDynamicStateProperty)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_STATE_COMPONENT, OnSelChangeTabStateComponent)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_LBN_SELCHANGE(IDC_LIST_ADDED_STATE, OnSelChangeListAddedState)
	ON_BN_CLICKED(IDC_BTN_AUTOSETTING, OnBtnAutosetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPDynamicStateProperty message handlers

enum TC_NAME
{
	TC_COLOR	= 0,
	TC_ALPHA	= 1,
	TC_TEXPOS	= 2,
	TC_SIZE		= 3,
	TC_MASS		= 4,
	TC_DELTAPOS	= 5,
	TC_ANGLE	= 6,
};

BOOL CDlgCPPDynamicStateProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tcStateComponent.InsertItem(TC_COLOR, "Color");
	m_tcStateComponent.InsertItem(TC_ALPHA, "Alpha");
	m_tcStateComponent.InsertItem(TC_SIZE, "Size");
	m_tcStateComponent.InsertItem(TC_MASS, "Mass");
	m_tcStateComponent.InsertItem(TC_TEXPOS, "TexPos");
	m_tcStateComponent.InsertItem(TC_DELTAPOS, "Delta");
	m_tcStateComponent.InsertItem(TC_ANGLE, "Angle");
	
	m_btnColor.ShowWindow(TRUE);
	m_edValue1.ShowWindow(FALSE);
	m_edValue2.ShowWindow(FALSE);
	m_edValue3.ShowWindow(FALSE);
	m_btnColor.SetColor(C_WHITE | CT_OPAQUE);
	m_fValue1 = m_fValue2 = m_fValue3 = 0;
	m_fAddTime = 0;
	UpdateData(SAVETOFORM);

	m_stDataName.SetWindowText("Color");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCPPDynamicStateProperty::OnSelChangeTabStateComponent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_btnColor.ShowWindow(FALSE);
	m_edValue1.ShowWindow(FALSE);
	m_edValue2.ShowWindow(FALSE);
	m_edValue3.ShowWindow(FALSE);

	CString strDataName;
	BOOL bShowValue3=FALSE, bShowValue2=FALSE, bShowValue1=FALSE, bShowColor=FALSE;

	switch((TC_NAME)m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:		strDataName = "Color";		bShowColor = TRUE;	break;
	case TC_ALPHA:		strDataName = "Alpha";		bShowValue1 = TRUE;	break;
	case TC_SIZE:		strDataName = "Size";		bShowValue1 = bShowValue2 = TRUE;	break;
	case TC_MASS:		strDataName = "Mass";		bShowValue1 = TRUE;	break;
	case TC_TEXPOS:		strDataName = "TexPos";		bShowValue1 = bShowValue2 = TRUE;	break;
	case TC_DELTAPOS:	strDataName = "DeltaPos";	bShowValue1 = bShowValue2 = bShowValue3 = TRUE;	break;
	case TC_ANGLE:		strDataName = "Angle";		bShowValue1 = bShowValue2 = bShowValue3 = TRUE;	break;
	}

	m_btnColor.ShowWindow(bShowColor);
	m_edValue1.ShowWindow(bShowValue1);
	m_edValue2.ShowWindow(bShowValue2);
	m_edValue3.ShowWindow(bShowValue3);

	m_stDataName.SetWindowText(strDataName.GetBuffer(0));
	if(bShowColor)	m_btnColor.SetColor(0xFFFFFFFF);
	if(bShowValue1)	m_edValue1.SetWindowText("0");
	if(bShowValue2)	m_edValue2.SetWindowText("0");
	if(bShowValue3)	m_edValue3.SetWindowText("0");
	
	Refresh(m_tcStateComponent.GetCurSel());
	
	*pResult = 0;
}

void CDlgCPPDynamicStateProperty::OnSelChangeListAddedState() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAddedState.GetCurSel() == -1) return;

	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:
		{
			m_fAddTime = m_ppdstate.GetColorTime(m_lbAddedState.GetCurSel());
			m_btnColor.SetColor(m_ppdstate.GetColorValue(m_lbAddedState.GetCurSel()));
		} break;
	case TC_ALPHA:
		{
			m_fAddTime = m_ppdstate.GetAlphaTime(m_lbAddedState.GetCurSel());
			m_fValue1 = m_ppdstate.GetAlphaValue(m_lbAddedState.GetCurSel());
		} break;
	case TC_SIZE:
		{
			m_fAddTime = m_ppdstate.GetSizeTime(m_lbAddedState.GetCurSel());
			m_fValue2 = m_ppdstate.GetSizeValue(m_lbAddedState.GetCurSel()).GetWidth();
			m_fValue1 = m_ppdstate.GetSizeValue(m_lbAddedState.GetCurSel()).GetHeight();
		} break;
	case TC_MASS:
		{
			m_fAddTime = m_ppdstate.GetMassTime(m_lbAddedState.GetCurSel());
			m_fValue1 = m_ppdstate.GetMassValue(m_lbAddedState.GetCurSel());
		} break;
	case TC_TEXPOS:
		{
			m_fAddTime = m_ppdstate.GetTexPosTime(m_lbAddedState.GetCurSel());
			m_fValue2 = m_ppdstate.GetTexPosValue(m_lbAddedState.GetCurSel()).GetRow();	//row
			m_fValue1 = m_ppdstate.GetTexPosValue(m_lbAddedState.GetCurSel()).GetCol();	//col
		} break;
	case TC_DELTAPOS:
		{
			m_fAddTime = m_ppdstate.GetDeltaPosTime(m_lbAddedState.GetCurSel());
			m_fValue3 = m_ppdstate.GetDeltaPosValue(m_lbAddedState.GetCurSel())(1);
			m_fValue2 = m_ppdstate.GetDeltaPosValue(m_lbAddedState.GetCurSel())(2);
			m_fValue1 = m_ppdstate.GetDeltaPosValue(m_lbAddedState.GetCurSel())(3);
		} break;
	case TC_ANGLE:
		{
			m_fAddTime = m_ppdstate.GetAngleTime(m_lbAddedState.GetCurSel());
			m_fValue3 = m_ppdstate.GetAngleValue(m_lbAddedState.GetCurSel())(1);
			m_fValue2 = m_ppdstate.GetAngleValue(m_lbAddedState.GetCurSel())(2);
			m_fValue1 = m_ppdstate.GetAngleValue(m_lbAddedState.GetCurSel())(3);
		} break;
	}
	UpdateData(SAVETOFORM);
}

void CDlgCPPDynamicStateProperty::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(SAVETOOBJECT);
	
	UINT addedPos = 0;
	switch((TC_NAME)m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:	addedPos = m_ppdstate.AddColor( m_fAddTime, m_btnColor.GetColor() );							break;
	case TC_ALPHA:	addedPos = m_ppdstate.AddAlpha( m_fAddTime, Clamp(UBYTE(m_fValue1), UBYTE(0), UBYTE(255)) );	break;
	case TC_SIZE:	addedPos = m_ppdstate.AddSize( m_fAddTime, m_fValue2, m_fValue1 );								break;
	case TC_MASS:	addedPos = m_ppdstate.AddMass( m_fAddTime, m_fValue1 );											break;
	case TC_TEXPOS:	addedPos = m_ppdstate.AddTexPos( m_fAddTime, Clamp(UBYTE(m_fValue2), UBYTE(0), UBYTE(255)),
																 Clamp(UBYTE(m_fValue1), UBYTE(0), UBYTE(255)) );	break;
	case TC_DELTAPOS:	addedPos = m_ppdstate.AddDeltaPos( m_fAddTime, FLOAT3D(m_fValue3, m_fValue2, m_fValue1) );	break;
	case TC_ANGLE:		addedPos = m_ppdstate.AddAngle( m_fAddTime, ANGLE3D(m_fValue3, m_fValue2, m_fValue1) );		break;
	}
	Refresh((TC_NAME)m_tcStateComponent.GetCurSel());
	m_lbAddedState.SetCurSel( (int)addedPos );
}

void CDlgCPPDynamicStateProperty::OnBtnModify() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAddedState.GetCurSel() == -1) return;

	OnBtnDel();
	OnBtnAdd();
}

void CDlgCPPDynamicStateProperty::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if(m_lbAddedState.GetCurSel() == -1) return;

	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:		m_ppdstate.RemoveColor(m_lbAddedState.GetCurSel());		break;
	case TC_ALPHA:		m_ppdstate.RemoveAlpha(m_lbAddedState.GetCurSel());		break;
	case TC_SIZE:		m_ppdstate.RemoveSize(m_lbAddedState.GetCurSel());		break;
	case TC_MASS:		m_ppdstate.RemoveMass(m_lbAddedState.GetCurSel());		break;
	case TC_TEXPOS:		m_ppdstate.RemoveTexPos(m_lbAddedState.GetCurSel());	break;
	case TC_DELTAPOS:	m_ppdstate.RemoveDeltaPos(m_lbAddedState.GetCurSel());	break;
	case TC_ANGLE:		m_ppdstate.RemoveAngle(m_lbAddedState.GetCurSel());		break;
	}
	UINT removedPos = m_lbAddedState.GetCurSel();
	m_lbAddedState.DeleteString(m_lbAddedState.GetCurSel());
	if(removedPos >= m_lbAddedState.GetCount()) removedPos = m_lbAddedState.GetCount() - 1;
	m_lbAddedState.SetCurSel(removedPos);
	Refresh((TC_NAME)m_tcStateComponent.GetCurSel());
}

void CDlgCPPDynamicStateProperty::OnBtnAutosetting() 
{
	// TODO: Add your control notification handler code here
	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:
		{
		} break;
	case TC_ALPHA:
		{
		} break;
	case TC_SIZE:
		{
		} break;
	case TC_MASS:
		{
		} break;
	case TC_TEXPOS:
		{
		} break;
	case TC_DELTAPOS:
		{
			INDEX cnt = m_lbAddedState.GetCount();
			INDEX i = 0;
			for( i = 0; i < cnt; ++i ) m_lbAddedState.DeleteString(0);
			cnt = m_ppdstate.GetDeltaPosCount();
			for( i = 0; i < cnt; ++i ) m_ppdstate.RemoveDeltaPos(0);
			CDlgCPPDSOffsetPosAutoSetting dlg;
			dlg.SetDynamicState(&m_ppdstate);
			dlg.DoModal();
			Refresh(TC_DELTAPOS);
		} break;
	case TC_ANGLE:
		{
		} break;
	}
}


void CDlgCPPDynamicStateProperty::Clear()
{
	m_ppdstate = CParticlesProcessDynamicState();
	m_stDataName.SetWindowText("Color");
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);
	m_tcStateComponent.SetCurSel(0);
	m_lbAddedState.ResetContent();
	m_fTwinklePeriod = 0;
	m_fFadeInTime = 0;
	m_fFadeOutTime = 0;
	m_fValue1 = 0;
	m_fValue2 = 0;
	m_fValue3 = 0;
	m_fAddTime = 0;

	UpdateData(SAVETOFORM);
}

void CDlgCPPDynamicStateProperty::Refresh(int index)
{
	m_lbAddedState.ResetContent();
	m_tcStateComponent.SetCurSel(index);

	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:
		{
			for(UINT i=0; i<m_ppdstate.GetColorCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "%.2f", m_ppdstate.GetColorTime(i) );
				//strOutput.Format( "%.2f/%x", m_ppdstate.GetColorTime(i), m_ppdstate.GetColorValue(i) );
				//원래 값까지 표시하려고 했으나 list의 sort와 부딪히는 면이 있어서 뺐다.
				if(m_ppdstate.GetColorTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_ALPHA:
		{
			for(UINT i=0; i<m_ppdstate.GetAlphaCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "%.2f", m_ppdstate.GetAlphaTime(i) );
				if(m_ppdstate.GetAlphaTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_SIZE:
		{
			for(UINT i=0; i<m_ppdstate.GetSizeCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_ppdstate.GetSizeTime(i) );
				if(m_ppdstate.GetSizeTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_MASS:
		{
			for(UINT i=0; i<m_ppdstate.GetMassCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_ppdstate.GetMassTime(i) );
				if(m_ppdstate.GetMassTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_TEXPOS:
		{
			for(UINT i=0; i<m_ppdstate.GetTexPosCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_ppdstate.GetTexPosTime(i) );
				if(m_ppdstate.GetTexPosTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_DELTAPOS:
		{
			for(UINT i=0; i<m_ppdstate.GetDeltaPosCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_ppdstate.GetDeltaPosTime(i) );
				if(m_ppdstate.GetDeltaPosTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_ANGLE:
		{
			for(UINT i=0; i<m_ppdstate.GetAngleCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_ppdstate.GetAngleTime(i) );
				if(m_ppdstate.GetAngleTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	}
}

