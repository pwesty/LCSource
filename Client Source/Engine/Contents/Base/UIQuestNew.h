// ----------------------------------------------------------------------------
//  File : UIQuestNew.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUEST_H_
#define	UIQUEST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define	QUEST_WIDTH						216
#define	QUEST_HEIGHT					380

#define	QUESTINFO_CHAR_WIDTH			170

// 월드컵이벤트
#define WORLDCUP_MAX_COUNTRY 32

enum e24Event
{
	EXCH_FIVE,						// 맞고 머니 교환
	EXCH_FOUR,						// 포커 머니 교환 
	OPEN_TREA,						// 보물 상자 열기
};

const int ciQuestClassifier			=	1000;

enum eSelectionQuest
{
	QUEST_TALK,
	QUEST_EVENT,
	QUEST_TREASURE_BOX,				// 보물 상자.
	QUEST_CHANGEWEAPON,
	QUEST_KILL_BOSS,				// 발록 격파
	QUEST_SAVE_PRINCESS,			// 공주 구출
	QUEST_AZAKA_RAVINE,				// 아자카 협곡
	QUEST_GATE_OF_DIMENSION,		// 차원의 문
	QUEST_HARVEST_MOON_DAY_EVENT,	// 추석이벤트
	QUEST_HARVEST_MOON_DAY_EVENT1,	// 2006 추석이벤트(송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT2,	// 2006 추석이벤트(오색송편 보상품과 교환하기)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE1,	// 2006 추석이벤트(송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE2,	// 2006 추석이벤트(오색송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT_GIVE_ITEM,	// 보상품을 지급받는다.
	QUEST_RENUAL_EVENT,
	QUEST_RENUAL_EVENT_OK,
	QUEST_EXCHANGE_MONEY,			// 랜디 이판사판 머니 교환 wooss 051031
	QUEST_OPEN_TREASURE,			// 랜디 이판사판 보물상자 열기
	QUEST_CHARACTER_CARD,			// 
	EVENT_NEWYEAR1,					// 2006 꿈과 희망 이벤트
	EVENT_NEWYEAR2,					// 인내의 열매 이벤트 
	EVENT_FIND_FRIEND,
	EVENT_COUPON ,					// 일본 쿠폰 이벤트 
	EVENT_CONNECT,					// 일본 접속자 이벤트	

	EVENT_CLOTHES_EXCHANGE,			// 일본 전통 의상 교환
	EVENT_OX_GOZONE,				// O.X이벤트 존 입장

	PLATINUM_EXCHANGE_STONE,        // 플래티늄 제련석 교환
	WIN_SELECT,						// 우승국 알아 맞추기
	EVENT_GOLDENBALL,				// 골든 볼 이벤트	
	EVENT_OPENBETAITEMGIFT,			// 미국 이벤트 무기 지급
	EVENT_NETCAFE_BOX,				// net cafe 캠페인
	EVENT_RAINYDAY,
	EVENT_BUDDHISM,					// 불교 촛불 축제 이벤트( 태국 )
	EVENT_COLLECTBUGS,				// 여름 곤충 채집 이벤트
	EVENT_MOTHERDAY,					// 태국 어머니날 이벤트
	EVENT_PROMOPACK,				// 프로모 패키지 이벤트 ( 말레샤 )
	EVENT_PROMO_KEY_ENTER,			// 프로모 패키지 인증키 입력

	EVENT_XMAS_2006,				// 2006 X-MAS Event [12/11/2006 Theodoric]
	EVENT_LOVE_LOVE,				// 2007발렌타인's 데이(러브러브 이벤트)
	EVENT_WHITEDAY_2007,			// 2007 WhiteDay Event
	NPC_HELP,						// NPC 안내 시스템
	
	EVENT_MAY,						// WSS_GUILD_MASTER 070411 -->><<
	EVENT_MINIGAME,					// WSS_MINIGAE 070420 곰돌이 이벤트
	EVENT_FLOWERS_SUPPORT,			// 꽃놀이 이벤트 개화 돕기
	EVENT_SUPPORT_EXCHANGE,			// 꽃놀이 이벤트 기여도와 교환권
	EVENT_SUMMER_2007,				// [070705: Su-won] EVENT_SUMMER_2007
	SIEGEWARFARE_MASTERTOWER1,		// WSS_DRATAN_SEIGEWARFARE 2007/07/30 // 가동하기
	SIEGEWARFARE_MASTERTOWER2,		// 타워 강화하기 
	SIEGEWARFARE_MASTERTOWER3,		// 성문 강화하기
	SIEGEWARFARE_MASTERTOWER4,		// 마스터 타워 수리하기
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// [070708: Su-won] EVENT_ADULT_OPEN
	EVENT_ADULT_MAGICCARD,					// 성인서버 오픈 이벤트(매직카드를 찾아라!)
	EVENT_ADULT_CHANGEJOB,					// 성인서버 오픈 이벤트(전직달성!절망의부적을 받아라!)
	EVENT_ADULT_ALCHEMIST,					// 성인서버 오픈 이벤트(장비 조합 연금술 이벤트)
	EVENT_SHOOT,							// 성인서버 오픈 이벤트(성인들만의 특권)
	// [070708: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////

