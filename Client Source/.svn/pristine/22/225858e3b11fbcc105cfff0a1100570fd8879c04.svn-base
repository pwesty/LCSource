// ----------------------------------------------------------------------------
//  File : UIButtonEx.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIBUTTONEX_H_
#define	UIBUTTONEX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIDrawFigure.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>


// UI expansion button state
enum UIBtnExState
{
	UBES_IDLE		= 0,
	UBES_CLICK		= 1,
	UBES_TOTAL		= 2,
};

//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
enum eUIBtnCharNameType
{
	CHAR_NAME_NONE = 0,
	CHAR_NAME_EXPEDITION = CHAR_NAME_NONE,	// 원정대 캐릭터이름, 정보 출력
	CHAR_NAME_BUYER = 1,
	CHAR_NAME_SELLER = 2,
	CHAR_NAME_OWNER = 3,
};

enum eSocketIconInfo
{
	SOCKET_CHAOS_NONE = 0,
	SOCKET_CHAOS_EQUIP,
	SOCKET_GENERAL_NONE,
	SOCKET_GENERAL_EQUIP,
	SOCKET_MAX_ICON,
};

#define JEWEL_GRADE_GROW_Y		21 // Y 라인 간의 증가 폭
#define JEWEL_GRADE_START_Y		131
#define JEWEL_GRADE_BTN_SIZE	16
#define JEWEL_GRADE_ICON_SIZE	16
#define NAS_INDEX				60000
#define NAS_TEX_ID				0		
#define NAS_TEX_ROW				0
#define NAS_TEX_COL				0

class CUIImageFont;
class CItems;
class CUISpriteAni;
class CUIImage;


// [sora] RAID_SYSTEM 원정대 버튼 처리 
#define EXP_BTN_WIDTH 148					// 원정대 버튼 크기
#define EXP_BTN_HEIGHT 15					// 

#ifdef G_RUSSIA
#define	MAX_ITEMINFO_LINE			40
#else
#define	MAX_ITEMINFO_LINE			30
#endif

#define		DEF_MAXSTRINGCHAR		26
#if	defined(G_BRAZIL) | defined(G_RUSSIA) 		// 빌라 요청 아이템 정보 툴팁 폭 50% 크기 증가
#define		DEF_MAXSTRINGCHAREXTEND	38
#endif

// ----------------------------------------------------------------------------
// Name : CUIButton
// Desc : Expansion version for item, skill and action
// ----------------------------------------------------------------------------
class CUIButtonEx : public CUIWindow
{
protected:
	CTextureData		*m_ptdAddTexture;
	CTextureData		*m_pExpeditionTexture;
	UWORD				m_uwBtnID;							// ID of button
	int					m_nIndex;							// Index of item, skill, action or quest
	int					m_nUniIndex;						// Unique index of item
	
	// ------- Cash Item ----------------->
	int					m_nCashIndex;
	int					m_nCashType;
	CTString			m_nCashName;
	CTString			m_nCashDesc;
	int					m_nCashListCount;
	int					m_nCashTypeIndex;
	// <-----------------------------------
	
	union
	{
		SBYTE			m_nWearType;						// Type of wearing item
		SBYTE			m_nSkillLevel;						// Level of skill
		SBYTE			m_nQuestType;						// Type of quest
		SBYTE			m_nEventType;						// Type of event
	};

	ULONG				m_ulPlus;							// Plus of item
	BOOL				m_bSkillDelay;						// If skill is delay state
	SBYTE				m_sbQuestFlag;					// If quest is completed

	union
	{
		ULONG			m_ulFlag;							// Flag of item
		BOOL			m_bSpecial;							// 특수 스킬인가?
		SBYTE			m_sbRemissionType;					// 면죄부 종류.
	};

	LONG				m_ulUsed;							// 내구도
	LONG				m_ulUsed2;							

