
#ifndef _UISIEGE_WARFARE_DOC_H
#define _UISIEGE_WARFARE_DOC_H

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Network/CharacterTarget.h>
// #include <Engine/Interface/UISiegeWarfare.h>
// #include <Engine/Interface/UISiegeWarfareNew.h> // WSS_NEW_GUILD_SYSTEM 070725
// #include <map>

#define SWS_NONE					0
#define SWS_SW_REQ					1
#define	SWS_MERCEAR_REQ				2
#define SWS_SET_SW_TIME				3
#define SWS_SW_INFO					4
#define SWS_SELECT_BATTLE			5
#define SWS_BATTLE					6
#define SWS_END						7
// WSS_DRATAN_SIEGEWARFARE 070727
#define SWS_APPLY_TOWER				8
#define SWS_UPGRADE_TOWER			9
#define	SWS_REPAIR_TOWER			10
#define	SWS_CONSENSUS_CRYSTAL		11
#define	SWS_WAIT_REBIRTH			12


// 공성 상태
#define WCSF_NORMAL					0  // 일반
#define WCSF_WAR_FIELD				1  // 야전
#define WCSF_WAR_CASTLE				2  // 공성

class SGuild
{
public:
	int			m_nGuildIndex;
	CTString	m_strGuildName;
	int			m_nPoint;

	SGuild()
	{
		m_nGuildIndex = -1;
		//m_strGuildName;
		m_nPoint = 0;
	}

	void SetGuildInfo ( int nGuildIndex, CTString strGuildName, int nPoint = -1 )
	{
		m_nGuildIndex = nGuildIndex;
		m_strGuildName = strGuildName;
		if( nPoint != -1 ) m_nPoint = nPoint;
	}

};



//------------------------------------------------------------------------------
// CUISiegeWarfareDoc
// Explain:  
// Date : 2005-07-01,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUISiegeWarfareDoc
{
	int						m_nUIState;					// UI상태 저장
	int						m_nSWTime;					// 공성 시간
	LONG					m_lLeftTime;				// 공성 진행 시간 
	TIME					m_tmLeftTime;				// 공성 남은 시간 

	SGuild					m_gdGuild[3];				// 상위 3개의 길드 정보
	SGuild					m_gdDefGuild;				// 수성길드

	int						m_nGuildPoint;
	int						m_nZoneIndex;
	
	// 공성 정보 
	int						m_nMonth, m_nDay, m_nHour, m_nMin;
	int						m_nGateState;
	
	std::map<SLONG, CEffectGroup *>		m_mapEG;
	std::map<SLONG, CEffectGroup *>		m_mapEGGuildMaster;
	std::map<SLONG, CEffectGroup *>		m_mapEGConsensus;   // WSS_DRATAN_SIEGEWARFARE 2007/10/14 교감이펙트
		
public:	

	friend CUISiegeWarfare;
	friend CUISiegeWarfareNew;
	
	CUISiegeWarfareDoc();
	~CUISiegeWarfareDoc();

	void Clear();

//받는 메세지 
	// 공성 시간 확인
	void GetTime( int nGuildIndex, CTString strGuildName, int nMonth, int nDay, int nHour, int nMin );
	
	// 공성 시작 알림(공지)
	void GuildWarStart( int nZoneIndex, int nRemainSec );	

	// 야전끝 공성 시작 알림(공지)
	void StartCastle( int nZoneIndex, int nRemainSec, int GuildIndex1, CTString GuildName1, int GuildIndex2, CTString GuildName2, int GuildIndex3, CTString GuildName3 );

	// 야전과 공성에서 받는 메세지
	void UpdatePoint( int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint );
		
	// 야전 종료 5분전부터 1분간격으로 갱신(공지)
	void RemainFiledTime( int nZoneIndex, int nRemainSec );

// 게임 접속시 받는 메세지
	// 공성 시간 확정 알림(공지)
	void TimeConfirm( int nZoneIndex, int nMonth, int nDay, int nHour, int nMin );
	
	// 공성 시작까지 남은 시간 알림(공지) - 시작 10분전 부터 1분간격으로 갱신
	void RemainStartTime( int nZoneIndex, int nMin );
	
	// 공성 상황 알림(공지) - 전투 상황 셋팅
	void CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint );
	// WSS_DRATAN_SIEGEWARFARE 070725
	void CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec);

	void EndWar( int nZoneIndex, int nWinDefense, int nOwnerGuildIndex, CTString strOwnerGuildName, int nOwnerIndex, CTString strOwnerName, int nNextMonth, int nNextDay, int nNextWeek, int nNextHour );
	void SetTimeRep( int nMonth, int nDay, int nHour, int nMin );
	
	void InvalidCommand( int nReqMsgType );

	// 에러 처리 코드
	void ErrorProc( int nErrorCode, UBYTE ubMsgType = -1 );

	
// Effect 
	void StartEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac, int nGuildIndex );
	void StopEffect( SLONG slCharIndex,  bool bAllStop = FALSE );
	void RestartEffect(int nZoneIndex);

	void StartGuildMasterEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac,  int nGuildIndex, LONG lGuildPosition );
	void StopGuildMasterEffect( SLONG slCharIndex,  bool bAllStop = FALSE );
	void RestartGuildMasterEffect();

	// WSS_DRATAN_SIEGEWARFARE 2007/10/14
	void StartConsensusEffect( SLONG slCharIndex, CEntity* penEntity);
	void StopConsensusEffect( SLONG slCharIndex,  bool bAllStop = FALSE );

// Etc
	// 남은 시간 설정 함수
	void SetLeftTime ( int nRemainSec );

// UI상태 관련
	ENGINE_API void SetUIState( int nUIState );

	// WSS_DRATAN_SIEGEWARFARE 070727
	inline int GetUIState() {return m_nUIState;}

	BOOL IsUIState( int nUIState )
	{
		return ( m_nUIState == nUIState )?TRUE:FALSE;
	}

	BOOL IsWar() // 공성전 전체 진행중
	{
		if( IsBattle() || IsSelectBattle() ) return TRUE;
		return FALSE;
	}

	BOOL IsBattle() // 수성전
	{
		return ( IsUIState( SWS_BATTLE ) )?TRUE:FALSE;
	}

	BOOL IsSelectBattle() // 선발전
	{
		return ( IsUIState( SWS_SELECT_BATTLE ) )?TRUE:FALSE;
	}

	BOOL IsSetSWTime()
	{
		return ( IsUIState( SWS_SET_SW_TIME ) )?TRUE:FALSE;
	}
	
	void SetGateState( int nOldGatesate, int nGateState );
	
	CTString GetCastleName( int nZoneIndex );

// Dealy와 관련 
	void SetDealy( BOOL bStart = TRUE );
	void RunDalyTime();
	
	bool m_bDelay;
	TIME m_tmNoticeTime;
};


ENGINE_API extern CUISiegeWarfareDoc* _pUISWDoc;

#endif // _UISIEGE_WARFARE_DOC_H
