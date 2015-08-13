// DlgCEffectControlProperty.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgCEffectControlProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectControlProperty dialog


CDlgCEffectControlProperty::CDlgCEffectControlProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCEffectControlProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCEffectControlProperty)
	m_fAddTime = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_fValue3 = 0.0f;
	m_fValue2 = 0.0f;
	m_fValue1 = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCEffectControlProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCEffectControlProperty)
	DDX_Control(pDX, IDC_LIST_ADDED_STATE, m_lbAddedState);
	DDX_Control(pDX, IDC_STATIC_STATE_NAME, m_stDataName);
	DDX_Control(pDX, IDC_VALUE3, m_edValue3);
	DDX_Control(pDX, IDC_VALUE2, m_edValue2);
	DDX_Control(pDX, IDC_VALUE1, m_edValue1);
	DDX_Control(pDX, IDC_BTN_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_TAB_STATE_COMPONENT, m_tcStateComponent);
	DDX_Text(pDX, IDC_ADD_TIME, m_fAddTime);
	DDV_MinMaxFloat(pDX, m_fAddTime, 0.f, 99999.f);
	DDX_Text(pDX, IDC_TWINKLE_PERIOD, m_fTwinklePeriod);
	DDV_MinMaxFloat(pDX, m_fTwinklePeriod, 0.f, 99999.f);
	DDX_Text(pDX, IDC_VALUE3, m_fValue3);
	DDX_Text(pDX, IDC_VALUE2, m_fValue2);
	DDX_Text(pDX, IDC_VALUE1, m_fValue1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCEffectControlProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgCEffectControlProperty)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_LBN_SELCHANGE(IDC_LIST_ADDED_STATE, OnSelchangeListAddedState)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_STATE_COMPONENT, OnSelchangeTabStateComponent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCEffectControlProperty message handlers

enum TC_NAME
{
	TC_COLOR	= 0,
	TC_ALPHA	= 1,
	TC_STRETCH	= 2,
	TC_POSITION	= 3,
	TC_ANGLE	= 4,
};

