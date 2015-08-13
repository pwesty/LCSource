
#include "stdh.h"

#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITalk.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Help/Util_Help.h>

// Define position
#define	MESSENGER_TITLE_SX					25
#define	MESSENGER_TITLE_SY					6

#define RESIZE_RANGE						10		// Window크기 재 조정 영역

#define MAX_MEMBER_COUNT					30


// 좌우 크기 늘릴 때 사용한 한개의 너비
#define UNIT_SIZE_DEFAULT					40
#define UNIT_SIZE_GUIDLINE					40
#define UNIT_SIZE_RESIZE_BOTTOM				28

#define	TOP_HEIGHT							25		// 타이틀 바의 높이
#define MY_INFO_HEIGHT						19		// 자신의 정보가 표시 되는 창의 높이 
#define BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT	5+5		// 스크롤바가 포함되어 있는 중간 창의 높이 
#define BACK_BOTTOM_HEIGHT					13		// 아랫쪽 창의 높이

#define MYINFO_ICON_X						10		
#define MYINFO_NAME_X						30
#define MYINFO_SET_X						155 - MYINFO_ICON_X +108
#define MYINFO_Y							28

#define MULTI_OFFSET_X						1
#define MULTI_OFFSET_Y						17

#define BUTTON_Y							257+17

#define BUTTON_GROUP_X						9
#define BUTTON_BLOCK_X						BUTTON_GROUP_X +78+4
#define BUTTON_INSERT_X						BUTTON_BLOCK_X +78+4
#define BUTTON_DELETE_X						BUTTON_INSERT_X +52+4



#define Tab_Y								47
#define Tab_MEMBER_X						40
#define Tab_BLOCK_X							179

#define COLOR_MYNAME						0xe1b300ff	

#define MAX_TALK							10		//최대로 띄울 수 있는 채팅창수

#define MESSENGER_LEVEL						0

#define BLOCK_GROUP_INDEX					-1

static CTString s_strReqCharName;
static bool	g_bUseChat[MAX_TALK];							//채팅창 사용 확인
static int	g_nMakeCharIndex[MAX_TALK];						//채팅창 생성 캐릭 인덱스
static int	g_nChatIndex[MAX_TALK];							//채팅창 인덱스

//extern INDEX g_iCountry;

// UI Renewal [9/16/2009 rumist] NHN service.
extern INDEX g_iXPosInMessanger;
extern INDEX g_iYPosInMessanger;


//채팅 글자 색
#define MAX_COLOR							9
COLOR ChatColor[MAX_COLOR] =
{
	0xFFFFFFFF,		// 흰색
	0x8B8C8AFF,		// 회색
	0x38EBFFFF,		// 파랑
	0xA53FBFFF,		// 보라
	0x3936E2FF,		// 진한파랑
	0x16844BFF,		// 녹색
	0x7F0E20FF,		// 빨강
	0xF78142FF,		// 주황
	0xCCBD1CFF,		// 노랑
};

//------------------------------------------------------------------------------
// CUIMessenger::CUIMessenger
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMessenger::CUIMessenger()
	: m_nTarIndex(0)
	, m_bPremiumChar(false)
{
	Reset();
	m_ptdMenuTexture = NULL;
}


//------------------------------------------------------------------------------
// CUIMessenger::~CUIMessenger
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMessenger::~CUIMessenger()
{
	Reset();
	STOCK_RELEASE(m_ptdMenuTexture);
}


//------------------------------------------------------------------------------
// CUIMessenger::ResetPosition
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIMessenger::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInMessanger = GetPosX();
	g_iYPosInMessanger = GetPosY();
}

//------------------------------------------------------------------------------
// CUIMessenger::AdjustPosition
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


//------------------------------------------------------------------------------
// CUIMessenger::Create
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	//m_strTitle = _S( 1611, "메신저" ); 
	m_strTitle = _S( 2981, "라스트 카오스 메신저"); 

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Set Rect
	m_rcTitle.SetRect( 0, 0, 512, 22 );
	ReSizing( RESIZE_RANGE, m_nWidth, m_nHeight );
	
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Set Background RectUV
	m_rt3BackT.rtL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rt3BackT.rtM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rt3BackT.rtR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );

	m_rt3BackMT.SetUV3( 36, 69, 76, 75, 76, 69, 212, 75, 212, 69, 252, 75, fTexWidth, fTexHeight );
	m_rt3BackMM.SetUV3( 36, 75, 76, 81, 76, 75, 212, 81, 212, 75, 252, 81, fTexWidth, fTexHeight );
	m_rt3BackMB.SetUV3( 36, 81, 76, 87, 76, 81, 212, 87, 212, 81, 252, 87, fTexWidth, fTexHeight );

	m_rt3Tab.SetUV3( 36, 108, 76, 111, 76, 108, 212, 111, 76, 108, 36, 111, fTexWidth, fTexHeight );
	m_rtTab.SetUV(375, 2, 492, 45, fTexWidth, fTexHeight);
	
	/***
	m_rt3BackFTab0.SetUV3( 36, 108, 76, 111, 76, 108, 212, 111, 76, 108, 212, 111, fTexWidth, fTexHeight );
	m_rt3BackFTab1.SetUV3( 0, 35, 40, 37, 40, 35, 176, 37, 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rt3BackBTab0.SetUV3( 76, 108, 212, 111, 76, 108, 212, 111, 76, 108, 36, 111, fTexWidth, fTexHeight );
	m_rt3BackBTab1.SetUV3( 40, 35, 176, 37, 40, 35, 176, 37, 40, 37, 0, 35, fTexWidth, fTexHeight );
	***/
	
	m_rt3BackMST.SetUV3( 36, 103, 76, 108, 76, 103, 212, 108, 212, 103, 252, 108, fTexWidth, fTexHeight );
	m_rt3BackMSM.SetUV3( 36, 108, 76, 111, 76, 108, 212, 111, 76, 108, 36, 111, fTexWidth, fTexHeight );
	//m_rt3BackMSM.SetUV3( 36, 108, 43, 111, 375, 2, 492, 45, 43, 108, 36, 111, fTexWidth, fTexHeight );
	m_rt3BackMSB.SetUV3( 36, 111, 76, 116, 76, 111, 212, 116, 212, 111, 252, 116, fTexWidth, fTexHeight );

	m_rt3BackM.SetUV3( 0, 35, 40, 37, 40, 35, 176, 37, 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rt3BackL.SetUV3( 73, 162, 101, 175, 101, 162, 129, 175, 129, 162, 157, 175, fTexWidth, fTexHeight );

	// bottom button
	m_btmSetState.Create( this, CTString( "" ), MYINFO_SET_X, MYINFO_Y, 13, 13 );
	m_btmSetState.SetUV( UBS_IDLE, 230, 147, 243, 160, fTexWidth, fTexHeight );
	m_btmSetState.SetUV( UBS_CLICK, 230, 161, 243, 174, fTexWidth, fTexHeight );
	m_btmSetState.CopyUV( UBS_IDLE, UBS_ON );
	m_btmSetState.CopyUV( UBS_IDLE, UBS_DISABLE ); 


	//Tab region
	m_rcFriendTab.SetRect(10, 47, 144, 65);
	m_rcBlockTab.SetRect(144, 47, 278, 65);
	m_rcBoard.SetRect(GROUP_X, GROUP_Y, 271-GROUP_X, GROUP_Y+m_nBoardHeight);
	//m_mgFriend.Create( this, GROUP_X, GROUP_Y, 260, 15 );

	//m_btnClose.Create( this, CTString( "" ), 151, 4, 14, 14 );   ==>
	m_btnClose.Create( this, CTString( "" ), 151+108, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE ); 

	//m_btnGroup.Create( this, _S( 1612,  "그룹"  ), BUTTON_GROUP_X, BUTTON_Y, 52, 21 );   ==>
	m_btnGroup.Create( this, _S(2982, "그룹관리"  ), BUTTON_GROUP_X, BUTTON_Y, 78, 21 ); 
	m_btnGroup.SetUV( UBS_IDLE, 0, 173, 53, 194, fTexWidth, fTexHeight );
	m_btnGroup.SetUV( UBS_CLICK, 0, 195, 53, 216, fTexWidth, fTexHeight );
	m_btnGroup.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGroup.CopyUV( UBS_IDLE, UBS_DISABLE ); 


	m_btnBlock.Create( this, _S(2983, "차단기능"  ), BUTTON_BLOCK_X, BUTTON_Y, 78, 21 ); 
	m_btnBlock.SetUV( UBS_IDLE, 0, 173, 53, 194, fTexWidth, fTexHeight );
	m_btnBlock.SetUV( UBS_CLICK, 0, 195, 53, 216, fTexWidth, fTexHeight );
	m_btnBlock.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBlock.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	//m_btnMember.Create( this, _S( 1613,  "친구"  ), BUTTON_MEMBER_X, BUTTON_Y, 52, 21 ); 
	m_btnInsert.Create( this, _S(550, "추가"  ), BUTTON_INSERT_X, BUTTON_Y, 52, 21 ); 
	m_btnInsert.SetUV( UBS_IDLE, 0, 173, 53, 194, fTexWidth, fTexHeight );
	m_btnInsert.SetUV( UBS_CLICK, 0, 195, 53, 216, fTexWidth, fTexHeight );
	m_btnInsert.CopyUV( UBS_IDLE, UBS_ON );
	m_btnInsert.CopyUV( UBS_IDLE, UBS_DISABLE );


	m_btnDelete.Create( this, _S( 1614,  "삭제"  ), BUTTON_DELETE_X, BUTTON_Y, 52, 21 ); 
	m_btnDelete.SetUV( UBS_IDLE, 0, 173, 53, 194, fTexWidth, fTexHeight );
	m_btnDelete.SetUV( UBS_CLICK, 0, 195, 53, 216, fTexWidth, fTexHeight );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_DISABLE ); 

	// Scroll Bar
	//m_sbMemberList.Create( this, 163, 45, 9, m_nBoardHeight + 3 - 2 );   ==>
	m_sbMemberList.Create( this, 163+108, 65, 9, m_nBoardHeight + 6);
	m_sbMemberList.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbMemberList.SetScrollPos( 0 );
	m_sbMemberList.SetScrollRange( 0 );
	m_sbMemberList.SetCurItemCount( 0 );
	//m_sbMemberList.SetItemsPerPage( ( m_nBoardHeight + GROUP_GAP ) / ( _pUIFontTexMgr->GetLineHeight()+2 ) );	
	m_sbMemberList.SetItemsPerPage( 13 );	

	// Up button
	m_sbMemberList.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbMemberList.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbMemberList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbMemberList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbMemberList.SetBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_sbMemberList.SetBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_sbMemberList.SetBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
	// Down button
	m_sbMemberList.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbMemberList.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbMemberList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbMemberList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	//m_sbMemberList.SetWheelRect( -181, -1, 180, 154 );
	m_sbMemberList.SetWheelRect( -181-108, -1, 180+108, m_nBoardHeight );
	
	CreateTrackPopup( fTexWidth, fTexHeight );
	CreateGroup( fTexWidth, fTexHeight );


	//서브 메뉴 툴팁에 사용될 화살표 텍스쳐 추가
	m_ptdMenuTexture = CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	fTexWidth = m_ptdMenuTexture->GetPixWidth();
	fTexHeight = m_ptdMenuTexture->GetPixHeight();

	m_rtArrow.SetUV(34, 117, 42, 125, fTexWidth, fTexHeight);		//노란 화살표
	//m_rtArrow.SetUV(11, 117, 19, 125, fTexWidth, fTexHeight);		//파란 화살표
}


