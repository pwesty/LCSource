#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/GameState.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UiSelChar.h>
#include <Engine/Ska/Render.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/World/WorldRayCasting.h>
//wooss 051017
#include <Engine/Base/Input.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Interface/UIOption.h>

extern BOOL g_bAutoLogin;
extern BOOL g_bAutoRestart;
#define CHECK_NUM 7
#define MIN_NAME_LEN	(4)
#define MAX_NAME_LEN	(16)
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")
#define CAMERA_TURN_RIGHT	(43819)
#define CAMERA_TURN_LEFT	(43818)
//#define EXSLOT // 슬롯 확장

typedef enum __tag_char_move_stat
{
	CMS_NORMAL = 1,
	CMS_MOVE_WAIT,
	CMS_MOVE_ROLLBACK,
} CMSTAT;

// ----------------------------------------------------------------------------
// Name : CUISelChar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISelChar::CUISelChar() : 
m_iDestPosID(-1),
m_pWorld(NULL),
m_bIsShowMessageInfo(FALSE),
m_bIsShowCharMoveMsgInfo(FALSE),
m_bIsLeftView(TRUE),
m_ulCharSlotTime(0),
m_CharSlotTimeOld(0)
{	
	memset(&m_aEGslotPosID[0], NULL, sizeof(m_aEGslotPosID));
	m_strServerMoveFailCharName = CTString("");
	m_ptdMsgTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUISelChar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelChar::~CUISelChar()
{
	Destroy();

	STOCK_RELEASE(m_ptdMsgTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcHP.SetRect( 0, 25, 0, 33 );
	m_rcMP.SetRect( 0, 39, 0, 47 );
	m_rcEXP.SetRect( 0, 53, 0, 61 );

	// Create select character
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\SelectCharacter.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtCharInfo.SetUV( 74, 0, 256, 68, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 2, 45, 4, 52, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 7, 45, 9, 52, fTexWidth, fTexHeight );
	m_rtEXP.SetUV( 12, 45, 14, 52, fTexWidth, fTexHeight );
	m_rtExSlot.SetUV( 93, 3, 106, 16,fTexWidth,fTexHeight);			//wooss 050819
	m_rtExSlotLine.SetUV( 74, 66, 255, 67,fTexWidth,fTexHeight);	//wooss 050819
	m_rtExSlotLineMidV.SetUV( 74, 63, 255, 65,fTexWidth,fTexHeight);//wooss 050819
//	m_rtExSlotLineMidH.SetUV( 93, 3, 106, 17,fTexWidth,fTexHeight);	//wooss 050819
//	m_rtExSlotLineBtm.SetUV( 93, 3, 106, 17,fTexWidth,fTexHeight);	//wooss 050819


	// OK Button(Connect)
	m_btnOK.Create( this, _S( 239, "게임 시작" ), 0, 0, 100, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// Create Character Button
	m_btnCreateChar.Create( this, _S( 140, "캐릭터 생성" ), 120, 0, 100, 21 );
	m_btnCreateChar.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnCreateChar.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnCreateChar.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCreateChar.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Delete Character Button
	m_btnDeleteChar.Create( this, _S( 2902, "삭제/취소" ), 240, 0, 100, 21 );
	m_btnDeleteChar.SetUV( UBS_IDLE, 0, 107, 100, 128, fTexWidth, fTexHeight );
	m_btnDeleteChar.SetUV( UBS_CLICK, 102, 107, 202, 128, fTexWidth, fTexHeight );
	m_btnDeleteChar.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDeleteChar.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Options Button
	m_btnOptions.Create( this, _S( 196, "옵션" ), 360, 0, 63, 21 );
	m_btnOptions.SetUV( UBS_IDLE, 0, 0, 63, 21, fTexWidth, fTexHeight );
	m_btnOptions.SetUV( UBS_CLICK, 0, 23, 63, 44, fTexWidth, fTexHeight );
	m_btnOptions.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOptions.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Exit Button
	m_btnExit.Create( this, _S( 181, "나가기" ),  440, 0, 63, 21 );
	m_btnExit.SetUV( UBS_IDLE, 0, 0, 63, 21, fTexWidth, fTexHeight );
	m_btnExit.SetUV( UBS_CLICK, 0, 23, 63, 44, fTexWidth, fTexHeight );
	m_btnExit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExit.CopyUV( UBS_IDLE, UBS_DISABLE );
	// hongkong used auto login. [8/26/2010 rumist]
	// [2012/10/10 : Sora] 빌라 자동로그인 추가로 버튼 비활성화
#if defined G_HONGKONG //|| defined (G_KOR)
	m_btnExit.SetEnable(FALSE);
#endif

	m_btnTurnR.Create( NULL, CTString( "" ), 0, 0, 100, 32 );
	m_btnTurnR.SetUV( UBS_IDLE, 4, 191, 110, 254, fTexWidth, fTexHeight );
	m_btnTurnR.SetUV( UBS_CLICK, 120, 191, 226, 254, fTexWidth, fTexHeight );
	m_btnTurnR.CopyUV( UBS_IDLE, UBS_ON );
	m_btnTurnR.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnTurnL.Create( NULL, CTString( "" ), 0, 0, 100, 32 );
	m_btnTurnL.SetUV( UBS_IDLE, 4, 129, 110, 192, fTexWidth, fTexHeight );
	m_btnTurnL.SetUV( UBS_CLICK, 120, 129, 226, 192, fTexWidth, fTexHeight );
	m_btnTurnL.CopyUV( UBS_IDLE, UBS_ON );
	m_btnTurnL.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Delete Time
	//RECT
	m_rcDelTime.SetRect( 0, 0, 181, 37 );
	// UV
	m_rtDelTime.SetUV( 74, 68, 255, 105, fTexWidth, fTexHeight );

	// set random seed 
	srand((unsigned int)time((time_t *)NULL));

	// init slot ani
	for(int i=0;i<MAX_SLOT;i++)
	{
		m_aSlotAni[i].m_idAni = ANIM_IDLE;
		m_aSlotAni[i].m_startTime = -1;
	}
	
 	m_ptdMsgTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_rcMessageInfo.SetRect( 20, 40, 215, 122 );
 	m_bxNoticeMsg.SetBoxUV(m_ptdMsgTexture,7,WRect(239,253,335,269));

	m_msgNCText.SetRenderRect( 20, 45, 187, 62 );
	m_msgNCText.AddString( _S(4794, "나이트쉐도우 캐릭터를 생성할 수 있는 조건을 만족하였습니다. 나이트쉐도우 캐릭터를 생성하여 육성하십시오." ) );
	
	// alert notice that enabled character move to other servers. [7/19/2012 rumist]
	m_rcCharMoveMsgInfo.SetRect(20, 40, 215, 122);
	m_bxCharMoveNoticeMsg.SetBoxUV(m_ptdMsgTexture, 7, WRect(239, 253, 335, 269));
	m_msgCMText.SetRenderRect(20, 45, 187, 62);
	m_msgCMText.AddString( _s("your some character waiting time that move to another server." ) );

 	m_bxCharSlotHelp.SetBoxUV(m_ptdMsgTexture,7,WRect(239,253,335,269));

	m_msgCharSlotHelp.SetRenderRect( 20, 135, 187, 72 );
	m_msgCharSlotHelp.AddString( _S( 5702, "총 8개까지의 캐릭터 생성이 가능 합니다. 기간이 만료 된 이후에는 이미 생성된 캐릭터의 플레이는 가능 하지만 캐릭터의 생성은 4개까지로 제한 됩니다." ) );

	// [2012/07/27 : Sora] 캐릭터 슬롯 확장 텍스트 길이에 맞추에서 박스 크기를 늘리도록 수정
	m_rcCharSlotHelp.SetRect( 20, 130, 215, 130 + m_msgCharSlotHelp.GetRenderHeight() + 7 );

	m_rcCharSlotRemain.SetRect( 0, 0, 300, 36 );
 	m_bxCharSlotRemain.SetBoxUV(m_ptdMsgTexture,7,WRect(239,253,335,269));
	m_strCharServerMove = _S(5715, "서버 이전 대기중");
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	m_btnBack.Create( NULL, _S( 2681, "이전" ),  440, 0, 63, 21 );
	m_btnBack.SetUV( UBS_IDLE, 0, 0, 63, 21, fTexWidth, fTexHeight );
	m_btnBack.SetUV( UBS_CLICK, 0, 23, 63, 44, fTexWidth, fTexHeight );
	m_btnBack.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBack.CopyUV( UBS_IDLE, UBS_DISABLE );
#endif
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - 520, pixMaxJ - 38 );

	// Region of character information
	m_rcCharInfo.SetRect( pixMaxI - 182, 0, pixMaxI, 68 );
	m_rcExSlot.SetRect(pixMaxI - 178, 70, pixMaxI-3, 120);		//wooss 050819
	m_rcExSlotLineTop.SetRect(pixMaxI - 182,68,pixMaxI,69);
	m_rcExSlotLineMidV.SetRect(pixMaxI - 182,69,pixMaxI,124);		
	m_rcExSlotLineMidH.SetRect(pixMaxI - 182,85,pixMaxI,86);		
	m_rcExSlotLineBtm.SetRect(pixMaxI - 182,124,pixMaxI,125);		

//	m_rcDelTime.SetRect(pixMaxI - 182,68,pixMaxI,115);		

	m_btnTurnL.SetPos(pixMaxI / 2 - 100, pixMaxJ - 80);
	m_btnTurnR.SetPos(pixMaxI / 2, pixMaxJ - 80);

	m_rcHP.Left = m_rcMP.Left = m_rcEXP.Left = pixMaxI - 171;
	UpdateStatus();

	m_rcCharSlotRemain.SetRect( pixMaxI / 2 - (m_rcCharSlotRemain.GetWidth()/2), 30,
								pixMaxI / 2 + (m_rcCharSlotRemain.GetWidth()/2), 30 + m_rcCharSlotRemain.GetHeight() );

#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
	m_btnBack.SetPos(pixMinI + 20, pixMaxJ - 38);
#endif								
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//-----------------------------------------------------------------------------
// Purpose: Reset
//-----------------------------------------------------------------------------
void CUISelChar::Reset()
{
	m_bIsLeftView = TRUE;
}

// ----------------------------------------------------------------------------
// Name : UpdateStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUISelChar::UpdateStatus()
{
	INDEX			iSlot = _pGameState->SelectedSlot() - 1;
	if(iSlot < 0 || iSlot >= MAX_SLOT) return;
	CGameState::SLOT_INFO	&CurrentSlot = _pGameState->m_SlotInfo[iSlot];

	FLOAT	fHPRatio = CurrentSlot.hp / (FLOAT)(CurrentSlot.maxHP );
	if( fHPRatio > 1.0f ) fHPRatio = 1.0f;
	FLOAT	fMPRatio = CurrentSlot.mp / (FLOAT)( CurrentSlot.maxMP );
	if( fMPRatio > 1.0f ) fMPRatio = 1.0f;
	FLOAT	fEXPRatio = CurrentSlot.curExp / (FLOAT)( CurrentSlot.needExp );
	FLOAT	fEXPBarRatio = fEXPRatio;
	if( fEXPBarRatio > 1.0f ) fEXPBarRatio = 1.0f;
	else if( fEXPBarRatio < 0.0f ) fEXPBarRatio = 0.0f;
	FLOAT	fSPRatio = ( CurrentSlot.sp % 10000 ) / 10000.0f;

	m_rcHP.Right = m_rcHP.Left + SELCHAR_BAR_WIDTH * fHPRatio;
	m_rcMP.Right = m_rcMP.Left + SELCHAR_BAR_WIDTH * fMPRatio;
	m_rcEXP.Right = m_rcEXP.Left + SELCHAR_BAR_WIDTH * fEXPBarRatio;

	m_strName.PrintF( "%s", CurrentSlot.name );
	m_strLevel.PrintF( "%d", CurrentSlot.level );
	m_strHP.PrintF( "%d/%d", CurrentSlot.hp, CurrentSlot.maxHP );
	m_strMP.PrintF( "%d/%d", CurrentSlot.mp, CurrentSlot.maxMP );
	m_strEXP.PrintF( "%.2f%%", fEXPRatio * 100.0f );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
#define TEXCOLOR_W  (0xFFFFFFFF)
#define TEXCOLOR_B	(0x484848FF)
void CUISelChar::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if(m_pWorld)
	{
		// Set select character texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		
		// Character information
		// Background
		pDrawPort->AddTexture( m_rcCharInfo.Left, m_rcCharInfo.Top, m_rcCharInfo.Right, m_rcCharInfo.Bottom,
			m_rtCharInfo.U0, m_rtCharInfo.V0, m_rtCharInfo.U1, m_rtCharInfo.V1,
			0xFFFFFFFF );
#ifdef EXSLOT
		//wooss 050819
		// Extend Slot
		// -------------확장 슬롯 (유료아이템)------------------------------->>
		// ------------------------------------------------------------------>>
		pDrawPort->AddTexture( m_rcExSlot.Left, m_rcExSlot.Top, m_rcExSlot.Right, m_rcExSlot.Bottom,
			m_rtExSlot.U0, m_rtExSlot.V0, m_rtExSlot.U1, m_rtExSlot.V1,
			0xFFFFFFFF );
		// Slot mid Vertical line
		pDrawPort->AddTexture( m_rcExSlotLineMidV.Left, m_rcExSlotLineMidV.Top, m_rcExSlotLineMidV.Right, m_rcExSlotLineMidV.Bottom,
			m_rtExSlotLineMidV.U0, m_rtExSlotLineMidV.V0, m_rtExSlotLineMidV.U1, m_rtExSlotLineMidV.V1,
			0xFFFFFFFF );
		// Slot top line
		pDrawPort->AddTexture( m_rcExSlotLineTop.Left, m_rcExSlotLineTop.Top, m_rcExSlotLineTop.Right, m_rcExSlotLineTop.Bottom,
			m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
			0xFFFFFFFF );
		// slot middle line
		pDrawPort->AddTexture( m_rcExSlotLineMidH.Left, m_rcExSlotLineMidH.Top, m_rcExSlotLineMidH.Right, m_rcExSlotLineMidH.Bottom,
			m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
			0xFFFFFFFF );
		// slot bottom line
		pDrawPort->AddTexture( m_rcExSlotLineBtm.Left, m_rcExSlotLineBtm.Top, m_rcExSlotLineBtm.Right, m_rcExSlotLineBtm.Bottom,
			m_rtExSlotLine.U0, m_rtExSlotLine.V0, m_rtExSlotLine.U1, m_rtExSlotLine.V1,
			0xFFFFFFFF );
		// ------------------------------------------------------------------<<
#endif
		
		// 이동 wooss 050819
		// Text in Character information
		int	nSX = m_rcCharInfo.Left;
		int	nSY = m_rcCharInfo.Top;
		
#ifdef EXSLOT		
		// -------------확장 슬롯 (유료아이템)------------------------------->>
		// ------------------------------------------------------------------>>
		CTString slotStr1,slotStr2;
		slotStr1.PrintF(_S( 2114, "확장슬롯 1 : %5d" ),m_exSlot1);   
		slotStr2.PrintF(_S( 2115, "확장슬롯 2 : %5d" ),m_exSlot2);
		
		pDrawPort->PutTextExCX( CTString("확장슬롯 남은 기간"), nSX + SELCHAR_EX_SLOT_X1,nSY + SELCHAR_EX_SLOT_Y1 );
		pDrawPort->PutTextExCX( slotStr1, nSX + SELCHAR_EX_SLOT_X2, nSY + SELCHAR_EX_SLOT_Y2 );
		pDrawPort->PutTextExCX( slotStr2, nSX + SELCHAR_EX_SLOT_X2, nSY + SELCHAR_EX_SLOT_Y3 );
		// ------------------------------------------------------------------<<
#endif
		if( _pGameState->SelectedSlot() > 0 )
		{
			// HP
			pDrawPort->AddTexture( m_rcHP.Left, m_rcHP.Top, m_rcHP.Right, m_rcHP.Bottom,
				m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
				0xFFFFFFFF );
			// MP
			pDrawPort->AddTexture( m_rcMP.Left, m_rcMP.Top, m_rcMP.Right, m_rcMP.Bottom,
				m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
				0xFFFFFFFF );
			// EXP
			pDrawPort->AddTexture( m_rcEXP.Left, m_rcEXP.Top, m_rcEXP.Right, m_rcEXP.Bottom,
				m_rtEXP.U0, m_rtEXP.V0, m_rtEXP.U1, m_rtEXP.V1,
				0xFFFFFFFF );
			
			// Text in Character information
			
			pDrawPort->PutTextExCX( m_strLevel, nSX + SELCHAR_LEVEL_CX,
				nSY + SELCHAR_NAME_SY, 0xFFD3A7FF );
			pDrawPort->PutTextEx( m_strName, nSX + SELCHAR_NAME_SX, nSY + SELCHAR_NAME_SY );
#if defined G_RUSSIA
			pDrawPort->PutTextEx( _S( 4411, "HP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_HP_SY );
			pDrawPort->PutTextEx( _S( 4412, "MP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_MP_SY );
			pDrawPort->PutTextEx( _S( 4413, "EXP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_EXP_SY );
#else
			pDrawPort->PutTextEx( CTString( "HP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_HP_SY );
			pDrawPort->PutTextEx( CTString( "MP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_MP_SY );
			pDrawPort->PutTextEx( CTString( "EXP" ), nSX + SELCHAR_HP_SX,
				nSY + SELCHAR_EXP_SY );
#endif
			pDrawPort->PutTextExCX( m_strHP, nSX + SELCHAR_HP_CX,
				nSY + SELCHAR_HP_SY );
			pDrawPort->PutTextExCX( m_strMP, nSX + SELCHAR_HP_CX,
				nSY + SELCHAR_MP_SY );
			pDrawPort->PutTextExCX( m_strEXP, nSX + SELCHAR_HP_CX,
				nSY + SELCHAR_EXP_SY );


			
			SLONG tv_time = _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time;
			if( tv_time  > 0 ) {
				CTString str_time,str_time2;
				//	time_t tv_time2;
				int tv_hour,tv_min;
				//	time(&tv_time2); 
				//	tv_time -= tv_time2;
				tv_hour = tv_time / (60*60);
				tv_min = (tv_time %(60*60)) /60;
		
				str_time.PrintF(_S( 2903, "삭제대기 남은시간"));
				pDrawPort->PutTextEx(str_time, 37,	3 );
				
				str_time.Clear();
				if(tv_hour >0 )	str_time.PrintF(_S( 2512, "%d시간"),tv_hour);
				if(tv_min > 0 ) {
					str_time2.PrintF(_S( 2513, "%d분"),tv_min);
					str_time += str_time2;
				}
				int tv_posX = (m_rcDelTime.Right - m_rcDelTime.Left)/2;
				pDrawPort->PutTextExCX(str_time, tv_posX , 21 ,0xFFAA44FF);
									
				pDrawPort->AddTexture( m_rcDelTime.Left, m_rcDelTime.Top, m_rcDelTime.Right, m_rcDelTime.Bottom,
					m_rtDelTime.U0, m_rtDelTime.V0, m_rtDelTime.U1, m_rtDelTime.V1,
					0xFFFFFFFF );
				
			}
			// 캐릭터 이전중 표시 [7/25/2012 Ranma]
#ifdef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM			
			if ( _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].sbMoveState == CMS_MOVE_WAIT )	
			{
				int tv_posX = (m_rcDelTime.Right - m_rcDelTime.Left)/2;
				pDrawPort->PutTextExCX(m_strCharServerMove, tv_posX , 21 ,0xFFAA44FF);
				pDrawPort->AddTexture( m_rcDelTime.Left, m_rcDelTime.Top, m_rcDelTime.Right, m_rcDelTime.Bottom,
					m_rtDelTime.U0, m_rtDelTime.V0, m_rtDelTime.U1, m_rtDelTime.V1,	0xFFFFFFFF );
			}
#endif
		}
			
		// OK Button(Connect)
		m_btnOK.Render();
		
		// Create Character Button
		m_btnCreateChar.Render();
		
		// Delete Character Button
		m_btnDeleteChar.Render();
		
		// Option Button
		m_btnOptions.Render();
		
		// Exit Button
		m_btnExit.Render();

#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
		m_btnBack.Render();
#endif


		if (_pGameState->GetExistCharNum() <= 4)
		{
			m_btnTurnR.SetEnable(FALSE);
			m_btnTurnL.SetEnable(FALSE);
		}else
		{
			m_btnTurnR.SetEnable(TRUE);
			m_btnTurnL.SetEnable(TRUE);

			 if (m_bIsLeftView)
			{
				m_btnTurnR.Render(TEXCOLOR_W);
				m_btnTurnL.Render(TEXCOLOR_B);
			}else
			{
				m_btnTurnR.Render(TEXCOLOR_B);
				m_btnTurnL.Render(TEXCOLOR_W);
			}
		}		

		
		// Render all elements
		pDrawPort->FlushRenderingQueue();
		
		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	
	
	// Check ANI
	// 선택된 캐릭터에 idle Animation을 제거함.
	for(int j = 0; j < MAX_SLOT; j++)
	{
		CEntity *pEntity = _pGameState->m_pEntModels[j];
		if(pEntity)
		{
			int iJob		= _pGameState->m_SlotInfo[j].job;
			INDEX idAni		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, m_aSlotAni[j].m_idAni ) );
			CModelInstance* pMI =  pEntity->GetModelInstance();
			
			if(pMI)
			{
				if(	pMI->IsAnimationPlaying(idAni)	&& m_aSlotAni[j].m_idAni != ANIM_IDLE && m_aSlotAni[j].m_idAni != ANIM_ATTACK_IDLE
					&& m_aSlotAni[j].m_idAni != ANIM_EXT_ATTACK_IDLE)
				{			
					if(_pTimer->CurrentTick() - m_aSlotAni[j].m_startTime	
						>= (pMI->GetAnimLength(idAni) + 0.3f ) )
					{
						pMI->NewClearState(CLEAR_STATE_LENGTH);
						INDEX idWalk = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
						m_aSlotAni[j].m_idAni = ANIM_IDLE;
						// put on - weapon ----------------------------------------------------<<
						int nItemSubType;
						int nItemIndex = -1;
						
						// 무기 착용 확인 
						SLONG iWear		=	_pGameState->m_SlotInfo[j].wear[WEAR_WEAPON]; // 무기 
						SLONG iWear3	=	_pGameState->m_SlotInfo[j].wear[WEAR_SHIELD];	// 방패 
						
						if (iWear > 0)
						{
							if (_pGameState->m_SlotInfo[j].bExtension)
							{
								idWalk = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_EXT_ATTACK_IDLE ) );
								m_aSlotAni[j].m_idAni = ANIM_EXT_ATTACK_IDLE;
							}
							else
							{
								idWalk = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_ATTACK_IDLE ) );
								m_aSlotAni[j].m_idAni = ANIM_ATTACK_IDLE;
							}
						}
						
						pMI->AddAnimation(idWalk,AN_LOOPING|AN_NORESTART,1,0);
						
						if(iWear > 0)
						{				
							CItemData* pID = _pNetwork->GetItemData(iWear);
							nItemIndex = pID->GetItemIndex();
							nItemSubType = pID->GetSubType();
							_pGameState->WearingArmor(pMI, *pID);
							_pGameState->m_SlotInfo[j].itemEffect.Change(
								_pGameState->m_SlotInfo[j].job
								, pID
								, pID->GetWearingPosition()
								, _pGameState->m_SlotInfo[j].itemPlus[WEAR_WEAPON]
								, &pMI->m_tmSkaTagManager
								, 1, nItemSubType );
						}
						if(iWear3 > 0)	
						{
							CItemData* pID = _pNetwork->GetItemData(iWear3);
							_pGameState->WearingArmor(pMI, *pID);
						}
						
						// ---------------------------------------------------------------------------->>
					}
				}
				
			}
		}
	}
	// show night shadow message [11/6/2009 rumist]
	if( m_bIsShowMessageInfo )
	{
		pDrawPort->InitTextureData( m_ptdMsgTexture );
		m_bxNoticeMsg.SetBoxPos(m_rcMessageInfo);
		m_bxNoticeMsg.Render();
		
		// Render all elements
		pDrawPort->FlushRenderingQueue();
		
		m_msgNCText.SetRenderPos( m_rcMessageInfo.Left, m_rcMessageInfo.Top );
		m_msgNCText.Render();
		const int GAP = 10;
	
	}
	
	// [2012/07/05 : Sora]  캐릭터 슬롯 확장
	UpdateCharSlotTime();

	if( m_ulCharSlotTime > 0 )
	{
		// 캐릭터 슬롯 확장 도움말
		pDrawPort->InitTextureData( m_ptdMsgTexture );
		m_bxCharSlotHelp.SetBoxPos(m_rcCharSlotHelp);
		m_bxCharSlotHelp.Render();
		pDrawPort->FlushRenderingQueue();
		
		m_msgCharSlotHelp.SetRenderPos( m_rcCharSlotHelp.Left, m_rcCharSlotHelp.Top );
		m_msgCharSlotHelp.Render();

		// 캐릭터 슬롯 남은 시간 출력
		pDrawPort->InitTextureData( m_ptdMsgTexture );
		m_bxCharSlotRemain.SetBoxPos(m_rcCharSlotRemain);
		m_bxCharSlotRemain.Render();
		pDrawPort->FlushRenderingQueue();

		pDrawPort->PutTextExCX( _S( 5707, "추가 슬롯 사용 기간 : " ) + GetRemainTime(),
								m_rcCharSlotRemain.Left + (m_rcCharSlotRemain.GetWidth()/2),
								m_rcCharSlotRemain.Top + 12);
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :burn
// ----------------------------------------------------------------------------
WMSG_RESULT CUISelChar::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	CTString	strMessage;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCreateChar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnDeleteChar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnOptions.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
			else if( m_btnBack.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
#endif
			if (!m_bIsLeftView)
				m_btnTurnL.MouseMessage( pMsg );
			else
				m_btnTurnR.MouseMessage( pMsg );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				//-- Down 동작을 표현하기위해
				m_btnOK.MouseMessage( pMsg );
				m_btnCreateChar.MouseMessage( pMsg );
				m_btnDeleteChar.MouseMessage( pMsg );
				m_btnOptions.MouseMessage( pMsg );
				m_btnExit.MouseMessage( pMsg );

				CUIManager::getSingleton()->RearrangeOrder( UI_SEL_CHAR, TRUE );		
				return WMSG_SUCCESS;
			}
			if (!m_bIsLeftView)
				m_btnTurnL.MouseMessage( pMsg );
			else
				m_btnTurnR.MouseMessage( pMsg );
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
			m_btnBack.MouseMessage( pMsg );
#endif
			SelectCharacter(nX, nY);
		}
		break;
		
	case WM_LBUTTONDBLCLK:
		{
			if( _pGameState->GetGameMode() != CGameState::GM_NETWORK )
			{
				SelectCharacter(nX, nY);

				if(_pGameState->SelectedSlot() > 0)
				{

					if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) break;
					if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].sbMoveState == CMS_MOVE_WAIT ) break;
#ifdef EXSLOT
					// wooss 050822 
					// 확장된 슬롯을 선택 했다면 남은 기간을 체크한다
					int tv_t1,tv_t2;
					GetExSlotTime(0,&tv_t1,&tv_t2);
					if(_pGameState->SelectedSlot()==5)
					{
						if(tv_t1<=0)
						{
							CUIManager* pUIManager = CUIManager::getSingleton();

							pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( CTString( "확장슬롯 1 의 이용 기간이 지났습니다"));
							pUIManager->CreateMessageBox( MsgBoxInfo );
							return WMSG_FAIL;
						}

					}
					else if(_pGameState->SelectedSlot()==6)
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if(tv_t2<=0)
						{
							pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( CTString( "확장슬롯 2 의 이용 기간이 지났습니다"));
							pUIManager->CreateMessageBox( MsgBoxInfo );
							return WMSG_FAIL;

						}
					}
#endif
					StartGame();					
				}
			}
		}
		break;
		
	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{			
				if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND && _pGameState->GetGameMode() != CGameState::GM_NETWORK)
					{
						if(_pGameState->SelectedSlot() > 0)
						{
#ifdef EXSLOT
							// wooss 050822 
							// 확장된 슬롯을 선택 했다면 남은 기간을 체크한다
							int tv_t1,tv_t2;
							GetExSlotTime(0,&tv_t1,&tv_t2);
							if(_pGameState->SelectedSlot()==5)
							{
								if(tv_t1<=0)
								{
									CUIManager* pUIManager = CUIManager::getSingleton();

									pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( CTString( "확장슬롯 1 의 이용 기간이 지났습니다"));
									pUIManager->CreateMessageBox( MsgBoxInfo );
									return WMSG_FAIL;
								}

							}
							else if(_pGameState->SelectedSlot()==6)
							{
								if(tv_t2<=0)
								{
									CUIManager* pUIManager = CUIManager::getSingleton();

									pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( CTString("캐릭터 선택 오류" ), UMBS_OK,UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( CTString( "확장슬롯 2 의 이용 기간이 지났습니다"));
									pUIManager->CreateMessageBox( MsgBoxInfo );
									return WMSG_FAIL;

								}

							}
#endif
							// 게임을 시작함.
							//_pSound->Mute();
							StartGame();
							//_pSound->UpdateSounds();
						}
						else
						{
							CUIManager* pUIManager = CUIManager::getSingleton();

							pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 241, "캐릭터 선택 오류" ), UMBS_OK,
														UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( _S( 242, "캐릭터를 선택하지 않으셨습니다." ) );
							pUIManager->CreateMessageBox( MsgBoxInfo );
						}
					}
				}
				else if( ( wmsgResult = m_btnCreateChar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND)
					{
						int slotMax = m_ulCharSlotTime > 0 ? MAX_SLOT : 4;
						if(_pGameState->GetExistCharNum() >= slotMax)
						{
//#ifndef G_RUSSIA//릴리즈 버그 러시아의 경우만 이 경우에 메모리 침범으로 클라가 죽는다.
							_pGameState->DisplayErrorMessage( MSG_FAIL_DB_FULL_CHAR, UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR);
//#endif
						}
						else
						{
							// connie LoginWorld 2 
							_pGameState->SetCameraByJob(0);

							// 슬롯에 몇개의 공간이 있는지 확인해서,
							// 꽉 차 있으면 더이상 생성할수 없음을 나타내줌.
							CUIManager::getSingleton()->SetUIGameState(UGS_CREATECHAR);

							// init slot effect 
							//for(int i=0;i<MAX_SLOT;i++)
							//	DestroyEffectGroupIfValid(m_aEGslotPosID[i]);

						}
					}
				}
				else if( ( wmsgResult = m_btnDeleteChar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND)
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if(_pGameState->SelectedSlot() > 0)
						{
							BOOL tv_chk = FALSE;

							// FIXME : 캐릭터 삭제 메세지 정의할것.
							pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER );
							
							CUIMsgBox_Info	MsgBoxInfo;
#if defined G_KOR || defined G_THAI
							int tv_line;
							// 주민번호 -> 주민번호 뒤 7자리 [10/29/2009 rumist]
							MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_OK | UMBS_INPUTPASSWORD,
													UI_SEL_CHAR, MSGCMD_DELETE_CHARACTER_SECURITY, 260 );
								
							if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) tv_chk = TRUE;
							if( !tv_chk )
							{
								// [2012/07/05 : Sora]  캐릭터 슬롯 확장
								if( _pGameState->SelectedSlot()-1 >= 4 &&  m_ulCharSlotTime == 0 )
								{
									pUIManager->CloseMessageBox( MSGCMD_NULL );
									
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK, UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( _S( 5703, "해당 기능은 캐릭터 확장 슬롯 아이템 사용 시 가능 합니다." ) );
									pUIManager->CreateMessageBox( MsgBoxInfo );
									return WMSG_SUCCESS;
								}

								MsgBoxInfo.AddString( _S( 2904, "아이디를 입력하세요.\n(삭제요청 후 24시간이 지나면 자동삭제)" ) );
								MsgBoxInfo.AddString( CTString( " " ) );
								tv_line = 3;
							}
							else 
							{
								MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
								tv_line = 2;
								
							}

							MsgBoxInfo.AddString( CTString( " " ) );

							// 주민번호 -> 주민번호 뒤 7자리 [10/29/2009 rumist]
							//MsgBoxInfo.AddStringEx( _S( 2906, "아이디 입력"),tv_line,0,0xFF9156FF);
							MsgBoxInfo.SetInputBox(tv_line,10,MAX_NAME_LEN+1);
							pUIManager->CreateMessageBox( MsgBoxInfo );
							pUIManager->Lock(TRUE);
#else // G_KOR
							
							MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_YESNO,
								UI_SEL_CHAR, MSGCMD_DELETE_CHARACTER );
							
							tv_chk = FALSE;
							if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0) tv_chk = TRUE;
							if( !tv_chk )
							{
								// [2012/07/05 : Sora]  캐릭터 슬롯 확장
								if( _pGameState->SelectedSlot()-1 >= 4 &&  m_ulCharSlotTime == 0 )
								{
									pUIManager->CloseMessageBox( MSGCMD_NULL );
									
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK, UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
									MsgBoxInfo.AddString( _S( 5703, "해당 기능은 캐릭터 확장 슬롯 아이템 사용 시 가능 합니다." ) );
									pUIManager->CreateMessageBox( MsgBoxInfo );
									return WMSG_SUCCESS;
								}

								MsgBoxInfo.AddString( _S( 2904, "캐릭터 삭제는 삭제요청 후 24시간이 지나야 삭제가 됩니다. 단, 시간안에 취소 가능" ) );
								MsgBoxInfo.AddString( CTString( " " ) );
							}
							else 
							{
								MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
								
							}
							
							pUIManager->CreateMessageBox( MsgBoxInfo );
