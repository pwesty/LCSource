#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIPetItemMix.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Interface/UIInventory.h>

enum eChangeSelection
{
	DRAGON_WEAPON = 2,						// 드래곤 무기
	KNIGHT_AMOR,						// 나이트 방어
};

#define DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT 3
#define DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT 3

static int _iMaxMsgStringChar = 0;

// -----------------------------------------------------------------------------
// Name : CUIPetItemMix()
// Desc : Constructor
// -----------------------------------------------------------------------------
CUIPetItemMix::CUIPetItemMix()
{
	Clear();
}

// -----------------------------------------------------------------------------
// Name : ~CUIPetItemMix()
// Desc : Destructor
// -----------------------------------------------------------------------------
CUIPetItemMix::~CUIPetItemMix()
{
	Destroy();
	ClearProcessItems();
}

// ------------------------------------------------------------------------------
// Name : Create()
// Desc :
// ------------------------------------------------------------------------------
void CUIPetItemMix::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PROCESS_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = PROCESS_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );

	m_rcItem.SetRect( 13, 33, 218, 203 );
	m_rcDesc.SetRect( 13, 203, 218, 331 );

	// Process Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PROCESS_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PROCESS_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Process button A( KNGIHT )
	m_btnOK[1].Create( this, _S( 2457, "제작 A" ), 12, 372-diff, 54, 21 );			
	m_btnOK[1].SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK[1].SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK[1].CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK[1].CopyUV( UBS_IDLE, UBS_DISABLE );

	// Process button B( KNGIHT )
	m_btnOK[2].Create( this, _S( 2458, "제작 B" ), 76, 372-diff, 54, 21 );			
	m_btnOK[2].SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK[2].SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK[2].CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK[2].CopyUV( UBS_IDLE, UBS_DISABLE );

	// Process button DRAGON
	m_btnOK[0].Create( this, _S( 2459, "제작" ), 76, 372-diff, 54, 21 );			
	m_btnOK[0].SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK[0].SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK[0].CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK[0].CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 149, 372-diff, 54, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of Process Item
	m_sbProcessItem.Create( this, 193, 51, 9, 153 );
	m_sbProcessItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetScrollRange( PROCESS_SLOT_ROW_TOTAL );
	m_sbProcessItem.SetCurItemCount( 0 );
	m_sbProcessItem.SetItemsPerPage( PROCESS_SLOT_ROW );	
	// Up button
	m_sbProcessItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProcessItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProcessItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProcessItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbPreconditionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbPreconditionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbPreconditionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPreconditionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPreconditionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_bSatisfiedA = FALSE;
	m_bSatisfiedB = FALSE;

	m_nPetType = -1;
	m_bDetailItemInfo = TRUE;
}

// ------------------------------------------------------------------------------
// Name : ResetPosition()
// Desc : 
// ------------------------------------------------------------------------------
void CUIPetItemMix::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ------------------------------------------------------------------------------
// Name : AdjustPosition
// Desc :
// ------------------------------------------------------------------------------
void CUIPetItemMix::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetItemMix::Clear()
{
	m_nSelectProcessItem	= -1;
	m_bWaitProcessResult	= TRUE;
	m_bSatisfiedA			= FALSE;
	m_bSatisfiedB			= FALSE;

//	m_nProcessText			= -1;				
	m_nRow					= -1;
	m_nCol					= -1;
	
	m_nNeedItemCount		= 0;

	m_nPetType = -1;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	for ( int i=0; i<T_KNIGHT_MAX; i++ )
		memset ( &m_NStuffKnight[i], 0, sizeof(CNeedStuff) );

	m_nJobIndex[TITAN] = 918; // 타이탄
	m_nJobIndex[KNIGHT] = 923; // 기사
	m_nJobIndex[HEALER] = 938; // 힐러
	m_nJobIndex[MAGE] = 928; // 메이지
	m_nJobIndex[ROGUE] = 933; // 로그
	m_nJobIndex[SORCERER] = 943; // 소서러
#ifdef CHAR_EX_ROGUE
	m_nJobIndex[EX_ROGUE] = 933; // [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_nJobIndex[EX_MAGE] = 928; // 2013/01/08 jeil EX메이지 추가 
#endif

	m_bWaitProcessResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();

	ClearProcessItems();

	InitNeedItem();
}

