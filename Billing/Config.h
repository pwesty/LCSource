/************************************
  수정사항 
  수정한 사람: bs
  수정일: 2004-11-19 오후 8:02:59
  설명: 스킬 시전 취소 효과, 스킬 조건 - 방패 착용 추가
 ************************************/

#ifndef __GAME_Config_H__
#define __GAME_Config_H__

// 국가별 설정
//#define LC_KOR		0	// 국내
//#define LC_TWN		1	// 대만 - 혁명
//#ifdef LC_TWN	
//#define LC_TWN2		2		// 대만 - 천하대란, LC_TWN과 동시에 설정해야 함
//#endif
//#define LC_CHN		3	// 중국
#define LC_TLD			4	// 태국
//#ifdef LC_TLD
//#define LC_TLD_ENG	5	// 태국 영어
//#endif
//#define	LC_JPN			6 // 일본
//#define	LC_MAL			7 // 말레이시아
//#ifdef LC_MAL
//#define	LC_MAL_ENG		8 // 말레이시아 영어 
//#endif
//#define	LC_USA			9	// 미국
//#define	LC_BRZ			10	// 브라질

// --- 국가별 설정

#define OLD_NETMSG						// TODO : 지울것
//#define TEST_SERVER						// 테스트 서버용
// #define CLOSED_BETA_TEST				// 클베 전용
// #define LC_BIG_ENDIAN				1 TODO : 빅 엔디안 장비에서만 사용
// #define PREOPEN_CHECK				// 프리오픈 플래그
#define FORCE_START_ZONE				// 강제로 시작시 시작 존으로
// #define NOKICK_HACKUSER				// TODO : 핵 유저 접속 종료 처리하려면 지우기
// #define NOCHECK_ATTACKSPEED			// TODO : 공격속도 검사 여부 결정
// #define CLEAR_ASSIST					// TODO : 보조 효과 저장 안함
// #define BLOCK_SUMMON_OTHERZONE_MOB	// TODO : 다른 존의 몹 소환을 막으려면 주석 풀기
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

#define LOGIN_SERVER_OPEN_CHECK_FILENAME	"login.open"		// 로그인 서버 오픈 검사 파일명

#define		GAMEID				1
#define		CPID				1
#define		ID_LENGTH			32
#define		MAX_PURCHASEITEM	10
#define		MAX_DEPOSITORY		25

#define	PLAYER_COUNT_NUM		9999	// 사용자수 조사 프로그램 번호

#define NUM_RESERVED_DESCS		5		// 리눅스에서 예약된 디스크립터 수
#define MAX_PLAYING				50000	// 최대 동접 수
#define MAX_ZONES				20		// 최대 존 수
#define MAX_NOTICE				5

#define MAX_PACKET_HEAD		((int) (sizeof(short)) + (sizeof(int)) + (sizeof(short) ) )
#define MAX_PACKET_DATA		(MAX_PACKET_SIZE - MAX_PACKET_HEAD)
#define MAX_PACKET_SIZE 1000

// 버퍼관련 길이 상수
#define MAX_MESSAGE_TYPE		((int)(sizeof(unsigned char)))
#define MAX_MESSAGE_SIZE		1000
#define MAX_MESSAGE_DATA		(MAX_MESSAGE_SIZE - MAX_MESSAGE_TYPE)

#ifdef OLD_NETMSG
// Reliable(2) + Sequence(4) + ID(2) + TxSize(4)
#define MAX_HEADER_SIZE ((long)(sizeof(unsigned short) + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(unsigned long)))
#else
#define MAX_HEADER_SIZE			((int)(sizeof(int)))
#endif
#define MAX_INPUTBUFFER_SIZE		((MAX_HEADER_SIZE + MAX_MESSAGE_SIZE) * 30)
#define MAX_OUTPUTBUFFER_SIZE		((MAX_MESSAGE_SIZE + MAX_HEADER_SIZE) * 300)
#define MAX_OUTPUTBUFFER_OUTUNIT	4096


#define PI       3.14159265358979323846f
#define PI_2     6.28318530717958623200f
#define PI_3_4	 2.35619449019234492884f	/* ((3 / 4) * PI) */
#define PI_HALF	 (PI / 2)

// 메신저 처리 용량 : 한 틱당 몇개의 입력을 받고 몇개의 메시지를 처리할지 결정
#define PROC_MESSENGER_INPUT_PER_PASS	50	// 틱당 메신저 입력 처리 수
#define PROC_MESSENGER_MSG_PER_PASS		50	// 틱당 메신저 메시지 처리 수
#define PROC_HELPER_INPUT_PER_PASS		50	// 틱당 메신저 입력 처리 수

// 컨넥터 처리 용량 : 한 틱당 몇개의 입력을 받고 몇개의 메시지를 처리할지 결정
#define PROC_CONNECTOR_INPUT_PER_PASS	50	// 틱당 컨넥터 입력 처리 수
#define PROC_CONNECTOR_MSG_PER_PASS		50	// 틱당 컨넥터 메시지 처리 수

// 시간 관련
#ifdef __GAME_SERVER__
	#define OPT_USEC		100000				/* 10 passes per second */
#else
	#define OPT_USEC		50000				/* 20 passes per second */
#endif
#define PASSES_PER_SEC	(1000000 / OPT_USEC)
#define RL_SEC			* PASSES_PER_SEC

