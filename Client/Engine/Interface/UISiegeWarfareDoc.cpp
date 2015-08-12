#include "stdh.h"


#include <map>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISignboard.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Object/ActorMgr.h>


#define MUSIC_CASTLE_BATTLE		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_battletheme.ogg")
#define MUSIC_MERAC_FIELD		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_meracpease.ogg")

// 전체 공성 시간 중에 야전 종료후 진행되는 공성 시간 
// 원래는 전체 공성 시간 60분 에서 야전 시간 15 + 공성 시간 45분으로 되어있기 때문에 
// CASTLETIME은 45로 설정 되어야 하지만 현재 테스트를 위해서 
// 전체 공성 시간 20 = 야전 10 + 공성 10으로 설정되어 있게 때문에 10으로 설정 하여 놓았음. 
// 서버와 시간 조정시 조정 바람.
#define	CASTLETIME			45 // 분 

// 공성 시작과 야전 시작시 Dealy 시간
#define	DEALY_TIME			3500 //초	 

CUISiegeWarfareDoc* _pUISWDoc = NULL;

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::CUISiegeWarfareDoc
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfareDoc::CUISiegeWarfareDoc()
{
	Clear();
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::~CUISiegeWarfareDoc
// Explain:  
// Date : 2005-07-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfareDoc::~CUISiegeWarfareDoc()
{
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::Clear
// Explain:  
// Date : 2005-07-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::Clear()
{
	m_nUIState		= SWS_NONE;
	m_nSWTime		= 0;	
	m_lLeftTime		= 0;
	m_tmLeftTime	= 0;
	
	for( int i = 0; i < 3; i++ )
		m_gdGuild[i].SetGuildInfo( -1, " ", 0 );
	
	m_nGuildPoint = 0;
	
	m_nMonth = 0; m_nDay = 0; m_nHour = 0; m_nMin = 0;	
	
	m_mapEG.clear();
	m_mapEGGuildMaster.clear();
	m_mapEGConsensus.clear();  // WSS_DRATAN_SIEGEWARFARE 2007/10/14
	m_bDelay = false;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetUIState
// Explain:  UI 상태를 지정합니다.
// 전투중인 경우에는 CNetwork의 ga_bGuildWar플레그를 조정합니다.
// Date : 2005-07-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetUIState( int nUIState ) 
{
	_pNetwork->ga_bGuildWar = FALSE;
	
	if( nUIState == SWS_SELECT_BATTLE || nUIState == SWS_BATTLE )
	{
		_pNetwork->ga_bGuildWar = TRUE;
	}
	
	m_nUIState = nUIState;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::GetTime
// Explain:  공성정보 요청후 UI를 통해 정보 출력
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::GetTime( int nGuildIndex, CTString strGuildName, int nMonth, int nDay, int nHour, int nMin )
{
	m_gdDefGuild.SetGuildInfo( nGuildIndex, strGuildName );
	
	m_nMonth			= nMonth;
	m_nDay				= nDay;
	m_nHour				= nHour;
	m_nMin				= nMin;
	
	CUIManager::getSingleton()->GetSiegeWarfare()->OpenSWInfo();
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::GuildWarStart
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::GuildWarStart( int nZoneIndex,  int nRemainSec )	
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(nZoneIndex == 7)
	{
		// FIXME : 문 닫아주기.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );	

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;	

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_CASTLE_BATTLE );

		Clear();
		
		_pNetwork->SendCastleMapRecent();

		m_nZoneIndex = nZoneIndex;
			
		CTString strMessage,  strMessage2;
		
		strMessage.PrintF(_S(2012, "[%s]지역 [%s]공성이 시작됩니다." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ));	
		strMessage2.PrintF(_S(2013,  "[%s]지역 내에 있는 공성 참여 유저들은 공성 시작 지점으로 이동합니다."), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ));	

		// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
		// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage, strMessage2 );
		pUIManager->GetSiegeWarfare()->OpenSelectBattle();
		
		RestartEffect(nZoneIndex);
		RestartGuildMasterEffect();
		SetLeftTime( nRemainSec );

		m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();


		//SetDealy();	
	}
	else if( nZoneIndex == 4)
	{
		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: 드라탄 공성 시작 루틴
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );
		CTString strMessage;
		strMessage.PrintF( _S( 2012, "[%s]지역 공성이 시작됩니다." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ));	
		// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
		// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage );

		m_nZoneIndex = nZoneIndex;
		RestartEffect(nZoneIndex);
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartCastle
// Explain: 야전끝 공성 시작 알림(공지) 
// Date : !2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartCastle( int nZoneIndex, int nRemainSec, int GuildIndex1, CTString GuildName1, int GuildIndex2, CTString GuildName2, int GuildIndex3, CTString GuildName3 )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(nZoneIndex == 7)
	{	
		// 공성이 시작되면 모든 문을 닫아 둔다.
		// FIXME : 문 닫아주기.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );	

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_CASTLE_BATTLE );

		_pNetwork->SendCastleMapRecent();
		
		m_nZoneIndex = nZoneIndex;
		
		m_gdGuild[0].SetGuildInfo( GuildIndex1, GuildName1 );	
		m_gdGuild[1].SetGuildInfo( GuildIndex2, GuildName2 );
		m_gdGuild[2].SetGuildInfo( GuildIndex3, GuildName3 );
		
		CTString strMessage1, strMessage2;
		for( int i = 0; i< 3; i++ )
		{
			if( m_gdGuild[i].m_nGuildIndex > 0 )
			{
				if( i >= 1 )
				{
					strMessage1 += ", ";
				}
				strMessage1 += m_gdGuild[i].m_strGuildName;
				strMessage1 += CTString(" ");
				strMessage1 += _S(75 , "길드" );	
			}
		}
		
		strMessage1 += _S( 2014, "가 공성측 길드로 선정 되었습니다." );	
		strMessage2.PrintF(_S( 2015,  "성주의 권좌가 얼마 남지 않았습니다. 진격하십시오." ) );		
		// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
		// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage1, strMessage2 );
		
		pUIManager->GetSiegeWarfare()->OpenBattle();
		
		RestartEffect(nZoneIndex);
		RestartGuildMasterEffect();
		SetLeftTime( nRemainSec );

		//SetDealy();
	}
	else if(nZoneIndex ==4 )
	{
		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: 드라탄 공성 시작 루틴
		// 공성이 시작되면 모든 문을 닫아 둔다.
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );

		CTString strMessage =_S( 3684,"공성이 시작되었습니다." );		
		// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
		// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
		CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );
		pUIManager->GetSiegeWarfareNew()->SetWarState(TRUE);

		m_nZoneIndex = nZoneIndex;
		RestartEffect(nZoneIndex);
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::UpdatePoint
// Explain:  전투 중 갱신되는 점수
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::UpdatePoint( int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint )
{
	m_gdGuild[0]	= Guild1;
	m_gdGuild[1]	= Guild2;
	m_gdGuild[2]	= Guild3;
	
	m_nGuildPoint = nMyPoint;
	m_gdDefGuild.m_nPoint = nDefensPoint;

	SetLeftTime( nRemainSec );
}

void CUISiegeWarfareDoc::SetLeftTime ( int nRemainSec )
{
	m_lLeftTime = nRemainSec;

	if( IsSelectBattle() )
	{
		m_lLeftTime -= MINUTE_TO_SEC( CASTLETIME );
	}

	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RemainFiledTime
// Explain: 야전전투가 종료되기 5분전부터 1분 간격으로 공성에 참여한 유저에게 전체공지 
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RemainFiledTime( int nZoneIndex, int nRemainSec )
{
	m_nZoneIndex	= nZoneIndex;
	SetLeftTime( nRemainSec );
	
	CTString strMessage;
	strMessage.PrintF(_S( 2016,  "[%s]지역 [야전전투]가 %d분 남았습니다." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), (m_lLeftTime) / 60);	
	// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
	// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::TimeConfirm
// Explain:  공성 시간 확정 알림(공지)
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::TimeConfirm( int nZoneIndex, int nMonth, int nDay, int nHour, int nMin )
{
	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage;
	strMessage.PrintF( _S( 2017, "[%s]지역 [%s]공성이 %d월 %d일 %d시로 예정되었습니다." ),	
		CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), nMonth, nDay, nHour );	
	// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
	// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RemainStartTime
// Explain: 공성 시작 까지 남은 시간을 시작 10분전부터 1분간격으로 공지
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RemainStartTime( int nZoneIndex, int nMin )
{
	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage;
	
	strMessage.PrintF( _S( 2018, "[%s]지역 [%s]공성이 %d분 남았습니다." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), nMin );	
	// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
	// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::CastleState
// Explain:  공성 상황 알림(공지) 및 전투상황 셋팅
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint )
{
	// Date : 2005-07-22(오전 10:00:32), By Lee Ki-hwan 
	// 비 참가자인 경우에는 
	SetGateState( 0, nGateState ); 
	
	if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return;
	
	m_nZoneIndex	= nZoneIndex;
		
	m_gdGuild[0]	= Guild1;
	m_gdGuild[1]	= Guild2;
	m_gdGuild[2]	= Guild3;
	
	m_nGuildPoint	= nMyPoint;
	m_gdDefGuild.m_nPoint = nDefensPoint;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( nState == WCSF_WAR_FIELD )
	{
		pUIManager->GetSiegeWarfare()->OpenSelectBattle();
	}
	else if( nState == WCSF_WAR_CASTLE )
	{
		pUIManager->GetSiegeWarfare()->OpenBattle();
	}
	else
	{
		return;
	}
	
	SetLeftTime( nRemainSec );
	RestartEffect(nZoneIndex);
	RestartGuildMasterEffect();
}


//------------------------------------------------------------------------------
// WSS_DRATAN_SIEGEWARFARE 070725
// Explain:  공성 상황 알림(공지) 및 전투상황 셋팅 for Dratan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec)
{
	SetGateState( 0, nGateState ); 
	
	if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_NONE )
		return;
	
	m_nZoneIndex	= nZoneIndex;
		
	if( nState == WCSF_WAR_CASTLE )
	{
		// 공성중 세팅
		CUIManager::getSingleton()->GetSiegeWarfareNew()->SetWarState(TRUE);
	}
	else return;
	
	SetLeftTime( nRemainSec );
	RestartEffect(nZoneIndex);
	RestartGuildMasterEffect();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::EndWar
// Explain:  메세지 표시후 공성 종료
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::EndWar( int nZoneIndex, int nWinDefense, int nOwnerGuildIndex, CTString strOwnerGuildName, int nOwnerIndex, CTString strOwnerName, int nNextMonth, int nNextDay, int nNextWeek, int nNextHour )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	INDEX	i;
	if( nZoneIndex == 7 )
	{
		SetUIState( SWS_END );	
		// 공성이 끝나면 모든 문을 열어 둔다.
		// FIXME : 문 열어주기.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, TRUE );

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;	

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_MERAC_FIELD );	

		// 이미지 출력 추가 
		_UISignBoard->ShowSingBoard ( 12, 7 );
			
		m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		_pNetwork->MyCharacterInfo.sbJoinFlagMerac = WCJF_NONE;
		
		// Start My Guild, Enemy Guild Effect Start
		ACTORMGR()->SetJoinFlagMerac(WCJF_NONE);
	}
	else if( nZoneIndex == 4)
	{		
		// WSS_DRATAN_SEIGEWARFARE 2007/10/1 -------------------------------->>
		// 공성 종료 관련 수정...
		SetUIState( SWS_END ); // WSS_DRATAN_SEIGEWARFARE 2007/10/11 
		
		if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_NONE )
			return;	
		// WSS_DRATAN_SIEGEWARFARE 2007/10/16
		//_pNetwork->MyCharacterInfo.sbJoinFlagDratan = WCJF_NONE;
		
		// Start My Guild, Enemy Guild Effect Start
		ACTORMGR()->SetJoinFlagMerac(WCJF_NONE);

		// -------------------------------------------------------------------<<

		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: 드라탄 공성 종료 루틴
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, TRUE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, TRUE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, TRUE );	

		pUIManager->GetSiegeWarfareNew()->SetWarState(FALSE);

		// WSS_DRATAN_SEIGEWARFARE 2007/08/14 -------------------------------->>
		// 390 ~ 399 공성 부활진지 초기화
		CTString tStr;
		for( i = 0; i < 10; i++)
		{	
			INDEX tNpcIdx = 390+i;
			_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = -1;
			_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = CTString("");

			// Hard Cording ^^;; 어쩔수 없이....
			// 몹 이름 변경
			CMobData* MD = CMobData::getData(tNpcIdx);
			if(MD->GetMobIndex()>0)
			{
				tStr.PrintF(_S( 3685,"공성 부활진지%d"),tNpcIdx-389);
				MD->SetName(tStr);
			}		
		}	
		// -------------------------------------------------------------------<<

		if( pUIManager->GetSiegeWarfareNew()->IsEnabled()&&
			pUIManager->GetSiegeWarfareNew()->IsVisible() )
		{
			pUIManager->RearrangeOrder(UI_SIEGE_WARFARE_NEW,FALSE);
		}
		
		// WSS_DRATAN_SIEGEWARFARE 2007/10/17
		// 교감중 공성 종료 처리
		if( _pNetwork->MyCharacterInfo.bConsensus )
		{
			pUIManager->GetSiegeWarfareNew()->StopConsensus(_pNetwork->MyCharacterInfo.index);
		}			
		StopConsensusEffect(0,TRUE);
		
		// WSS_DRATAN_SIEGEWARFARE 2007/10/18
		// 상태 초기화
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayerInit(false);

	}

	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage1;
	CTString strMessage2;
	
	// 공성 끝났면 지도는 초기화 
	pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 0 );
	pUIManager->GetMap()->ReSetData();

	if( nWinDefense )
	{
		if( strOwnerGuildName.Length() <= 0 )
		{
			strMessage1.PrintF( _S( 2019, "[%s]지역 [%s]공성에 실패 하였습니다." ), 
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ) );	
		}
		else
		{
			strMessage1.PrintF( _S(2020 , "[%s]지역 [%s]공성이 종료되었습니다. [%s]길드가 수성에 성공하였습니다." ),
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), strOwnerGuildName );		
		}
		
	}
	else
	{
		strMessage1.PrintF( _S(2021 , "[%s]지역 [%s]공성이 종료되었습니다. 새로운 성주로 [%s]길드의 [%s]길드장이 선출 되었습니다." ), 
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), strOwnerGuildName, strOwnerName );	
	}
	
	strMessage2.PrintF(_S( 2022, "다음 공성전 신청은 %d월 %d일 %s요일 오후 %d시부터 신청이 가능합니다." ), 
		nNextMonth, nNextDay, pUIManager->GetWeekToString( nNextWeek ), nNextHour );	
	// [2012/06/04 : Sora] ITS 8986 공성시작전 공성시작 지점으로 이동 시 캐릭터 이동불가 상태 발생 수정
	// 공성 메시지 출력하면서 해당 UI를 활성화 시켜주어햐 한다. 공성 메시지 출력 방식 변경
	pUIManager->GetSiegeWarfare()->SetNotice( strMessage1, strMessage2 );
	
	// Date : 2005-11-18(오후 4:27:13), By Lee Ki-hwan
	// 메세지 박스 표시 추가 
	pUIManager->CloseMessageBox( MSGCMD_NULL );
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2341, "공성 종료" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage1 );
	pUIManager->CreateMessageBox( MsgBoxInfo );
		
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetTimeRep
// Explain:  
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetTimeRep( int nMonth, int nDay, int nHour, int nMin )
{
	CTString strMessage;
	strMessage.PrintF(_S(2147 ,  "공성 시간이 %d월 %d일 %d시로 설정되었습니다." ), nMonth, nDay, nHour );  
	CUIManager::getSingleton()->GetSiegeWarfare()->Message( MSGCMD_SIEGE_WARFARE_INFO, strMessage, UMBS_OK  );
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-07-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::InvalidCommand( int nReqMsgType )
{
	static int nMaxMsgType = 17;
	int nReqMsg = nMaxMsgType;
	
	
	static  int naReqMsgType[17] = { 
		MSG_GUILD_CREATE,				// 0
			MSG_GUILD_LEVELUP,
			MSG_GUILD_BREAKUP,
			MSG_GUILD_REGIST_REQ,
			MSG_GUILD_REGIST_ALLOW,
			MSG_GUILD_REGIST_CANCEL,		// 5
			MSG_GUILD_OUT_REQ,
			MSG_GUILD_KICK,
			MSG_GUILD_CHANGE_BOSS,
			MSG_GUILD_APPOINT_OFFICER,		// 9
			MSG_GUILD_FIRE_OFFICER,
			MSG_GUILD_BATTLE_REQ_REQ,
			MSG_GUILD_BATTLE_REQ_REJECT,
			MSG_GUILD_BATTLE_REQ_ACCEPT,
			MSG_GUILD_BATTLE_STOP_REQ,
			MSG_GUILD_BATTLE_STOP_REJECT,	// 15
			MSG_GUILD_BATTLE_STOP_ACCEPT,	// 16
	};
	
	for( int i = 0; i < nMaxMsgType; i++ )
	{
		if( naReqMsgType[i] == nReqMsgType )
		{
			nReqMsg = i;
			break;
		}
	}
	
	
	static CTString strReqMsgType[] = {
	_S( 2023, 	"공성전 중에는 길드를 생성할 수 없습니다." ),					// MSG_GUILD_CREATE			
		_S( 2024, 	"공성전 중에는 길드를 레벨업할 수 없습니다." ),				// MSG_GUILD_LEVELUP	
		_S(2025 , 	"공성전 중에는 길들를 해체할 수 없습니다." ),					// MSG_GUILD_BREAKUP	
			_S( 2026, "공성전 중에는 길드 등록 신청을 할 수 없습니다." ),			// MSG_GUILD_REGIST_REQ	
			_S(2027 , "공성전 중에는 길드 등록 수락을 할 수 없습니다." ),			// MSG_GUILD_REGIST_ALLOW	
		_S( 2028, 	"공성전 중에는 길드 등록 취소를 할 수 없습니다." ),			// MSG_GUILD_REGIST_CANCEL	
			_S( 2029, "공성전 중에는 길드 탈퇴 신청을 할 수 없습니다." ),			// MSG_GUILD_OUT_REQ	
			_S( 2030, "공성전 중에는 길드 퇴출을 할 수 없습니다." ),				// MSG_GUILD_KICK	
			_S( 2031, "공성전 중에는 길드장을 변경할 수 없습니다." ),				// MSG_GUILD_CHANGE_BOSS	
		_S(2032 , 	"공성전 중에는 부단장을 임명할 수 없습니다." ),				// MSG_GUILD_APPOINT_OFFICER	
		_S( 2033, 	"공성전 중에는 부단장을 해임할 수 없습니다." ),				// MSG_GUILD_FIRE_OFFICER	
			_S( 2034, "공성전 중에는 길드전투 신청을 할 수 없습니다." ),			// MSG_GUILD_BATTLE_REQ_REQ	
			_S( 2035, "공성전 중에는 길드전투 신청을 거절할 수 없습니다." ),		// MSG_GUILD_BATTLE_REQ_REJECT	
			_S( 2036, "공성전 중에는 길드전투 신청을 수락할 수 없습니다." ),		// MSG_GUILD_BATTLE_REQ_ACCEPT	
		_S( 2037, 	"공성전 중에는 길드전투 중단 신청을 할 수 없습니다." ),		// MSG_GUILD_BATTLE_STOP_REQ	
			_S( 2038, "공성전 중에는 길드전투 중단 신청을 거절할 수 없습니다." ),	// MSG_GUILD_BATTLE_STOP_REJECT	
		_S( 2039, 	"공성전 중에는 길드전투 중단 신청을 수락할 수 없습니다." ),	// MSG_GUILD_BATTLE_STOP_ACCEPT	
			"UnKnow Message"
	};
	
	CUIManager::getSingleton()->GetSiegeWarfare()->ErrorMessage( strReqMsgType[nReqMsg-1] );
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetGateState
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetGateState( int nOldGatesate, int nGateState )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 0, 1 왼쪽문
	if (g_slZone == 7) //  메라크
	{
		if( (nGateState & ( 1 << 0 )) && (nGateState & ( 1 << 1 )))
		{		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, TRUE );
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		}
		
		// 2, 3 오른쪽문
		if( (nGateState & ( 1 << 2 )) && (nGateState & ( 1 << 3 )))
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, TRUE );
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		}
		
		// 4, 5 중앙문.
		if( (nGateState & ( 1 << 4 )) && (nGateState & ( 1 << 5 )))
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, TRUE );
			
			// 내 성문 열림
			if (nOldGatesate != nGateState)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 2040, "공성측이 내성문까지 진격하였습니다. 수성측은 권좌를 보호하십시오." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );
		}
	}