// ----------------------------------------------------------------------------
// Name : InitNeedItem()
// Desc : 나이트 메어 아이템 재료 인덱스 저장
// ----------------------------------------------------------------------------
void CUIPetItemMix::InitNeedItem( void )
{
	m_NStuffKnight[T_HELMET].nNeedA[0] = 886; // 25%	투구
	m_NStuffKnight[T_HELMET].nNeedA[1] = 889;
	m_NStuffKnight[T_HELMET].nNeedA[2] = 894;
	m_NStuffKnight[T_HELMET].nNeedB[0] = 888; // 75%	
	m_NStuffKnight[T_HELMET].nNeedB[1] = 891;
	m_NStuffKnight[T_HELMET].nNeedB[2] = 894;

	m_NStuffKnight[T_COAT].nNeedA[0] = 891; // 25%		상의
	m_NStuffKnight[T_COAT].nNeedA[1] = 892;
	m_NStuffKnight[T_COAT].nNeedA[2] = 894;
	m_NStuffKnight[T_COAT].nNeedB[0] = 893; // 75%
	m_NStuffKnight[T_COAT].nNeedB[1] = 889;
	m_NStuffKnight[T_COAT].nNeedB[2] = 894;

	m_NStuffKnight[T_PANTS].nNeedA[0] = 886; // 25%		하의
	m_NStuffKnight[T_PANTS].nNeedA[1] = 891;
	m_NStuffKnight[T_PANTS].nNeedA[2] = 894;
	m_NStuffKnight[T_PANTS].nNeedB[0] = 890; // 75%
	m_NStuffKnight[T_PANTS].nNeedB[1] = 892;
	m_NStuffKnight[T_PANTS].nNeedB[2] = 894;

	m_NStuffKnight[T_BOOTS].nNeedA[0] = 889; // 25%		부츠
	m_NStuffKnight[T_BOOTS].nNeedA[1] = 892;
	m_NStuffKnight[T_BOOTS].nNeedA[2] = 894;
	m_NStuffKnight[T_BOOTS].nNeedB[0] = 890; // 75%
	m_NStuffKnight[T_BOOTS].nNeedB[1] = 886;
	m_NStuffKnight[T_BOOTS].nNeedB[2] = 894;

	m_NStuffKnight[T_GLOVE].nNeedA[0] = 889; // 25%		장갑
	m_NStuffKnight[T_GLOVE].nNeedA[1] = 892;
	m_NStuffKnight[T_GLOVE].nNeedA[2] = 894;
	m_NStuffKnight[T_GLOVE].nNeedB[0] = 888; // 75%
	m_NStuffKnight[T_GLOVE].nNeedB[1] = 886;
	m_NStuffKnight[T_GLOVE].nNeedB[2] = 894;

	m_NStuffKnight[T_SHIELD].nNeedA[0] = 886; // 25%	방패
	m_NStuffKnight[T_SHIELD].nNeedA[1] = 892;
	m_NStuffKnight[T_SHIELD].nNeedA[2] = 894;
	m_NStuffKnight[T_SHIELD].nNeedB[0] = 893; // 75%
	m_NStuffKnight[T_SHIELD].nNeedB[1] = 891;
	m_NStuffKnight[T_SHIELD].nNeedB[2] = 894;
}

