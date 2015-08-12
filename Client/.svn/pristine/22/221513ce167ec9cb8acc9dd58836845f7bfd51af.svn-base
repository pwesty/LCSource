// DlgAutosettingFeatherFall.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgAutosettingFeatherFall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutosettingFeatherFall dialog


CDlgAutosettingFeatherFall::CDlgAutosettingFeatherFall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutosettingFeatherFall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutosettingFeatherFall)
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_fPeriod = 0.0f;
	m_fWidth = 0.0f;
	m_uiRepeat = 0;
	//}}AFX_DATA_INIT
}


void CDlgAutosettingFeatherFall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutosettingFeatherFall)
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_fHeight);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_fLength);
	DDX_Text(pDX, IDC_EDIT_PERIOD, m_fPeriod);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_fWidth);
	DDX_Text(pDX, IDC_EDIT_REPEAT, m_uiRepeat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutosettingFeatherFall, CDialog)
	//{{AFX_MSG_MAP(CDlgAutosettingFeatherFall)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutosettingFeatherFall message handlers
const FLOAT horiVal[4] = { 0.25f, 0.125f+0.01f+horiVal[0], 0.0675f+0.02f+horiVal[1], 0.0175f+0.03f+horiVal[2] };
const FLOAT vertVal[4] = { 0.25f, 0.25f+vertVal[0], 0.25f+vertVal[1], 0.25f+vertVal[2] };

void CDlgAutosettingFeatherFall::FillSettingInfo(CParticlesProcessDynamicState &ppds)
{
	UpdateData(SAVETOOBJECT);

	const FLOAT	timeVal = 1.0f / 32.0f * m_fPeriod;
	const FLOAT widthVal[4] =
	{
		horiVal[0] * m_fWidth
		, horiVal[1] * m_fWidth
		, horiVal[2] * m_fWidth
		, horiVal[3] * m_fWidth
	};
	const FLOAT lengthVal[4] =
	{
		horiVal[0] * m_fLength
		, horiVal[1] * m_fLength
		, horiVal[2] * m_fLength
		, horiVal[3] * m_fLength
	};
	const FLOAT heightVal[4] =
	{
		vertVal[0] * m_fHeight
		, vertVal[1] * m_fHeight
		, vertVal[2] * m_fHeight
		, vertVal[3] * m_fHeight
	};

	FLOAT3D vPos;
	FLOAT currentTime=0.0f;
	for(UINT uiRepeat=0; uiRepeat<m_uiRepeat; ++uiRepeat)
	{
		currentTime = m_fPeriod * uiRepeat;
		if(uiRepeat == 0) ppds.AddDeltaPos( currentTime, FLOAT3D(0, 0, 0) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[0], heightVal[0], -lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[1], heightVal[1], -lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[2], heightVal[2], -lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[3], -lengthVal[3]) );

		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[2], -lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[1], -lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[0], -lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], 0, 0) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[0], lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[1], lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[2], lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[3], heightVal[3], lengthVal[3]) );

		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[2], heightVal[2], lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[1], heightVal[1], lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(-widthVal[0], heightVal[0], lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(0, 0, 0) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[0], heightVal[0], -lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[1], heightVal[1], -lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[2], heightVal[2], -lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[3], -lengthVal[3]) );

		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[2], -lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[1], -lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[0], -lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], 0, 0) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[0], lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[1], lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[2], lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[3], heightVal[3], lengthVal[3]) );

		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[2], heightVal[2], lengthVal[2]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[1], heightVal[1], lengthVal[1]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(widthVal[0], heightVal[0], lengthVal[0]) );
		ppds.AddDeltaPos( currentTime += timeVal, FLOAT3D(0, 0, 0) );
	}
}
