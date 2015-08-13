#ifndef __CONFIG_LOCALIZE_GAMIGO_H__
#define __CONFIG_LOCALIZE_GAMIGO_H__

#define LC_GAMIGO

#if defined(LC_GER)
#define GER_LOG											// 독일 로그 추가
#define VERSIONKEY "1106"
#define INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS
#define DEV_GAMIGO_HTTPS								// 가미고 Https 인증 방식 ( linux 만 적용 )
#endif

#if defined (LC_SPN)
#define INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN			// 해외 로컬 접근 제한 - new 버젼
#define INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_NEW_USER	// 신규유저는 유럽국가만 허용
#define IP_NATION_CHECK
#endif

#if defined (LC_FRC)
#endif

#if defined (LC_PLD)
#endif

#if defined (LC_ITA)
#define EVENT_EUR2_PROMOTION
#endif

////// 국가 필드 설정 //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#define		HARDCORE_SERVER

#ifdef LC_GER
#define ITEM_NAME_FIELD         "a_name_ger"
#define NPC_NAME_FIELD          "a_name_ger"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_ger"
#define EVENT_EXTRA_FILED       "a_extra_ger"
#elif defined (LC_SPN)
#define ITEM_NAME_FIELD         "a_name_spn"
#define NPC_NAME_FIELD          "a_name_spn"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_spn"
#define EVENT_EXTRA_FILED       "a_extra_spn"

#elif defined (LC_FRC)
#define ITEM_NAME_FIELD         "a_name_frc"
#define NPC_NAME_FIELD          "a_name_frc"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_frc"
#define EVENT_EXTRA_FILED       "a_extra_frc"

#elif defined (LC_PLD)
#define ITEM_NAME_FIELD         "a_name_pld"
#define NPC_NAME_FIELD          "a_name_pld"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_pld"
#define EVENT_EXTRA_FILED       "a_extra_pld"

#elif LC_ITA
#define ITEM_NAME_FIELD         "a_name_ita"
#define NPC_NAME_FIELD          "a_name_ita"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_ita"
#define EVENT_EXTRA_FILED       "a_extra_ita"
#elif LC_TUR
#define ITEM_NAME_FIELD         "a_name_tur"
#define NPC_NAME_FIELD          "a_name_tur"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_tur"
#define EVENT_EXTRA_FILED       "a_extra_tur"
#elif LC_UK
#define ITEM_NAME_FIELD         "a_name_uk"
#define NPC_NAME_FIELD          "a_name_uk"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_uk"
#define EVENT_EXTRA_FILED       "a_extra_uk"
#endif

////// OX QUIZ 국가 코드 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (LC_GER)
#define OX_QUIZ_COUNTRY_CODE	13
#elif defined (LC_SPN)
#define OX_QUIZ_COUNTRY_CODE    14
#elif defined (LC_FRC)
#define OX_QUIZ_COUNTRY_CODE    15
#elif defined (LC_PLD)
#define OX_QUIZ_COUNTRY_CODE    16
#elif defined (LC_TUR)
#define OX_QUIZ_COUNTRY_CODE    18
#elif defined (LC_ITA)
#define OX_QUIZ_COUNTRY_CODE    19
#elif defined (LC_UK)
#define OX_QUIZ_COUNTRY_CODE    24
#endif

////// 상시 이벤트 /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#define DOUBLE_PET_EXP
#ifdef DOUBLE_PET_EXP
#define DOUBLE_PET_EXP_AUTO
#define DOUBLE_PET_EXP_SAVEFILE	".doublepetexp"
#endif

////// IP 자동 제한 설정 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define IP_BLOCK_AUTO
#if defined (IP_BLOCK_AUTO )
#define IP_BLOCKNOKICK_AUTO     // 아이피 블럭모드 일때 킥하지 않고 로그만 남김
#endif // #if defined (IP_BLOCK_AUTO )