	SQUAD				m_llCount;							// Item Type : 갯수 Skill Seal Type : 숙련도 
	SQUAD				m_llPrice;							// Price of Item
	SQUAD				m_llTime;							// Use Item Start Time
	SBYTE				m_sbOptionType[MAX_OPTION_INC_ORIGIN];	// Option type of item
	LONG				m_lOptionLevel[MAX_OPTION_INC_ORIGIN];	// Option level of item
	LONG				m_lOriginOptionVar[MAX_OPTION_INC_ORIGIN];
	SWORD				m_nTab;								// Tab of item
	SWORD				m_InvenIndex;
	SWORD				m_nServerIdx;						// 서버 인덱스
	UWORD				m_uwRareIndex;						// 레어 옵션 인덱스

	SBYTE				m_nTextureID;						// Texture ID for rendering this button
	SBYTE				m_nWhichUI;							// From which UI
	UIBtnExType			m_betType;							// Type of button ( item, skill or action )	
	BOOL				m_bEmpty;
	UIBtnExState		m_bsState;							// State of UI expansion button
	UIRectUV			m_rtUV;								// UV of button	 
	
	BOOL				m_bLButtonDown;						// Left Mouse Button Click
	CTString			m_strButtonInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colButtonInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	CTString			m_strSetItemInfo[MAX_ITEMINFO_LINE];		// Set Item information string
	COLOR				m_colSetItemInfo[MAX_ITEMINFO_LINE];		// Color of Set item information string
	INDEX				m_nCurInfoLines;
	INDEX				m_nCurSetItemInfoLines;
	//UIRectUV			m_rtPopupInfo;
	CUIDrawBox			m_bxPopupInfo;
	UIRectUV			m_rtPopupName;
	BOOL				m_bShow;
	BOOL				m_bSetItem;
	BOOL				m_bWidthExtend;						// 빌라요청 가로 폭 증가 여부

	SBYTE				Item_Belong;
	LONG				Item_SkillIndex[MAX_ITEM_SKILL];
	SBYTE				Item_SkillLevel[MAX_ITEM_SKILL];
	LONG				m_lState_plus;

	// socket system [5/10/2010 rumist]
	SBYTE				Item_SocketCount;
	LONG				Item_SocketJewelIndex[MAX_SOCKET_OPTION];

	//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
	eUIBtnCharNameType	m_eCharType;
	CTString			m_strCharName;
	
	// [sora] 원정대 정보
	SBYTE m_sbJob; 
	SLONG m_slPosition;				// 직책
	UIRectUV m_rtPositionUV[2];		// 직책 표시
	UIRectUV m_rtHPUV;
	SLONG	m_slBarWidth;
	SLONG	m_slLevel;
	BOOL	m_bOnline;
	
	// Equip suit
	INDEX	m_iSuitIndex; // 한벌 의상 인덱스
	INDEX	m_iColor;

// 속성 시스템 스킬 아이콘 위에 속성 표시 [1/22/2013 Ranma]
	BOOL		m_bAttribute;
	int			m_nAtt;
	int			m_nDef;
	UIRectUV	m_rtAttributeIconAtt[eICON_ATTR_MAX];
	UIRectUV	m_rtAttributeIconDef[eICON_ATTR_MAX];
	CTextureData	*m_ptdAttrTexture;			// Texture of window

#ifdef DURABILITY
	CUIImage*	m_imgDurabilityZero;
	bool		m_bDuraZero;
#endif	//	DURABILITY

	BOOL			m_bSocketCreatedItem;
	int				m_nSocketInfoShowLine[MAX_SOCKET_OPTION];
	CTextureData	*m_ptdSocketTexture;
	UIRectUV		m_rtSocketIcon[SOCKET_MAX_ICON];
	CTextureData	*m_ptdJewelGradeTexture;
	UIRectUV		m_rtGradeIcon[JEWEL_GRADE_MAX];
	// Fortune Info
	BOOL m_bHasFortuneInfo;
	int m_nFortuneSkillIndex;
	int m_nFortuneSkillLevel;
	CTString m_strFortuneInfoTitle;
	CTString m_strFortuneSkillName;
	std::vector<CTString> m_vecFortuneSkillDesc;
	std::vector<CTString> m_vecFortuneSkillComment;
	int m_nFortuneSkillIconTexID;
	UIRectUV m_rtFotuneSkillIconUV;
	int m_nFortuneInfoWidth;
	int m_nFortuneInfoHeight;

