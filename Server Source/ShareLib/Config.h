/************************************
  수정사항
  수정한 사람: bs
  수정일: 2004-11-19 오후 8:02:59
  설명: 스킬 시전 취소 효과, 스킬 조건 - 방패 착용 추가
 ************************************/

#ifndef __GAME_Config_H__
#define __GAME_Config_H__

// 국가별 설정 : MAKEFILE에서
//#define LC_KOR		0	// 국내
//#define LC_TLD			4	// 태국
//#ifdef LC_TLD
//#endif // LC_TLD
//#define	LC_USA			9	// 미국
//#define	LC_BRZ			10	// 브라질

//#define	LC_GER			13  // 독일

#if defined (LC_KOR)
#define LC_LOCAL_STRING "KOR"
#define LC_LOCAL_CODE		LC_KOR
#elif defined (LC_TLD)
#define LC_LOCAL_STRING "TLD"
#define LC_LOCAL_CODE		LC_TLD
#elif defined (LC_USA)
#define LC_LOCAL_STRING "USA"
#define LC_LOCAL_CODE		LC_USA
#elif defined (LC_BRZ)
#define LC_LOCAL_STRING "BRZ"
#define LC_LOCAL_CODE		LC_BRZ
#elif defined (LC_GER)
#define LC_LOCAL_STRING "DE"
#define LC_LOCAL_CODE		LC_GER
#elif defined (LC_FRC)
#define LC_LOCAL_STRING "FR"
#define LC_LOCAL_CODE		LC_FRC
#elif defined (LC_PLD)
#define LC_LOCAL_STRING "PD"
#define LC_LOCAL_CODE		LC_PLD
#elif defined (LC_SPN)
#define LC_LOCAL_STRING "ES"
#define LC_LOCAL_CODE		LC_SPN
#elif defined (LC_RUS)
#define LC_LOCAL_STRING "RUS"
#define LC_LOCAL_CODE		LC_RUS
#elif defined (LC_TUR)
#define LC_LOCAL_STRING "TR"
#define LC_LOCAL_CODE		LC_TUR
#elif defined (LC_ITA)
#define LC_LOCAL_STRING "IT"
#define LC_LOCAL_CODE		LC_ITA
#elif defined (LC_MEX)
#define LC_LOCAL_STRING "MEX"
#define LC_LOCAL_CODE		LC_MEX
#elif defined (LC_UK)
#define LC_LOCAL_STRING "UK"
#define LC_LOCAL_CODE		LC_UK
#else
#error "Not Set Language"
#endif // #if defined (LC_KOR)
// --- 국가별 설정

//#define		BLOCK_PVP					// TODO : PVP 막으려면 주석 풀기

#define SERVER_VERSION			600		// 게임 서버 버전
#define SERVER_MIN_VERSION		600		// 메신저, 컨넥터에서 수용하는 최저 서버 버전
#define SERVER_MAX_VERSION		700		// 메신저, 컨넥터에서 수용하는 최고 서버 버전
#define CONNECTOR_SERVER_NUM	888		// 컨넥터 서버 번호
#define LOGIN_SERVER_NUM		999		// 로그인 서버 번호
#define MESSENGER_SERVER_NUM	777		// 메신저 서버 번호
#define BILL_SERVER_VERSION		"BillCruxV40" // 빌링서버 프로토콜 버전
#define		GAMEID				1		// 라카 게임 아이디 1번

#define BILL_LOCATION_HOME		0		// 일반
#define	BILL_LOCATION_PCBANG	1		// 등록된 pc방

#define PCBANG_LOGIN			10		// pc방 로그인
#define PCBANG_LOGOUT			11		// pc방 로그 아웃

#define LOGIN_SERVER_OPEN_CHECK_FILENAME	"login.open"		// 로그인 서버 오픈 검사 파일명

//#define		GAMEID				1 // 이거 중복이라 주석 처리
#define		CPID				1
#define		ID_LENGTH			32
#define		MAX_PURCHASEITEM	16
#define		MAX_DEPOSITORY		25

#define	PLAYER_COUNT_NUM		9999	// 사용자수 조사 프로그램 번호

#define NUM_RESERVED_DESCS		5		// 리눅스에서 예약된 디스크립터 수
#define MAX_PLAYING				50000	// 최대 동접 수
#define MAX_ZONES				43		// 최대 존 수
#define MAX_NOTICE				5

#define PI       3.14159265358979323846f
#define PI_2     6.28318530717958623200f
#define PI_3_4	 2.35619449019234492884f	/* ((3 / 4) * PI) */
#define PI_HALF	 (PI / 2)

// 시간 관련
#ifdef __GAME_SERVER__
#define OPT_USEC		100000				/* 10 passes per second */
#else
#define OPT_USEC		50000				/* 20 passes per second */
#endif
#define PASSES_PER_SEC	(1000000 / OPT_USEC)
#define RL_SEC			* PASSES_PER_SEC
#define RL_OCLOCK			3600				// 실제 정시.
#define RL_24CLOCK			86400				// 실제 24시
// 실제 시간과의 관계
#define PULSE_REAL_SEC				(1 RL_SEC)				// 실제 1초
#define PULSE_REAL_MIN				(60 RL_SEC)				// 실제 1분
#define PULSE_REAL_TENMIN			(600 RL_SEC)			// 실제 10분
#define PULSE_REAL_HALF				(1800 RL_SEC)			// 실제 30분
#define	PULSE_REAL_HOUR				(3600 RL_SEC)			// 실제 1시간
#define PULSE_REAL_24HOUR			(86400 RL_SEC)

#define PULSE_PCROOM_COUNT			(3 * PULSE_REAL_MIN)	// 피씨방 접속자 수 카운트 주기
#define PULSE_TOTAL_COUNT			PULSE_REAL_MIN			// 전체 접속 수 카운트 주기
#define PULSE_NPC_AI				2						// NPC 최소 AI 지연 시간 : 0.5초
#define	PULSE_NPC_REGEN				(15 RL_SEC)				// NPC 리젠 최소 지연 시간 : 실제 30초
#define PULSE_WARPDELAY				(10 * TIME_ONE_SECOND)	// 순간이동 딜레이 : 10초
#define PULSE_BILLITEMREQ			(20 * TIME_ONE_SECOND)	// 빌링아이템 지급 제한 시간 20초  이시간이 지나면 셋을 푼다.
#define PULSE_BILLITEMLISTREQ		(4 * PULSE_REAL_SEC)	// 빌링아이템 리스트 요청 제한 시간 이시간이 지나면 셋을 푼다.
#define PULSE_WARPCOUNTDOWN			(5 * PULSE_REAL_SEC)	// 순간이동 카운트 다운 시작 : 5초 전부터
#define PULSE_ASSIST_CHECK			PULSE_REAL_SEC			// 보조효과 시간 감소 검사 딜레이
#define PULSE_HACK_CHECK_TIMEOUT	(6 * PULSE_REAL_MIN)	// 펄스 메시지 타임아웃 6분
#define PULSE_HACK_PULSE_REFRESH	PULSE_REAL_HOUR			// 핵 검사 펄스 갱신 주기 1시간
#define PULSE_HACK_PULSE_THRESHOLD	(15 * PULSE_REAL_SEC)	// 핵 펄스 오차 범위 15초
#define PULSE_HACK_CLIENT_THRESHOLD	(55 * PULSE_REAL_SEC)	// 핵 펄스 클라이언트 최소 주기 55초 이보다 자주 보내면 안됨!
#define PULSE_PKMODE_DELAY			10		 				// PK모드에서 평화모드로 전환시 딜레이
#define PULSE_HACK_ATTACK_THRESHOLD	2						// 공속 오차 허용 범위
#define	PULSE_HACK_ATTACK_COUNT		5						// 어택 핵카운트 모으는 숫자
#define PULSE_ATTACKMODE_DELAY		(3 * PULSE_REAL_SEC)	// 공격 모드 풀리는 딜레이 : 마지막 공격이후 이 시간동안은 공격 중이다
#define PULSE_POST_REGEN_DELAY		(3 * PULSE_REAL_SEC)	// 리젠이후 선공하기 전까지 딜레이
#define PULSE_PRODUCE_DELAY			(2 * PULSE_REAL_SEC)	// 생산 딜레이
#define PULSE_PET_SYMPATHY_INCREASE	(5 * PULSE_REAL_MIN)	// 펫 교감도 증가 딜레이
#define PULSE_PET_SYMPATHY_DECREASE	(PULSE_REAL_HOUR)		// 펫 교감도 감소 딜레이
#define PULSE_PET_HUNGRY_DECREASE	(5 * PULSE_REAL_MIN)	// 펫 배고픔 딜레이
#define PULSE_AUTO_SKILL_DECREASE	(10 * TIME_ONE_SECOND)	// 자동 스킬 시전 시간
#define PULSE_KEY_CHANGE_THRESHOLD	(10 * PULSE_REAL_SEC)	// 키 변경 임계 시간 : 10 초
#define PULSE_KEY_CHANGE			(20 * PULSE_REAL_SEC)	// 키 변경 주기 : 20 초
#define	PULSE_SERVER_ALIVE			(2 * PULSE_REAL_MIN)	// 빌링 서버 얼라이브 메세지 타임
#define	PULSE_PCBANG_ITEM			(PULSE_REAL_HOUR)		// 등록된 PC방에서 아이템 지급 주기
#define PULSE_SAVECOUNT_NEW				5					// 컨넥터의 동접 플레이어 수 저장 시간
#define PULSE_SAVEMAXCOUNT_NEW			2					// 컨넥터의 최대 동접 플레이어 수 저장 시간
#define PULSE_TRIGGER_SYSTEM		(4 * PULSE_REAL_SEC)	// 트리거 proc 제어 주기\
 
#define TIME_ONE_SECOND				1						// 1초
#define TIME_ONE_MIN				60						// 1분
#define TIME_ONE_HOUR				60 * 60					// 1시간
#define TIME_ONE_DAY				TIME_ONE_HOUR * 24		// 1일
#define TIME_ONE_WEEK				TIME_ONE_DAY * 7		// 1주일
#define TIME_ONE_MONTH				TIME_ONE_DAY * 30		// 30일

#define MAX_ID_NAME_LENGTH			64		// 아이디, 이름 길이
#define MAX_PWD_LENGTH				32		// 암호 길이
#define HOST_LENGTH					30		// 호스트 주소 문자열 길이
#define MAX_CHAR_NAME_LENGTH		50		// 케릭터 이름 길이
#define MAX_GUILD_NAME_LENGTH		50		// 길드 이름 길이
#define MAX_ITEM_NAME_LENGTH		50		// 아이템 이름길이
#define FRIEND_MAX_MEMBER			50		// 친구 멤버 수
#define	FRIEND_GROUP_MAX_NUMBER		10		// 그룹 수
#define MAX_GROUP_NAME_LENGTH		20		// 그룹 이름 길이
#define BLOCK_MAX_MEMBER			20		// 최대 블럭 인원수
#define	MAX_GIFT_MESSAGE_LENGTH		100		// 선물 메세지 길이 100바이트

// 전역으로 사용하는 문자배열, CServer의 Run에서만 사용!!!
#define MAX_STRING_LENGTH		8192

// 접속 상태 상수
#define CON_PLAYING						0		// 게임 중
#define CON_CLOSE						1		// 연결 끊김
#define CON_MOVESERVER					2		// 서버 이동중
#define CON_MOVESERVER_WAIT				3		// 서버 이동중
#define CON_WAIT_SCARD					4		// 보안 카드 대기 중
#define CON_MENU						10		// 메뉴에서의 동작
#define CON_DISCONNECT					17		// 게임중 연결 끊김
#define CON_WAIT_USER_DB				19		// 캐릭터 데이터 기다림
#define CON_WAIT_NEW_CHAR_DB			20		// 캐릭터 생성 기다림
#define CON_WAIT_DEL_CHAR_DB			21		// 캐릭터 삭제 기다림
#define CON_WAIT_PLAY_CHAR_DB			22		// 캐릭터 상세 정보 기다림
#define CON_GET_LOGIN					23		// 로그인 시도
#define CON_PREPARE_PLAY				25		// 게임 시작 대기중
#define CON_PREPARE_PLAY_BEFORE			26		// 캐릭터 삭제 취소
#define CON_WAIT_IDEN_MSG				30		// 인증 기다림
#define CON_GET_IDEN_MSG				31		// 인증 요청
#define CON_GET_DUMMY					32		// 패킷을 무시함

#define STATE(d)			((d)->m_connected)
#define WAITTING_DB(d)		((d)->m_bwaitdb)
#define UPDATING_DB(d)		((d)->m_updatedb)
#define PLAYMODE(d)			((d)->m_playmode)

// 신규 생성 케릭터, 선물도착 여부, 태국일경우 성인
//커넥터 class CUser의 m_userFlag
#define NOVICE					( 1 << 0 )
#define	RECV_GIFT				( 1 << 1 )
#define ADULT					( 1 << 2 )
#define IS_CHARACTER			( 1 << 3 )
#define IS_NEWUSER_JPN_2007_03	( 1 << 4 )
#define IS_GM_CHAR				( 1 << 5 )

/////////////////////////////////////////////////////////////////////////////
// 국가별 설정
#include "Config_Localize.h"
// ---- 국가별 설정
/////////////////////////////////////////////////////////////////////////////

// 스킬포인트
#define MAX_SKILLPOINT		2000000000

// 직업별 상수
#ifdef EX_ROGUE
#define JOBCOUNT			9
#else
#define JOBCOUNT			7
#endif // EX_ROGUE
#define JOB_TITAN			0
#define JOB_KNIGHT			1
#define JOB_HEALER			2
#define JOB_MAGE			3
#define JOB_ROGUE			4
#define JOB_SORCERER		5
#define JOB_NIGHTSHADOW		6
#ifdef EX_ROGUE
#define JOB_EX_ROGUE		7
#endif // EX_ROGUE
#ifdef EX_MAGE
#define JOB_EX_MAGE			8
#endif

#define JOB_PET				10
#define JOB_APET			11

#ifdef EX_ROGUE
#ifdef EX_MAGE
#define JOB2COUNT			17
#else
#define JOB2COUNT			15
#endif
#else
#define JOB2COUNT			13
#endif // EX_ROGUE

#define JOB_2ND_SPLIT_INDEX	2

#define JOB_2ND_HIGHLANDER			1		// 하이랜더
#define	JOB_2ND_WARMASTER			2		// 워마스터

#define JOB_2ND_ROYALKNIGHT			1		// 로열나이트
#define	JOB_2ND_TEMPLEKNIGHT		2		// 템플나이트

#define JOB_2ND_ARCHER				1		// 아처
#define	JOB_2ND_CLERIC				2		// 클레릭

#define JOB_2ND_WIZARD				1		// 위자드
#define	JOB_2ND_WITCH				2		// 위치

#define JOB_2ND_ASSASSIN			1		// 어쎄씬
#define	JOB_2ND_RANGER				2		// 레인저

#define JOB_2ND_ELEMENTALLIST		1		// 엘리멘탈 리스트
#define JOB_2ND_SPECIALLIST			2		// 스페셜 리스트

#define JOB_2ND_NIGHTSHADOW			1		// 나이트 쉐도우
#define JOB_2ND_NIGHTSHADOW2		2		// Dummy 사용안함

#define JOB_2ND_EX_ASSASSIN			1		// EX 어쎄씬
#define JOB_2ND_EX_RANGER			2		// EX 레인저

#define JOB_2ND_EX_WIZARD			1		// EX 위자드
#define JOB_2ND_EX_WITCH			2		// EX 위치

#define JOB_2ND_PET_HORSE			0	// 말
#define JOB_2ND_PET_DRAGON			1	// 용
#define JOB_2ND_PET_HORSE_MOUNT		2	// 말 : 탈것
#define JOB_2ND_PET_DRAGON_MOUNT	3	// 용 : 탈것

#define JOB_2ND_LEVEL			31		// 전직 레벨

#define JOB_SUB_TRADER			( 1 << 0 )		// 부직업 상인

// 캐릭터 flag
#define CHAT_FLAG_NO_SAY		(1 << 0)		// 일반 채팅 금지
#define CHAT_FLAG_NO_PARTY		(1 << 1)		// 파티 채팅 금지
#define CHAT_FLAG_NO_GUILD		(1 << 2)		// 길드 채팅 금지
#define CHAT_FLAG_NO_TRADE		(1 << 3)		// 매매 채팅 금지
#define CHAT_FLAG_NO_WHISPER	(1 << 4)		// 귓속말 금지
#define CHAT_FLAG_NO_SHOUT		(1 << 5)		// 외치기 금지
#define CHAT_FLAG_NO_EXPED		(1 << 6)		// 원정대 채팅 금지

// 머리 모양 개수
#define HAIR_TITAN			5
#define HAIR_KNIGHT			5
#define HAIR_HEALER			5
#define HAIR_MAGE			5
#define HAIR_ROGUE			5
#define HAIR_SORCERER		5
#define HAIR_NIGHTSHADOW	5
#define HAIR_EX_LOG			5
#define HAIR_EX_MAGE		5

#define HAIR_RED_CAP		10			// 빨간 산타 모자
#define HAIR_GREEN_CAP		20			// 녹색 산타 모자

// 얼굴 모양 개수
#define FACE_TITAN			5
#define FACE_KNIGHT			5
#define FACE_HEALER			5
#define FACE_MAGE			5
#define FACE_ROGUE			5
#define FACE_SORCERER		5
#define FACE_NIGHTSHADOW	5
#define FACE_EX_ROGUE		5
#define FACE_EX_MAGE		5

// 데이터베이스 동작 상태
#define DB_NOP					0		// DB No Operation...
#define DB_UPDATE				1		// Request DB Update...
#define DB_WAIT_SAVE			10		// Save Delay...
#define DB_END					11		// 빌링 없을때 로그아웃 완료