	EVENT_BJMONO_2007_REQ,					// SK BJ MONO 응모권 받기
	EVENT_BJMONO_2007_CHANGE_REQ,			// SK BJ MONO 응모권 교환
	
	EVENT_TG2007_1000DAYS_CAP,				// 라스트카오스 1000일 기념모자 받기
	EVENT_TG2007_1000DAYS_CAP_UPGRADE,		// 기념모자 업그레이드
	EVENT_TG2007_1000DAYS_FIRECRACKER,		// 폭죽 받기 이벤트
	EVENT_TG2007_FRUITFULL,					// 풍년이벤트  	
	EVENT_TG2007_SCREENSHOT,				// 스크린샷 이벤트
	
	EVENT_HALLOWEEN2007_DEVILHAIR,			// 악마날개 머리띠 지급 받기
	EVENT_HALLOWEEN2007_CANDYBASKET,		// 사탕바구니에 사탕 받기
	EVENT_HALLOWEEN2007_PUMKINHEAD,			// 호박머리 탈 교환 받기
	EVENT_HALLOWEEN2007_WITCHHAT,			// 마녀모자 교환 받기
	EVENT_HALLOWEEN2007_PUMKIN,				// 호박교환 받기
	
	// 071129_ttos: 판매 대행협회 회장
	CASH_PERSONSHOP_CLOSE,					// 아이템 판매 대행 종료
	CASH_PERSONSHOP_ITEM,					// 보관된 물품 회수
	CASH_PERSONSHOP_NAS,					// 보관된 나스 회수

	EVENT_XMAS2007_DECO,					// 크리스마스트리 장식하기
	EVENT_XMAS2007_BLESSEDNESS,				// 크리스마스 축복 받기

	EVENT_RED_TREASUREBOX,					//붉은색 보물상자 이벤트

	EVENT_SAKURA_2008,						//2008년 벚꽃 이벤트

	EVENT_CHAOSBALL,						//카오스볼 이벤트

	EVENT_PHOENIX_REQ,							//피닉스 이벤트
	
	EVENT_HANAPOS_SK_EVENT,					//[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
	
	MIDLEVEL_SUPPORT_REQ,					// 중저레벨 지원사 스킬 요청
	
	EVENT_REQUITAL_1,						//[ttos_2009_4_13]: 서버 오류 보상이벤트
	EVENT_REQUITAL_2,						//[ttos_2009_4_13]: 출석 체크 보상
	EVENT_ADD_UP_NEW_AFRON,					// 아프론 , 블리스 서버 통합이벤트기념품 지급(아프론 다운)
	
	QUEST_RESTORE_ITEM,						// [100208: selo] 퀘스트 아이템 다시 받기
	
	EVENT_WOLRDCUP_2010_EVENT2,				// ($E_WC2010) [100514: selo] 2010 남아공 월드컵 Event2 응모, 현황
	EVENT_WOLRDCUP_2010_EVENT2_STATUS,		// ($E_WC2010) [100524: selo] 2010 남아공 월드컵 Event2 현황
	EVENT_WOLRDCUP_2010_EVENT2_GIFT,		// ($E_WC2010) [100517: selo] 2010 남아공 월드컵 Event2 현황, 보상
	EVENT_WOLRDCUP_2010_EVENT3,				// ($E_WC2010) [100514: selo] 2010 남아공 월드컵 Event3 매일 매일 응원하기

	ATTENDANT_DATE_REQ,						// [7/2/2010 kiny8216] 출석 시스템 (연속 출석일 요청)
	DUNGEON_DIFFICULTY_NORMAL,				// 아칸사원 Normal
	DUNGEON_DIFFICULTY_HARD,				// 아칸사원 Hard
	EVENT_TEMP,								// 단발성 이벤트 용(100907 아칸 사원 이벤트)
	
	REGISTER_MERCHANT,						// [2010/08/25 : Sora] ADD_SUBJOB 상인 등록

	EVENT_PROMOTION2,						// [10/6/2010 kiny8216] 쿠폰 프로모션 이벤트

	EVENT_ATTENDANCE_2011,					// [2011/01/18 : Sora] 출석 이벤트

	EVENT_COMEBACK,							// 휴면 계정 보상 이벤트

	EVENT_BIRTHDAY_GIFT,					// 생일 이벤트 : 선물 요청
	EVENT_BIRTHDAY_INFO,					// 생일 이벤트 : 생일 정보 요청

	EVENT_TREASURE_MAP,						// 보물찾기 이벤트의 [나무]보물 열쇠 교환 요청

	EVENT_LOI_KRATHONG,						// 태국 크라통 이벤트 재료아이템 교환 요청

	EVENT_KB_EXCHAGE_HEART,					// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트 선물상자 교환
	EVENT_KB_EXCHAGE_FLAG,					// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트 이벤트의상 교환

	EVENT_SONGKRAN,							// 송크란 이벤트.

	SYNDICATE_JEWEL_GIVE,
	SYNDICATE_SECESSION,
};

