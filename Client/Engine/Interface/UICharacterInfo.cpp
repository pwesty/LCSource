#include "stdh.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UICharacterInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Contents/Base/UICalendar.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIBilling.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Interface/UIItemProduct.h>
#include <Engine/Interface/UIAffinityInfo.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Contents/Base/UIRankingSystem.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIParty.h>
#include <Engine/Interface/UIPartyAuto.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Interface/UIQuest.h>
#include <Engine/Contents/Login/ServerSelect.h>

//static INDEX	g_iQuestIndex;
//static INDEX	g_iSelBtn;

#define PARTY_AUTO_ENABLE	// 파티 오토 시스템 열림

#define ACTION_NUM_SELL		(28)

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxActionStringChar = 0;
static int	_iMaxSkillStringChar = 0;
//static int	_iMaxQuestDescStringChar = 0;

// Define skill text position
#define	SKILL_TAB_TEXT_SY				33
#ifdef ADJUST_MEMORIZE_SKILL
#define	SKILL_ACTIVE_TAB_TEXT_CX		54
#define	SKILL_PASSIVE_TAB_TEXT_CX		102
#define	SKILL_MEMORIZE_TAB_TEXT_CX		149
#define	SKILL_SPECIAL_TAB_TEXT_CX		197
#else
#define	SKILL_ACTIVE_TAB_TEXT_CX		61
#define	SKILL_PASSIVE_TAB_TEXT_CX		125
#define	SKILL_SPECIAL_TAB_TEXT_CX		61
#define	SKILL_VOUCHER_TAB_TEXT_CX		125
#endif

extern INDEX g_iCountry;
extern UINT	g_uiEngineVersion;

extern INDEX g_iXPosInCharInfo;
extern INDEX g_iYPosInCharInfo;


CTString _ClassName[11];

extern int _aSummonSkill[5] = { 293, 294, 300, 301, 307 };
//////////////////////////////////////////////////////////////////////////
//NEW_USER_INTERFACE
#define	CHARINFO_NEW_TAB_HEIGHT				50
#define	CHARINFO_NEW_UPPERTAB_WIDTH			66
#define CHARINFO_NEW_STATUS_TAB_WIDTH		115


// NEW_USER_INTERFACE
#define	SKILLINFO_NEW_SLOT_SX				43
#define	SKILLINFO_NEW_SLOT_SY				100
#define	SKILLINFO_NEW_SLOT_OFFSETY				37
#define	SKILLINFO_NEW_NAME_CX					159
#define	SKILLINFO_NEW_NAME_SY					102
#define	SKILLINFO_NEW_NEED_RX					191
#define	SKILLINFO_NEW_SLOT_OFFSETX				234

#define	SKILLINFO_NEW_ACTIVE_TAB_CX			64
#define	SKILLINFO_NEW_PASSIVE_TAB_CX			192
#define	SKILLINFO_NEW_TAB_SY					43

#define	SKILLINFO_NEW_TAB_WIDTH				122

#define	SKILLINFO_NEW_CURSP_SX					98
#define	SKILLINFO_NEW_CURSP_RX					243
#define	SKILLINFO_NEW_CURSP_SY					290


COLOR	m_ClassNameColor[11];				//성향 등급에 따른 색상
CTextureData* m_ptdButtonTexture;
//////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// Name : CUICharacterInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICharacterInfo::CUICharacterInfo()
{
	m_ucipCurrentPage	= UCIP_STATUS;
	m_bShowToolTip		= FALSE;
	m_strToolTip		= CTString( "" );

	m_bLockStatPoint	= FALSE;
	m_strStrength		= CTString( "" );
	m_strDexterity		= CTString( "" );
	m_strIntelligence	= CTString( "" );
	m_strConstitution	= CTString( "" );
	m_strStatPoint		= CTString( "" );
	m_strAttack			= CTString( "" );
	m_strMagicAttack	= CTString( "" );
	m_strDefense		= CTString( "" );
	m_strMagicDefense	= CTString( "" );
	m_strExp			= CTString( "" );
	m_strSP				= CTString( "" );
	m_strPenalty		= CTString( "" );
	m_strPenaltyAcc		= CTString( "" );
	m_strFame			= CTString( "" );
	
//NEW_USER_INTERFACE
	m_strHitRate = CTString( "" );	//물리 명중도
	m_strDodgeRate = CTString( "" );	//물리 회피도
	m_strCritical = CTString( "" );	//크리티컬
	m_strSpeed = CTString( "" );		//이동속도

	m_strMagicHitRate	= CTString( "" );	//마법 명중도
	m_strMagicDodgeRate = CTString( "" );		//마법 회피도
	m_strDeadly = CTString( "" );;		//데들리
	m_strAttackSpeed = CTString( "" );			//공격속도


	m_strAttributeFire		= CTString( "" );
	m_strAttributeWater	= CTString( "" );
	m_strAttributeEarth			= CTString( "" );
	m_strAttributeWind				= CTString( "" );
	m_strAttributeLight		= CTString( "" );
	m_strAttributeDark		= CTString( "" );

	_ClassName[0] = _S(4050, "카오스 킹"); _ClassName[1] = _S(4051, "다크 나이트"); _ClassName[2] = _S(1196, "어쌔신");
	_ClassName[3] = _S(4052, "머더"); _ClassName[4] = _S(4053, "무법자"); _ClassName[5] = _S(92, "일반");
	_ClassName[6] = _S(4054, "헌터"); _ClassName[7] = _S(4055, "헌터 마스터"); _ClassName[8] = _S(4056, "나이트");
	_ClassName[9] = _S(4057, "세인트 나이트"); _ClassName[10] =_S(4058, "가디언");

	m_strTitleName = _S(69, "캐릭터 정보");

	m_bCharDescVisible = FALSE;

	//성향등급에 따른 색상 (순서는 위의성향등급명참조)
	m_ClassNameColor[0] = 0xFF0000FF;
	m_ClassNameColor[1] = 0xFF0000FF;
	m_ClassNameColor[2] = 0xFF0000FF;
	m_ClassNameColor[3] = 0xFF0000FF;
	m_ClassNameColor[4] = 0xFFFFFFFF;
	m_ClassNameColor[5] = 0xFFFFFFFF;
	m_ClassNameColor[6] = 0x00FF00FF;
	m_ClassNameColor[7] = 0x00FF00FF;
	m_ClassNameColor[8] = 0x00FF00FF;
	m_ClassNameColor[9] = 0x00FF00FF;
	m_ClassNameColor[10] = 0x00FF00FF;

	m_ptdButtonTexture = NULL;
//////////////////////////////////////////////////////////////////////////
	m_nCurrentSkillTab		= SKILL_ITEM;
	m_nSelActiveSkillID		= -1;
	m_nSelPassiveSkillID	= -1;
	m_nSelMemorizeSkillID	= -1;
	m_nSelSpecialSkillID	= -1;
	m_bShowSkillInfo		= FALSE;
	m_nCurSkillInfoLines	= 0;

	m_bPartyActionTab		= TRUE;
	m_nSelNormalActionID	= -1;
	m_nSelSocialActionID	= -1;
	m_nSelPartyActionID		= -1;
	m_nSelGuildActionID		= -1;
	m_bShowActionInfo		= FALSE;
	m_nCurActionInfoLines	= 0;

	//m_bQuestTab				= TRUE;
	//m_bLockRequestQuest		= FALSE;
	//m_nSelQuestID			= -1;
	//m_nSelEventID			= -1;
	m_nTargetIndex			= -1;
	m_nTargetUIType			= -1;
	//m_bLockCharacterInfo = FALSE;
	m_ptdButtonTexture = NULL;

	m_nSkillDescRow = -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUICharacterInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICharacterInfo::~CUICharacterInfo()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
}
// 신형 캐릭터 정보 초기화
void CUICharacterInfo::CreateNewCharacterInfo(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_bStatusTabOpen[STATUS_TAB_PKINFO_N_ATTRIBUTE] = 0;
	m_bStatusTabOpen[STATUS_TAB_PHYSICAL_N_MAGICAL] = 0;

	for(int i=0; i<3; i++)
	{
		m_nSkillActionUpperTab[i] = 0;
	}

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// 배경
	m_rtBackground.SetUV(0, 0, 512, 341, fTexWidth, fTexHeight );
	m_rtSkillInfo.SetUV(0, 348, 478, 641, fTexWidth, fTexHeight);
	m_rtAttributeInfo.SetUV(266, 653, 495, 713, fTexWidth, fTexHeight );
// 속성 시스템 아이콘 UV [1/17/2013 Ranma]
	// 무
	m_rtAttributeIconAtt[ATTRIBUTE_NONE_ATT].SetUV(266, 717, 304, 755, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_NONE_DEF].SetUV(307, 717, 345, 755, fTexWidth, fTexHeight );
	// 불
	m_rtAttributeIconAtt[ATTRIBUTE_FIRE_ATT].SetUV(266, 757, 304, 795, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_FIRE_DEF].SetUV(307, 757, 345, 795, fTexWidth, fTexHeight );
	// 물
	m_rtAttributeIconAtt[ATTRIBUTE_WATER_ATT].SetUV(266, 798, 304, 836, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_WATER_DEF].SetUV(307, 798, 345, 836, fTexWidth, fTexHeight );
	// 대지
	m_rtAttributeIconAtt[ATTRIBUTE_EARTH_ATT].SetUV(266, 880, 304, 918, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_EARTH_DEF].SetUV(307, 880, 345, 918, fTexWidth, fTexHeight );
	// 바람
	m_rtAttributeIconAtt[ATTRIBUTE_WIND_ATT].SetUV(266, 839, 304, 877, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_WIND_DEF].SetUV(307, 839, 345, 877, fTexWidth, fTexHeight );
	// 암
	m_rtAttributeIconAtt[ATTRIBUTE_DARK_ATT].SetUV(266, 962, 304, 1000, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_DARK_DEF].SetUV(307, 962, 345, 1000, fTexWidth, fTexHeight );
	// 빛
	m_rtAttributeIconAtt[ATTRIBUTE_LIGHT_ATT].SetUV(266, 921, 304, 959, fTexWidth, fTexHeight );
	m_rtAttributeIconDef[ATTRIBUTE_LIGHT_DEF].SetUV(307, 921, 345, 959, fTexWidth, fTexHeight );

	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();


	// 각 아이콘 표시
	m_rtTabIcon[0].SetUV(149, 205, 162, 219, fTexWidth, fTexHeight );
	m_rtTabIcon[1].SetUV(166, 205, 179, 219,fTexWidth, fTexHeight );
	m_rtTabIcon[2].SetUV(183, 205, 196, 219, fTexWidth, fTexHeight );
	m_rtTabIcon[3].SetUV(200, 205, 213, 219, fTexWidth, fTexHeight );
	m_rtTabIcon[4].SetUV(149, 222, 162, 236, fTexWidth, fTexHeight );
	m_rtTabIcon[5].SetUV(166, 222, 179, 236, fTexWidth, fTexHeight );
	m_rtTabIcon[6].SetUV(183, 222, 196, 236, fTexWidth, fTexHeight );
	m_rtTabIcon[7].SetUV(200, 222, 213, 236, fTexWidth, fTexHeight );

	//선택된테두리
	m_rtSelOutlineTopL.SetUV( 145, 138, 157, 150, fTexWidth, fTexHeight );
	m_rtSelOutlineTopM.SetUV( 157, 138, 226, 150, fTexWidth, fTexHeight );
	m_rtSelOutlineTopR.SetUV( 226, 138, 239, 150, fTexWidth, fTexHeight );
	m_rtSelOutlineMiddleL.SetUV( 145, 150, 157, 158, fTexWidth, fTexHeight );
	m_rtSelOutlineMiddleM.SetUV( 157, 150, 226, 158, fTexWidth, fTexHeight );
	m_rtSelOutlineMiddleR.SetUV( 226, 150, 239, 158, fTexWidth, fTexHeight );
	m_rtSelOutlineBottomL.SetUV( 145, 158, 157, 171, fTexWidth, fTexHeight );
	m_rtSelOutlineBottomM.SetUV( 157, 158, 226, 171, fTexWidth, fTexHeight );
	m_rtSelOutlineBottomR.SetUV( 226, 158, 239, 171, fTexWidth, fTexHeight );

	//스킬 설명창
	m_rtSkillDescUL.SetUV( 0, 137, 20, 157, fTexWidth, fTexHeight );
	m_rtSkillDescUM.SetUV( 20, 137, 120, 157, fTexWidth, fTexHeight );
	m_rtSkillDescUR.SetUV( 120, 137, 140, 157, fTexWidth, fTexHeight );
	m_rtSkillDescML.SetUV( 0, 157, 20, 177, fTexWidth, fTexHeight );
	m_rtSkillDescMM.SetUV( 20, 157, 120, 177, fTexWidth, fTexHeight );
	m_rtSkillDescMR.SetUV( 120, 157, 140, 177, fTexWidth, fTexHeight );
	m_rtSkillDescLL.SetUV( 0, 206, 20, 226, fTexWidth, fTexHeight );
	m_rtSkillDescLM.SetUV( 20, 206, 120, 226, fTexWidth, fTexHeight );
	m_rtSkillDescLR.SetUV( 120, 206, 140, 226, fTexWidth, fTexHeight );

	//사이드 탭 텍스쳐
	m_rtSelectedTab.SetUV(59, 0, 80, 15, fTexWidth, fTexHeight);
	m_rtSelectedTabMiddle.SetUV(59, 16, 80, 36, fTexWidth, fTexHeight);
	m_rtSelectedTabLow.SetUV(59, 85, 80, 101, fTexWidth, fTexHeight);


	//사이드 비선택 탭
	m_rtUnSelectedTab.SetUV(86, 0, 107, 15, fTexWidth, fTexHeight);
	m_rtUnSelectedTabMiddle.SetUV(86, 16, 107, 36, fTexWidth, fTexHeight);
	m_rtUnSelectedTabLow.SetUV(86, 85, 107, 101, fTexWidth, fTexHeight);

	m_rtSelectedStatusTab.SetUV(0, 106, 15, 127, fTexWidth, fTexHeight );
	m_rtSelectedStatusTabMiddle.SetUV(16, 106, 31, 127, fTexWidth, fTexHeight );
	m_rtSelectedStatusTabRight.SetUV(85, 106, 100, 127, fTexWidth, fTexHeight );
	m_rtSelectedStatusTabDeco.SetUV(122, 34, 132, 44, fTexWidth, fTexHeight );

	m_rtUnSelectedStatusTab.SetUV(104, 106, 119, 127, fTexWidth, fTexHeight );
	m_rtUnSelectedStatusTabMiddle.SetUV(120, 106, 135, 127, fTexWidth, fTexHeight );
	m_rtUnSelectedStatusTabRight.SetUV(189, 106, 204, 127, fTexWidth, fTexHeight );
 	m_rtUnSelectedStatusTabDeco.SetUV(139, 34, 149, 44, fTexWidth, fTexHeight );

	m_rtCharInfoDescUL.SetUV( 0, 229, 8, 237, fTexWidth, fTexHeight );
	m_rtCharInfoDescUM.SetUV( 8, 229, 76, 237, fTexWidth, fTexHeight );
	m_rtCharInfoDescUR.SetUV( 76, 229, 84, 237, fTexWidth, fTexHeight );
	m_rtCharInfoDescML.SetUV( 0, 237, 8, 243, fTexWidth, fTexHeight );
	m_rtCharInfoDescMM.SetUV( 8, 237, 76, 243, fTexWidth, fTexHeight );
	m_rtCharInfoDescMR.SetUV( 76, 237, 84, 243, fTexWidth, fTexHeight );
	m_rtCharInfoDescLL.SetUV( 0, 243, 8, 251, fTexWidth, fTexHeight );
	m_rtCharInfoDescLM.SetUV( 8, 243, 76, 251, fTexWidth, fTexHeight );
	m_rtCharInfoDescLR.SetUV( 76, 243, 84, 251, fTexWidth, fTexHeight );

	m_btnClose.Create( this, CTString( "" ), 488, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK,229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_DISABLE,155, 74, 169, 88, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );


	m_sbScrollBar.Create( this, 487, 99, 10, 219 );
	m_sbScrollBar.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( SKILLLEARN_NEW_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetItemsPerPage( SKILLLEARN_NEW_SLOT_ROW );

	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -430, 0, 430, 322 );

	// Strength growth button
	m_btnGPStrength.Create( this, CTString( "" ), 474, 45, 15, 15 );
	m_btnGPStrength.SetUV( UBS_IDLE, 121, 49, 136, 64, fTexWidth, fTexHeight );
	m_btnGPStrength.SetUV( UBS_CLICK, 138, 49, 153, 64, fTexWidth, fTexHeight );
	m_btnGPStrength.SetUV( UBS_DISABLE, 155, 74, 170, 89, fTexWidth, fTexHeight );
	m_btnGPStrength.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGPStrength.SetEnable( FALSE );


	// Dexterity growth button
	m_btnGPDexterity.Create( this, CTString( "" ), 474, 67, 15, 15 );
	m_btnGPDexterity.SetUV( UBS_IDLE, 121, 49, 136, 64, fTexWidth, fTexHeight );
	m_btnGPDexterity.SetUV( UBS_CLICK, 138, 49, 153, 64, fTexWidth, fTexHeight );
	m_btnGPDexterity.SetUV( UBS_DISABLE, 155, 74, 170, 89, fTexWidth, fTexHeight );
	m_btnGPDexterity.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGPDexterity.SetEnable( FALSE );

	// Intelligence growth button
	m_btnGPIntelligence.Create( this, CTString( "" ), 474, 88, 15, 15 );
	m_btnGPIntelligence.SetUV( UBS_IDLE, 121, 49, 136, 64, fTexWidth, fTexHeight );
	m_btnGPIntelligence.SetUV( UBS_CLICK, 138, 49, 153, 64, fTexWidth, fTexHeight );
	m_btnGPIntelligence.SetUV( UBS_DISABLE, 155, 74, 170, 89, fTexWidth, fTexHeight );
	m_btnGPIntelligence.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGPIntelligence.SetEnable( FALSE );

	// Constitution growth button
	m_btnGPConstitution.Create( this, CTString( "" ), 474, 109, 15, 15 );
	m_btnGPConstitution.SetUV( UBS_IDLE, 121, 49, 136, 64, fTexWidth, fTexHeight );
	m_btnGPConstitution.SetUV( UBS_CLICK, 138, 49, 153, 64, fTexWidth, fTexHeight );
	m_btnGPConstitution.SetUV( UBS_DISABLE, 155, 74, 170, 89, fTexWidth, fTexHeight );
	m_btnGPConstitution.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGPConstitution.SetEnable( FALSE );


	m_rcIcons.SetRect( 0, 0, SKILLINFO_NEW_WIDTH, SKILLINFO_NEW_HEIGHT);
	m_rcTitle.SetRect( 0, 0, CHARINFO_NEW_WIDTH, 36 );
	//사이드 탭 영역
	m_rcCharInfoTab.SetRect(7, 37, 28, 246);

	//탭 그리는 크기
	m_rcTabLayer.SetRect(7, 37, 28, 87);
	m_rcInfoRegion.SetRect(28, 37, 506, 330);
	m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].SetRect(33, 240, 263, 261);
	m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].SetRect(270, 150, 500, 171);

	//m_rcSkillActionTab.SetRect(34, 44, 233, 64);
	m_rcSkillActionTab.SetRect(34, 44, 298, 64);
	m_rcCharInfoDesc.SetRect(0, 0, 100, 30);

	m_rsSkillName.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);
	m_rsSkillDesc.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rsCurrentSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);
	m_rsNextSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);

	m_rcSkillInfo.SetRect(0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rcButtonArea.SetRect(0, 0, 212, 34);

		// Skill buttons
	// Active skill
	int		iRow;

	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_btnItemSpecialSkill[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_SKILL, 0, iRow );

	}
	// Special skill
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_btnSpecialSkill[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_SKILL, 0, iRow );
	}
	// Voucher skill
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_btnVoucherSkill[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_SKILL, 0, iRow );
	}
	
	// Action buttons
	// Normal action button
	for( iRow = 0; iRow < ACTION_NORAML_SLOT_ROW_TOTAL ; iRow++ )
	{
		
		m_btnNormalAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_ACTION, 0, iRow);
		
	}
	// Social action button
	for( iRow = 0; iRow < ACTION_SOCIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		
		m_btnSocialAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_ACTION, 0, iRow);
		
	}
	// Party action button
	for( iRow = 0; iRow < ACTION_PARTY_SLOT_ROW_TOTAL ; iRow++ )
	{
		
		m_btnPartyAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_ACTION, 0, iRow);
		
	}
	// Guild action button
	for( iRow = 0; iRow < ACTION_GUILD_SLOT_ROW_TOTAL ; iRow++ )
	{
		
		m_btnGuildAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
										UBET_ACTION, 0, iRow);
		
	}
	
}


//////////////////////////////////////////////////////////////////////////
// 수치 변동에 따른 컬러값 계산
//////////////////////////////////////////////////////////////////////////
COLOR CUICharacterInfo::GetStrColor(int nAdded)
{
	if(nAdded == 0)
	{
		return 0xFFC000FF;
	}
	else if(nAdded > 0)
	{
		return 0x00B050FF;
	}
	else if(nAdded < 0)
	{
		return 0xFF0000FF;
	}

	return 0xFF0000FF;
}

void CUICharacterInfo::RenderCharInfoDesc()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	pDrawPort->AddTexture( m_rcCharInfoDesc.Left, m_rcCharInfoDesc.Top,
										m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Top + 8,
										m_rtCharInfoDescUL.U0, m_rtCharInfoDescUL.V0, m_rtCharInfoDescUL.U1, m_rtCharInfoDescUL.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Top,
										m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Top + 8,
										m_rtCharInfoDescUM.U0, m_rtCharInfoDescUM.V0, m_rtCharInfoDescUM.U1, m_rtCharInfoDescUM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Top,
										m_rcCharInfoDesc.Right, m_rcCharInfoDesc.Top + 8,
										m_rtCharInfoDescUR.U0, m_rtCharInfoDescUR.V0, m_rtCharInfoDescUR.U1, m_rtCharInfoDescUR.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Left, m_rcCharInfoDesc.Top + 8,
										m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Bottom - 8,
										m_rtCharInfoDescML.U0, m_rtCharInfoDescML.V0, m_rtCharInfoDescML.U1, m_rtCharInfoDescML.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Top + 8,
										m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Bottom - 8,
										m_rtCharInfoDescMM.U0, m_rtCharInfoDescMM.V0, m_rtCharInfoDescMM.U1, m_rtCharInfoDescMM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Top + 8,
										m_rcCharInfoDesc.Right, m_rcCharInfoDesc.Bottom - 8,
										m_rtCharInfoDescMR.U0, m_rtCharInfoDescMR.V0, m_rtCharInfoDescMR.U1, m_rtCharInfoDescMR.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Left, m_rcCharInfoDesc.Bottom - 8,
										m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Bottom,
										m_rtCharInfoDescLL.U0, m_rtCharInfoDescLL.V0, m_rtCharInfoDescLL.U1, m_rtCharInfoDescLL.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Left + 8, m_rcCharInfoDesc.Bottom - 8,
										m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Bottom,
										m_rtCharInfoDescLM.U0, m_rtCharInfoDescLM.V0, m_rtCharInfoDescLM.U1, m_rtCharInfoDescLM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcCharInfoDesc.Right - 8, m_rcCharInfoDesc.Bottom - 8,
										m_rcCharInfoDesc.Right, m_rcCharInfoDesc.Bottom,
										m_rtCharInfoDescLR.U0, m_rtCharInfoDescLR.V0, m_rtCharInfoDescLR.U1, m_rtCharInfoDescLR.V1,
										0xFFFFFFBB );

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	int nPosX, nPosY;
	
	nPosX = m_rcCharInfoDesc.Left + (m_rcCharInfoDesc.GetWidth() / 2);
	nPosY = m_rcCharInfoDesc.Top+ ((m_rcCharInfoDesc.GetHeight() - _pUIFontTexMgr->GetFontHeight()) / 2);

	pDrawPort->PutTextExCX( m_strCharInfoDesc, nPosX, nPosY, 0xF2F2F2FF );

	pDrawPort->EndTextEx();
}

