// ----------------------------------------------------------------------------
//  File : UICharacterInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICHARACTERINFO_H_
#define	UICHARACTERINFO_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Interface/UISkillLearn.h>

// Define skill slot
#define	SKILL_SLOT_ROW					8
#define	SKILL_ACTIVE_SLOT_ROW_TOTAL		50
#define	SKILL_PASSIVE_SLOT_ROW_TOTAL	50
#define SKILL_MEMORIZE_SLOT_ROW_TOTAL	50
#define	SKILL_SPECIAL_SLOT_ROW_TOTAL	30

// Action type
enum ActionType
{
	ACTION_NORMAL	= 0,
	ACTION_SOCIAL,
	ACTION_PARTY,
	ACTION_GUILD,
	ACTION_PET,
	ACTION_WILDPET,
};

// Define action slot
#define	ACTION_NORAML_SLOT_ROW_TOTAL	30
#define	ACTION_SOCIAL_SLOT_ROW_TOTAL	20
#define	ACTION_PARTY_SLOT_ROW_TOTAL		10
#define	ACTION_GUILD_SLOT_ROW_TOTAL		20

enum eCharInfoNewPage
{
	PAGE_CHARINFO_NEW_STATUS = 0,
	PAGE_CHARINFO_NEW_SKILL,
	PAGE_CHARINFO_NEW_ACTION_SOCIAL,
	PAGE_CHARINFO_NEW_GUILD_PARTY,
	PAGE_CHARINFO_NEW_SYNDICATE, // 결사대
	PAGE_CHARINFO_NEW_MAX
};

//////////////////////////////////////////////////////////////////////////

class CUITab;
class CUIText;
class CUICheckButton;
class CUIImage;
class CUIImageArray;
class CUIArray;
class CUIArrayItem;
class CUIScrollBar;
class CUIProgressBar;

// ----------------------------------------------------------------------------
// Name : CUICharacterInfo
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUICharacterInfo : public CUIWindow
{
protected:
	int 					m_ucipCurrentPage;						// Current page of character information
	BOOL					m_bLockStatPoint;						// If using stat point is locked or not

	// Skill buttons
	int						m_nIdxSpecialSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];	// Buttons of special skill
	int						m_nIdxVoucherSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];	// Buttons of voucher skill
	int						m_nIdxItemSpecialSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];

	// Action buttons
	int						m_nIdxNormalAction[ACTION_NORAML_SLOT_ROW_TOTAL];	// Buttons of normal action
	int						m_nIdxSocialAction[ACTION_SOCIAL_SLOT_ROW_TOTAL];	// Buttons of special action
	int						m_nIdxPartyAction[ACTION_PARTY_SLOT_ROW_TOTAL];		// Buttons of party action
	int						m_nIdxGuildAction[ACTION_GUILD_SLOT_ROW_TOTAL];		// Buttons of guild action

	int*					m_pIdxSelectedSkill;	//현재 스킬
	//////////////////////////////////////////////////////////////////////////
	CTString				m_strTitleName;

	int						m_nSkillActionUpperTab[3];
	int						m_nCurrentSkillType;
	int						m_nBtnTotal;
	int						m_pSelectedSkillSatisfied[SKILL_ACTIVE_SLOT_ROW_TOTAL];
	//////////////////////////////////////////////////////////////////////////
protected:
	//////////////////////////////////////////////////////////////////////////
	//NEW_USER_INTERFACE
	void	SetCurrentPageNewCharacterInfo(int nTabNum);		// 좌측 탭 설정
	void	SetCurrentSkillInfo(int nSideTab, int nUpperTab);	// 스킬창 탭 설정
	COLOR	GetStrColor(int nAdded);						// 능력치 보정 색조사
	
	void	UpdateSkillInfo(int nType, int nTotal);
	void	updateStatusPt(int type, int value);
	//NEW_USER_INTERFACE///
	//////////////////////////////////////////////////////////////////////////
