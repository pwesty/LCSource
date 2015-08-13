
#include "stdh.h"

// 헤더파일 정리. [12/1/2009 rumist]
// temporary header [11/20/2009 rumist]
#include <Engine/Entities/CashShopData.h>
#include <Engine/Interface/UIMultiEditBox.h>
#include <vector>
#include <Engine/Interface/UICashShop.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Interface/UIChatting.h>

#define	CASHSHOP_TAB_WIDTH					100
#define	CASHSHOP_TITLE_OFFSETX				25
#define	CASHSHOP_TITLE_OFFSETY				5
#define	CASHSHOP_TAB_TEXT_OFFSETY			34
#define RIGHT_SLOT_SIZE_BASKET				5
#define	LEFT_SLOT_SIZE_BUY_INFO				5
#define	RIGHT_SLOT_SIZE_MOVE_COL			5
#define	RIGHT_SLOT_SIZE_MOVE_ROW			2
#define	HISTORY_SLOT_SIZE					4	
#define	SLOT_SIZE							33
#define	SLOT_GAP							5
#define	MIN_YEAR							2006
#define	MAX_YEAR							2011
#define	MIN_MONTH							1
#define	MAX_MONTH							12
#define	MIN_DAY								1
#define	MAX_DAY								31
#define LIST_SIZE							6	
#define SHOP_TOP_SLOT_SX					113
#define SHOP_TOP_SLOT_SY					77
#define TRADE_TOP_SLOT_SX					354	
#define TRADE_TOP_SLOT_SY					97
#define SET_TOP_SLOT_SX						17
#define SET_TOP_SLOT_SY						105
#define	SET_TOP_ITEM_SX						40
#define SET_TOP_ITEM_SY						65
#define KIT_TOP_ITEM_SX						125
#define KIT_TOP_ITEM_SY						111
#define INVEN_TOP_ITEM_SX					364
#define INVEN_TOP_ITEM_SY					107
#define	HISTORY_TOP_ITEM_SX					150
#define HISTORY_TOP_ITEM_SY					165
#define BTN_WIDTH_1							75
#define BTN_WIDTH_2							63
#define BTN_WIDTH_3							90
#define BTN_WIDTH_4							150			//Su-won
#define BTN_HEIGHT_1						31
#define MAX_CASH_ITEMINFO_CHAR				34
#define MAX_CASH_HELP_CHARS					108

extern INDEX g_iCountry;

//캐쉬아이템 선물하기 기능추가

// ----------------------------------------------------------------------------
// Name : CUICashShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
BOOL IsSetItem(int idx)
{
	CCashShopData& CD_SET = _pNetwork->GetCashShopData(SET_ITEM);
	std::vector<CCashShopData::CASH_SHOP_DATA>::iterator tv_begin,tv_end;
	tv_begin = CD_SET.m_vShopItemArray.begin();
	tv_end = CD_SET.m_vShopItemArray.end();
	BOOL bIsSet =  FALSE ;
	while(tv_begin != tv_end){
		if( tv_begin->m_shopItemIndex == idx) bIsSet = TRUE;	
		tv_begin++;
	}
	return bIsSet;
}


// ----------------------------------------------------------------------------
// Name : CUICashShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
BOOL IsPlatinumItem(int idx)
{
	CCashShopData& CD_PLAT = _pNetwork->GetCashShopData(PLATINUM_ITEM);
	std::vector<CCashShopData::CASH_SHOP_DATA>::iterator tv_begin,tv_end;
	tv_begin = CD_PLAT.m_vShopItemArray.begin();
	tv_end = CD_PLAT.m_vShopItemArray.end();
	BOOL bIsPlat =  FALSE ;
	while(tv_begin != tv_end){
		if( tv_begin->m_shopItemIndex == idx) bIsPlat = TRUE;	
		tv_begin++;
	}
	return bIsPlat;
}

// ----------------------------------------------------------------------------
// Name : CUICashShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
int ReturnPlatinumItemTypeIndex(int idx)
{
	CCashShopData& CD_PLAT = _pNetwork->GetCashShopData(PLATINUM_ITEM);
	std::vector<CCashShopData::CASH_SHOP_DATA>::iterator tv_begin,tv_end;
	tv_begin = CD_PLAT.m_vShopItemArray.begin();
	tv_end = CD_PLAT.m_vShopItemArray.end();
	int tv_loop =0;
	while(tv_begin != tv_end){
		if( tv_begin->m_shopItemIndex == idx) {
			return tv_loop;
		}
		tv_begin++;
		tv_loop++;
	}
	return -1;
}





	
// ----------------------------------------------------------------------------
// Name : CUICashShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICashShop::CUICashShop()
	: m_pSetInfo(NULL)
	, m_pConfirmInfo(NULL)
{
	m_nCurShopTab	  =		m_nOldShopTab		= UCST_NEW_CASH;
	m_nCurShopSection =		m_nOldShopSection	= UCSS_NEWITEM;
	m_nGoodsSlotNum   =     m_nKitSlotNum       = 0 ;
	m_nShopItemID	  =     m_nTradeItemID		= -1;
	m_nKitItemID	  =     m_nInvenItemID		= -1;
//	m_pSetInfo = new CUISetInfo;
	m_nMyCash =0 ;
	m_bShowShopInfo = FALSE;
	m_bShowTradeInfo = FALSE;
	m_bShowKitInfo = FALSE;
	m_bShowInvenInfo = FALSE;
	m_bShowHistoryInfo = FALSE;
	m_bShowSendGift = FALSE;
	m_iShowItemIdx	= -1;
	m_nCurInfoLines		= 0;
	m_abtnKitItems.New(KIT_SLOT_TOTAL);
	m_abtnInvenItems.New(INVEN_SLOT_TOTAL);
	m_shopItemCnt = 0;
	m_pbtnShopItems = NULL;
	m_bBringItem = FALSE;





	m_anInvenOrder.New(INVEN_SLOT_TOTAL);		//Su-won

	m_nSendScrollPos = m_nRecvScrollPos =0; 	//Su-won
	m_nSendYear =m_nSendMonth =m_nSendDay =0;	//Su-won
	m_nRecvYear =m_nRecvMonth =m_nRecvDay =0;	//Su-won
}

// ----------------------------------------------------------------------------
// Name : ~CUICashShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICashShop::~CUICashShop()
{
	Destroy();

	SAFE_DELETE(m_pSetInfo);
	SAFE_DELETE(m_pConfirmInfo);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	int		i;	

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 700, 22 );
	m_rcTab.SetRect( 288, 31, 689, 49 );
	m_rcShopItems.SetRect( SHOP_TOP_SLOT_SX + 20, SHOP_TOP_SLOT_SY, 385, 375 ); // 아이템 버튼장소( 226 )
	m_rcTradeItems.SetRect( TRADE_TOP_SLOT_SX + 50,TRADE_TOP_SLOT_SY,438,282 ); // 구매할 아이템 버튼 장소
	m_rcKitItems.SetRect( KIT_TOP_ITEM_SX + 30, KIT_TOP_ITEM_SY, 311 + 30, 295 );
	m_rcInvenItems.SetRect(INVEN_TOP_ITEM_SX + 50,INVEN_TOP_ITEM_SY,550 + 50,177);
	m_rcHelp.SetRect(CASHSHOP_TITLE_OFFSETX, SHOP_TOP_SLOT_SY ,650,400);
	m_rcRecvItems.SetRect( KIT_TOP_ITEM_SX + 30, KIT_TOP_ITEM_SY, 311 + 30, 295+ 70 );	//받은 선물을 나타낼 영역 :Su-won
	// connie [2009/10/23] - test
	m_rcKitItemsJP.SetRect(TRADE_TOP_SLOT_SX + 50, TRADE_TOP_SLOT_SY, 666, 290);
	
	// Create texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 0, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 0, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 0, 75, 96, 93, fTexWidth, fTexHeight );

	// line 
	m_rtLineH.SetUV(228, 0, 230, 0, fTexWidth,fTexHeight);
	m_rtLineV.SetUV(228, 0, 228, 2, fTexWidth,fTexHeight);
	
	m_rtInfoUL.SetUV( 129, 98, 136, 105, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 139, 98, 141, 105, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 144, 98, 151, 105, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 129, 108, 136, 110, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 139, 108, 141, 110, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 144, 108, 151, 110, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 129, 113, 136, 120, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 139, 113, 141, 120, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 144, 113, 151, 120, fTexWidth, fTexHeight );
/*
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
*/
	// Outline of selected item