enum eEvent
{
	EVENT_NEWYEAR1_REWARD,
	EVENT_NEWYEAR2_TIMECHECK,
	EVENT_NEWYEAR2_REWARD,

	EVENT_FF_REG,
	EVENT_FF_ISREWARD,
	EVENT_FF_TIMECHECK,
	EVENT_FF_REWARD,

	EVENT_REWARD_1,
	EVENT_REWARD_2,
	EVENT_REWARD_3,

	EVENT_MAY_CHILDREN,
	EVENT_MAY_PARENTS,
	EVENT_MAY_MASTER,

	EVENT_WORLDCUP_2010_EVENT2_SEL1,		// ($E_WC2010) [100514: selo] 우승 국가 응모	
	EVENT_WORLDCUP_2010_EVENT2_SEL2,		// ($E_WC2010) [100514: selo] 내 응모 현황
	EVENT_WORLDCUP_2010_EVENT2_SEL3,		// ($E_WC2010) [100517: selo] 당첨 보상 받기

	EVENT_WORLDCUP_2010_EVENT3_SEL1,		// ($E_WC2010) [100518: selo] 응원카드 받기
	EVENT_WORLDCUP_2010_EVENT3_SEL2,		// ($E_WC2010) [100518: selo] 매일매일 응원 보상 받기
	EVENT_WORLDCUP_2010_EVENT3_GIFT1,		// ($E_WC2010) [100518: selo] 결승 기원 상자 받기
	EVENT_WORLDCUP_2010_EVENT3_GIFT2,		// ($E_WC2010) [100518: selo] 4강 기원 상자 받기
	EVENT_WORLDCUP_2010_EVENT3_GIFT3,		// ($E_WC2010) [100518: selo] 8강 기원 상자 받기
	EVENT_WORLDCUP_2010_EVENT3_GIFT4,		// ($E_WC2010) [100518: selo] 16강 기원 상자 받기
};

class CUIQuest : public CUIWindow
{
protected:
	FLOAT					m_fNpcX, m_fNpcZ;
	int						m_iNpcIndex;
	int						m_iNpcVirIdx;
	int						m_iCurQuestIndex;

	// Event save data
	CTString				m_strTeamA, m_strTeamB;			// 골든 볼 이벤트 용
	int						m_ScoreTeamA, m_ScoreTeamB;		
	SYSTEMTIME				m_GoldenEndTime;
	BOOL					m_bGoldenBallEntry;				// 응모 가능
	BOOL					m_bGoldenBallRequital;			// 보상 가능
	BOOL					m_bStartGoldenBall;
	CTString				m_strCountry[WORLDCUP_MAX_COUNTRY];
	std::map<INDEX, BYTE>	m_mapNationFlag;				// ($E_WC2010) [100517: selo] 국기 아이템 확인용(map이 검색에서 빠르다)
	
	// [2011/02/09 : Sora]	퀘스트아이템 다시 받기
	int						m_restoreQuestIndex;
	SLONG					m_restoreQuestItemIndex;
	
public:
	CUIQuest();
	~CUIQuest();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	ENGINE_API void	OpenQuest( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	void	QuestError( UBYTE ubError );
	void	CloseQuest();
	void	SetCurQuest(int iQuestIndex) { m_iCurQuestIndex = iQuestIndex; }
	int 	GetNpcPosX(){ return m_fNpcX;}
	int		GetNpcPosZ(){ return m_fNpcZ;}
	int		GetNpcIndex(){ return m_iNpcIndex; }
	void	SetStrTeamName( CTString strTeamA, CTString strTeamB, SYSTEMTIME GoldenEndTime, BOOL bFlag )
	{ 
		m_strTeamA = strTeamA; m_strTeamB = strTeamB;
		if( bFlag ) m_GoldenEndTime = GoldenEndTime;
	}

	void	SetTeamScore( int ScoreA, int ScoreB ) { m_ScoreTeamA = ScoreA; m_ScoreTeamB = ScoreB; }
	void	SetGoldenBallEntry( BOOL bType ) {
		m_bGoldenBallEntry = bType;
		if( bType == TRUE ) m_bStartGoldenBall = TRUE;
	}
	void	SetGoldenBallRequital( BOOL bType ) {
		m_bGoldenBallRequital = bType;
		if( bType == FALSE ) m_bStartGoldenBall = FALSE;
	}

	CTString	GetStrTeamA( void ) { return m_strTeamA; }
	CTString	GetStrTeamB( void ) { return m_strTeamB; }

	int			GetScoreTeamA( void ) { return m_ScoreTeamA; }
	int			GetScoreTeamB( void ) { return m_ScoreTeamB; }

	SYSTEMTIME	GetGoldenTime( void ) { return m_GoldenEndTime; }

	void	CreateMessageBox_WorldCup2010_Event2( INDEX nType );
	void	InitNationFlag();
	BOOL	IsNationFlag(INDEX nItemIndex)
	{
		return m_mapNationFlag.find(nItemIndex) == m_mapNationFlag.end() ? FALSE : TRUE;
	}
};


#endif	// UIQUEST_H_