// 실제 시간과의 관계
#define PULSE_REAL_SEC				(1 RL_SEC)				// 실제 1초
#define PULSE_REAL_MIN				(60 RL_SEC)				// 실제 1분
#define PULSE_REAL_TENMIN			(600 RL_SEC)			// 실제 10분
#define PULSE_REAL_HALF				(1800 RL_SEC)			// 실제 30분
#define	PULSE_REAL_HOUR				(3600 RL_SEC)			// 실제 1시간
#define PULSE_CONNECT_TIMEOUT		(3 * PULSE_REAL_MIN)	// 입력 대기 시간, 지정 시간 동안 입력이 없으면 접속을 종료
#define PULSE_SAVE_PC				(10 * PULSE_REAL_MIN)	// 자동 저장 시간
#define PULSE_PCROOM_COUNT			(3 * PULSE_REAL_MIN)	// 피씨방 접속자 수 카운트 주기
#define PULSE_TOTAL_COUNT			PULSE_REAL_MIN			// 전체 접속 수 카운트 주기
#define PULSE_NPC_AI				2						// NPC 최소 AI 지연 시간 : 0.5초
#define	PULSE_NPC_REGEN				(15 RL_SEC)				// NPC 리젠 최소 지연 시간 : 실제 30초
// TODO : DELETE #define PULSE_SAVECOUNT				(5 * PULSE_REAL_MIN)	// 컨넥터의 동접 플레이어 수 저장 시간
// TODO : DELETE #define PULSE_SAVEMAXCOUNT			(2 * PULSE_REAL_MIN)	// 컨넥터의 최대 동접 플레이어 수 저장 시간
#define PULSE_WARPDELAY				(10 * PULSE_REAL_SEC)	// 순간이동 딜레이 : 10초
#define PULSE_BILLITEMREQ			(20 * PULSE_REAL_SEC)	// 빌링아이템 지급 제한 시간 20초  이시간이 지나면 셋을 푼다.
#define PULSE_BILLITEMLISTREQ		(4 * PULSE_REAL_SEC)	// 빌링아이템 리스트 요청 제한 시간 이시간이 지나면 셋을 푼다.
#define PULSE_WARPCOUNTDOWN			(5 * PULSE_REAL_SEC)	// 순간이동 카운트 다운 시작 : 5초 전부터
#define PULSE_ASSIST_CHECK			PULSE_REAL_SEC			// 보조효과 시간 감소 검사 딜레이
#define PULSE_HACK_CHECK_TIMEOUT	(6 * PULSE_REAL_MIN)	// 펄스 메시지 타임아웃 6분
#define PULSE_HACK_PULSE_REFRESH	PULSE_REAL_HOUR			// 핵 검사 펄스 갱신 주기 1시간
#define PULSE_HACK_PULSE_THRESHOLD	(15 * PULSE_REAL_SEC)	// 핵 펄스 오차 범위 15초
#define PULSE_HACK_CLIENT_THRESHOLD	(55 * PULSE_REAL_SEC)	// 핵 펄스 클라이언트 최소 주기 55초 이보다 자주 보내면 뷁!
#define PULSE_PKMODE_DELAY			(10 RL_SEC)				// PK모드에서 평화모드로 전환시 딜레이
#define PULSE_HACK_ATTACK_THRESHOLD	2						// 공속 오차 허용 범위
#define	PULSE_HACK_ATTACK_COUNT		5						// 어택 핵카운트 모으는 숫자
#define PULSE_ATTACKMODE_DELAY		(3 * PULSE_REAL_SEC)	// 공격 모드 풀리는 딜레이 : 마지막 공격이후 이 시간동안은 공격 중이다
#define PULSE_POST_REGEN_DELAY		(3 * PULSE_REAL_SEC)	// 리젠이후 선공하기 전까지 딜레이
#define PULSE_PRODUCE_DELAY			(2 * PULSE_REAL_SEC)	// 생산 딜레이
#define PULSE_PET_SYMPATHY_INCREASE	(5 * PULSE_REAL_MIN)	// 펫 교감도 증가 딜레이
#define PULSE_PET_SYMPATHY_DECREASE	(PULSE_REAL_HOUR)		// 펫 교감도 감소 딜레이
#define PULSE_PET_HUNGRY_DECREASE	(5 * PULSE_REAL_MIN)	// 펫 배고픔 딜레이
#define PULSE_AUTO_SKILL_DECREASE	(10 * PULSE_REAL_SEC)	// 자동 스킬 시전 시간
#define PULSE_KEY_CHANGE_THRESHOLD	(10 * PULSE_REAL_SEC)	// 키 변경 임계 시간 : 10 초
#define PULSE_KEY_CHANGE			(20 * PULSE_REAL_SEC)	// 키 변경 주기 : 20 초
#define	PULSE_SERVER_ALIVE			(2 * PULSE_REAL_MIN)	// 빌링 서버 얼라이브 메세지 타임 
#define	PULSE_PCBANG_ITEM			(PULSE_REAL_HOUR)	// 등록된 PC방에서 아이템 지급 주기
#define PULSE_SAVECOUNT_NEW				5						// 컨넥터의 동접 플레이어 수 저장 시간
#define PULSE_SAVEMAXCOUNT_NEW			2						// 컨넥터의 최대 동접 플레이어 수 저장 시간


// t_connect_XXX 저장 주기
#define PERIOD_SAVE_COUNT			5		// 존별 사용자수 5분간격 저장
#define PERIOD_SAVE_MAX				2		// 최대 동접수 2분간격 저장

// Variables for the output buffering system
#define MAX_SOCK_BUF            (12 * 1024) // Size of kernel's sock buf