#endif // G_KOR
						}
						else
						{
							pUIManager->CloseMessageBox( MSGCMD_SELCHAR_ERROR );
							
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 241, "캐릭터 선택 오류" ), UMBS_OK,
														UI_SEL_CHAR, MSGCMD_SELCHAR_ERROR );
							MsgBoxInfo.AddString( _S( 242, "캐릭터를 선택하지 않으셨습니다." ) );
							pUIManager->CreateMessageBox( MsgBoxInfo );
						}
					}
				}
				else if( ( wmsgResult = m_btnOptions.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CUIManager::getSingleton()->GetOption()->OpenOption();
				}
				else if( ( wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if(g_bAutoLogin)
						{
							extern BOOL	_bLoginProcess;
							_bLoginProcess = TRUE;
							_pGameState->GetGameMode() = CGameState::GM_NONE;
							pUIManager->GetGame()->StopGame();							

							extern CTString g_nmID;
							extern CTString g_nmPW;
							extern CTString g_nmCID;

							_pNetwork->m_strUserID = g_nmID;
							_pNetwork->m_strUserPW = g_nmPW;
							_pNetwork->m_strUserCID = g_nmCID;

							if (GameDataManager* pGameData = GameDataManager::getSingleton())
							{
								if (CServerSelect* pServerSelect = pGameData->GetServerData())
									pServerSelect->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
							}

							//pUIManager->GetSelServer()->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
						
							// connie [2009/7/21] - login2 
							const int	iCameraEntityID	= 43565;

							CEntity		*pCameraEntity = NULL;
							CEntity		*pEntity = NULL;
							BOOL bExist = _pNetwork->ga_World.EntityExists( iCameraEntityID, pCameraEntity );
							if(bExist)
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StartCamera(pCameraEntity, FALSE);
							}

							pUIManager->SetUIGameState( UGS_SELSERVER );						
						}
						else
						{
							g_bAutoRestart = FALSE;
							
							_pSound->Mute();
							// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
							//_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
							_cmiComm.Disconnect();

							pUIManager->SetUIGameState(UGS_LOGIN);
							// 기존에 월드에 있던 엔티티를 제거함.
							for(int i = 0; i < MAX_SLOT; ++i)
							{
								CEntity *pEntity = _pGameState->m_pEntModels[i];
								if(pEntity)
								{
									pEntity->en_RenderType = CEntity::RT_SKAEDITORMODEL;
								}
							}
							_pGameState->SelectedSlot() = 0; //슬롯 선택 커서 초기화.
							
							_pGameState->BackToLogin();
						}
						
						// initialize NC card flag. [12/2/2009 rumist]
						_pGameState->SetCreatableNightShadow( FALSE );
						
//#define RESTRICT_SOUND 	
#ifdef RESTRICT_SOUND
						extern BOOL _bNTKernel;
						//if( _bNTKernel )

							_pSound->UpdateSounds();
						#endif

						// init slot effect 
						//for(int i=0;i<MAX_SLOT;i++)
						//	DestroyEffectGroupIfValid(m_aEGslotPosID[i]);

					}
				}

				return WMSG_SUCCESS;
			}
			else if ( !m_bIsLeftView && ( wmsgResult = m_btnTurnL.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER_SECURITY );
				pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER );
				pUIManager->Lock(FALSE);

				_pGameState->SelectedSlot() = 0;
				_pGameState->SelCharToMove(CAMERA_TURN_LEFT);
				m_bIsLeftView = TRUE;
				return WMSG_SUCCESS;
			}
			else if ( m_bIsLeftView && ( wmsgResult = m_btnTurnR.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER_SECURITY );
				pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER );
				pUIManager->Lock(FALSE);

				_pGameState->SelectedSlot() = 0;
				_pGameState->SelCharToMove(CAMERA_TURN_RIGHT);
				m_bIsLeftView = FALSE;
				return WMSG_SUCCESS;
			}
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
			else if( ( wmsgResult = m_btnBack.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();
					
					_pSound->Mute();
					// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
					//_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
					_cmiComm.Disconnect();

					// 기존에 월드에 있던 엔티티를 제거함.
					for(int i = 0; i < MAX_SLOT; ++i)
					{
						CEntity *pEntity = _pGameState->m_pEntModels[i];
						if(pEntity)
						{
							pEntity->en_RenderType = CEntity::RT_SKAEDITORMODEL;
						}
					}
					_pGameState->SelectedSlot() = 0; //슬롯 선택 커서 초기화.
					
					g_bAutoRestart = TRUE;
					_pGameState->BackToLogin();											

					pUIManager->SetUIGameState(UGS_LOGIN);
					Sleep(1000);
				}
			}
