#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIQuestComplete.h>

// Position and Size
#define DEF_SIGEWARFARE_START_Y		238

#define SIGEWARFARE_WIDTH			311		// 원도우 크기
#define SIGEWARFARE_HEIGHT			275

// Battle시 정보 표시 
#define BATTLE_WIDTH				118		
#define BATTLE_HEIGHT				155
#define UNIT_TEXT_OFFSETX			8		// 길드 이름
#define UNIT_TEXT_OFFSETY			6
#define UNIT_POINT_OFFSETX			114		// 길드 포인트
#define UNIT_POINT_OFFSETY			22
#define UNIT_HEIGHT					44		// 객체 하나 하나		
#define UNIT_TIME_HEIGHT			21

#define INFO_OFFSETY				160		// Selection Battle시 부가 정보창 
#define INFO_WIDTH					118
#define INFO_HEIGHT					93

#define LEFT_TIME_X					29		// 남은시간
#define LEFT_TIME_Y					2

#define TIME_NUMBER_WIDTH			12		// 시간 표시하는 버호
#define TIME_NUMBER_HEIGHT			17

#define COLON_WIDTH					4		// :
#define COLON_INDEX					10	

#define NUMBER_WIDTH				16		// Point 표시하는 번호
#define NUMBER_HEIGHT				19

// 시간 설정
#define TITLE_SX					25		// 타이틀 바
#define TITLE_SY					5
#define TITLE_HEIGHT				25
											// 배경
#define BACK_MIDDLE_DESC			150		// 중간 설명 창
#define BACK_MIDDLE_SELECT			57		// 아래 선택 창
#define BACK_BOTTOM_HEIGHT			7		// 아랫쪽 창의 높이

#define CHECKBOX_SIZE				11		// 체크 박스
#define OPTION_TEXT_X1				23		
#define OPTION_TEXT_X2				143
#define OPTION_TEXT_Y				52

#define START_BOTTOM_BUTTON_Y		242		// 아래 확인 버튼

// Color
// 시간 설정 시 사용 
#define SW_COLOR_TITLE				0xE18600FF	// 타이틀
#define SW_COLOR_DESC				0xA3A1A3FF	// 설명
#define SW_COLOR_TEXT				0xF2F2F2FF	// 좀 중요한 글

#define SW_COLOR_GUILD_NAME			0xE18600FF	// 길드 이름 
#define SW_COLOR_GUILD_POINT		0xd7d0a8ff	// 길드포인트
#define SW_COLOR_DEFENSE_POINT		0xa9a6d9ff  // 수성측포인트

// Selection
#define SEL_SW_REQ					0		// 공성신청  
#define	SEL_MERCEAR_REQ				1		// 용병신청
#define SEL_SET_SW_TIME				2		// 공성시간 설정
#define SEL_SW_INFO					3		// 공성 정보 확인
#define SEL_SW_CANCEL				4		// 취소하기
#define SEL_COSTUME_REQ				5		// 성주 코스튬 요청
#define SEL_GUILD_COSTUME_REQ		6		// 성주 길드 코스튬 요청
#define SEL_GUILD_TITLE_REQ			7		// 성주 길드 호칭 요청

#define SEL_DEF_MERCEAR_GUILD_REQ	0		// 용병 길드 신청
#define	SEL_DEF_MERCEAR_REQ			1		// 수성측 용병 신청
#define SEL_ATT_MERCEAR_REQ			2		// 공성측 용병 신청
#define SEL_MERCEAR_CANCEL			3		// 취소하기

// Notice
#define	NOTICE_DELAY				8000			
#define	NOTICE_FADEOUT				7000
#define	NOTICE_FADETIME				( NOTICE_DELAY - NOTICE_FADEOUT )

#define SIEGEWARFAREINFO_PER_PAGE	5