#define MAX_ID_NAME_LENGTH			32		// 아이디, 이름 길이
#define MAX_PWD_LENGTH				32		// 암호 길이
#define HOST_LENGTH					30		// 호스트 주소 문자열 길이
#define MAX_CHAR_NAME_LENGTH		50		// 케릭터 이름 길이
#define MAX_GUILD_NAME_LENGTH		50		// 길드 이름 길이	
#define MAX_ITEM_NAME_LENGTH		50		// 아이템 이름길이 
#define MAX_STASH_PASSWORD_LENGTH	8		// 창고 암호 길이
#define FRIEND_MAX_MEMBER			50		// 친구 멤버 수
#define	FRIEND_GROUP_MAX_NUMBER		10		// 그룹 수
#define MAX_GROUP_NAME_LENGTH		20		// 그룹 이름 길이 
#define BLOCK_MAX_MEMBER			20		// 최대 블럭 인원수 
#define	MAX_GIFT_MESSAGE_LENGTH		100		// 선물 메세지 길이 100바이트



// 전역으로 사용하는 문자배열, CServer의 Run에서만 사용!!!
#define MAX_STRING_LENGTH		8192

#define DFLT_IP		NULL;				// 서버 아이피 설정

// 접속 상태 상수
#define CON_PLAYING						0		// 게임 중
#define CON_CLOSE						1		// 연결 끊김
#define CON_MOVESERVER					2		// 서버 이동중
#define CON_MOVESERVER_WAIT				3		// 서버 이동중
#define CON_MENU						10		// 메뉴에서의 동작
#define CON_DISCONNECT					17		// 게임중 연결 끊김
#define CON_WAIT_USER_DB				19		// 캐릭터 데이터 기다림
#define CON_WAIT_NEW_CHAR_DB			20		// 캐릭터 생성 기다림
#define CON_WAIT_DEL_CHAR_DB			21		// 캐릭터 삭제 기다림
#define CON_WAIT_PLAY_CHAR_DB			22		// 캐릭터 상세 정보 기다림
#define CON_WAIT_DEL_CANCEL_CHAR_DB		26		// 캐릭터 삭제 취소
#define CON_GET_LOGIN					23		// 로그인 시도
#define CON_PREPARE_PLAY				25		// 게임 시작 대기중
#define CON_WAIT_IDEN_MSG				30		// 인증 기다림
#define CON_GET_IDEN_MSG				31		// 인증 요청

#define STATE(d)			((d)->m_connected)
#define WAITTING_DB(d)		((d)->m_bwaitdb)
#define UPDATING_DB(d)		((d)->m_updatedb)
#define PLAYMODE(d)			((d)->m_playmode)

// 신규 생성 케릭터, 선물도착 여부, 태국일경우 성인
//커넥터 class CUser의 m_userFlag
#define NOVICE		( 1 << 0 )
#define	RECV_GIFT	( 1 << 1 )
#define ADULT		( 1 << 2 )
#define IS_CHARACTER	( 1 << 3 )

/////////////////////////////////////////////////////////////////////////////
// 국가별 설정
#if defined (LC_KOR)
#include "Config_KOR.inh"
#elif defined (LC_TWN)
#include "Config_TWN.inh"
#elif defined (LC_CHN)
#include "Config_CHN.inh"
#elif defined (LC_TLD)
#include "Config_TLD.inh"
#elif defined (LC_JPN)
#include "Config_JPN.inh"
#elif defined (LC_MAL)
#include "Config_MAL.inh"
#elif defined (LC_USA)
#include "Config_USA.inh"
#elif defined (LC_BRZ)
#include "Config_BRZ.inh"
#else
#include "Config_KOR.inh"
#endif
// ---- 국가별 설정
/////////////////////////////////////////////////////////////////////////////

// LC_TIME
#define	LCHOUR  150							// 현실시간 2분 30초
#define	LCDAY	(LCHOUR * 24)					// 24시간 1일
#define	LCMON	(LCDAY * 30)					// 한달 30일
#define	LCYEAR	(LCMON * 12)					// 1년 12달

#ifdef LC_TIME
#define OUTPUTDAY LCDAY
#else
#define OUTPUTDAY (60 * 60 * 24 )
#endif

// 스킬포인트
#define MAX_SKILLPOINT		2000000000


// 직업별 상수
#define JOBCOUNT			6
#define JOB_TITAN			0
#define JOB_KNIGHT			1
#define JOB_HEALER			2
#define JOB_MAGE			3
#define JOB_ROGUE			4
#define JOB_SORCERER		5
#define JOB_PET				6


#define JOB2COUNT			12
#define JOB_2ND_HIGHLANDER		1		// 하이랜더
#define	JOB_2ND_WARMASTER		2		// 워마스터
#define JOB_2ND_ROYALKNIGHT		1		// 로열나이트
#define	JOB_2ND_TEMPLEKNIGHT	2		// 템플나이트
#define JOB_2ND_ARCHER			1		// 아처
#define	JOB_2ND_CLERIC			2		// 클레릭
#define JOB_2ND_WIZARD			1		// 위자드
#define	JOB_2ND_WITCH			2		// 위치
#define JOB_2ND_ASSASSIN		1		// 머쎄씬
#define	JOB_2ND_RANGER			2		// 레인저
#define JOB_2ND_ELEMENTALLIST	1		// 엘리멘탈 리스트
#define JOB_2ND_SPECIALLIST		2		// 스페셜 리스트

#define JOB_2ND_PET_HORSE			0	// 말
#define JOB_2ND_PET_DRAGON			1	// 용
#define JOB_2ND_PET_HORSE_MOUNT		2	// 말 : 탈것
#define JOB_2ND_PET_DRAGON_MOUNT	3	// 용 : 탈것


#define JOB_2ND_LEVEL			31		// 전직 레벨


// 얼굴 모양 개수
#define HAIR_TITAN			3
#define HAIR_KNIGHT			3
#define HAIR_HEALER			3
#define HAIR_MAGE			3
#define HAIR_ROGUE			3
#define HAIR_SORCERER		3


