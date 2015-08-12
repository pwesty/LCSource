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
// Character information page
enum CharInfoPage
{
	UCIP_STATUS		= 0,
	UCIP_SKILL,
	UCIP_ACTION,
	UCIP_SOCIAL,
	UCIP_GROUP,
	//UCIP_QUEST,
};


// Staus tab 
#define CHARINFO_TAB_TOTAL				5//6
#define	CHARINFO_TAB_HEIGHT				53
#define	CHARINFO_TAB_WIDTH				21
#define	CHARINFO_TAB_GAP				3
#define	CHARINFO_SUBTAB_WIDTH			96
#define	CHARINFO_3SUBTAB_WIDTH			64
#define	CHARINFO_4SUBTAB_WIDTH			48


// Ex icon coordinate
#define	CHARINFO_SLOT_SX				41
#define	CHARINFO_SLOT_SY				58
#define	CHARINFO_SLOT_INFO_CX			142


// Define status text position
#define STATUS_SEMI_TITLE_SX			47
#define STATUS_SEMI_TITLE_PROFILE_SY	31
#define	STATUS_NAME_SX					39
#define	STATUS_NAME_SY					49
#define	STATUS_DEGREE_SY				95
#define	STATUS_NAME_MAIN_SX				85
#define	STATUS_PROFILE_OFFSETY			14

#define STATUS_SEMI_TITLE_STATE_SY		141
#define	STATUS_STR_SX					39
#define	STATUS_STR_SY					159
#define	STATUS_STR_MAIN_RX				195
#define	STATUS_STR_OFFSETY				15
#define	STATUS_STATPOINT_SY				223
#define	STATUS_ATTACK_SY				240
#define	STATUS_ATTACK_OFFSETY			15
#define	STATUS_EXP_MAIN_RX				217 
#define	STATUS_EXP_SY					302
#define	STATUS_SP_SY					316
//#define	STATUS_HP_SX					75
//#define	STATUS_HP_CX					160
//#define	STATUS_HP_SY					318
//#define	STATUS_MP_SY					334

#define STATUS_SEMI_TITLE_PENALTY_SY	333
#define STATUS_PENALTY_SY				350
#define STATUS_PENALTY_ACC_SY			364

//#define ADJUST_MEMORIZE_SKILL 

// Define size of gague
#define	STATUS_BAR_WIDTH				164


// Skill tab
enum SkillTab
{
	SKILL_ITEM	= 0,
	SKILL_SPECIAL,
	SKILL_VOUCHER,			// 증표
#ifdef ADJUST_MEMORIZE_SKILL
	SKILL_MEMORIZE,
#endif
	SKILL_TOTAL,
};


// Define skill slot
#define	SKILL_SLOT_ROW					8
#define	SKILL_ACTIVE_SLOT_ROW_TOTAL		50
#define	SKILL_PASSIVE_SLOT_ROW_TOTAL	50
#define SKILL_MEMORIZE_SLOT_ROW_TOTAL	50
#define	SKILL_SPECIAL_SLOT_ROW_TOTAL	30

// Define max char and line of skill informaion 
#define	SKILLINFO_CHAR_CHAR_WIDTH		160
#define	MAX_SKILLINFO_LINE				20


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
#define	ACTION_SLOT_ROW					8
#define	ACTION_NORAML_SLOT_ROW_TOTAL	30
#define	ACTION_SOCIAL_SLOT_ROW_TOTAL	20
#define	ACTION_PARTY_SLOT_ROW_TOTAL		10
#define	ACTION_GUILD_SLOT_ROW_TOTAL		20


// Define action text position
#define	ACTION_TAB_TEXT_SY				33
#define	ACTION_NORMAL_TAB_TEXT_CX		124
#define	ACTION_SOCIAL_TAB_TEXT_CX		124
#define	ACTION_PARTY_TAB_TEXT_CX		77
#define	ACTION_GUILD_TAB_TEXT_CX		172