////// 운영자 레벨 제한 //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define     GM_LEVEL_ECHO               5
#define     GM_LEVEL_GO_ZONE            3
#define     GM_LEVEL_GOTO               3
#define     GM_LEVEL_ITEMDROP           10
#define     GM_LEVEL_COUNT_PC           10
#define     GM_LEVEL_LOOPCOUNT          10
#define     GM_LEVEL_IMMORTAL           3
#define     GM_LEVEL_SHUTDOWN           10
#define     GM_LEVEL_REBOOT             9
#define     GM_LEVEL_COUNT_NPC          9
#define     GM_LEVEL_VISIBLE            3
#define     GM_LEVEL_GO_PC              3
#define     GM_LEVEL_GO_NPC             3
#define     GM_LEVEL_LEVELUP            8
#define     GM_LEVEL_KICK               5
#define     GM_LEVEL_COMON              8
#define     GM_LEVEL_SUMMON             9
#define     GM_LEVEL_SILENCE            5
#define     GM_LEVEL_WHERE_AM_I         3
#define     GM_LEVEL_QUEST_COMPLETE     7
#define     GM_LEVEL_STATPOINT          7
#define     GM_LEVEL_SKILLPOINT         7
#define     GM_LEVEL_SET_REGENSEC       8
#define     GM_LEVEL_NPC_REGEN          8
#define     GM_LEVEL_KILL_NPC           1
#define     GM_LEVEL_EXPUP              8
#define     GM_LEVEL_BLOODPOINT         8
#define     GM_LEVEL_ENDGAME            9
#define     GM_LEVEL_OBSERVE            9
#define     GM_LEVEL_CHATMONITOR        5
#define     GM_LEVEL_DOUBLE_EVENT       9
#define     GM_LEVEL_LATTO_EVENT        9
#define     GM_LEVEL_DOUBLE_EXP_EVENT   9
#define     GM_LEVEL_NEWYEAR_EVENT      9
#define     GM_LEVEL_VALENTINE_EVENT    9
#define     GM_LEVEL_WHITEDAY_EVENT     9
#define     GM_LEVEL_LETTER_EVENT       9
#define     GM_LEVEL_RELOAD_GMIP        10
#define     GM_LEVEL_SPEEDUP            3
#define     GM_LEVEL_FAMEUP             9
#define     GM_LEVEL_EVENT              9
#define     GM_LEVEL_PD4                9
#define     GM_LEVEL_KICK_ID            5
#define     GM_LEVEL_RESURRECTION       9
#define     GM_LEVEL_RECOMMEND          9
#define     GM_LEVEL_SETLIMIT           9
#define     GM_LEVEL_DISGUISE           9
#define     GM_LEVEL_LOGINSERVER        9
#define     GM_LEVEL_CALTIME            10
#define     GM_LEVEL_KICK_CHAR_INDEX    5
#define     GM_LEVEL_EVENTSHOW          5
#define     GM_LEVEL_EVENTSETTING       10
#define     GM_LEVEL_CHANCE_EVENT       9
#define     GM_LEVEL_GMCHATMONITOR      10
#define     GM_LEVEL_DOUBLE_ITEM_EVENT  9
#define     GM_LEVEL_DOUBLE_PET_EXP_EVENT   9
#define     GM_LEVEL_DOUBLE_ATTACK_EVENT    9
#define     GM_LEVEL_EVENT_DROPITEM     10
#define     GM_LEVEL_KICK_PC_MULRI      9
#define     GM_LEVEL_SILENCE_PC_MULTI   9
#define     GM_LEVEL_SET_EXPLIMIT       10
#define     GM_LEVEL_DOUBLE_EVENT_AUTO  10
#define     GM_LEVEL_UPGRADE_EVENT      10
#define     GM_LEVEL_KICK_GUILDMEMBER   10
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

////// 가미고 공통 디파인 /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#define SERVER_AUTHENTICATION		// 서버 기동 제한 시스템

#define DURABILITY
#define PREMIUM_CHAR
#define INTERGRATION_SERVER
#define EU_INTERGRATION_SERVER
#if defined (EU_INTERGRATION_SERVER)
#define INTERGRATION_GER 13
#define INTERGRATION_SPN 14
#define INTERGRATION_FRC 15
#define INTERGRATION_PLD 16
#define INTERGRATION_TUR 18
#define INTERGRATION_ITA 19
#define INTERGRATION_UK  24
#endif

#define DROP_MAKE_DOCUMENT                              // 제작서 드롭
#define MAX_UPGRADE_PLUS                    25          // 아이템 업그레이드 최대 플러스 수치
#define RECOMMEND_SERVER_POTION_INDEX       1393
#define PULSE_GUILD_POINT_UPDATE            (5 * 60)    // 길드 랭킬용 경험치 저장 간격
#define DEFAULT_LIMIT_EXP               250
#define RANDOM_ZUNO                                     // 주노 지역 리스폰 위치 추가
#define NEW_DOUBLE_GM_SAVEFILE  ".doubleevent"      // DOUBLE EVENT 명령어 저장
#define DEFAULT_NAS_GET_PERCENT         100             // 나스 습득량 조절
#define DEFAULT_EXP_PERCENT             100             // 경험치 획득량 조절
#define DEFAULT_SP_PERCENT              100             // sp 획득량 조절
#define DEFAULT_PRO_GET_NUMBER          1               // 생산 습득 갯수
#define GER_SERIAL_PROMOTION            "FP"

