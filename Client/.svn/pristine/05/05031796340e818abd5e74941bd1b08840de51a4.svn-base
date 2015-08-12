#include "stdh.h"

// header 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Entities/LevelupGuide.h>
#include <Engine/Entities/SkillTree.h>
#include <Engine/Entities/Skill.h>
#include <Engine/Help/Util_Help.h>

#ifdef	IMPROV1107_NOTICESYSTEM
#define	CHATMSG_NOTICE_MOVEPITCHTIME	300
#define	CHATMSG_NOTICE_VIEWCOUNT		3
#endif

CUIAutoHelp*	_UIAutoHelp = NULL;
extern INDEX g_iCountry;
bool _SAutoHelpInfo::CheckInfo()
{
	// 한번만 하는 거여 .. ~
	if ( m_bJustOnce ) 
	{
		if ( m_bActive )
		{
			return false;
		}
	}

	if ( m_iEndLevel == -1 && m_iStartLevel == -1 )
	{
	
	}
	else
	{
		// FIXME : 헤더 내에서 네트워크를 사용하고 있음.
		SWORD Level = _pNetwork->MyCharacterInfo.level;
					
		// 레벨 체크 
		if ( Level < m_iStartLevel || Level > m_iEndLevel )
		{
			return false;
		}
	}
	// 종족 체크 ( 생략 즐 )

	
	m_bActive = true; 
	return true;
}