// 게임 로그인 종류
#define	MAX_CHAR_COUNT			8		// 최대 생성 가능 캐릭터 수
#define MAX_SKILL				60		// 캐릭터 당 최대 스킬 개수
#define MAX_ASSIST_HELP			30		// 보조 효과 : 이로운 것
#define MAX_ASSIST_CURSE		8		// 보조 효과 : 해로운 것
#define MAX_ASSIST_ABS			40		// 절대 시간 보조효과
#define MAX_EXT_CHAR_COUNT		2		// 유료아이템으로 추가 할수 있는 슬롯수

///////
// 기타
#define NO_CELL				(-100)		// 셀에 포함 안된 상태
#define EPSILON				(1.0e-5f)	// Tolerance for FLOATs
#define CELL_SIZE			48.0f		// 한 셀의 attr 개수 (m 단위)

#define CELL_EXT			3			// 영향을 미치는 셀 영역

#define MULTIPLE_HEIGHTMAP	100.0f		// 높이맵 곱해지는 수치
#define MAX_PLAYER_LIST		8192		// 최대 동시 접속자 수
#define MAX_MEMPOS					15			// 장소 기억 수
#define MAX_MEMPOS_NORMAL			5			// 장소 기억 수

#define ATTACK_TYPE_NORMAL			0			// 일반공격
#define ATTACK_TYPE_DOUBLE			2			// 더블공격 ( 공속한번에 2번 공격 )
#define ATTACK_TYPE_INFINITY		10			// 무한공격 ( 공속검사 안함 )

///////////////////
// NPC 관련
#define		MAX_NPC_SKILL			4
#define		MAX_NPC_DROPITEM		20
#define		MAX_NPC_PRODUCT			5
#define		MAX_NPC_DROPITEM_LOOP	4
#define		MAX_NPC_DROPJEWEL		20

// NPC FLAG --
#define		NPC_SHOPPER				(1 << 0)	// 상점주인여부?
#define		NPC_FIRSTATTACK			(1 << 1)	// 선공여부?
#define		NPC_ATTACK				(1 << 2)	// 반격가능?
#define		NPC_MOVING				(1 << 3)	// 이동가능?
#define		NPC_PEACEFUL			(1 << 4)	// 평화적인 몹(원클릭으로 공격 안 시킬때)
#define		NPC_ZONEMOVER			(1 << 5)	// 존 이동시켜주는 NPC
#define		NPC_CASTLE_GUARD		(1 << 6)	// 수호병
#define		NPC_REFINER				(1 << 7)	// 연금술사 NPC
#define		NPC_QUEST				(1 << 8)	// Quest NPC
#define		NPC_CASTLE_TOWER		(1 << 9)	// 공격혁 수호상
#define		NPC_MINERAL				(1 << 10)	// 미네랄
#define		NPC_CROPS				(1 << 11)	// 농작물
#define		NPC_ENERGY				(1 << 12)	// 에너지
#define		NPC_ETERNAL				(1 << 13)	// 지속
#define		NPC_LORD_SYMBOL			(1 << 14)	// 성주의 증명
#define		NPC_REMISSION			(1 << 15)	// 면죄부
#define		NPC_EVENT				(1 << 16)	// 이벤트
#define		NPC_GUARD				(1 << 17)	// 경비병
#define		NPC_KEEPER				(1 << 18)	// 창고
#define		NPC_GUILD				(1 << 19)	// 길드
#define		NPC_MBOSS				(1 << 20)	// 준보스
#define		NPC_BOSS				(1 << 21)	// 보스
#define		NPC_RESETSTAT			(1 << 22)	// 스탯 초기화
#define		NPC_CHANGEWEAPON		(1 << 23)	// 무기 교체
#define		NPC_WARCASTLE			(1 << 24)	// 공성 NPC
#define		NPC_DISPLAY_MAP			(1 << 25)	// 지도에 표시되는지 여부
#define		NPC_QUEST_COLLECT		(1 << 26)	// 수집 퀘스트용
#define		NPC_PARTY				(1 << 27)	// 파티몹 표시용
#define		NPC_RAID				(1 << 28)	// 레이드 몹
#define		NPC_SUBCITY				(1 << 29)	// 서브 마을 NPC
#define		NPC_CHAOCITY			(1 << 30)	// 카오마을 전용 리젠 NPC
#define		NPC_HUNTERCITY			(1 << 31)	// 헌터마을 전용 리젠 NPC

// NPC FLAG1(확장) --
#define		NPC1_TRADEAGENT			(1 << 0)	// 거래대행(경매) 시스템
#define		XXX_NPC1_COLLSION			(1 << 1)	// 레이드 인스턴트 던전에 나오는 NPC
#define		NPC1_FACTORY			(1 << 2)	// 제작 시스템 NPC
#define		NPC1_TRIGGER_CHOICE		(1 << 3)	// NPC와 대화하면 트리거 발동
#define		NPC1_TRIGGER_KILL		(1 << 4)	// NPC를 죽이면 트리거 발동
#define		NPC1_NOT_NPCPORTAL		(1 << 5)	// NPCPORTAL 스크롤 리스트에 출려되면 않되는 넘들은 체크
#define		NPC1_DONT_ATTACK		(1 << 6)	// 토템 npc는 공격을 받지 않는다.
#define		NPC1_AFFINITY			(1 << 7)	// 친화도
#define		NPC1_SHADOW				(1 << 8)	// 그림자 속성 ( 그림자 마법가루 사용시만 보임 )
#define		NPC1_CRAFTING			(1 << 9)	// 제작2 제작 가능한 npc
#define		NPC1_TOTEM_ITEM			(1 << 10)	// 아이템으로 인한 토템 소환

// NPC AI TYPE
#define		NPC_AI_NORMAL			0			// 일반 NPC
#define		NPC_AI_TANKER			1			// 일반 NPC
#define		NPC_AI_DAMAGEDEALER		2			// 일반 NPC
#define		NPC_AI_HEALER			3			// 일반 NPC

#define IS_NPC(ch)			(ch->m_type == MSG_CHAR_NPC)	// NPC 인가요?
#define TO_NPC(ch)			((ch == NULL) ? NULL : (IS_NPC(ch) ? (CNPC*)ch : (CNPC*)NULL))		// NPC로 캐스팅
#define IS_PC(ch)			(ch->m_type == MSG_CHAR_PC)		// PC 인가요?
#define TO_PC(ch)			((ch == NULL) ? NULL : (IS_PC(ch) ? (CPC*)ch : (CPC*)NULL))			// PC로 캐스팅
#define IS_PET(ch)			(ch->m_type == MSG_CHAR_PET)	// 애완동물인가?
#define TO_PET(ch)			((ch == NULL) ? NULL : (IS_PET(ch) ? (CPet*)ch : (CPet*)NULL))		// Pet로 캐스팅

#define IS_APET(ch)			(ch->m_type == MSG_CHAR_APET)
#define TO_APET(ch)			((ch == NULL) ? NULL : (IS_APET(ch) ? (CAPet*)ch : (CAPet*)NULL))		// AttackPet로 캐스팅

#define IS_ELEMENTAL(ch)	(ch->m_type == MSG_CHAR_ELEMENTAL)	// 소환수 인가?
#define TO_ELEMENTAL(ch)	((ch == NULL) ? NULL : (IS_ELEMENTAL(ch) ? (CElemental*)ch : (CElemental*)NULL)) // 소환수로 캐스팅
#define GET_CHAR_TYPE(ch)	((char)(ch->m_type))
#define DEAD(ch)			(ch->m_hp <= 0)					// 캐릭터 죽었나요?
#define IS_IN_CELL(ch)		(ch->m_cellX >= 0)				// 캐릭터가 셀에 위치해 있나요?

#define CMD_NUM 200						// 최대 메시지 종류 수

#define GET_X(obj)			(obj->m_pos.m_x)
#define GET_Z(obj)			(obj->m_pos.m_z)
#define GET_H(obj)			((obj && obj->m_pArea) ? obj->m_pArea->GetHeight(GET_YLAYER(obj), GET_X(obj), GET_Z(obj)) : 0.0f)
#define GET_R(obj)			(obj->m_pos.m_r)
#define GET_YLAYER(obj)		(obj->m_pos.m_yLayer)

// 속성맵 상수
#define	MATT_WALKABLE			0x0001
#define	MATT_PEACE				0x0002
#define	MATT_PRODUCT_PUBLIC		0x0004
#define	MATT_PRODUCT_PRIVATE	0x0008
#define	MATT_STAIR_UP			0x0010
#define	MATT_STAIR_DOWN			0x0020
#define	MATT_WAR				0x0040
#define	MATT_FREEPKZONE			0x0080

#define	MATT_UNWALKABLE			0x8000

///////////////////
// 아이템 관련 상수

// 050311 : bs : 무기가 사용하는 스탯에 대한 상수 추가
#define USING_STR		0
#define USING_DEX		1
#define USING_INT		2
// --- 050311 : bs : 무기가 사용하는 스탯에 대한 상수 추가
// 상품 타입
// category
#define		CATEGORY_HOTNNEW							10000
#define		CATEGORY_PLATINUM							20000
#define		CATEGORY_DISPOSABLE							30000
#define		CATEGORY_COSTUME							40000
#define		CATEGORY_PET								50000
#define		CATEGORY_SERVICE							60000
#define		CATEGORY_PACKAGE							70000
// type
#define		TYPE_HOTNNEW_NONE							10000	//TYPE 미분류
#define		TYPE_HOTNNEW_NEW							10100
#define		TYPE_HOTNNEW_DISCOUNT						10200
#define		TYPE_HOTNNEW_HOT							10300
#define		TYPE_PLATINUM_NONE							20000
#define		TYPE_DISPOSABLE_NONE						30000
#define		TYPE_DISPOSABLE_CHARACTER_GROW				30100
#define		TYPE_DISPOSABLE_ABILITY_BUILDUP				30200
#define		TYPE_DISPOSABLE_POTION						30300
#define		TYPE_EQUIP_NONE								40000
#define		TYPE_EQUIP_EQUIPMENT						40100
#define		TYPE_EQUIP_BUILDUP							40200
#define		TYPE_EQUIP_COSTUME							40300
#define		TYPE_AVATAR_NONE							50000
#define		TYPE_AVATAR_PET								50100
#define		TYPE_AVATAR_MERCENARY						50200
#define		TYPE_SERVICE_NONE							60000
#define		TYPE_SERVICE_CONVENIENCE					60100
#define		TYPE_SERVICE_ETC							60200
#define		TYPE_PACKAGE_NONE							70000
//미국을 위한 추가분
#define		TYPE_PACKAGE_BOOSTERS						70100
#define		TYPE_PACKAGE_ENHANCEMENTS					70200
#define		TYPE_PACKAGE_POTIONS						70300
#define		TYPE_PACKAGE_VANITY							70400
#define		TYPE_PACKAGE_UPGRADE						70500
#define		TYPE_PACKAGE_PETS							70600
#define		TYPE_PACKAGE_MERCENARY						70700
#define		TYPE_PACKAGE_CONVENIENCE					70800
#define		TYPE_PACKAGE_OTHERS							70900
//미국을 위한 추가분
#define 	TYPE_OLD_GOODS								90000
#define		TYPE_LIST_START								0
#define		TYPE_MAX_COUNT								30
// subtype
#define		SUBTYPE_HOTNNEW_NONE_NONE					10000 // TYPE, SUBTYPE 미분류
#define		SUBTYPE_HOTNNEW_NEW_NONE					10100
#define		SUBTYPE_HOTNNEW_DISCOUNT_NONE				10200
#define		SUBTYPE_PLATINUM_NONE_NONE					20000
#define		SUBTYPE_DISPOSABLE_NONE_NONE				30000
#define		SUBTYPE_DISPOSABLE_CHARACTER_GROW_NONE		30100
#define		SUBTYPE_DISPOSABLE_CHARACTER_GROW_EXP		30101
#define		SUBTYPE_DISPOSABLE_CHARACTER_GROW_SP		30102
#define		SUBTYPE_DISPOSABLE_CHARACTER_GROW_DROP		30103
#define		SUBTYPE_DISPOSABLE_CHARACTER_GROW_MIX		30104
#define		SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_NONE		30200
#define		SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_HPEXT	30201
#define		SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_MPEXT	30202
#define		SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_VOL		30203	// VOL = Velocity of Light
#define		SUBTYPE_DISPOSABLE_POTION_NONE				30300
#define		SUBTYPE_DISPOSABLE_POTION_HPRECOVER			30301
#define		SUBTYPE_DISPOSABLE_POTION_MPRECOVER			30302
#define		SUBTYPE_DISPOSABLE_POTION_DUALRECOVER		30303
#define		SUBTYPE_EQUIP_NONE_NONE						40000
#define		SUBTYPE_EQUIP_EQUIPMENT_NONE				40100
#define		SUBTYPE_EQUIP_EQUIPMENT_WEAPON				40101
#define		SUBTYPE_EQUIP_EQUIPMENT_ARMOR				40102
#define		SUBTYPE_EQUIP_EQUIPMENT_ACCESSORY			40103
#define		SUBTYPE_EQUIP_BUILDUP_NONE					40200
#define		SUBTYPE_EQUIP_BUILDUP_MELDING				40201
#define		SUBTYPE_EQUIP_BUILDUP_SOCKET				40202
#define		SUBTYPE_EQUIP_BUILDUP_BLOOD					40203
#define 	SUBTYPE_EQUIP_BUILDUP_REFINE				40204
#define		SUBTYPE_EQUIP_COSTUME_NONE					40300
#define		SUBTYPE_EQUIP_COSTUME_TITAN					40301
#define		SUBTYPE_EQUIP_COSTUME_KNIGHT				40302
#define		SUBTYPE_EQUIP_COSTUME_HEALER				40303
#define		SUBTYPE_EQUIP_COSTUME_MAGE					40304
#define		SUBTYPE_EQUIP_COSTUME_ROGUE					40305
#define		SUBTYPE_EQUIP_COSTUME_SORCERER				40306
#define		SUBTYPE_EQUIP_COSTUME_NIGHTSHADOW			40307
#define		SUBTYPE_AVATAR_NONE_NONE					50000
#define		SUBTYPE_AVATAR_PET_NONE						50100
#define		SUBTYPE_AVATAR_PET_PET						50101
#define		SUBTYPE_AVATAR_PET_PETITEM					50102
#define		SUBTYPE_AVATAR_MERCENARY_NONE				50200
#define		SUBTYPE_AVATAR_MERCENARY_MONSTER			50201
#define		SUBTYPE_SERVICE_NONE_NONE					60000
#define		SUBTYPE_SERVICE_CONVENIENCE_NONE			60100
#define		SUBTYPE_SERVICE_CONVENIENCE_SCROLL			60101
#define		SUBTYPE_SERVICE_CONVENIENCE_CARD			60102
#define		SUBTYPE_SERVICE_CONVENIENCE_ETC_NONE		60200
#define		SUBTYPE_PACKAGE_NONE_NONE					70000
//미국을 위한 추가분
#define		SUBTYPE_PACKAGE_BOOSTERS_NONE				70100
#define		SUBTYPE_PACKAGE_BOOSTERS_XP					70101
#define		SUBTYPE_PACKAGE_BOOSTERS_SP					70102
#define		SUBTYPE_PACKAGE_BOOSTERS_DROP				70103
#define		SUBTYPE_PACKAGE_BOOSTERS_MIXED				70104
#define		SUBTYPE_PACKAGE_ENHANCEMENTS_NONE			70200
#define		SUBTYPE_PACKAGE_ENHANCEMENTS_HPMP			70201
#define		SUBTYPE_PACKAGE_ENHANCEMENTS_MIXED			70202
#define		SUBTYPE_PACKAGE_POTIONS_NONE				70300
#define		SUBTYPE_PACKAGE_POTIONS_HPRECOVERY			70301
#define		SUBTYPE_PACKAGE_POTIONS_MPRECOVERY			70302
#define		SUBTYPE_PACKAGE_POTIONS_MIXED				70303
#define		SUBTYPE_PACKAGE_VANITY_NONE					70400
#define		SUBTYPE_PACKAGE_VANITY_TITAN				70401
#define		SUBTYPE_PACKAGE_VANITY_KNIGHT				70402
#define		SUBTYPE_PACKAGE_VANITY_HEALER				70403
#define		SUBTYPE_PACKAGE_VANITY_MAGE					70404
#define		SUBTYPE_PACKAGE_VANITY_ROGUE				70405
#define		SUBTYPE_PACKAGE_VANITY_SORCERER				70406
#define		SUBTYPE_PACKAGE_VANITY_NIGHTSHADOW			70407
#define		SUBTYPE_PACKAGE_UPGRADE_NONE				70500
#define		SUBTYPE_PACKAGE_UPGRADE_MELDING				70501
#define		SUBTYPE_PACKAGE_UPGRADE_STONENRUNES			70502
#define		SUBTYPE_PACKAGE_UPGRADE_SOCKETS				70503
#define		SUBTYPE_PACKAGE_PETS_NONE					70600
#define		SUBTYPE_PACKAGE_PETS_PETS					70601
#define		SUBTYPE_PACKAGE_PETS_PETSITEM				70602
#define		SUBTYPE_PACKAGE_MERCENARY_NONE				70700
#define		SUBTYPE_PACKAGE_CONVENIENCE_NONE			70800
#define		SUBTYPE_PACKAGE_CONVENIENCE_SCROLLS			70801
#define		SUBTYPE_PACKAGE_CONVENIENCE_CARDS			70802
#define		SUBTYPE_PACKAGE_OTHERS_NONE					70900
//미국을 위한 추가분
#define		SUBTYPE_LIST_START							1
#define		SUBTYPE_MAX_COUNT							80
// 상품 플래그
#define		CATALOG_FLAG_NEW						(1 << 0)			// 신상품
#define		CATALOG_FLAG_POPULAR					(1 << 1)			// 인기상품
#define		CATALOG_FLAG_DISCOUNT					(1 << 2)			// 특가상품
#define		CATALOG_FLAG_RECOMM_1					(1 << 3)			// 추천 아이템1
#define		CATALOG_FLAG_RECOMM_2					(1 << 4)			// 추천 아이템2
#define		CATALOG_FLAG_RECOMM_3					(1 << 5)			// 추천 아이템3
#define		CATALOG_FLAG_RECOMM_4					(1 << 6)			// 추천 아이템4
#define		CATALOG_FLAG_RECOMM_5					(1 << 7)			// 추천 아이템5
#define		CATALOG_FLAG_NOGIFT						(1 << 8)			// 선물금지
#define		CATALOG_FLAG_RESV2						(1 << 9)			// 예약 플래그
#define		CATALOG_FALG_TO_LIMIT(a, b)				(b = 0 | (a >> 10))	// 카탈로그 FLAG 출력
#define		CATALOG_FALG_FROM_LIMIT(a, b)			(a = a | (b << 10))	// 카탈로그 FLAG 입력
#define		CATALOG_FLAG_MAX_LIMIT					(2000000)			// 카탈로그 플래그 LIMIT MAX