//------------------------------------------------------------------------------
// CUIMessenger::CreateGroup
// Explain:  그룹 생성 ( 너무 복잡해서 뺐당)
// Date : 2005-05-22,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::CreateGroup( float fTexWidth, float fTexHeight )
{
	m_fTexWidth =fTexWidth;
	m_fTexHeight =fTexHeight;

	// Icon Set RectUV
	m_rcIcon[TITAN][OFFLINE].SetUV( 55, 211, 73, 226, fTexWidth, fTexHeight );
	m_rcIcon[TITAN][ONLINE].SetUV( 55, 195, 73, 210, fTexWidth, fTexHeight );
	m_rcIcon[TITAN][LEFT_POSITION].SetUV( 55, 179, 73, 194, fTexWidth, fTexHeight );

	m_rcIcon[KNIGHT][OFFLINE].SetUV( 74, 211, 92, 226, fTexWidth, fTexHeight );
	m_rcIcon[KNIGHT][ONLINE].SetUV( 74, 195, 92, 210, fTexWidth, fTexHeight );
	m_rcIcon[KNIGHT][LEFT_POSITION].SetUV( 74, 179, 92, 194, fTexWidth, fTexHeight );

	m_rcIcon[MAGE][OFFLINE].SetUV( 93, 211, 111, 226, fTexWidth, fTexHeight );
	m_rcIcon[MAGE][ONLINE].SetUV( 93, 195, 111, 210, fTexWidth, fTexHeight );
	m_rcIcon[MAGE][LEFT_POSITION].SetUV( 93, 179, 111, 194, fTexWidth, fTexHeight );

	m_rcIcon[HEALER][OFFLINE].SetUV( 112, 211, 130, 226, fTexWidth, fTexHeight );
	m_rcIcon[HEALER][ONLINE].SetUV( 112, 195, 130, 210, fTexWidth, fTexHeight );
	m_rcIcon[HEALER][LEFT_POSITION].SetUV( 112, 179, 130, 194, fTexWidth, fTexHeight );

	m_rcIcon[ROGUE][OFFLINE].SetUV( 131, 211, 149, 226, fTexWidth, fTexHeight );
	m_rcIcon[ROGUE][ONLINE].SetUV( 131, 195, 149, 210, fTexWidth, fTexHeight );
	m_rcIcon[ROGUE][LEFT_POSITION].SetUV( 131, 179, 149, 194,fTexWidth, fTexHeight );

	m_rcIcon[SORCERER][OFFLINE].SetUV( 93, 227, 111, 242,fTexWidth, fTexHeight );
	m_rcIcon[SORCERER][ONLINE].SetUV( 74, 227, 92, 242,fTexWidth, fTexHeight );
	m_rcIcon[SORCERER][LEFT_POSITION].SetUV( 55, 227, 73, 242, fTexWidth, fTexHeight );
	
	// TO-KR-T20091127-012 : add night shadow job icon to messenger. [12/1/2009 rumist]
	m_rcIcon[NIGHTSHADOW][OFFLINE].SetUV( 475, 159, 493, 174, fTexWidth, fTexHeight );
	m_rcIcon[NIGHTSHADOW][ONLINE].SetUV( 456, 159, 474, 174, fTexWidth, fTexHeight );
	m_rcIcon[NIGHTSHADOW][LEFT_POSITION].SetUV( 437, 159, 455, 174, fTexWidth, fTexHeight );

#ifdef CHAR_EX_ROGUE	// [2012/08/27 : Sora] EX로그 추가
	m_rcIcon[EX_ROGUE][OFFLINE].SetUV( 131, 211, 149, 226, fTexWidth, fTexHeight );
	m_rcIcon[EX_ROGUE][ONLINE].SetUV( 131, 195, 149, 210, fTexWidth, fTexHeight );
	m_rcIcon[EX_ROGUE][LEFT_POSITION].SetUV( 131, 179, 149, 194,fTexWidth, fTexHeight );
#endif
#ifdef CHAR_EX_MAGE		// 2013/01/08 jeil EX메이지 추가 
	m_rcIcon[EX_MAGE][OFFLINE].SetUV( 93, 211, 111, 226, fTexWidth, fTexHeight );
	m_rcIcon[EX_MAGE][ONLINE].SetUV( 93, 195, 111, 210, fTexWidth, fTexHeight );
	m_rcIcon[EX_MAGE][LEFT_POSITION].SetUV( 93, 179, 111, 194, fTexWidth, fTexHeight );
#endif

	m_mgBlock.Create( this, GROUP_X, GROUP_Y, GROUP_WIDTH, 15 );
	m_mgBlock.SetGroupName( _S(2985, "차단된 유저" ) ); 
	m_mgBlock.SetIndex( BLOCK_GROUP_INDEX );

	m_mgBlock.m_rtSelectBar.SetUV(187, 46, 204, 61, fTexWidth, fTexHeight);
	
	m_mgBlock.m_btnExtension.Create( (CUIWindow*)&m_mgBlock, CTString( "" ), 0, 0, 14, 14 );
	m_mgBlock.m_btnExtension.SetUV( UBS_IDLE, 230, 117, 244, 131, fTexWidth, fTexHeight );
	m_mgBlock.m_btnExtension.SetUV( UBS_CLICK, 230, 132, 244, 146, fTexWidth, fTexHeight );
	m_mgBlock.m_btnExtension.CopyUV( UBS_IDLE, UBS_ON );
	m_mgBlock.m_btnExtension.CopyUV( UBS_IDLE, UBS_DISABLE ); 
	
	m_mgBlock.m_btnReduction.Create((CUIWindow*)&m_mgBlock, CTString( "" ), 0, 0, 14, 14 );
	m_mgBlock.m_btnReduction.SetUV( UBS_IDLE, 230, 33, 244, 47, fTexWidth, fTexHeight );
	m_mgBlock.m_btnReduction.SetUV( UBS_CLICK, 230, 48, 244, 62, fTexWidth, fTexHeight );
	m_mgBlock.m_btnReduction.CopyUV( UBS_IDLE, UBS_ON );
	m_mgBlock.m_btnReduction.CopyUV( UBS_IDLE, UBS_DISABLE );
			
	
	/*****
	// 테스트로 맴버들의 리스트 받아서 입력 한다.
	int nCreateMemberCount = 20;

	for( int i =0; i < nCreateMemberCount; ++i )
	{
		CTString strName;
		strName.PrintF( "%c%s%d", rand()%25+65, "일일일일일일일", i );
		m_vecGroup[0]->AddMember(CMemberInfo( i, 0, strName, (rand()%(TOTAL_JOB)), (rand()%(TOTAL_CONDITION)) ) );
		//m_vecGroup[1]->AddMember(CMemberInfo( i, 16, strName, (rand()%(TOTAL_JOB)), (rand()%(TOTAL_CONDITION)) ) );

	}
	for(i =0; i <15; ++i )
	{
		CTString strName;
		strName.PrintF( "%c%s-%d", rand()%25+65, "이이이이이이이", i );
		m_mgBlock.AddMember(CMemberInfo( i, 0, strName, (rand()%(TOTAL_JOB)), (rand()%(TOTAL_CONDITION)) ) );
	}
	
	m_vecGroup[0]->ReloadMember();
	//m_vecGroup[1]->ReloadMember();
	m_mgBlock.ReloadMember();
	/*****/
	ResizeScrollBar();
}


//------------------------------------------------------------------------------
// CUIMessenger::CreateTrackPopup
// Explain:  이것도 마찬가지도 Create에서 복잡해서 때어 냈당
// Date : 2005-05-22,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::CreateTrackPopup( float fTexWidth, float fTexHeight )
{
	// 내 상태 설정 메뉴
	m_tpSetMyState.m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_tpSetMyState.m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_tpSetMyState.Create( this, 93+108, 43, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE );
	m_tpSetMyState.SetScrollBar( FALSE );
	m_tpSetMyState.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpSetMyState.SetOverColor( 0xF8E1B5FF );
	m_tpSetMyState.SetSelectColor( 0xF8E1B5FF );

	m_tpSetMyState.AddMenuList( _S( 1615,  "오프라인"  ) ); 
	m_tpSetMyState.AddMenuList( _S( 1616,  "온라인"  ) );	
	m_tpSetMyState.AddMenuList( _S( 1617,  "자리비움"  ) );
	
	// 그룹 관련 메뉴 설정 
	m_tpGroup.m_rtBackUL.CopyUV( m_tpSetMyState.m_rtBackUL );
	m_tpGroup.m_rtBackUM.CopyUV( m_tpSetMyState.m_rtBackUM );
	m_tpGroup.m_rtBackUR.CopyUV( m_tpSetMyState.m_rtBackUR );
	m_tpGroup.m_rtBackML.CopyUV( m_tpSetMyState.m_rtBackML );
	m_tpGroup.m_rtBackMM.CopyUV( m_tpSetMyState.m_rtBackMM );
	m_tpGroup.m_rtBackMR.CopyUV( m_tpSetMyState.m_rtBackMR );
	m_tpGroup.m_rtBackLL.CopyUV( m_tpSetMyState.m_rtBackLL );
	m_tpGroup.m_rtBackLM.CopyUV( m_tpSetMyState.m_rtBackLM );
	m_tpGroup.m_rtBackLR.CopyUV( m_tpSetMyState.m_rtBackLR );

	m_tpGroup.Create( this, 6,  BUTTON_Y +21, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE );
	m_tpGroup.SetScrollBar( FALSE );
	m_tpGroup.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpGroup.SetOverColor( 0xF8E1B5FF );
	m_tpGroup.SetSelectColor( 0xF8E1B5FF );

	m_tpGroup.AddMenuList( _S(2987, "새그룹 추가") +CTString("     "));
	m_tpGroup.AddMenuList( _S(2988, "그룹 삭제") +CTString("     "));
	m_tpGroup.AddMenuList( _S(2989, "그룹 이름 변경" ) +CTString("     "));
	
	//멤버 관련 메뉴 설정
	m_tpMemberMenu.m_rtBackUL.CopyUV( m_tpSetMyState.m_rtBackUL );
	m_tpMemberMenu.m_rtBackUM.CopyUV( m_tpSetMyState.m_rtBackUM );
	m_tpMemberMenu.m_rtBackUR.CopyUV( m_tpSetMyState.m_rtBackUR );
	m_tpMemberMenu.m_rtBackML.CopyUV( m_tpSetMyState.m_rtBackML );
	m_tpMemberMenu.m_rtBackMM.CopyUV( m_tpSetMyState.m_rtBackMM );
	m_tpMemberMenu.m_rtBackMR.CopyUV( m_tpSetMyState.m_rtBackMR );
	m_tpMemberMenu.m_rtBackLL.CopyUV( m_tpSetMyState.m_rtBackLL );
	m_tpMemberMenu.m_rtBackLM.CopyUV( m_tpSetMyState.m_rtBackLM );
	m_tpMemberMenu.m_rtBackLR.CopyUV( m_tpSetMyState.m_rtBackLR );

	m_tpMemberMenu.Create( this, 0,  0, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE, TRUE );
	m_tpMemberMenu.SetScrollBar( FALSE );
	m_tpMemberMenu.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpMemberMenu.SetOverColor( 0xF8E1B5FF );
	m_tpMemberMenu.SetSelectColor( 0xF8E1B5FF );

	m_tpMemberMenu.AddMenuList( _S(2990, "그룹이동") +CTString("     ") );
	m_tpMemberMenu.AddMenuList( _S(2991, "대화하기") );
	m_tpMemberMenu.AddMenuList( _S(338, "삭제" ) );
	m_tpMemberMenu.AddMenuList( _S(2992, "차단" ) );

	
	//그룹 관련 메뉴
	m_tpGroupMenu.m_rtBackUL.CopyUV( m_tpSetMyState.m_rtBackUL );
	m_tpGroupMenu.m_rtBackUM.CopyUV( m_tpSetMyState.m_rtBackUM );
	m_tpGroupMenu.m_rtBackUR.CopyUV( m_tpSetMyState.m_rtBackUR );
	m_tpGroupMenu.m_rtBackML.CopyUV( m_tpSetMyState.m_rtBackML );
	m_tpGroupMenu.m_rtBackMM.CopyUV( m_tpSetMyState.m_rtBackMM );
	m_tpGroupMenu.m_rtBackMR.CopyUV( m_tpSetMyState.m_rtBackMR );
	m_tpGroupMenu.m_rtBackLL.CopyUV( m_tpSetMyState.m_rtBackLL );
	m_tpGroupMenu.m_rtBackLM.CopyUV( m_tpSetMyState.m_rtBackLM );
	m_tpGroupMenu.m_rtBackLR.CopyUV( m_tpSetMyState.m_rtBackLR );

	m_tpGroupMenu.Create( this, 0,  0, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE, TRUE );
	m_tpGroupMenu.SetScrollBar( FALSE );
	m_tpGroupMenu.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpGroupMenu.SetOverColor( 0xF8E1B5FF );
	m_tpGroupMenu.SetSelectColor( 0xF8E1B5FF );

	m_tpGroupMenu.AddMenuList( _S(2987, "새그룹 추가") );
	m_tpGroupMenu.AddMenuList( _S(2988, "그룹 삭제") );
	m_tpGroupMenu.AddMenuList( _S(2989, "그룹 이름 변경") );

	//그룹 리스트 서브메뉴
	m_tpGroupList.m_rtBackUL.CopyUV( m_tpSetMyState.m_rtBackUL );
	m_tpGroupList.m_rtBackUM.CopyUV( m_tpSetMyState.m_rtBackUM );
	m_tpGroupList.m_rtBackUR.CopyUV( m_tpSetMyState.m_rtBackUR );
	m_tpGroupList.m_rtBackML.CopyUV( m_tpSetMyState.m_rtBackML );
	m_tpGroupList.m_rtBackMM.CopyUV( m_tpSetMyState.m_rtBackMM );
	m_tpGroupList.m_rtBackMR.CopyUV( m_tpSetMyState.m_rtBackMR );
	m_tpGroupList.m_rtBackLL.CopyUV( m_tpSetMyState.m_rtBackLL );
	m_tpGroupList.m_rtBackLM.CopyUV( m_tpSetMyState.m_rtBackLM );
	m_tpGroupList.m_rtBackLR.CopyUV( m_tpSetMyState.m_rtBackLR );

	m_tpGroupList.Create( this, 0,  0, 100, 1000, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE, TRUE );
	m_tpGroupList.SetScrollBar( FALSE );
	m_tpGroupList.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpGroupList.SetOverColor( 0xF8E1B5FF );
	m_tpGroupList.SetSelectColor( 0xF8E1B5FF );


	//차단 멤버 관련 메뉴 설정
	m_tpBlock.m_rtBackUL.CopyUV( m_tpSetMyState.m_rtBackUL );
	m_tpBlock.m_rtBackUM.CopyUV( m_tpSetMyState.m_rtBackUM );
	m_tpBlock.m_rtBackUR.CopyUV( m_tpSetMyState.m_rtBackUR );
	m_tpBlock.m_rtBackML.CopyUV( m_tpSetMyState.m_rtBackML );
	m_tpBlock.m_rtBackMM.CopyUV( m_tpSetMyState.m_rtBackMM );
	m_tpBlock.m_rtBackMR.CopyUV( m_tpSetMyState.m_rtBackMR );
	m_tpBlock.m_rtBackLL.CopyUV( m_tpSetMyState.m_rtBackLL );
	m_tpBlock.m_rtBackLM.CopyUV( m_tpSetMyState.m_rtBackLM );
	m_tpBlock.m_rtBackLR.CopyUV( m_tpSetMyState.m_rtBackLR );

	m_tpBlock.Create( this, 0,  0, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE, TRUE );
	m_tpBlock.SetScrollBar( FALSE );
	m_tpBlock.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpBlock.SetOverColor( 0xF8E1B5FF );
	m_tpBlock.SetSelectColor( 0xF8E1B5FF );
	
	m_tpBlock.AddMenuList( _S(3013, "차단해제"));
	m_tpBlock.AddMenuList( _S(338, "삭제"));
}


