#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Base/KeyNames.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UILacarette.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIWebBoard.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UISystemMenu.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UICashShopEX.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Contents/Base/UIRankingSystem.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Info/MyInfo.h>

static INT64 g_InputTabTime = 0; // 귀찮다..

// ----------------------------------------------------------------------------
// Name : CUIPlayerInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPlayerInfo::CUIPlayerInfo()
{
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
	m_ptdCommonBtnTexture = NULL;
	m_ptdMessageBoxTexture = NULL;
	m_ptdExpeditionTexture = NULL;
	m_ptdQuestBookTexture = NULL;
	m_llShortCutTime = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPlayerInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPlayerInfo::~CUIPlayerInfo()
{
	Destroy();
	STOCK_RELEASE(m_ptdCommonBtnTexture);
	STOCK_RELEASE(m_ptdMessageBoxTexture);
	STOCK_RELEASE(m_ptdExpeditionTexture);
	STOCK_RELEASE(m_ptdQuestBookTexture);
	STOCK_RELEASE(m_ptdGuildBattleTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtToolTipL.SetUV( 239, 253, 273, 272, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 274, 253, 318, 272, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 319, 253, 339, 272, fTexWidth, fTexHeight );

	// Web board button
	m_btnBoard.Create( this, CTString( "" ), 7, 27, 18, 18 );
	m_btnBoard.SetUV( UBS_IDLE, 142, 14, 160, 32, fTexWidth, fTexHeight );
	m_btnBoard.SetUV( UBS_CLICK, 161, 14, 179, 32, fTexWidth, fTexHeight );
	m_btnBoard.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBoard.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Messanger button
	m_btnMessanger.Create( this, CTString( "" ), 31, 27, 18, 18 );
	m_btnMessanger.SetUV( UBS_IDLE, 180, 14, 198, 32, fTexWidth, fTexHeight );
	m_btnMessanger.SetUV( UBS_CLICK, 199, 14, 217, 32, fTexWidth, fTexHeight );
	m_btnMessanger.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMessanger.CopyUV( UBS_IDLE, UBS_DISABLE );

	// System menu button
	m_btnSysMenu.Create( this, CTString( "" ), 61, 27, 18, 18 );
	m_btnSysMenu.SetUV( UBS_IDLE, 218, 14, 236, 32, fTexWidth, fTexHeight );
	m_btnSysMenu.SetUV( UBS_CLICK, 218, 33, 236, 51, fTexWidth, fTexHeight );
	m_btnSysMenu.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSysMenu.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Inventory button
	m_btnInven.Create( this, CTString( "" ), 91, 27, 18, 18 );
	m_btnInven.SetUV( UBS_IDLE, 142, 33, 160, 51, fTexWidth, fTexHeight );
	m_btnInven.SetUV( UBS_CLICK, 161, 33, 179, 51, fTexWidth, fTexHeight );
	m_btnInven.CopyUV( UBS_IDLE, UBS_ON );
	m_btnInven.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Character info button
	m_btnCharInfo.Create( this, CTString( "" ), 115, 27, 18, 18 );
	m_btnCharInfo.SetUV( UBS_IDLE, 180, 33, 198, 51, fTexWidth, fTexHeight );
	m_btnCharInfo.SetUV( UBS_CLICK, 199, 33, 217, 51, fTexWidth, fTexHeight );
	m_btnCharInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCharInfo.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnBoard.SetEnable(FALSE);
	m_btnMessanger.SetEnable(FALSE);
	m_btnSysMenu.SetEnable(FALSE);
	m_btnInven.SetEnable(FALSE);
	m_btnCharInfo.SetEnable(FALSE);

	m_rcQuickMenu.SetRect( 0, 0, 322, 27 );
	m_rtQuickMenu.SetUV( 0, 306, 321, 334, fTexWidth, fTexHeight );

	int nSubSkill = 0;
	for(int i=0; i<QUICKMENU_BTN_MAX; ++i)
	{
		m_btnQuickMenu[i].Create( this, CTString( "" ), 30 +i*24, 0, 22, 22);

		if (i == QUICKMENU_BTN_SKILL)
		{
			m_btnQuickMenu[i].SetUV( UBS_IDLE, 0, 170, 22, 192, fTexWidth, fTexHeight );
			m_btnQuickMenu[i].SetUV( UBS_CLICK, 0, 196, 22, 218, fTexWidth, fTexHeight );
			nSubSkill = 1;
		}
		else
		{
			m_btnQuickMenu[i].SetUV( UBS_IDLE, (i - nSubSkill) * 26, 222, 21 + (i - nSubSkill) * 26 +1, 244, fTexWidth, fTexHeight );
			m_btnQuickMenu[i].SetUV( UBS_CLICK, (i - nSubSkill) * 26, 248, 21 + (i - nSubSkill) * 26 +1, 270, fTexWidth, fTexHeight );
		}
		m_btnQuickMenu[i].CopyUV( UBS_IDLE, UBS_ON );
		m_btnQuickMenu[i].CopyUV( UBS_IDLE, UBS_DISABLE );
	}

	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetPos(m_btnQuickMenu[QUICKMENU_BTN_RANKING].GetPosX() + 24, 0);

	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].Create( this, CTString( "" )
		,m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].GetPosX() + 24, 0, 22, 22);
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_IDLE, 598, 225, 620, 247, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_CLICK, 598, 249, 620, 271, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetPos(m_btnQuickMenu[QUICKMENU_BTN_RANKING].GetPosX() + 24, 0);

	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].Create( this, CTString( "" )
		,m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].GetPosX() + 24, 0, 22, 22);
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_IDLE, 769, 225, 791, 247, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_CLICK, 769, 249, 791, 271, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_DISABLE );

	// TopUI 하나로 통일 하면서 러시아 상점, 라카렛 라카볼 텍스쳐 위치 다른거 추가 [12/14/2012 Ranma]
