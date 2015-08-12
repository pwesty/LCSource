#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <map>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Ska/Render.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Object/ActorMgr.h>

static int	_iMaxMsgStringChar = 0;

// Define text position
#define	GB_TITLE_TEXT_OFFSETX		25		// 타이틀 바 스트링 
#define	GB_TITLE_TEXT_OFFSETY		5

#define GB_TITLE_HEIGHT				26		// 타이틀 바 높이
#define START_BOTTOM_BUTTON_Y		(GB_HEIGHT - 29)	// 버튼의 시작 높이

#define	GB_DESC_CHAR_WIDTH			(168	 + 95)	// 설명 글 너비
#define DESC_LIST_BOX_HEIGHT		135		// 설명 글 높이

#define GB_REQ_NAS_EDIT_POS_X		80		// 나스 입력 에디스 박스 높이
#define GB_REQ_NAS_EDIT_POS_Y		166
#define GB_REQ_NAS_EDIT_WIDTH		60
#define GB_REQ_NAS_EDIT_HEIGHT		16
#define EDITBOX_OFFSETX				4


#define GB_REQ_TIME_EDIT_POS_X		200		// 시간 입력 에디스 박스 높이
#define GB_REQ_TIME_EDIT_POS_Y		166
#define GB_REQ_TIME_EDIT_WIDTH		60
#define GB_REQ_TIME_EDIT_HEIGHT		16
#define EDITBOX_OFFSETX				4


// 최대 최소 사용 나스
#define GB_MAX_NAS					100			// 단위	: 만 나스
#define GB_MIN_NAS					10
#define GB_COMMISSION				5			// 단위 : %

//브라질 나스 표시 수정
#define GB_MAX_NAS_EX				"1,000,000"			// 단위	: 나스
#define GB_MIN_NAS_EX				"100,000"

// 전투 시간 ( 분단위 )
#define GB_MIN_TIME					10
#define GB_MAX_TIME					300

#define SMALL_NUMBER_WIDTH			16
#define SMALL_NUMBER_HEIGHT			20
#define LARGE_NUMBER_WIDTH			21
#define LARGE_NUMBER_HEIGHT			27

#define LEFT_TIME_X					922
#define LEFT_TIME_Y					259

#define RIGHT_SPACE					12
#define BOX_WIDTH					118
#define BOX_HEIGHT					61

#define BLUE_BOX_Y					302
#define RED_BOX_Y					369

#define GUILD_NAME_Y				308
#define GUILD_NAME2_Y				371

#define BATTLE_ZONE_NAME_Y			LEFT_TIME_Y + SMALL_NUMBER_HEIGHT + 2		

#define BOX_Y						256
#define BASE_BOX_HEIHGT				42

#define RED_KILL_POINT_Y			393
#define BLUE_KILL_POINT_Y			329

#define GB_MIN_MEMBER				10
#define COLON_WIDTH					9
#define GB_LEFT_TIME_HOUR			30
#define GB_LEFT_TIME				( 30 * 60 )
#define GB_START_MINUTE				1
	
// Macro Func 좀 편해 보고자 만든 것들...
#define SYSERROR_MSG(X)	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( CTString (X), SYSMSG_ERROR );

extern BOOL g_bNasTrans; //브라질 나스 표시 수정
//------------------------------------------------------------------------------
// CUIGuildBattle::CloseAllMsgBox
// Explain:  
// Date : 2005-03-22(오전 11:29:42) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::CloseAllMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_ERROR);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_REQ);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_ACCEPT);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_REQ);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_ACCEPT);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_REQ_REP);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_MESSAGE);
}


//------------------------------------------------------------------------------
// CUIGuildBattle::SetNotice
// Explain:  
// Date : 2005-03-21(오후 9:53:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::SetNotice ( CTString strNoticeMessage, CTString strNoticeMessage2 )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	CloseAllMsgBox();

	pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
	m_bShowNotice = TRUE;
	
	m_strNoticeMessage = strNoticeMessage;
	m_strNoticeMessage2 = strNoticeMessage2;

	// 위쪽에 출력 될 공지
	int nWidth = ( m_strNoticeMessage.Length() ) * 
			 ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
		
	m_rcNotice1.Left = nCX - nWidth / 2;
	m_rcNotice1.Right = m_rcNotice1.Left + nWidth;


	// 아래쪽에 출력될 공지
	nWidth = ( strNoticeMessage2.Length() ) *
			 ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
		
	m_rcNotice2.Left = nCX - nWidth / 2;
	m_rcNotice2.Right = m_rcNotice2.Left + nWidth;

	// 시간 셋팅
	m_tmNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}