//------------------------------------------------------------------------------
// CUIMessenger::ReSizing
// Explain:  크기 재조정시 영역에 대한 정보 갱신
//			nResizeRange, nWidth, nHeight
// Date : 2005-05-18,Author: Lee Ki-hwan
// - 050522 : 크기가 변하지 않기 때문에 적용되지 않음
//------------------------------------------------------------------------------
void CUIMessenger::ReSizing( int nResizeRange, int nWidth, int nHeight )
{
	m_rcResizeLeft.SetRect( 0, m_rcTitle.Bottom, nResizeRange, nHeight - nResizeRange );
	m_rcResizeRight.SetRect( nWidth - nResizeRange, m_rcTitle.Bottom, nWidth, nHeight - nResizeRange );
	
	m_rcResizeBottomL.SetRect( 0, nHeight - nResizeRange, nResizeRange, nHeight );
	m_rcResizeBottomM.SetRect( nResizeRange, nHeight - nResizeRange, nWidth - nResizeRange, nHeight );
	m_rcResizeBottomR.SetRect( nWidth - nResizeRange, nHeight - nResizeRange, nWidth, nHeight );
}


//------------------------------------------------------------------------------
// CUIMessenger::Render
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::Render()
{
	if( IsVisible() == false )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX2;
	nX2 = m_nPosX + m_nWidth;

	RenderBackground( m_nPosX, m_nPosY );

	RenderMyInfo( m_nPosX, m_nPosY );

	RenderGroup( m_nPosX, m_nPosY );
	RenderTab( m_nPosX, m_nPosY );
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_btnClose.Render();
	m_btnGroup.Render();
	m_btnBlock.Render();
	m_btnInsert.Render();
	m_btnDelete.Render();
	m_sbMemberList.Render();
	
	pDrawPort->FlushRenderingQueue();

	// Title
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + MESSENGER_TITLE_SX,
										m_nPosY + MESSENGER_TITLE_SY );

	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	m_tpSetMyState.Render();
	m_tpGroup.Render();
	m_tpMemberMenu.Render();
	m_tpGroupMenu.Render();
	m_tpGroupList.Render();
	m_tpBlock.Render();
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();


	//서브 메뉴 화살표 렌더링
	pDrawPort->InitTextureData( m_ptdMenuTexture );
	if( m_tpGroup.IsVisible() )
	{
		RenderMenuArrow( m_tpGroup.GetAbsPosX() +105, m_tpGroup.GetAbsPosY(), 7, 7, 1);
		RenderMenuArrow( m_tpGroup.GetAbsPosX() +105, m_tpGroup.GetAbsPosY(), 7, 7, 2);
	}

	if( m_tpMemberMenu.IsVisible() )
	{
		RenderMenuArrow( m_tpMemberMenu.GetAbsPosX() +70, m_tpMemberMenu.GetAbsPosY(), 7, 7, 0);
	}
	pDrawPort->FlushRenderingQueue();

	
	//드래그 중인 멤버 렌더링
	if( m_bDragging )
		RenderSelectMember(m_nX, m_nY);
}


//------------------------------------------------------------------------------
// CUIMessenger::RenderRectUV3
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 )
{
	int nX2 = nX + nWidth;
	int nY2 = nY + nHeight;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->AddTexture( nX, nY, nX + nUnitSize, nY2,
										rtRectUV3.rtL.U0, rtRectUV3.rtL.V0, rtRectUV3.rtL.U1, rtRectUV3.rtL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX + nUnitSize, nY, nX2 - nUnitSize, nY2,
										rtRectUV3.rtM.U0, rtRectUV3.rtM.V0, rtRectUV3.rtM.U1, rtRectUV3.rtM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - nUnitSize, nY, nX2, nY2,
										rtRectUV3.rtR.U0, rtRectUV3.rtR.V0, rtRectUV3.rtR.U1, rtRectUV3.rtR.V1,
										0xFFFFFFFF );
}


//------------------------------------------------------------------------------
// CUIMessenger::RenderBack
// Explain: 창의 배경  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RenderBackground( int nX, int nY ) 
{
	int nBoardHeight = m_nBoardHeight + 18;
	// Top
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, TOP_HEIGHT, m_rt3BackT ); nY += TOP_HEIGHT;

	// My Info
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 5, m_rt3BackMT ); nY += 5;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 9, m_rt3BackMM ); nY += 9;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 5, m_rt3BackMB ); nY += 5;

	
	//Tab
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, 20, m_rt3Tab );  
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, 20, m_rt3Tab );  
	nY +=20;

	// Regist Other Player List	
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, nBoardHeight - BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT -20, m_rt3BackMSM ); 
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, nBoardHeight - BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT -20, m_rt3BackMSM ); 
	nY += nBoardHeight - BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT -20;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT, m_rt3BackMSB ); 
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT, m_rt3BackMSB ); 
	nY += BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT;

	// Bottom
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, m_nPosY + m_nHeight - BACK_BOTTOM_HEIGHT - nY, m_rt3BackM ); 
	nY += m_nPosY + ( m_nHeight - BACK_BOTTOM_HEIGHT ) - nY; 
	RenderRectUV3( UNIT_SIZE_RESIZE_BOTTOM, nX, nY, m_nWidth, BACK_BOTTOM_HEIGHT, m_rt3BackL ); 
}


//------------------------------------------------------------------------------
// CUIMessenger::RenderMyInfo
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RenderMyInfo( int nX, int nY )
{
	nY += MYINFO_Y;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Draw Icon
	pDrawPort->AddTexture( nX + MYINFO_ICON_X, nY, nX + MYINFO_ICON_X + ICON_WIDTH, nY + ICON_HEIGHT,
										m_rcIcon[m_MyInfo.m_eJob][m_MyInfo.m_eCondition].U0, m_rcIcon[m_MyInfo.m_eJob][m_MyInfo.m_eCondition].V0,
										m_rcIcon[m_MyInfo.m_eJob][m_MyInfo.m_eCondition].U1, m_rcIcon[m_MyInfo.m_eJob][m_MyInfo.m_eCondition].V1,
										0Xffffffff );
	// Button
	m_btmSetState.Render();
	
	pDrawPort->FlushRenderingQueue();

	// Name 
	pDrawPort->PutTextEx( m_MyInfo.m_strName, nX + MYINFO_NAME_X,
										nY, COLOR_MYNAME );
}


//------------------------------------------------------------------------------
// CUIMessenger::RenderGroup
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RenderGroup( int nX, int nY )
{
	// 출력 시킬 위치 조정 Offset~
	nY = GROUP_Y;

	ResizeScrollBar(); //Group의 정보에 의해 스크롤 바 크기 조절
		
	int nBoardHeight = m_nBoardHeight; // 출력 가능 영역
	int nPos		 = m_sbMemberList.GetScrollPos(); // 스크롤 바 Pos

	int nLineY = -m_sbMemberList.GetScrollPos();
	if( m_bFriendTab )
	{
		for(int i=0; i<m_vecGroup.size(); ++i)
			m_vecGroup[i]->Render( nLineY, nBoardHeight );
	}
	else
		m_mgBlock.Render(nLineY, nBoardHeight);
}


//------------------------------------------------------------------------------
// CUIMessenger::Reset
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::Reset()
{
	m_nBoardHeight = 195;
	m_nSelectGroup = 0;


	for(int i=0; i<m_vecGroup.size(); ++i)
		delete m_vecGroup[i];

	m_vecGroup.clear();
	m_tpGroupList.ResetAllStrings();

	m_mgBlock.Clear();
	
	
	s_strReqCharName.Clear();

	memset( g_bUseChat, 0, sizeof(bool) * MAX_TALK );
	memset( g_nMakeCharIndex, -1, sizeof(int) * MAX_TALK );
	memset( g_nChatIndex, 0, sizeof(int) * MAX_TALK );

	m_nTalkCount = 0;


	m_bFriendTab =true;

	m_bSelect =false;
	m_bDragging =false;
	m_bDrop = false;

	m_nFriendScrollPos =0;
	m_nBlockScrollPos =0;

	m_nActiveMenu =-1;
}


//------------------------------------------------------------------------------
// CUIMessenger::ToggleVisible
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ToggleVisible()
{
	if( !IsUseLevel() ) return;

	if( IsVisible() )
	{
		g_iXPosInMessanger = GetPosX();
		g_iYPosInMessanger = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInMessanger, g_iYPosInMessanger );
	}

	BOOL	bVisible = !IsVisible();
	CUIManager::getSingleton()->RearrangeOrder( UI_MESSENGER, bVisible );

}