#if defined (G_RUSSIA)
	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetUV( UBS_IDLE, 465, 978, 487, 1000, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetUV( UBS_CLICK, 465, 1002, 487, 1024, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetPos(m_btnQuickMenu[QUICKMENU_BTN_RANKING].GetPosX() + 24, 0);

	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].Create( this, CTString( "" )
		,m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].GetPosX() + 24, 0, 22, 22);
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_IDLE, 441, 978, 463, 1000, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_CLICK, 441, 1002, 463, 1024, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].SetPos(m_btnQuickMenu[QUICKMENU_BTN_RANKING].GetPosX() + 24, 0);

	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].Create( this, CTString( "" )
		,m_btnQuickMenu[QUICKMENU_BTN_CASH_SHOP].GetPosX() + 24, 0, 22, 22);
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_IDLE, 489, 978, 511, 1000, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].SetUV( UBS_CLICK, 489, 1002, 511, 1024, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_LAKA_BALL].CopyUV( UBS_IDLE, UBS_DISABLE );

#endif // defined (G_RUSSIA)

	//도움말 대신 랭킹 added by sam 10/11/26
	m_btnQuickMenu[QUICKMENU_BTN_RANKING].SetUV( UBS_IDLE, 623, 225, 645, 247, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_RANKING].SetUV( UBS_CLICK, 623, 249, 645, 271, fTexWidth, fTexHeight );
	m_btnQuickMenu[QUICKMENU_BTN_RANKING].CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuickMenu[QUICKMENU_BTN_RANKING].CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// [090727: selo] 선택되어 있는 퀘스트 표시용 리스트박스	
	m_lbSelectedQuest.Create(0, 1024 - 270, 270, 260, 240, _pUIFontTexMgr->GetFontHeight() + 4, 12, 8, 1, TRUE);
	m_lbSelectedQuest.SetColumnPosX( 0, 1 );
	
	// [090817: selo] 퀘스트 공지 리스트 박스
	m_lbQuestNotice.Create(0, 0, 125, 200, 520, _pUIFontTexMgr->GetFontHeight() + 4, 12, 8, 1, TRUE);
	m_lbQuestNotice.SetColumnPosX(0, 1);
	m_iQuestNoticeMaxStringCnt = 0;
	m_iQuestNoticeLineCnt = 0;
	
	// [090821: selo] 퀘스트 공지 닫기 버튼	
	m_ptdCommonBtnTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	fTexWidth = m_ptdCommonBtnTexture->GetPixWidth();
	fTexHeight = m_ptdCommonBtnTexture->GetPixHeight();
	m_btnQuestNoticeClose.Create( this, CTString( "" ), 0, 0, 16, 16 );	
	m_btnQuestNoticeClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnQuestNoticeClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnQuestNoticeClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuestNoticeClose.CopyUV( UBS_IDLE, UBS_DISABLE );	
	m_btnQuestNoticeClose.SetEnable(FALSE);

	// [090907: selo] 시간 관련
	m_ptdGuildBattleTexture = CreateTexture(CTString("Data\\Interface\\GuildBattle.tex"));
	fTexWidth = m_ptdGuildBattleTexture->GetPixWidth();
	fTexHeight = m_ptdGuildBattleTexture->GetPixHeight();
	for( int nCnt = 0 ; nCnt < 10; nCnt++ )
	{
		m_rtSmallNumber[nCnt].SetUV( nCnt*16, 0, 
								nCnt*16+16, 20, fTexWidth, fTexHeight );

		m_rtLargeNumber[nCnt].SetUV( nCnt*21, 20, 
								nCnt*21+21, 20 + 27, fTexWidth, fTexHeight );					// UV of background		
	}

	m_rtColon.SetUV ( 164, 0, 164 + 9, 20, fTexWidth, fTexHeight );		

	m_ptdExpeditionTexture = CreateTexture( CTString( "Data\\Interface\\Expedition.tex" ) );
	fTexWidth = m_ptdExpeditionTexture->GetPixWidth();
	fTexHeight = m_ptdExpeditionTexture->GetPixHeight();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMinI, pixMinJ );
//	_pUIBuff->SetMyGoodBuffPos( m_nPosX + m_nWidth + 2, 2 );
	_pUIBuff->SetMyGoodBuffPos( m_nPosX + m_nWidth + 44, 2 );

	// UI_REFORM :Su-won
	ResetQuickMenuPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : [sora]