//------------------------------------------------------------------------------
// CUIGuildBattle::CUIGuildBattle()
// Explain:  
// Date : 2005-03-17(오전 11:24:53) Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildBattle::CUIGuildBattle()
{
//	Clear();
	m_ptdStateTexture = NULL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::~CUIGuildBattle()
// Explain:  
// Date : 2005-03-17(오전 11:24:51) Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildBattle::~CUIGuildBattle()
{
	if( m_ptdStateTexture )
	{
		_pTextureStock->Release( m_ptdStateTexture );
		m_ptdStateTexture = NULL;
	}
	
	Clear ();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::Clear
// Explain:  
// Date : 2005-03-17(오후 4:33:11) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Clear()
{
	// Controls	
	//m_ebGuildReqNas.ResetString();				// 길드 신청 금액 입력
	m_lbGBReqdDesc.ResetAllStrings();

	m_tmNoticeTime	= 0;	
	m_bShowNotice	= FALSE;
	m_strNoticeMessage.Clear();
	m_strNoticeMessage2.Clear();
		
	//상대 편 길드의 정보
	m_strTargetGuildName.Clear();
	m_nTargetCharIndex = -1;
	m_nTargetGuildIndex = -1;

	m_eGBState = GBS_NONE;
	m_bShowNotice = FALSE;

	m_strGuildName.Clear();
	m_nCharIndex = -1;
	m_nGuildIndex = -1;

	m_lLeftTime = GB_LEFT_TIME;
	
	m_nKillPoint = 0;
	m_nTargetKillPoint = 0;

	m_nBattleZone = -1;
	m_nPrize = 0;

	m_mapEG.clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::Create
// Explain:  
// Date : 2005-03-17(오전 11:24:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	Clear();
	m_strTitle = _S( 1097, "길드전투" ); 

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = GB_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// 길드 요청
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	
	// Nas Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );

	m_ebGuildReqNas.Create( this, GB_REQ_NAS_EDIT_POS_X+2, GB_REQ_NAS_EDIT_POS_Y, GB_REQ_NAS_EDIT_WIDTH-2, GB_REQ_NAS_EDIT_HEIGHT, 16 );
	m_ebGuildReqNas.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebGuildReqNas.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	m_ebGuildReqTime.Create( this, GB_REQ_TIME_EDIT_POS_X+2, GB_REQ_TIME_EDIT_POS_Y, GB_REQ_TIME_EDIT_WIDTH-2, GB_REQ_TIME_EDIT_HEIGHT, 16 );
	m_ebGuildReqTime.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebGuildReqTime.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );


	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 164, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 234, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box of guild description
	m_lbGBReqdDesc.Create( this, 8, 28, 283, DESC_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );
	m_lbGBReqdDesc.CreateScroll( TRUE, 0, 0, 9, DESC_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );

	// Up button
	m_lbGBReqdDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGBReqdDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGBReqdDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGBReqdDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGBReqdDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// Guild State In Battle
	m_ptdStateTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\GuildBattle.tex" ) );
	
	fTexWidth	= m_ptdStateTexture->GetPixWidth();
	fTexHeight	= m_ptdStateTexture->GetPixHeight();


	m_rcRedBox.SetRect( 0, RED_BOX_Y, 0, RED_BOX_Y + BOX_HEIGHT );		
	m_rcBlueBox.SetRect( 0, BLUE_BOX_Y, 0, BLUE_BOX_Y + BOX_HEIGHT );		
	m_rcBox.SetRect( 0, BOX_Y, 0, BOX_Y + BASE_BOX_HEIHGT );	

	m_rtRedBox.SetUV( 0, 52, 118, 113, fTexWidth, fTexHeight );				// UV of background		
	m_rtBlueBox.SetUV( 119, 52, 237, 113, fTexWidth, fTexHeight );			// UV of background		
	m_rtBox.SetUV( 8, 57, 108, 72, fTexWidth, fTexHeight );					// UV of background		
	
	for( int i = 0 ; i < 10; i++ )
	{
		m_rtSmallNumber[i].SetUV( i*SMALL_NUMBER_WIDTH, 0, 
								i*SMALL_NUMBER_WIDTH+SMALL_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );

		m_rtLargeNumber[i].SetUV( i*LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, 
								i*LARGE_NUMBER_WIDTH+LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT + LARGE_NUMBER_HEIGHT, fTexWidth, fTexHeight );					// UV of background		
	}



	m_rtColon.SetUV ( 164, 0, 164 + COLON_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );		
		
	m_rcNotice1.SetRect( 0, 100, 0, 119 );
	m_rcNotice2.SetRect ( 0, 120, 0, 139 );

	// Notice
	m_rtNoticeL.SetUV( 189, 0, 221, 19, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 243, 22, 250, 41, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 223, 0, 255, 19, fTexWidth, fTexHeight );
	
	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::Close
// Explain:  
// Date : 2005-03-21(오후 5:13:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Close()
{

	CloseAllMsgBox();
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, FALSE );

	Clear();
}

// Define notice delay time
#define	NOTICE_DELAY		8000			// milliseconds
#define	NOTICE_FADEOUT		7000
#define	NOTICE_FADETIME		( NOTICE_DELAY - NOTICE_FADEOUT )

//------------------------------------------------------------------------------
// CUIGuildBattle::Render
// Explain:  
// Date : 2005-03-17(오전 11:28:56) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Render()
{
	if ( m_eGBState == GBS_REQ )
	{
		RenderGBReq();
	}
	else if ( m_eGBState == GBS_IN_BATTLE )
	{
		RenderGBStatus();
	}

	if( m_bShowNotice == FALSE )
		return;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmNoticeTime;
	if( llCurDelay < CHATMSG_NOTICE_DELAY )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = 0x18ff00ff;
		if( llCurDelay > NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( NOTICE_DELAY - llCurDelay ) / (FLOAT)NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;
			
			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}

		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

		pDrawPort->InitTextureData( m_ptdStateTexture );

		// Add render regions
		// Background
		pDrawPort->AddTexture( m_rcNotice1.Left - 32, m_rcNotice1.Top,
												m_rcNotice1.Left, m_rcNotice1.Bottom,
												m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice1.Left, m_rcNotice1.Top,
												m_rcNotice1.Right, m_rcNotice1.Bottom,
												m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice1.Right, m_rcNotice1.Top,
												m_rcNotice1.Right + 32, m_rcNotice1.Bottom,
												m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
												colBackground );

		// bottom
		pDrawPort->AddTexture( m_rcNotice2.Left - 32, m_rcNotice2.Top,
												m_rcNotice2.Left, m_rcNotice2.Bottom,
												m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice2.Left, m_rcNotice2.Top,
												m_rcNotice2.Right, m_rcNotice2.Bottom,
												m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice2.Right, m_rcNotice2.Top,
												m_rcNotice2.Right + 32, m_rcNotice2.Bottom,
												m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
												colBackground );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		pDrawPort->PutTextEx( m_strNoticeMessage, m_rcNotice1.Left, m_rcNotice1.Top + 4, colText );
		pDrawPort->PutTextEx( m_strNoticeMessage2, m_rcNotice2.Left, m_rcNotice2.Top + 4, colText );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		m_bShowNotice = FALSE;

		// 종료 상태에서 공지가 띄워 졌다면 길드전은 종료 된다.
		if ( m_eGBState == GBS_END ) 
		{
			Close();
		}
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderGBReq 
// Explain: 길드 신청 화면
// Date : 2005-03-17(오후 4:29:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderGBReq()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;

	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + GB_TITLE_HEIGHT;


	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
		
	// Not Manager
	pDrawPort->AddTexture( m_nPosX, nY, nX, nY + DESC_LIST_BOX_HEIGHT+ 2,
											m_rtBackMiddle1.U0, m_rtBackMiddle1.V0,
											m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
											0xFFFFFFFF );
	
	nY += DESC_LIST_BOX_HEIGHT+ 2;

	// Middle 2
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );


	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	m_lbGBReqdDesc.Render();

	// Edit Box
	// Lower left

	nX	= m_nPosX + GB_REQ_NAS_EDIT_POS_X;
	nX2 = nX + GB_REQ_NAS_EDIT_WIDTH;

	nY	= m_nPosY + GB_REQ_NAS_EDIT_POS_Y;
	nY2 = nY + GB_REQ_NAS_EDIT_HEIGHT;

	pDrawPort->AddTexture( nX, nY, nX + EDITBOX_OFFSETX, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + EDITBOX_OFFSETX, nY, nX2 - EDITBOX_OFFSETX, nY2,
		m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - EDITBOX_OFFSETX, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );

	m_ebGuildReqNas.Render();

	nX	= m_nPosX + GB_REQ_TIME_EDIT_POS_X;
	nX2 = nX + GB_REQ_TIME_EDIT_WIDTH;

	nY	= m_nPosY + GB_REQ_TIME_EDIT_POS_Y;
	nY2 = nY + GB_REQ_TIME_EDIT_HEIGHT;

	pDrawPort->AddTexture( nX, nY, nX + EDITBOX_OFFSETX, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + EDITBOX_OFFSETX, nY, nX2 - EDITBOX_OFFSETX, nY2,
		m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - EDITBOX_OFFSETX, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );

	m_ebGuildReqTime.Render();
	
	m_btnOK.Render();
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextEx( m_strTitle, m_nPosX + GB_TITLE_TEXT_OFFSETX,		
										m_nPosY + GB_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S( 1098, "만 나스" ), m_nPosX + GB_REQ_NAS_EDIT_POS_X + GB_REQ_NAS_EDIT_WIDTH + 4,		
										m_nPosY + GB_REQ_NAS_EDIT_POS_Y + 3, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S ( 1902, "분" ), m_nPosX + GB_REQ_TIME_EDIT_POS_X + GB_REQ_TIME_EDIT_WIDTH + 4,		
										m_nPosY + GB_REQ_TIME_EDIT_POS_Y + 3, 0xFFFFFFFF );  


	// Flush all render text queue
	pDrawPort->EndTextEx();

}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderGBStatus
// Explain:  
// Date : 2005-03-17(오후 4:29:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderGBStatus()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdStateTexture );

	// 남은 시간 표시 ( ImageFont );
	

	int nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH;

	m_rcBox.Left = m_rcBlueBox.Left = m_rcRedBox.Left = nOffsetX;
	m_rcBox.Right = m_rcBlueBox.Right = m_rcRedBox.Right = nOffsetX + BOX_WIDTH;

	// Ren, Blue Box 표시
	pDrawPort->AddTexture( m_rcBox.Left, m_rcBox.Top, m_rcBox.Right, m_rcBox.Bottom,
									m_rtBox.U0, m_rtBox.V0,
									m_rtBox.U1, m_rtBox.V1,
									0xFFFFFFFF );

	pDrawPort->AddTexture( m_rcBlueBox.Left, m_rcBlueBox.Top, m_rcBlueBox.Right, m_rcBlueBox.Bottom,
									m_rtBlueBox.U0, m_rtBlueBox.V0,
									m_rtBlueBox.U1, m_rtBlueBox.V1,
									0xFFFFFFFF );
	
	pDrawPort->AddTexture( m_rcRedBox.Left, m_rcRedBox.Top, m_rcRedBox.Right, m_rcRedBox.Bottom,
									m_rtRedBox.U0, m_rtRedBox.V0,
									m_rtRedBox.U1, m_rtRedBox.V1,
									0xFFFFFFFF );

	// 남은 시간 표시 ( Image Font : Small )
	RenderLeftTime();

	// Kill Point 표시 ( ImageFont : Large);
	RenderKillPoint();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	CTString strMessage;
	
	strMessage.PrintF ( _S( 1099, "전투지역 : %s" ), CZoneInfo::getSingleton()->GetZoneName( m_nBattleZone ) ); 	
	
	int nX = m_rcRedBox.Left + ( m_rcRedBox.Right - m_rcRedBox.Left ) / 2;

	// 존 표시 (for TEXT)
	pDrawPort->PutTextExCX( strMessage, nX, BATTLE_ZONE_NAME_Y, 0xFFFFFFE5, TRUE, 0x181818FF );

	// 길드명 표시 (for TEXT)
	pDrawPort->PutTextExCX( m_strGuildName, nX, GUILD_NAME_Y, 0xA6C0FFE5 );
	pDrawPort->PutTextExCX( m_strTargetGuildName, nX, GUILD_NAME2_Y, 0xFF7E6EE5 );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::DrawNumber 
