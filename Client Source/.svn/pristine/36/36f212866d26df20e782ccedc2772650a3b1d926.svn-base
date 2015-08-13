#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIQuiz.h>
#include <Engine/Interface/UIInventory.h>

#define QUIZ_SELL_TOP_SLOT_SX			11		// Sell Mode(Top)
#define QUIZ_SELL_TOP_SLOT_SY			28
#define QUIZ_SELL_BOTTOM_SLOT_SX		11		// (Bottom)
#define QUIZ_SELL_BOTTOM_SLOT_SY		142


// FIXME : 서버에서 하드 코딩하길래, 같이 하드 코딩한 부분....
// FIXME : 아이템 데이터 로딩후에 바로 판별 하는게 좋을거 같은데...
// NOTE : 낱말 조합 이벤트 용 아이템 인덱스.
static int _aiWordItem[] =
{
	646,
	647,
	648,
	649,
	650,
	651,
	652,
	653,
	654,
	655,
	656,
	657,
	658,
	659,
	660,
	661,
};

// ----------------------------------------------------------------------------
// Name : CUIQuiz()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuiz::CUIQuiz()
{	
	m_nSelUserItemID	= -1;
	m_nSelQuizItemID	= -1;
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_bShowItemInfo		= FALSE;
	m_bDetailItemInfo	= FALSE;
	m_nCurInfoLines		= 0;	
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuiz()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuiz::~CUIQuiz()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	//m_nBackSplitHeight = 50;
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	// Inventory region	
	m_rcTop.SetRect( 4, 137, 190, 286 );
	m_rcBottom.SetRect( 4, 24, 190, 95 );

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 59, 216, 62, fTexWidth, fTexHeight );

	m_rtBlank.SetUV( 0, 120, 216, 130, fTexWidth, fTexHeight );

	// Inventory
	m_rtUserInven.SetUV( 0, 131, 216, 282, fTexWidth, fTexHeight );				// 아래쪽
	m_rtQuizInven.SetUV( 0, 283, 216, 358, fTexWidth, fTexHeight );			// 381 - 358 = 23	위쪽인벤.
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );		// 인벤 분리.

	// Outline of unmovable item
	m_rtUnmovableOutline.SetUV( 218, 86, 250, 118, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 51, 250, 83, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoUL.SetUV( 0, 476, 7, 483, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 10, 476, 12, 483, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 15, 476, 22, 483, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 486, 7, 488, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 10, 486, 12, 488, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 15, 486, 22, 488, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 491, 7, 498, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 10, 491, 12, 498, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 15, 491, 22, 498, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );

	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Sell button of shop
	m_btnOK.Create( this, _S( 191, "확인" ), 30, 106, 62, 21 );
	m_btnOK.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	m_btnCancel.Create( this, _S( 139, "취소" ), 124, 106, 62, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Bottom scroll bar
	m_sbScrollBar.Create( this, 194, 143, 9, 137 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10);
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( QUIZ_USER_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetCurItemCount( QUIZ_USER_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetItemsPerPage( QUIZ_USER_SLOT_ROW );
	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -192, -6, 191, 148 );	

	// Slot items
	// Quiz Slot(5x4)
	for( int i = 0; i < QUIZ_USER_SLOT_MAX; i++ )
	{
		m_abtnUserItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_QUIZ, UBET_ITEM, 0, i );
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
		m_abtnQuizItems[iItem].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_QUIZ, UBET_ITEM );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenQuiz()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::OpenQuiz( )
{
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	ResetQuiz();

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_SHOP );

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareUserItems();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIQuiz::PrepareUserItems()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );
	
	RefreshPlayerItem();

	// Set size & position
	//SetSize( QUIZ_MAIN_WIDTH, QUIZ_MAIN_HEIGHT );
	CDrawPort	*pdp = pUIManager->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade items
	nX = QUIZ_SELL_TOP_SLOT_SX;
	nY = QUIZ_SELL_TOP_SLOT_SY;
	for( int nItem = 0; nItem < QUIZ_QUIZ_SLOT_TOTAL; nItem++ )
	{
		if( nItem == QUIZ_QUIZ_SLOT_COL )
		{
			nY += 35;	nX = QUIZ_SELL_BOTTOM_SLOT_SX;
		}
		m_abtnQuizItems[nItem].SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_QUIZ, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIQuiz::RefreshPlayerItem()
{
	int		i;

	for( i = 0; i < QUIZ_USER_SLOT_MAX; i++ )
	{

		CItems*	pItems = &_pNetwork->MySlotItem[0][i];
		if( pItems->Item_Sum > 0 )
		{
				m_abtnUserItems[i].SetItemInfo( 0, i, pItems->Item_Index,
															pItems->Item_UniIndex, pItems->Item_Wearing );
				m_abtnUserItems[i].SetItemPlus( pItems->Item_Plus );
				m_abtnUserItems[i].SetItemCount( pItems->Item_Sum );

			for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; sbOption++ )
			{
				m_abtnUserItems[i].SetItemOptionData( sbOption,
																	pItems->GetOptionType( sbOption ),
																	pItems->GetOptionLevel( sbOption ),
																	pItems->GetOriginOptionVar( sbOption ));
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIQuiz::ResetQuiz()
{
	m_nSelUserItemID	= -1;
	m_nSelQuizItemID	= -1;
	m_nNumOfItem		= 0;

	ClearItems();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_QUIZ, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SHOP );

	m_nCurRow = 0;
	m_sbScrollBar.SetScrollPos( 0 );

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_SHOP );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIQuiz::ClearItems()
{
	int		i;
	for( i = 0; i < QUIZ_QUIZ_SLOT_TOTAL; i++)
		m_abtnQuizItems[i].InitBtn();			

	for( i = 0; i < QUIZ_USER_SLOT_MAX; i++)
	{
		m_abtnUserItems[i].InitBtn();
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return ;

	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
#if defined(G_THAI)
	int		iPos;
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strItemInfo);
	INDEX	nChatMax= (MAX_ITEMINFO_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		m_strItemInfo[m_nCurInfoLines] = strItemInfo;
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = MAX_ITEMINFO_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strItemInfo,0,iPos))
				break;
		}
		nSplitPos = iPos;
		
		// Split string
		CTString	strTemp;
		strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		
		// Trim space
		if( strTemp[0] == ' ' )
		{
			int	nTempLength = strTemp.Length();
			for( iPos = 1; iPos < nTempLength; ++iPos )
			{
				if( strTemp[iPos] != ' ' )
					break;
			}
			
			strTemp.TrimLeft( strTemp.Length() - iPos );
		}
		
		AddItemInfoString( strTemp, colItemInfo );
		
	}
	
#else
	// If length of string is less than max char
	if( nLength <= MAX_ITEMINFO_CHAR )
	{
		m_strItemInfo[m_nCurInfoLines] = strItemInfo;
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = MAX_ITEMINFO_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strItemInfo[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
		
		// Split string
		CTString	strTemp;
		strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		
		// Trim space
		if( strTemp[0] == ' ' )
		{
			int	nTempLength = strTemp.Length();
			int iPos;
			for( iPos = 1; iPos < nTempLength; iPos++ )
			{
				if( strTemp[iPos] != ' ' )
					break;
			}
			
			strTemp.TrimLeft( strTemp.Length() - iPos );
		}
		
		AddItemInfoString( strTemp, colItemInfo );
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc : nWhichSlot - 0: opposite slot, 1: my slot, 2: inventory slot
// ----------------------------------------------------------------------------
// FIXME : 중복되는 코드가 너무 많음.
BOOL CUIQuiz::GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight, int nTradeItem, int inven_idx )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex;
	SQUAD		llCount;
	ULONG		ulItemPlus, ulFlag;
	SBYTE		sbOptionType[MAX_OPTION_INC_ORIGIN];
	LONG		lOptionLevel[MAX_OPTION_INC_ORIGIN];

	switch( nWhichSlot )
	{
	case 0:
		{
			nIndex		= m_abtnQuizItems[nTradeItem].GetItemIndex();
			llCount		= m_abtnQuizItems[nTradeItem].GetItemCount();
			ulItemPlus	= m_abtnQuizItems[nTradeItem].GetItemPlus();
			ulFlag		= m_abtnQuizItems[nTradeItem].GetItemFlag();

			for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnQuizItems[nTradeItem].GetItemOptionType( sbOption );
				lOptionLevel[sbOption] = m_abtnQuizItems[nTradeItem].GetItemOptionLevel( sbOption );
			}
		}
		break;

	case 1:
		{
			nIndex		= m_abtnUserItems[inven_idx].GetItemIndex();
			llCount		= m_abtnUserItems[inven_idx].GetItemCount();
			ulItemPlus	= m_abtnUserItems[inven_idx].GetItemPlus();
			ulFlag		= m_abtnUserItems[inven_idx].GetItemFlag();

			for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; sbOption++ )
			{
				sbOptionType[sbOption] = m_abtnUserItems[inven_idx].GetItemOptionType( sbOption );
				lOptionLevel[sbOption] = m_abtnUserItems[inven_idx].GetItemOptionLevel( sbOption );
			}
		}
		break;
	}
	
	if( nIndex < 0 )
		return FALSE;

	CItemData	*pItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( nIndex );

	// Get item name
	if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
	{
		CTString	strCount;
		strCount.PrintF( "%I64d", llCount );
		CUIManager::getSingleton()->InsertCommaToString( strCount );		
		strTemp.PrintF( "%s(%s)", szItemName, strCount );
	}
	else
	{
		if( ulItemPlus > 0 )
			strTemp.PrintF( "%s +%d", szItemName, ulItemPlus );
		else
		{
			if( nWhichSlot == 0 && pItemData->GetFlag() & ITEM_FLAG_COUNT )
			{
				CTString	strCount;
				strCount.PrintF( "%I64d", llCount );
				CUIManager::getSingleton()->InsertCommaToString( strCount );
				strTemp.PrintF( "%s(%s)", szItemName, strCount );
			}
			else
			{
				strTemp = szItemName;
			}
		}
	}
	AddItemInfoString( strTemp );

	// Get item information in detail
	if( m_bDetailItemInfo )
	{

		switch( pItemData->GetType() )
		{
		case CItemData::ITEM_WEAPON:		// Weapon item
			{
				// Level
				int	nItemLevel = pItemData->GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( pItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				int	nPlusValue;
				if( ulItemPlus > 0 )
				{
					// Physical attack
					if( pItemData->GetPhysicalAttack() > 0 )
					{
						//nPlusValue = (int)( pItemData->GetPhysicalAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - pItemData->GetPhysicalAttack();
						nPlusValue = CItems::CalculatePlusDamage( pItemData->GetPhysicalAttack(), ulItemPlus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 355, "공격력 : %d + %d" ), pItemData->GetPhysicalAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 161, "공격력 : %d" ), pItemData->GetPhysicalAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1891, "물리 공격력 + 75" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}

					// Magic attack
					if( pItemData->GetMagicAttack() > 0 )
					{
						//nPlusValue = (int)( pItemData->GetMagicAttack() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - pItemData->GetMagicAttack();
						nPlusValue = CItems::CalculatePlusDamage( pItemData->GetMagicAttack(), ulItemPlus, pItemData->GetLevel() >= 146 ? TRUE : FALSE);
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 356, "마법 공격력 : %d + %d" ), pItemData->GetMagicAttack(), nPlusValue );
						else
							strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), pItemData->GetMagicAttack() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1892, "마법 공격력 + 50" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}
				}
				else
				{
					// Physical attack
					if( pItemData->GetPhysicalAttack() > 0 )
					{
						strTemp.PrintF( _S( 161, "공격력 : %d" ), pItemData->GetPhysicalAttack() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic attack
					if( pItemData->GetMagicAttack() > 0 )
					{
						strTemp.PrintF( _S( 162, "마법 공격력 : %d" ), pItemData->GetMagicAttack() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
			}
			break;

		case CItemData::ITEM_SHIELD:		// Shield item
			{
				// Level
				int	nItemLevel = pItemData->GetLevel();
				if( nItemLevel > 0 )
				{
					strTemp.PrintF( _S( 160, "레벨: %d" ), nItemLevel );
					AddItemInfoString( strTemp,
										nItemLevel > _pNetwork->MyCharacterInfo.level ? 0xD28060FF : 0x9E9684FF );
				}

				// Class
				CUIManager::GetClassOfItem( pItemData, strTemp );
				AddItemInfoString( strTemp, 0x9E9684FF );

				int	nPlusValue;
				if( ulItemPlus > 0 )
				{
					// Physical defense
					if( pItemData->GetPhysicalDefence() > 0 )
					{
						//nPlusValue = (int)( pItemData->GetPhysicalDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - pItemData->GetPhysicalDefence();
						nPlusValue = CItems::CalculatePlusDamage( pItemData->GetPhysicalDefence(), ulItemPlus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 357, "방어력 : %d + %d" ), pItemData->GetPhysicalDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 163, "방어력 : %d" ), pItemData->GetPhysicalDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );

							strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}

					// Magic defense
					if( pItemData->GetMagicDefence() > 0 )
					{
						//nPlusValue = (int)( pItemData->GetMagicDefence() *
						//					pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - pItemData->GetMagicDefence();
						nPlusValue = CItems::CalculatePlusDamage( pItemData->GetMagicDefence(), ulItemPlus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
						if( nPlusValue > 0 )
							strTemp.PrintF( _S( 358, "마법 방어력 : %d + %d" ), pItemData->GetMagicDefence(), nPlusValue );
						else
							strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), pItemData->GetMagicDefence() );
															
						AddItemInfoString( strTemp, 0xDEC05BFF );

						if( ulItemPlus >= 15 )
						{
							strTemp.PrintF(_S( 1893, "물리 방어력 + 100" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );

							strTemp.PrintF(_S( 1894, "마법 방어력 + 50" ));		
							AddItemInfoString( strTemp, 0xDEC05BFF );
						}
					}
				}
				else
				{
					// Physical defense
					if( pItemData->GetPhysicalDefence() > 0 )
					{
						strTemp.PrintF( _S( 163, "방어력 : %d" ), pItemData->GetPhysicalDefence() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}

					// Magic defense
					if( pItemData->GetMagicDefence() > 0 )
					{
						strTemp.PrintF( _S( 164, "마법 방어력 : %d" ), pItemData->GetMagicDefence() );
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
			}
			break;

		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
			}
			break;

				// 일회용
		case CItemData::ITEM_ONCEUSE:
			{
				// 퀘스트 정보 표시.
				if ( pItemData->GetSubType() == CItemData::ITEM_SUB_QUEST_SCROLL )
				{	
					const int iQuestIndex = pItemData->GetNum0();

					if( iQuestIndex != -1 )
					{
						// 퀘스트 이름 출력
						strTemp.PrintF( "%s", CQuestSystem::Instance().GetQuestName( iQuestIndex ) );
						AddItemInfoString( strTemp, 0xDEC05BFF );
						
						const int iMinLevel = CQuestSystem::Instance().GetQuestMinLevel( iQuestIndex );
						const int iMaxLevel = CQuestSystem::Instance().GetQuestMaxLevel( iQuestIndex );

						// 레벨 제한 출력.
						strTemp.PrintF( _S( 1660, "레벨 제한 : %d ~ %d" ), iMinLevel, iMaxLevel );		
						AddItemInfoString( strTemp, 0xDEC05BFF );
					}
				}
			}
			break;

		case CItemData::ITEM_POTION:	// Date : 2005-01-07,   By Lee Ki-hwan
			{
				// Date : 2005-01-14,   By Lee Ki-hwan
				
				if ( pItemData->GetSubType() == CItemData::POTION_UP )
				{
					if( ulFlag > 0 )
					{
						// Level
						strTemp.PrintF( _S( 160, "레벨: %d" ), ulFlag );
						AddItemInfoString( strTemp, 0xD28060FF );

						// 향상 타입
						int nSkillType = pItemData->GetSkillType();
						CSkill	&rSkill = _pNetwork->GetSkillData( nSkillType );
						int Power = rSkill.GetPower( ulFlag - 1);

						if(  pItemData->GetNum1() == CItemData::POTION_UP_PHYSICAL ) // 물리
						{
							if(  pItemData->GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 790, "물리 공격력 +%d 상승" ), Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 791, "물리 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}

						}
						else if( pItemData->GetNum1() == CItemData::POTION_UP_MAGIC ) // 마법
						{
							if(  pItemData->GetNum2() == CItemData::POTION_UP_ATTACK ) // 공격
							{
								strTemp.PrintF ( _S( 792, "마법 공격력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
							else if( pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE ) // 방어
							{
								strTemp.PrintF ( _S( 793, "마법 방어력 +%d 상승" ),  Power );
								AddItemInfoString( strTemp, 0xDEC05BFF );
							}
						}
					}
				}
			}

		case CItemData::ITEM_BULLET:		// Bullet item
			{
			}
			break;

		case CItemData::ITEM_ETC:			// Etc item
			{
				switch( pItemData->GetSubType() )
				{
				case CItemData::ITEM_ETC_REFINE:
					{
						// FIXME : 레벨 표시가 안된다구 해서...
						// 블러드라고 표시가 되어있다면, 표시를 없애준다.
						/*if(ulFlag & FLAG_ITEM_OPTION_ENABLE)
						{
							ulFlag ^= FLAG_ITEM_OPTION_ENABLE;
						}*/

						// Level
						if( ulFlag > 0 )
						{
							strTemp.PrintF( _S( 160, "레벨: %d" ), ulFlag );
							AddItemInfoString( strTemp, 0xD28060FF );
						}
					}
					break;

				// 블러드 아이템 & 정화석.
				case CItemData::ITEM_ETC_OPTION:
					{
					}
					break;
				}
			}
			break;
		}

		const int iFame = pItemData->GetFame();
		if( iFame > 0 )
		{
			strTemp.PrintF( _S( 1096, "명성 %d 필요" ), iFame );		
			AddItemInfoString( strTemp, 0xDEC05BFF );
		}


		// Options
		switch( pItemData->GetType() )
		{
		case CItemData::ITEM_WEAPON:
		case CItemData::ITEM_SHIELD:
		case CItemData::ITEM_ACCESSORY:
			{
				for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; sbOption++ )
				{
					if( sbOptionType[sbOption] == -1 || lOptionLevel[sbOption] == 0 )
						break;

					COptionData* odItem = COptionData::getData( sbOptionType[sbOption] );

					if (odItem == NULL)
						continue;

					strTemp.PrintF( "%s : %d", odItem->GetName(), odItem->GetValue( lOptionLevel[sbOption] - 1 ) ); 
					AddItemInfoString( strTemp, 0x94B7C6FF );
				}
				if( ulFlag & FLAG_ITEM_OPTION_ENABLE )
				{
					AddItemInfoString( _S( 511, "블러드 옵션 가능" ), 0xE53535FF );		
				}
				if( ulFlag & FLAG_ITEM_SEALED )
				{
					AddItemInfoString( _S( 512, "봉인된 아이템" ), 0xE53535FF );		
				}
			}
			break;
		}

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( nIndex );
		if( pDesc[0] != NULL )
		{
			//AddItemInfoString( CTString( " " ) );
			strTemp.PrintF( "%s", pDesc );
			AddItemInfoString( strTemp, 0x9E9684FF );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}
	else
	{
		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::ShowItemInfo( BOOL bShowInfo, BOOL bRenew, int nTradeItem, int inven_idx )
{
	static int	nOldBtnID = -1;
	int			nBtnID;

	m_bShowItemInfo = FALSE;

	// Hide item information
	if( !bShowInfo )
	{
		nOldBtnID = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX = 0, nInfoPosY = 0;

	// Trade
	if( nTradeItem >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID		= m_abtnQuizItems[nTradeItem].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnQuizItems[nTradeItem].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelQuizItemID == nTradeItem;
			if( !GetItemInfo( 0, nInfoWidth, nInfoHeight, nTradeItem ) )
				m_bShowItemInfo = FALSE;
		}
	}
	// Quiz
	else if( inven_idx >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_abtnUserItems[inven_idx].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_abtnUserItems[inven_idx].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			m_bDetailItemInfo = m_nSelUserItemID == inven_idx;
			if( !GetItemInfo( 1, nInfoWidth, nInfoHeight, -1, inven_idx ) )
				m_bShowItemInfo = FALSE;
		}
	}

	// Update item information box
	if( m_bShowItemInfo && bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		CUIManager* pUIManager = CUIManager::getSingleton();
		
		if( nInfoPosX < pUIManager->GetMinI() )
			nInfoPosX = pUIManager->GetMinI();
		else if( nInfoPosX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoPosX = pUIManager->GetMaxI() - nInfoWidth;

		if( nInfoPosY - nInfoHeight < pUIManager->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE;
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowItemInfo )
		nOldBtnID = -1;
}

// ----------------------------------------------------------------------------
// Name : RenderItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIQuiz::RenderItems()
{
	int iQuizX, iQuizY;	
	iQuizX = QUIZ_SELL_BOTTOM_SLOT_SX;
	iQuizY = QUIZ_SELL_BOTTOM_SLOT_SY;
	

	int	i;
	int	nX = iQuizX;
	int	nY = iQuizY;
	int	iRowS = m_nCurRow * QUIZ_USER_SLOT_COL;
	int	iRowE = (iRowS + QUIZ_USER_SLOT_ROW) * QUIZ_USER_SLOT_COL;
	
	for( i = iRowS; i < iRowE; i++ )
	{
		m_abtnUserItems[i].SetPos( nX, nY );
		nX += 35;

		if( m_abtnUserItems[i].IsEmpty() )
			continue;
			
		m_abtnUserItems[i].Render();

		if (i > iRowS && (i % QUIZ_USER_SLOT_COL) == 0)
		{
			nX = iQuizX;	nY += 35;
		}
	}

	// ---Trade slot items---
	for( int iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnQuizItems[iItem].IsEmpty() )
			continue;
			
		m_abtnQuizItems[iItem].Render();
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Outline of items in shop slot ( unmovable )	
	for( i = iRowS; i < iRowE; i++ )
	{
		// If button is empty
		if( m_abtnUserItems[i].IsEmpty() )
			continue;
			
		// Not wearing, not refine stone, can trade
		int			nIndex = m_abtnUserItems[i].GetItemIndex();
		CItemData*	pItemData = _pNetwork->GetItemData( nIndex );
		if( m_abtnUserItems[i].GetItemWearType() < 0 &&
				( pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
				pItemData->GetFlag() & ITEM_FLAG_TRADE )
			continue;
			
		m_abtnUserItems[i].GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
			m_rtUnmovableOutline.U0, m_rtUnmovableOutline.V0,
			m_rtUnmovableOutline.U1, m_rtUnmovableOutline.V1,
			0xFFFFFFFF );
	}	

	// Outline of selected item
	if( m_nSelQuizItemID >= 0 )
	{
		m_abtnQuizItems[m_nSelQuizItemID].GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelUserItemID >= 0 )
	{
		m_abtnUserItems[m_nSelUserItemID].GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
						m_rtSelectOutline.U0, m_rtSelectOutline.V0,
						m_rtSelectOutline.U1, m_rtSelectOutline.V1,
						0xFFFFFFFF );
	}

	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{
		// Item information region
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right, m_rcItemInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Render item information
		int	nInfoX = m_rcItemInfo.Left + 12;
		int	nInfoY = m_rcItemInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 3,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 3,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	
	
	int nX = m_nPosX;
	int nY = m_nPosY + 22;

	// 위쪽 인벤.
	pDrawPort->AddTexture( nX, nY,
										nX + QUIZ_QUIZ_WIDTH, nY + QUIZ_TRADE_HEIGHT,
										m_rtQuizInven.U0, m_rtQuizInven.V0,
										m_rtQuizInven.U1, m_rtQuizInven.V1,
										0xFFFFFFFF );

	nY += QUIZ_TRADE_HEIGHT;
	pDrawPort->AddTexture( nX, nY,
										nX + QUIZ_QUIZ_WIDTH, nY + 38,
										m_rtBlank.U0, m_rtBlank.V0,
										m_rtBlank.U1, m_rtBlank.V1,
										0xFFFFFFFF );

	nY += 38;
	pDrawPort->AddTexture( nX, nY,
										nX + QUIZ_QUIZ_WIDTH, nY + 1,
										m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
										m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
										0xFFFFFFFF );

	// 아래쪽 인벤.
	nY += 1;
	pDrawPort->AddTexture( nX, nY,
										nX + QUIZ_QUIZ_WIDTH, nY + QUIZ_QUIZ_HEIGHT,
										m_rtUserInven.U0, m_rtUserInven.V0,
										m_rtUserInven.U1, m_rtUserInven.V1,
										0xFFFFFFFF );
	

	// Bottom scroll bar		
	m_sbScrollBar.Render();

	// Close button
	m_btnClose.Render();
	
	m_btnOK.Render();
	
	// Cancel button of shop
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render item information
	pDrawPort->PutTextEx( _S( 1214,  "낱말 맞추기"  ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,		
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );	

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render Items
	RenderItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuiz::MouseMessage( MSG *pMsg )
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
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Hold item button
			if( pUIManager->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				// Quiz items
				if( m_nSelUserItemID >= 0 )
				{				
					// Not wearing, not refine stone, can trade
					CItems		*pItems = &_pNetwork->MySlotItem[0][m_nSelUserItemID];
					CItemData*	pItemData = pItems->ItemData;
					if( pItems->Item_Wearing == -1 &&
						( pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
						pItemData->GetFlag() & ITEM_FLAG_TRADE )
					{
						pUIManager->SetHoldBtn( m_abtnUserItems[m_nSelUserItemID] );
						int	nOffset = BTN_SIZE / 2;
						pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

						m_abtnUserItems[m_nSelUserItemID].SetBtnState( UBES_IDLE );
					}
				}
				// Trade items
				else if( m_nSelQuizItemID >= 0 )
				{
					pUIManager->SetHoldBtn( m_abtnQuizItems[m_nSelQuizItemID] );
					int	nOffset = BTN_SIZE / 2;
					pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnQuizItems[m_nSelQuizItemID].SetBtnState( UBES_IDLE );
				}

				bLButtonDownInItem	= FALSE;
			}

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
			// Sell button of shop
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of shop
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
			// Bottom Scroll bar
			else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}			
			// Quiz
			else if( IsInsideRect( nX, nY, m_rcTop ) )
			{
				int	i;
				int	iRowS = m_nCurRow * QUIZ_USER_SLOT_COL;						// Start Row
				int	iRowE = (iRowS + QUIZ_USER_SLOT_ROW) * QUIZ_USER_SLOT_COL;	// End Row
				int	nWhichIdx = -1;
				
				for( i = iRowS; i < iRowE; ++i )
				{
					if( m_abtnUserItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						nWhichIdx = i;
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichIdx );

				return WMSG_SUCCESS;
			}
			// Trade
			else if( IsInsideRect( nX, nY, m_rcBottom ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnQuizItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;	
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichItem );

				return WMSG_SUCCESS;
			}

			// Hide tool tip
			ShowItemInfo( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
				// Sell button of shop
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}				
				// Quiz items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelUserItemID = -1;
					m_nSelQuizItemID = -1;

					int	i;
					int	iRowS = m_nCurRow * QUIZ_USER_SLOT_COL;
					int	iRowE = (iRowS + QUIZ_USER_SLOT_ROW) * QUIZ_USER_SLOT_COL;	// 4줄~
					
					for( i = iRowS; i < iRowE; ++i )
					{
						if( m_abtnUserItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelUserItemID = i;	// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, -1, i );

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_QUIZ, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					m_nSelQuizItemID = -1;
					m_nSelUserItemID = -1;

					for( int iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnQuizItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelQuizItemID = iItem;			// 선택된 슬롯의 아이템 ID

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_QUIZ, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}				
				// Bottom Scroll bar
				else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbScrollBar.GetScrollPos();
				}				

				pUIManager->RearrangeOrder( UI_QUIZ, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			bLButtonDownInItem = FALSE;

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If shop isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetQuiz();
					}

					return WMSG_SUCCESS;
				}				
				// Sell button of shop
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 SendBingo();

						 // Clear Member Value
						 ResetQuiz();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of shop
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetQuiz();
					}

					return WMSG_SUCCESS;
				}				
				// Bottom Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}				
				// Quiz items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	i;
					int	iRowS = m_nCurRow * QUIZ_USER_SLOT_COL;
					int	iRowE = (iRowS + QUIZ_USER_SLOT_ROW) * QUIZ_USER_SLOT_COL;

					for (i = iRowS; i < iRowE; ++i)
					{
						if( m_abtnUserItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					for( int iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnQuizItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
			}		
			else
			{
				//////////////////////////////////////////////////////////////////////////
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and is from shop
					if( pUIManager->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						pUIManager->GetHoldBtn().GetWhichUI() == UI_QUIZ )
					{
						// Trade items
						if( IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from shop slot
							if( m_nSelQuizItemID < 0 ||
								m_abtnQuizItems[m_nSelQuizItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
							{
								AddQuizItem( pUIManager->GetHoldBtn().GetInvenIndex(),
												pUIManager->GetHoldBtn().GetItemUniIndex(),
												1 );

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Quiz items
						else if( IsInsideRect( nX, nY, m_rcTop ) )
						{
							// If this item is moved from trade slot
							if( m_nSelUserItemID < 0 ||
								m_abtnUserItems[m_nSelUserItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
							{
								DelQuizItem( pUIManager->GetHoldBtn().GetInvenIndex(),
												pUIManager->GetHoldBtn().GetItemUniIndex(),
												1,
												m_nSelQuizItemID );

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item

					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				} // If - IsInside
				//////////////////////////////////////////////////////////////////////////
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Trade items
				if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	nTradeItemID = m_nSelQuizItemID;
					m_nSelQuizItemID = -1;

					for( int iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnQuizItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							DelQuizItem( m_abtnQuizItems[iItem].GetInvenIndex(),
											m_abtnQuizItems[iItem].GetItemUniIndex(),
											1,
											nTradeItemID );

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// Quiz items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelUserItemID = -1;

					int	i;
					int	iRowS = m_nCurRow * QUIZ_USER_SLOT_COL;
					int	iRowE = (iRowS + QUIZ_USER_SLOT_ROW) * QUIZ_USER_SLOT_COL;
					
					for (i = iRowS; i < iRowE; ++i)
					{
						if( m_abtnUserItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{								
							// Not wearing, not refine stone, can trade
								CItems		*pItems = &_pNetwork->MySlotItem[0][i];
								CItemData	*pItemData = pItems->ItemData;
								if( pItems->Item_Wearing == -1 &&
									( pItemData->GetType() != CItemData::ITEM_ETC || 
									pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
									pItemData->GetFlag() & ITEM_FLAG_TRADE )
							{
								AddQuizItem( i, m_abtnUserItems[i].GetItemUniIndex(), 1 );
							}							

							// Show tool tup
							ShowItemInfo( TRUE, TRUE, -1, i );

							return WMSG_SUCCESS;
						}
					}
				}				
				// Bottom Scroll bar
				else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbScrollBar.GetScrollPos();
				}				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Bottom Scroll bar
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempIndex;
static int		nTempUniIndex;
static int		nTempIdx;
static SQUAD	llTempCount;
static int		nTempTradeItemID;

// ----------------------------------------------------------------------------
// Name : AddQuizItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIQuiz::AddQuizItem( int nIdx, int nUniIndex, SQUAD llCount )
{
	nTempIdx		= nIdx;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempIndex		= m_abtnUserItems[nTempIdx].GetItemIndex();
	ULONG ulItemPlus= m_abtnUserItems[nTempIdx].GetItemPlus();

	UserToQuiz( llTempCount );	
}

// ----------------------------------------------------------------------------
// Name : DelQuizItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIQuiz::DelQuizItem( int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID )
{
	nTempIdx		= nIdx;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempTradeItemID= nTradeItemID;
	nTempIndex		= m_abtnQuizItems[nTempTradeItemID].GetItemIndex();
	
	QuizToUser( llTempCount );	
}

// ----------------------------------------------------------------------------
// Name : SendBingo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuiz::SendBingo()
{
	LONG lItemCount = 0;
	int iItem;
	for( iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnQuizItems[iItem].IsEmpty() )
			continue;
		lItemCount++;
	}

	CNetworkMessage nmLetter(MSG_EVENT);
	nmLetter << (UBYTE)MSG_EVENT_LETTER;
	nmLetter << (UBYTE)MSG_EVENT_LETTER_REQ;	
	nmLetter << lItemCount;

	for( int i = 0; i < QUIZ_QUIZ_SLOT_TOTAL; ++i )
	{
		if( m_abtnQuizItems[iItem].IsEmpty() )
			continue;

		LONG lItemIndex = m_abtnQuizItems[i].GetItemIndex();
		nmLetter << lItemIndex;
	}
	_pNetwork->SendToServerNew(nmLetter);	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIQuiz::UserToQuiz( SQUAD llCount )
{
	// FIXME : 벡터로 고쳐서 처리하는게 좋지 않을까???
	BOOL bSatisfied = FALSE;
	for(int i = 0; i < (sizeof(_aiWordItem) / sizeof(int)); ++i )
	{
		if( _aiWordItem[i] == nTempIndex )
		{
			bSatisfied = TRUE;
			break;
		}
	}

	if( !bSatisfied )
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 1215, "낱말 아이템만 등록할 수 있습니다." ), SYSMSG_ERROR );		
		return;
	}

	// Find same item in trade slot
	int iItem ;
	for( iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnQuizItems[iItem].GetItemIndex() == nTempIndex )
			break;
	}

	// If there is same item
	if( iItem < QUIZ_QUIZ_SLOT_TOTAL )
	{
		// Check if item is countable
		CItemData*	pItemData = _pNetwork->GetItemData( nTempIndex );
		if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
		{
			// Update count of trade item
			SQUAD	llNewCount = m_abtnQuizItems[iItem].GetItemCount();
			llNewCount += llCount;
			m_abtnQuizItems[iItem].SetItemCount( llNewCount );
												
			//if( !m_bBuyQuiz )
			{
				// Update count of shop item
				llNewCount = m_abtnUserItems[nTempIdx].GetItemCount();
				llNewCount -= llCount;
				m_abtnUserItems[nTempIdx].SetItemCount( llNewCount );
				if( llNewCount <= 0 )
				{
					m_abtnUserItems[nTempIdx].SetEmpty( TRUE );

					// Unselect item
					if( m_nSelUserItemID == nTempIdx )
						m_nSelUserItemID = -1;
				}
			}
			return;
		}
	}

	// Find empty slot
	for( iItem = 0; iItem < QUIZ_QUIZ_SLOT_TOTAL; iItem++ )
	{
		if( m_abtnQuizItems[iItem].IsEmpty() )
			break;
	}

	// If there is not empty slot
	if( iItem == QUIZ_QUIZ_SLOT_TOTAL )
	{
		// Add system message		
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 1216, "최대 10개의 낱말을 등록할 수 있습니다." ), SYSMSG_ERROR );		

		return;
	}

	m_abtnQuizItems[iItem].SetItemInfo( 0, nTempIdx, nTempIndex, nTempUniIndex, -1 );
	m_abtnQuizItems[iItem].SetItemCount( llCount );
	
	//if( !m_bBuyQuiz )
	{												
		// Update count of shop item
		SQUAD	llNewCount = m_abtnUserItems[nTempIdx].GetItemCount();
		llNewCount -= llCount;
		m_abtnUserItems[nTempIdx].SetItemCount( llNewCount );
		if( llNewCount <= 0 )
		{
			m_abtnUserItems[nTempIdx].SetEmpty( TRUE );

			// Unselect item
			if( m_nSelUserItemID == nTempIdx )
				m_nSelUserItemID = -1;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIQuiz::QuizToUser( SQUAD llCount )
{
	// Update count of trade item
	SQUAD	llNewCount = m_abtnQuizItems[nTempTradeItemID].GetItemCount();
	llNewCount -= llCount;
	m_abtnQuizItems[nTempTradeItemID].SetItemCount( llNewCount );

	if( llNewCount <= 0 )
	{
		// Rearrange buttons
		for( int iArrItem = nTempTradeItemID; iArrItem < QUIZ_QUIZ_SLOT_TOTAL; iArrItem++ )
		{
			if( ( iArrItem + 1 ) == QUIZ_QUIZ_SLOT_TOTAL )
			{
				m_abtnQuizItems[iArrItem].InitBtn();
				break;
			}

			if( m_abtnQuizItems[iArrItem + 1].IsEmpty() )
			{
				m_abtnQuizItems[iArrItem].InitBtn();
				break;
			}

			m_abtnQuizItems[iArrItem].SetItemInfo( 0,
													m_abtnQuizItems[iArrItem + 1].GetInvenIndex(),
													m_abtnQuizItems[iArrItem + 1].GetItemIndex(),
													m_abtnQuizItems[iArrItem + 1].GetItemUniIndex(),
													-1 );
			m_abtnQuizItems[iArrItem].SetItemCount( m_abtnQuizItems[iArrItem + 1].GetItemCount() );
		}

		// Unselect item
		if( nTempTradeItemID == m_nSelQuizItemID )
			m_nSelQuizItemID = -1;
	}

	//if( !m_bBuyQuiz )
	{
		// Update count of shop item
		llNewCount = m_abtnUserItems[nTempIdx].GetItemCount();
		llNewCount += llCount;
		m_abtnUserItems[nTempIdx].SetItemCount( llNewCount );
		if( llNewCount > 0 )
			m_abtnUserItems[nTempIdx].SetEmpty( FALSE );
	}
}