void CUICharacterInfo::ShowCharInfoDesc(int nTabID)
{

	m_strCharInfoDesc.Clear();
	
	CTString strTemp;
	
	if(nTabID == PAGE_CHARINFO_NEW_SKILL)
	{
		strTemp.PrintF("%s", _S(6077, "특수 스킬") );
	}
	else if(nTabID == PAGE_CHARINFO_NEW_ACTION_SOCIAL)
	{
		strTemp.PrintF( "%s,%s",_S(94, "액션") ,_S(96, "소셜") );	
	}
	else if(nTabID == PAGE_CHARINFO_NEW_GUILD_PARTY)
	{
		strTemp.PrintF("%s,%s",_S(97, "파티") ,_S(98, "길드") );
	}
	else
	{
		strTemp.PrintF("%s", _S(69, "캐릭터 정보") );
	}

	m_strCharInfoDesc = strTemp;

	int nNewPosX = m_nPosX + m_rcTabLayer.Left - 100;
	int nNewPosY = m_nPosY + m_rcTabLayer.Top + (m_rcTabLayer.GetHeight() * nTabID) + (nTabID * 2);

#if defined(JAPAN)
	{
		if(nTabID == PAGE_CHARINFO_NEW_ACTION_SOCIAL)
		{
			m_rcCharInfoDesc.SetRect(0, 0, 140, 30);
		}
		else
		{
			m_rcCharInfoDesc.SetRect(0, 0, 100, 30);
		}

		nNewPosX = m_nPosX + m_rcTabLayer.Left - m_rcCharInfoDesc.GetWidth();
	}
#endif

	if(nNewPosX < 0)
	{
		nNewPosX = m_rcTabLayer.Right;
	}

	m_rcCharInfoDesc.SetRect(nNewPosX, nNewPosY, nNewPosX + m_rcCharInfoDesc.GetWidth(), nNewPosY + m_rcCharInfoDesc.GetHeight());
}

void  CUICharacterInfo::ShowSkillDesc(int nRow /* = -1 */)
{
	ResetSkillDescString();

	CTString strTemp;
	COLOR	strColor;

	int nIndex = 0;
	if(m_nCurrentSkillType > BTN_SKILL_VOUCHER)
	{
		nIndex = m_btnSelectedSkill[nRow].GetActionIndex();
	}
	else
	{
		nIndex = m_btnSelectedSkill[nRow].GetSkillIndex(); 
	}

	if(m_nCurrentSkillType == BTN_SKILL_ACTIVE || m_nCurrentSkillType == BTN_SKILL_PASSIVE || m_nCurrentSkillType == BTN_SKILL_VOUCHER)
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );
		int nLevel = CUIManager::getSingleton()->GetCharacterInfo()->GetSkillLevel(nIndex, FALSE);
		if(nLevel == 0 && (rSkill.GetType() != CSkill::ST_SEAL))
			return;
		
		strColor =  0xFFFF00FF;
		if (rSkill.GetType() == CSkill::ST_SEAL)
		{
			strTemp.PrintF("%s",rSkill.GetName());
		}else
		{
#if defined(G_RUSSIA)
				strTemp.PrintF("%s %s.%d",rSkill.GetName(), _S( 4414, "LV" ), nLevel);
#else
				strTemp.PrintF("%s lv.%d",rSkill.GetName(), nLevel);			
#endif
		}
		m_rsSkillName.AddString(strTemp, strColor);
		
		nLevel--;
		strColor = 0xF2F2F2FF;

		switch( rSkill.GetType() )
		{
		case CSkill::ST_MELEE:					// Active
		case CSkill::ST_RANGE:					// Active
		case CSkill::ST_MAGIC:					// Active
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				if( rSkill.GetFlag() & SF_SINGLEMODE )
					strTemp.PrintF(_S(499, "퍼스널던전 전용 스킬"));
				else
					strTemp.PrintF(_S(63, "액티브 스킬"));
				
				m_rsSkillDesc.AddString(strTemp, strColor);
				
				strTemp = rSkill.GetDescription();
				m_rsSkillDesc.AddString(strTemp, strColor);

				int NeedItem[2], NeedItemCount[2];
				CItemData* pNeedItemData[2];
				CTString strNeedItem[2];
				int i;

				NeedItem[0] = rSkill.GetNeedItemIndex1(m_btnSelectedSkill[nRow].GetSkillLevel()-1);
				NeedItem[1] = rSkill.GetNeedItemIndex2(m_btnSelectedSkill[nRow].GetSkillLevel()-1);
				NeedItemCount[0] = rSkill.m_vectorSkillLevels[m_btnSelectedSkill[nRow].GetSkillLevel()-1].needItemCount1;
				NeedItemCount[1] = rSkill.m_vectorSkillLevels[m_btnSelectedSkill[nRow].GetSkillLevel()-1].needItemCount2;

				for (i=0; i<2; ++i)
				{
					pNeedItemData[i] = _pNetwork->GetItemData(NeedItem[i]);

					if (NeedItem[i] > 0)
					{
						strNeedItem[i].PrintF(" %s(%d)", pNeedItemData[i]->GetName(), NeedItemCount[i]);
					}
				}

				const char* pNeedWords = _S(4405, "소모 아이템");
				
				if (NeedItem[0] > 0 || NeedItem[1] > 0)
				{
					strTemp.PrintF("%s:%s%s", pNeedWords, strNeedItem[0], strNeedItem[1]);
					m_rsSkillDesc.AddString(strTemp, strColor);
				}

				int	nNeedMP = rSkill.GetNeedMP( nLevel );
				int	nNeedHP = rSkill.GetNeedHP( nLevel );
				if( nNeedHP == 0 )
				{
					if( nNeedMP != 0 )
					{
						strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );	
						AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
					}
					else
					{
						strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );
						AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
					}
				}

				if( rSkill.GetPower( nLevel ) > 0 )
				{
					strTemp.PrintF( _S( 65, "위력 : %d" ), rSkill.GetPower( nLevel ) );
					AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
				}

				strTemp.PrintF( _S( 66, "유효 거리 : %.1f" ), rSkill.GetFireRange() );
				AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
				if(rSkill.GetDurTime(nLevel) > 0)
				{
					strTemp.PrintF( _S( 4172, "지속시간 : %d초 " ), rSkill.GetDurTime(nLevel) / 10);
					AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
				}
				
				if(rSkill.GetReUseTime() > 0)
				{
					strTemp.PrintF( _S( 4173, "재사용 시간 : %d초 " ), rSkill.GetReUseTime() / 10);
					AddSkillDescString(SKILLINFO_CURRENT, strTemp, strColor);
				}

			}
			break;

		case CSkill::ST_PASSIVE:				// Passive
		{
			strTemp.PrintF(_S(67, "패시브 스킬"));
			m_rsSkillDesc.AddString(strTemp, strColor);
			
			strTemp = rSkill.GetDescription();
			m_rsSkillDesc.AddString(strTemp, strColor);
		}
		break;
		case CSkill::ST_SEAL:
		{
			strTemp.PrintF(_S(4490, "증표"));
			m_rsSkillDesc.AddString(strTemp, strColor);

			strTemp = rSkill.GetDescription();
			m_rsSkillDesc.AddString(strTemp, strColor);
		}break;
		}

	}
	else if(m_nCurrentSkillType == BTN_SKILL_SPECIAL)
	{

		CSpecialSkill &rSkill = _pNetwork->GetSSkillData( nIndex );

		int nLevel = CUIManager::getSingleton()->GetCharacterInfo()->GetSkillLevel(nIndex, TRUE);
		
		strColor =  0xFFFF00FF;
#if defined(G_RUSSIA)
			strTemp.PrintF("%s %s.%d",rSkill.GetName(), _S( 4414, "LV" ), nLevel);
#else
			strTemp.PrintF("%s lv.%d",rSkill.GetName(), nLevel);
#endif
		m_rsSkillName.AddString(strTemp, strColor);
		

		nLevel--;
		strColor = 0xF2F2F2FF;

		strTemp.PrintF(_S(656, "생산스킬"));
		m_rsSkillDesc.AddString(strTemp, strColor);
		
		strTemp = rSkill.GetDescription();
		m_rsSkillDesc.AddString(strTemp, strColor);
		AddSkillDescString(SKILLINFO_CURRENT, CTString(""), 0x00000000);
	}
	else
	{
		CAction	&rSelAction = _pNetwork->GetActionData(nIndex);

		strTemp = rSelAction.GetName();
		m_rsSkillDesc.AddString(strTemp, 0xFFFF00FF);
		
		strTemp = rSelAction.GetDesc();
		m_rsSkillDesc.AddString(strTemp, 0xF2F2F2FF);
		AddSkillDescString(SKILLINFO_CURRENT, CTString(""), 0x00000000);
	}


	int nNewPosX;
	int nNewPosY;

	m_btnSelectedSkill[nRow].GetAbsPos(nNewPosX, nNewPosY);

	if(nNewPosX + GetSkillDescWidth() > CUIManager::getSingleton()->GetDrawPort()->GetWidth())
	{
		nNewPosX -= GetSkillDescWidth();
	}
	if(nNewPosY + GetSkillDescHeight() + 34 > CUIManager::getSingleton()->GetDrawPort()->GetHeight())
	{
		nNewPosY -= GetSkillDescHeight();
	}
	else
	{
		nNewPosY += 34;
	}

	SetSkillDescPos(nNewPosX, nNewPosY);	
}


void CUICharacterInfo::RenderSkillDesc()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	ShowSkillDesc(m_nSkillDescRow);

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
										m_rtSkillDescUL.U0, m_rtSkillDescUL.V0, m_rtSkillDescUL.U1, m_rtSkillDescUL.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
										m_rtSkillDescUM.U0, m_rtSkillDescUM.V0, m_rtSkillDescUM.U1, m_rtSkillDescUM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top,
										m_rcSkillInfo.Right, m_rcSkillInfo.Top + 20,
										m_rtSkillDescUR.U0, m_rtSkillDescUR.V0, m_rtSkillDescUR.U1, m_rtSkillDescUR.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
										m_rtSkillDescML.U0, m_rtSkillDescML.V0, m_rtSkillDescML.U1, m_rtSkillDescML.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
										m_rtSkillDescMM.U0, m_rtSkillDescMM.V0, m_rtSkillDescMM.U1, m_rtSkillDescMM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Right, m_rcSkillInfo.Bottom - 20,
										m_rtSkillDescMR.U0, m_rtSkillDescMR.V0, m_rtSkillDescMR.U1, m_rtSkillDescMR.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom,
										m_rtSkillDescLL.U0, m_rtSkillDescLL.V0, m_rtSkillDescLL.U1, m_rtSkillDescLL.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom,
										m_rtSkillDescLM.U0, m_rtSkillDescLM.V0, m_rtSkillDescLM.U1, m_rtSkillDescLM.V1,
										0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Right, m_rcSkillInfo.Bottom,
										m_rtSkillDescLR.U0, m_rtSkillDescLR.V0, m_rtSkillDescLR.U1, m_rtSkillDescLR.V1,
										0xFFFFFFBB );


	m_rsSkillName.Render();
	m_rsSkillDesc.Render();

	m_rsCurrentSkillInfo.Render();
	m_rsNextSkillInfo.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
}


void CUICharacterInfo::AddSkillDescString(int nSkillInfoList, CTString strSkillInfo, COLOR strColor)
{
	if(nSkillInfoList == SKILLINFO_CURRENT)
	{
		m_rsCurrentSkillInfo.AddString(strSkillInfo, strColor);
	}
	else if(nSkillInfoList == SKILLINFO_NEXT)
	{
		m_rsNextSkillInfo.AddString(strSkillInfo, strColor);
	}

	int nHeight = 0;
	int nWidth =  0;

	if(m_rsNextSkillInfo.GetMaxStrWidth() == 0)
	{

		m_rsSkillName.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);
		
		m_rsSkillDesc.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MIN_WIDTH;
	}
	else
	{

		m_rsSkillName.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);
		
		m_rsSkillDesc.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MAX_WIDTH;
	}

	nHeight = m_rsSkillName.GetMaxStrHeight() + m_rsSkillDesc.GetMaxStrHeight();
	if( m_rsCurrentSkillInfo.GetMaxStrHeight() > m_rsNextSkillInfo.GetMaxStrHeight() )
	{
		nHeight += m_rsCurrentSkillInfo.GetMaxStrHeight();
	}
	else
	{
		nHeight += m_rsNextSkillInfo.GetMaxStrHeight();
	}


	m_rcSkillInfo.SetRect(m_rcSkillInfo.Left, m_rcSkillInfo.Top, m_rcSkillInfo.Left + nWidth + 20, m_rcSkillInfo.Top + nHeight + 20);
}


void CUICharacterInfo::ResetSkillDescString()
{
	m_rsSkillName.ClearString();
	m_rsSkillDesc.ClearString();

	m_rsCurrentSkillInfo.ClearString();
	m_rsNextSkillInfo.ClearString();
}


void CUICharacterInfo::SetSkillDescPos(int nPosX, int nPosY)
{
	int nNextPosy;

	m_rsSkillName.SetPos(nPosX + 10, nPosY + 10);
	nNextPosy = nPosY + m_rsSkillName.GetHeight() + 10;
	m_rsSkillDesc.SetPos(nPosX + 10, nNextPosy);

	nNextPosy += m_rsSkillDesc.GetHeight();
	m_rsCurrentSkillInfo.SetPos(nPosX + 10, nNextPosy);

	m_rsNextSkillInfo.SetPos(nPosX + m_rsCurrentSkillInfo.GetWidth() + 10, nNextPosy);

	m_rcSkillInfo.SetRect(nPosX, nPosY, nPosX+m_rcSkillInfo.GetWidth(), nPosY+m_rcSkillInfo.GetHeight());
}


int CUICharacterInfo::GetSkillDescWidth()
{
	return m_rcSkillInfo.GetWidth();
}


int CUICharacterInfo::GetSkillDescHeight()
{
	return m_rcSkillInfo.GetHeight();
}



void CUICharacterInfo::RenderNewCharacterInfoSkill()
{
	//스킬, 액션등 렌더링

	int nX, nY;
	int nWidth, nHeight;

	int nPosX, nPosY;
	GetAbsPos(nPosX, nPosY);

	int	iRow, iRowS, iRowE;

	nX = m_rcSkillActionTab.Left + m_nPosX;
	nY = m_rcSkillActionTab.Top + m_nPosY;

	nWidth = m_rcSkillActionTab.GetWidth() / 4;
	nHeight = m_rcSkillActionTab.GetHeight();

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	nX = nPosX + m_rcInfoRegion.Left;
	nY = nPosY + m_rcInfoRegion.Top;
	nWidth = m_rcInfoRegion.GetWidth();
	nHeight = m_rcInfoRegion.GetHeight();

	pDrawPort->AddTexture( nX, nY, nX + nWidth, nY + nHeight,
										m_rtSkillInfo.U0, m_rtSkillInfo.V0, m_rtSkillInfo.U1, m_rtSkillInfo.V1,
										0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );


	m_sbScrollBar.Render();

	pDrawPort->FlushRenderingQueue();

	//스킬 이름 출력
	int nX2 = 0;
	nY = SKILLINFO_NEW_NAME_SY;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	int nCharSP		= _pNetwork->MyCharacterInfo.sp;
	iRowS = m_sbScrollBar.GetScrollPos() * 2;
	iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		if( (iRow - iRowS) % 2 != 0)
		{
			nX = SKILLINFO_NEW_NAME_CX + SKILLINFO_NEW_SLOT_OFFSETX;
			nX2  = SKILLINFO_NEW_NEED_RX + SKILLINFO_NEW_SLOT_OFFSETX;
		}
		else
		{
			nX = SKILLINFO_NEW_NAME_CX;
			nX2  = SKILLINFO_NEW_NEED_RX;
			if(iRow != iRowS)
				nY += SKILLINFO_NEW_SLOT_OFFSETY;
		}

		if( m_btnSelectedSkill[iRow].IsEmpty() )
			break;


		if(m_nCurrentSkillType == BTN_SKILL_ITEM)
		{
			CSkill	&rSkill= _pNetwork->GetSkillData( m_btnSelectedSkill[iRow].GetSkillIndex() );

			SBYTE	sbLevel = m_btnSelectedSkill[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );


			m_strDesc.PrintF( "%s", rSkill.GetName() );
			pDrawPort->PutTextExCX( m_strDesc, nPosX + nX, nPosY + nY,
													0xFFC672FF);

#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
			m_strDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "숙련도" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about japan, german, europe3, europe2, netherlands.
			// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
			m_strDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about russia
			m_strDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
			pDrawPort->PutTextExRX( m_strDesc, nPosX + nX2,
													nPosY + nY + 17, 0xBDA99FFF );
		}
		else if(m_nCurrentSkillType == BTN_SKILL_SPECIAL)
		{
			CSpecialSkill &rSSkill = _pNetwork->GetSSkillData( m_btnSelectedSkill[iRow].GetSkillIndex() );

			SBYTE	sbLevel = m_btnSelectedSkill[iRow].GetSkillLevel();
			int		nNeedLevel = rSSkill.GetLearnLevel( sbLevel - 1 );


			m_strDesc.PrintF( "%s", rSSkill.GetName() );
			pDrawPort->PutTextExCX( m_strDesc, nPosX + nX, nPosY + nY,
													0xFFC672FF);

#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
			m_strDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "숙련도" ), rSSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about japan, german, europe3, europe2, netherlands.
			// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
			m_strDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about russia
			m_strDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSSkill.GetLearnSP( sbLevel - 1 ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
			pDrawPort->PutTextExRX( m_strDesc, nPosX + nX2,
													nPosY + nY + 17, 0xBDA99FFF );
		}
		else if(m_nCurrentSkillType == BTN_SKILL_VOUCHER)
		{
			CSkill	&rSelSeal = _pNetwork->GetSkillData(m_btnSelectedSkill[iRow].GetActionIndex());

			m_strDesc.PrintF( "%s", rSelSeal.GetName() );
			pDrawPort->PutTextExCX( m_strDesc, nPosX + nX, nPosY + nY,
													0xFFC672FF);
			m_strDesc.PrintF( _S(4491, "숙련도 : %I64d"), m_btnSelectedSkill[iRow].GetSkillSealExp() );
#if defined(G_USA)
			{
				pDrawPort->PutTextExCX( m_strDesc, nPosX + nX2 - 26, nPosY + nY + 17, 0xBDA99FFF );
			}
#else
			{			
				pDrawPort->PutTextExRX( m_strDesc, nPosX + nX2, nPosY + nY + 17, 0xBDA99FFF );
			}
#endif
		}
		else
		{
			CAction	&rSelAction = _pNetwork->GetActionData(m_btnSelectedSkill[iRow].GetActionIndex());

			m_strDesc.PrintF( "%s", rSelAction.GetName() );

			m_strDesc.PrintF( "%s", rSelAction.GetName() );
			pDrawPort->PutTextExCX( m_strDesc, nPosX + nX, nPosY + nY + 6,
													0xFFC672FF );
		}
	}
	
	pDrawPort->EndTextEx();

	pDrawPort->FlushRenderingQueue();

	// 버튼 렌더링
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	nX = SKILLINFO_NEW_SLOT_SX; 
	nY = SKILLINFO_NEW_SLOT_SY;

	iRowS = m_sbScrollBar.GetScrollPos() * 2;		
	iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++ )
	{
		if( (iRow - iRowS) % 2 != 0)
		{
			nX = SKILLINFO_NEW_SLOT_SX + SKILLINFO_NEW_SLOT_OFFSETX;
		}
		else
		{
			nX = SKILLINFO_NEW_SLOT_SX;
			if(iRow != iRowS)
				nY += SKILLINFO_NEW_SLOT_OFFSETY;
		}
		if( m_btnSelectedSkill[iRow].IsEmpty() )		
			break;
		
		m_btnSelectedSkill[iRow].SetPos( nX, nY );

		if(m_pSelectedSkillSatisfied[iRow] == ALL_SATISFIED)
			m_btnSelectedSkill[iRow].Render();
		else
		{
			m_btnSelectedSkill[iRow].Render();
		}

	}

	if(m_nCurrentSkillType < BTN_ACTION_NORMAL) // 스킬 버튼일경우
	{
		pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );
	}
	else
	{
		pDrawPort->FlushBtnRenderingQueue( UBET_ACTION );
	}

	iRowS = m_sbScrollBar.GetScrollPos() * 2;
	iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;
	if( m_nSelectedSkillID >= 0 && iRowS <= m_nSelectedSkillID && m_nSelectedSkillID < iRowE )
	{
		pDrawPort->InitTextureData( m_ptdButtonTexture );

		nX = m_btnSelectedSkill[m_nSelectedSkillID].GetAbsPosX();
		nY = m_btnSelectedSkill[m_nSelectedSkillID].GetAbsPosY();

		pDrawPort->AddTexture( nX - 1, nY - 1, nX + 12, nY + 12,
											m_rtSelOutlineTopL.U0, m_rtSelOutlineTopL.V0, m_rtSelOutlineTopL.U1, m_rtSelOutlineTopL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY - 1, nX + 195, nY + 12,
											m_rtSelOutlineTopM.U0, m_rtSelOutlineTopM.V0, m_rtSelOutlineTopM.U1, m_rtSelOutlineTopM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 195, nY - 1, nX + 208, nY + 12,
											m_rtSelOutlineTopR.U0, m_rtSelOutlineTopR.V0, m_rtSelOutlineTopR.U1, m_rtSelOutlineTopR.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( nX - 1, nY + 12, nX + 12, nY + 20,
											m_rtSelOutlineMiddleL.U0, m_rtSelOutlineMiddleL.V0, m_rtSelOutlineMiddleL.U1, m_rtSelOutlineMiddleL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY + 12, nX + 195, nY + 20,
											m_rtSelOutlineMiddleM.U0, m_rtSelOutlineMiddleM.V0, m_rtSelOutlineMiddleM.U1, m_rtSelOutlineMiddleM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 195, nY + 12, nX + 208, nY + 20,
											m_rtSelOutlineMiddleR.U0, m_rtSelOutlineMiddleR.V0, m_rtSelOutlineMiddleR.U1, m_rtSelOutlineMiddleR.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( nX - 1, nY + 20, nX + 11, nY + 33,
											m_rtSelOutlineBottomL.U0, m_rtSelOutlineBottomL.V0, m_rtSelOutlineBottomL.U1, m_rtSelOutlineBottomL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY + 20, nX + 195, nY + 33,
											m_rtSelOutlineBottomM.U0, m_rtSelOutlineBottomM.V0, m_rtSelOutlineBottomM.U1, m_rtSelOutlineBottomM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 195, nY + 20, nX + 208, nY + 33,
											m_rtSelOutlineBottomR.U0, m_rtSelOutlineBottomR.V0, m_rtSelOutlineBottomR.U1, m_rtSelOutlineBottomR.V1,
											0xFFFFFFFF );
	}

	pDrawPort->FlushRenderingQueue();


	
	pDrawPort->EndTextEx();



	pDrawPort->FlushRenderingQueue();


	int nCount= 2;
	if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_SKILL)
	{
		nCount = 3;//3;
	}

	nX = m_rcSkillActionTab.Left + m_nPosX;
	nY = m_rcSkillActionTab.Top + m_nPosY;

	nWidth = m_rcSkillActionTab.GetWidth() / 4;
	nHeight = m_rcSkillActionTab.GetHeight();
	
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	int		i;
	for( i = 0; i<nCount; i++)
	{
		if(i == m_nSkillActionUpperTab[m_ucipCurrentPage-1]) // 지금 표시되는탭
		{
			pDrawPort->AddTexture(nX, nY, nX + 15, nY + nHeight, 
												m_rtSelectedStatusTab.U0, m_rtSelectedStatusTab.V0, 
												m_rtSelectedStatusTab.U1, m_rtSelectedStatusTab.V1, 
												0xFFFFFFFF);	
			pDrawPort->AddTexture(nX + 15, nY, nX + nWidth - 15, nY + nHeight, 
												m_rtSelectedStatusTabMiddle.U0, m_rtSelectedStatusTabMiddle.V0, 
												m_rtSelectedStatusTabMiddle.U1, m_rtSelectedStatusTabMiddle.V1, 
												0xFFFFFFFF);
			pDrawPort->AddTexture(nX + nWidth - 15, nY, nX + nWidth, nY + nHeight, 
												m_rtSelectedStatusTabRight.U0, m_rtSelectedStatusTabRight.V0, 
												m_rtSelectedStatusTabRight.U1, m_rtSelectedStatusTabRight.V1, 
												0xFFFFFFFF);
		}
		else
		{
			pDrawPort->AddTexture(nX, nY, nX + 15, nY + nHeight, 
												m_rtUnSelectedStatusTab.U0, m_rtUnSelectedStatusTab.V0, 
												m_rtUnSelectedStatusTab.U1, m_rtUnSelectedStatusTab.V1, 
												0xFFFFFFFF);	
			pDrawPort->AddTexture(nX + 15, nY, nX + nWidth - 15, nY + nHeight, 
												m_rtUnSelectedStatusTabMiddle.U0, m_rtUnSelectedStatusTabMiddle.V0, 
												m_rtUnSelectedStatusTabMiddle.U1, m_rtUnSelectedStatusTabMiddle.V1, 
												0xFFFFFFFF);
			pDrawPort->AddTexture(nX + nWidth - 15, nY, nX + nWidth, nY + nHeight, 
												m_rtUnSelectedStatusTabRight.U0, m_rtUnSelectedStatusTabRight.V0, 
												m_rtUnSelectedStatusTabRight.U1, m_rtUnSelectedStatusTabRight.V1, 
												0xFFFFFFFF);
		}
		nX += nWidth + 2;
	}

	pDrawPort->FlushRenderingQueue();

	nX = m_rcSkillActionTab.Left + m_nPosX + (m_rcSkillActionTab.GetWidth() / 8);
	nWidth = m_rcSkillActionTab.GetWidth() / 4;
	nY = m_rcSkillActionTab.Top + m_nPosY + 5;

	// 수정 필요
	CTString strTabDesc[3][4] = { {_S(4288, "아이템"),_S(93, "특수"),_S(4490, "증표")},
								  {_S(94, "액션"),_S(96, "소셜"),CTString(""),CTString("")},
								  {_S(97, "파티"),_S(98, "길드"),CTString(""),CTString("")} };

	for( i = 0; i < 4; i++)
	{
		pDrawPort->PutTextExCX( strTabDesc[m_ucipCurrentPage-1][i], nX, nY, 0xDED9A0FF );
		nX += nWidth;
	}

	pDrawPort->EndTextEx();
}