	bool	m_bShowStack;
	CUIImageFont* m_pImageFont;

	bool	m_bSkillBlend;

	bool	m_bWearTab;

	// 내구도
	int	m_nDurNow;
	int	m_nDurMax;

	// toggle ani
	CUISpriteAni*	m_paniToggle;
	bool			m_bToggleState;

protected:
	// UV
	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	UIRectUV	&GetUV() { return m_rtUV; }

public:
	CUIButtonEx();
	~CUIButtonEx();

	// Create buton
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					SBYTE nWhichUI = -1, UIBtnExType betType = UBET_ITEM,
					SBYTE nTab = -1, LONG inven_idx = -1 );

	// Initialize button
	void	InitBtn();

	void	Destroy();

	// Copy & swap
	void	CopyItemInfo( CUIButtonEx &btnSrc );
	void	CopySkillInfo( CUIButtonEx &btnSrc );
	void	CopyActionInfo( CUIButtonEx &btnSrc );
	void	CopyComboInfo( CUIButtonEx &btnSrc );
	void	SwapItemInfo( CUIButtonEx &btnSrc );

	// Render
	void	Render();
	void	RenderInfoPopup();
	void	RenderFixInfoPopup(bool bUpDown = false/*true 위로, false 아래로*/); // 고정되는 팝업창 랜더 추가 [9/12/2012 Ranma]
	void	RenderDefaultButton();
	void	RenderHighlight( const COLOR colHighlight, const ULONG ulPBT = PBT_ADD );

	// Common information
	void	SetBaseInfo(UIBtnExType type, SBYTE cTexID, int texRow, int texCol);
	void	SetTextureID( SBYTE nTexID ) { m_nTextureID = nTexID; }
	void	SetBtnType( UIBtnExType betType ) { m_betType = betType; }
	void	SetEmpty( BOOL bEmpty ) { m_bEmpty = bEmpty; }
	void	SetBtnState( UIBtnExState bsState ) { m_bsState = bsState; }
	UWORD	GetBtnID() const { return m_uwBtnID; }
	int		GetIndex() const { return m_nIndex; }
	SBYTE	GetTextureID() const { return m_nTextureID; }
	SBYTE	GetWhichUI() const { return m_nWhichUI; }
	void	SetWhichUI( SBYTE WhichUI )	{ m_nWhichUI = WhichUI; }
	UIBtnExType		GetBtnType() const { return m_betType; }
	BOOL	IsEmpty() const { return m_bEmpty; }
	UIBtnExState	GetBtnState() const { return m_bsState; }

	// Item information
	void	SetItemInfo( SBYTE nTab, LONG inven_idx, int nIndex, int nUniIndex, SBYTE nWearingType ,
		int nCashIndex = -1, int nCashSection = -1, CTString nName ="", CTString nDesc ="", int nListCount = -1, int nTypeIndex = -1);
	void	SetItemIndex( int nIndex ) { m_nIndex = nIndex; }
	void	SetItemUniIndex( int nUniIndex ) { m_nUniIndex = nUniIndex; }
	void	SetItemWearType( SBYTE nWearType ) { m_nWearType = nWearType; }
	void	SetItemPlus(ULONG ulPlus);
	void	SetItemFlag( ULONG ulFlag ) { m_ulFlag = ulFlag; }
	void	SetItemUsed( LONG ulUsed )	{ m_ulUsed = ulUsed; }
	void	SetItemUsed2( LONG ulUsed2 ) { m_ulUsed2 = ulUsed2; }
	void	SetItemOptionData( SBYTE sbOption, SBYTE sbOptionType, LONG lOptionLevel, LONG lOriginOptionVar )
	{
		m_sbOptionType[sbOption] = sbOptionType;
		m_lOptionLevel[sbOption] = lOptionLevel;
		m_lOriginOptionVar[sbOption] = lOriginOptionVar;
	}
	void	SetItemCount( SQUAD llCount ) { m_llCount = llCount; }
	void	SetItemPrice( SQUAD llPrice ) { m_llPrice = llPrice; }
	void	SetUseItemStartTime( SQUAD llTime ) { m_llTime = llTime; }
	void	SetItemTab( SBYTE nTab ) { m_nTab = nTab; }
	void	SetInvenIndex( LONG Index) { m_InvenIndex = Index; }
	void	SetServerIndex(SWORD Index) { m_nServerIdx = Index; }
	void	SetItemLocation( SBYTE nTab, LONG inven_idx ) { m_nTab = nTab; m_InvenIndex = inven_idx; }
	void	SetItemRareIndex( UWORD uwIndex ) { m_uwRareIndex =uwIndex; }
	void	SetCashName(CTString strName) { m_nCashName = strName;}

	int		GetItemIndex() const { return m_nIndex; }
	int		GetItemUniIndex() const { return m_nUniIndex; }
	SBYTE	GetItemWearType() const { return m_nWearType; }
	// ---------- Cash Item ---------->
	int		GetCashIndex() const { return m_nCashIndex; }
	int		GetCashType() const { return m_nCashType; }
	CTString GetCashName() const { return m_nCashName; }
	CTString GetCashDesc() const { return m_nCashDesc; }
	int		GetCashListCount() const { return m_nCashListCount; }
	int		GetCashTypeIndex() const { return m_nCashTypeIndex; }
	
	//CUIWindow* GetWindow() { return m_pParentWnd ;}
	//void	  SetWindow(CUIWindow* pParentWnd) { m_pParentWnd = pParentWnd ;}	

	// <-------------------------------
	ULONG	GetItemPlus() const { return m_ulPlus; }
	LONG	GetItemUsed() const { return m_ulUsed; }
	LONG	GetItemUsed2() const { return m_ulUsed2; }
	ULONG	GetItemFlag() const { return m_ulFlag; }
	SBYTE	GetItemOptionType( int nOption ) const { return m_sbOptionType[nOption]; }
	LONG	GetItemOptionLevel( int nOption ) const { return m_lOptionLevel[nOption]; }
	LONG	GetItemOriginOptionVar( int nOption ) const { return m_lOriginOptionVar[nOption]; }
	SQUAD	GetItemCount() const { return m_llCount; }
	SQUAD	GetItemPrice() const { return m_llPrice; }
	SQUAD	GetUseItemStartTime() const { return m_llTime; }
	SBYTE	GetItemTab() const { return m_nTab; }
	SWORD	GetInvenIndex() const { return m_InvenIndex; }
	SWORD	GetServerIndex() const { return m_nServerIdx; }
	UWORD	GetItemRareIndex() const {return m_uwRareIndex; }

	// Skill information
	void	SetSkillInfo( int nIndex, SBYTE sbSkillLevel, BOOL bSpecial = FALSE );
	void	SetSkillLevel( SBYTE sbSkillLevel ) { m_nSkillLevel = sbSkillLevel; }
	void	SetSkillDelay( BOOL bSkillDelay ) { m_bSkillDelay = bSkillDelay; }
	void	SetSkillSealExp(SQUAD llCount ) { m_llCount = llCount; }
	int		GetSkillIndex() const { return m_nIndex; }
	SBYTE	GetSkillLevel() const { return m_nSkillLevel; }
	BOOL	GetSkillDelay() const { return m_bSkillDelay; }
	BOOL	GetSkillSpecial() const { return m_bSpecial; }
	SQUAD	GetSkillSealExp() const { return m_llCount; }

	// 속성 시스템 [1/23/2013 Ranma]

	int		GetAttrAtt() const 
	{ 
		return m_nAtt; 
	}

	int		GetAttrDef() const 
	{ 
		return m_nDef; 
	}


			void	SetSkillAffinityInfo( int _nIndex );
	inline	void	SetSkillAffinityIndex( int _nIndex )				{ m_nIndex = _nIndex;			}
	inline	void	SetSkillAffinityMaxPt( SQUAD _maxPoint )			{ m_llPrice = _maxPoint;		}
	inline	void	SetSkillAffinityCurrentPt( SQUAD _currentPoint )	{ m_llCount = _currentPoint;	}
	inline	void	SetSkillExpertCurrentPt( SQUAD _currentPoint )		{ m_llCount	= _currentPoint;	}
	inline	int		GetSkillAffinityIndex()		const					{ return m_nIndex;	}
	inline	SQUAD	GetSkillAffinityMaxPt()		const					{ return m_llPrice;	}
	inline	SQUAD	GetSkillAffinityCurrentPt() const					{ return m_llCount;	}
	inline	SQUAD	GetSkillExpertCurrentPt()	const					{ return m_llCount;	}
	
	// Action information
	void	SetActionInfo( int nIndex );
	int		GetActionIndex() const { return m_nIndex; }

	// 원정대
	void	SetExpeditionInfo( SBYTE sbJob, SLONG slPosition, CTString strText , SLONG slBarWidth, SLONG slLevel);
	void	CopyExpeditionInfo(CUIButtonEx &btnSrc);
	void	SetBarWidth(SLONG slBarWidth) { m_slBarWidth = slBarWidth; }
	void	SetExpeditionPosition(SLONG slPosition) { m_slPosition = slPosition; }
	void	SetOnline(BOOL bOnline) { m_bOnline = bOnline; }
	void	SetExpeditionCharName(CTString strName) { m_strCharName = strName; }
	void	SetExpeditionCharLevel(SLONG slLevel) { m_slLevel = slLevel; }
	
	// Quest information
	void	SetQuestInfo( int nIndex, SBYTE sbType, SBYTE sbQuestFlag );
	void	SetQuestFlag( BOOL bQuestFlag ) { m_sbQuestFlag = bQuestFlag; }
	int		GetQuestIndex() const { return m_nIndex; }
	SBYTE	GetQuestType() const { return m_nQuestType; }
	SBYTE	GetQuestFlag() const { return m_sbQuestFlag; }

	// Event information
	void	SetEventInfo( int nIndex );
	int		GetEventIndex() const { return m_nIndex; }

	// Remission information
	void	SetRemissionInfo( SBYTE iRemissionType, SWORD nTab = -1, SWORD inven_idx = -1, int nIndex = -1, int nUniIndex = -1, SBYTE nWearingType = -1 );
	SBYTE	GetRemissionType() const { return m_sbRemissionType; }

	void	SetShowInfo(BOOL bshow) { m_bShow = bshow; }
	BOOL	GetShowInfo() const { return m_bShow; }
	BOOL	GetSetItemChack() const { return m_bSetItem; }

	//[2/9/2009 Sora]버튼에 구매대행 관련캐릭터 이름 표시
	eUIBtnCharNameType GetCharType() { return m_eCharType; }
	CTString GetCharName() { return m_strCharName; }
	void	SetCharName(eUIBtnCharNameType eCharType, CTString strCharName);

	// [sora] 원정대 관련
	SBYTE GetCharJob() { return m_sbJob; }
	SLONG GetCharPosition() { return m_slPosition; }
	UIRectUV GetHPUV() { return m_rtHPUV; };
	SLONG GetBarWidth() { return m_slBarWidth; }
	BOOL GetOnline() { return m_bOnline; }
	SLONG GetCharLevel() { return m_slLevel; }

	// Equip Suit
	void SetSuitIndex(INDEX iSuit) { m_iSuitIndex = iSuit; }
	INDEX GetSuitIndex(void) { return m_iSuitIndex; }
	
	// Item blending color setting
	void	SetItemColor(INDEX color) { m_iColor = color; }
	INDEX	GetItemColor() { return m_iColor; }


	// MissionCase information
	void	SetComboInfo( SBYTE nTab = -1, SBYTE nRow = -1, SBYTE nCol = -1);

	// Copy & swap
	void	Copy( CUIButtonEx &btnSrc );
	void	Swap( CUIButtonEx &btnSrc );

	void	CopyItems( CItems* pItem);

	void	AddInfoString( CTString &strInfo, COLOR colInfo = 0xF2F2F2FF,BOOL bSetIteminfo = FALSE );

