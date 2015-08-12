#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Common/Packet/ptype_inventory.h>
#include <Common/Packet/ptype_old_do_stash.h>
#include <Engine/Interface/UISecurity.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Info/MyInfo.h>

// Define item slot
#define WAREHOUSE_WAREHOUSE_WIDTH			216
#define WAREHOUSE_TOP_HEIGHT				74
#define WAREHOUSE_MIDDLE_HEIGHT				34
#define WAREHOUSE_BOTTOM_HEIGHT				41
#define WAREHOUSE_TRADE_HEIGHT				75
#define WAREHOUSE_DAYS_HEIGHT				27		// wooss 050811
#define WAREHOUSE_BUY_TOP_SLOT_SX			11		// Buy Mode(Top)
#define WAREHOUSE_BUY_TOP_SLOT_SY			55		// wooss 28 -> 55	
#define WAREHOUSE_BUY_BOTTOM_SLOT_SX		11		// (Bottom)
#define WAREHOUSE_BUY_BOTTOM_SLOT_SY		239		// wooss 214 -> 241
#define DEF_EXPANSION_TEXT_AREA				(214)	// 만료 기간 표시 텍스트 영역.

#define	ITEM_LIST_START		(1 << 0)
#define ITEM_LIST_END		(1 << 1)
#define ITEM_LIST_EMPTY		(1 << 2)

enum eSelection
{
	WAREHOUSE_IN,
	WAREHOUSE_OUT,
	WAREHOUSE_CHANGEPW,
	WAREHOUSE_TALK,
};

// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;

class CmdWareHouseAddItem : public Command
{
public:
	CmdWareHouseAddItem() : m_pWnd(NULL)	{}
	void setData(CUIWareHouse* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CUIWareHouse* m_pWnd;
};

class CmdWareHouseDelItem : public Command
{
public:
	CmdWareHouseDelItem() : m_pWnd(NULL)	{}
	void setData(CUIWareHouse* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->DelItemCallback();
	}
private:
	CUIWareHouse* m_pWnd;
};

// ----------------------------------------------------------------------------
// Name : CUIWareHouse()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIWareHouse::CUIWareHouse()
	: m_pItemsTemp(NULL)
{
	m_bStorageWareHouse		= FALSE;
	m_nSelWareHouseItemID	= -1;
	m_nSelTradeItemID		= -1;
	m_nSelectedWareHouseID	= -1;
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_nTex				= 1;
	m_strTotalPrice		= CTString( "0" );
	m_bHasPassword		= false;
	m_strPW				= CTString( "0" );
	m_nNpcIndex			= -1;
	m_nNPCVIdx			= -1;

	m_useTime			= 0; //wooss 050817

	//2013/04/02 jeil 나스 아이템 제거 
	m_strTotalNas		= CTString( "0" );
	m_strInNas			= CTString( "0" );
	m_strOutNas			= CTString( "0" );
	m_nInNas			= 0;
	m_nOutNas			= 0;
	m_nTotalNas			= 0;
	m_bCashRemote		= false;
	m_nTempTradeIdx	= -1;
	m_nTempStorageIdx	= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIWareHouse()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIWareHouse::~CUIWareHouse()
{
	Destroy();

	if( !m_vectorStorageItems.empty() )
		m_vectorStorageItems.clear();

	int		i;

	for (i = 0; i < ITEM_COUNT_IN_INVENTORY_STASH; ++i)
		SAFE_DELETE(m_pIconWareHouseItems[i]);

	for (i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
		SAFE_DELETE(m_pIconTradeItems[i]);
}


int CUIWareHouse::GetWareHouseItemIndex( int nInvenIdx )
{
	if ( nInvenIdx > ITEM_COUNT_IN_INVENTORY_STASH || nInvenIdx < 0)
		return -1;

	return m_pIconWareHouseItems[nInvenIdx]->getIndex();
}

LONGLONG CUIWareHouse::GetWareHouseItemCount( int nInvenIdx )
{
	if (nInvenIdx < 0 || nInvenIdx >= m_vectorStorageItems.size())
		return -1;
	
	return m_vectorStorageItems[nInvenIdx]->Item_Sum;
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	//wooss 050811
	//extend inventory rect
	m_rcExInvenRect.SetRect( 0, 23, 216,50 );
	
	// Inventory region
	m_rcTop.SetRect( 4, 48, 190, 229 );			//wooss 24 -> 48 , 205 -> 229
	m_rcBottom.SetRect( 4, 232, 190, 306 );		//wooss 209 -> 232 , 282 -> 306

	// Create warehouse texture
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottomLine.SetUV( 0, 59, 216, 61, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );	

	// wooss 050811
	// extend inventory remain days
	m_rtExInven.SetUV( 0, 98, 216, 129, fTexWidth, fTexHeight);

	// Inventory
	m_rtTopTopInven.SetUV( 0, 131, 216, 205, fTexWidth, fTexHeight );		// 74
	m_rtTopMiddleInven.SetUV( 0, 206, 216, 240, fTexWidth, fTexHeight );	// 34
	m_rtTopBottomInven.SetUV( 0, 241, 216, 282, fTexWidth, fTexHeight );	// 41
	m_rtBottomInven.SetUV( 0, 283, 216, 358, fTexWidth, fTexHeight );
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );
	m_rtBlankInven.SetUV( 0, 387, 216, 393, fTexWidth, fTexHeight );
	m_rtPriceInven.SetUV( 0, 359, 216, 381, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 51, 250, 83, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buy button of warehouse
	m_btnWareHouseKeep.Create( this, _S( 812, "보관" ), 105, 362, 51, 21 );			//wooss 312->339		//jeil 339 -> 362
	m_btnWareHouseKeep.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseKeep.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseKeep.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseKeep.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Sell button of warehouse
	m_btnWareHouseTake.Create( this, _S( 813, "찾기" ), 105, 362, 51, 21 );			//wooss 312->339
	m_btnWareHouseTake.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseTake.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseTake.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseTake.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of warehouse
	m_btnWareHouseCancel.Create( this, _S( 139, "취소" ), 159, 362, 51, 21 );		//wooss 312->339
	m_btnWareHouseCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnWareHouseCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnWareHouseCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWareHouseCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	//입금 버튼
	m_btnInNas.Create( this, _S( 5906, "입금" ), 51, 362, 51, 21 );			
	m_btnInNas.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnInNas.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnInNas.CopyUV( UBS_IDLE, UBS_ON );
	m_btnInNas.CopyUV( UBS_IDLE, UBS_DISABLE );

	//출금 버튼
	m_btnOutNas.Create( this, _S( 5907, "출금" ), 51, 362, 51, 21 );	
	m_btnOutNas.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnOutNas.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnOutNas.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOutNas.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Top scroll bar
	m_sbTopScrollBar.Create( this, 194, 54, 9, 171 );			//wooss 28 -> 54
	m_sbTopScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbTopScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollRange( WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL );
	m_sbTopScrollBar.SetCurItemCount( WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL );	 //wooss
	m_sbTopScrollBar.SetItemsPerPage( WAREHOUSE_WAREHOUSE_SLOT_ROW );
	// Up button
	m_sbTopScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbTopScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbTopScrollBar.SetBarTopUV( 217, 16, 226, 24, fTexWidth, fTexHeight );	//wooss 25 -> 24
	m_sbTopScrollBar.SetBarMiddleUV( 217, 27, 226, 29, fTexWidth, fTexHeight );	//wooss 30 -> 29 
	m_sbTopScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );	//wooss
	// Wheel region
	m_sbTopScrollBar.SetWheelRect( -192, -6, 191, 148 );

	// Slot items
	// WareHouse Slot(5x4)
	for (int i = 0; i < ITEM_COUNT_IN_INVENTORY_STASH; ++i)
	{
		//m_pIconWareHouseItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM, 0, i );
		m_pIconWareHouseItems[i] = new CUIIcon();
		m_pIconWareHouseItems[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM);
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{
		//m_pIconTradeItems[iItem].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM );
		m_pIconTradeItems[iItem] = new CUIIcon();
		m_pIconTradeItems[iItem]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_WAREHOUSE, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIWareHouse::SetNPCPos( int nNPCIdx, FLOAT fX, FLOAT fZ )
{
	m_fNpcX			= fX;
	m_fNpcZ			= fZ;
	m_nNpcIndex		= nNPCIdx;
	
	if (CEntity* pEntity = INFO()->GetTargetEntity(eTARGET))
		m_nNPCVIdx = pEntity->GetNetworkID();
}

// ----------------------------------------------------------------------------
// Name : OpenWareHouse()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::OpenWareHouse( SBYTE sbSetPW, bool bCashRemoteOpen )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_WAREHOUSE_REQ ) || IsVisible())
		return;
	
	if(pUIManager->GetSecurity()->IsVisible() )
	{
		return;
	}
	
	// If inventory is already locked
	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		// 이미 Lock 인 창이 있을 경우 열지 못한다.
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}
	
	pUIManager->CloseMessageBox( MSGCMD_SECURITY_PASSWORD);
	pUIManager->CloseMessageBox( MSGCMD_CONFIRM_PASSWORD);
	pUIManager->CloseMessageBox( MSGCMD_OLD_PASSWORD);	

	// 패스워드 기능 삭제 -> Quest has 로 사용중
	BOOL bHasQuest = sbSetPW ? TRUE : FALSE;
	ResetWareHouse();

	// Set position of target npc
	//m_fNpcX = fX;
	//m_fNpcZ = fZ;

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_WAREHOUSE );

	pUIManager->CloseMessageBoxL( MSGLCMD_WAREHOUSE_REQ );

	// Create refine message box
	pUIManager->CreateMessageBoxL( _S( 814, "보관소" ) , UI_WAREHOUSE, MSGLCMD_WAREHOUSE_REQ );	

	if( m_nNpcIndex != -1 )
	{
		CTString	strNpcName = CMobData::getData(m_nNpcIndex)->GetName();
		pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	}

	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, _S( 815, "물품 보관을 원하십니까?" ) , -1, 0xA3A1A3FF );		
	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, TRUE, _S( 816, "저희 물품 보관소에서는 여행자님의 물품을 안전하게 보관하고 언제 어디서든지 저희 물품 보관소가 있는 곳이라면 물품을 찾으실 수 있습니다." ) , -1, 0xA3A1A3FF );		