// ----------------------------------------------------------------------------
// Name : InitPetItemMix()
// Desc : 아이템 정보 갱신
// ----------------------------------------------------------------------------
void CUIPetItemMix::InitPetItemMix( int nType )
{	
	ClearProcessItems();

	CUIIcon* pIcon = NULL;
	int bit = 0x00000001;

	if( DRAGON_WEAPON == nType )
	{
		for( int i=0; i<T_DRAGON_MAX; i++ )
		{
			if( i == 0 )
			{
				CItemData* pItemData = _pNetwork->GetItemData( UNI_DRAGON_SWORD );
				if( pItemData->GetItemIndex() == -1 ) 
					continue;

				pIcon = new CUIIcon;
				pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETITEMMIX, UBET_ITEM);
				pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
				m_vecIcons.push_back ( pIcon );
			}
			else
			{
				CItemData* pItemData = _pNetwork->GetItemData( UNI_DUAL_DRAGON_SWORD + i-1 );

				if( pItemData->GetItemIndex() == -1 ) 
					continue;

				pIcon = new CUIIcon;
				pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETITEMMIX, UBET_ITEM);
				pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());					
				m_vecIcons.push_back ( pIcon );
			}
		}
	}
	else if( KNIGHT_AMOR == nType )
	{	
		UBYTE ubJob = _pNetwork->MyCharacterInfo.job;

		for( int i=0; i<T_SHIELD; i++ )
		{
			CItemData* pItemData = _pNetwork->GetItemData( m_nJobIndex[ubJob]+i );

			if( pItemData->GetItemIndex() == -1 )
				continue;

			pIcon = new CUIIcon;
			pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETITEMMIX, UBET_ITEM);
			pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
			m_vecIcons.push_back( pIcon );
		}

		if( ubJob == KNIGHT )
		{
			CItemData* pItemData = _pNetwork->GetItemData( UNI_KNIGHT_DEFENDSHIELD );
			
			if( pItemData->GetItemIndex() != -1 )
			{
				pIcon = new CUIIcon;
				pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETITEMMIX, UBET_ITEM);
				pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
				m_vecIcons.push_back( pIcon );
			}
		}
	}

	AddString ( _S( 561, "가공품을 선택해 주십시오." ), COLOR_TEXT  );		

	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetCurItemCount( m_vecIcons.size()  );

	if( DRAGON_WEAPON == nType )
		m_btnOK[0].SetEnable ( m_bSatisfiedA );	
	else if( KNIGHT_AMOR == nType )
	{
		m_btnOK[1].SetEnable ( m_bSatisfiedA );
		m_btnOK[2].SetEnable ( m_bSatisfiedB );
	}
}

// ----------------------------------------------------------------------------
// Name : OpenPetItemMix()
// Desc : 조합 창을 연다.
// ----------------------------------------------------------------------------
void CUIPetItemMix::OpenPetItemMix(  int nType  )
{
	// If this is already exist
	if( IsVisible() || nType < DRAGON_WEAPON )	return;

	Clear ();
	m_nPetType = nType;

	CUIManager::getSingleton()->RearrangeOrder( UI_PETITEMMIX, TRUE );
	
	InitPetItemMix ( nType );
}

// ----------------------------------------------------------------------------
// Name : ClosePetItemMix()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetItemMix::ClosePetItemMix()
{
	// Close refine
	CUIManager::getSingleton()->RearrangeOrder( UI_PETITEMMIX, FALSE );

	Clear();
}

