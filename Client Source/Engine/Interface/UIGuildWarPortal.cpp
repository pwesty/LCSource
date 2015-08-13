#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <map>
#include <vector>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Interface/UISelectResource.h>

#define BUTTON_HEIGHT	21
#define BUTTON_WIDTH	131
#define BUTTON_SPAN		4
#define TITLEBAR_HEIGHT 26

CTString m_strType[5];	// WSS_DRATAN_SEIGEWARFARE 2007/08/07 4->5 
int		m_nGateIndex[4][3] = {	7, 5, 4,
								6, 5, 4,
								6, 7, 4,
								6, 7, 5, 
							};

// WSS_DRATAN_SEIGEWARFARE 2007/08/07----->>
int		m_nGateIndexDratan[5][4] = {	14,15,16,17,
										13,15,16,17,
										13,14,16,17,
										13,14,15,17,
										13,14,15,16,
									};
// ---------------------------------------<<

enum Type{ A_TYPE, B_TYPE, C_TYPE, D_TYPE, E_TYPE};

#define	DEALY_TIME			3500 //초	 

// ----------------------------------------------------------------------------
// Name : CUIGuildWarPortal()
// Desc : Constructorreturn WMSG_FAIL;
// ----------------------------------------------------------------------------
CUIGuildWarPortal::CUIGuildWarPortal()
{
	m_bPrePortal		= FALSE;
	m_nPortal			= -1;

	m_tmLeftTime		= 0;
	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	m_btNpcAvailable	= 0;
	m_nSelPotalIdx		= 0;

}

// ----------------------------------------------------------------------------
// Name : ~CUIGuildWarPortal()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGuildWarPortal::~CUIGuildWarPortal()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 147, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	
	
	m_strType[0] =_S( 1922, "A지점으로 이동" );
	m_strType[1] =_S( 1923, "B지점으로 이동" );
	m_strType[2] =_S( 1924, "C지점으로 이동" );
	m_strType[3] =_S( 1925, "D지점으로 이동" ); 
// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	m_strType[4] =_S( 3670, "E지점으로 이동" ); 
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );	
}

// ----------------------------------------------------------------------------
// Name : OpenGuildWarPortal()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::OpenGuildWarPortal( int npcIndex  )
{	
	PortalCancel();
	switch( _pNetwork->MyCharacterInfo.sbJoinFlagMerac )
	{
	// 수성길드 : 수성측만 사용가능 
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		break;
	default : 
		return;
	}

	if( IsVisible() )
	{
		ResetGuildWarPortal();
		OpenGuildWarPortal( npcIndex );
		return;
	}
	

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	CUIButton TempUIButton;
	TempUIButton.SetUV( UBS_IDLE, 0, 117, 131, 138, fTexWidth, fTexHeight );
	TempUIButton.SetUV( UBS_CLICK, 0, 140, 131, 161, fTexWidth, fTexHeight );
	TempUIButton.CopyUV( UBS_IDLE, UBS_ON );
	TempUIButton.CopyUV( UBS_IDLE, UBS_DISABLE );

	const int iStartX = 20;
	const int iStartY = 30;	
	int iCurPosX = iStartX;
	int iCurPosY = iStartY;

	CTString strName;
		
	switch( npcIndex )
	{
	case 240 : // 하드 코딩 되는 NPC Index
		m_nNpcType = A_TYPE;
		break;
	case 241 :
		m_nNpcType = B_TYPE;
		break;
	case 242 :
		m_nNpcType = C_TYPE;
		break;
	case 243 :
		m_nNpcType = D_TYPE;
		break;
	}

	for( INDEX i = 0; i < 4; ++i )
	{
		if( i == m_nNpcType ) continue ;

		TempUIButton.Create( this, m_strType[i], iCurPosX, iCurPosY, BUTTON_WIDTH, BUTTON_HEIGHT );
		m_vectorResourceList.push_back(TempUIButton);
		iCurPosY += BUTTON_HEIGHT + BUTTON_SPAN;
	}
	int nHeight = m_vectorResourceList.size() * (BUTTON_HEIGHT + BUTTON_SPAN) + TITLEBAR_HEIGHT + 10;
	SetSize( m_nWidth, nHeight );
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILDWARPORTAL, TRUE );
	return;
}