#define CHAT_BLOCK_USA                                  // 외치기
#define CHAT_BLOCK_USA_NEXT_TIME	30 // 다음 외칠수 있는 시간
#define CHAT_BLOCK_USA_LEVEL		15 // 외치기 가능한 레벨

#define EVENT_PACKAGE_ITEM                              // 패키지 시리얼을 이용한 아이템 지급
#define EVENT_PACKAGE_ITEM_DBLOG
#define EGEHA_ZONE_PAYMENT              500000          // 에게하 입장료
#define RAID_NPC_RAGE_SKILL             394             // 레이드 몹 분노 스킬 인덱스
#define RAID_NPC_RAGE_SKILL_LEVEL       2               // 레이드 몹 분노 스킬 레벨
#define RAID_NPC_NOTICE_CELL_EXT        5
#define MAX_RARE_OPTION_SETTING         10              // 레어 옵션당 옵션 세팅 수
#define MAX_RARE_OPTION_ITEM            10              // 아이템당 레어 옵션 수
#define PULSE_MONSTER_RAID_MOVE         (14 * PULSE_REAL_MIN)   // 몬스터 레이드 이동 시간
#define EXTREME_CUBE                                    // 익스트림 큐브
#define HANARO_EVENT					//초보지원이벤트 추가(2014-5-28)
#define PARTY_BUG_GER
#define MONSTER_COMBO_FIX
#define LOGIN_TIME_CHECK
#define BATTLE_PARTY_BOSS_CHANGE		// 전투파티 시 파티장 변경에 대한 조건 검사
#define CASHITEM_EXPSP_LIMIT_BUG
#define LACARETTE_SYSTEM
#define DUNGEON_TIME_SAVEFILE	".dungeontime"	// 던전타임 세팅 저장 파일.

#define XTRAP
#define	_COMPOSITE_ITEM_UPGRADE_BUG_FIX
#define CHECK_CASTLE_AREA						// 공성지역 체크 안되는 버그 수정
#define BUGFIX_WARP_TO_WARZONE								// 소환마법서 아이템을 이용하여 공성지역 이동불가
#define TIME_IN_WARCASTLE_BUG_FIX				// 공성중 텔레키네시스 스킬 못쓰도록 막음
#define NEW_DOUBLE_GM_AUTO
#define QA_RANGESKILL_APET_DAMAGE				// PC 범위스킬사용시 APET 데미지 막음
#define TRADE_AGENT_LOG_SPEED_CHECK
#define ENABLE_SUBJOB							// 상인
#define BUGFIX_DROPITEM_LEVEL_CHECK				//
#define MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING
#define PASSWORD_ENCRYPT_SHA256
#define DEV_SYSTEM_SKILL_EXTEND_2ND
#define BUGFIX_USE_SKILL_TO_NPC
#define BUGFIX_TRADEAGENT_COPY
#define CONTINENT_PARTY_RECALL
#define BUGFIX_DRATAN_CRISTAL_RESPOND
#define UPDATE_CHAOS_TIME_ALWAYS
#define BUGFIX_CASHSHOP_PURCHASE_CONFIRM
#define DISABLE_TRADEAGENT_CHECKCALC
#define BUGFIX_GUILD_SKILL_COOLTIME			// 길드스킬 쿨타임 감소옵션 적용안되게 수정
#define WARCASTLE_STATE_CHANNEL_CYNC	// 공성 상태를 타 채널과 cync 하는 명령이 추가.
#define	WARCASTLE_MOVE_MESSAGE_LIMIT	// 공성전 이동 메시지 팝업창 제한.
#define GAMIGO_CASH_AUTHENTICATION_CODE_USE // authentication code를 사용한 캐쉬 결제 웹페이지 연동 (가미고 요청)
#define SYSTEM_TREASURE_MAP                                 // 보물 찾기 시스템
#define GM_TREASURE_MAP_SAVE_FILE ".treasuremapsave"
#define SYSTEM_TREASURE_MAP_LINKZONE_DROP                    // 던전 보물 지도 드랍
#define DEV_LETS_PARTYTIME									// 파티형 아이템
#define JUNO_RENEWAL_MESSAGEBOX								// 독일용, 신규캐릭터 이동 시 쥬노로 갔다가 버뱅크로 간다.
#define GUILD_CONTRIBUTE_EXP_LIMIT		50			// 길두 기부 경험치를 max 50% 까지만 가능하도록 수정
#define BUGFIX_WARCASTLE_REGEN						// 부활진지 리젠시간 수정
#define BUGFIX_ITEMSELL_HACKUSER					// 아이템판매시 아이템 카운트 검사 추가
#define BUGFIX_REFINE_SPECIAL_ITEM					// 128레벨 아이템 변환시 100% 고급제련석으로 주는 버그 수정. 방어구와 똑같이 처리하도록 수정.
#define UPGRADE_EVENT_AUTO			// 업그레이드 이벤트 자동화
#define UPGRADE_EVENT_AUTO_SAVEFILE		".upgradeevent"
#ifdef EXTREME_CUBE
#define EXTREME_CUBE_VER2								// 익스트림 큐브 개편 (시즌2)
#endif
#define SYSTEM_MONSTER_MERCENARY_CARD_EX						// 몬스터 용병의 사이즈 변경 기능 추가
#define SYSTEM_TREASURE_MAP_GAMIGO							// 가미고요청 : 보물지도를 인벤에 가지고 있어야 보물상자가 드랍. 1000개의 나무열쇠로 1개의 황금열쇠 교환.
#define BUGFIX_DONT_USE_GET_FIRST_PARTY			// 입수우선 파티를 퀵슬롯에서 제거
#define REFORM_PK_PENALTY_201108
#define DEV_GUILD_MARK		// 길드 마크
#define DEV_GUILD_STASH		// 길드 창고
#define NOTICE_SYSTEM		// 공지 시스템
#define GUILD_REMOTE_INVITE		// 길드 원격 가입 시스템
#define BUGFIX_REMOVE_SPACE
#define NO_PERIOD_COMPOSITE
#define GUILD_MEMBER_KICK_JOIN_DELAY						// 길드원을 킥하면 3일동안 다른 길드에 가입하지 못하도록 수정.
#define IP_NATION_CHECK
#define RECOMMEND_SERVER_LEVEL          30
#define DEFAULT_NAS_PERCENT             100             // 나스 드롭확률 조절
#define DEFAULT_PRO_PERCENT             100             // 생산 습득 확률 조절
#define GAMIGO_ITEM_COUNT_HACK_DELETE					// 아이템 카운트를 증가시키는 해킹을 방지. 10레벨 이하의 캐릭터가 로그 아웃 할때 카운트가 되는 아이템이 하나만 인벤에 있으면 무조건 삭제 처리.
#define IMP_SPEED_SERVER						// 스피드 서버
#define EX_ROGUE	// EX 로그 캐릭터
#define ASSIST_DECREASE_SKILL_MP
#define ASSIST_REDUCE_SKILL
#define SKILL_TYPE_NO_COOL_TIME
#define EX_MAGE