// Explain:  
// Date : 2005-03-22(오후 7:54:56) Lee Ki-hwan
// bool bLarge : 큰 숫자
// bool bRight : 오른쪽 정렬
//------------------------------------------------------------------------------
void CUIGuildBattle::DrawNumber ( int x, int y, int nNumber, bool bLarge )
{
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= SMALL_NUMBER_WIDTH;
	int nHeight			= SMALL_NUMBER_HEIGHT;

	if ( bLarge )
	{
		rtNumber	= m_rtLargeNumber[nNumber];
		nWidth		= LARGE_NUMBER_WIDTH;	
		nHeight		= LARGE_NUMBER_HEIGHT;
	}

	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + nWidth, y + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderLeftTime
// Explain:  
// Date : 2005-03-22(오후 8:44:31) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderLeftTime()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmLeftTime;

	int lLeftTime = m_lLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	int iSec = lLeftTime % 60;
	lLeftTime /= 60;

	int iMin = lLeftTime % 60;
	int iTime = lLeftTime /= 60;

	int nX = CUIManager::getSingleton()->GetDrawPort()->GetWidth() - RIGHT_SPACE - ( SMALL_NUMBER_WIDTH * 6 + COLON_WIDTH * 2 );
	int nY = LEFT_TIME_Y;

	int t10;
	// 시간 출력
	
	t10 = iTime / 10;
	iTime %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iTime );		nX += SMALL_NUMBER_WIDTH;

	DrawColon( nX, nY );		nX += COLON_WIDTH;

	// 분 출력 
	t10 = iMin / 10;
	iMin %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iMin );		nX += SMALL_NUMBER_WIDTH;

	DrawColon ( nX, nY );		nX += COLON_WIDTH;

	// 초 출력 
	t10 = iSec / 10;
	iSec %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iSec );	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderKillPoint
// Explain:  
// Date : 2005-03-23(오후 3:02:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderKillPoint()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;
	
	int nKillPoint = m_nKillPoint;
	int nTargetKillPoint = m_nTargetKillPoint;

	int nHalfWidth = LARGE_NUMBER_WIDTH / 2 ;

	int i = 0;
	int nRenderNumber = 0;

	// Blue Kill Point
	for( i = 1;  ; i *= 10 )
	{
		if ( nKillPoint / i > 0 )
		{
			nOffsetX -= nHalfWidth;		
		}
		else
		{
			break;
		}
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nOffsetX -= nHalfWidth;	
	}
	
	while( i )
	{		
		nRenderNumber = nKillPoint / i;
		nKillPoint %= i;

		DrawNumber ( nOffsetX, BLUE_KILL_POINT_Y, nRenderNumber, TRUE);

		printf ( "%d", nRenderNumber );
		nOffsetX += LARGE_NUMBER_WIDTH;	
		i /= 10;
	}

	// Red Kill Point
	nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;

	for( i = 1;  ; i *= 10 )
	{
		if ( nTargetKillPoint / i > 0 )
		{
			nOffsetX -= nHalfWidth;		
		}
		else
		{
			break;
		}
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nOffsetX -= nHalfWidth;	
	}
	
	while( i )
	{		
		nRenderNumber = nTargetKillPoint / i;
		nTargetKillPoint %= i;

		DrawNumber ( nOffsetX, RED_KILL_POINT_Y, nRenderNumber, TRUE);

		printf ( "%d", nRenderNumber );
		nOffsetX += LARGE_NUMBER_WIDTH;	
		i /= 10;
	}

}


//------------------------------------------------------------------------------
// CUIGuildBattle::DrawColon
// Explain:  
// Date : 2005-03-22(오후 9:57:29) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::DrawColon( int x, int y )
{
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + COLON_WIDTH, y + SMALL_NUMBER_HEIGHT,
							m_rtColon.U0, m_rtColon.V0, m_rtColon.U1, m_rtColon.V1,
							0xFFFFFFFF );	
}


//------------------------------------------------------------------------------
// CUIGuildBattle::AddGBReqdDescString
// Explain:  
// Date : 2005-03-17(오후 5:03:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::AddGBReqdDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

#if defined (THAI)
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strDesc);
	INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbGBReqdDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

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

			AddGBReqdDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}

	}
#else		
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		int iPos;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbGBReqdDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		int		iPos;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

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

			AddGBReqdDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
#endif
}


//------------------------------------------------------------------------------
// CUIGuildBattle::OpenGB
// Explain:  
// Date : 2005-03-17(오전 11:39:11) Lee Ki-hwan
//
//------------------------------------------------------------------------------
void CUIGuildBattle::OpenGBReq( int nTargetCharIndex, CTString strGuildName )
{
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( m_eGBState == GBS_IN_REQ_ACCEPT )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1100, "이미 길드 전투가 성립되었습니다."), SYSMSG_ERROR ); 	
		return;
	}
	else if( m_eGBState == GBS_IN_BATTLE )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1101, "이미 길드 전투가 시작되었습니다."), SYSMSG_ERROR );  	
		return;
	}
	else if( m_eGBState == GBS_IN_REQ )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1102, "이미 길드 전투 신청 중입니다."), SYSMSG_ERROR ); 	
		return;
	}
	
	Clear ();
	m_ebGuildReqNas.ResetString();
	m_ebGuildReqTime.ResetString();

	// 길드를 신청한 캐릭터의 Index는 저장해서 나중에 서버에 신청 메세지를 보낼 때 사용 한다.
	m_nTargetCharIndex = nTargetCharIndex;
	m_strTargetGuildName = strGuildName;
		
	CTString strMessage;

	strMessage.PrintF( _S( 1103, "[%s]길드에게 길드 전투를 신청합니다." ), m_strTargetGuildName ); 	
	AddGBReqdDescString ( strMessage );

	strMessage.PrintF( _S( 1386, "전투 신청금과 전투시간을 입력하세요." ), m_strTargetGuildName ); 	
	AddGBReqdDescString ( strMessage );
	
	strMessage.PrintF( _S( 1387, "길드전투에서 승리하게 되면 %d%%의 수수료를 제외한 양쪽 길드의 모든 신청금을 갖습니다." ), GB_COMMISSION ); 	
	AddGBReqdDescString( strMessage );
	strMessage = " ";
	AddGBReqdDescString( strMessage );

	if(g_bNasTrans)// 브라질 나스 표시 수정
	{
		strMessage.PrintF( _S( 1388, "전투 신청금 : %s 나스 ~ %s 나스" ),GB_MIN_NAS_EX, GB_MAX_NAS_EX); //테스트 번역 요청
	}else
	{
		strMessage.PrintF( _S( 1388, "전투 신청금 : %d만나스 ~ %d만나스" ), GB_MIN_NAS, GB_MAX_NAS ); 	
	}	
	AddGBReqdDescString( strMessage );

	strMessage.PrintF( _S( 1389, "전투 시간 : %d분 ~ %d분" ), GB_MIN_TIME, GB_MAX_TIME ); 	
	AddGBReqdDescString( strMessage );

	m_ebGuildReqNas.SetFocus( TRUE );
	m_ebGuildReqNas.SetString( "10" );	// 10만 나스 기본 셋팅

	m_ebGuildReqTime.SetFocus( FALSE );
	m_ebGuildReqTime.SetString( "30" );	// 30분 기본 셋팅

	pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );

	m_eGBState = GBS_REQ;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::ResetPosition
