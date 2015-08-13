// ----------------------------------------------------------------------------
//  File : UIQuickSlot.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIQUICKSLOT_H_
#define	UIQUICKSLOT_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define quick slot buttons
#define	QSLOT_PAGE_COUNT				3

// [12/10/2012 박훈] UI개편으로 인한 x,y  변경
#define	QSLOT_BTN_COUNT					12
#define	QSLOT_BTN_SX					20
#define	QSLOT_BTN_SY					7

// [12/10/2012 박훈] UI개편으로 인한 x,y  변경
// Define outline of buttons
#define	QSLOT_BTN_OUTLINE_SX			167
#define	QSLOT_BTN_OUTLINE_SY			40
#define	QSLOT_BTN_OUTLINE_SIZE			34

// Define position of buttons
#define	QSLOT_PREV_BTN_SX				440
#define	QSLOT_PREV_BTN_SY				5
#define	QSLOT_NEXT_BTN_SX				440 
#define	QSLOT_NEXT_BTN_SY				31

// [12/10/2012 박훈] Define position of Add, Delete Button
#define QSLOT_ADD_BTN_SX				2
#define QSLOT_ADD_BTN_SY				6
#define QSLOT_DELETE_BTN_SX				2
#define QSLOT_DELETE_BTN_SY				26

#define	QSLOT_ROT_BTN_SX				460
#define	QSLOT_ROT_BTN_SY				15

// Define position of text
#define	QSLOT_TAB_TEXT_CX				22
#define	QSLOT_TAB_TEXT_SY				16

//  [12/10/2012 박훈] Define size of quick slot (개편UI)
#define	QUICKSLOT_WIDTH					457 //wooss
//#define	QUICKSLOT_HEIGHT			44	
#define	QUICKSLOT_HEIGHT				44

#define	QSLOT_GIFT_BTN_X				39
#define	QSLOT_GIFT_BTN_Y				31
#define	QSLOT_GIFT_BTN_ARROW			9

//  [1/2/2013 Ranma]
#define QSLOT_TAIL_SIZE					20
#define QSLOT_RCSLOT_AREA				((QUICKSLOT_WIDTH) - (QSLOT_TAIL_SIZE))

//Su-won
enum eGiftPos
{
	GIFTPOS_TOP,
	GIFTPOS_BOTTOM,
	GIFTPOS_RIGHT,
	GIFTPOS_LEFT,
};

// 퀵슬롯 확장 버튼 [12/18/2012 Ranma]
enum eQSlotEXBtn
{
	QSLOT_ADD,
	QSLOT_DEL,
	QSLOT_END,
};

enum eQUICKSLOT_TYPE
{
	QUICKSLOT_TYPE_EMPTY = -1,		// 슬롯 비었음
	QUICKSLOT_TYPE_SKILL = 0,		// 스킬타입
	QUICKSLOT_TYPE_ACTION,			// 액션타입
	QUICKSLOT_TYPE_ITEM,			// 아이템타입
	QUICKSLOT_TYPE_ITEM_WEAR,		// 장착 아이템 타입
};

typedef CUIIcon*	abtnItems[QSLOT_PAGE_COUNT][QSLOT_BTN_COUNT];	// Buttons of quick slot

extern INDEX g_bQuickSlotEX1;
extern INDEX g_bQuickSlotEX2;

