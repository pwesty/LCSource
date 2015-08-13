#ifdef LC_RUS

#define JUNO_RENEWAL_MESSAGEBOX					// 독일용, 신규캐릭터 이동 시 쥬노로 갔다가 버뱅크로 간다.
#define SERVER_AUTHENTICATION		// 서버 기동 제한 시스템
#define GIVE_ITEM_IN_CHANGE_JOB		// 전직시 아이템 지급하기

////// 국가 필드 설정 //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define ITEM_NAME_FIELD         "a_name_rus"
#define NPC_NAME_FIELD          "a_name_rus"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_rus"
#define EVENT_EXTRA_FILED       "a_extra_rus"

////// IP 자동 제한 설정 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define IP_BLOCK_AUTO
#if defined (IP_BLOCK_AUTO )
#define IP_BLOCKNOKICK_AUTO     // 아이피 블럭모드 일때 킥하지 않고 로그만 남김
#endif // #if defined (IP_BLOCK_AUTO )

////// 해외 로컬 제한 설정 /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////// OX QUIZ 국가 코드 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define OX_QUIZ_COUNTRY_CODE    17

////// 상시 이벤트 /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////// 업데이트 ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define ENABLE_SUBJOB							// 상인
#define XTRAP
#define DURABILITY
#define PREMIUM_CHAR
#define RUS_BCRYPT										// 러시아 Botan bcrypt 적용
#define ASSIST_DECREASE_SKILL_MP
#define ASSIST_REDUCE_SKILL
#define SKILL_TYPE_NO_COOL_TIME
#define EX_ROGUE	// EX 로그 캐릭터
#define DROP_MAKE_DOCUMENT                              // 제작서 드롭

#define MAX_UPGRADE_PLUS                25              // 아이템 업그레이드 최대 플러스 수치

#define PULSE_GUILD_POINT_UPDATE        (5 * 60)        // 길드 랭킬용 경험치 저장 간격


#define RECOMMEND_SERVER_POTION_INDEX   1393
#define RECOMMEND_SERVER_LEVEL          60              // 2008 여름이벤트 이후 30 -> 60으로 변경

#define PARTY_QUEST_ITEM_BUG_			// 파티 퀘스트 아이템 지급이 되지 않던 문제
#define HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER	//파티원이 내 퀘스트 해주기(몹 잡아주면 카운트 되게 함. 기존엔 파티원도 동일한 퀘가 있어야 같이 되었음)



// 추가 유료아이템 7종

#define DEFAULT_LIMIT_EXP 250


#define RANDOM_ZUNO									   // 주노 지역 리스폰 위치 추가

#define DEFAULT_NAS_PERCENT             50				// 나스 드롭확률 조절
#define DEFAULT_NAS_GET_PERCENT         100				// 나스 습득량 조절
#define DEFAULT_EXP_PERCENT             100				// 경험치 획득량 조절
#define DEFAULT_SP_PERCENT              100				// sp 획득량 조절
#define DEFAULT_PRO_PERCENT             80				// 생산 습득 확률 조절
#define DEFAULT_PRO_GET_NUMBER          1				// 생산 습득 갯수


#define CHAT_BLOCK_USA									// 외치기
#define CHAT_BLOCK_USA_NEXT_TIME	30	// 다음 외칠수 있는 시간
#define CHAT_BLOCK_USA_LEVEL		1 // 외치기 가능한 레벨

#define EVENT_PACKAGE_ITEM								// 패키지 시리얼을 이용한 아이템 지급
#define EVENT_PACKAGE_ITEM_DBLOG



#define MONSTER_AI										// 몬스터 AI

#define SETTING_IF_INNER_IP								// 외부 아이피와 내부 아이피가 나뉘면 사용 ...yhj

#define RAID_NPC_RAGE_SKILL				394				// 레이드 몹 분노 스킬 인덱스
#define RAID_NPC_RAGE_SKILL_LEVEL		2				// 레이드 몹 분노 스킬 레벨
#define RAID_NPC_NOTICE_CELL_EXT		5
#define MAX_RARE_OPTION_SETTING			10				// 레어 옵션당 옵션 세팅 수
#define MAX_RARE_OPTION_ITEM			10				// 아이템당 레어 옵션 수
#define PULSE_MONSTER_RAID_MOVE		(14 * PULSE_REAL_MIN)	// 몬스터 레이드 이동 시간

#define WARCASTLE_SUBNUMBER_MEMBER_VALUE				// 메라크 공성전 서버 느려지는 것 체크하기 위한 임시 패치
#define EGEHA_ZONE_PAYMENT				500000			// 에게하 입장료
#define NEW_ACCERY_ADD									// 새로운 악세사리 4종
#define MAL_DISABLE_PKPENALTY							// 자유전투 구역(2채널)