//------------------------------------------------------------------------------
// CUIMessenger::MouseMessage
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIMessenger::MouseMessage( MSG *pMsg )
{
	static BOOL	bTitleBarClick = FALSE;
	static int	nOldX, nOldY;
	
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelect && m_nX!=nX && m_nY!=nY)
				m_bDragging =true;

			if( m_bDragging)
			{
				m_nX =nX;
				m_nY =nY;
			}
		
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );
				return WMSG_SUCCESS;
			}

			if( m_tpSetMyState.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( m_tpSetMyState.IsFocused() )
				{
					m_tpSetMyState.SetFocus(false);
					m_tpSetMyState.Hide();
				}
			}

			//그룹 메뉴 처리
			if( m_tpGroup.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				//"그룹 삭제" 메뉴를 가르키면 그룹 리스트 표시
				if( m_tpGroup.GetCurSel() ==GROUP_DELETE )
				{
					int PosX =m_tpGroup.GetPosX() +m_tpGroup.GetWidth();
					int PosY =m_tpGroup.GetPosY() +m_tpGroup.GetLineHeight() -5;
					m_tpGroupList.SetPos( PosX, PosY );
					m_tpGroupList.Show();
				}
				//"그룹명 변경" 메뉴를 가르키면 그룹 리스트 표시
				else if( m_tpGroup.GetCurSel() ==GROUP_RENAME)
				{
					int PosX =m_tpGroup.GetPosX() +m_tpGroup.GetWidth();
					int PosY =m_tpGroup.GetPosY() +m_tpGroup.GetLineHeight()*2 -5;
					m_tpGroupList.SetPos( PosX, PosY );
					m_tpGroupList.Show();
				}
				else
					m_tpGroupList.Hide();

				m_nActiveMenu =1;
				return WMSG_SUCCESS;
			}
			
			//멤버 메뉴 처리
			if( m_tpMemberMenu.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				//"그룹이동" 메뉴를 가르키면 그룹 리스트 표시
				if( m_tpMemberMenu.GetCurSel() ==0 )
				{
					int PosX =m_tpMemberMenu.GetPosX() +m_tpMemberMenu.GetWidth();
					int PosY =m_tpMemberMenu.GetPosY() -5;
					m_tpGroupList.SetPos( PosX, PosY );
					m_tpGroupList.Show();
					m_nActiveMenu =0;
				}
				//다른 메뉴를 가르키면 그룹 리스트 숨김
				else
					m_tpGroupList.Hide();

				return WMSG_SUCCESS;
			}
						
			//그룹 리스트를 가르킬때...
			if( m_tpGroupList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{	
				m_tpGroupList.Hide();
				if( m_tpMemberMenu.IsFocused() )
				{
					m_tpMemberMenu.SetFocus(false);
					m_tpMemberMenu.Hide();
				}
				if( m_tpGroup.IsFocused() )
				{
					m_tpGroup.SetFocus(false);
					m_tpGroup.Hide();
				}
			}

			//그룹 메뉴 처리
			if( m_tpGroupMenu.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( m_tpGroupMenu.IsFocused() )
				{
					m_tpGroupMenu.SetFocus(false);
					m_tpGroupMenu.Hide();
				}
			}
		
			if( m_tpBlock.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( m_tpBlock.IsFocused() )
				{
					m_tpBlock.SetFocus(false);
					m_tpBlock.Hide();
				}
			}

			if( m_btmSetState.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnGroup.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnBlock.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnInsert.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_sbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;







		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_tpGroup.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			else if( m_tpMemberMenu.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			else if( m_tpGroupMenu.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			else if( m_tpGroupList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			else if( m_tpBlock.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->RearrangeOrder( UI_MESSENGER, TRUE );

				nOldX = nX;		nOldY = nY;
			
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}

				if( m_tpSetMyState.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
								
				//Friend Tab
				else if( IsInsideRect( nX, nY, m_rcFriendTab ) )
				{
					if( !m_bFriendTab )
					{
						m_bFriendTab =true;
						m_nBlockScrollPos =m_sbMemberList.GetScrollPos();
					
						ResizeScrollBar();
						m_sbMemberList.SetScrollPos(m_nFriendScrollPos);

						m_mgBlock.SetFocus(false);
					}
					return WMSG_SUCCESS;
				}
				//block Tab
				else if( IsInsideRect( nX, nY, m_rcBlockTab ) )
				{
					if( m_bFriendTab )
					{
						m_bFriendTab = false;
						m_nFriendScrollPos =m_sbMemberList.GetScrollPos();
					
						ResizeScrollBar();
						m_sbMemberList.SetScrollPos(m_nBlockScrollPos);

						for(int i=0; i<m_vecGroup.size(); ++i)
							m_vecGroup[i]->SetFocus(false);
					}
					return WMSG_SUCCESS;
				}
				else if( m_btmSetState.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnGroup.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnBlock.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnInsert.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_sbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_bFriendTab )
				{
					for(int i=0; i<m_vecGroup.size(); ++i)
					{
						if( m_vecGroup[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							//m_nSelectGroup = i;
				
							for(int j=0; j<m_vecGroup.size(); ++j)
								m_vecGroup[j]->SetFocus(false);
							m_vecGroup[i]->SetFocus(true);
							
							if( m_vecGroup[i]->IsSelect() && IsInsideRect( nX, nY, m_rcBoard ))
							{
								m_nX =nX;
								m_nY =nY;
								m_bSelect = true;
								m_miSelectMember =m_vecGroup[i]->GetSelectMember();
							}
							else
								m_miSelectMember.m_nGroupIndex =-2;

				return WMSG_SUCCESS;
			}
		}
					//m_nSelectGroup =-2;
					return WMSG_SUCCESS;
				}
				else
				{
					if( m_mgBlock.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						//m_nSelectGroup = BLOCK_GROUP_INDEX;

						m_mgBlock.SetFocus(true);
					
						if( m_mgBlock.IsSelect() && IsInsideRect( nX, nY, m_rcBoard ))
							m_miSelectMember =m_mgBlock.GetSelectMember();
						else
							m_miSelectMember.m_nGroupIndex =-2;
					}
					else
						m_mgBlock.SetFocus(false);

					return WMSG_SUCCESS;
				}
			}
			break;
		}

	case WM_RBUTTONDOWN:
		{
			m_tpSetMyState.Hide();
			m_tpMemberMenu.Hide();
			m_tpGroupMenu.Hide();
			m_tpGroup.Hide();
			m_tpBlock.Hide();

			if( IsInside( nX, nY) )
			{
				if( m_bFriendTab)
				{
					for(int i=0; i<m_vecGroup.size(); ++i)
					{
						if( m_vecGroup[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							//m_nSelectGroup = i;

							for(int j=0; j<m_vecGroup.size(); ++j)
								m_vecGroup[j]->SetFocus(false);
							m_vecGroup[i]->SetFocus(true);

							if( IsInsideRect( nX, nY, m_rcBoard ) )
							{
								int x = nX -GetAbsPosX() +3;
								int y = nY -GetAbsPosY();

								//그룹 선택
								if( m_vecGroup[i]->GetSelectLine() ==-1)
								{
									m_nSelectGroup =i;
									//마우스 포인터 위치로 위치 지정
									m_tpGroupMenu.SetPos(x, y);
									m_tpGroupMenu.Show();
								}
								//멤버 선택
								else
								{
									m_miSelectMember =m_vecGroup[i]->GetSelectMember();
									//마우스 포인터 위치로 위치 지정
									m_tpMemberMenu.SetPos(x, y);
									m_tpMemberMenu.Show();
								}
							}
							//return WMSG_SUCCESS;
						}
					}
				}
				else
				{
					if( m_mgBlock.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_mgBlock.SetFocus(true);
						if( m_mgBlock.IsSelect() && IsInsideRect( nX, nY, m_rcBoard ))
						{
							//m_nSelectGroup = BLOCK_GROUP_INDEX;
							m_miSelectMember =m_mgBlock.GetSelectMember();

							//마우스 포인터 위치로 위치 지정
							int x = nX -GetAbsPosX() +3;
							int y = nY -GetAbsPosY();
							m_tpBlock.SetPos(x, y);
							m_tpBlock.Show();
						}
						else
							m_miSelectMember.m_nGroupIndex =-2;
					}
					else
						m_mgBlock.SetFocus(false);

					//return WMSG_SUCCESS;
				}
			}
			break;
		}

	case WM_LBUTTONUP:
		{
			bool bDragging =m_bDragging;

			m_bDragging =false;
			m_bSelect =false;

			if( bDragging )
				m_bDrop =TRUE;

			//상태 메뉴 처리
			if( m_tpSetMyState.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// 내 상태 변경 
				SetMyState( (eCondition)m_tpSetMyState.GetCurSel() );

				m_tpSetMyState.Hide();
				return WMSG_SUCCESS;
			}
			else
				m_tpSetMyState.Hide();

			// 멤버 메뉴 처리
			if( m_tpMemberMenu.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				eMemberMenu nSelectLine =(eMemberMenu)m_tpMemberMenu.GetCurSel();
				if( nSelectLine !=MM_GROUP_CHANGE)
				{
					SetMemberMenu( nSelectLine, 0);

					m_tpMemberMenu.Hide();
				}

				return WMSG_SUCCESS;
			}
			else
				m_tpMemberMenu.Hide();

			//그룹 기능 처리
			if( m_tpGroup.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				//새그룹 추가할 때
				eGroup nSelectLine =(eGroup)m_tpGroup.GetCurSel();
				if( nSelectLine ==GROUP_INSERT)
				{
					SetGroup( nSelectLine, 0 );

					m_tpGroup.Hide();
				}
				
					return WMSG_SUCCESS;
			}
			
			if( m_tpGroupList.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				int nSelectGroup =m_tpGroupList.GetCurSel();

				if( m_nActiveMenu ==0 )
				{
					SetMemberMenu( MM_GROUP_CHANGE, nSelectGroup );

					m_tpMemberMenu.Hide();
				}
				else
				{
					eGroup nSelectLine =(eGroup)m_tpGroup.GetCurSel();
					SetGroup( nSelectLine , nSelectGroup);

					m_tpGroup.Hide();	
				}
				
				m_tpGroupList.Hide();

				return WMSG_SUCCESS;
			}
			else
			{
					m_tpGroup.Hide();
				m_tpGroupList.Hide();
			}


			// 그룹 메뉴 처리
			if( m_tpGroupMenu.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				eGroup nSelectLine =(eGroup)m_tpGroupMenu.GetCurSel();
				
				SetGroup( nSelectLine, m_nSelectGroup);

				m_tpGroupMenu.Hide();			
				return WMSG_SUCCESS;
			}
			else
				m_tpGroupMenu.Hide();


			//차단 메뉴 처리
			if( m_tpBlock.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				/*****
				if( m_tpBlock.GetCurSel() ==0)
				{
					MsgBoxCommand(MSGCMD_MESSENGER_DEL_REQ, true, m_miSelectMember.m_strName);
				}
				else if( m_tpBlock.GetCurSel() ==1)
				{
					UnBlock(m_miSelectMember.m_nCharIndex, m_miSelectMember.m_strName);
					_pNetwork->MgrUnBlock(m_miSelectMember.m_nCharIndex);
				
					_pNetwork->MgrSetCondition( m_MyInfo.m_nCharIndex, m_MyInfo.m_eCondition );
				}
				*****/

				if( m_tpBlock.GetCurSel() ==0)
				{
					CTString strMessage;
					strMessage.PrintF(_S(3014, "%s님을 차단해제하시겠습니까?"), m_miSelectMember.m_strName );
					Message( MSGCMD_MESSENGER_UNBLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );
				}
				else  if( m_tpBlock.GetCurSel() ==1)
					MsgBoxCommand(MSGCMD_MESSENGER_BLOCK_DEL_REQ, true, m_miSelectMember.m_strName);

				m_tpBlock.Hide();
				return WMSG_SUCCESS;
				}
			else
				m_tpBlock.Hide();


			if( IsInside( nX, nY ) )
			{
				bTitleBarClick = FALSE;
				
				if( m_btmSetState.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_tpSetMyState.Show();
					return WMSG_SUCCESS;
				}
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ToggleVisible();
					return WMSG_SUCCESS;
				}
				else if( m_btnGroup.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_tpGroup.Show();
					return WMSG_SUCCESS;
				}
				else if( m_btnBlock.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					Message( MSGCMD_MESSENGER_BLOCK_REQ, _S(3004, "캐릭터 차단"), _S(3007, "차단할 캐릭명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );
					return WMSG_SUCCESS;
				}
				else if( m_btnInsert.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					Message( MSGCMD_MESSENGER_ADD_REQ, _S(2993, "친구추가"), _S( 1620, "등록할 친구의 이름을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );

					return WMSG_SUCCESS;
				}
				else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					Message( MSGCMD_MESSENGER_DEL_REQ, _S(2999, "캐릭터 삭제"), _S(3040, "삭제할 캐릭터명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );
					return WMSG_SUCCESS;
				}
				else if( m_sbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				
				else if( m_bFriendTab )
				{
					for(int i=0; i<m_vecGroup.size(); ++i)
					{
						if( m_vecGroup[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							//멤버를 드래그해서 다른 그룹으로 가져왔을 때 -> 해당 그룹으로 이동
							if( m_bDrop )
							{
								ChangeGroup(i);
								m_bDrop =FALSE;
							}

							return WMSG_SUCCESS;
						}
					}
				}
				else
				{
					if( m_mgBlock.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						//ResizeScrollBar();
						return WMSG_SUCCESS;
					}
				}

				m_bDrop =false;
				
				return WMSG_SUCCESS;
			}
			else
				m_bDrop =bDragging;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			WMSG_RESULT wmsgResult;
			if( IsInside( nX, nY ) )
			{
				if( m_bFriendTab )
				{
					for(int i=0; i<m_vecGroup.size(); ++i)
					{
						if( ( wmsgResult = m_vecGroup[i]->MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
								if( ReadyOpenTalk( m_vecGroup[i]->GetSelectMember() ) )
								{
									// UITalk를 스스로 생성만
									OpenTalk( m_MyInfo.m_nCharIndex , m_vecGroup[i]->GetSelectMember() );
								}

//								if( ReadyOpenTalk( m_vecGroup[i]->GetSelectMember() ) )
//									_pNetwork->MgrFriendInvite(m_MyInfo.m_nCharIndex, -1, m_vecGroup[i]->GetSelectMember().m_nCharIndex);
							}
							return WMSG_SUCCESS;
						}
					}
				}
			}
				
			return WMSG_SUCCESS;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUIMessenger::MsgBoxCommand
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	static CTString strBlockMember;
	static CMemberInfo miDelMember;

	switch ( nCommandCode )
	{

	case MSGCMD_MESSENGER_ADD_REQ:
		{
			if( bOK )
			{
				CMemberInfo mi =FindBlockMember(strInput);
				if( mi.m_nCharIndex >=0 && mi.m_nGroupIndex<0)
				{
					Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S(2995, "차단된 캐릭입니다. 먼저 차단해제 후 친구를 추가하시기바랍니다." ), UMBS_OK );		
					
				}
				else
				{
					if (UIMGR()->checkName(strInput, 0) == FALSE)
						return;

					s_strReqCharName =strInput;
					// 친구 등록 요청
					RegistMemberReq( strInput );
				}
			}

		}		
		break;

	case MSGCMD_MESSENGER_DEL_REQ:
	case MSGCMD_MESSENGER_BLOCK_DEL_REQ:
		{
			if( bOK )
			{
				if (UIMGR()->checkName(strInput, 0) == FALSE)
					return;

				if ((miDelMember = FindMember(strInput)).m_nCharIndex != -1 ||
					(miDelMember = FindBlockMember(strInput)).m_nCharIndex != -1)
				{
					m_miSelectMember =miDelMember;

					CTString strMessage;
					if( miDelMember.m_nGroupIndex >=0 )
					{
						strMessage.PrintF(_S(3000,"%s님을 삭제하시겠습니까?"), miDelMember.m_strName);

						if (nCommandCode == MSGCMD_MESSENGER_DEL_REQ)
							Message( MSGCMD_MESSENGER_DEL, _S(2999, "캐릭터 삭제"), strMessage, UMBS_YESNO );
						else
							Message( MSGCMD_MESSENGER_BLOCK_DEL, _S(2999, "캐릭터 삭제"), strMessage, UMBS_YESNO );
					}
					else
					{
						strMessage.PrintF(_S(3014, "%s님을 차단해제하시겠습니까?"), miDelMember.m_strName);
						Message( MSGCMD_MESSENGER_UNBLOCK, _S(3013, "차단 해제"), strMessage, UMBS_YESNO );						
					}
				}
				else
				{
					// (한글 4~8자, 영문 2~16자)
					if( strInput.Length() < 2 || strInput.Length() > 16 )
						Message( MSGCMD_MESSENGER_ERROR, _S(3003, "친구 삭제 에러"), _S(3002, "삭제하고자 하는 친구 이름이 올바르지 않습니다." ), UMBS_OK );		
					else
						Message( MSGCMD_MESSENGER_ERROR, _S(3003, "친구 삭제 에러"), _S(781, "대상 캐릭터가 존재하지 않습니다."), UMBS_OK);
				}
			}
		}		
		break;

	case MSGCMD_MESSENGER_DEL :
		{
			if( bOK )
			{
				_pNetwork->MgrDeleteMember( m_MyInfo.m_nCharIndex, m_miSelectMember.m_nCharIndex, miDelMember.m_strName );

				m_miSelectMember =miDelMember;
				DeleteMember();
			}
		}
		break;
	case MSGCMD_MESSENGER_BLOCK_DEL:
		{
			if (bOK)
			{
				_pNetwork->MgrFriendDeleteBlock(miDelMember.m_nCharIndex);
				m_miSelectMember =miDelMember;
				DeleteMember();
			}
		}
		break;
	case MSGCMD_MESSENGER_ADD_WAIT:
		{
			_pNetwork->MgrRegistCancel(m_nTarIndex, s_strReqCharName);
		}
		break;
	case MSGCMD_MESSENGER_ADD_REP:
		{
			if( bOK )
			{
				RegistAllow();
				return;
				
			}
			_pNetwork->MgrRegistCancel(m_nTarIndex, s_strReqCharName);

		}
		break;

	case MSGCMD_MESSENGER_BLOCK_REQ:
		{
			if( bOK )
			{
				strBlockMember =strInput;
				if( strInput.Length() < 2 || strInput.Length() > 16  || strInput==m_MyInfo.m_strName)
				{
					Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3009, "차단하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
					return;
				}
				if( FindBlockMember(strInput).m_nCharIndex >=0 )
				{
					Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3011, "이미 차단된 캐릭입니다."), UMBS_OK);
					return;
				}

				CUIManager* pUIManager = CUIManager::getSingleton();

				for(int i=0; i<10; ++i)
				{
					if( g_bUseChat[i] &&
						pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->IsExistTarget(strInput) )
					{
						Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3012, "대화중인 상대는 차단할 수 없습니다."), UMBS_OK);
						return;
					}
				}
	
				if (UIMGR()->checkName(strInput, 0) == FALSE)
					return;

				CTString strMessage;
				strMessage.PrintF(_S(3005, "%s님을 차단하시겠습니까?"), strInput);
				Message( MSGCMD_MESSENGER_BLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );		
			}
		}
		break;

	case MSGCMD_MESSENGER_BLOCK:
		{
			if( bOK)
			{
				_pNetwork->MgrBlock( strBlockMember );	
			}
		}
		break;

	case MSGCMD_MESSENGER_UNBLOCK:
		{
			if( bOK)
			{
				_pNetwork->MgrUnBlock(m_miSelectMember.m_nCharIndex);
				
				_pNetwork->MgrSetCondition( m_MyInfo.m_nCharIndex, m_MyInfo.m_eCondition );
			}
		}
		break;

	case MSGCMD_MESSENGER_ERROR:
		{
			
		}
		break;

	case MSGCMD_MESSENGER_GROUP_ADD:
		{
			if( bOK )
			{
				if( FindGroup(strInput) >=0 )
					Message( MSGCMD_MESSENGER_ERROR, _S(3020, "그룹추가 에러"), _S(3021, "같은 이름을 가진 그룹이 존재하고 있습니다."), UMBS_OK);
				else if( UTIL_HELP()->IsSpecialChar(strInput.str_String) == true )
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S( 437, "문장에 금지된 단어가 포함되어 있습니다.")); 
					UIMGR()->CreateMessageBox( MsgBoxInfo );					
				}
				else
					_pNetwork->MgrGroupAdd(strInput);

			}
		}
		break;

	case MSGCMD_MESSENGER_GROUP_DEL:
		{
			if( bOK )
			{
				if (m_vecGroup.size() <= m_nSelectGroup)
					return;

				if( m_vecGroup[m_nSelectGroup]->m_vecMember.empty() )
				{
					_pNetwork->MgrGroupDel(m_vecGroup[m_nSelectGroup]->m_nIndex);
					DeleteGroup();
				}
				else
					Message( MSGCMD_MESSENGER_ERROR, _S(3025, "그룹 삭제 에러"), _S(3027, "그룹에 대화 상대가 있을때는 삭제가 불가능합니다.\n그룹을 삭제하려면 대화상대를 다른 그룹으로 옮기셔야 가능합니다."), UMBS_OK);
			}
		}
		break;

	case MSGCMD_MESSENGER_GROUP_RENAME:
		{
			if( bOK )
			{
				if (m_vecGroup.size() <= m_nSelectGroup)
					return;

				if( FindGroup(strInput) >=0 )
					Message( MSGCMD_MESSENGER_ERROR, _S(3030, "그룹명 변경 에러"), _S(3021, "같은 이름을 가진 그룹이 존재하고 있습니다."), UMBS_OK);
				else if( UTIL_HELP()->IsSpecialChar(strInput.str_String) == true )
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S( 437, "문장에 금지된 단어가 포함되어 있습니다.")); 
					UIMGR()->CreateMessageBox( MsgBoxInfo );					
				}
				else
					_pNetwork->MgrGroupRename(m_vecGroup[m_nSelectGroup]->m_nIndex, strInput);
			}
		}
		break;

	case MSGCMD_MESSENGER_GROUP_MOVE:
		{
			if( bOK )
			{
			}
		}
		break;
	}

}


//------------------------------------------------------------------------------
// CUIMessenger::SetMyInfo
// Explain:  초기 정보 셋팅 (거의 필요 없을 듯... 한데... )
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::SetMyInfo( int nCharIndex, CTString strName, eJob nJob, eCondition eState )
{
	if(nCharIndex != -1)	m_MyInfo.m_nCharIndex		= nCharIndex;
	m_MyInfo.m_strName		= strName;
	if(nJob != TOTAL_JOB)	m_MyInfo.m_eJob				= nJob;
	m_MyInfo.m_eCondition	= eState;
}


//------------------------------------------------------------------------------
// CUIMessenger::SetMyState
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::SetMyState( eCondition eState )
{
	if( m_MyInfo.m_eCondition == eState )
		return;
	
	m_MyInfo.m_eCondition = eState;

	//! 내상태를 변경한 후에는 서버에 알려 줘야함
	_pNetwork->MgrSetCondition( m_MyInfo.m_nCharIndex, m_MyInfo.m_eCondition );
}


//------------------------------------------------------------------------------
// CUIMessenger::RegistMemberReq
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RegistMemberReq( CTString strName )
{
	// (한글 4~8자, 영문 2~16자)
	if( strName.Length() < 2 || strName.Length() > 16 )
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S( 1937, "등록하고자 하는 친구 이름이 올바르지 않습니다." ), UMBS_OK );		
		return;
	}

	// Edited : CTString의 == 은 대소문자를 가리지 않아서 일본요청으로 수정
/*	if(g_iCountry == JAPAN)
	{
		if( strcmp(strName,  m_MyInfo.m_strName) == 0 )	
		{
			Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S( 1938, "자신 자신은 등록할 수 없습니다." ), UMBS_OK );	
			return;
		}
	}
	else
	{
		if( strName == m_MyInfo.m_strName )	
		{
			Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S( 1938, "자신 자신은 등록할 수 없습니다." ), UMBS_OK );	
			return;
		}
	}*/

	if( strcmp(strName,  m_MyInfo.m_strName) == 0 )	
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S( 1938, "자신 자신은 등록할 수 없습니다." ), UMBS_OK );	
		return;
	}

	//이미 친구로 등록된 상태
	if( (FindMember(strName).m_nCharIndex != -1) || (FindBlockMember(strName).m_nCharIndex != -1) )
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(2997, "친구 추가 에러"), _S(2998, "이미 등록되어 있는 캐릭입니다." ), UMBS_OK );	
		return;
	}

	_pNetwork->MgrRegistReq( m_MyInfo.m_nCharIndex, strName );
}