// ----------------------------------------------------------------------------
// WSS_DRATAN_SEIGEWARFARE 2007/08/07
// Name : OpenGuildWarPortalDratan()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::OpenGuildWarPortalDratan()
{	
	PortalCancel();
	switch( _pNetwork->MyCharacterInfo.sbJoinFlagDratan )
	{
	// 수성길드 : 수성측만 사용가능 
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		break;
	default : 
		return;
	}

	if( IsVisible() )
	{
		// WSS_DRATAN_SIEGEWARFARE 2007/10/15
		// 더블 클릭시 문제가 있어서 수정...
	//	ResetGuildWarPortal();
	//	OpenGuildWarPortalDratan();
		return;
	}
	

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	CUIButton TempUIButton;
	TempUIButton.SetUV( UBS_IDLE, 0, 117, 131, 138, fTexWidth, fTexHeight );
	TempUIButton.SetUV( UBS_CLICK, 0, 140, 131, 161, fTexWidth, fTexHeight );
	TempUIButton.CopyUV( UBS_IDLE, UBS_ON );
	TempUIButton.CopyUV( UBS_IDLE, UBS_DISABLE );

	const int iStartX = 20;
	const int iStartY = 30;	
	int iCurPosX = iStartX;
	int iCurPosY = iStartY;

	CTString strName;
		
	m_nNpcType = m_nSelPotalIdx - 382;
	
	int tLoop = 0;
	for( INDEX i = 0; i < 5; ++i )
	{
		if( i == m_nNpcType ) continue ;
		if(!(m_btNpcAvailable&(1<<i))) continue;

		tLoop++;

		TempUIButton.Create( this, m_strType[i], iCurPosX, iCurPosY, BUTTON_WIDTH, BUTTON_HEIGHT );
		m_vectorResourceList.push_back(TempUIButton);
		iCurPosY += BUTTON_HEIGHT + BUTTON_SPAN;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// WSS_DRATAN_SIEGEWARFARE 2007/10/15
	if( tLoop == 0 )
	{
		pUIManager->GetChattingUI()->AddSysMessage(_S( 3810,"이동 가능한 워프타워가 존재하지 않습니다."));

	}
	else 
	{
		int nHeight = m_vectorResourceList.size() * (BUTTON_HEIGHT + BUTTON_SPAN) + TITLEBAR_HEIGHT + 10;
		SetSize( m_nWidth, nHeight );
		pUIManager->RearrangeOrder( UI_GUILDWARPORTAL, TRUE );
	}
	
}



// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildWarPortal::Render()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmLeftTime;
	
	if( m_bPrePortal )
	{
		if( llCurDelay > DEALY_TIME )
		{
			if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac != WCJF_NONE)
				_pNetwork->SendWarpTeleport( m_nGateIndex[m_nNpcType][m_nPortal] );
			else if(_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_NONE)
				_pNetwork->SendWarpTeleport( m_nGateIndexDratan[m_nNpcType][m_nPortal] );
			PortalCancel( FALSE );
			ResetGuildWarPortal();
		}
		return;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	int nX = m_nPosX;
	int nY = m_nPosY;
	pDrawPort->AddTexture( nX, m_nPosY, nX + m_nWidth, nY + TITLEBAR_HEIGHT,
		m_rtBackTop.U0, m_rtBackTop.V0, 
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY += TITLEBAR_HEIGHT;	
	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight - 7,
		m_rtBackMiddle1.U0, m_rtBackMiddle1.V0, 
		m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
		0xFFFFFFFF );
	
	nY = m_nPosY + m_nHeight - 7;

	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackBottom.U0, m_rtBackBottom.V0, 
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );	

	m_btnClose.Render();

	std::vector<CUIButton>::iterator itend = m_vectorResourceList.end();
	std::vector<CUIButton>::iterator it = m_vectorResourceList.begin();

	for(; it != itend; ++it)
	{
		(*it).Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	// Text in skill learn	
	pDrawPort->PutTextEx( _S( 1926, "워프 게이트" ), m_nPosX + SELECTRESOURCE_TITLE_TEXT_OFFSETX, 
		m_nPosY + SELECTRESOURCE_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuildWarPortal::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
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
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			
			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			for(int i = 0; i < m_vectorResourceList.size(); ++i)
			{
				m_vectorResourceList[i].MouseMessage(pMsg);
			}
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

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else
				{
					for(int i = 0; i < m_vectorResourceList.size(); ++i)
					{
						if( (wmsgResult = m_vectorResourceList[i].MouseMessage( pMsg ) )!= WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}	
							
				CUIManager::getSingleton()->RearrangeOrder( UI_GUILDWARPORTAL, TRUE );
				return WMSG_SUCCESS;
			}
			else
			{
			//	ResetGuildWarPortal();
			}			
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			
			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetGuildWarPortal();
				}
				return WMSG_SUCCESS;
			}
			else
			{	
				for(int i = 0; i < m_vectorResourceList.size(); ++i)
				{
					if( (wmsgResult = m_vectorResourceList[i].MouseMessage( pMsg ) )!= WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND)
						{
							PrePortal( i );
						}
						return WMSG_SUCCESS;
					}
				}
			}						
		}
		break;
	}
	return WMSG_SUCCESS;
}