// 머리 모양 개수
#define FACE_TITAN			3
#define FACE_KNIGHT			3
#define FACE_HEALER			3
#define FACE_MAGE			3
#define FACE_ROGUE			3
#define FACE_SORCERER		3


// 데이터베이스 동작 상태
#define DB_NOP					0		// DB No Operation...
#define DB_UPDATE				1		// Request DB Update...
#define DB_WAIT_SAVE			10		// Save Delay...
#define DB_END					11		// 빌링 없을때 로그아웃 완료 



// 게임 로그인 종류
#define	MAX_CHAR_COUNT			6		// 최대 생성 가능 캐릭터 수
#define MAX_SKILL				60		// 캐릭터 당 최대 스킬 개수
#define MAX_ASSIST_HELP			12		// 보조 효과 : 이로운 것
#define MAX_ASSIST_CURSE		8		// 보조 효과 : 해로운 것
#define MAX_ASSIST_ABS			10		// 절대 시간 보조효과
#define MAX_EXT_CHAR_COUNT		2		// 유료아이템으로 추가 할수 있는 슬롯수


///////
// 기타
#define NO_CELL				(-100)		// 셀에 포함 안된 상태
#define EPSILON				(1.0e-5f)	// Tolerance for FLOATs
#define CELL_SIZE			12.0f		// 한 셀의 attr 개수 (m 단위)
#define CELL_EXT			4			// 영향을 미치는 셀 영역
#define MULTIPLE_HEIGHTMAP	100.0f		// 높이맵 곱해지는 수치
#define MAX_PLAYER_LIST		8192		// 최대 동시 접속자 수

#ifdef FEEITEM
#define MAX_MEMPOS					15			// 장소 기억 수
#define MAX_MEMPOS_NORMAL			5			// 장소 기억 수
#else
#define MAX_MEMPOS					5			// 장소 기억 수
#define MAX_MEMPOS_NORMAL			5			// 장소 기억 수
#endif



///////////////////
// NPC 관련

#define		MAX_NPC_SKILL			2
#define		MAX_NPC_DROPITEM		20
#define		MAX_NPC_PRODUCT			5
#define		MAX_NPC_DROPITEM_LOOP	4

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

// NPC AI TYPE
#define		NPC_AI_NORMAL			0			// 일반 NPC
#define		NPC_AI_TANKER			1			// 일반 NPC
#define		NPC_AI_DAMAGEDEALER		2			// 일반 NPC
#define		NPC_AI_HEALER			3			// 일반 NPC


#define IS_NPC(ch)			(ch->m_type == MSG_CHAR_NPC)	// NPC 인가요?
#define TO_NPC(ch)			((ch == NULL) ? NULL : (IS_NPC(ch) ? (CNPC*)ch : (CNPC*)NULL))		// NPC로 캐스팅
#define IS_PC(ch)			(ch->m_type == MSG_CHAR_PC)		// PC 인가요?
#define TO_PC(ch)			((ch == NULL) ? NULL : (IS_PC(ch) ? (CPC*)ch : (CPC*)NULL))			// PC로 캐스팅
#ifdef ENABLE_PET
#define IS_PET(ch)			(ch->m_type == MSG_CHAR_PET)	// 애완동물인가?
#define TO_PET(ch)			((ch == NULL) ? NULL : (IS_PET(ch) ? (CPet*)ch : (CPet*)NULL))		// Pet로 캐스팅
#else
#define IS_PET(ch)			(false)
#define TO_PET(ch)			(NULL)
#endif
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
#define MAPATT_FIELD				0		// 이동가능
#define MAPATT_PEACEZONE			10		// 피스존
#define MAPATT_PRODUCT_PUBLIC		20		// 공유 생산 시설
#define MAPATT_PRODUCT_PRIVATE		30		// 사유 생산 시설
#define MAPATT_STAIR_UP				40		// 위로 가는 계단
#define MAPATT_STAIR_DOWN			50		// 아래로 가는 계단
#define MAPATT_WARZONE				60		// 공성 지역
/////////////////////////////////////////////
// BANGWALL : 2005-07-18 오전 11:24:48
// Comment : free pkzone
#define MAPATT_FREEPKZONE			70		// 프리 피케이죤
#define MAPATT_BLOCK				255		// 이동 불가


///////////////////
// 아이템 관련 상수

// 050311 : bs : 무기가 사용하는 스탯에 대한 상수 추가
#define USING_STR		0
#define USING_DEX		1
// --- 050311 : bs : 무기가 사용하는 스탯에 대한 상수 추가
// 상품 타입
#define		CTYPE_CONSUME		0	// 소모성 상품
#define		CTYPE_TERM			1	// 기간 상품
#define		CTYPE_POTION		2	// 포션
#define		CTYPE_AVATA			3	// 아바타
#define		CTYPE_WEAPON		4	// 무기
#define		CTYPE_WEAR			5	// 무기
#define		CTYPE_SET			6	// 세트 상품
#define		CTYPE_PLATINUM			7	// 플래티늄, 한정판매 상품
// 상품 플래그 
#define		CATALOG_FLAG_NEW	 (1 << 0)	 // 신상품
#define		CATALOG_FLAG_POP	 (1 << 1)	 // 인기상품
#define		CATALOG_FLAG_RESV1	 (1 << 3)	 // 예약 플래그 
#define		CATALOG_FLAG_RESV2	 (1 << 4)	 // 예약 플래그 
#define		CATALOG_FLAG_RESV3	 (1 << 5)	 // 예약 플래그 
#define		CATALOG_FLAG_RESV4	 (1 << 7)	 // 예약 플래그 
#define		CATALOG_FLAG_RESV5	 (1 << 8)	 // 예약 플래그 
#define		CATALOG_FLAG_RESV6	 (1 << 9)	 // 예약 플래그 
#define		CATALOG_FALG_TO_LIMIT(a, b) (b = 0 | (a >> 10)) 
#define		CATALOG_FALG_FROM_LIMIT(a, b) ( a = a | ( b << 10 ) )