/////////////// 버튼 스트링 ////////////////////////////////
	void	GetButtonInfo(int &nInfoWidth, int &nInfoHeight);
/////////////// Item  /////////////////////////////////////
	//void	ItemInfo(CTString &strTemp);
	//void	noCountItem( CItemData &rItemData, CTString &strTemp );
/////////////// Item End //////////////////////////////////

/////////////// Skill  ////////////////////////////////////
	//void	SkillInfo(CTString &strTem);
/////////////// Skill End /////////////////////////////////

/////////////// Action  ////////////////////////////////////
	//void	ActionInfo(CTString &strTem);
/////////////// Action End /////////////////////////////////	
	CTString GetOptionPaciveInfo(CItemData& rItem);
	CTString GetJewelOptionInfo(CItemData& rItem);
	CTString GetOptionAciveSkillInfo(CItemData& rItem);
	CTString GetJewelOptionSkillInfo(CItemData& rItem);

	int		GetWearLevelReduction();
	int		GetJewelLevelReduction();

	void	GetSetItemInfo(int &nInfoWidth, int &nInfoHeight);
	
	DOUBLE	GetCoolTime(DOUBLE reUseTime, DOUBLE startTime);	// [091009 sora] 쿨타임 계산
	DOUBLE	GetReuseTime();
	BOOL	HasReuseTimeItem();	// [2010/08/04 : Sora] 쿨타임을 가지고 있는 아이템 체크

	inline void	SetItemBelong(SBYTE sbBelong)	{ Item_Belong = sbBelong; }
	inline SBYTE GetItemBelong()	{ return Item_Belong; }

	inline void	SetItemSkill(SBYTE sbIndex, LONG lSkillIndex, SBYTE sbSkillLevel)
	{
		Item_SkillIndex[sbIndex] = lSkillIndex;
		Item_SkillLevel[sbIndex] = sbSkillLevel;
	}
	// socket system [5/10/2010 rumist]
	inline void	InitItemSocketInfo()
	{
		Item_SocketCount = 0;
		memset( &Item_SocketJewelIndex[0], -1, sizeof(LONG) * MAX_SOCKET_OPTION );
	}

	inline void SetItemSocketCount( SBYTE sbCount )			{ Item_SocketCount = sbCount;	}
	inline void	SetItemSocket( SBYTE sbIndex, LONG lJewelIndex )
	{
		Item_SocketJewelIndex[sbIndex] = lJewelIndex;
	}

	inline LONG GetItemSkillIndex(SBYTE sbPos)
	{
		return Item_SkillIndex[sbPos];
	}
	
	inline SBYTE GetItemSkillLevel(SBYTE sbPos)
	{
		return Item_SkillLevel[sbPos];
	}

	// socket system [5/10/2010 rumist]
	inline const SBYTE	GetItemSocketCount() const			{ return Item_SocketCount;		}
	inline LONG GetItemSocketJewelIndex( SBYTE sbPos )
	{
		return Item_SocketJewelIndex[sbPos];
	}

	inline BOOL GetItemSocketExist()	const				{ return (Item_SocketCount > 0);	}

	inline void SetItemPlus2(LONG lplus)
	{
		m_lState_plus = lplus;
	}

	inline LONG GetItemPlus2()
	{
		return m_lState_plus;
	}

	// toggle Ani State
	void SetToggleState(bool bToggle) { m_bToggleState = bToggle; }
	bool IsToggleState()			  { return m_bToggleState; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	bool	GetStack() { return m_bShowStack; }
	void	SetStack(bool bStack) { m_bShowStack = bStack; }

	void	SetSkillBlend(bool bBlend, int idx);
	bool	GetSkillBlend()				{ return m_bSkillBlend; }

	void	UpdateCoolTime();

	bool	IsWearTab()				{ return m_bWearTab; }
	void	setWearTab(bool bVal)	{ m_bWearTab = bVal; }

	// 내구도
	void	SetItemDurNow(int nowDur);
	void	SetItemDurMax(int maxDur) { m_nDurMax = maxDur; }
	int	GetItemDurMax()			{ return m_nDurMax; }
	int	GetItemDurNow()			{ return m_nDurNow; }

protected:

	void	RenderStack(int nX, int nY, CDrawPort* pDrawPort);
	void	RenderDurZero(int nX, int nY, CDrawPort* pDraw);
	void	RenderToggleAni(int nX, int nY, CDrawPort* pDrawPort);
};


#endif	// UIBUTTONEX_H_