//------------------------------------------------------------------------------
// CUIGuildWarPortal::ResetGuildWarPortal
// Explain:  
// Date : 2005-08-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildWarPortal::ResetGuildWarPortal()
{
	if(!m_vectorResourceList.empty())
		m_vectorResourceList.clear();
	
	m_nNpcType = -1;

	m_bPrePortal		= FALSE;
	m_nPortal			= -1;
	m_tmLeftTime		= 0;
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_GUILDWARPORTAL, FALSE );

	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	ResetPosition( pDrawPort->dp_MinI, pDrawPort->dp_MinJ, pDrawPort->dp_MaxI, pDrawPort->dp_MaxJ );

	m_btNpcAvailable	= 0;
	m_nSelPotalIdx		= 0;
}

//------------------------------------------------------------------------------
// CUIGuildWarPortal::PortalCancel
// Explain:  
// Date : 2005-08-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildWarPortal::PortalCancel( BOOL bMessage )	// 워프 중인 상태를 취소한다.
{
	if( m_bPrePortal == FALSE )
		return;

	if( bMessage == TRUE )
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 1927, "워프가 취소되었습니다." ) );  
	}

	std::map<SLONG, CEffectGroup *>::iterator it = m_mapEG.find( _pNetwork->MyCharacterInfo.index );
	if(it != m_mapEG.end())
	{
		if((*it).second != NULL && CEffectGroupManager::Instance().IsValidCreated((*it).second))
			(*it).second->Stop(0);
		m_mapEG.erase( it );
	}

	ResetGuildWarPortal();
}

//------------------------------------------------------------------------------
// CUIGuildWarPortal::PrePortal
// Explain:  
// Date : 2005-08-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildWarPortal::PrePortal( int nPotal )
{
	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	CTagManager *pTM = NULL;

	if(CEntity::GetPlayerEntity(0) != NULL
		&& CEntity::GetPlayerEntity(0)->en_pmiModelInstance != NULL)
	{
		pTM = &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager;
	}

	CEffectGroup *pEG = StartEffectGroup("Teleport Start"		//Hardcording
					, pTM
					,_pTimer->GetLerpedCurrentTick());
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( _pNetwork->MyCharacterInfo.index, pEG);
	m_mapEG.insert(tmpVal);	
	
	m_bPrePortal = TRUE;
	m_nPortal = nPotal;

}

void CUIGuildWarPortal::SetDratanPortalFlag(int npcIndex )
{
	if( npcIndex >= 382 && npcIndex <= 386)
	{
		m_btNpcAvailable |= 1<<(npcIndex-382);
	}
	else ASSERT("WRONG POTAL NPC INDEX!!");
}