#define		CATALOG_FLAG_MAX_LIMIT (2000000)

// Item Flag : 최상위 1비트는 사용 불가!!!
// 6비트까지 케르, 넨, 일반 제련석의 레벨과, 플레티늄 제련석을 바른 아이템의 레벨로 사용
#define		PLATINUM_MAX_PLUS				127 
#define		FLAG_ITEM_PLATINUM_GET(a, b)	(b = a & PLATINUM_MAX_PLUS)
#define		FLAG_ITEM_PLATINUM_SET(a, b)	(a = ( (a &~ PLATINUM_MAX_PLUS) | b ))
#define		FLAG_ITEM_OPTION_ENABLE			(1 << 7)	// 아이템 플래그에 옵션을 붙일 수 있는 상태를 구분, Bit 연산
#define		FLAG_ITEM_SEALED				(1 << 8)	// 아이템 봉인
#define		FLAG_ITEM_SUPER_STONE_USED		(1 << 9)	// 슈퍼고제로 업그레이드 했음..//0627
#define		FLAG_ITEM_BOOSTER_ADDED			(1 << 10)	// 부스터 장착 여부
#define		FLAG_ITEM_SILVERBOOSTER_ADDED	(1 << 11)	// 실버부스터 장착 여부
#define		FLAG_ITEM_GOLDBOOSTER_ADDED		(1 << 12)	// 골드부스터 장착 여부
#define		FLAG_ITEM_PLATINUMBOOSTER_ADDED	(1 << 13)	// 골드부스터 장착 여부
#define		FLAG_ITEM_COMPOSITION			(1 << 14)	// 아이템 합성 여부
#define		FLAG_ITEM_LENT					(1 << 15)	// 대여된 아이템

#define		FLAG_ITEM_RESERVED				(1 << 31)	// 예약된 것 사용하면 절대 안됨!

// Item Proto flag
#define		ITEM_FLAG_COUNT		(1 << 0)	// 셀 수 있는 아이템인가
#define		ITEM_FLAG_DROP		(1 << 1)	// 드롭  가능 아이템인가가
#define		ITEM_FLAG_UPGRADE	(1 << 2)	// 업그레이드 가능한 아이템인가
#define		ITEM_FLAG_EXCHANGE	(1 << 3)	// 교환 가능 아이템인가가
#define		ITEM_FLAG_TRADE		(1 << 4)	// 매매 가능 아이템인가가
#define		ITEM_FLAG_BORKEN	(1 << 5)	// 파괴 가능 아이템인가가
#define		ITEM_FLAG_MADE		(1 << 6)	// 생산 가능 아이템인가가
#define		ITEM_FLAG_MIX		(1 << 7)	// 조합 아이템 : DB에 저장안됨
#define		ITEM_FLAG_CASH		(1 << 8)	// 유료 아이템
#define		ITEM_FLAG_LORD		(1 << 9)	// 성주 전용 아이템
#define		ITEM_FLAG_NO_STASH	(1 << 10)	// 창고 보관 불가능 아이템
#define		ITEM_FLAG_CHANGE	(1 << 11)	// 다른 직업용 아이템으로 교환 가능 여부
#define		ITEM_FLAG_COMPOSITE	(1 << 12)	// 아이템 합성 가능 여부
#define		ITEM_FLAG_CASHMOON	(1 << 13)	// 캐쉬 문스톤 룰렛 가능 여부 
#define		ITEM_FLAG_LENT		(1 << 14)	// 대여용 무기
#define		ITEM_FLAG_RARE		(1 << 15)	// 레어 아이템

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

// SUBTYPE : 방어구 --
#define		IWEAR_HELMET			0		// 머리
#define		IWEAR_ARMOR				1		// 상의
#define		IWEAR_PANTS				2		// 하의
#define		IWEAR_GLOVE				3		// 장갑
#define		IWEAR_SHOES				4		// 신발
#define		IWEAR_SHIELD			5		// 방패

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

// IONCE_WARP : 종류
#define		IONCE_WARP_RETURN		0		// 특정 존 귀환 장치
#define		IONCE_WARP_MEMPOS		1		// 기억된 장소로 이동
#define		IONCE_WARP_MARGADUM_PVP	2		// 메라크 pvp 이동
#define		IONCE_WARP_PARTYRECALL	3		// 060227 : bs : 파티리콜

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
#define		IETC_UPGRADE_GENERAL	0	// 일반제련석
#define		IETC_UPGRADE_SPECIAL	1	// 고급제련석
#define		IETC_UPGRADE_SPECIAL_SUPER	2	// 슈퍼고급제련석//0627
#define		IETC_UPGRADE_BOOSTER	3	// 부스터
#define		IETC_UPGRADE_LUCKY		4	// 행운의 고급 제련석
#define		IETC_UPGRADE_PLATINUM	5	// 플래티늄 제련석 


// SUBTYPE : 악세사리
#define		IACCESSORY_CHARM		0	// 부적
#define		IACCESSORY_MAGICSTONE	1	// 마련석
#define		IACCESSORY_LIGHTSTONE	2	// 반짝이는돌
#define		IACCESSORY_EARING		3	// 귀걸이
#define		IACCESSORY_RING			4	// 반지
#define		IACCESSORY_NECKLACE		5	// 목걸이
#define		IACCESSORY_PET			6	// 목걸이

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