// Define max char and line of action informaion 
#define	ACTIONINFO_CHAR_WIDTH			160
#define	MAX_ACTIONINFO_LINE				10


/*
// Define quest slot
#define	QUEST_SLOT_ROW					5
#define	QUEST_SLOT_ROW_TOTAL			10


// Define quest text
#define	QUEST_TAB_TEXT_SY				33
#define	QUEST_TAB_TEXT_CX				77
#define	EVENT_TAB_TEXT_CX				172
#define	QUESTDESC_CHAR_WIDTH			170
*/


// Define text position
#define	CHARINFO_TITLE_TEXT_OFFSETX		25
#define	CHARINFO_TITLE_TEXT_OFFSETY		5


// Define size of character information
#define	CHARINFO_WIDTH					230
#define	CHARINFO_HEIGHT					385


// 텍스쳐만 바꿔주는 느낌으로 내부 변수들은 그대로 유지
//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#define CHARINFO_NEW_WIDTH				512
#define CHARINFO_NEW_HEIGHT				340

enum eCharInfoNewPage
{
	PAGE_CHARINFO_NEW_STATUS = 0,
	PAGE_CHARINFO_NEW_SKILL,
	PAGE_CHARINFO_NEW_ACTION_SOCIAL,
	PAGE_CHARINFO_NEW_GUILD_PARTY,
};