// ----------------------------------------------------------------------------
// Name : CUIAutoHelp()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIAutoHelp::CUIAutoHelp()
{
	m_ptdBaseTexture = NULL;
	m_ptdClassification = NULL;
	m_nUpStat = 0;
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIAutoHelp()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIAutoHelp::~CUIAutoHelp()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Clear()
{
	m_bStop				= FALSE;		// 정지 상태인감?
	m_fShowTime			= AUTOHELO_SHOW_TIME;	// 보여 지고 있는 시간
	m_tmStartTime		= 0;			// 보이기 시작 한 시간
	m_bVisible			= FALSE;		// 화면에 보여 주고 있는가?
	m_nActiveIndex		= -1;			// 현제 보여 주고 있는 도움말 Index
	m_tmEndTime			= 0;
	m_tmClassification  = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_tmCurTime			= 0;
	m_nKillMobIndex		= -1;
	m_RndHelp			= FALSE;

	for ( int i = 0; i < AUTOHELP_COUNT; i++ )
	{
		m_AutoHelpInfo[i].Clear();
	}
		
	memset ( m_bCondition, FALSE, sizeof ( BOOL ) * AUTOHELP_COUNT );
#if	defined(IMPROV1107_NOTICESYSTEM)
	m_vGMNotice.clear();
	m_bmovGMNotice		= FALSE;
#endif

	STOCK_RELEASE(m_ptdBaseTexture);
	STOCK_RELEASE(m_ptdClassification);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Create()
{
	Clear ();
	
	// initialize valiables ---------------------wooss 060328-->>
	m_tmCheckTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_bChecked		= FALSE;
	m_iNoticeTime	= 3;
	// --------------------------------------------------------<<

	// Create refine texture
	m_ptdBaseTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Notice.tex" ) );

#if defined(G_KOR) 
		m_ptdClassification = _pTextureStock->Obtain_t (CTString ( "Data\\Interface\\Loading\\Classification_15_kor.tex" ) );
		m_rcClassification.SetRect(0,0,112,131);
		m_rtClassification.SetUV(7,53,120,184,m_ptdClassification->GetWidth(),m_ptdClassification->GetHeight());
#endif


		FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
		FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	m_rcNotice.SetRect( 0, 119, 0, 138 );
	m_rcGMNotice.SetRect( 0, 140, 0, 159 );
	
	
	// Notice
	m_rtNoticeL.SetUV( 365, 131, 397, 150, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 404, 131, 411, 150, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 419, 131, 451, 150, fTexWidth, fTexHeight );

	
	m_AutoHelpInfo[0].SetInfo ( _S( 467, "앉아 있으면 체력이 더 빨리 회복 됩니다." ), 
								1, 2 );

	m_AutoHelpInfo[1].SetInfo ( _S( 468,  "바닥에 떨어진 아이템을 마우스로 클릭하면 집을 수 있습니다." ), 
								1, 2, -1, TRUE );

	m_AutoHelpInfo[2].SetInfo (  _S( 469, "줍기 버튼을 이용하면 더 편리하게 아이템을 집을 수 있습니다." ),
								1, 2, -1, TRUE );

	m_AutoHelpInfo[3].SetInfo (  _S( 470, "스킬마스터를 찾아가면, 현재의 스킬레벨을 올리거나 새로운 스킬을 배울 수 있습니다." ) ,
								1, 3 );

	m_AutoHelpInfo[4].SetInfo (  _S( 471, "드라탄에는 생산지역이 있습니다. 지도를 열어 보세요." ),
								1, 4, -1, TRUE );

	m_AutoHelpInfo[5].SetInfo ( _S( 472, "5레벨이 되시면, 퍼스널 던전에 갈 수 있습니다." ), 
								2, 4 );
	// Quest ...			
	m_AutoHelpInfo[6].SetInfo ( _S( 473, "현재 레벨에서 [약제사 에노린]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								1, 1, -1, TRUE );
	m_AutoHelpInfo[7].SetInfo ( _S( 474, "현재 레벨에서 [경비대장 레온]에게 가면 생산 스킬 퀘스트를 수행할 수 있습니다." ), 
								7, 7,-1, TRUE );
	m_AutoHelpInfo[8].SetInfo ( _S( 475, "현재 레벨에서 트라탄의 [마을 관리인 렌]에게 가면 생산 스킬 퀘스트를 수행할 수 있습니다." ), 
								8, 8, -1,TRUE );
	m_AutoHelpInfo[9].SetInfo ( _S( 476, "현재 레벨에서 [잡화상인 게레스]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								9, 9, -1,TRUE );
	m_AutoHelpInfo[10].SetInfo ( _S( 477, "현재 레벨에서 [경비대장 레온]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								15, 15, -1, TRUE );
	m_AutoHelpInfo[11].SetInfo ( _S( 478, "현재 레벨에서 [방어구상인 로엘]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								23, 23, -1,TRUE );
	m_AutoHelpInfo[12].SetInfo ( _s( "" ), 
								1, 9, -1 );
	// Random Help ...
	m_AutoHelpInfo[13].SetInfo ( _S( 480, "사용하지 않는 무기와 방어구를 알케미스트에게 가져가시면 제련석으로 교환할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[14].SetInfo ( _S( 481, "ALT+B, B키를 누르면 공지사항 및 게시판을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[15].SetInfo ( _S( 482, "ALT+W, W키를 누르면 지도를 통해서 게임 내 NPC 위치를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[16].SetInfo ( _S( 483, "ALT+T, T키를 누르면 공격력 방어력 등의 캐릭터 정보를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[17].SetInfo ( _S( 484, "ALT+A, A키를 누르면 공격/앉기/줍기/교환 아이콘을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[18].SetInfo ( _S( 485, "ALT+S, S키를 누르면 일반, 강화, 특수스킬을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[19].SetInfo ( _S( 486, "ALT+Q, Q키를 누르면 진행중인 퀘스트를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[20].SetInfo ( _S( 487, "ALT+C, C키를 누르면 소셜창을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[21].SetInfo ( _S( 488, "ALT+Z, Z키를 누르면 옵션설정의 시스템창을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[22].SetInfo ( _S( 489, "TAB, ALT+E, E키를 이용하여 캐릭터 인벤토리창을 열 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[23].SetInfo ( _S( 490, "귓속말은 !캐릭터명 하고싶은 말을 통해서 이용할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[24].SetInfo ( _S( 491, "사냥중 앉기를 하면 서있을 때보다 빠르게 HP와 MP가 회복됩니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[25].SetInfo ( _S( 492, "인벤토리창을 연후 아이템을 더블 클릭하면 장착 및 탈착을 할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[26].SetInfo ( _S( 493, "아이템 업그레이드는 제련석을 이용하여 업그레이드할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[27].SetInfo ( _S( 494, "제련석은 사냥을 통해서 획득하거나 란돌성의 알케미스트를 통해서 교환할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[28].SetInfo ( _S( 495, "방향키(화살표를 이용하면 화면이동 및 시점변환을 할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[29].SetInfo ( _S( 496, "다른 존으로 이동하기 위해서는 공간술사 NPC를 통해서 이동할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[30].SetInfo ( _S( 497, "지도의 가장자리를 마우스로 움직이면, 지도의 크기를 확대 축소할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[31].SetInfo ( _S( 498, "지도를 연 상태에서 마을을 클릭하면 마을 상세지도가 나옵니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );


#if defined(G_JAPAN)
		m_AutoHelpInfo[32].SetInfo ( _S( 3042, "일본 2서버 특징" ), RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[33].SetInfo ( _S( 3043, "일본 3서버 특징" ), RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[34].SetInfo ( _S( 3044, "일본 4서버 특징" ), RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
#endif

	
// 070820_ttos: 브라질의 경고문구, 레벨 제한 없이 출력 

#if defined(G_BRAZIL)
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S].SetInfo ( _S( 2547,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+1].SetInfo ( _S( 2548,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+2].SetInfo ( _S( 2549,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+3].SetInfo ( _S( 2550,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S].SetInfo ( _S( 2551,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+1].SetInfo ( _S( 2552,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+2].SetInfo ( _S( 2553,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+3].SetInfo ( _S( 2554,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
	

#else

		m_AutoHelpInfo[AU_NO_TOXICOSIS_S].SetInfo ( _S( 2547,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+1].SetInfo ( _S( 2548,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+2].SetInfo ( _S( 2549,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+3].SetInfo ( _S( 2550,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S].SetInfo ( _S( 2551,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+1].SetInfo ( _S( 2552,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+2].SetInfo ( _S( 2553,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+3].SetInfo ( _S( 2554,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		

#endif
	
	
	m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_RndHelp			= FALSE;
	
}

// ----------------------------------------------------------------------------
// Name : SetInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetInfo ( DWORD dwAutoHelpInfo )
{
	if ( dwAutoHelpInfo < 0 || dwAutoHelpInfo >= AUTOHELP_COUNT ) 
	{
		return;
	}

	TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if ( NowTime - m_tmEndTime < INTERVAL_TIME ) return;

	if( AU_NO_TOXICOSIS_S <= dwAutoHelpInfo && AU_NO_TOXICOSIS_VACATION_E >= dwAutoHelpInfo)
	{
		ShowAutoHelp ( dwAutoHelpInfo );
		return;
	}
	
	switch ( dwAutoHelpInfo )
	{

	case AU_MOB_KILL :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;

				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
			}
	
			float CharHp	= _pNetwork->MyCharacterInfo.hp;
			float CharMaxHp = _pNetwork->MyCharacterInfo.maxHP;

			float hp = ( CharHp / CharMaxHp ) * 100.0f;

			if ( hp <= 40 ) // 체력이 40% 이하이면...
			{
				if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
				{
					ShowAutoHelp ( dwAutoHelpInfo );
				}
				break;
			}
		}
		break;

	case AU_ITEM_DROP :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;
	
				// 초기화
				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
		
			}

			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			m_bCondition[dwAutoHelpInfo] = TRUE;


		}
		break;
	case AU_GET_SKILL_POINT :
		{
			if ( m_bVisible ) return;
			if ( !m_AutoHelpInfo[dwAutoHelpInfo].m_bActive )
			{
				m_AutoHelpInfo[dwAutoHelpInfo].m_bActive = TRUE;
				return ;
			}
		}
		break;	
	// 그 밖에 상황 조건이 안 겹치는 것들 ...
	default :
		{
			if ( m_bVisible && !m_RndHelp )
			{
				return;	
			}
			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			//m_bCondition[dwAutoHelpInfo] = TRUE;
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowAutoHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::ShowAutoHelp ( int nIndex )
{
	if ( nIndex < 0 || nIndex >= AUTOHELP_COUNT ) 
	{
		return;
	}

	// 데이터 초기화
	m_bVisible		= TRUE;
	m_nActiveIndex	= nIndex;
	m_fShowTime		= AUTOHELO_SHOW_TIME;

	int			nWidth = ( m_AutoHelpInfo[m_nActiveIndex].m_strMessage.Length() + 6 ) *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int			nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	// wooss 051019
	// for Thai
	int tv_num = 0;

#if defined(G_THAI)
		nWidth=FindThaiLen(m_AutoHelpInfo[m_nActiveIndex].m_strMessage);
#endif

	m_rcNotice.Left = nCX - nWidth / 2;
	m_rcNotice.Right = m_rcNotice.Left + nWidth;

	

	m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Render ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( STAGEMGR()->GetCurStage() != eSTAGE_GAMEPLAY ) return;


	if ( !m_bVisible )
	{
		static int interval = INTERVAL_TIME + RND_HELP_TIME;
		static bool bFirst = false;
		static bool bTemp = false;

		TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		
#if defined(G_THAI)
			
			int tv_rnd;
			__int64 tv_time = llCurTime - m_tmCheckTime;
			INDEX tv_tmp ;
			time((time_t*)&tv_tmp);
			tm* tv_tm = localtime((time_t*)&tv_tmp);
			if(tv_tm->tm_hour == 3 && tv_tm->tm_min == 55 && tv_tm->tm_sec == 0) {
				tv_rnd = ( rand()% (AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_VACATION_S + 1) ) + AU_NO_TOXICOSIS_VACATION_S;
				SetInfo ( tv_rnd );
			} else {
				if(tv_time/(60*60*1000) != 0){
					if((tv_time/(60*60*1000))%(m_iNoticeTime) == 0 ) {
					m_iNoticeTime +=3;
					tv_rnd = ( rand()% (AU_NO_TOXICOSIS_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
					SetInfo ( tv_rnd );
					}
				}
			}
		 
#endif
#if defined(G_BRAZIL)
		{
			int tv_rnd;
			tv_rnd = (rand()%(AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
			SetInfo ( tv_rnd );
		}
#endif
		if ( NowTime - m_tmEndTime > interval )
		{
			if ( bFirst == false )
			{
				m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				bFirst = true;
				return;
			}


			m_RndHelp = TRUE;
			int rnd;
			
#if defined(G_JAPAN)
			{
				rnd = ( rand()% (AU_RANDOM_HELP_END - AU_RANDOM_HELP_START ) ) + AU_RANDOM_HELP_START;
			}
#else
			{
				rnd = ( rand()% (AU_RANDOM_HELP_END - AU_RANDOM_HELP_START - 3 ) ) + AU_RANDOM_HELP_START;
			}
#endif
			SetInfo ( rnd ); // 1개 이상 일때 수정해주세요..^^
			
			m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			m_RndHelp = FALSE;
		}
		return;
	}
		

	__int64	llCurDelay;

	llCurDelay = llCurTime - m_tmStartTime;

	if( llCurDelay < AUTOHELO_SHOW_TIME )
	{
		COLOR	colBackground = 0x000000FF;
		// tutorial renewal : 헬프 시스템 컬러 변경. [8/3/2010 rumist]
		COLOR	colText = 0xFFFF00FF;
		
		if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;

			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}
	
	
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		// Background
		pDrawPort->AddTexture( m_rcNotice.Left - 32, m_rcNotice.Top,
											m_rcNotice.Left, m_rcNotice.Bottom,
											m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
											colBackground );

		pDrawPort->AddTexture( m_rcNotice.Left, m_rcNotice.Top,
											m_rcNotice.Right, m_rcNotice.Bottom,
											m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
											colBackground );

		pDrawPort->AddTexture( m_rcNotice.Right, m_rcNotice.Top,
											m_rcNotice.Right + 32, m_rcNotice.Bottom,
											m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
											colBackground );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// 도움말 앞뒤로 표시하는 장식을 국가 별로 다르게 설정 합시다~

		CTString	strDecoration = "";

#if defined(G_KOR)
			strDecoration = "♣";
#endif
		
		CTString strTemp = strDecoration + " ";
		strTemp += m_AutoHelpInfo[m_nActiveIndex].m_strMessage + " ";
		strTemp += strDecoration;
		
		// Text in notice region
		pDrawPort->PutTextEx( strTemp, m_rcNotice.Left, m_rcNotice.Top + 4, colText );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		m_bVisible			= FALSE;
		m_nActiveIndex		= -1;

		m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	}
}

// ----------------------------------------------------------------------------
// Name : SetGMNotice()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetGMNotice ( CTString strGMNotice, COLOR colText )
{
	m_strGMNotice = strGMNotice;
	m_colGMTextColor = colText;

	// 데이터 초기화
	m_bShowGMNotice	= TRUE;
	
	int			nWidth;
	
#if defined(G_THAI)
	
	nWidth = FindThaiLen(m_strGMNotice);

#else

	nWidth = m_strGMNotice.Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;	
#endif

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int			nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	m_rcGMNotice.Left = nCX - nWidth / 2;
	m_rcGMNotice.Right = m_rcGMNotice.Left + nWidth;

	m_tmGMNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

#ifdef	IMPROV1107_NOTICESYSTEM
	SGMNotice		sNotice;

	sNotice.strGMNotice			= strGMNotice;
	sNotice.colGMTextColor		= colText;
	sNotice.i64GMFadeTime = sNotice.i64GMNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	sNotice.bCompleteVisible	= FALSE;
	sNotice.rcBackground.Left	= m_rcGMNotice.Left;
	sNotice.rcBackground.Right	= m_rcGMNotice.Right;

	m_vGMNotice.push_back(sNotice);

	if(m_vGMNotice.size() > 1)
	{
		m_bmovGMNotice		= TRUE;
		m_i64movGMNotice	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderGMNotice()
// Desc : 운영자 공지 표시 제거
// ----------------------------------------------------------------------------
void CUIAutoHelp::ClearGMNNotice()
{
#ifdef	IMPROV1107_NOTICESYSTEM
	m_vGMNotice.clear();
	m_bmovGMNotice		= FALSE;
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderGMNotice()
// Desc : 운영자 공지
// ----------------------------------------------------------------------------
void CUIAutoHelp::RenderGMNotice ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	UtilHelp* pUtil = UtilHelp::getSingleton();

	if( m_bShowGMNotice == FALSE )
		return;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmGMNoticeTime;
#ifdef	IMPROV1107_NOTICESYSTEM

	if(m_vGMNotice.size() > 0)
	{
		float	fposYPitchNotice	= 0.0f;

		if(m_bmovGMNotice)
		{
			__int64 llCurMoveDelady	= llCurTime - m_i64movGMNotice;

			if(llCurMoveDelady >= CHATMSG_NOTICE_MOVEPITCHTIME)
			{
				m_bmovGMNotice	= FALSE;
			}

			float	fMoveRatio	= 1.0f - ((float)llCurMoveDelady / (float)CHATMSG_NOTICE_MOVEPITCHTIME);
			if(fMoveRatio < 0.0f)
				fMoveRatio		= 0.0f;
			fposYPitchNotice	= (float)(m_rcGMNotice.Bottom - m_rcGMNotice.Top) * fMoveRatio;
		}

		// 공지 그리기
		int		iNoticeSize	= m_vGMNotice.size();
		std::vector<SGMNotice>::iterator	itNotice	= m_vGMNotice.end() - 1;
		for(int i = 0; i < iNoticeSize; i++)
		{
			SGMNotice&	sNotice			= (*itNotice);
			int			iFadeType		= 0;		// -1 : FadeIN(나타남) 0 : 페이드없음 1 : FadeOUT(사라짐)
			float		fAlphaRatio		= 1.0f;		// 1 = ALPHA 100%, 0 = 0% (FADE)
			COLOR		colBackground	= 0xFFFFFFFF;
			COLOR		colText			= sNotice.colGMTextColor;
			int			iNoticeHeight	= m_rcGMNotice.Bottom - m_rcGMNotice.Top;
			float		fYNotice		= m_rcGMNotice.Top;

			if(i > CHATMSG_NOTICE_VIEWCOUNT)
			{
				if(m_vGMNotice.end() != itNotice)
					itNotice = m_vGMNotice.erase(itNotice);

				continue;
			}
			else if(i == CHATMSG_NOTICE_VIEWCOUNT)
			{	// 허용 라인을 넘었으므로 강제 FADEOUT시키고 제거
				sNotice.i64GMNoticeTime		-= CHATMSG_NOTICE_DELAY;
			}

			if(i > 0)
			{	// 첫번째 라인 (제일 하단) 이 아닐 경우 fposYPitchNotice 값을 영향받는다. (위로 이동한다)
				fYNotice	= m_rcGMNotice.Top - (i * iNoticeHeight);
				fYNotice	+= fposYPitchNotice;
			}
			
			if(llCurTime - sNotice.i64GMNoticeTime > CHATMSG_NOTICE_DELAY)
			{	// 사라질 시간이 다되었다.
				if(sNotice.bCompleteVisible)
				{
					sNotice.i64GMFadeTime		= llCurTime;		// FadeTime 재설정
					sNotice.bCompleteVisible	= FALSE;
				}

				iFadeType		= 1;
			}
			else
			{
				if(!sNotice.bCompleteVisible)		// 아직 보여주지 않은 상태는 무조건 FadeIN 상태
				{
					iFadeType		= -1;
				}
			}

			if(iFadeType == -1)
			{	// FadeIN (나타나는 효과)
				fAlphaRatio			= (float)(llCurTime - sNotice.i64GMFadeTime) / (float)CHATMSG_NOTICE_FADETIME;
				if(fAlphaRatio >= 1.0f)
				{
					fAlphaRatio					= 1.0f;
					sNotice.bCompleteVisible	= TRUE;
				}
			}
			else if(iFadeType == 1)
			{
				//FadeOUT (사라지는 효과)
				fAlphaRatio			-= (float)(llCurTime - sNotice.i64GMFadeTime) / (float)CHATMSG_NOTICE_FADETIME;
				if(fAlphaRatio <= 0.0f)
				{
					fAlphaRatio		= 0.0f;
					itNotice = m_vGMNotice.erase(itNotice);

					if(itNotice == m_vGMNotice.begin())
						break;
				}
			}

			if(fAlphaRatio > 0.0f)
			{
				// TEXT (알파가 먹지를 않아 색채를 뺌, 일정 알파값이 떨어지면 텍스트를 알아서 감춤.)
				if (pUtil != NULL)
				{
					colText			= pUtil->GetColorAlpha(fAlphaRatio, colText);
					colText			= pUtil->GetColorContrast(fAlphaRatio, colText);
					// BACKGROUND
					colBackground	= pUtil->GetColorAlpha(fAlphaRatio, colBackground);
				}
				// Set texture
				pUIManager->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
				// Background
				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Left - 32, fYNotice,
													sNotice.rcBackground.Left, fYNotice + iNoticeHeight,
													m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
													colBackground );

				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Left, fYNotice,
													sNotice.rcBackground.Right, fYNotice + iNoticeHeight,
													m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
													colBackground );

				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Right, fYNotice,
													sNotice.rcBackground.Right + 32, fYNotice + iNoticeHeight,
													m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
													colBackground );

				// Render all elements
				pUIManager->GetDrawPort()->FlushRenderingQueue();

				// Text in notice region
				pUIManager->GetDrawPort()->PutTextEx( sNotice.strGMNotice, sNotice.rcBackground.Left, fYNotice + 4, colText );

				// Flush all render text queue
				pUIManager->GetDrawPort()->EndTextEx();
			}

			if(itNotice == m_vGMNotice.begin())
				break;

			itNotice--;
		}
	}
	else if(m_vGMNotice.size() <= 0)
		m_bShowGMNotice = FALSE;

#else	// #ifdef	IMPROV1107_NOTICESYSTEM
	if( llCurDelay < CHATMSG_NOTICE_DELAY )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = m_colGMTextColor;
		if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;

			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}

		// Set texture
		pUIManager->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		// Background
		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Left - 32, m_rcGMNotice.Top,
											m_rcGMNotice.Left, m_rcGMNotice.Bottom,
											m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
											colBackground );

		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Left, m_rcGMNotice.Top,
											m_rcGMNotice.Right, m_rcGMNotice.Bottom,
											m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
											colBackground );

		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Right, m_rcGMNotice.Top,
											m_rcGMNotice.Right + 32, m_rcGMNotice.Bottom,
											m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
											colBackground );

		// Render all elements
		pUIManager->GetDrawPort()->FlushRenderingQueue();

		// Text in notice region
		pUIManager->GetDrawPort()->PutTextEx( m_strGMNotice, m_rcGMNotice.Left, m_rcGMNotice.Top + 4, colText );

		// Flush all render text queue
		pUIManager->GetDrawPort()->EndTextEx();
	}
	else
		m_bShowGMNotice = FALSE;
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

}

void CUIAutoHelp::ClassificationShowRender()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( STAGEMGR()->GetCurStage() != eSTAGE_GAMEPLAY ) 
		return;

	if( (llCurTime - m_tmClassification) > CLASSIFICATION_TIME && !m_bShowClassification )
	{ // 한 시간에 한번 보여주자.(15세 이용가)
		m_tmCurTime = llCurTime;
		m_bShowClassification = TRUE;
	}

	if( m_bShowClassification == FALSE )
		return;

	// 보여주자
	if( llCurTime - m_tmCurTime < 3000 )	// 3초 가량만
	{
		CDrawPort* pDrawPort = pUIManager->GetDrawPort();

		pDrawPort->InitTextureData( m_ptdClassification );

		PIX pixdpw = pDrawPort->GetWidth()*0.15;
		FLOAT fPersent		= 1.0f;
		fPersent = (FLOAT)pixdpw / m_rcClassification.GetWidth();

		INDEX fWidth = m_rcClassification.GetWidth();//*fPersent;
		INDEX fHeigth = m_rcClassification.GetHeight();//*fPersent;
		INDEX fPosX = pDrawPort->GetWidth() - ( fWidth+150 ) ; // 150은 레이더 크기
		INDEX fPosY = ( fHeigth / 4 );

		pDrawPort->AddTexture( fPosX, fPosY,
												fPosX + fWidth, fPosY + fHeigth, 
												m_rtClassification.U0, m_rtClassification.V0, m_rtClassification.U1, m_rtClassification.V1,
												0xFFFFFFFF );

		pDrawPort->FlushRenderingQueue();
	}
	else
	{ // 3초 이상은 안돼
		m_tmClassification = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_bShowClassification = FALSE;
	}
}


void CUIAutoHelp::initialize()
{
// 	int			i;
// 	CUIImage*	pImg = NULL;
// 
// 	const char* str_notice[] = {
// 		"notice_l",
// 		"notice_c",
// 		"notice_r",
// 	};
// 
// 	for( i = 0; i < 3; ++i )
// 	{
// 		pImg = (CUIImage*)findUI( str_notice[i] );
// 
// 		if( pImg )
// 		{
// 			addChild( pImg );
// 		}
// 	}

}

void CUIAutoHelp::GetLvGuidString( int lv )
{
	if (CLevelUpGuide::getData(lv) == NULL)
		return;

	CTString strTmp = CUIBase::getText(CLevelUpGuide::getData(lv)->strIndex);

	if (strTmp.IsEmpty() == TRUE)
		return;

	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strTmp, SYSMSG_USER);
}

void CUIAutoHelp::LearnTheSkill( int lv )
{
	int nCnt = 0;
	
	int nJob = _pNetwork->MyCharacterInfo.job;
	int _2ndJob = _pNetwork->MyCharacterInfo.job2;
	int _tmp2ndJob = 0;
	int nIndex = 0;

	nCnt = CSkillTree::m_nSkillCount[nJob][0];

	if (_2ndJob > 0)
		nCnt += CSkillTree::m_nSkillCount[nJob][_2ndJob];

	for (int i = 0; i < nCnt; ++i)
	{
		if (i > CSkillTree::m_nSkillCount[nJob][0])
			_tmp2ndJob = _2ndJob;

		nIndex = CSkillTree::m_SkillTree[nJob][_tmp2ndJob][i/8].index[i%8];

		CSkill& rSkill = _pNetwork->GetSkillData(nIndex);

		for (int j = 0; j < rSkill.GetMaxLevel(); ++j)
		{
			if (rSkill.GetLearnLevel(j) == lv)
			{
				CTString strTmp;
				strTmp.PrintF(_S(6213, "새로운 스킬 [%s]의 %d레벨을 배울 수 있습니다."), rSkill.GetName(), j + 1);

				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strTmp, SYSMSG_USER);
				break;
			}
		}
	}
}

void CUIAutoHelp::SetStatpoint( int old, int cur )
{
	m_nUpStat = 0;

	if (old >= cur)
		return;

	m_nUpStat = cur - old;
}