// Explain:  
// Date : 2005-03-17(오전 11:24:41) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::AdjustPosition
// Explain:  
// Date : 2005-03-17(오전 11:24:34) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}



//------------------------------------------------------------------------------
// CUIGuildBattle::SetFocus
// Explain:  
// Date : 2005-03-17(오전 11:24:29) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	//m_ebGuildReqNas.SetFocus( TRUE );
	//m_ebGuildReqTime.SetFocus( FALSE );
	if( !bVisible )
	{
		m_ebGuildReqNas.SetFocus( FALSE );
		m_ebGuildReqTime.SetFocus ( FALSE );
	}

}


//------------------------------------------------------------------------------
// CUIGuildBattle::MouseMessage
// Explain:  
// Date : 2005-03-17(오전 11:24:22) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::MouseMessage( MSG *pMsg )
{
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

			// 길드 신청 원도우 
			if ( IsGBReq() )
			{
				// OK Button
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsGBReq() )
			{
				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();
					nOldX = nX;		nOldY = nY;
				
					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}

				
					// OK Button
					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						GBReq();	// 길드 전투 신청
						return WMSG_SUCCESS;
					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}						
					else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_ebGuildReqTime.SetFocus ( FALSE );
						pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
						return WMSG_SUCCESS;
					}
					else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_ebGuildReqNas.SetFocus ( FALSE );
						pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
						return WMSG_SUCCESS;
					}
				
					pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
		
			if ( IsGBReq() )
			{
				// OK Button
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					Close();
					return WMSG_SUCCESS;
				}
				else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebGuildReqTime.SetFocus ( FALSE );
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebGuildReqNas.SetFocus ( FALSE );
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
					
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::KeyMessage
// Explain:  
// Date : 2005-03-17(오전 11:24:17) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;

	if ( IsGBReq() )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			GBReq();// 길드 전투 신청
			return WMSG_SUCCESS; 
		}
		if( m_ebGuildReqNas.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
		else if( m_ebGuildReqTime.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}
	return WMSG_FAIL;

}