void CUICharacterInfo::RenderNewCharacterInfoStatus()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int nNewPosX, nNewPosY;

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	m_btnGPStrength.Render();
	m_btnGPDexterity.Render();
	m_btnGPIntelligence.Render();
	m_btnGPConstitution.Render();

	for(int i=0; i<STATUS_TAB_MAX; i++)
	{
		nNewPosX = m_nPosX + m_rcStatusTab[i].Left;
		nNewPosY = m_nPosY + m_rcStatusTab[i].Top;

		if(m_bStatusTabOpen[i] == 0)
		{
			pDrawPort->AddTexture(nNewPosX, nNewPosY, nNewPosX + 15, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTab.U0, m_rtSelectedStatusTab.V0, m_rtSelectedStatusTab.U1, m_rtSelectedStatusTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + 15, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 14, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTabMiddle.U0, m_rtSelectedStatusTabMiddle.V0, m_rtSelectedStatusTabMiddle.U1, m_rtSelectedStatusTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 14, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 1, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTabRight.U0, m_rtSelectedStatusTabRight.V0, m_rtSelectedStatusTabRight.U1, m_rtSelectedStatusTabRight.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nNewPosX + 11, nNewPosY + 8, nNewPosX + 21, nNewPosY + 19,
										   m_rtSelectedStatusTabDeco.U0, m_rtSelectedStatusTabDeco.V0, m_rtSelectedStatusTabDeco.U1, m_rtSelectedStatusTabDeco.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 22, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 11, nNewPosY + 19,
										   m_rtSelectedStatusTabDeco.U0, m_rtSelectedStatusTabDeco.V0, m_rtSelectedStatusTabDeco.U1, m_rtSelectedStatusTabDeco.V1, 0xFFFFFFFF);


			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 1, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 16, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTab.U0, m_rtUnSelectedStatusTab.V0, m_rtUnSelectedStatusTab.U1, m_rtUnSelectedStatusTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 16, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth() - 15, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTabMiddle.U0, m_rtUnSelectedStatusTabMiddle.V0, m_rtUnSelectedStatusTabMiddle.U1, m_rtUnSelectedStatusTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth() - 15, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth(), nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTabRight.U0, m_rtUnSelectedStatusTabRight.V0, m_rtUnSelectedStatusTabRight.U1, m_rtUnSelectedStatusTabRight.V1, 0xFFFFFFFF);			
			
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 12, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 22, nNewPosY + 19,
										   m_rtUnSelectedStatusTabDeco.U0, m_rtUnSelectedStatusTabDeco.V0, m_rtUnSelectedStatusTabDeco.U1, m_rtUnSelectedStatusTabDeco.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth() - 23, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth() - 11, nNewPosY + 19,
										   m_rtUnSelectedStatusTabDeco.U0, m_rtUnSelectedStatusTabDeco.V0, m_rtUnSelectedStatusTabDeco.U1, m_rtUnSelectedStatusTabDeco.V1, 0xFFFFFFFF);
		}
		else
		{
			pDrawPort->AddTexture(nNewPosX, nNewPosY, nNewPosX + 15, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTab.U0, m_rtUnSelectedStatusTab.V0, m_rtUnSelectedStatusTab.U1, m_rtUnSelectedStatusTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + 15, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 14, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTabMiddle.U0, m_rtUnSelectedStatusTabMiddle.V0, m_rtUnSelectedStatusTabMiddle.U1, m_rtUnSelectedStatusTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 14, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 1, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtUnSelectedStatusTabRight.U0, m_rtUnSelectedStatusTabRight.V0, m_rtUnSelectedStatusTabRight.U1, m_rtUnSelectedStatusTabRight.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nNewPosX + 11, nNewPosY + 8, nNewPosX + 21, nNewPosY + 19,
										   m_rtUnSelectedStatusTabDeco.U0, m_rtUnSelectedStatusTabDeco.V0, m_rtUnSelectedStatusTabDeco.U1, m_rtUnSelectedStatusTabDeco.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 22, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth()/2 - 11, nNewPosY + 19,
										   m_rtUnSelectedStatusTabDeco.U0, m_rtUnSelectedStatusTabDeco.V0, m_rtUnSelectedStatusTabDeco.U1, m_rtUnSelectedStatusTabDeco.V1, 0xFFFFFFFF);


			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 1, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 16, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTab.U0, m_rtSelectedStatusTab.V0, m_rtSelectedStatusTab.U1, m_rtSelectedStatusTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 16, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth() - 15, nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTabMiddle.U0, m_rtSelectedStatusTabMiddle.V0, m_rtSelectedStatusTabMiddle.U1, m_rtSelectedStatusTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth() - 15, nNewPosY, nNewPosX + m_rcStatusTab[i].GetWidth(), nNewPosY + m_rcStatusTab[i].GetHeight(),
										   m_rtSelectedStatusTabRight.U0, m_rtSelectedStatusTabRight.V0, m_rtSelectedStatusTabRight.U1, m_rtSelectedStatusTabRight.V1, 0xFFFFFFFF);			
			
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 12, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth()/2 + 22, nNewPosY + 19,
										   m_rtSelectedStatusTabDeco.U0, m_rtSelectedStatusTabDeco.V0, m_rtSelectedStatusTabDeco.U1, m_rtSelectedStatusTabDeco.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX + m_rcStatusTab[i].GetWidth() - 23, nNewPosY + 8, nNewPosX + m_rcStatusTab[i].GetWidth() - 11, nNewPosY + 19,
										   m_rtSelectedStatusTabDeco.U0, m_rtSelectedStatusTabDeco.V0, m_rtSelectedStatusTabDeco.U1, m_rtSelectedStatusTabDeco.V1, 0xFFFFFFFF);
		}

	}


	pDrawPort->FlushRenderingQueue();

	int	nX1 = m_nPosX + m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].Left + (m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].GetWidth() / 4);
	int	nX2;
	int	nY = m_nPosY + m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].Top + 5;
	int nStringHGap;

	pDrawPort->PutTextExCX( _S(4174, "PK 정보"), nX1, nY, 0xDED9A0FF );
	nX1 += m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].GetWidth() / 2;
	// 속성 시스템 추가로 인한 수정 [1/14/2013 Ranma] 속성친화도 -> 속성 정보
	pDrawPort->PutTextExCX( _S(5844, "속성 정보"), nX1, nY, 0xDED9A0FF );

	nX1 = m_nPosX + m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].Left + (m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].GetWidth() / 4);
	nY = m_nPosY + m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].Top + 5;

	pDrawPort->PutTextExCX( CTString("01"), nX1, nY, 0xDED9A0FF );
	nX1 += m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].GetWidth() / 2;
	pDrawPort->PutTextExCX( CTString("02"), nX1, nY, 0xDED9A0FF );



	nX1 = m_nPosX + 36;
	nX2 = m_nPosX + 223;
	nY = m_nPosY + 48;
	nStringHGap = 20;
	pDrawPort->PutTextEx( _S( 71, "이름" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( _pNetwork->MyCharacterInfo.name, nX2, nY, 0xFFFFFFFF );
	nY += nStringHGap;
	pDrawPort->PutTextEx( _S( 72, "클래스" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( CJobInfo::getSingleton()->GetName(_pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2),
										nX2, nY, 0xFFFFFFFF );
	nY += nStringHGap;
	pDrawPort->PutTextEx( _S( 73, "레벨" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( pUIManager->GetPlayerInfo()->GetStringOfLevel(),
										nX2, nY, 0xFF0000FF );
	nY = m_nPosY + 111;
	pDrawPort->PutTextEx( _S( 75, "길드" ), nX1, nY, 0xDED9A0FF );
	if(_pNetwork->MyCharacterInfo.lGuildIndex == -1)
	pDrawPort->PutTextExRX( _S( 76, "길드 없음" ), nX2, nY, 0xD6A6D6FF );
	else
	{
		CTString strGuildName = _pNetwork->MyCharacterInfo.strGuildName;
		pDrawPort->PutTextExRX( strGuildName, nX2, nY, 0xD6A6D6FF );
	}
	nY += nStringHGap;
	pDrawPort->PutTextEx( _S( 4176, "직위"), nX1, nY, 0xDED9A0FF );
	if( _pNetwork->MyCharacterInfo.guildPosName.Length()  == 0)
	{
		pDrawPort->PutTextExRX( _S( 4177, "직위 없음"), nX2, nY, 0xABC98AFF );
	}
	else
	{
		pDrawPort->PutTextExRX( _pNetwork->MyCharacterInfo.guildPosName, nX2, nY, 0xABC98AFF );
	}
	
	nY += nStringHGap;
#ifdef ADD_SUBJOB
	pDrawPort->PutTextEx( _S( 5049, "전문직업" ), nX1, nY, 0xDED9A0FF );
	CTString strSubJobName;
	if( _pNetwork->MyCharacterInfo.slSubJob > 0 )
	{
		strSubJobName = pUIManager->GetSubJobName(_pNetwork->MyCharacterInfo.slSubJob);
	}
	else
	{
		strSubJobName = _S(3865, "없음");
	}

	pDrawPort->PutTextExRX( strSubJobName, nX2, nY, 0x3fb9f2FF );
#else
	pDrawPort->PutTextEx( _S( 77, "동맹" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( _S( 78, "동맹 없음" ), nX2, nY, 0xABC98AFF );
#endif

	nY += nStringHGap + 3;
//	nX2 = m_nPosX + 205;
	pDrawPort->PutTextEx( _S( 90, "숙련도" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strSP, nX2, nY, 0xFFFFFFFF );

	nY += nStringHGap;
	pDrawPort->PutTextEx( _S( 1083, "명성치" ), nX1, nY, 0xDED9A0FF );		
	pDrawPort->PutTextExRX( m_strFame, nX2, nY, 0xFFC000FF );

	nY += nStringHGap + 5;
	pDrawPort->PutTextEx( _S( 89, "경험치" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strExp, nX2 + 34, nY, 0xFFFFFFFF );


	COLOR strColor;
	int added = 0;

	nX1 = m_nPosX + 280;
	nX2 = m_nPosX + 467;
	nY = m_nPosY + 46;
	
	strColor = GetStrColor(_pNetwork->MyCharacterInfo.opt_str);
	pDrawPort->PutTextEx( _S( 80, "힘" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strStrength, nX2, nY, strColor );
	
	nY += nStringHGap + 1;
	strColor = GetStrColor(_pNetwork->MyCharacterInfo.opt_dex);
	pDrawPort->PutTextEx( _S( 81, "민첩" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strDexterity, nX2, nY, strColor );
	
	nY += nStringHGap + 1;
	strColor = GetStrColor(_pNetwork->MyCharacterInfo.opt_intel);
	pDrawPort->PutTextEx( _S( 82, "지혜" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strIntelligence, nX2, nY, strColor );
	
	nY += nStringHGap + 1;
	strColor = GetStrColor(_pNetwork->MyCharacterInfo.opt_con);
	pDrawPort->PutTextEx( _S( 83, "체질" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strConstitution, nX2, nY, strColor );

	nY += nStringHGap + 5;
	pDrawPort->PutTextEx( _S( 84, "성장 포인트" ), nX1, nY, 0xDED9A0FF );
	pDrawPort->PutTextExRX( m_strStatPoint, nX2 + 29, nY, 0xFF5426FF );


///텝으로 갈리는 부분/////////////////////////////////////////////////////
	nX1 = m_nPosX + 280;
	nY = m_nPosY + 176;
	nX2 = m_nPosX + 490;
	nStringHGap = 18;
	
	if(m_bStatusTabOpen[STATUS_TAB_PHYSICAL_N_MAGICAL] == 0)
	{

		strColor = GetStrColor(_pNetwork->MyCharacterInfo.addedAttack);
		pDrawPort->PutTextEx( _S( 4178, "물리공격력" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strAttack, nX2, nY, strColor );
		
		nY += nStringHGap;
		strColor = GetStrColor(_pNetwork->MyCharacterInfo.addedDefense);
		pDrawPort->PutTextEx( _S( 4179, "물리방어력" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strDefense, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.hitRate - _pNetwork->MyCharacterInfo.baseHitRate;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4180, "물리명중도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strHitRate, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.dodgeRate - _pNetwork->MyCharacterInfo.baseDodgeRate;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4181, "물리회피도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strDodgeRate, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.critical - _pNetwork->MyCharacterInfo.baseCritical;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4182, "크리티컬" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strCritical, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = ( _pNetwork->MyCharacterInfo.runspeed * 10 ) - ( _pNetwork->MyCharacterInfo.baseRunSpeed * 10 );
		strColor = GetStrColor(added);
//		strColor = GetStrColor(0);
		pDrawPort->PutTextEx( _S( 4183, "이동속도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strSpeed, nX2, nY, strColor );
	}
	else
	{
		strColor = GetStrColor(_pNetwork->MyCharacterInfo.addedMagic);
		pDrawPort->PutTextEx( _S( 86, "마법공격력" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strMagicAttack, nX2, nY, strColor );
		
		nY += nStringHGap;
		strColor = GetStrColor(_pNetwork->MyCharacterInfo.addedResist);
		pDrawPort->PutTextEx( _S( 88, "마법방어력" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strMagicDefense, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.magicHitRate - _pNetwork->MyCharacterInfo.baseMagicHitRate;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4184, "마법명중도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strMagicHitRate, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.magicDodgeRate - _pNetwork->MyCharacterInfo.baseMagicDodgeRate;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4185, "마법회피도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strMagicDodgeRate, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.deadly - _pNetwork->MyCharacterInfo.baseDeadly;
		strColor = GetStrColor(added);
		pDrawPort->PutTextEx( _S( 4186, "데들리" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strDeadly, nX2, nY, strColor );
		
		nY += nStringHGap;
		added = _pNetwork->MyCharacterInfo.attackspeed - _pNetwork->MyCharacterInfo.baseAttackSpeed;
		strColor = GetStrColor(added);
//		strColor = GetStrColor(0);
		pDrawPort->PutTextEx( _S( 4187, "공격속도" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strAttackSpeed, nX2, nY, strColor );
	}

	nX1 = m_nPosX + 280;
	nX2 = m_nPosX + 489;
	nY += nStringHGap + 3;

#if defined(G_RUSSIA)
	{
		pDrawPort->PutTextEx( _S( 4411, "HP" ), nX1, nY, 0xDED9A0FF );
	}
#else
	{
		pDrawPort->PutTextEx( "HP", nX1, nY, 0xDED9A0FF );
	}
#endif

	CTString strTemp;

	added = _pNetwork->MyCharacterInfo.maxHP - _pNetwork->MyCharacterInfo.baseHP;
//	strColor = GetStrColor(added);
	strColor = GetStrColor(0);
	strTemp.PrintF("%d/%d", _pNetwork->MyCharacterInfo.hp, _pNetwork->MyCharacterInfo.maxHP);
	pDrawPort->PutTextExRX( strTemp, nX2, nY, strColor );

	nY += nStringHGap + 2;
	added = _pNetwork->MyCharacterInfo.maxMP - _pNetwork->MyCharacterInfo.baseMP;
//	strColor = GetStrColor(added);
	strColor = GetStrColor(0);

#if defined(G_RUSSIA)
	{
		pDrawPort->PutTextEx( _S( 4412, "MP" ), nX1, nY, 0xDED9A0FF );
	}
#else
	{
		pDrawPort->PutTextEx( "MP", nX1, nY, 0xDED9A0FF );
	}
#endif

	strTemp.PrintF("%d/%d", _pNetwork->MyCharacterInfo.mp, _pNetwork->MyCharacterInfo.maxMP);
	pDrawPort->PutTextExRX( strTemp, nX2, nY, strColor );

///텝으로 갈리는 부분/////////////////////////////////////////////////////

	if(m_bStatusTabOpen[STATUS_TAB_PKINFO_N_ATTRIBUTE] == 0)
	{
		nX1 = m_nPosX + 41;
		nX2 = m_nPosX + 245;
		nY = m_nPosY + 266;
#ifdef NEW_CHAO_SYS
		CTString pkname;
		int		 pkcolor = 0;
		
		if(_pNetwork->MyCharacterInfo.pkpenalty > 0)
		{
			pkname = _S(4054,"헌터");
			pkcolor = 0x0070C0FF;

		}
		else if(_pNetwork->MyCharacterInfo.pkpenalty < 0)
		{
			pkname = _S(5582,"카오");
			pkcolor = 0xFF0000FF;
		}
		else
		{
			pkname = _S(92,"일반");
			pkcolor = 0xFFFFFFFF;
		}
		pDrawPort->PutTextEx( _S( 4188, "성향등급" ), nX1, nY, 0xDED9A0FF );
// 아프론만 성향등급이 있다.
//		pDrawPort->PutTextExRX( _ClassName[_pNetwork->MyCharacterInfo.pktitle + 5], nX2, nY, m_ClassNameColor[_pNetwork->MyCharacterInfo.pktitle + 5] );
		pDrawPort->PutTextExRX( pkname, nX2, nY, pkcolor );

		nY += nStringHGap + 2;
		pDrawPort->PutTextEx( _S( 502, "성향 수치" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strPenalty, nX2, nY, strColor );

		nY += nStringHGap + 2;
		pDrawPort->PutTextEx( _S( 503, "누적 수치" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strPenaltyAcc, nX2, nY, 0xFFC000FF );
#else
		pDrawPort->PutTextEx( _S( 4188, "성향등급" ), nX1, nY, 0xDED9A0FF );
// 아프론만 성향등급이 있다.
//		pDrawPort->PutTextExRX( _ClassName[_pNetwork->MyCharacterInfo.pktitle + 5], nX2, nY, m_ClassNameColor[_pNetwork->MyCharacterInfo.pktitle + 5] );
		pDrawPort->PutTextExRX( _ClassName[_pNetwork->MyCharacterInfo.pktitle + 5], nX2, nY, 0xFFFFFFFF );

		nY += nStringHGap + 2;
		pDrawPort->PutTextEx( _S( 502, "성향 수치" ), nX1, nY, 0xDED9A0FF );
		strColor = GetStrColor(_pNetwork->MyCharacterInfo.pkpenalty);
		pDrawPort->PutTextExRX( m_strPenalty, nX2, nY, strColor );

		nY += nStringHGap + 2;
		pDrawPort->PutTextEx( _S( 503, "누적 수치" ), nX1, nY, 0xDED9A0FF );
		pDrawPort->PutTextExRX( m_strPenaltyAcc, nX2, nY, 0xFFC000FF );
#endif
	}
	else
	{
		// 속성 시스템 [1/14/2013 Ranma]
		strColor = GetStrColor(0);

		int nTextX1 = m_nPosX + 33 + 105;
		int nTextX2 = m_nPosX + 33 + 219;
		int nTextY1 = m_nPosY + 263 + 32;
		CTString strAttLv, strDefLv;
		// 공격 속성 아이콘 레벨 
		strAttLv.PrintF("%d", _pNetwork->MyCharacterInfo.attrattLv);
		// 속성 시스템 레벨 스트링넣는곳 [1/18/2013 Ranma]
		//pDrawPort->PutTextExRX("Lv", nTextX1 - 20, nTextY1, 0xDED9A0FF);
		pDrawPort->PutTextExRX( strAttLv, nTextX1, nTextY1, 0xDED9A0FF );
		// 방어 속성 아이콘 레벨 
		strDefLv.PrintF("%d", _pNetwork->MyCharacterInfo.attrdefLv);
		//pDrawPort->PutTextExRX( "Lv", nTextX2 - 20, nTextY1 - 1/*이미지가 1픽셀 밀렸음 특단의 조치*/ , 0xDED9A0FF );
		pDrawPort->PutTextExRX( strDefLv, nTextX2, nTextY1 - 1/*이미지가 1픽셀 밀렸음 특단의 조치*/ , 0xDED9A0FF );
	}

	pDrawPort->EndTextEx();

}

void CUICharacterInfo::RenderNewCharacterInfo()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// 배경 윈도우
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + CHARINFO_NEW_WIDTH - 1, m_nPosY + CHARINFO_NEW_HEIGHT - 1,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// 속성 시스템 속성 정보 탭에 그려질 배경 윈도우 출력 부분 [1/14/2013 Ranma]
	if(m_bStatusTabOpen[STATUS_TAB_PKINFO_N_ATTRIBUTE] != 0)
	{
		pDrawPort->AddTexture(m_nPosX + 33, m_nPosY + 263, m_nPosX + 262, m_nPosY + 323,
							   m_rtAttributeInfo.U0, m_rtAttributeInfo.V0, m_rtAttributeInfo.U1, m_rtAttributeInfo.V1, 0xFFFFFFFF);

		int nX1 = m_nPosX + 33/*캐릭터인포창에서 속성창 그릴 부분*/ + 9/*아이콘 그릴 위치*/;
		int nX2 = m_nPosX + 33/*캐릭터인포창에서 속성창 그릴 부분*/ + 123/*2번째 아이콘 그릴 시작 위치*/;
		int nY = m_nPosY + 263 + 10;

		int nAttIndex = _pNetwork->MyCharacterInfo.attratt;
		int nDefIndex = _pNetwork->MyCharacterInfo.attrdef;

		// 공격 속성 아이콘
		pDrawPort->AddTexture(nX1, nY, nX1 + 41, nY + 40,
							m_rtAttributeIconAtt[nAttIndex].U0, m_rtAttributeIconAtt[nAttIndex].V0,
							m_rtAttributeIconAtt[nAttIndex].U1, m_rtAttributeIconAtt[nAttIndex].V1, 0xFFFFFFFF);

		// 방어 속성 아이콘
		pDrawPort->AddTexture(nX2, nY, nX2 + 41, nY + 40,
							m_rtAttributeIconDef[nDefIndex].U0, m_rtAttributeIconDef[nDefIndex].V0,
							m_rtAttributeIconDef[nDefIndex].U1, m_rtAttributeIconDef[nDefIndex].V1, 0xFFFFFFFF);
	}
	
	
	pDrawPort->FlushRenderingQueue();


	pDrawPort->InitTextureData( m_ptdButtonTexture );

	m_btnClose.Render();

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_STATUS)
	{
		RenderNewCharacterInfoStatus();
	}
	else
	{
		RenderNewCharacterInfoSkill();
	}

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	int nNewPosX, nNewPosY;
	int cnt = 1;
	for(int i =0; i<4; i++)
	{
		nNewPosX = m_nPosX + m_rcTabLayer.Left;
		nNewPosY = m_nPosY + m_rcTabLayer.Top + (m_rcTabLayer.GetHeight() * i) + (i * 2);

		if(i == m_ucipCurrentPage)
		{
			pDrawPort->AddTexture(nNewPosX, nNewPosY, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + 15,
											   m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX, nNewPosY + 15, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + m_rcTabLayer.GetHeight() - 15,
											   m_rtSelectedTabMiddle.U0, m_rtSelectedTabMiddle.V0, m_rtSelectedTabMiddle.U1, m_rtSelectedTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX,  nNewPosY + m_rcTabLayer.GetHeight() - 15, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + m_rcTabLayer.GetHeight(),
											   m_rtSelectedTabLow.U0, m_rtSelectedTabLow.V0, m_rtSelectedTabLow.U1, m_rtSelectedTabLow.V1, 0xFFFFFFFF);

			pDrawPort->AddTexture(nNewPosX + 6, nNewPosY+ (m_rcTabLayer.GetHeight()/2) - 6, nNewPosX + 19, nNewPosY+ (m_rcTabLayer.GetHeight()/2) + 7,
											   m_rtTabIcon[cnt - 1].U0, m_rtTabIcon[cnt - 1].V0, m_rtTabIcon[cnt - 1].U1, m_rtTabIcon[cnt - 1].V1, 0xFFFFFFFF);
		}
		else
		{
			pDrawPort->AddTexture(nNewPosX, nNewPosY, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + 15,
											   m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX, nNewPosY + 15, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + m_rcTabLayer.GetHeight() - 15,
											   m_rtUnSelectedTabMiddle.U0, m_rtUnSelectedTabMiddle.V0, m_rtUnSelectedTabMiddle.U1, m_rtUnSelectedTabMiddle.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nNewPosX, nNewPosY+ m_rcTabLayer.GetHeight() - 15, nNewPosX + m_rcTabLayer.GetWidth(), nNewPosY + m_rcTabLayer.GetHeight(),
											   m_rtUnSelectedTabLow.U0, m_rtUnSelectedTabLow.V0, m_rtUnSelectedTabLow.U1, m_rtUnSelectedTabLow.V1, 0xFFFFFFFF);

			pDrawPort->AddTexture(nNewPosX + 6, nNewPosY+ (m_rcTabLayer.GetHeight()/2) - 6, nNewPosX + 19, nNewPosY+ (m_rcTabLayer.GetHeight()/2) + 7,
											   m_rtTabIcon[cnt].U0, m_rtTabIcon[cnt].V0, m_rtTabIcon[cnt].U1, m_rtTabIcon[cnt].V1, 0xFFFFFFFF);
		}

		cnt += 2;
	}

	pDrawPort->FlushRenderingQueue();

	if(m_bCharDescVisible)
	{
		RenderCharInfoDesc();
	}

	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextExCX( m_strTitleName, m_nPosX + (CHARINFO_NEW_WIDTH/2), m_nPosY + 15, 0xDED9A0FF );

	pDrawPort->EndTextEx();

	if(m_nSkillDescRow >= 0)
	{
		m_btnSelectedSkill[m_nSkillDescRow].RenderInfoPopup();
	}
}

// 탭에 따른 버튼 변경
void CUICharacterInfo::SetCurrentSkillInfo(int nSideTab, int nUpperTab)
{
	if(nSideTab == PAGE_CHARINFO_NEW_SKILL)
	{
		switch(nUpperTab)
		{
			case SKILL_ITEM:
				m_nCurrentSkillType = BTN_SKILL_ITEM;
				m_nBtnTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnItemSpecialSkill;
				break;
			case SKILL_SPECIAL:
				m_nCurrentSkillType = BTN_SKILL_SPECIAL;
				m_nBtnTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnSpecialSkill;
				break;
			case SKILL_VOUCHER:
				m_nCurrentSkillType = BTN_SKILL_VOUCHER;
				m_nBtnTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnVoucherSkill;
				break;
		}
		
	}
	else if(nSideTab == PAGE_CHARINFO_NEW_ACTION_SOCIAL)
	{
		switch(nUpperTab)
		{
			case 0:
				m_nCurrentSkillType = BTN_ACTION_NORMAL;
				m_nBtnTotal = ACTION_NORAML_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnNormalAction;
				break;
			case 1:
				m_nCurrentSkillType = BTN_ACTION_SOCIAL;
				m_nBtnTotal = ACTION_SOCIAL_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnSocialAction;
				break;
		}	
	}
	else if(nSideTab == PAGE_CHARINFO_NEW_GUILD_PARTY)
	{
		switch(nUpperTab)
		{
			case 0:
				m_nCurrentSkillType = BTN_ACTION_PARTY;
				m_nBtnTotal = ACTION_PARTY_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnPartyAction;
				break;
			case 1:
				m_nCurrentSkillType = BTN_ACTION_GUILD;
				m_nBtnTotal = ACTION_GUILD_SLOT_ROW_TOTAL;
				m_btnSelectedSkill = m_btnGuildAction;
				break;
		}	
	}

	
	m_nSelectedSkillID = -1;
	m_nSkillActionUpperTab[nSideTab - 1] = nUpperTab;

	if(m_nCurrentSkillType < BTN_ACTION_NORMAL)
	{
		BOOL bSpecial = FALSE;
		if(m_nCurrentSkillType == BTN_SKILL_SPECIAL)
		{
			bSpecial = TRUE;
		}
		for(int i=0; i<m_nBtnTotal; i++)
		{
			m_pSelectedSkillSatisfied[i] = CUIManager::getSingleton()->GetSkillLearn()->IsSatisfiedSkill( m_btnSelectedSkill[i].GetSkillIndex(), m_btnSelectedSkill[i].GetSkillLevel(), bSpecial);
		}
	}

	int nItemTotal = 0;
	for(int i = 0; i<m_nBtnTotal; i++)
	{
		if(m_btnSelectedSkill[i].IsEmpty())
		{
			break;
		}
		nItemTotal++;
	}

	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetCurItemCount( (nItemTotal / 2) + (nItemTotal%2) );
}

void CUICharacterInfo::SetCurrentPageNewCharacterInfo(int nTabNum)
{
	if(m_ucipCurrentPage != nTabNum)
	{
		m_ucipCurrentPage = nTabNum;
	}
	else
	{
		return;
	}


	if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_SKILL)
	{
		m_strTitleName.PrintF("%s", _S(6077, "특수 스킬") );
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[0]);	
	}
	else if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_ACTION_SOCIAL)
	{
		m_strTitleName.PrintF( "%s,%s",_S(94, "액션") ,_S(96, "소셜") );
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[1]);	
	}
	else if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_GUILD_PARTY)
	{
		m_strTitleName.PrintF("%s,%s",_S(97, "파티") ,_S(98, "길드") );
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[2]);	
	}
	else
	{
		m_strTitleName.PrintF("%s", _S(69, "캐릭터 정보") );
	}



}

WMSG_RESULT CUICharacterInfo::MouseMessageNewCharacterInfo(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			if( !(IsInside( nX, nY )) )
				return WMSG_FAIL;
				
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			m_btnClose.MouseMessage( pMsg );

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			m_bCharDescVisible = FALSE;

			if( IsInsideRect( nX, nY, m_rcCharInfoTab ) )
			{
				int nTabID = ( nY - m_nPosY - m_rcCharInfoTab.Top ) / CHARINFO_NEW_TAB_HEIGHT;
				if(nTabID >= PAGE_CHARINFO_NEW_STATUS && nTabID <= PAGE_CHARINFO_NEW_GUILD_PARTY)
				{
					m_bCharDescVisible = TRUE;
					ShowCharInfoDesc(nTabID);
				}
			}

			// Move inventory
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}

			if(m_ucipCurrentPage != PAGE_CHARINFO_NEW_STATUS)
			{
				if(IsInsideRect(nX, nY, m_rcInfoRegion))
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					if( pUIManager->GetHoldBtn().IsEmpty() && bLButtonDownInBtn && ( pMsg->wParam & MK_LBUTTON ) &&
						( ndX != 0 || ndY != 0 ) )
					{
						// Active skill
						if(m_nCurrentSkillType != BTN_SKILL_PASSIVE && 
							m_nCurrentSkillType != BTN_SKILL_SPECIAL &&
							m_nCurrentSkillType != BTN_SKILL_VOUCHER)
						{
							if( m_nSelectedSkillID >= 0 )
							{
								int	nSelRow = m_nSelectedSkillID;						

								pUIManager->SetHoldBtn( m_btnSelectedSkill[nSelRow]);
								int	nOffset = BTN_SIZE / 2;
								pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

								m_btnSelectedSkill[nSelRow].SetBtnState( UBES_IDLE );
							}
						}

						bLButtonDownInBtn = FALSE;
					}

					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					int	iRow;
					int	iRowS = m_sbScrollBar.GetScrollPos() * 2;
					int	iRowE;
					iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;
					m_nSkillDescRow = -1;
				
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnSelectedSkill[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSkillDescRow = iRow;
							break;
						}
					}

					return WMSG_SUCCESS;
				}
			}	
			else if( bLButtonDownInBtn == FALSE ) //if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_STATUS )
			{
				// Strength growth point button
				if( m_btnGPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Dexterity growth point button
				else if( m_btnGPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Intelligence growth point button
				else if( m_btnGPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Constitution growth point button
				else if( m_btnGPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}

			//return WMSG_SUCCESS;

		}
		break;
		
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY )/* && !m_bLockCharacterInfo*/)
			{
				nOldX = nX;		nOldY = nY;
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcCharInfoTab ) )
				{
					int nTabID = ( nY - m_nPosY - m_rcCharInfoTab.Top ) / CHARINFO_NEW_TAB_HEIGHT;
					if(nTabID >= PAGE_CHARINFO_NEW_STATUS && nTabID <= PAGE_CHARINFO_NEW_GUILD_PARTY)
					{
						SetCurrentPageNewCharacterInfo(nTabID);
					}
				}


				if(IsInsideRect(nX, nY, m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE]))
				{
					int nTabID = ( nX - m_nPosX - m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].Left ) / CHARINFO_NEW_STATUS_TAB_WIDTH;
					m_bStatusTabOpen[STATUS_TAB_PKINFO_N_ATTRIBUTE] = nTabID;
				}
				else if(IsInsideRect(nX, nY, m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL]))
				{
					int nTabID = ( nX - m_nPosX - m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].Left ) / CHARINFO_NEW_STATUS_TAB_WIDTH;
					m_bStatusTabOpen[STATUS_TAB_PHYSICAL_N_MAGICAL] = nTabID;
				}
				
				if(m_ucipCurrentPage != PAGE_CHARINFO_NEW_STATUS)
				{
					if( IsInsideRect(nX, nY, m_rcSkillActionTab) )
					{
						int nTabID =  (nX - m_nPosX - m_rcSkillActionTab.Left) / CHARINFO_NEW_UPPERTAB_WIDTH;
						if(nTabID == 2)
						{
							if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_SKILL)
								SetCurrentSkillInfo(m_ucipCurrentPage, nTabID);
						}
						else
						{
							SetCurrentSkillInfo(m_ucipCurrentPage, nTabID);
						}

						
					}
					
					if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					if(IsInsideRect(nX, nY, m_rcInfoRegion))
					{
						int	nOldSelSkillID = m_nSelectedSkillID;
						m_nSelectedSkillID = -1;

						int	iRow;
						int	iRowS = m_sbScrollBar.GetScrollPos() * 2;
						int	iRowE;
						iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;
						int	nWhichRow = -1;

						int nPosX = 0;
						int nPosY = 0;
						int nWidth = m_rcButtonArea.GetWidth();
						int nHeight = m_rcButtonArea.GetHeight();

						CUIManager* pUIManager = CUIManager::getSingleton();

						for( iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnSelectedSkill[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelectedSkillID = iRow;

								bLButtonDownInBtn = TRUE;
								pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
								return WMSG_SUCCESS;
							}

							if(!m_btnSelectedSkill[iRow].IsEmpty())
							{
								if(m_nSelectedSkillID == -1)
								{
									nPosX = m_btnSelectedSkill[iRow].GetAbsPosX() - GetAbsPosX();
									nPosY = m_btnSelectedSkill[iRow].GetAbsPosY() - GetAbsPosY();
									m_rcButtonArea.SetRect(nPosX, nPosY, nPosX + nWidth, nPosY + nHeight);
									if(IsInsideRect(nX, nY, m_rcButtonArea))
									{
										if(m_nSelectedSkillID != iRow)
										{
											m_nSelectedSkillID = iRow;
										}
										pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
										return WMSG_SUCCESS;
									}
								}
							}
						}

						pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
						return WMSG_SUCCESS;
//						return m_spSkillInfo.MouseMessage(pMsg);
					}
				}			
				else //if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_STATUS )
				{
					if( !m_bLockStatPoint && bLButtonDownInBtn == FALSE)
					{
						bLButtonDownInBtn = TRUE;
						// Strength growth point button
						if( m_btnGPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SendUseStatPoint( MSG_STATPOINT_USE_STR );
						}
						// Dexterity growth point button
						else if( m_btnGPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SendUseStatPoint( MSG_STATPOINT_USE_DEX );
						}
						// Intelligence growth point button
						else if( m_btnGPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SendUseStatPoint( MSG_STATPOINT_USE_INT );
						}
						// Contitution growth point button
						else if( m_btnGPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SendUseStatPoint( MSG_STATPOINT_USE_CON );
						}
					}
				}
				
				CUIManager::getSingleton()->RearrangeOrder( UI_CHARACTERINFO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( pUIManager->GetHoldBtn().IsEmpty() )
				{
					// Title bar
					bTitleBarClick = FALSE;
					
					if( !IsFocused() )
						return WMSG_FAIL;

					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							ToggleVisible();
						}

						return WMSG_SUCCESS;
					}
					if(m_ucipCurrentPage != PAGE_CHARINFO_NEW_STATUS)
					{

						if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
							return WMSG_SUCCESS;

						if(IsInsideRect(nX, nY, m_rcInfoRegion))
						{
							int	iRow;
							int	iRowS = m_sbScrollBar.GetScrollPos() * 2;
							int	iRowE;
							

							iRowE = iRowS + SKILLINFO_NEW_SLOT_ROW;

							for( iRow = iRowS; iRow < iRowE; iRow++ )
							{
								if( ( wmsgResult =  m_btnSelectedSkill[iRow].MouseMessage( pMsg ) ) != WMSG_FAIL )
								{
									if( wmsgResult == WMSG_COMMAND )
									{
										if( m_nCurrentSkillType == BTN_SKILL_ITEM )
										{
											int nSkillIndex = m_btnSelectedSkill[iRow].GetSkillIndex();
											CSkill &skill = _pNetwork->GetSkillData( nSkillIndex );

											if ( skill.GetType() == CSkill::ST_MELEE ||
											skill.GetType() == CSkill::ST_RANGE ||
											skill.GetType() == CSkill::ST_MAGIC ||
											skill.GetType() == CSkill::ST_SUMMON_TOTEM_SKILL )
											{
												if( !m_btnSelectedSkill[iRow].GetSkillDelay() )
													pUIManager->GetCharacterInfo()->UseSkill( nSkillIndex );
											}											
										}
										else if (m_nCurrentSkillType > BTN_SKILL_SPECIAL)
										{
											pUIManager->GetCharacterInfo()->UseAction( m_btnSelectedSkill[iRow].GetActionIndex() );
										}
									}
								}
							}
						}
//						pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
						return WMSG_SUCCESS;
					}
					else// if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_STATUS )
					{
						// Strength growth point button
						if( ( wmsgResult = m_btnGPStrength.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
						// Dexterity growth point button
						else if( ( wmsgResult = m_btnGPDexterity.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
						// Intelligence growth point button
						else if( ( wmsgResult = m_btnGPIntelligence.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
						// Constitution growth point button
						else if( ( wmsgResult = m_btnGPConstitution.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
				}
				else
				{
					if( IsInside( nX, nY ) )
					{
						// Reset holding button
						pUIManager->ResetHoldBtn();

						return WMSG_SUCCESS;
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if(m_ucipCurrentPage != PAGE_CHARINFO_NEW_STATUS)
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
				}
				else if( !m_bLockStatPoint )
				{
					// Strength growth point button
					if( m_btnGPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SendUseStatPoint( MSG_STATPOINT_USE_STR );
						return WMSG_SUCCESS;
					}
					else if( m_btnGPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SendUseStatPoint( MSG_STATPOINT_USE_DEX );
						return WMSG_SUCCESS;
					}
					else if( m_btnGPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SendUseStatPoint( MSG_STATPOINT_USE_INT );
						return WMSG_SUCCESS;
					}
					else if( m_btnGPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SendUseStatPoint( MSG_STATPOINT_USE_CON );
						return WMSG_SUCCESS;
					}
				}
				return WMSG_SUCCESS;
			}
		}

		case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if(m_ucipCurrentPage > PAGE_CHARINFO_NEW_STATUS)
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				return WMSG_SUCCESS;
			}
		}
		break;
	}
	
	return WMSG_FAIL;
}



// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CreateNewCharacterInfo(pParentWnd, nX, nY, CHARINFO_NEW_WIDTH, CHARINFO_NEW_HEIGHT);
}

// ----------------------------------------------------------------------------
// Name : RegisterActions()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RegisterActions()
{
	// Clear action buttons
	int		iAct;
	for( iAct = 0; iAct < ACTION_NORAML_SLOT_ROW_TOTAL; iAct++ )
		m_btnNormalAction[iAct].InitBtn();
	for( iAct = 0; iAct < ACTION_SOCIAL_SLOT_ROW_TOTAL; iAct++ )
		m_btnSocialAction[iAct].InitBtn();
	for( iAct = 0; iAct < ACTION_PARTY_SLOT_ROW_TOTAL; iAct++ )
		m_btnPartyAction[iAct].InitBtn();
	for( iAct = 0; iAct < ACTION_GUILD_SLOT_ROW_TOTAL; iAct++ )
		m_btnGuildAction[iAct].InitBtn();

	int	ctNormalAction = 0;
	int	ctSocialAction = 0;
	int	ctPartyAction = 0;
	int	ctGuildAction = 0;

	// Register action data
	for( int iAction = 1; iAction <= _pNetwork->wo_iNumOfAction; iAction++ )
	{
		CAction	&rActionData = _pNetwork->GetActionData(iAction);

		// If job is different
		if( !( rActionData.GetJob() & ( 1 << _pNetwork->MyCharacterInfo.job ) ) )
			continue;

		if ( _pNetwork->m_iServerType == SERVER_TYPE_HARDCORE &&
			rActionData.GetType() == ACTION_NORMAL &&
			rActionData.GetIndex() == 30 /*후견인 시스템*/)
			continue;

		switch( rActionData.GetType() )
		{
		case ACTION_WILDPET:
		case ACTION_PET:
		case ACTION_NORMAL:
			m_btnNormalAction[ctNormalAction++].SetActionInfo( rActionData.GetIndex() );
			break;

		case ACTION_SOCIAL:		
			m_btnSocialAction[ctSocialAction++].SetActionInfo( rActionData.GetIndex() );
			break;

		case ACTION_PARTY:
			m_btnPartyAction[ctPartyAction++].SetActionInfo( rActionData.GetIndex() );
			break;

		case ACTION_GUILD:
			m_btnGuildAction[ctGuildAction++].SetActionInfo( rActionData.GetIndex() );
			break;
		}
	}

	// Reset scroll bars
	m_sbNormalActionScrollBar.SetCurItemCount( ctNormalAction );
	m_sbSocialActionScrollBar.SetCurItemCount( ctSocialAction );
	m_sbPartyActionScrollBar.SetCurItemCount( ctPartyAction );
	m_sbGuildActionScrollBar.SetCurItemCount( ctGuildAction );
	m_sbNormalActionScrollBar.SetScrollPos( 0 );
	m_sbSocialActionScrollBar.SetScrollPos( 0 );
	m_sbPartyActionScrollBar.SetScrollPos( 0 );
	m_sbGuildActionScrollBar.SetScrollPos( 0 );
}

void CUICharacterInfo::UpdateSkillInfo(int nType, int nTotal)
{
	m_strDesc.Clear();
	
	if(nType < BTN_ACTION_NORMAL)
	{
		BOOL bSpecial = FALSE;
		if(nType == BTN_SKILL_SPECIAL)
		{
			bSpecial = TRUE;
		}

		CUIManager* pUIManager = CUIManager::getSingleton();
		CUISkillLearn* pUISkillLearn = pUIManager->GetSkillLearn();

		for(int i=0; i<nTotal; i++)
		{
			m_pSelectedSkillSatisfied[i] = pUISkillLearn->IsSatisfiedSkill( m_btnSelectedSkill[i].GetSkillIndex(), m_btnSelectedSkill[i].GetSkillLevel(), bSpecial);
		}
	}

	int nItemTotal = 0;
	for(int i = 0; i<nTotal; i++)
	{
		if(m_btnSelectedSkill[i].IsEmpty())
		{
			break;
		}
		nItemTotal++;
	}


	m_nBtnTotal = nTotal;
	m_sbScrollBar.SetCurItemCount( (nItemTotal / 2)  + (nItemTotal % 2) );
}

// ----------------------------------------------------------------------------
// Name : UpdateStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::UpdateStatus( int iAttack, int iMagic, int iDefense, int iResist )
{
	m_strStrength.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.str,
										_pNetwork->MyCharacterInfo.opt_str );
	m_strDexterity.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.dex,
										_pNetwork->MyCharacterInfo.opt_dex );
	m_strIntelligence.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.intel,
											_pNetwork->MyCharacterInfo.opt_intel );
	m_strConstitution.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.con,
											_pNetwork->MyCharacterInfo.opt_con );

	m_strAttack.PrintF( "%d", iAttack );
	m_strMagicAttack.PrintF( "%d", iMagic );
	m_strDefense.PrintF( "%d", iDefense );
	m_strMagicDefense.PrintF( "%d", iResist );

	m_strExp.PrintF( "%I64d/%I64d", _pNetwork->MyCharacterInfo.curExp, _pNetwork->MyCharacterInfo.needExp );
	m_strSP.PrintF( "%d", _pNetwork->MyCharacterInfo.sp / 10000 );
	m_strPenalty.PrintF( "%d", _pNetwork->MyCharacterInfo.pkpenalty );
	m_strPenaltyAcc.PrintF( "%d", _pNetwork->MyCharacterInfo.pkcount );
	m_strFame.PrintF( "%d", _pNetwork->MyCharacterInfo.fame );

	m_strHitRate.PrintF( "%d", 0 );	//물리 명중도
	m_strDodgeRate.PrintF( "%d", 0 );	//물리 회피도
	m_strCritical.PrintF( "%d", 0 );	//크리티컬
	m_strSpeed.PrintF( "%64.1f", _pNetwork->MyCharacterInfo.runspeed );		//이동속도

	m_strMagicHitRate.PrintF( "%d", 0 );	//마법 명중도
	m_strMagicDodgeRate.PrintF( "%d", 0 );		//마법 회피도
	m_strDeadly.PrintF( "%d", 0 );		//데들리
	m_strAttackSpeed.PrintF( "%d", _pNetwork->MyCharacterInfo.attackspeed );			//공격속도


	m_strAttributeFire.PrintF( "%d", 0 );
	m_strAttributeWater.PrintF( "%d", 0 );
	m_strAttributeEarth.PrintF( "%d", 0 );
	m_strAttributeWind.PrintF( "%d", 0 );

	m_strAttributeLight.PrintF( "%d", 0 );	
	m_strAttributeDark.PrintF( "%d", 0 );

	m_strHitRate.PrintF( "%d", _pNetwork->MyCharacterInfo.hitRate );	//물리 명중도
	m_strDodgeRate.PrintF( "%d", _pNetwork->MyCharacterInfo.dodgeRate );	//물리 회피도
	m_strCritical.PrintF( "%d", _pNetwork->MyCharacterInfo.critical );	//크리티컬
	m_strSpeed.PrintF( "%64.1f", _pNetwork->MyCharacterInfo.runspeed );			//이동속도

	m_strMagicHitRate.PrintF( "%d", _pNetwork->MyCharacterInfo.magicHitRate );	//마법 명중도
	m_strMagicDodgeRate.PrintF( "%d", _pNetwork->MyCharacterInfo.magicDodgeRate );		//마법 회피도
	m_strDeadly.PrintF( "%d", _pNetwork->MyCharacterInfo.deadly );		//데들리
	m_strAttackSpeed.PrintF( "%d", _pNetwork->MyCharacterInfo.attackspeed );			//공격속도


	m_strAttributeFire.PrintF( "%d", 0 );
	m_strAttributeWater.PrintF( "%d", 0 );
	m_strAttributeEarth.PrintF( "%d", 0 );
	m_strAttributeWind.PrintF( "%d", 0 );

	m_strAttributeLight.PrintF( "%d", 0 );	
	m_strAttributeDark.PrintF( "%d", 0 );	

	int nBtnType = 0;
	int nSlotTotal = 0;

	if(m_ucipCurrentPage == PAGE_CHARINFO_NEW_SKILL)
	{
		switch(m_nSkillActionUpperTab[0])
		{
			case SKILL_ITEM:
				nBtnType = BTN_SKILL_ITEM;
				nSlotTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				break;
			case SKILL_SPECIAL:
				nBtnType = BTN_SKILL_SPECIAL;
				nSlotTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				break;
			case SKILL_VOUCHER:
				nBtnType = BTN_SKILL_VOUCHER;
				nSlotTotal = SKILL_SPECIAL_SLOT_ROW_TOTAL;
				break;
			default:
				nBtnType = BTN_SKILL_ACTIVE;
				nSlotTotal = SKILL_ACTIVE_SLOT_ROW_TOTAL;
		}

	UpdateSkillInfo(nBtnType, nSlotTotal);
	}
	if(CUIManager::getSingleton()->DoesUIExist(UI_SKILLLEARN))
	{
		CUIManager::getSingleton()->GetSkillLearn()->UpdateSkillLearn();
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUICharacterInfo::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
	g_iXPosInCharInfo = GetPosX();
	g_iYPosInCharInfo = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : AddSkillInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::AddSkillInfoString( CTString &strSkillInfo, COLOR colSkillInfo )
{
	if( m_nCurSkillInfoLines >= MAX_SKILLINFO_LINE )
		return;

	// Get length of string
	INDEX	nLength = strSkillInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
#if defined(G_THAI)
	{
		int		iPos;
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strSkillInfo);
		INDEX	nChatMax= (_iMaxSkillStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			m_strSkillInfo[m_nCurSkillInfoLines] = strSkillInfo;
			m_colSkillInfo[m_nCurSkillInfoLines++] = colSkillInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxSkillStringChar;
			BOOL	b2ByteChar = FALSE;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strSkillInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strSkillInfo.Split( nSplitPos, m_strSkillInfo[m_nCurSkillInfoLines], strTemp );
			m_colSkillInfo[m_nCurSkillInfoLines++] = colSkillInfo;

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddSkillInfoString( strTemp, colSkillInfo );

		}
		
	}
#else
	{
		// If length of string is less than max char
		if( nLength <= _iMaxSkillStringChar )
		{
			m_strSkillInfo[m_nCurSkillInfoLines] = strSkillInfo;
			m_colSkillInfo[m_nCurSkillInfoLines++] = colSkillInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxSkillStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strSkillInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strSkillInfo.Split( nSplitPos, m_strSkillInfo[m_nCurSkillInfoLines], strTemp );
			m_colSkillInfo[m_nCurSkillInfoLines++] = colSkillInfo;

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

			AddSkillInfoString( strTemp, colSkillInfo );
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : GetSkillDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::GetSkillDesc( int nSkillIndex, int nSkillLevel, BOOL bSpecial )
{
	if(!bSpecial)
	{
		CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );

		switch( rSelSkill.GetType() )
		{
		case CSkill::ST_MELEE:
		case CSkill::ST_RANGE:
		case CSkill::ST_MAGIC:
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
				int	nNeedMP = rSelSkill.GetNeedMP( nSkillLevel - 1 );
				int	nNeedHP = rSelSkill.GetNeedHP( nSkillLevel - 1 );
				if( nNeedHP == 0 )
				{
					if( nNeedMP == 0 )
					{
#if defined(G_RUSSIA)
							m_strShortDesc2.PrintF( "%s %d", _S( 4414, "LV" ), nSkillLevel );
#else
							m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );
#endif
					}
					else
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if( pUIManager->GetNeedMPReductionRate() >0)
						{
							int nNeedMPReduction = (nNeedMP*pUIManager->GetNeedMPReductionRate())/100;
#if defined(G_RUSSIA)
								m_strShortDesc2.PrintF( "%s %d %s %d (-%d)", _S( 4414, "LV" ), nSkillLevel, _S( 4412, "MP" ) ,nNeedMP, nNeedMPReduction );
#else
								m_strShortDesc2.PrintF( "Lv %d MP %d (-%d)", nSkillLevel, nNeedMP, nNeedMPReduction );
#endif
						}
						else
						{
#if defined(G_RUSSIA)
								m_strShortDesc2.PrintF( "%s %d %s %d", _S( 4414, "LV" ), nSkillLevel, _S( 4412, "MP" ) ,nNeedMP);
#else
								m_strShortDesc2.PrintF( "Lv %d MP %d", nSkillLevel, nNeedMP );
#endif
						}
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
#if defined(G_RUSSIA)
							m_strShortDesc2.PrintF( "%s %d %s %d", _S( 4414, "LV" ), nSkillLevel, _S( 4411, "HP" ) ,nNeedHP);
#else
							m_strShortDesc2.PrintF( "Lv %d HP %d", nSkillLevel, nNeedHP );
#endif
					}
					else
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if( pUIManager->GetNeedMPReductionRate() >0)
						{
							int nNeedMPReduction = (nNeedMP*pUIManager->GetNeedMPReductionRate())/100;
#if defined(G_RUSSIA)
								m_strShortDesc2.PrintF( "%s %d %s %d (-%d) %s %d", _S( 4414, "LV" ), nSkillLevel,_S( 4412, "MP" ), nNeedMP, nNeedMPReduction,_S( 4411, "HP" ), nNeedHP );
#else
								m_strShortDesc2.PrintF( "Lv %d MP %d (-%d) HP %d", nSkillLevel, nNeedMP, nNeedMPReduction, nNeedHP );
#endif
							/// 여기부터-
						}
						else
						{
#if defined(G_RUSSIA)
								m_strShortDesc2.PrintF( "%s %d %s %d %s %d",_S( 4414, "LV" ), nSkillLevel,_S( 4412, "MP" ), nNeedMP,_S( 4411, "HP" ), nNeedHP );
#else
								m_strShortDesc2.PrintF( "Lv %d MP %d HP %d", nSkillLevel, nNeedMP, nNeedHP );
#endif
						}
					}
				}
			}
			break;

		case CSkill::ST_PASSIVE:
			{
				m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
#if defined(G_RUSSIA)
					m_strShortDesc2.PrintF( "%s %d",_S( 4414, "LV" ), nSkillLevel );
#else
					m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );
#endif
			}
			break;

		/*
		case CSkill::ST_PRODUCT:
		case CSkill::ST_EVENT:
			{
				m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
			}
			break;
		*/
		}
	}
	else
	{
		CSpecialSkill	&rSelSkill = _pNetwork->GetSSkillData( nSkillIndex );
		m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
#if defined(G_RUSSIA)
			m_strShortDesc2.PrintF( "%s %d",_S( 4414, "LV" ), nSkillLevel );
#else
			m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );
#endif
	}
}

// ----------------------------------------------------------------------------
// Name : GetSkillInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::GetSkillInfo( int nSkillIndex, int nSkillLevel, BOOL bSpecial, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurSkillInfoLines = 0;

	if(!bSpecial)
	{		
		CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );
		// Get skill name
#if defined(G_RUSSIA)
			strTemp.PrintF( "%s %s %d", rSelSkill.GetName(),_S( 4414, "LV" ), nSkillLevel );
#else
			strTemp.PrintF( "%s Lv %d", rSelSkill.GetName(), nSkillLevel );
#endif
		AddSkillInfoString( strTemp );

		// Get skill type etc...
		nSkillLevel--;
		switch( rSelSkill.GetType() )
		{
		case CSkill::ST_MELEE:
		case CSkill::ST_RANGE:
		case CSkill::ST_MAGIC:
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				if( rSelSkill.GetFlag() & SF_SINGLEMODE )
					AddSkillInfoString( _S( 499, "퍼스널던전 전용 스킬" ), 0xCACACAFF );		
				else
					AddSkillInfoString( _S( 63, "액티브 스킬" ), 0xCACACAFF );

				int	nNeedMP = rSelSkill.GetNeedMP( nSkillLevel );
				int	nNeedHP = rSelSkill.GetNeedHP( nSkillLevel );
				if( nNeedHP == 0 )
				{
					if( nNeedMP != 0 )
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if( pUIManager->GetNeedMPReductionRate() >0)
						{
							int nNeedMPReduction = (nNeedMP*pUIManager->GetNeedMPReductionRate())/100;
							strTemp.PrintF( _S( 64, "소모 MP : %d" )+" (-%d)", nNeedMP, nNeedMPReduction );
						}
						else
							strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillInfoString( strTemp, 0x94B7C6FF );
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
						AddSkillInfoString( strTemp, 0x94B7C6FF );
					}
					else
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if( pUIManager->GetNeedMPReductionRate() >0)
						{
							int nNeedMPReduction = (nNeedMP*pUIManager->GetNeedMPReductionRate())/100;
							strTemp.PrintF( _S( 64, "소모 MP : %d" )+" (-%d)", nNeedMP, nNeedMPReduction );
						}
						else
							strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillInfoString( strTemp, 0x94B7C6FF );
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
						AddSkillInfoString( strTemp, 0x94B7C6FF );
					}
				}

				if( rSelSkill.GetPower( nSkillLevel ) > 0 )
				{
					strTemp.PrintF( _S( 65, "위력 : %d" ), rSelSkill.GetPower( nSkillLevel ) );
					AddSkillInfoString( strTemp, 0x94B7C6FF );
				}
				strTemp.PrintF( _S( 66, "유효 거리 : %.1f" ), rSelSkill.GetFireRange() );
				AddSkillInfoString( strTemp, 0x94B7C6FF );
			}
			break;

		case CSkill::ST_PASSIVE:
			AddSkillInfoString( _S( 67, "패시브 스킬" ), 0xCACACAFF );
			break;
		}

		const char	*pDesc = rSelSkill.GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s", pDesc );
			AddSkillInfoString( strTemp, 0x9E9684FF );
		}
	}
	// Special Skill
	else
	{
		CSpecialSkill	&rSelSkill = _pNetwork->GetSSkillData( nSkillIndex );		
		// Get skill name
#if defined(G_RUSSIA)
			strTemp.PrintF( "%s %s %d", rSelSkill.GetName(),_S( 4414, "LV" ), nSkillLevel );
#else
			strTemp.PrintF( "%s Lv %d", rSelSkill.GetName(), nSkillLevel );
#endif
		AddSkillInfoString( strTemp );

		// Get skill type etc...
		nSkillLevel--;		
		AddSkillInfoString( _S( 67, "패시브 스킬" ), 0xCACACAFF );
		const char	*pDesc = rSelSkill.GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s", pDesc );
			AddSkillInfoString( strTemp, 0x9E9684FF );
		}
	}

	nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + _iMaxSkillStringChar *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurSkillInfoLines * _pUIFontTexMgr->GetLineHeight();
}