//	m_rtSelectOutline.SetUV( 129, 98, 148, 117, fTexWidth, fTexHeight );
	m_rtSelectOutlineUL.SetUV( 129, 98, 130, 99, fTexWidth, fTexHeight );
	m_rtSelectOutlineUM.SetUV( 135, 98, 136, 99, fTexWidth, fTexHeight );
	m_rtSelectOutlineUR.SetUV( 146, 98, 147, 99, fTexWidth, fTexHeight );
	m_rtSelectOutlineLL.SetUV( 129, 116, 130, 117, fTexWidth, fTexHeight );
	m_rtSelectOutlineLM.SetUV( 136, 116, 137, 117, fTexWidth, fTexHeight );
	m_rtSelectOutlineLR.SetUV( 146, 116, 147, 117, fTexWidth, fTexHeight );
	m_rtSelectOutlineML.SetUV( 129, 103, 130, 104, fTexWidth, fTexHeight );
	m_rtSelectOutlineMR.SetUV( 147, 103, 148, 104, fTexWidth, fTexHeight );

	//  Left Scroll Bar
	m_sbLeft.Create( this, 373,73, 9, 316 );
	m_sbLeft.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbLeft.SetScrollPos( 0 );
	m_sbLeft.SetScrollRange( m_nGoodsSlotNum );
	m_sbLeft.SetCurItemCount( m_nGoodsSlotNum );
	m_sbLeft.SetItemsPerPage( LEFT_SLOT_SIZE_BUY );
	// Up button
	m_sbLeft.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbLeft.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbLeft.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbLeft.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbLeft.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbLeft.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbLeft.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbLeft.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbLeft.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbLeft.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbLeft.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbLeft.SetWheelRect( -257, -1, 256, 320 );

	// Right Scroll Bar
	m_sbRight.Create( this, 657,92, 9, 200 );
	m_sbRight.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbRight.SetScrollPos( 0 );
	m_sbRight.SetScrollRange( MAX_KIT_SIZE );
	m_sbRight.SetCurItemCount( MAX_KIT_SIZE );
	m_sbRight.SetItemsPerPage( RIGHT_SLOT_SIZE_BASKET );
	// Up button
	m_sbRight.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbRight.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbRight.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbRight.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbRight.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbRight.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbRight.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbRight.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbRight.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbRight.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbRight.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbRight.SetWheelRect( -269, -1, 270, 203 );
	
	
	// Close Button
	m_btnClose.Create( this, CTString( "" ), 667, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// New Item Button
	m_btnNewItem.Create(this, _S( 2570, "신상품" ), 23, 71 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnNewItem.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnNewItem.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnNewItem.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnNewItem.CopyUV( UBS_IDLE, UBS_ON );

	// Hot Item Button
	m_btnHotItem.Create(this, _S( 2571, "인기상품" ), 23, 108 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnHotItem.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnHotItem.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnHotItem.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnHotItem.CopyUV( UBS_IDLE, UBS_ON );

	// Consume button
	m_btnConsume.Create(this, _S(2371,"소모상품"), 23, 71 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnConsume.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnConsume.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnConsume.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnConsume.CopyUV( UBS_IDLE, UBS_ON );
	// TimeLimit 

	m_btnTimeLimit.Create(this, _S(2372,"기간상품"), 23, 108 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnTimeLimit.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnTimeLimit.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnTimeLimit.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnTimeLimit.CopyUV( UBS_IDLE, UBS_ON );
	// Potion

	m_btnPotion.Create(this, _S(2373,"물약"), 23, 145 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnPotion.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnPotion.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnPotion.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnPotion.CopyUV( UBS_IDLE, UBS_ON );
	// Avata
	m_btnAvata.Create(this, _S(2374,"아바타 "), 23, 182 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnAvata.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnAvata.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnAvata.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnAvata.CopyUV( UBS_IDLE, UBS_ON );
	// Weapon
	m_btnWeapon.Create(this, _S(2375,"무기"), 23, 219 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnWeapon.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnWeapon.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnWeapon.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnWeapon.CopyUV( UBS_IDLE, UBS_ON );
	// Depend
	m_btnDefend.Create(this, _S(2376,"방어구"), 23, 256 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnDefend.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnDefend.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnDefend.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnDefend.CopyUV( UBS_IDLE, UBS_ON );
	
	// Set --------------------------------------------------------->
	
	m_btnSet.Create(this, _S(2377,"세트상품"), 23, 293 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnSet.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnSet.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnSet.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnSet.CopyUV( UBS_IDLE, UBS_ON );

	// Platinum Item ------------------------------------------------>>
	m_btnPlatinum.Create(this, _S(2743,"플레티늄상품"), 23, 145 , BTN_WIDTH_3 , BTN_HEIGHT_1 );
	m_btnPlatinum.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnPlatinum.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnPlatinum.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnPlatinum.CopyUV( UBS_IDLE, UBS_ON );

	// Set Item Array
	for( i = 0; i < LEFT_SLOT_SIZE_BUY ; i++ )
	{
		m_aBtnSetItem[i].Create(this, _S(2378,"자세히"), 222, 83 + i*(SLOT_SIZE+SLOT_GAP)  , BTN_WIDTH_1 , 21);
		m_aBtnSetItem[i].SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
		m_aBtnSetItem[i].SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
		m_aBtnSetItem[i].SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
		m_aBtnSetItem[i].CopyUV( UBS_IDLE, UBS_ON );
	}

	
//	m_pSetInfo->Create( NULL, nX, nY, SETINFO_WIDTH, SETINFO_HEIGHT );



	// Set <------------------------------------------------------------
	
	// Clear 
	m_btnClear.Create(this, _S(2379,"비우기"), 495, 361 , BTN_WIDTH_1 , 21);
	m_btnClear.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnClear.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnClear.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnClear.CopyUV( UBS_IDLE, UBS_ON );
	// Pay 
	m_btnPay.Create(this, _S(2380,"결제"), 585, 361 , BTN_WIDTH_1 , 21);
	m_btnPay.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnPay.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnPay.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnPay.CopyUV( UBS_IDLE, UBS_ON );
	// Cancel
	m_btnCancel.Create(this, _S(139,"취소"), 590, 407 , BTN_WIDTH_1 , 21);
	m_btnCancel.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	// Info bought items
	m_btnInfoBuyItem.Create(this, _S(2381,"구매 물품"),23,71,BTN_WIDTH_1,21);
	m_btnInfoBuyItem.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoBuyItem.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoBuyItem.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoBuyItem.CopyUV( UBS_IDLE, UBS_ON );
	// Info History
	m_btnInfoBuyHistory.Create(this, _S(2382,"구매 내역"),23,98,BTN_WIDTH_1,21);
	m_btnInfoBuyHistory.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoBuyHistory.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoBuyHistory.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoBuyHistory.CopyUV( UBS_IDLE, UBS_ON );

	// Info Gift			: Su-won
	m_btnInfoGiftHistory.Create(this, _S(3085, "선물 내역"),23,125,BTN_WIDTH_1,21);
	m_btnInfoGiftHistory.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoGiftHistory.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoGiftHistory.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoGiftHistory.CopyUV( UBS_IDLE, UBS_ON );
	
	// Info Send Gift		: Su-won
	m_btnInfoSendHistory.Create(this, _S(3086, "보낸 선물내역 확인"),108+30+53, 70+17,BTN_WIDTH_4,21);
	m_btnInfoSendHistory.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoSendHistory.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoSendHistory.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoSendHistory.CopyUV( UBS_IDLE, UBS_ON );
	// Info Receive Gift	: Su-won
	m_btnInfoReceiveHistory.Create(this, _S(3087, "받은 선물내역 확인"), 108+30+53+BTN_WIDTH_4+53, 70+17,BTN_WIDTH_4,21);
	m_btnInfoReceiveHistory.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoReceiveHistory.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoReceiveHistory.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoReceiveHistory.CopyUV( UBS_IDLE, UBS_ON );

	// Info Receive Gift	: Su-won
	m_btnInfoReceive.Create(this, _S(3088, "받은 선물"),23,152,BTN_WIDTH_1,21);
	m_btnInfoReceive.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoReceive.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoReceive.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoReceive.CopyUV( UBS_IDLE, UBS_ON );

	// Send Gift			: Su-won	
	m_btnGift.Create(this, _S(3089, "선물하기"), 363+50, 210,BTN_WIDTH_1,21);
	m_btnGift.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnGift.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnGift.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnGift.CopyUV( UBS_IDLE, UBS_ON );

	// Move Inventory
	m_btnInfoMove.Create(this, _S(2383,"가져오기"), 363+50+(SLOT_SIZE+SLOT_GAP)*5-SLOT_GAP -BTN_WIDTH_1, 210, BTN_WIDTH_1,21);		//수정: Su-won
	m_btnInfoMove.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnInfoMove.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnInfoMove.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnInfoMove.CopyUV( UBS_IDLE, UBS_ON );

	// Send 			: Su-won	
	m_btnSend.Create(this, _S(3090, "보내기"), 363+50+(SLOT_SIZE+SLOT_GAP)*5-SLOT_GAP -BTN_WIDTH_2, 376, BTN_WIDTH_2,21);
	m_btnSend.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnSend.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnSend.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnSend.CopyUV( UBS_IDLE, UBS_ON );

	// CharanterName EditBox 			: Su-won	
	m_ebChar.Create( this, 348+50+88, 252+30, 96, 13, 16 );
	m_ebChar.SetReadingWindowUV( 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_ebChar.SetCandidateUV( 0, 94, 63, 115, fTexWidth, fTexHeight );
				
	// Gift Message EditBox 			: Su-won	
	//m_ebGiftMessage.Create( this, 348+50+5, 252+55+20, 209, 45, MAX_GIFT_MESSAGE);
	m_ebGiftMessage.Create( this, 348+50+5, 252+55+17, 197, 45, 3);
	m_ebRecvGiftMsg[0].Create( this, 147, 100+(SLOT_SIZE+SLOT_GAP)+48, 197, 45, 3);
	m_ebRecvGiftMsg[1].Create( this, 147, 100+(SLOT_SIZE+SLOT_GAP)+48+142+5, 197, 45, 3);
	
	//GiftSearch History Button			: Su-won
	m_btnGiftSearch.Create(this, _S(386,"검색"),479+30,115+31,50,21);
	m_btnGiftSearch.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnGiftSearch.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnGiftSearch.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnGiftSearch.CopyUV( UBS_IDLE, UBS_ON );

	//Search History Button
	m_btnSearch.Create(this, _S(386,"검색"),479+30,115,50,21);
	m_btnSearch.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnSearch.CopyUV( UBS_IDLE, UBS_ON );

	m_btnBillingLink.Create(this, _S(2387,"캐쉬 충전"), 500, 407 , BTN_WIDTH_3 , 21 );
	m_btnBillingLink.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnBillingLink.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnBillingLink.SetUV( UBS_DISABLE, 128, 76, 191, 97, fTexWidth, fTexHeight );
	m_btnBillingLink.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBillingLink.SetEnable(FALSE);

#if defined(G_GERMAN)
	m_btnBillingLink.SetEnable(TRUE);
#endif

	// ITEM SLOT
	m_rtItemSlot.SetUV(193,0,227,34,fTexWidth,fTexHeight);
	// Black Box
	m_rtBlackBox.SetUV(13,51,76,60,fTexWidth,fTexHeight);
	// Black Box with Line
	m_rtBlackboxL.SetUV(11,49,78,62,fTexWidth,fTexHeight);

	// ComboBox 
	// year
	m_cbYear.Create( this, 145+30, 118, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbYear.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbYear.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbYear.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbYear.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	// Year - scroll bar
	m_cbYear.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbYear.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbYear.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbYear.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbYear.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbYear.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbYear.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbYear.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbYear.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbYear.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbYear.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbYear.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	CTString tv_str;
	for( i = MIN_YEAR; i <= MAX_YEAR ; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbYear.AddString( tv_str);
	}
	m_cbYear.SetCurSel( 0 );
	
	// Month
	m_cbMonth.Create( this, 255+30, 118, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbMonth.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbMonth.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbMonth.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbMonth.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	// Month - scroll bar
	m_cbMonth.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbMonth.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbMonth.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbMonth.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbMonth.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbMonth.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbMonth.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbMonth.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbMonth.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbMonth.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbMonth.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbMonth.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	
	
	for( i = MIN_MONTH; i <= MAX_MONTH; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbMonth.AddString( tv_str);
	}
	m_cbMonth.SetCurSel( 0 );

	// Day
	m_cbDay.Create( this, 365+30, 118, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbDay.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbDay.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbDay.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbDay.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	m_cbDay.SetCurSel( 0 );
	
	// Day - scroll bar
	m_cbDay.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbDay.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbDay.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbDay.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbDay.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbDay.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbDay.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbDay.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbDay.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbDay.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbDay.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbDay.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	
	for( i = MIN_DAY; i <= MAX_DAY; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbDay.AddString( tv_str);
	}

	///////////////////////////////////////////////////////////////////////////////////
	//Su-won	|------------------------------------>
	m_cbGiftYear.Create( this, 145+30, 118+31, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbGiftYear.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbGiftYear.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbGiftYear.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbGiftYear.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	// Year - scroll bar
	m_cbGiftYear.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbGiftYear.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbGiftYear.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbGiftYear.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbGiftYear.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbGiftYear.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbGiftYear.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbGiftYear.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbGiftYear.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbGiftYear.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbGiftYear.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbGiftYear.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	for( i = MIN_YEAR; i <= MAX_YEAR; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbGiftYear.AddString( tv_str);
	}
	m_cbGiftYear.SetCurSel( 0 );
	
	// Month
	m_cbGiftMonth.Create( this, 255+30, 118+31, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbGiftMonth.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbGiftMonth.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	// Month - scroll bar
	m_cbGiftMonth.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbGiftMonth.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbGiftMonth.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbGiftMonth.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbGiftMonth.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbGiftMonth.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbGiftMonth.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbGiftMonth.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbGiftMonth.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	
	
	for( i = MIN_MONTH; i <= MAX_MONTH; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbGiftMonth.AddString( tv_str);
	}
	m_cbGiftMonth.SetCurSel( 0 );

	// Day
	m_cbGiftDay.Create( this, 365+30, 118+31, 77, 15, 63, 1, 13, 13,
						LIST_SIZE, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbGiftDay.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cbGiftDay.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cbGiftDay.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cbGiftDay.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	m_cbGiftDay.SetCurSel( 0 );
	
	// Day - scroll bar
	m_cbGiftDay.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbGiftDay.SetScrollUpUV( UBS_IDLE, 46, 117, 53, 123, fTexWidth, fTexHeight );
	m_cbGiftDay.SetScrollUpUV( UBS_CLICK, 56, 117, 64, 123, fTexWidth, fTexHeight );
	m_cbGiftDay.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbGiftDay.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbGiftDay.SetScrollBarTopUV( 90, 43, 98, 51, fTexWidth, fTexHeight );
	m_cbGiftDay.SetScrollBarMiddleUV( 90, 52, 98, 58, fTexWidth, fTexHeight );
	m_cbGiftDay.SetScrollBarBottomUV( 90, 59, 98, 66, fTexWidth, fTexHeight );
	// Down button
	m_cbGiftDay.SetScrollDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_cbGiftDay.SetScrollDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_cbGiftDay.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbGiftDay.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	
	for( i = MIN_DAY; i <= MAX_DAY; i++)
	{
		tv_str.PrintF("%d",i);
		m_cbGiftDay.AddString( tv_str);
	}
	//Su-won	<------------------------------------------------------------------|
	////////////////////////////////////////////////////////////////////////////////

	// History Scroll Bar
	m_sbHistory.Create( this, 524+30,160, 9, 156 );
	m_sbHistory.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbHistory.SetScrollPos( 0 );
	m_sbHistory.SetScrollRange( 0 );
	m_sbHistory.SetCurItemCount( 0 );
	m_sbHistory.SetItemsPerPage( HISTORY_SLOT_SIZE );
	// Up button
	m_sbHistory.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbHistory.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbHistory.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbHistory.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbHistory.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbHistory.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbHistory.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbHistory.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbHistory.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbHistory.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbHistory.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbHistory.SetWheelRect( -184, -1, 183, 176 );

	// 선물 내역 스크롤바 :Su-won
	m_sbGift.Create( this, 524+40,160+31, 9, 156 );
	m_sbGift.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbGift.SetScrollPos( 0 );
	m_sbGift.SetScrollRange( 0 );
	m_sbGift.SetCurItemCount( 0 );
	m_sbGift.SetItemsPerPage( HISTORY_SLOT_SIZE );
	// Up button
	m_sbGift.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbGift.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbGift.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbGift.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbGift.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbGift.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbGift.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbGift.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbGift.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbGift.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbGift.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbGift.SetWheelRect( -184, -1, 183, 176 );

	// 받은 선물 스크롤바 :Su-won
	m_sbReceive.Create( this, 335+30-9, 90, 9, 299 );
	m_sbReceive.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbReceive.SetScrollPos( 0 );
	m_sbReceive.SetScrollRange( 0 );
	m_sbReceive.SetCurItemCount( 0 );
	m_sbReceive.SetItemsPerPage( 2 );
	// Up button
	m_sbReceive.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbReceive.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbReceive.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbReceive.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbReceive.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbReceive.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbReceive.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbReceive.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbReceive.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbReceive.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbReceive.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbReceive.SetWheelRect( -227, -1, 226, 298 );

	// Slot items
	// Shop Slot MAX 8
//	for( i = 0; i < LEFT_SLOT_SIZE_BUY; i++ )
//		m_pbtnShopItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM);
		
	// KIT Slot  MAX 10
	for( i = 0; i < MAX_KIT_SIZE; i++)
		m_abtnTradeItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM );	

	for( i = 0; i < INVEN_SLOT_TOTAL; i++)
		 m_abtnInvenItems[i].Create(this, 0,0,BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM);		// items to move to Inventory 
	
	for( i = 0; i < KIT_SLOT_TOTAL; i++)
		m_abtnKitItems[i].Create(this,0,0,BTN_SIZE, BTN_SIZE, UI_CASH_SHOP,UBET_ITEM);	// Bought items
	
	m_abtnShowItem.Create( this, 0, 0, 50, 50, UI_CASH_SHOP, UBET_ITEM );

	// create setItemInfo UI
	m_pSetInfo = new CUISetInfo;
	// create confirmInfo UI
	m_pConfirmInfo = new CUIConfirmInfo;
	m_pConfirmInfo->Create(NULL,m_nPosX+279,m_nPosY+65,SETINFO_WIDTH,SETINFO_HEIGHT + 60);
}


// ----------------------------------------------------------------------------
// Name : OpenCashShop()
// Desc :
// ----------------------------------------------------------------------------

void CUICashShop::OpenCashShop()
{
	m_nOldShopTab = UCST_NEW_CASH;
	m_nCurShopTab = UCST_NEW_CASH;
	m_nCurShopSection=UCSS_NEWITEM;

	CUIManager::getSingleton()->RearrangeOrder( UI_CASH_SHOP, TRUE );
	PrepareShopItems(  );
	

}


// ----------------------------------------------------------------------------
// Name : CloseCashShop()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::CloseCashShop()
{
	m_nCurShopSection	= UCSS_NEWITEM;
	m_nCurShopTab		= m_nOldShopTab = UCST_NEW_CASH;
	m_nMyCash			= 0;
	 
	m_pSetInfo->SetEnable(FALSE);
	m_pSetInfo->SetVisible(FALSE);
	m_pConfirmInfo->SetEnable(FALSE);
	m_pConfirmInfo->SetVisible(FALSE);
	ClearBtnItems(m_abtnTradeItems,(int)MAX_KIT_SIZE);
	ClearBtnItems(m_pbtnShopItems,m_shopItemCnt);
	ClearBtnItems(m_abtnKitItems.sa_Array ,(int)KIT_SLOT_TOTAL);
	ClearBtnItems(m_abtnInvenItems.sa_Array,(int)INVEN_SLOT_TOTAL);
	ClearBtnItems(m_abtnHistoryItems.sa_Array,(int)m_abtnHistoryItems.Count());
	
	//Su-won	|-------->
	ClearBtnItems(m_abtnSendHistoryItems.sa_Array,(int)m_abtnSendHistoryItems.Count());
	ClearBtnItems(m_abtnRecvHistoryItems.sa_Array,(int)m_abtnRecvHistoryItems.Count());
	ClearBtnItems(m_abtnRecvGift.sa_Array,(int)m_abtnRecvGift.Count());
	
	m_cbYear.SetCurSel(0); 
	m_cbMonth.SetCurSel(0);
	m_cbDay.SetCurSel(0);
	m_cbGiftYear.SetCurSel(0); 
	m_cbGiftMonth.SetCurSel(0);
	m_cbGiftDay.SetCurSel(0);

	m_nSendYear =m_nSendMonth =m_nSendDay =0;
	m_nRecvYear =m_nRecvMonth =m_nRecvDay =0;
	m_abtnSendHistoryItems.Clear();
	m_abtnRecvHistoryItems.Clear();

	m_nSendScrollPos = m_nRecvScrollPos =0;
	
	m_bShowSendGift = FALSE;
	m_ebChar.SetFocus(FALSE);
	//Su-won	<--------|
	
	CUIManager::getSingleton()->RearrangeOrder( UI_CASH_SHOP, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_pSetInfo->ResetPosition(  pixMinI,  pixMinJ,  pixMaxI,  pixMaxJ );
	m_pConfirmInfo->ResetPosition(  pixMinI,  pixMinJ,  pixMaxI,  pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
	m_pSetInfo->AdjustPosition(   pixMinI,   pixMinJ,   pixMaxI, pixMaxJ );
	m_pConfirmInfo->AdjustPosition(   pixMinI,   pixMinJ,   pixMaxI, pixMaxJ );
}


// ----------------------------------------------------------------------------
// Name : RenderShopHelp()
// Desc : 
// ----------------------------------------------------------------------------

void CUICashShop::RenderHelp()
{
	m_nCurInfoLines	= 0;
	CTString tv_str;
	int tv_length = MAX_CASH_HELP_CHARS ;
	
#if defined(G_THAI)
	tv_length = 90;
#endif

	AddItemInfoString( _S( 2586,"♥아이리스 상점이란?"), 0xFFAA44FF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2587," 아이리스 상점은 게임이용에 유용한 특별한 아이템을 판매하는 곳입니다.");
	tv_str+=_S(2588,"아이리스 상점의 아이템을 구매하려면 캐시가 필요하며, 캐시는 라스트 카오스 홈페이지에서 다양한 결제수단으로 바로 충전할 수 있습니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
		
	tv_str=_S(2589,"♥캐시 충전방법");
	AddItemInfoString( CTString("\n"), 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	AddItemInfoString( tv_str, 0xFFAA44FF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2590,"① 라스트 카오스 홈페이지에서 충전할 계정 아이디로 로그인 후 오른쪽의 [캐시 충전] 버튼 클릭");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2591,"② 원하는 결제수단과 금액을 선택하고 결제를 진행하면 결제금액과 1대1 비율로 캐시가 충전됩니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2592,"③ [내역 확인]에서는 월별 충전내역과 일별 아이템 구매내역을 확인 할 수 있습니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	
#if defined(G_USA)
	{
		tv_str = _S(2674, "4. 지불 방식을 결정하고 순서에 따라 계정에 Aeria Points를 받는다.");
		AddItemInfoString( tv_str, 0xFFFFFFFF, MAX_CASH_ITEMINFO_LINE, tv_length);
	}
#endif

	AddItemInfoString( CTString("\n"), 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2593,"♥아이리스 상점 이용방법");
	AddItemInfoString( tv_str, 0xFFAA44FF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2594,"① [아이템 구매] 목록에서 구입하고자 하는 아이템을 더블클릭하거나 장바구니에 끌어다 놓습니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2595,"② 장바구니 앞에 있는 [결제]버튼을 누르면 보유 캐시에서 물품금액만큼의 캐시가 차감되며 아이템이 구매됩니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );
	tv_str=_S(2596,"③ 구매한 아이템은 상점의 [구매 정보]에서 확인할 수 있으며, 캐릭터의 인벤토리로 옮기려면 [구매한 물품]의 "); 
	tv_str+=_S(2597,"아이템을 더블크릭하거나 오른쪽의 [옮길 물품]으로 끌어다 놓은 후 [가져오기]버튼을 클릭하면 됩니다.");
	AddItemInfoString( tv_str, 0xFFFFFFFF , MAX_CASH_ITEMINFO_LINE , tv_length );

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Render item information
	int	nInfoX = m_nPosX + m_rcHelp.Left;
	int	nInfoY = m_nPosY +m_rcHelp.Top;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		pUIManager->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight()+2;
	}

}
// ----------------------------------------------------------------------------
// Name : RenderShopItems()
// Desc : 
// ----------------------------------------------------------------------------

void CUICashShop::RenderShopItems()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int iShopX, iShopY , i;
	
	iShopX = SHOP_TOP_SLOT_SX + 20; // eons
	iShopY = SHOP_TOP_SLOT_SY;
	
	int	nX = iShopX;
	int	nY = iShopY;

	int tv_pos = m_sbLeft.GetScrollPos();
	int tv_x, tv_y;

	int tv_max;
	tv_max = tv_pos+ LEFT_SLOT_SIZE_BUY;
	tv_max = tv_max>m_shopItemCnt?m_shopItemCnt:tv_max;
	for( i = tv_pos; i < tv_max; i++)
	{
		m_pbtnShopItems[i].SetPos( nX, nY );

		if( m_pbtnShopItems[i].IsEmpty() )
			continue;
		
		m_pbtnShopItems[i].Render();
		
		CCashShopData& CD = _pNetwork->GetCashShopData(m_nCurShopSection);
		m_pbtnShopItems[i].GetAbsPos(tv_x,tv_y);
		tv_x  += SLOT_GAP+SLOT_SIZE;

		CTString strTitle, strNum;
//		strTitle = CD.m_vShopItemArray[i+tv_pos].m_itemName;
		strTitle = m_pbtnShopItems[i].GetCashName();
		if(IsPlatinumItem(m_pbtnShopItems[i].GetCashIndex())){
			CCashShopData& CD = _pNetwork->GetCashShopData(PLATINUM_ITEM);
			INDEX limitCnt =CD.m_vShopItemArray[ReturnPlatinumItemTypeIndex(m_pbtnShopItems[i].GetCashIndex())].m_limitCnt;
			COLOR numCol;

			if(limitCnt > 0 ){
				strNum.PrintF( _S( 2890, "남은 수량 : %d" ), limitCnt);
				numCol = 0xF2F2F2FF;
			}
			else {
				strNum.PrintF( _S( 2891, "남은 수량 : 품절" ) );
				numCol = 0x9b9595FF;
			}
			pDrawPort->PutTextEx(strTitle, tv_x,tv_y);
			tv_y += SLOT_SIZE/2;
			pDrawPort->PutTextEx(strNum, tv_x,tv_y,numCol);
			
		}
		else 
		{
				
			if(!IsSetItem(m_pbtnShopItems[i].GetCashIndex()))
			{
				if( CD.m_vShopItemArray[m_pbtnShopItems[i].GetCashTypeIndex()].m_vItemDataArray[0].m_itemCnt > 1 )
				{
					strNum.PrintF( " (%d)", CD.m_vShopItemArray[m_pbtnShopItems[i].GetCashTypeIndex()].m_vItemDataArray[0].m_itemCnt );
					strTitle += strNum;
				}
			}

			if( m_nCurShopSection != UCSS_SET) {
				tv_y += SLOT_SIZE/4;
				pDrawPort->PutTextEx(strTitle, tv_x,tv_y);
			}
		}
		
		nY += SLOT_SIZE+SLOT_GAP;
	}

}

// ----------------------------------------------------------------------------
// Name : RenderTradeItems()
// Desc : 
// ----------------------------------------------------------------------------

void CUICashShop::RenderTradeItems()
{
	int iShopX, iShopY , i;
	
	iShopX = TRADE_TOP_SLOT_SX + 50;	// eons
	iShopY = TRADE_TOP_SLOT_SY;
	
	int	nX = iShopX;
	int	nY = iShopY;
	int tv_pos = m_sbRight.GetScrollPos();

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for( i = tv_pos ; i < RIGHT_SLOT_SIZE_BUY+tv_pos; i++)
	{
		m_abtnTradeItems[i].SetPos( nX, nY );
		nY += SLOT_SIZE+SLOT_GAP;
		if( m_abtnTradeItems[i].IsEmpty() ){
			continue;
		}
		
		m_abtnTradeItems[i].Render();
		int tv_x, tv_y;
		m_abtnTradeItems[i].GetAbsPos(tv_x,tv_y);
		tv_x  += SLOT_GAP+SLOT_SIZE;
		pDrawPort->PutTextEx(m_abtnTradeItems[i].GetCashName(), tv_x,tv_y+SLOT_SIZE/4);
		
	}
	if(m_bShowTradeInfo){
		RenderItemInfo(m_abtnTradeItems[m_iShowItemIdx]);
		
	}

}

// ----------------------------------------------------------------------------
// Name : RenderKitItems()
// Desc : 
// ----------------------------------------------------------------------------

void CUICashShop::RenderKitItems()
{
	int iShopX, iShopY , i , j;
	
	iShopX = KIT_TOP_ITEM_SX+30;
	iShopY = KIT_TOP_ITEM_SY;
	
	int	nX = iShopX;
	int	nY = iShopY;
	
	for( i = 0, j=0 ; i < KIT_SLOT_TOTAL ; i++)
	{
		if((i%LEFT_SLOT_SIZE_BUY_INFO) == 0 && i > 0 ) j ++, nX = iShopX;
		nY = iShopY + (SLOT_SIZE+SLOT_GAP)*j;
		m_abtnKitItems[i].SetPos( nX, nY );
		nX += (SLOT_SIZE+SLOT_GAP);
		
		if( m_abtnKitItems[i].IsEmpty() ){
			continue;
		}
		
		m_abtnKitItems[i].Render();
	}
	if(m_bShowKitInfo){
		
		RenderItemInfo(m_abtnKitItems[m_iShowItemIdx]);
		
	}

}

// ----------------------------------------------------------------------------
// Name : RenderInvenItems()
// Desc : 
// ----------------------------------------------------------------------------

void CUICashShop::RenderInvenItems()
{
	int iShopX, iShopY , i , j;
	
	iShopX = INVEN_TOP_ITEM_SX+50;
	iShopY = INVEN_TOP_ITEM_SY;
	
	int	nX = iShopX;
	int	nY = iShopY;
	
	for( i = 0, j=0 ; i < INVEN_SLOT_TOTAL ; i++)
	{

		if((i%RIGHT_SLOT_SIZE_BUY) == 0 && i > 0) j ++ , nX = iShopX;
		nY = iShopY + (SLOT_SIZE+SLOT_GAP)*j;
		m_abtnInvenItems[i].SetPos( nX, nY );
		nX += (SLOT_SIZE+SLOT_GAP);
		
		if( m_abtnInvenItems[i].IsEmpty() ){
			continue;
		}
		
		m_abtnInvenItems[i].Render();

	}
	if(m_bShowInvenInfo){
		RenderItemInfo(m_abtnInvenItems[m_iShowItemIdx]);
		
	}

}

// ----------------------------------------------------------------------------
// Name : RenderHistoryItems()
// Desc : 
// ----------------------------------------------------------------------------


void CUICashShop::RenderHistoryItems()
{
	int nCnt = m_abtnHistoryItems.Count();
	if( nCnt <= 0 )
		return;

	int iShopX = HISTORY_TOP_ITEM_SX + 30;
	int iShopY = HISTORY_TOP_ITEM_SY;

	int	nX = iShopX;
	int	nY = iShopY;
	int tv_pos = m_sbHistory.GetScrollPos();
	int tv_max = HISTORY_SLOT_SIZE>nCnt?nCnt:HISTORY_SLOT_SIZE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for(int i = tv_pos ; i < tv_max+tv_pos; i++)
	{
		m_abtnHistoryItems[i].SetPos( nX, nY );
		nY += SLOT_SIZE+SLOT_GAP;
		if( m_abtnHistoryItems[i].IsEmpty() ){
			continue;
		}

		m_abtnHistoryItems[i].Render();
		int tv_x, tv_y;
		m_abtnHistoryItems[i].GetAbsPos(tv_x,tv_y);
		tv_x  += SLOT_GAP+SLOT_SIZE;

		// Item Name
		pDrawPort->PutTextEx(m_abtnHistoryItems[i].GetCashName(), tv_x,tv_y+SLOT_SIZE/4);

		CTString nTemp;
		// Item Count
		int tv_cnt=m_abtnHistoryItems[i].GetItemCount() ;
		nTemp.PrintF("%d",tv_cnt);
		pDrawPort->PutTextExCX(nTemp, tv_x+213,tv_y+SLOT_SIZE/4);

		// Item Price
		CCashShopData& CD	= _pNetwork->GetCashShopData(m_abtnHistoryItems[i].GetCashType());
		int tv_sum = CD.m_vShopItemArray[m_abtnHistoryItems[i].GetCashTypeIndex()].m_cash * tv_cnt;
		nTemp.PrintF("%d", tv_sum);
		pDrawPort->PutTextExCX(nTemp , tv_x+300,tv_y+SLOT_SIZE/4);
	}
}

void CUICashShop::RenderSendHistoryItems()
{
	int nCnt = m_abtnSendHistoryItems.Count();
	if( nCnt <= 0 )
		return;

	m_sbGift.SetCurItemCount(nCnt);

	int iShopX = HISTORY_TOP_ITEM_SX + 30;
	int iShopY = HISTORY_TOP_ITEM_SY + 31;

	int	nX = iShopX;
	int	nY = iShopY;
	int tv_pos = m_sbGift.GetScrollPos();
	int tv_max = HISTORY_SLOT_SIZE>nCnt?nCnt:HISTORY_SLOT_SIZE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for( int i = tv_pos ; i < tv_max+tv_pos; i++ )
	{
		m_abtnSendHistoryItems[i].SetPos( nX, nY );
		nY += SLOT_SIZE+SLOT_GAP;
		if( m_abtnSendHistoryItems[i].IsEmpty() ){
			continue;
		}

		m_abtnSendHistoryItems[i].Render();
		int tv_x, tv_y;
		m_abtnSendHistoryItems[i].GetAbsPos(tv_x,tv_y);
		tv_x  += SLOT_GAP+SLOT_SIZE;

		// Item Name
		pDrawPort->PutTextEx(m_abtnSendHistoryItems[i].GetCashName(), tv_x,tv_y+SLOT_SIZE/4);

		// 받은 사람 이름
		pDrawPort->PutTextExCX(m_astrSendChar[i] , tv_x+285,tv_y+SLOT_SIZE/4);
	}
}

void CUICashShop::RenderReceiveHistoryItems()
{
	int nCnt = m_abtnRecvHistoryItems.Count();
	if( nCnt <= 0 )
		return;

	m_sbGift.SetCurItemCount(nCnt);

	int iShopX = HISTORY_TOP_ITEM_SX + 30;
	int iShopY = HISTORY_TOP_ITEM_SY + 31;

	int	nX = iShopX;
	int	nY = iShopY;
	int tv_pos = m_sbGift.GetScrollPos();
	int tv_max = HISTORY_SLOT_SIZE>nCnt?nCnt:HISTORY_SLOT_SIZE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for( int i = tv_pos ; i < tv_max+tv_pos; i++ )
	{
		m_abtnRecvHistoryItems[i].SetPos( nX, nY );
		nY += SLOT_SIZE+SLOT_GAP;
		if( m_abtnRecvHistoryItems[i].IsEmpty() ){
			continue;
		}

		m_abtnRecvHistoryItems[i].Render();
		int tv_x, tv_y;
		m_abtnRecvHistoryItems[i].GetAbsPos(tv_x,tv_y);
		tv_x  += SLOT_GAP+SLOT_SIZE;

		// Item Name
		pDrawPort->PutTextEx(m_abtnRecvHistoryItems[i].GetCashName(), tv_x,tv_y+SLOT_SIZE/4);

		// 보낸 사람 이름
		pDrawPort->PutTextExCX(m_astrReceiveChar[i] , tv_x+285,tv_y+SLOT_SIZE/4);
	}
}

void CUICashShop::RenderRecvGift()
{
	int nCnt = m_abtnRecvGift.Count();
	if( nCnt <= 0 )
		return;

	int	nX = 107+35+5;
	int	nY = 95+5;

	int tv_pos = m_sbReceive.GetScrollPos();
	int tv_max = 2>nCnt ? nCnt:2;
	int nEnable =0;

	for( int j = 0 ; j< m_abtnRecvGift.sa_Count ; ++j)
		m_abtnRecvGift[j].SetEnable(FALSE);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for( int i = 0 ; nEnable<tv_max && i<nCnt ; ++i)
	{
		if( m_abtnRecvGift[i].IsEmpty() ){
			continue;
		}
		if( tv_pos >0)
		{
			--tv_pos;
			continue;
		}

		m_abtnRecvGift[i].SetEnable(TRUE);

		m_abtnRecvGift[i].SetPos( nX, nY );
		nY += 142+5;

		m_abtnRecvGift[i].Render();
		int tv_x, tv_y;
		m_abtnRecvGift[i].GetAbsPos(tv_x,tv_y);

		// Item Name
		pDrawPort->PutTextEx(m_abtnRecvGift[i].GetCashName(), tv_x+SLOT_GAP+SLOT_SIZE, tv_y+SLOT_SIZE/4);

		// Send Date
		CTString nTemp;
		struct tm *tmDate =localtime((time_t*)&m_anDate[m_anRecvOrder[i]]);
		nTemp.PrintF(_S( 3091, "%d년 %d월 %d일" ), tmDate->tm_year+1900, tmDate->tm_mon+1, tmDate->tm_mday);
		pDrawPort->PutTextEx(nTemp, tv_x+10 +70, tv_y +SLOT_GAP+SLOT_SIZE);

		// Send Character
		pDrawPort->PutTextEx(m_astrSend[m_anRecvOrder[i]], tv_x+10 +70, tv_y +SLOT_GAP+SLOT_SIZE +15);

		// Gift Message
		CTString temp = m_ebRecvGiftMsg[nEnable].GetString();
		if( temp != m_astrGiftMessage[m_anRecvOrder[i]] )
		{
			int	len	=strlen( m_astrGiftMessage[m_anRecvOrder[i]].str_String ) - 2;
			if ( strncmp ( "\r\n", m_astrGiftMessage[m_anRecvOrder[i]].str_String + len, 2 ) != 0 )
				m_astrGiftMessage[m_anRecvOrder[i]] +="\r\n";
			m_ebRecvGiftMsg[nEnable].SetString(m_astrGiftMessage[m_anRecvOrder[i]].str_String);
		}
		m_ebRecvGiftMsg[nEnable].Render();
		++nEnable;
	}

	RenderInvenItems();
}


// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::RenderItemInfo(CUIButtonEx& btnInfo)
{
	m_nCurInfoLines	= 0;

	CTString tv_name;
	tv_name.PrintF("%s",btnInfo.GetCashName());
	AddItemInfoString(tv_name,0xFF63B1FF,MAX_CASH_ITEMINFO_LINE,MAX_CASH_ITEMINFO_CHAR);
	AddItemInfoString(btnInfo.GetCashDesc(),0xFFF284FF,MAX_CASH_ITEMINFO_LINE,MAX_CASH_ITEMINFO_CHAR);

	int tv_x =  btnInfo.GetAbsPosX() + BTN_SIZE+SLOT_GAP;
	int tv_y =  btnInfo.GetAbsPosY();
	int tv_width = 25 - _pUIFontTexMgr->GetFontSpacing() + MAX_CASH_ITEMINFO_CHAR *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	int tv_height = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	m_rcItemInfo.SetRect(tv_x,tv_y,tv_x+tv_width,tv_y+tv_height);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Initialize texture data
	pDrawPort->InitTextureData(m_ptdBaseTexture);

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

	
	// Render item information
	int	nInfoX = m_rcItemInfo.Left + 12;
	int	nInfoY = m_rcItemInfo.Top + 8;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		pDrawPort->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}
}

// ----------------------------------------------------------------------------
// Name : RednerShopItemInfo()
// Desc : View Shop Item Infomation 
// ----------------------------------------------------------------------------
void CUICashShop::RednerShopItemInfo( CUIButtonEx& btnInfo )
{
	m_nCurInfoLines		= 0;

	AddItemInfoString(btnInfo.GetCashDesc(),0xFFF284FF,MAX_CASH_ITEMINFO_LINE,MAX_CASH_ITEMINFO_CHAR);	

	btnInfo.SetPos( 427, 111 );
	btnInfo.Render();

	int	nInfoX = btnInfo.GetAbsPosX() ;
	int	nInfoY = btnInfo.GetAbsPosY() ;

	int tv_pos = m_sbLeft.GetScrollPos();
	CCashShopData& CD = _pNetwork->GetCashShopData(m_nCurShopSection);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	CTString strName,strPrice,strCnt;
	strName =  btnInfo.GetCashName();
	strPrice.PrintF( "%s : %d", _S( 2397, "가격" ), CD.m_vShopItemArray[btnInfo.GetCashTypeIndex()].m_cash );
	if( SET_ITEM != btnInfo.GetCashType() )
	{
		if(!IsSetItem(btnInfo.GetCashIndex()))
		{
			strCnt.PrintF( "%s : %d", _S( 2396, "수량" ), CD.m_vShopItemArray[btnInfo.GetCashTypeIndex()].m_vItemDataArray[0].m_itemCnt );
			pDrawPort->PutTextEx( strCnt, nInfoX + 70, nInfoY + 20, 0x6BD2FFFF );
		}
	}
	pDrawPort->PutTextEx( strName, nInfoX + 70, nInfoY , 0xFF63B1FF );
	pDrawPort->PutTextEx( strPrice, nInfoX + 70, nInfoY + 40, 0x6BD2FFFF );

	nInfoY +=80;

	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		pDrawPort->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}
	

}

// ----------------------------------------------------------------------------
// Name : RenderOutline()
// Desc : render select button outline 
// ----------------------------------------------------------------------------
void CUICashShop::RenderOutline( const float fI0, const float fJ0 )
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
	// Flush all render text queue
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( fI0-1, fJ0, fI0+3, fJ0+2,
										m_rtSelectOutlineUL.U0, m_rtSelectOutlineUL.V0, m_rtSelectOutlineUL.U1, m_rtSelectOutlineUL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0+3, fJ0, fI0+29, fJ0+2,
										m_rtSelectOutlineUM.U0, m_rtSelectOutlineUM.V0, m_rtSelectOutlineUM.U1, m_rtSelectOutlineUM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0+29, fJ0, fI0+33, fJ0+2,
										m_rtSelectOutlineUR.U0, m_rtSelectOutlineUR.V0, m_rtSelectOutlineUR.U1, m_rtSelectOutlineUR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0-1, fJ0+2, fI0+1, fJ0+32,
										m_rtSelectOutlineML.U0, m_rtSelectOutlineML.V0, m_rtSelectOutlineML.U1, m_rtSelectOutlineML.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0+31, fJ0+2, fI0+33, fJ0+32,
										m_rtSelectOutlineMR.U0, m_rtSelectOutlineMR.V0, m_rtSelectOutlineMR.U1, m_rtSelectOutlineMR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0-1, fJ0+32, fI0+3, fJ0+34,
										m_rtSelectOutlineLL.U0, m_rtSelectOutlineLL.V0, m_rtSelectOutlineLL.U1, m_rtSelectOutlineLL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0+3, fJ0+32, fI0+29, fJ0+34,
										m_rtSelectOutlineLM.U0, m_rtSelectOutlineLM.V0, m_rtSelectOutlineLM.U1, m_rtSelectOutlineLM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( fI0+29, fJ0+32, fI0+33, fJ0+34,
										m_rtSelectOutlineLR.U0, m_rtSelectOutlineLR.V0, m_rtSelectOutlineLR.U1, m_rtSelectOutlineLR.V1,
										0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2 ,i;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 52;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 15;
	pDrawPort->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// Tab
	nX = m_nPosX + m_rcTab.Left;
	nX2 = nX + CASHSHOP_TAB_WIDTH;
	nY = m_nPosY + m_rcTab.Top;
	nY2 = m_nPosY + m_rcTab.Bottom;
	for( i= 0; i < UCST_MAIN_END; i++ )
	{
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
											m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
											0xFFFFFFFF );
		nX += CASHSHOP_TAB_WIDTH;
		nX2 += CASHSHOP_TAB_WIDTH;
	}

	// Close button
	m_btnClose.Render();
	
	// Cancel button
	m_btnCancel.Render();

#if defined(G_GERMAN)
	{
		m_btnBillingLink.Render();
	}
#endif
	// Text in cash shop
	// Title
	pDrawPort->PutTextEx( _S(2384,"유료 상점"), m_nPosX + CASHSHOP_TITLE_OFFSETX,
										m_nPosY + CASHSHOP_TITLE_OFFSETY, 0xFFFFFFFF );
	// Tab  CASHSHOP_TAB_WIDTH
	nY = m_nPosY + CASHSHOP_TAB_TEXT_OFFSETY;
	nX = m_nPosX + m_rcTab.Left + CASHSHOP_TAB_WIDTH / 2;
	
	pDrawPort->PutTextExCX( _S( 2570, "신상품" ), nX, nY,
											m_nCurShopTab == UCST_NEW_CASH ? 0xE1B300FF : 0x6B6B6BFF );
	nX += CASHSHOP_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S(2385,"아이템 구매"), nX, nY,	
											m_nCurShopTab == UCST_BUY ? 0xE1B300FF : 0x6B6B6BFF );
	nX += CASHSHOP_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S(2386,"구매 정보"), nX, nY,
											m_nCurShopTab == UCST_BUY_INFO ? 0xE1B300FF : 0x6B6B6BFF );
	nX += CASHSHOP_TAB_WIDTH;
	pDrawPort->PutTextExCX( _S(284,"도움말"), nX, nY,	
											m_nCurShopTab == UCST_HELP ? 0xE1B300FF : 0x6B6B6BFF );

	
	switch(m_nCurShopTab)
	{
	case UCST_NEW_CASH:
	case UCST_BUY:
		{
			// My Line Art T,.T
			nX = m_nPosX;
			nY = m_nPosY;
			
			// left horizon line
			pDrawPort->AddTexture( nX+128, nY+70, nX+384, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+128, nY+390, nX+384, nY+391,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			// right horizon line
			pDrawPort->AddTexture( nX+398, nY+70, nX+667, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+398, nY+90, nX+667, nY+91,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+398, nY+293, nX+667, nY+294,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+398, nY+350, nX+667, nY+351,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+398, nY+390, nX+667, nY+391,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			// cash box horizon 
			pDrawPort->AddTexture( nX+538, nY+304, nX+658, nY+305,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+538, nY+318, nX+658, nY+319,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+538, nY+327, nX+658, nY+328,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+538, nY+341, nX+658, nY+342,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );


			// left vertical line
			pDrawPort->AddTexture( nX+127, nY+70, nX+128, nY+390, 
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+384, nY+70, nX+385, nY+390, 
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			// right vertical line	
			pDrawPort->AddTexture( nX+398, nY+70, nX+399, nY+390,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+667, nY+70, nX+668, nY+390,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			// cash box vertical	
			pDrawPort->AddTexture( nX+538, nY+303, nX+539, nY+318,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+538, nY+328, nX+539, nY+342,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+658, nY+303, nX+659, nY+318,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+658, nY+328, nX+659, nY+342,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			
			//item slot box
			//left slot
			nY2=SLOT_SIZE+SLOT_GAP;
			for(i=0;i<LEFT_SLOT_SIZE_BUY;i++)
				pDrawPort->AddTexture( nX+133, nY+77+nY2*i, nX+166, nY+110+nY2*i,
												m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
												0xFFFFFFFF );

			if( !m_bShowShopInfo )	// 아이템 정보를 보여주지 않을때 
			{
				for(i=0;i<RIGHT_SLOT_SIZE_BASKET;i++)
					pDrawPort->AddTexture( nX+404, nY+97+nY2*i, nX+437, nY+130+nY2*i,
													m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
													0xFFFFFFFF );
			}			
			
			// buy item left button
			if( m_nCurShopTab == UCST_NEW_CASH )
			{
				m_btnNewItem.Render();
				m_btnHotItem.Render();
				m_btnPlatinum.Render();
			}
			else
			{
				m_btnConsume.Render();
				m_btnTimeLimit.Render();
				m_btnPotion.Render();
				m_btnAvata.Render();
				m_btnWeapon.Render();
				m_btnDefend.Render();
				m_btnSet.Render();
			}
			// set item detail button
	//		m_btnDetail.Render();
			
			// buy item right-bottom button  
			m_btnClear.Render();
			m_btnPay.Render();
			
			// Scrollbar 
			m_sbLeft.Render();
			
			switch(m_nCurShopSection)
			{
			case UCSS_CONSUME :
				break;
			case UCSS_TIME_LIMIT :
				break;
			case UCSS_POTION :
				break;
			case UCSS_AVATA : 
				break;
			case UCSS_WEAPON :
				break;
			case UCSS_DEFEND :
				break;
			case UCSS_SET :
				for(i=0;i<LEFT_SLOT_SIZE_BUY;i++)
				{
					m_aBtnSetItem[i].Render();
				}
				break;
			case UCSS_NEWITEM:
				break;
			case UCSS_HOTITEM:
				break;
			}
			pDrawPort->FlushRenderingQueue();

			// Draw Text
			if( m_bShowShopInfo ){ // 아이템 정보 보이기
				RednerShopItemInfo( m_abtnShowItem );
			}
			else	// 장바구니 보이기
			{
				m_sbRight.Render();
				pDrawPort->PutTextExCX( _S(2388,"장바구니"), m_nPosX+541,m_nPosY+75, 0xFFFFFFFF ); //( + 85 )
			}

			CTString tv_price;
			pDrawPort->PutTextExCX( _S(2389,"상품 금액"), m_nPosX+495,m_nPosY+305, 0xFFFFFFFF );			
			tv_price.PrintF("%d",CalculateKitPrice());
			pDrawPort->PutTextExRX( tv_price , m_nPosX+655,m_nPosY+305, 0xFFFFFFFF );
			pDrawPort->PutTextExCX( _S(2390,"보유 캐쉬"), m_nPosX+495,m_nPosY+329, 0xFFFFFFFF );
			tv_price.PrintF("%d",m_nMyCash);
			pDrawPort->PutTextExRX( tv_price , m_nPosX+655,m_nPosY+329, 0xFFFFFFFF );			

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Flush all render text queue
			pDrawPort->EndTextEx();

			RenderShopItems();
			
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

			if( !m_bShowShopInfo ) { RenderTradeItems(); }

// eons outline			
			if( m_iShowItemIdx != -1 && m_bShowShopInfo )
			{
				if(m_iShowItemIdx>=m_sbLeft.GetScrollPos() && m_iShowItemIdx<=(m_sbLeft.GetScrollPos() + LEFT_SLOT_SIZE_BUY -1))
					RenderOutline( nX+133, nY+77+nY2*(m_iShowItemIdx-m_sbLeft.GetScrollPos()));
			}
//
		}
		break;
		
	case UCST_BUY_INFO:
		
		// Art Line t..t
		// left horizon line
		nX = m_nPosX;
		nY = m_nPosY;

		// Draw button	
		m_btnInfoBuyItem.Render();
		m_btnInfoBuyHistory.Render();

		m_btnInfoGiftHistory.Render();		// "선물 내역" 버튼 렌더링 :Su-won
		m_btnInfoReceive.Render();			// "받은 선물" 버튼 렌더링: Su-won

		if(m_nCurShopSection==UCSS_BUY_INFO)
		{
				
			
			// left horizon line
			pDrawPort->AddTexture( nX+108+30, nY+70, nX+334+30, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+90, nX+334+30, nY+91,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+319, nX+334+30, nY+320,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			// right horizon line
			pDrawPort->AddTexture( nX+348+50, nY+70, nX+567+50, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+90, nX+567+50, nY+91,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+195, nX+567+50, nY+196,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+246, nX+567+50, nY+247,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

			// left vertical line
			pDrawPort->AddTexture( nX+107+30, nY+70, nX+108+30, nY+320,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+334+30, nY+70, nX+335+30, nY+320,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			// right vertical line
			pDrawPort->AddTexture( nX+348+50, nY+70, nX+349+50, nY+245,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+567+50, nY+70, nX+568+50, nY+245,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			// left item slot
			nY2=SLOT_SIZE+SLOT_GAP;
			for(i=0;i<LEFT_SLOT_SIZE_BUY_INFO;i++)
				for(int j=0;j<LEFT_SLOT_SIZE_BUY_INFO;j++)
					pDrawPort->AddTexture( nX+125+30+nY2*j, nY+110+nY2*i, nX+158+30+nY2*j, nY+143+nY2*i,
														m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
														0xFFFFFFFF );
			// right item slot
			for(i=0;i<RIGHT_SLOT_SIZE_MOVE_ROW;i++)
				for(int j=0;j<RIGHT_SLOT_SIZE_MOVE_COL;j++)
					pDrawPort->AddTexture( nX+363+50+nY2*j, nY+106+nY2*i, nX+396+50+nY2*j, nY+139+nY2*i,
														m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
														0xFFFFFFFF );
			m_btnInfoMove.Render();

			m_btnGift.Render();				// "선물하기" 버튼 렌더링 :Su-won

			// 선물 보내기창 렌더링 :Su-won
			if( m_bShowSendGift )
				RenderSendGiftUI();				

			// Draw text
			pDrawPort->PutTextExCX( _S(2392,"구매한 물품"), m_nPosX+220+30,m_nPosY+75, 0xFFFFFFFF );
			pDrawPort->PutTextExCX( _S(2393,"옮길 물품"), m_nPosX+456+50, m_nPosY+75, 0xFFFFFFFF );

		
			// Render all elements
			pDrawPort->FlushRenderingQueue();
			
			// Render all button elements
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

			// Draw button	
			RenderInvenItems();	
			RenderKitItems();
		} 
		else if(m_nCurShopSection==UCSS_BUY_HISTORY)
		{
			// horizon line
			pDrawPort->AddTexture( nX+108+30, nY+70, nX+567+30, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+319, nX+567+30, nY+320,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			// left vertical line
			pDrawPort->AddTexture( nX+107+30, nY+70, nX+108+30, nY+320,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+566+30, nY+70, nX+567+30, nY+320,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			pDrawPort->PutTextExCX( _S(2394,"구매 내역을 확인하시고 싶은 날짜를 선택해 주세요."), 
												m_nPosX+341+30,m_nPosY+95, 0xFFFFFFFF );
		

			// Black Box
			pDrawPort->AddTexture( nX+138+30, nY+139, nX+533+30, nY+315,
												m_rtBlackBox.U0, m_rtBlackBox.V0, m_rtBlackBox.U1, m_rtBlackBox.V1,
												0xFFFFFFFF );
			// Line Row
			pDrawPort->AddTexture( nX+138+30,nY+139,nX+533+30, nY+160,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
			// Line Column
			pDrawPort->AddTexture( nX+356+30,nY+139,nX+357+30, nY+315,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+442+30,nY+139,nX+443+30, nY+315,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
			// Item Slot
			nY2=SLOT_SIZE+SLOT_GAP;
			for(i=0;i<HISTORY_SLOT_SIZE;i++)
				pDrawPort->AddTexture( nX+149+30, nY+164+nY2*i, nX+182+30, nY+197+nY2*i,
													m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
													0xFFFFFFFF );
			m_sbHistory.Render();

			pDrawPort->PutTextEx( _S(2121,"년"), m_nPosX + 226+30, m_nPosY + 119, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(2122,"월"), m_nPosX + 338+30, m_nPosY + 119, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(2123,"일"), m_nPosX + 446+30, m_nPosY + 119, 0xFFFFFFFF );

//			if(!m_cbYear.IsDropList())			
				pDrawPort->PutTextEx( _S(2395,"물품"), m_nPosX + 225+30,	m_nPosY + 143, 0xFFFFFFFF );
//			if(!m_cbMonth.IsDropList())
				pDrawPort->PutTextEx( _S(2396,"수량"), m_nPosX + 381+30, m_nPosY + 143, 0xFFFFFFFF );
//			if(!m_cbDay.IsDropList())
				pDrawPort->PutTextEx( _S(2397,"가격"), m_nPosX + 482+30, m_nPosY + 143, 0xFFFFFFFF );
		
			// Search button
			m_btnSearch.Render();

			RenderHistoryItems();

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Render all button elements
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

			// Flush all render text queue
			pDrawPort->EndTextEx();
			
			// Set board texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );
			
			// List Box
			m_cbYear.Render();
			m_cbMonth.Render();
			m_cbDay.Render();
		}

		// 선물 내역 페이지 렌더링 :Su-won			|---------->
		else if(m_nCurShopSection==UCSS_BUY_SENDHISTORY || m_nCurShopSection==UCSS_BUY_RECEIVEHISTORY)
		{
			int nAddWidth =10;
			int nAddHeight =31;
			// horizon line
			pDrawPort->AddTexture( nX+108+30, nY+70, nX+567+30 +nAddWidth, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+319 +nAddHeight, nX+567+30 +nAddWidth, nY+320 +nAddHeight,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			// vertical line
			pDrawPort->AddTexture( nX+107+30, nY+70, nX+108+30, nY+320 +nAddHeight,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+566+30 +nAddWidth, nY+70, nX+567+30 +nAddWidth, nY+320 +nAddHeight,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			if( m_nCurShopSection==UCSS_BUY_SENDHISTORY)
				pDrawPort->PutTextExCX( _S(3092, "보낸 선물 내역을 확인하고 싶은 날짜를 선택해 주세요."), 
													m_nPosX+341+30,m_nPosY+95 +nAddHeight, 0xFFFFFFFF );
			else
				pDrawPort->PutTextExCX( _S(3093, "받은 선물 내역을 확인하고 싶은 날짜를 선택해 주세요."), 
													m_nPosX+341+30,m_nPosY+95 +nAddHeight, 0xFFFFFFFF );
		

			// Black Box
			pDrawPort->AddTexture( nX+138+30, nY+139 +nAddHeight, nX+533+30 +nAddWidth, nY+315 +nAddHeight,
												m_rtBlackBox.U0, m_rtBlackBox.V0, m_rtBlackBox.U1, m_rtBlackBox.V1,
												0xFFFFFFFF );
			// Line Row
			pDrawPort->AddTexture( nX+138+30,nY+139 +nAddHeight, nX+533+30 +nAddWidth, nY+160 +nAddHeight,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );

			pDrawPort->AddTexture( nX+399+30,nY+139+nAddHeight,nX+400+30, nY+315+nAddHeight,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );

			// Item Slot
			nY2=SLOT_SIZE+SLOT_GAP;
			for(i=0;i<HISTORY_SLOT_SIZE;i++)
				pDrawPort->AddTexture( nX+149+30, nY+164+nY2*i +nAddHeight, nX+182+30, nY+197+nY2*i +nAddHeight,
													m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
													0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(2121,"년"), m_nPosX + 226+30, m_nPosY + 119 +nAddHeight, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(2122,"월"), m_nPosX + 338+30, m_nPosY + 119 +nAddHeight, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(2123,"일"), m_nPosX + 446+30, m_nPosY + 119 +nAddHeight, 0xFFFFFFFF );


			//if(!m_cbMonth.IsDropList())
			//	pDrawPort->PutTextEx( _S(2396,"수량"), m_nPosX + 381+30, m_nPosY + 143 +nAddHeight, 0xFFFFFFFF );
			
			if( m_nCurShopSection==UCSS_BUY_SENDHISTORY)
			{
				pDrawPort->PutTextEx( _S(3094, "보낸 물품"), m_nPosX + 225+30,	m_nPosY + 143 +nAddHeight, 0xFFFFFFFF );
				pDrawPort->PutTextEx( _S(3095, "받는 캐릭터명"), m_nPosX + 482+5-20, m_nPosY + 143 +nAddHeight, 0xFFFFFFFF );
			}
			else
			{
				pDrawPort->PutTextEx( _S(3096, "받은 물품"), m_nPosX + 225+30,	m_nPosY + 143 +nAddHeight, 0xFFFFFFFF );
				pDrawPort->PutTextEx( _S(3097, "보낸 캐릭터명"), m_nPosX + 482+5-20, m_nPosY + 143 +nAddHeight, 0xFFFFFFFF );
			}
		
			m_btnInfoSendHistory.Render();		//'보낸 선물내역 확인'	버튼 렌더링
			m_btnInfoReceiveHistory.Render();	//'받은 선물내역 확인'	버튼 렌더링

			// "검색" 버튼 렌더링
			m_btnGiftSearch.Render();

			// 선물 내역 랜더링
			if( m_nCurShopSection==UCSS_BUY_SENDHISTORY )
				RenderSendHistoryItems();
			if( m_nCurShopSection==UCSS_BUY_RECEIVEHISTORY )
				RenderReceiveHistoryItems();

			// 스크롤바 렌더링
			m_sbGift.Render();

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Render all button elements
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

			// Flush all render text queue
			pDrawPort->EndTextEx();
			
			// Set board texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );
			
			// List Box
			m_cbGiftYear.Render();
			m_cbGiftMonth.Render();
			m_cbGiftDay.Render();
		}
		// 선물 내역 페이지 렌더링 :Su-won			<----------|

		// 받은 선물 페이지 렌더링 :Su-won			|---------->
		else if(m_nCurShopSection==UCSS_BUY_RECEIVE)
		{
			// left horizon line
			pDrawPort->AddTexture( nX+108+30, nY+70, nX+334+30, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+90, nX+334+30, nY+91,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+30, nY+319 +70, nX+334+30, nY+320 +70,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

			pDrawPort->AddTexture( nX+108+35, nY+95, nX+334+15, nY+96,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+35, nY+95+142, nX+334+15, nY+96+142,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+35, nY+95+142+5, nX+334+15, nY+96+142+5,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+108+35, nY+95+142+5+142, nX+334+15, nY+96+142+5+142,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

			// right horizon line
			pDrawPort->AddTexture( nX+348+50, nY+70, nX+567+50, nY+71,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+90, nX+567+50, nY+91,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+195, nX+567+50, nY+196,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+348+50, nY+246, nX+567+50, nY+247,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

			// left vertical line
			pDrawPort->AddTexture( nX+107+30, nY+70, nX+108+30, nY+320 +70,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+334+30, nY+70, nX+335+30, nY+320 +70,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			pDrawPort->AddTexture( nX+107+35, nY+95, nX+108+35, nY+95+142,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+334+15, nY+95, nX+335+15, nY+95+142,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+107+35, nY+95+142+5, nX+108+35, nY+95+142+5+142,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+334+15, nY+95+142+5, nX+335+15, nY+95+142+5+142,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			// right vertical line
			pDrawPort->AddTexture( nX+348+50, nY+70, nX+349+50, nY+245,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );
			pDrawPort->AddTexture( nX+567+50, nY+70, nX+568+50, nY+245,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

			
			// left item slot
			pDrawPort->AddTexture( nX+107+35+5, nY+95+5, nX+107+35+5 +SLOT_SIZE, nY+95+5 +SLOT_SIZE,
														m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
														0xFFFFFFFF );
			pDrawPort->AddTexture( nX+107+35+5, nY+95+10 +142, nX+107+35+5 +SLOT_SIZE, nY+95+10 +SLOT_SIZE +142,
														m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
														0xFFFFFFFF );

			// right item slot
			nY2=SLOT_SIZE+SLOT_GAP;
			for(i=0;i<RIGHT_SLOT_SIZE_MOVE_ROW;i++)
				for(int j=0;j<RIGHT_SLOT_SIZE_MOVE_COL;j++)
					pDrawPort->AddTexture( nX+363+50+nY2*j, nY+106+nY2*i, nX+396+50+nY2*j, nY+139+nY2*i,
														m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
														0xFFFFFFFF );
			m_btnInfoMove.Render();

			m_sbReceive.Render();

			// Draw text
			pDrawPort->PutTextExCX( _S(3098, "받은 선물함"), m_nPosX+220+30,m_nPosY+75, 0xFFFFFFFF );
			pDrawPort->PutTextExCX( _S(2393,"옮길 물품"), m_nPosX+456+50, m_nPosY+75, 0xFFFFFFFF );

			pDrawPort->PutTextEx( _S(3099, "보낸날짜:"), nX+107+35+5 +10, nY+95+5 +nY2, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(3100, "보낸친구:"), nX+107+35+5 +10, nY+95+5 +nY2+15, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(3101, "선물메시지"), nX+107+35+5 +10, nY+95+5 +nY2+30, 0xFFFFFFFF );
			
			pDrawPort->PutTextEx( _S(3099, "보낸날짜:"), nX+107+35+5 +10, nY+95+10 +nY2 +142, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(3100, "보낸친구:"), nX+107+35+5 +10, nY+95+10 +nY2+15 +142, 0xFFFFFFFF );
			pDrawPort->PutTextEx( _S(3101, "선물메시지"), nX+107+35+5 +10, nY+95+10 +nY2+30 +142, 0xFFFFFFFF );

			
			pDrawPort->AddTexture( nX+147 ,nY+100+nY2+48, nX+157 +187+3 , nY+100+nY2+48 +45-3,//+m_ebGiftMessage.GetHeight(),
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
			
			pDrawPort->AddTexture( nX+147 ,nY+100+nY2+48 +147, nX+157 +187+3 , nY+100+nY2+48 +147 +45-3,//+m_ebGiftMessage.GetHeight(),
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
		
			// Render all elements
			pDrawPort->FlushRenderingQueue();
			
			// Render all button elements
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

			RenderRecvGift();
		}
		// 받은 선물 페이지 렌더링 :Su-won			<----------|
	
		break;
	case UCST_HELP:
		{
			RenderHelp();
		}
		break;

	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	if(m_pSetInfo->IsEnabled() && m_pSetInfo->IsVisible()) {
				m_pSetInfo->Render(); 
				// Render all button elements
				pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
				
				// Render all elements
				pDrawPort->FlushRenderingQueue();

				// Flush all render text queue
				pDrawPort->EndTextEx();
	}
	if(m_pConfirmInfo->IsEnabled() && m_pConfirmInfo->IsVisible()) {
		m_pConfirmInfo->Render();
		// Render all button elements
		pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
		// Render all elements
		pDrawPort->FlushRenderingQueue();
		// Flush all render text queue
		pDrawPort->EndTextEx();
	}

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShop::MouseMessage( MSG *pMsg )
{
	if(m_pConfirmInfo->IsEnabled()&&m_pConfirmInfo->IsVisible()) return m_pConfirmInfo->MouseMessage(pMsg);
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	static int nOldLeftScroll = m_sbLeft.GetScrollPos();
	static int nOldTabNum = 0;
//	m_btnClose.SetWndState(UBS_IDLE);
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	m_x=nX;
	m_y=nY;
	// temp var
	int i;
	if(m_pSetInfo!=NULL && m_pSetInfo->IsEnabled()) {
		if(m_pSetInfo->MouseMessage(pMsg)==WMSG_SUCCESS) return WMSG_SUCCESS;
	}
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			if( !IsInsideRect( nX, nY, m_rcShopItems ) )
				InitShowInfo();

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				pUIManager->SetMouseCursorInsideUIs();
				
				ButtonMoveMsg( pMsg );

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ){}
				
				// Move board
				else if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );

					return WMSG_SUCCESS;
				}
				// Hold item button
				
				else if( pUIManager->GetHoldBtn().IsEmpty() && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
					( ndX != 0 || ndY != 0 ) )
				{
					InitShowInfo();

					if(m_nShopItemID >=0){
						pUIManager->SetHoldBtn( m_pbtnShopItems[m_nShopItemID] );
						int	nOffset = BTN_SIZE / 2;
						pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
						m_pbtnShopItems[m_nShopItemID].SetBtnState( UBES_IDLE );
						bLButtonDownInItem	= FALSE;
					} else if(m_nTradeItemID >=0){
						pUIManager->SetHoldBtn( m_abtnTradeItems[m_nTradeItemID] );
						int	nOffset = BTN_SIZE / 2;
						pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
						m_abtnTradeItems[m_nTradeItemID].SetBtnState( UBES_IDLE );
						bLButtonDownInItem	= FALSE;

					}else if(m_nKitItemID >=0){
						pUIManager->SetHoldBtn( m_abtnKitItems[m_nKitItemID] );
						int	nOffset = BTN_SIZE / 2;
						pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
						m_abtnKitItems[m_nKitItemID].SetBtnState( UBES_IDLE );
						bLButtonDownInItem	= FALSE;
					} else if(m_nInvenItemID >=0){
						pUIManager->SetHoldBtn( m_abtnInvenItems[m_nInvenItemID] );
						int	nOffset = BTN_SIZE / 2;
						pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
						m_abtnInvenItems[m_nInvenItemID].SetBtnState( UBES_IDLE );
						bLButtonDownInItem	= FALSE;
					} else if(m_nRecvItemID >=0){
					pUIManager->SetHoldBtn( m_abtnRecvGift[m_nRecvItemID] );
					int	nOffset = BTN_SIZE / 2;
					pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );
					m_abtnRecvGift[m_nRecvItemID].SetBtnState( UBES_IDLE );
					bLButtonDownInItem	= FALSE;
					}


				}
					
				// ScrollBar
				else if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH )
					{	
						// 구매 아이템 정보 표시(기존 툴팁박스 사용)
						if( IsInsideRect( nX, nY, m_rcTradeItems ) )
						{
			//				m_nShopItemID = -1;
			//				m_nTradeItemID = -1;
			//				m_nKitItemID = -1;
			//				m_nInvenItemID = -1;
							for( i = 0; i < RIGHT_SLOT_SIZE_BUY; i++ )
							{
								WMSG_RESULT tv_msg =m_abtnTradeItems[i+m_sbRight.GetScrollPos()].MouseMessage( pMsg );
								if( tv_msg != WMSG_FAIL )
									if ( tv_msg == WMSG_SUCCESS ) {
										
										// Show Info
										if(m_iShowItemIdx == i+m_sbRight.GetScrollPos())  {
											BOOL tv_show = !m_bShowTradeInfo;
											InitShowInfo();
											m_bShowTradeInfo = tv_show;
										}
										else {
											InitShowInfo();								
											m_iShowItemIdx = i+m_sbRight.GetScrollPos();
											m_bShowTradeInfo = TRUE;
										}
										
										pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
										return tv_msg;							
									}
										
							}					
						}
						// Left Scroll Bar
						else if( (wmsgResult = m_sbLeft.MouseMessage( pMsg ) ) != WMSG_FAIL ){
						//	PrepareShopItems();
						//	m_abtnShowItem.Copy( m_pbtnShopItems[m_iShowItemIdx] );
							return wmsgResult;
						}
						// Right Scroll Bar
						else if((wmsgResult = m_sbRight.MouseMessage( pMsg ) ) != WMSG_FAIL){
							return wmsgResult;
						}
					}
				else if(m_nCurShopTab == UCST_BUY_INFO )
				{
					if(m_nCurShopSection == UCSS_BUY_INFO){
						

					}
					else if(m_nCurShopSection == UCSS_BUY_HISTORY)
					{
						if( m_cbYear.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
						else if( m_cbMonth.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						else if( m_cbDay.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						else if( m_sbHistory.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					else if(m_nCurShopSection == UCSS_BUY_SENDHISTORY || m_nCurShopSection == UCSS_BUY_RECEIVEHISTORY)
					{
						if( m_cbGiftYear.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
						else if( m_cbGiftMonth.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						else if( m_cbGiftDay.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						else if( m_sbGift.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
				}
					else if(m_nCurShopSection == UCSS_BUY_RECEIVE)
					{
						if( m_sbReceive.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;

						if( m_ebRecvGiftMsg[0].MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						if( m_ebRecvGiftMsg[1].MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				SetFocus ( TRUE );
				pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );

				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
			
				}
				// Close button
				if( ( m_btnClose.MouseMessage( pMsg ) )  != WMSG_FAIL ) {}
				// Tab
				else if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					m_nCurShopTab		= ( nX - m_nPosX - m_rcTab.Left ) / CASHSHOP_TAB_WIDTH;
					if(m_nCurShopTab!=m_nOldShopTab)
					{
						m_nOldShopTab	= m_nCurShopTab; 
						if( m_nCurShopTab == UCST_BUY) {
							m_nCurShopSection=UCSS_CONSUME;
							PrepareShopItems();
						}
						else if( m_nCurShopTab == UCST_NEW_CASH ) {
							m_nCurShopSection=UCSS_NEWITEM;
							PrepareShopItems();
						}
						else if( m_nCurShopTab == UCST_BUY_INFO){
							m_nCurShopSection=UCSS_BUY_INFO;

							m_bShowSendGift =FALSE;
							ClearBtnItems(m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);
							ClearBtnItems(m_abtnKitItems.sa_Array,KIT_SLOT_TOTAL);
							_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASELIST_REQ);
						}
					
					}
				
				}
			
				else if( ( m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL ) {}
				else if( ( m_btnBillingLink.MouseMessage(pMsg))!= WMSG_FAIL) {}
				else if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH )
				{	
					if( ( m_btnConsume.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnTimeLimit.MouseMessage(pMsg ) ) != WMSG_FAIL &&	m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnNewItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
					else if( ( m_btnHotItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
					else if( ( m_btnPlatinum.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
					else if( ( m_btnPotion.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnAvata.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnWeapon.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnDefend.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
					else if( ( m_btnSet.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	//				else if( ( m_btnDetail.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					else if( ( m_btnClear.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					else if( ( m_btnPay.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					else if( ( m_btnInfoBuyItem.MouseMessage(pMsg))!= WMSG_FAIL) {}
					else if( ( m_btnInfoBuyHistory.MouseMessage(pMsg))!= WMSG_FAIL) {}
					

					if( ( m_sbLeft.MouseMessage(pMsg)) != WMSG_FAIL ){}
					else if( ( m_sbRight.MouseMessage(pMsg)) != WMSG_FAIL ){}					
					// Shop Items
					else if( IsInsideRect( nX, nY, m_rcShopItems ) )
					{
						m_nShopItemID = -1;
						m_nTradeItemID = -1;
						m_nKitItemID = -1;
						m_nInvenItemID = -1;
						m_nRecvItemID = -1;


						for( i = m_sbLeft.GetScrollPos(); i < m_shopItemCnt; i++ )
						{
							WMSG_RESULT tv_msg =m_pbtnShopItems[i].MouseMessage( pMsg );
							if( tv_msg != WMSG_FAIL )
								if ( tv_msg == WMSG_SUCCESS ) {
									m_nShopItemID=i;
									bLButtonDownInItem	= TRUE;

									// Show Info
									if(m_iShowItemIdx == m_nShopItemID && nOldTabNum == m_nCurShopTab )  {
										BOOL tv_show = !m_bShowShopInfo;
										InitShowInfo();
										m_bShowShopInfo = tv_show;
										
										if( m_bShowShopInfo ) 
											{ m_abtnShowItem.Copy( m_pbtnShopItems[i] ); }
									}
									else {
										InitShowInfo();
										m_iShowItemIdx = m_nShopItemID;
										nOldTabNum = m_nCurShopTab;
										m_bShowShopInfo = TRUE;
										m_abtnShowItem.Copy( m_pbtnShopItems[i] );
									}

									pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
									return tv_msg;								
								}
								
						}

					}
					// Trade Items
					else if( IsInsideRect( nX, nY, m_rcTradeItems ) )
					{
						m_nShopItemID = -1;
						m_nTradeItemID = -1;
						m_nKitItemID = -1;
						m_nInvenItemID = -1;
						m_nRecvItemID = -1;


						for( i = 0; i < RIGHT_SLOT_SIZE_BUY; i++ )
						{
							WMSG_RESULT tv_msg =m_abtnTradeItems[i+m_sbRight.GetScrollPos()].MouseMessage( pMsg );
							if( tv_msg != WMSG_FAIL )
								if ( tv_msg == WMSG_SUCCESS ) {
									m_nTradeItemID=i+m_sbRight.GetScrollPos();
									bLButtonDownInItem	= TRUE;	
									return tv_msg;							
								}
								
						}						

					}

					if( m_nCurShopSection == UCSS_SET ){
						for(i=0; i<LEFT_SLOT_SIZE_BUY ; i++){
							if( (m_aBtnSetItem[i].MouseMessage(pMsg))!= WMSG_FAIL) { }
						}
					}

				}
				else if( m_nCurShopTab == UCST_BUY_INFO)
				{
					if( ( m_btnInfoBuyItem.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					else if( ( m_btnInfoBuyHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					else if( ( m_btnInfoGiftHistory.MouseMessage(pMsg))!= WMSG_FAIL) {}
					else if( ( m_btnInfoReceive.MouseMessage(pMsg))!= WMSG_FAIL) {}
								
					if(m_nCurShopSection == UCSS_BUY_HISTORY)
					{
						if( ( m_cbYear.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_cbMonth.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_cbDay.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_sbHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_btnSearch.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}


					}
					else if(m_nCurShopSection == UCSS_BUY_INFO)
					{
						if( (m_btnInfoMove.MouseMessage(pMsg)) != WMSG_FAIL ){}	
						else if( (m_btnGift.MouseMessage(pMsg)) != WMSG_FAIL ){}
						else if( (m_btnSend.MouseMessage(pMsg)) != WMSG_FAIL ){}
						else if( (m_ebChar.MouseMessage(pMsg)) != WMSG_FAIL )	
						{
							m_ebChar.SetFocus(TRUE);
							m_ebGiftMessage.SetFocus(FALSE);
						}
						else if( (m_ebGiftMessage.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							m_ebChar.SetFocus(FALSE);
							m_ebGiftMessage.SetFocus(TRUE);
						}
						else if( IsInsideRect( nX, nY, m_rcKitItems ) )
						{
							m_nKitItemID = -1;
							m_nInvenItemID = -1;
							m_nShopItemID = -1;
							m_nTradeItemID = -1;
							m_nRecvItemID = -1;

							for( i = 0; i < KIT_SLOT_TOTAL; i++ )
							{
								WMSG_RESULT tv_msg =m_abtnKitItems[i].MouseMessage( pMsg );
								if( tv_msg != WMSG_FAIL )
									if ( tv_msg == WMSG_SUCCESS ) {
										m_nKitItemID=i;
										bLButtonDownInItem	= TRUE;	

										// Show Info
										if(m_iShowItemIdx == m_nKitItemID)  {
											BOOL tv_show = !m_bShowKitInfo;
											InitShowInfo();
											m_bShowKitInfo = tv_show;
										}
										else {
											InitShowInfo();								
											m_iShowItemIdx = m_nKitItemID;
											m_bShowKitInfo = TRUE;
									
										}

										pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
										return tv_msg;	
									}
									
							}

						}
						else if( IsInsideRect( nX, nY, m_rcInvenItems ) )
						{
							m_nKitItemID = -1;
							m_nInvenItemID = -1;
							m_nShopItemID = -1;
							m_nTradeItemID = -1;
							m_nRecvItemID = -1;

							for( i = 0; i < INVEN_SLOT_TOTAL; i++ )
							{
								WMSG_RESULT tv_msg =m_abtnInvenItems[i].MouseMessage( pMsg );
								if( tv_msg != WMSG_FAIL )
								{
									if ( tv_msg == WMSG_SUCCESS ) 
									{
										m_nInvenItemID=i;
										bLButtonDownInItem	= TRUE;	

										// Show Info
										if(m_iShowItemIdx == m_nInvenItemID)  {
											BOOL tv_show = !m_bShowInvenInfo;
											InitShowInfo();
											m_bShowInvenInfo = tv_show;
										}
										else {
											InitShowInfo();								
											m_iShowItemIdx = m_nInvenItemID;
											m_bShowInvenInfo = TRUE;
									
										}
										pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
										return tv_msg;	
									}
								}
							}
						}
					}
					// Gift 관련 페이지 처리 :Su-won	|---------->
					else if(m_nCurShopSection == UCSS_BUY_SENDHISTORY )
					{
						if( ( m_cbGiftYear.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							 m_nSendYear =m_cbGiftYear.GetCurSel();
						else if( ( m_cbGiftMonth.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							m_nSendMonth =m_cbGiftMonth.GetCurSel();
						else if( ( m_cbGiftDay.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							m_nSendDay =m_cbGiftDay.GetCurSel();
						else if( ( m_btnGiftSearch.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_sbGift.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_btnInfoSendHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_btnInfoReceiveHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					}
					else if( m_nCurShopSection == UCSS_BUY_RECEIVEHISTORY )
					{
						if( ( m_cbGiftYear.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							m_nRecvYear =m_cbGiftYear.GetCurSel();
						else if( ( m_cbGiftMonth.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							m_nRecvMonth =m_cbGiftMonth.GetCurSel();
						else if( ( m_cbGiftDay.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
							m_nRecvDay =m_cbGiftDay.GetCurSel();
						else if( ( m_btnGiftSearch.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_sbGift.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_btnInfoSendHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( ( m_btnInfoReceiveHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
					}
										
					else if(m_nCurShopSection == UCSS_BUY_RECEIVE)
					{
						if( (m_btnInfoMove.MouseMessage(pMsg)) != WMSG_FAIL ){}	
						else if( ( m_sbReceive.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
						else if( (m_ebRecvGiftMsg[0].MouseMessage(pMsg)) != WMSG_FAIL )
						{
							m_ebRecvGiftMsg[0].SetFocus(FALSE);
						}
						else if( (m_ebRecvGiftMsg[1].MouseMessage(pMsg)) != WMSG_FAIL )
						{
							m_ebRecvGiftMsg[1].SetFocus(FALSE);
									}
									

						if( IsInsideRect( nX, nY, m_rcRecvItems ) )
						{
							m_nKitItemID = -1;
							m_nInvenItemID = -1;
							m_nShopItemID = -1;
							m_nTradeItemID = -1;
							m_nRecvItemID = -1;

							int tv_pos = m_sbReceive.GetScrollPos();
							int tv_max = 2>m_abtnRecvGift.sa_Count ? m_abtnRecvGift.sa_Count:2;
							for( i = 0; i < m_abtnRecvGift.sa_Count; i++ )
							{
								WMSG_RESULT tv_msg =m_abtnRecvGift[i].MouseMessage( pMsg );
								if( tv_msg != WMSG_FAIL )
									if ( tv_msg == WMSG_SUCCESS ) {
										m_nRecvItemID=i;
										bLButtonDownInItem	= TRUE;	

										// Show Info
										if(m_iShowItemIdx == m_nRecvItemID)  {
											BOOL tv_show = !m_bShowKitInfo;
											InitShowInfo();
											m_bShowKitInfo = tv_show;
							}
										else {
											InitShowInfo();								
											m_iShowItemIdx = m_nRecvItemID;
											m_bShowKitInfo = TRUE;
									
										}

										pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
										return tv_msg;	
									}
							}
						}
						else if( IsInsideRect( nX, nY, m_rcInvenItems ) )
						{
							m_nKitItemID = -1;
							m_nInvenItemID = -1;
							m_nShopItemID = -1;
							m_nTradeItemID = -1;
							m_nRecvItemID = -1;
						
							for( i = 0; i < INVEN_SLOT_TOTAL; i++ )
							{
								WMSG_RESULT tv_msg =m_abtnInvenItems[i].MouseMessage( pMsg );
								if( tv_msg != WMSG_FAIL )
									if ( tv_msg == WMSG_SUCCESS ) {
										m_nInvenItemID=i;
										bLButtonDownInItem	= TRUE;	

										// Show Info
										if(m_iShowItemIdx == m_nInvenItemID)  {
											BOOL tv_show = !m_bShowInvenInfo;
											InitShowInfo();
											m_bShowInvenInfo = tv_show;
										}
										else {
											InitShowInfo();								
											m_iShowItemIdx = m_nInvenItemID;
											m_bShowInvenInfo = TRUE;

										}
										pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
										return tv_msg;	
			
									}
						}
					
					}
				}
					// Gift 관련 페이지 처리 :Su-won	<----------|
				}
							
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;
				// Item clicked
				bLButtonDownInItem	= FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL  )
				{
					if(wmsgResult== WMSG_COMMAND)	
						CloseCashShop();
				}
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL  )
				{
					if(wmsgResult== WMSG_COMMAND)
						CloseCashShop();
				}
				else if( ( wmsgResult = m_btnBillingLink.MouseMessage(pMsg)) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						OpenBillingHomePage();
					}
				}
				else if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH )
				{				
					if( ( wmsgResult = m_btnConsume.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND)
						{
							m_nCurShopSection=UCSS_CONSUME;												
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnTimeLimit.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_TIME_LIMIT;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnNewItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_NEWITEM;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnHotItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_HOTITEM;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnPlatinum.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_PLATINUM;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnPotion.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_POTION;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnAvata.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_AVATA;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnWeapon.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_WEAPON;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnDefend.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_DEFEND;
							PrepareShopItems(  );
						}
					}
					else if( ( wmsgResult = m_btnSet.MouseMessage(pMsg)) != WMSG_FAIL && m_nCurShopTab == UCST_BUY )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_SET;
							PrepareShopItems(  );
						}
					}					
					else if( ( wmsgResult = m_btnClear.MouseMessage(pMsg)) != WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND){
							ClearBtnItems(m_abtnTradeItems,(int)MAX_KIT_SIZE);
							m_nTradeItemID = - 1;
						}
						// CLEAR Action
					}
					else if( ( wmsgResult = m_btnPay.MouseMessage(pMsg)) != WMSG_FAIL )
					{
						// TODO : 주문 내역서 출력
						m_pConfirmInfo->SetEnable(TRUE);
						m_pConfirmInfo->SetVisible(TRUE);		
						return wmsgResult;
					}
					else if( IsInsideRect( nX, nY, m_rcShopItems ) )
					{
						for( i = m_sbLeft.GetScrollPos(); i < m_shopItemCnt ; i++ )
						{
							if( m_pbtnShopItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Nothing
								return WMSG_SUCCESS;
							}
						}
					
					}
					else if( IsInsideRect( nX, nY, m_rcTradeItems ) )
					{
						for( i = 0; i < RIGHT_SLOT_SIZE_BUY; i++ )
						{
							if( m_abtnTradeItems[i+m_sbRight.GetScrollPos()].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								//Nothing
								return WMSG_SUCCESS;
							}
						}
					
					}
					else if( ( m_sbLeft.MouseMessage(pMsg)) != WMSG_FAIL ){
//						PrepareShopItems();
					}
					else if( ( m_sbRight.MouseMessage(pMsg)) != WMSG_FAIL ){
//						PrepareShopItems();					
					}					
					
					if(m_nCurShopSection == UCSS_SET) {
						for( i=0; i<LEFT_SLOT_SIZE_BUY ; i++){
							if((wmsgResult=m_aBtnSetItem[i].MouseMessage(pMsg))!=WMSG_FAIL)
							{
							
								if(m_shopItemCnt<=(i+m_sbLeft.GetScrollPos())){
									return wmsgResult;
								}
					//			if(m_pSetInfo!=NULL) {
					//				delete(m_pSetInfo);
					//			}
					//			if(!m_pSetInfo->IsEnabled()) 
					//				delete(m_pSetInfo);
					//			m_pSetInfo = new CUISetInfo;
								m_pSetInfo->Create(NULL,m_nPosX+279,m_nPosY+65,SETINFO_WIDTH,SETINFO_HEIGHT,
									m_pbtnShopItems[i+m_sbLeft.GetScrollPos()].GetCashListCount());
								m_pSetInfo->SetItemInfo(m_pbtnShopItems[i+m_sbLeft.GetScrollPos()]);
								m_pSetInfo->SetEnable(TRUE);								
								m_pSetInfo->SetVisible(TRUE);
								return wmsgResult;
							
							
							}
						}
					}
	
				}
				else if( m_nCurShopTab == UCST_BUY_INFO)
				{
					if( ( wmsgResult = m_btnInfoBuyItem.MouseMessage(pMsg)) != WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND){
							m_nCurShopSection=UCSS_BUY_INFO;

							m_bShowSendGift =FALSE;
							ClearBtnItems(m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);
							ClearBtnItems(m_abtnKitItems.sa_Array,KIT_SLOT_TOTAL);
							_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASELIST_REQ);
						}
					}
					else if( ( wmsgResult = m_btnInfoBuyHistory.MouseMessage(pMsg)) != WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND)
							m_nCurShopSection=UCSS_BUY_HISTORY;												
					}
					
					// Gift 관련 페이지 :Su-won	|---------->
					else if( ( m_btnInfoGiftHistory.MouseMessage(pMsg))!= WMSG_FAIL) 
					{
						if( m_nCurShopSection != UCSS_BUY_SENDHISTORY && m_nCurShopSection != UCSS_BUY_RECEIVEHISTORY)
						{
							m_nCurShopSection=UCSS_BUY_SENDHISTORY;

							m_cbGiftYear.SetCurSel(m_nSendYear);
							m_cbGiftMonth.SetCurSel(m_nSendMonth);
							m_cbGiftDay.SetCurSel(m_nSendDay);
							
							m_sbGift.SetCurItemCount(m_abtnSendHistoryItems.sa_Count);
							m_sbGift.SetScrollPos(m_nSendScrollPos);
						}
					}
					else if( ( m_btnInfoReceive.MouseMessage(pMsg))!= WMSG_FAIL) 
					{
						m_nCurShopSection=UCSS_BUY_RECEIVE;

						ClearBtnItems(m_abtnRecvGift.sa_Array, m_abtnRecvGift.sa_Count);
						ClearBtnItems(m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);

						_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECVLIST_REQ);
					}
					// 추가(Gift): Su-won	<----------|
					
				
					if( m_nCurShopSection == UCSS_BUY_INFO){
						if((wmsgResult=m_btnInfoMove.MouseMessage(pMsg)) != WMSG_FAIL ){
							if(wmsgResult == WMSG_COMMAND && !m_bBringItem){
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_BRING_REQ);
								m_bBringItem = TRUE;
								return wmsgResult;
							}
						}	
						
						// Gift 관련 처리 :Su-won	|---------->
						// '선물하기' 버튼 클릭
						else if( (m_btnGift.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							m_bShowSendGift = !m_bShowSendGift;
							if( m_bShowSendGift )
							{
								pUIManager->RearrangeOrder( UI_CASH_SHOP, TRUE );
								m_ebChar.SetFocus(TRUE);
								m_ebGiftMessage.SetFocus(FALSE);

								m_ebChar.ResetString();
								m_ebGiftMessage.ResetString();
							}
							return WMSG_SUCCESS;
						}
						//"보내기" 버튼 클릭
						else if( (wmsgResult=m_btnSend.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								Message( MSGCMD_GIFT_SEND, _S(3102, "선물 보내기"), _S(3103, "옮길 물품에 있는 아이템을 위의 친구에게 선물하시겠습니까?"), UMBS_YESNO);
															
								return wmsgResult;
							}
						}
						else if( (m_ebChar.MouseMessage(pMsg)) != WMSG_FAIL ){}
						else if( (m_ebGiftMessage.MouseMessage(pMsg)) != WMSG_FAIL ){}
						else if( (m_ebRecvGiftMsg[0].MouseMessage(pMsg)) != WMSG_FAIL ){}
						else if( (m_ebRecvGiftMsg[1].MouseMessage(pMsg)) != WMSG_FAIL ){}
						// Gift 관련 처리 :Su-won	<----------|

						else if( IsInsideRect( nX, nY, m_rcKitItems ) )
						{
							for( i = 0; i < KIT_SLOT_TOTAL; i++ )
							{
								if( m_abtnKitItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
								{
									// Nothing
									return WMSG_SUCCESS;
								}
							}
						}
						else if(IsInsideRect( nX, nY, m_rcInvenItems ))
						{
							for( i = 0; i < INVEN_SLOT_TOTAL; i++ )
							{
								if( m_abtnInvenItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
								{
									// Nothing
									return WMSG_SUCCESS;
								}
							}
						}
					}
					else if(m_nCurShopSection==UCSS_BUY_HISTORY){
						
						if( ( wmsgResult = m_cbYear.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
							}
								
						}
						else if( ( wmsgResult = m_cbMonth.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
							}
								
						}
						else if( ( wmsgResult = m_cbDay.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
							}
								
						}
						else if( ( wmsgResult = m_sbHistory.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
							}
								
						}
						else if( ( wmsgResult = m_btnSearch.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								if(m_abtnHistoryItems.Count()>0) m_abtnHistoryItems.Delete();
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASEHISTORY_REQ);
								return wmsgResult;
							}
								
						}
						
					}
				
					// Gift 관련 페이지 처리 :Su-won		|------------------------------>
					else if(m_nCurShopSection==UCSS_BUY_SENDHISTORY || m_nCurShopSection == UCSS_BUY_RECEIVEHISTORY)
					{
						
						if( ( wmsgResult = m_cbGiftYear.MouseMessage(pMsg)) != WMSG_FAIL ) {}
						else if( ( wmsgResult = m_cbGiftMonth.MouseMessage(pMsg)) != WMSG_FAIL ) {}
						else if( ( wmsgResult = m_cbGiftDay.MouseMessage(pMsg)) != WMSG_FAIL ) {}
						else if( ( wmsgResult = m_sbGift.MouseMessage(pMsg)) != WMSG_FAIL )	{}
						else if( ( m_btnInfoSendHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
						{
							if( m_nCurShopSection==UCSS_BUY_RECEIVEHISTORY )
							{
								m_nCurShopSection =UCSS_BUY_SENDHISTORY;

								m_cbGiftYear.SetCurSel(m_nSendYear);
								m_cbGiftMonth.SetCurSel(m_nSendMonth);
								m_cbGiftDay.SetCurSel(m_nSendDay);

								m_nRecvScrollPos = m_sbGift.GetScrollPos();
								m_sbGift.SetCurItemCount(m_abtnSendHistoryItems.sa_Count);
								m_sbGift.SetScrollPos(m_nSendScrollPos);
							}
						}
						else if( ( m_btnInfoReceiveHistory.MouseMessage(pMsg ) ) != WMSG_FAIL ) 
						{
							if( m_nCurShopSection==UCSS_BUY_SENDHISTORY )
							{
								m_nCurShopSection =UCSS_BUY_RECEIVEHISTORY;

								m_cbGiftYear.SetCurSel(m_nRecvYear);
								m_cbGiftMonth.SetCurSel(m_nRecvMonth);
								m_cbGiftDay.SetCurSel(m_nRecvDay);

								m_nSendScrollPos = m_sbGift.GetScrollPos();
								m_sbGift.SetCurItemCount(m_abtnRecvHistoryItems.sa_Count);
								m_sbGift.SetScrollPos(m_nRecvScrollPos);
							}
						}
						else if( ( wmsgResult = m_btnGiftSearch.MouseMessage(pMsg)) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								if(m_nCurShopSection==UCSS_BUY_SENDHISTORY)
								{
									if(m_abtnSendHistoryItems.Count()>0)
										m_abtnSendHistoryItems.Delete();
									_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ);
								
									return wmsgResult;
								}
								if(m_nCurShopSection==UCSS_BUY_RECEIVEHISTORY)
								{
									if(m_abtnRecvHistoryItems.Count()>0)
										m_abtnRecvHistoryItems.Delete();
									_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ);

									return wmsgResult;
								}
							}
						}
					}

					else if(m_nCurShopSection==UCSS_BUY_RECEIVE)
					{
						//'받은 선물'페이지에서 '가져오기' 버튼 클릭
						if( (m_btnInfoMove.MouseMessage(pMsg)) != WMSG_FAIL ) 
						{
							if (!m_bBringItem)
							{
								m_bBringItem = TRUE;
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECV_REQ);
							}
						}
						else if( ( wmsgResult = m_sbReceive.MouseMessage(pMsg)) != WMSG_FAIL )	{}
						else if( ( wmsgResult = m_ebRecvGiftMsg[0].MouseMessage(pMsg)) != WMSG_FAIL )	{}
						else if( ( wmsgResult = m_ebRecvGiftMsg[1].MouseMessage(pMsg)) != WMSG_FAIL )	{}						
						
						if(IsInsideRect( nX, nY, m_rcRecvItems ))
						{
							int tv_pos = m_sbReceive.GetScrollPos();
							int tv_max = 2>m_abtnRecvGift.sa_Count ? m_abtnRecvGift.sa_Count:2;
							
							for( int i =0 ; i < m_abtnRecvGift.sa_Count; i++)
							{
								if( m_abtnRecvGift[i].MouseMessage( pMsg ) != WMSG_FAIL )
								{
									// Nothing
									return WMSG_SUCCESS;
								}
							}
						}

						else if(IsInsideRect( nX, nY, m_rcInvenItems ))
						{
							for( i = 0; i < INVEN_SLOT_TOTAL; i++ )
							{
								if( m_abtnInvenItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
								{
									// Nothing
									return WMSG_SUCCESS;
								}
							}
						}
					}
					// Gift 관련 페이지 처리 :Su-won		<------------------------------|
				
				}
				
				return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and is from shop
					if( pUIManager->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						pUIManager->GetHoldBtn().GetWhichUI() == UI_CASH_SHOP )
					{
						
						if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH ){

							// connie [2009/10/23] - test
							int tv_idx = 0;
#if defined(G_JAPAN)							
							if(IsInsideRect(nX, nY, m_rcKitItemsJP))
							{
								tv_idx = 0;
								m_abtnTradeItems[0].Copy(pUIManager->GetHoldBtn());
								// Reset holding button
								pUIManager->ResetHoldBtn();
								return WMSG_SUCCESS;
							}
#endif
							// Shop items
							if( IsInsideRect( nX, nY, m_rcShopItems ) )
							{
								// If this item is moved from shop slot
								if( m_nShopItemID < 0 ||
									m_pbtnShopItems[m_nShopItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
								{
									if(m_nTradeItemID >=0 ){
										m_abtnTradeItems[m_nTradeItemID].InitBtn();
										m_nTradeItemID= -1;
									}
									pUIManager->ResetHoldBtn();
									return WMSG_SUCCESS;
								}
							}
							// Trade items
							else if( IsInsideRect( nX, nY, m_rcTradeItems ))
							{
								// If this item is moved from shop slot
								int n_x = nX;
								int n_y = nY;
								InsideNumXY(m_rcTradeItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
								
								if(m_nTradeItemID >=0 ){
									m_abtnTradeItems[m_nTradeItemID].InitBtn();
									m_nTradeItemID = -1;
									
								}
								
								int tv_idx = n_y+m_sbRight.GetScrollPos();
								m_abtnTradeItems[tv_idx].Copy(pUIManager->GetHoldBtn());
																
								// Reset holding button
								pUIManager->ResetHoldBtn();
								return WMSG_SUCCESS;
							} 
							else {
								if(m_nTradeItemID >=0 ){
									m_abtnTradeItems[m_nTradeItemID].InitBtn();
									m_nTradeItemID = -1;
									
								}
							}
		
						}
						else if(m_nCurShopTab == UCST_BUY_INFO)
						{
							if( m_nCurShopSection==UCSS_BUY_INFO && IsInsideRect( nX, nY, m_rcKitItems ) )
							{
								// If this item is moved from shop slot
								if( m_nKitItemID < 0 ||
									m_abtnKitItems[m_nKitItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
								{
									// If this item is moved from shop slot
									int n_x = nX;
									int n_y = nY;
									InsideNumXY(m_rcKitItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
									m_nKitItemID = n_x + n_y*LEFT_SLOT_SIZE_BUY_INFO;
									if( m_abtnKitItems[m_nKitItemID].IsEmpty())
									{						
										m_abtnKitItems[m_nKitItemID].Copy(pUIManager->GetHoldBtn());
										m_abtnInvenItems[m_nInvenItemID].InitBtn();
										// Reset holding buttonbutton
										pUIManager->ResetHoldBtn();
										return WMSG_SUCCESS;
									}
								}
							}
							
							// 인벤 슬롯에 있는 아이템을 받은 선물 슬롯으로 가져왔을 때 :Su-won
							else if( m_nCurShopSection==UCSS_BUY_RECEIVE 
										&& IsInsideRect( nX, nY, m_rcRecvItems )
										&& m_nInvenItemID >=0 )
							{
								for( int i=0 ; i<m_abtnRecvGift.sa_Count; ++i)
								{
									if(m_abtnRecvGift[i].IsEmpty())
									{
										m_abtnRecvGift[i].Copy(pUIManager->GetHoldBtn());
										m_abtnInvenItems[m_nInvenItemID].InitBtn();
	
										m_sbReceive.SetCurItemCount( m_sbReceive.GetCurItemCount() +1);
										
										m_anRecvOrder[i] =m_anInvenOrder[m_nInvenItemID];
										
										// Reset holding buttonbutton
										pUIManager->ResetHoldBtn();
										return WMSG_SUCCESS;
									}
								}
							}
							else if( IsInsideRect( nX, nY, m_rcInvenItems ) )
							{
								// If this item is moved from shop slot
								if( m_nInvenItemID < 0  || 
									m_abtnInvenItems[m_nInvenItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
								{
									// If this item is moved from shop slot
									int n_x = nX;
									int n_y = nY;
									InsideNumXY(m_rcInvenItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
									m_nInvenItemID = n_x + n_y*RIGHT_SLOT_SIZE_MOVE_COL;
									if( m_abtnInvenItems[m_nInvenItemID].IsEmpty() )
									{						
										m_abtnInvenItems[m_nInvenItemID].Copy(pUIManager->GetHoldBtn());

										if(m_nCurShopSection !=UCSS_BUY_RECEIVE)
											m_abtnKitItems[m_nKitItemID].InitBtn();
										else
										{
											m_abtnRecvGift[m_nRecvItemID].InitBtn();

											m_anInvenOrder[m_nInvenItemID] =m_anRecvOrder[m_nRecvItemID];
											
											m_sbReceive.SetCurItemCount( m_sbReceive.GetCurItemCount() -1);
											if( m_sbReceive.GetScrollPos() >m_sbReceive.GetCurItemCount()-2 )
												m_sbReceive.SetScrollPos( m_sbReceive.GetCurItemCount()-2 );
										}
										// Reset holding button
										pUIManager->ResetHoldBtn();
										return WMSG_SUCCESS;
									}
								}
							}
						} 
						// If - If holding button is item
						// Reset holding button
						pUIManager->ResetHoldBtn();
					
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH ){
				// Shop Items
				if( IsInsideRect( nX, nY, m_rcShopItems) )
				{
					// If this item is moved from shop slot
					int n_x = nX;
					int n_y = nY;
					InsideNumXY(m_rcShopItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
					
					//카테고리의 비어있는 슬롯을 클릭하면 뻑나거나
					//다른 카테고리의 아이템이 장바구니에 추가되는 버그때문에 추가		:Su-won
					if( n_y+m_sbLeft.GetScrollPos() >=m_shopItemCnt)
						return WMSG_SUCCESS;

					if( ( wmsgResult = m_pbtnShopItems[n_y+m_sbLeft.GetScrollPos()].MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{

							for( int i=0 ; i<MAX_KIT_SIZE ;i++){
							if(m_abtnTradeItems[i].IsEmpty()){
								m_abtnTradeItems[i].Copy(m_pbtnShopItems[n_y+m_sbLeft.GetScrollPos()]); 
								m_bShowShopInfo = FALSE;
								return WMSG_SUCCESS;
							}
							}

							CUIManager::getSingleton()->GetChatting()->AddSysMessage(_S(2398,"장바구니는 최대 10개까지 넣을 수 있습니다."));

												
						}
					}

					return WMSG_SUCCESS;
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcTradeItems ) )
				{
					// If this item is moved from shop slot
					int n_x = nX;
					int n_y = nY;
					InsideNumXY(m_rcTradeItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
					m_abtnTradeItems[n_y+m_sbRight.GetScrollPos()].InitBtn();
					m_bShowTradeInfo = FALSE;
					return WMSG_SUCCESS;

				} 
			} else if( m_nCurShopTab == UCST_BUY_INFO)
			{
				if(m_nCurShopSection !=UCSS_BUY_RECEIVE)
				{
				// Kit Items
				if( IsInsideRect( nX, nY, m_rcKitItems) )
				{
					// If this item is moved from shop slot
					int n_x = nX;
					int n_y = nY;
					InsideNumXY(m_rcKitItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
					int tv_sum = n_x+n_y*LEFT_SLOT_SIZE_BUY_INFO;

					for( int i=0 ; i<INVEN_SLOT_TOTAL ;i++)
					{
						if(m_abtnInvenItems[i].IsEmpty())
						{
						m_abtnInvenItems[i].Copy(m_abtnKitItems[tv_sum]);
						m_abtnKitItems[tv_sum].InitBtn();
						return WMSG_SUCCESS;
					}


				}
					CUIManager::getSingleton()->GetChatting()->AddSysMessage(_S(2399,"한번에 10개까지만 이동 가능 합니다."));
					return WMSG_SUCCESS;
				}
				// Inven items
				else if( IsInsideRect( nX, nY, m_rcInvenItems ) )
				{
					// If this item is moved from shop slot
					int n_x = nX;
					int n_y = nY;
					InsideNumXY(m_rcInvenItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
					int tv_sum = n_x+n_y*RIGHT_SLOT_SIZE_BASKET;
						for( int i=0 ; i<KIT_SLOT_TOTAL ;i++)
						{
							if(m_abtnKitItems[i].IsEmpty())
							{
							m_abtnKitItems[i].Copy(m_abtnInvenItems[tv_sum]);
							m_abtnInvenItems[tv_sum].InitBtn();
							return WMSG_SUCCESS;
						}

					}
					CUIManager::getSingleton()->GetChatting()->AddSysMessage(_S(2400,"빈 공간이 없습니다."));
					return WMSG_SUCCESS;
				} 
				}
				else if(m_nCurShopSection ==UCSS_BUY_RECEIVE)
				{
					// Kit Items
					if( IsInsideRect( nX, nY, m_rcRecvItems) )
					{
						//int nGift =m_sbReceive.GetScrollPos(); 
						int nCount =m_abtnRecvGift.sa_Count>2 ? 2: m_abtnRecvGift.sa_Count;
						
						for(int iRecv =0; iRecv <m_abtnRecvGift.sa_Count; ++iRecv)
						{
							if( !m_abtnRecvGift[iRecv].IsEmpty() &&
								m_abtnRecvGift[iRecv].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								for( int iInven=0 ; iInven<INVEN_SLOT_TOTAL ;iInven++)
								{
									if(m_abtnInvenItems[iInven].IsEmpty())
									{
										m_abtnInvenItems[iInven].Copy(m_abtnRecvGift[iRecv]);

										///////////////////////////////
										m_anInvenOrder[iInven] =m_anRecvOrder[iRecv];
										///////////////////////////////

										m_abtnRecvGift[iRecv].InitBtn();

										m_sbReceive.SetCurItemCount( m_sbReceive.GetCurItemCount() -1);
										if( m_sbReceive.GetScrollPos() >m_sbReceive.GetCurItemCount()-2 )
											m_sbReceive.SetScrollPos( m_sbReceive.GetCurItemCount()-2 );
										return WMSG_SUCCESS;
									}
								}
								CUIManager::getSingleton()->GetChatting()->AddSysMessage(_S(2399,"한번에 10개까지만 이동 가능 합니다."));
							}
						}
						return WMSG_SUCCESS;
					}
					// Inven items
					else if( IsInsideRect( nX, nY, m_rcInvenItems ) )
					{
						// If this item is moved from shop slot
						int n_x = nX;
						int n_y = nY;
						InsideNumXY(m_rcInvenItems,SLOT_SIZE ,SLOT_GAP,&n_x, &n_y);
						int tv_sum = n_x+n_y*RIGHT_SLOT_SIZE_BASKET;
						for( int i=0 ; i<m_abtnRecvGift.sa_Count; ++i){
							if(m_abtnRecvGift[i].IsEmpty())
							{
								if( m_abtnInvenItems[tv_sum].IsEmpty() )
									continue;
								m_abtnRecvGift[i].Copy(m_abtnInvenItems[tv_sum]);

								///////////////////////////////
								m_anRecvOrder[i] =m_anInvenOrder[tv_sum];
								///////////////////////////////

								m_abtnInvenItems[tv_sum].InitBtn();

								m_sbReceive.SetCurItemCount( m_sbReceive.GetCurItemCount() +1);
								return WMSG_SUCCESS;
							}
						}
						return WMSG_SUCCESS;
					} 
				}
			}
			
		}
		break;

	case WM_MOUSEWHEEL:
		{
		
			if( IsInside( nX, nY) )
			{
				if( m_nCurShopTab == UCST_BUY || m_nCurShopTab == UCST_NEW_CASH )
				{
					// Top Scroll bar
					if( m_sbLeft.MouseMessage( pMsg ) != WMSG_FAIL )
					{	

//						m_abtnShowItem.Copy( m_pbtnShopItems[m_iShowItemIdx] );
						return WMSG_SUCCESS;
					}
					// Bottom Scroll bar
					else if( m_sbRight.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
				}
				else if( m_nCurShopTab == UCST_BUY_INFO )
				{
					if( m_nCurShopSection ==UCSS_BUY_HISTORY )
					{
						if(m_cbYear.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
						if(m_cbMonth.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
						if(m_cbDay.MouseMessage(pMsg) != WMSG_FAIL )

						if(m_sbHistory.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;

						return WMSG_SUCCESS;
					}

					// Gift 관련 페이지 휠 기능 추가 :Su-won	|---------->
					if( m_nCurShopSection ==UCSS_BUY_SENDHISTORY || m_nCurShopSection ==UCSS_BUY_RECEIVEHISTORY )
					{
						if(m_cbGiftYear.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
						if(m_cbGiftMonth.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
						if(m_cbGiftDay.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;

						if(m_sbGift.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					if( m_nCurShopSection ==UCSS_BUY_RECEIVE )
					{
						if(m_sbReceive.MouseMessage(pMsg) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					// Gift 관련 페이지 휠 기능 추가 :Su-won	<----------|
				}
			}
		}
		break;
	}

	return WMSG_SUCCESS;
}
void CUICashShop::ClearBtnItems(CUIButtonEx* btn,int size)
{
	for( int i=0 ; i<size ;i++)
		btn[i].InitBtn();
	
}
void CUICashShop::PrepareShopItems( )
{
	int i,tv_num; // temp var
			
	// Enable된 아이템 개수
	CCashShopData& CD = _pNetwork->GetCashShopData(m_nCurShopSection);
	CCashShopData::CASH_SHOP_DATA tv_shop;
	m_shopItemCnt=CD.m_vShopItemArray.size();
	
	for( i=0; i<CD.m_vShopItemArray.size(); i++){
		if(!CD.m_vShopItemArray[i].m_enable) m_shopItemCnt--;
	}

	// Set scroll size 
	m_sbLeft.SetCurItemCount(m_shopItemCnt);
	m_sbLeft.SetScrollPos(0);

	// Reset items
	if( m_shopItemCnt <= 0 ){
		m_shopItemCnt = 0 ;
		return;
	}

	// 기존 8개를 계속해서 생성하던 방식에서 
	// 최대개수 만큼 생성하고 해당 샵이 변경 될 때만 호출, 8개까지만 화면에 출력하는 형식으로 바꿈
	if (m_pbtnShopItems !=NULL) delete[] m_pbtnShopItems;
	m_pbtnShopItems = new CUIButtonEx[m_shopItemCnt];
	
#if defined(G_THAI) || defined(G_JAPAN)
	{
		for( i = 0,tv_num =CD.m_vShopItemArray.size()-1; i < m_shopItemCnt; --tv_num)		//캐쉬 아이템 리스트 꺼꾸로 정렬
		{
			tv_shop = CD.m_vShopItemArray[tv_num];
			if(CD.m_vShopItemArray[tv_num].m_enable){
				m_pbtnShopItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM);
				m_pbtnShopItems[i].SetBaseInfo(UBET_ITEM, tv_shop.m_texID, tv_shop.m_texRow, tv_shop.m_texCol);
				m_pbtnShopItems[i].SetItemInfo( -1, -1, -1, -1, -1,
					tv_shop.m_shopItemIndex, m_nCurShopSection, tv_shop.m_itemName,tv_shop.m_itemDesc,tv_shop.m_itemListCnt,tv_shop.m_typeIndex);
				i++;
			}
		}
	}
#else
	{
		for( i = 0,tv_num =0; i < m_shopItemCnt; tv_num++)
		{
			tv_shop = CD.m_vShopItemArray[tv_num];
			if(CD.m_vShopItemArray[tv_num].m_enable){
				m_pbtnShopItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM);
				m_pbtnShopItems[i].SetBaseInfo(UBET_ITEM, tv_shop.m_texID, tv_shop.m_texRow, tv_shop.m_texCol);
				m_pbtnShopItems[i].SetItemInfo( -1, -1, -1, -1, -1,
					tv_shop.m_shopItemIndex, m_nCurShopSection, tv_shop.m_itemName,tv_shop.m_itemDesc,tv_shop.m_itemListCnt,tv_shop.m_typeIndex);
				i++;
			}
		}
	}
#endif
}


// ---wooss 060503 ------------------------------------------------------------>>
// 
// Name : CUIConfirmInfo()
//
// Desc : Constructor
//
// ----------------------------------------------------------------------------
CUIConfirmInfo::CUIConfirmInfo()
{
	SetEnable(FALSE);
	SetVisible(FALSE);
}

CUIConfirmInfo::~CUIConfirmInfo()
{
	Destroy();
}

void CUIConfirmInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 300, 22 );
	
	// Create texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );	
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 0, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 0, 89, 52, fTexWidth, fTexHeight );
	
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );


	// Close Button
	m_btnClose.Create( this, CTString(""), 267, 6, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK Button
	m_btnOK.Create( this, _S( 2380, "결제" ), 154, 361, BTN_WIDTH_2 , 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK Button
	m_btnCancel.Create( this, _S(139, "취소" ), 219, 361, BTN_WIDTH_2 , 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// line 
	m_rtLineH.SetUV(228, 0, 230, 0, fTexWidth,fTexHeight);
	m_rtLineV.SetUV(228, 0, 228, 2, fTexWidth,fTexHeight);

	// Set Item Button
//	for(  int i = 0; i < MAX_KIT_SIZE ; i++ )
//		m_btnConfirmInfo[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM );	


	// ITEM SLOT
	m_rtItemSlot.SetUV(193,0,227,34,fTexWidth,fTexHeight);
	// Black Box
	m_rtBlackBox.SetUV(13,51,76,60,fTexWidth,fTexHeight);
	// Black Box with Line
	m_rtBlackboxL.SetUV(11,49,78,62,fTexWidth,fTexHeight);
	

}


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIConfirmInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIConfirmInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIConfirmInfo::Render()
{
	int nX = m_nPosX;
	int nY = m_nPosY;
	int nX2 = m_nPosX + m_nWidth;
	int nY2 = m_nPosY + 60;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );


	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 40;
	pDrawPort->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// Render Buttons
	m_btnClose.Render();
	m_btnOK.Render();
	m_btnCancel.Render();



	// Text in cash shop
	// Title
	pDrawPort->PutTextEx( _S(2634,"구매 내역 확인창"), m_nPosX + CASHSHOP_TITLE_OFFSETX,
										m_nPosY + CASHSHOP_TITLE_OFFSETY, 0xFFFFFFFF );

	// horizon line
	pDrawPort->AddTexture( m_nPosX+12, m_nPosY+80, m_nPosX+288, m_nPosY+81,
										m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+12, m_nPosY+260, m_nPosX+288, m_nPosY+261,
										m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+12, m_nPosY+290, m_nPosX+288, m_nPosY+291,
										m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,0xFFFFFFFF );
	// vertical line
	pDrawPort->AddTexture( m_nPosX+205, m_nPosY+57, m_nPosX+206, m_nPosY+260,
										m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,0xFFFFFFFF );

	// 구매 내역 

	pDrawPort->PutTextEx( _S(2392,"구입 물품"), 
												m_nPosX + 99,
												m_nPosY + 64, 
												0x5566FFFF );
	pDrawPort->PutTextEx( _S(2397, "가격"), 
												m_nPosX + 231,
												m_nPosY + 64, 
												0x5566FFFF );

	int tv_sum = 0;
	int tv_index;
	int tv_type;	
	CTString tv_str ;
	
	for( int i =0; i<MAX_KIT_SIZE ; i++ ){
		if(!pUIManager->GetCashShop()->m_abtnTradeItems[i].IsEmpty()) {
			pDrawPort->PutTextEx( pUIManager->GetCashShop()->m_abtnTradeItems[i].GetCashName(), 
												m_nPosX + 19,
												m_nPosY + 100 +15*i, 
												0xEEFFFFFF );
			tv_index=pUIManager->GetCashShop()->m_abtnTradeItems[i].GetCashTypeIndex();
			tv_type = pUIManager->GetCashShop()->m_abtnTradeItems[i].GetCashType();

			CCashShopData& CD = _pNetwork->GetCashShopData( tv_type );

			tv_str.PrintF("%d",CD.m_vShopItemArray[tv_index].m_cash);
			pDrawPort->PutTextEx(  tv_str, 
												m_nPosX + 231 ,
												m_nPosY + 100 +15*i, 
												0xFFEEFFFF );

			tv_sum+=CD.m_vShopItemArray[tv_index].m_cash;
		
		}
	}	
	
	tv_str.PrintF(_S(2635,"결제 금액 : %d"),tv_sum);
	pDrawPort->PutTextEx(  tv_str, 
												m_nPosX + 100 ,
												m_nPosY + 268 , 
												0x5566FFFF );

	pDrawPort->PutTextEx(  _S(2636,"구매한 물품은 구매 정보에서"), 
										m_nPosX + 20 ,
										m_nPosY + 310 , 
										0xFF44aaFF );
	pDrawPort->PutTextEx(  _S(2637,"캐릭의 인벤창으로 옮기실 수 있습니다."), 
										m_nPosX + 20 ,
										m_nPosY + 325 , 
										0xFF44aaFF );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();


}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIConfirmInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bConfirmTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) ){
				SetFocus(TRUE);
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}
			else SetFocus(FALSE);

			// Move board
			if( bConfirmTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
			
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bConfirmTitleBarClick = TRUE;
				} 
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Close button
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
						
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bConfirmTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close , OK button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL || m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						SetEnable(FALSE);
						SetVisible(FALSE);
						return WMSG_SUCCESS;
				}
				else if( (wmsgResult=m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL ){
					// TODO : 결제
					if(wmsgResult == WMSG_COMMAND){
						for( int i =0; i<MAX_KIT_SIZE ; i++){
							if(!pUIManager->GetCashShop()->m_abtnTradeItems[i].IsEmpty()) {
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASE_REQ);
								break;
							}
						}
					}

					// Close Window after send message...
					SetEnable(FALSE);
					SetVisible(FALSE);
					// PAY Action
					return wmsgResult;
				}
				 
				
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	}

	return WMSG_SUCCESS;

}

// ----------------------------------------------------------------------------
// Name : CUICashShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISetInfo::CUISetInfo()
{
	SetEnable(FALSE);
	SetVisible(FALSE);
	m_nGoodsNum = 0 ;
}

CUISetInfo::~CUISetInfo()
{
	Destroy();
}

void CUISetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight ,int nGoodsNum)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nGoodsNum = nGoodsNum;	

	// Region of each part
	m_rcSetTitle.SetRect( 0, 0, 300, 22 );
	
	// Create texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );	
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 0, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 0, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	//  Left Scroll Bar
	m_sbSet.Create( this, 278,103, 9, 192 );
	m_sbSet.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbSet.SetScrollPos( 0 );
	m_sbSet.SetScrollRange( m_nGoodsNum );
	m_sbSet.SetCurItemCount( m_nGoodsNum );
	m_sbSet.SetItemsPerPage( SET_ITEM_INFO );
	// Up button
	m_sbSet.SetUpUV( UBS_IDLE, 46,117,54,123, fTexWidth, fTexHeight );
	m_sbSet.SetUpUV( UBS_CLICK, 56,117,64,123,  fTexWidth, fTexHeight );
	m_sbSet.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSet.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbSet.SetDownUV( UBS_IDLE, 66, 117, 74, 123, fTexWidth, fTexHeight );
	m_sbSet.SetDownUV( UBS_CLICK, 76, 117, 84, 123, fTexWidth, fTexHeight );
	m_sbSet.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSet.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbSet.SetBarTopUV( 90, 43, 98, 47, fTexWidth, fTexHeight );
	m_sbSet.SetBarMiddleUV( 90, 48, 98, 62, fTexWidth, fTexHeight );
	m_sbSet.SetBarBottomUV( 90, 63, 98, 66, fTexWidth, fTexHeight );
	// Wheel region
	m_sbSet.SetWheelRect( -184, -1, 183, 176 );

	// Close Button
	m_btnSetClose.Create( this, CTString( "" ), 267, 6, 14, 14 );
	m_btnSetClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnSetClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnSetClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSetClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK Button
	m_btnSetOK.Create( this, _S( 191,"확인" ), 207, 300, BTN_WIDTH_2 , 21 );
	m_btnSetOK.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnSetOK.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnSetOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSetOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// line 
	m_rtLineH.SetUV(228, 0, 230, 0, fTexWidth,fTexHeight);
	m_rtLineV.SetUV(228, 0, 228, 2, fTexWidth,fTexHeight);

	// Set Item Button
	m_btnSetInfo = new CUIButtonEx[m_nGoodsNum];
	for(  int i = 0; i < m_nGoodsNum; i++ )
		m_btnSetInfo[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM );	


	// ITEM SLOT
	m_rtItemSlot.SetUV(193,0,227,34,fTexWidth,fTexHeight);
	// Black Box
	m_rtBlackBox.SetUV(13,51,76,60,fTexWidth,fTexHeight);
	// Black Box with Line
	m_rtBlackboxL.SetUV(11,49,78,62,fTexWidth,fTexHeight);
	
	
}


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUISetInfo::ClearBtnItems(CUIButtonEx* btn,int size)
{
	for( int i=0 ; i<size ;i++)
		btn[i].InitBtn();
	
}

void CUISetInfo::SetItemInfo(CUIButtonEx& btnSrc)
{
	m_btnSetItem=btnSrc;
	m_btnSetItem.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CASH_SHOP, UBET_ITEM);
	m_btnSetItem.Copy(btnSrc);
	
	if(!m_btnSetItem.IsEmpty()){
		CCashShopData& CD = _pNetwork->GetCashShopData(UCSS_SET);
		std::vector<CCashShopData::ITEM_DATA>::iterator itr_begin = CD.m_vShopItemArray[m_btnSetItem.GetCashTypeIndex()].m_vItemDataArray.begin();
		std::vector<CCashShopData::ITEM_DATA>::iterator itr_end = CD.m_vShopItemArray[m_btnSetItem.GetCashTypeIndex()].m_vItemDataArray.end();
		std::vector<CCashShopData::ITEM_DATA>::iterator itr_go = itr_begin; 
		int nListCnt = m_btnSetItem.GetCashListCount();
	
		ClearBtnItems(m_btnSetInfo,m_nGoodsNum);
		for(int i=0 ; itr_go!= itr_end || i < nListCnt ; itr_go++ ,i++ ){
			m_btnSetInfo[i].SetItemInfo(-1, -1, itr_go->m_itemIndex, -1, -1);
			m_btnSetInfo[i].SetItemCount(itr_go->m_itemCnt );
		}		

	} else ClearBtnItems(m_btnSetInfo,m_nGoodsNum);

}

void CUISetInfo::RenderSetItem()
{
	if( m_btnSetItem.IsEmpty() == TRUE )
		return;

	int nX = SET_TOP_SLOT_SX;
	int nY = SET_TOP_SLOT_SY;
	int iShopX = SET_TOP_ITEM_SX;
	int iShopY = SET_TOP_ITEM_SY;
	int tv_pos = m_sbSet.GetScrollPos();

	m_btnSetItem.SetPos( iShopX, iShopY );
	m_btnSetItem.Render();	

	int tv_x,tv_y;
	m_btnSetItem.GetAbsPos(tv_x,tv_y);
	tv_x += SLOT_GAP+SLOT_SIZE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->PutTextEx(m_btnSetItem.GetCashName(), tv_x,tv_y+SLOT_SIZE/4);
				
	CCashShopData& CS = _pNetwork->GetCashShopData(UCSS_SET);
	int tv_num = (m_nGoodsNum > (SET_ITEM_INFO+tv_pos))?(SET_ITEM_INFO+tv_pos):m_nGoodsNum;
	if( tv_num >= tv_pos )
	{
		for( int i = tv_pos ; i < tv_num; i++ )
		{
			if( m_btnSetInfo[i].IsEmpty() ){
				nY += SLOT_SIZE+SLOT_GAP;
				continue;
			}
			m_btnSetInfo[i].SetPos( nX, nY );
			m_btnSetInfo[i].Render();
			m_btnSetInfo[i].GetAbsPos(tv_x,tv_y);
			tv_x  += SLOT_GAP+SLOT_SIZE;
			nY += SLOT_SIZE+SLOT_GAP;
			CItemData* pCD = _pNetwork->GetItemData(m_btnSetInfo[i].GetIndex());
			pDrawPort->PutTextEx(pCD->GetName(), tv_x,tv_y+SLOT_SIZE/4);
			CTString tv_str;
			tv_str.PrintF("(%d)",m_btnSetInfo[i].GetItemCount());
			pDrawPort->PutTextExRX( tv_str , tv_x + 220,tv_y+SLOT_SIZE/4);			
		}
	}
}

void CUISetInfo::Render()
{
	int nX = m_nPosX;
	int nY = m_nPosY;
	int nX2 = m_nPosX + m_nWidth;
	int nY2 = m_nPosY + 60;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );


	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 40;
	pDrawPort->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// Set Item Close button
	m_btnSetClose.Render();
	
	// OK button
	m_btnSetOK.Render();

	// Text in cash shop
	// Title
	pDrawPort->PutTextEx( _S(2401,"세트상품 정보"), m_nPosX + CASHSHOP_TITLE_OFFSETX,
										m_nPosY + CASHSHOP_TITLE_OFFSETY, 0xFFFFFFFF );

	//item slot box
	//left slot
	nY2=SLOT_SIZE+SLOT_GAP;
	
	// set item slot
	pDrawPort->AddTexture( m_nPosX+40, m_nPosY+64, m_nPosX+40+SLOT_SIZE, m_nPosY+64+SLOT_SIZE,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
										0xFFFFFFFF );
	
	// items
	for(int i=0;i<SET_ITEM_INFO;i++)
		pDrawPort->AddTexture( m_nPosX+17, m_nPosY+105+nY2*i, m_nPosX+17+SLOT_SIZE, m_nPosY+105+SLOT_SIZE+nY2*i,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
										0xFFFFFFFF );
	// horizon line
	pDrawPort->AddTexture( m_nPosX+12, m_nPosY+100, m_nPosX+287, m_nPosY+102,
										m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,0xFFFFFFFF );

	// Scrollbar 
	m_sbSet.Render();
	

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render Set items
	RenderSetItem();

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISetInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bSetTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) ){
				SetFocus(TRUE);
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}
			else SetFocus(FALSE);

			// Move board
			if( bSetTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnSetClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_sbSet.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
			
				nOldX = nX;		nOldY = nY;

				
				// Title bar
				if( IsInsideRect( nX, nY, m_rcSetTitle ) )
				{
					bSetTitleBarClick = TRUE;
				} 
				// OK button
				else if( m_btnSetOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Close button
				else if( m_btnSetClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_sbSet.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
		
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bSetTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close , OK button
				if( ( wmsgResult = m_btnSetClose.MouseMessage( pMsg ) ) != WMSG_FAIL  
					|| m_btnSetOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						SetEnable(FALSE);
						SetVisible(FALSE);
						m_btnSetItem.InitBtn();
						ClearBtnItems(m_btnSetInfo,m_nGoodsNum);
				}
				return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY) )
			{
				// Set Scroll bar
				if( m_sbSet.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : InsideNumXY()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShop::InsideNumXY(UIRect rc,int btnSize,int btnGap, int* n_x, int* n_y )
{
	int tv_i,tv_divY,tv_divX,tv_sum;
	if( IsInsideRect(*n_x,*n_y,rc) ){
		
		ConvertToWindow(*n_x,*n_y);
		tv_divY=rc.GetHeight()/btnSize;
		tv_divX=rc.GetWidth()/btnSize;

		for(tv_i=1; tv_i <= tv_divY ;tv_i++){
			tv_sum=btnSize*tv_i+rc.Top+btnGap*(tv_i-1);
			if( tv_sum > *n_y) {
				*n_y=tv_i-1;
				break;
			}
		}
		for( tv_i=1; tv_i <= tv_divX ; tv_i++){
			tv_sum=btnSize*tv_i+rc.Left+btnGap*(tv_i-1);
			if( tv_sum > *n_x) {
				*n_x=tv_i-1;
				break;
			}
			
		}
		
	}
}

//-----------------------------------------------------------------------------
// Name	  : CaculateKitPrice()
// Input  : void
// Output : int
//-----------------------------------------------------------------------------
int CUICashShop::CalculateKitPrice()
{
	int	iTotalPrice	= 0;
	
	for( int i = 0; i < MAX_KIT_SIZE; i++ )
	{
		if( m_abtnTradeItems[i].IsEmpty() )
			continue;
		
		CCashShopData& CD		= _pNetwork->GetCashShopData(m_abtnTradeItems[i].GetCashType());
		iTotalPrice				+= CD.m_vShopItemArray[m_abtnTradeItems[i].GetCashTypeIndex()].m_cash;
	}
	return iTotalPrice;
}


//-----------------------------------------------------------------------------
// Name	  : CaculateKitPrice()
// Input  : void
// Output : int
//-----------------------------------------------------------------------------
BOOL CUICashShop::SetCashIndexToBtn(int nCashIndex,int nUniIndex,CUIButtonEx& btn )
{
	for(int nType=0;nType<MAX_CASHSHOP_CLASS;nType++)
	{
		CCashShopData& CD = _pNetwork->GetCashShopData(nType);
		int tv_itemNum = CD.m_vShopItemArray.size();
		for( int i=0 ; i<tv_itemNum ;i++){
			CCashShopData::CASH_SHOP_DATA tv_shop = CD.m_vShopItemArray[i];
			if(tv_shop.m_shopItemIndex == nCashIndex){
				btn.Create(this,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP, UBET_ITEM);
				btn.SetBaseInfo(UBET_ITEM, tv_shop.m_texID, tv_shop.m_texRow,tv_shop.m_texCol);
				btn.SetItemInfo( -1, -1, -1 ,nUniIndex,-1,
					tv_shop.m_shopItemIndex, tv_shop.m_type, tv_shop.m_itemName,tv_shop.m_itemDesc,tv_shop.m_itemListCnt,tv_shop.m_typeIndex);
				return TRUE;
			}
		}
	}
	return FALSE;

}


// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShop::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo ,int maxLine, int maxChars)
{
	if( m_nCurInfoLines >= maxLine )
		return ;

	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
#if defined(G_THAI)
	{
		// Get length of string
		int		iPos;
		INDEX	nThaiLen = FindThaiLen(strItemInfo);
		INDEX	nChatMax= (maxChars-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
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
			int		nSplitPos = maxChars;
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

			AddItemInfoString( strTemp, colItemInfo ,MAX_CASH_ITEMINFO_LINE,maxChars);

		}
		
	} 
#else 
	{
		// If length of string is less than max char
		if( nLength <= maxChars )
		{
			m_strItemInfo[m_nCurInfoLines] = strItemInfo;
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = maxChars;
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

			AddItemInfoString( strTemp, colItemInfo ,maxLine,maxChars);
		}
	}
#endif
}

void CUICashShop::InitShowInfo()
{
	m_bShowShopInfo = FALSE;
	m_bShowTradeInfo = FALSE;
	m_bShowInvenInfo = FALSE;
	m_bShowKitInfo = FALSE;
	m_bShowHistoryInfo = FALSE;
}

void CUICashShop::ButtonMoveMsg( MSG *pMsg )
{
	if( ( m_btnConsume.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnTimeLimit.MouseMessage(pMsg ) ) != WMSG_FAIL &&	m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnNewItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
	else if( ( m_btnHotItem.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
	else if( ( m_btnPlatinum.MouseMessage( pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_NEW_CASH ) {}
	else if( ( m_btnPotion.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnAvata.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnWeapon.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnDefend.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnSet.MouseMessage(pMsg ) ) != WMSG_FAIL && m_nCurShopTab == UCST_BUY ) {}
	else if( ( m_btnClear.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
	else if( ( m_btnPay.MouseMessage(pMsg ) ) != WMSG_FAIL ) {}
	else if( ( m_btnInfoBuyItem.MouseMessage(pMsg))!= WMSG_FAIL) {}
	else if( ( m_btnInfoBuyHistory.MouseMessage(pMsg))!= WMSG_FAIL) {}
}

// 선물 보내기창 렌더링 :Su-won
void CUICashShop::RenderSendGiftUI()										
{
	int nX = m_nPosX;
	int nY = m_nPosY;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	//horizon line
	pDrawPort->AddTexture( nX+349+50, nY+252, nX+567+50, nY+253,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

	pDrawPort->AddTexture( nX+349+50, nY+252 +20, nX+567+50, nY+253 +20,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

	pDrawPort->AddTexture( nX+349+50, nY+252 +150, nX+567+50, nY+253 +150,
												m_rtLineH.U0, m_rtLineH.V0, m_rtLineH.U1, m_rtLineH.V1,
												0xFFFFFFFF );

	//vertical line
	pDrawPort->AddTexture( nX+348+50, nY+252, nX+349+50, nY+253 +150,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

	pDrawPort->AddTexture( nX+567+50, nY+252, nX+568+50, nY+253 +150,
												m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
												0xFFFFFFFF );

	pDrawPort->PutTextExCX( _S(3104, "선물 받을 친구"), nX+456+50, nY+252+3, 0xFFFFFFFF );
	pDrawPort->PutTextEx( _S(3105, "캐릭터 명"), nX+348+50+10, nY+252+33, 0xFFFFFFFF );
	pDrawPort->PutTextEx( _S(3101, "선물 메시지"), nX+348+50+10, nY+252+51, 0xFFFFFFFF );

	pDrawPort->AddTexture( nX+m_ebChar.GetPosX() ,nY+m_ebChar.GetPosY(), nX +m_ebChar.GetPosX() +m_ebChar.GetWidth() , nY+m_ebChar.GetPosY() +m_ebChar.GetHeight(),
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );
	
	pDrawPort->AddTexture( nX+m_ebGiftMessage.GetPosX() ,nY+m_ebGiftMessage.GetPosY(), nX +m_ebGiftMessage.GetPosX() +m_ebGiftMessage.GetWidth() +3, nY+m_ebGiftMessage.GetPosY() +45 -3,
												m_rtLineH.U0,m_rtLineH.V0,m_rtLineH.U1,m_rtLineH.V1,
												0xFFFFFFFF );

	m_ebChar.Render();
	pDrawPort->FlushRenderingQueue();
	
	m_ebGiftMessage.Render();
	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	//			TEST			|->// 그냥 넣을까???
	CTString temp;

#if defined(G_RUSSIA)
		temp.PrintF("(%d/100 蓀煜)", m_ebGiftMessage.GetAllStringLength());
#else
		temp.PrintF("(%d/100 Byte)", m_ebGiftMessage.GetAllStringLength());
#endif

	pDrawPort->PutTextEx( temp, nX+348+50+10 +90, nY+252+55, 0xFFFFFFFF );
	//			TEST			<-|//
	
	m_btnSend.Render();

	// Reading window
	if( m_ebChar.DoesShowReadingWindow() )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebChar.RenderReadingWindow();

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// 선물 관련 입력 때문에 추가 :Su-won
WMSG_RESULT	CUICashShop::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	
	WMSG_RESULT	wmsgResult;

	if( pMsg->wParam == VK_TAB )
	{
		if( m_ebChar.IsFocused() )
		{
			m_ebChar.SetFocus( FALSE );
			m_ebGiftMessage.SetFocus( TRUE );
		}
		else if( m_ebGiftMessage.IsFocused() )
		{
			m_ebGiftMessage.SetFocus( FALSE );
			m_ebChar.SetFocus( TRUE );
		}

		return WMSG_SUCCESS;
	}

	if( m_ebChar.IsFocused() )
	{
		if( ( wmsgResult = m_ebChar.KeyMessage( pMsg ) ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}

	if( m_ebGiftMessage.IsFocused() )
	{
		if( pMsg->wParam != VK_RETURN ||
			m_ebGiftMessage.GetAllStringLength()+2 <=MAX_GIFT_MESSAGE)
		{
			if( ( wmsgResult = m_ebGiftMessage.KeyMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS; 
		}
		else
			return WMSG_SUCCESS; 
	}
		
	return WMSG_FAIL;
}

// 선물 관련 입력 때문에 추가 :Su-won
WMSG_RESULT	CUICashShop::CharMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	

	if( m_ebChar.IsFocused() )
	{
		if( m_ebChar.CharMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}

	if( m_ebGiftMessage.IsFocused() )
	{
		if( m_ebGiftMessage.GetAllStringLength()+1 <=MAX_GIFT_MESSAGE )
		{
			if( m_ebGiftMessage.CharMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS; 
		}
	}
	
	return WMSG_FAIL;
}

// 선물 관련 입력 때문에 추가 :Su-won
WMSG_RESULT	CUICashShop::IMEMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;

	if( m_ebChar.IsFocused() )
		return m_ebChar.IMEMessage( pMsg );
	if( m_ebGiftMessage.IsFocused() )
	{
		if( m_ebGiftMessage.GetAllStringLength()+2 <= MAX_GIFT_MESSAGE )
			return m_ebGiftMessage.IMEMessage( pMsg );
	}

	return WMSG_FAIL;
}

BOOL CUICashShop::IsEditBoxFocused()
{
	if( m_ebChar.IsFocused() || m_ebGiftMessage.IsFocused() )
		return TRUE;
	else
		return FALSE;
}

void CUICashShop::OpenRecvGiftPage()
{
	OpenCashShop();

	_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_BALANCE_REQ);

	m_nOldShopTab = UCST_BUY_INFO;
	m_nCurShopTab = UCST_BUY_INFO;
	m_nCurShopSection=UCSS_BUY_RECEIVE;
	
	ClearBtnItems(m_abtnRecvGift.sa_Array, m_abtnRecvGift.sa_Count);
	ClearBtnItems(m_abtnInvenItems.sa_Array,INVEN_SLOT_TOTAL);

	_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECVLIST_REQ);
}

void CUICashShop::Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, dwStyle, UI_CASH_SHOP, nCommandCode ); 
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUICashShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode)
	{
		case MSGCMD_GIFT_SEND:
		{
			if( bOK)
			{
				m_bShowSendGift =false;
				_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_REQ);				
			}
			break;
		}
	}
}

BOOL CUICashShop::IsEmptyRecvGift()
{
	for( int i=0; i<m_abtnRecvGift.sa_Count; ++i )
	{
		if ( !m_abtnRecvGift[i].IsEmpty() )
			return FALSE;
	}
	
	CUIManager::getSingleton()->GetQuickSlot()->SetGiftRecv(FALSE);
	return TRUE;
}

void CUICashShop::OpenBillingHomePage()
{
	ShellExecute(NULL, "open", "https://billing.gamigogames.de", NULL, NULL, SW_SHOWNORMAL);
}