#endif
			
		}
		break;
	}

	return WMSG_FAIL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUISelChar::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( bOK )
	{
		switch( nCommandCode )
		{
			
			case MSGCMD_DELETE_CHARACTER:
				_pSound->Mute();
				DeleteCharacter();
			#ifdef RESTRICT_SOUND
				extern BOOL _bNTKernel;
				//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
				
				break;
			
			case MSGCMD_DELETE_CHARACTER_SECURITY:
				_pSound->Mute();
				DeleteCharacter(strInput);
			#ifdef RESTRICT_SOUND
				extern BOOL _bNTKernel;
				//if( _bNTKernel )
				_pSound->UpdateSounds();
			#endif
				
				break;			
		}
	}
	else
	{
		switch( nCommandCode )	
		{
		case MSGCMD_DELETE_CHARACTER_SECURITY: // 예외처리: 캐릭터 삭제 팝업창에서 x버튼 클릭 문제 수정
			Lock(FALSE);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CUISelChar::ShowCharacter()
{
	if(!m_pWorld) return FALSE;
	
	// 받아들인 정보를 바탕으로...
	// 엔티티를 생성하고 , ModelInstance를 생성하고...

	int aiMarker[MAX_SLOT] =
	{
		43585, 43571, 43406, 43570,
		43826, 43827, 43828, 43829,
	};	

//강동민 수정 시작 클로즈 2차 작업	08.18
	for(int i = 0; i < MAX_SLOT; ++i)
	{
		CEntity *pEntity		= m_pWorld->EntityFromID(aiMarker[i]);
		if(pEntity)
		{
		//	pEntity->SetFlagOn(ENF_HIDDEN);
			pEntity->en_RenderType = CEntity::RT_SKAEDITORMODEL;
		}
	}
//강동민 수정 끝 클로즈 2차 작업		08.18

	int iSlot = 0;
	while(_pGameState->m_SlotInfo[iSlot].bActive == TRUE)
	{	
		CEntity *penMarker					= m_pWorld->EntityFromID(aiMarker[iSlot]);
		//CPlacement3D plMarker	= penMarker->GetLerpedPlacement();		
		_pGameState->m_pEntModels[iSlot]	= penMarker;
		penMarker->en_RenderType			= CEntity::RT_SKAMODEL;
		penMarker->en_ulCollisionFlags |= ECF_TESTMASK;
		penMarker->ModelChangeNotify();

		const int iSlotJob = _pGameState->m_SlotInfo[iSlot].job;

		//penEntity->InitAsSkaModel();				
		penMarker->SetSkaModel(JobInfo().GetFileName(iSlotJob));
		CModelInstance* pMI = penMarker->GetModelInstance();
		ASSERT(pMI != NULL && "Invalid Model Instance!");
		if(pMI)
		{
			pMI->mi_iFaceMesh	= -1;
			//pMI->mi_iFaceTexture= -1;
			pMI->mi_iHairMesh	= -1;
			//pMI->mi_iHairTexture= -1;
		}

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, iSlotJob, _pGameState->m_SlotInfo[iSlot].hairstyle, _pGameState->m_SlotInfo[iSlot].facestyle);
		CPrintF("Slot : %d, Hair : %d, Face : %d\n", iSlot, _pGameState->m_SlotInfo[iSlot].hairstyle, _pGameState->m_SlotInfo[iSlot].facestyle);
		BOOL bIsWeapon = FALSE;
		
		if( _pNetwork->wo_iNumOfItem > 0 )
		{
			// 슬롯의 캐릭터에 장비를 장착한 모습을 보여줘야 하는 부분...
			// 상의 하의 무기 방패 장갑 신발 특수
			for(int i = 0; i < WEAR_COUNT ; ++i)
			{
//				if( i == 1) continue;	// 무기
//				if( i == 3) continue;	// 방어구
				
				int nItemSubType;
				int nItemIndex = -1;

				const SLONG iWear = _pGameState->m_SlotInfo[iSlot].wear[i];
				CItemData* pID = _pNetwork->GetItemData(iWear);
				
				if(iWear > 0)
				{
					if(pID->GetWearingPosition() != WEAR_HELMET || (CTString)pID->GetItemSmcFileName() != MODEL_TREASURE)
					{
						nItemIndex = pID->GetItemIndex();
						nItemSubType = pID->GetSubType();
						_pGameState->DeleteDefaultArmor(pMI, pID->GetWearingPosition(), iSlotJob);
						_pGameState->WearingArmor(pMI, *pID);

						if (pID->GetWearingPosition() == WEAR_WEAPON)
						{
							bIsWeapon = TRUE;
						}
					}
				}
				_pGameState->m_SlotInfo[iSlot].itemEffect.Change(
					_pGameState->m_SlotInfo[iSlot].job
					, pID
					, wearTypeTable[i]
					, _pGameState->m_SlotInfo[iSlot].itemPlus[i]
					, &pMI->m_tmSkaTagManager
					, 1, nItemSubType );
			}
		}
		
		if(pMI)
		{
			const int iJob = _pGameState->m_SlotInfo[iSlot].job;
			INDEX idIdle;

			if (bIsWeapon)
			{
				if (_pGameState->m_SlotInfo[iSlot].bExtension)
				{
					idIdle = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_EXT_ATTACK_IDLE ) );
				}
				else
				{
					idIdle = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_ATTACK_IDLE ) );
				}
			}
			else
			{
				idIdle = ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
			}

			pMI->AddAnimation(idIdle,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	
			// 060608
			if( _pGameState->m_SlotInfo[iSlot].m_time >0 )
			{
				pMI->SetModelColor( 0x000000FF );
				//pMI->m_tmSkaTagManager.SetOwner(_pGameState->m_pEntModels[iSlot]);
				//if(!CEffectGroupManager::Instance().IsValidCreated(m_aEGslotPosID[iSlot]))
				//	m_aEGslotPosID[iSlot] = StartEffectGroup("STATE_DELETE",&pMI->m_tmSkaTagManager,_pTimer->GetLerpedCurrentTick());
			}
			else
			{
				pMI->SetModelColor( 0xFFFFFFFF );
				//StopEffectGroupIfValid(m_aEGslotPosID[iSlot],0.1f);
			}
#ifdef ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM
			// 캐릭터 이동 관련 조건 삽입 [7/19/2012 rumist]
			if( _pGameState->m_SlotInfo[iSlot].sbMoveState == CMS_MOVE_WAIT )
			{
				// 특정 캐릭터 검정색으로 도배. 나중에 색깔을 바꿔달라면 바꿔줄 수 있음.
				pMI->SetModelColor( 0x000000FF );
			
				m_bIsShowCharMoveMsgInfo = TRUE;
			}
			if ( _pGameState->m_SlotInfo[iSlot].sbMoveState == CMS_MOVE_ROLLBACK )
			{
				CTString strTemp = _pGameState->m_SlotInfo[iSlot].name;
				if (m_strServerMoveFailCharName == _s(""))
				{
					m_strServerMoveFailCharName += strTemp;
				}
				else
				{
					m_strServerMoveFailCharName += _s(" , ") + strTemp;
				}
			}
#endif
		}	

		iSlot++;
	}
	// night shadow card. [11/6/2009 rumist]
	m_bIsShowMessageInfo = _pGameState->IsCreatableNightShadow();
	// show message box for roll back.
	return TRUE;
}