// ----------------------------------------------------------------------------
// Name : ShowSkillInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ShowSkillInfo( BOOL bShowInfo, CUIButtonEx *pSkillBtn, BOOL bSpecial )
{
	static int	nOldSkillIndex = -1;

	m_bShowSkillInfo = FALSE;

	// Hide skill information
	if( !bShowInfo )
	{
		nOldSkillIndex = -1;
		return;
	}

	int		nSkillIndex = pSkillBtn->GetSkillIndex();
	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX = 0, nInfoPosY = 0;

	m_bShowSkillInfo = TRUE;

	// Update skill information
	if( nOldSkillIndex != nSkillIndex )
	{
		bUpdateInfo = TRUE;
		nOldSkillIndex = nSkillIndex;
		pSkillBtn->GetAbsPos( nInfoPosX, nInfoPosY );

		// Get skill information
		GetSkillInfo( nSkillIndex, pSkillBtn->GetSkillLevel(), bSpecial, nInfoWidth, nInfoHeight );
	}

	// Update skill information box
	if( bUpdateInfo )
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
			m_rcSkillInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcSkillInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowSkillInfo )
	{
		nOldSkillIndex = -1;
		m_bShowSkillInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : AddActionInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::AddActionInfoString( CTString &strActionInfo, COLOR colActionInfo )
{
	if( m_nCurActionInfoLines >= MAX_ACTIONINFO_LINE )
		return;

	// Get length of string
	INDEX	nLength = strActionInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
#if defined(G_THAI)
	{
		int		iPos;
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strActionInfo);
		INDEX	nChatMax= (_iMaxSkillStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			m_strActionInfo[m_nCurActionInfoLines] = strActionInfo;
			m_colActionInfo[m_nCurActionInfoLines++] = colActionInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxActionStringChar;
			BOOL	b2ByteChar = FALSE;
			for( iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strActionInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strActionInfo.Split( nSplitPos, m_strActionInfo[m_nCurActionInfoLines], strTemp );
			m_colActionInfo[m_nCurActionInfoLines++] = colActionInfo;

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddActionInfoString( strTemp, colActionInfo );

		}
		
	}
#else
	{
		// If length of string is less than max char
		if( nLength <= _iMaxActionStringChar )
		{
			m_strActionInfo[m_nCurActionInfoLines] = strActionInfo;
			m_colActionInfo[m_nCurActionInfoLines++] = colActionInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxActionStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strActionInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strActionInfo.Split( nSplitPos, m_strActionInfo[m_nCurActionInfoLines], strTemp );
			m_colActionInfo[m_nCurActionInfoLines++] = colActionInfo;

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

			AddActionInfoString( strTemp, colActionInfo );
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : GetActionDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::GetActionDesc( int nActionIndex )
{
	CAction	&rSelAction = _pNetwork->GetActionData(nActionIndex);
	// Get action name
	m_strShortDesc = rSelAction.GetName();
}

// ----------------------------------------------------------------------------
// Name : GetActionInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::GetActionInfo( int nActionIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurActionInfoLines = 0;

	CAction	&rSelAction = _pNetwork->GetActionData(nActionIndex);

	// Get action name
	strTemp = rSelAction.GetName();
	AddActionInfoString( strTemp );

	// Get action description
	if( rSelAction.GetDesc() != NULL )
	{
		strTemp = rSelAction.GetDesc();
		AddActionInfoString( strTemp, 0x94BAC6FF );
	}

	nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + _iMaxActionStringChar *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurActionInfoLines * _pUIFontTexMgr->GetLineHeight();
}


// Name : ShowActionInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ShowActionInfo( BOOL bShowInfo, CUIButtonEx *pActionBtn )
{
	static int	nOldActionIndex = -1;

	m_bShowActionInfo = FALSE;

	// Hide action information
	if( !bShowInfo )
	{
		nOldActionIndex = -1;
		return;
	}

	int		nActionIndex = pActionBtn->GetActionIndex();
	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX = 0, nInfoPosY = 0;

	m_bShowActionInfo = TRUE;

	// Update action information
	if( nOldActionIndex != nActionIndex )
	{
		bUpdateInfo = TRUE;
		nOldActionIndex = nActionIndex;
		pActionBtn->GetAbsPos( nInfoPosX, nInfoPosY );

		// Get action information
		GetActionInfo( nActionIndex, nInfoWidth, nInfoHeight );
	}

	// Update action information box
	if( bUpdateInfo )
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
			m_rcActionInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcActionInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowActionInfo )
	{
		nOldActionIndex = -1;
		m_bShowActionInfo = FALSE;
	}
}


// ----------------------------------------------------------------------------
// Name : RenderSkillBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderSkillBtns()
{
	int	nX = CHARINFO_SLOT_SX, nY = CHARINFO_SLOT_SY;
	int	iRow, iRowS, iRowE;

#ifdef ADJUST_MEMORIZE_SKILL
	// Memorize skill tab
	if( m_nCurrentSkillTab == SKILL_MEMORIZE )
	{
		// Memorize skill button
		iRowS = m_sbMemorizeSkillScrollBar.GetScrollPos();		
		iRowE = iRowS + SKILL_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{      
			m_btnMemorizeSkill[iRow].SetPos( nX, nY );
			if( m_btnMemorizeSkill[iRow].IsEmpty() )		
				continue;
			
			m_btnMemorizeSkill[iRow].Render();      
		}
	}
#endif
	if( m_nCurrentSkillTab == SKILL_ITEM)
	{
		// Item Special Skill
		iRowS = m_sbItemSkillSpecialScrollBar.GetScrollPos();		
		iRowE = iRowS + SKILL_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{      
			m_btnItemSpecialSkill[iRow].SetPos( nX, nY );
			if( m_btnItemSpecialSkill[iRow].IsEmpty() )		
				continue;

			m_btnItemSpecialSkill[iRow].Render();      
		}
	}
	// Special skill tab
	if( m_nCurrentSkillTab == SKILL_SPECIAL)
	{
		// Special skill button
		iRowS = m_sbSpecialSkillScrollBar.GetScrollPos();		
		iRowE = iRowS + SKILL_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{      
			m_btnSpecialSkill[iRow].SetPos( nX, nY );
			if( m_btnSpecialSkill[iRow].IsEmpty() )		
				continue;
			
			m_btnSpecialSkill[iRow].Render();      
		}
	}
	else if ( m_nCurrentSkillTab == SKILL_VOUCHER)
	{
		iRowS = m_sbVoucherSkillScrollBar.GetScrollPos();
		iRowE = iRowS + SKILL_SLOT_ROW;
		for (iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{
			m_btnVoucherSkill[iRow].SetPos(nX, nY);
			if (m_btnVoucherSkill[iRow].IsEmpty() )
			{
				continue;
			}

			m_btnVoucherSkill[iRow].Render();
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );
	

#ifdef ADJUST_MEMORIZE_SKILL
	// Memorize skill tab
	else if( m_nCurrentSkillTab == SKILL_MEMORIZE )
	{
		nX = CHARINFO_SLOT_INFO_CX;
		nY = CHARINFO_SLOT_SY + 3;

		iRowS = m_sbMemorizeSkillScrollBar.GetScrollPos();
		iRowE = iRowS + SKILL_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{     
			if( m_btnMemorizeSkill[iRow].IsEmpty() )
				continue;            		
  
			GetSkillDesc( m_btnMemorizeSkill[iRow].GetSkillIndex(),
							m_btnMemorizeSkill[iRow].GetSkillLevel() );

			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
			pDrawPort->PutTextExCX( m_strShortDesc2, m_nPosX + nX, m_nPosY + nY + 15, 0xC5C5C5FF );
		}
	}
#endif
	// Special skill tab
	if( m_nCurrentSkillTab == SKILL_SPECIAL)
	{
		nX = CHARINFO_SLOT_INFO_CX;
		nY = CHARINFO_SLOT_SY + 3; // Date : 2005-01-21,   By Lee Ki-hwan

		iRowS = m_sbSpecialSkillScrollBar.GetScrollPos();
		iRowE = iRowS + SKILL_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{     
			if( m_btnSpecialSkill[iRow].IsEmpty() )
				continue;            		
  
			GetSkillDesc( m_btnSpecialSkill[iRow].GetSkillIndex(),
							m_btnSpecialSkill[iRow].GetSkillLevel(), TRUE );

			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
			pDrawPort->PutTextExCX( m_strShortDesc2, m_nPosX + nX, m_nPosY + nY + 15, 0xC5C5C5FF );
		}
	}
	else if( m_nCurrentSkillTab == SKILL_VOUCHER)
	{
		nX = CHARINFO_SLOT_INFO_CX;
		nY = CHARINFO_SLOT_SY + 3; // Date : 2005-01-21,   By Lee Ki-hwan

		iRowS = m_sbVoucherSkillScrollBar.GetScrollPos();
		iRowE = iRowS + SKILL_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{     
			if( m_btnVoucherSkill[iRow].IsEmpty() )
				continue;            		
  
			GetSkillDesc( m_btnVoucherSkill[iRow].GetSkillIndex(),
							m_btnVoucherSkill[iRow].GetSkillLevel(), TRUE );

			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
			pDrawPort->PutTextExCX( m_strShortDesc2, m_nPosX + nX, m_nPosY + nY + 15, 0xC5C5C5FF );
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderActionBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderActionBtns()
{
	int	iRow, iRowS, iRowE;
	int	nX = CHARINFO_SLOT_SX, nY = CHARINFO_SLOT_SY;
	// Normal action button
	if( m_ucipCurrentPage == UCIP_ACTION )
	{
		iRowS = m_sbNormalActionScrollBar.GetScrollPos();
		iRowE = iRowS + ACTION_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39)
		{
			m_btnNormalAction[iRow].SetPos( nX, nY );		
			if( m_btnNormalAction[iRow].IsEmpty() )
				continue;
			
			m_btnNormalAction[iRow].Render();
		}
	}
	// Social action button
	else if( m_ucipCurrentPage == UCIP_SOCIAL )
	{
		iRowS = m_sbSocialActionScrollBar.GetScrollPos();
		iRowE = iRowS + ACTION_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39)
		{
			m_btnSocialAction[iRow].SetPos( nX, nY );		
			if( m_btnSocialAction[iRow].IsEmpty() )
				continue;
			
			m_btnSocialAction[iRow].Render();
		}
	}
	// Group action button
	else if( m_ucipCurrentPage == UCIP_GROUP )
	{
		// Party action tab
		if( m_bPartyActionTab )
		{
			iRowS = m_sbPartyActionScrollBar.GetScrollPos();
			iRowE = iRowS + ACTION_SLOT_ROW;
			for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39)
			{
				m_btnPartyAction[iRow].SetPos( nX, nY );		
				if( m_btnPartyAction[iRow].IsEmpty() )
					continue;
				
				m_btnPartyAction[iRow].Render();
			}
		}
		// Guild action tab
		else
		{
			iRowS = m_sbGuildActionScrollBar.GetScrollPos();
			iRowE = iRowS + ACTION_SLOT_ROW;
			for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39)
			{
				m_btnGuildAction[iRow].SetPos( nX, nY );		
				if( m_btnGuildAction[iRow].IsEmpty() )
					continue;
				
				m_btnGuildAction[iRow].Render();
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ACTION );

	// Prepare Text
	nX = CHARINFO_SLOT_INFO_CX;
	nY = CHARINFO_SLOT_SY + 10;

	// Normal action button
	if( m_ucipCurrentPage == UCIP_ACTION )
	{
		iRowS = m_sbNormalActionScrollBar.GetScrollPos();
		iRowE = iRowS + ACTION_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{     
			if( m_btnNormalAction[iRow].IsEmpty() )
				continue;            		

			GetActionDesc( m_btnNormalAction[iRow].GetActionIndex() );

			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
		}
	}
	// Social action button
	else if( m_ucipCurrentPage == UCIP_SOCIAL )
	{
		iRowS = m_sbSocialActionScrollBar.GetScrollPos();
		iRowE = iRowS + ACTION_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
		{     
			if( m_btnSocialAction[iRow].IsEmpty() )
				continue;            		

			GetActionDesc( m_btnSocialAction[iRow].GetActionIndex() );

			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
		}
	}
	// Group action button
	else if( m_ucipCurrentPage == UCIP_GROUP )
	{
		// Party action tab
		if( m_bPartyActionTab )
		{
			iRowS = m_sbPartyActionScrollBar.GetScrollPos();
			iRowE = iRowS + ACTION_SLOT_ROW;
			for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
			{     
				if( m_btnPartyAction[iRow].IsEmpty() )
					continue;            		

				GetActionDesc( m_btnPartyAction[iRow].GetActionIndex() );

				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
			}
		}
		// Guild action tab
		else
		{
			iRowS = m_sbGuildActionScrollBar.GetScrollPos();
			iRowE = iRowS + ACTION_SLOT_ROW;
			for( iRow = iRowS; iRow < iRowE; iRow++, nY += 39 )
			{     
				if( m_btnGuildAction[iRow].IsEmpty() )
					continue;            		

				GetActionDesc( m_btnGuildAction[iRow].GetActionIndex() );

				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + nX, m_nPosY + nY, 0xC5C5C5FF );
			}
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderStatusContent()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderStatusContent()
{
	// Render growth point buttons
	m_btnGPStrength.Render();
	m_btnGPDexterity.Render();
	m_btnGPIntelligence.Render();
	m_btnGPConstitution.Render();

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in character information
	pDrawPort->PutTextEx( _S( 69, "캐릭터 정보" ), m_nPosX + CHARINFO_TITLE_TEXT_OFFSETX,
										m_nPosY + CHARINFO_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// Profile
	pDrawPort->PutTextEx( _S( 70, "프로필" ), m_nPosX + STATUS_SEMI_TITLE_SX,
										m_nPosY + STATUS_SEMI_TITLE_PROFILE_SY, 0xE1B300FF );

  	int	nX1 = m_nPosX + STATUS_NAME_SX;
	int	nX2 = m_nPosX + STATUS_NAME_MAIN_SX;
	int	nY = m_nPosY + STATUS_NAME_SY;
	pDrawPort->PutTextEx( _S( 71, "이름" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextEx( _pNetwork->MyCharacterInfo.name, nX2, nY, 0xFFFFFFFF );
	nY += STATUS_PROFILE_OFFSETY;
	pDrawPort->PutTextEx( _S( 72, "클래스" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextEx( CJobInfo::getSingleton()->GetName(_pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2),
										nX2, nY, 0x8B8B8BFF );
	nY += STATUS_PROFILE_OFFSETY;
	pDrawPort->PutTextEx( _S( 73, "레벨" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextEx( pUIManager->GetPlayerInfo()->GetStringOfLevel(),
										nX2, nY, 0xD91900FF );
	nY = m_nPosY + STATUS_DEGREE_SY;
	pDrawPort->PutTextEx( _S( 74, "칭호" ), nX1, nY, 0xABABABFF  );
	//pDrawPort->PutTextEx( _S( 74, "칭호" ), nX1, nY, 0xFF9674FF  );
	nY += STATUS_PROFILE_OFFSETY;
	pDrawPort->PutTextEx( _S( 75, "길드" ), nX1, nY, 0xABABABFF );
	if(_pNetwork->MyCharacterInfo.lGuildIndex == -1)
	pDrawPort->PutTextEx( _S( 76, "길드 없음" ), nX2, nY, 0xD6A6D6FF );
	else
	{
		CTString strGuildName = _pNetwork->MyCharacterInfo.strGuildName;
		pDrawPort->PutTextEx( strGuildName, nX2, nY, 0xD6A6D6FF );
	}
	nY += STATUS_PROFILE_OFFSETY;
	pDrawPort->PutTextEx( _S( 77, "동맹" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextEx( _S( 78, "동맹 없음" ), nX2, nY, 0xABC98AFF );
  
	// State
	pDrawPort->PutTextEx( _S( 79, "캐릭터 상태" ), m_nPosX + STATUS_SEMI_TITLE_SX,
										m_nPosY + STATUS_SEMI_TITLE_STATE_SY , 0xE1B300FF );
	nX1 = m_nPosX + STATUS_STR_SX;
	nX2 = m_nPosX + STATUS_STR_MAIN_RX;
	nY = m_nPosY + STATUS_STR_SY;
	pDrawPort->PutTextEx( _S( 80, "힘" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strStrength, nX2, nY, 0xE18600FF );
	nY += STATUS_STR_OFFSETY;
	pDrawPort->PutTextEx( _S( 81, "민첩" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strDexterity, nX2, nY, 0xE18600FF );
	nY += STATUS_STR_OFFSETY;
	pDrawPort->PutTextEx( _S( 82, "지혜" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strIntelligence, nX2, nY, 0xE18600FF );
	nY += STATUS_STR_OFFSETY;
	pDrawPort->PutTextEx( _S( 83, "체질" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strConstitution, nX2, nY, 0xE18600FF );

	nY = m_nPosY + STATUS_STATPOINT_SY;
	pDrawPort->PutTextEx( _S( 84, "성장 포인트" ), nX1, nY, 0xFFFFFFFF );
	pDrawPort->PutTextExRX( m_strStatPoint, nX2, nY, 0xFF5426FF );

	nY = m_nPosY + STATUS_ATTACK_SY;
	pDrawPort->PutTextEx( _S( 85, "공격력" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strAttack, nX2, nY, 0xE18600FF );
	nY += STATUS_ATTACK_OFFSETY;
	pDrawPort->PutTextEx( _S( 86, "마법공격력" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strMagicAttack, nX2, nY, 0xE18600FF );
	nY += STATUS_ATTACK_OFFSETY;
	pDrawPort->PutTextEx( _S( 87, "방어력" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strDefense, nX2, nY, 0xE18600FF );
	nY += STATUS_ATTACK_OFFSETY;
	pDrawPort->PutTextEx( _S( 88, "마법방어력" ), nX1, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strMagicDefense, nX2, nY, 0xE18600FF );

	nX2 = m_nPosX + STATUS_EXP_MAIN_RX;
	nY = m_nPosY + STATUS_EXP_SY;
	pDrawPort->PutTextEx( _S( 89, "경험치" ), nX1, nY, 0x949494FF );
	pDrawPort->PutTextExRX( m_strExp, nX2, nY, 0xB3B6BCFF );
	nY = m_nPosY + STATUS_SP_SY;
	pDrawPort->PutTextEx( _S( 90, "숙련도" ), nX1, nY, 0x949494FF );
	pDrawPort->PutTextExRX( m_strSP, nX2, nY, 0xABA7A4FF );

	// Penalty
	pDrawPort->PutTextEx( _S( 501, "캐릭터 성향" ), m_nPosX + STATUS_SEMI_TITLE_SX,		
										m_nPosY + STATUS_SEMI_TITLE_PENALTY_SY , 0xE1B300FF );

	nX2 = m_nPosX + STATUS_STR_MAIN_RX;
	nY = m_nPosY + STATUS_PENALTY_SY;
	pDrawPort->PutTextEx( _S( 502, "성향수치" ), nX1, nY, 0xABABABFF );					
	pDrawPort->PutTextExRX( m_strPenalty, m_nPosX + 110, nY, 0xE18600FF );

	pDrawPort->PutTextEx( _S( 503, "누적수치" ), nX1 + 85, nY, 0xABABABFF );
	pDrawPort->PutTextExRX( m_strPenaltyAcc, nX2, nY, 0xE18600FF );

	nY = m_nPosY + STATUS_PENALTY_ACC_SY;
	pDrawPort->PutTextEx( _S( 1083, "명성치" ), nX1, nY, 0xABABABFF );		
	pDrawPort->PutTextExRX( m_strFame, nX2, nY, 0xE18600FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderSkillContent()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderSkillContent()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render tab
#ifdef ADJUST_MEMORIZE_SKILL
	int	nX = m_nPosX + m_rcSubTab.Left + CHARINFO_4SUBTAB_WIDTH;
#else
	int	nX = m_nPosX + m_rcSubTab.Left + CHARINFO_3SUBTAB_WIDTH;
#endif
	pDrawPort->AddTexture( nX, m_nPosY + m_rcTab.Top,
										nX + 1, m_nPosY + m_rcTab.Bottom,
										m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
										0xFFFFFFFF );
#ifdef ADJUST_MEMORIZE_SKILL
	nX += CHARINFO_4SUBTAB_WIDTH;
	pDrawPort->AddTexture( nX, m_nPosY + m_rcTab.Top,
										nX + 1, m_nPosY + m_rcTab.Bottom,
										m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
										0xFFFFFFFF );

	nX += CHARINFO_4SUBTAB_WIDTH;
#else
	nX += CHARINFO_3SUBTAB_WIDTH;
#endif	
	pDrawPort->AddTexture( nX, m_nPosY + m_rcTab.Top,
										nX + 1, m_nPosY + m_rcTab.Bottom,
										m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
										0xFFFFFFFF );
  	
	// Scroll bars
// 	if( m_nCurrentSkillTab == SKILL_ACTIVE )
// 		m_sbActiveSkillScrollBar.Render();
// 	else if( m_nCurrentSkillTab == SKILL_PASSIVE )
// 		m_sbPassiveSkillScrollBar.Render();
	if (m_nCurrentSkillTab == SKILL_ITEM)
		m_sbItemSkillSpecialScrollBar.Render();
#ifdef ADJUST_MEMORIZE_SKILL
	else if( m_nCurrentSkillTab == SKILL_MEMORIZE )
		m_sbMemorizeSkillScrollBar.Render();
#endif
	else if( m_nCurrentSkillTab == SKILL_SPECIAL)
		m_sbSpecialSkillScrollBar.Render();
	else if (m_nCurrentSkillTab == SKILL_VOUCHER)
		m_sbVoucherSkillScrollBar.Render();
	

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in skill
 	pDrawPort->PutTextEx( _S( 6077, "특수 스킬" ), m_nPosX + CHARINFO_TITLE_TEXT_OFFSETX,
 										m_nPosY + CHARINFO_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 4288, "아이템" ), m_nPosX + SKILL_ACTIVE_TAB_TEXT_CX,
											m_nPosY + SKILL_TAB_TEXT_SY,
											m_nCurrentSkillTab == SKILL_ITEM ? 0xE1B300FF : 0x6B6B6BFF );
// 	pDrawPort->PutTextExCX( _S( 276, "강화" ), m_nPosX + SKILL_PASSIVE_TAB_TEXT_CX,
// 											m_nPosY + SKILL_TAB_TEXT_SY,
// 											m_nCurrentSkillTab == SKILL_PASSIVE ? 0xE1B300FF : 0x6B6B6BFF );
#ifdef ADJUST_MEMORIZE_SKILL
pDrawPort->PutTextExCX( _S( 1084, "기억" ), m_nPosX + SKILL_MEMORIZE_TAB_TEXT_CX,			
											m_nPosY + SKILL_TAB_TEXT_SY,
											m_nCurrentSkillTab == SKILL_MEMORIZE ? 0xE1B300FF : 0x6B6B6BFF );
#endif
	pDrawPort->PutTextExCX( _S( 93, "특수" ), m_nPosX + SKILL_SPECIAL_TAB_TEXT_CX,
											m_nPosY + SKILL_TAB_TEXT_SY,
											m_nCurrentSkillTab == SKILL_SPECIAL ? 0xE1B300FF : 0x6B6B6BFF );

	pDrawPort->PutTextExCX( _S( 4490, "증표" ), m_nPosX + SKILL_VOUCHER_TAB_TEXT_CX,
											m_nPosY + SKILL_TAB_TEXT_SY,
											m_nCurrentSkillTab == SKILL_VOUCHER ? 0xE1B300FF : 0x6B6B6BFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render skill buttons
	RenderSkillBtns();

	// Skill information ( name and property etc... )
	if( m_bShowSkillInfo )
	{
		// Set skill texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top,
											m_rcSkillInfo.Left + 7, m_rcSkillInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Left + 7, m_rcSkillInfo.Top,
											m_rcSkillInfo.Right - 7, m_rcSkillInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Right - 7, m_rcSkillInfo.Top,
											m_rcSkillInfo.Right, m_rcSkillInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top + 7,
											m_rcSkillInfo.Left + 7, m_rcSkillInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Left + 7, m_rcSkillInfo.Top + 7,
											m_rcSkillInfo.Right - 7, m_rcSkillInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Right - 7, m_rcSkillInfo.Top + 7,
											m_rcSkillInfo.Right, m_rcSkillInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Bottom - 7,
											m_rcSkillInfo.Left + 7, m_rcSkillInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Left + 7, m_rcSkillInfo.Bottom - 7,
											m_rcSkillInfo.Right - 7, m_rcSkillInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSkillInfo.Right - 7, m_rcSkillInfo.Bottom - 7,
											m_rcSkillInfo.Right, m_rcSkillInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Render skill information
		int	nInfoX = m_rcSkillInfo.Left + 12;
		int	nInfoY = m_rcSkillInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurSkillInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strSkillInfo[iInfo], nInfoX, nInfoY, m_colSkillInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : RenderActionContent()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderActionContent()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Scroll bar of normal action
	m_sbNormalActionScrollBar.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render action buttons
	RenderActionBtns();

	// Text in action
	pDrawPort->PutTextEx( _S( 94, "액션" ), m_nPosX + CHARINFO_TITLE_TEXT_OFFSETX,
									m_nPosY + CHARINFO_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 95, "일반" ), m_nPosX + ACTION_NORMAL_TAB_TEXT_CX,
											m_nPosY + ACTION_TAB_TEXT_SY, 0xE1B300FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Action information
	if( m_bShowActionInfo )
	{
		// Set action texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right, m_rcActionInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Render action information
		int	nInfoX = m_rcActionInfo.Left + 12;
		int	nInfoY = m_rcActionInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurActionInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strActionInfo[iInfo], nInfoX, nInfoY, m_colActionInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSocialContent()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderSocialContent()
{
	// Scroll bar of social action
	m_sbSocialActionScrollBar.Render();

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render action buttons
	RenderActionBtns();

	// Text in action
	pDrawPort->PutTextEx( _S( 94, "액션" ), m_nPosX + CHARINFO_TITLE_TEXT_OFFSETX,
									m_nPosY + CHARINFO_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 96, "소셜" ), m_nPosX + ACTION_SOCIAL_TAB_TEXT_CX,
											m_nPosY + ACTION_TAB_TEXT_SY, 0xE1B300FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Action information
	if( m_bShowActionInfo )
	{
		// Set action texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right, m_rcActionInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Render action information
		int	nInfoX = m_rcActionInfo.Left + 12;
		int	nInfoY = m_rcActionInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurActionInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strActionInfo[iInfo], nInfoX, nInfoY, m_colActionInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : RenderGroupContent()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RenderGroupContent()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render tab 
	pDrawPort->AddTexture( m_nPosX + m_rcTab.Left, m_nPosY + m_rcTab.Top,
										m_nPosX + m_rcTab.Right, m_nPosY + m_rcTab.Bottom,
										m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
										0xFFFFFFFF );

	// Scroll bar of group action
	if( m_bPartyActionTab )
		m_sbPartyActionScrollBar.Render();
	else
		m_sbGuildActionScrollBar.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render action buttons
	RenderActionBtns();

	// Text in action
	pDrawPort->PutTextEx( _S( 94, "액션" ), m_nPosX + CHARINFO_TITLE_TEXT_OFFSETX,
									m_nPosY + CHARINFO_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 97, "파티" ), m_nPosX + ACTION_PARTY_TAB_TEXT_CX,
											m_nPosY + ACTION_TAB_TEXT_SY,
											m_bPartyActionTab ? 0xE1B300FF : 0x6B6B6BFF );
	pDrawPort->PutTextExCX( _S( 98, "길드" ), m_nPosX + ACTION_GUILD_TAB_TEXT_CX,
											m_nPosY + ACTION_TAB_TEXT_SY,
											m_bPartyActionTab ? 0x6B6B6BFF : 0xE1B300FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Action information
	if( m_bShowActionInfo )
	{
		// Set action texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top,
											m_rcActionInfo.Right, m_rcActionInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Top + 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Left + 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcActionInfo.Right - 7, m_rcActionInfo.Bottom - 7,
											m_rcActionInfo.Right, m_rcActionInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Render action information
		int	nInfoX = m_rcActionInfo.Left + 12;
		int	nInfoY = m_rcActionInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurActionInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strActionInfo[iInfo], nInfoX, nInfoY, m_colActionInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::Render()
{
	// Set character information texture ( top )
	CUIManager::getSingleton()->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	RenderNewCharacterInfo();
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		extern INDEX	g_iEnterChat;
		int	nInfoX, nInfoY;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case UCIP_STATUS:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 101, "스테이터스" ), "(T,Alt+T)" );
				else
					m_strToolTip.PrintF( "%s%s", _S( 101, "스테이터스" ), "(Alt+T)" );
			}
			break;

		case UCIP_SKILL:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 6077, "특수 스킬" ) );
				else
					m_strToolTip.PrintF( "%s%s", _S( 6077, "특수 스킬" ) );
			}
			break;

		case UCIP_ACTION:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 103, "액션" ), "(J,Alt+J)" );
				else
					m_strToolTip.PrintF( "%s%s", _S( 103, "액션" ), "(Alt+J)" );
			}
			break;

		case UCIP_SOCIAL:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 104, "소셜" ), "(C,Alt+C)" );
				else
					m_strToolTip.PrintF( "%s%s", _S( 104, "소셜" ), "(Alt+C)" );
			}
			break;

		case UCIP_GROUP:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 105, "그룹" ), "(G,Alt+G)" );
				else
					m_strToolTip.PrintF( "%s%s", _S( 105, "그룹" ), "(Alt+G)" );
			}
			break;

/*
		case UCIP_QUEST:
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s%s", _S( 106, "퀘스트" ), "(Q,Alt+Q)" );
				else
					m_strToolTip.PrintF( "%s%s", _S( 106, "퀘스트" ), "(Alt+Q)" );
			}
			break;
*/
		}
		int	nInfoWidth;
#if defined(G_THAI)
		{
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
		} 
#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		int	nInfoHeight = 22;

		nInfoX = m_nPosX + m_rcCharInfoTab.Right + 2;
		nInfoY = m_nPosY + m_rcCharInfoTab.Top +
					( CHARINFO_TAB_HEIGHT - _pUIFontTexMgr->GetLineHeight() ) / 2;

		if( nInfoX + nInfoWidth > CUIManager::getSingleton()->GetMaxI() )
			nInfoX = CUIManager::getSingleton()->GetMaxI() - nInfoWidth;

		m_rcToolTip.Left = nInfoX;
		m_rcToolTip.Top = nInfoY + ( CHARINFO_TAB_HEIGHT + CHARINFO_TAB_GAP ) * nToolTipID;
		m_rcToolTip.Right = m_rcToolTip.Left + nInfoWidth;
		m_rcToolTip.Bottom = m_rcToolTip.Top + nInfoHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ToggleVisible()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( IsVisible() )
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		SetCurrentPageNewCharacterInfo(PAGE_CHARINFO_NEW_STATUS);
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ToggleVisibleStatus()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

 	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_STATUS )
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		// bug Fix. [12/23/2009 rumist]
		m_nSkillDescRow = -1;
		SetCurrentPageNewCharacterInfo(PAGE_CHARINFO_NEW_STATUS);
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
	
	// [091223 sora] 창닫기와 전환시 표시되고 잇는 버튼위치값 초기화
	m_nSkillDescRow = -1;
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleSkill()
// Desc :
// ----------------------------------------------------------------------------
// 스킬 정보 보기중 단축키로 인한 화면 전환시 이전 스킬 정보창이 계속 보임. FIX  [12/23/2009 rumist]
void CUICharacterInfo::ToggleVisibleSkill()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_SKILL )
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		m_nSkillDescRow = -1;
		m_ucipCurrentPage = PAGE_CHARINFO_NEW_SKILL;
		m_strTitleName.PrintF("%s", _S(4228, "특수 스킬") );
		m_nSkillActionUpperTab[0] = 0;
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[0]);	
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleAction()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ToggleVisibleAction()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_ACTION_SOCIAL && m_nSkillActionUpperTab[1] == 0)
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		m_nSkillDescRow = -1;
		m_ucipCurrentPage = PAGE_CHARINFO_NEW_ACTION_SOCIAL;
		m_strTitleName.PrintF( "%s,%s",_S(94, "액션") ,_S(96, "소셜") );
		m_nSkillActionUpperTab[1] = 0;
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[1]);
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleSocial()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ToggleVisibleSocial()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_ACTION_SOCIAL && m_nSkillActionUpperTab[1] == 1)
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		m_nSkillDescRow = -1;
		m_ucipCurrentPage = PAGE_CHARINFO_NEW_ACTION_SOCIAL;
		m_strTitleName.PrintF( "%s,%s",_S(94, "액션") ,_S(96, "소셜") );
		m_nSkillActionUpperTab[1] = 1;
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[1]);	
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleGroup()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ToggleVisibleGroup()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetBilling()->IsLock() ) return;

	if( IsVisible() )
	{
		g_iXPosInCharInfo = GetPosX();
		g_iYPosInCharInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInCharInfo, g_iYPosInCharInfo );
	}

	if( m_ucipCurrentPage == PAGE_CHARINFO_NEW_GUILD_PARTY && m_nSkillActionUpperTab[2] == 0)
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, bVisible );
		// [091223 sora] 창을 닫을때 탭 설명표시 초기화
		if(!bVisible)
			m_bCharDescVisible = FALSE;
	}
	else
	{
		m_nSkillDescRow = -1;
		m_ucipCurrentPage = PAGE_CHARINFO_NEW_GUILD_PARTY;

		m_strTitleName.PrintF("%s,%s",_S(97, "파티") ,_S(98, "길드") );
		m_nSkillActionUpperTab[2] = 0;
		SetCurrentSkillInfo(m_ucipCurrentPage, m_nSkillActionUpperTab[2]);
		pUIManager->RearrangeOrder( UI_CHARACTERINFO, TRUE );
	}
}

// ----------------------------------------------------------------------------
// Name : GetSkillLevel()
// Desc :
// ----------------------------------------------------------------------------
SBYTE CUICharacterInfo::GetSkillLevel( int nIndex, BOOL bSpecial )
{
	int		nSkill;

	if( !bSpecial )
	{
		//물뿌리기 스킬일 경우...
		if( nIndex ==436 )
			return 1;
		
		for( nSkill = 0; nSkill < SKILL_ACTIVE_SLOT_ROW_TOTAL; nSkill++ )
		{
			if( m_btnActiveSkill[nSkill].GetSkillIndex() == nIndex )
				return m_btnActiveSkill[nSkill].GetSkillLevel();
		}
		
		for( nSkill = 0; nSkill < SKILL_PASSIVE_SLOT_ROW_TOTAL; nSkill++ )
		{
			if( m_btnPassiveSkill[nSkill].GetSkillIndex() == nIndex )
				return m_btnPassiveSkill[nSkill].GetSkillLevel();
		}

		for( nSkill = 0; nSkill < SKILL_SPECIAL_SLOT_ROW_TOTAL; nSkill++ )
		{
			if( m_btnItemSpecialSkill[nSkill].GetSkillIndex() == nIndex )
				return m_btnItemSpecialSkill[nSkill].GetSkillLevel();
		}

#ifdef ADJUST_MEMORIZE_SKILL
		for( nSkill = 0; nSkill < SKILL_MEMORIZE_SLOT_ROW_TOTAL; nSkill++ )
		{
			if( m_btnMemorizeSkill[nSkill].GetSkillIndex() == nIndex )
				return m_btnMemorizeSkill[nSkill].GetSkillLevel();
		}
#endif
	}
	else
	{
		for( nSkill = 0; nSkill < SKILL_SPECIAL_SLOT_ROW_TOTAL; nSkill++ )
		{
			if( m_btnSpecialSkill[nSkill].GetSkillIndex() == nIndex )
				return m_btnSpecialSkill[nSkill].GetSkillLevel();
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICharacterInfo::MouseMessage( MSG *pMsg )
{
	return MouseMessageNewCharacterInfo(pMsg);
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
//static INDEX	iNpcIndex;

// ----------------------------------------------------------------------------
// Name : UseAction()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::UseAction( int nIndex )
{
	if( _pNetwork->MyCharacterInfo.sbShopType != PST_NOSHOP && nIndex != ACTION_NUM_SELL)
	{
		return;
	}

	if( CUIManager::getSingleton()->IsCSFlagOn( CSF_WAREHOUSE ) )
	{
		return;
	}
	
	if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
	{ // 비행모드에서는 액션 안됨
		return;
	}

	if ( CUIManager::getSingleton()->IsCSFlagOn( CSF_ITEMWEARING ) )
	{
		return; // 장비 착용중에는 사용 불가
	}
	
	// [100311: selo] OX 존, 몬스터 콤보 존 에서 앉기 못하게 함
	// [100510: selo] 큐브 존도 안지 못하게 함 레이드 던전도 포함
	if( nIndex == 3 )
	{
		ULONG zoneNo = _pNetwork->MyCharacterInfo.zoneNo;
		if( zoneNo == 14 || zoneNo == 22 || zoneNo == 25 || zoneNo == 33 || zoneNo == 35 || zoneNo == 36)
			return;
	}

	CAction	&rActionData = _pNetwork->GetActionData(nIndex);
	LONG	lIndex = rActionData.GetIndex();

	switch( rActionData.GetType() )
	{
	case ACTION_NORMAL:
		{
			CEntity			*penPlEntity;
			CPlayerEntity	*penPlayerEntity;
			penPlEntity = CEntity::GetPlayerEntity( 0 );
			penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

			switch( lIndex )
			{
			case 2:		// 걷기, 뛰기
			case 3:		// 앉기, 서기
			case 23:	// 전투, 비전투
				{
					penPlayerEntity->UseAction( nIndex );
				}
				break;

			case 1:		// 공격
				{
#if !(defined G_MAXICO) && !(defined G_BRAZIL)
					penPlayerEntity->CommandAttack();
#endif
				}
				break;

			case 4:		// 줍기	//0826
				{
					penPlayerEntity->SearchNearItem();
				}
				break;

			case 5:		// 교환
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						CUIManager::getSingleton()->GetExchange()->SendExchangeReq_Req( strTarget );
					}
				}
				break;

			case 27:	// 지도
				{
					CUIManager::getSingleton()->GetMap()->ToggleVisible();
				}
				break;

			case 28:	// 판매하기
				{
					// 인벤토리 락을 걸었다가 해제했다가 해야하기 때문에,
					// ToggleVisible()로 하면 안될거 같음...
					if ( _pNetwork->MyCharacterInfo.bWildPetRide )
					{
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString( _S(5314,"개인 상점을 개설할 수 없는 상태입니다."));	
						CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
						break;	
					}
					CUIManager::getSingleton()->GetPersonalShop()->OpenPersonalShop( TRUE );
					
				}
				break;				
				
			case 30:	// 후견인 신청.
				{
					CUIManager::getSingleton()->GetHelper()->OpenHelper( );					
				}
				break;
			case 43:	//ttos 몬스터 콤보
				{
					CUIManager::getSingleton()->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REQ);
				}
				break;
			case 46:	//아이템 제작
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					for (int i = 0; i < SKILL_SPECIAL_SLOT_ROW_TOTAL; i++)
					{
						if (!m_btnVoucherSkill[i].IsEmpty() && m_btnVoucherSkill[i].GetSkillIndex() != 655)
						{
							pUIManager->GetItemProduct()->SetExpertData(&m_btnVoucherSkill[i]);
						}
						
					}
					
					pUIManager->GetItemProduct()->OpenItemProduct();
					pUIManager->RearrangeOrder( UI_CHARACTERINFO, FALSE );
					
				}break;
			case 47:
				{
					//CUIManager::getSingleton()->GetAffinityInfo()->SetAffinityData();
					CUIManager::getSingleton()->GetAffinityInfo()->OpenAffinityInfo();
				}
				break;
			case 48:
				{
					CUIManager::getSingleton()->GetChatFilter()->OpenChatFilter();
				}break;
			case 49:
				{
					if (!CUIManager::getSingleton()->GetNickName()->IsVisible())
					{
						_pNetwork->SendNickNameMessage(MSG_EX_TITLE_SYSTEM_LIST_REQ, 0);
					}
				}
				break;
			case 51:
				{
					CUIManager::getSingleton()->GetRankingViewEx()->ToggleVisible();
					break;
				}
			case 52:
				{
					CUIManager::getSingleton()->GetCalendar()->OpenCalendar();
				}
				break;
			}
		}
		break;

	case ACTION_PET:		// 애완동물...
		{
			switch( lIndex )
			{
			case 35:		// 펫 소환
				{
					if( _pNetwork->MyCharacterInfo.bPetRide )
					{
						return;
					}					
					_pNetwork->CallPet(_pNetwork->_PetTargetInfo.lIndex);
				}
				break;
			case 36:		// 펫 마을 귀환
				{
					if( !_pNetwork->MyCharacterInfo.bPetRide )
					{
						return;
					}
					_pNetwork->SendPetWarpTown();
				}
				break;
			case 37:		// 펫 애완동물 휴식
				{
					if( !_pNetwork->MyCharacterInfo.bPetRide )
					{
						return;
					}
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( 3 );
				}
				break;
			case 38:		// 펫 고속 이동.
				{
					if( !_pNetwork->MyCharacterInfo.bPetRide )
					{
						return;
					}
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( 2 );					
				}
				break;
				
			case 40:		// 펫 아이템 줍기.
				{
					if( _pNetwork->MyCharacterInfo.bPetRide )
					{
						return;
					}
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UsePetPickItem();					
				}
				break;
			}
		}
		break;
	case ACTION_WILDPET:
		{
			switch( lIndex )
			{
			case 50:
				{
					if( CUIManager::getSingleton()->GetInventory()->IsWearing( WEAR_PET ) )
					{
						CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S(2189,"애완동물을 슬롯에 장착하여야만 합니다."), SYSMSG_ERROR );
						return;
					}
					if( (CUIManager::getSingleton()->GetInventory()->GetWearingBtn(WEAR_PET).GetItemFlag()&( FLAG_ITEM_SEALED )) )
 					{
						CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S(5348,"봉인된 상태에서는 탑승할 수 없습니다."), SYSMSG_ERROR );
						return;
					}
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( lIndex );					
				}break;
			}
		}break;

	case ACTION_SOCIAL:
		{
			// 애완동물을 타고 있을때는 소셜 액션이 불가능.
			if( !_pNetwork->MyCharacterInfo.bPetRide )
			{
				//물뿌리기 액션일 때...
				if( nIndex ==42 )
				{
					CPlayerEntity	*penPlayerEntity = ((CPlayerEntity*)CEntity::GetPlayerEntity(0));
					if( penPlayerEntity->CommandAction() )
						penPlayerEntity->UseSkill( 436 );
				}
				else
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction( nIndex );


				// [071211: Su-won] EVENT_NEWYEAR_2008
				if( IS_EVENT_ON(TEVENT_NEWYEAR_2008) )
				{
					if( nIndex == 10 ) 
					{
						if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER 
							&& _pNetwork->MyCharacterInfo.index != _pNetwork->_TargetInfo.pen_pEntity->GetNetworkID()
							)
						{
							_pNetwork->ClientSystemMessage( _S(3964, "새해복 많이 받으세요!"), SYSMSG_ERROR );

							CNetworkMessage nmSkill((UBYTE)MSG_SKILL);
							nmSkill<< (UBYTE)MSG_SKILL_NEWYEAR_2008;
							nmSkill<< _pNetwork->MyCharacterInfo.index;
							nmSkill<< _pNetwork->_TargetInfo.pen_pEntity->GetNetworkID();

							_pNetwork->SendToServerNew(nmSkill);
						}
					}
				}
			}
		}
		break;

	case ACTION_PARTY:
		{
			switch( lIndex )
			{
			case 6:		// 파티 신청(균등 분배)
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						CUIManager::getSingleton()->GetParty()->SendPartyInvite( PT_PEACEEVER, strTarget );
					}
				}
				break;
			case 7:		// 파티 신청(입수 우선)
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						CUIManager::getSingleton()->GetParty()->SendPartyInvite( PT_SURVIVAL, strTarget );
					}
				}
				break;
			case 8:		// 파티 신청(공격형)
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						CUIManager::getSingleton()->GetParty()->SendPartyInvite( PT_ATTACK, strTarget );
					}
				}
				break;

			case 9:		// 파티 탈퇴
				{
					CUIManager::getSingleton()->GetParty()->SendPartyQuit();
				}
				break;


			case 39:	// 파티장 위임 
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if( pUIManager->GetParty()->GetMemberCount() <= 0 ) // 파티중이 아니라면
						{
							break;
						}
						if( !pUIManager->GetParty()->AmILeader() ) // 내가 파티장이 아니라면
						{
							pUIManager->GetParty()->PartyError( MSG_PARTY_ERROR_NOT_BOSS );
							break;
						}
							
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						
						if( pUIManager->GetParty()->IsOurParty( strTarget ) ) // 우리 파티원일 때만 위임 처리 
						{		
							pUIManager->GetParty()->MandateBossReq( strTarget );
						}
						else  // 우리 파티원아니라면...
						{
							pUIManager->GetParty()->PartyError( MSG_PARTY_ERROR_NOT_PARTY_MEMBER );
						}
						
						
					}
				}
				break;

			case 12:		// 파티 강퇴
				{
					if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						CTString	strTarget = _pNetwork->_TargetInfo.TargetName;
						CUIManager::getSingleton()->GetParty()->SendPartyKick( strTarget );
					}
				}
				break;

			case 33:	// 파티 멤버창 토글
				{
					CUIManager::getSingleton()->GetParty()->ToggleVisible();
				}
				break;