// ------------------------------------------------------------------------------
// Name : Render
// Desc :
// ------------------------------------------------------------------------------
void CUIPetItemMix::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESS_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	pDrawPort->AddTexture( m_nPosX, m_nPosY+PROCESS_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	pDrawPort->PutTextEx( _S( 2459, "제작" ) , m_nPosX + PROCESS_TITLE_TEXT_OFFSETX,				
										m_nPosY + PROCESS_TITLE_TEXT_OFFSETY );

	// Render Process Type
	if( m_nPetType == DRAGON_WEAPON )
		pDrawPort->PutTextExCX( _S( 2460, "드래곤 웨폰 제작" ), m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	else if( m_nPetType == KNIGHT_AMOR )
		pDrawPort->PutTextExCX( _S( 2461, "나이트 메어 방어구 제작" ), m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );

	// Process Item Scroll bar
	m_sbProcessItem.Render();

	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// Process button ( A, B )
	if( m_nPetType == DRAGON_WEAPON )
		m_btnOK[0].Render();
	else if( m_nPetType == KNIGHT_AMOR )
	{
		m_btnOK[1].Render();
		m_btnOK[2].Render();
	}

	// Cancel button
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();


	// Render Process Item
	CTString m_strShortDesc;
	int	nX = SLEARN_SLOT_SX;
	int	nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbProcessItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_vecIcons.size() )
		iEnd = m_vecIcons.size();

	for ( int i = m_sbProcessItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_vecIcons[i]->IsEmpty() )
		{
			// render Item
			m_vecIcons[i]->SetPos ( nX, nY );
			m_vecIcons[i]->Render(pDrawPort);
			
			
			// render Item desc 
			CItemData* pItemData = _pNetwork->GetItemData ( m_vecIcons[i]->getIndex() );
			const char* szItemName = _pNetwork->GetItemName ( m_vecIcons[i]->getIndex() );
			
			pDrawPort->PutTextExCX( CTString ( szItemName ), m_nPosX + 122, m_nPosY + nY + 8, 0xC3C3C3ff );
												
			nY += SLEARN_SLOT_OFFSETY;
		}
	}
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	// render sel outline
	if ( m_nSelectProcessItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProcessItem - m_sbProcessItem.GetScrollPos();

		if ( nOffset >= 0 && nOffset < PROCESS_SLOT_ROW )
		{
			BoxX = m_nPosX + nX -1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) -1;

			
			pDrawPort->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
											m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xffffffff );
		}
	}
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetItemMix::MouseMessage( MSG *pMsg )
{
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
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	iRowS = m_sbProcessItem.GetScrollPos();
				int	iRowE = iRowS + PROCESS_SLOT_ROW;

				if ( m_vecIcons.size() < iRowE )
				{
					iRowE = m_vecIcons.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						m_vecIcons[iRow]->MouseMessage( pMsg );
					}
				}
			}
			
		
			// Move refine
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK[1].MouseMessage( pMsg ) != WMSG_FAIL )  // 제작 타입 A
				return WMSG_SUCCESS;
			else if( m_btnOK[2].MouseMessage( pMsg ) != WMSG_FAIL )  // 제작 타입 B
				return WMSG_SUCCESS;
			else if( m_btnOK[0].MouseMessage( pMsg ) != WMSG_FAIL && m_nPetType == DRAGON_WEAPON )  // DRAGON
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
				// List box of skill desc
			else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
	
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOK[0].MouseMessage( pMsg ) != WMSG_FAIL && m_nPetType == DRAGON_WEAPON )
				{
					// Nothing
				}
				else if( m_btnOK[1].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnOK[2].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PETITEMMIX, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if (UIMGR()->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetItemMix();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK[0].MouseMessage( pMsg ) ) != WMSG_FAIL &&
							m_nPetType == DRAGON_WEAPON )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendPetItemMixReq( 0, DRAGON_WEAPON );

					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnOK[1].MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendPetItemMixReq( 0, KNIGHT_AMOR );

					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnOK[2].MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendPetItemMixReq( 1, KNIGHT_AMOR );

					return WMSG_SUCCESS;
				}				
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetItemMix();

					return WMSG_SUCCESS;
				}
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProcessItem.GetScrollPos();
					int	iRowE = iRowS + PROCESS_SLOT_ROW;

					if ( m_vecIcons.size() < iRowE )
					{
						iRowE = m_vecIcons.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_vecIcons[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectProcessItem = iRow;

							SelectItem ( m_nSelectProcessItem );
							return WMSG_SUCCESS;
						}
					}
				}
			
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}

		break;
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{ 
					return m_sbProcessItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbPreconditionDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetItemMix::SelectItem ( int _nIndex )
{
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProcessItem;
		if ( _nIndex == -1 ) return;
	}
	int	 i;
	BOOL bNeedItemA			= FALSE;	
	BOOL bNeedItemB			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	BOOL bNeedItems[6];
	for( i = 0; i < 6; i++ )
		bNeedItems[i] = FALSE;
		
	m_lbPreconditionDesc.ResetAllStrings();
	
	// 생산 하고자 하는 아이템 목록
	if ( m_vecIcons[_nIndex]->getIndex() == -1 ) return;
	CItemData* pProcessItemData = _pNetwork-> GetItemData ( m_vecIcons[_nIndex]->getIndex() );

	// 필요 아이템 
	int nIndexTemp = 0;
	int NeedCntA[MAX_MAKE_ITEM_MATERIAL] = {0,};
	int NeedCntB[MAX_MAKE_ITEM_MATERIAL] = {0,};
	int nResultA = 0, nResultB = 0;
	CItemData* pInvenItem = NULL;
	
	int		nCnt;
	//필요 아이템 종류와 갯수 얻기 
	if( m_nPetType == DRAGON_WEAPON )
	{
		for( i = 0; i < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; i++ )
		{
			nIndexTemp = pProcessItemData->GetNeedItemIndex( i );

			if( nIndexTemp == -1 ) return;

			m_NeedItems[i].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
			m_NeedItems[i].llCount	= 1; 
		}

		int		t, i, each;

		for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
		{
			if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
			else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
			else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

			for (i = 0 ; i < each; ++i)
			{

				pInvenItem = _pNetwork->MySlotItem[t][i].ItemData;

				for( nCnt = 0; nCnt < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt].ItemData != NULL &&
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt].ItemData->GetItemIndex() )
					{
						NeedCntA[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt].MatTab = t;
						m_NeedItems[nCnt].inven_idx = i;
						bNeedItems[nCnt] = TRUE;
						// todo : 종료 조건 찾을 것
					}
				}
			}
		}

		
		for ( i = 0; i < DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT; i++ )
		{
			nResultA += NeedCntA[i];
		}

		if( nResultA == DEF_MAX_DRAGON_WEAPON_MATERIAL_COUNT )	bNeedItemA = TRUE;	// 조합 아이템 조건 만족

	}
	else if( m_nPetType == KNIGHT_AMOR )
	{
		int nJob = _pNetwork->MyCharacterInfo.job;

		for( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++ )
		{
			m_NeedItems[i].ItemData = _pNetwork->GetItemData( m_NStuffKnight[_nIndex].nNeedA[i] );
			m_NeedItems[i].llCount	= 1; 
		}
		for( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++ )
		{
			m_NeedItems[i+3].ItemData = _pNetwork->GetItemData( m_NStuffKnight[_nIndex].nNeedB[i] );
			m_NeedItems[i+3].llCount	= 1; 
		}

		int		t, i, each;

		for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
		{
			if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
			else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
			else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

			for (i = 0 ; i < each; ++i)
			{

				pInvenItem = _pNetwork->MySlotItem[t][i].ItemData;

				for( nCnt = 0; nCnt < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt].ItemData != NULL && 
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt].ItemData->GetItemIndex() )
					{
						NeedCntA[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt].MatTab = t;
						m_NeedItems[nCnt].inven_idx = i;
						bNeedItems[nCnt] = TRUE;
					}
				}

				for( nCnt = 0; nCnt < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; nCnt++ )
				{
					if (m_NeedItems[nCnt+3].ItemData !=NULL && 
						pInvenItem != NULL &&
						pInvenItem->GetItemIndex() == m_NeedItems[nCnt+3].ItemData->GetItemIndex())
					{
						NeedCntB[nCnt] = 1;
						bNeedItemCount = TRUE;
						m_NeedItems[nCnt+3].MatTab = t;
						m_NeedItems[nCnt+3].inven_idx = i;
						bNeedItems[nCnt+3] = TRUE;
					}
				}
			}
		}

		for ( i = 0; i < DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT; i++)
		{
			nResultA += NeedCntA[i];
			nResultB += NeedCntB[i];
		}

		if( nResultA == DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT )	bNeedItemA = TRUE;	// 조합 아이템 조건 만족
		if( nResultB == DEF_MAX_KNIGHT_AMOR_MATERIAL_COUNT )	bNeedItemB = TRUE;	// 조합 아이템 조건 만족
	}

