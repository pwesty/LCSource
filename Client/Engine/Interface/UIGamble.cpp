#include "stdh.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>

#define GAMBLE_NUMBER_WIDTH					10
#define GAMBLE_NUMBER_HEIGHT				15
#define	GAMBLE_TIME_SPAN					0.1 // 0.03 -> 0.1

// [071122 Su-won] NEW_MOONSTONE
INDEX MOONSTONEINDEX[MOONSTONE_COUNT]  = {2545, 2546, 723, 2547, 2548};

// ----------------------------------------------------------------------------
// Name : CUIGamble()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIGamble::CUIGamble()
{
	m_strAccumulate		= CTString("");
	m_iAccumulate		= 0;	
	m_pNpcEntity		= NULL;
	m_bSendedStart		= FALSE;
	m_bSelectedFixed	= FALSE;
	m_bIsCashMoonStone	= FALSE;
	m_bIsReady			= FALSE;
	m_bStart			= FALSE;
	m_selCashItemIdx	= -1;


	m_iUsedMoonStone =0;
	m_rtNewMark = NULL;
	m_iTexID = NULL;

#ifdef MOONSTONE_COUNT_ADD	// [2010/11/23 : Sora] 문스톤 개수 추가
	MOONSTONEINDEX[5]  = 6092;
#endif
}