	CTString strMessage;	
	strMessage.PrintF( _S( 817, "물품을 맡긴다." ) );			
	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage, WAREHOUSE_IN );

	strMessage.PrintF( _S( 818, "물품을 찾는다." ) );			
	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage, WAREHOUSE_OUT );

#ifdef	STASH_PASSWORD
	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, _S( 819, "암호를 변경한다." ), WAREHOUSE_CHANGEPW );
#endif	// STASH_PASSWORD

	if( bHasQuest )
	{
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_WAREHOUSE_REQ);				
	}

	strMessage.PrintF( _S( 1220, "취소한다." ) );		
	pUIManager->AddMessageBoxLString( MSGLCMD_WAREHOUSE_REQ, FALSE, strMessage );

	m_bCashRemote = bCashRemoteOpen;

	if (bCashRemoteOpen == true)
		m_nNPCVIdx = -1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::PrepareWareHouse()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Already exchange
	if( pUIManager->IsCSFlagOn( CSF_EXCHANGE ) )
	{
		pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
		pUIManager->GetChattingUI()->AddSysMessage( _S( 821, "교환중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// Already personal shop
	if( pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) )
	{
		pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
		pUIManager->GetChattingUI()->AddSysMessage( _S( 822, "개인 상점 사용중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	// Already shop
	if( pUIManager->IsCSFlagOn( CSF_SHOP ) )
	{
		pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
		pUIManager->GetChattingUI()->AddSysMessage( _S( 807, "상점 거래중에는 창고를 사용할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	RefreshWareHouse();

	/*
	int	iRow, iCol;
	int	nX = WAREHOUSE_BUY_TOP_SLOT_SX;
	int	nY = WAREHOUSE_BUY_TOP_SLOT_SY;
	int	iRowS = m_nCurRow;
	int	iRowE = iRowS + WAREHOUSE_WAREHOUSE_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		for( iCol = 0; iCol < WAREHOUSE_WAREHOUSE_SLOT_COL; iCol++, nX += 35 )
		{
			m_abtnWareHouseItems[iRow][iCol].SetPos( nX, nY );
		}
		nX = WAREHOUSE_BUY_TOP_SLOT_SX;	nY += 35;
	}
	*/

	int nX = WAREHOUSE_BUY_BOTTOM_SLOT_SX;
	int nY = WAREHOUSE_BUY_BOTTOM_SLOT_SY;
	// ---Trade slot items---
	for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
	{	
		if( iItem == WAREHOUSE_TRADE_SLOT_COL )
		{
			nY += 35;	nX = WAREHOUSE_BUY_BOTTOM_SLOT_SX;
		}
		m_pIconTradeItems[iItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_WAREHOUSE, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::ResetWareHouse()
{
	m_nSelWareHouseItemID	= -1;
	m_nSelTradeItemID		= -1;
	m_nSelectedWareHouseID	= -1;
	m_strTotalPrice = CTString( "0" );
	m_nTotalPrice	= 0;
	m_nNumOfItem	= 0;
	m_bHasQuest		= FALSE;

	m_strTotalNas		= CTString( "0" );
	m_strInNas			= CTString( "0" );
	m_strOutNas			= CTString( "0" );
	m_nInNas			= 0;
	m_nOutNas			= 0;
	m_nTotalNas			= 0;

	ClearItems();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_WAREHOUSE, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_WAREHOUSE );

	// Close message box of warehouse
	pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
	pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
	pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
	pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
	pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_TAKE_MONEY );		//2013/04/04 jeil 나스 아이템 제거 

	pUIManager->GetMsgBoxNumOnly()->CloseBox();

	m_nCurRow = 0;
	m_sbTopScrollBar.SetScrollPos( 0 );

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::RefreshWareHouse()
{
	for(int i = 0; i < ITEM_COUNT_IN_INVENTORY_STASH; i++)
	{
		m_pIconWareHouseItems[i]->clearIconData();
	}

	ASSERT(m_vectorStorageItems.size() <= WAREHOUSE_WAREHOUSE_SLOT_COL * WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL && 
		"Invalid Storge Count" );

	int t = 0;
	vec_Items_iter pos = m_vectorStorageItems.begin();
	vec_Items_iter end = m_vectorStorageItems.end();

	for (; pos != end; ++pos)
	{
		if ((*pos)->Item_Sum > 0)
		{
			m_pIconWareHouseItems[t]->setData((*pos));
			m_pIconWareHouseItems[t]->Hide(FALSE);
			
			t++;
		}
	}
// wooss 050812
// 아이템 25개를 넘으면 스크롤 확장되던 것을 아이템 개수에 상관없이 스크롤되게 변형
// wooss 060807 다시 풀어 줌....
	const int	ctWareHouseItems	= m_vectorStorageItems.size();	
	int	nScrollItem = ( ctWareHouseItems + WAREHOUSE_WAREHOUSE_SLOT_COL - 1 ) / WAREHOUSE_WAREHOUSE_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIWareHouse::ClearItems()
{
	vec_Items_iter iter = m_vectorStorageItems.begin();
	vec_Items_iter eiter = m_vectorStorageItems.end();

	for (; iter != eiter; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_vectorStorageItems.clear();

	iter = m_vecTradeItems.begin();
	eiter = m_vecTradeItems.end();
	
	for (; iter != eiter; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_vecTradeItems.clear();

	int		i;
	for( i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i )
		m_pIconTradeItems[i]->clearIconData();

	for( i = 0; i < ITEM_COUNT_IN_INVENTORY_STASH; ++i )
	{
		m_pIconWareHouseItems[i]->clearIconData();
	}
}

void CUIWareHouse::DelWareHouseItemToInventory()
{
	// 보관 모드에서만 동작
	if (m_bStorageWareHouse == FALSE)
		return;

	if(m_nSelTradeItemID < 0)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	int i, nCnt = m_vecTradeItems.size();

	for (i = 0; i < nCnt; ++i)
	{
		if (m_vecTradeItems[i]->Item_UniIndex == pItems->Item_UniIndex)
		{
			m_nTempTradeIdx = i;
			break;
		}
	}

	if (i == nCnt)
		return;

	m_pItemsTemp = pItems;

	DelWareHouseItem();
}

// ----------------------------------------------------------------------------
// Name : RenderWareHouseItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIWareHouse::RenderWareHouseItems()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int	nX = WAREHOUSE_BUY_TOP_SLOT_SX;
	int	nY = WAREHOUSE_BUY_TOP_SLOT_SY;

	int	i;
	int	iRowS = (m_nCurRow * WAREHOUSE_WAREHOUSE_SLOT_COL);
	int	iRowE = (m_nCurRow + WAREHOUSE_WAREHOUSE_SLOT_ROW) * WAREHOUSE_WAREHOUSE_SLOT_COL;
	
	for (i = iRowS; i < iRowE; ++i)
	{
		if ((i % WAREHOUSE_WAREHOUSE_SLOT_COL) == 0)
		{
			nX = WAREHOUSE_BUY_TOP_SLOT_SX;	
			if (i > iRowS)	nY += 35;
		}

		m_pIconWareHouseItems[i]->SetPos( nX, nY );
		nX += 35;

		if (m_pIconWareHouseItems[i]->IsEmpty() == true)
			continue;
			
		m_pIconWareHouseItems[i]->Render(pDrawPort);
	}

	// ---Trade slot items---
	int		iItem;
	for( iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconTradeItems[iItem]->IsEmpty() == true)
				continue;
		
		m_pIconTradeItems[iItem]->Render(pDrawPort);
	}	

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	pDrawPort->EndTextEx();

	// Set warehouse texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );	


	// Outline of selected item
	if( m_nSelTradeItemID >= 0 )
	{
		m_pIconTradeItems[m_nSelTradeItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelWareHouseItemID >= 0 )
	{
		m_pIconWareHouseItems[m_nSelWareHouseItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::Render()
{	
	if ( m_bCashRemote == false)
	{
		// Check distance
		FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
		FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

		if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
			ResetWareHouse();
	}	

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set warehouse texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nX, nY;

	// Add render regions
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 38,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 38,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 2,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );	
	pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 2,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackBottomLine.U0, m_rtBackBottomLine.V0,
										m_rtBackBottomLine.U1, m_rtBackBottomLine.V1,
										0xFFFFFFFF );	

	// Buy
	nX = m_nPosX;
	nY = m_nPosY + 22;

	//wooss 050811 ExInventory window
	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_DAYS_HEIGHT,
		m_rtExInven.U0, m_rtExInven.V0,
		m_rtExInven.U1, m_rtExInven.V1,
		0xFFFFFFFF );
	nY += WAREHOUSE_DAYS_HEIGHT;
	
	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_TOP_HEIGHT,
		m_rtTopTopInven.U0, m_rtTopTopInven.V0,
		m_rtTopTopInven.U1, m_rtTopTopInven.V1,
		0xFFFFFFFF );

	nY += WAREHOUSE_TOP_HEIGHT;

	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_MIDDLE_HEIGHT,
		m_rtTopMiddleInven.U0, m_rtTopMiddleInven.V0,
		m_rtTopMiddleInven.U1, m_rtTopMiddleInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_MIDDLE_HEIGHT;

	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_MIDDLE_HEIGHT,
		m_rtTopMiddleInven.U0, m_rtTopMiddleInven.V0,
		m_rtTopMiddleInven.U1, m_rtTopMiddleInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_MIDDLE_HEIGHT;

	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_BOTTOM_HEIGHT,
		m_rtTopBottomInven.U0, m_rtTopBottomInven.V0,
		m_rtTopBottomInven.U1, m_rtTopBottomInven.V1,
		0xFFFFFFFF );
	
	nY += WAREHOUSE_BOTTOM_HEIGHT;

	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 1,
		m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
		m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
		0xFFFFFFFF );
	
	nY += 1;
	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + WAREHOUSE_TRADE_HEIGHT,
		m_rtBottomInven.U0, m_rtBottomInven.V0,
		m_rtBottomInven.U1, m_rtBottomInven.V1,
		0xFFFFFFFF );

	nY += WAREHOUSE_TRADE_HEIGHT;

	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 23,
		m_rtPriceInven.U0, m_rtPriceInven.V0,
		m_rtPriceInven.U1, m_rtPriceInven.V1,
		0xFFFFFFFF );
	nY += 23;
	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 23,
		m_rtPriceInven.U0, m_rtPriceInven.V0,
		m_rtPriceInven.U1, m_rtPriceInven.V1,
		0xFFFFFFFF );

	nY += 23;
	pDrawPort->AddTexture( nX, nY,
		nX + WAREHOUSE_WAREHOUSE_WIDTH, nY + 1,
		m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
		m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
		0xFFFFFFFF );

	// Bottom scroll bar		
	m_sbTopScrollBar.Render();

	// Close button
	m_btnClose.Render();

	if( m_bStorageWareHouse )
	{
		// Buy & sell button of warehouse
		m_btnWareHouseKeep.Render();
		m_btnInNas.Render();	//2013/04/02 jeil 나스 아이템 제거
	}
	else
	{
		m_btnWareHouseTake.Render();
		m_btnOutNas.Render();	//2013/04/02 jeil 나스 아이템 제거
	}
	
	// Cancel button of warehouse
	m_btnWareHouseCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render item information
	pDrawPort->PutTextEx( _S( 823, "창고" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,	
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );

	if(m_bStorageWareHouse)
	{
		pDrawPort->PutTextEx( _S( 5908, "입금 금액" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX-8, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY );	
		pDrawPort->PutTextExRX( m_strInNas, m_nPosX + WAREHOUSE_TRADEPRICE_POSX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY, pUIManager->GetNasColor( m_strInNas ) );
		pDrawPort->PutTextEx( _S( 824, "보관료" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX+10, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY+23 );	
		pDrawPort->PutTextExRX( m_strTotalPrice, m_nPosX + WAREHOUSE_TRADEPRICE_POSX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY+23, pUIManager->GetNasColor( m_strTotalPrice ) );
	}
	else
	{
		pDrawPort->PutTextEx( _S( 5909, "보관 금액" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX-8, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY );	
		pDrawPort->PutTextExRX( m_strTotalNas, m_nPosX + WAREHOUSE_TRADEPRICE_POSX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY, pUIManager->GetNasColor( m_strTotalNas ) );
		pDrawPort->PutTextEx( _S( 5910, "출금 금액" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX-8, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY+23 );	
		pDrawPort->PutTextExRX( m_strOutNas, m_nPosX + WAREHOUSE_TRADEPRICE_POSX, m_nPosY + WAREHOUSE_BUY_TRADEPRICE_POSY+23, pUIManager->GetNasColor( m_strOutNas ) );
	}
	// -----------------------------------------------------------<<
	
	CTString tv_str;

	if (m_useTime > 0)
	{
		tm* pTime = NULL;

		pTime = localtime((time_t*)&m_useTime);

		tv_str.PrintF(  _S( 6070,"만료 : %d년%d월%d일%d시%d분"),pTime->tm_year + 1900
			,pTime->tm_mon + 1,pTime->tm_mday,pTime->tm_hour, pTime->tm_min);
	}

	pDrawPort->PutTextExCX(tv_str, m_nPosX + DEF_EXPANSION_TEXT_AREA / 2, m_nPosY + WAREHOUSE_EXTEND_TEXT_POSY ,DEF_UI_COLOR_WHITE);
	
	{
		// 창고 현재량 / 저장 가능량 출력
		CTString tmp;
		int		cap =  150;

		if (m_useTime > 0)
			cap = 300;

		tmp.PrintF("(%d / %d)", m_vectorStorageItems.size(), cap);
		pDrawPort->PutTextEx(tmp, m_nPosX + 148, m_nPosY + 40, DEF_UI_COLOR_WHITE);
	}
	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render Items
	RenderWareHouseItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIWareHouse::MouseMessage( MSG *pMsg )
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

			// Move warehouse
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				// WareHouse items
				if( m_nSelWareHouseItemID >= 0 )
				{					
					// Close message box of warehouse
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_TAKE_MONEY );		//2013/04/04 jeil 나스 아이템 제거 
					
					pUIManager->SetHoldBtn(m_pIconWareHouseItems[m_nSelWareHouseItemID]);
					
					pUIManager->GetMsgBoxNumOnly()->CloseBox();
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of warehouse
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
					pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_TAKE_MONEY );		//2013/04/04 jeil 나스 아이템 제거 

					pUIManager->SetHoldBtn(m_pIconTradeItems[m_nSelTradeItemID]);

					pUIManager->GetMsgBoxNumOnly()->CloseBox();
				}

				bLButtonDownInItem	= FALSE;
			}

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Buy button of warehouse
			else if( m_bStorageWareHouse && m_btnWareHouseKeep.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Sell button of warehouse
			else if( !m_bStorageWareHouse && m_btnWareHouseTake.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of warehouse
			else if( m_btnWareHouseCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Top Scroll bar
			else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			else if(m_bStorageWareHouse && m_btnInNas.MouseMessage( pMsg ) != WMSG_FAIL)	//2013/04/02 나스 아이템 제거
			{
				return WMSG_SUCCESS;
			}
			else if(!m_bStorageWareHouse && m_btnOutNas.MouseMessage( pMsg ) != WMSG_FAIL)	//2013/04/02 나스 아이템 제거
			{
				return WMSG_SUCCESS;
			}
			else
			{
				int	i;
				int	iRowS = m_nCurRow * WAREHOUSE_WAREHOUSE_SLOT_COL;								// Start Row
				int	iRowE = (m_nCurRow + WAREHOUSE_WAREHOUSE_SLOT_ROW) * WAREHOUSE_WAREHOUSE_SLOT_COL;	// End Row
				
				for (i = iRowS; i < iRowE; ++i)
				{
					m_pIconWareHouseItems[i]->MouseMessage( pMsg );
				}

				int	iItem;
				for( iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
				{
					m_pIconTradeItems[iItem]->MouseMessage( pMsg );
				}
				return WMSG_FAIL;
			}
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
				// Buy button of warehouse
				else if( m_bStorageWareHouse && m_btnWareHouseKeep.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Sell button of warehouse
				else if( !m_bStorageWareHouse && m_btnWareHouseTake.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of warehouse
				else if( m_btnWareHouseCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if(m_bStorageWareHouse && m_btnInNas.MouseMessage( pMsg ) != WMSG_FAIL)	//2013/04/02 나스 아이템 제거
				{
				
					//입금 버튼
				}
				else if(!m_bStorageWareHouse && m_btnOutNas.MouseMessage( pMsg ) != WMSG_FAIL)	//2013/04/02 나스 아이템 제거
				{
				
					//출금 버튼
				}
				// WareHouse items
				else if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelWareHouseItemID = -1;
					m_nSelTradeItemID = -1;

					int	i;
					int	iRowS = m_nCurRow * WAREHOUSE_WAREHOUSE_SLOT_COL;
					int	iRowE = (m_nCurRow + WAREHOUSE_WAREHOUSE_SLOT_ROW) * WAREHOUSE_WAREHOUSE_SLOT_COL;
					
					for( i = iRowS; i < iRowE; i++ )
					{
						if (m_pIconWareHouseItems[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Update selected item
							m_nSelWareHouseItemID = i;	// 선택된 슬롯의 아이템 ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_WAREHOUSE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					m_nSelTradeItemID = -1;
					m_nSelWareHouseItemID = -1;

					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconTradeItems[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// 선택된 슬롯의 아이템 ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_WAREHOUSE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}				

				pUIManager->RearrangeOrder( UI_WAREHOUSE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInItem = FALSE;

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If warehouse isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Buy button of warehouse
				else if( m_bStorageWareHouse && ( wmsgResult = m_btnWareHouseKeep.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 KeepItems();
						 
						 // Clear Member Value
						 //ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Sell button of warehouse
				else if( !m_bStorageWareHouse && ( wmsgResult = m_btnWareHouseTake.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 TakeItems();

						 // Clear Member Value
						 //ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				else if(m_bStorageWareHouse && ( wmsgResult = m_btnInNas.MouseMessage( pMsg ) ) != WMSG_FAIL )	//2013/04/02 나스 아이템 제거
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						InNas();
					}
					return WMSG_SUCCESS;
					//입금 버튼
				}
				else if(!m_bStorageWareHouse && ( wmsgResult = m_btnOutNas.MouseMessage( pMsg ) ) != WMSG_FAIL )	//2013/04/02 나스 아이템 제거
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						OutNas();
					}
					return WMSG_SUCCESS;
					//출금 버튼
				}
				// Cancel button of warehouse
				else if( ( wmsgResult = m_btnWareHouseCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetWareHouse();
					}

					return WMSG_SUCCESS;
				}
				// Top Scroll bar
				else if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//wooss 050812
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					// Nothing
					return WMSG_SUCCESS;
				}				
				// WareHouse items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	i;
					int	iRowS = m_nCurRow * WAREHOUSE_WAREHOUSE_SLOT_COL;
					int	iRowE = (m_nCurRow + WAREHOUSE_WAREHOUSE_SLOT_ROW) * WAREHOUSE_WAREHOUSE_SLOT_COL;
					
					for( i = iRowS; i < iRowE; i++ )
					{
						if (m_pIconWareHouseItems[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconTradeItems[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
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
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					// If holding button is item and is from warehouse
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_WAREHOUSE )
					{
						// Trade items
						if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from warehouse slot
							if (m_nSelTradeItemID < 0 ||
								m_pIconTradeItems[m_nSelTradeItemID]->getIndex() != pDrag->getIndex())
							{
								AddWareHouseItem(m_nSelWareHouseItemID);

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// WareHouse items
						else if( !m_bStorageWareHouse && IsInsideRect( nX, nY, m_rcTop ) )
						{
							// If this item is moved from trade slot
							if (m_nSelWareHouseItemID < 0 ||
								m_pIconWareHouseItems[m_nSelWareHouseItemID]->getIndex() != pDrag->getIndex())
							{
 								m_nTempTradeIdx = m_nSelTradeItemID;
								m_pItemsTemp = m_vecTradeItems[m_nTempTradeIdx];

								int nCnt = m_vectorStorageItems.size();

								for (int i = 0; i < nCnt; ++i)
								{
									if (m_vectorStorageItems[i]->Item_UniIndex == m_pItemsTemp->Item_UniIndex)
									{
										m_nTempStorageIdx = i;
										break;
									}
								}

								DelWareHouseItem();

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item
					else if (pDrag->getBtnType() == UBET_ITEM &&
							 pDrag->GetWhichUI() == UI_INVENTORY && m_bStorageWareHouse)
					{
						// Trade items
						if( IsInsideRect( nX, nY, m_rcBottom ) )
						{
							AddWareHouseItemFromInventory( );

							// Reset holding button
							pUIManager->ResetHoldBtn();

							return WMSG_SUCCESS;
						}
					}

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
					int	nTradeItemID = m_nSelTradeItemID;
					m_nSelTradeItemID = -1;
					CUIManager* pUIManager = CUIManager::getSingleton();

					for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconTradeItems[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Close message box of warehouse
							pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
							pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
							pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
							pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
							pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_TAKE_MONEY );		//2013/04/04 jeil 나스 아이템 제거 

							pUIManager->GetMsgBoxNumOnly()->CloseBox();

							if (iItem >= m_vecTradeItems.size())
								return WMSG_FAIL;

							m_pItemsTemp = m_vecTradeItems[iItem];
							m_nTempTradeIdx = nTradeItemID;

							int nCnt = m_vectorStorageItems.size();

							for (int i = 0; i < nCnt; ++i)
							{
								if (m_vectorStorageItems[i]->Item_UniIndex == m_pItemsTemp->Item_UniIndex)
								{
									m_nTempStorageIdx = i;
									break;
								}
							}

							DelWareHouseItem();
							
							return WMSG_SUCCESS;
						}
					}
				}
				// WareHouse items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelWareHouseItemID = -1;
					CUIManager* pUIManager = CUIManager::getSingleton();

					int	i;
					int	iRowS = m_nCurRow * WAREHOUSE_WAREHOUSE_SLOT_COL;
					int	iRowE = (m_nCurRow + WAREHOUSE_WAREHOUSE_SLOT_ROW) * WAREHOUSE_WAREHOUSE_SLOT_COL;
					
					for( i = iRowS; i < iRowE; i++ )
					{
						if (m_pIconWareHouseItems[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							if( !m_bStorageWareHouse )
							{
								{
									// Close message box of warehouse
									pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_ITEM );
									pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_DEL_ITEM );
									pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_PLUSITEM );
									pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_ADD_MONEY );
									pUIManager->CloseMessageBox( MSGCMD_WAREHOUSE_TAKE_MONEY );		//2013/04/04 jeil 나스 아이템 제거 

									pUIManager->GetMsgBoxNumOnly()->CloseBox();

									AddWareHouseItem(i);
								}
							}

							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Top Scroll bar
				if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}				
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIWareHouse::AddWareHouseItem( int nIndex )
{
	if (nIndex < 0 || nIndex >= m_vectorStorageItems.size())
		return;

	m_nTempStorageIdx = nIndex;

	CUIManager* pUIManager = CUIManager::getSingleton();
	m_pItemsTemp = m_vectorStorageItems[nIndex];
	CItemData*	pItemData = m_vectorStorageItems[nIndex]->ItemData;
	int		nItemIndex = m_vectorStorageItems[nIndex]->Item_Index;
	SQUAD	nCount = m_vectorStorageItems[nIndex]->Item_Sum;
	int		nItemPlus = m_vectorStorageItems[nIndex]->Item_Plus;
	m_nTempMax = -1;

	// Ask quantity
	if ((m_bStorageWareHouse && (pItemData->GetFlag() & ITEM_FLAG_COUNT)) ||
		(!m_bStorageWareHouse && (pItemData->GetFlag() & ITEM_FLAG_COUNT) && nCount > 1))
	{
		CTString	strMessage;
		const char	*szItemName = _pNetwork->GetItemName(nItemIndex);

		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			strMessage.PrintF( _S( 1323, "몇 나스를 옮기시겠습니까?" ) );

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
				UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_ITEM );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			pUIManager->GetMsgBoxNumOnly()->CloseBox();
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdWareHouseAddItem* pCmd = new CmdWareHouseAddItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 823, "창고" ), strMessage, 1, nCount);
		}
	}
	else if (nItemPlus > 0 && !(pUIManager->IsPet(pItemData) || pUIManager->IsWildPet(pItemData) ||
			 (pItemData->GetType() == CItemData::ITEM_ETC && 
			  (pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || nItemIndex == 6941))))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_YESNO,		
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		const char*	szItemName = _pNetwork->GetItemName(nItemIndex);
		strMessage.PrintF(_S2(264, szItemName, "[%s +%d]<를> 옮기시겠습니까?"), szItemName, nItemPlus);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		WareHouseToTrade();
	}
}

// ----------------------------------------------------------------------------
// Name : AddWareHouseItemFromInventory()
// Desc : From inventory to trade
// ----------------------------------------------------------------------------
void CUIWareHouse::AddWareHouseItemFromInventory()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	if (pItems->Item_Wearing >= 0) // 착용중인 장비일 경우 등록 되면 안된다.
		return;

	m_pItemsTemp = pItems;
		
	CItemData*	pItemData	= m_pItemsTemp->ItemData;
	const char* szItemName	= pItemData->GetName();

	int		nItemIndex = m_pItemsTemp->Item_Index;
	int		nItemPlus = m_pItemsTemp->Item_Plus;
	ULONG	ulItemFlag = m_pItemsTemp->Item_Flag;
	m_nTempMax = m_pItemsTemp->Item_Sum;

	// Ask quantity
	if( ( m_bStorageWareHouse && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && m_nTempMax > 1) ||
		( !m_bStorageWareHouse && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && m_nTempMax > 1 ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;

		// 나스의 경우.
		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
				UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_MONEY );
			//strMessage.PrintF( _S( 826, "보관하실 나스를 천 단위로 입력해 주십시요. 보관료는 %d%%입니다." ), m_nTex );		
			strMessage.PrintF(  _S( 960, "보관하실 나스를 입력해 주십시오." )  );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdWareHouseAddItem* pCmd = new CmdWareHouseAddItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 823, "창고" ), strMessage, 1, m_nTempMax);
		}		
	}
	else if ((pItemData->GetFlag() & ITEM_FLAG_ORIGIN) && (ulItemFlag& FLAG_ITEM_BELONG))
	{
		CTString tv_str;
		tv_str.PrintF(_S(2578, "저장할 수 없는 아이템[%s] 입니다." ),pItemData->GetName() );
		pUIManager->GetChattingUI()->AddSysMessage( tv_str, SYSMSG_ERROR );
		return;
	}
	else if (nItemPlus > 0 && !(pUIManager->IsPet(pItemData) || pUIManager->IsWildPet(pItemData) ||
			pItemData->GetType() == CItemData::ITEM_ETC && (pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || nItemIndex == 6941)))
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_YESNO,		
									UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_PLUSITEM );
		//const char	*szItemName = pItemData->GetName();
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, nItemPlus );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		WareHouseToTrade();
	}
}

// ----------------------------------------------------------------------------
// Name : DelWareHouseItem()
// Desc : From trade to warehouse
// ----------------------------------------------------------------------------
void CUIWareHouse::DelWareHouseItem()
{
	if (m_pItemsTemp == NULL)
		return;

	CItemData	*pItemData = m_pItemsTemp->ItemData;

	if (pItemData == NULL)
		return;
	
	// Ask quantity
	if (m_pItemsTemp->Item_Sum > 1)
	{
		CTString	strMessage;
		const char	*szItemName = _pNetwork->GetItemName(m_pItemsTemp->Item_Index);

		if (pItemData->GetType() != CItemData::ITEM_ETC ||
			pItemData->GetSubType() != CItemData::ITEM_ETC_MONEY)
		{
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdWareHouseDelItem* pCmd = new CmdWareHouseDelItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 823, "창고" ), strMessage, 1, m_pItemsTemp->Item_Sum);
		}
	}
	else
	{
		TradeToWareHouse();
	}
}

// ----------------------------------------------------------------------------
// Name : KeepItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::KeepItems()
{
	int		i;
	// Buy nothing
	if( m_nTotalPrice < 0 && m_nInNas <= 0 )
	{
		ErrMessage(eERR_KEEP_FAIL_EMPTY);
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Can't stash items
	
	for (i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; i++)
	{
		// Add system message
		CTString tv_str;
		CItems*		pItems = m_pIconTradeItems[i]->getItems();

		if (pItems == NULL)
			continue;

		CItemData*	pItemData = pItems->ItemData;

		if (pItemData == NULL)
			continue;

#ifdef ADD_SUBJOB

		if (pItemData->IsFlag(ITEM_FLAG_SELLER) ?
			!pUIManager->CheckSellerItem(UI_WAREHOUSE, pItemData->GetFlag()) :
			(pItemData->GetFlag() & ITEM_FLAG_NO_STASH) ||
			(pItemData->GetFlag() & ITEM_FLAG_ORIGIN && pItems->Item_Flag & FLAG_ITEM_BELONG) || 
			pItemData->GetFlag() & ITEM_FLAG_ABS)
#else
		if ((pItemData->GetFlag() & ITEM_FLAG_NO_STASH) ||
			(pItemData->GetFlag() & ITEM_FLAG_ORIGIN && pItems->Item_Flag & FLAG_ITEM_BELONG) || 
			pItemData->GetFlag() & ITEM_FLAG_ABS)
#endif
		{
			tv_str.PrintF(_S(2578, "저장할 수 없는 아이템[%s] 입니다."), pItemData->GetName());
			pUIManager->GetChattingUI()->AddSysMessage(tv_str, SYSMSG_ERROR);
			return;
		}

	}

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		pUIManager->GetChattingUI()->AddSysMessage( _S( 827, "나스가 부족하여 물품을 보관할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	// Check inventory space
	int	ctEmptySlot = 0;
	int	ctTradeItems = m_nNumOfItem;
	
	for( i = 0; i < ITEM_COUNT_IN_INVENTORY_STASH; i++ )
	{
		if (m_pIconWareHouseItems[i]->IsEmpty())
		{
			ctEmptySlot++;
			continue;
		}

		int	nIndex = m_pIconWareHouseItems[i]->getIndex();
		CItemData*	pItemData = CItemData::getData(nIndex);

		if (pItemData->GetFlag() & ITEM_FLAG_COUNT)
		{
			for( int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
			{
				if (nIndex == m_pIconTradeItems[iItem]->getIndex())
					ctTradeItems--;
			}
		}
	}

	// Not enough space
	if( ctEmptySlot < ctTradeItems )
	{
		// Add system message
		pUIManager->GetChattingUI()->AddSysMessage( _S( 265, "인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	//_pNetwork->BuyItem( m_nSelectedWareHouseID, m_nNumOfItem, m_nTotalPrice );
	SendWareHouseKeepReq();
}

// ----------------------------------------------------------------------------
// Name : TakeItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIWareHouse::TakeItems()
{
	// Sell nothing
	if( m_nTotalPrice < 0 && m_nOutNas <= 0 )
	{
		ErrMessage(eERR_TAKE_FAIL_EMPTY);
		return;
	}

	//_pNetwork->SellItem( m_nSelectedWareHouseID, m_nNumOfItem, m_nTotalPrice );
	SendWareHouseTakeReq();
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
//		cpp2angel edit (04.12.18)
// ----------------------------------------------------------------------------
void CUIWareHouse::MsgBoxLCommand( int nCommandCode, int nResult )
{
	if( nCommandCode != MSGLCMD_WAREHOUSE_REQ )
		return;

	// [090527: selo] 확장팩 퀘스트 이야기 한다 처리 수정을 위한 루틴
	int iQuestIndex = -1;
	if( ciQuestClassifier < nResult )	
	{
		iQuestIndex = nResult;
		nResult = ciQuestClassifier;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch ( nResult ) 
	{
	case WAREHOUSE_IN: // 물품을 맡긴다
		{
			if (pUIManager->GetInventory()->IsLocked() == TRUE ||
				pUIManager->GetInventory()->IsLockedArrange() == TRUE)
			{
				// 이미 Lock 인 창이 있을 경우 열지 못한다.
				pUIManager->GetInventory()->ShowLockErrorMessage();
				return;
			}

			m_bStorageWareHouse			= TRUE;

			if (m_bHasPassword)
			{
				pUIManager->GetSecurity()->OpenSecurity(TRUE);
			}
			else
			{
				SendListReq(CTString(""));
			}
			
			// [KH_07043] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3313, "캐릭터의 인벤토리 창에서 창고에 보관 하고자 하는 물품을 클릭 후 창고의 하단 빈칸에 드래그 하여 올려 놓습니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3314, "보관버튼을 누르면 보관료만큼 자신의 인벤토리에서 나스가 빠져나가고 물품은 창고에 보관 됩니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3315, "※ 보관료는 맡기려는 물품 1개당 100나스 입니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3316, "※ 흰색 테두리가 둘러진 아이템은 창고에 보관이 불가능합니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3317, "※ 창고에 보관 가능한 공간은 기본 50칸이며, 창고확장카드를 사용하면 150칸으로 확장할 수 있습니다."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}		
		}
		break;
		
	case WAREHOUSE_OUT: // 물품을 찾는다
		{
			if (pUIManager->GetInventory()->IsLocked() == TRUE ||
				pUIManager->GetInventory()->IsLockedArrange() == TRUE)
			{
				// 이미 Lock 인 창이 있을 경우 열지 못한다.
				pUIManager->GetInventory()->ShowLockErrorMessage();
				return;
			}

			m_bStorageWareHouse			= FALSE;
			
			if (m_bHasPassword)
			{
				pUIManager->GetSecurity()->OpenSecurity(TRUE);
			}
			else
			{
				SendListReq(CTString(""));
			}

			// [KH_07043] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3318, "창고에 보관된 물품 중 찾으려는 물품을 창고 창 아래의 빈칸으로 옮기고 찾기 버튼을 클릭 합니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3319, "찾은 물품은 자신의 인벤토리에 옮겨지게 됩니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3320, "※ 최대 10개의 아이템을 한번에 찾을 수 있습니다."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}
		}
		break;
		
	case WAREHOUSE_TALK:
		{
			// 퀘스트 창 띄우기
			//pUIManager->GetQuest()->OpenQuest( pUIManager->GetCharacterInfo()->GetMobIndex(), m_fNpcX, m_fNpcZ );
			CUIQuestBook::TalkWithNPC();
		}
		break;
		
	case WAREHOUSE_CHANGEPW : // 암호를 변경한다.
		{
			pUIManager->CloseMessageBoxL( MSGLCMD_SECURITY_REQ );
			
			pUIManager->CreateMessageBoxL( _S( 1743, "창고 보안설정" ) , UI_SECURITY, MSGLCMD_SECURITY_REQ );		
			
			pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, _S( 1778, "창고 보안을 위하여 암호를 설정 하실 수 있습니다." ), -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, CTString("  "), -1, 0xA3A1A3FF );
			pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, TRUE, _S( 1779, "암호를 설정하면 창고를 이용할 때 설정하신 암호를 입력하여야만 창고를 이용할 수 있습니다." ), -1, 0xA3A1A3FF );	
			
			pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1780, "암호 설정" ), CUISecurity::SET_PASSWORD );
			if( m_bHasPassword )
			{
				pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1781, "암호 분실" ), CUISecurity::UNSET_PASSWORD );
			}
			pUIManager->AddMessageBoxLString( MSGLCMD_SECURITY_REQ, FALSE, _S( 1220, "취소한다." ) );
		}
		break;	
		
		// [090527: selo] 확장팩 퀘스트 수정
	case ciQuestClassifier:
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
		}
		break;
		
	default:
		{
			// Character state flags
			pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
		}
		break;
	}
}

static SQUAD	llTempNas = 0;

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIWareHouse::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_WAREHOUSE_ADD_ITEM:
		{
 			char	*pcInput	= strInput.str_String;
 			int		nLength		= strInput.Length();
 			int		iChar;
 			for( iChar = 0; iChar < nLength; iChar++ )
 			{
 				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
 					break;
 			}
 
 			if( iChar == nLength )
 			{
 				SQUAD	llCount = _atoi64( pcInput );
 				
				if (llCount > 0)
 				{
 					if (llCount > m_pItemsTemp->Item_Sum)
 						llCount = m_pItemsTemp->Item_Sum;
 
 					CItemData* pItemData = m_pItemsTemp->ItemData;
 					
					if (llCount > pItemData->GetStack())
 						llCount = pItemData->GetStack();
 
 					WareHouseToTrade(llCount);
 				} 				
 			}
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_PLUSITEM:
		{
			WareHouseToTrade();
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_MONEY:	//2013/04/04 jeil 나스 아이템 제거 
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				//if( llCount > 0 && llCount <= llTempCount )
				if( (llCount+m_nInNas) > 0 && (llCount+m_nInNas) <= _pNetwork->MyCharacterInfo.money)
				{
					llTempNas		= llCount;
					//SendWareHouseKeepReqNas( llCount);
					m_nInNas		+= llCount;
					m_strInNas.PrintF( "%I64d", m_nInNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strInNas );
				
				}
				else
				{
					CTString strInNasError;
					strInNasError.PrintF( _S( 5911, "현재 가지고 있는 나스 보다 많은 나스를 입금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strInNasError );
				}
			}
		}
		break;

	case MSGCMD_WAREHOUSE_ADD_REQ:
		{
			WareHouseToTrade();
		}
		break;

	case MSGCMD_WAREHOUSE_ERROR:
		{
		}
		break;

	case MSGCMD_WAREHOUSE_DEL_ITEM:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if (iChar == nLength)
			{
				SQUAD	llCount = _atoi64( pcInput );
				
				if (llCount > 0 && llCount <= m_pItemsTemp->Item_Sum)
					TradeToWareHouse();
			}
		}
		break;
	case MSGCMD_WAREHOUSE_TAKE_MONEY:	//2013/04/04 jeil 나스 아이템 제거 
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}
			
			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				if( (llCount+m_nOutNas) > 0 && (llCount+m_nOutNas) <= m_nTotalNas )
				{
					llTempNas		= llCount;
					//SendWareHouseTakeReqNas( llCount);
					m_nOutNas		+= llCount;
					m_strOutNas.PrintF( "%I64d", m_nOutNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strOutNas );
					
				}
				else
				{
					CTString strOutNasError;
					strOutNasError.PrintF( _S( 5912, "현재 입금 되어 있는 나스 보다 많은 나스를 출금 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strOutNasError );
				}
			}
		}
		
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::WareHouseToTrade( SQUAD llCount )
{	
	// Find same item in trade slot
	int		iItem;
	for (iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++)
	{
		if (m_pIconTradeItems[iItem]->getItems() != NULL &&
			m_pIconTradeItems[iItem]->getItems()->Item_UniIndex == m_pItemsTemp->Item_UniIndex)
			break;
	}

	// Check if item is countable
	CItemData*	pItemData = m_pItemsTemp->ItemData;

	// If there is same item
	if (iItem < WAREHOUSE_TRADE_SLOT_TOTAL)
	{
		CItems* pItems = m_pIconTradeItems[iItem]->getItems();

		if (pItems == NULL)
			return;

		if (pItemData->GetFlag() & ITEM_FLAG_COUNT)
		{
			// Update count of trade item
			SQUAD	llNewCount	= pItems->Item_Sum;
			llNewCount			+= llCount;

			// 인벤에 가고 있는 갯수보다 많은 갯수의 아이템을 보관하려 할때...
			if (m_bStorageWareHouse && m_nTempMax > 0)
			{
				if( llNewCount > m_nTempMax )
					llNewCount = m_nTempMax;
			}
			
			m_pIconTradeItems[iItem]->setCount(llNewCount);
												
			if (m_bStorageWareHouse == FALSE)
			{
				// Update count of warehouse item
				llNewCount = m_vectorStorageItems[m_nTempStorageIdx]->Item_Sum;
				llNewCount -= llCount;
				
				m_pIconWareHouseItems[m_nTempStorageIdx]->setCount(llNewCount);

				if( llNewCount <= 0 )
				{
					m_pIconWareHouseItems[m_nTempStorageIdx]->Hide(TRUE);

					// Unselect item
					if (m_nSelWareHouseItemID == m_nTempStorageIdx)
						m_nSelWareHouseItemID = -1;
				}
			}

			m_nTotalPrice = CalculateStoragePrice();
			m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
			CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );

			m_pItemsTemp = NULL;
			return;
		}
		else
		{
			if(m_bStorageWareHouse)
			{
				m_nSelWareHouseItemID = -1;
				m_pItemsTemp = NULL;
				return;
			}
		}
	}
	else
	{
		// Find empty slot
		for( iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconTradeItems[iItem]->IsEmpty())
				break;
		}

		// If there is not empty slot
		if( iItem == WAREHOUSE_TRADE_SLOT_TOTAL )
		{
			CTString	strMessage;

			// Add system message
			if( m_bStorageWareHouse )
				strMessage.PrintF( _S( 831, "한번에 최대 %d개의 아이템을 보관하실 수 있습니다." ), WAREHOUSE_TRADE_SLOT_TOTAL );	
			else
				strMessage.PrintF( _S( 832, "한번에 최대 %d개의 아이템을 찾으실 수 있습니다." ), WAREHOUSE_TRADE_SLOT_TOTAL );		

			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
			return;
		}

		if (iItem <= m_vecTradeItems.size())
		{
			CItems* pNewItems = new CItems(m_pItemsTemp->Item_Index);
			m_vecTradeItems.push_back(pNewItems);
		}

		CItems* pItems = m_vecTradeItems[iItem];
		memcpy(pItems, m_pItemsTemp, sizeof(CItems));
		m_pIconTradeItems[iItem]->setData(pItems);
		m_pIconTradeItems[iItem]->setCount(llCount);

		if (m_bStorageWareHouse == FALSE)
		{												
			// Update count of warehouse item
			SQUAD	llNewCount = m_vectorStorageItems[m_nTempStorageIdx]->Item_Sum;
			llNewCount -= llCount;

			m_pIconWareHouseItems[m_nTempStorageIdx]->setCount(llNewCount);			
			
			if( llNewCount <= 0 )
			{
				m_pIconWareHouseItems[m_nTempStorageIdx]->Hide(TRUE);

				// Unselect item
				if (m_nSelWareHouseItemID == m_nTempStorageIdx)
					m_nSelWareHouseItemID = -1;
			}
		}

		m_nTotalPrice = CalculateStoragePrice();
		m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
		CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );

		m_pItemsTemp = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::TradeToWareHouse( SQUAD llCount )
{
	if (m_pItemsTemp == NULL)
		return;

	// Update count of trade item
	SQUAD llTrade = m_vecTradeItems[m_nTempTradeIdx]->Item_Sum - llCount;
	m_pIconTradeItems[m_nTempTradeIdx]->setCount(llTrade);

	if (m_vecTradeItems[m_nTempTradeIdx]->Item_Sum <= 0)
	{
		// Rearrange buttons
		for( int iArrItem = m_nTempTradeIdx; iArrItem < WAREHOUSE_TRADE_SLOT_TOTAL; iArrItem++ )
		{
			if ((iArrItem + 1) == WAREHOUSE_TRADE_SLOT_TOTAL)
			{
				m_pIconTradeItems[iArrItem]->clearIconData();				
				break;
			}

			if (m_pIconTradeItems[iArrItem + 1]->IsEmpty())
			{
				m_pIconTradeItems[iArrItem]->clearIconData();
				break;
			}

			m_pIconTradeItems[iArrItem]->setData(m_pIconTradeItems[iArrItem + 1]->getItems());
		}

		if (m_bStorageWareHouse == FALSE)
		{
			// Update count of warehouse item
			if (m_nTempStorageIdx >= m_vectorStorageItems.size())
			{
				m_vectorStorageItems.push_back(m_vecTradeItems[m_nTempTradeIdx]);
			}
			else
			{
				if (m_vectorStorageItems[m_nTempStorageIdx] == NULL)
					m_vectorStorageItems[m_nTempStorageIdx] = m_vecTradeItems[m_nTempTradeIdx];
				else
					m_vectorStorageItems[m_nTempStorageIdx]->Item_Sum += m_pItemsTemp->Item_Sum;
			}

			m_pIconWareHouseItems[m_nTempStorageIdx]->setData(m_vectorStorageItems[m_nTempStorageIdx]);
		}
		else
		{
			SAFE_DELETE(m_vecTradeItems[m_nTempTradeIdx]);
		}

		m_vecTradeItems.erase(m_vecTradeItems.begin() + m_nTempTradeIdx);

		// Unselect item
		if (m_nTempTradeIdx == m_nSelTradeItemID)
			m_nSelTradeItemID = -1;
	}

	if (m_bStorageWareHouse == FALSE)
	{
		SQUAD llNewCount = m_vectorStorageItems[m_nTempStorageIdx]->Item_Sum + llCount;
		m_pIconWareHouseItems[m_nTempStorageIdx]->setCount(llNewCount);

		if (m_vectorStorageItems[m_nTempStorageIdx]->Item_Sum > 0)
			m_pIconWareHouseItems[m_nTempStorageIdx]->Hide(FALSE);
	}

	m_nTotalPrice = CalculateStoragePrice();
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );

	m_pItemsTemp = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 보관료를 계산합니다.
// Input  :
//-----------------------------------------------------------------------------
SQUAD CUIWareHouse::CalculateStoragePrice()
{
	SQUAD llTotalPrice		= 0;
	SQUAD llItemCnt			= 0;
	const int nTax			= 100;	// 아이템 한개의 세금 100원.
	
	for (int iItem = 0; iItem < WAREHOUSE_TRADE_SLOT_TOTAL; iItem++)
	{	
		if (m_pIconTradeItems[iItem]->IsEmpty())
			continue;

		llItemCnt		   += m_vecTradeItems[iItem]->Item_Sum;
	}

	llTotalPrice += llItemCnt * nTax;
	
	return llTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseKeepReq
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::SendWareHouseKeepReq()
{
	LONG lKeepCount = 0;
	int		i;
	
	for( i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i )
	{
		if (m_pIconTradeItems[i]->IsEmpty())
			continue;
		lKeepCount++;
	}

	if(m_nInNas == 0 && lKeepCount <= 0)
	{
		return;
	}

	CNetworkMessage nmMessage;
	RequestClient::doStashKeep* packet = reinterpret_cast<RequestClient::doStashKeep*>(nmMessage.nm_pubMessage);
	packet->type = MSG_STASH;
	packet->subType = MSG_STASH_KEEP;
#ifdef	STASH_PASSWORD
	memset(packet->password, 0, MAX_STASH_PASSWORD_LENGTH+1);
	int nSize = m_strPW.Length();
	if (nSize > MAX_STASH_PASSWORD_LENGTH)
		nSize = MAX_STASH_PASSWORD_LENGTH;
	memcpy(packet->password, m_strPW.str_String, nSize);
#endif	// STASH_PASSWORD
	packet->npcKind = m_nNPCVIdx;
	packet->keepMoney = m_nInNas;
	packet->keepCount = lKeepCount;

	for (i = 0; i < lKeepCount; ++i)
	{
		SWORD nTab		= m_vecTradeItems[i]->Item_Tab;
		SWORD nIdx		= m_vecTradeItems[i]->InvenIndex;
		LONG lIndex		= m_vecTradeItems[i]->Item_UniIndex;
		int  nCnt		= (int)m_vecTradeItems[i]->Item_Sum;

		packet->list[i].tab = nTab;
		packet->list[i].invenIndex = nIdx;
		packet->list[i].virtualIndex = lIndex;
		packet->list[i].count = nCnt;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * lKeepCount) );

	_pNetwork->SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseTakeReq
// Input  :
//-----------------------------------------------------------------------------
void CUIWareHouse::SendWareHouseTakeReq()
{
	LONG lTakeCount = 0;
	int		i;
	for( i = 0; i < WAREHOUSE_TRADE_SLOT_TOTAL; ++i)
	{
		if (m_pIconTradeItems[i]->IsEmpty())
			continue;
		lTakeCount++;
	}

	if(m_nOutNas == 0 && lTakeCount <= 0)
	{
		return;
	}

	// 찾기 요청				: password(str) takecount(n) [item_idx(n) count(ll)]:takecount
	CNetworkMessage nmMessage;
	RequestClient::doStashTake* packet = reinterpret_cast<RequestClient::doStashTake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_STASH;
	packet->subType = MSG_STASH_TAKE;
#ifdef	STASH_PASSWORD
	memset(packet->password, 0, MAX_STASH_PASSWORD_LENGTH+1);
	int nSize = m_strPW.Length();
	if (nSize > MAX_STASH_PASSWORD_LENGTH)
		nSize = MAX_STASH_PASSWORD_LENGTH;
	memcpy(packet->password, m_strPW.str_String, nSize);
#endif	// STASH_PASSWORD
	packet->npcKind = m_nNPCVIdx;
	packet->takeMoney = m_nOutNas;
	packet->takeCount = lTakeCount;

	for (i = 0; i < lTakeCount; ++i)
	{		
		SWORD nIdx		= m_vecTradeItems[i]->GetServerIndex();
		LONG lIndex		= m_vecTradeItems[i]->Item_UniIndex;
		int  nCnt		= (int)m_vecTradeItems[i]->Item_Sum;

		packet->list[i].invenIndex = nIdx;
		packet->list[i].virtualIndex = lIndex;
		packet->list[i].count = nCnt;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * lTakeCount) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUIWareHouse::ReceiveList( CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	UpdateClient::invenList* pPack = reinterpret_cast<UpdateClient::invenList*>(istr->GetBuffer());

	for (int nList = 0; nList < pPack->count; ++nList)
	{
		UpdateClient::itemInfo* pInfo = &pPack->info_list[nList];


		CItems* pItems = new CItems(pInfo->dbIndex);
		pItems->InitOptionData();

		SBYTE opt_cnt = pInfo->option_count;

		CItemData* pItemData = CItemData::getData(pInfo->dbIndex);
	
		//레어 아이템이면...
		if( pItemData->GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if (opt_cnt == 0)
				pItems->SetRareIndex(0);
			//감정된 레어 아이템
			else
				pUIManager->SetRareOption(&pPack->info_list[nList], *pItems);
		}		
		else
		{
			//레어 아이템이 아니면...
			if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				pItems->SetItemBelong( pItemData->GetItemBelong() );

				int		OpCount;

				for (OpCount = 0; OpCount < MAX_ORIGIN_OPTION; OpCount++)
				{
					pItems->SetOptionData( OpCount, pItemData->GetOptionOriginType(OpCount), 
						pItemData->GetOptionOriginLevel(OpCount), pInfo->origin_var[OpCount] );
				}

				// 아이템 스킬 세팅
				for (OpCount = 0; OpCount < MAX_ITEM_SKILL; OpCount++)
				{
					pItems->SetItemSkill(OpCount, pItemData->GetOptionSkillType(OpCount), pItemData->GetOptionSkillLevel(OpCount));
				}
			}
			else
			{
				for (int OpCount = 0; OpCount < opt_cnt; OpCount++)
				{
					pItems->SetOptionData( OpCount, pInfo->option_type[OpCount], 
						pInfo->option_level[OpCount], ORIGIN_VAR_DEFAULT );
				}
			}
		}

		//(*istr) >> useTime;
		//pUIManager->GetWareHouse()->SetUseTime(useTime);	//wooss 050817

		pItems->SetItemPlus2(pInfo->plus_2);
		pItems->SetData( pInfo->dbIndex, pInfo->virtualIndex, -1, -1, pInfo->plus, pInfo->flag, -1, 
			pInfo->used, pInfo->used_2, -1, pInfo->itemCount );

		pItems->SetServerIndex(pInfo->invenIndex);

	// socket system. [6/23/2010 rumist]
		pItems->InitSocketInfo();
		if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
		{
			SBYTE	sbSocketCreateCount = 0;
			int		sc;

			for (sc = 0; sc < JEWEL_MAX_COUNT; ++sc)
			{
				if (pInfo->socket[sc] >= 0)
				{
					sbSocketCreateCount++;
					pItems->SetSocketOption( sc, pInfo->socket[sc] );
				}
			}

			pItems->SetSocketCount( sbSocketCreateCount );
		}

#ifdef DURABILITY
		pItems->SetDurability(pInfo->now_durability, pInfo->max_durability);
#endif	//	DURABILITY

		m_vectorStorageItems.push_back(pItems);
	}

	if (IsVisible() == FALSE)
	{		
		pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_WAREHOUSE );
		pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );		
	}

	PrepareWareHouse();
}

//나스 입금 팝업 생성 
void CUIWareHouse::InNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_ADD_MONEY))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 5906, "입금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_WAREHOUSE, MSGCMD_WAREHOUSE_ADD_MONEY );
	strMessage.PrintF( _S( 5904, "몇 개의 나스를 입금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	return;
}
//나스 출금하기 팝업 생성 
void CUIWareHouse::OutNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WAREHOUSE_TAKE_MONEY))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 5907, "출금" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_WAREHOUSE, MSGCMD_WAREHOUSE_TAKE_MONEY );
	strMessage.PrintF( _S( 5905, "몇개의 나스를 출금 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	return;
}

void CUIWareHouse::ReceiveNas( CNetworkMessage *istr )
{
	UpdateClient::money* pPack = reinterpret_cast<UpdateClient::money*>(istr->GetBuffer());

	if (pPack->subType == MSG_SUB_UPDATE_MONENY)
	{
		__int64 delta_nas = pPack->nas - m_nTotalNas;

		if (delta_nas > 0)
		{
			CTString strOutNas;
			strOutNas.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), delta_nas);		
			
			_pNetwork->ClientSystemMessage( strOutNas );
		}

		ResetWareHouse();
	}

	m_nTotalNas = pPack->nas;

	m_strTotalNas.PrintF( "%I64d", m_nTotalNas );
	CUIManager::getSingleton()->InsertCommaToString( m_strTotalNas );	
}

void CUIWareHouse::ErrMessage( int nErr )
{
	if( CUIManager::getSingleton()->DoesMessageBoxExist( MSGCMD_NULL ) )
		return;

	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	switch( nErr )
	{
		case eERR_KEEP_FAIL_EMPTY:
			MsgInfo.AddString( _S( 5554, "맡길 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );		
			break;
		case eERR_TAKE_FAIL_EMPTY:
			MsgInfo.AddString( _S( 5555, "찾을 아이템이 없습니다. 확인후 다시 하시기 바랍니다.") );		
			break;
	}
	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void CUIWareHouse::SendListReq(CTString& strPW)
{
	CNetworkMessage nmMessage;
	RequestClient::doStashList* packet = reinterpret_cast<RequestClient::doStashList*>(nmMessage.nm_pubMessage);
	packet->type = MSG_STASH;
	packet->subType = MSG_STASH_LIST;

#ifdef	STASH_PASSWORD
	m_strPW = strPW;
	memset(packet->password, 0, MAX_STASH_PASSWORD_LENGTH+1);
	int nSize = strPW.Length();
	if (nSize > MAX_STASH_PASSWORD_LENGTH)
		nSize = MAX_STASH_PASSWORD_LENGTH;
	memcpy(packet->password, strPW.str_String, nSize);
#endif	// STASH_PASSWORD

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUIWareHouse::AddItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
	{
		SQUAD llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();
		WareHouseToTrade(llCount);
	}
}

void CUIWareHouse::DelItemCallback()
{
	SQUAD llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if (llCount > 0 && llCount <= m_pItemsTemp->Item_Sum)
	{
		TradeToWareHouse(llCount);
	}
}