// 조건 출력 
	CTString strTitle;
	CTString strSpace = "";
	CTString strJobNames[TOTAL_JOB+1] =
	{
		_S( 44, "기사" ), _S( 43, "타이탄" ), _S( 45, "힐러" ),
		_S( 46, "메이지" ), _S( 47, "로그" ), _S( 48,"소서러" ), "",
	};

// 필요 스킬 출력 
	if( m_nPetType == DRAGON_WEAPON )
	{
		strTitle.PrintF( _S( 2462, "필요 재료" ) );
		strTitle = strTitle + "(" + strJobNames[_nIndex/2] + ")";
		AddString ( strTitle, COLOR_SUB_TITLE ); 

		for( int i=0; i<3; i++ )
		{
			const char* szItemName = NULL;
			if (m_NeedItems[i].ItemData != NULL)
			{
				szItemName = m_NeedItems[i].ItemData->GetName();
				strTitle.PrintF ( _S( 576, "%s : %d개" ), strSpace + szItemName, m_NeedItems[i].llCount );
			}

			AddString ( strTitle, bNeedItems[i]? COLOR_TEXT : COLOR_NONE );
		}
		
		m_bSatisfiedA = bNeedItemA;
		m_btnOK[0].SetEnable ( m_bSatisfiedA );
	}
	else if( m_nPetType == KNIGHT_AMOR )
	{
		AddString ( _S( 2463, "필요 재료A" ), COLOR_SUB_TITLE );

		for( i = 0; i < 3; i++ )
		{
			const char* szItemName = NULL;
			if (m_NeedItems[i].ItemData != NULL)
			{
				szItemName = m_NeedItems[i].ItemData->GetName();
				strTitle.PrintF ( _S( 576, "%s : %d개 " ), strSpace + szItemName, m_NeedItems[i].llCount );
			}

			AddString ( strTitle, bNeedItems[i]? COLOR_TEXT : COLOR_NONE );
		}

		AddString ( _S( 2464, "필요 재료B" ), COLOR_SUB_TITLE );

		for( i = 0; i < 3; i++ )
		{
			const char* szItemName = NULL;
			if (m_NeedItems[i+3].ItemData != NULL)
			{
				szItemName = m_NeedItems[i+3].ItemData->GetName();
				strTitle.PrintF ( _S( 576, "%s : %d개 " ), strSpace + szItemName, m_NeedItems[i+3].llCount );
			}

			AddString ( strTitle, bNeedItems[i+3]? COLOR_TEXT : COLOR_NONE );
		}

		m_bSatisfiedA = bNeedItemA;
		m_bSatisfiedB = bNeedItemB;

		m_btnOK[1].SetEnable ( m_bSatisfiedA );
		m_btnOK[2].SetEnable ( m_bSatisfiedB );
	}

}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIPetItemMix::AddString ( CTString& strText, COLOR colText )
{
	int nLength =  strText.Length();

	if( nLength >= _iMaxMsgStringChar )
	{
		nLength -= nLength - _iMaxMsgStringChar;
		
		do
		{
			if ( strText[nLength] == ' ' )
			{
				break;
			}

		} while ( nLength-- );

		CTString strTemp2, strTemp;

		strText.Split( nLength, strTemp2, strTemp );
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbPreconditionDesc.AddString ( 0, strText, colText );
	}

}





// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendPetItemMixReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetItemMix::SendPetItemMixReq( int nbtn, int nType )
{
	if( m_bWaitProcessResult == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 941, "스킬 사용중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage(  _S( 942, "순간 이동중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	_pNetwork->SendPetItemMix ( m_vecIcons[m_nSelectProcessItem]->getIndex(), nbtn );

	m_bWaitProcessResult = TRUE;
	
	if( nType == DRAGON_WEAPON )
		m_btnOK[nbtn].SetEnable ( !m_bWaitProcessResult );
	else if( nType == KNIGHT_AMOR )
		m_btnOK[nbtn+1].SetEnable ( !m_bWaitProcessResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PetItemMixRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetItemMix::PetItemMixRep( SBYTE sbResult )
{
	// Show result
	CTString	strMessage;
	
	switch( sbResult )
	{
	case MSG_EX_PET_MIX_ITEM_ERROR_OK:
		strMessage = _S( 580, "제작에 성공 하였습니다." );										
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_FAIL:
		strMessage = _S( 577, "제작에 실패 하였습니다." );
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_NOITEM:
		strMessage = _S( 2465, "제작에 필요한 재료가 부족합니다." );
		break;
	case MSG_EX_PET_MIX_ITEM_ERROR_NOMIX:
		strMessage = _S( 2466, "제작에 적합하지 않은 조합입니다." );
		break;
	}

	ClosePetItemMix();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2459, "제작" ), UMBS_OK, UI_PETITEMMIX, MSG_EX_PET_MIX_ITEM );		
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

	m_bWaitProcessResult = FALSE;	
}

void CUIPetItemMix::ClearProcessItems()
{
	int	i;
	int	max = m_vecIcons.size();

	for (i = 0; i < max; ++i)
	{
		SAFE_DELETE(m_vecIcons[i]);
	}

	m_vecIcons.clear();
}