//------------------------------------------------------------------------------
// CUISiegeWarfare::CUISiegeWarfare
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfare::CUISiegeWarfare()
{
	Clear();	
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::~CUISiegeWarfare
// Explain:  
// Date : 2005-07-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfare::~CUISiegeWarfare()
{
	if( _pUISWDoc!= NULL ) 
	{
		delete _pUISWDoc;
		_pUISWDoc = NULL;
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::ResetPosition
// Explain:  가운데 정렬
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	int nTempState = _pUISWDoc->GetUIState();

	// OpenSetSWTime - 가운데 정렬
	// OpenBattle - 전투 시에는 오른쪽 정렬 

	if (nTempState == SWS_BATTLE || nTempState == SWS_SELECT_BATTLE)
	{
		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
		SetPos(	pDrawPort->dp_MaxI - GetWidth() - 10, DEF_SIGEWARFARE_START_Y );
	}else
	{
		SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	}
	
}


//------------------------------------------------------------------------------
// CUISiegeWarfare::ResetPos
// Explain: 강제적으로 윈도우의 위치 조정 
// OpenSetSWTime - 가운데 정렬
// OpenBattle - 전투 시에는 오른쪽 정렬 
// Date : 2005-07-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::ResetPos( BOOL bCenter )
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( bCenter ) // 가운데 정렬 
	{
		ResetPosition( pDrawPort->dp_MinI, pDrawPort->dp_MinJ, pDrawPort->dp_MaxI, pDrawPort->dp_MaxJ );	
	}
	else // 오른쪽 정렬 
	{
		SetPos(	pDrawPort->dp_MaxI - GetWidth() - 10, DEF_SIGEWARFARE_START_Y );
	}
}
	
//------------------------------------------------------------------------------
// CUISiegeWarfare::AdjustPosition
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::Clear
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::Clear()
{
	//_pUISWDoc->SetUIState( SWS_NONE );

	m_tmNoticeTime	= 0;	
	m_bShowNotice	= FALSE;
	
	m_strNoticeMessage.Clear();
	m_strNoticeMessage2.Clear();
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::Create
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	_pUISWDoc = new CUISiegeWarfareDoc();
	_pUISWDoc->SetUIState( SWS_NONE );

	m_strTitle = _S(2417,"공성");	// 공성

	CUIWindow::Create(pParentWnd, nX, nY, SIGEWARFARE_WIDTH, SIGEWARFARE_HEIGHT);

	// Set Rect
	m_rcTitle.SetRect( 0, 0, 512, 22 );
		
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Set Background RectUV
	m_rt3BackT.SetUV3( 0, 0, 40, 26, 40, 0, 176, 26, 176, 0, 216, 26, fTexWidth, fTexHeight );
	m_rt3BackSM.SetUV3( 0, 27, 40, 30, 40, 27, 176, 30, 176, 27, 216, 30, fTexWidth, fTexHeight );
	m_rt3BackNM.SetUV3( 0, 31, 40, 34, 40, 31, 176, 34, 176, 31, 216, 34, fTexWidth, fTexHeight );
	m_rt3BackM.SetUV3( 0, 35, 40, 37, 40, 35, 176, 37, 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rt3BackL.SetUV3( 0, 38, 40, 45, 40, 38, 176, 45, 176, 38, 216, 45, fTexWidth, fTexHeight );

	// Button Create
	m_btnOK.Create( this, _S( 191, "확인" ), 90, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnCancel.Create( this, _S( 139, "취소" ), 162, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnClose.Create( this, CTString( "" ), 279, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// CheckButton Create
	int nStrWidth;
	nStrWidth = ( _S( 1980, "토요일" ).Length() + 3 ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );	
	
	m_cbtnSaturday.Create( this, OPTION_TEXT_X1 + nStrWidth, OPTION_TEXT_Y, 11, 11,
							_S( 1980, "토요일" ), TRUE, nStrWidth, nStrWidth );		
	
	m_cbtnSaturday.SetUV( UCBS_NONE, 12, 161, 23, 172, fTexWidth, fTexHeight );
	m_cbtnSaturday.SetUV( UCBS_CHECK, 0, 161, 11, 172, fTexWidth, fTexHeight );
	m_cbtnSaturday.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnSaturday.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnSaturday.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnSaturday.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnSaturday.SetCheck( FALSE );

	nStrWidth = ( _S( 1981, "일요일" ).Length() + 3 ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );	

	m_cbtnSunday.Create( this, OPTION_TEXT_X2 + nStrWidth, OPTION_TEXT_Y, 11, 11,
							_S( 1981, "일요일" ), TRUE, nStrWidth, nStrWidth );		
	
	m_cbtnSunday.SetUV( UCBS_NONE, 12, 161, 23, 172, fTexWidth, fTexHeight );
	m_cbtnSunday.SetUV( UCBS_CHECK, 0, 161, 11, 172, fTexWidth, fTexHeight );
	m_cbtnSunday.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnSunday.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnSunday.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnSunday.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnSunday.SetCheck( FALSE );

	// List box of guild description
	m_lbDesc.Create( this, 8, 142, 290, BACK_MIDDLE_SELECT + 40, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );

	int black = 74; // black 를 0으로 놓으면 회색 바탕의 Edit 박스 생성
	CTString strSiegeTime;
	int btnSize = 60;

#if defined G_USA
	strSiegeTime = CTString("Set Time");
	btnSize = 94;
#else
	strSiegeTime = CTString("PM");
#endif
	m_sbtnSWTime.Create( this, 24, 96, btnSize, 14, strSiegeTime, 37 );
	m_sbtnSWTime.SetDataBackUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
							141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
	
	m_sbtnSWTime.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbtnSWTime.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbtnSWTime.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbtnSWTime.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_sbtnSWTime.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbtnSWTime.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbtnSWTime.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbtnSWTime.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	m_sbtnSWTime.CreateButtons( 9, 7 );
	m_sbtnSWTime.SetWheelRect( -19, -75, 19, 165 );

	// [2010/11/01 : Sora] 공성 개편
	m_sbtnSWZone.Create( this, 134, 96, 105, 14, CTString(""), 95 );
	m_sbtnSWZone.SetDataBackUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
							141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
	
	m_sbtnSWZone.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbtnSWZone.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbtnSWZone.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbtnSWZone.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_sbtnSWZone.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbtnSWZone.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbtnSWZone.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbtnSWZone.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	m_sbtnSWZone.CreateButtons( 9, 7 );
	m_sbtnSWTime.SetWheelRect( -19, -75, 19, 165 );

	// Number
	int nOffsetX = 256;
	int nOffsetY = 157;

	int i;
	for( i = 0 ; i < 10; i++ )
	{
		m_rtNumber[i].SetUV( nOffsetX + i * NUMBER_WIDTH, nOffsetY, 
							nOffsetX + i * NUMBER_WIDTH + NUMBER_WIDTH, nOffsetY + NUMBER_HEIGHT, 
							fTexWidth, fTexHeight );
	}

	m_rtNumber[i].SetUV( nOffsetX + i * NUMBER_WIDTH+4, nOffsetY, 
							nOffsetX + i * NUMBER_WIDTH + NUMBER_WIDTH-4, nOffsetY + NUMBER_HEIGHT, 
							fTexWidth, fTexHeight );

	m_rtBattleBack.SetUV( 256, 0, 374, 155, fTexWidth, fTexHeight );

	m_rtSelectBattleBack.SetUV( 375, 0, 493, 93, fTexWidth, fTexHeight );

	// Notice
	//m_rcNotice1.SetRect( 0, 100, 0, 119 );
	//m_rcNotice2.SetRect ( 0, 120, 0, 139 );
	m_rcNotice1.SetRect( 0, 75, 0, 94 );
	m_rcNotice2.SetRect ( 0, 95, 0, 114 );

	m_rtNoticeL.SetUV( 375, 95, 407, 114, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 426, 117, 440, 136, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 409, 95, 441, 114, fTexWidth, fTexHeight );

}

//------------------------------------------------------------------------------
// CUISiegeWarfare::MsgBoxLCommand
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_SIEGE_WARFARE:
		{
			switch( nResult ) 
			{
			case SEL_SW_REQ:		// 공성 신청
				OpenSWReq();
				break;
			case SEL_MERCEAR_REQ:	// 용병 신청 
				OpenMercearReq();
				break;
			case SEL_SET_SW_TIME:	// 공성시간 설정 
				_pNetwork->SendGuildWarTimeMenu();
				break;
			case SEL_SW_INFO:		// 공성정보 확인
				_pNetwork->SendGuildWarTimeReq();
				break;
			case SEL_SW_CANCEL:		// 취소하기
				CloseSiegeWarfare();
				break;
			case SEL_COSTUME_REQ:
				OpenCostumeReq();
				break;
			// 성주 길드 코스튬 [1/21/2011 ldy1978220]
			case SEL_GUILD_COSTUME_REQ:
				{
					_pNetwork->LordGuildCostumeReq();
				}				
				break;
			case SEL_GUILD_TITLE_REQ:
				{
					_pNetwork->LordGuildTitleReq();
				}				
				break;
			}
		}
		break;
	case MSGLCMD_MERCEAR_REQ:
		{
			switch( nResult )
			{
			case SEL_DEF_MERCEAR_GUILD_REQ:	// 수성측 용병 길드 신청 
				{
					_pNetwork->DefenseGuildReq();
				}
				break;
			case SEL_DEF_MERCEAR_REQ:		// 수성측 용병참여
				{
					//_pNetwork->AttackCharReq();
					_pNetwork->AttackGuildReq();
				}
				break;
			case SEL_ATT_MERCEAR_REQ:		// 공성측 용병 참여 
				{
					//_pNetwork->AttackGuildReq();
					_pNetwork->AttackCharReq();
				}
				break;
			case SEL_MERCEAR_CANCEL:		// 취소하기
				{
					OpenSiegeWarfare();
				}
				break;
			}
		}
		break;
	case MSGLCMD_LORD_COSTUME_REQ:
		{
			if ( nResult == 0 )
			{
				// 서버로 성주 코스튬 아이템 요청
				_pNetwork->LordCostumeReq();
			}
		}
		break;
	}

}

//------------------------------------------------------------------------------
// CUISiegeWarfare::MsgBoxCommand
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SIEGE_WARFARE_INFO:
		//OpenSiegeWarfare();
		break;
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::CloseSiegeWarfare
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::CloseSiegeWarfare()
{
	Clear();
	_pUISWDoc->Clear();
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE, FALSE );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::RenderLeftTime
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::RenderLeftTime()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds(); // 현재 시간 얻기
	__int64	llCurDelay = llCurTime - _pUISWDoc->m_tmLeftTime;

	int lLeftTime = _pUISWDoc->m_lLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	int iSec = lLeftTime % 60;
	lLeftTime /= 60;

	int iMin = lLeftTime % 60;
	int iTime = lLeftTime /= 60;

	int nX = m_nPosX + LEFT_TIME_X;
	int nY = m_nPosY + LEFT_TIME_Y;

	int t10;
	// 시간 출력
	
	t10 = iTime / 10;
	iTime %= 10;

	DrawNumber( nX, nY, t10, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );			nX += TIME_NUMBER_WIDTH;
	DrawNumber( nX, nY, iTime, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );			nX += TIME_NUMBER_WIDTH;

	DrawNumber( nX, nY, COLON_INDEX, COLON_WIDTH, TIME_NUMBER_HEIGHT );			nX += COLON_WIDTH;

	// 분 출력 
	t10 = iMin / 10;
	iMin %= 10;

	DrawNumber( nX, nY, t10, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );			nX += TIME_NUMBER_WIDTH;
	DrawNumber( nX, nY, iMin, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );			nX += TIME_NUMBER_WIDTH;

	DrawNumber( nX, nY, COLON_INDEX, COLON_WIDTH, TIME_NUMBER_HEIGHT );			nX += COLON_WIDTH;

	// 초 출력 
	t10 = iSec / 10;
	iSec %= 10;

	DrawNumber ( nX, nY, t10, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );			nX += TIME_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iSec, TIME_NUMBER_WIDTH, TIME_NUMBER_HEIGHT );	
}

void CUISiegeWarfare::RenderNotice()
{
	if ( m_bShowNotice == FALSE )
		return;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmNoticeTime;
	if( llCurDelay < CHATMSG_NOTICE_DELAY )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = 0x00fffbff;
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

		pDrawPort->InitTextureData( m_ptdBaseTexture );
		
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
		
		if( m_bBottomNotice )
		{
			
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
		}
		
		// Render all elements
		pDrawPort->FlushRenderingQueue();
		
		
		pDrawPort->PutTextEx( m_strNoticeMessage, m_rcNotice1.Left, m_rcNotice1.Top + 4, colText );
		if( m_bBottomNotice )
			pDrawPort->PutTextEx( m_strNoticeMessage2, m_rcNotice2.Left, m_rcNotice2.Top + 4, colText );
		
		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		m_bShowNotice = FALSE;
		if( _pUISWDoc->IsUIState( SWS_END ) ) 
		{
			_pUISWDoc->StopEffect( -1, TRUE );
			_pUISWDoc->StopGuildMasterEffect( -1, TRUE );
			
			CloseSiegeWarfare();
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::Render
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::Render()
{
	_pUISWDoc->RunDalyTime();
	
	if( _pUISWDoc->IsUIState( SWS_SET_SW_TIME ) ) // 시간 설정 윈도우 
	{
		RenderSetSWTime();
	}
	else if( _pUISWDoc->IsBattle() || _pUISWDoc->IsSelectBattle() )
	{
		RenderBattle();
	}
	else if( _pUISWDoc->IsUIState( SWS_END ) ) 
	{	
		

	}
	
	RenderNotice();
}
 
//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenSlelectBattle
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenSelectBattle()
{
	SetSize( BATTLE_WIDTH, BATTLE_HEIGHT + INFO_HEIGHT + 4 );
	ResetPos( FALSE );
	_pUISWDoc->SetUIState( SWS_SELECT_BATTLE );
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE, TRUE );
	
	_pUISWDoc->m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}

//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenBattle
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenBattle()
{
	SetSize( BATTLE_WIDTH, BATTLE_HEIGHT );
	ResetPos( FALSE );

	_pUISWDoc->SetUIState( SWS_BATTLE );
	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE, TRUE );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenSiegeWarfare
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenSiegeWarfare()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SIEGE_WARFARE ) )
		return;	
	
	CTString strMessage;		// 설명 글...
	CTString strNecessity[4];	// 필요 조건
	CTString strSelection[5];	// 선택 스트링 
	CTString strName;
	int i;
	INDEX npcIndex = pUIManager->GetQuestBookComplete()->GetTargetIndex();

	// Create guild message box
	pUIManager->CreateMessageBoxL( m_strTitle, UI_SIEGE_WARFARE, MSGLCMD_SIEGE_WARFARE );					
	
	// [2010/11/01 : Sora] 공성 개편
	strName = _S( 1982, "공성NPC" );
	if( npcIndex == 1155 )
	{
		strName = CMobData::getData( npcIndex )->GetName();

		strMessage = _S( 5163, "근무중 이상 무!!\n각 지역에 있는 모든 공성정보 및 시간 설정은 저에게 말씀해 주십시오.\n바로 시정도록 하겠습니다." );	
	}
	else
	{
		strMessage = _S( 1983, "이곳엔 무슨 일이신가요?\n설마 성주와 전투를 신청하기 위해서 오신 건 아니겠죠?\n성주라는 것이 그렇게 쉽게되는 것이 아닙니다. 목숨이 아깝다면 그만 돌아 가시죠." );	
		
		strNecessity[0] = _S( 1984, "필요 길드레벨 : 5Lv" );	
		strNecessity[1] = _S( 1985, "필요 아이템 : 영웅의 증표" );	
		strNecessity[2] = _S( 1986, "필요 길드원 : 20명" );	
		strNecessity[3] = _S( 1987, "필요 나스 : 10만 나스" );	
	}

	pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, TRUE, strName, -1, SW_COLOR_TITLE );	

	// Description
	pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, TRUE, strMessage, -1, SW_COLOR_DESC );
	pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, TRUE, CTString(" "), -1, SW_COLOR_TEXT );
	
	// WSS_DRATAN_SEIGEWARFARE 2007/09/11	
	// 메라크만 필요 정보가 나오게 한다.
	if( npcIndex == 219 ) // 성관리인
	{
		for( i = 0; i < 4; i++ )
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, TRUE, strNecessity[i], -1, SW_COLOR_TEXT );
		}
	}	

	strSelection[0] = _S( 1988, "공성 신청." );	
	// WSS_DRATAN_SEIGEWARFARE 2007/09/11
	// 드라탄의 경우 용병->수성
	if( npcIndex == 387 )
	{
		strSelection[1] = _S( 1998, "수성측 용병길드 참여." );	
	}
	else
	{
		strSelection[1] = _S( 1989, "용병 신청." );	
	}
	
	strSelection[2] = _S( 1990, "공성시간 설정." );	
	strSelection[3] = _S( 1991, "공성정보 확인." );	
	strSelection[4] = _S( 880, "취소하기." );	

	CTString strLordCostume = _S( 5115, "성주 코스튬" );

	if ( !pUIManager->GetSiegeWarfareNew()->GetWarState() && !_pUISWDoc->IsWar() )
	{
		if ( (npcIndex == 219 || npcIndex == 387) && _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS && _pNetwork->MyCharacterInfo.ubGuildNameColor != 0 )
		{	// 드라탄 성주 길드이고 길마이면,
			pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, FALSE, strLordCostume, SEL_COSTUME_REQ );
		}

		// 성주 길드만 클라이언트에서 체크함 [1/21/2011 ldy1978220]
		if ( npcIndex == 1155 && _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS && _pNetwork->MyCharacterInfo.ubGuildNameColor != 0 )
		{	// 드라탄 성주 길드이고 길마이면,
			pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, FALSE, _S(2169,"성주 길드 의상받기"), SEL_GUILD_COSTUME_REQ );
			pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, FALSE, _S(2175,"성주 길드 호칭받기"), SEL_GUILD_TITLE_REQ );
		}
	}

	for( i = 0; i< 5; i++ )
	{
		//말레이시아는 공성시간 설정 메뉴 뺌. 메라크 공성만...		Su-won
#if defined G_MAL
		if(i==2 && pUIManager->GetQuestBookComplete()->GetTargetIndex() == 219 )
			continue;
#endif
		if( npcIndex == 1155 )
		{
			if( i < 2 )
				continue;
		}
		pUIManager->AddMessageBoxLString( MSGLCMD_SIEGE_WARFARE, FALSE, strSelection[i], i );
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenSWReq
// Explain:  공성 신청
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenSWReq() 
{	
	_pUISWDoc->SetUIState( SWS_SW_REQ );
	_pNetwork->AttackReq();
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenMercearReq
// Explain:  용병신청
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenMercearReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// WSS_DRATAN_SEIGEWARFARE 2007/09/11 ----------------------------->>
	// 드라탄의 경우 수성 길드 요청
	if( pUIManager->GetQuestBookComplete()->GetTargetIndex() == 387 )
	{
		_pNetwork->DefenseGuildReq();
		return;
	}	
	// ----------------------------------------------------------------<<

	static int nTotalSelection = 4;
	_pUISWDoc->SetUIState( SWS_MERCEAR_REQ );

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_MERCEAR_REQ ) )
		return;	
	
	CTString strMessage;		// 설명 글...
	CTString strNecessity[4];	// 필요 조건
	CTString strSelection[4];	// 선택 스트링 
	int i;

	// Create guild message box
	pUIManager->CreateMessageBoxL( m_strTitle, UI_SIEGE_WARFARE, MSGLCMD_MERCEAR_REQ );					

	pUIManager->AddMessageBoxLString( MSGLCMD_MERCEAR_REQ, TRUE, _S( 1992, "용병신청 NPC" ), -1, SW_COLOR_TITLE );		

	strMessage =_S( 1993,  "용병으로 이번 전투에 참가하고 싶으신가요?\n전장은 위험한 곳입니다. 굳이 참가를 하고 싶으시다면 말리지는 않겠지만 ..." );	
	
	strNecessity[0] = _S( 1994, "용병 참여" );	
	strNecessity[1] = _S( 1995, "필요 레벨 : 15Lv 이상" );	
	strNecessity[2] = _S( 1996, "용병 길드 참여" );	
	strNecessity[3] = _S( 1997, "필요 레벨 : 4Lv 이상" );	
	//0xE18600FF
	// Description
	pUIManager->AddMessageBoxLString( MSGLCMD_MERCEAR_REQ, TRUE, strMessage, -1, SW_COLOR_DESC );
	pUIManager->AddMessageBoxLString( MSGLCMD_MERCEAR_REQ, TRUE, CTString(" "), -1, SW_COLOR_DESC );

	for( i = 0; i < 4; i++ )
	{
		COLOR Color = SW_COLOR_TITLE;
		if( i & 1 ) Color = SW_COLOR_TEXT; //임시
		pUIManager->AddMessageBoxLString( MSGLCMD_MERCEAR_REQ, TRUE, strNecessity[i], -1, Color );
	}

	strSelection[0] = _S( 1998, "수성측 용병길드 참여." );	
	strSelection[1] = _S( 1999, "수성측 용병 참여." );	
	strSelection[2] = _S( 2000, "공성측 용병 참여." );	
	strSelection[3] =_S( 880,  "취소하기." );	

	for( i = 0; i< 4; i++ )
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_MERCEAR_REQ, FALSE, strSelection[i], i );
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::
// Explain:  정보 확인
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenSWInfo()
{
	_pUISWDoc->SetUIState( SWS_SW_INFO  );

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SIEGE_WARFARE_INFO ) )
		return;

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_SIEGE_WARFARE, MSGCMD_SIEGE_WARFARE_INFO );
	

	MsgBoxInfo.AddString( _S( 2001, "수성길드" ), SW_COLOR_TITLE, TEXT_CENTER );	// 수성 길드	

	if( _pUISWDoc->m_gdDefGuild.m_nGuildIndex <= 0 )
	{
		strMessage.PrintF(_S( 2002,  "수성 길드가 존재하지 않습니다." ) );	
	}
	else
	{
		strMessage.PrintF(_S( 2146,  "%s 길드" ), _pUISWDoc->m_gdDefGuild.m_strGuildName ); 
	}
	
	MsgBoxInfo.AddString( strMessage, SW_COLOR_TEXT, TEXT_CENTER ); // XXXXXXXXX길드이름
	
	MsgBoxInfo.AddString( CTString(" ") ); // XXXXXXXXX길드이름
	
	MsgBoxInfo.AddString( _S( 2003, "공성 시간" ), SW_COLOR_TITLE, TEXT_CENTER ); // 공성 시간	
	
	strMessage.PrintF(_S( 2004,  "%d월 %d일 %d" ), _pUISWDoc->m_nMonth, _pUISWDoc->m_nDay, _pUISWDoc->m_nHour );	
	MsgBoxInfo.AddString( strMessage, SW_COLOR_TEXT, TEXT_CENTER ); // XXXXXXXXX날짜 
	MsgBoxInfo.AddString( CTString( " " ) );

	pUIManager->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUISiegeWarfare::