// 아이템 업그레이드 관련
#define	ITEM_UPGRADE_RESULT_PLUS		0		// +1
#define	ITEM_UPGRADE_RESULT_MINUS		1		// -1
#define	ITEM_UPGRADE_RESULT_NOCHANGE	2		// 0
#define	ITEM_UPGRADE_RESULT_BROKEN		3		// 깨짐
#define	ITEM_UPGRADE_PLATINUM_RESULT_PLUS 4		// 플래티늄 플러스를 발랐음

#define ITEM_UPGRADE_PROB_PLUS_VALUE		12			// 12위의 기준 되는 수치
#define ITEM_UPGRADE_PROB_PLUS_PENALTY_LEV	200			// 업그레이드 수치당 떨어지는 확률
#define ITEM_UPGRADE_PROB_PLUS_PENALTY_MAX	4000		// 업그레이드 최대 떨어지는 확률 (40%) -> 최소 성공 확률 보장을 위한 것
#define	ITEM_UPGRADE_PROB_GENERAL_PLUS		5000		// 일반 제련석 확률 50%
#define ITEM_UPGRADE_PROB_SPECIAL_NOCHANGE	2000		// 고급 제련석 유지 확률 20% (1 ~ 20)
#define ITEM_UPGRADE_PROB_SPECIAL_BROKEN	2700		// 고급 제련석 파괴 확률 7% (21 ~ 30)
#define ITEM_UPGRADE_PROB_SPECIAL_PLUS		7700		// 고급 제련석 성공 최대 확률 50% (31 ~ 80)

#define ITEM_UPGRADE_PENALTY_BROKEN			3			// 깨지거나 -1 없는 아이템 등급의 한계

#define ITEM_UPGRADE_GENERAL_MINLEVEL		1			// 일반제련석 최소 1레벨 부터
#define ITEM_UPGRADE_GENERAL_MAXLEVEL		80			// 60레벨 까지

#define ITEM_REFINE_RESULT_TWO				0			// 무기 일반 제련석 2개
#define ITEM_REFINE_RESULT_ONE				1			// 방어구 일반 제련석 1개
#define ITEM_REFINE_RESULT_SPECIAL			2			// 고급 제련석 1개


// 인벤 위치 관련
#define INVENTORY_NORMAL			0	// 일반 탭
#define INVENTORY_QUEST				1	// 퀘스트 탭
#define INVENTORY_EVENT				2	// 이벤트 탭

// 착용 관련
#ifdef ENABLE_PET
#define MAX_WEARING					11	// 캐릭터 당 최대 착용 장비 수 : 투구 상의 무기 하의 방패 장갑 신발 악세1 악세2 악세3 애완동물
#else
#define MAX_WEARING					10	// 캐릭터 당 최대 착용 장비 수 : 투구 상의 무기 하의 방패 장갑 신발 악세1 악세2 악세3
#endif
#define WEARING_NONE				-1	// 착용 안함
#define WEARING_SHOW_START			1	// 외부로 보이는 착용 장비 시작 인덱스
#define WEARING_SHOW_END			6	// 외부로 보이는 착용 장비 끝 인덱스
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
#ifdef ENABLE_PET
#define WEARING_PET					10
#endif
#ifndef ENABLE_PET
// 클라이언트에서 악세사리 워치 정하지 않고 보낼경우 사용 (더블클릭 --)
#define WEARING_ACCESSORY_ANYWHERE	(MAX_WEARING)
#endif

#define GET_INVENTORY(ch, tab) ((tab == INVENTORY_NORMAL) ? &ch->m_invenNormal \
							  : (tab == INVENTORY_QUEST) ? &ch->m_invenQuest \
							  : (tab == INVENTORY_EVENT) ? &ch->m_invenEvent : NULL)

#define GET_TAB(type, subtype) ( (type != ITYPE_ETC) ? INVENTORY_NORMAL : (subtype == IETC_QUEST) ? INVENTORY_QUEST : (subtype == IETC_EVENT) ? INVENTORY_EVENT : INVENTORY_NORMAL)


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

#define		PLAYER_STATE_SITDOWN		(1 << 0)	// 앉기/서기
#define		PLAYER_STATE_MOVING			(1 << 1)	// 이동/정지
#define		PLAYER_STATE_WARP			(1 << 2)	// 귀환 중
#define		PLAYER_STATE_PKMODE			(1 << 3)	// PK 모드
#define		PLAYER_STATE_PKMODEDELAY	(1 << 4)	// PK 모드 해제 딜레이
#define		PLAYER_STATE_RAMODE			(1 << 5)	// 정당방위 모드
#define		PLAYER_STATE_CHANGE			(1 << 6)	// 변신중
#define		PLAYER_STATE_BILLITEM		(1 << 7)	// 빌링아이템 지급중
#define		PLAYER_STATE_CASHITEM		(1 << 8)	// 캐쉬아이템 지급중

#ifdef NEW_PK
#define		RAMODE_USE_PULSE			(3 * PULSE_REAL_MIN)	// 정당방위 유지 시간 30s
#else
#define		RAMODE_USE_PULSE			(30 * PULSE_REAL_SEC)	// 정당방위 유지 시간 30s
#endif
#define		RAMODE_DELAY_PULSE			(5 * PULSE_REAL_SEC)	// 정당방위 해제 알림 5s
#define		RAMODE_MAX_TARGET			8			// 정당방위 적용 최대 8

#ifdef PKMODE_LIMIT_LEVEL
#else
#define		PKMODE_LIMIT_LEVEL			15			// 레벨 이하 PK 못함
#endif