// ----------------------------------------------------------------------------
void CUIPlayerInfo::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_EXPEDITION_END:	// 원정대 해체
			if( bOK )
			{
				GAMEDATAMGR()->GetPartyInfo()->SendExpeditionEnd();
			}

			break;

		case MSGCMD_EXPEDITION_QUIT: // 원정대 탈퇴
			if( bOK )
			{
				GAMEDATAMGR()->GetPartyInfo()->SendExpeditionQuit();
			}

			break;

		case MSGCMD_EXPEDITION_COLLECTITEM: // 원정대 아이템 수집
			if( bOK )
			{
				// 원정대 퀘스트아이템 검색요청 메시지 전송
				_pNetwork->ExpeditionCollectQuestItemReq(_pNetwork->MyCharacterInfo.index);
			}

			break;
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( m_rcQuickMenu.Left, m_rcQuickMenu.Top, m_rcQuickMenu.Right, m_rcQuickMenu.Bottom,
										m_rtQuickMenu.U0, m_rtQuickMenu.V0, m_rtQuickMenu.U1, m_rtQuickMenu.V1,
										0xFFFFFFFF );

	int i;
	for ( i = 0; i < QUICKMENU_BTN_MAX; ++i )
	{
		m_btnQuickMenu[i].Render();
	}
	pDrawPort->FlushRenderingQueue();

	// Render my buff
	_pUIBuff->RenderMyBuff();

	// Tool tip
	if( m_bShowToolTip )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcToolTip.Left + 7, m_rcToolTip.Top,
											m_rcToolTip.Right - 7, m_rcToolTip.Bottom,
											m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcToolTip.Right - 7, m_rcToolTip.Top,
											m_rcToolTip.Right, m_rcToolTip.Bottom,
											m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Text in tool tip
		pDrawPort->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}

	// [090608: selo] 전체화면에 보여질 시간제한 퀘스트의 남은시간을 랜더링 한다.
	RenderTimeAttackRemainTime();
	// [090817: selo] 퀘스트 공지 랜더링 한다.
	RenderQuestNotice();
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ShowToolTip( BOOL bShow, int nToolTipID )
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
		int	nInfoX, nInfoY, nWidth;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case 0:		// Web board
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(B,Alt+B)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(Alt+B)" );

				m_btnBoard.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnBoard.GetWidth();
			}
			break;

		case 1:		// Messanger
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(F,Alt+F)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(Alt+F)" );

				m_btnMessanger.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnMessanger.GetWidth();
			}
			break;

		case 2:		// System menu
			{
#if defined (G_GERMAN)
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Y,Alt+Y)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Y)" );
#else
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Z,Alt+Z)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Z)" );
#endif

				m_btnSysMenu.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnSysMenu.GetWidth();
			}
			break;

		case 3:		// Inventory
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(I,Alt+I)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(Alt+I)" );

				m_btnInven.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnInven.GetWidth();
			}
			break;

		case 4:		// Character information
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(T,Alt+T)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 101, "스테이터스" ), "(Alt+T)" );

				m_btnCharInfo.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnCharInfo.GetWidth();
			}
			break;
		}

		int nInfoWidth;