#ifdef PARTY_AUTO_ENABLE
			case 41:	// 파티 오토 매칭 시스템
				{
					if( IsVisible() ) { ToggleVisible(); }
					CUIManager::getSingleton()->GetPartyAuto()->OpenPartyMatching();
				}
				break;
#endif
			}
		}
		break;

	case ACTION_GUILD:
		{
			switch( lIndex )
			{
				case 24:	// 가입하기
				{
					// 다른 길드에 가입되어 있으면 탈퇴를 물어본다.
					if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_NOMEMBER )
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_DESTROY);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
						pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
						
						CTString		strMessage;
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OK,		
							UI_GUILD, MSGCMD_GUILD_ERROR );
						if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_NOMEMBER)
						{
							strMessage.PrintF( _S( 866, "이미 길드에 가입되어 있습니다.\n먼저 가입되어 있는 길드에서 탈퇴하여 주십시오." ) );	
						}
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
					else if( _pNetwork->_TargetInfo.bIsActive && _pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						// Find index of character
						SLONG	slIndex = -1;
						INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
						for( INDEX iCharacter = 0; iCharacter < ctCha; iCharacter++ ) 
						{
							CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iCharacter];
							CEntity	*pEntity		= _pNetwork->_TargetInfo.pen_pEntity;
							if( pEntity )
							{
								if( ct.cha_iClientIndex == pEntity->en_ulID )
								{
									CUIManager* pUIManager = CUIManager::getSingleton();

									// FIXME : 요청에 의해서 임시적으로 막은 부분입니다.
									if( ct.cha_lGuildPosition == GUILD_MEMBER_BOSS || 
										ct.cha_lGuildPosition == GUILD_MEMBER_VICE_BOSS )
									{
										slIndex = ct.cha_Index;
										pUIManager->GetGuild()->JoinGuild( ct.cha_lGuildIndex, ct.cha_Index, ct.cha_strGuildName, ct.cha_iSyndicateType );
									}
									else
									{
										pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
										CUIMsgBox_Info	MsgBoxInfo;
										MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
										MsgBoxInfo.AddString( _S( 867, "길드장에게만 가입 신청이 가능합니다." ) );	
										pUIManager->CreateMessageBox( MsgBoxInfo );		
									}									
									break;
								}
							}
						}						
					}
				}
				break;

			case 25:	// 탈퇴하기
				{
					// 캐릭터가 단장이라면 탈퇴를 할 수 없음.
					if( _pNetwork->MyCharacterInfo.lGuildPosition > 0 && _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS)
					{	
						CUIManager::getSingleton()->GetGuild()->QuitGuild();
					}
				}
				break;
			
			case 26:	// 길드 관리
				{
					// 서버로 메세지를 보낸 후에, Receive한 곳에서 호출해야 함.
					// WSS_NEW_GUILD_SYSTEM 070704
					// WSS_TEST 
					// TODO : 임시 길드 창
					if( _pNetwork->MyCharacterInfo.lGuildLevel < LIMIT_GUILD_LEVEL )
						CUIManager::getSingleton()->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );						
					else 
						CUIManager::getSingleton()->GetGuild()->OpenGuildManagerNew();
				}
				break;
			
			case 34:// 임시 코드 길드 공지 테스트 
				{
					if( !_pNetwork->IsLord() ) 
					{
						return;
					}


					CTString strSysMessage;
					CUIMsgBox_Info	MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo( _S( 1886, "성주공지" ), UMBS_USER_12 | UMBS_INPUT_MASK | UMBS_ALIGN_RIGHT, UI_NONE, MSGCMD_GUILD_LORD_NOTICE, 300 ); 
					MsgBoxInfo.SetUserBtnName( _S( 1887, "공지" ), _S( 870, "닫기" ) ); 
					MsgBoxInfo.SetInputBox( 2, 2, 80, 235 );	
				
					strSysMessage.PrintF( _S( 1889, "공지할 내용을 입력하십시오." ) ); 
					MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
					
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
					
				}
				break;

			case 31 : // 길드 전투 신청 
				{
					// 타겟이 없거나 올바르지 않다면
					if( !_pNetwork->_TargetInfo.bIsActive || !_pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						return;
					}
					
					/*****
					// 존 정보 체크 
					if ( ZoneInfo().GetZoneType( g_slZone ) != ZONE_FIELD )
					{
						CUIManager::getSingleton()->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, 
										_S( 1085, "길두전투를 신청할 수 있는 지역이 아닙니다." ) );	
						return;
					}
					*****/
					
					// 내 길드 정보 체크 
					if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS )
					{
						CUIManager::getSingleton()->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, 
								_S( 1086, "길드장만 길드전투를 신청할 수 있습니다." ) );	
						return;
					}
					