// Item Flag : 최상위 1비트는 사용 불가!!!
// 6비트까지 케르, 넨, 일반 제련석의 레벨과, 플레티늄 제련석을 바른 아이템의 레벨로 사용
#define		PLATINUM_MAX_PLUS				127
#define		FLAG_ITEM_PLATINUM_GET(a, b)	(b = a & PLATINUM_MAX_PLUS)
#define		FLAG_ITEM_PLATINUM_SET(a, b)	( (a &~ PLATINUM_MAX_PLUS) | b )
#define		FLAG_ITEM_OPTION_ENABLE			(1 << 7)	// 아이템 플래그에 옵션을 붙일 수 있는 상태를 구분, Bit 연산
#define		FLAG_ITEM_SEALED				(1 << 8)	// 아이템 봉인
#define		FLAG_ITEM_SUPER_STONE_USED		(1 << 9)	// 슈퍼고제로 업그레이드 했음..//0627
#define		FLAG_ITEM_BOOSTER_ADDED			(1 << 10)	// 부스터 장착 여부
#define		FLAG_ITEM_SILVERBOOSTER_ADDED	(1 << 11)	// 실버부스터 장착 여부
#define		FLAG_ITEM_GOLDBOOSTER_ADDED		(1 << 12)	// 골드부스터 장착 여부
#define		FLAG_ITEM_PLATINUMBOOSTER_ADDED	(1 << 13)	// 골드부스터 장착 여부
#define		FLAG_ITEM_COMPOSITION			(1 << 14)	// 아이템 합성 여부
#define		FLAG_ITEM_LENT					(1 << 15)	// 대여된 아이템
#define		FLAG_ITEM_LEVELDOWN				(1 << 16)	// 레벨 다운된 아이템
#define		FLAG_ITEM_BELONG				(1 << 17)	// 귀속 된 아이템
#define		FLAG_ITEM_SOCKET				(1 << 18)
#define		FLAG_ITEM_SUPER_RUNE_USED		(1 << 19)	// 초고급룬 사용 했음.
#define		FLAG_ITEM_RESERVED				(1 << 31)	// 예약된 것 사용하면 절대 안됨!

// Item Proto flag
#define		ITEM_FLAG_COUNT					((LONGLONG)1 << 0)	// 셀 수 있는 아이템인가
#define		ITEM_FLAG_DROP					((LONGLONG)1 << 1)	// 드롭  가능 아이템인가가
#define		ITEM_FLAG_UPGRADE				((LONGLONG)1 << 2)	// 업그레이드 가능한 아이템인가
#define		ITEM_FLAG_EXCHANGE				((LONGLONG)1 << 3)	// 교환 가능 아이템인가가
#define		ITEM_FLAG_TRADE					((LONGLONG)1 << 4)	// 매매 가능 아이템인가가
#define		ITEM_FLAG_BORKEN				((LONGLONG)1 << 5)	// 파괴 가능 아이템인가가
#define		ITEM_FLAG_MADE					((LONGLONG)1 << 6)	// 생산 가능 아이템인가가
#define		ITEM_FLAG_MIX					((LONGLONG)1 << 7)	// 조합 아이템 : DB에 저장안됨
#define		ITEM_FLAG_CASH					((LONGLONG)1 << 8)	// 유료 아이템
#define		ITEM_FLAG_LORD					((LONGLONG)1 << 9)	// 성주 전용 아이템
#define		ITEM_FLAG_NO_STASH				((LONGLONG)1 << 10)	// 창고 보관 불가능 아이템
#define		ITEM_FLAG_CHANGE				((LONGLONG)1 << 11)	// 다른 직업용 아이템으로 교환 가능 여부
#define		ITEM_FLAG_COMPOSITE				((LONGLONG)1 << 12)	// 아이템 합성 가능 여부
#define		ITEM_FLAG_DUPLICATE				((LONGLONG)1 << 13)	// 중복 확인
#define		ITEM_FLAG_LENT					((LONGLONG)1 << 14)	// 대여용 무기
#define		ITEM_FLAG_RARE					((LONGLONG)1 << 15)	// 레어 아이템
#define		ITEM_FLAG_ABS					((LONGLONG)1 << 16)	// 시간제 아이템
#define		ITEM_FLAG_NOTREFORM				((LONGLONG)1 << 17)	// 리폼 불가 아이템
#define		ITEM_FLAG_ZONEMOVE_DEL			((LONGLONG)1 << 18) // 존이동 시 아이템 삭제
#define     ITEM_FLAG_ORIGIN				((LONGLONG)1 << 19)	// 오리지널 옵션
#define     ITEM_FLAG_TRIGGER				((LONGLONG)1 << 20)	// 트리거 이벤트에 사용되는 아이템
#define     ITEM_FLAG_RAIDSPECIAL			((LONGLONG)1 << 21)	// 현재 사용 안함(추후 다른 것으로 사용)
#define		ITEM_FLAG_QUEST					((LONGLONG)1 << 22) // 퀘스트
#define		ITEM_FLAG_BOX					((LONGLONG)1 << 23) //  럭키드로우박스
#define		ITEM_FLAG_NOTTRADEAGENT			((LONGLONG)1 << 24)	// 거래대행등록 불가
#ifdef DURABILITY
#define     ITEM_FLAG_DURABILITY			((LONGLONG)1 << 25)	// 장비 내구도
#endif
#define		ITEM_FLAG_COSTUME2				((LONGLONG)1 << 26)
#define		ITEM_FLAG_SOCKET				((LONGLONG)1 << 27)		// 소켓 생성 가능 아이템
#define		ITEM_FLAG_SELLER				((LONGLONG)1 << 28)		// 상인용 아이템
#define		ITEM_FLAG_CASTLLAN				((LONGLONG)1 << 29)		// 성주만 착용 가능한 아이템
#define		ITEM_FLAG_LETSPARTY				((LONGLONG)1 << 30)		// 파티형아이템 ( 파티에 메세지 전송 )
#define		ITEM_FLAG_NONRVR				((LONGLONG)1 << 31)		// RVR사용 불가
#define		ITEM_FLAG_QUESTGIVE				((LONGLONG)1 << 32)		// 퀘스트 기부 가능 아이템
#define		ITEM_FLAG_TOGGLE				((LONGLONG)1 << 33)		// 토글 플래그
#define		ITEM_FLAG_COMPOSE				((LONGLONG)1 << 34)		// 합성 재료 플래그
#define		ITEM_FLAG_NOTSINGLE				((LONGLONG)1 << 35)		// 싱글던전 사용불가 플래그
#define		ITEM_FLAG_INVISIBLE_COSTUME		((LONGLONG)1 << 36)		// 투명 코스튬 플래그

//장착 아이템 종류
#define		ITEM_NORMAL			0	//일반 장착 아이템
#define		ITEM_COSTUM			1	//개별 코스튬 아이템
#define		ITEM_COSTUM_SUIT	2	//코스튬 세트 아이템

// 아이템 분류 TYPE
#define		ITYPE_WEAPON			0		// 무기
#define		ITYPE_WEAR				1		// 방어구
#define		ITYPE_ONCE				2		// 일회용
#define		ITYPE_SHOT				3		// 탄환
#define		ITYPE_ETC				4		// 기타
#define		ITYPE_ACCESSORY			5		// 악세사리
#define		ITYPE_POTION			6		// 포션

// SUBTYPE : 무기  --
#define		IWEAPON_NIGHT			0		// 기사도		나이트
#define		IWEAPON_CROSSBOW		1		// 석궁			로그
#define		IWEAPON_STAFF			2		// 스테프		메이지
#define		IWEAPON_BIGSWORD		3		// 대검			타이탄
#define		IWEAPON_AXE				4		// 도끼			타이탄
#define		IWEAPON_SHORTSTAFF		5		// 숏스테프		메이지
#define		IWEAPON_BOW				6		// 활			힐러
#define		IWEAPON_SHORTGUM		7		// 단검			로그
#define		IWEAPON_MINING			8		// 채굴도구		-
#define		IWEAPON_GATHERING		9		// 채집도구		-
#define		IWEAPON_CHARGE			10		// 차지도구		-
#define		IWEAPON_TWOSWORD		11		// 이도류		나이트
#define		IWEAPON_WAND			12		// 완드			힐러
#define		IWEAPON_SCYTHE			13		// 사이드		소서러
#define		IWEAPON_POLEARM			14		// 폴암			소서러
#define		IWEAPON_SOUL			15		// 소울(혼)		나이트쉐도우

// SUBTYPE : 방어구 --
#define		IWEAR_HELMET			0		// 머리
#define		IWEAR_ARMOR				1		// 상의
#define		IWEAR_PANTS				2		// 하의
#define		IWEAR_GLOVE				3		// 장갑
#define		IWEAR_SHOES				4		// 신발
#define		IWEAR_SHIELD			5		// 방패
#define		IWEAR_BACKWING			6		// 등짝
#define		IWEAR_SUIT				7		// 한벌

// SUBTYPE : 일회용 --
#define		IONCE_WARP				0		// 이동
#define		IONCE_PROCESS_DOC		1		// 가공 문서
#define		IONCE_MAKE_TYPE_DOC		2		// 무기방어구 제작 문서
#define		IONCE_BOX				3		// 상자
#define		IONCE_MAKE_POTION_DOC	4		// 포션 제작 문서
#define		IONCE_CHANGE_DOC		5		// 변신 주문서
#define		IONCE_QUEST_SCROLL		6		// 퀘스트 문서
#define		IONCE_CASH				7		// 캐쉬 아이템
#define		IONCE_SUMMON			8		// 몬스터 소환서
#define		IONCE_ETC				9		// 기타 하드코딩 일회용 아이템
#define		IONCE_TARGET			10		// 타켓에 아이템적용
#define		IONCE_TITLE				11		// 호칭 시스템
#define		IONCE_REWARD_PACKAGE	12		// 점핑 패키지 -> 리워드 패키지로 변경 : 이 타입의 아이템을 사용하면 리워드 패키지 보상이 지급된다.
#define		IONCE_JUMPING_POTION	13		// 점핑 포션 : 사용하면 캐릭테 레벨을 특정 레벨로 올려주는 아이템
#define		IONCE_EXTEND_CHARACTER_SLOT	14 // 캐릭터 확장 슬롯 아이템
#define		IONCE_SERVER_TRANS		15		//서버 이전 아이템
#define		IONCE_REMOTE_EXPRESS	16		// 원격 창고 아이템
#define		IONCE_JEWEL_POCKET			17	// 일반 보석 주머니
#define		IONCE_CHAOS_JEWEL_POCKET	18	// 카오스 보석 주머니
#define		IONCE_CASH_INVENTORY		19	// 인벤 열쇠 (유료 가방)
#define		IONCE_PET_STASH				20	// 펫 창고 이용권
#define		IONCE_GPS			21		//	gps 아이템
#define		IONCE_HOLY_WATER	22		//	성수 아이템
#define		IONCE_PROTECT_PVP	23		//PVP 보호

// IONCE_WARP : 종류
#define		IONCE_WARP_RETURN		0		// 특정 존 귀환 장치
#define		IONCE_WARP_MEMPOS		1		// 기억된 장소로 이동
#define		IONCE_WARP_MARGADUM_PVP	2		// 메라크 pvp 이동
#define		IONCE_WARP_PARTYRECALL	3		// 060227 : bs : 파티리콜
#define		IONCE_WARP_MEMPOSPLUS	4		// 유료 아이템 이동
#define		IONCE_WARP_NPC_PORTAL_SCROLL 30 // NpcPortalScroll 용 임시 방편 툴에서는 사용하지 않는다.

// IONCE_BOX : 종류
#define		IONCE_BOX_REMAKE		0		// 재활의상자
#define		IONCE_BOX_ARCANE		1		// 비밀의상자
#define		IONCE_BOX_COLLECT_BUG	2		// 곤충 채집 상자

// SUBTYPE : 탄환 --
#define		ISHOT_ATKBULLET		0	// 어택블릿
#define		ISHOT_MPBULLET		1	// 마나블릿
#define		ISHOT_ARROW			2	// 화살

// SUBTYPE : 기타 --
#define		IETC_QUEST			0	// 퀘스트용
#define		IETC_EVENT			1	// 이벤트용
#define		IETC_SKILLUP		2	// 스킬 상승
#define		IETC_UPGRADE		3	// 제련
#define		IETC_MATERIAL		4	// 재료
#define		IETC_MONEY			5	// 돈
#define		IETC_PRODUCT		6	// 생산품
#define		IETC_PROCESS		7	// 가공품
#define		IETC_OPTION			8	// 옵션
#define		IETC_SAMPLE			9	// 시료
#define		IETC_TEXTURE		10	// 텍스쳐 For Client
#define		IETC_MIX_TYPE1		11	// 공성조합1
#define		IETC_MIX_TYPE2		12	// 공성조합2
#define		IETC_MIX_TYPE3		13	// 공성조합3
#define		IETC_PET_AI			14	// 펫
#define		IETC_QUEST_TRIGGER	15	// 퀘스트 트리거
#define		IETC_JEWEL			16	// 보석
#define		IETC_STABILIZER		17	// 안정제
#define		IETC_PROCESS_SCROLL	18	// 가공 스크롤
#define		IETC_MONSTER_MERCENARY_CARD 19 // 몬스터 용병 카드
#define		IETC_GUILD_MARK		20
#define		IETC_REFORMER		21
#define		IETC_CHAOSJEWEL		22	//카오스 보석
#define		IETC_FUNCTIONS		23	//기능성
#define		IETC_RVR_JEWEL		24	//창조의 보석

// IETC_PROCESS 종류1
#define		IETC_PROCESS_STONE			0	// 광석 가공품
#define		IETC_PROCESS_PLANT			1	// 식물 가공품
#define		IETC_PROCESS_ELEMENT		2	// 원소 가공품

// IETC_PROCESS 종류2
#define		IETC_PROCESS_STONE_0		0	// 스톤 온 가공품
#define		IETC_PROCESS_STONE_1		1	// 스톤 리스 가공품
#define		IETC_PROCESS_STONE_2		2	// 스톤 웨버 가공품
#define		IETC_PROCESS_STONE_3		3	// 스톤 비스트 가공품
#define		IETC_PROCESS_STONE_4		4	// 스톤 웰스던 가공품

#define		IONCE_PROCESS_PLANT_0		0	// 크락의 노란잎 가공품
#define		IONCE_PROCESS_PLANT_1		1	// 크락의 줄기 가공품
#define		IONCE_PROCESS_PLANT_2		2	// 크락의 파란잎 가공품
#define		IONCE_PROCESS_PLANT_3		3	// 크락의 가시 가공품
#define		IONCE_PROCESS_PLANT_4		4	// 크락의 꽃 가공품

#define		IONCE_PROCESS_ELEMENT_0		0	// E등급 원소 가공품
#define		IONCE_PROCESS_ELEMENT_1		1	// D등급 원소 가공품
#define		IONCE_PROCESS_ELEMENT_2		2	// C등급 원소 가공품
#define		IONCE_PROCESS_ELEMENT_3		3	// B등급 원소 가공품
#define		IONCE_PROCESS_ELEMENT_4		4	// A등급 원소 가공품

// IETC_OPTION 종류
#define		IETC_OPTION_TYPE_BLOOD	0	// 블러드 젬
#define		IETC_OPTION_TYPE_CLEAR	1	// 정화석

// IETC_UPGRADE 종류
#define		IETC_UPGRADE_GENERAL		0	// 일반제련석
#define		IETC_UPGRADE_SPECIAL		1	// 고급제련석
#define		IETC_UPGRADE_SPECIAL_SUPER	2	// 슈퍼고급제련석//0627
#define		IETC_UPGRADE_BOOSTER		3	// 부스터
#define		IETC_UPGRADE_LUCKY			4	// 행운의 고급 제련석
#define		IETC_UPGRADE_PLATINUM		5	// 플래티늄 제련석
#define		IETC_UPGRADE_CHAOS			6	// 특급 제련석
#define		IETC_UPGRADE_PURITY_RUNE	7   // 정화된 룬
#define		IETC_UPGRADE_DEVIL_RUNE		8   // 악마의 룬
#define		IETC_UPGRADE_CHAOS_RUNE		9	// 카오스 룬
#define		IETC_UPGRADE_SUPER_RUNE		10	// 초고급 룬
#define		IETC_UPGRADE_LUCKY_RUNE		11	// 행운 룬
// IETC_REFORMER 종류
#define		IETC_REFORMER_MID_GRADE		0	// 중급 리포머
#define		IETC_REFORMER_HIGH_GRADE	1	// 상급 리포머