#define HANARO_EVENT									// 하나로 이벤트 2009

#define PET_TURNTO_NPC_ITEM

//-----------    Update 13
#define EXTREME_CUBE					// 익스트림 큐브
#define TRADE_AGENT_LOG_SPEED_CHECK		// 거래대행 로그 추가


////////////////////  확장팩  ////////////////////
#define RESTRICT_PVP_SKILL         3		// 엄격한 스킬 규칙 : 몹에 타겟팅 되어 있을 경우, PK모드, 정당방위 모드 인비/데스모션 불가능
// 암흑의 제단에서 인비저빌리티가 안써지려면 있어야 하는 define
#define NO_PERIOD_COMPOSITE					// 조합아이템 중 기간이 없는 아이템 추가

#define _COMPOSITE_ITEM_UPGRADE_BUG_FIX

//#define QUEST_EXTEND_REPEAT_DAY				// 일일 퀘스트
////////////////////  확장팩  ////////////////////

#define LACARETTE_SYSTEM						// 라카렛 시스템 (문스톤2)

#define DUNGEON_TIME_SAVEFILE	".dungeontime"	// 던전타임 세팅 저장 파일.





#define DEV_SYSTEM_SKILL_EXTEND_2ND			// 캐릭터 스킬 확장 2차





//-- End of Update 1009 --//

// -- Mini Package 2011 -- //

#define RAIDBOX_DROP_COUNT_MODIFY		// 스페셜 상자 드롭 규칙 변경
// -- End of Mini Package 2011 -- //

// test server [110513], live server [110517]


// test server [110519], live server [110531]
// ---------------- Update 1104 -----------------------
#ifdef EXTREME_CUBE
#define EXTREME_CUBE_VER2									// 큐브2
#endif // EXTREME_CUBE
#define SYSTEM_MONSTER_MERCENARY_CARD_EX
#define SYSTEM_MONSTER_MERCENARY_CARD_LOW_LEVEL_SUPPORT
// ---------------- Update 1104 -----------------------

// test server [110608], live server [110614]
#define CHANGE_WARCASTLE_SETTING				// 공성 전 PVP 설정 변경

// ---------------- Update 1105 -----------------------
// test server : [110615], live server [110628]
#define DEV_LETS_PARTYTIME
#define SYSTEM_TREASURE_MAP										// 보물 찾기 시스템
#define SYSTEM_TREASURE_MAP_GAMIGO
#define GM_TREASURE_MAP_SAVE_FILE		 ".treasuremapsave"
#define SYSTEM_TREASURE_MAP_LINKZONE_DROP
// ---------------- Update 1105 -----------------------

// ---------------- Event ------------------
#define UPGRADE_EVENT_AUTO			// 업그레이드 이벤트 자동화
#define UPGRADE_EVENT_AUTO_SAVEFILE		".upgradeevent"

// --------------- BugFix -------------------
#define BUGFIX_USE_SKILL_TO_NPC




#define DISCONNECT_HACK_CHARACTER			// 핵 프로그램 유저 방출

#define BUGFIX_MERCENARY_GETOWNER_NULL_SERVERDOWN	// 용병의 주인이 없을 때 나는 크래시를 막기 위한 수정



#define BUGFIX_GUILD_SKILL_COOLTIME		// 길드스킬 쿨타임 감소옵션 적용안되게 수정

// test server [110513], live server [110517]


// test server [110519], live server [110531]
#define BUGFIX_HUGE_SKILL_ABSTIME

// test server [110601], live server [110607]

//////////////////// UPDATE 1106 ////////////////////

#define REFORM_PK_PENALTY_201108
#ifdef REFORM_PK_PENALTY_201108
#define REFORM_PK_PENALTY_BATTLE_201108	 // 공격/방어 패널티
#define REFORM_PK_PENALTY_SHOP_201108	 // 상점 이용 패널티
#endif


//////////////////// UPDATE 1106 ////////////////////

#define NO_CHATTING

//////// UPDATE 121Q (12년 4월 26일 ) ////////

#define DEV_GUILD_MARK		// 길드 마크
#define DEV_GUILD_STASH		// 길드 창고

#define NOTICE_SYSTEM           // 공지 시스템

#define GUILD_REMOTE_INVITE		// 길드 원격 가입 시스템

//////// UPDATE 121Q-1 (12년 4월 26일 ) ////////