public:
	CUICharacterInfo();
	~CUICharacterInfo();

	// Create
	void	initialize();

	void	initCahrInfo(); // Tab 1 캐릭터 정보
	void	initSyndicateInfo(); // Tab 5 결사대 정보
	
	void	initSecondTab(); // Tab 2 아이템, 특수, 증표
	void	SetSSkill();
	void	SetItemSkill();
	void	SetSeal();

	void	initThirdTab(); // Tab 3 액션, 소셜
	void	SetAction();
	void	SetSocial();

	void	initCommunity(); // Tab 4 길드, 파티
	void	SetGuild();
	void	SetParty();
	
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Update status
	void	UpdateStatus( int iAttack, int iMagic, int iDefense, int iResist );
	void	UpdateExpAndSp(void);

	// Toggle visible
	void	ToggleVisible();
	void	ToggleVisibleStatus();
	void	ToggleVisibleAction();
	void	ToggleVisibleSocial();
	void	ToggleVisibleGroup();
	void	ToggleVisibleSyndicate();

	// Register actions
	void	RegisterActions();
	void	UnlockStatPoint() { m_bLockStatPoint = FALSE; }

	// Command functions
	void	UseAction( int nIndex );
	void	UseSkill( int nIndex );
	void	ClearSkills();
	void	ClearSSkills();
	void	StartSkillDelay( int nIndex );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void	NotifyStatPoint();
	void	UseStatPoint( UBYTE ubStatType, SLONG slStatChange );
	void	StatPointError( SBYTE sbError );
	void	AddSkill( int nSkillIndex, SBYTE sbSkillLevel, BOOL bSpecial = FALSE );
	void	AddSeal( int nSkillIndex, SQUAD nSealExp );
	void	UpdateSealExp(int nSealIndex, SQUAD lSealExp );

	// nSSkillType은 0 = 채굴, 1 = 채집, 2 = 차지
	int	CheckSSkill( int nSSkillType );

	// 인덱스에 해당하는 스킬을 갖고 있는지 확인.
	BOOL CheckSSkillByIndex( int iIndex, int nNeedLevel = -1, BOOL* bNeedLevel = FALSE );

	// Internal functions
	void		SendUseStatPoint( UBYTE ubStatType );

	// 신규 함수
	void		OpenUI(int nTabNum);
	void		CloseUI();

	void		ChangeTab();
	void		ChangeTabSkill();
	void		ChangeTabAction();
	void		ChangeTabCommunity();

	void		OnUpdate( float fDeltaTime, ULONG ElapsedTime );


	void		DragSkill(int nSkillIdx, BOOL bSpecial);
	void		DragAction(int nActionIdx);

	// 결사대 업데이트
	void		UpdateSyndicateMark();
	void		UpdateTotalJewel();
	void		UpdateMySyndiPos();
	void		UpdateSyndiHistory();

	void UpdateSelectImage( CUIIcon* pIcon );

	// 신규 UI 변수