// SUBTYPE : 악세사리
#define		IACCESSORY_CHARM		0	// 부적
#define		IACCESSORY_MAGICSTONE	1	// 마련석
#define		IACCESSORY_LIGHTSTONE	2	// 반짝이는돌
#define		IACCESSORY_EARING		3	// 귀걸이
#define		IACCESSORY_RING			4	// 반지
#define		IACCESSORY_NECKLACE		5	// 목걸이
#define		IACCESSORY_PET			6	// 펫
#define		IACCESSORY_ATTACK_PET	7	// 공격형 펫
#define		IACCESSORY_ARTIFACT		8	// 유물

// SUBTYPE : 포션
#define		IPOTION_STATE			0	// 상태치료
#define		IPOTION_HP				1	// HP회복
#define		IPOTION_MP				2	// MP회복
#define		IPOTION_DUAL			3	// 듀얼회복
#define		IPOTION_STAT			4	// Stat상승
#define		IPOTION_ETC				5	// 기타 (이동향상)
#define		IPOTION_UP				6	// 공방향상
#define		IPOTION_TEARS			7	// 눈물
#define		IPOTION_CRYSTAL			8	// 결정
#define		IPOTION_NPC_PORTAL		9	// npc포탈스크롤 타입 (NHN 채널링 서비스)
#define		IPOTION_HP_SPEEDUP		10	// hp회복속도증가
#define		IPOTION_MP_SPEEDUP		11  // mp회복속도증가
#define		IPOTION_PET_HP			12  // 펫hp회복
#define		IPOTION_PET_SPEEDUP		13  // 펫이속증
#define		IOPTION_TOTEM			14	
#define		IPOTION_PET_MP			15  // 펫hp회복

// IPOTION_TEARS 종류
#define		IPOTION_TEARS_TYPE_SAFE		0	// 구원의 눈물
#define		IPOTION_TEARS_TYPE_FORGIVE	1	// 용서의 눈물

#define MAX_INVENTORY_SIZE			100							// 탭 당 최대 아이템 개수
#define ITEMS_PER_ROW				5							// 한 줄당 최대 아이템 개수
#define MAX_INVENTORY_ROWS			(MAX_INVENTORY_SIZE / ITEMS_PER_ROW)	// 줄수
#define MAX_SERIAL_LENGTH			20							// 시리얼 번호 길이
#define MAX_ITEM_BUY				10							// 상점에서 최대 살 수 있는 아이템 종류의 수
#define MAX_ITEM_SELL				10							// 상점에 최대 팔 수 있는 아이템 종류의 수
#define	GROUND_ITEM_PULSE			(5 * PULSE_REAL_MIN)		// 땅에 아이템 없어지는 시간 60s
#define	ITEM_PREPERENCE_PULSE		(30 * PULSE_REAL_SEC)		// 아이템 우선권 셋팅 시간 10s

#define MAX_QUEST_INVENTORY_ROWS	(MAX_INVENTORY_SIZE / 20)
#define MAX_EVENT_INVENTORY_ROWS	(MAX_INVENTORY_SIZE / 20)

// 아이템 업그레이드 관련
#define	ITEM_UPGRADE_RESULT_PLUS		0		// +1
#define	ITEM_UPGRADE_RESULT_MINUS		1		// -1
#define	ITEM_UPGRADE_RESULT_NOCHANGE	2		// 0
#define	ITEM_UPGRADE_RESULT_BROKEN		3		// 깨짐
#define	ITEM_UPGRADE_PLATINUM_RESULT_PLUS 4		// 플래티늄 플러스를 발랐음
#define ITEM_UPGRADE_RUNE_PROTECTION_ITEM 5		// HIGH_RUNE_PROTECTION_ITEM 상급 Rune item 사용

#define ITEM_UPGRADE_PROB_PLUS_VALUE		12			// 12위의 기준 되는 수치
#define ITEM_UPGRADE_PROB_PLUS_PENALTY_LEV	200			// 업그레이드 수치당 떨어지는 확률
#define ITEM_UPGRADE_PROB_PLUS_PENALTY_MAX	4000		// 업그레이드 최대 떨어지는 확률 (40%) -> 최소 성공 확률 보장을 위한 것
#define	ITEM_UPGRADE_PROB_GENERAL_PLUS		5000		// 일반 제련석 확률 50%
#define ITEM_UPGRADE_PROB_SPECIAL_NOCHANGE	2000		// 고급 제련석 유지 확률 20% (1 ~ 20)
#define ITEM_UPGRADE_PROB_SPECIAL_BROKEN	2700		// 고급 제련석 파괴 확률 7% (21 ~ 30)
#define ITEM_UPGRADE_PROB_SPECIAL_PLUS		7700		// 고급 제련석 성공 최대 확률 50% (31 ~ 80)

#define ITEM_UPGRADE_PENALTY_BROKEN			3			// 깨지거나 -1 없는 아이템 등급의 한계

#define ITEM_UPGRADE_GENERAL_MINLEVEL		1			// 일반제련석 최소 1레벨 부터
#define ITEM_UPGRADE_GENERAL_MAXLEVEL		146			// 일반 제련석의 최대 레벨 (확장팩 부터는 146이상은 룬으로 업그레이드)

#define ITEM_REFINE_RESULT_TWO				0			// 무기 일반 제련석 2개
#define ITEM_REFINE_RESULT_ONE				1			// 방어구 일반 제련석 1개
#define ITEM_REFINE_RESULT_SPECIAL			2			// 고급 제련석 1개

// 착용 관련
#define MAX_WEARING					28	// 캐릭터 당 최대 착용 장비 수 : 투구 상의 무기 하의 방패 장갑 신발 악세1 악세2 악세3 애완동물 등
#define WEARING_NONE				-1	// 착용 안함

#define WEARING_SHOW_START			0	// 외부로 보이는 착용 장비 시작 인덱스
#define WEARING_SHOW_END			9	// 외부로 보이는 착용 장비 끝 인덱스

#define WEARING_HELMET				0	// 착용 장비 순서
#define WEARING_ARMOR_UP			1
#define WEARING_WEAPON				2
#define WEARING_ARMOR_DOWN			3
#define WEARING_SHIELD				4
#define WEARING_GLOVE				5
#define WEARING_BOOTS				6
#define WEARING_ACCESSORY1			7
#define WEARING_ACCESSORY2			8
#define WEARING_ACCESSORY3			9
#define WEARING_PET					10

#define WEARING_BACKWING			11

#define COSTUME2_WEARING_HELMET				20	// 착용 장비 순서
#define COSTUME2_WEARING_ARMOR_UP			21
#define COSTUME2_WEARING_WEAPON				22
#define COSTUME2_WEARING_ARMOR_DOWN			23
#define COSTUME2_WEARING_SHIELD				24
#define COSTUME2_WEARING_GLOVE				25
#define COSTUME2_WEARING_BOOTS				26
#define COSTUME2_WEARING_BACK				27

#define COSTUME2_WEARING_SUIT				30

#define COSTUME2_WEARING_START				20 // COSTUME2 WEARING START NUMBER.
#define MAX_COSTUME_WEARING					8
#define DEFAULT_MAX_WEARING					12

// [110207:selo] 한 벌 의상
#define COSTUME2_SUIT_MAX_WEARING			5	// 한 벌 의상에서 입히는 방어구의 개수

/////////////////
// 세트 아이템 관련
#define MAX_SET_ITEM_OPTION			11			// 세트 아이템에 붙는 최대 옵션 개수

#define SET_ITEM_OPTION_NONE		-1
#define SET_ITEM_OPTION				0			// 세트 아이템에 붙는 옵션
#define SET_ITEM_SKILL				1			// 세트 아이템에 붙는 스킬

/////////////////
// 제작 시스템 관련
#define		MAX_FACTORY_ITEM_STUFF	5			// 제작 아이템 최대 재료 수
#define		MAX_SEAL_TYPE_SKILL		10			// 제작 스킬 갯수

/////////////////
// 교환 관련 상수

#define		MAX_EXCHANGE_ITEMS		10		// 최대 교환 개수

//////////////////////////
// 공격 관련

#define		HATE_DECREASE_UNIT		100						// HATE 감소 단위
#define		HATE_FIRST_ATTACK		1500					// 선공시 HATE 수치값 1/10000 단위
#define		NPC_DISABLE_TIME		(PULSE_REAL_SEC * 3)	// npc 리젠 후 안보이면서 움직이는 시간 : 3 sec
#define		NPC_ACTIVITY_DELAY		10						// npc 행동 딜레이 10 : 1초
#define		MAX_PENALTY_EXP_LEVEL	(7.01f)					// 레벨차에 따른 최대 페널티 경험치 : 7레벨
#define		MAX_PENALTY_SP_LEVEL	10						// 레벨차에 따른 최대 페널티 SP : 10레벨
#define		DOWN_LEVEL_EXP			10						// 하위 레벨 몬스터 잡았을때 경험치 하락 수치 %
#define		DOWN_LEVEL_SP			10						// 하위 레벨 몬스터 잡았을때 SP 하락 수치 %
#define		UP_LEVEL_SP				5						// 상위 레벨 몬스터 잡았을때 SP 상승 수치 %
#define		MONEY_DROP_PROB			8000					// pc -> npc npc죽을때 돈 드롭 확률
#define		UNIT_CEHCK_BLOCK		0.5f					// 몬스터 공격시 블럭 체크 기준 : 0.5m
#define		NPC_ATTACK_DELETE_PULSE	(PULSE_REAL_MIN * 3)	// 공격제한Pulse 해당시간동안 공격안했을 시 어택리스트에서 삭제

#define		ACCESSORY_USED_ATTACK		10			// 공격 시 악세사리 내구도 10 감소
#define		ACCESSORY_USED_DEFENSE		10			// 방어 시 악세사리 내구도 10 감소
#define		ACCESSORY_USED_DEATH		200			// 사망 시 악세사리 내구도 200 감소

// 사망시 경험치 패널티
#define		DEATH_PENALTY_EXP_1		((double)0.00)			// 1 ~ 10
#define		DEATH_PENALTY_EXP_2		((double)0.05)			// 11 ~ 20
#define		DEATH_PENALTY_EXP_3		((double)0.03)			// 21 ~ 35
#define		DEATH_PENALTY_EXP_4		((double)0.02)			// 36 ~

// 사망시 SP 패널티
#define		DEATH_PENALTY_SP_1		((double)0.00)			// 1 ~ 10
#define		DEATH_PENALTY_SP_2		((double)0.05)			// 11 ~ 20
#define		DEATH_PENALTY_SP_3		((double)0.03)			// 21 ~ 35
#define		DEATH_PENALTY_SP_4		((double)0.02)			// 36 ~

// 명중 플래그
#define		HITTYPE_MISS			(1 << 0)		// 회피
#define		HITTYPE_WEAK			(1 << 1)		// 약하게
#define		HITTYPE_NORMAL			(1 << 2)		// 보통
#define		HITTYPE_STRONG			(1 << 3)		// 강하게
#define		HITTYPE_HARD			(1 << 4)		// 매우 강하게
#define		HITTYPE_CRITICAL		(1 << 5)		// 크리티컬
#define		HITTYPE_DEADLY			(1 << 6)		// 죽도록 강하게

#define		HITTYPE_MAX_PROB_MISS		5000		// 미스
#define		HITTYPE_MAX_PROB_WEAK		3000		// 위크
#define		HITTYPE_MAX_PROB_STRONG		3000		// 스트롱
#define		HITTYPE_MAX_PROB_HARD		2000		// 하드
#define		HITTYPE_MAX_PROB_CRITICAL	1500		// 크리
#define		HITTYPE_MAX_PROB_DEADLY		1000		// 데들리

// 기본 공격 거리
#define		ATTACK_RANGE_TITAN				2.3f	// 타이탄
#define		ATTACK_RANGE_KNIGHT				2.3f	// 나이트
#define		ATTACK_RANGE_HEALER				20.0f	// 힐러
#define		ATTACK_RANGE_MAGE				15.0f	// 메이지
#define		ATTACK_RANGE_ROGUE_1			2.3f	// 로그 : 단검
#define		ATTACK_RANGE_ROGUE_2			15.0f	// 로그 : 석궁
#define		ATTACK_RANGE_SORCERER			3.0f	// 소서러
#define		ATTACK_RANGE_SORCERER_HELLOUND	15.0f	// 소서러 : 강신 : 헬운드
#define		ATTACK_RANGE_SORCERER_ELENEN	3.0f	// 소서러 : 강신 : 엘레넨
#define		ATTACK_RANGE_NIGHTSHADOW		15.0f	// 나이트 쉐도우

///////////////////////////
// 파티 관련

#define		MAX_PARTY_MEMBER		8			// 최대 파티 인원
#define		PARTY_PLUS_EXP			10			// 파티 사냥시 추가 경험치 : 10%
#define		PARTY_PLUS_SP			10			// 파티 사냥시 추가 SP : 10%
#define		PARTY_PLUS_EXP_COUNT	5			// 파티 인원에 따른 추가 경험치 : 5%
#define		PARTY_PLUS_SP_COUNT		5			// 파티 인원에 따른 추가 SP : 5%

// 파티 경험치 배분시
#define		PARTY_EXP_DAMAGE		60			// 데미지 비율 경험치
#define		PARTY_EXP_SAME			20			// 파티 균등 경험치
#define		PARTY_EXP_LEVEL			20			// 레벨 비율 경험치
// 파티 SP 배분시
#define		PARTY_SP_DAMAGE			60			// 데미지 비율 SP
#define		PARTY_SP_SAME			20			// 파티 균등 SP
#define		PARTY_SP_LEVEL			20			// 레벨 비율 SP

/////////////////////////
// character 상태 관련

#define		NPC_RECOVER_PULSE		(PULSE_REAL_SEC * 3)		// npc 상태 회복 : 3초
#define		PC_RECOVER_PULSE		(PULSE_REAL_SEC * 3)		// pc 상태 회복 : 3초

#define		ASSIST_TYPE_SKILL		0		// 보조 종류 : 스킬
#define		ASSIST_TYPE_ITEM		1		// 보조 종류 : 아이템

#define		PLAYER_STATE_RESET			(0 << 0)	// 초기화
#define		PLAYER_STATE_SITDOWN		(1 << 0)	// 앉기/서기
#define		PLAYER_STATE_MOVING			(1 << 1)	// 이동/정지
#define		PLAYER_STATE_WARP			(1 << 2)	// 귀환 중
#define		PLAYER_STATE_PKMODE			(1 << 3)	// PK 모드				// ADULT_SERVER -> PK 가해자
#define		PLAYER_STATE_PKMODEDELAY	(1 << 4)	// PK 모드 해제 딜레이	// ADULT_SERVER -> 사용안함
#define		PLAYER_STATE_RAMODE			(1 << 5)	// 정당방위 모드
#define		PLAYER_STATE_CHANGE			(1 << 6)	// 변신중
#define		PLAYER_STATE_BILLITEM		(1 << 7)	// 빌링아이템 지급중
#define		PLAYER_STATE_CASHITEM		(1 << 8)	// 캐쉬아이템 지급중
#define		PLAYER_STATE_SUPPORTER		(1 << 9)	// 게임 서포터 플래그 : 특정 아이템 착용시 이펙트
#define		PLAYER_STATE_OTHERZONE		(1 << 10)	// 다른 속성 존
#define		PLAYER_CRISTAL_RESPOND		(1 << 11)	// 공성 크리스탈과 교감중
#define		PLAYER_STATE_FLYING			(1 << 12)	// 비행모드(나이트쉐도우)
#define		PLAYER_STATE_DARKNESS		(1 << 13)	// 사도모드(나이트쉐도우)
#define		PLAYER_STATE_SKILL			(1 << 14)	// 스킬발동모드(나이트쉐도우)
#define		PLAYER_STATE_CASH_ZONE_MOVE (1 << 15)	// 캐쉬아이템(소환, 찾아가기, 파티리콜)
#define		PLAYER_STATE_INVINCIBILITY	(1 << 16)	// 무적 상태

#define		RAMODE_USE_PULSE			(3 * TIME_ONE_MIN)	// 정당방위 유지 시간 3min

#define		RAMODE_DELAY_PULSE			(5 * TIME_ONE_MIN)	// 정당방위 해제 알림 5s
#define		RAMODE_MAX_TARGET			8			// 정당방위 적용 최대 8
#define		MAX_PK_COUNT				150			// 최대 PK 누적 수치
#define		PK_HUNTER_POINT_MAX			 32000		// 헌터 맥스 수치
#define		PK_HUNTER_POINT_2			 19000		// 헌터 2단계
#define		PK_HUNTER_POINT_1			  6000		// 헌터 1단계
#define		PK_CHAOTIC_POINT_1			 -6000		// 카오 1단계
#define		PK_CHAOTIC_POINT_2			-19000		// 카오 2단계
#define		PK_CHAOTIC_POINT_MAX		-32000		// 카오 맥스 수치

#ifdef PKMODE_LIMIT_LEVEL
#else
#define		PKMODE_LIMIT_LEVEL			15			// 레벨 이하 PK 못함
#endif

#define     PK_NAME_CHAOSLEGION         (-5)
#define     PK_NAME_NECROKNIGHT         (-4)
#define     PK_NAME_OUTLAW              (-3)
#define     PK_NAME_FEARCASTER          (-2)
#define     PK_NAME_MURDERER            (-1)
#define     PK_NAME_NORMAL              ( 0)
#define     PK_NAME_AVENGER             (+1)
#define     PK_NAME_LIGHTPORTER         (+2)
#define     PK_NAME_GUARDIAN            (+3)
#define     PK_NAME_SAINTKNIGHT         (+4)
#define     PK_NAME_DIVINEPROTECTOR     (+5)