#define EX_MAGE
////// 운영자 레벨 제한 ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define     GM_LEVEL_ECHO               7
#define     GM_LEVEL_GO_ZONE            3
#define     GM_LEVEL_GOTO               5
#define     GM_LEVEL_ITEMDROP           10
#define     GM_LEVEL_COUNT_PC           10
#define     GM_LEVEL_LOOPCOUNT          10
#define     GM_LEVEL_IMMORTAL           5
#define     GM_LEVEL_SHUTDOWN           10
#define     GM_LEVEL_REBOOT             10
#define     GM_LEVEL_COUNT_NPC          9
#define     GM_LEVEL_VISIBLE            5
#define     GM_LEVEL_GO_PC              3
#define     GM_LEVEL_GO_NPC             3
#define     GM_LEVEL_LEVELUP            8
#define     GM_LEVEL_KICK               7
#define     GM_LEVEL_COMON              7
#define     GM_LEVEL_SUMMON             8
#define     GM_LEVEL_SILENCE            1
#define     GM_LEVEL_WHERE_AM_I         3
#define     GM_LEVEL_QUEST_COMPLETE     8
#define     GM_LEVEL_STATPOINT          7
#define     GM_LEVEL_SKILLPOINT         7
#define     GM_LEVEL_SET_REGENSEC       8
#define     GM_LEVEL_NPC_REGEN          8
#define     GM_LEVEL_KILL_NPC           8
#define     GM_LEVEL_EXPUP              8
#define     GM_LEVEL_BLOODPOINT         7
#define     GM_LEVEL_ENDGAME            9
#define     GM_LEVEL_OBSERVE            9
#define     GM_LEVEL_CHATMONITOR        7
#define     GM_LEVEL_DOUBLE_EVENT       9
#define     GM_LEVEL_LATTO_EVENT        9
#define     GM_LEVEL_DOUBLE_EXP_EVENT   9
#define     GM_LEVEL_NEWYEAR_EVENT      9
#define     GM_LEVEL_VALENTINE_EVENT    9
#define     GM_LEVEL_WHITEDAY_EVENT     9
#define     GM_LEVEL_LETTER_EVENT       9
#define     GM_LEVEL_RELOAD_GMIP        10
#define     GM_LEVEL_SPEEDUP            7
#define     GM_LEVEL_FAMEUP             7
#define     GM_LEVEL_EVENT              8
#define     GM_LEVEL_PD4                7
#define     GM_LEVEL_KICK_ID            7
#define     GM_LEVEL_RESURRECTION       9
#define     GM_LEVEL_RECOMMEND          8
#define     GM_LEVEL_SETLIMIT           9
#define     GM_LEVEL_DISGUISE           9
#define     GM_LEVEL_LOGINSERVER        10
#define     GM_LEVEL_CALTIME            10
#define     GM_LEVEL_KICK_CHAR_INDEX    7
#define     GM_LEVEL_EVENTSHOW          5
#define     GM_LEVEL_EVENTSETTING       10
#define     GM_LEVEL_CHANCE_EVENT       9
#define     GM_LEVEL_GMCHATMONITOR      10
#define     GM_LEVEL_DOUBLE_ITEM_EVENT  9
#define     GM_LEVEL_DOUBLE_PET_EXP_EVENT   9
#define     GM_LEVEL_DOUBLE_ATTACK_EVENT    9
#define     GM_LEVEL_EVENT_DROPITEM     10
#define     GM_LEVEL_KICK_PC_MULRI      7
#define     GM_LEVEL_SILENCE_PC_MULTI   9
#define     GM_LEVEL_SET_EXPLIMIT       8
#define     GM_LEVEL_DOUBLE_EVENT_AUTO  10
#define     GM_LEVEL_UPGRADE_EVENT      10
#define     GM_LEVEL_KICK_GUILDMEMBER   7
#define     GM_LEVEL_APPLY_SKILL        10
#define		GM_LEVEL_STATE_FLAG			10
#define		GM_LEVEL_DATE				1
#define		GM_LEVEL_PLAY_TRIGGER		10
#define		GM_LEVEL_BATTLE_START		10
#define		GM_LEVEL_BATTLE_STOP		10
#define		GM_LEVEL_RESET_RAID			10
#define		GM_LEVEL_SET_SEAL_EXP		10
#define		GM_LEVEL_END_SUBHELPER		10
#define		GM_LEVEL_LCBALL_RESET		10
#define		GM_LEVEL_ECHONPC			9
#define		GM_LEVEL_DT_EXPSET			10
#define     GM_LEVEL_ITEMGET			10
#define		GM_LEVEL_ITEMGETRANGE		10
#define		GM_LEVEL_JOBRESET			10
#define		GM_LEVEL_JOBCHANGE			10

#endif // LC_RUS