BOOL CDlgCEffectControlProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_tcStateComponent.InsertItem(TC_COLOR, "Color");
	m_tcStateComponent.InsertItem(TC_ALPHA, "Alpha");
	m_tcStateComponent.InsertItem(TC_STRETCH, "Stretch");
	m_tcStateComponent.InsertItem(TC_POSITION, "Postion");
	m_tcStateComponent.InsertItem(TC_ANGLE, "Angle");
	
	m_btnColor.ShowWindow(TRUE);
	m_edValue1.ShowWindow(FALSE);
	m_edValue2.ShowWindow(FALSE);
	m_edValue3.ShowWindow(FALSE);

	m_stDataName.SetWindowText("Color");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCEffectControlProperty::RefreshPosSpline(BOOL bShow, SINT selPointIdx)
{
	if(m_effectControl.GetPosCount() < 2) return;

	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_bShowSpline = bShow;
	if(!bShow) return;
	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_siCurrentPoint = SINT(selPointIdx / _pTimer->TickQuantum);
	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_vectorSplinePoint.clear();
	m_effectControl.GetPosSpline().Prepare();
	FLOAT timeBegin = m_effectControl.GetPosTime(0);
	FLOAT timeEnd = m_effectControl.GetPosTime(m_effectControl.GetPosCount()-1);
	for(FLOAT time=timeBegin; time<=timeEnd; time+=_pTimer->TickQuantum)
	{
		((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_vectorSplinePoint.push_back(
			m_effectControl.GetPosSpline().Value(time) );
	}
}

void CDlgCEffectControlProperty::OnSelchangeTabStateComponent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_btnColor.ShowWindow(FALSE);
	m_edValue1.ShowWindow(FALSE);
	m_edValue2.ShowWindow(FALSE);
	m_edValue3.ShowWindow(FALSE);

	((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_bShowSpline = FALSE;
	CString strDataName;
	BOOL bShowValue3=FALSE, bShowValue2=FALSE, bShowValue1=FALSE, bShowColor=FALSE;

	switch((TC_NAME)m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:		strDataName = "Color";		bShowColor = TRUE;	break;
	case TC_ALPHA:		strDataName = "Alpha";		bShowValue1 = TRUE;	break;
	case TC_STRETCH:	strDataName = "Stretch";	bShowValue3 = bShowValue2 = bShowValue1 = TRUE;	break;
	case TC_POSITION:
		{
			strDataName = "Postion";
			bShowValue3 = bShowValue2 = bShowValue1 = TRUE;
			RefreshPosSpline(TRUE, -1);
		}	break;
	case TC_ANGLE:		strDataName = "Angle";		bShowValue3 = bShowValue2 = bShowValue1 = TRUE;	break;
	}

	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);
	m_btnColor.ShowWindow(bShowColor);
	m_edValue1.ShowWindow(bShowValue1);
	m_edValue2.ShowWindow(bShowValue2);
	m_edValue3.ShowWindow(bShowValue3);
	m_btnColor.SetColor(C_WHITE | CT_OPAQUE);
	m_fValue1 = m_fValue2 = m_fValue3 = 0;
	m_fAddTime = 0;
	UpdateData(SAVETOFORM);

	m_stDataName.SetWindowText(strDataName.GetBuffer(0));

	Refresh(m_tcStateComponent.GetCurSel());
	
	*pResult = 0;
}

void CDlgCEffectControlProperty::OnSelchangeListAddedState() 
{
	if(m_lbAddedState.GetCurSel() == -1) return;

	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:
		{
			m_fAddTime = m_effectControl.GetColorTime(m_lbAddedState.GetCurSel());
			m_btnColor.SetColor(m_effectControl.GetColorValue(m_lbAddedState.GetCurSel()));
		} break;
	case TC_ALPHA:
		{
			m_fAddTime = m_effectControl.GetAlphaTime(m_lbAddedState.GetCurSel());
			m_fValue1 = m_effectControl.GetAlphaValue(m_lbAddedState.GetCurSel());
		} break;
	case TC_STRETCH:
		{
			m_fAddTime = m_effectControl.GetStretchTime(m_lbAddedState.GetCurSel());
			m_fValue3 = m_effectControl.GetStretchValue(m_lbAddedState.GetCurSel())(1);
			m_fValue2 = m_effectControl.GetStretchValue(m_lbAddedState.GetCurSel())(2);
			m_fValue1 = m_effectControl.GetStretchValue(m_lbAddedState.GetCurSel())(3);
		} break;
	case TC_POSITION:
		{
			m_fAddTime = m_effectControl.GetPosTime(m_lbAddedState.GetCurSel());
			m_fValue3 = m_effectControl.GetPosValue(m_lbAddedState.GetCurSel())(1);
			m_fValue2 = m_effectControl.GetPosValue(m_lbAddedState.GetCurSel())(2);
			m_fValue1 = m_effectControl.GetPosValue(m_lbAddedState.GetCurSel())(3);
			((CSeriousSkaStudioDoc*)theApp.GetDocument())->m_siCurrentPoint = SINT(m_lbAddedState.GetCurSel() / _pTimer->TickQuantum);
		} break;
	case TC_ANGLE:
		{
			m_fAddTime = m_effectControl.GetAngleTime(m_lbAddedState.GetCurSel());
			m_fValue3 = m_effectControl.GetAngleValue(m_lbAddedState.GetCurSel())(1);
			m_fValue2 = m_effectControl.GetAngleValue(m_lbAddedState.GetCurSel())(2);
			m_fValue1 = m_effectControl.GetAngleValue(m_lbAddedState.GetCurSel())(3);
		} break;
	}
	UpdateData(SAVETOFORM);
}

void CDlgCEffectControlProperty::OnBtnAdd() 
{
	UpdateData(SAVETOOBJECT);
	
	UINT addedPos = 0;
	switch((TC_NAME)m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:		addedPos = m_effectControl.AddColor( m_fAddTime, m_btnColor.GetColor() );							break;
	case TC_ALPHA:		addedPos = m_effectControl.AddAlpha( m_fAddTime, Clamp(UBYTE(m_fValue1), UBYTE(0), UBYTE(255)) );	break;
	case TC_STRETCH:	addedPos = m_effectControl.AddStretch( m_fAddTime, FLOAT3D(m_fValue3, m_fValue2, m_fValue1) );		break;
	case TC_POSITION:
		{
			addedPos = m_effectControl.AddPos( m_fAddTime, FLOAT3D(m_fValue3, m_fValue2, m_fValue1) );			RefreshPosSpline(TRUE, -1);
			RefreshPosSpline(TRUE, addedPos);
		}	break;
	case TC_ANGLE:		addedPos = m_effectControl.AddAngle( m_fAddTime, FLOAT3D(m_fValue3, m_fValue2, m_fValue1) );		break;
	}
	Refresh((TC_NAME)m_tcStateComponent.GetCurSel());
	m_lbAddedState.SetCurSel( addedPos );
}

void CDlgCEffectControlProperty::OnBtnDel() 
{
	if(m_lbAddedState.GetCurSel() == -1) return;

	BOOL bRefreshSpline = FALSE;
	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:		m_effectControl.RemoveColor(m_lbAddedState.GetCurSel());	break;
	case TC_ALPHA:		m_effectControl.RemoveAlpha(m_lbAddedState.GetCurSel());	break;
	case TC_STRETCH:	m_effectControl.RemoveStretch(m_lbAddedState.GetCurSel());	break;
	case TC_POSITION:
		{
			m_effectControl.RemovePos(m_lbAddedState.GetCurSel());
			bRefreshSpline = TRUE;
		}	break;
	case TC_ANGLE:		m_effectControl.RemoveAngle(m_lbAddedState.GetCurSel());	break;
	}
	UINT removedPos = m_lbAddedState.GetCurSel();
	m_lbAddedState.DeleteString(m_lbAddedState.GetCurSel());
	if(removedPos >= m_lbAddedState.GetCount()) removedPos = m_lbAddedState.GetCount() - 1;
	if(bRefreshSpline) RefreshPosSpline(TRUE, removedPos);
	Refresh((TC_NAME)m_tcStateComponent.GetCurSel());
	m_lbAddedState.SetCurSel(removedPos);
}

