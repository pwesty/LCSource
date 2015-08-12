
#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISingleBattle.h>


// Define text position
#define	GB_TITLE_TEXT_OFFSETX		25		// 타이틀 바 스트링 
#define	GB_TITLE_TEXT_OFFSETY		5

#define GB_TITLE_HEIGHT				26		// 타이틀 바 높이
#define START_BOTTOM_BUTTON_Y		(GB_HEIGHT - 29)	// 버튼의 시작 높이

#define SMALL_NUMBER_WIDTH			16
#define SMALL_NUMBER_HEIGHT			20
#define LARGE_NUMBER_WIDTH			21
#define LARGE_NUMBER_HEIGHT			27

#define LEFT_TIME_X					922
#define LEFT_TIME_Y					198

#define RIGHT_SPACE					12
#define BOX_WIDTH					118
#define BOX_HEIGHT					61

//#define BLUE_BOX_Y					241
//#define RED_BOX_Y					308
#define BLUE_BOX_Y					308
#define RED_BOX_Y					241

#define SINGLE_NAME_Y				247
#define SINGLE_NAME2_Y				310

#define BATTLE_ZONE_NAME_Y			LEFT_TIME_Y + SMALL_NUMBER_HEIGHT + 2		

#define BOX_Y						195
#define BASE_BOX_HEIHGT				42

//#define RED_KILL_POINT_Y			332
//#define BLUE_KILL_POINT_Y			268
#define RED_KILL_POINT_Y			268
#define BLUE_KILL_POINT_Y			332

#define COLON_WIDTH					9


//------------------------------------------------------------------------------
// CUISingleBattle::CUISingleBattle()
// Explain:  
//------------------------------------------------------------------------------
CUISingleBattle::CUISingleBattle()
{	
}


//------------------------------------------------------------------------------
// CUISingleBattle::~CUISingleBattle()
// Explain:  
//------------------------------------------------------------------------------
CUISingleBattle::~CUISingleBattle()
{
	Clear ();
}

//------------------------------------------------------------------------------
// CUISingleBattle::Clear
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::Clear()
{	
}

//------------------------------------------------------------------------------
// CUISingleBattle::Create
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	Clear();
	m_strTitle = _S( 1097, "길드전투" ); 

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\GuildBattle.tex" ) );	
	
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();


	m_rcRedBox.SetRect( 0, RED_BOX_Y, 0, RED_BOX_Y + BOX_HEIGHT );		
	m_rcBlueBox.SetRect( 0, BLUE_BOX_Y, 0, BLUE_BOX_Y + BOX_HEIGHT );		
	m_rcBox.SetRect( 0, BOX_Y, 0, BOX_Y + BASE_BOX_HEIHGT );	

	m_rtRedBox.SetUV( 0, 52, 118, 113, fTexWidth, fTexHeight );				// UV of background		
	m_rtBlueBox.SetUV( 119, 52, 237, 113, fTexWidth, fTexHeight );			// UV of background		
	m_rtBox.SetUV( 8, 57, 108, 72, fTexWidth, fTexHeight );					// UV of background		
	
	for( int i = 0 ; i < 10; i++ )
	{
		m_rtSmallNumber[i].SetUV( i*SMALL_NUMBER_WIDTH, 0, 
								i*SMALL_NUMBER_WIDTH+SMALL_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );

		m_rtLargeNumber[i].SetUV( i*LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, 
								i*LARGE_NUMBER_WIDTH+LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT + LARGE_NUMBER_HEIGHT, fTexWidth, fTexHeight );					// UV of background		
	}



	m_rtColon.SetUV ( 164, 0, 164 + COLON_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );
}


//------------------------------------------------------------------------------
// CUISingleBattle::Close
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::Close()
{
	if( IsVisible() == FALSE )
		return;

	CUIManager::getSingleton()->RearrangeOrder( UI_SINGLE_BATTLE, FALSE );
	Clear();
}

//------------------------------------------------------------------------------
// CUISingleBattle::SetFocus
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

//------------------------------------------------------------------------------
// CUISingleBattle::Render
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::Render()
{
	RenderGBStatus();
}