//------------------------------------------------------------------------------
// CUIMessenger::RegistAllow
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RegistAllow()
{
	_pNetwork->MgrRegistAllow( m_nTarIndex, s_strReqCharName );
}


//------------------------------------------------------------------------------
// CUIMessenger::RegistMemberReq
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::RegistMemberReq( int nCharIndex, CTString strName )
{
	m_nTarIndex = nCharIndex;
	s_strReqCharName = strName;

	CTString strMessage;
	strMessage.PrintF( _S( 1623, "%s님이 친구등록 요청을 하셨습니다. 수락 하시겠습니까?" ), strName );
	Message( MSGCMD_MESSENGER_ADD_REP, _S(2996, "친구등록 요청"), strMessage, UMBS_YESNO );
}


//------------------------------------------------------------------------------
// CUIMessenger::DeleteMember
// Explain:  
// Date : 2005-05-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::DeleteMember()
{
	/********
	// Date : 2005-06-08(오후 1:57:15), By Lee Ki-hwan : 아무런 선택도 안된 상태에서 삭제시 
	if ( m_miTarget.m_nCharIndex == -1 ) return; 

	CTString strMessage;
	strMessage.PrintF( _S( 1624, "정말로 %s님을 대화상대에서 삭제하시겠습니까?" ), m_miTarget.m_strName );
	Message( MSGCMD_MESSENGER_DEL, _s("친구 삭제"), strMessage, UMBS_OKCANCEL );
	*********/

	CTString strName =m_miSelectMember.m_strName;
	
	int nGroup =FindGroup(m_miSelectMember.m_nGroupIndex);
	if( nGroup >=0)
		m_vecGroup[nGroup]->DeleteMember(strName);

	bool bDel =m_mgBlock.DeleteMember(strName);
	if( bDel )
		MsgBoxCommand(MSGCMD_MESSENGER_UNBLOCK, true, CTString());

	if( m_bFriendTab)
		m_miSelectMember =m_vecGroup[nGroup]->GetSelectMember();
	else
		m_miSelectMember =m_mgBlock.GetSelectMember();
	
	ResizeScrollBar();

	CTString strMessage;
	strMessage.PrintF(_S(3001, "%s님을 삭제했습니다."), strName);
	Message( MSGCMD_MESSENGER_ERROR, _S(2999, "캐릭터 삭제"), strMessage, UMBS_OK );		
}


//------------------------------------------------------------------------------
// CUIMessenger::DeleteMember
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::DeleteMember( int nCharIndex )
{
	// 친구 이름이 바뀌면 대화중이 창은 찾아서 닫는다.
	{
		int		max_group = m_vecGroup.size();
		int		i, j;
		std::string strFriend;

		for (i = 0; i < max_group; ++i)
		{
			for (j = 0; j < m_vecGroup[i]->m_vecMember.size(); ++j)
			{
				if (m_vecGroup[i]->m_vecMember[j].m_nCharIndex == nCharIndex)
				{
					strFriend = m_vecGroup[i]->m_vecMember[j].m_strName.str_String;

					i = max_group;
					break;
				}
			}
		}

		CUIManager* pUIManager = CUIManager::getSingleton();

		if (strFriend.empty() == false)
		{
			// 이미 대화중이다.
			for( i =0; i < MAX_TALK; ++i )
			{
				CUITalk* UITalk = pUIManager->GetTalk(i+UI_MESSENGER_TALK_START);			

				if (g_bUseChat[i] && g_nChatIndex[i] < 0 && 
					UITalk->IsExistTarget(strFriend.c_str()))
				{
					UITalk->Close();
					break;
				}
			}

		}
	}

	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		if ( m_vecGroup[i]->DeleteMember( nCharIndex ) )
			break;
	}
			
	bool bDel =m_mgBlock.DeleteMember(nCharIndex);
	if( bDel )
		MsgBoxCommand(MSGCMD_MESSENGER_UNBLOCK, true, CTString());

	ResizeScrollBar();
}