// ----------------------------------------------------------------------------
// Name : ~CUIGamble()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGamble::~CUIGamble()
{
	if (m_iTexID)
	{
		delete [] m_iTexID;
		m_iTexID = NULL;
	}

	if (m_rtNewMark)
	{
		delete [] m_rtNewMark;
		m_rtNewMark = NULL;
	}

	for(int i = 0; i < 5; ++i)
		SAFE_DELETE(m_pIconsMoonStone[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	int		i;

	m_rcMainTitle.SetRect( 0, 0, GAMBLE_MAIN_WIDTH, 150 );
	m_rcSlot[0].SetRect( 7, 103, 52, 148 );
	m_rcSlot[1].SetRect( 56, 103, 101, 148 );
	m_rcSlot[2].SetRect( 105, 103, 150, 148 );
	m_rcSlot[3].SetRect( 155, 103, 200, 148 );
	m_rcSlot[4].SetRect( 204, 103, 249, 148 );	

	// Create shop texture
	// russia texture. [9/7/2010 rumist]
#if defined (G_RUSSIA)
		m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MoonStone_rus.tex" ) );
#else
		m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MoonStone.tex" ) );
#endif
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	// [071122: Su-won] NEW_MOONSTONE
	m_rtBackTop.SetUV( 0, 0, GAMBLE_MAIN_WIDTH, 148, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 149, GAMBLE_MAIN_WIDTH, 166, fTexWidth, fTexHeight );		// 
	m_rtBackBottom.SetUV( 0, 167, GAMBLE_MAIN_WIDTH, GAMBLE_MAIN_HEIGHT, fTexWidth, fTexHeight );

	// [071122: Su-won] NEW_MOONSTONE
	m_btnStop.Create( this, CTString(""), 8, 167+26, 113, 29 );
	m_btnStop.SetUV( UBS_IDLE, 0, 404, 112, 432, fTexWidth, fTexHeight );
	m_btnStop.SetUV( UBS_CLICK, 114, 404, 226, 432, fTexWidth, fTexHeight );
	m_btnStop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnStop.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	// [071122: Su-won] NEW_MOONSTONE
	m_btnCancel.Create( this, CTString(""), 135, 167+26, 113, 29 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 434, 112, 462, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 114, 434, 226, 462, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	for( i = 0 ; i < GAMBLE_TOTAL_NUMBER; ++i )
	{
		m_rtSmallNumber[i].SetUV( i * GAMBLE_NUMBER_WIDTH, 358, 
								i * GAMBLE_NUMBER_WIDTH + GAMBLE_NUMBER_WIDTH, 368, fTexWidth, fTexHeight );
	}

	m_rtSelect.SetUV( 92, 262, 137, 307, fTexWidth, fTexHeight );	

// [071122: Su-won] NEW_MOONSTONE
	for( i = 0; i < 5; ++i)
	{
		m_pIconsMoonStone[i] = new CUIIcon;
		m_pIconsMoonStone[i]->Create(this, 17 + i * 49, 158, 25, 25, UI_GAMBLE, UBET_ITEM);
	}

	ReadMoonStoneLOD();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenGamble()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::OpenGamble( CEntity *pNpcEntity , BOOL bIsCashMoonStone)
{
	ASSERT( pNpcEntity != NULL && "Invalid Npc Entity!" );
	if( IsVisible() || !pNpcEntity )
		return;

	Init();

	// Set position of target npc
	m_pNpcEntity	= pNpcEntity;

	// Set cash moonstone
	m_bIsCashMoonStone = bIsCashMoonStone;

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	if(!m_bIsCashMoonStone)
	{
		m_rtMark[0].SetUV( 0, 216, 45, 261, fTexWidth, fTexHeight );
		m_rtMark[1].SetUV( 46, 216, 91, 261, fTexWidth, fTexHeight );
		m_rtMark[2].SetUV( 92, 216, 137, 261, fTexWidth, fTexHeight );
		m_rtMark[3].SetUV( 138, 216, 183, 261, fTexWidth, fTexHeight );
		m_rtMark[4].SetUV( 184, 216, 229, 261, fTexWidth, fTexHeight );
		m_rtMark[5].SetUV( 0, 262, 45, 307, fTexWidth, fTexHeight );
		m_rtMark[6].SetUV( 46, 262, 91, 307, fTexWidth, fTexHeight );
		_pNetwork->SendMoonStoneStartReq();
	}
	else 
	{
		//Reset Slot UV
		m_rtMark[0].SetUV( 138, 262, 183, 307, fTexWidth, fTexHeight );
		m_rtMark[1].SetUV( 184, 262, 229, 307, fTexWidth, fTexHeight );
		m_rtMark[2].SetUV( 0, 308, 45, 352, fTexWidth, fTexHeight );
		m_rtMark[3].SetUV( 46, 308, 91, 352, fTexWidth, fTexHeight );
		m_rtMark[4].SetUV( 92, 308, 137, 352, fTexWidth, fTexHeight );
		m_rtMark[5].SetUV( 138, 308, 183, 352, fTexWidth, fTexHeight );
		m_rtMark[6].SetUV( 184, 308, 229, 352, fTexWidth, fTexHeight );

		CUIManager* pUIManager = CUIManager::getSingleton();

		pUIManager->GetGamble()->SetAccumulate( 0 );
		pUIManager->GetGamble()->ShowGamble();
		pUIManager->SetCSFlagOn( CSF_MOONSTONE );
	}

// [071122: Su-won] NEW_MOONSTONE
	for(int i=0; i<5; ++i)
	{
		m_pIconsMoonStone[i]->setData(UBET_ITEM, MOONSTONEINDEX[i]);
		
		CItemData*	pItemData = _pNetwork->GetItemData( MOONSTONEINDEX[2] );

		int nTextureID = pItemData->GetIconTexID();
		int	nTexRow = pItemData->GetIconTexRow();
		int	nTexCol = pItemData->GetIconTexCol();
		int nUVSX = BTN_SIZE *nTexCol;
		int nUVSY = BTN_SIZE *nTexRow;

		FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, nTextureID );
		FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, nTextureID );
		
		m_rtNewMark[i].SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

		m_iTexID[i] =nTextureID;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowGamble()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::ShowGamble()
{	
	CUIManager::getSingleton()->RearrangeOrder( UI_GAMBLE, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Init()
{
	for( int i = 0; i < GAMBLE_TOTAL_SLOT; ++i )
	{
		m_dSlotOld[i]			= 0;
		m_dSlotElapsed[i]		= 0;
		m_iSelectedMarker[i]	= -1;
		m_iCurMarker[i]			= i;	
		m_iStoppedMarker[i]		= FALSE;
	}
	memset( &m_iMarkerScore[0], 0, sizeof(int) * GAMBLE_TOTAL_MARK );	
	
	m_iStopSlot				= 0;
	m_iMaxMarker			= -1;
	Stop();	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::ResetGamble()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(m_bIsCashMoonStone)
	{
		pUIManager->CloseMessageBox(MSGCMD_GAMBLE_READY);
		pUIManager->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);
		pUIManager->RearrangeOrder( UI_INVENTORY, FALSE );	
	}

	if( pUIManager->IsCSFlagOn( CSF_MOONSTONE ) )
	{
		pUIManager->SetCSFlagOff( CSF_MOONSTONE );
	}

	m_pNpcEntity			= NULL;
	m_bSendedStart			= FALSE;
	m_bIsReady				= FALSE;
	m_bStart				= FALSE;
	m_bIsCashMoonStone		= FALSE;
	pUIManager->RearrangeOrder( UI_GAMBLE, FALSE );
	m_cashItemList.clear();
	m_selCashItemIdx	= -1;

	for( int i = 0; i < GAMBLE_TOTAL_SLOT; ++i)
	{
		if (m_pIconsMoonStone[i] == NULL)
			continue;

		m_pIconsMoonStone[i]->CloseProc();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Start()
{	
	m_bStart		= TRUE;	
	m_bSendedStart	= FALSE;

// [071122: Su-won] NEW_MOONSTONE
	for(int i=0; i<m_saReward[m_iUsedMoonStone].Count(); ++i)
	{
		INDEX iItem =m_saReward[m_iUsedMoonStone][i];
		CItemData*	pItemData = _pNetwork->GetItemData( iItem );

		int nTextureID = pItemData->GetIconTexID();
		int	nTexRow = pItemData->GetIconTexRow();
		int	nTexCol = pItemData->GetIconTexCol();
		int nUVSX = BTN_SIZE *nTexCol;
		int nUVSY = BTN_SIZE *nTexRow;

		FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, nTextureID );
		FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, nTextureID );
		
		m_rtNewMark[i].SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

		m_iTexID[i] =nTextureID;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Stop()
{
	m_bStart	= FALSE;
	m_bBegin	= FALSE;
	CUIManager::getSingleton()->ResetHoldBtn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::SetSelectedMarker( int iSlot, int iCount )
{
	const int iSelectedNumber	= iSlot;

	for( int t = 0; t < iCount; ++t )
	{
		m_iSelectedMarker[t]	= iSlot;
	}

	m_bSelectedFixed	= (iCount == 1) ? TRUE : FALSE;

	if( iCount != GAMBLE_TOTAL_SLOT )
	{		
		// 나머지 슬롯을 결정함.
		for( int i = iCount; i < GAMBLE_TOTAL_SLOT ; ++i )
		{
			int iNewNumber	= 0;
			do
			{
				iNewNumber = rand() % GAMBLE_TOTAL_MARK;
				
				// 랜덤으로 돌려야 되는 슬롯이 2개 이상있으며...
				if( GAMBLE_TOTAL_SLOT - iCount > 1 )
				{
					// 얻은 랜덤값과 같은 슬롯이 있는지 확인함...
					for( int j = iCount; j < i; ++j )
					{
						if( m_iSelectedMarker[j] == iNewNumber )
						{
							// while문에서 걸리도록 하기 위해서...
							iNewNumber	= iSelectedNumber;
							break;
						}
					}
				}
			}
			while( iSelectedNumber == iNewNumber );
			m_iSelectedMarker[i] = iNewNumber;
		}

		// 뒤섞기~!!!
		// NOTE : 1개일때는 제외!!!
		// NOTE : 1개일 때는 무조건 첫번째값이 표시가 되어야 하기 때문에...
		// NOTE : 4개의 랜덤값을 얻은 이후, 2개씩 교환을 합니다.
		if( iCount != 1 )
		{
			int iRand[4];
			for( int i = 0; i < 4; ++i )
			{
				iRand[i] = rand() % GAMBLE_TOTAL_SLOT;
			}
			
			// Swap
			{
				int iTemp = m_iSelectedMarker[iRand[0]];
				m_iSelectedMarker[iRand[0]] = m_iSelectedMarker[iRand[1]];
				m_iSelectedMarker[iRand[1]]	= iTemp;
			}
			
			// Swap
			{
				int iTemp = m_iSelectedMarker[iRand[2]];
				m_iSelectedMarker[iRand[2]] = m_iSelectedMarker[iRand[3]];
				m_iSelectedMarker[iRand[3]]	= iTemp;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 누적수치를 셋팅합니다.
//-----------------------------------------------------------------------------
void CUIGamble::SetAccumulate(int iAccumulate)
{
	m_iAccumulate		= iAccumulate;
	m_strAccumulate.PrintF( "%d", m_iAccumulate );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::ShowPrizeEffect()
{
	ASSERT( m_pNpcEntity != NULL && "Invalid Npc Entity!" );
	if( m_pNpcEntity )
	{
		// 이펙트 표시...
		StartEffectGroup("npcGamblingWin", &m_pNpcEntity->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

// ----------------------------------------------------------------------------
// Name : DrawNumber()
// Desc : 오른쪽 정렬 형태로 출력...
// ----------------------------------------------------------------------------
void CUIGamble::DrawNumber( int x, int y, CTString &strNumber )
{
	// Get length of string
	INDEX	nLength = strNumber.Length();
	if( nLength == 0 )
		return;

	int iStartPos = x - GAMBLE_NUMBER_WIDTH * nLength;

	// Check line character
	for( int iPos = 0; iPos < nLength; iPos++ )
	{
		CTString strTemp;
		strTemp.PrintF("%c", strNumber[iPos] );
		int iNumber = atoi( strTemp );
		DrawNumber( iStartPos + GAMBLE_NUMBER_WIDTH * iPos, y, iNumber );
	}
}

// ----------------------------------------------------------------------------
// Name : DrawNumber()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::DrawNumber( int x, int y, int nNumber )
{
	ASSERT( nNumber >= 0 && nNumber <= 9 );
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= GAMBLE_NUMBER_WIDTH;
	int nHeight			= GAMBLE_NUMBER_HEIGHT;
	
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + nWidth, y + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::Render()
{
	// moonstone ui auto closing [8/31/2010 rumist]
	FLOAT3D vNPCPos = m_pNpcEntity->GetPlacement().pl_PositionVector;
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - vNPCPos(1);
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - vNPCPos(3);
	
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{	
		ResetGamble();
		return;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nX, nY, nX2, nY2;

	nX		= m_nPosX;
	nX2		= nX + m_nWidth;

// [071122: Su-won] NEW_MOONSTONE
	nY		= m_nPosY;	
	nY2		= nY + 148;

	pDrawPort->AddTexture( nX, nY, nX2, nY2,
		m_rtBackTop.U0, m_rtBackTop.V0,
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY		= nY2;
	nY2		= nY +45;

	pDrawPort->AddTexture( nX, nY, nX2, nY2,
		m_rtBackMiddle.U0, m_rtBackMiddle.V0,
		m_rtBackMiddle.U1, m_rtBackMiddle.V1,
		0xFFFFFFFF );

	//nY		= nY2;
	//nY2		= nY +19;

	pDrawPort->AddTexture( nX, nY, nX2, nY2,
		m_rtBackMiddle.U0, m_rtBackMiddle.V0,
		m_rtBackMiddle.U1, m_rtBackMiddle.V1,
		0xFFFFFFFF );

	nY		= nY2;

	pDrawPort->AddTexture( nX, nY, nX2, m_nPosY + m_nHeight+19,
		m_rtBackBottom.U0, m_rtBackBottom.V0,
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );
	
	m_btnStop.Render();
	
	// Cancel button of shop
	m_btnCancel.Render();	

	int		i;
	for (i = 0; i < GAMBLE_TOTAL_SLOT; ++i)
	{
		if( m_bStart )
		{		
			DOUBLE	dCurTime	= _pTimer->GetHighPrecisionTimer().GetSeconds();
			m_dSlotElapsed[i]	+= dCurTime - m_dSlotOld[i];
			m_dSlotOld[i]		= dCurTime;
			
			if( m_dSlotElapsed[i] > GAMBLE_TIME_SPAN && !m_iStoppedMarker[i] )
			{
				m_iCurMarker[i]++;

				// [071122: Su-won] NEW_MOONSTONE
				m_iCurMarker[i] = m_iCurMarker[i] % m_saReward[m_iUsedMoonStone].Count();
				do
				{
					m_dSlotElapsed[i] -= GAMBLE_TIME_SPAN;
				}
				while( m_dSlotElapsed[i] > GAMBLE_TIME_SPAN );

				if( i == m_iStopSlot )
				{
					if( m_bBegin && m_iCurMarker[i] == m_iSelectedMarker[i] )
					{
						m_iStoppedMarker[i]	= TRUE;

						// 마지막 슬롯까지 다 정지했다면...
						if( m_iStopSlot == GAMBLE_TOTAL_SLOT - 1 )
						{
							Stop();
							if(!m_bIsCashMoonStone)
								_pNetwork->SendMoonStoneResultReq();
							else{
								_pNetwork->SendCashMoonStoneReward();
							}
												
							int iMaxScore	= 0;							
							for( int t = 0; t < GAMBLE_TOTAL_MARK; ++t )
							{
								if( m_iMarkerScore[t] > iMaxScore )
								{									
									iMaxScore		= m_iMarkerScore[t];
									m_iMaxMarker	= t;
								}
								else if( m_iMarkerScore[t] == iMaxScore )
								{
									if( m_iMaxMarker <= t )
									{
										iMaxScore		= m_iMarkerScore[t];
										m_iMaxMarker	= t;
									}
								}
							}
						}

						m_iStopSlot++;
					}
				}
			}
		}
		RenderSlot( i, m_iCurMarker[i] );

// [071122: Su-won] NEW_MOONSTONE
#ifndef NEW_MOONSTONE
		RenderSelectedSlot();
#endif

	}
	
	if(m_bIsCashMoonStone)
	{
		CTString tv_str;
		if(m_bIsReady)
			tv_str=CTString("Press Start Button!");
		else
			tv_str=CTString("Insert Item...");
		pDrawPort->PutTextExCX(tv_str,m_nPosX+129,m_nPosY+80);

	}
	else
	{
		DrawNumber( m_nPosX + 190, m_nPosY + 80, m_strAccumulate );
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();


	for (i = 0; i < 5; ++i)
		m_pIconsMoonStone[i]->Render(pDrawPort);

	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM, PBT_OPAQUE );

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	RenderSelectedSlot();
	pDrawPort->FlushRenderingQueue();

	// Render Text
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::RenderSlot( int iSlot, int iMarker )
{
	if( iSlot == -1 )
		return;

	if( iMarker == -1 )
		iMarker = 0;

	int iCurSlot	= iSlot % GAMBLE_TOTAL_SLOT;
	int iCurMarker	= iMarker % GAMBLE_TOTAL_MARK;

// [071122: Su-won] NEW_MOONSTONE
	iCurMarker	= iMarker % m_saReward[m_iUsedMoonStone].Count();

	CUIManager::getSingleton()->GetDrawPort()->AddBtnTexture( m_iTexID[iCurMarker],
								m_nPosX + m_rcSlot[iCurSlot].Left, m_nPosY + m_rcSlot[iCurSlot].Top, m_nPosX + m_rcSlot[iCurSlot].Right, m_nPosY + m_rcSlot[iCurSlot].Bottom, 
								m_rtNewMark[iCurMarker].U0, m_rtNewMark[iCurMarker].V0, m_rtNewMark[iCurMarker].U1, m_rtNewMark[iCurMarker].V1, 
								0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : RenderSelectedSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::RenderSelectedSlot( )
{
	if( m_iMaxMarker == -1 )
		return;

	// [071122: Su-won] NEW_MOONSTONE
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( 
			m_nPosX + m_rcSlot[2].Left, m_nPosY + m_rcSlot[2].Top, m_nPosX + m_rcSlot[2].Right, m_nPosY + m_rcSlot[2].Bottom, 
			m_rtSelect.U0, m_rtSelect.V0, m_rtSelect.U1, m_rtSelect.V1, 
			0xFFFFFFFF );	
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIGamble::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

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
			
			// Sell button
			else if( m_bStart && m_btnStop.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( !m_bStart && m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			// [071122: Su-won] NEW_MOONSTONE
			for (int i = 0; i < 5; ++i)
			{
				m_pIconsMoonStone[i]->MouseMessage( pMsg );
			}

			return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				
				// Title bar
				if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( m_bStart && m_btnStop.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( !m_bStart && m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				// [071122: Su-won] NEW_MOONSTONE
				for(int i=0; i<5; ++i)
				{
					if( !m_bStart && m_pIconsMoonStone[i]->MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}

				if(this->IsVisible())
					CUIManager::getSingleton()->RearrangeOrder( UI_GAMBLE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// If refine isn't focused
		 	if( !IsFocused() )
				return WMSG_FAIL;	
			
			if( !m_bStart && ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult != WMSG_FAIL )
					ResetGamble();
				
				return WMSG_SUCCESS;
			}
			else if( m_bStart && ( wmsgResult = m_btnStop.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					m_bBegin	= TRUE;
		
				}
				return WMSG_SUCCESS;
			}

			// [071122: Su-won] NEW_MOONSTONE
			for(int i=0; i<5; ++i)
			{
				if( !m_bStart && m_pIconsMoonStone[i]->MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if( !m_bSendedStart )
					{
						m_iUsedMoonStone =i;

						_pNetwork->SendMoonStoneTryReq();

						Init();
						m_bSendedStart	= TRUE;
					}
					return WMSG_SUCCESS;					
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{					
			}
		}
		break;
	}

	return WMSG_FAIL;
}


/////////////////////////////////////////////////////////////////////////
// [071122: Su-won] NEW_MOONSTONE
void CUIGamble::ReadMoonStoneLOD()
{
	FILE* fMoonStone = NULL;

	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString tPath = strFullPath + "data\\MoonStone.lod";

	INDEX iMax = 0;

	if( fMoonStone = fopen(tPath.str_String ,"rb") )
	{
		for( int i=0; i<MOONSTONE_COUNT; ++i)
		{
			int nCount;
			fread(&nCount,sizeof(int),1,fMoonStone);

			m_saReward[i].New(nCount);

			if (nCount > iMax) iMax = nCount; // 최대 갯수

			for(int j=0; j<nCount; ++j)
				fread(&m_saReward[i][j],sizeof(int),1,fMoonStone);
		}
	}

	ASSERT(iMax > 0 && "MoonStone Reward Count 0");

	if (iMax > 0)
	{
		m_iTexID = new INDEX[iMax];
		m_rtNewMark = new UIRectUV[iMax];
	}

	fclose(fMoonStone);
}

void CUIGamble::SetSelectedItem( INDEX iRewardItem )
{
	int iRewardCount =m_saReward[m_iUsedMoonStone].Count();

	UBYTE ubReward =0;

	for(int i=0; i<iRewardCount; ++i)
	{
		if( m_saReward[m_iUsedMoonStone][i] == iRewardItem )
		{
			ubReward =i;
			break;
		}
	}

	m_iSelectedMarker[0] = ubReward-2;
	m_iSelectedMarker[1] = ubReward-1;
	m_iSelectedMarker[2] = ubReward;
	m_iSelectedMarker[3] = ubReward+1;
	m_iSelectedMarker[4] = ubReward+2;

	if( m_iSelectedMarker[0] <0 )
		m_iSelectedMarker[0] += iRewardCount;

	if( m_iSelectedMarker[1] <0 )
		m_iSelectedMarker[1] += iRewardCount;

	if( m_iSelectedMarker[3] >=iRewardCount )
		m_iSelectedMarker[3] -= iRewardCount;

	if( m_iSelectedMarker[4] >=iRewardCount )
		m_iSelectedMarker[4] -= iRewardCount;
}

INDEX CUIGamble::GetUsedMoonStoneIndex()
{ 
#ifdef MOONSTONE_COUNT_ADD	// [2010/11/23 : Sora] 문스톤 개수 추가
	if( m_iUsedMoonStone == 2 ) //문스톤(723) 버튼에서 사용
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		// 문스톤(723)을 우선적으로 사용후 유료 문스톤(6092) 사용
		if( pUIManager->GetInventory()->GetItemCount( MOONSTONEINDEX[2] ) == 0 && pUIManager->GetInventory()->GetItemCount( MOONSTONEINDEX[5] ) > 0 )
			m_iUsedMoonStone = 5;
	}
#endif

	return MOONSTONEINDEX[m_iUsedMoonStone]; 
}

// [071122: Su-won] NEW_MOONSTONE
/////////////////////////////////////////////////////////////////////////