// Explain:  시간 설정 
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenSetSWTime()
{
	SetSize( SIGEWARFARE_WIDTH, SIGEWARFARE_HEIGHT );
	ResetPos();

	_pUISWDoc->SetUIState( SWS_SET_SW_TIME );
	SetDayOfWeek( SATURDAY );

	m_lbDesc.ResetAllStrings();

	CTString strTemp, strAmPm;
	int hour;
	m_lbDesc.AddString( 0, _S( 5164, "예약된 공성 시간" ), SW_COLOR_TITLE );	
	
	m_sbtnSWZone.ResetAllDatas();

	CUIManager* pUIManager = CUIManager::getSingleton();

	for( int i=0; i<m_vecSWInfo.size(); ++i )
	{
		strTemp.PrintF(_s("%s(%s)"), CZoneInfo::getSingleton()->GetZoneName( m_vecSWInfo[i].zone ), _pUISWDoc->GetCastleName( m_vecSWInfo[i].zone ) );
		m_sbtnSWZone.AddData( strTemp );

		hour = m_vecSWInfo[i].zone == 4 ? m_vecSWInfo[i].hour - 12 + 2 : m_vecSWInfo[i].hour - 12 + 1;
		if( hour >= 12 )
		{
			if( hour > 12 )
			{
				hour -= 12;
			}
			strAmPm = "AM";
		}
		else
		{
			strAmPm = "PM";
		}

		//added by sam 10/12/20 공성 날짜 글씨가 안나오거나 깨짐
		//-------------------------------------------------------->		
		int iStringIndex = pUIManager->GetWeekToStringIndex( m_vecSWInfo[i].wday  );
		CTString ctDay;
		if ( -1 != iStringIndex)
		{
			ctDay = _S(iStringIndex,"요일");
			strTemp.PrintF( _s("[%s] %s : PM%d:00 ~ %s%d:00"), CZoneInfo::getSingleton()->GetZoneName( m_vecSWInfo[i].zone ), 							
						ctDay, m_vecSWInfo[i].hour - 12, 
						 strAmPm, hour);	// 드라탄 2시간 메라크 1시간
		}
		else
		{
			strTemp.PrintF( _s("[%s] %s : PM%d:00 ~ %s%d:00"), CZoneInfo::getSingleton()->GetZoneName( m_vecSWInfo[i].zone ), 
						pUIManager->GetWeekToString( m_vecSWInfo[i].wday ), m_vecSWInfo[i].hour - 12, 							
						 strAmPm, hour);	// 드라탄 2시간 메라크 1시간
		}
		/*
		strTemp.PrintF( _s("[%s] %s : PM%d:00 ~ %s%d:00"), ZoneInfo().GetZoneName( m_vecSWInfo[i].zone ), 
					pUIManager->GetWeekToString( m_vecSWInfo[i].wday ), m_vecSWInfo[i].hour - 12, 							
					 strAmPm, hour);	// 드라탄 2시간 메라크 1시간
		/**/
		
		//<------------------------------------------------------------------
		m_lbDesc.AddString( 0, strTemp, SW_COLOR_TEXT );
	}

	m_lbDesc.AddString( 0, _S( 2005, "공성가능 시간" ), SW_COLOR_TITLE );	
	m_lbDesc.AddString( 0, _S( 2006, "토요일 : PM 06:00 ~ PM 11:00" ), SW_COLOR_TEXT );	
	m_lbDesc.AddString( 0, _S( 2007 , "일요일 : PM 01:00 ~ PM 11:00" ), SW_COLOR_TEXT );	

	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE, TRUE );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::RenderSetSWTime
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::RenderSetSWTime()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	pDrawPort->InitTextureData( m_ptdBaseTexture );