//------------------------------------------------------------------------------
// CUIMessenger::SetMemberCondition
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::SetMemberCondition( int nCharIndex, eCondition eState )
{
	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		bool bExist =m_vecGroup[i]->SetMemberCondition(nCharIndex, eState);
	}

	m_mgBlock.SetMemberCondition(nCharIndex, eState);

	if (nCharIndex == _pNetwork->MyCharacterInfo.index && eState == OFFLINE)
	{
		for( int i =0; i < MAX_TALK; ++i )
		{
			if( g_bUseChat[i] )
			{
				g_bUseChat[i] = false;
				g_nChatIndex[i] = 0;

				CUIManager::getSingleton()->GetTalk(i+UI_MESSENGER_TALK_START)->Close();
			}
		} 
	}
}

eCondition CUIMessenger::GetMemberCondition( int nCharIndex )
{
	eCondition eCon = OFFLINE;
	CMemberInfo kInfo;

	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		kInfo = m_vecGroup[i]->GetMemberInfo(nCharIndex);

		if (kInfo.m_nCharIndex == nCharIndex)
		{
			eCon = kInfo.m_eCondition;
			break;
		}
	}

	return eCon;
}

//------------------------------------------------------------------------------
// CUIMessenger::AddFriendList
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::AddFriendList( int nCharIndex, int nGroupIndex, CTString strName, eJob nJob, eCondition eState, bool bBlock )
{
	CMemberInfo miTemp;

	miTemp.SetInfo( nCharIndex, nGroupIndex, strName, nJob, eState );

	if( !bBlock )
	{
		int nGroup =FindGroup(nGroupIndex);

		if( nGroup >=0 )
			m_vecGroup[nGroup]->AddMember( miTemp );
	}
	else
	{
		CMemberInfo mi =FindMember(strName);
		if( mi.m_nCharIndex ==-1 )
			m_mgBlock.AddMember( CMemberInfo(nCharIndex, strName) );
		else
		{
			m_miSelectMember =mi;
			ChangeGroup(BLOCK_GROUP_INDEX);
		}
	}

	if( CUIManager::getSingleton()->DoesMessageBoxExist( MSGCMD_MESSENGER_ADD_WAIT ) )
	{
		CTString strMessage;
		strMessage.PrintF( _S( 1626, "%s님을 친구 목록에 추가였습니다." ), strName  );

		ErrorMessage( strMessage );		
	}
}

void CUIMessenger::AddFriendList( CMemberInfo miMember,  bool bBlock )
{
	if( !bBlock )
	{
		int nGroup =FindGroup( miMember.m_nGroupIndex );

		if( nGroup >=0 )
			m_vecGroup[nGroup]->AddMember( miMember );
	}
	else
	{
		CMemberInfo mi =FindMember(miMember.m_strName);
		if( mi.m_nCharIndex ==-1 )
			m_mgBlock.AddMember( miMember );
		else
		{
			m_miSelectMember =mi;
			ChangeGroup(BLOCK_GROUP_INDEX);
		}
	}

	if( CUIManager::getSingleton()->DoesMessageBoxExist( MSGCMD_MESSENGER_ADD_WAIT ) )
	{
		CTString strMessage;
		strMessage.PrintF( _S( 1626, "%s님을 친구 목록에 추가였습니다." ), miMember.m_strName  );

		ErrorMessage( strMessage );		
	}
}


//------------------------------------------------------------------------------
// CUIMessenger::ReSetFriendList
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ReSetFriendList()
{
	/*****
	m_mgFriend.ResetAllMember();
		
	std::vector<CMemberInfo>::iterator iterBegin = m_vecFriendList.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecFriendList.end();
	std::vector<CMemberInfo>::iterator iter;
	
	std::sort( m_vecFriendList.begin(), m_vecFriendList.end(), CompareObj );

	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		m_mgFriend.AddMember( *iter );
	}

	//m_mgFriend.ReloadMember();	
	*****/
}


