#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIPartyAuto.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>

extern INDEX		g_iCountry;
extern CDrawPort	*_pdpMain;

#define PT_JOIN_WIDTH	215
#define PT_JOIN_HEIGHT	165
#define PT_LEADER_JOIN_WIDTH	400
#define PT_LEADER_JOIN_HEIGHT	200
#define PT_PARTY_LIST_WIDTH		600
#define PT_PARTY_LIST_HEIGHT	450

#define MAX_PARTY_NAME_LEN	20	// [2012/04/03 : Sora]  파티 오토매칭 파티명 최대길이 수정

// Window basic size
#define WBS_TITLE_HEIGHT	22
#define WBS_GAP_HEIGHT		3
#define WBS_BOTTOM_HEIGHT	3

#define WBS_JOIN_HEIGHT		134
#define WBS_LEADER_JOIN_HEIGHT_1	141
#define WBS_LEADER_JOIN_HEIGHT_2	30

#define WBS_PARTY_LIST_BGD_1	398
#define WBS_PARTY_LIST_BGD_2	352
#define WBS_PARTY_LIST_BGD_WIDTH	576

// check box TAB
#define CB_TAB		40

// UV Pos
#define UVP_TAB_X	96
#define UVP_TAB_Y   25	

// Rect Select Tab
#define RST_HEIGHT_TAB	26

// Menu Tab( center align )
#define NUMBER_POS		42			// 42
#define SUBJECT_STR		154			// 165
#define LIMIT_DIFF		273			// 273
#define ZONE_POS		348			// 348
#define NEED_CLASS		469			// 469
#define PARTY_TYPE		550			// 550

// Class Image size ( n*n )
#define CLASS_IMAGE_SIZE	18
#define LEVELDIFF_IMAGE_SIZE 22

// LIST EACH START HEGIHT
#define LESH_NUMBER			66
#define LESH_STRING			65
#define LESH_LEVELDIFF		59
#define LESH_IMAGE			62

// TEXT SCROLL DELAY TIME
#define TEXT_DELAY_TIME		200