/////////////////////////
// Quick Slot 관련

#define		QUICKSLOT_PAGE_NUM		3		// 3페이지
#define		QUICKSLOT_MAXSLOT		12		// 10개 단축창 F1-F10

#define		QUICKSLOT_TYPE_EMPTY		-1		// 슬롯 비었음
#define		QUICKSLOT_TYPE_SKILL		0		// 스킬타입
#define		QUICKSLOT_TYPE_ACTION		1		// 액션타입
#define		QUICKSLOT_TYPE_ITEM			2		// 아이템타입
#define		QUICKSLOT_TYPE_ITEM_WEAR	3	    // 장착 아이템 타입

///////////////////////
// 메세지 보내기 매크로
#define SEND_Q(msg, desc)		{ if (desc != NULL) (desc)->WriteToOutput(msg); }

////////////////////////
// 컨넥터 관련 유저 상태
#define CONN_WAIT_IDEN			0			// 빌링 시스템 인증 대기
#define CONN_PLAYING			1			// 플레이중
#define CONN_CHANGE_SERVER		2			// 서버 이동 중

#define LOGIN_STATE_FAIL		-1
#define LOGIN_STATE_NOT			0
#define LOGIN_STATE_SUCCESS		1
#define LOGIN_STATE_NOPLAY		2

///////////////////////
// 명중, 회피 관련 상수
#define		MIN_HITRATE		1000		// 최소 명중 10%
#define		MAX_HITRATE		9500		// 최대 명중 95%
#define		MIN_CRITICAL	500			// 최소 크리티컬 5%
#define		MAX_CRITICAL	6600		// 최대 크리티컬 66%

// 기타 이펙트 타입
#define	EFFECT_ETC_LEVELUP		0	// 레벨업 이펙트
#define	EFFECT_ETC_GOZONE		1	// go_zone 이펙트

// 액션 종류
#define ACTION_GENERAL			0	// 일반
#define ACTION_SOCIAL			1	// 소셜
#define ACTION_PARTY			2	// 파티
#define ACTION_GUILD			3	// 길드
#define ACTION_PET				4	// 펫
#define ACTION_TITLE			5	// 호칭 시스템

#define	AGT_SITDOWN				3	// 앉기서기
#define	AGT_PKMODE				23	// PK 선언
#define AGT_PET_SITDOWN			37	// 펫 앉기
#define AGT_THROW_WATER			42	// 물뿌리기
#define AGT_FACTORY				46	// 제작

///////////////////////
// 존 번호
#define ZONE_START						0		// 시작 마을
#define ZONE_DUNGEON1					1		// 던전 1
#define ZONE_SINGLE_DUNGEON1			2		// 싱글 던전 1
#define ZONE_DUNGEON2					3		// 던전 2
#define ZONE_DRATAN						4		// 필드 2 드라탄
#define ZONE_SINGLE_DUNGEON2			5		// 싱글 던전 2
#define	ZONE_SINGLE_DUNGEON_TUTORIAL	6		// 싱글 던전 1 튜토리얼판
#define	ZONE_MERAC						7		// 메라크
#define	ZONE_GUILDROOM					8		// 길드룸
#define	ZONE_DUNGEON3					9		// 던전 3
#define	ZONE_SINGLE_DUNGEON3			10		// 싱글 던전 3
#define	ZONE_SINGLE_DUNGEON4			11		// 싱글 던전 4
#define ZONE_DUNGEON4					12		// 던전 4: 드라탄
#define ZONE_FREE_PK_DUNGEON			13		// PK 던전
#define ZONE_OXQUIZROOM					14		// OX 퀴즈방
#define ZONE_EGEHA						15		// 에게하
#define ZONE_EGEHA_PK					16		// 에케하 PK존
#define ZONE_EGEHA_DUNGEON_1			17		// 에게하 던전 1F~7F
#define ZONE_EGEHA_DUNGEON_8			18		// 에게하 던전 8F
#define ZONE_EGEHA_DUNGEON_9			19		// 에게하 던전 9F
#define ZONE_EGEHA_DUNGEON_10			20		// 에게하 던전 10F

#define ZONE_DRATAN_CASTLE_DUNGEON		21		// 드라탄 공성 보상 던전
#define ZONE_COMBO_DUNGEON				22		// 몬스터 콤보 던전
#define ZONE_STREIANA					23		// 스트레이아나
#define ZONE_PK_TOURNAMENT				24		// 붉은모래바람 투기장
#define ZONE_EXTREME_CUBE				25		// 큐브
#define ZONE_SPRIT_CAVE					26		// 영혼의 동굴
#define ZONE_QUANIAN_CAVE				27		// 콰니안의 동굴
#define ZONE_GOLEM_CAVE					28		// 골렘의 동굴
#define ZONE_EBONY_MINE					29		// 에보니탄광
#define ZONE_MISTY_CANYON				30		// 미스티협곡
#define ZONE_FLORAIM_CAVE				31		// 플로라임 동굴
#define ZONE_MONDSHINE					32		// 몬드샤인
#define ZONE_CAPPELLA_1					33		// 예배당 1차
#define ZONE_CAPPELLA_2					34		// 예배당 2차
#define ZONE_ALTER_OF_DARK				35		// 암흑의 제단
#define ZONE_AKAN_TEMPLE				36		// 아칸사원 존
#define ZONE_TRIVIA_CANYON				37		// 트리비아
#define ZONE_ROYAL_RUMBLE				38		// 로얄럼블 존
#define ZONE_TARIAN						39		// 타리안
#define ZONE_BLOODYMIR					40		// 블러디미르
#define ZONE_TARIAN_DUNGEON				41		// 람스로의 지하던전 - 타리안 던전
#define ZONE_RVR						42		// RVR Test 던전

///////////////////////////////////////
// Quest 관련

// 종류별 타입
#define		QTYPE_KIND_REPEAT				0		// 반복형 퀘스트
#define		QTYPE_KIND_COLLECTION			1		// 수집형 퀘스트
#define		QTYPE_KIND_DELIVERY				2		// 전달형 퀘스트
#define		QTYPE_KIND_DEFEAT				3		// 격파 퀘스트(싱글던전1)
#define		QTYPE_KIND_SAVE					4		// 구출 퀘스트(싱글던전2)
#define		QTYPE_KIND_MINING_EXPERIENCE	5		// 채굴 체험 퀘스트
#define		QTYPE_KIND_GATHERING_EXPERIENCE	6		// 채집 체험 퀘스트
#define		QTYPE_KIND_CHARGE_EXPERIENCE	7		// 차지 체험 퀘스트
#define		QTYPE_KIND_PROCESS_EXPERIENCE	8		// 가공 체험 퀘스트
#define		QTYPE_KIND_MAKE_EXPERIENCE		9		// 제작 체험 퀘스트
#define		QTYPE_KIND_TUTORIAL				10		// 튜토리얼 퀘스트
#define		QTYPE_KIND_PK					11		// PK 퀘스트
#define		QTYPE_KIND_SEARCH				12		// 탐색 퀘스트

// 파티스케일
#define		QTYPE_SCALE_PERSONAL		0
#define		QTYPE_SCALE_PARTY			1
#define     QTYPE_SCALE_BATTLEGROUP		2

// 수행별 타입
#define		QTYPE_REPEAT_ONCE			0		// 1회용 퀘스트
#define		QTYPE_REPEAT_UNLIMITED		1		// 무한 퀘스트
#define		QTYPE_REPEAT_DAY			2		// 일일 퀘스트

// 시작 타입
#define		QSTART_NPC					0		// NPC를 통한 시작
#define		QSTART_ITEM					1		// 아이템을 통한 시작
#define		QSTART_LEVEL				2		// 레벨 업에 의해 시작
#define		QSTART_AREA					3		// 특정지역에 도달하여 시작

// 퀘스트 수행 조건
#define		QCONDITION_NPC				0		// NPC
#define		QCONDITION_ITEM				1		// 아이템
#define		QCONDITION_ITEM_NORMAL		2		// 아이템(일반)
#define		QCONDITION_PC				3		// PC (캐릭터 PK)
#define		QCONDITION_AREA				4
#define		QCONDITION_ITEM_USE			5		// 아이템(사용)
#define		QCONDITION_TRIGGER			6		// 트리거 발동

// 퀘스트 보상 종류
#define		QPRIZE_ITEM					0		// 아이템
#define		QPRIZE_MONEY				1		// 나스
#define		QPRIZE_EXP					2		// 경험치
#define		QPRIZE_SP					3		// 스킬포인트
#define		QPRIZE_SKILL				4		// 스킬
#define		QPRIZE_SSKILL				5		// 특수스킬
#define		QPRIZE_STATPOINT			6		// 스뎃포인트
#define		QPRIZE_RVRPOINT				7		// RVR 포인트

#define		QUEST_MAX_PC				16		// 캐릭터 멀티 퀘스트 10개 -> 16
#define		QUEST_MAX_NEED_ITEM			5		// 퀘스트 시 필요아이템 최대 5

#define		QUEST_MAX_CONDITION			3		// 퀘스트 수행조건 최대 3
#define		QUEST_MAX_CONDITION_DATA	4		// 퀘스트 수행조건 하나당 부가 데이타 최대 4
#define		QUEST_MAX_ITEM_DROP_MONSTER	3		// 수집 퀘스트 : 아이템 드롭 몬스터 최대 3

#define		QUEST_MAX_PRIZE				5		// 퀘스트 보상 최대 5

#define		QUEST_MAX_OPTPRIZE			7

#define		QUEST_MAX_NPC				50		// npc 하나당 최대 퀘스트 보유 수 50
#define		QUEST_MAX_PC_COMPLETE		100		// pc가 완료한 1회용 퀘스트 최대 수 100

// 퀘스트 상태 상수
#define		QUEST_STATE_ALL				0		// 전체 퀘스트 검색시
#define		QUEST_STATE_INIT			1		// 퀘스트 초기 상태
#define		QUEST_STATE_RUN				2		// 퀘스트 실행 중
#define		QUEST_STATE_DONE			3		// 퀘스트 완료 상태 (완전 완료)
#define		QUEST_STATE_ABANDON			4		// 퀘스트 포기 상태

#define		S2_TICKET_MOB_START			13		// 싱글던전2 입장권 주기 (몹잡았을때) 몹레벨 시작
#define		S2_TICKET_MOB_END			27		// 싱글던전2 입장권 주기 (몹잡았을때) 몹레벨 끝

#define		S2_TICKET_LEVEL_START		13		// 싱글던전2 입장권 주기 (레벨업) pc레벨 시작
#define		S2_TICKET_LEVEL_END			27		// 싱글던전2 입장권 주기 (레벨업) pc레벨 끝

#define		S2_TICKET_DROP_PROB			50		// 싱글던전2 입장권 드롭 확률

#define		S2_TICKET_QUEST_DONE		5		// 5회 퀘스트 완료하면 입장권 제공

////////////////////////////
// 아이템 드랍 확률
#define		DROP_PHOENIX_MEMBERS_TICKET		10		// 피닉스 가입권 드랍 확률  0.1%

/////////////////////
// 이벤트 관련 상수들
// #define BETAEVENT						// 클베 이벤트

/////////////////////
// 옵션 관련 항목들
//
#define		OPTION_STR_UP				0		// 힘 상승
#define		OPTION_DEX_UP				1		// 민첩 상승
#define		OPTION_INT_UP				2		// 지혜 상승
#define		OPTION_CON_UP				3		// 체질 상승
#define		OPTION_HP_UP				4		// 생명력 상승
#define		OPTION_MP_UP				5		// 마나 상승

#define		OPTION_DAMAGE_UP			6		// 물리 공격력 상승
#define		OPTION_MELEE_DAMAGE_UP		7		// 물리 근거리 공격력 상승
#define		OPTION_RANGE_DAMAGE_UP		8		// 물리 원거리 공격력 상승
#define		OPTION_MELEE_HIT_UP			9		// 물리 근접 명중률 상승
#define		OPTION_RANGE_HIT_UP			10		// 물리 원거리 명중률 상승

#define		OPTION_DEFENSE_UP			11		// 물리 방어력 상승
#define		OPTION_MELEE_DEFENSE_UP		12		// 물리 근거리 방어력 상승
#define		OPTION_RANGE_DEFENSE_UP		13		// 물리 원거리 방어력 상승
#define		OPTION_MELEE_AVOID_UP		14		// 물리 근접 회피율 상승
#define		OPTION_RANGE_AVOID_UP		15		// 물리 원거리 회피율 상승

#define		OPTION_MAGIC_UP				16		// 마법 공격력 상승
#define		OPTION_MAGIC_HIT_UP			17		// 마법 명중률 상승
#define		OPTION_RESIST_UP			18		// 마법 방어력 상승
#define		OPTION_RESIST_AVOID_UP		19		// 마법 회피율 상승

#define		OPTION_ALL_DAMAGE_UP		20		// 전체 공격력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_HIT_UP			21		// 전체 명중률 상승 (근접물리, 원격물리, 마법)

#define		OPTION_ALL_DEFENSE_UP		22		// 전체 방어력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_AVOID_UP			23		// 전체 회피율 상승 (근접물리, 원격물리, 마법)

#define		OPTION_NOT_USED_24			24		// 사용 안함
#define		OPTION_NOT_USED_25			25		// 사용 안함

#define		OPTION_ATTR_FIRE			26
#define		OPTION_ATTR_WATER			27
#define		OPTION_ATTR_WIND			28
#define		OPTION_ATTR_EARTH			29
#define		OPTION_ATTR_DARK			30
#define		OPTION_ATTR_LIGHT			31

// 공성 조합 아이템용 옵션
#define		OPTION_MIX_STR				36		// 힘
#define		OPTION_MIX_DEX				37		// 민첩
#define		OPTION_MIX_INT				38		// 지혜
#define		OPTION_MIX_CON				39		// 체질

#define		OPTION_MIX_ATTACK			40		// 물공
#define		OPTION_MIX_MAGIC			41		// 마공
#define		OPTION_MIX_DEFENSE			42		// 물방
#define		OPTION_MIX_RESIST			43		// 마방

#define		OPTION_MIX_STURN			44		// 스턴
#define		OPTION_MIX_BLOOD			45		// 블러드
#define		OPTION_MIX_MOVE				46		// 무브
#define		OPTION_MIX_POISON			47		// 포이즌
#define		OPTION_MIX_SLOW				48		// 슬로우
// -- 공성 조합 아이템용 옵션

// 2006 하반기 신규 옵션
#define		OPTION_DOWN_LIMITLEVEL		49		// 착용제한 레벨 다운
#define		OPTION_INCREASE_INVEN		50		// HP회복 정수값 (2013 07버젼까지는 소지량 증가였음)
#define		OPTION_STEAL_MP				51		// 마나 흡수
#define		OPTION_STEAL_HP				52		// 생명력 흡수
#define		OPTION_ATTACK_BLIND			53		// 암흑 공격
#define		OPTION_ATTACK_POISON		54		// 독 공격
#define		OPTION_ATTACK_CRITICAL		55		// 크리티컬 확률 증가
#define		OPTION_RECOVER_HP			56		// HP 회복력 상승
#define		OPTION_RECOVER_MP			57		// MP 회복력 상승
#define		OPTION_DECREASE_SKILL_DELAY	58		// 스킬 쿨타임 감소
#define		OPTION_DECREASE_SKILL_MP	59		// MP 소모량 감소
#define		OPTION_RESIST_STONE			60		// 스톤 내성 증가
#define		OPTION_RESIST_STURN			61		// 스턴 내성 증가
#define		OPTION_RESIST_SILENT		62		// 침묵 내성 증가
#define		OPTION_BLOCKING				63		// 블럭률 증가
#define		OPTION_MOVESPEED			64		// 이동 속도 향상
// --- 2006 하반기 신규 옵션
#define		OPTION_FLYSPEED				65		// 비행 속도 향상
#define     OPTION_ATTACK_DEADLY        66      // 데들리 확률 증가