#define UNIT_SIZE_DEFAULT					40

	// Background 
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, TITLE_HEIGHT, m_rt3BackT ); 

	nY += TITLE_HEIGHT;
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, BACK_MIDDLE_DESC, m_rt3BackSM ); 
	nY += BACK_MIDDLE_DESC;
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, 3, m_rt3BackM ); 
	nY += 3;
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, BACK_MIDDLE_SELECT, m_rt3BackNM ); 
	nY += BACK_MIDDLE_SELECT;

	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, 37 - BACK_BOTTOM_HEIGHT, m_rt3BackM ); 
	nY += 37 - BACK_BOTTOM_HEIGHT;
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, BACK_BOTTOM_HEIGHT, m_rt3BackL ); 

	// 공성일 설정 
	m_cbtnSaturday.Render();
	m_cbtnSunday.Render();

	// 공성 시간 설정 
	// SpinButton render
	m_sbtnSWTime.Render();

	m_sbtnSWZone.Render();

	// Description ListBox : 공성 가능 시간
	m_lbDesc.Render();
	
	// Button
	m_btnOK.Render();
	m_btnCancel.Render();
	m_btnClose.Render();


	pDrawPort->FlushRenderingQueue();

	// Title
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + TITLE_SX,
										m_nPosY + TITLE_SY );
	
	// 공성일
	pDrawPort->PutTextEx( _S(2008 , "공성일" ), m_nPosX + 18, m_nPosY + 35, SW_COLOR_TITLE );	

	// 공성시간
	pDrawPort->PutTextEx( _S( 2009, "공성시간" ), m_nPosX + 18, m_nPosY + 81, SW_COLOR_TITLE );
	
	CTString cstrTiem = _S( 2009, "공성시간");
	int nGap = _pUIFontTexMgr->GetFontWidth() * cstrTiem.Length();
	if ( nGap > 80 )
		pDrawPort->PutTextEx( _S( 5165, "지역" ), m_nPosX + nGap + 18 + 5, m_nPosY + 81, SW_COLOR_TITLE );
	else 
		pDrawPort->PutTextEx( _S( 5165, "지역" ), m_nPosX + 98, m_nPosY + 81, SW_COLOR_TITLE );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::RenderPoint
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::RenderPoint( int nX, int nY, int nPoint )
{
	// 출력될 위치 기준으로 오른쪽 정렬
	int i = 1;
	int nRenderNumber;

	while( nPoint / i > 0  )
	{
		nX -= NUMBER_WIDTH;		
		i *= 10;
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nX -= NUMBER_WIDTH;	
	}
	
	while( i )
	{		
		nRenderNumber = nPoint / i;
		nPoint %= i;

		DrawNumber( nX, nY, nRenderNumber );

		printf ( "%d", nRenderNumber );
		nX += NUMBER_WIDTH;	
		i /= 10;
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::RenderBattle
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::RenderBattle()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + BATTLE_WIDTH, m_nPosY + BATTLE_HEIGHT,
										m_rtBattleBack.U0, m_rtBattleBack.V0, m_rtBattleBack.U1, m_rtBattleBack.V1,
										0xFFFFFFFF );

	if( _pUISWDoc->IsSelectBattle() )
	{
		int nOffsetX, nOffsetY;
		nOffsetX = m_nPosX;
		nOffsetY = m_nPosY + INFO_OFFSETY;
	
		pDrawPort->AddTexture( nOffsetX, nOffsetY, nOffsetX + INFO_WIDTH, nOffsetY + INFO_HEIGHT,
										m_rtSelectBattleBack.U0, m_rtSelectBattleBack.V0, m_rtSelectBattleBack.U1, m_rtSelectBattleBack.V1,
										0xFFFFFFFF );
	
		pDrawPort->PutTextEx( _S( 2010, "길드포인트" ), nOffsetX + UNIT_TEXT_OFFSETX,	
											nOffsetY + ( UNIT_HEIGHT * 0 ) 
													+ UNIT_TEXT_OFFSETY, SW_COLOR_DEFENSE_POINT );	
		// Render Number
		RenderPoint(	nOffsetX + UNIT_POINT_OFFSETX, 
						nOffsetY + ( UNIT_HEIGHT * 0 + UNIT_POINT_OFFSETY ),
						_pUISWDoc->m_nGuildPoint );

		pDrawPort->PutTextEx( _S( 2011, "수성측포인트" ), nOffsetX + UNIT_TEXT_OFFSETX,	
											nOffsetY + ( UNIT_HEIGHT * 1 ) 
													+ UNIT_TEXT_OFFSETY, SW_COLOR_DEFENSE_POINT );	
		// Render Number
		RenderPoint(	nOffsetX + UNIT_POINT_OFFSETX, 
						nOffsetY + ( UNIT_HEIGHT * 1 + UNIT_POINT_OFFSETY ),
						_pUISWDoc->m_gdDefGuild.m_nPoint );

		
		pDrawPort->FlushRenderingQueue();
	}
	
	RenderLeftTime();

	for( int i = 0; i < 3; i++)
	{
		
		pDrawPort->PutTextEx( _pUISWDoc->m_gdGuild[i].m_strGuildName, m_nPosX + UNIT_TEXT_OFFSETX,
											m_nPosY + UNIT_TIME_HEIGHT + ( UNIT_HEIGHT * i ) 
													+ UNIT_TEXT_OFFSETY, SW_COLOR_GUILD_NAME );	
		// Render Number
		RenderPoint(	m_nPosX + UNIT_POINT_OFFSETX, 
						m_nPosY + UNIT_TIME_HEIGHT + ( UNIT_HEIGHT * i + UNIT_POINT_OFFSETY ),
						_pUISWDoc->m_gdGuild[i].m_nPoint );
	}

	pDrawPort->FlushRenderingQueue();
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}