#if defined(G_THAI)
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY -= 1;

		if( nInfoX < CUIManager::getSingleton()->GetMinI() )
			nInfoX = CUIManager::getSingleton()->GetMinI();

		m_rcToolTip.SetRect( nInfoX, nInfoY - nInfoHeight, nInfoX + nInfoWidth, nInfoY );
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleUIWebBoard()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIWebBoard()
{
	CUIManager::getSingleton()->GetWebBoard()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUIMessanger()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIMessanger()
{
	CUIManager::getSingleton()->GetMessenger()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUISysMenu()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUISysMenu()
{
	CUIManager::getSingleton()->GetSystemMenu()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUIInventory()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIInventory()
{
	CUIManager::getSingleton()->GetInventory()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUICharacterInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUICharacterInfo()
{
	CUIManager::getSingleton()->GetCharacterInfo()->ToggleVisible();
}

// ----------------------------------------------------------------------------
// Name : ToggleUIHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ToggleUIHelp()
{
	CUIManager::getSingleton()->GetHelp()->ToggleVisible();
}

// ----------------------------------------------------------------------------
//  [8/3/2009 sora]
// Name : ClearSelectedQuest()
// Desc : 선택된 퀘스트 리스트 clear
// ----------------------------------------------------------------------------
void CUIPlayerInfo::ClearSelectedQuest()
{
	GAMEDATAMGR()->GetQuest()->ClearSelectedQuestList();
	m_iSelectedQuestLineCnt = 0;
	m_lbSelectedQuest.ResetAllStrings();
}

// ----------------------------------------------------------------------------
//  [6/8/2009 selo]
// Name : RenderTimeAttackRemainTime()
// Desc : 전제화면에 표시할 타임어택 남은시간 랜더한다.
//		  정보는 QuestBookList 에서 얻어온다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderTimeAttackRemainTime()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( TRUE == GAMEDATAMGR()->GetQuest()->IsRaidNow() )
	{
		int nPosX = pUIManager->GetDrawPort()->GetWidth() - 120;
		RenderTime(nPosX, 200, GAMEDATAMGR()->GetQuest()->GetRaidReminTime(), 0xFF0000FF);		
	}	
}

// ----------------------------------------------------------------------------
//  [8/17/2009 selo]
// Name : UpdateQuestNotice()
// Desc : 퀘스트 공지를 갱신 한다
// ----------------------------------------------------------------------------
void CUIPlayerInfo::UpdateQuestNotice(INDEX iQuestIndex)
{
	// [090821: selo] 퀘스트 공지 닫기 버튼
	m_btnQuestNoticeClose.SetEnable(TRUE);

	m_lbQuestNotice.ResetAllStrings();
	m_iQuestNoticeMaxStringCnt = 0;
	m_iQuestNoticeLineCnt = 0;

	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));

	CTString strTemp;
	strTemp.PrintF("%s", qdd.GetDesc3());
	AddQuestNoticeString(strTemp, 0xFFA500FF);
}

// ----------------------------------------------------------------------------
//  [8/17/2009 selo]
// Name : AddQuestNoticeString()
// Desc : 퀘스트 공지 스트링 등록 처리
// ----------------------------------------------------------------------------
void CUIPlayerInfo::AddQuestNoticeString(CTString& strMessage, const COLOR col)
{
	++m_iQuestNoticeLineCnt;

	INDEX	nLength = strMessage.Length();
	if( nLength == 0 )
		return;	

	int iPos;
	for( iPos = 0; iPos < nLength; ++iPos )
	{
		if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
			break;
	}

	if( m_iQuestNoticeMaxStringCnt < iPos )
		m_iQuestNoticeMaxStringCnt = iPos;

	if( iPos == nLength )
	{
		m_lbQuestNotice.AddString(0, strMessage, col, FALSE);
	}
	else
	{
		CTString strTemp, strTemp2;
		strMessage.Split(iPos + 1, strTemp2, strTemp);
		
		m_lbQuestNotice.AddString(0, strTemp2, col, FALSE);

		AddQuestNoticeString(strTemp, col);
	}

}

// ----------------------------------------------------------------------------
//  [8/17/2009 selo]
// Name : RenderQuestNotice()
// Desc : 퀘스트 공지를 그린다
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderQuestNotice()
{
	if( FALSE == m_btnQuestNoticeClose.IsEnabled() )
		return;

	if( 0 == m_iQuestNoticeMaxStringCnt )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	if( 30 < GAMEDATAMGR()->GetQuest()->GetQuestNoticeElapsedTime() )
	{
		m_lbQuestNotice.ResetAllStrings();
		m_iQuestNoticeMaxStringCnt = 0;
		m_iQuestNoticeLineCnt = 0;

		return;
	}

	int	nWidth = ( m_iQuestNoticeMaxStringCnt + 6 ) *
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;

	int			nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	UIRect rcNotice;
	UIRectUV rtBoard;

	pDrawPort->InitTextureData( m_ptdQuestBookTexture );

	rcNotice.Left = nCX - nWidth / 2;
	rcNotice.Right = rcNotice.Left + nWidth;
	rcNotice.Top = 125;
	rcNotice.Bottom = 125 + 13 + (m_iQuestNoticeLineCnt * (_pUIFontTexMgr->GetFontHeight() + 4));	

	rtBoard.SetUV(16, 44, 16+200, 44+250, m_ptdQuestBookTexture->GetPixWidth(), m_ptdQuestBookTexture->GetPixHeight());

	pDrawPort->AddTexture( rcNotice.Left, rcNotice.Top, rcNotice.Right, rcNotice.Bottom,
		rtBoard.U0, rtBoard.V0, rtBoard.U1, rtBoard.V1,
		0xFFFFFFC0 );	
	
	m_lbQuestNotice.SetPosX(rcNotice.Left);

	m_lbQuestNotice.Render();
	
	// 리스트 박스 렌더 마침
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	// [090821: selo] 공지 닫기 버튼
	pDrawPort->InitTextureData(m_ptdCommonBtnTexture);

	m_btnQuestNoticeClose.SetPos(rcNotice.Right - 16 - 4, rcNotice.Top + 4);
	m_btnQuestNoticeClose.Render();

	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
//  [9/7/2009 selo]
// Name : RenderTime()
// 시작시간을 기점으로 흐른 시간 그리기(단위 초)
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderElapsedTime(int nPosX, int nPosY, DOUBLE dStartTime, COLOR col)	
{
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE dTime = dCurTime - dStartTime;
	
	if( dCurTime < 0 )
	{
		RenderTime(nPosX, nPosY, 0, 0, 0, col);
		return;
	}

	int nTime = static_cast<int>(dTime);
	int nSec = nTime % 60;
	nTime /= 60;

	int nMin = nTime % 60;
	int nHour = nTime /= 60;

	RenderTime(nPosX, nPosY, nHour, nMin, nSec, col);
}

// ----------------------------------------------------------------------------
//  [9/7/2009 selo]
// Name : RenderTime()
// 시작시간을 기점으로 남은 시간 그리기(단위 초)
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderRemainTime(int nPosX, int nPosY, DOUBLE dTargetTime, COLOR col)
{
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE dTime = dTargetTime - dCurTime;
	
	if( dCurTime < 0 )
	{
		RenderTime(nPosX, nPosY, 0, 0, 0, col);
		return;
	}

	int nTime = static_cast<int>(dTime);
	int nSec = nTime % 60;
	nTime /= 60;

	int nMin = nTime % 60;
	int nHour = nTime /= 60;

	RenderTime(nPosX, nPosY, nHour, nMin, nSec, col);
}


// ----------------------------------------------------------------------------
//  [9/8/2009 selo]
// Name : RenderTime()
// Desc : 시간을 그린다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderTime(int nPosX, int nPosY, DOUBLE dTime, COLOR col)
{
	if( dTime < 0 )
	{
		RenderTime(nPosX, nPosY, 0, 0, 0, col);
		return;
	}

	int nTime = static_cast<int>(dTime);
	int nSec = nTime % 60;
	nTime /= 60;

	int nMin = nTime % 60;
	int nHour = nTime /= 60;

	RenderTime(nPosX, nPosY, nHour, nMin, nSec, col);	
}

// ----------------------------------------------------------------------------
//  [9/7/2009 selo]
// Name : RenderTime()
// Desc : 시간을 그린다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::RenderTime(int nPosX, int nPosY, int nHour, int nMinute, int nSecond, COLOR col)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdGuildBattleTexture);

	int t10 = 0;
	
	// 시 출력
	t10 = nHour / 10;
	nHour %= 10;

	DrawNumber( nPosX, nPosY, t10, col );
	nPosX += 16;
	DrawNumber( nPosX, nPosY, nHour, col );
	nPosX += 16;
	DrawColon( nPosX, nPosY, col );
	nPosX += 9;

	// 분 출력
	t10 = nMinute / 10;
	nMinute %= 10;

	DrawNumber( nPosX, nPosY, t10, col );
	nPosX += 16;
	DrawNumber( nPosX, nPosY, nMinute, col );
	nPosX += 16;
	DrawColon( nPosX, nPosY, col );
	nPosX += 9;

	// 초 출력
	t10 = nSecond / 10;
	nSecond %= 10;

	DrawNumber( nPosX, nPosY, t10, col );
	nPosX += 16;
	DrawNumber( nPosX, nPosY, nSecond, col );
	nPosX += 16;

	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
//  [9/7/2009 selo]
// Name : DrawNumber()
// Desc : 숫자를 그린다.
// ----------------------------------------------------------------------------
void CUIPlayerInfo::DrawNumber(int nPosX, int nPosY, int nNumber, COLOR col, bool bLarge)
{
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= 16;
	int nHeight			= 20;


	if ( bLarge )
	{
		rtNumber	= m_rtLargeNumber[nNumber];
		nWidth		= 21;	
		nHeight		= 27;
	}
	
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( nPosX, nPosY, nPosX + nWidth, nPosY + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							col );
}

// ----------------------------------------------------------------------------
//  [9/7/2009 selo]
// Name : DrawColon()
// Desc : 콜론을 그린다. 
// ----------------------------------------------------------------------------
void CUIPlayerInfo::DrawColon(int nPosX, int nPosY, COLOR col)
{
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( nPosX, nPosY, nPosX + 9, nPosY + 20,
							m_rtColon.U0, m_rtColon.V0, m_rtColon.U1, m_rtColon.V1,
							col );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPlayerInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Buff mouse message
	if( ( wmsgResult = _pUIBuff->MouseMessageMyBuff( pMsg ) ) != WMSG_FAIL )
	{
		if( wmsgResult == WMSG_COMMAND )
			CUIManager::getSingleton()->RearrangeOrder( UI_PLAYERINFO, TRUE );

		return wmsgResult;
	}

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			m_btnQuestNoticeClose.MouseMessage( pMsg );

			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Web board button
			if( m_btnBoard.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 0 );
				return WMSG_SUCCESS;
			}
			// Messanger button
			else if( m_btnMessanger.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 1 );
				return WMSG_SUCCESS;
			}
			// System menu button
			else if( m_btnSysMenu.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 2 );
				return WMSG_SUCCESS;
			}
			// Inventory button
			else if( m_btnInven.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 3 );
				return WMSG_SUCCESS;
			}
			// Character info button
			else if( m_btnCharInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, 4 );
				return WMSG_SUCCESS;
			}
			for(int i=0; i<QUICKMENU_BTN_MAX; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ShowQuickMenuToolTip( TRUE, i);
					return WMSG_SUCCESS;
				}
			}

			// Hide tool tip
			ShowToolTip( FALSE );
			ShowQuickMenuToolTip(FALSE);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			// [090821: selo] 퀘스트 공지 닫기 버튼
			if( m_btnQuestNoticeClose.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing
			}

			CUIManager* pUIManager = CUIManager::getSingleton();

			for(int i=0; i<QUICKMENU_BTN_MAX; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					pUIManager->RearrangeOrder( UI_PLAYERINFO, TRUE );
					return WMSG_SUCCESS;
				}
			}

			// Quick Menu 영역을 클릭하면 무효화
			if (IsInsideRect(nX, nY, m_rcQuickMenu) == TRUE)
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			// [090821: selo] 퀘스트 공지 닫기 버튼
			if( ( wmsgResult = m_btnQuestNoticeClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					m_btnQuestNoticeClose.SetEnable(FALSE);
				}
				
				return WMSG_SUCCESS;
			}

			for(int i=0; i<QUICKMENU_BTN_MAX; ++i)
			{
				if( m_btnQuickMenu[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					OpenQuickMenu(i);
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
		}
		break;

	case WM_RBUTTONDOWN:
		{
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ProcessShortCut()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIPlayerInfo::ProcessShortCut( MSG *pMsg )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsInputLock() )
		return FALSE;

	// locked short cut. [1/25/2011 rumist]
	if( pUIManager->GetLacaBall()->IsVisible() )
		return FALSE;
	if( pUIManager->GetReformSystem()->IsVisible() ) // 리폼 시스템 UI가 열려 있을 경우 단축키 막음 [9/6/2012 Ranma]
		return FALSE;
	if (pUIManager->GetCashShopEX()->IsVisible()) // 캐시샵 UI가 열려 있을 경우 단축키 입력 막음
		return FALSE;
	if (pUIManager->GetRankingViewEx()->IsVisible() && pUIManager->GetRankingViewEx()->IsEditBoxFocused()) // 랭킹 시스템 보여지고, 에디트박스 포커싱일 경우 입력 막음
		return FALSE;
	if( pMsg->wParam == VK_TAB )
	{
#if (defined(G_JAPAN) || defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2) || defined(G_NETHERLANDS) )
		return FALSE;
#endif
		if (g_InputTabTime > 0)
		{
			INT64 DeltaTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - g_InputTabTime;
			
			if (DeltaTime < 500)
			{
				return TRUE;
			}
		}
		
		g_InputTabTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		CEntity			*penPlEntity;
		CPlayerEntity	*penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
		penPlayerEntity->EnemyTargetSelected_InputTab();
		return TRUE;
	}
	else
	{
		INDEX			iScanCode = ( pMsg->lParam >> 16 ) & 0x1FF;
		extern INDEX	g_iEnterChat;
		extern UBYTE	_abKeysPressed[256];

		CUIManager* pUIManager = CUIManager::getSingleton();

		// Quick slot
		if( _abKeysPressed[KID_LALT] && !pUIManager->IsFocusAllEditBox())
		{
			/****
			//Alt+F1~F3을 눌렀을 때 퀵슬롯 페이지 전환
			if( iScanCode >= 59 && iScanCode <= 61 )
			{
				pUIManager->GetQuickSlot()->ChangePage( iScanCode - 59 );
				return TRUE;
			}
			****/

			//Alt+1~3을 눌렀을 때 퀵슬롯 페이지 전환
			if( //g_iEnterChat && 
				iScanCode >= 2 && iScanCode <= 4 )
			{
				pUIManager->GetQuickSlot()->ChangePage( iScanCode - 2 );
				return TRUE;
			}
		}
		else if( _abKeysPressed[KID_LCONTROL] && !pUIManager->IsFocusAllEditBox())
		{
			if( g_iEnterChat && !pUIManager->IsFocusAllEditBox() )
			{
				switch( iScanCode )
				{
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
					{
						if( pUIManager->GetQuickSlot()->GetPage() == 2 || pUIManager->GetUI(UI_QUICKSLOT3)->IsVisible() )
						{
							pUIManager->GetQuickSlot()->UseQuickSlot( iScanCode - 2 , 2);
							break;
						}
					}
				default:
					return FALSE;
				}
			}
			return TRUE;
		}
		else if( _abKeysPressed[KID_LSHIFT] && !pUIManager->IsFocusAllEditBox())
		{
			switch( iScanCode )
			{
			case 2:
			case 3:
			case 4:
			case 5:			
				pUIManager->GetSummonFirst()->SetCommand( iScanCode - 2 );
				break;			
			case 6:
				pUIManager->GetSummonFirst()->UseSkill();
				break;
			case 44:  // 'z'    
			case 45:  // 'x'    
			case 46:  // 'c'    
			case 47:  // 'v'
				pUIManager->GetSummonSecond()->SetCommand( iScanCode - 44 );
				break;       
			case 48:  // 'b'
				pUIManager->GetSummonSecond()->UseSkill();
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			if( g_iEnterChat && !pUIManager->IsFocusAllEditBox() )
			{
				if( iScanCode >= 2 && iScanCode <= 13 && pUIManager->GetQuickSlot()->GetPage() == 0)
				{
					pUIManager->GetQuickSlot()->UseQuickSlot( iScanCode - 2 , 0);
					return TRUE;
				}
				
				if( pUIManager->GetQuickSlot()->GetPage() == 1 || pUIManager->GetUI(UI_QUICKSLOT2)->IsVisible() )
				{
					if( iScanCode >= 59 && iScanCode <= 68 )
					{
						pUIManager->GetQuickSlot()->UseQuickSlot( iScanCode - 59, 1 );
						return TRUE;
					}
					else if( iScanCode == 87 || iScanCode == 88 )
					{
						pUIManager->GetQuickSlot()->UseQuickSlot( iScanCode - 77, 1 );
						return TRUE;
					}
				}
			}
		}

		// Short cut
		if( (_abKeysPressed[KID_LALT] || g_iEnterChat) && !pUIManager->IsFocusAllEditBox())
		{
			if( !_abKeysPressed[KID_LALT] )
			{
				int	nUIIndex = pUIManager->GetUIIndexByOrder( 0 );
				if( pUIManager->GetUI( nUIIndex )->IsEditBoxFocused() )
					return FALSE;
			}
			switch( iScanCode )
			{
			case 23:		// 'i' - inventory
				pUIManager->GetInventory()->ToggleVisible();
				break;

			case 50:		// 'm' - map
				pUIManager->GetMap()->ToggleVisible();
				break;

			case 20:		// 't' - status
				pUIManager->GetCharacterInfo()->ToggleVisibleStatus();
				break;

			case 24:		// 'o' - Syndicate
				pUIManager->GetCharacterInfo()->ToggleVisibleSyndicate();
				break;

			case 37:		// 'k' - skill
				{
					// ### skill new
					//pUIManager->GetCharacterInfo()->ToggleVisibleSkill();
					pUIManager->GetSkillNew()->OpenUI();
 				}
				break;
 			case 27:// ']'UI 오픈 테스트를 위해서 만들어 놨다 테스트 끝나면 꼭 주석 처리 testcode mail
 				{
 				}
 				break;

			case 36:		// 'j' - action
				{
					pUIManager->GetCharacterInfo()->ToggleVisibleAction();
				}
				break;
				
			case 46:		// 'c' - social
				pUIManager->GetCharacterInfo()->ToggleVisibleSocial();
				break;

			case 34:		// 'g' - group
				pUIManager->GetCharacterInfo()->ToggleVisibleGroup();
				break;

			case 38:		// 'l' - quest
				pUIManager->GetQuestBookList()->ToggleVisible();
				break;

			case 44:		// 'z' - system menu
				pUIManager->GetSystemMenu()->ToggleVisible();
				break;
#if !defined(G_GERMAN) // 독일 로컬 요청으로 "B" 단축키 막음[4/13/2011 ldy1978220]
			case 48:		// 'b' - web board
				pUIManager->GetWebBoard()->ToggleVisible();
				break;	
#endif
			case 33:		// 'f' - messanger
				pUIManager->GetMessenger()->ToggleVisible();
				break;

			case 45:		// 'x' - all UIs
				pUIManager->ToggleVisibleUIs();
				break;

			case 25:	       // 'p' petInfo
				{
					bool bWildPet = false;
					ObjInfo* pInfo = ObjInfo::getSingleton();

					if (pInfo->GetMyApetInfo() != NULL)
					{
						bWildPet = pInfo->GetMyApetInfo()->bIsActive && !pInfo->GetMyApetInfo()->bDeath ? true : false;
					}

					if(bWildPet == true)
					{
						pUIManager->GetWildPetInfo()->ToggleVisible();
					}else
					{
						pUIManager->GetPetInfo()->ToggleVisible();
					}
				}
				break;
				// Short cut Help System.  [9/15/2009 rumist]
			case 53:		// '/' - Help.
				pUIManager->GetHelp()->ToggleVisible();
				break;	
				// short cut about wild pet mount [12/8/2010 rumist]
			case 19:		// 'r' - wild pet mount
				{
					SQUAD llTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - m_llShortCutTime;

					if (llTime >  500)
					{
						m_llShortCutTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
					}
					else
					{
						return FALSE;
					}
					pUIManager->GetCharacterInfo()->UseAction( 50 );
				}
				break;
				// ITS 4377 : ranking system view short cut [10/6/2011 rumist]
				case 47: // 'v' - ranking system view.
				{
					pUIManager->GetRankingViewEx()->ToggleVisible();
				}
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI_REFORM :Su-won
void CUIPlayerInfo::ResetQuickMenuPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	m_rcQuickMenu.SetRect( 0, pixMaxJ-28, 322, pixMaxJ );

	for(int i=0; i<QUICKMENU_BTN_MAX; ++i)
	{
		m_btnQuickMenu[i].SetPosY(pixMaxJ -25);
	}
}

void CUIPlayerInfo::ShowQuickMenuToolTip(BOOL bShow, int nToolTipID)
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
		int	nInfoX, nInfoY, nWidth;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case QUICKMENU_BTN_CHAR_INFO:		// Character information
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 69, "캐릭터 정보창" ), "(T,Alt+T)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 69, "캐릭터 정보창" ), "(Alt+T)" );
			}
			break;
		case QUICKMENU_BTN_INVEN:		// Inventory
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(I,Alt+I)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 166, "인벤토리" ), "(Alt+I)" );
			}
			break;
		case QUICKMENU_BTN_SKILL:		// Skill
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 91, "스킬" ), "(K,Alt+K)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 91, "스킬" ), "(Alt+K)" );
			}
			break;
		case QUICKMENU_BTN_QUEST:		// Quest Book
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S(1700, "퀘스트북" ), "(L,Alt+L)" );
				else
					m_strToolTip.PrintF( "%s %s", _S(1700, "퀘스트북" ), "(Alt+L)" );
			}
			break;
		case QUICKMENU_BTN_PET_INFO:		// Pet Info
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(P,Alt+P)" );
				else
					m_strToolTip.PrintF( "%s %s", _S(2173, "펫 정보창" ), "(Alt+P)" );
			}
			break;
		case QUICKMENU_BTN_MESSENGER:		// Messanger
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(F,Alt+F)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 458, "메신저" ), "(Alt+F)" );
			}
			break;
		case QUICKMENU_BTN_WEB_BOARD:		// Web board
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(B,Alt+B)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 385, "게시판" ), "(Alt+B)" );
			}
			break;
		case QUICKMENU_BTN_SYSTEM_MENU:		// System menu
			{
#if defined(G_GERMAN)
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Y,Alt+Y)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Y)" );
#else
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Z,Alt+Z)" );
				else
					m_strToolTip.PrintF( "%s %s", _S( 299, "시스템" ), "(Alt+Z)" );