// ----------------------------------------------------------------------------
// Name : CUIQuickSlot
// Desc :
// ----------------------------------------------------------------------------
class CUIQuickSlot : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnPrevSlot;									// Button for prev slot
	CUIButton			m_btnNextSlot;									// Button for next slot
	// [12/10/2012 박훈] 개편UI 
	enum QSLOT { QS_1 = 0, QS_2, QS_END };
	CUIWindow*			m_pChildQuickSlot[QS_END];
	// Add, Delete 버튼 추가
	CUIButton			m_btnAddSlot;									// Button for Add Slot
	CUIButton			m_btnDeleteSlot;								// Button for Delete Slot

	CUIButton			m_btnRotate;									// Button for rotating this UI
	CUIButton			m_btnQuestBook;									// Button for Quest Book
	//wooss
	CUIButton			m_btnCashShop;									// Button for Cash Shop
	//Su-won
	CUIButton			m_btnGiftNotice;								//선물 도착 알림 버튼
	
	BOOL				m_bHorizontal;									// If quick slot is horizontal or vertical
	abtnItems			m_abtnItems;									// Buttons of quick slot
	int					m_nCurrentPage;									// Current quick slot page
	CTString			m_strCurrentPage;								// Text of current quick slot page
	CTString			m_strPageNumber;								// Text of quick slot pageNumber

	// Slot information
	int					m_nSelSlotBtnID;								// Selected button ID of quick slot
	BOOL				m_bShowSlotInfo;								// If slot tool tip is shown or not
	BOOL				m_bShowExSlotInfo;
	CTString			m_strSlotInfo;									// Slot information string
	UIRect				m_rcSlotInfo;									// Slot information region

	//Su-won
	BOOL				m_bGiftRecv;									//선물 도착 여부
	CTextureData		*m_ptdGiftTexture;								//선물 도착 버튼 텍스쳐

	// Region of each part
	UIRect				m_rcTitle1;										// Region of title1
	UIRect				m_rcTitle2;										// Region of title2
	UIRect				m_rcSlot;										// Region of slot
	
	// [12/10/2012 박훈] 1번 퀵슬롯은 Rotation기능이 삭제 됨으로 하나로 통일
	UIRectUV				m_rtSlotNumberText;
	UIRectUV				m_rtSlotNumberText1;
	UIRectUV				m_rtSlotNumberText2;
	UIRectUV				m_rtShop;

	// UV of each part
	UIRectUV			m_rtBackground;									// UV of background

	// [12/10/2012 박훈] UI개편 버튼
	UIRectUV			m_rtNextOver;
	UIRectUV			m_rtNextClick;
	UIRectUV			m_rtPrevOver;
	UIRectUV			m_rtPrevClick;
	UIRectUV			m_rtAddSlotOver;
	UIRectUV			m_rtAddSlotClick;
	UIRectUV			m_rtDeleteSlotOver;
	UIRectUV			m_rtDeleteSlotClick;

	UIRectUV			m_rtSlotInfoL;									// UV of left region of slot information
	UIRectUV			m_rtSlotInfoM;									// UV of middle region of slot information
	UIRectUV			m_rtSlotInfoR;									// UV of right region of slot information
	UIRectUV			m_rtBtnOutline;									// UV of outline of buttons
	UIRectUV			m_rtHorzSlotText;								// UV of horizontal slot text
	
	UIRectUV			m_rtQuestBook;
	UIRectUV			m_rtQuestBookDown;
	
	// [12/10/2012 박훈] UI개편으로 인한 캐쉬샵 수정
	UIRectUV			m_rtChashShopIdle;
	UIRectUV			m_rtCashShopOver;
	UIRectUV			m_rtCashShopDown;								// UV of cash shop button down

	//Su-won
	UIRectUV			m_rtGift[4][2];									//선물 도착 알림 버튼 UV
	eGiftPos			m_eGiftPos;										//선물 버튼 위치
	BOOL				m_bIsInside;

	int		m_nCurChildSlot;

	BOOL	m_bWearing;

protected:
	// Internal functions
	void	RenderBtns();
	//Su-won
	void	RenderGiftButton();
	void	ShowExSlotInfo( BOOL bShowInfo, int nSlotIndex = -1 );
	//Su-won
	void	SetGiftNoticePos();
public:
	// Command functions
	void	AddBtnToQuickSlot( int nSlotIndex, int nCurrentPage );
	void	SwapBtnsInQuickSlot(int nPage1, int nSlot1, int nPage2, int nSlot2);

	// Network message functions ( send )
	void	SendAddBtn( int nPage, int nSlotNum, int nSlotType = -1, int nData0 = -1, int nData1 = -1 );
	void	SendSwapBtns( int nPage1, int nSlot1, int nPage2, int nSlot2 );

public:
	CUIQuickSlot();
	~CUIQuickSlot();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	UseQuickSlot( int nSlotIndex, int nCurrentPage = 0 );
	void	ChangePage( int nPage );
	void	RemoveBtn(CUIIcon* pIcon);
	BOOL	StartSkillDelay( int nIndex );
	//Su-won
	void	SetGiftRecv(BOOL bGiftRecv)	{ m_bGiftRecv =bGiftRecv; }

	// Network message functions ( receive )
	void	AddBtn( int nPage, int nSlotNum, int nSlotType = -1, int nData0 = -1, int nData1 = -1 );
	void	SwapBtns( int nPage, int nSlotNum1, int nSlotNum2 );
	void	ClearAllBtns( int nPage );

	void	RemoveSummonSkill();
	void	RemoveElementalSkill();
	void	RemovePetSkill();
	void	RemoveWildPetSkill();

	// 스킬 초기화 사용시 퀵슬롯에 등록된 모든 스킬들을 삭제한다.
	void	ResetQuickSLotSKill();
	
	// [sora] 090731 퀵슬롯 item count Update
	void	UpdateItemCount(int nUinIndex, SQUAD llCount);

	// 내구도 작업
	void	UpdateItemDurability(int nUinIdx, UWORD now, UWORD max = 0);

	// [12/14/2012 Ranma]
	void	ToggleVisibleEXQuickSlot(BOOL bResultView);
	BOOL	CheckSlotEXBtn(eQSlotEXBtn eBtn); // 더하기 버튼인지 빼기 버튼인지
	void	SetDisableCheckSlotEX();
	BOOL	IsInQuickSolt(UIBtnExType BtnType, int nIndex); // 퀵슬롯에 등록되어 있는지를 찾는다.
	BOOL	IsInQuickSlot(int nUinIndex);
	int		GetPage() {return m_nCurrentPage;}

	void	SetWearingLock(BOOL bLock)  { m_bWearing = bLock; }
	BOOL	GetWearingLock()			{ return m_bWearing; }
	ENGINE_API void	SetToggle( int nType, int nIndex, bool bToggle );

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
private:

	void	setQuickSlotItemData(int nPage, int nSlot, int type, int idx, CItems* pItem, bool bWearTab, bool bItemRef = true);
};


#endif	// UIQUICKSLOT_H_