// ----------------------------------------------------------------------------
// Name : CUIPartyAuto()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPartyAuto::CUIPartyAuto()
{
	m_nSelectState = -1;
	m_nCurrentPageNum = 0;
	m_FrontChPos = 0;
	m_bTextDir = TRUE;
	m_vectorPartyListData.clear();
	m_ptdAddTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIParty()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPartyAuto::~CUIPartyAuto()
{
	if( m_ptdAddTexture )
	{
		_pTextureStock->Release( m_ptdAddTexture );
		m_ptdAddTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	INDEX tv_chat,tv_fame,tv_auto,tv_summon;
	extern BOOL g_bIsMalEng;

	switch(g_iCountry)
	{
	case KOREA:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	case THAILAND:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	case USA:
	case ENGLAND:
		tv_chat=tv_summon=0;
		tv_auto=1;
		tv_fame=3;
		break;
	case BRAZIL:
	case GERMANY:
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
	case FRANCE:
	case POLAND:
	case RUSSIA:
	case TURKEY:
	case MEXICO:
	case ITALY:
	case USA_FRANCE:
	case USA_SPAIN:
	case NETHERLANDS:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	}

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	// join set
	m_rcJoinTitle.SetRect( 0, 0, 215, 22 );
	m_rcSelLine.SetRect( 27, 58, 571, 317 );

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtJoinTopL.SetUV( 0, 0, 21, 21, fTexWidth, fTexHeight );
	m_rtJoinTopM.SetUV( 22, 0, 193, 21, fTexWidth, fTexHeight );
	m_rtJoinTopR.SetUV( 194, 0, 215, 21, fTexWidth, fTexHeight );

	m_rtJoinGapL.SetUV( 0, 22, 21, 25, fTexWidth, fTexHeight );
	m_rtJoinGapM.SetUV( 22, 22, 193, 25, fTexWidth, fTexHeight );
	m_rtJoinGapR.SetUV( 194, 22, 215, 25, fTexWidth, fTexHeight );

	m_rtJoinMiddleL.SetUV( 0, 30, 21, 33, fTexWidth, fTexHeight );
	m_rtJoinMiddleM.SetUV( 22, 30, 193, 33, fTexWidth, fTexHeight );
	m_rtJoinMiddleR.SetUV( 194, 30, 215, 33, fTexWidth, fTexHeight );

	m_rtJoinBottomL.SetUV( 0, 42, 2, 44, fTexWidth, fTexHeight );
	m_rtJoinBottomM.SetUV( 3, 42, 212, 44, fTexWidth, fTexHeight );
	m_rtJoinBottomR.SetUV( 213, 42, 215, 44, fTexWidth, fTexHeight );

	m_rtStrTooltipTopL.SetUV( 164, 46, 165, 47, fTexWidth, fTexHeight );
	m_rtStrTooltipTopM.SetUV( 166, 46, 180, 47, fTexWidth, fTexHeight );
	m_rtStrTooltipTopR.SetUV( 181, 46, 182, 47, fTexWidth, fTexHeight );

	m_rtStrTooltipMiddleL.SetUV( 164, 48, 165, 62, fTexWidth, fTexHeight );
	m_rtStrTooltipMiddleM.SetUV( 166, 48, 180, 62, fTexWidth, fTexHeight );
	m_rtStrTooltipMiddleR.SetUV( 181, 48, 182, 62, fTexWidth, fTexHeight );

	m_rtStrTooltipBottomL.SetUV( 164, 63, 165, 64, fTexWidth, fTexHeight );
	m_rtStrTooltipBottomM.SetUV( 166, 63, 180, 64, fTexWidth, fTexHeight );
	m_rtStrTooltipBottomR.SetUV( 181, 63, 182, 64, fTexWidth, fTexHeight );

	m_btnEqualDivide.Create( this, _S(4743, "일반 파티" ), 59, 58, 95, 21 );
	m_btnEqualDivide.SetUV( UBS_IDLE, 134, 117, 228, 137, fTexWidth, fTexHeight );
	m_btnEqualDivide.SetUV( UBS_CLICK, 134, 139, 228, 159, fTexWidth, fTexHeight );
	m_btnEqualDivide.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEqualDivide.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnFight.Create( this, _S( 2662, "전투형 파티" ), 59, 106, 95, 21 );
	m_btnFight.SetUV( UBS_IDLE, 134, 117, 228, 137, fTexWidth, fTexHeight );
	m_btnFight.SetUV( UBS_CLICK, 134, 139, 228, 159, fTexWidth, fTexHeight );
	m_btnFight.CopyUV( UBS_IDLE, UBS_ON );
	m_btnFight.CopyUV( UBS_IDLE, UBS_DISABLE );	

#ifdef DISABLE_PARTY_TYPE_BATTLE	// [2010/07/12 : Sora] 전투형 파티 제거
	m_btnFight.SetEnable(FALSE);
#endif

	m_btnRegistOK.Create( this, _S( 191, "확인" ), 131, 171, 63, 21 );
	m_btnRegistOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnRegistOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnRegistOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRegistOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnRegistCancel.Create( this, _S( 139, "취소" ), 206, 171, 63, 21 );
	m_btnRegistCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnRegistCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnRegistCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRegistCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	int nStrWidth = ( _S( 43, "타이탄" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 타이탄
	m_cbtnTitan.Create( this, 121+CB_TAB, 74, 11, 11, _S( 43, "타이탄" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnTitan.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnTitan.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnTitan.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnTitan.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnTitan.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnTitan.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnTitan.SetCheck( FALSE );

	m_rtTitanImage.SetUV( 254, 178, 275, 199, fTexWidth, fTexHeight );
	
	nStrWidth = ( _S( 44, "기사" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 기사
	m_cbtnKnight.Create( this, 215+CB_TAB, 74, 11, 11, _S( 44, "기사" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnKnight.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnKnight.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnKnight.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnKnight.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnKnight.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnKnight.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnKnight.SetCheck( FALSE );

	m_rtKnightImage.SetUV( 279, 178, 300, 199, fTexWidth, fTexHeight );

	nStrWidth = ( _S( 45, "힐러" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 힐러
	m_cbtnHealer.Create( this, 312+CB_TAB, 74, 11, 11, _S( 45, "힐러" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnHealer.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnHealer.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnHealer.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnHealer.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnHealer.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnHealer.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnHealer.SetCheck( FALSE );

	m_rtHealerImage.SetUV( 304, 178, 325, 199, fTexWidth, fTexHeight );

	nStrWidth = ( _S( 46, "메이지" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 메이지
	m_cbtnMage.Create( this, 121+CB_TAB, /*103*/74+UVP_TAB_Y, 11, 11, _S( 46, "메이지" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnMage.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnMage.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnMage.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnMage.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnMage.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnMage.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnMage.SetCheck( FALSE );

	m_rtMageImage.SetUV( 254, 203, 275, 224, fTexWidth, fTexHeight );

	nStrWidth = ( _S( 47, "로그" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 로그
	m_cbtnRogue.Create( this, 215+CB_TAB, /*103*/74+UVP_TAB_Y, 11, 11, _S( 47, "로그" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnRogue.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnRogue.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnRogue.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnRogue.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnRogue.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnRogue.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnRogue.SetCheck( FALSE );

	m_rtRogueImage.SetUV( 279, 203, 300, 224, fTexWidth, fTexHeight );

	nStrWidth = ( _S( 48, "소서러" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 소서러
	m_cbtnSorcerer.Create( this, 312+CB_TAB, /*103*/74+UVP_TAB_Y, 11, 11, _S( 48, "소서러" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnSorcerer.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnSorcerer.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnSorcerer.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnSorcerer.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnSorcerer.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnSorcerer.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnSorcerer.SetCheck( FALSE );

	m_rtSorcererImage.SetUV( 304, 203, 325, 224, fTexWidth, fTexHeight );

	nStrWidth = ( _S( 4410, "나이트셰도우" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// 나이트쉐도우
	m_cbtnNightShadow.Create( this, 121+CB_TAB, 74+(UVP_TAB_Y*2), 11, 11, _S( 4410, "나이트셰도우"  ), TRUE, CB_TAB+35, CB_TAB );
	m_cbtnNightShadow.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnNightShadow.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnNightShadow.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnNightShadow.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnNightShadow.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnNightShadow.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnNightShadow.SetCheck( FALSE );

	m_rtNightShadowImage.SetUV( 471, 131, 492, 152, fTexWidth, fTexHeight );


#ifdef CHAR_EX_ROGUE
	// [2012/08/27 : Sora] EX로그 추가
	m_cbtnEXRogue.Create( this, 215+CB_TAB, 74+(UVP_TAB_Y*2), 11, 11, _S(5732,"EX로그"), TRUE, CB_TAB, CB_TAB );
	m_cbtnEXRogue.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnEXRogue.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnEXRogue.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnEXRogue.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnEXRogue.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnEXRogue.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnEXRogue.SetCheck( FALSE );

	m_rtEXRogueImage.SetUV( 472, 107, 492, 127, fTexWidth, fTexHeight );
#endif
#ifdef CHAR_EX_MAGE
	//	2013/01/08 jeil EX메이지 추가 및 스트링 나오면 다시 수정 필요 
	m_cbtnEXMage.Create( this, 312+CB_TAB, 74+(UVP_TAB_Y*2), 11, 11, _S( 5820, "아크메이지" ), TRUE, CB_TAB, CB_TAB );
	m_cbtnEXMage.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnEXMage.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnEXMage.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnEXMage.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnEXMage.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnEXMage.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnEXMage.SetCheck( FALSE );
	
	m_rtEXMageImage.SetUV( 447, 107, 467, 127, fTexWidth, fTexHeight );
#endif
	// 레벨 제한
	m_cbtnLevelLimit.Create( this, 98, 131, 11, 11, CTString( "" ) );
	m_cbtnLevelLimit.SetUV( UCBS_NONE, 12, 161, 22, 171, fTexWidth, fTexHeight );
	m_cbtnLevelLimit.SetUV( UCBS_CHECK, 0, 161, 10, 171, fTexWidth, fTexHeight );
	m_cbtnLevelLimit.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE);
	m_cbtnLevelLimit.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE);
	m_cbtnLevelLimit.SetTextColor( TRUE, 0xFFFFFFFF );
	m_cbtnLevelLimit.SetTextColor( FALSE, 0xFFFFFFFF );
	m_cbtnLevelLimit.SetCheck( FALSE );

	// Input box
	m_ebNoticeName.Create( this, 98, 39, 262, 16, MAX_PARTY_NAME_LEN );
	m_ebNoticeName.SetFocus( FALSE );
	m_ebNoticeName.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebNoticeName.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	m_rtNoticeNameL.SetUV( 55, 162, 56, 177, fTexWidth, fTexHeight );
	m_rtNoticeNameM.SetUV( 57, 162, 69, 177, fTexWidth, fTexHeight );
	m_rtNoticeNameR.SetUV( 70, 162, 71, 177, fTexWidth, fTexHeight );

	// close button
	m_btnClose.Create( this, CTString( "" ), PT_JOIN_WIDTH-27, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_ptdAddTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	fTexWidth = m_ptdAddTexture->GetPixWidth();
	fTexHeight = m_ptdAddTexture->GetPixHeight();

	m_rtSelListBackTopL.SetUV( 11, 49, 12, 50, fTexWidth, fTexHeight );
	m_rtSelListBackTopM.SetUV( 13, 49, 75, 50, fTexWidth, fTexHeight );
	m_rtSelListBackTopR.SetUV( 76, 49, 77, 50, fTexWidth, fTexHeight );

	m_rtSelListBackMiddleL.SetUV( 11, 51, 12, 59, fTexWidth, fTexHeight );
	m_rtSelListBackMiddleM.SetUV( 13, 51, 75, 59, fTexWidth, fTexHeight );
	m_rtSelListBackMiddleR.SetUV( 76, 51, 77, 59, fTexWidth, fTexHeight );

	m_rtSelListBackBottomL.SetUV( 11, 60, 12, 61, fTexWidth, fTexHeight );
	m_rtSelListBackBottomM.SetUV( 13, 60, 75, 61, fTexWidth, fTexHeight );
	m_rtSelListBackBottomR.SetUV( 76, 60, 77, 61, fTexWidth, fTexHeight );

	m_rtSelListMiddleL.SetUV( 0, 30, 21, 40, fTexWidth, fTexHeight );
	m_rtSelListMiddleM.SetUV( 22, 30, 66, 40, fTexWidth, fTexHeight );
	m_rtSelListMiddleR.SetUV( 67, 30, 88, 40, fTexWidth, fTexHeight );

	m_rtListSubjectRect.SetUV( 90, 25, 106, 41, fTexWidth, fTexHeight );
	m_rtline.SetUV( 90, 18, 98, 18, fTexWidth, fTexHeight );
	m_rtSelectBar.SetUV( 109, 30, 120, 42, fTexWidth, fTexHeight );
#ifdef CHAR_EX_ROGUE	//2013/01/08 jeil EX메이지 추가 나중에 좌표값 추가 수정 필요 
#ifdef CHAR_EX_MAGE
	m_cmbNeedClass.Create( this, 89, 363, 103, 16, 87, 2, 15, 13, 10, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );	
#else
	m_cmbNeedClass.Create( this, 89, 363, 103, 16, 87, 2, 15, 13, 9, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );	
#endif
	
#else
#ifdef CHAR_EX_MAGE
	m_cmbNeedClass.Create( this, 89, 363, 103, 16, 87, 2, 15, 13, 9, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );	
#else
	m_cmbNeedClass.Create( this, 89, 363, 103, 16, 87, 2, 15, 13, 8, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );	
#endif
	
#endif

	m_cmbNeedClass.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cmbNeedClass.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cmbNeedClass.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cmbNeedClass.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	m_cmbNeedClass.AddString( _S( 506, "전체" ) );
	m_cmbNeedClass.AddString( _S( 43, "타이탄" ) );
	m_cmbNeedClass.AddString( _S( 44, "기사" ) );
	m_cmbNeedClass.AddString( _S( 45, "힐러" ) );
	m_cmbNeedClass.AddString( _S( 46, "메이지" ) );
	m_cmbNeedClass.AddString( _S( 47, "로그" ) );
	m_cmbNeedClass.AddString( _S( 48, "소서러" ) );
	m_cmbNeedClass.AddString( _S(4410, "나이트셰도우" ));
#ifdef CHAR_EX_ROGUE
	m_cmbNeedClass.AddString( _S( 5732, "EX로그" ) );	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_cmbNeedClass.AddString( _S( 5820, "아크메이지" ) );	// 2013/01/08 jeil EX메이지 추가 스트링 나오면 추가 수정 필요 
#endif
	m_cmbNeedClass.SetCurSel( 0 );

	m_cmbPartyType.Create( this, 463, 363, 103, 16, 87, 2, 15, 13, 4, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbPartyType.SetBackUV( 115, 44, 192, 59, fTexWidth, fTexHeight );
	m_cmbPartyType.SetDownBtnUV( 124, 0, 137, 13, fTexWidth, fTexHeight );
	m_cmbPartyType.SetUpBtnUV( 124, 14, 137, 27, fTexWidth, fTexHeight );	
	m_cmbPartyType.SetDropListUV( 115, 60, 192, 75, fTexWidth, fTexHeight );
	m_cmbPartyType.AddString( _S( 506, "전체" ) );
	m_cmbPartyType.AddString( _S( 3354, "일반" ) );

#ifndef DISABLE_PARTY_TYPE_BATTLE	// [2010/07/12 : Sora] 전투형 파티 제거
	m_cmbPartyType.AddString( _S( 2665, "전투형" ) );
#endif

	m_cmbPartyType.SetCurSel( 0 );

	m_btnNextbtn.Create( this, CTString( "" ), 343, 328, 9, 9 );
	m_btnNextbtn.SetUV( UBS_IDLE, 11, 117, 19, 125, fTexWidth, fTexHeight );
	m_btnNextbtn.SetUV( UBS_CLICK, 34, 117, 42, 125, fTexWidth, fTexHeight );
	m_btnNextbtn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNextbtn.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnPrevbtn.Create( this, CTString( "" ), 248, 328, 9, 9 );
	m_btnPrevbtn.SetUV( UBS_IDLE, 0, 117, 8, 125, fTexWidth, fTexHeight );
	m_btnPrevbtn.SetUV( UBS_CLICK, 23, 117, 31, 125, fTexWidth, fTexHeight );
	m_btnPrevbtn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrevbtn.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnRefresh.Create( this, CTString( "" ), 540, 322, 21, 21 );
	m_btnRefresh.SetUV( UBS_IDLE, 171, 98, 191, 118, fTexWidth, fTexHeight );
	m_btnRefresh.SetUV( UBS_CLICK, 217, 92, 237, 112, fTexWidth, fTexHeight );
	m_btnRefresh.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRefresh.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_rtPartyLvImage.SetUV( 193, 93, 214, 114, fTexWidth, fTexHeight );
	m_rtPeaceeverImage.SetUV( 193, 73, 210, 90, fTexWidth, fTexHeight );
	m_rtSurvival.SetUV( 212, 73, 229, 90, fTexWidth, fTexHeight );
	m_rtAttack.SetUV( 231, 73, 248, 90, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set refine texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Background
	if( m_nSelectState == PARTY_REGIST )	// 파티(원) 등록
	{
		JoinRender();
	}
	else if( m_nSelectState == PARTY_REGIST_LEADER )
	{
		LeaderJoinRender();
	}
	else if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
	{
		SelectListRender();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();	

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Reading window
	if( m_ebNoticeName.DoesShowReadingWindow() )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebNoticeName.RenderReadingWindow();

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : JoinRender()
// Desc : 가입 희망 파티 설정
// ----------------------------------------------------------------------------
void CUIPartyAuto::JoinRender()
{
	// Add render regions
	int nX, nY;
		
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + WBS_TITLE_HEIGHT;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Top
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 22, nY,
									m_rtJoinTopL.U0, m_rtJoinTopL.V0, m_rtJoinTopL.U1, m_rtJoinTopL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, m_nPosY, nX - 22, nY,
									m_rtJoinTopM.U0, m_rtJoinTopM.V0, m_rtJoinTopM.U1, m_rtJoinTopM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, m_nPosY, nX, nY,
									m_rtJoinTopR.U0, m_rtJoinTopR.V0, m_rtJoinTopR.U1, m_rtJoinTopR.V1,
									0xFFFFFFFF );
	// Gap
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapL.U0, m_rtJoinGapL.V0, m_rtJoinGapL.U1, m_rtJoinGapL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapM.U0, m_rtJoinGapM.V0, m_rtJoinGapM.U1, m_rtJoinGapM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapR.U0, m_rtJoinGapR.V0, m_rtJoinGapR.U1, m_rtJoinGapR.V1,
									0xFFFFFFFF );
	// middle
	nY += WBS_GAP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_JOIN_HEIGHT,
									m_rtJoinMiddleL.U0, m_rtJoinMiddleL.V0, m_rtJoinMiddleL.U1, m_rtJoinMiddleL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_JOIN_HEIGHT,
									m_rtJoinMiddleM.U0, m_rtJoinMiddleM.V0, m_rtJoinMiddleM.U1, m_rtJoinMiddleM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_JOIN_HEIGHT,
									m_rtJoinMiddleR.U0, m_rtJoinMiddleR.V0, m_rtJoinMiddleR.U1, m_rtJoinMiddleR.V1,
									0xFFFFFFFF );

	// Gap
	nY += WBS_JOIN_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapL.U0, m_rtJoinGapL.V0, m_rtJoinGapL.U1, m_rtJoinGapL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapM.U0, m_rtJoinGapM.V0, m_rtJoinGapM.U1, m_rtJoinGapM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapR.U0, m_rtJoinGapR.V0, m_rtJoinGapR.U1, m_rtJoinGapR.V1,
									0xFFFFFFFF );

	// bottom
	nY += WBS_GAP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomL.U0, m_rtJoinBottomL.V0, m_rtJoinBottomL.U1, m_rtJoinBottomL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 3, nY, nX - 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomM.U0, m_rtJoinBottomM.V0, m_rtJoinBottomM.U1, m_rtJoinBottomM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 3, nY, nX, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomR.U0, m_rtJoinBottomR.V0, m_rtJoinBottomR.U1, m_rtJoinBottomR.V1,
									0xFFFFFFFF );

	m_btnClose.Render();
	m_btnEqualDivide.Render();

#ifndef DISABLE_PARTY_TYPE_BATTLE	// [2010/07/12 : Sora] 전투형 파티 제거
	m_btnFight.Render();
#endif

	// Title
	pDrawPort->PutTextEx( _S( 2666, "가입 희망 파티 설정" ), m_nPosX + 25, m_nPosY + 5, 0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : LeaderJoinRender()
// Desc : 파티장의 파티 등록
// ----------------------------------------------------------------------------
void CUIPartyAuto::LeaderJoinRender()
{
	// Add render regions
	int nX, nY;
		
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + WBS_TITLE_HEIGHT;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Top
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 22, nY,
									m_rtJoinTopL.U0, m_rtJoinTopL.V0, m_rtJoinTopL.U1, m_rtJoinTopL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, m_nPosY, nX - 22, nY,
									m_rtJoinTopM.U0, m_rtJoinTopM.V0, m_rtJoinTopM.U1, m_rtJoinTopM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, m_nPosY, nX, nY,
									m_rtJoinTopR.U0, m_rtJoinTopR.V0, m_rtJoinTopR.U1, m_rtJoinTopR.V1,
									0xFFFFFFFF );

	// Gap
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapL.U0, m_rtJoinGapL.V0, m_rtJoinGapL.U1, m_rtJoinGapL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapM.U0, m_rtJoinGapM.V0, m_rtJoinGapM.U1, m_rtJoinGapM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_GAP_HEIGHT,
									m_rtJoinGapR.U0, m_rtJoinGapR.V0, m_rtJoinGapR.U1, m_rtJoinGapR.V1,
									0xFFFFFFFF );

	// middle 1
	nY += WBS_GAP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_LEADER_JOIN_HEIGHT_1,
									m_rtJoinMiddleL.U0, m_rtJoinMiddleL.V0, m_rtJoinMiddleL.U1, m_rtJoinMiddleL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_LEADER_JOIN_HEIGHT_1,
									m_rtJoinMiddleM.U0, m_rtJoinMiddleM.V0, m_rtJoinMiddleM.U1, m_rtJoinMiddleM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_LEADER_JOIN_HEIGHT_1,
									m_rtJoinMiddleR.U0, m_rtJoinMiddleR.V0, m_rtJoinMiddleR.U1, m_rtJoinMiddleR.V1,
									0xFFFFFFFF );
	// Notice Name 262, 16
	pDrawPort->AddTexture( m_nPosX+98, m_nPosY+39, m_nPosX+99, m_nPosY+54, 
									m_rtNoticeNameL.U0, m_rtNoticeNameL.V0, m_rtNoticeNameL.U1, m_rtNoticeNameL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+100, m_nPosY+39, m_nPosX+357, m_nPosY+54, 
									m_rtNoticeNameM.U0, m_rtNoticeNameM.V0, m_rtNoticeNameM.U1, m_rtNoticeNameM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX+358, m_nPosY+39, m_nPosX+359, m_nPosY+54, 
									m_rtNoticeNameR.U0, m_rtNoticeNameR.V0, m_rtNoticeNameR.U1, m_rtNoticeNameR.V1,
									0xFFFFFFFF );

	// Gap
	nY += WBS_LEADER_JOIN_HEIGHT_1;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_LEADER_JOIN_HEIGHT_2,
									m_rtJoinGapL.U0, m_rtJoinGapL.V0, m_rtJoinGapL.U1, m_rtJoinGapL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_LEADER_JOIN_HEIGHT_2,
									m_rtJoinGapM.U0, m_rtJoinGapM.V0, m_rtJoinGapM.U1, m_rtJoinGapM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_LEADER_JOIN_HEIGHT_2,
									m_rtJoinGapR.U0, m_rtJoinGapR.V0, m_rtJoinGapR.U1, m_rtJoinGapR.V1,
									0xFFFFFFFF );
	// bottom
	nY += WBS_LEADER_JOIN_HEIGHT_2;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomL.U0, m_rtJoinBottomL.V0, m_rtJoinBottomL.U1, m_rtJoinBottomL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 3, nY, nX - 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomM.U0, m_rtJoinBottomM.V0, m_rtJoinBottomM.U1, m_rtJoinBottomM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 3, nY, nX, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomR.U0, m_rtJoinBottomR.V0, m_rtJoinBottomR.U1, m_rtJoinBottomR.V1,
									0xFFFFFFFF );
	nX = m_nPosX + 96;	nY = m_nPosY + 66;
	// Job Image
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtTitanImage.U0, m_rtTitanImage.V0, m_rtTitanImage.U1, m_rtTitanImage.V1,
									0xFFFFFFFF );
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtKnightImage.U0, m_rtKnightImage.V0, m_rtKnightImage.U1, m_rtKnightImage.V1,
									0xFFFFFFFF );
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtHealerImage.U0, m_rtHealerImage.V0, m_rtHealerImage.U1, m_rtHealerImage.V1,
									0xFFFFFFFF );
	nX = m_nPosX + 96;	nY += UVP_TAB_Y;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtMageImage.U0, m_rtMageImage.V0, m_rtMageImage.U1, m_rtMageImage.V1,
									0xFFFFFFFF );
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtRogueImage.U0, m_rtRogueImage.V0, m_rtRogueImage.U1, m_rtRogueImage.V1,
									0xFFFFFFFF );
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtSorcererImage.U0, m_rtSorcererImage.V0, m_rtSorcererImage.U1, m_rtSorcererImage.V1,
									0xFFFFFFFF );
	nX = m_nPosX + 96; nY += UVP_TAB_Y;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtNightShadowImage.U0, m_rtNightShadowImage.V0, m_rtNightShadowImage.U1, m_rtNightShadowImage.V1,
									0xFFFFFFFF );
#ifdef CHAR_EX_ROGUE
	// [2012/08/27 : Sora] EX로그 추가
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
									m_rtEXRogueImage.U0, m_rtEXRogueImage.V0, m_rtEXRogueImage.U1, m_rtEXRogueImage.V1,
									0xFFFFFFFF );
#endif

#ifdef CHAR_EX_MAGE
	// 2013/01/08 jeil EX메이지 추가
	nX += UVP_TAB_X;
	pDrawPort->AddTexture( nX, nY, nX+22, nY+22,
		m_rtEXMageImage.U0, m_rtEXMageImage.V0, m_rtEXMageImage.U1, m_rtEXMageImage.V1,
		0xFFFFFFFF );
#endif

	m_btnClose.Render();
	m_ebNoticeName.Render();
	m_btnRegistOK.Render();
	m_btnRegistCancel.Render();

	m_cbtnTitan.Render();			// Titan
	m_cbtnKnight.Render();			// Knight
	m_cbtnHealer.Render();			// Healer
	m_cbtnMage.Render();				// Mage
	m_cbtnRogue.Render();			// Rogue
	m_cbtnSorcerer.Render();			// Socceror
	m_cbtnNightShadow.Render();
#ifdef CHAR_EX_ROGUE
	m_cbtnEXRogue.Render();	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_cbtnEXMage.Render();	// 2013/01/08 jeil EX메이지 추가 
#endif
	m_cbtnLevelLimit.Render();		// level limit

	// Title
	pDrawPort->PutTextEx( _S( 2667, "파티 등록" ), m_nPosX + 25, m_nPosY + 5, 0xFFFFFFFF );
	pDrawPort->PutTextEx( _S( 2668, "파티 공고" ), m_nPosX + 25, m_nPosY + 39, 0xCC7D28FF );
	pDrawPort->PutTextEx( _S( 2669, "필요 클래스" ), m_nPosX + 25, m_nPosY + 74, 0xCC7D28FF );
	pDrawPort->PutTextEx( _S( 2670, "레벨 제한" ), m_nPosX + 25, m_nPosY + 141, 0xCC7D28FF );
}

// ----------------------------------------------------------------------------
// Name : SelectListRender()
// Desc : 파티 및 파티원 선택 리스트 창
// ----------------------------------------------------------------------------
void CUIPartyAuto::SelectListRender()
{
	// Add render regions
	int nX, nY;
		
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + WBS_TITLE_HEIGHT;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Top
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 22, nY,
									m_rtJoinTopL.U0, m_rtJoinTopL.V0, m_rtJoinTopL.U1, m_rtJoinTopL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, m_nPosY, nX - 22, nY,
									m_rtJoinTopM.U0, m_rtJoinTopM.V0, m_rtJoinTopM.U1, m_rtJoinTopM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, m_nPosY, nX, nY,
									m_rtJoinTopR.U0, m_rtJoinTopR.V0, m_rtJoinTopR.U1, m_rtJoinTopR.V1,
									0xFFFFFFFF );

	// bottom
	nY += WBS_PARTY_LIST_BGD_1;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomL.U0, m_rtJoinBottomL.V0, m_rtJoinBottomL.U1, m_rtJoinBottomL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 3, nY, nX - 3, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomM.U0, m_rtJoinBottomM.V0, m_rtJoinBottomM.U1, m_rtJoinBottomM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 3, nY, nX, nY + WBS_BOTTOM_HEIGHT,
									m_rtJoinBottomR.U0, m_rtJoinBottomR.V0, m_rtJoinBottomR.U1, m_rtJoinBottomR.V1,
									0xFFFFFFFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();	

	// Set refine texture
	pDrawPort->InitTextureData( m_ptdAddTexture );	

	// Background 1
	nY = m_nPosY + WBS_TITLE_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 22, nY + WBS_PARTY_LIST_BGD_1,
									m_rtSelListMiddleL.U0, m_rtSelListMiddleL.V0, m_rtSelListMiddleL.U1, m_rtSelListMiddleL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 22, nY, nX - 22, nY + WBS_PARTY_LIST_BGD_1,
									m_rtSelListMiddleM.U0, m_rtSelListMiddleM.V0, m_rtSelListMiddleM.U1, m_rtSelListMiddleM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX - 22, nY, nX, nY + WBS_PARTY_LIST_BGD_1,
									m_rtSelListMiddleR.U0, m_rtSelListMiddleR.V0, m_rtSelListMiddleR.U1, m_rtSelListMiddleR.V1,
									0xFFFFFFFF );

	// Background 2
	int nX1, nY1, nX2;
	nX1 = m_nPosX + 11;
	nY1 = m_nPosY + 31;
	nX2 = WBS_PARTY_LIST_BGD_WIDTH - 2;
	// Top
	pDrawPort->AddTexture( nX1, nY1, nX1 + 2, nY1 + 2,
									m_rtSelListBackTopL.U0, m_rtSelListBackTopL.V0, m_rtSelListBackTopL.U1, m_rtSelListBackTopL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + 2, nY1, nX1 + nX2, nY1 + 2,
									m_rtSelListBackTopM.U0, m_rtSelListBackTopM.V0, m_rtSelListBackTopM.U1, m_rtSelListBackTopM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + nX2, nY1, nX1 + WBS_PARTY_LIST_BGD_WIDTH, nY1 + 2,
									m_rtSelListBackTopR.U0, m_rtSelListBackTopR.V0, m_rtSelListBackTopR.U1, m_rtSelListBackTopR.V1,
									0xFFFFFFFF );
	// Middle
	nY1 += 2;

	pDrawPort->AddTexture( nX1, nY1, nX1 + 2, nY1 + WBS_PARTY_LIST_BGD_2,
									m_rtSelListBackMiddleL.U0, m_rtSelListBackMiddleL.V0, m_rtSelListBackMiddleL.U1, m_rtSelListBackMiddleL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + 2, nY1, nX1 + nX2, nY1 + WBS_PARTY_LIST_BGD_2,
									m_rtSelListBackMiddleM.U0, m_rtSelListBackMiddleM.V0, m_rtSelListBackMiddleM.U1, m_rtSelListBackMiddleM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + nX2, nY1, nX1 + WBS_PARTY_LIST_BGD_WIDTH, nY1 + WBS_PARTY_LIST_BGD_2,
									m_rtSelListBackMiddleR.U0, m_rtSelListBackMiddleR.V0, m_rtSelListBackMiddleR.U1, m_rtSelListBackMiddleR.V1,
									0xFFFFFFFF );
	// bottom
	nY1 += WBS_PARTY_LIST_BGD_2;

	pDrawPort->AddTexture( nX1, nY1, nX1 + 2, nY1 + 2,
									m_rtSelListBackBottomL.U0, m_rtSelListBackBottomL.V0, m_rtSelListBackBottomL.U1, m_rtSelListBackBottomL.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + 2, nY1, nX1 + nX2, nY1 + 2,
									m_rtSelListBackBottomM.U0, m_rtSelListBackBottomM.V0, m_rtSelListBackBottomM.U1, m_rtSelListBackBottomM.V1,
									0xFFFFFFFF );
	pDrawPort->AddTexture( nX1 + nX2, nY1, nX1 + WBS_PARTY_LIST_BGD_WIDTH, nY1 + 2,
									m_rtSelListBackBottomR.U0, m_rtSelListBackBottomR.V0, m_rtSelListBackBottomR.U1, m_rtSelListBackBottomR.V1,
									0xFFFFFFFF );

	// Subject Background
	pDrawPort->AddTexture( m_nPosX+15, m_nPosY+35, m_nPosX + 583, m_nPosY + 57,
									m_rtListSubjectRect.U0, m_rtListSubjectRect.V0, m_rtListSubjectRect.U1, m_rtListSubjectRect.V1,
									0xFFFFFFFF );
	
	// line Background
	nY = m_nPosY + 57;
	nY1 = 26;
	pDrawPort->AddTexture( m_nPosX+27, nY+nY1, m_nPosX+572, nY+nY1+1,
									m_rtline.U0, m_rtline.V0, m_rtline.U1, m_rtline.V1,
									0xFFFFFFFF );
	for( int i=0; i<9; i++ )
	{
		nY1 += 25;
		pDrawPort->AddTexture( m_nPosX+27, nY+nY1, m_nPosX+572, nY+nY1+1,
										m_rtline.U0, m_rtline.V0, m_rtline.U1, m_rtline.V1,
										0xFFFFFFFF );
	}
	nY1 += 1;
	pDrawPort->AddTexture( m_nPosX+27, nY+nY1, m_nPosX+572, nY+nY1+1,
									m_rtline.U0, m_rtline.V0, m_rtline.U1, m_rtline.V1,
									0xFFFFFFFF );
	////////////////////////////// line end ////////////////////////////////////

	if( m_nSelectLine != -1 )
	{
		int nSelX = m_nPosX+27;
		int nSelY = m_nPosY+65+( m_nSelectLine * 25 );

		pDrawPort->AddTexture( nSelX, nSelY, nSelX+545, nSelY+13,
										m_rtSelectBar.U0, m_rtSelectBar.V0, m_rtSelectBar.U1, m_rtSelectBar.V1,
										0xFFFFFFFF );
	}

	m_btnNextbtn.Render();
	m_btnPrevbtn.Render();
	m_btnRefresh.Render();
	m_cmbNeedClass.Render();
	m_cmbPartyType.Render();

	// record render
	ListRecordRender();

	// Render all elements
	pDrawPort->FlushRenderingQueue();	

	// Set refine texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_btnClose.Render();
	m_cbtnLevelLimit.Render();		// level limit
	m_btnRegistOK.Render();
	m_btnRegistCancel.Render();

	// menu
	if( m_nSelectState == INVITE_LIST )
	{
		pDrawPort->PutTextEx( _S( 2671, "초대가능 파티원 리스트" ), m_nPosX + 25, m_nPosY + 5, 0xFFFFFFFF );// title
		pDrawPort->PutTextExCX( _S( 2672, "번호" ), m_nPosX + NUMBER_POS, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 2673, "캐릭명" ), m_nPosX + SUBJECT_STR, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 73, "레벨" ), m_nPosX + LIMIT_DIFF, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 3857, "접속위치" ), m_nPosX + ZONE_POS, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 72, "클래스" ), m_nPosX + NEED_CLASS, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 2676, "희망파티" ), m_nPosX + PARTY_TYPE, m_nPosY + 40, 0xFFFFCAFF );
	}
	else if( m_nSelectState == PARTY_LIST )
	{
		pDrawPort->PutTextEx( _S( 2677, "참가가능 파티 리스트" ), m_nPosX + 25, m_nPosY + 5, 0xFFFFFFFF );// title
		pDrawPort->PutTextExCX( _S( 2672, "번호" ), m_nPosX + NUMBER_POS, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 2678, "파티모집창" ), m_nPosX + SUBJECT_STR, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 97, "파티" ), m_nPosX + LIMIT_DIFF-10, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 73, "레벨" ), m_nPosX + LIMIT_DIFF+30, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 3857, "접속위치" ), m_nPosX + ZONE_POS+20, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 2669, "필요클래스" ), m_nPosX + NEED_CLASS, m_nPosY + 40, 0xFFFFCAFF );
		pDrawPort->PutTextExCX( _S( 2679, "파티종류" ), m_nPosX + PARTY_TYPE, m_nPosY + 40, 0xFFFFCAFF );
	}

	int nStrWidth = _S( 2670, "레벨제한" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	
	pDrawPort->PutTextEx( _S( 2670, "레벨제한" ), m_nPosX+318-nStrWidth, m_nPosY+366, 0xFFAC4FFF );

	nStrWidth = _S( 2679, "파티종류" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	pDrawPort->PutTextEx( _S( 2679, "파티종류" ), m_nPosX+457-nStrWidth, m_nPosY+366, 0xFFAC4FFF );

	if( m_nSelectState == INVITE_LIST )
	{
		nStrWidth = _S( 72, "클래스" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

		pDrawPort->PutTextEx( _S( 72, "클래스" ), m_nPosX+83-nStrWidth, m_nPosY+366, 0xFFAC4FFF );
	}
	else if( m_nSelectState == PARTY_LIST )
	{
		nStrWidth = _S( 2669, "필요클래스" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

		pDrawPort->PutTextEx( _S( 2669, "필요클래스" ), m_nPosX+83-nStrWidth, m_nPosY+366, 0xFFAC4FFF );
	}

	nStrWidth = _S( 386, "검색" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	pDrawPort->PutTextEx( _S( 386, "검색" ), m_nPosX+540-nStrWidth-7, m_nPosY+326, 0xFFFFFFFF );

	// [091110: selo] "다음" 텍스트 위치 잡기
	int nNext_ExtraX = 0;
/*	if( USA_SPAIN == g_iCountry )
		nNext_ExtraX = 10;	/**/

	
	nStrWidth = _S( 2680, "다음" ).Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	pDrawPort->PutTextEx( _S( 2680, "다음" ), m_nPosX+343-nStrWidth-7 + nNext_ExtraX, m_nPosY+326, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S( 2681, "이전" ), m_nPosX+264, m_nPosY+326, 0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : ListRecordRender()
// Desc : 파티 리스트 각 레코드
// ----------------------------------------------------------------------------
void CUIPartyAuto::ListRecordRender()
{
	if( m_vectorPartyListData.empty() == true )
		return;

	int		Number, nLeveldiff, nZone, nPartyType;
	int		nSaveHeight = 0;
	DWORD	dwClass;
	CTString strSubject;
	COLOR	strColor;
	int		nPartyNum = 2;

	int TotalListNum = m_vectorPartyListData.size();

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for( int num=0; num<TotalListNum; num++ )
	{
		Number = ( num + (m_nCurrentPageNum*10) + 1 );				// 번호
		strSubject = m_vectorPartyListData[num].strSubject;	// 제목
		nLeveldiff = m_vectorPartyListData[num].nLeveldiff;	// 레벨제한
		nZone = m_vectorPartyListData[num].nZone;				// 위치
		dwClass = m_vectorPartyListData[num].dwJobflag;		// 직업 플래그
		nPartyType = m_vectorPartyListData[num].nPartyType;	// 파티 종류
		nPartyNum = m_vectorPartyListData[num].nPartyNum;
		nSaveHeight = ( num * 25 );// 높이

		CTString strOutput;
		// 번호
		strOutput.PrintF( CTString( "%d" ), Number );
		pDrawPort->PutTextExCX( strOutput, m_nPosX + NUMBER_POS, m_nPosY+LESH_NUMBER+nSaveHeight, 0xFFFFFFFF );
		// 제목

		if( strSubject.Length() > 30 )
		{
			CTString strTemp1, strTemp2;

			BOOL b2Byte = Is2ByteChar( 30, 0, strSubject );
				
			if( b2Byte )
			{ strSubject.Split( 31, strTemp1, strTemp2 ); }
			else
			{ strSubject.Split( 30, strTemp1, strTemp2 ); }

			strOutput = strTemp1;
			strColor = 0xFFF284FF;
		}else if (nPartyNum >= 8)
		{
			strColor = 0xA7A9A5FF;
			strOutput = strSubject;
		}
		else
		{
			strColor = 0xFFFFFFFF;
			strOutput = strSubject;
		}
		
		if( m_nSelectState == INVITE_LIST )// 캐릭명 중앙 정렬
			pDrawPort->PutTextExCX( strOutput, m_nPosX + SUBJECT_STR, m_nPosY+LESH_STRING+nSaveHeight, 0xFFFFFFFF );			
		else if( m_nSelectState == PARTY_LIST )
		{

			if( m_bShowTextScroll && m_nSelectLine == num && strSubject.Length() > 30 )
			{
				TextScroll( m_vectorPartyListData[m_nSelectLine].strSubject );
			}
			else
			{
				pDrawPort->PutTextEx( strOutput, m_nPosX + 64, m_nPosY+LESH_STRING+nSaveHeight, strColor );
			}
		}
		// 레벨 제한
		int nX = LIMIT_DIFF - 11;
		int nY = LESH_LEVELDIFF + nSaveHeight;
		int nTempGap = 0;
		strColor = 0xFFFFFFFF;

		if( m_nSelectState == PARTY_LIST )
		{
			if (nPartyNum >= 8)
			{
				strColor = 0xA7A9A5FF;
			}			

			CTString strPartyNum;
			strPartyNum.PrintF("%d/8",nPartyNum);

			pDrawPort->PutTextExCX( strPartyNum, m_nPosX + nX, m_nPosY+LESH_STRING+nSaveHeight, strColor );
			
			nX += 30;
			nTempGap = 20;
		}

		if( nLeveldiff == 0 )// 0일때 레벨제한 있음
		{
			pDrawPort->AddTexture( m_nPosX+nX, m_nPosY+nY, m_nPosX+nX+LEVELDIFF_IMAGE_SIZE , m_nPosY+nY+LEVELDIFF_IMAGE_SIZE ,
										m_rtPartyLvImage.U0, m_rtPartyLvImage.V0, m_rtPartyLvImage.U1, m_rtPartyLvImage.V1,
										strColor );			
		}
		// 위치
		strOutput = CZoneInfo::getSingleton()->GetZoneName( nZone );
		pDrawPort->PutTextExCX( strOutput, m_nPosX + ZONE_POS+nTempGap, m_nPosY+LESH_STRING+nSaveHeight, strColor );

		// 파티 타입
		int nSubWidth = PARTY_TYPE - 9;
		int nTypeY = LESH_IMAGE + nSaveHeight;

		if( nPartyType == PT_PEACEEVER )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtPeaceeverImage.U0, m_rtPeaceeverImage.V0, m_rtPeaceeverImage.U1, m_rtPeaceeverImage.V1,
										0xFFFFFFFF );			
		}
		else if( nPartyType == PT_SURVIVAL )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtSurvival.U0, m_rtSurvival.V0, m_rtSurvival.U1, m_rtSurvival.V1,
										0xFFFFFFFF );	
		}
		else if(  nPartyType == PT_ATTACK )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtAttack.U0, m_rtAttack.V0, m_rtAttack.U1, m_rtAttack.V1,
										0xFFFFFFFF );	
		}

		// 클래스
		// Render all elements
		pDrawPort->FlushRenderingQueue();	

		// Set refine texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		int	nJobCnt = 0;

		if( dwClass & JF_TITAN ) nJobCnt++;
		if( dwClass & JF_KNIGHT ) nJobCnt++;
		if( dwClass & JF_HEALER ) nJobCnt++;
		if( dwClass & JF_MAGE ) nJobCnt++;
		if( dwClass & JF_ROGUE ) nJobCnt++;
		if( dwClass & JF_SORCERER ) nJobCnt++;
		if( dwClass & JF_NIGHTSHADOW ) nJobCnt++;
#ifdef CHAR_EX_ROGUE
		if( dwClass & JF_EX_ROGUE ) nJobCnt++;	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
		if( dwClass & JF_EX_MAGE ) nJobCnt++;	// 2013/01/08 jeil EX메이지 추가
#endif
		nSubWidth = NEED_CLASS - ( ( nJobCnt * CLASS_IMAGE_SIZE ) / 2 );

		if( dwClass & JF_TITAN )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtTitanImage.U0, m_rtTitanImage.V0, m_rtTitanImage.U1, m_rtTitanImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;
		}
		
		if( dwClass & JF_KNIGHT )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtKnightImage.U0, m_rtKnightImage.V0, m_rtKnightImage.U1, m_rtKnightImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;	
		}
		
		if( dwClass & JF_HEALER )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtHealerImage.U0, m_rtHealerImage.V0, m_rtHealerImage.U1, m_rtHealerImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;	
		}
		
		if( dwClass & JF_MAGE )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtMageImage.U0, m_rtMageImage.V0, m_rtMageImage.U1, m_rtMageImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;	
		}
		
		if( dwClass & JF_ROGUE )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtRogueImage.U0, m_rtRogueImage.V0, m_rtRogueImage.U1, m_rtRogueImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;	
		}
		
		if( dwClass & JF_SORCERER )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtSorcererImage.U0, m_rtSorcererImage.V0, m_rtSorcererImage.U1, m_rtSorcererImage.V1,
										0xFFFFFFFF );
			nSubWidth += CLASS_IMAGE_SIZE;
		}
		
		if( dwClass & JF_NIGHTSHADOW )
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtNightShadowImage.U0, m_rtNightShadowImage.V0, m_rtNightShadowImage.U1, m_rtNightShadowImage.V1,
										0xFFFFFFFF );

			nSubWidth += CLASS_IMAGE_SIZE;	
		}