#endif
				
			}
			break;
		case QUICKMENU_BTN_RANKING:		// Help
			{
#if defined (G_GERMAN)
				m_strToolTip.PrintF( "%s", _S(5213, "랭킹"));
#else
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S(5213, "랭킹"), "(V,Alt+V)");
				else
					m_strToolTip.PrintF( "%s %s", _S(5213, "랭킹"), "(Alt+V)");
#endif		
			}
			break;
		case QUICKMENU_BTN_CASH_SHOP:		// Cash Shop
			{
				m_strToolTip.PrintF( "%s", _S(2572, "아이리스 상점"));
			}
			break;
		case QUICKMENU_BTN_LAKA_BALL:
			{
				m_strToolTip.PrintF( "%s", _S( 5350, "라카볼"));
			}
			break;
		}

		m_btnQuickMenu[nToolTipID].GetAbsPos( nInfoX, nInfoY );
		nWidth = m_btnQuickMenu[nToolTipID].GetWidth();

		int nInfoWidth;
#if defined(G_THAI)
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		int	nInfoHeight = 22;

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY -= 1;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoX < pUIManager->GetMinI() )
			nInfoX = pUIManager->GetMinI();

		m_rcToolTip.SetRect( nInfoX, nInfoY - nInfoHeight, nInfoX + nInfoWidth, nInfoY );
	}
}