/*					if ( CUIManager::getSingleton()->GetGuild()->GetGuildMemberCount() < GB_MIN_MEMBER )
					{
						CTString strMessage;
						//strMessage.PrintF ( _S( 1087, "길드 인원이 부족하여 전투를 신청할 수 없습니다.\n(최소인원 : %d명)" ), GB_MIN_MEMBER ); 
						strMessage.PrintF ( _s( "길드 인원이 부족하여 전투를 신청할 수 없습니다.\n(최소인원 : %d명)\n(현재인원: %d명" ),
							GB_MIN_MEMBER,  CUIManager::getSingleton()->GetGuild()->GetGuildMemberCount());
						
						CUIManager::getSingleton()->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, strMessage );
						return;
					}*/
									
					//!! 길드 전투 신청 
					INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
				
					for( INDEX iCharacter = 0; iCharacter < ctCha; iCharacter++ ) 
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iCharacter];
						CEntity	*pEntity		= _pNetwork->_TargetInfo.pen_pEntity;
						
						if( pEntity )
						{
							if( ct.cha_iClientIndex == pEntity->en_ulID ) // Find
							{
								CUIManager* pUIManager = CUIManager::getSingleton();

								if( ct.cha_lGuildPosition != GUILD_MEMBER_BOSS )
								{
									pUIManager->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, 
										_S( 1088, "길드장에게만 길드전투를 신청할 수 있습니다." ) );	
									return;
								}
								else 
								{
									pUIManager->GetGuildBattle()->OpenGBReq( ct.cha_Index, ct.cha_strGuildName );
								}
								break;
							}
						}
					}
				}
				break;

			case 32 : // 길드 전투 중단 신청 
				{
					// 타겟이 없거나 올바르지 않다면
					if( !_pNetwork->_TargetInfo.bIsActive || !_pNetwork->_TargetInfo.TargetType == CHARACTER )
					{
						return;
					}

					CUIManager* pUIManager = CUIManager::getSingleton();

					// 길드 중단 신청 
					// 내 길드 정보 체크 
					if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS )
					{
						pUIManager->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, 
								_S( 1089, "길드장만 길드 전투 중단을 신청할 수 있습니다." ) );	
						return;
					}
					
					if( pUIManager->GetGuildBattle()->IsInBattle() == FALSE )
					{
						pUIManager->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, _S( 1090, "길드 전투 중이 아닙니다." ) ); 
						return;
					}

					// 길드 취소 신청
					INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
				
					for( INDEX iCharacter = 0; iCharacter < ctCha; iCharacter++ ) 
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iCharacter];
						CEntity	*pEntity		= _pNetwork->_TargetInfo.pen_pEntity;
						
						if( pEntity )
						{
							if( ct.cha_iClientIndex == pEntity->en_ulID ) // Find
							{
								if( ct.cha_lGuildPosition != GUILD_MEMBER_BOSS )
								{
									pUIManager->GetGuildBattle()->GBErrorMessage ( MSGCMD_GUILD_BATTLE_ERROR, 
										_S( 1091, "길드장에게만 길드 전투중단을 신청할 수 있습니다." ) );	
									return;
								}
								else 
								{
									pUIManager->GetGuildBattle()->GBStopReq( ct.cha_Index, ct.cha_strGuildName );
								}
								break;
							}
						}
					}
				}
				break;
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : UseSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::UseSkill( int nIndex )
{
	if( _pNetwork->MyCharacterInfo.sbShopType != PST_NOSHOP || 
		CUIManager::getSingleton()->IsCSFlagOn( CSF_TELEPORT ) )
		return;

	// 강신스킬은 캐릭터 변환된 상태에서는 사용할 수 없다
	if(nIndex == 309 || nIndex == 313)
	{
		if(_pNetwork->MyCharacterInfo.eMorphStatus != CNetworkLibrary::MyChaInfo::eMORPH_END)
		{
			return;
		}
	}

	// 개인상점 시도중일 시 스킬시전하지 않음
	if(_pNetwork->m_iNetworkResponse[MSG_PERSONALSHOP] != 0)
		return;

	// 테스트용.
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseSkill( nIndex );
	//_pNetwork->CreateSlave( 0, CEntity::GetPlayerEntity(0), 1 );
}