private:
	void		ChangeCheckImg(int nTabNum);
	void		ChangePage(int nTabNum);
	void		ChangeTabPage(int nSubNum);
	void		UpdateBaseInfo();
	void		SetChildVisible(CUIArrayItem* pItem, bool bVisible);
	void		UpdateArr(CUIArray* pArr, int nItemCnt);

	// main
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX;
	int			m_nMouseY;

	CUIText*	m_pstrTitle;
	CUIText*	m_pMoveTitle;
	CUIButton*	m_pBtnClose;

	CUICheckButton* m_pBtnMainCheckBtn[PAGE_CHARINFO_NEW_MAX];
	CUIImage*	m_pImgCheck[PAGE_CHARINFO_NEW_MAX];
	CUIImage*	m_pImgUnCheck[PAGE_CHARINFO_NEW_MAX];

	CUITab*		m_pMainTab;

	struct stStatTooltip
	{
		int nStrIndex[4];
	};

	stStatTooltip*	m_pstrTooltip;
	int				m_nTooltipMax;

	void LoadTooltipString();
	CTString GetTooltipString(int nJob, int _2nd, int index);
	void UpdateStatTooltip();

	enum eCHARINFO_BASE
	{
		eCHARINFO_BASE_NAME,
		eCHARINFO_BASE_CLASS,
		eCHARINFO_BASE_LEVEL,
		eCHARINFO_BASE_GUILD,
		eCHARINFO_BASE_GUILDPOS,
		eCHARINFO_BASE_SUB,
		eCHARINFO_BASE_SUB2,
		eCHARINFO_BASE_SP,
		eCHARINFO_BASE_FRAME,
		eCHARINFO_BASE_EXP,
		eCHARINFO_BASE_MAX
	};

	enum eCHARINFO_STAT
	{
		eCHARINFO_STAT_STR,
		eCHARINFO_STAT_DEX,
		eCHARINFO_STAT_INT,
		eCHARINFO_STAT_CON,
		eCHARINFO_STAT_STATPOINT,
		eCHARINFO_STAT_HP,
		eCHARINFO_STAT_MP,
		eCHARINFO_STAT_ATTACK,
		eCHARINFO_STAT_DEFENSE,
		eCHARINFO_STAT_HITRATE,
		eCHARINFO_STAT_DODGERATE,
		eCHARINFO_STAT_CRITICAL,
		eCHARINFO_STAT_MOVESPEED,
		eCHARINFO_STAT_MAGICATTACK,
		eCHARINFO_STAT_MAGICDEFENSE,
		eCHARINFO_STAT_MAGICHITRATE,
		eCHARINFO_STAT_MAGICDODGERATE,
		eCHARINFO_STAT_DEADLY,
		eCHARINFO_STAT_ATTACKSPEED,
		eCHARINFO_STAT_MAX
	};

	//----------------------------------------------------------
	//        
	//       |-----------------------|
	//       | Base     |   stat     |
	//       |          |            |
	//       |          |
	//       |-----------
	//       | Pk, 속성  |
	//       |----------|

	// PAGE_CHARINFO_NEW_STATUS tab
	CUIText*	m_ptxtBase[eCHARINFO_BASE_MAX];
	CUIText*	m_ptxtStat[eCHARINFO_STAT_MAX];

	enum 
	{ 
		eBTN_MAX = 4
	};
	CUIButton*	m_pBtnStatus[eBTN_MAX];

	// PK정보, 속성정보
	CUIText*	m_pstrPk;
	CUIText*	m_pstrPkPoint;
	CUIText*	m_pstrPkPointAcc;

	CUIImageArray*	m_pImgArrAtt;
	CUIImageArray*	m_pImgArrDef;
	CUIText*	m_pstrAttlv;
	CUIText*	m_pstrDeflv;

	enum eTAB_SKILL
	{
		eTAB_SKILL_ITEM = 0,
		eTAB_SKILL_SSKILL,
		eTAB_SKILL_SEAL,
		eTAB_SKILL_MAX,
	};
	//----------------------------------------------------------
	//       |item | Sskill | Seal   |
	//       |-----------------------|
	//       |1icon|Text|2icon|Text  |
	//       |          |            |
	//       |          |            |

	CUITab*		m_pTabSkill;
	CUIArray*	m_pArrSkill[eTAB_SKILL_MAX];
	CUIScrollBar* m_pScrollSkill[eTAB_SKILL_MAX];

	enum eTAB_ACTION
	{
		eTAB_ACTION_ACTION = 0,
		eTAB_ACTION_SOCIAL,
		eTAB_ACTION_MAX,
	};
	//----------------------------------------------------------
	//       |action | social        |
	//       |-----------------------|
	//       |1icon|Text|2icon|Text  |
	//       |          |            |
	//       |          |            |

	CUITab*		m_pTabAction;
	CUIArray*	m_pArrAction[eTAB_ACTION_MAX];
	CUIScrollBar* m_pScrollAction[eTAB_ACTION_MAX];

	enum eTAB_COMMUNITY
	{
		eTAB_COMMUNITY_PARTY = 0,
		eTAB_COMMUNITY_GUILD,
		eTAB_COMMUNITY_MAX,
	};
	//----------------------------------------------------------
	//       |Party | Guild          |
	//       |-----------------------|
	//       |1icon|Text|2icon|Text  |
	//       |          |            |
	//       |          |            |

	CUITab*		m_pTabCommunity;
	CUIArray*	m_pArrCommunity[eTAB_COMMUNITY_MAX];
	CUIScrollBar* m_pScrollCommunity[eTAB_COMMUNITY_MAX];

	// 결사대
	enum eTAB_SYNDICATE
	{
		eTAB_SYNDICATE_KAY = 0,
		eTAB_SYNDICATE_DEAL,
		eTAB_SYNDICATE_MAX
	};

	enum eSTRING_TYPE
	{
		eSTRING_TYPE_TIME = 0,
		eSTRING_TYPE_HISTORY,
	};
	
	CTString	GetHistoryText(eSTRING_TYPE eType, int nIdx);

	CUIProgressBar* m_pProgressBar[eTAB_SYNDICATE_MAX];
	CUIButton*		m_pBtnSyndicateInfo;
	CUIImage*		m_pImgMark[eTAB_SYNDICATE_MAX];
	CUIImage*		m_pImgNoJoin;
	CUIText*		m_pTextMyPos;
	CUIProgressBar* m_pPbSyndicateExp;
	CUITextBox*		m_pTbHistory;
	CUIScrollBar*	m_pScrollSyndiHistory;
	CUIImage*		m_pImgSelect;
};


#endif	// UICHARACTERINFO_H_