//------------------------------------------------------------------------------
// CUISingleBattle::RenderGBStatus
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::RenderGBStatus()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int nX;

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// 남은 시간 표시 ( ImageFont );

	int nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH;

	m_rcBox.Left = m_rcBlueBox.Left = m_rcRedBox.Left = nOffsetX;
	m_rcBox.Right = m_rcBlueBox.Right = m_rcRedBox.Right = nOffsetX + BOX_WIDTH;

	pDrawPort->AddTexture( m_rcRedBox.Left, m_rcRedBox.Top, m_rcRedBox.Right, m_rcRedBox.Bottom,
									m_rtRedBox.U0, m_rtRedBox.V0,
									m_rtRedBox.U1, m_rtRedBox.V1,
									0xFFFFFFFF );

	// Kill Point 표시 ( ImageFont : Large);
	RenderKillPoint();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	//CTString strMessage;	
	//strMessage.PrintF ( _S( 1099, "전투지역 : %s" ), ZoneInfo().GetZoneName( m_nBattleZone ) ); 	
	
	nX = m_rcRedBox.Left + ( m_rcRedBox.Right - m_rcRedBox.Left ) / 2;
	
	pDrawPort->PutTextExCX( _S( 1740, "죽인 적의 수" ), nX, SINGLE_NAME_Y, 0xA6C0FFE5 );		

	// Flush all render text queue
	pDrawPort->EndTextEx();
}


//------------------------------------------------------------------------------
// CUISingleBattle::DrawNumber 
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::DrawNumber ( int x, int y, int nNumber, bool bLarge )
{	
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= SMALL_NUMBER_WIDTH;
	int nHeight			= SMALL_NUMBER_HEIGHT;


	if ( bLarge )
	{
		rtNumber	= m_rtLargeNumber[nNumber];
		nWidth		= LARGE_NUMBER_WIDTH;	
		nHeight		= LARGE_NUMBER_HEIGHT;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->AddTexture( x, y, x + nWidth, y + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );

}

//------------------------------------------------------------------------------
// CUISingleBattle::RenderKillPoint
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::RenderKillPoint()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int nOffsetX			= pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;	
	int nKillPoint			= _pNetwork->wo_dwKilledEnemyCount;
	int nTargetKillPoint	= _pNetwork->wo_dwKilledEnemyCount;
	int nHalfWidth			= LARGE_NUMBER_WIDTH / 2 ;

	int i = 0;
	int nRenderNumber = 0;

	// Red Kill Point
	nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;

	for( i = 1;  ; i *= 10 )
	{
		if ( nTargetKillPoint / i > 0 )
		{
			nOffsetX -= nHalfWidth;		
		}
		else
		{
			break;
		}
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nOffsetX -= nHalfWidth;	
	}
	
	while( i )
	{		
		nRenderNumber = nTargetKillPoint / i;
		nTargetKillPoint %= i;

		DrawNumber ( nOffsetX, RED_KILL_POINT_Y, nRenderNumber, TRUE);

		printf ( "%d", nRenderNumber );
		nOffsetX += LARGE_NUMBER_WIDTH;	
		i /= 10;
	}
}

//------------------------------------------------------------------------------
// CUISingleBattle::OpenSingleBattle
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::OpenSingleBattle()
{
	if( IsVisible() == TRUE )
		return;

	CUIManager::getSingleton()->RearrangeOrder( UI_SINGLE_BATTLE, TRUE );
}


//------------------------------------------------------------------------------
// CUISingleBattle::DrawColon
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::DrawColon( int x, int y )
{
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + COLON_WIDTH, y + SMALL_NUMBER_HEIGHT,
							m_rtColon.U0, m_rtColon.V0, m_rtColon.U1, m_rtColon.V1,
							0xFFFFFFFF );	
}

//------------------------------------------------------------------------------
// CUISingleBattle::ResetPosition
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


//------------------------------------------------------------------------------
// CUISingleBattle::AdjustPosition
// Explain:  
//------------------------------------------------------------------------------
void CUISingleBattle::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


//------------------------------------------------------------------------------
// CUISingleBattle::MouseMessage
// Explain:  
//------------------------------------------------------------------------------
WMSG_RESULT	CUISingleBattle::MouseMessage( MSG *pMsg )
{
	return WMSG_FAIL;
}