#define		OPTION_STR_UP_RATE			67
#define		OPTION_DEX_UP_RATE			68
#define		OPTION_INT_UP_RATE			69
#define		OPTION_CON_UP_RATE			70
#define		OPTION_HP_UP_RATE			71
#define		OPTION_MP_UP_RATE			72
#define		OPTION_WEAPON_UP_RATE		73
#define		OPTION_ARMOR_UP_RATE		74
#define		OPTION_MELEE_HIT_UP_RATE	75
#define		OPTION_MAGIC_HIT_UP_RATE	76
#define		OPTION_MELEE_AVOID_RATE		77
#define		OPTION_MAGIC_AVOID_RATE		78
#define		OPTION_RECOVER_HP_RATE		79
#define		OPTION_RECOVER_MP_RATE		80
#define		OPTION_TEST_QUEST			81
#define		OPTION_EXP_UP_RATE			82
#define		OPTION_SP_UP_RATE			83
#define		OPTION_APET_TRANS_END		84
//보석 컨텐츠가 들어가면서 추가된 보석의 옵션 타입
#define		OPTION_APET_ELEMENT_HPUP	85
#define		OPTION_APET_ELEMENT_ATTUP	86
#define		OPTION_ATT_FIRE_UP			87		// 화염 속성 공격 레벨 상승
#define		OPTION_ATT_WATER_UP			88		// 빙정 속성 공격 레벨 상승
#define		OPTION_ATT_WIND_UP			89		// 폭풍 속성 공격 레벨 상승
#define		OPTION_ATT_EARTH_UP			90		// 대지 속성 공격 레벨 상승
#define		OPTION_ATT_DARK_UP			91		// 어둠 속성 공격 레벨 상승
#define		OPTION_ATT_LIGHT_UP			92		// 빛 속성 공격 레벨 상승
#define		OPTION_DEF_FIRE_UP			93		// 화염 속성 방어 레벨 상승
#define		OPTION_DEF_WATER_UP			94		// 빙정 속성 방어 레벨 상승
#define		OPTION_DEF_WIND_UP			95		// 폭풍 속성 방어 레벨 상승
#define		OPTION_DEF_EARTH_UP			96		// 대지 속성 방어 레벨 상승
#define		OPTION_DEF_DARK_UP			97		// 어둠 속성 방어 레벨 상승
#define		OPTION_DEF_LIGHT_UP			98		// 빛 속성 방어 레벨 상승
#define		OPTION_ALL_STAT_UP			99		// 모든 스텟(4종) 상승
#define		OPTION_PVP_DAMAGE_ABSOLB	100		// PVP 데미지 흡수(%)
#define		OPTION_DEBUF_DECR_TIME		101		// 디버프 시간 감소(%)
#define		OPTION_RECOVER_HP_NOTRATE	102		// HP회복 정수값
#define		OPTION_RECOVER_MP_NOTRATE	103		// MP회복 정수값
#define		OPTION_INCREASE_STRONG		104		// 강인함 증가
#define		OPTION_INCREASE_HARD		105		// 하드 적중도 증가
#define		OPTION_INCREASE_HP			106		// hp 회복력 상승
#define		OPTION_CLIENT_1				107		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)
#define		OPTION_CLIENT_2				108		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)
#define		OPTION_CLIENT_3				109		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)
#define		OPTION_CLIENT_4				110		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)
#define		OPTION_CLIENT_5				111		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)
#define		OPTION_CLIENT_6				112		// 클라이언트 lod를 위한 데이터(기능은 하드코딩되어있음)

#define		MAX_NUM_OPTION				120

#define		OPTION_VALUE_SHIFT			8		// 2byte 값중 왼쪽 1Byte 옵션 번호 오른쪽 1Byte 옵션 레벨
#define		OPTION_MAX_LEVEL			7		// 옵션 최대 레벨 현재 7

#define		MAX_ITEM_OPTION				5		// 아이템에 붙는 최대 옵션
#define		MAX_ACCESSORY_OPTION		5		// 악세사리에 붙을 수 있는 최대 옵션 수
#define		MAX_WEAPON_OPTION			5		// 무기에 붙을 수 있는 최대 옵션 수
#define		MAX_ARMOR_OPTION			3		// 방어구에 붙을 수 있는 최대 옵션 수
#define		MAX_ITEM_OPTION_DEL			3		// 옵션을 지울 수 있는 옵션 수
#define		MAX_TITLE_OPTION			5		// 호칭에 붙는 최대 옵션 수

/////////////////
// 레어 옵션 관련

// 레어 등급
#define RARE_OPTION_GRADE_A			0
#define RARE_OPTION_GRADE_B			1
#define RARE_OPTION_GRADE_C			2
#define RARE_OPTION_GRADE_D			3
#define RARE_OPTION_GRADE_E			4

// 레어 타입
#define RARE_OPTION_TYPE_WEAPON		0
#define RARE_OPTION_TYPE_ARMOR		1
#define RARE_OPTION_TYPE_ACCESSORY	2

/////////////////
// 오리지널 옵션 관련
#define MAX_ORIGIN_OPTION			6
#define MAX_ORIGIN_SKILL			3
#define MAX_ORIGIN					MAX_ORIGIN_OPTION + MAX_ORIGIN_SKILL

#define ORIGIN_OPTION_BELONG_NONE	-1
#define ORIGIN_OPTION_BELONG_WEAR	0			// 착용 시 귀속

/////////////////////////////
// Blood Point 관련

#define		PC_BLOOD_POINT				5000	// 기본 Blood Point
#define		ATTACK_BLOOD_POINT			15		// 공격시 감소 Blood Point
#define		DEFENSE_BLOOD_POINT			3		// 방어시 감소 Blood Point

///////////////////////////////
// 생산 관련 항목들
//
#define		MAX_PRODUCTS_FROM_NPC		5		// npc가 내뱉는 생산품 최대 수
#define		MAX_ACCEPTABLE				4		// npc 한마리당 채집 가능 인원
#define		PER_DAMAGE_GET_PRODUCT		5		// 5 때릴때 마다 생산품 취득
#define		MAX_MAKE_ITEM_MATERIAL		10		// 아이템 제작시 최대 재료 수
#define		NPC_PRODUCE_DELETE_PULSE	(PULSE_REAL_SEC * 4)	// 생산 제한Pulse 해당시간동안 생산안했을 시 어택리스트에서 삭제

#define		MAX_ACCESSORY_MIX			3		// 재활의 상자에 들어가능 아이템 수
#define		MAX_ARCANE_MIX				3		// 비밀의 상자에 들어가는 아이템 수

#define		ITEM_ARCANE_MATERIAL_UPGRADE		0	// 비밀의 상자 재료 0번 제련석
#define		ITEM_ARCANE_MATERIAL_ACCESSORY		1	// 비밀의 상자 재료 1번 악세사리
#define		ITEM_ARCANE_MATERIAL_SAMPLE			2	// 비밀의 상자 재료 2번 시료

#define		MAKE_SAMPLE_ON_PROCESS_PROB		600		// 가공시 만들어질 사료의 확률

#define		MAX_ITEM_MAKE_SUCCESS_PROB	8000	// 아이템 제작 최대 성공률 80%

#define		ETERNAL_PENALTY_PROB		30		// 지속은 비지속의 20% 성공률
#define		PUBLIC_PENALTY_PROB			60		// 공유지는 사유지의 60% 성공률

#define		GetMakeProb(limitLevel, itemLevel) (limitLevel - itemLevel) * 250

/////////////////////////////
// Special Skill 관련
//

// Special Skill 타입
#define		SSKILL_MINING		0	// 채굴
#define		SSKILL_GATHERING	1	// 채집
#define		SSKILL_CHARGE		2	// 차지
#define		SSKILL_STONE		3	// 광석정련술
#define		SSKILL_PLANT		4	// 식물가공술
#define		SSKILL_ELEMENT		5	// 원소정제술

#define		SSKILL_MAKE_WEAPON	6	// 무기 제작 기술
#define		SSKILL_MAKE_WEAR	7	// 방어구 제작 기술
#define		SSKILL_MAKE_G_B		8	// 장갑 부츠 제작 기술
#define		SSKILL_MAKE_ARMOR	9	// 갑바 바지 제작 기술
#define		SSKILL_MAKE_H_S		10	// 투구 방패 제작 기술

#define		SSKILL_MAX_LEVEL	5	// Special Skill 최대 레벨

//////////////////////////////////////
// 사제 시스템 관련
//

#define		TEACH_MAX_STUDENTS			10			// 최대 견습생 수
#define		TEACH_LEVEL_TEACHER			50			// 후견인 가능 레벨
#define		TEACH_LEVEL_STUDENT			30			// 견습생 가능 레벨
#define		TEACH_LEVEL_SUCCESS			50			// 사제 성공 레벨
#define		TEACH_LIMIT_DAY				10			// 10일 동안 견습생이 접속을 않하면 사제 관계를 끊는다.
#define		TEACH_SP_STUDENT			5			// 견습생이 사냥시 제자가 얻는 보너스 SP 5%
#define		TEACH_SP_TEACHER			2			// 견습생이 사냥시 선생이 얻는 보너스 SP 2%
#define		TEACH_STUDENT_MAX_GIVEUP	3			// 견습생이 포기 가능한 최대 횟수

#define		TEACH_FAME_PRIZE			10			// 보상 명성치

#define		CANCELTEACHER				(1 << 0)	// 선생만 해제
#define		CANCELSTUDENT				(1 << 1)	// 학생만 해제

///////////////
// 펫 관련 상수

#define MAX_OWN_PET							2				// 펫을 소유 할수 있는 갯수
#define PET_HORSE_QUEST_INDEX				106				// 말퀘스트 인덱스
#define PET_DRAGON_QUEST_INDEX				109				// 용퀘스트 인덱스
#define PET_BLUE_HORSE_QUEST_INDEX			145				// 파란 말퀘스트 인덱스
#define PET_PINK_DRAGON_QUEST_INDEX			146				// 핑크 용퀘스트 인덱스
#define PET_UNKOWN_HORSE_QUEST_INDEX		147				// 불가사리 말퀘스트 인덱스
#define PET_UNKOWN_DRAGON_QUEST_INDEX		148				// 불가사리 용퀘스트 인덱스

#define	PET_HORSE_EGG_INDEX				948				// 말 알 인덱스
#define	PET_DRAGON_EGG_INDEX			949				// 용 알 인덱스
#define	PET_BLUE_HORSE_EGG_INDEX		1707			// 블루말 알 인덱스
#define	PET_PINK_DRAGON_EGG_INDEX		1706			// 핑크용 알 인덱스
#define	PET_UNKOWN_HORSE_EGG_INDEX		1709			// 불가사의말 알 인덱스
#define	PET_UNKOWN_DRAGON_EGG_INDEX		1708			// 불가사의용 알 인덱스

#define PET_HORSE_ITEM_INDEX			871				// 말 아이템 인덱스
#define PET_DRAGON_ITEM_INDEX			872				// 용 아이템 인덱스
#define PET_BLUE_HORSE_ITEM_INDEX		1710			// 말 아이템 인덱스
#define PET_PINK_DRAGON_ITEM_INDEX		1711			// 용 아이템 인덱스
#define PET_UNKOWN_HORSE_ITEM_INDEX		1713			// 말 아이템 인덱스
#define PET_UNKOWN_DRAGON_ITEM_INDEX	1712			// 용 아이템 인덱스

#define PET_TYPE_HORSE					(0x10)		// 말
#define PET_TYPE_DRAGON					(0x20)		// 용
#define PET_TYPE_BLUE_HORSE				(0x30)		// 파란 말
#define PET_TYPE_PINK_DRAGON			(0x40)		// 핑크 용
#define PET_TYPE_UNKOWN_HORSE			(0x50)		// 불가사의말
#define PET_TYPE_UNKOWN_DRAGON			(0x60)		// 불가사의용

#define PET_GRADE_CHILD		(0X01)		// 유년기
#define PET_GRADE_ADULT		(0X02)		// 성숙기
#define PET_GRADE_MOUNT		(0X03)		// 탈것

#define PET_TYPE_MASK		(0xf0)		// 타입 마스크
#define PET_GRADE_MASK		(0x0f)		// 등급 마스크

#define PET_DEFAULT_HUNGRY		50		// 최초 배고픔 수치
#define PET_DEFAULT_SYMPATHY	50		// 최초 교감도 수치
#define PET_DEFAULT_RUNSPEED	15.0f	// 기본 달리기 속도
#define PET_MAX_HUNGRY			100		// 최대 배고픔 수치
#define PET_MAX_SYMPATHY		100		// 최대 교감도
#define PET_MAX_HP				100		// 최대 HP
#define PET_ADULT_LEVEL			16		// 성숙기 기준 레벨
#define PET_MOUNT_LEVEL			31		// 탈것 기준 레벨

#define PET_MAX_LEVEL			100		// 펫 최고 레벨 100
#define MAX_APET_LEVEL			165		// 공격형 펫 최고 레벨 165

typedef enum _tagPetColorType
{
	NON_COLOR		= 0,	// 기본
	RED_COLOR		= 1,	// 빨강
	SCARLET_COLOR	= 2,	// 주홍
	YELLOW_COLOR	= 3,	// 노랑
	GREEN_COLOR		= 4,	// 초록
	BLUE_COLOR		= 5,	// 파랑
	DEEP_BLUE_COLOR	= 6,	// 남색
	VIOLET_COLOR	= 7,	// 보라
	BLACK_COLOR		= 8,	// 검정
	WHITE_COLOR		= 9,	// 흰
	PHOENIX_COLOR	= 10,	// 피닉스 이벤트
	BLACK_HEART_COLOR	= 11,	// 검정 하트
	WHITE_HEART_COLOR	= 12,	// 하양 하트
	YELLOW_HEART_COLOR	= 13,	// 노랑 하트
	BLACK_DIAMOND_COLOR		= 14,	// 검정 다이아
	BLACK_CLOVER_COLOR		= 15,	// 검정 클로버
	BLACK_SPADE_COLOR		= 16,	// 검정 스페이드
	BLACK_SKELETON_COLOR	= 17,	// 검정 해골
	WHITE_DIAMOND_COLOR		= 18,	// 흰색 다이아
	WHITE_CLOVER_COLOR		= 19,	// 흰색 클로버
	WHITE_SPADE_COLOR		= 20,	// 흰색 스페이드
	WHITE_SKELETON_COLOR	= 21,	// 흰색 해골
	YELLOW_DIAMOND_COLOR	= 22,	// 노랑 다이아
	YELLOW_CLOVER_COLOR		= 23,	// 노랑 클로버
	YELLOW_SPADE_COLOR		= 24,	// 노랑 스페이드
	YELLOW_SKELETON_COLOR	= 25,	// 노랑 해골
	RED_HEART_COLOR		= 26,	// 빨강 하트
	RED_CLOVER_COLOR		= 27,	// 빨강 클로버
	RED_DIAMOND_COLOR		= 28,	// 빨강 다이아
	RED_SPADE_COLOR		= 29,	// 빨강 스페이드
	RED_SKELETON_COLOR		= 30,	// 빨강 해골
	SCARLET_HEART_COLOR		= 31,	// 주황 하트
	SCARLET_CLOVER_COLOR		= 32,	// 주황 클로버
	SCARLET_DIAMOND_COLOR		= 33,	// 주황 다이아
	SCARLET_SPADE_COLOR		= 34,	// 주황 스페이드
	SCARLET_SKELETON_COLOR	= 35,	// 주황 해골
	GREEN_HEART_COLOR		= 36,	// 초록 하트
	GREEN_CLOVER_COLOR		= 37,	// 초록 클로버
	GREEN_DIAMOND_COLOR		= 38,	// 초록 다이아
	GREEN_SPADE_COLOR		= 39,	// 초록 스페이드
	GREEN_SKELETON_COLOR		= 40,	// 초록 해골
	BLUE_HEART_COLOR		= 41,	// 파란 하트
	BLUE_CLOVER_COLOR		= 42,	// 파란 클로버
	BLUE_DIAMOND_COLOR		= 43,	// 파란 다이아
	BLUE_SPADE_COLOR		= 44,	// 파란 스페이드
	BLUE_SKELETON_COLOR		= 45,	// 파란 해골
	DEEP_BLUE_HEART_COLOR		= 46,	// 남색 하트
	DEEP_BLUE_CLOVER_COLOR	= 47,	// 남색 클로버
	DEEP_BLUE_DIAMOND_COLOR	= 48,	// 남색 다이아
	DEEP_BLUE_SPADE_COLOR		= 49,	// 남색 스페이드
	DEEP_BLUE_SKELETON_COLOR	= 50,	// 남색 해골
	VIOLET_HEART_COLOR			= 51,	// 보라 하트
	VIOLET_CLOVER_COLOR			= 52,	// 보라 클로버
	VIOLET_DIAMOND_COLOR		= 53,	// 보라 다이아
	VIOLET_SPADE_COLOR			= 54,	// 보라 스페이드
	VIOLET_SKELETON_COLOR		= 55,	// 보라 해골
	HORSE_WILLIAM_COLOR			= 56,	// 타는말 전용 WILLIAM 껍데기
	HORSE_ALBER_COLOR			= 57,	// 타는말 전용 ALBER 껍데기
	HORSE_REDEYE_COLOR			= 58,	// 타는말 전용 REDEYE 껍데기
	HORSE_ZEBRA_COLOR			= 59,	// 타는말 전용 ZEBRA  껍데기
	HORSE_HAROLD_COLOR			= 60,	// 타는말 전용 HAROLD  껍데기
	HORSE_VIOLET_COLOR			= 61,	// 타는말 전용 VIOLET  껍데기
} PET_COLOR_TYPE;

///////////////////
// 소환수 관련 상수

#define ELEMENTAL_FIRE			0		// 불의 정령
#define ELEMENTAL_WIND			1		// 바람의 정령
#define ELEMENTAL_EARTH			2		// 대지의 정령
#define ELEMENTAL_WATER			3		// 물의 정령
#define ELEMENTAL_GUARD			4		// 소환 경비병(임시)

#define ELEMENTAL_ATTACK_SPEED	20		// 기본 공속
#define ELEMENTAL_RUN_SPEED		8.0f	// 기본 이속
#define ELEMENTAL_RECOVER_HP	1		// 3초에 소환수 체력 회복량

#define ELEMENTAL_ATTACK_RANGE_FIRE		2.0f	// 공격거리 : 불의정령
#define ELEMENTAL_ATTACK_RANGE_WIND		7.0f	// 공격거리 : 바람의정령
#define ELEMENTAL_ATTACK_RANGE_EARTH	3.0f	// 공격거리 : 대지의정령
#define ELEMENTAL_ATTACK_RANGE_WATER	7.0f	// 공격거리 : 물의정령
#define ELEMENTAL_ATTACK_RANGE_GUARD	5.0f	// 공격거리 : 경비병(임시)

#if defined(LC_KOR) || defined(LC_USA) || defined (LC_GAMIGO) || defined(LC_BILA) || defined(LC_RUS) || defined(LC_TLD)
#define SUMMON_DELAY			(PULSE_REAL_SEC * 5)	// 소환 딜레이
#else
#define SUMMON_DELAY			(PULSE_REAL_MIN * 5)	// 소환 딜레이
#endif
#define SUMMON_DURATION			(TIME_ONE_MIN * 20)	// 소환 지속시간