// ----------------------------------------------------------------------------
// Name : ClearSkills()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ClearSkills()
{
	int		iRow;
	for( iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
		m_btnActiveSkill[iRow].InitBtn();

	for( iRow = 0; iRow < SKILL_PASSIVE_SLOT_ROW_TOTAL; iRow++ )
		m_btnPassiveSkill[iRow].InitBtn();
#ifdef ADJUST_MEMORIZE_SKILL
	for( iRow = 0; iRow < SKILL_MEMORIZE_SLOT_ROW_TOTAL; iRow++ )
		m_btnMemorizeSkill[iRow].InitBtn();
#endif
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		m_btnVoucherSkill[iRow].InitBtn();
	}

	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		m_btnItemSpecialSkill[iRow].InitBtn();
	}
}

// ----------------------------------------------------------------------------
// Name : ClearSSkills()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::ClearSSkills()
{
	for( int iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
		m_btnSpecialSkill[iRow].InitBtn();
}

// ----------------------------------------------------------------------------
// Name : ClearSkills()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::UpdateSkillLevel( int nIndex, SBYTE sbLevel, BOOL bSpecial )
{
	int		iRow;
	if( !bSpecial )
	{
		for( iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnActiveSkill[iRow].GetSkillIndex() == nIndex )
			{
				m_btnActiveSkill[iRow].SetSkillLevel( sbLevel );
				return;
			}
		}
		
		for( iRow = 0; iRow < SKILL_PASSIVE_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnPassiveSkill[iRow].GetSkillIndex() == nIndex )
			{
				m_btnPassiveSkill[iRow].SetSkillLevel( sbLevel );
				return;
			}
		}

		for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnItemSpecialSkill[iRow].GetSkillIndex() == nIndex )
			{
				m_btnItemSpecialSkill[iRow].SetSkillLevel( sbLevel );
				return;
			}
		}