void CUIPlayerInfo::OpenQuickMenu(int nMenu)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	// if activate laca ball, disable all ui. [1/26/2011 rumist]
	if( pUIManager->GetLacaBall()->IsVisible() )
		return;

	switch(nMenu)
	{
		//스테이터스
	case QUICKMENU_BTN_CHAR_INFO:
		ToggleUICharacterInfo();
		break;
		//인벤토리
	case QUICKMENU_BTN_INVEN:
		ToggleUIInventory();
		break;
		// Skill
	case QUICKMENU_BTN_SKILL:
		CUIManager::getSingleton()->GetSkillNew()->OpenUI();
		break;
		//퀘스트북
	case QUICKMENU_BTN_QUEST:
		pUIManager->GetQuestBookList()->ToggleVisible();
		break;
		//펫 정보창
	case QUICKMENU_BTN_PET_INFO:
		{
			if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
			{
				pUIManager->GetWildPetInfo()->ToggleVisible();
			}else
			{
				pUIManager->GetPetInfo()->ToggleVisible();
			}
		}
		
		break;
		//메신저
	case QUICKMENU_BTN_MESSENGER:
		ToggleUIMessanger();
		break;
		//게시판
	case QUICKMENU_BTN_WEB_BOARD:
		ToggleUIWebBoard();
		break;
		//시스템
	case QUICKMENU_BTN_SYSTEM_MENU:
		ToggleUISysMenu();
		break;
		//도움말
	case QUICKMENU_BTN_RANKING:
	// ITS : 4378 [Ranking] 채팅창 하단에 시스템 메뉴에서 랭킹 시스템 버튼 클릭 시 도움말 UI가 나오는 오류 [10/6/2011 rumist]
		pUIManager->GetRankingViewEx()->ToggleVisible();
		break;
		//아이리스 상점
	case QUICKMENU_BTN_CASH_SHOP:
		{
			if (!pUIManager->GetCashShopEX()->IsVisible() && !pUIManager->GetCashShopEX()->GetCashShopLock())
				_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_SHOP_OPEN_REQ);
		}
		break;
	case QUICKMENU_BTN_LAKA_BALL:	// 라카렛
		{
			if( !pUIManager->GetLacaBall()->IsVisible() )
				pUIManager->GetLacaBall()->ToggleVisible();
		}
		break;
	}
}