/////////////////
// 강신 관련 상수
#define EVOCATION_NONE			0		// 강신 안함
#define EVOCATION_HELLOUND		313		// 헬운드 강신
#define EVOCATION_ELENEN		309		// 엘레넨 강신

#if defined (LC_RUS)
#define EVOCATION_DELAY			(PULSE_REAL_MIN * 0)	// 강신 딜레이
#else
#define EVOCATION_DELAY			(PULSE_REAL_MIN * 5)	// 강신 딜레이
#endif
#define EVOCATION_DURATION		(PULSE_REAL_MIN * 30)	// 강신 지속시간

////////////////////////////////////
// Event 관련

////////////////////
// Latto Event 관련
#define		LATTO_EVENT_UPGRADE_PROB		3000		// 10%
#define		LATTO_EVENT_BLOOD_PROB			3000		// 10%
#define		LATTO_EVENT_PARTY_PROB			5000		// 50%
#define		LATTO_EVENT_PROCESS_PROB		3000		// 30%
#define		LATTO_EVENT_EXPSP_PROB			3000		// 30%

#define		LATTO_EVENT_EXPSP_LEVEL			15			// 15 레벨
#define		LATTO_EVENT_EXP_PERCENT			30			// 기준 : 30%
#define		LATTO_EVENT_PARTY_LEVEL_MIN		5			// 5 레벨
#define		LATTO_EVENT_PARTY_LEVEL_MAX		15			// 15 레벨

#define		CHANGE_26LEVEL_GENERAL_STONE_PROB	800			// 26레벨 일반 제련석 나올 확률
#define		CHANGE_SPECIAL_STONE_PROB			900			// 고급제련석 나올 확률
#define		CHANGE_22LEVEL_EVENT_WEAPON_PROB	1000		// 22레벨 이벤트 무기 (종족별) 나올 확률
#define		CHANGE_31LEVEL_ARMOR_PROB			1100		// 31레벨 방어구 상의 (종족별) 나올 확률
#define		CHANGE_ATTACK_UP_PROB				5300		// 공격력 향상제 나올 확률
#define		CHANGE_DEFENSE_UP_PROB				9500		// 방어력 향상제 나올 확률
#define		CAHNGE_50000NAS_PROB				10000		// 5만 나스 나올 확률

///////////////////////
// New Year Evnet 관련
#define		NEWYEAR_EVENT_RICESOUP_DROP_PROB	1000	// 10%
#define		NEWYEAR_EVENT_RICEMANDO_DROP_PROB	1000	// 10%
#define		NEWYEAR_EVENT_RICESOUP_UP_EXP		12		// 120%
#define		NEWYEAR_EVENT_RICEMANDO_UP_SP		13		// 130%

#define CLIENT_OPTION_EFFECT	( 1 << 0 )
#define CLIENT_OPTION_WARP		( 1 << 1 )

#define GOLDENBALL_STATUS_NOTHING		0
#define GOLDENBALL_STATUS_VOTE			1
#define GOLDENBALL_STATUS_GIFT			2
#define GOLDENBALL_STATUS_VOTE_END		3
#define GOLDENBALL_TEAM_NAME_LENGTH		32
#define GOLDENBALL_BALL_INDEX			1482
#define GOLDENBALL_CARD_INDEX			1483

#define APET_WEAR_HEAD		0
#define APET_WEAR_BODY		1
#define APET_WEAR_WEAPON	2
#define APET_WEAR_ACCE		3
#define APET_WEARPOINT		4

#define APET_MAX_AI			6	// 최대 AI slot


// t_cashItemdate 필드명
#define CASH_ITEM_DATE_FIELD_MEMPOS			"a_mempos_new"
#define CASH_ITEM_DATE_FIELD_CHAR_SLOT0		"a_charslot0_new"
#define CASH_ITEM_DATE_FIELD_CHAR_SLOT1		"a_charslot1_new"
#define CASH_ITEM_DATE_FIELD_STASHEXT		"a_stashext_new"

#if defined (LC_KOR)
#define WAR_CASTLE_SUBNUMBER_MERAC	1	// 공성이 가능한 서브서버 번호
#define RAID_SUBNUMBER		1			// 원정대 레이드 전용 채널 지정
#elif defined(LC_GAMIGO) || defined (LC_BILA) || defined (LC_USA) || defined (LC_RUS)
#define WAR_CASTLE_SUBNUMBER_MERAC  4   // 공성이 가능한 서브서버 번호
#define RAID_SUBNUMBER		3			// 원정대 레이드 전용 채널 지정
#else
#define WAR_CASTLE_SUBNUMBER_MERAC	1	// 공성이 가능한 서브서버 번호
#define RAID_SUBNUMBER		3			// 원정대 레이드 전용 채널 지정
#endif

typedef struct _TimeStruct
{
	int		year;		// 연도 끝 두자리 % 62
	int		month;		// 월
	int		day;		// 일
	int		hour;		// 시
	int		min;		// 분
	int		sec;		// 초
} TIMESTRUCT;

#define HACK_LIMIT 0.06f
#define HACK_TYPE_PULSE		0
#define HACK_TYPE_CLIENT	1

#define STATE_MONSTER_TAMING			(1 << 0)	// 몬스터 테이밍 상태
#define STATE_MONSTER_CONFUSION			(1 << 1)	// 몬스터 혼란 상태
#define STATE_MONSTER_NO_SOUL			(1 << 3)	// 몬스터가 영혼을 빼앗긴 상태
#define STATE_MONSTER_MERCENARY			(1 << 4)	// 몬스터 용병 카드 사용 상태
#define STATE_MONSTER_TOTEM_BUFF		(1 << 5)	// 몬스터가 토템이되어 버프를 마구 뿌린다.
#define STATE_MONSTER_TOTEM_ATTK		(1 << 6)	// 몬스터가 토템이도어 공격을 하넹.
#define STATE_MONSTER_TRAP				(1 << 7)	// 몬스터가 트랩이다.
#define STATE_MONSTER_PARASITE			(1 << 9)	// 몬스터가 기생충 감염 상태.
#define STATE_MONSTER_SUICIDE			(1 << 10)	// 몬스터가 자살 공격을 한다.
#define STATE_MONSTER_TOTEM_ITEM_BUFF	(1 << 11)	// 아이템으로 소환한 토템
#define STATE_MONSTER_TOTEM_ITEM_ATTK	(1 << 12)	// 아이템으로 소환한 토템

#define TRIGGER_MAX_ARRAYCOUNT				10		// 트리거 맴버 변수 중 배열인 것의 최대 열 개수

//////////triggerSet_type///////
#define TRIGGERSET_ALTER_OF_DARK			1			// 암흑의 제단
#define TRIGGERSET_CAPPELLA_1				2			// 예배당 1차
#define TRIGGERSET_CAPPELLA_2				3			// 예배당 1차
#define TRIGGERSET_TESTRAID					4			// 테스트 Raid
#define TRIGGERSET_MONDSHINE				5			// 몬드샤인
#define TRIGGERSET_STREIANA					6			// 스트레이아나
#define TRIGGERSET_EGEHA					7			// 에게하

//////////trigger_type///////
#define TRIGGER_ITEM_COMPOSITION			1			// 아이템 결합
#define TRIGGER_ITEM_DROP					2			// 아이템 드랍
#define TRIGGER_OPEN_THE_DOOR				3			// 문 열기
#define TRIGGER_START_RAID					4			// 레이드 시작(시작 시간 설정 가능)
#define TRIGGER_MESSAGE_PRINT				5			// 게임 메세지 출력
#define TRIGGER_SUMMON_NPC					6			// npc 소환
#define TRIGGER_USE_DEBUF					7			// 디버프 걸림
#define TRIGGER_ACTIVE_OBJECT				8			// Object 활성화
#define TRIGGER_REMOVE_NPC					9			// npc 자동 제거
#define TRIGGER_TIME_LIMIT					10			// 시간 제한
#define TRIGGER_QUEST_COMPLETE				11			// 퀘스트 완료
#define TRIGGER_ITEM_GET_INVEN				12			// 아이템 획득
#define TRIGGER_CONSECUTIVE_DIALOGUE		13			// 다이얼로그 대화
#define TRIGGER_ACTIVE_EFFECT				14			// 이펙트 발생

//////////trigger_Subtype_OpenTheDoor///////
#define TRIGGER_SUBTYPE_NONE_OPEN_THE_DOOR	0
#define TRIGGER_SUBTYPE_NPC_OPEN_THE_DOOR	1
#define TRIGGER_SUBTYPE_ITEM_OPEN_THE_DOOR	2

//////////trigger_Subtype_OpenTheDoor///////
#define TRIGGER_SUBTYPE_NONE_REMOVE_NPC		0
#define TRIGGER_SUBTYPE_ITEM_REMOVE_NPC		1

////////// triggerflag ////////
#define TRIGGER_FLAG_NAMEDNPC_DEATH1					(1 << 0)	// 암흑의 제단 1차 네임드 NPC 죽은 것 여부
#define TRIGGER_FLAG_ACTIVE_OBJECT1						(1 << 1)	// 암흑의 제단 3차 오브젝트1 발동 여부
#define TRIGGER_FLAG_ACTIVE_OBJECT2						(1 << 2)	// 암흑의 제단 3차 오브젝트2 발동 여부
#define TRIGGER_FLAG_ACTIVE_OBJECT3						(1 << 3)	// 암흑의 제단 3차 오브젝트3 발동 여부
#define TRIGGER_FLAG_ACTIVE_OBJECT4						(1 << 4)	// 암흑의 제단 3차 오브젝트4 발동 여부
#define TRIGGER_FLAG_NAMEDNPC_SUMMON1					(1 << 5)	// 암흑의 제단 3차 네임드 NPC 소환 되었는지 여부
#define TRIGGER_FLAG_NAMEDNPC_DEATH2					(1 << 6)	// 암흑의 제단 3차 네임드 NPC 죽은 것 여부
#define TRIGGER_FLAG_NAMEDNPC_DEATH1_DOOR_SAVE			(1 << 7)	// 암흑의 제단 1차 네임드 NPC 죽으면 열리는 문 활성화 저장 시키기
#define TRIGGER_FLAG_NAMEDNPC_DEATH2_DOOR_SAVE			(1 << 8)	// 암흑의 제단 3차 네임드 NPC 죽으면 열리는 문 활성화 저장 시키기
#define TRIGGER_FLAG_NAMEDNPC_SUMMON_1003				(1 << 9)	// 암흑의 제단 3차 네임드 NPC 소환하는 제단 활성화 저장 시키기
#define TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE			(1 << 10)	// 암흑의 제단 1차 네임드 NPC 전에 죽었었는지 여부
#define TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE			(1 << 11)	// 암흑의 제단 3차 네임드 NPC 전에 죽었었는지 여부
#define TRIGGER_FLAG_NAMEDNPC_DEATH1018_BEFORE			(1 << 12)	// 암흑의 제단 5차 네임드 NPC 전에 죽었었는지 여부

////////// saveTriggerInfo ////////////
#define TRIGGER_SAVE_ALTER_OF_DARK_NONE		0			// 암흑의 제단 트리거 저장 초기화
#define TRIGGER_SAVE_ALTER_OF_DARK_1002		1			// npc 1002가 죽은 곳까지 트리거 저장
#define TRIGGER_SAVE_ALTER_OF_DARK_1003		2			// npc 1003가 죽은 곳까지 트리거 저장
#define TRIGGER_SAVE_ALTER_OF_DARK_1018		3			// npc 1018가 죽은 곳까지 트리거 저장, 암흑의 제단 클리어

////////// TriggerSaveMode ////////////
#define TRIGGER_MODE_NONE					0			// DB 작업은 스킵
#define TRIGGER_MODE_SAVE					1			// DB 데이터 저장하기
#define TRIGGER_MODE_DELLETE				2			// DB 데이터 지우기

////////// Trigger_state_flag ////////////
#define TRIGGER_PROTO_FLAG_CONTINUE			(1 << 0)	// 지속 가능
#define TRIGGER_PROTO_FLAG_DOMINO			(1 << 1)	// 도미노 형식 트리거 발동
#define TRIGGER_PROTO_FLAG_PLAY_ONE_PC		(1 << 2)	// 일반 필드에서 한 명에게만 보여야 한다

#define RAID_JOIN_LEVEL_LIMIT_CAPPELLA					140			// 예배당 입장 레벨 제한
#define RAID_JOIN_LEVEL_LIMIT_ALTER_OF_DARK				160			// 암제 입장 레벨 제한
#define RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE			90			// 아칸사원 입장 레벨 제한
#define RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE			110			// 아칸사원 입장 레벨 제한
#define RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE_NORMAL	60			// 아칸사원 입장 레벨 제한 노멀 난이도
#define RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE_NORMAL	80			// 아칸사원 입장 레벨 제한 노멀 난이도
#define RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE_HELL		120			// 아칸사원 입장 레벨 제한 HELL 난이도
#define RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE_HELL		140			// 아칸사원 입장 레벨 제한 HELL 난이도
#define RAID_JOIN_LEVEL_LOW_LIMIT_TARIAN_DUNGEON		175			// 타리안 던전 레벨 제한
#define RAID_JOIN_LEVEL_HIGH_LIMIT_TARIAN_DUNGEON		185			// 타리안 던전 레벨 제한

#define TITLE_SYSTEM_NO_TITLE				0			// 호칭이 없을 때는 0
#define TITLE_SYSTEM_OPTION_NUM				0			// CTitle의 Option에 관한 배열 index
#define TITLE_SYSTEM_OPTION_LEVEL			1			// CTitle의 option에 관한 배열 index

// 퀘스트 트리거
#define QUESTITEM_MAX_ARRAYCOUNT			10		// 퀘스트 트리거 아이템 맴버 변수 중 배열인 것의 최대 열 개수

#define MAX_NORMAL_SOCKET_COUNT	6
#define MAX_CHAOS_SOCKET_COUNT	1
#define MAX_SOCKET_COUNT		(MAX_NORMAL_SOCKET_COUNT + MAX_CHAOS_SOCKET_COUNT)
#define SOCKET_DB_LENGTH	50

#define GUILD_ACTIVE_SKILL_START	-1
#define GUILD_PASSIVE_SKILL_START	-2
#define GUILD_ETC_SKILL_START		-3
#define SKILL_TYPE_ACTIVE			0
#define SKILL_TYPE_PASSIVE			1
#define SKILL_TYPE_ETC				2

// TODO
#define TOUCH_FIELD					0
#define KILL_NPC					1
#define TAKE_ITEM					2
#define ALL_IN_AREA					3
#define NO_IN_AREA					4
#define MOVE_IN_AREA				5
#define MOVE_OUT_AREA				6
#define COUNT_DOWN					7
#define START_RAID					8
#define ITEM_USE					9
#define TODO_MAX_NUM				10

// OUTPUT
#define TOUCH_FIELD					0
#define SUMMON_EXTRA				1
#define SUMMON_DISTRICT				2
#define SUMMON_MBOSS				3
#define SUMMON_BOSS					4
#define GROUND_EFFECT_ON			5
#define GROUND_EFFECT_OFF			6
#define TOGGLE_GATE					7
#define TOGGLE_PORTAL				8
#define CREATE_ITEM_OBJECT			9
#define DEBUFF_SKILL				10
#define END_RAID					11
#define PAUSE_AREA					12
#define BUFF_SKILL					13
#define TOGGLE_CAMERA				14
#define COUNT_DOWN_START			15
#define SAVE_POINT					16
#define MODEL_HOLDER_PADOX			17
#define	NPC_SAY						18
#define APPLY_SKILL					19
#define	CURE_SKILL					20
#define OUTPUT_MAX_NUM				21

#define OBJECT_DATA_FLAG_AUTO_ATTACK (1 << 0)

// OBJECT_TYPE
#define OBJECT_TYPE_TODO			0
#define OBJECT_TYPE_OUTPUT			1
#define OBJECT_TYPE_OBJECT_STATE	2
#define OBJECT_TYPE_OBJECT_REFRESH	3

#define RAID_DATA_STATE_READY		0
#define RAID_DATA_STATE_COMPLETE	1

// OBJECT_LIST
#define OBJECT_TOUCH_FIELD			0
#define OBJECT_GATE					1
#define OBJECT_PORTAL				2
#define OBJECT_ITEM					3
#define OBJECT_PADOX_MODEL			4
#define OBJECT_CAMERA				5

#define DUNGEON_DIFFICULTY_NORMAL	0
#define DUNGEON_DIFFICULTY_HARD		1
#define DUNGEON_DIFFICULTY_HELL		2

#define GUILD_MAX_GRADE_EX		29

#define RAID_RESET_TIME_HOUR		3	// 리셋 시작 hour
#define RAID_RESET_TIME_MIN			0	// 리셋 시작 min
// 각 로컬별로 매일매일 초기화 할 것인지, 일주일에 한번할 것인지 정할 수 있다.
#define RAID_RESET_TIME_DAY			7	// allday 매일매일 초기화. 이부분은 각 로컬 별로 세팅해주어도 된다.
//#define RAID_RESET_TIME_DAY			0	// sun만 초기화
//#define RAID_RESET_TIME_DAY			1	// mon만 초기화
//#define RAID_RESET_TIME_DAY			2	// tue만 초기화
//#define RAID_RESET_TIME_DAY			3	// wed만 초기화
//#define RAID_RESET_TIME_DAY			4	// thu만 초기화
//#define RAID_RESET_TIME_DAY			5	// fri만 초기화
//#define RAID_RESET_TIME_DAY			6	// sat만 초기화