//------------------------------------------------------------------------------
// CUIMessenger::ErrorMsg
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ErrorMsg( int nError )
{
	switch( nError )
	{
		case MSG_FRIEND_ERROR_OK:
			{
				DeleteMemberRep();
			}
			break;

		case MSG_FRIEND_ERROR_PACKET:    
			{
				ErrorMessage( _S( 1627, "알수 없는 데이터 입니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_NOT_EXIST: //존재하지 않는다.
			{
				ErrorMessage( _S( 1628, "해당 케릭터가 존재 하지 않거나 [오프라인] 상태입니다." ) );
			}
			break;
		case 	MSG_FRIEND_ERROR_FULLMEMBER: //친구 정원 초과.
			{
				ErrorMessage( _S( 1629, "더 이상 등록할 수 없습니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_ALREADY_EXIST: //이미 친구리스트에 존재.
			{
				ErrorMessage( _S( 1630, "이미 등록 되어 있습니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_WAIT_OTHER: //다른 사람으로부터 친구등록을 요청받은 상태..
			{
				ErrorMessage( _S( 1631, "이미 등록 요청을 받을 상태입니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_NOT_MATCH_INDEX: //서로간 친구 요청:승인 아이디가 일치하지 않는다.
			{
				ErrorMessage( _S( 1632, "아이디가 일치하지 않습니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_HELPER_SERVER: //헬퍼서버와의 접속오류.
			{
				ErrorMessage( _S( 1633, "알수 없는 데이터 입니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_GAMESERVER:
			{
				ErrorMessage( _S( 1634, "알수 없는 데이터 입니다." ) );
			}
			break;
		case MSG_FRIEND_ERROR_REGIST_REFUSE_PVP:
			{
				ErrorMessage( _S( 3070, "PVP중인 캐릭터는 친구등록할 수 없습니다.") );
			}
			break;
	}
}


//------------------------------------------------------------------------------
// CUIMessenger::ResizeScrollBar
// Explain:  Group의 상태에 따라 스크롤 바의 크기 조절
//			1. Extension
//			2. Member Count
// Date : 2005-05-22,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ResizeScrollBar()
{
	/********
	static bool OnLineExt = m_mgFriend.IsExtension();

	int nSize = 0;

	if( m_mgFriend.IsExtension() ) 
	{
		nSize = m_mgFriend.m_vecMember.size() + 1; 
	}
	else
	{
		nSize = 1;
	}
	
	// Pos을 이동시켜야 할까요?
	m_sbMemberList.SetCurItemCount( nSize );
	
	if( OnLineExt != m_mgFriend.IsExtension() )
	{
		m_sbMemberList.ChangeScrollPos( 0 );
		OnLineExt = m_mgFriend.IsExtension();
	}
	********/
	int nSize =0;

	if( m_bFriendTab)
	{
		for(int i=0; i<m_vecGroup.size(); ++i)
		{
			if( m_vecGroup[i]->IsExtension() )
				nSize +=m_vecGroup[i]->m_vecMember.size();
			
			++nSize;
		}
	}
	else
	{
		if( m_mgBlock.IsExtension() )
			nSize +=m_mgBlock.m_vecMember.size();

		++nSize;
	}

	m_sbMemberList.SetCurItemCount( nSize );
	m_sbMemberList.SetScrollRange( nSize );

	int nScrollPos = m_sbMemberList.GetScrollPos();
	int nItemPerPage =m_sbMemberList.GetItemsPerPage();

	if( nScrollPos > nSize-nItemPerPage)
	{
		nScrollPos =nSize-nItemPerPage;
		if(nScrollPos <0)
			nScrollPos =0;
		m_sbMemberList.SetScrollPos(nScrollPos);
	}
}


//------------------------------------------------------------------------------
// CUIMessenger::DeleteMemberRep
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::DeleteMemberRep()
{
	CTString strMessage;
	
	if( CUIManager::getSingleton()->DoesMessageBoxExist( MSGCMD_MESSENGER_ADD_WAIT ) ) // 추가 요청중이라면 
	{
		strMessage.PrintF( _S( 1635, "친구가 새로 추가되었습니다." ) );
	}
	else
	{	
		strMessage.PrintF( _S( 1636, "친구가 삭제 되었습니다." ) );
	}
}


//------------------------------------------------------------------------------
// CUIMessenger::Message
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) == TRUE )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	//MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_MESSENGER, nCommandCode );   ==>
	MsgBoxInfo.SetMsgBoxInfo( strTitle, dwStyle, UI_MESSENGER, nCommandCode ); 
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorMessage
// Explain: 에러 메세지 출력 시 사용하는 메세지 박스
// Date : 2005-03-19(오후 12:28:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::ErrorMessage( CTString strErrorMessage )
{
	CloseAllMessageBox();
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_MESSENGER, MSGCMD_MESSENGER_ERROR );
	MsgBoxInfo.AddString( strErrorMessage );	
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );		
}


void CUIMessenger::CloseAllMessageBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	for( int i = MSGCMD_MESSENGER_START; i <= MSGCMD_MESSENGER_END; i++ )
		pUIManager->CloseMessageBox( i );
}


//------------------------------------------------------------------------------
// CUIMessenger::ReadyOpenTalk
// Explain:  
// Date : , Author:
//------------------------------------------------------------------------------
bool CUIMessenger::ReadyOpenTalk( CMemberInfo miMemberInfo, bool bFocus )
{
	if ( miMemberInfo.m_nCharIndex == -1 )
		return false;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 현재 대화상대와 대화중인지
	for( int i =0; i < MAX_TALK; ++i )
	{
		//if( nUseTalbe[i] != -1 )
		if( g_bUseChat[i] )
		{
			CUITalk* UITalk = pUIManager->GetTalk(i+UI_MESSENGER_TALK_START);
			if( UITalk->IsExistTarget(miMemberInfo.m_strName) )
			{
				if( miMemberInfo.m_eCondition == OFFLINE )
				{
					TalkErrorMessage( _S( 1637, "대화상대가 [오프라인] 상태입니다." ) );
					return false;
				}
				else
				{
					if ( bFocus )
					{	// 현재 대화 중인 ~
						pUIManager->RearrangeOrder( i + UI_MESSENGER_TALK_START, TRUE );
					}
					// 1:1 대화중이면 새로운 창을 만들지 않음...
					if( UITalk->m_vecTarget.size() ==2)
					{
						//Message가 떠서 불편하다 (개인적 생각) [12/8/2006 KwonYongDae]
						//Message( MSGCMD_MESSENGER_ERROR, _S(3033, "대화하기 에러"), _S(3038, "이미 대화중인 상대입니다."), UMBS_OK);
						return false;
					}
				}
			}
		}
	}

	if( miMemberInfo.m_eCondition == OFFLINE )
	{	
		//!오프라인 대화상대와는 대화를 할 수 없습니다.
		ErrorMessage( _S( 1638,  "[오프라인] 상태인 대상과는 대화를 할 수 없습니다."  ) );
		return false;
	}


	// 남은게 없다면 메세지 뿌려주고 ㅡㅡ;~
	if( m_nTalkCount >= MAX_TALK )
	{
		//!더이상 대화창을 열 수 없습니다.
		ErrorMessage( _S( 1639,  "더 이상 대화창을 열 수 없습니다."  ) );
		return false;
	} 
	else
		return true;
}


//------------------------------------------------------------------------------
// CUIMessenger::CloseTalk
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::CloseTalk( int nIndex )
{
	g_bUseChat[nIndex-UI_MESSENGER_TALK_START] = false;
	g_nMakeCharIndex[nIndex-UI_MESSENGER_TALK_START] = -1;
	g_nChatIndex[nIndex-UI_MESSENGER_TALK_START] = 0;

	m_nTalkCount--;
}

//------------------------------------------------------------------------------
// CUIMessenger::RevOneVsOneTalk
// Explain:  
// Date : 2006-12-11,Author: KwonYongDae
//------------------------------------------------------------------------------
//void CUIMessenger::RevTalk( int nSendIndex, CTString strSendName, CTString strRecvName, CTString strMessage )
void CUIMessenger::RevOneVsOneTalk( int nSenderIndex, int nResiverIndex, CTString strSendName, CTString strChat, int nColIndex )
{
	bool bExistTarget =false;
	int  i = 0;
	CMemberInfo Sender, Resiver;

	// 보낸사람의 정보를 찾는다
	for( i=0; i<m_vecGroup.size(); ++i)
	{
		Sender	= m_vecGroup[i]->GetMemberInfo( nSenderIndex );
		if( Sender.m_nCharIndex != -1 )
		{
			break;
		}
	}

	// 보내는사람이 나 자신이라면
	if( Sender.m_nCharIndex == -1 && GetMyInfo().m_nCharIndex == nSenderIndex )	
		Sender = GetMyInfo();
	
	// 받는사람의 정보를 찾는다.
	for( i=0; i<m_vecGroup.size(); ++i)
	{
		Resiver	= m_vecGroup[i]->GetMemberInfo( nResiverIndex );

		if( Resiver.m_nCharIndex != -1	)
		{
			break;
		}
	}
	// 받는사람이 나 자신이라면
	if( Resiver.m_nCharIndex == -1 && GetMyInfo().m_nCharIndex == nResiverIndex )	
		Resiver = GetMyInfo();

	CUIManager* pUIManager = CUIManager::getSingleton();

	for( i =0; i < MAX_TALK; ++i )
	{
		CUITalk* UITalk = pUIManager->GetTalk(i+UI_MESSENGER_TALK_START);
		if( g_bUseChat[i] && 
			g_nChatIndex[i] < 0 /*UITalk->GetClientIndex()*/
			&& UITalk->IsExistTarget( Resiver.m_strName )
			&& UITalk->IsExistTarget( Sender.m_strName ) )// 사용중인창, 연결이 되지 않은창 보낸사람이 있는 창
		{
			UITalk->AddTalkListString( strSendName, strChat, false, ChatColor[nColIndex] );
			bExistTarget =true;
			break;
		}
	}

	//대화중인 상대가 아닌 사람에게서 메시지가 오면 새창을 띄움...
	if( !bExistTarget )
	{			
		//새로 창을 띄워야 합니다
		if( ReadyOpenTalk( Sender ) )
		{
			int nUIIndex = OpenTalk( m_MyInfo.m_nCharIndex , Sender );
			pUIManager->GetTalk(nUIIndex)->AddTalkListString( strSendName, strChat, false, ChatColor[nColIndex]);
		}
	}	
}

//------------------------------------------------------------------------------
// CUIMessenger::RevTalk
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
//void CUIMessenger::RevTalk( int nSendIndex, CTString strSendName, CTString strRecvName, CTString strMessage )
void CUIMessenger::RevTalk( int nMakeCharIndex, int nChatIndex, CTString strSendName, CTString strChat, int nColIndex )
{
	bool bExistTarget =false;
	CUIManager* pUIManager = CUIManager::getSingleton();

	for( int i =0; i < MAX_TALK; ++i )
	{
		CUITalk* UITalk = pUIManager->GetTalk(i+UI_MESSENGER_TALK_START);
		if( g_bUseChat[i] && g_nChatIndex[i]==nChatIndex)// 현재 대화중인 상대가 말을 걸어 왔네여~
		{
			
			UITalk->AddTalkListString( strSendName, strChat, false, ChatColor[nColIndex] );
			bExistTarget =true;
			break;
		}
	}
}

//------------------------------------------------------------------------------
// CUIMessenger::TalkErrorMessage
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMessenger::TalkErrorMessage( CTString strMessage )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int iUI;
	for( iUI = UI_MESSENGER_TALK_START; iUI < UI_MESSENGER_TALK_END; iUI++ )
	{
		if( pUIManager->GetTalk(iUI)->IsFocused() )
		{
			pUIManager->GetTalk(iUI)->AddErrorTalk( strMessage );
			break;
		}
	}

	if( iUI == UI_MESSENGER_TALK_END )
	{
		ErrorMessage ( strMessage );
	}
}

bool CUIMessenger::IsUseLevel()
{
	if ( _pNetwork->m_ubGMLevel >= MESSENGER_LEVEL )
	{
		return true;
	}

	return false;
}

void CUIMessenger::RenderTab(int nX, int nY)
{
	int nY_Friend =nY + Tab_Y;
	int nY_Block =nY + Tab_Y;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( m_bFriendTab )
	{
		//RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY_Friend -3, m_nWidth/2, 20, m_rt3BackFTab0 );
		//RenderRectUV3( UNIT_SIZE_GUIDLINE, nX +m_nWidth/2, nY_Block -3, m_nWidth/2, 20, m_rt3BackBTab1 ); 
		pDrawPort->AddTexture( nX +m_nWidth/2 , nY_Friend-3, nX +m_nWidth -8, nY_Friend-3 +20,
										m_rtTab.U0, m_rtTab.V0,	m_rtTab.U1, m_rtTab.V1,
										0Xffffffff );

		nY_Block +=2;		
	}
	else
	{
		//RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY_Friend -3, m_nWidth/2, 20, m_rt3BackFTab1 );
		//RenderRectUV3( UNIT_SIZE_GUIDLINE, nX +m_nWidth/2, nY_Block -3, m_nWidth/2, 20, m_rt3BackBTab0 ); 
		pDrawPort->AddTexture( nX +8, nY_Friend-3, nX +8 +m_nWidth/2 -8, nY_Friend-3 +20,
										m_rtTab.U0, m_rtTab.V0,	m_rtTab.U1, m_rtTab.V1,
										0Xffffffff );

		nY_Friend +=2;
	}

	/*
	pDrawPort->AddTexture( nX +8, nY + Tab_Y-3 +20, nX +m_nWidth -8, nY + Tab_Y-3 +20 +m_nBoardHeight + 6,
										m_rtTab.U0, m_rtTab.V0,	m_rtTab.U1, m_rtTab.V1,
										0Xffffffff );
	*/
	
	// [091110: selo] 텍스트 위치 잡기
//	extern INDEX g_iCountry;
	int nBlock_ExtraX = 0;
//	if( USA_SPAIN == g_iCountry )
//		nBlock_ExtraX = -20;
	
	pDrawPort->PutTextEx( _S(2984, "등록된 친구들"), nX + Tab_MEMBER_X,
										nY_Friend, m_bFriendTab ? 0xE1B300FF :  0xA2A2A2FF);

	pDrawPort->PutTextEx( _S(2985, "차단된 캐릭터"), nX + Tab_BLOCK_X + nBlock_ExtraX,
										nY_Block, !m_bFriendTab ? 0xE1B300FF :  0xA2A2A2FF );

}

void CUIMessenger::SetGroup(enum eGroup eState, int nSubMenu)
{
	switch(eState)
	{
	case GROUP_INSERT:
		Message( MSGCMD_MESSENGER_GROUP_ADD, _S(3018, "새그룹 추가"), _S(3019, "새로운 그룹명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );
		break;
	case GROUP_DELETE:
		{
			m_nSelectGroup =nSubMenu;
			if( m_nSelectGroup !=0 )
			{
				if (m_vecGroup.size() <= nSubMenu)
				{
					return;
				}

				CTString strMessage;
				strMessage.PrintF( _S(3024, "%s 그룹을 삭제하시겠습니까?" ), m_vecGroup[nSubMenu]->m_strGroupName );
				Message( MSGCMD_MESSENGER_GROUP_DEL, _S(3023, "그룹 삭제"), strMessage, UMBS_OKCANCEL );
			}
			else
				Message( MSGCMD_MESSENGER_ERROR, _S(3025, "그룹삭제 에러"), _S(3026, "기본 그룹은 삭제할 수 없습니다."), UMBS_OK);
			break;
		}
	case GROUP_RENAME:
		m_nSelectGroup =nSubMenu;
		if( m_nSelectGroup !=0 )
		{
			if (m_vecGroup.size() <= nSubMenu)
			{
				return;
			}

			Message( MSGCMD_MESSENGER_GROUP_RENAME, _S(3028, "그룹명 변경"), _S(3029, "변경할 그룹명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );
		}
		else
		{
			Message( MSGCMD_MESSENGER_ERROR, _S(3030, "그룹명 변경 에러"), _S(3031, "기본 그룹은 이름을 변경할 수 없습니다."), UMBS_OK);
		}
			
		break;
	}
}

void CUIMessenger::RenderSelectMember(int nX, int nY)
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );	

	pDrawPort->PutTextEx( m_miSelectMember.m_strName, nX +(GROUP_MEMBER_NAME_OFFSET_X -GROUP_MEMBER_ICON_OFFSET_X), nY, 0xf2f2f2ff);


	pDrawPort->AddTexture( nX, nY,	nX+ ICON_WIDTH, nY +ICON_HEIGHT,
										m_rcIcon[m_miSelectMember.m_eJob][m_miSelectMember.m_eCondition].U0, m_rcIcon[m_miSelectMember.m_eJob][m_miSelectMember.m_eCondition].V0,
										m_rcIcon[m_miSelectMember.m_eJob][m_miSelectMember.m_eCondition].U1, m_rcIcon[m_miSelectMember.m_eJob][m_miSelectMember.m_eCondition].V1,
										0Xffffffff );

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}

void CUIMessenger::AddGroup(int nGroupIndex, CTString strName)
{
	if( m_vecGroup.size() ==10)
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(3020, "그룹추가 에러"), _S(3022, "더이상 그룹을 생성할 수 없습니다."), UMBS_OK);		
		return;
	}

	if( nGroupIndex ==-1)
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(3020, "그룹추가 에러"), _S(3, "잘못된 문자가 포함되어있습니다."), UMBS_OK);		
		return;
	}

	if( nGroupIndex ==0)
		strName =_S(2986, "라스트카오스친구");

	CUIGroup* mgTemp = new CUIGroup;
		
	mgTemp->Create( this, GROUP_X, GROUP_Y, GROUP_WIDTH, 15 );
	mgTemp->SetGroupName( strName); 
	mgTemp->m_btnExtension.Create( (CUIWindow*)mgTemp, CTString( "" ), 0, 0, 14, 14 );
	mgTemp->m_btnExtension.SetUV( UBS_IDLE, 230, 117, 244, 131, m_fTexWidth, m_fTexHeight );
	mgTemp->m_btnExtension.SetUV( UBS_CLICK, 230, 132, 244, 146, m_fTexWidth, m_fTexHeight );
	mgTemp->m_btnExtension.CopyUV( UBS_IDLE, UBS_ON );
	mgTemp->m_btnExtension.CopyUV( UBS_IDLE, UBS_DISABLE ); 
	
	mgTemp->m_btnReduction.Create((CUIWindow*)mgTemp, CTString( "" ), 0, 0, 14, 14 );
	mgTemp->m_btnReduction.SetUV( UBS_IDLE, 230, 33, 244, 47, m_fTexWidth, m_fTexHeight );
	mgTemp->m_btnReduction.SetUV( UBS_CLICK, 230, 48, 244, 62, m_fTexWidth, m_fTexHeight );
	mgTemp->m_btnReduction.CopyUV( UBS_IDLE, UBS_ON );
	mgTemp->m_btnReduction.CopyUV( UBS_IDLE, UBS_DISABLE ); 
	
	mgTemp->m_rtSelectBar.SetUV(187, 46, 204, 61, m_fTexWidth, m_fTexHeight);
	mgTemp->SetIndex( nGroupIndex );
	
	m_vecGroup.push_back(mgTemp);

	m_tpGroupList.AddMenuList(strName);

	ResizeScrollBar();
}

void CUIMessenger::DeleteGroup()
{
	if (m_vecGroup.size() <= m_nSelectGroup)
		return;

	delete m_vecGroup[m_nSelectGroup];
	m_vecGroup.erase( m_vecGroup.begin() +m_nSelectGroup);
	
	m_tpGroupList.DeleteMenuList(0, m_nSelectGroup);

	ResizeScrollBar();
}

void CUIMessenger::SetMemberMenu(enum eMemberMenu eMenu, int nSubMenu)
{
	switch(eMenu)
	{
	case MM_GROUP_CHANGE:
		{
			int nGroupIndex =FindGroup(m_miSelectMember.m_nGroupIndex);
			if( nSubMenu!=nGroupIndex )
				ChangeGroup(nSubMenu);
		}
		break;
	case MM_CHAT:
		// 1:1 Chatting [12/29/2006 KwonYongDae]
		if( ReadyOpenTalk( m_miSelectMember ) )
		{
			OpenTalk( m_MyInfo.m_nCharIndex , m_miSelectMember );
		}
//			_pNetwork->MgrFriendInvite(m_MyInfo.m_nCharIndex, -1, m_miSelectMember.m_nCharIndex);

		break;
	case MM_DELETE:
		MsgBoxCommand( MSGCMD_MESSENGER_DEL_REQ, true, m_miSelectMember.m_strName);
		break;
	case MM_BLOCK:
		MsgBoxCommand(MSGCMD_MESSENGER_BLOCK_REQ, true, m_miSelectMember.m_strName );
		break;
	case MM_TELEPORT:
		_pNetwork->SendPremiumCharJumpReq(m_miSelectMember.m_strName);
		break;
	}
}

void CUIMessenger::ChangeGroup(int nGroup)
{
	int nPrevGroup =FindGroup(m_miSelectMember.m_nGroupIndex);

	if( nPrevGroup == nGroup )
		return;

	if (nPrevGroup < 0 || nPrevGroup >= m_vecGroup.size())
		return;

	// 차단 시 nGroup 이 -1 로 들어온다.
	if (nGroup >= 0 && m_vecGroup.size() <= nGroup)
		return;

	m_vecGroup[nPrevGroup]->DeleteMember(m_miSelectMember.m_strName);
	
	if( nGroup ==BLOCK_GROUP_INDEX)
	{
		//m_miSelectMember.m_nGroupIndex =m_mgBlock.m_nIndex;
		m_mgBlock.AddMember(m_miSelectMember);
	}
	else
	{
		_pNetwork->MgrGroupMove(m_vecGroup[nGroup]->m_nIndex, m_miSelectMember.m_nCharIndex);

		m_miSelectMember.m_nGroupIndex =m_vecGroup[nGroup]->m_nIndex;
		m_vecGroup[nGroup]->AddMember(m_miSelectMember);
	}
			
	m_miSelectMember =m_vecGroup[nPrevGroup]->GetSelectMember();
	
	ResizeScrollBar();
}


//------------------------------------------------------------------------------
// CUIMessenger::FindGroup
// Explain: 그룹 백터에서 그룹의 위치를 찾아 반환
// Date : 2006-5-29, Author: Lee Su-won
//------------------------------------------------------------------------------
int CUIMessenger::FindGroup(int nGroupIndex)
{
	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		if( m_vecGroup[i]->m_nIndex == nGroupIndex )
			return i;
	}

	return -1;
}

int CUIMessenger::FindGroup(CTString strName)
{
	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		if( m_vecGroup[i]->m_strGroupName == strName )
			return i;
	}

	return -1;
}

CMemberInfo CUIMessenger::FindMember(CTString strName)
{
	for(int i=0; i<m_vecGroup.size(); ++i)
	{
		for(int j=0; j<m_vecGroup[i]->m_vecMember.size(); ++j)
		{
			// Edited : CTString의 == 은 대소문자를 가리지 않아서 일본요청으로 수정
		/*	if(g_iCountry == JAPAN)
			{
				if( strcmp(m_vecGroup[i]->m_vecMember[j].m_strName, strName) == 0 )
					return m_vecGroup[i]->m_vecMember[j];
			}
			else
			{
				if( m_vecGroup[i]->m_vecMember[j].m_strName == strName)
					return m_vecGroup[i]->m_vecMember[j];
			}*/
			//[ttos_2010_4_26]: 캐릭명은 대소문자 구분해야한다.
			if( strcmp(m_vecGroup[i]->m_vecMember[j].m_strName, strName) == 0 )
					return m_vecGroup[i]->m_vecMember[j];

		}
	}

	return CMemberInfo();
}

CMemberInfo CUIMessenger::FindBlockMember(CTString strName)
{
	for(int i=0; i<m_mgBlock.m_vecMember.size(); ++i)
	{
		if( m_mgBlock.m_vecMember[i].m_strName == strName )
			return m_mgBlock.m_vecMember[i];
	}

	return CMemberInfo();
}


void CUIMessenger::RenameGroup(int nGroupIndex, CTString strNewName)
{
	if( nGroupIndex ==-1 )
	{
		Message( MSGCMD_MESSENGER_ERROR, _S(3030, "그룹명 변경 에러"), _S(3, "잘못된 문자가 포함되어있습니다."), UMBS_OK);
		return;
	}

	m_vecGroup[m_nSelectGroup]->SetGroupName( strNewName );	

	m_tpGroupList.SetString(0, m_nSelectGroup, strNewName);
}


void CUIMessenger::SetChat(int nMakeCharIndex, int nChatIndex, CTString strName)
{
	bool bNewChat =true;
	int i = 0 ;

	// Index  [12/13/2006 KwonYongDae]
	for( i =0; i < MAX_TALK; ++i )
	{
		if( g_nChatIndex[i] == nChatIndex )
		{
			bNewChat =false;
			break;
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( bNewChat )		// 새창으로 생성하기 전에 1:1 대화에서 상대가 초대된 상대가 있는 쪽을 검색한다.
	{
		for( i =0; i < MAX_TALK; ++i )
		{
			// 드래그된 창과 같은 창을 찾아라
			if( g_nChatIndex[i] < 0 &&
				pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->IsExistTarget( strName ) ) 
			{
				bNewChat =false;
				break;
			}
		}
	}
	

	

	//새로운 창 생성
	if( bNewChat )
		OpenTalk(nMakeCharIndex, nChatIndex, strName);
	//기존 창에 대화 상대 초대
	else
	{
		if( g_nChatIndex[i] < 0 ) //처음 다른사람을 초대
		{
			g_nChatIndex[i] = nChatIndex;
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->SetIndex( nMakeCharIndex, nChatIndex);
			if( m_MyInfo.m_nCharIndex == nMakeCharIndex )	// 초대하는 사람일때
				_pNetwork->MgrFriendInvite(nMakeCharIndex, nChatIndex, pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->GetTargetIndex() );
		}
		pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->AddTalkTarget(strName);
	}


}

int CUIMessenger::OpenTalk(int nMakeCharIndex, int nChatIndex, CTString strName)
{
	if (m_MyInfo.m_eCondition == OFFLINE)
	{
		TalkErrorMessage( _S( 1646, "[오프라인] 상태에서는 대화를 할 수 없습니다." ) );
		return -1;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	for( int i =0; i < MAX_TALK; ++i )
	{
		if( !g_bUseChat[i] )
		{
			g_bUseChat[i] =true;
			g_nMakeCharIndex[i] =nMakeCharIndex;
			g_nChatIndex[i] =nChatIndex;

			++m_nTalkCount;

			//pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->AddTalkTarget(m_MyInfo.m_strName);
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->Open( strName );
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->SetIndex(nMakeCharIndex, nChatIndex);
	
			//_pNetwork->MgrFriendInvite( m_MyInfo.m_nCharIndex, -1, miMemberInfo.m_strName);
			return ( i+UI_MESSENGER_TALK_START );
		}
	} 
	
	return -1;
}

//------------------------------------------------------------------------------
// CUIMessenger::OpenTalk
// Explain: 서버 인덱스가 존재하지 않는 창을 만든다.
// Date : 2006-12-8,Author: KwonYongDae
//------------------------------------------------------------------------------
int CUIMessenger::OpenTalk(int nMakerIndex, const CMemberInfo targetInfo )
{
	if (m_MyInfo.m_eCondition == OFFLINE)
	{
		TalkErrorMessage( _S( 1646, "[오프라인] 상태에서는 대화를 할 수 없습니다." ) );
		return -1;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	for( int i =0; i < MAX_TALK; ++i )
	{
		if( !g_bUseChat[i] )
		{
			g_bUseChat[i] = true;
			g_nChatIndex[i] = -1*(i+1);

			++m_nTalkCount;

			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->Open( g_nChatIndex[i] , targetInfo );
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->SetIndex( nMakerIndex, g_nChatIndex[i] );
			return ( i+UI_MESSENGER_TALK_START );
		}
	} 
	return -1;
}

void CUIMessenger::AddChatMember(int nMakeCharIndex, int nChatIndex, CTString strName)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	for(int i=0; i<MAX_TALK; ++i)
	{
		if( g_nChatIndex[i] ==nChatIndex )
		{
			//if( m_MyInfo.m_strName != strName)
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->AddTalkTarget(strName);
			break;
		}
	}
}

void CUIMessenger::DeleteChatMember(int nMakeCharIndex, int nChatIndex, CTString strName)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	for(int i=0; i<MAX_TALK; ++i)
	{
		if( g_bUseChat[i] && g_nChatIndex[i] ==nChatIndex )
		{
			pUIManager->GetTalk(i+UI_MESSENGER_TALK_START)->DeleteTalkTarget(strName);
			break;
		}
	}
}

void CUIMessenger::UnBlock(BYTE cError, int nCharIndex, CTString strName)
{
	/*****
	m_bFriendTab = true;
	m_nBlockScrollPos =m_sbMemberList.GetScrollPos();
	m_sbMemberList.SetScrollPos(m_nFriendScrollPos);
	ResizeScrollBar();
	*****/

	//SetBlock(nCharIndex, strName, false);

	switch(cError)
	{
	case MSG_EX_MESSENGER_BLOCK_NOTCHAR:
		Message( MSGCMD_MESSENGER_ERROR, _S(3016, "차단해제 에러"), _S(11, "존재하지 않는 캐릭터입니다."), UMBS_OK);		
		return;
	case MSG_EX_MESSENGER_BLOCK_INVALIDNAME:
		Message( MSGCMD_MESSENGER_ERROR, _S(3016, "차단해제 에러"), _S(3017, "차단해제하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
		return;
	}

	CMemberInfo mi =FindBlockMember(strName);

	if( mi.m_nGroupIndex >=0)
		AddFriendList(mi, false);
	
	m_mgBlock.DeleteMember(nCharIndex);

	CTString strMessage;
	strMessage.PrintF( _S( 3015, "%s님이 차단해제되었습니다." ), strName );
	Message( MSGCMD_MESSENGER_ERROR, _S(3013, "차단 해제"), strMessage, UMBS_OK );
}

void CUIMessenger::Block(BYTE cError, int nCharIndex, CTString strName)
{
	switch(cError)
	{
	case MSG_EX_MESSENGER_BLOCK_NOTCHAR:
		Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(11, "존재하지 않는 캐릭터입니다."), UMBS_OK);		
		return;
	case MSG_EX_MESSENGER_BLOCK_INVALIDNAME:
		Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3009, "차단하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
		return;
	case MSG_EX_MESSENGER_NOMORE_BLOCK:
		Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3010, "더이상 차단할 수 없습니다."), UMBS_OK);		
		return;
	case MSG_EX_MESSENGER_ALREADY_BLOCK:
		Message( MSGCMD_MESSENGER_ERROR, _S(3008, "차단 에러"), _S(3011, "이미 차단된 캐릭입니다."), UMBS_OK);
		return;
	}

	/***
	m_bFriendTab = false;
	m_nFriendScrollPos =m_sbMemberList.GetScrollPos();
	m_sbMemberList.SetScrollPos(m_nBlockScrollPos);
	ResizeScrollBar();
	****/

	//SetBlock(nCharIndex, strName, true);
	m_miSelectMember =FindMember(strName);
	if( m_miSelectMember.m_nCharIndex >=0)
		ChangeGroup(BLOCK_GROUP_INDEX);
	else
		m_mgBlock.AddMember( CMemberInfo(nCharIndex, strName) );

	CTString strMessage;
	strMessage.PrintF(_S(3006, "%s님이 차단되었습니다."), strName);
	Message( MSGCMD_MESSENGER_ERROR, _S(3004, "캐릭터 차단"), strMessage, UMBS_OK );
}

void CUIMessenger::SetDefaultGroupName(CTString strName)
{ 
	m_vecGroup[0]->SetGroupName(strName); 

	m_tpGroupList.SetString(0, 0, strName);
};

void CUIMessenger::RenderMenuArrow(int nX, int nY, int nWidth, int nHeight, int nLine)
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int nYSpace =((_pUIFontTexMgr->GetLineHeight()+2) -nHeight)/2 +2;

	pDrawPort->AddTexture( nX, nY +nYSpace +(_pUIFontTexMgr->GetLineHeight()+2)*nLine,
										nX+nWidth, nY +nYSpace +(_pUIFontTexMgr->GetLineHeight()+2)*nLine+nHeight,
										m_rtArrow.U0, m_rtArrow.V0, m_rtArrow.U1, m_rtArrow.V1,
										0xFFFFFFFF );
}

void CUIMessenger::SetBlock( int nCharIndex, CTString strName, bool bBlock)
{
	/***
	bool bFriend =false;

	int i,j;
	for(i=0; i<m_vecGroup.size(); ++i)
	{	
		for(j=0; j<m_vecGroup[i]->m_vecMember.size(); ++j)
		{
			if( m_vecGroup[i]->m_vecMember[j].m_strName ==strName )
			{
				m_vecGroup[i]->SetBlock(j, bBlock);
			
				bFriend =true;
				break;
			}
		}
		if( bFriend )
			break;
	}

	if( bBlock )
	{
		if( bFriend )
			m_mgBlock.AddMember( m_vecGroup[i]->m_vecMember[j] );
		else
			m_mgBlock.AddMember( CMemberInfo(nCharIndex, strName) );
	}
	else
		m_mgBlock.DeleteMember( strName );
		***/
}

void CUIMessenger::UpdateMemberMenu()
{
	m_tpMemberMenu.ResetAllStrings();

	m_tpMemberMenu.AddMenuList( _S(2990, "그룹이동") +CTString("     ") );
	m_tpMemberMenu.AddMenuList( _S(2991, "대화하기") );
	m_tpMemberMenu.AddMenuList( _S(338, "삭제" ) );
	m_tpMemberMenu.AddMenuList( _S(2992, "차단" ) );

#ifdef PREMIUM_CHAR
	if (m_bPremiumChar == true)
	{
		CTString strMenu, strCount;
		int Count = 0;
		CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();

		if (pChar != NULL)
			Count = pChar->getTeleportCount();
		
		strMenu.PrintF( _S(6344, "텔레포트" ) );
		strCount.PrintF( _S(6343, "(%d/%d)"), Count, PREMIUM_CHAR_JUMP_COUNT_MAX );
		strMenu = strMenu + strCount;

		if (Count >= PREMIUM_CHAR_JUMP_COUNT_MAX)
			m_tpMemberMenu.AddMenuList( strMenu, 0xF20000FF);
		else
			m_tpMemberMenu.AddMenuList( strMenu, 0xD67FFFFF);
	}
#endif	//	PREMIUM_CHAR
}

void CUIMessenger::SetPremiumBenefit( bool bUse )
{
	m_bPremiumChar = bUse; 
	UpdateMemberMenu();
}