//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Name : CUICharacterInfo
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUICharacterInfo : public CUIWindow
{
protected:
	/**** Common *******************/
	int 					m_ucipCurrentPage;						// Current page of character information
	CUIButton				m_btnClose;								// Close button of Character information
	BOOL					m_bShowToolTip;							// If tool tip is shown or not
	CTString				m_strToolTip;							// String of tool tip
	CTString				m_strShortDesc;							// Name and level...
	CTString				m_strShortDesc2;						// Name and level...

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Region
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcCharInfoTab;						// Region of character info tab
	UIRect					m_rcTab;			      				// Region of tab
	UIRect					m_rcSubTab;								// Region of sub tab
	UIRect					m_rcToolTip;							// Region of tool tip
	UIRect					m_rcType2Splitter;						// Region of splitter of type2
	UIRect					m_rcType2Bottom;						// Region of bottom region of type2
	UIRect					m_rcIcons;								// Region of icons
	//UIRect					m_rcQuestIcons;							// Region of quest icons
	//UIRect					m_rcQuestCancelIcons;					// Region of quest cancel icons

	// UV
	UIRectUV				m_rtType1;								// UV of Type1 ( status )
	UIRectUV				m_rtType2;								// UV of Type2 ( skill, action, quest )
	UIRectUV				m_rtType2Splitter;						// UV of splitter of type2
	UIRectUV				m_rtType2Bottom;						// UV of bottom region of type2
	UIRectUV				m_rtItemTypeTab[CHARINFO_TAB_TOTAL];	// UV of item type tab
	UIRectUV				m_rtSelItemTypeTab[CHARINFO_TAB_TOTAL];	// UV of selected item type tab
	UIRectUV				m_rtTab;								// UV of tab
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	UIRectUV				m_rtToolTipL;							// UV of left region of tool tip
	UIRectUV				m_rtToolTipM;							// UV of middle region of tool tip
	UIRectUV				m_rtToolTipR;							// UV of right region of tool tip
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button


	/**** Status *******************/
	// Controls
	CUIButton				m_btnGPStrength;						// Button of strength growth point
	CUIButton				m_btnGPDexterity;						// Button of dexterity growth point
	CUIButton				m_btnGPIntelligence;					// Button of intelligence growth point
	CUIButton				m_btnGPConstitution;					// Button of constitution growth point
	BOOL					m_bLockStatPoint;						// If using stat point is locked or not

	// Information
	CTString				m_strStrength;							// String of strength
	CTString				m_strDexterity;							// String of dexterity
	CTString				m_strIntelligence;						// String of intelligence
	CTString				m_strConstitution;						// String of constitution
	CTString				m_strStatPoint;							// String of stat point
	CTString				m_strAttack;							// String of attack
	CTString				m_strMagicAttack;						// String of magic attack
	CTString				m_strDefense;							// String of defense
	CTString				m_strMagicDefense;						// String of magic defense
	CTString				m_strSP;								// String of SP
	CTString				m_strExp;								// String of Exp
	CTString				m_strPenalty;							// String of pk penalty
	CTString				m_strPenaltyAcc;						// String of pk accumulation penalty
	CTString				m_strFame;

	// Region of each part
	//UIRect					m_rcHP;									// Region of HP
	//UIRect					m_rcMP;									// Region of MP

	// UV of each part
	//UIRectUV				m_rtHP;									// UV of HP
	//UIRectUV				m_rtMP;									// UV of MP


	/**** Skill ********************/
	// Controls
	int						m_nCurrentSkillTab;						// Current skill tab
	CUIScrollBar			m_sbActiveSkillScrollBar;				// Scroll bar of active skill
	//CUIScrollBar			m_sbPassiveSkillScrollBar;				// Scroll bar of passive skill
	CUIScrollBar			m_sbMemorizeSkillScrollBar;				// Scroll bar of Memorize skill
	CUIScrollBar			m_sbSpecialSkillScrollBar;				// Scroll bar of special skill
	CUIScrollBar			m_sbVoucherSkillScrollBar;				// Scroll bar of Voucher skill

	// Skill buttons
	CUIButtonEx				m_btnActiveSkill[SKILL_ACTIVE_SLOT_ROW_TOTAL];		// Buttons of active skill
	CUIButtonEx				m_btnPassiveSkill[SKILL_PASSIVE_SLOT_ROW_TOTAL];	// Buttons of passive skill
	CUIButtonEx				m_btnMemorizeSkill[SKILL_MEMORIZE_SLOT_ROW_TOTAL];	// Buttons of Memorize skill
	CUIButtonEx				m_btnSpecialSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];	// Buttons of special skill
	CUIButtonEx				m_btnVoucherSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];	// Buttons of voucher skill
	CUIButtonEx				m_btnItemSpecialSkill[SKILL_SPECIAL_SLOT_ROW_TOTAL];

	// Skill information
	int						m_nSelActiveSkillID;					// Selected skill ID of active skill
	int						m_nSelPassiveSkillID;					// Selected skill ID of passive skill
	int						m_nSelMemorizeSkillID;					// Selected skill ID of memorize skill
	int						m_nSelSpecialSkillID;					// Selected skill ID of special skill
	int						m_nSelVoucherSkillID;					// Selected skill ID of Voucher skill
	BOOL					m_bShowSkillInfo;						// If skill tool tip is shown or not
	int						m_nCurSkillInfoLines;					// Count of current skill information lines
	CTString				m_strSkillInfo[MAX_SKILLINFO_LINE];		// Skill information string
	COLOR					m_colSkillInfo[MAX_SKILLINFO_LINE];		// Color of skill information string
	UIRect					m_rcSkillInfo;							// Skill information region


	/**** Action *******************/		
	// Controls
	BOOL					m_bPartyActionTab;						// If current tab is party action or guild action
	CUIScrollBar			m_sbNormalActionScrollBar;				// Scroll bar of normal action
	CUIScrollBar			m_sbSocialActionScrollBar;				// Scroll bar of social action
	CUIScrollBar			m_sbPartyActionScrollBar;				// Scroll bar of party action
	CUIScrollBar			m_sbGuildActionScrollBar;				// Scroll bar of guild action
	CUIScrollBar			m_sbItemSkillSpecialScrollBar;

	// Action buttons
	CUIButtonEx				m_btnNormalAction[ACTION_NORAML_SLOT_ROW_TOTAL];	// Buttons of normal action
	CUIButtonEx				m_btnSocialAction[ACTION_SOCIAL_SLOT_ROW_TOTAL];	// Buttons of special action
	CUIButtonEx				m_btnPartyAction[ACTION_PARTY_SLOT_ROW_TOTAL];		// Buttons of party action
	CUIButtonEx				m_btnGuildAction[ACTION_GUILD_SLOT_ROW_TOTAL];		// Buttons of guild action

	// Action information
	int						m_nSelNormalActionID;					// Selected action ID of normal action
	int						m_nSelSocialActionID;					// Selected action ID of social action
	int						m_nSelPartyActionID;					// Selected action ID of party action
	int						m_nSelGuildActionID;					// Selected action ID of guild action
	BOOL					m_bShowActionInfo;						// If action tool tip is shown or not
	int						m_nCurActionInfoLines;					// Count of current action information lines
	CTString				m_strActionInfo[MAX_ACTIONINFO_LINE];	// Action information string
	COLOR					m_colActionInfo[MAX_ACTIONINFO_LINE];	// Color of action information string
	UIRect					m_rcActionInfo;							// Action information region


	/**** Quest & event*************/