#ifdef CHAR_EX_ROGUE
		if( dwClass & JF_EX_ROGUE )	// [2012/08/27 : Sora] EX로그 추가
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
										m_rtEXRogueImage.U0, m_rtEXRogueImage.V0, m_rtEXRogueImage.U1, m_rtEXRogueImage.V1,
										0xFFFFFFFF );
			
			nSubWidth += CLASS_IMAGE_SIZE;
		}
#endif

#ifdef CHAR_EX_MAGE
		if( dwClass & JF_EX_MAGE )	// 2013/01/08 jeil EX메이지 추가
		{
			pDrawPort->AddTexture( m_nPosX+nSubWidth, m_nPosY+nTypeY, m_nPosX+nSubWidth+CLASS_IMAGE_SIZE, m_nPosY+nTypeY+CLASS_IMAGE_SIZE,
				m_rtEXMageImage.U0, m_rtEXMageImage.V0, m_rtEXMageImage.U1, m_rtEXMageImage.V1,
				0xFFFFFFFF );
				
		}
#endif

		if( num < ( TotalListNum - 1 ) )
		{
			// Render all elements
			pDrawPort->FlushRenderingQueue();	

			// Set refine texture
			pDrawPort->InitTextureData( m_ptdAddTexture );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc : Data Initial
// ----------------------------------------------------------------------------
void CUIPartyAuto::Clear()
{
	m_nSelectState = -1;
	m_dwJobFlag = 0;
	m_nLevelLimit = 0;
	m_nCurrentPageNum = 0;
	m_FrontChPos = 0;
	m_nSelectLine = -1;
	m_bShowTextScroll = FALSE;
	m_bTextDir = TRUE;
	m_ebNoticeName.ResetString();
	m_vectorPartyListData.clear();
	
	// 체크 박스 초기화
	m_cbtnTitan.SetCheck( FALSE );			// Titan
	m_cbtnKnight.SetCheck( FALSE );			// Knight
	m_cbtnHealer.SetCheck( FALSE );			// Healer
	m_cbtnMage.SetCheck( FALSE );				// Mage
	m_cbtnRogue.SetCheck( FALSE );			// Rogue
	m_cbtnSorcerer.SetCheck( FALSE );			// Socceror
	m_cbtnNightShadow.SetCheck( FALSE);
#ifdef CHAR_EX_ROGUE
	m_cbtnEXRogue.SetCheck(FALSE); // [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE 
	m_cbtnEXMage.SetCheck(FALSE); // 2013/01/08 jeil EX메이지 추가
#endif
	m_cbtnLevelLimit.SetCheck( FALSE );

	m_cmbNeedClass.SetCurSel( 0 );
	m_cmbPartyType.SetCurSel( 0 );

	m_ebNoticeName.SetFocus(FALSE);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenPartyMatching()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::OpenPartyMatching()
{
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( GAMEDATAMGR()->GetPartyInfo()->IsPartyRequested() )
		return;

	pUIManager->CloseMessageBoxL( MSGLCMD_PARTYAUTOMATCH_REQ );
	pUIManager->CreateMessageBoxL( _S( 2682, "파티 매칭 시스템" ), UI_PARTYAUTO, MSGLCMD_PARTYAUTOMATCH_REQ );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, TRUE,
				_S( 2683, "용사님들이 보다 수월하게 모험을 하실 수 있도록 파티원들을 모집해 주는 시스템입니다." ), -1, 0xA3A1A3FF );

	CTString strMessage;
	strMessage.PrintF( _S( 2684, "파티(원) 등록" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_REGIST );

	strMessage.PrintF( _S( 2685, "초대가능 파티원 리스트" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, INVITE_LIST );

	strMessage.PrintF( _S( 2686, "참가가능 파티 리스트" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_LIST );

	strMessage.PrintF( _S( 2687, "파티(원) 등록 삭제" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage, PARTY_DEL );
					
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PARTYAUTOMATCH_REQ, FALSE, strMessage );
}

// ----------------------------------------------------------------------------
// Name : OpenAutoParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::OpenAutoParty( int nType )
{
	if( IsVisible() == TRUE )
		return;

	Clear();

	if( nType == PARTY_REGIST )
	{
		SetSize( PT_JOIN_WIDTH, PT_JOIN_HEIGHT );	// 가입 희망 파티 설정 창 사이즈로 변환
		m_rcJoinTitle.SetRect( 0, 0, PT_JOIN_WIDTH, 22 );
		m_btnClose.SetPos( PT_JOIN_WIDTH - 27, 4 );
		m_ebNoticeName.SetEnable( FALSE );
		m_ebNoticeName.SetFocus( FALSE );

		m_nSelectState = PARTY_REGIST;
	}
	else if( nType == PARTY_REGIST_LEADER )
	{
		SetSize( PT_LEADER_JOIN_WIDTH, PT_LEADER_JOIN_HEIGHT );	// 파티장 파티 등록 창 사이즈로 변환
		m_rcJoinTitle.SetRect( 0, 0, PT_LEADER_JOIN_WIDTH, 22 );
		m_btnClose.SetPos( PT_LEADER_JOIN_WIDTH - 27, 4 );
		m_btnRegistOK.SetText( _S( 191, "확인" ) );
		m_btnRegistOK.SetPos( 131, 171 );
		m_btnRegistCancel.SetPos( 206, 171 );
		m_cbtnLevelLimit.SetPos( 98, 139 );

		m_ebNoticeName.SetEnable( TRUE );
		m_ebNoticeName.SetFocus( TRUE );

		m_nSelectState = PARTY_REGIST_LEADER;
	}
	else if( nType == INVITE_LIST || nType == PARTY_LIST )
	{// 초대 가능 파티원, 참가 가능 파티 리스트 선택 창 사이즈로 변환 
		SetSize( P_AUTO_WIDTH, P_AUTO_HEIGHT );
		m_rcJoinTitle.SetRect( 0, 0, P_AUTO_WIDTH, 22 );
		m_btnClose.SetPos( P_AUTO_WIDTH - 27, 4 );
		m_btnRegistOK.SetPos( 231, 393 );
		m_btnRegistCancel.SetPos( 304, 393 );
		m_cbtnLevelLimit.SetPos( 324, 366 );
		m_ebNoticeName.SetEnable( FALSE );
		m_ebNoticeName.SetFocus( FALSE );

		//test
//		for( int i=0; i<10; i++ )
//		{
//			AddPartyData( 123, 1, 4, CTString( "안세요aa 하요bb b하세요하1123 cc하세요dddd하세" ), 0x0000003F, 0, 2 );
//		}

		if( nType == INVITE_LIST )
		{
			m_btnRegistOK.SetText( _S( 2688, "파티초대" ) );
			_pNetwork->SendPartyListReq( m_nCurrentPageNum, -1, 0, -1, FALSE );
		}
		else if( nType == PARTY_LIST )
		{
			m_btnRegistOK.SetText( _S( 2689, "참가신청" ) );
			_pNetwork->SendPartyListReq( m_nCurrentPageNum, -1, 0, -1, TRUE );
		}

		m_nSelectState = nType;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, TRUE );
	AdjustUIPos();
}

// ----------------------------------------------------------------------------
// Name : AdjustUIPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::AdjustUIPos()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	PIX pixMinI, pixMaxI, pixMinJ, pixMaxJ;

	pixMinI = pUIManager->GetMinI();
	pixMinJ = pUIManager->GetMinJ();
	pixMaxI = pUIManager->GetMaxI();
	pixMaxJ = pUIManager->GetMaxJ();

	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CloseAutoParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::CloseAutoParty()
{
	
	CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, FALSE );

	Clear();
}

// ----------------------------------------------------------------------------
// Name : JoinMouseMsg()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::JoinMouseMsg( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		if( m_btnEqualDivide.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnFight.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		break;
	case WM_LBUTTONDOWN:
		if( m_btnEqualDivide.MouseMessage( pMsg ) != WMSG_FAIL )
		{
						
		}
		else if( m_btnFight.MouseMessage( pMsg ) != WMSG_FAIL )
		{

		}
		break;
	case WM_LBUTTONUP:
		if( ( wmsgResult = m_btnEqualDivide.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{// 균등 분배 파티
			if( wmsgResult == WMSG_COMMAND )
			{
				_pNetwork->SendPartyRegistNormal( PT_PEACEEVER );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnFight.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{// 전투형 파티
			if( wmsgResult == WMSG_COMMAND )
			{
				_pNetwork->SendPartyRegistNormal( PT_ATTACK );
			}

			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONDBLCLK:
		if( ( wmsgResult = m_btnEqualDivide.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{// 균등 분배 파티
			if( wmsgResult == WMSG_COMMAND )
			{
				_pNetwork->SendPartyRegistNormal( PT_PEACEEVER );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnFight.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{// 전투형 파티
			if( wmsgResult == WMSG_COMMAND )
			{
				_pNetwork->SendPartyRegistNormal( PT_ATTACK );
			}

			return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : LeaderRegMouseMsg()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::LeaderRegMouseMsg( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		if( m_ebNoticeName.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnRegistOK.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnRegistCancel.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		break;
	case WM_LBUTTONDOWN:
		if( m_ebNoticeName.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, TRUE );
		}
		else if( m_cbtnTitan.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnTitan.IsChecked() )	m_dwJobFlag |= JF_TITAN;
			else 	m_dwJobFlag &= ~JF_TITAN;
		}
		else if( m_cbtnKnight.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnKnight.IsChecked() )	m_dwJobFlag |= JF_KNIGHT;
			else	m_dwJobFlag &= ~JF_KNIGHT;
		}
		else if( m_cbtnHealer.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnHealer.IsChecked() )	m_dwJobFlag |= JF_HEALER;
			else	m_dwJobFlag &= ~JF_HEALER;
		}
		else if( m_cbtnMage.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnMage.IsChecked() )	m_dwJobFlag |= JF_MAGE;
			else	m_dwJobFlag &= ~JF_MAGE;
		}
		else if( m_cbtnRogue.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnRogue.IsChecked() )	m_dwJobFlag |= JF_ROGUE;
			else	m_dwJobFlag &= ~JF_ROGUE;
		}
		else if( m_cbtnSorcerer.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnSorcerer.IsChecked() )	m_dwJobFlag |= JF_SORCERER;
			else	m_dwJobFlag &= ~JF_SORCERER;
		}
		else if( m_cbtnNightShadow.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			if( m_cbtnNightShadow.IsChecked() )	m_dwJobFlag |= JF_NIGHTSHADOW;
			else	m_dwJobFlag &= ~JF_NIGHTSHADOW;
		}
#ifdef CHAR_EX_ROGUE
		else if( m_cbtnEXRogue.MouseMessage( pMsg ) != WMSG_FAIL )	// [2012/08/27 : Sora] EX로그 추가
		{
			if( m_cbtnEXRogue.IsChecked() )	m_dwJobFlag |= JF_EX_ROGUE;
			else	m_dwJobFlag &= ~JF_EX_ROGUE;
		}
#endif

#ifdef CHAR_EX_MAGE
		else if( m_cbtnEXMage.MouseMessage( pMsg ) != WMSG_FAIL )	// 2013/01/08 jeil EX메이지 추가
		{
			if( m_cbtnEXMage.IsChecked() )	m_dwJobFlag |= JF_EX_MAGE;
			else	m_dwJobFlag &= ~JF_EX_MAGE;
		}
#endif
		else if( m_cbtnLevelLimit.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			m_nLevelLimit = m_cbtnLevelLimit.IsChecked() ? 1 : 0;
		}
		else if( m_btnRegistOK.MouseMessage( pMsg ) != WMSG_FAIL )
		{

		}
		else if( m_btnRegistCancel.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			
		}
		break;
	case WM_LBUTTONUP:
		if( ( wmsgResult = m_btnRegistOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				PressOKBtn();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRegistCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				CloseAutoParty();
			}

			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONDBLCLK:
		if( ( wmsgResult = m_btnRegistOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				PressOKBtn();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRegistCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				CloseAutoParty();
			}

			return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : PartyListMouseMsg()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::PartyListMouseMsg( MSG *pMsg )
{
	WMSG_RESULT wmsgResult;

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		if( m_btnRegistOK.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnRegistCancel.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnPrevbtn.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_btnPrevbtn.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_cmbPartyType.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if( m_cmbNeedClass.MouseMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS;
		else if (m_btnRefresh.MouseMessage( pMsg ) != WMSG_FAIL)
			return WMSG_SUCCESS;
		break;
	case WM_LBUTTONDOWN:
		if( m_cbtnLevelLimit.MouseMessage( pMsg ) != WMSG_FAIL )
		{
			m_nLevelLimit = m_cbtnLevelLimit.IsChecked() ? 1 : 0;
		}
		m_btnRegistOK.MouseMessage( pMsg );
		m_btnRegistCancel.MouseMessage( pMsg );
		m_btnNextbtn.MouseMessage( pMsg );
		m_btnPrevbtn.MouseMessage( pMsg );
		m_btnRefresh.MouseMessage( pMsg );
		m_cmbNeedClass.MouseMessage( pMsg );
		m_cmbPartyType.MouseMessage( pMsg );
		break;
	case WM_LBUTTONUP:
		if( ( wmsgResult = m_btnRegistOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				PressOKBtn();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRegistCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				CloseAutoParty();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnNextbtn.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( 1 );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnPrevbtn.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( -1 );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRefresh.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( 0 );
			}

			return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONDBLCLK:
		if( ( wmsgResult = m_btnRegistOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				PressOKBtn();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRegistCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				CloseAutoParty();
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnNextbtn.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( 1 );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnPrevbtn.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( -1 );
			}

			return WMSG_SUCCESS;
		}
		else if( ( wmsgResult = m_btnRefresh.MouseMessage( pMsg ) ) != WMSG_FAIL )
		{
			if( wmsgResult == WMSG_COMMAND )
			{
				SendPartyReq( 0 );
			}

			return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int nX = LOWORD( pMsg->lParam );
	int nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			if( m_nSelectState >= PARTY_REGIST )
			{
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					int ndX = nX - nOldX;
					int ndY = nY - nOldY;
					nOldX = nX; nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_nSelectState == PARTY_REGIST )
				{
					wmsgResult = JoinMouseMsg( pMsg );

					if( wmsgResult != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
				}
				else if( m_nSelectState == PARTY_REGIST_LEADER )
				{
					wmsgResult = LeaderRegMouseMsg( pMsg );

					if( wmsgResult != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
				}
				else if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
				{
					wmsgResult = PartyListMouseMsg( pMsg );

					if( IsInsideRect( nX, nY, m_rcSelLine ) )
					{	
						if( m_nSelectLine == ListSelectLine( nY ) )
						{
							if( !m_bShowTextScroll )
							{
								mtmTextScrollTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
							}

							m_bShowTextScroll = TRUE;
						}
						else
						{
							m_bShowTextScroll = FALSE;
							m_bTextDir = TRUE;
							m_FrontChPos = 0;
						}
					}
					else
					{
						m_bShowTextScroll = FALSE;
						m_bTextDir = TRUE;
						m_FrontChPos = 0;
					}

					if( wmsgResult != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( m_nSelectState >= PARTY_REGIST )
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
					else if( IsInsideRect( nX, nY, m_rcJoinTitle ) )
					{
						bTitleBarClick = TRUE;
					}
					else if( m_nSelectState == PARTY_REGIST )
					{
						wmsgResult = JoinMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == PARTY_REGIST_LEADER )
					{
						wmsgResult = LeaderRegMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
					{
						if( IsInsideRect( nX, nY, m_rcSelLine ) )
						{	
							int nSel = ListSelectLine( nY );

							if( nSel < m_vectorPartyListData.size()  )
							{ 
								m_nSelectLine = nSel;
			
								if( !m_bShowTextScroll )
								{
									mtmTextScrollTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
								}

								m_bShowTextScroll = TRUE;
							}
						}

						wmsgResult = PartyListMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					
					CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, TRUE );
					return WMSG_SUCCESS;
				}
				else
				{
					if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
					{
						if( ( wmsgResult = m_cmbNeedClass.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult != WMSG_FAIL )
							{
								CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, TRUE );
								return WMSG_SUCCESS;
							}
						}
						else if( ( wmsgResult = m_cmbPartyType.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult != WMSG_FAIL )
							{
								CUIManager::getSingleton()->RearrangeOrder( UI_PARTYAUTO, TRUE );
								return WMSG_SUCCESS;
							}
						}
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( m_nSelectState >= PARTY_REGIST )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseAutoParty();

						return WMSG_SUCCESS;
					}
					else if( m_nSelectState == PARTY_REGIST )
					{
						wmsgResult = JoinMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == PARTY_REGIST_LEADER )
					{
						wmsgResult = LeaderRegMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
					{
						wmsgResult = PartyListMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
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
	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_nSelectState >= PARTY_REGIST )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseAutoParty();

						return WMSG_SUCCESS;
					}
					else if( m_nSelectState == PARTY_REGIST )
					{
						wmsgResult = JoinMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == PARTY_REGIST_LEADER )
					{
						wmsgResult = LeaderRegMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
					else if( m_nSelectState == INVITE_LIST || m_nSelectState == PARTY_LIST )
					{
						wmsgResult = PartyListMouseMsg( pMsg );

						if( wmsgResult != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}
				}				

				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{
		return WMSG_SUCCESS;
	}

	if( m_nSelectState == PARTY_REGIST_LEADER )
	{
		if( m_ebNoticeName.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::IMEMessage( MSG *pMsg )
{
	if( m_nSelectState == PARTY_REGIST_LEADER )
	{	
		return m_ebNoticeName.IMEMessage( pMsg );
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPartyAuto::CharMessage( MSG *pMsg )
{
	if( m_nSelectState == PARTY_REGIST_LEADER )
	{
		if( m_ebNoticeName.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PARTYAUTO_REGIST:	// 파티 등록
		{

		}
		break;
	case MSGCMD_PARTYAUTO_JOIN_ALLOW:
		{
			m_bIsPartyRequested =FALSE;

			if( bOK )
			{
				_pNetwork->SendPartyJoinAllow( m_nCharIndex );
			}
			else
			{
				_pNetwork->SendPartyJoinReject( m_nCharIndex );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CTString strMessage;
	switch( nCommandCode )
	{
	case MSGLCMD_PARTYAUTOMATCH_REQ:
		{
			if( nResult == PARTY_REGIST ) // 파티 등록
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() <= 0 ) // 파티중이 아니라면
				{
					OpenAutoParty( PARTY_REGIST );
				}
				else
				{
					if( GAMEDATAMGR()->GetPartyInfo()->AmILeader() )
					{
						OpenAutoParty( PARTY_REGIST_LEADER );
					}
					else
					{
						pUIManager->CloseMessageBoxL( MSGLCMD_PARTYAUTOMATCH_REQ );
						CUIMsgBox_Info MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						CTString	strMessage = _S( 2690, "파티장만 등록할 수 있습니다" );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
				}
			}
			else if( nResult == INVITE_LIST ) // 초대가능 리스트
			{
				OpenAutoParty( INVITE_LIST );
			}
			else if( nResult == PARTY_LIST ) // 참가 가능 리스트
			{
				OpenAutoParty( PARTY_LIST );
			}
			else if( nResult == PARTY_DEL ) // 파티 등록 삭제
			{
				_pNetwork->SendPartyRegistDelete();
			}
		}
		break;
	}
}
// ----------------------------------------------------------------------------
// Name : PressOKBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::PressOKBtn()
{
	if( m_nSelectState == PARTY_REGIST_LEADER )
	{
		// Get input string
		if( m_ebNoticeName.GetString() )
		{
			m_strNoticeName = m_ebNoticeName.GetString();

			if( m_strNoticeName.Length() > MAX_PARTY_NAME_LEN )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_REGIST_ERROR );
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_PARTYAUTO, MSGCMD_PARTYAUTO_REGIST_ERROR );
				CTString	strMessage = _S( 2691, "파티 공고가 너무 깁니다.\n( 한글 최대 25자, 영문 최대 50자" );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}
			else if( m_strNoticeName.Length() <= 0 )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_REGIST_ERROR );
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_PARTYAUTO, MSGCMD_PARTYAUTO_REGIST_ERROR );
				CTString	strMessage = _S( 2692, "파티 공고를 입력하세요." );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}
			
			if( m_dwJobFlag == 0 ) m_dwJobFlag = JF_ALLCLASS;

			_pNetwork->SendPartyRegistParty( m_strNoticeName, m_dwJobFlag, m_nLevelLimit );
		}
	}
	else if( m_nSelectState == INVITE_LIST )
	{
		if( m_nSelectLine != -1 )
		{
			if( !m_vectorPartyListData.empty() )
				_pNetwork->SendPartyInviteReq( m_vectorPartyListData[m_nSelectLine].nIndex );
		}
		else
		{
			CTString strMessage = _S( 2693, "파티에 초대할 사람을 선택하지 않았습니다" );
			_pNetwork->ClientSystemMessage( strMessage );
			return;
		}
	}
	else if( m_nSelectState == PARTY_LIST )
	{
		if( m_nSelectLine != -1 )
		{
			if( !m_vectorPartyListData.empty() )
				_pNetwork->SendPartyJoinReq( m_vectorPartyListData[m_nSelectLine].nIndex );
		}
		else
		{
			CTString strMessage = _S( 2694, "참가할 파티를 선택하지 않았습니다" );
			_pNetwork->ClientSystemMessage( strMessage );
			return;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MatchRegMemberRep()
// Desc : 파티 매칭 일반 등록 응답
// ----------------------------------------------------------------------------
void CUIPartyAuto::MatchRegMemberRep( int nErrorcode )
{
	CTString	strMessage;

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_OK:
		strMessage = _S( 2695, "초대가능 파티원 리스트에 캐릭이 등록 되었습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY:
		strMessage = _S( 2696, "이미 파티에 참여 중입니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG:
		strMessage = _S( 2697, "이미 파티 등록이 되어 있습니다" );
		break;
	}

	CloseAutoParty();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MatchRegPartyRep()
// Desc : 파티 매칭 일반 등록 응답
// ----------------------------------------------------------------------------
void CUIPartyAuto::MatchRegPartyRep( int nErrorcode )
{
	CTString	strMessage;

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_OK:
		strMessage = _S( 2698, "참가가능 파티 리스트에 파티가 등록 되었습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS:
		strMessage = _S( 2699, "파티장만 파티등록 신청을 할 수 있습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_FULL:
		strMessage = _S( 2700, "파티인원이 가득차 있습니다" );
		break;
	case MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG:
		strMessage = _S( 2701, "이미 리스트에 등록된 상태입니다" );
		break;
	}

	CloseAutoParty();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 2667, "파티 등록" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : AddPartyData()
// Desc : 파티 리스트 데이터 추가
// ----------------------------------------------------------------------------
void CUIPartyAuto::AddPartyData( int nIndex, int nLvdiff, int nZone, 
								CTString strComment, DWORD dwFlag, int limitLv, int nPtType )
{
	PartyListData PartyData;

	PartyData.nIndex = nIndex;
	PartyData.nLeveldiff = nLvdiff;
	PartyData.nZone = nZone;
	PartyData.strSubject = strComment;
	PartyData.dwJobflag = dwFlag;
	PartyData.nLimitLv = limitLv;
	PartyData.nPartyType = nPtType;

	m_vectorPartyListData.push_back( PartyData );
}
// ----------------------------------------------------------------------------
// Name : AddCharData()
// Desc : 캐릭터 데이터 추가( 테스트용 )
// ----------------------------------------------------------------------------
void CUIPartyAuto::AddCharData( int nIndex, int nLvdiff, int nZone,
							   CTString strCharName, int nJob, int limitLv, int nPtType )
{
	PartyListData CharData;

	CharData.nIndex = nIndex;
	CharData.nLeveldiff = nLvdiff;
	CharData.nZone = nZone;
	CharData.strSubject = strCharName;
	
	if( nJob == TITAN ) { CharData.dwJobflag |= JF_TITAN; }
	else if( nJob == KNIGHT ) { CharData.dwJobflag |= JF_KNIGHT; }
	else if( nJob == HEALER ) { CharData.dwJobflag |= JF_HEALER; }
	else if( nJob == MAGE ) { CharData.dwJobflag |= JF_MAGE; }
	else if( nJob == ROGUE ) { CharData.dwJobflag |= JF_ROGUE; }
	else if( nJob == SORCERER ) { CharData.dwJobflag |= JF_SORCERER; }
#ifdef CHAR_EX_ROGUE
	else if( nJob == EX_ROGUE ) { CharData.dwJobflag |= JF_EX_ROGUE; }	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	else if( nJob == EX_MAGE ) { CharData.dwJobflag |= JF_EX_MAGE; }	// 2013/01/08 jeil EX메이지 추가
#endif
	CharData.nLimitLv = limitLv;
	CharData.nPartyType = nPtType;

	m_vectorPartyListData.push_back( CharData );
}
// ----------------------------------------------------------------------------
// Name : ListSelectLine()
// Desc :
// ----------------------------------------------------------------------------
int CUIPartyAuto::ListSelectLine( int nY )
{
	const int ndivide = 26;
	int nSelY = nY - ( m_nPosY + 58 );

	nSelY /= ndivide;

	return nSelY;
}

// ----------------------------------------------------------------------------
// Name : ReceiveMemberData()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ReceiveMemberData( CNetworkMessage *istr )
{
	SLONG	slCnt;
	SLONG	slPageNum;

	(*istr) >> slPageNum;
	(*istr) >> slCnt;

	if( slPageNum < 0 || slCnt == 0 )	// 잘못된 페이지 or 목록 없음
		return;

	m_vectorPartyListData.clear();
	m_nCurrentPageNum = slPageNum;

	for( SLONG i=0; i<slCnt; i++ )
	{
		SLONG		slIndex, slZone;
		SBYTE		sbLvdiff, sbPtType, sbJob;
		CTString	strName;

		PartyListData	TempData;
		
		(*istr) >> slIndex;	// 캐릭터 식별자(n)
		(*istr) >> strName;	// 캐릭터 이름(str)
		(*istr) >> sbLvdiff;	// 레벨 제한(c)
		(*istr) >> slZone;	// 위치(n)
		(*istr) >> sbJob;		// 직업(c)
		(*istr) >> sbPtType;	// 파티 종류(c)

		TempData.nIndex = (int)slIndex;
		TempData.strSubject = strName;
		TempData.nLeveldiff = (int)sbLvdiff;
		TempData.nZone = (int)slZone;

		if( sbJob == 0 )	TempData.dwJobflag = JF_TITAN;
		else if( sbJob == 1 )	TempData.dwJobflag = JF_KNIGHT;
		else if( sbJob == 2 )	TempData.dwJobflag = JF_HEALER;
		else if( sbJob == 3 )	TempData.dwJobflag = JF_MAGE;
		else if( sbJob == 4 )	TempData.dwJobflag = JF_ROGUE;
		else if( sbJob == 5 )	TempData.dwJobflag = JF_SORCERER;
		else if( sbJob == 6 )	TempData.dwJobflag = JF_NIGHTSHADOW;
#ifdef CHAR_EX_ROGUE
		else if( sbJob == 7 )	TempData.dwJobflag = JF_EX_ROGUE;	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
		else if( sbJob == 8 )	TempData.dwJobflag = JF_EX_MAGE;	// 2013/01/08 jeil EX메이지 추가
#endif
		TempData.nPartyType = (int)sbPtType;

		m_vectorPartyListData.push_back( TempData );
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveMemberData()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ReceivePartyData( CNetworkMessage *istr )
{
	SLONG	slCnt;
	SLONG	slPageNum;

	(*istr) >> slPageNum;
	(*istr) >> slCnt;

	if( slPageNum < 0 || slCnt == 0 )	// 잘못된 페이지 or 목록 없음
		return;

	m_nCurrentPageNum = slPageNum;	
	m_vectorPartyListData.clear();

	for( SLONG i=0; i<slCnt; i++ )
	{
		SLONG		slIndex, slZone, slJobFlag;
		SBYTE		sbLvdiff, sbPtType, sbLimitLv;
		CTString	strName;
		// [ldy1978220 2011/6/7] 파티 오토 매칭 개선 
		SLONG		slPartyNum = 2;

		PartyListData	TempData;

		(*istr) >> slIndex;	// 파티장 식별자(n)
		(*istr) >> sbLvdiff;	// 레벨 제한(c)
		(*istr) >> slZone;	// 위치(n)
		// [ldy1978220 2011/6/7] 파티 오토 매칭 개선 
		(*istr) >> slPartyNum;		// 파티 매칭 인원 
		(*istr) >> slJobFlag;		// 직업(n)
		(*istr) >> sbPtType;	// 파티 종류(c)
		(*istr) >> sbLimitLv;		// 제한 레벨( 쓰지 않음 )(c)
		(*istr) >> strName;	// 파티 이름(str)

		TempData.nIndex = (int)slIndex;
		TempData.nLeveldiff = (int)sbLvdiff;
		TempData.nZone = (int)slZone;
		TempData.nPartyNum = (int)slPartyNum;
		TempData.dwJobflag = (DWORD)slJobFlag;
		TempData.nPartyType = (int)sbPtType;
		TempData.nLimitLv = (int)sbLimitLv;
		TempData.strSubject = strName;

		m_vectorPartyListData.push_back( TempData );
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePartyInviteMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ReceivePartyInviteMessage( int nErrorcode, CNetworkMessage *istr )
{
	CTString strMessage;

	CloseAutoParty();

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_OK:
		{
			SBYTE sbPartyType;
			SLONG slBoss, slDest;
			CTString strBossName, strDestName;

			(*istr) >> sbPartyType;
			(*istr) >> slBoss;
			(*istr) >> strBossName;
			(*istr) >> slDest;
			(*istr) >> strDestName;

			SLONG slMyIndex = _pNetwork->MyCharacterInfo.index; 

			CUIManager* pUIManager = CUIManager::getSingleton();

			if( slBoss == slMyIndex )
			{
				GAMEDATAMGR()->GetPartyInfo()->SetPartyRequested( TRUE );
				GAMEDATAMGR()->GetPartyInfo()->SetDestName( strDestName );
			}

			GAMEDATAMGR()->GetPartyInfo()->PartyInvite( sbPartyType, slBoss, strBossName );

			return;
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_NOT_FOUND:
		{
			strMessage = _S( 2702, "등록 목록에 없는 유저입니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_LEVEL:
		{
			strMessage = _S( 2703, "파티에 들어오기에는 레벨이 적합하지 않습니다" );	
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_NOBOSS:
		{
			strMessage = _S( 2704, "파티장만이 초대할 수 있습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE:
		{
			strMessage = _S( 2705, "파티 타입이 맞지 않습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET:
		{
			strMessage = _S( 396, "대상이 이미 파티 중입니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_FULL:
		{
			strMessage = _S( 2700, "파티 인원이 가득 차있습니다." );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST:
		{
			strMessage = _S( 2708, "대상은 파티 요청 중입니다" );
		}
		break;
	}

	_pNetwork->ClientSystemMessage( strMessage );
}

// ----------------------------------------------------------------------------
// Name : ReceivePartyInviteMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::ReceivePartyJoinMessage( int nErrorcode, CNetworkMessage *istr )
{
	CTString strMessage;

	CloseAutoParty();

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_OK:
		{
			SBYTE sbPartyType, sbCharJob;
			SLONG slBoss, slChar;
			CTString strBossName, strCharName;

			(*istr) >> sbPartyType;
			(*istr) >> slBoss;
			(*istr) >> strBossName;
			(*istr) >> slChar;
			(*istr) >> strCharName;
			(*istr) >> sbCharJob;

			PartyMatchJoin( sbPartyType, slBoss, strBossName, slChar, strCharName, sbCharJob );
			return;
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET:
		{
			strMessage = _S( 2709, "이미 다른 파티에 가입중입니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_NOT_FOUND:
		{
			strMessage = _S( 2710, "매칭 목록에 없습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_LEVEL:
		{
			strMessage = _S( 2711, "파티에 참가하기에는 레벨이 맞지 않습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL:
		{
			strMessage = _S( 2712, "레벨 제한 조건을 만족하지 못합니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_JOB:
		{
			strMessage = _S( 2713, "직업이 조건에 만족하지 않습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_FULL:
		{
			strMessage = _S( 2700, "파티 인원이 가득 차있습니다." );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST:
		{
			strMessage = _S( 2715, "초대 요청자가 있어서 참가신청을 할 수 없습니다" );
		}
		break;
	}

	_pNetwork->ClientSystemMessage( strMessage );
}

// ----------------------------------------------------------------------------
// Name : PartyMatchJoin()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::PartyMatchJoin( SBYTE sbType, SLONG slBoss, CTString strBossName, 
										SLONG slChar, CTString strCharName, SBYTE sbCharJob )
{
	CTString strMessage;
	CTString strJob;

	if( sbCharJob == TITAN ) { strJob = _S( 43, "타이탄" ); }
	else if( sbCharJob == KNIGHT ) { strJob = _S( 44, "기사" ); }
	else if( sbCharJob == HEALER ) { strJob = _S( 45, "힐러" ); }
	else if( sbCharJob == MAGE ) { strJob = _S( 46, "메이지" ); }
	else if( sbCharJob == ROGUE ) { strJob = _S( 47, "로그" ); }
	else if( sbCharJob == SORCERER ) { strJob = _S( 48, "소서러" ); }
#ifdef CHAR_EX_ROGUE
	else if( sbCharJob == EX_ROGUE ) { strJob = _S( 5732, "EX로그" ); }// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	else if( sbCharJob == EX_MAGE ) { strJob = _S( 5820, "아크메이지" ); }// 2013/01/08 jeil EX메이지 추가
#endif
	m_nCharIndex = (int)slChar;
	m_bIsPartyRequested = TRUE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	SLONG slMyIndex = _pNetwork->MyCharacterInfo.index; 

	if( slBoss == slMyIndex )
	{
		strMessage.PrintF( _S( 2716, "%s님(%s)이 파티 가입 신청을 하셨습니다. 허락하시겠습니까?" ), strCharName, strJob );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_YESNO, UI_PARTYAUTO, MSGCMD_PARTYAUTO_JOIN_ALLOW );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		strMessage.PrintF( _S( 2717, "파티장 %s에게 파티참가를 요청 중입니다" ), strBossName );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 215, "파티" ), UMBS_CANCEL, UI_PARTYAUTO, MSGCMD_PARTYAUTO_JOIN_ALLOW );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
// Name : SendPartyReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIPartyAuto::SendPartyReq( int nNum )
{
	if( (m_nSelectState + nNum) < 0 )
		return;

	int nSendPage;
	int nJob = m_cmbNeedClass.GetCurSel();
	// 입수우선이 없으르로 중간에 +1을 함
	int nPtType = (m_cmbPartyType.GetCurSel() == 2 ? m_cmbPartyType.GetCurSel()+1 : m_cmbPartyType.GetCurSel());
	int nLvLimit = m_cbtnLevelLimit.IsChecked() ? 1 : 0;
	
	m_nSelectLine = -1;

	if( nNum != 0 )
	{
		nSendPage = m_nCurrentPageNum+nNum;
	}
	else if( nNum == 0 )
	{
		nSendPage = 0;
		m_vectorPartyListData.clear();
	}

	if( m_nSelectState == INVITE_LIST )
		_pNetwork->SendPartyListReq( nSendPage, nJob-1, nLvLimit, nPtType-1, FALSE );
	else if( m_nSelectState = PARTY_LIST )
		_pNetwork->SendPartyListReq( nSendPage, nJob-1, nLvLimit, nPtType-1, TRUE );
}

// ----------------------------------------------------------------------------
// Name : TextScroll()
// Desc : 문자열을 스크롤 한다.(한글자씩)
// ----------------------------------------------------------------------------
void CUIPartyAuto::TextScroll( CTString strText )
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	CTString strTemp1, strTemp2;
	CTString strSaveSplit = strText;
	char cCurrent;

	int nX, nY;	
	int nSaveHeight = m_nSelectLine * 25;
	int nStartPos = m_nPosX + 64; // 텍스트 맨 앞 위치
	int nEndPos = nStartPos +  ( 30 * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) );

	nY = m_nPosY+LESH_STRING+nSaveHeight;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - mtmTextScrollTime;

	if( llCurDelay < TEXT_DELAY_TIME )
	{
		if( m_bTextDir )
		{
			strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
			strSaveSplit = strTemp2;

			if( strSaveSplit.Length() > 30 )
			{
				BOOL b2Byte = Is2ByteChar( 30, 0, strSaveSplit );
				
				if( b2Byte )
				{ strSaveSplit.Split( 31, strTemp1, strTemp2 ); }
				else
				{ strSaveSplit.Split( 30, strTemp1, strTemp2 ); }

				strSaveSplit = strTemp1;				
			}
			
			nX = nStartPos;
			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}
		else
		{
			strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
			strSaveSplit = strTemp1;

			int nStrWidth = strSaveSplit.Length() * 
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			nX = nEndPos-nStrWidth;

			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}

		return;
	}
	// 시간 갱신
	mtmTextScrollTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	if( m_bTextDir )
	{
		if( m_FrontChPos <= strSaveSplit.Length() )
		{
			cCurrent = strSaveSplit[m_FrontChPos];

			if( cCurrent == '\n' || cCurrent == '\r' || cCurrent == ' ' )
			{
				m_FrontChPos++;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}
			else if( cCurrent & 0x80 )
			{
				m_FrontChPos += 2;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}
			else
			{
				m_FrontChPos += 1;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp2;
			}

			nX = nStartPos;
			if( strSaveSplit.Length() > 30 )
			{
				BOOL b2Byte = Is2ByteChar( 30, 0, strSaveSplit );
				
				if( b2Byte )
				{ strSaveSplit.Split( 31, strTemp1, strTemp2 ); }
				else
				{ strSaveSplit.Split( 30, strTemp1, strTemp2 ); }
					
				strSaveSplit = strTemp1;				
			}

			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );
		}
		else
		{
			m_bTextDir = FALSE;
			m_FrontChPos = 0;
		}
	}
	else
	{
		if( m_FrontChPos < 30 )
		{
			cCurrent = strSaveSplit[m_FrontChPos];

			if( cCurrent == '\n' || cCurrent == '\r' || cCurrent == ' ' )
			{
				m_FrontChPos++;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			else if( cCurrent & 0x80 )
			{
				m_FrontChPos += 2;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			else
			{
				m_FrontChPos += 1;
				strSaveSplit.Split( m_FrontChPos, strTemp1, strTemp2 );
				strSaveSplit = strTemp1;
			}
			int nStrWidth = strSaveSplit.Length() * 
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			nX = nEndPos-nStrWidth;
			pDrawPort->PutTextEx( strSaveSplit, nX, nY, 0xFFF284FF );

			if( m_FrontChPos >= 30 )
			{
				m_bTextDir = TRUE;
				m_FrontChPos = 0;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Is2ByteChar()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIPartyAuto::Is2ByteChar( int nCharIndex, int nFirstCheck, CTString strText )
{
	BOOL	b2ByteChar = FALSE;
	const char	*pcTemp = &strText[nFirstCheck];

	for( int iPos = nFirstCheck; iPos < nCharIndex; iPos++, pcTemp++ )
	{
#if defined (G_THAI)
		b2ByteChar = FALSE;
#else
		if( (*pcTemp & 0x80) )
		{
			if( _pUIFontTexMgr->GetLanguage() == FONT_JAPANESE && ((UCHAR)*pcTemp >= 0xa1 && (UCHAR)*pcTemp <= 0xdf) ) 
				b2ByteChar = FALSE;
			else b2ByteChar = !b2ByteChar;
		}
		else
			b2ByteChar = FALSE;
#endif
	}

	return b2ByteChar;
}