#define		PK_NAME_CHAOSLEGION			(-5)
#define		PK_NAME_NECROKNIGHT			(-4)
#define		PK_NAME_OUTLAW				(-3)
#define		PK_NAME_FEARCASTER			(-2)
#define		PK_NAME_MURDERER			(-1)
#define		PK_NAME_NORMAL				( 0)
#define		PK_NAME_AVENGER				(+1)
#define		PK_NAME_LIGHTPORTER			(+2)
#define		PK_NAME_GUARDIAN			(+3)
#define		PK_NAME_SAINTKNIGHT			(+4)
#define		PK_NAME_DIVINEPROTECTOR		(+5)


/////////////////////////
// Quick Slot 관련

#define		QUICKSLOT_PAGE_NUM		3		// 3페이지
#define		QUICKSLOT_MAXSLOT		10		// 10개 단축창 F1-F10
#define		QUICKSLOT_TYPE_EMPTY	-1		// 슬롯 비었음
#define		QUICKSLOT_TYPE_SKILL	0		// 스킬타입
#define		QUICKSLOT_TYPE_ACTION	1		// 액션타입
#define		QUICKSLOT_TYPE_ITEM		2		// 아이템타입


///////////////////////
// 메세지 보내기 매크로
#define SEND_Q(msg, desc)		{ if (desc != NULL) (desc)->WriteToOutput(msg); }

// 메신저 상태 검사
#define IS_RUNNING_MSGR			(!gserver.m_brunmessenger && gserver.m_messenger && !gserver.m_messenger->m_bclosed)

// 헬퍼 상태 검사
#define IS_RUNNING_HELPER		(!gserver.m_brunhelper && gserver.m_helper && !gserver.m_helper->m_bclosed)


////////////////////////
// 컨넥터 관련 유저 상태
#define CONN_WAIT_IDEN			0			// 빌링 시스템 인증 대기
#define CONN_PLAYING			1			// 플레이중
#define CONN_CHANGE_SERVER		2			// 서버 이동 중

#define IS_RUNNING_CONN			(!gserver.m_brunconnector && gserver.m_connector && !gserver.m_connector->m_bclosed)

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


#define	AGT_SITDOWN				3	// 앉기서기
#define	AGT_PKMODE				23	// PK 선언
#define AGT_PET_SITDOWN			37	// 펫 앉기


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


// 수행별 타입
#define		QTYPE_REPEAT_ONCE			0		// 1회용 퀘스트
#define		QTYPE_REPEAT_UNLIMITED		1		// 무한 퀘스트


// 시작 타입
#define		QSTART_NPC					0		// NPC를 통한 시작
#define		QSTART_ITEM					1		// 아이템을 통한 시작
#define		QSTART_LEVEL				2		// 레벨 업에 의해 시작
#define		QSTART_AREA					3		// 특정지역에 도달하여 시작


// 퀘스트 수행 조건
#define		QCONDITION_NPC				0		// NPC
#define		QCONDITION_ITEM				1		// 아이템
#define		QCONDITION_ITEM_NORMAL		2		// 아이템(일반)

// 퀘스트 보상 종류
#define		QPRIZE_ITEM					0		// 아이템
#define		QPRIZE_MONEY				1		// 나스
#define		QPRIZE_EXP					2		// 경험치
#define		QPRIZE_SP					3		// 스킬포인트
#define		QPRIZE_SKILL				4		// 스킬
#define		QPRIZE_SSKILL				5		// 특수스킬

#define		QUEST_MAX_PC				16		// 캐릭터 멀티 퀘스트 10개 -> 16
#define		QUEST_MAX_NEED_ITEM			5		// 퀘스트 시 필요아이템 최대 5

#define		QUEST_MAX_CONDITION			3		// 퀘스트 수행조건 최대 3
#define		QUEST_MAX_CONDITION_DATA	4		// 퀘스트 수행조건 하나당 부가 데이타 최대 4
#define		QUEST_MAX_ITEM_DROP_MONSTER	3		// 수집 퀘스트 : 아이템 드롭 몬스터 최대 3

#define		QUEST_MAX_PRIZE				5		// 퀘스트 보상 최대 5

#define		QUEST_MAX_NPC				50		// npc 하나당 최대 퀘스트 보유 수 50
#define		QUEST_MAX_PC_COMPLETE		100		// pc가 완료한 1회용 퀘스트 최대 수 100

#ifdef QUEST_DATA_EXTEND
// 퀘스트 상태 상수
#define		QUEST_STATE_ALL				0		// 전체 퀘스트 검색시
#define		QUEST_STATE_INIT			1		// 퀘스트 초기 상태
#define		QUEST_STATE_RUN				2		// 퀘스트 실행 중
#define		QUEST_STATE_DONE			3		// 퀘스트 완료 상태 (완전 완료)
#define		QUEST_STATE_ABANDON			4		// 퀘스트 포기 상태
#endif // QUEST_DATA_EXTEND

#define		S2_TICKET_MOB_START			13		// 싱글던전2 입장권 주기 (몹잡았을때) 몹레벨 시작
#define		S2_TICKET_MOB_END			27		// 싱글던전2 입장권 주기 (몹잡았을때) 몹레벨 끝

#define		S2_TICKET_LEVEL_START		13		// 싱글던전2 입장권 주기 (레벨업) pc레벨 시작
#define		S2_TICKET_LEVEL_END			27		// 싱글던전2 입장권 주기 (레벨업) pc레벨 끝

#define		S2_TICKET_DROP_PROB			50		// 싱글던전2 입장권 드롭 확률

#define		S2_TICKET_QUEST_DONE		5		// 5회 퀘스트 완료하면 입장권 제공



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

#define		OPTION_MP_STILL				24		// 마나 스틸
#define		OPTION_HP_STILL				25		// 생명력 스틸