#define PARTY_QUEST_ITEM_BUG_			// 파티 퀘스트 아이템 지급이 되지 않던 문제
#define HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER	//파티원이 내 퀘스트 해주기(몹 잡아주면 카운트 되게 함. 기존엔 파티원도 동일한 퀘가 있어야 같이 되었음)
#define GIVE_ITEM_IN_CHANGE_JOB


////// 가미고 캐쉬 인증 코드 디파인 ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GAMIGO_CASH_AUTHENTICATION_CODE_USE // authentication code를 사용한 캐쉬 결제 웹페이지 연동 (가미고 요청)

#if defined(LC_GER)
#define LCOAL_GAMIGO_GROUP_ID			19
#define LOCAL_AUTHENTICATION_SECRETCODE "b8779493de6958e68fc422a58daf713e"
#elif defined(LC_SPN)
#define LCOAL_GAMIGO_GROUP_ID			37
#define LOCAL_AUTHENTICATION_SECRETCODE "643debd2620d19013e0b918af6bd749a"
#elif defined(LC_FRC)
#define LCOAL_GAMIGO_GROUP_ID			36
#define LOCAL_AUTHENTICATION_SECRETCODE "2fbbbf178046c7634fc8bad1c81f6bab"
#elif defined(LC_PLD)
#define LCOAL_GAMIGO_GROUP_ID			38
#define LOCAL_AUTHENTICATION_SECRETCODE "1088d561108d0c46dda33cfcffc2c1f0"
#elif defined(LC_TUR)
#define LCOAL_GAMIGO_GROUP_ID			68
#define LOCAL_AUTHENTICATION_SECRETCODE "14143358d9b27eb222588745da79eb6e"
#elif defined(LC_ITA)
#define LCOAL_GAMIGO_GROUP_ID			66
#define LOCAL_AUTHENTICATION_SECRETCODE "47bb626b57e501d786b1f3264aa8b260"
#elif defined(LC_LND)
#define LCOAL_GAMIGO_GROUP_ID			83
#define LOCAL_AUTHENTICATION_SECRETCODE "f46e5007701f1a5047e3e72321795ef"
#elif defined(LC_UK)
#define LCOAL_GAMIGO_GROUP_ID			19
#define LOCAL_AUTHENTICATION_SECRETCODE "b8779493de6958e68fc422a58daf713e"
#endif

#endif

#endif