void CUISelChar::ShowCharMoveRollbackMsgBox(bool bShow)
{
 	if (m_strServerMoveFailCharName.Length() <= 0)
		return;

	if (bShow)
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(5722,"에러 메시지"), UMBS_OK, UI_NONE, MSGCMD_NULL, 316 );
		CTString strTemp;
		CTString strTemp2;
		strTemp = _S(5716, "신청 하신 서버 이전이 다음과 같은 이유로 취소 되었습니다.");
		MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
		strTemp2.PrintF(_S(5717, " [ %s ] 캐릭터의 서버 이전이 실패 하였습니다. 자세한 문의는 고객 센터에서 확인 하실 수 있습니다."), m_strServerMoveFailCharName);
		MsgBoxInfo.AddString( strTemp2, 0xF2F2F2FF, TEXT_CENTER );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		
		m_strServerMoveFailCharName = "";
	}		
}

//-----------------------------------------------------------------------------
// Purpose: 선택된 캐릭터를 삭제합니다.
//-----------------------------------------------------------------------------
BOOL CUISelChar::DeleteCharacter()
{	
	if(_pGameState->SelectedSlot() <= 0)
		return FALSE;

	if( _pNetwork->m_bSendMessage == TRUE )
		return FALSE;

	ULONG ulDelIndex = _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].index;
	BOOL tv_chk = FALSE;
	
	if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0 ) tv_chk = TRUE;
	
	_pNetwork->SendDeleteCharacter(ulDelIndex,tv_chk);
	CUIManager::getSingleton()->Lock(TRUE);

	return FALSE;
}