#ifdef ADJUST_MEMORIZE_SKILL
		for( iRow = 0; iRow < SKILL_MEMORIZE_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnMemorizeSkill[iRow].GetSkillIndex() == nIndex )
			{
				m_btnMemorizeSkill[iRow].SetSkillLevel( sbLevel );
				return;
			}
		}
#endif
	}
	else
	{
		for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnSpecialSkill[iRow].GetSkillIndex() == nIndex )
			{
				m_btnSpecialSkill[iRow].SetSkillLevel( sbLevel );
				return;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : StartSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::StartSkillDelay( int nIndex )
{
	for( int iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnActiveSkill[iRow].GetSkillIndex() == nIndex )
		{
			m_btnActiveSkill[iRow].SetSkillDelay( TRUE );
			CUIManager::getSingleton()->GetSkillNew()->SetSkillCoolTime(nIndex);
			break;
		}
 		else if( iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL && m_btnItemSpecialSkill[iRow].GetSkillIndex() == nIndex )
		{
 			m_btnItemSpecialSkill[iRow].SetSkillDelay( TRUE );
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_EVENT_PRIZE:
		if( bOK )
		{
			_pNetwork->SendEventPrize();
		}
		break;
	case MSGCMD_QUEST_COMPLETE:
		if( bOK )
		{			
			_pNetwork->GoZone(0, 0);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{	
	case MSGLCMD_QUEST_TUTORIAL: // MSG_QUEST_PRIZE 에서만 처리
		{
			CUIManager::getSingleton()->GetQuest()->LockQuest(FALSE);
			_pNetwork->GoZone(0, 0); // 새로운 튜토리얼 모드에서 신규 캐릭터는 이동 위치가 새로운 위치이다.(서버에서 문앞으로 보내준다.)
		}
		break;
	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendUseStatPoint()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::SendUseStatPoint( UBYTE ubStatType )
{
	if( m_bLockStatPoint == TRUE )
		return;

	_pNetwork->SendUseStatPoint( ubStatType );
	m_bLockStatPoint = TRUE;
}

// ----------------------------------------------------------------------------
// Name : SendQuestReq()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUICharacterInfo::SendQuestReq( int nNpcIndex )
{
	_pNetwork->SendQuestMessage( MSG_QUEST_REQUEST, nNpcIndex );
	m_bLockRequestQuest = TRUE;
}
*/

// ----------------------------------------------------------------------------
// Name : SendQuestCancel()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUICharacterInfo::SendQuestCancel( int nQuestIndex )
{
	_pNetwork->SendQuestMessage( MSG_QUEST_GIVEUP, nQuestIndex );
}
*/

// ----------------------------------------------------------------------------
// Name : SendQuestStart()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUICharacterInfo::SendQuestStart( int nQuestIndex )
{
	_pNetwork->SendQuestMessage( MSG_QUEST_START, nQuestIndex );
}
*/

// ----------------------------------------------------------------------------
// Name : SendQuestPrize()
// Desc :
// ----------------------------------------------------------------------------
//void CUICharacterInfo::SendQuestPrize( int nQuestIndex )
//{
	//_pNetwork->SendQuestMessage( MSG_QUEST_PRIZE, nQuestIndex );
//}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : NotifyStatPoint()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::NotifyStatPoint()
{
	// Update states of buttons
	if( _pNetwork->MyCharacterInfo.StatPoint > 0 )
	{
		m_btnGPStrength.SetEnable( TRUE );
		m_btnGPDexterity.SetEnable( TRUE );
		m_btnGPIntelligence.SetEnable( TRUE );
		m_btnGPConstitution.SetEnable( TRUE );
	}
	else
	{
		m_btnGPStrength.SetEnable( FALSE );
		m_btnGPDexterity.SetEnable( FALSE );
		m_btnGPIntelligence.SetEnable( FALSE );
		m_btnGPConstitution.SetEnable( FALSE );
	}

	// Update strings of stats
	m_strStatPoint.PrintF( "%d", _pNetwork->MyCharacterInfo.StatPoint );
}

// ----------------------------------------------------------------------------
// Name : UseStatPoint()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::UseStatPoint( UBYTE ubStatType, SLONG slStatChange )
{
	// Update states of buttons & strings of stats
	NotifyStatPoint();

	// System message
	CTString	strMessage;
	switch( ubStatType )
	{
	case MSG_STATPOINT_USE_STR:
		{
			strMessage.PrintF( _S( 438, "힘이 %d 상승하였습니다." ), slStatChange );
			updateStatusPt(MSG_STATPOINT_USE_STR, slStatChange);
		}
		break;
	case MSG_STATPOINT_USE_DEX:
		{
			strMessage.PrintF( _S( 439, "민첩이 %d 상승하였습니다." ), slStatChange );
			updateStatusPt(MSG_STATPOINT_USE_DEX, slStatChange);
		}
		break;
	case MSG_STATPOINT_USE_INT:
		{
			strMessage.PrintF( _S( 440, "지혜가 %d 상승하였습니다." ), slStatChange );
			updateStatusPt(MSG_STATPOINT_USE_INT, slStatChange);
		}
		break;
	case MSG_STATPOINT_USE_CON:
		{
			strMessage.PrintF( _S( 441, "체질이 %d 상승하였습니다." ), slStatChange );
			updateStatusPt(MSG_STATPOINT_USE_CON, slStatChange);
		}
		break;
	}

	CUIManager::getSingleton()->GetChatting()->AddSysMessage( strMessage );

	UnlockStatPoint();
}

// ----------------------------------------------------------------------------
// Name : StatPointError()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::StatPointError( SBYTE sbError )
{
	// System message
	CTString	strMessage;	

	switch( sbError )
	{
	case MSG_STATPOINT_ERROR_NOTENOUGH:
		strMessage = _S( 1092, "스탯 포인트가 부족합니다." );		
		break;
	case MSG_STATPOINT_ERROR_CANTUSE:
		strMessage = _S( 1093, "스탯 포인트를 사용할 수 없습니다." );		
		break;
	case MSG_STATPOINT_ERROR_WRONGPACKET:
		strMessage = _S( 5608, "전직 이후 사용할 수 있습니다." );		
		break;
	case MSG_STATPOINT_ERROR_NOMONEY:
		strMessage = _S( 1095, "돈이 부족합니다." );		
		break;
	default:
		ASSERTALWAYS("Invalid Stat Point Message!!!");
		break;
	}

	CUIManager::getSingleton()->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );

	UnlockStatPoint();
}

// ----------------------------------------------------------------------------
// Name : AddSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::AddSkill( int nSkillIndex, SBYTE sbSkillLevel, BOOL bSpecial )
{
	if(!bSpecial)
	{
		const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);
		CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );

		const int iJob = _pNetwork->MyCharacterInfo.job;
		const int iJob2 = _pNetwork->MyCharacterInfo.job2;

		// 소환스킬 인덱스 찾구,
		// 소환수 사용 스킬.
		if( iJob == SORCERER && 
			iJob2 == 2 && rSelSkill.GetType() != CSkill::ST_PASSIVE )
		{
			if( ( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) ) )		
				return;		
			else
			{
				for( int i = 0; i < iSummonSkillCount; ++i )
				{
					if( _aSummonSkill[i] == nSkillIndex )
					{					
						return;
					}
				}
			}
		}

		switch( rSelSkill.GetType() )
		{
		case CSkill::ST_MELEE:			// Active skill
		case CSkill::ST_RANGE:			// Active skill
		case CSkill::ST_MAGIC:			// Active skill
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				int	iRow;
				for( iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
				{
					if( !(rSelSkill.GetFlag() & SF_ITEM_SPECIAL_SKILL) )
					{
						if( m_btnActiveSkill[iRow].IsEmpty() )
						{
							m_btnActiveSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel );
							//m_sbActiveSkillScrollBar.SetCurItemCount( iRow + 1 );
							break;
						}
					}
					else
					{
						if (iRow >= SKILL_SPECIAL_SLOT_ROW_TOTAL)
							continue;

						if (m_btnItemSpecialSkill[iRow].IsEmpty())
						{
							m_btnItemSpecialSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel );
							break;
						}
					}
				}
			}
			break;
		case CSkill::ST_PASSIVE:		// Passive skill
			{
				int	iRow;
				for( iRow = 0; iRow < SKILL_PASSIVE_SLOT_ROW_TOTAL; iRow++ )
				{
					if( !(rSelSkill.GetFlag() & SF_ITEM_SPECIAL_SKILL) )
					{
						if( m_btnPassiveSkill[iRow].IsEmpty() )
						{
							m_btnPassiveSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel );
							//m_sbPassiveSkillScrollBar.SetCurItemCount( iRow + 1 );
							break;
						}
					}
					else
					{
						if (iRow >= SKILL_SPECIAL_SLOT_ROW_TOTAL)
							continue;

						if( m_btnItemSpecialSkill[iRow].IsEmpty() )
						{
							m_btnItemSpecialSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel );
							break;
						}
					}
				}
			}
			break;
		case CSkill::ST_SEAL:
			{
				int	iRow;
				for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
				{
					if( m_btnVoucherSkill[iRow].IsEmpty() )
					{
						m_btnVoucherSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel );
						m_sbVoucherSkillScrollBar.SetCurItemCount( iRow + 1 );
						break;
					}
				}

			}break;
		}
	}	
	else
	{
		int	iRow;
		for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnSpecialSkill[iRow].IsEmpty() )
			{
				m_btnSpecialSkill[iRow].SetSkillInfo( nSkillIndex, sbSkillLevel, bSpecial );
				m_sbSpecialSkillScrollBar.SetCurItemCount( iRow + 1 );
				break;
			}
		}
	}
}

void CUICharacterInfo::AddSeal( int nSkillIndex, SQUAD nSealExp )
{
	int	iRow;
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnVoucherSkill[iRow].IsEmpty() )
		{
			m_btnVoucherSkill[iRow].SetSkillInfo( nSkillIndex, 0 );
			m_btnVoucherSkill[iRow].SetSkillSealExp(nSealExp);
			m_sbVoucherSkillScrollBar.SetCurItemCount( iRow + 1 );
			break;
		}
	}
}

void CUICharacterInfo::UpdateSealExp(int nSealIndex, SQUAD lSealExp )
{
	for(int iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnVoucherSkill[iRow].GetSkillIndex() == nSealIndex )
		{
			m_btnVoucherSkill[iRow].SetSkillInfo( nSealIndex, 0 );
			m_btnVoucherSkill[iRow].SetSkillSealExp(lSealExp);			
			break;
		}
	}
}
// ----------------------------------------------------------------------------
// Name : CheckSSkill()
// Desc : 특수 스킬을 갖고 있는지 확인합니다.
// ----------------------------------------------------------------------------
int CUICharacterInfo::CheckSSkill( int nSSkillType )
{
	int	iRow;
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		if( !m_btnSpecialSkill[iRow].IsEmpty() )
		{
			const int iSSkillIndex		= m_btnSpecialSkill[iRow].GetSkillIndex();
			CSpecialSkill &rSkillData	= _pNetwork->GetSSkillData(iSSkillIndex);
			if(rSkillData.GetType() == nSSkillType)
				return iSSkillIndex;
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
// Name : CheckSSkill()
// Desc : 특수 스킬을 갖고 있는지 인덱스를 통해서 확인합니다.
// ----------------------------------------------------------------------------
BOOL CUICharacterInfo::CheckSSkillByIndex( int iIndex, int nNeedLevel, BOOL* bNeedLevel )
{
	int	iRow;
	CSpecialSkill  NeedSSkill = _pNetwork->GetSSkillData ( iIndex );

	*bNeedLevel = FALSE;

	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		if( !m_btnSpecialSkill[iRow].IsEmpty() )
		{
			CSpecialSkill  CharSSkill = _pNetwork->GetSSkillData ( m_btnSpecialSkill[iRow].GetSkillIndex() );

			// 타입 체크 
			if ( NeedSSkill.GetType() == CharSSkill.GetType() )
			{
				// Preference 체크 
				if ( NeedSSkill.GetPreference() != -1 ) 
				{
					if ( NeedSSkill.GetPreference() < CharSSkill.GetPreference() ) 
					{
						*bNeedLevel = TRUE;
						return TRUE;
					}
					else if ( NeedSSkill.GetPreference() == CharSSkill.GetPreference() )
					{
						// 레벨 체크 
						if ( GetSkillLevel ( CharSSkill.GetIndex(), TRUE ) >= nNeedLevel )
						{
							*bNeedLevel = TRUE;
							return TRUE;
						}
					}
				}
				else // 아니면 인덱스 체크 
				{
					if ( CharSSkill.GetIndex() == iIndex)
					{
						// 레벨 체크 
						if ( GetSkillLevel ( CharSSkill.GetIndex(), TRUE ) >= nNeedLevel )
						{
							*bNeedLevel = TRUE;
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : RemoveSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RemoveSkill( int nSkillIndex, BOOL bSpecial )
{
	if(!bSpecial)
	{
		int	iRow;
		for( iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnActiveSkill[iRow].GetSkillIndex() == nSkillIndex )
			{
				m_btnActiveSkill[iRow].InitBtn();
			}
		}
	}
	// Dongmin
	else
	{
		int	iRow;
		for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnSpecialSkill[iRow].GetSkillIndex() == nSkillIndex )
			{
				m_btnSpecialSkill[iRow].InitBtn();
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RemoveSummonSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUICharacterInfo::RemoveSummonSkill()
{	
	// 소환 스킬 목록.	
	const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);
	int	iRow;
	for( iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
	{		
		const int iSkillIndex = m_btnActiveSkill[iRow].GetSkillIndex();
		if( iSkillIndex == -1 ) continue;
		CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );

		// 소환스킬 인덱스 찾구,
		// 소환수 사용 스킬.
		if( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) )
		{
			m_btnActiveSkill[iRow].InitBtn();
		}
		else
		{
			for( int i = 0; i < iSummonSkillCount; ++i )
			{
				if( _aSummonSkill[i] == iSkillIndex )
				{
					m_btnActiveSkill[iRow].InitBtn();
					break;
				}
			}
		}
	}
}

BOOL CUICharacterInfo::GetSkillDelay( int nIndex )
{
	for( int iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnActiveSkill[iRow].GetSkillIndex() == nIndex )
		{
			if( m_btnActiveSkill[iRow].GetSkillDelay() )
				return TRUE;
			else 
				return FALSE;
		}
		else if( iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL && m_btnItemSpecialSkill[iRow].GetSkillIndex() == nIndex )
		{
			if ( m_btnItemSpecialSkill[iRow].GetSkillDelay() )
				return TRUE;
			else 
				return FALSE;
		}
	}
	return FALSE;
}

void CUICharacterInfo::UpdateExpAndSp(void)
{
	m_strExp.PrintF( "%I64d/%I64d", _pNetwork->MyCharacterInfo.curExp, _pNetwork->MyCharacterInfo.needExp );
	m_strSP.PrintF( "%d", _pNetwork->MyCharacterInfo.sp / 10000 );	
}

UQUAD CUICharacterInfo::GetSealExp(int nIndex)
{
	for( int iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnVoucherSkill[iRow].GetSkillIndex() == nIndex )
		{
			return m_btnVoucherSkill[iRow].GetSkillSealExp();
		}
	}
	return 0;
}


CUIButtonEx* CUICharacterInfo::GetSkillBtn( int idx )
{
	for( int iRow = 0; iRow < SKILL_ACTIVE_SLOT_ROW_TOTAL; iRow++ )
	{
		if( m_btnActiveSkill[iRow].GetSkillIndex() == idx )
		{
			return &m_btnActiveSkill[iRow];
		}
	}
	return NULL;
}

void CUICharacterInfo::initialize()
{
	m_bStatusTabOpen[STATUS_TAB_PKINFO_N_ATTRIBUTE] = 0;
	m_bStatusTabOpen[STATUS_TAB_PHYSICAL_N_MAGICAL] = 0;

	for(int i=0; i<3; i++)
	{
		m_nSkillActionUpperTab[i] = 0;
	}

	m_rcIcons.SetRect( 0, 0, SKILLINFO_NEW_WIDTH, SKILLINFO_NEW_HEIGHT);
	m_rcTitle.SetRect( 0, 0, CHARINFO_NEW_WIDTH, 36 );
	//사이드 탭 영역
	m_rcCharInfoTab.SetRect(7, 37, 28, 246);

	//탭 그리는 크기
	m_rcTabLayer.SetRect(7, 37, 28, 87);
	m_rcInfoRegion.SetRect(28, 37, 506, 330);
	m_rcStatusTab[STATUS_TAB_PKINFO_N_ATTRIBUTE].SetRect(33, 240, 263, 261);
	m_rcStatusTab[STATUS_TAB_PHYSICAL_N_MAGICAL].SetRect(270, 150, 500, 171);

	//m_rcSkillActionTab.SetRect(34, 44, 233, 64);
	m_rcSkillActionTab.SetRect(34, 44, 298, 64);
	m_rcCharInfoDesc.SetRect(0, 0, 100, 30);

	m_rsSkillName.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);
	m_rsSkillDesc.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rsCurrentSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);
	m_rsNextSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);

	m_rcSkillInfo.SetRect(0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rcButtonArea.SetRect(0, 0, 212, 34);

	// Skill buttons
	// Active skill
	int		iRow;

	// Special skill
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_btnSpecialSkill[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_SKILL, 0, iRow );
	}
	// Voucher skill
	for( iRow = 0; iRow < SKILL_SPECIAL_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_btnVoucherSkill[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_SKILL, 0, iRow );
	}

	// Action buttons
	// Normal action button
	for( iRow = 0; iRow < ACTION_NORAML_SLOT_ROW_TOTAL ; iRow++ )
	{

		m_btnNormalAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_ACTION, 0, iRow);
	}
	// Social action button
	for( iRow = 0; iRow < ACTION_SOCIAL_SLOT_ROW_TOTAL ; iRow++ )
	{

		m_btnSocialAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_ACTION, 0, iRow);
	}
	// Party action button
	for( iRow = 0; iRow < ACTION_PARTY_SLOT_ROW_TOTAL ; iRow++ )
	{

		m_btnPartyAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_ACTION, 0, iRow);
	}
	// Guild action button
	for( iRow = 0; iRow < ACTION_GUILD_SLOT_ROW_TOTAL ; iRow++ )
	{

		m_btnGuildAction[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_CHARACTERINFO,
			UBET_ACTION, 0, iRow);
	}
}

void CUICharacterInfo::OnUpdate( float fElapsedTime )
{

}

void CUICharacterInfo::OnRender( CDrawPort* pDraw )
{

}

void CUICharacterInfo::updateStatusPt(int type, int value)
{
	switch( type )
	{
	case MSG_STATPOINT_USE_STR:
		{
			_pNetwork->MyCharacterInfo.str += value;
			m_strStrength.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.str,
				_pNetwork->MyCharacterInfo.opt_str );
		}
		break;
	case MSG_STATPOINT_USE_DEX:
		{
			_pNetwork->MyCharacterInfo.dex += value;
			m_strDexterity.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.dex,
				_pNetwork->MyCharacterInfo.opt_dex );
		}
		break;
	case MSG_STATPOINT_USE_INT:
		{
			_pNetwork->MyCharacterInfo.intel += value;
			m_strIntelligence.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.intel,
				_pNetwork->MyCharacterInfo.opt_intel );
		}
		break;
	case MSG_STATPOINT_USE_CON:
		{
			_pNetwork->MyCharacterInfo.con += value;
			m_strConstitution.PrintF( "%3d (%2d)", _pNetwork->MyCharacterInfo.con,
				_pNetwork->MyCharacterInfo.opt_con );
		}
		break;
	}
	
	m_strStatPoint.PrintF( "%d", _pNetwork->MyCharacterInfo.StatPoint );
}