#define		OPTION_ATT_FIRE_UP			26		// 화염 속성 확률 상승
#define		OPTION_ATT_WATER_UP			27		// 빙정 속성 확률 상승
#define		OPTION_ATT_WIND_UP			28		// 폭풍 속성 확률 상승
#define		OPTION_ATT_EARTH_UP			29		// 대지 속성 확률 상승
#define		OPTION_ALL_ATT_UP			30		// 모든 속성 확률 상승

#define		OPTION_ATT_FIRE_DOWN		31		// 화염 속성 확률 감소
#define		OPTION_ATT_WATER_DOWN		32		// 빙정 속성 확률 감소
#define		OPTION_ATT_WIND_DOWN		33		// 폭풍 속성 확률 감소
#define		OPTION_ATT_EARTH_DOWN		34		// 대지 속성 확률 감소
#define		OPTION_ALL_ATT_DOWN			35		// 모든 속성 확률 감소

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

#define		MAX_NUM_OPTION				49		// 옵션 총 숫자
#define		OPTION_VALUE_SHIFT			8		// 2byte 값중 왼쪽 1Byte 옵션 번호 오른쪽 1Byte 옵션 레벨
#define		OPTION_MAX_LEVEL			7		// 옵션 최대 레벨 현재 7

#define		MAX_ITEM_OPTION				5		// 아이템에 붙는 최대 옵션
#define		MAX_ACCESSORY_OPTION		5		// 악세사리에 붙을 수 있는 최대 옵션 수
#define		MAX_WEAPON_OPTION			5		// 무기에 붙을 수 있는 최대 옵션 수
#define		MAX_ARMOR_OPTION			3		// 방어구에 붙을 수 있는 최대 옵션 수
#define		MAX_ITEM_OPTION_DEL			3		// 옵션을 지울 수 있는 옵션 수


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

#ifdef TEST_SERVER

#define		TEACH_MAX_STUDENTS			8			// 최대 학생 수
#define		TEACH_LEVEL_TEACHER			18			// 선생 가능 레벨
#define		TEACH_LEVEL_STUDENT			4			// 제자 가능 레벨
#define		TEACH_LEVEL_SUCCESS			5			// 사제 성공 레벨 
#define		TEACH_LIMIT_SEC				864000		// 10일
#define		TEACH_FAME_PRIZE			20			// 보상 명성치
#define		TEACH_PRIZE_LEVEL			0			// 10000나스 지급 레벨

#else

#define		TEACH_MAX_STUDENTS			8			// 최대 학생 수
#define		TEACH_LEVEL_TEACHER			18			// 선생 가능 레벨
#define		TEACH_LEVEL_STUDENT			10			// 제자 가능 레벨
#define		TEACH_LEVEL_SUCCESS			20			// 사제 성공 레벨 
#define		TEACH_LIMIT_SEC				864000		// 10일
#define		TEACH_FAME_PRIZE			10			// 보상 명성치
#define		TEACH_PRIZE_LEVEL			9			// 10000나스 지급 레벨

#endif

#define		CANCELTEACHER				(1 << 0)	// 선생만 해제
#define		CANCELSTUDENT				(1 << 1)	// 학생만 해제

///////////////
// 펫 관련 상수

#ifdef ENABLE_PET

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

#define PET_MAX_LEVEL			60		// 펫 최고 레벨 60

#endif // #ifdef ENABLE_PET

///////////////////
// 소환수 관련 상수

#define ELEMENTAL_FIRE			0		// 불의 정령
#define ELEMENTAL_WIND			1		// 바람의 정령
#define ELEMENTAL_EARTH			2		// 대지의 정령
#define ELEMENTAL_WATER			3		// 물의 정령

#define ELEMENTAL_ATTACK_SPEED	20		// 기본 공속
#define ELEMENTAL_RUN_SPEED		8.0f	// 기본 이속
#define ELEMENTAL_RECOVER_HP	1		// 3초에 소환수 체력 회복량

#define ELEMENTAL_ATTACK_RANGE_FIRE		2.0f	// 공격거리 : 불의정령
#define ELEMENTAL_ATTACK_RANGE_WIND		7.0f	// 공격거리 : 바람의정령
#define ELEMENTAL_ATTACK_RANGE_EARTH	3.0f	// 공격거리 : 대지의정령
#define ELEMENTAL_ATTACK_RANGE_WATER	7.0f	// 공격거리 : 물의정령

#ifdef BSTEST
#define SUMMON_DELAY			(PULSE_REAL_MIN * 0)	// 소환 딜레이
#else
#define SUMMON_DELAY			(PULSE_REAL_MIN * 5)	// 소환 딜레이
#endif // #ifdef BSTEST
#define SUMMON_DURATION			(PULSE_REAL_MIN * 7)	// 소환 지속시간


/////////////////
// 강신 관련 상수
#define EVOCATION_NONE			0		// 강신 안함
#define EVOCATION_HELLOUND		1		// 헬운드 강신
#define EVOCATION_ELENEN		2		// 엘레넨 강신

#ifdef BSTEST
#define EVOCATION_DELAY			(PULSE_REAL_MIN * 0)	// 강신 딜레이
#else
#define EVOCATION_DELAY			(PULSE_REAL_MIN * 5)	// 강신 딜레이
#endif // #ifdef BSTEST
#define EVOCATION_DURATION		(PULSE_REAL_MIN * 10)	// 강신 지속시간

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


// t_cashItemdate 필드명
#define CASH_ITEM_DATE_FIELD_MEMPOS			"a_mempos_new"
#define CASH_ITEM_DATE_FIELD_CHAR_SLOT0		"a_charslot0_new"
#define CASH_ITEM_DATE_FIELD_CHAR_SLOT1		"a_charslot1_new"
#define CASH_ITEM_DATE_FIELD_STASHEXT		"a_stashext_new"


#endif