#define TREASUREBOX_DROP_LIMIT_RECT	6	// 보물상자 드랍 RECT 안에 max 개수
#define TREASUREBOX_DROP_LIMIT_ZONE	36	// 보물상자 드랍 zone 안에 max 개수
// zonedata flag
#define ZONE_FLAG_TREASUREBOX		(1<<0) // 보물상자 드랍

#define TREASURE_BOX_NPC_REGEN 1*60*60	// 보물상자 npc regen 시간. 1시간 뒤에는 npc가 사라진다.

#define TREASURE_BOX_NPC_INDEX 1255		// 보물상자 npc 인덱스
#define TREASURE_MAP_ITEM_INDEX 6941	// 보물지도 아이템 인덱스
#define TREASURE_KEY_ITEM_INDEX 6939	// 보물상자 일반 열쇠 아이템 인덱스

#if defined(LC_GAMIGO) || defined (LC_USA) || defined (LC_BILA) || defined (LC_RUS)
#define WAR_GROUND_CHANNEL			4
#else
#define WAR_GROUND_CHANNEL			2
#endif
#define	MAX_ROYAL_RUMBLE_PLAYER		40
#define	MIN_ROYAL_RUMBLE_PLAYER		10
#define	MIN_ROOKIE_LEVEL			50
#define	MAX_ROOKIE_LEVEL			89
#define MIN_SENIOR_LEVEL			90
#define MAX_SENIOR_LEVEL			120
#define	MIN_MASTER_LEVEL			121
#define	MAX_MASTER_LEVEL			MAX_LEVEL
#define LEVEL_TYPE_ROOKIE			0
#define	LEVEL_TYPE_SENIOR			1
#define LEVEL_TYPE_MASTER			2
#define LEVEL_TYPE_ALL				3
#define ROOKIE_REGIST_NEED_PRICE	1000000
#define	SENIOR_REGIST_NEED_PRICE	10000000
#define	MASTER_REGIST_NEED_PRICE	100000000
#define ROOKIE_REGIST_ITEM			7340
#define SENIOR_REGIST_ITEM			7341
#define MASTER_REGIST_ITEM			7342
#define PLAYER_NOT_SORT				0 // 미분류
#define PLAYER_SELECTED				1 // 선택됨
#define PLAYER_DROP					2 // 탈락
#define PLAYER_DRAW					3 // 경기 드로우
#define PLAYER_GREATER_THEN_MAX		0 // 신청자가 maximum보다 크다
#define PLAYER_SMALLER_THEN_MIN		1 // 신청자가 minimum보다 작다
#define PLAYER_BETWEEN_MIN_AND_MAX	2 // 신청자가 min~max이다.
#define DEBUFF_TYPE_20MINUTE		0
#define DEBUFF_TYPE_8PLAYER			1
#define DEBUFF_TYPE_PEACEZONE		2
#define ROYAL_RUMBLE_DEBUFF_SKILL	1355 // 수정필요 1은 아님
#define WARGROUND_MAX_POINT			1000000
#define WARGROUND_MAX_ACC_POINT		10000000
#define ROYAL_RUMBLE_SCHEDULE		5	// 하루 다섯경기
#if defined (LC_TLD)
#define ROYAL_RUMBLE_START_HOUR		21
#elif defined (LC_USA)
#define ROYAL_RUMBLE_START_HOUR		16
#else
#define ROYAL_RUMBLE_START_HOUR		22
#endif

#define LEVEL_COUNT		2

#ifdef DEV_GUILD_MARK
#define GUILD_MARK_SKILL_INDEX	1378
#endif

#ifdef DEV_GUILD_STASH
#define GUILD_STASH_KEEP_LOG	0
#define GUILD_STASH_TAKE_LOG	1
#endif // DEV_GUILD_STASH
#define MAX_TITLE_COUNT	200

#define RANKING_REFRESH_DAY 1	// 랭킹 갱신 요일 (0:일요일 ~ 6:토요일)
#define RANKING_REFRESH_HOUR 6	// 랭킹 갱신 시간

typedef enum _tagRankingTypeEx
{
	RANKTYPE_LEVELMASTER	= 0,
	RANKTYPE_JOBRANKER		= 1,
} RANKING_TYPE_EX;

//////////////////////////////////////////////////////////////////////////
typedef enum _eventLogType
{
	EVENT_LOGTYPE_AFFINITY_LAKIN	= 0,
	EVENT_LOGTYPE_KILL_EREBUS		= 1,
	EVENT_LOGTYPE_KOKO_MYUN			= 2,
} EVENT_LOG_TYPE;

typedef enum _tagProgressType
{
	PROGRESS_TYPE_NONE = -1,
	PROGRESS_TYPE_CRAFT = 0,
	PROGRESS_TYPE_DECOMPOSE,
	PROGRESS_TYPE_PRODUCE
} PROGRESS_TIME_TYPE;

typedef enum _tagNpcProdceState
{
	NPC_PRODUCE_STATE_ALIVE,	// 채집 가능 상태
	NPC_PRODUCE_STATE_LOOT,		// 룻 가능 상태
	NPC_PRODUCE_STATE_DEAD,		// 죽은 상태
} NPC_PRODUCE_STATE;

typedef enum _tagLootType
{
	LOOT_TYPE_PC  = 1,	// CPcLootManager
	LOOT_TYPE_MOB,		// CMobLootManager
} LOOT_TYPE;

#define KICK_RESERVE_TIME			86400	// 1일
#define LAST_CONNECT_TIME_FOR_KICK	604800	// 7일
enum _eKickStatus
{
	GMKS_NORMAL		=	0,
	GMKS_RESERVE	=	1,
};

#if defined (GUILD_REMOTE_INVITE)
enum _eRemoteInviteType
{
	GRI_MASTER		=	0,		// 길마가 가입 권유
	GRI_JOINER		=	1,		// 유저가 가입 신청
};
#endif // GUILD_REMOTE_INVITE

typedef enum  _eventAutomationType
{
	A_EVENT_SEPTEMBER		= 5,	// 9월 가을 맞이 이벤트
	A_EVENT_RAIN_DROP		= 20,	// 빗방울 일벤트
	A_EVENT_COLLECT_BUG		= 22,	// 곤충 채집 이벤트
	A_EVENT_CHUSUK			= 25,	// 추석 이벤트
	A_EVENT_PANDORA_BOX		= 26,	// 판도라의 상자 (할로윈 2006 이벤트, 미국 상시 이벤트)
	A_EVENT_VALENTINE_DAY	= 29,	// 발렌타인 데이 이벤트
	A_EVENT_WHITE_DAY		= 31,	// 화이트 데이
	A_EVENT_SONGKRAN		= 32,	// 송크란 이벤트
	A_EVENT_EGGS_HUNT		= 33,	// 부활절 이벤트
	A_EVENT_BEAR_DOLL		= 35,	// 곰돌이 이벤트
	A_EVENT_INDEPENDENCE_DAY= 40,	// 독립기념일
	A_EVENT_HALLOWEEN		= 49,	// 할로윈
	A_EVENT_XMAS			= 50,	// 크리스마스 이벤트
	A_EVENT_GOLDEN_WEEK		= 62,	// 골든 위크 이벤트
	A_EVENT_MAGIC_CARD		= 74,	// 매직 카드 이벤트 // 2008 섬머 이벤트에 사용됨.
	A_EVENT_APRIL_FOOL		= 75,	// 4월 만우절
	A_EVENT_WORLDCUP_TOTO	= 80,
	A_EVENT_WORLDCUP_TOTO_STATUS = 81,
	A_EVENT_WORLDCUP_TOTO_GIFT = 82,
	A_EVENT_ARTIFACT_HUNTER	= 84,
} EVENT_AUTOMATION_TYPE;

typedef enum _indexDataType
{
	INDEX_DATA_EXP_RATIO = 0,
	INDEX_DATA_SP_RATIO,
	INDEX_DATA_MAX,
} INDEX_DATA_TYPE;

#define PC_MIN_RUNSPEED	2.0f		// 캐릭터 최저 스피드

#define SERVER_TRANS_ERROR_SUCCESS				(0 << 0)
#define	SERVER_TRANS_ERROR_EXIST_GUILD		(1 << 0)		//	길드가 있는 경우
#define	SERVER_TRANS_ERROR_EXIST_GUARDIAN	(1 << 1)		//	후견인이 있는 경우
#define	SERVER_TRANS_ERROR_30DAYS				(1 << 2)		//	사용한지 30일이 안된경우
#define	SERVER_TRANS_ERROR_BLOCK_SERVER		(1 << 3)		//	서버가 블록 상태일 경우
#define	SERVER_TRANS_ERROR_INVALID_SERVER	(1 << 4)
#define SERVER_TRANS_ERROR_DB_INSERT			(1 << 5)
#define SERVER_TRANS_ERROR_NOT_EXIST_ITEM	(1 << 6)		//  사용중 아이템 이동
#define SERVER_TRANS_ERROR_ROLLBACK_SUCCESS (1 << 7)		//  롤백 성공

#define GOLD_MAGNIFIER			1839	// 황금 돋보기 아이템 인덱스
#define MAX_VARIATION_COUNT		6

enum eITEM_GRADE
{
	eITEM_GRADE_NORMAL		= -1,	//일반 등급
	eITEM_GRADE_RARE_HERO,			//레어 영웅등급
	eITEM_GRADE_RARE_UNIQUE,		//에어 특별등급
	eITEM_GRADE_RARE_RARE,			//레어 귀한등급
	eITEM_GRADE_RARE_MAGIC,			//레어 고급등급
	eITEM_GRADE_RARE_BASIC,			//레어 기본등급
	eITEM_GRADE_RARE_NOTOPEN,		//레어 미감정 등급
	eITEM_GRADE_ORIGIN,				//전설 등급

	eITEM_GRADE_MAX
};

#define AFFINITY_INFOTAB_SUCCESS			(0 << 0)		//친화도 정보창 성공
#define AFFINITY_INFOTAB_ERROR_NOTEXIST		(1 << 0)		//연결된 친화도 없음
#define AFFINITY_INFOTAB_NOT_REWARD			(1 << 1)		//다음 보상이 없음

#define JEWEL_COMPOSITE_POSITION_WEAPON		(1 << 0)
#define JEWEL_COMPOSITE_POSITION_HELMET		(1 << 1)
#define JEWEL_COMPOSITE_POSITION_ARMOR		(1 << 2)
#define JEWEL_COMPOSITE_POSITION_PANTS		(1 << 3)
#define JEWEL_COMPOSITE_POSITION_GLOVES		(1 << 4)
#define JEWEL_COMPOSITE_POSITION_SHOES		(1 << 5)
#define JEWEL_COMPOSITE_POSITION_SHIELD		(1 << 6)
#define JEWEL_COMPOSITE_POSITION_BACKWING	(1 << 7)
#define JEWEL_COMPOSITE_POSITION_ACCESSORY	(1 << 8)
#define JEWEL_COMPOSITE_POSITION_ALL		511

#define JEWEL_MAX_LEVEL						15		//보석 최대 레벨
#define JEWEL_POCKET_MAX_LEVEL				14		//보석 주머니 최대 레벨
#ifdef CIRCLE_WINDOWS
typedef __int64 GoldType_t;
#else
typedef long long GoldType_t;
#endif

#define MAX_PET_STASH_KEEP_COUNT	20		//펫 창고 보관 최대 개수
#define PET_STASH_INCREASE_POINT	1		//펫 창고에서의 회복 수치(배고픔, 충성도)
#define MAX_EFFECT_COUNT			5		//펫 창고 아이템 사용시 나올 수 있는 이펙트 정보 개수

#define IMMUN_SKILL_MCT_TIME		120

#define NAS_ITEM_DB_INDEX			(19)

#define SYDICATE_QUEST_LIST_PERCENT 30		// 결사대 퀘스트 지급 확률
#define SYNDICATE_OUT_NEEDNAS		10000000

namespace SYNDICATE
{
enum eSYNDICATE_TYPE
{
	eSYNDICATE_NONE		= 0	,		// 무소속
	eSYNDICATE_KAILUX		,		// 카이룩스	: PC 및 NPC 참여 가능
	eSYNDICATE_DEALERMOON	,		// 딜라문	: PC 및 NPC 참여 가능
	eSYNDICATE_FLOR			,		// 플로르	: NPC로만 구성
	eSYNDICATE_MAX			,		// max value
};

enum eSYNDICATE_HISTORY_TYPE
{
	JOIN,			// 가입
	REMOVE,			// 탈퇴
	DEGRADE,		// 강등
	PROMOTION,		// 승급
	WIN,			// 승리
	LOSE,			// 지다
};

enum eSYNDICATE_GRADE
{
	SQUIRE		= 1	,		// 견습 기사 카이룩스
	KNIGHT			,		// 정식 기사
	GENTOR			,		// 상급 기사
	HONORISE		,		// 지휘관
	BARONE			,		// 남작
	VISCONTE		,		// 자작
	CONTE			,		// 백작
	MARQUISE		,		// 후작
	DUKA			,		// 공작
	PRINCIPE		,		// 제왕

	NEOPTYE		= 1	,		// 초심자 딜라문
	ZELATOR			,		// 견습자
	THEORICUS		,		// 숙련자
	PHILOSOPHUS		,		// 현명한자
	ADEPTUS			,		// 진의를 깨달은 자
	MAGUS			,		// 지혜를 획득한 자
	IPSISSIMUS		,		// 깨달음을 얻은 자
};

#define		SQUIRE_POINT		524999999
#define		KNIGHT_POINT		944999999
#define		GENTOR_POINT		1280999999
#define		HONORISE_POINT		1553999999
#define		BARONE_POINT		1763999999
#define		VISCONTE_POINT		1910999999
#define		CONTE_POINT			2015999999
#define		MARQUISE_POINT		2078999999

#define		NEOPTYE_POINT		734999999
#define		ZELATOR_POINT		1259999999
#define		THEORICUS_POINT		1679999999
#define		PHILOSOPHUS_POINT	1889999999
#define		ADEPTUS_POINT		2078999999

#ifdef  WIN32
inline int getSyndicateGrade(int syndicateType, __int64 point)
#else
inline int getSyndicateGrade(int syndicateType, long long point)
#endif
{
	switch(syndicateType)
	{
	case eSYNDICATE_KAILUX:
		{
			if (0 <= point && point <= SQUIRE_POINT)
				return SQUIRE;
			else if (SQUIRE_POINT < point && point <= KNIGHT_POINT)
				return KNIGHT;
			else if (KNIGHT_POINT < point && point <= GENTOR_POINT)
				return GENTOR;
			else if (GENTOR_POINT < point && point <= HONORISE_POINT)
				return HONORISE;
			else if (HONORISE_POINT < point && point <= BARONE_POINT)
				return BARONE;
			else if (BARONE_POINT < point && point <= VISCONTE_POINT)
				return VISCONTE;
			else if (VISCONTE_POINT < point && point <= CONTE_POINT)
				return CONTE;
			else if (CONTE_POINT < point && point <= MARQUISE_POINT)
				return MARQUISE;
			else
				return DUKA;
		}
		break;
	case eSYNDICATE_DEALERMOON:
		{
			if (0 <= point && point <= NEOPTYE_POINT)
				return NEOPTYE;
			else if (NEOPTYE_POINT < point && point <= ZELATOR_POINT)
				return ZELATOR;
			else if (ZELATOR_POINT < point && point <= THEORICUS_POINT)
				return THEORICUS;
			else if (THEORICUS_POINT < point && point <= PHILOSOPHUS_POINT)
				return PHILOSOPHUS;
			else if (PHILOSOPHUS_POINT < point && point <= ADEPTUS_POINT)
				return ADEPTUS;
			else
				return MAGUS;
		}
		break;
	default:
		return 0;
	}
}

#ifdef  WIN32
inline __int64	getSyndicateGradeMaxPoint(int syndicateType, int grade)
#else
inline long long getSyndicateGradeMaxPoint(int syndicateType, int grade)
#endif
{
	switch(syndicateType)
	{
	case eSYNDICATE_KAILUX:
		{
			if (grade == SQUIRE)
				return SQUIRE_POINT;
			else if (grade == KNIGHT)
				return KNIGHT_POINT;
			else if (grade == GENTOR)
				return GENTOR_POINT;
			else if (grade == HONORISE)
				return HONORISE_POINT;
			else if (grade == BARONE)
				return BARONE_POINT;
			else if (grade == VISCONTE)
				return VISCONTE_POINT;
			else if (grade == CONTE)
				return CONTE_POINT;
			else if (grade == MARQUISE)
				return MARQUISE_POINT;
			else
				return 0;
		}
		break;
	case eSYNDICATE_DEALERMOON:
		{
			if (grade == NEOPTYE)
				return NEOPTYE_POINT;
			else if (grade == ZELATOR)
				return ZELATOR_POINT;
			else if (grade == THEORICUS)
				return THEORICUS_POINT;
			else if (grade == PHILOSOPHUS)
				return PHILOSOPHUS_POINT;
			else if (grade == ADEPTUS)
				return ADEPTUS_POINT;
			else
				return 0;
		}
		break;
	default:
		return 0;
	}
}
}

#define PS_MAX_SHOPNAME				100		// 상점 이름 최대 길이 100 바이트

enum
{
	TOGGLE_SKILL = 0,
	TOGGLE_ITEM,
};

enum
{
	ITEM_CASTLE_WAR_FLAG_DONE,
	ITEM_CASTLE_WAR_FLAG_ALL,
	ITEM_CASTLE_WAR_FLAG_MERAC,
	ITEM_CASTLE_WAR_FLAG_DRATAN,
};

#define CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_OWNER	500000
#define CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_NORMAL	1000000	


enum
{
	MOVE_TO_GUILDROOM = 0,
};

#define ARTIFACT_FIND_GPS_ITEM_INDEX	10954

#endif // __GAME_Config_H__ 0