/*
	// Controls
	BOOL					m_bQuestTab;							// If current tab is quest or not
	CUIScrollBar			m_sbQuestIconScrollBar;					// Scroll bar of quest icon slot
	CUIScrollBar			m_sbEventIconScrollBar;					// Scroll bar of event icon slot
	CUIListBox				m_lbQuestDesc;							// List box of quest description
	CUIListBox				m_lbEventDesc;							// List box of event description
	BOOL					m_bLockRequestQuest;					// If requesting quest is locked or not

	// Quest buttons
	CUIButtonEx				m_btnQuest[QUEST_SLOT_ROW_TOTAL];		// Buttons of quest
	CUIButtonEx				m_btnEvent[QUEST_SLOT_ROW_TOTAL];		// Buttons of quest
	CUIButton				m_btnQuestCancel[QUEST_SLOT_ROW_TOTAL];	// Buttons of quest cancel

	// Quest information
	int						m_nSelQuestID;							// Selected quest ID
	int						m_nSelEventID;							// Selected event ID
*/

	// FIXME : 여기에 들어갈 부분이 아닌거 같음.
	int						m_nTargetIndex;
	SBYTE					m_nTargetUIType;
	//BOOL					m_bLockCharacterInfo;



//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
	enum eStatusTab
	{
		STATUS_TAB_PKINFO_N_ATTRIBUTE = 0, //PK & 속성탭
		STATUS_TAB_PHYSICAL_N_MAGICAL,     // 물리&마법 공격력
		STATUS_TAB_MAX,					
	};

	enum eAttributeATTInfo
	{
		ATTRIBUTE_NONE_ATT = 0,
		ATTRIBUTE_FIRE_ATT = 1,
		ATTRIBUTE_WATER_ATT = 2,
		ATTRIBUTE_EARTH_ATT = 3,
		ATTRIBUTE_WIND_ATT = 4,
		ATTRIBUTE_DARK_ATT = 5,
		ATTRIBUTE_LIGHT_ATT = 6,
		ATTRIBUTE_END_ATT,
	};

	enum eAttributeDEFInfo
	{
		ATTRIBUTE_NONE_DEF = 0,
		ATTRIBUTE_FIRE_DEF = 1,
		ATTRIBUTE_WATER_DEF = 2,
		ATTRIBUTE_EARTH_DEF = 3,
		ATTRIBUTE_WIND_DEF = 4,
		ATTRIBUTE_DARK_DEF = 5,
		ATTRIBUTE_LIGHT_DEF = 6,
		ATTRIBUTE_END_DEF,
	};

	CTString				m_strTitleName;

	UIRectUV				m_rtBackground;
	UIRectUV				m_rtSkillInfo;
	UIRectUV				m_rtAttributeInfo;	// 속성 부분의 표시 화면

	// 속성 시스템 [1/17/2013 Ranma]
	UIRectUV				m_rtAttributeIconAtt[ATTRIBUTE_END_ATT];
	UIRectUV				m_rtAttributeIconDef[ATTRIBUTE_END_DEF];
	UIRectUV				m_rtTabIcon[8];
	UIRectUV				m_rtSelectedTab;	//좌측 세로탭
	UIRectUV				m_rtSelectedTabMiddle;				
	UIRectUV				m_rtSelectedTabLow;
	UIRectUV				m_rtUnSelectedTab; //선택되지 않은 세로 탭
	UIRectUV				m_rtUnSelectedTabMiddle;				
	UIRectUV				m_rtUnSelectedTabLow;
	UIRectUV				m_rtSelectedStatusTab; //선택된 가로탭 
	UIRectUV				m_rtSelectedStatusTabMiddle;
	UIRectUV				m_rtSelectedStatusTabRight;
	UIRectUV				m_rtSelectedStatusTabDeco; //탭 장식
	UIRectUV				m_rtUnSelectedStatusTab; //안 선택된 가로탭
	UIRectUV				m_rtUnSelectedStatusTabMiddle;
	UIRectUV				m_rtUnSelectedStatusTabRight;
 	UIRectUV				m_rtUnSelectedStatusTabDeco;

	UIRectUV				m_rtCharInfoDescUL;								
	UIRectUV				m_rtCharInfoDescUM;								
	UIRectUV				m_rtCharInfoDescUR;								
	UIRectUV				m_rtCharInfoDescML;								
	UIRectUV				m_rtCharInfoDescMM;								
	UIRectUV				m_rtCharInfoDescMR;								
	UIRectUV				m_rtCharInfoDescLL;								
	UIRectUV				m_rtCharInfoDescLM;								
	UIRectUV				m_rtCharInfoDescLR;								

	UIRectUV				m_rtSelOutlineTopL;	//선택된 스킬 표시 라인
	UIRectUV				m_rtSelOutlineTopM;
	UIRectUV				m_rtSelOutlineTopR;
	UIRectUV				m_rtSelOutlineMiddleL;
	UIRectUV				m_rtSelOutlineMiddleM;
	UIRectUV				m_rtSelOutlineMiddleR;
	UIRectUV				m_rtSelOutlineBottomL;
	UIRectUV				m_rtSelOutlineBottomM;
	UIRectUV				m_rtSelOutlineBottomR;


	// 스킬 설명창
	UIRectUV				m_rtSkillDescUL;								
	UIRectUV				m_rtSkillDescUM;								
	UIRectUV				m_rtSkillDescUR;								
	UIRectUV				m_rtSkillDescML;								
	UIRectUV				m_rtSkillDescMM;								
	UIRectUV				m_rtSkillDescMR;								
	UIRectUV				m_rtSkillDescLL;								
	UIRectUV				m_rtSkillDescLM;								
	UIRectUV				m_rtSkillDescLR;								

	UIRect					m_rcTabLayer; //옆에 탭 그리는 너비
	UIRect					m_rcSkillActionTab;
	UIRect					m_rcInfoRegion;
	UIRect					m_rcStatusTab[STATUS_TAB_MAX]; // 캐릭터 스테터스 내부의 탭영역
	UIRect					m_rcCharInfoDesc;
	UIRect					m_rcButtonArea;	//버튼영역을 더 넓게 체크
	UIRect					m_rcSkillDesc; //스킬 설명창영역


	BOOL					m_bCharDescVisible;

	int						m_bStatusTabOpen[STATUS_TAB_MAX]; //각 스테터스 내부 탭이 열려있는지 여부
	int						m_nSkillActionUpperTab[3];
	int						m_nSelectedSkillID;
	int						m_nCurrentSkillType;
	int						m_nBtnTotal;
	int						m_pSelectedSkillSatisfied[SKILL_ACTIVE_SLOT_ROW_TOTAL];
	int						m_nSkillDescRow;
	
	CTString				m_strHitRate;	//물리 명중도
	CTString				m_strDodgeRate;	//물리 회피도
	CTString				m_strCritical;	//크리티컬
	CTString				m_strSpeed;		//이동속도
	CTString				m_strMagicHitRate;	//마법 명중도
	CTString				m_strMagicDodgeRate;		//마법 회피도
	CTString				m_strDeadly;		//데들리
	CTString				m_strAttackSpeed;			//공격속도
	CTString				m_strAttributeFire;	//속성
	CTString				m_strAttributeWater;
	CTString				m_strAttributeEarth;
	CTString				m_strAttributeWind;
	CTString				m_strAttributeLight;
	CTString				m_strAttributeDark;
	
	CTString				m_strCharInfoDesc;
	CTString				m_strDesc;

	CUIScrollBar			m_sbScrollBar;
	CUIButtonEx*			m_btnSelectedSkill;	//현재 스킬

	CUIRectString			m_rsSkillName;
	CUIRectString			m_rsSkillDesc;
	CUIRectString			m_rsCurrentSkillInfo;
	CUIRectString			m_rsNextSkillInfo;
													//NEW_USER_INTERFACE///