// WSS_DRATAN_SIEGEWARFARE 070723
	// 드라탄 외문 오른편
	if (g_slZone == 4)	// 드라탄
	{
		if( (nGateState & ( 1 << 6 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, TRUE );
			// 외 성문 열림
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 공성중이면...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"외성문이 파괴되었습니다." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		}
		// 드라탄 내문 2
		if( (nGateState & ( 1 << 7 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, TRUE );
			
			// 내 성문 열림
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 공성중이면...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(  3686,"공성측이 내성문까지 진격하였습니다. 수성측은 크리스탈을 보호하십시오." ) ); 
			}
			
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		}
		// 드라탄 내문 3
		if( (nGateState & ( 1 << 8 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, TRUE );
			
			// 내 성문 열림
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 공성중이면...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(  3686,"공성측이 내성문까지 진격하였습니다. 수성측은 크리스탈을 보호하십시오." ) ); 
			}
			
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );
		}
		// 드라탄 외성문 왼편
		if( (nGateState & ( 1 << 9 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, TRUE );
			// 외 성문 열림
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 공성중이면...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"외성문이 파괴되었습니다." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		}
		// 드라탄 외성문 가운데
		if( (nGateState & ( 1 << 10 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, TRUE );
			// 외 성문 열림
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 공성중이면...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"외성문이 파괴되었습니다." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  지역에 따른 성이름 ... 하드코딩
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISiegeWarfareDoc::GetCastleName( int nZoneIndex )
{	
	CTString strCastleName = _S( 2041, "공성없음" );	
	switch( nZoneIndex )
	{
	case 4://드라탄 // WSS_DRATAN_SIEGEWARFARE 070723
		strCastleName = _S(  3687, "카오스" ); 
		break;
	case 7://메라크
		strCastleName = _S( 2042, "칸단트" ); // 미리 공간 확보 해 놓기(20)개 정도
		break;
	}
	
	return strCastleName;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::ErrorProc
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::ErrorProc( int nErrorCode, UBYTE ubMsgType )
{
	static CTString strErrorMessage[MSG_GUILD_WAR_ERROR_TOTAL]	= 
	{
	_S( 2043, 	"다 잘 됐네여" ),	
		_S( 2044, 	"이미 공성이 진행중 입니다." ),		
		_S( 2045, 	"수성측 길드는 공성 신청을 할 수 없습니다." ),	
			_S( 2046, "길드에 가입되어 있지 않으면 공성 신청을 할 수 없습니다." ),	
			_S( 2047, "이미 공성/수성 신청을 되어있습니다." ),	
			_S( 2048, "공성 가능 시간이 아닙니다." ),	
			_S( 2049, "길드 레벨 부족하여 공성 신청을 할 수 없습니다." ),	
			_S( 2050, "길드 인원이 부족하여 공성 신청을 할 수 없습니다." ),	
			_S( 2051, "영웅의 증표가 없으면 공성 신청을 할 수 없습니다." ),	
			_S( 2052, "나스가 부족하여 공성 신청을 할 수 없습니다." ),	
			_S( 2053, "성주가 없는 성에는 수성 용병 신청을 할 수 없습니다." ),	
			_S( 2054, "길드의 방침에 따라 개인 용병 신청을 할 수 없습니다." ),	
			_S( 2055, "레벨이 부족하여 개인 용병 신청을 할 수 없습니다." ),	
			_S( 2056, "성주가 아니면 공성 설정을 변경할 수 엇습니다." ),	
			_S( 2057, "공성 가능 시간이 아닙니다." ),	
			_S( 2057, "공성 가능 시간이 아닙니다." ),
			_S( 1688, "조건이 맞지 않습니다."),		
	};

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 성공 처리
	if( nErrorCode == MSG_GUILD_ERROR_OK && ubMsgType != -1 )
	{
		CTString strMessage;
		switch ( ubMsgType )
		{
		case MSG_GUILD_WAR_JOIN_ATTACK_GUILD: // 공성신청 완료
			{
				strMessage = _S(2058 , "공성 신청이 완료되었습니다." );		
			}
			break;
		case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD: // 수성 길드 신청완료 
			{
				strMessage = _S( 2059, "수성측 용병 길드 신청이 완료되었습니다." );	
			}
			break;
		case MSG_GUILD_WAR_JOIN_ATTACK_CHAR: // 공성 용병 신청 완료 
			{
				strMessage = _S( 2060, "공성측 용병 신청이 완료되었습니다." );	
			}
			break;
		case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR: // 수성 용병신청 완료 
			{
				strMessage =_S(2061 ,  "수성측 용병신청이 완료되었습니다." );	
			}
			break;
			
		default : return;
		}
		
		pUIManager->GetSiegeWarfare()->Message( MSGCMD_SIEGE_WARFARE_INFO, strMessage, UMBS_OK );
	}
	else 
	{
		pUIManager->GetSiegeWarfare()->ErrorMessage( strErrorMessage[nErrorCode] );
	}
}
//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartEffect
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlag, int nGuildIndex )
{
	CTString strEffectName;
	
	// 메라크
	if( IsWar() )
	{	
		if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return; // 이미 존재 하는가?
		if( sbJoinFlag == WCJF_NONE || sbJoinFlag <= -1 ) return;
		if( slCharIndex == -1 ) return;
		if( penEntity == NULL ) return;
		
		// 용병인 경우에는... 길드 Index가 -1로 옵니다. 그래서 에러 체크 제거~
		//if( nGuildIndex == -1 ) return;
		
		switch( sbJoinFlag )
		{
			// 수성길드
		case WCJF_OWNER:
			strEffectName = "cpp_gb_defens";
			break;
			
			// 수성측길드
		case WCJF_DEFENSE_GUILD:
		case WCJF_DEFENSE_CHAR:
			strEffectName = "cpp_gb_blue";
			break;
			
			// 공성측 길드
		case WCJF_ATTACK_GUILD:
		case WCJF_ATTACK_CHAR:
			{
				strEffectName = "cpp_gb_red";
				if( nGuildIndex != -1 && nGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
				{
					strEffectName = "cpp_gb_guild";
					break;
				}

				if( IsBattle() ) // 수성전
				{
					for( int i =0 ; i < 3; i++ )
					{
						if( nGuildIndex == m_gdGuild[i].m_nGuildIndex )
						{
							strEffectName = "cpp_gb_attack";
							break;
						}
					}
				}
			}
			break;
			
		default :
			return;
		}
	}
	// WSS_DRATAN_SEIGEWARFARE 2007/08/30 --------------------------->>
	// 드라탄
	else if( CUIManager::getSingleton()->GetSiegeWarfareNew()->GetWarState())
	{
		if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return; // 이미 존재 하는가?
		if( sbJoinFlag == WCJF_NONE || sbJoinFlag <= -1 ) return;
		if( slCharIndex == -1 ) return;
		if( penEntity == NULL ) return;
		
		switch( sbJoinFlag )
		{
			// 수성 길드
		case WCJF_OWNER:
		case WCJF_DEFENSE_GUILD: // WSS_DRATAN_SIEGEWARFARE 2007/10/1
		case WCJF_DEFENSE_CHAR:
			{
				if(_pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex) strEffectName = "DEFFEND_FORCE_OUR";	
				else strEffectName = "DEFFEND_FORCE_OTHER";
			}
			break;		
			
			// 공성 길드
		case WCJF_ATTACK_GUILD:
			{
				if(_pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex) strEffectName = "ATTACK_FORCE_OUR";
				else strEffectName = "ATTACK_FORCE_OTHER";				
			}
			break;
			
		default :
			return;
		}

	}
	// 기타
	else return;
	// --------------------------------------------------------------<<

	
	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEG.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopEffect
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEG.begin(); itEG != m_mapEG.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEG.clear();
	}
	else
	{
		// 이펙터가 활성화 되어 있지 않다면 끝내라..
		if( m_mapEG.find( slCharIndex ) == m_mapEG.end() ) return;
		
		if( m_mapEG[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEG[slCharIndex] ) )
			m_mapEG[slCharIndex]->Stop(0.0f);
		
		m_mapEG.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RestartEffect(int nZoneIndex)
{
	StopEffect( -1, TRUE );

	bool bJoinFlag = false;
	SBYTE sbJoinFlag = _pNetwork->MyCharacterInfo.sbJoinFlagMerac;

	if (nZoneIndex == 4)
	{
		bJoinFlag = true;
		sbJoinFlag = _pNetwork->MyCharacterInfo.sbJoinFlagDratan;
	}

	// Start Effect My Character
	StartEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), sbJoinFlag, _pNetwork->MyCharacterInfo.lGuildIndex );
	
	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->RefreshSiegeWarfareEffect(bJoinFlag);
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartEffect
// Explain:  
// Date : 2005-11-18(오후 3:49:15), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartGuildMasterEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac, int nGuildIndex, LONG lGuildPosition )
{
	CTString strEffectName = "GuildMaster";
	
	if( !IsWar() ) return; // 전투중이 아니라면 씹자~
	if( m_mapEGGuildMaster.find( slCharIndex ) != m_mapEGGuildMaster.end() ) return; // 이미 존재 하는가?
	
	if( sbJoinFlagMerac == WCJF_NONE || 
		sbJoinFlagMerac <= -1 || 
		sbJoinFlagMerac == WCJF_DEFENSE_CHAR ||
		 sbJoinFlagMerac == WCJF_ATTACK_CHAR  ) return;

	if( slCharIndex == -1 ) return;
	if( penEntity == NULL ) return;
	if( nGuildIndex == -1 ) return;
	if( lGuildPosition != GUILD_MEMBER_BOSS ) return;

	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEGGuildMaster.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopEffect
// Explain:  
// Date : 2005-11-18(오후 3:49:12), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopGuildMasterEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEGGuildMaster.begin(); itEG != m_mapEGGuildMaster.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEGGuildMaster.clear();
	}
	else
	{
		// 이펙터가 활성화 되어 있지 않다면 끝내라..
		if( m_mapEGGuildMaster.find( slCharIndex ) == m_mapEGGuildMaster.end() ) return;
		
		if( m_mapEGGuildMaster[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEGGuildMaster[slCharIndex] ) )
			m_mapEGGuildMaster[slCharIndex]->Stop(0.0f);
		
		m_mapEGGuildMaster.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-11-18(오후 3:49:10), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RestartGuildMasterEffect()
{
	StopGuildMasterEffect( -1, TRUE );

	StartGuildMasterEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), 
							_pNetwork->MyCharacterInfo.sbJoinFlagMerac, 
							_pNetwork->MyCharacterInfo.lGuildIndex, 
							_pNetwork->MyCharacterInfo.lGuildPosition );
		
	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->RefreshGuildMasterEffect();
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartConsensusEffect
// WSS_DRATAN_SIEGEWARFARE 2007/10/14
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartConsensusEffect( SLONG slCharIndex, CEntity* penEntity )
{
	CTString strEffectName = "JEWEL";	
	
	if( m_mapEGConsensus.find( slCharIndex ) != m_mapEGConsensus.end() ) return; // 이미 존재 하는가?

	if( slCharIndex == -1 ) return;
	if( penEntity == NULL ) return;

	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEGConsensus.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopConsensusEffect
// WSS_DRATAN_SIEGEWARFARE 2007/10/14
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopConsensusEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEGConsensus.begin(); itEG != m_mapEGConsensus.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEGConsensus.clear();
	}
	else
	{
		// 이펙터가 활성화 되어 있지 않다면 끝내라..
		if( m_mapEGConsensus.find( slCharIndex ) == m_mapEGConsensus.end() ) return;
		
		if( m_mapEGConsensus[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEGConsensus[slCharIndex] ) )
			m_mapEGConsensus[slCharIndex]->Stop(0.0f);
		
		m_mapEGConsensus.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetDealy
// Explain:  
// Date : 2005-08-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetDealy( BOOL bStart )
{
	CTString strMessage;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( bStart ) // Start
	{
		m_bDelay = true;
		m_tmNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();	

		// Set Stop Move
		pUIManager->SetCSFlagOn( CSF_CASTLE_WAR_READY );
				
		if( _pNetwork->m_ubGMLevel > 1 )
		{
			strMessage.PrintF( "INFO-START-Dealy (%d)", m_tmNoticeTime );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StopPlayer();
	}
	else // Stop
	{
		m_bDelay = false;
		
		// ReSet Stop Move
		pUIManager->SetCSFlagOff( CSF_CASTLE_WAR_READY );
		
		if( _pNetwork->m_ubGMLevel > 1 )
		{
			strMessage.PrintF( "INFO-STOP-Dealy" );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RunDalyTime
// Explain:  CUISiegeWarfare의 Render에서 계속 Time 체크 
// Date : 2005-08-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RunDalyTime()
{
	if( !m_bDelay ) return;
	
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmNoticeTime;
	
	static __int64 IDealyTime = DEALY_TIME;	
	
	if( llCurDelay > IDealyTime )
	{
		SetDealy( FALSE ); // Stop
		llCurDelay = 0;
	}
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StopPlayer();
}