//------------------------------------------------------------------------------
// CUISiegeWarfare::DrawNumber
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::DrawNumber( int nX, int nY, int nNumber, int nWidth, int nHeight )
{
	UIRectUV rtNumber = m_rtNumber[nNumber];

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->AddTexture( nX, nY, nX + nWidth, nY + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );

}

//------------------------------------------------------------------------------
// CUIMessenger::RenderRectUV3
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 )
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
// CUISiegeWarfare::CloseAllMsgBox
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::CloseAllMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_SIEGE_WARFARE_ERROR);
	pUIManager->CloseMessageBox(MSGCMD_SIEGE_WARFARE_INFO);
}

//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorMessage
// Explain:  
// Date : 2005-03-19(오후 12:28:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::ErrorMessage( CTString strErrorMessage )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_SIEGE_WARFARE_ERROR);

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_SIEGE_WARFARE, MSGCMD_SIEGE_WARFARE_ERROR );
	MsgBoxInfo.AddString( strErrorMessage );	
	pUIManager->CreateMessageBox( MsgBoxInfo );		
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBMessage
// Explain:  
// Date : 2005-03-21(오후 4:46:32) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::Message( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_SIEGE_WARFARE, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::SetDayOfWeek
// Explain:  
// Date : 2005-06-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::SetDayOfWeek( int nDayOfWeek )
{
	int nTimePrev, nTimeNext;

	if( nDayOfWeek == SATURDAY )
	{
		m_cbtnSaturday.SetCheck( TRUE );
		m_cbtnSunday.SetCheck( FALSE );

		m_sbtnSWTime.ResetAllDatas();

		nTimePrev = 6; nTimeNext = 11;
		nTimePrev = 5;

#if defined G_USA
		nTimePrev = 10;
		nTimeNext = 23;
#endif // G_USA

		for( int i = nTimePrev; i <= nTimeNext; i++ ) //!시간 조정
		{
			CTString strTime;
			strTime.PrintF( "%d", i );
			m_sbtnSWTime.AddData( strTime );
		}

	}
	else if( nDayOfWeek == SUNDAY )
	{
		m_cbtnSaturday.SetCheck( FALSE );
		m_cbtnSunday.SetCheck( TRUE );

		m_sbtnSWTime.ResetAllDatas();

		nTimePrev = 1; nTimeNext = 11;

#if defined G_USA
		nTimePrev = 11;
		nTimeNext = 20;
#endif

		for( int i = nTimePrev; i <= nTimeNext; i++ )//!시간
		{
			CTString strTime;
			strTime.PrintF( "%d", i );
			m_sbtnSWTime.AddData( strTime );
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::MouseMessage
// Explain:  
// Date : 2005-06-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUISiegeWarfare::MouseMessage( MSG *pMsg )
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
			//if( IsInside( nX, nY ) )

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
				return WMSG_SUCCESS;
			}

			// 길드 신청 원도우 
			if( _pUISWDoc->IsSetSWTime() ) // 시간 설정 윈도우 
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_sbtnSWTime.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_sbtnSWZone.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( _pUISWDoc->IsSetSWTime() ) // 시간 설정 윈도우 
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;
				
					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}

					// OK Button
					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_cbtnSaturday.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SetDayOfWeek( SATURDAY );
						return WMSG_SUCCESS;
					}
					else if( m_cbtnSunday.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						SetDayOfWeek( SUNDAY );
						return WMSG_SUCCESS;
					}
					else if( m_sbtnSWTime.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if( m_sbtnSWZone.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
		
			if( _pUISWDoc->IsSetSWTime() ) // 시간 설정 윈도우 
			{				
				if( IsInside( nX, nY ) )
				{
					// OK Button
					if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							int nDay = -1;
							int nHour = 0;

							// 날짜얻기
							if( m_cbtnSaturday.IsChecked() )		nDay = SATURDAY;
							else if ( m_cbtnSunday.IsChecked() )	nDay = SUNDAY;
							// 시간 얻기
							
#if defined G_USA
							nHour = m_sbtnSWTime.GetDataToNumber();
#else
							nHour = m_sbtnSWTime.GetDataToNumber() + 12;
#endif
			
							int zone = m_vecSWInfo[m_sbtnSWZone.GetSelectPos()].zone;
							_pNetwork->SetTimeReq( nDay, nHour, zone);

						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CloseSiegeWarfare();
							OpenSiegeWarfare();
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CloseSiegeWarfare();
							OpenSiegeWarfare();
						}
						return WMSG_SUCCESS;
					}
					else if( m_sbtnSWTime.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if( m_sbtnSWZone.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					return WMSG_SUCCESS;
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( _pUISWDoc->IsSetSWTime() ) // 시간 설정 윈도우 
			{
				if( IsInside( nX, nY ) )
				{ // 버튼 클릭이 잘되지 않는 문제로 더블 클릭은 처리하지 않는다.
//					if( m_sbtnSWTime.MouseMessage( pMsg ) != WMSG_FAIL )
//							return WMSG_SUCCESS;

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( _pUISWDoc->IsSetSWTime() ) // 시간 설정 윈도우 
			{
				if( IsInside( nX, nY ) )
				{
					if( m_sbtnSWTime.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_sbtnSWZone.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					
					return WMSG_SUCCESS;		
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::SetNotice
// Explain:  
// Date : 2005-03-21(오후 9:53:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfare::SetNotice( CTString strNoticeMessage, CTString strNoticeMessage2 )
{
	CloseAllMsgBox();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_SIEGE_WARFARE, TRUE );

	// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
	// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
#ifdef	IMPROV1107_NOTICESYSTEM
	_UIAutoHelp->SetGMNotice( strNoticeMessage );
	
	if( strNoticeMessage2.Length() > 0 )
	{
		_UIAutoHelp->SetGMNotice( strNoticeMessage2 );
	}
#else
	m_bShowNotice = TRUE;
	m_bBottomNotice = TRUE;

	pUIManager->GetChattingUI()->AddSysMessage( strNoticeMessage, SYSMSG_NORMAL );	

	if( strNoticeMessage2.Length() <= 1 )
	{
		m_bBottomNotice = FALSE;
	}
	else
	{
		pUIManager->GetChattingUI()->AddSysMessage( strNoticeMessage2, SYSMSG_NORMAL );	
	}

	int nWidth = 0;
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	int nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	m_strNoticeMessage = strNoticeMessage;
	m_strNoticeMessage2 = strNoticeMessage2;

	// 위쪽에 출력 될 공지
	nWidth = ( m_strNoticeMessage.Length() ) * 
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
#endif
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::OpenCostumeReq
// Explain: 성주 코스튬 지급 요청
// Date : 2010-09-15
//------------------------------------------------------------------------------
void CUISiegeWarfare::OpenCostumeReq()
{
	CTString strTitle = _S( 5115, "성주 코스튬" );
	CTString strMessage;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CreateMessageBoxL( strTitle, UI_SIEGE_WARFARE, MSGLCMD_LORD_COSTUME_REQ );
	
	pUIManager->AddMessageBoxLString( MSGLCMD_LORD_COSTUME_REQ, TRUE, _S( 1748, "안내" ) );		
	
	strMessage =_S( 5116, "안녕하세요. 성주님. 수많은 시련과 고난 속에 경쟁 길드와의 결투에서 승리하여 성주가 되신 것을 축하 드립니다. 이젠 이 복장을 입고 성의 주인임을 세상에 알리시길 바랍니다." );	
	pUIManager->AddMessageBoxLString( MSGLCMD_LORD_COSTUME_REQ, TRUE, strMessage, -1, SW_COLOR_DESC );
	pUIManager->AddMessageBoxLString( MSGLCMD_LORD_COSTUME_REQ, TRUE, CTString(" "), -1, SW_COLOR_DESC );
	
	strMessage = _S( 1839, "아이템 지급" );
	pUIManager->AddMessageBoxLString( MSGLCMD_LORD_COSTUME_REQ, FALSE, strMessage, 0 );
}

//------------------------------------------------------------------------------
// CUISiegeWarfare::SetSiegeWarfareTimeInfo
// Explain: 공성 시간 정보 저장
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfare::SetSiegeWarfareTimeInfo( stSiegeWarfareTimeInfo swInfo )
{
	m_vecSWInfo.push_back( swInfo );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::CUISiegeWarfareInfo
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
CUISiegeWarfareInfo::CUISiegeWarfareInfo()
{

}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::~CUISiegeWarfareInfo
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
CUISiegeWarfareInfo::~CUISiegeWarfareInfo()
{

}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::Create
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_Interface.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_bxBackground[0].SetBoxUV(m_ptdBaseTexture, 120, 36, WRect(0,98,272,253));
	m_bxBackground[0].SetBoxPos(WRect(0,0,nWidth,nHeight));

	m_bxBackground[1].SetBoxUV(m_ptdBaseTexture, 20, WRect(0,304,221,346));
	m_bxBackground[1].SetBoxPos(WRect(20, 80, 380, 175));

	m_bxBackground[2].SetBoxUV(m_ptdBaseTexture, 20, WRect(0,304,221,346));
	m_bxBackground[2].SetBoxPos(WRect(20, 200, 380, 295));

	m_bxBackground[3].SetBoxUV(m_ptdBaseTexture, 3, WRect(339,464,368,466));
	m_bxBackground[3].SetBoxPos(WRect(20, 50, 380, 53));

	m_btnOK.Create(this, CTString(_S(191, "확인")), 150, 310, 100, 20);
	m_btnOK.SetUV(UBS_IDLE,282,244,351,265, fTexWidth, fTexHeight );
	m_btnOK.SetUV(UBS_CLICK, 364,244,433,265, fTexWidth, fTexHeight);
	m_btnOK.CopyUV(UBS_IDLE, UBS_ON);
	m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_rsMyGuildName.Create(this, 28, 248, 85, 25 );
	m_rsMyGuildName.SetAlign( ALIGN_CENTER );

	m_rsMyGuildBossName.Create(this, 113, 248, 85, 25 );
	m_rsMyGuildBossName.SetAlign( ALIGN_CENTER );

	m_rcTitle.SetRect(0, 0, nWidth, 30);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::Render
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::Render()
{
	CTString strTemp;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	for(int i =0; i < 4; i++)
	{
		m_bxBackground[i].Render(m_nPosX,m_nPosY);
	}

	m_btnOK.Render();

	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextExCX(_S( 5166, "금주 공성 일정"), m_nPosX+200, m_nPosY+18);

	pDrawPort->PutTextEx(_S( 5167, "수성 정보"), m_nPosX + 25, m_nPosY + 60);
	pDrawPort->PutTextEx(_S( 5168, "나의 정보"), m_nPosX + 25, m_nPosY + 180);
	

	pDrawPort->PutTextExCX( _S( 2001, "수성길드"), m_nPosX + 100, m_nPosY + 85 );
	pDrawPort->PutTextExCX( _S( 5165, "지역"), m_nPosX + 230, m_nPosY + 85 );
	pDrawPort->PutTextExCX( _S( 1718, "시간"), m_nPosX + 330, m_nPosY + 85 );

	if( m_vecDefGuildInfo.size() == 0 )
	{
		pDrawPort->PutTextExCX( _S( 2002,  "수성 길드가 존재하지 않습니다." ), m_nPosX + 200, m_nPosY + 124);
	}
	else
	{
		for( int i=0; i<m_vecDefGuildInfo.size(); ++i )
		{
			pDrawPort->PutTextExCX( m_vecDefGuildInfo[i].guildName, m_nPosX + 100, m_nPosY + 110+(i*28));
			pDrawPort->PutTextExCX( CZoneInfo::getSingleton()->GetZoneName( m_vecDefGuildInfo[i].zone ), m_nPosX + 230, m_nPosY + 110+(i*28));

			strTemp.PrintF(_S( 2004,  "%d월 %d일 %d" ), m_vecDefGuildInfo[i].month, m_vecDefGuildInfo[i].day, m_vecDefGuildInfo[i].hour );
			pDrawPort->PutTextExCX( strTemp, m_nPosX + 330, m_nPosY + 110+(i*28));
		}
	}

	pDrawPort->PutTextExCX( _S( 3451, "길드 명"), m_nPosX + 70, m_nPosY + 205 );
	pDrawPort->PutTextExCX( _S( 4344, "길드 마스터"), m_nPosX + 155, m_nPosY + 205 );
	pDrawPort->PutTextExCX( _S( 5169, "공성 관계"), m_nPosX + 255, m_nPosY + 205 );
	pDrawPort->PutTextExCX( _S( 5165, "지역"), m_nPosX + 335, m_nPosY + 205 );

	if( m_MySiegeWarfareInfo.vecZone.size() > 0 )
	{
		m_rsMyGuildName.Render();
		m_rsMyGuildBossName.Render();

		for( int j=0; j<m_MySiegeWarfareInfo.vecZone.size(); ++j )
		{	
			pDrawPort->PutTextExCX( GetJoinStatusString( m_MySiegeWarfareInfo.vecJoinFlag[j] ), m_nPosX + 255, m_nPosY + 248 + (j*14) );
			pDrawPort->PutTextExCX( CZoneInfo::getSingleton()->GetZoneName( m_MySiegeWarfareInfo.vecZone[j] ), m_nPosX + 335, m_nPosY + 248 + (j*14) );	
		} 
	}
	else
	{
		pDrawPort->PutTextExCX( _S( 5170, "공성 참여 정보가 존재하지 않습니다." ), m_nPosX + 200, m_nPosY + 248);
	}

	pDrawPort->EndTextEx();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::ResetPosition
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::AdjustPosition
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::MouseMessage
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
WMSG_RESULT CUISiegeWarfareInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;
					
					Move( ndX, ndY );
					
					return WMSG_SUCCESS;
				}

				m_btnOK.MouseMessage( pMsg );
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}

					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}

					CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_INFO, TRUE );
					return WMSG_SUCCESS;
				}

			}
			break;

		case WM_LBUTTONUP:
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				bTitleBarClick = FALSE;

				if (pUIManager->GetDragIcon() == NULL)
				{
					if( !IsFocused() )
						return WMSG_FAIL;

					if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							ToggleSiegeWarfareInfo( FALSE );
						}

						return WMSG_SUCCESS;
					}
				}
				else
				{
					pUIManager->ResetHoldBtn();
				}

			}
			break;
	}

	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::InitSiegeWarfareInfo
// Explain: 
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::InitSiegeWarfareInfo()
{
	m_MySiegeWarfareInfo.Init();
	m_vecDefGuildInfo.clear();

	m_rsMyGuildName.ClearString();
	m_rsMyGuildBossName.ClearString();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::SetSiegeWarfareDefGuildInfo
// Explain: 수성길드 정보 저장
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::SetSiegeWarfareDefGuildInfo( stSiegeWarfareDefGuildInfo defGuildInfo )
{
	m_vecDefGuildInfo.push_back(defGuildInfo);
} 

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::SetMySiegeWarfareGuildInfo
// Explain: 내 길드정보 저장
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::SetMySiegeWarfareGuildInfo( CTString guildName, CTString guildBossName )
{

	m_MySiegeWarfareInfo.guildName = guildName;
	m_MySiegeWarfareInfo.guildBossName = guildBossName;

	m_rsMyGuildName.AddString( guildName );
	m_rsMyGuildBossName.AddString( guildBossName );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::SetMySiegeWarfareJoinInfo
// Explain: 내 공성정보 저장
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::SetMySiegeWarfareJoinInfo( char zone, char joinFlag )
{
	m_MySiegeWarfareInfo.vecZone.push_back( zone );
	m_MySiegeWarfareInfo.vecJoinFlag.push_back( joinFlag );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::ToggleSiegeWarfareInfo
// Explain: 공성정보창 열고닫기
// Date : 2010-11-01
//------------------------------------------------------------------------------
void CUISiegeWarfareInfo::ToggleSiegeWarfareInfo( BOOL bShown )
{
	if( !bShown )
	{
		InitSiegeWarfareInfo();
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_SIEGE_WARFARE_INFO, bShown );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareInfo::GetJoinStatusString
// Explain: 공성참여 정보 스트링
// Date : 2010-11-01
//------------------------------------------------------------------------------
CTString CUISiegeWarfareInfo::GetJoinStatusString( char joinFlag )
{
	switch( joinFlag )
	{
		case WCJF_NONE:
			return "";
		case WCJF_OWNER:
			return _S( 5171, "성주길드");
		case WCJF_DEFENSE_GUILD:
			return _S( 2001, "수성길드" );
		case WCJF_DEFENSE_CHAR:
			return _S( 5172, "수성측용병");
		case WCJF_ATTACK_GUILD:
			return _S( 5173, "공성길드");
		case WCJF_ATTACK_CHAR:
			return _S( 5174, "공성측용병");
	}
	
	return "";
}