//-----------------------------------------------------------------------------
// Purpose: 선택된 캐릭터를 삭제합니다. 2
//-----------------------------------------------------------------------------
BOOL CUISelChar::DeleteCharacter(CTString  secuNum)
{	
	CTString strMessage;
	if(_pGameState->SelectedSlot() <= 0)
		return FALSE;

	if( _pNetwork->m_bSendMessage == TRUE )
		return FALSE;

	ULONG ulDelIndex = _pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].index;
	BOOL tv_chk = FALSE;
	if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time > 0 ) tv_chk = TRUE;

	_pNetwork->SendDeleteCharacter(ulDelIndex, secuNum, tv_chk );
	CUIManager::getSingleton()->Lock(TRUE);

	return FALSE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUISelChar::StartGame()
{
	if( PopUpMsgBoxExist() )
		return;

	const int iSelectedSlot		= _pGameState->SelectedSlot()-1;
	ASSERT( iSelectedSlot >= 0 && iSelectedSlot <= (MAX_SLOT - 1) && "Invalid Selected Slot!!!" );
	if( iSelectedSlot < 0 || iSelectedSlot > (MAX_SLOT - 1) )
	{
		return;
	}

	ULONG	StartIndex;
	StartIndex	= _pGameState->m_SlotInfo[iSelectedSlot].index;

	if(_pNetwork->m_bSendMessage)
		return;

	_pNetwork->SendGameStart(StartIndex);

	CUIManager* pUIManager = CUIManager::getSingleton();

	_pGameState->ClearCharacterSlot();
/*
	CGame* _pGame			= pUIManager->GetGame();
	CPlayerCharacter &pc	= _pGame->gm_apcPlayers[0];
	pc.pc_iPlayerType		= _pGameState->m_SlotInfo[iSelectedSlot].job;
	
	CPlayerEntity *penPlayerEntity	= NULL;
	CEntity* penPlEntity			= NULL;
	penPlEntity						= CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity					= (CPlayerEntity*) penPlEntity;	
	penPlayerEntity->CharacterChanged(pc);
	*/
	pUIManager->Lock(TRUE);
	// wooss 051017 
	// 캐릭터 선택시 Input.cpp ReadKeyboardDevice::_abKeysDressed[KIP_MOUSE1] 에서 토글식으로 처리하여
	// 캐릭터를 선택하고 게임시작을 누르면 마우스 버튼이 누른 상태로 표시되어 게임을 시작하면 캐릭터가 마우스 포인터쪽으로 뛰어가게 된다.
	_pInput->SetLMousePressed(FALSE);

	m_bIsShowMessageInfo = _pGameState->IsCreatableNightShadow();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nX - 
//			nY - 
// Output : WMSG_RESULT
//-----------------------------------------------------------------------------
void CUISelChar::SelectCharacter(int nX, int nY)
{
	if( PopUpMsgBoxExist() )
		return;

	CEntity*		penPlEntity		= NULL;
	CPlayerEntity*	penPlayerEntity = NULL;
	penPlEntity						= CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity					= (CPlayerEntity*) penPlEntity;
	CPlacement3D plCamera			= penPlayerEntity->en_plLastViewpoint;
	CAnyProjection3D apr;
	penPlayerEntity->GetProjection(apr);	
	ASSERT( apr != NULL && "Invalid Projection3D!" );
	if( apr == NULL )
		return;
	apr->Prepare();

	CUIManager* pUIManager = CUIManager::getSingleton();

	// make the ray from viewer point through the mouse point, in current projection
	CPlacement3D plRay;
	apr->RayThroughPoint(
		FLOAT3D((float)nX, ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() ) - (float)nY, 0.0f), 
		plRay);
	
	// construct cast ray for base world
	CCastRay crRay( NULL, plRay);
	crRay.cr_penHit						= NULL;
	crRay.cr_ttHitModels				= CCastRay::TT_NONE;     // only brushes block the damage
	crRay.cr_bHitTranslucentPortals		= FALSE;
	crRay.cr_bPhysical					= FALSE;
	
	for(int i = 0; i < MAX_SLOT; ++i)
	{
		if(!_pGameState->m_SlotInfo[i].bActive) continue;
		CEntity *pEntity = _pGameState->m_pEntModels[i];
		if(pEntity)
		{
		
			// 슬롯과 충돌했음.
			crRay.TestModelCollisionBox(pEntity);
			if(crRay.cr_penHit != NULL && (crRay.cr_penHit->en_RenderType == CEntity::RT_SKAMODEL))
			{
				int iSelectedSlot = _pGameState->SelectedSlot();

				// 현재 선택된 캐릭터가 변함이 없을때...
				if( iSelectedSlot != i + 1 )
				{

			/*		// 선택된 캐릭터에 idle Animation을 제거함.
					if(iSelectedSlot > 0)
					{
						CModelInstance* pMI		= _pGameState->m_pEntModels[iSelectedSlot - 1]->GetModelInstance();
						if(pMI)
						{
							const int iJob		= _pGameState->m_SlotInfo[iSelectedSlot - 1].job;
							INDEX idWalk		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, ANIM_IDLE ) );
							pMI->AddAnimation(idWalk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
						}
					}
			*/	

					iSelectedSlot = i + 1;

					// 선택된 캐릭터에 idle Animation을 줍니다.
					_pGameState->SelectedSlot() = iSelectedSlot;
					{
						CModelInstance* pMI		= _pGameState->m_pEntModels[iSelectedSlot - 1]->GetModelInstance();
						if(pMI)
						{
							const int iJob		= _pGameState->m_SlotInfo[iSelectedSlot - 1].job;
							const BOOL bExtension = _pGameState->m_SlotInfo[iSelectedSlot - 1].bExtension;

							INDEX idAttack;
							int tv_flag =0;

					//		if( pMI->IsNotAnimationPlaying()) return;
							if(_pGameState->m_SlotInfo[_pGameState->SelectedSlot()-1].m_time <=0 )
							{
								do{ 
									m_aSlotAni[iSelectedSlot - 1].m_idAni = rand()%( ANIM_SOCIAL_10 - ANIM_SOCIAL_0  +1) +ANIM_SOCIAL_0;
								}
								while (m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_2
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_5 
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_7 
									|| m_aSlotAni[iSelectedSlot - 1].m_idAni == ANIM_SOCIAL_9) ;

								tv_flag = NULL;							
							}
							else
							{
								m_aSlotAni[iSelectedSlot - 1].m_idAni = ANIM_SOCIAL_5;
								tv_flag = NULL;
							}

							idAttack		= ska_GetIDFromStringTable( JobInfo().GetAnimationName( iJob, m_aSlotAni[iSelectedSlot - 1].m_idAni ) );

							pMI->AddAnimation(idAttack,tv_flag,1,0);

							// Check animation time
							m_aSlotAni[iSelectedSlot - 1].m_startTime = _pTimer->CurrentTick();
							
							// put off - weapon ----------------------------------------------------<<
							// 무기 착용 확인 
							SLONG iWear		=	_pGameState->m_SlotInfo[i].wear[WEAR_WEAPON]; // 무기 
							SLONG iWear3	=	_pGameState->m_SlotInfo[i].wear[WEAR_SHIELD];	// 방패 

							BOOL bTakeOffArmor = FALSE;
							
							if(iWear > 0 ){
								CItemData* pID = _pNetwork->GetItemData(iWear);

								if (_pGameState->m_SlotInfo[i].job != NIGHTSHADOW)
								{
									_pGameState->TakeOffArmor(pMI, *pID);
								}
								_pGameState->m_SlotInfo[i].itemEffect.DeleteEffect(pID->GetWearingPosition());
								bTakeOffArmor = TRUE;
								_pGameState->m_SlotInfo[i].itemEffect.Change(
									_pGameState->m_SlotInfo[i].job,
									pID,
									pID->GetWearingPosition(),
									-1,
									&pMI->m_tmSkaTagManager,
									1, pID->GetSubType() );
							}
							if(iWear3 > 3)	
							{
								CItemData* pID = _pNetwork->GetItemData(iWear3);
								_pGameState->TakeOffArmor(pMI, *pID);
								bTakeOffArmor = TRUE;
								_pGameState->m_SlotInfo[i].itemEffect.Change(
									_pGameState->m_SlotInfo[i].job,
									pID,
									pID->GetWearingPosition(),
									-1,
									&pMI->m_tmSkaTagManager,
									1, pID->GetSubType() );
							}

							//if (bTakeOffArmor)
								//_pGameState->m_SlotInfo[i].itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
							// ---------------------------------------------------------------------------->>																					
						}
					}

					// Update character status
					UpdateStatus();
					
					// reset button status.	
					m_btnOK.SetEnable(TRUE);
					m_btnDeleteChar.SetEnable(TRUE);

					if((_pGameState->m_SlotInfo[iSelectedSlot - 1].m_time) > 0)
					{	
						m_btnOK.SetEnable(FALSE);	
					}
					// ITS#10074 : BUGFIX : 조건식의 조건상의 문제 발생. [8/3/2012 rumist]
					else if(_pGameState->m_SlotInfo[iSelectedSlot-1].sbMoveState == CMS_MOVE_WAIT)			// 캐릭터 이동 [7/19/2012 rumist]					
					{	
						m_btnOK.SetEnable(FALSE);
						m_btnDeleteChar.SetEnable(FALSE);	
					}
				}

				return;
			} // 충돌
		}
	}

	return;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUISelChar::Lock(BOOL bLock)
{
#if !defined(G_KOR)
	m_btnCreateChar.SetEnable(!bLock);
#endif
	m_btnDeleteChar.SetEnable(!bLock);
	//m_btnOptions.SetEnable(!bLock);	
	m_btnOK.SetEnable(!bLock);
	//m_btnCancel.SetEnable(!bLock);
}


void CUISelChar::SetExSlotTime(int slotNum,int rTime)
{
	if(slotNum==1) m_exSlot1=rTime;
	else if(slotNum==2) m_exSlot2=rTime;
	
}

int	CUISelChar::GetExSlotTime(int slotNum,int* tSlot1,int* tSlot2)
{
	if(slotNum==1) return m_exSlot1;
	else if(slotNum==2) return m_exSlot2;
	else if(slotNum==0) 
	{
		*tSlot1=m_exSlot1;
		*tSlot2=m_exSlot2;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// SetCharSlotTime 남은시간을 계산하여 스트링으로 반환
// [2012/07/05 : Sora]  캐릭터 슬롯 확장
//-----------------------------------------------------------------------------
void CUISelChar::SetCharSlotTime( ULONG ulTime )
{
	m_ulCharSlotTime = ulTime;
	m_CharSlotTimeOld = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

//-----------------------------------------------------------------------------
// GetRemainTime 남은시간을 계산하여 스트링으로 반환
// [2012/07/05 : Sora]  캐릭터 슬롯 확장
//-----------------------------------------------------------------------------
CTString CUISelChar::GetRemainTime()
{
	CTString strtemp = "";
	
	if( m_ulCharSlotTime >=  86400 ) // 60 * 60 * 24 (하루)
	{
		strtemp.PrintF( _S( 2511, "%d일" ),  m_ulCharSlotTime / 86400 );
	}
	else if( m_ulCharSlotTime >= 3600 )
	{
		strtemp.PrintF( _S( 2512, "%d시간" ),  m_ulCharSlotTime / 3600 );
	}
	else
	{
		// 60초 미만도 1분으로 표시
		strtemp.PrintF( _S( 2513, "%d분" ),  m_ulCharSlotTime >= 60  ? m_ulCharSlotTime/60 : 1 );
	}

	return strtemp;
}

//-----------------------------------------------------------------------------
// UpdateCharSlotTime 남은시간을 갱신
// [2012/07/05 : Sora]  캐릭터 슬롯 확장
//-----------------------------------------------------------------------------
void CUISelChar::UpdateCharSlotTime()
{
	if( m_ulCharSlotTime == 0 )
		return;

	__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	if( tmpTime - m_CharSlotTimeOld < 1000 )
		return;

	__int64 passedTime = ( ( tmpTime - m_CharSlotTimeOld ) / 1000 );

	if( m_ulCharSlotTime > passedTime )
	{
		m_ulCharSlotTime -= passedTime;
	}
	else
	{
		m_ulCharSlotTime = 0;
	}

	m_CharSlotTimeOld = tmpTime;
}

BOOL CUISelChar::PopUpMsgBoxExist()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_DELETE_CHARACTER_SECURITY ) ||
		pUIManager->DoesMessageBoxExist( MSGCMD_DELETE_CHARACTER ) ||
		pUIManager->DoesMessageBoxExist( MSGCMD_SELCHAR_ERROR ) )
	{
		return TRUE;
	}

	return FALSE;
}