//////////////////////////////////////////////////////////////////////////
protected:
	// Internal functions
	void	ResetActions();
	void	ResetSkills();
	void	RenderStatusContent();
	void	RenderSkillContent();
	void	RenderActionContent();
	void	RenderSocialContent();
	void	RenderGroupContent();
	//void	RenderQuestContent();
	void	RenderSkillBtns();
	void	RenderActionBtns();
	//void	RenderQuestBtns();
	void	AddSkillInfoString( CTString &strSkillInfo, COLOR colSkillInfo = 0xF2F2F2FF );
	void	GetSkillInfo( int nSkillIndex, int nSkillLevel, BOOL bSpecial, int &nInfoWidth, int &nInfoHeight );
	void	ShowSkillInfo( BOOL bShowInfo, CUIButtonEx *pSkillBtn = NULL, BOOL bSpecial = FALSE );
	void	GetSkillDesc( int nSkillIndex, int nSkillLevel, BOOL bSpecial = FALSE);
	void	AddActionInfoString( CTString &strActionInfo, COLOR colActionInfo = 0xF2F2F2FF );
	void	GetActionInfo( int nActionIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowActionInfo( BOOL bShowInfo, CUIButtonEx *pActionBtn = NULL );
	void	GetActionDesc( int nActionIndex );
	//void	AddQuestDescString( CTString &strDesc, const COLOR colDesc );
	//void	AddEventDescString( CTString &strDesc );
	//void	GetQuestDesc( BOOL bFullDesc, int nQuestIndex );
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

	// Network message functions ( send )
	//void	SendQuestReq( int nNpcIndex );
	//void	SendQuestCancel( int nQuestIndex );
	//void	SendQuestStart( int nQuestIndex );
	//void	SendQuestPrize( int nQuestIndex);
	void	SendUseStatPoint( UBYTE ubStatType );

//////////////////////////////////////////////////////////////////////////
//NEW_USER_INTERFACE
	void	RenderNewCharacterInfoStatus();						// 캐릭터 정로 렌더링
	void	RenderNewCharacterInfoSkill();						// 스킬, 액션등의 렌더링
	void	SetCurrentPageNewCharacterInfo(int nTabNum);		// 좌측 탭 설정
	void	SetCurrentSkillInfo(int nSideTab, int nUpperTab);	// 스킬창 탭 설정
	void	ShowCharInfoDesc(int nTabID = -1);					// 촤측 탭 설명 창
	COLOR	GetStrColor(int nAdded);						// 능력치 보정 색조사
	void	RenderCharInfoDesc();								// 좌측 탭 설명 렌더
	void	ShowSkillDesc(int nRow = -1);						// 스킬 설명 창 세팅
	void	RenderSkillDesc();									
	void	AddSkillDescString(int nSkillInfoList, CTString strSkillInfo, COLOR strColor = 0xF2F2F2FF);
	void	ResetSkillDescString();
	void	SetSkillDescPos(int nPosX, int nPosY);
	int	    GetSkillDescWidth();
	int 	GetSkillDescHeight();

	void	UpdateSkillInfo(int nType, int nTotal);
	void	updateStatusPt(int type, int value);
													//NEW_USER_INTERFACE///
//////////////////////////////////////////////////////////////////////////
public:
	CUICharacterInfo();
	~CUICharacterInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
	void	CreateNewCharacterInfo( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	RenderNewCharacterInfo();
	WMSG_RESULT MouseMessageNewCharacterInfo( MSG *pMsg );
													//NEW_USER_INTERFACE///
//////////////////////////////////////////////////////////////////////////
	// Render
	void	Render();

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
	void	ToggleVisibleSkill();
	void	ToggleVisibleAction();
	void	ToggleVisibleSocial();
	void	ToggleVisibleGroup();
	//void	ToggleVisibleQuest();

	// Get skill level
	SBYTE	GetSkillLevel( int nIndex, BOOL bSpecial );

	// Get string of sp
	CTString	&GetStringOfSP() { return m_strSP; }

	// Register actions
	void	RegisterActions();

	// Request quest 
	//ENGINE_API void	RequestQuest( int nNpcIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ );

	// Unlock requesting quest & using stat point
	//void	UnlockQuest() { m_bLockRequestQuest = FALSE; }
	void	UnlockStatPoint() { m_bLockStatPoint = FALSE; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	UseAction( int nIndex );
	void	UseSkill( int nIndex );
	void	ClearSkills();
	void	ClearSSkills();
	void	UpdateSkillLevel( int nIndex, SBYTE sbLevel, BOOL bSpecial = FALSE);
	void	StartSkillDelay( int nIndex );
	BOOL	GetSkillDelay( int nIndex );
	//void	ClearQuests();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void	NotifyStatPoint();
	void	UseStatPoint( UBYTE ubStatType, SLONG slStatChange );
	void	StatPointError( SBYTE sbError );
	void	AddSkill( int nSkillIndex, SBYTE sbSkillLevel, BOOL bSpecial = FALSE );
	void	RemoveSkill( int nSkillIndex, BOOL bSpecial = FALSE );
	void	AddSeal( int nSkillIndex, SQUAD nSealExp );
	void	UpdateSealExp(int nSealIndex, SQUAD lSealExp );

	// 소서러가 사용하는 모든 소환 스킬을 제거합니다.
	void	RemoveSummonSkill();

	// nSSkillType은 0 = 채굴, 1 = 채집, 2 = 차지
	int	CheckSSkill( int nSSkillType );

	// 인덱스에 해당하는 스킬을 갖고 있는지 확인.
	BOOL CheckSSkillByIndex( int iIndex, int nNeedLevel = -1, BOOL* bNeedLevel = FALSE );
	UQUAD GetSealExp(int nIndex);

	CUIButtonEx*	GetSkillBtn(int idx);
	void			UpdateSkillCoolTime()
	{
		int i;
		for( i = 0; i < SKILL_ACTIVE_SLOT_ROW_TOTAL; ++i )
		{
			m_btnActiveSkill[i].UpdateCoolTime();
		}
	}

	// FIXME : UIQuest와 UICharacterInfo에 들어가야 할 부분이 구분이 제대로 안됨.
/*
	void	OpenQuestIntro( CQuestDynamicData *pQuestDD );
	void	CancelQuestRequest();
	void	AddQuest( int nQuestIndex, BOOL bNewQuest = TRUE );
	void	RemoveQuest( int nQuestIndex );
	void	UpdateQuestData( int nQuestIndex );
	void	CompleteQuest( int nQuestIndex );
*/

	inline	int GetMobIndex() const { return m_nTargetIndex; }
	//inline	void UnlockCharacterInfo()	{ m_bLockCharacterInfo = FALSE; }

public :

protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
	void OnRender( CDrawPort* pDraw );
};


#endif	// UICHARACTERINFO_H_

