#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Info/MyInfo.h>


#define COMMAND_BUTTON_SIZE 18

// ----------------------------------------------------------------------------
// Name : CUISummon()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISummon::CUISummon( int nUIIndex )
: m_nUIIndex(nUIIndex)
, m_pIconSkill(NULL)
{
	m_penEntity			= NULL;	
	m_nSummonType		= CSlaveInfo::SLAVE_FIRE;
	m_nCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
	m_nOldCommand		= CSlaveInfo::COMMAND_NONE;
	m_nIndex			= -1;	
	m_nLeftTime			= 0;
	m_nMaxTime			= 0;
	m_bSetMaxTime		= FALSE;	
	m_bShowToolTip		= FALSE;
	m_strToolTip		= CTString("");
}

// ----------------------------------------------------------------------------
// Name : ~CUISummon()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISummon::~CUISummon()
{
	SAFE_DELETE(m_pIconSkill);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Set Slave Name 
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_FIRE,_S(2526,"불의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_WIND,_S(2527,"바람의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_WATER,_S(2528,"물의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_LAND,_S(2529,"땅의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_GUARD,_S(3981, "경비병"));

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 12, 43 );
	m_rcHP.SetRect( 60, 5, 167, 8 );
	m_rcHPBack.SetRect( 55, 4, 172, 9 );
	m_rcTime.SetRect( 60, 11, 167, 14 );	
	m_rcTimeBack.SetRect( 55, 10, 172, 15 );
	//m_rcSkill.SetRect( 153, 20, 171, 38 );
	m_rcSkill.SetRect( 0, 0, SUMMON_WIDTH, SUMMON_HEIGHT );

	m_rcSummonImage.SetRect( 17, 4, 52, 39 );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Summon.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground1.SetUV( 0, 0, SUMMON_WIDTH, SUMMON_HEIGHT, fTexWidth, fTexHeight );	

	// HP
	m_rtHP.SetUV( 185, 45, 191, 48, fTexWidth, fTexHeight );

	// Time
	m_rtTime.SetUV( 185, 50, 191, 53, fTexWidth, fTexHeight );

	// HP background
	m_rtHPBack.SetUV( 55, 4, 172, 9, fTexWidth, fTexHeight );

	// Time background
	m_rtTimeBack.SetUV( 55, 10, 172, 15, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoL.SetUV( 0, 101, 7, 123, fTexWidth, fTexHeight );
	m_rtInfoM.SetUV( 10, 101, 12, 123, fTexWidth, fTexHeight );
	m_rtInfoR.SetUV( 15, 101, 22, 123, fTexWidth, fTexHeight );

	// Summon Image
	m_rtSummon[CSlaveInfo::SLAVE_WIND].SetUV( 0, 45, 35, 80, fTexWidth, fTexHeight );	
	m_rtSummon[CSlaveInfo::SLAVE_WATER].SetUV( 74, 45, 109, 80, fTexWidth, fTexHeight );	
	m_rtSummon[CSlaveInfo::SLAVE_FIRE].SetUV( 37, 45, 72, 80, fTexWidth, fTexHeight );	
	m_rtSummon[CSlaveInfo::SLAVE_LAND].SetUV( 111, 45, 146, 80, fTexWidth, fTexHeight );
	m_rtSummon[CSlaveInfo::SLAVE_GUARD].SetUV(148, 45, 184, 80, fTexWidth, fTexHeight);
	
	m_btnCommand[CSlaveInfo::COMMAND_ATTACK].Create( this, CTString( "" ), 57, 20, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE );
	m_btnCommand[CSlaveInfo::COMMAND_ATTACK].SetUV( UBS_IDLE, 0, 82, 17, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_ATTACK].SetUV( UBS_CLICK, 19, 82, 36, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_ATTACK].CopyUV( UBS_IDLE, UBS_ON );
	m_btnCommand[CSlaveInfo::COMMAND_ATTACK].CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnCommand[CSlaveInfo::COMMAND_PINCER].Create( this, CTString( "" ), 81, 20, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE );
	m_btnCommand[CSlaveInfo::COMMAND_PINCER].SetUV( UBS_IDLE, 38, 82, 55, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_PINCER].SetUV( UBS_CLICK, 57, 82, 74, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_PINCER].CopyUV( UBS_IDLE, UBS_ON );
	m_btnCommand[CSlaveInfo::COMMAND_PINCER].CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnCommand[CSlaveInfo::COMMAND_HOLD].Create( this, CTString( "" ), 105, 20, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE );
	m_btnCommand[CSlaveInfo::COMMAND_HOLD].SetUV( UBS_IDLE, 76, 82, 93, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_HOLD].SetUV( UBS_CLICK, 95, 82, 112, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_HOLD].CopyUV( UBS_IDLE, UBS_ON );
	m_btnCommand[CSlaveInfo::COMMAND_HOLD].CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnCommand[CSlaveInfo::COMMAND_PROTECTION].Create( this, CTString( "" ), 129, 20, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE );
	m_btnCommand[CSlaveInfo::COMMAND_PROTECTION].SetUV( UBS_IDLE, 114, 82, 131, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_PROTECTION].SetUV( UBS_CLICK, 133, 82, 150, 99, fTexWidth, fTexHeight );
	m_btnCommand[CSlaveInfo::COMMAND_PROTECTION].CopyUV( UBS_IDLE, UBS_ON );
	m_btnCommand[CSlaveInfo::COMMAND_PROTECTION].CopyUV( UBS_IDLE, UBS_DISABLE );

	m_pIconSkill = new CUIIcon;
	m_pIconSkill->Create(this, 153, 20, COMMAND_BUTTON_SIZE, COMMAND_BUTTON_SIZE, m_nUIIndex, UBET_SKILL);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int iRow = m_nUIIndex - UI_SUMMON_START;

	SetPos( pixMinI + pUIManager->GetPlayerInfo()->GetPosX() + SUMMON_START_X_GAP, pUIManager->GetPlayerInfo()->GetHeight() + (GetHeight() * iRow));
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::UpdateHPInfo()
{	
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CSlaveTargetInfom* pSlaveInfo = pInfo->GetMySlaveInfo(m_nUIIndex - UI_SUMMON_START);
	// HP	
	float fHP = pSlaveInfo->fHealth;
	float fMaxHP = pSlaveInfo->fMaxHealth;
	FLOAT	fHPRatio = Clamp( fHP / fMaxHP, 0.0f, 1.0f );	
	m_rcHP.Right = m_rcHP.Left + SUMMON_BAR_WIDTH * fHPRatio;
}

// ----------------------------------------------------------------------------
// Name : UpdateTimeInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::UpdateTimeInfo()
{	
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds(); // 현재 시간 얻기
	__int64	llCurDelay = llCurTime - m_tmLeftTime;

	int lLeftTime = m_nLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	float fTime = (float)lLeftTime/(float)m_nMaxTime;
	FLOAT	fTimeRatio = Clamp( fTime, 0.0f, 1.0f );	
	m_rcTime.Right = m_rcTime.Left + SUMMON_BAR_WIDTH * fTimeRatio;
}

// ----------------------------------------------------------------------------
// Name : SetLeftTime()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::SetLeftTime( int nLeftTime )
{
	m_nLeftTime		= nLeftTime;
	m_tmLeftTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
}

// ----------------------------------------------------------------------------
// Name : SetLeftTime()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::SetMaxTime( int nMaxTime )
{
	if( !m_bSetMaxTime )
	{
		m_nMaxTime = nMaxTime;
		m_bSetMaxTime = TRUE;
	}
}

// ----------------------------------------------------------------------------
// Name : SetSummonType()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::SetSummonType( int iType )
{
	m_nSummonType = iType;
	const INDEX iSkillIndex = SlaveInfo().GetSkillIndex( m_nSummonType );
	AddSkill( iSkillIndex );
}

// ----------------------------------------------------------------------------
// Name : SetCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::SetCommand( INDEX iCommand )
{
	if( !GetSummonEntity() )
		return;		
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSummonCommand( GetSummonEntity(), iCommand );
}

// ----------------------------------------------------------------------------
// Name : GetCommand()
// Desc :
// ----------------------------------------------------------------------------
INDEX& CUISummon::GetCommand()
{
	return m_nCurrentCommand;
};

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::Render()
{
	if( !GetSummonEntity() )
		return;

	// Timer for highlight buttons
	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;
	if( dElapsedTime > 0.5 )
	{
		bHighlight = !bHighlight;
		do
		{
			dElapsedTime -= 0.5;
		}
		while( dElapsedTime > 0.5 );
	}	

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + SUMMON_HEIGHT,
		m_rtBackground1.U0, m_rtBackground1.V0, m_rtBackground1.U1, m_rtBackground1.V1,
		0xFFFFFFFF );	
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	
	// Update info of target
	UpdateHPInfo();
	UpdateTimeInfo();
	
	// 커맨드가 변화되었다면...
	if( m_nCurrentCommand != m_nOldCommand && m_nCurrentCommand != CSlaveInfo::COMMAND_NONE )
	{
		m_nOldCommand = m_nCurrentCommand;
		for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
		{
			m_btnCommand[i].SetBtnState( i == m_nCurrentCommand ? UBS_CLICK : UBS_IDLE );
		}
	}
	
	// Background of HP
	pDrawPort->AddTexture( m_nPosX + m_rcHPBack.Left, m_nPosY + m_rcHPBack.Top,
		m_nPosX + m_rcHPBack.Right, m_nPosY + m_rcHPBack.Bottom,
		m_rtHPBack.U0, m_rtHPBack.V0, m_rtHPBack.U1, m_rtHPBack.V1,
		0xFFFFFFFF );

	// Background of Time
	pDrawPort->AddTexture( m_nPosX + m_rcTimeBack.Left, m_nPosY + m_rcTimeBack.Top,
		m_nPosX + m_rcTimeBack.Right, m_nPosY + m_rcTimeBack.Bottom,
		m_rtTimeBack.U0, m_rtTimeBack.V0, m_rtTimeBack.U1, m_rtTimeBack.V1,
		0xFFFFFFFF );
	
	// HP
	pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
		0xFFFFFFFF );

	// Time
	pDrawPort->AddTexture( m_nPosX + m_rcTime.Left, m_nPosY + m_rcTime.Top,
		m_nPosX + m_rcTime.Right, m_nPosY + m_rcTime.Bottom,
		m_rtTime.U0, m_rtTime.V0, m_rtTime.U1, m_rtTime.V1,
		0xFFFFFFFF );

	// Summon Image
	pDrawPort->AddTexture( m_nPosX + m_rcSummonImage.Left, m_nPosY + m_rcSummonImage.Top,
		m_nPosX + m_rcSummonImage.Right, m_nPosY + m_rcSummonImage.Bottom,
		m_rtSummon[m_nSummonType].U0, m_rtSummon[m_nSummonType].V0, 
		m_rtSummon[m_nSummonType].U1, m_rtSummon[m_nSummonType].V1,
		0xFFFFFFFF );
	
	for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
	{
		m_btnCommand[i].Render();
	}	
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	if (m_pIconSkill->IsEmpty() == false)
	{
		m_pIconSkill->Render(pDrawPort);
	}

	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture, FALSE, PBT_ADD );

	if( bHighlight )
	{	
		if( m_nCurrentCommand != CSlaveInfo::COMMAND_NONE )
		{
			m_btnCommand[m_nCurrentCommand].RenderHighlight( 0xFFFFFFFF );		
		}
	}
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Tool tip
	if( m_bShowToolTip )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top,
											m_rcToolTip.Left + 7, m_rcToolTip.Bottom,
											m_rtInfoL.U0, m_rtInfoL.V0, m_rtInfoL.U1, m_rtInfoL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcToolTip.Left + 7, m_rcToolTip.Top,
											m_rcToolTip.Right - 7, m_rcToolTip.Bottom,
											m_rtInfoM.U0, m_rtInfoM.V0, m_rtInfoM.U1, m_rtInfoM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcToolTip.Right - 7, m_rcToolTip.Top,
											m_rcToolTip.Right, m_rcToolTip.Bottom,
											m_rtInfoR.U0, m_rtInfoR.V0, m_rtInfoR.U1, m_rtInfoR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Text in tool tip
		pDrawPort->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{		
		int	nInfoX, nInfoY, nWidth, nHeight;

		nOldToolTipID	= nToolTipID;
		BOOL bFirstSlot	= (m_nUIIndex == UI_SUMMON_START);		// 첫번째 슬롯인 경우.
		switch( nToolTipID )
		{
		case CSlaveInfo::COMMAND_ATTACK:
			if( bFirstSlot )
				m_strToolTip.PrintF( "%s %s", _S(2354, "선공" ), CTString( "(Shift + 1)" ) );
			else
				m_strToolTip.PrintF( "%s %s", _S(2354, "선공" ), CTString( "(Shift + Z)" ) );
			m_btnCommand[nToolTipID].GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnCommand[nToolTipID].GetWidth();
			nHeight = m_btnCommand[nToolTipID].GetHeight();
			break;
			
		case CSlaveInfo::COMMAND_PINCER:
			if( bFirstSlot )
				m_strToolTip.PrintF( "%s %s", _S(2355, "협공" ), CTString( "(Shift + 2)" ) );
			else
				m_strToolTip.PrintF( "%s %s", _S(2355, "협공" ), CTString( "(Shift + X)" ) );
			m_btnCommand[nToolTipID].GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnCommand[nToolTipID].GetWidth();
			nHeight = m_btnCommand[nToolTipID].GetHeight();
			break;
			
		case CSlaveInfo::COMMAND_HOLD:
			if( bFirstSlot )
				m_strToolTip.PrintF( "%s %s", _S(2356, "홀드" ), CTString( "(Shift + 3)" ) );
			else
				m_strToolTip.PrintF( "%s %s", _S(2356, "홀드" ), CTString( "(Shift + C)" ) );
			m_btnCommand[nToolTipID].GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnCommand[nToolTipID].GetWidth();
			nHeight = m_btnCommand[nToolTipID].GetHeight();			
			break;
			
		case CSlaveInfo::COMMAND_PROTECTION:
			if( bFirstSlot )
				m_strToolTip.PrintF( "%s %s", _S(2357, "수호" ), CTString( "(Shift + 4)" ) );
			else
				m_strToolTip.PrintF( "%s %s", _S(2357, "수호" ), CTString( "(Shift + V)" ) );
			m_btnCommand[nToolTipID].GetAbsPos( nInfoX, nInfoY );
			nWidth = m_btnCommand[nToolTipID].GetWidth();
			nHeight = m_btnCommand[nToolTipID].GetHeight();			
			break;

		case 5:
			{
				int		nSkillIndex = m_pIconSkill->getIndex();
				CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIndex );
				
				if( bFirstSlot )
					m_strToolTip.PrintF( "%s %s", rSkill.GetName(), CTString( "(Shift + 5)" ) );
				else
					m_strToolTip.PrintF( "%s %s", rSkill.GetName(), CTString( "(Shift + B)" ) );
				m_pIconSkill->GetAbsPos( nInfoX, nInfoY );
				nWidth = m_pIconSkill->GetWidth();
				nHeight = m_pIconSkill->GetHeight();
				break;
			}
		}
		
		int	nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY -= nHeight ;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoX += pUIManager->GetMaxI() - ( nInfoX + nInfoWidth );

		m_rcToolTip.SetRect( nInfoX, nInfoY, nInfoX + nInfoWidth, nInfoY + nInfoHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : AddSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::AddSkill( int iSkillIndex )
{
	// Empty slot
	if( iSkillIndex == -1 )
	{
		m_pIconSkill->clearIconData();
	}
	else
	{
		CSkill	&rSkillData = _pNetwork->GetSkillData( iSkillIndex );
		
		if( rSkillData.GetUsingSorcererFlag() & (1 << (m_nSummonType + 1) ) )
		{
			m_pIconSkill->setSkill(iSkillIndex);
			SlaveInfo().SetSkillIndex( m_nSummonType, iSkillIndex );
		}
	}
}

//------------------------------------------------------------------------------
// CUISummon::UseSkill()
// Explain:  
//------------------------------------------------------------------------------
void CUISummon::UseSkill()
{
	const int iSkillIndex = m_pIconSkill->getIndex();
	if (iSkillIndex != -1 && MY_INFO()->GetSkillDelay(iSkillIndex) == false)
	{
		UseSkill( iSkillIndex );
	}
}

//------------------------------------------------------------------------------
// CPetInfo::UseSkill()
// CUISummon:  
//------------------------------------------------------------------------------
void CUISummon::UseSkill( int nIndex )
{	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseSlaveSkill( m_penEntity, nIndex );	
}

// ----------------------------------------------------------------------------
// Name : StartSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUISummon::StartSkillDelay( int nIndex )
{
	BOOL bResult = FALSE;	
	if (m_pIconSkill->getBtnType() == UBET_SKILL &&
		m_pIconSkill->getIndex() == nIndex)
	{
		m_pIconSkill->setCooltime(true);
		bResult = TRUE;
	}
	return bResult;
}

// ----------------------------------------------------------------------------
// Name : ResetSummon()
// Desc :
// ----------------------------------------------------------------------------
void CUISummon::ResetSummon()
{
	m_penEntity			= NULL;	
	m_nSummonType		= CSlaveInfo::SLAVE_FIRE;
	m_nCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
	m_nOldCommand		= CSlaveInfo::COMMAND_NONE;
	m_nIndex			= -1;
	m_nLeftTime			= 0;
	m_nMaxTime			= 0;
	m_bSetMaxTime		= FALSE;
	m_bShowToolTip		= FALSE;
	m_strToolTip		= CTString("");

	for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
	{
		m_btnCommand[i].SetBtnState( UBS_IDLE );
	}	
	
	m_pIconSkill->clearIconData();
}

// ----------------------------------------------------------------------------
// Name : WarpOfResetSummon()
// Date : 2006-07-11(오전 10:08:24), By eons
// Desc : 텔레포트 시에 소환수 리셋( 주어진 명령 및 엔티티만 초기화 )
// ----------------------------------------------------------------------------
void CUISummon::WarpOfResetSummon()
{
	m_penEntity			= NULL;	
	m_nSummonType		= CSlaveInfo::SLAVE_FIRE;
	m_nCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
	m_nOldCommand		= CSlaveInfo::COMMAND_NONE;

	m_bShowToolTip		= FALSE;
	m_strToolTip		= CTString("");

	for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
	{
		m_btnCommand[i].SetBtnState( UBS_IDLE );
	}	
	
	m_pIconSkill->clearIconData();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISummon::MouseMessage( MSG *pMsg )
{
	// If target is not exist
	if( !GetSummonEntity() )
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !GetSummonEntity() )
					bTitleBarClick = FALSE;

				return WMSG_SUCCESS;
			}
			// Option button
			else if (m_pIconSkill->MouseMessage(pMsg) != WMSG_FAIL)
			{
				ShowToolTip( TRUE, 5 );
				return WMSG_SUCCESS;
			}
			else
			{
				for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
				{
					if( m_btnCommand[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						ShowToolTip( TRUE, i );
						return WMSG_SUCCESS;
					}
				}
			}

			ShowToolTip( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else
				{
					BOOL bIsButtonClick = FALSE;

					if (m_pIconSkill->MouseMessage(pMsg) != WMSG_FAIL)
					{
						bIsButtonClick = TRUE;
					}

					for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
					{
						if( m_btnCommand[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bIsButtonClick = TRUE;
							break;
						}					
					}

					if( !bIsButtonClick )
					{
						BOOL bIsButtonClick = FALSE;
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( GetSummonEntity() );
					}
				}

				CUIManager::getSingleton()->RearrangeOrder( m_nUIIndex, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					for( int i = CSlaveInfo::COMMAND_ATTACK; i < CSlaveInfo::COMMAND_TOTAL; ++i )
					{
						if( ( wmsgResult = m_btnCommand[i].MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								SetCommand( i );								
							}
							return WMSG_SUCCESS;
						}
					}

					if ((wmsgResult = m_pIconSkill->MouseMessage(pMsg)) != WMSG_FAIL)
					{
						UseSkill();							
						return WMSG_SUCCESS;
					}
				}
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					if ((pUIManager->GetDragIcon()->getBtnType() == UBET_SKILL && 
						 pUIManager->GetDragIcon()->GetWhichUI() == UI_SKILL_NEW) || 
						(pUIManager->GetDragIcon()->getBtnType() == UBET_SKILL &&  
						 pUIManager->GetDragIcon()->GetWhichUI() == UI_QUICKSLOT))
					{
						if( IsInsideRect( nX, nY, m_rcSkill ) )
						{							
							int	nIndex = pUIManager->GetDragIcon()->getIndex();
							AddSkill( nIndex );			
						}
					}
					// Reset holding button
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