void CDlgCEffectControlProperty::OnBtnModify() 
{
	OnBtnDel();
	OnBtnAdd();
}


void CDlgCEffectControlProperty::Clear()
{
	m_fAddTime = 0.0f;
	m_fTwinklePeriod = 0.0f;
	m_fValue3 = 0.0f;
	m_fValue2 = 0.0f;
	m_fValue1 = 0.0f;
	m_btnColor.SetColor(C_WHITE|CT_OPAQUE);

	m_tcStateComponent.SetCurSel(0);
	m_lbAddedState.ResetContent();

	m_btnColor.ShowWindow(TRUE);
	m_edValue1.ShowWindow(FALSE);
	m_edValue2.ShowWindow(FALSE);
	m_edValue3.ShowWindow(FALSE);

	m_stDataName.SetWindowText("Color");

	UpdateData(SAVETOFORM);
}

void CDlgCEffectControlProperty::Refresh(int index)
{
	m_lbAddedState.ResetContent();
	m_tcStateComponent.SetCurSel(index);

	switch(m_tcStateComponent.GetCurSel())
	{
	case TC_COLOR:
		{
			for(UINT i=0; i<m_effectControl.GetColorCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "%.2f", m_effectControl.GetColorTime(i) );
				//strOutput.Format( "%.2f/%x", m_effectControl.GetColorTime(i), m_effectControl.GetColorValue(i) );
				//원래 값까지 표시하려고 했으나 list의 sort와 부딪히는 면이 있어서 뺐다.
				if(m_effectControl.GetColorTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_ALPHA:
		{
			for(UINT i=0; i<m_effectControl.GetAlphaCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "%.2f", m_effectControl.GetAlphaTime(i) );
				if(m_effectControl.GetAlphaTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_STRETCH:
		{
			for(UINT i=0; i<m_effectControl.GetStretchCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_effectControl.GetStretchTime(i) );
				if(m_effectControl.GetStretchTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_POSITION:
		{
			for(UINT i=0; i<m_effectControl.GetPosCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_effectControl.GetPosTime(i) );
				if(m_effectControl.GetPosTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	case TC_ANGLE:
		{
			for(UINT i=0; i<m_effectControl.GetAngleCount(); ++i)
			{
				CString strOutput;
				strOutput.Format( "0%.2f", m_effectControl.GetAngleTime(i) );
				if(m_effectControl.GetAngleTime(i) < 10) strOutput = "0" + strOutput;
				m_lbAddedState.AddString(strOutput);
			}
		} break;
	}
}