//------------------------------------------------------------------------------
// CUIGuildBattle::IMEMessage
// Explain:  
// Date : 2005-03-17(오전 11:24:16) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::IMEMessage( MSG *pMsg )
{
	if ( IsGBReq() )
	{
		WMSG_RESULT wmsgResult;

		if( ( wmsgResult = m_ebGuildReqNas.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
		else 
		{
			return m_ebGuildReqTime.IMEMessage( pMsg );
		}
	}
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::CharMessage
// Explain:  
// Date : 2005-03-17(오전 11:24:14) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::CharMessage( MSG *pMsg )
{
	if ( IsGBReq() )
	{
		if( m_ebGuildReqNas.CharMessage( pMsg ) != WMSG_FAIL )
		{	
			return WMSG_SUCCESS;
		}
		else if( m_ebGuildReqTime.CharMessage( pMsg ) != WMSG_FAIL )
		{	
			return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::MsgBoxCommand
// Explain:  
// Date : 2005-03-17(오전 11:24:26) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{

	switch( nCommandCode )
	{
	
	case MSGCMD_GUILD_BATTLE_STOP_REQ:
		{
			if( !bOK )	return;
			else _pNetwork->GBStopReq( m_nTargetCharIndex );
		}
		break;
	case MSGCMD_GUILD_BATTLE_STOP_REQ_REP:
		{
			if ( bOK )	// 길드 중단 요청 수락 
				_pNetwork->GBStopReqAccept();
			else		// 길드 중단 요청 거절
				_pNetwork->GBStopReqReject();						
		}
		break;
	case MSGCMD_GUILD_BATTLE_REQ:
		{
			// 길드 요청 취소
			if( !bOK )	return;
			else _pNetwork->GBReqReject();
		}
		break;
	case MSGCMD_GUILD_BATTLE_ACCEPT:
		{
			if ( bOK )	// 길드 요청 수락 
				_pNetwork->GBReqAccept();
			else		// 길드 요청 거절
				_pNetwork->GBReqReject();			
		}
		break;
	case MSGCMD_GUILD_BATTLE_ERROR:	// 에러 발생
		{
			if( !bOK )	return;
			else Close();			
		}
		break;
	case MSGCMD_GUILD_BATTLE_MESSAGE: //일반 알림 메세지
		{
			if( !bOK )	return;
		}
		break;
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::MsgBoxLCommand
// Explain:  
// Date : 2005-03-17(오전 11:24:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGCMD_GUILD_BATTLE_ERROR:
		{

		}
		break;
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReq
// Explain:  
// Date : 2005-03-18(오후 4:48:09) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReq()
{
	CloseAllMsgBox();
	CTString strMessage;
	
	// 에러 체크 
	// 1. 나스가 범위 내에 있는가 
	int nGuildReqNas = atoi( m_ebGuildReqNas.GetString() );
	if( nGuildReqNas > GB_MAX_NAS || nGuildReqNas < GB_MIN_NAS )
	{
		if(g_bNasTrans)//브라질 나스 표시 수정
		{
			strMessage.PrintF( _S( 1106, "%s에서 %s 나스의 금액을 입력하여 주십시오." ), GB_MIN_NAS_EX, GB_MAX_NAS_EX );	
		}else
		{
			strMessage.PrintF( _S( 1106, "%d만에서 %d만 나스의 금액을 입력하여 주십시오." ), GB_MIN_NAS, GB_MAX_NAS );	
		}
		

		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, strMessage );
		m_ebGuildReqNas.ResetString();
		return;
	}

	// 1.5 시간이 범위 내에 있는가 
	int nGuildReqTime = atoi ( m_ebGuildReqTime.GetString() );
	if( nGuildReqTime > GB_MAX_TIME || nGuildReqTime < GB_MIN_TIME )
	{
		strMessage.PrintF( _S ( 1903, "%d분에서 %d분 사이의 시간을 입력하여 주십시오." ), GB_MIN_TIME, GB_MAX_TIME );	

		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, strMessage );
		m_ebGuildReqTime.ResetString();
		return;
	}
	
	nGuildReqTime *= 60; // 초단위로 변환

	// 2. 소지금이 충분한가?
	if( _pNetwork->MyCharacterInfo.money/10000 < nGuildReqNas )
	{
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1107, "소유 나스가 부족하여 전투신청을 할 수 없습니다." ) ); 	
		m_ebGuildReqNas.ResetString();
		return;
	}
	
	_pNetwork->GBReq ( m_nTargetCharIndex, nGuildReqNas, nGuildReqTime );
		
	GBMessage ( MSGCMD_GUILD_BATTLE_REQ, _S( 1108, "길드 전투를 신청 중입니다.\n취소하시겠습니까?" ), UMBS_OK ); 	
	
	m_eGBState = GBS_IN_REQ;	
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReg
// Explain: 길드 전투 신청 받음 
// Date : 2005-03-18(오후 2:26:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReq( int nGuildIndex, CTString strGuildName, int nPrize, int nTime )
{
	CloseAllMsgBox();
	CTString strMessage;
	if (g_bNasTrans)//브라질 나스 표시 수정ㅇ
	{
		CTString strTempNas;
		strTempNas.PrintF("%d0000",nPrize);

		CUIManager::getSingleton()->InsertCommaToString( strTempNas ); 
		strMessage.PrintF( _S( 1904, "[%s]길드가 길드 전투를 신청하였습니다. 전투 신청금은 %s 나스 전투 시간은 %d분입니다.\n수락 하시겠습니까?" ), 
			strGuildName.str_String, strTempNas,(nTime/60) ); 	
	}else
	{
		strMessage.PrintF( _S( 1904, "[%s]길드가 길드 전투를 신청하였습니다. 전투 신청금은 %d만나스 전투 시간은 %d분입니다.\n수락 하시겠습니까?" ), strGuildName.str_String, nPrize, (nTime/60) ); 	
	}
	GBMessage ( MSGCMD_GUILD_BATTLE_ACCEPT, strMessage, UMBS_YESNO );

}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReqReject
// Explain:  길드 전투 요청을 거절당함 
// Date : 2005-03-18(오후 2:53:16) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReqReject( int nRejectCharIndex )
{
	CloseAllMsgBox();
		
	if( nRejectCharIndex == _pNetwork->MyCharacterInfo.index ) // 취소 한 캐릭터가 자신이면
	{
		SYSERROR_MSG( _S( 1110, "길드 전투 신청이 중단되었습니다." ) ); 	
	}
	else // 취소 한 캐릭터가 상대 편이라면
	{
		SYSERROR_MSG( _S( 1110, "길드 전투 신청이 중단되었습니다." ) );	
	}

	Close();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReqAccept
// Explain:  길드 전투 요청을 수락 받음 ALL
// Date : 2005-03-18(오후 2:53:22) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReqAccept( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime )
{
	CloseAllMsgBox();

	// DATA Settting....
	// 데이터 셋팅
	// 내길드
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
	}
	else
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
	}
	
	m_nBattleZone			= nZone;
	m_nPrize				= nPrize;
	
	m_lLeftTime				= nTime;

	//길드전 공표 메세지 출력	
	CTString strMessage, strMessage2;
	strMessage.PrintF( _S( 1111, "[%s]길드와 [%s]길드간의 길드 전투가 성립 되었습니다." ), strGuildName1, strGuildName2 ); 	
	strMessage2.PrintF( _S( 1112, "%d분 후부터 %d분 동안 두 길드는 적대 상태가 됩니다." ), GB_START_MINUTE, (m_lLeftTime/60) ); 	
	
	SetNotice ( strMessage, strMessage2 );

	m_eGBState = GBS_IN_REQ_ACCEPT;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStart
// Explain:  길드 전투 시작
// Date : 2005-03-18(오후 2:53:39) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStart( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime )
{
	// 데이터 셋팅
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
	}
	else
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
	}
	
	m_nBattleZone			= nZone;
	m_nPrize				= nPrize;

	m_lLeftTime				= nTime;
		
	// 길드전 시작 메세지 출력 // Dealy Time 을 정해 주여야 한다!!!
	CTString strMessage, strMessage2;
	strMessage.PrintF( _S( 1116, "[%s]길드와 [%s]길드간의 길드 전투가 시작 되었습니다." ), strGuildName1, strGuildName2 );
	if (g_bNasTrans)//브라질 나스 표시 수정
	{
		CTString strTempNas;
		strTempNas.PrintF("%d0000",nPrize);

		CUIManager::getSingleton()->InsertCommaToString( strTempNas );
		strMessage2.PrintF( _S( 1117, "양 길드의 전투 신청금 총 합은 %s 나스 입니다." ), strTempNas );
	}else
	{
		strMessage2.PrintF( _S( 1117, "양 길드의 전투 신청금 총 합은 %d만 나스 입니다." ), nPrize );
	}

	SetNotice ( strMessage, strMessage2 );

	//!! Start Effect
	StartEffect();

	m_eGBState = GBS_IN_BATTLE;

	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
}


//------------------------------------------------------------------------------
// CUIGuildbattle::GBStatue
// Explain:  길드 전투 상태 표시 
// Date : 2005-03-18(오후 4:31:27) Lee Ki-hwan
// - 1분마다 한번씩 갱싱, 데이터 변경시 갱신 
// - int Time; // 초단위 (남은시간)
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone )
{
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nKillPoint			= nCount1;
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
		m_nTargetKillPoint		= nCount2;
	}
	else
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nKillPoint			= nCount2;
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
		m_nTargetKillPoint		= nCount1;
	}
	
	m_nBattleZone			= nZone;
	m_lLeftTime				= Time;

	// 종료후 다시 접속했을 때 처리
	if ( m_eGBState != GBS_IN_BATTLE && g_slZone == m_nBattleZone )
	{
		m_eGBState = GBS_IN_BATTLE; 	
		SetEnable( TRUE );
		SetVisible ( TRUE );
		StartEffect();
		CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
	}
	else if (m_eGBState == GBS_IN_BATTLE && g_slZone != m_nBattleZone)
	{ // 존 이동 시 처리
		m_eGBState = GBS_BATTILE_NOT_ZONE;
		StopGuildEffect(_pNetwork->MyCharacterInfo.index);
	}
	
	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReq
// Explain: 길드 전투 중단 요청
// Date : 2005-03-21(오후 7:58:07) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReq( int nTargetCharIndex, CTString strGuildName )
{
	CloseAllMsgBox();

	m_nTargetCharIndex = nTargetCharIndex;
	
	CTString strMessage;
	strMessage.PrintF( _S( 1118, "[%s]길드에게 길드 전투중단 신청을 하시겠습니까?" ), strGuildName ); 	

	GBMessage ( MSGCMD_GUILD_BATTLE_STOP_REQ, strMessage, UMBS_YESNO );	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReq
// Explain: 길드 전투 중단 요청을 받음
// Date : 2005-03-18(오후 2:53:27) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqRev( int nGuildIndex, CTString strGuildName )
{
	CloseAllMsgBox();

	CTString strMessage;
	strMessage.PrintF( _S( 1119, "[%s]길드가 길드 전투중단 신청을 하였습니다. 수락하시겠습니까?" ), strGuildName ); 	
	
	GBMessage ( MSGCMD_GUILD_BATTLE_STOP_REQ_REP, strMessage, UMBS_YESNO );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReqReject
// Explain:  길드 전투 중단 요청을 거절 당함
// Date : 2005-03-18(오후 2:53:33) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqReject( int nRejectCharIndex )
{
	CloseAllMsgBox();
	
	if( nRejectCharIndex == _pNetwork->MyCharacterInfo.index ) // 걸절자가 자기 자신
	{
		SYSERROR_MSG( _S( 1120, "길드 전투 중단 요청이 취소되었습니다." ) ); 	
	}
	else  // 상대 편이 거절
	{
		SYSERROR_MSG( _S( 1120, "길드 전투 중단 요청이 취소되었습니다." ) ); 	
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReqAccept
// Explain: 길드 전투 중단 요청을 수락 받음
// Date : 2005-03-18(오후 3:27:01) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqAccept()
{
	CloseAllMsgBox();

	GBMessage ( MSGCMD_GUILD_BATTLE_MESSAGE, _S ( 1121, "길드 전투가 중단 되었습니다." ), UMBS_OK );	
}

//------------------------------------------------------------------------------
// CUIGuildBattle::GBEnd
// Explain: 길드 전투 종료
// Date : 2005-03-18(오후 2:53:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBEnd( int nWinnerGuildIndex, int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize )
{
	CloseAllMsgBox();
	StopEffect();
	
	m_eGBState = GBS_END;

	CTString strMessage, strMessage2;
	CTString strWinnerGuildName;

	if ( nWinnerGuildIndex == -1 )
	{
		strMessage.PrintF( _S( 1122, "[%s]길드와 [%s]길드간의 길드전투가 무승부가 되었습니다." ), strGuildName1, strGuildName2 );
		if(g_bNasTrans)//브라질 나스 표시 수정 
		{
			CTString strTempNas;
			strTempNas.PrintF("%d",nPrize);
			CUIManager::getSingleton()->InsertCommaToString( strTempNas );
			strMessage2.PrintF( _S( 1123, "세금 %d%%를 제외한 길드 전투 신청금 %s나스는 각 길드장에게 되돌아 갑니다." ), GB_COMMISSION, strTempNas ); 	
		}else{
			strMessage2.PrintF( _S( 1123, "세금 %d%%를 제외한 길드 전투 신청금 %d나스는 각 길드장에게 되돌아 갑니다." ), GB_COMMISSION, nPrize ); 	
		}

	}
	else
	{
		strWinnerGuildName = (nGuildIndex1 == nWinnerGuildIndex)?strGuildName1:strGuildName2;
		strMessage.PrintF( _S( 1124, "[%s]길드와 [%s]길드간의 길드전투에서 [%s]길드가 승리하였습니다." ), strGuildName1, strGuildName2, strWinnerGuildName ); 	
		if(g_bNasTrans)//브라질 나스 표시 수정 
		{
			CTString strTempNas;
			strTempNas.PrintF("%d",nPrize);
			CUIManager::getSingleton()->InsertCommaToString( strTempNas );
			strMessage2.PrintF( _S( 1125, "[%s]길드에게 세금 %d%%를 제외한 전투 신청금 %s나스를 지급합니다." ), strWinnerGuildName, GB_COMMISSION, strTempNas ); 	
		}else{
			strMessage2.PrintF( _S( 1125, "[%s]길드에게 세금 %d%%를 제외한 전투 신청금 %d나스를 지급합니다." ), strWinnerGuildName, GB_COMMISSION, nPrize ); 	
		}
	}

	SetNotice ( strMessage, strMessage2 );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorMessage
// Explain:  
// Date : 2005-03-19(오후 12:28:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBErrorMessage( int nCommandCode, CTString strErrorMessage )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(nCommandCode);
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_GUILD_BATTLE, nCommandCode );
	MsgBoxInfo.AddString( strErrorMessage );	
	pUIManager->CreateMessageBox( MsgBoxInfo );		
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBMessage
// Explain:  
// Date : 2005-03-21(오후 4:46:32) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBMessage( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_GUILD_BATTLE, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}



//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorProcess
// Explain:  
// Date : Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBErrorProcess( int nErrorCode )
{
	CloseAllMsgBox();
	switch ( nErrorCode )
	{
	case MSG_GUILD_ERROR_BATTLE_NOGUILD:			// 길드전 - 관련 길드가 없음
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1126, "관련 길드가 없습니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOTBOSS:			// 길드전 - 길드장이 없음
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1127, "길드장이 존재하지 않습니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT:		// 길드전 - 인원 수 부족
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1128, "길드 인원이 부족하여 길드 전투를 할 수 없습니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_ZONE:				// 길드전 - 가능 존이 아님
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1129, "길드 전투가 가능한 존이 아닙니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_ALREADY_BATTLE:			// 길드전 - 이미 길드전 중
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1130, "이미 길드 전투가 진행 중입니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_PRIZE:				// 길드전 - 배팅금액 자체 오류 (길드전 배팅 허용금액이 아님)
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S ( 1131, "신청 금액이 너무 작거나 큽니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOT_REQ:			// 길드전 - 신청한적 없음
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1132, "길드 전투를 신청한 적이 없습니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE:		// 길드전 - 배팅금액 모질라
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1133, "나스가 부족하여 길드 전투를 진행할 수 없습니다." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOT_BATTLE:			// 길드전 = 길드전 중이 아님
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1134, "진투 전투 중이 아닙니다." ) ); 	
		break;
	}
	
	// Date : 2005-04-06(오후 5:08:43), By Lee Ki-hwan
	// 에러 메세지 후에는 무조건 상황 종료...
	//Close();
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, FALSE );
	Clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StartGuildEffect
// Explain:  
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StartGuildEffect( SLONG slCharIndex, CEntity* penEntity, BOOL bEnemy )
{
	// 이미 존재 하는지 확인 해서 존재하면 return 
	if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return;
		
	CEffectGroup *pEG = StartEffectGroup( bEnemy?"cpp_gb_red":"cpp_gb_blue"
						, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
						,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEG.insert(tmpVal);
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StopGuildEffect
// Explain:  
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StopGuildEffect( SLONG slCharIndex )
{
	// 이펙터가 활성화 되어 있지 않다면 끝내라..
	if( m_mapEG.find( slCharIndex ) == m_mapEG.end() ) return;

	if( m_mapEG[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEG[slCharIndex] ) )
		m_mapEG[slCharIndex]->Stop(0.0f);

	m_mapEG.erase( slCharIndex );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StartEffect
// Explain:  
// Date : 2005-03-22(오전 11:16:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StartEffect()
{
	// Start Effect My Character
	StartGuildEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), FALSE );

	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->StartGuildEffect();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StopEffect
// Explain:  모든 캐릭터의 Effect를 중지시킨다.
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StopEffect()
{
	// All Character Effect Stop
	std::map<SLONG, CEffectGroup *>::iterator	itEG;

	for ( itEG = m_mapEG.begin(); itEG != m_mapEG.end(); ++itEG )
	{
		if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
			(*itEG).second->Stop(0.0f);
	}

	m_mapEG.clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::IsEnemy
// Explain:  적인지 확인 (공격시 사용)
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL ENGINE_API CUIGuildBattle::IsEnemy( unsigned long ulID )
{
	if ( !IsInBattle() ) return FALSE;

	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_CHARACTER, ulID);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		// Guild Indexx Check
		if (pTarget->cha_lGuildIndex == m_nTargetGuildIndex)
		{
			return TRUE;
		}		
	}

	return FALSE;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::IsEnemyGuild
// Explain: 적 길드 인지 확인 (Effect 표시 할때 사용)
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int ENGINE_API CUIGuildBattle::IsEnemyGuild( unsigned long ulGuildIndex )
{
	if( ulGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
	{
		return -1; // 아군
	}
	else if( ulGuildIndex == m_nTargetGuildIndex )
	{
		return 1; // 적군 
	}
	return 0;
}



//------------------------------------------------------------------------------
// CUIGuildBattle::IsInBattle
// Explain: 전투 중인지 확인 
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIGuildBattle::IsInBattle()
{ 
	if( m_eGBState == GBS_IN_BATTLE && g_slZone == m_nBattleZone ) 
		return TRUE; 
	
	return FALSE; 
}