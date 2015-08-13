#ifdef LC_KOR

////// 국가 필드 설정 //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#define PREMIUM_CHAR			// 프리미엄캐릭터 (by 가미고)
#define SERVER_AUTHENTICATION		// 서버 기동 제한 시스템

#define		TLD_EVENT_SONG
#define		HARDCORE_SERVER
#define		STASH_PASSWORD
#define DURABILITY		// 장비 내구도

#define GIVE_ITEM_IN_CHANGE_JOB		// 전직시 아이템 지급하기

#define ITEM_NAME_FIELD         "a_name"
#define NPC_NAME_FIELD          "a_name"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_kor"
#define EVENT_EXTRA_FILED       "a_extra_kor"

#define ENABLE_SUBJOB							// 상인

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

#define OX_QUIZ_COUNTRY_CODE    1

////// 상시 이벤트 /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////// 업데이트 시스템 /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define CHARDEL_CHECKID                 // 케릭터 삭제시 민증번호 체크
#define EMPAS_LOGIN                     // 엠파스 로그인

#define RECOMMEND_SERVER_POTION_INDEX   1393
#define RECOMMEND_SERVER_LEVEL          60          // 2008 여름이벤트 이후 30 -> 60으로 변경
#define DROP_MAKE_DOCUMENT              // 제작서 드롭
// User_db의 t_count_external_id 테이블의 인덱스와 동일하게 맞추어야 함.
#define MAX_UPGRADE_PLUS    25          // 아이템 업그레이드 최대 플러스 수치

#define RAID_NPC_RAGE_SKILL             394     // 레이드 몹 분노 스킬 인덱스
#define RAID_NPC_RAGE_SKILL_LEVEL       2       // 레이드 몹 분노 스킬 레벨
#define RAID_NPC_NOTICE_CELL_EXT        5
#define MAX_RARE_OPTION_SETTING         10      // 레어 옵션당 옵션 세팅 수
#define MAX_RARE_OPTION_ITEM            10      // 아이템당 레어 옵션 수

#define PULSE_MONSTER_RAID_MOVE     (14 * PULSE_REAL_MIN)   // 몬스터 레이드 이동 시간

#define PARTY_QUEST_ITEM_BUG_			// 파티 퀘스트 아이템 지급이 되지 않던 문제
#define HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER	//파티원이 내 퀘스트 해주기(몹 잡아주면 카운트 되게 함. 기존엔 파티원도 동일한 퀘가 있어야 같이 되었음)


#define PULSE_GUILD_POINT_UPDATE        (5 * 60)    // 길드 랭킬용 경험치 저장 간격


#define NEW_DOUBLE_GM_SAVEFILE  ".doubleevent"      // DOUBLE EVENT 명령어 저장
#define DEFAULT_NAS_PERCENT     100     // 나스 드롭확률 조절
#define DEFAULT_NAS_GET_PERCENT 100     // 나스 습득량 조절
#define DEFAULT_EXP_PERCENT     100     // 경험치 획득량 조절
#define DEFAULT_SP_PERCENT      100     // sp 획득량 조절
#define DEFAULT_PRO_PERCENT     100     // 생산 습득 확률 조절
#define DEFAULT_PRO_GET_NUMBER  1       // 생산 습득 갯수
#define NEW_DOUBLE_GM_ZONE
#define NEW_DOUBLE_EVENT_AUTO
#define TLD_DOUBLE_EVENT    300 // 태국은 더블 이벤트 사용 시 300%까지 지원
#define DOUBLE_ITEM_DROP    // 아이템 드롭 2배 이벤트
#define DOUBLE_ITEM_DROP_SAVEFILE   ".doubleitemevent"      // DOUBLE EVENT 명령어 저장


#define PET_TURNTO_NPC_ITEM             // PET NPC 변신 사이즈 추가// 팻 변신 아이템 ( 아이템 추가시 ItemUse에 인덱스 설정 )

#define DEFAULT_LIMIT_EXP               250

#define EVENT_PCBANG_2ND

#define HANARO_EVENT		59		// 하나로 이벤트 2009   - 3채널로 변경애햐 함


#define MONSTER_AI              // 몬스터 A.I
#define NO_CHATTING             // 채팅 차단
#define EXTREME_CUBE            // 익스트림 큐브


////////////////////  확장팩  ////////////////////
////////////////////  확장팩  ////////////////////

#define MONSTER_COMBO_FIX

//////////////////// 2월 4일 업데이트 ////////////////////
//
#define CONTINENT_PARTY_RECALL							// 대륙별 파티리콜 아이템 추가
#define NO_PERIOD_COMPOSITE								// 조합아이템 중 기간이 없는 아이템 추가
//
#define DYNAMIC_DUNGEON_BUG_FIX							// 테오스 무덤에서 스킬 사용 시 몹의 체력이 복구 되어지는 것처럼 보여지는 현상. maxHP reset으로 인해 그런것 처럼 보임.
#define ATTACK_LIST_BUG_FIX								// GPSS Code : LC-KR-P20091201-012
//////////////////// 2월 4일 업데이트 ////////////////////
////////////////////////////// 기능 추가 /////////////////////////////
#define DUNGEON_TIME_SAVEFILE	".dungeontime"						// 던전타임 세팅 저장 파일.
#define LACARETTE_SYSTEM											// 라카렛 시스템 (문스톤2)
#define IP_NATION_CHECK												// 로그인 서버, 접속한 유저가 어느 국가인지 확인하고 t_users에 기록함.
//
//
// 이벤트 진행 시 시작시간 종료시간을								//
// 입력하여 자동으로 진행될 수 있도록 함.							//
#define NEW_DOUBLE_GM_AUTO											//
//
#ifdef NEW_DOUBLE_EVENT_AUTO										//
#define NEW_DOUBLE_EVENT_AUTO_TIME									//
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME									//
#define NEW_DOUBLE_EVENT_AUTO_SAVEFILE ".doubleeventautotime"		//
#endif																//
#endif																//
//
#define EVENT_ITEMDROP_AUTO											//
#ifdef EVENT_ITEMDROP_AUTO											//
#define EVENT_ITEMDROP_AUTO_SAVEFILE ".eventitemdropauto"			//
#endif																//
//
#define GM_EXP_LIMIT_AUTO											//
#ifdef GM_EXP_LIMIT_AUTO											//
#define GM_EXP_LIMIT_AUTO_SAVEFILE ".explimitsave"					//
#endif																//
// 이벤트 진행 시 시작시간 종료시간을								//
// 입력하여 자동으로 진행될 수 있도록 함.							//
//
#define JUMIN_DB_CRYPT												// 웹DB의 주빈번호 암호화로 인한 주민번호 체크 삭제.(2010/09/16)
//
//
//
#define UPGRADE_EVENT_AUTO											// 업그레이드 이벤트 자동화(2010/10/07)
#define UPGRADE_EVENT_AUTO_SAVEFILE		".upgradeevent"				//
//
#define NPC_PORTAL_SCROLL_DRATAN									// 캐릭터 생성시 드라탄 NPC스크롤 지급.

////////////////////////// UPDATE 1009 ///////////////////////////////
//
//
//
//
//
//
////////////////////////// UPDATE 1009 ///////////////////////////////
////////////////////////////// 기능 추가 /////////////////////////////

//////////////////// 아이리스대장정 ///////////////////////

//////////////////// 아이리스대장정 ///////////////////////
//////////////// MiniPackage(110310) ////////////////////
#define DISCONNECT_HACK_CHARACTER
#define RAIDBOX_DROP_COUNT_MODIFY
//////////////// MiniPackage(110310) ////////////////////
///////////////////////// UPDATE 1104 ////////////////////////////
#ifdef EXTREME_CUBE
#define EXTREME_CUBE_VER2									// 큐브2
#endif // EXTREME_CUBE
#define SYSTEM_MONSTER_MERCENARY_CARD_EX
#define SYSTEM_MONSTER_MERCENARY_CARD_LOW_LEVEL_SUPPORT
///////////////////////// UPDATE 1104 ////////////////////////////
///////////////////////// UPDATE 1105 ////////////////////////////
#define SYSTEM_TREASURE_MAP
#define SYSTEM_TREASURE_MAP_GAMIGO// 보물 찾기 시스템
#define GM_TREASURE_MAP_SAVE_FILE    ".treasuremapsave"
#define SYSTEM_TREASURE_MAP_LINKZONE_DROP                       // 던전 보물 지도 드랍
#define DEV_LETS_PARTYTIME
#define CHANGE_WARCASTLE_SETTING				// 공성 전 PVP 설정 변경
//#endif
///////////////////////// 110615 UPDATE ////////////////////////////
/////////////// 버그 수정 ////////////////

#define CASHITEM_EXPSP_LIMIT_BUG		// 경험치 획득제한이 300% 이상일 경우 경험치가 제대로 들어가지 않는 버그 수정 GPSS : LC-KR-P20100219-013
#define BATTLE_PARTY_BOSS_CHANGE		// 전투파티 시 파티장 변경에 대한 조건 검사(권상욱) GPSS : LC-DE-P20100123-001

#define ASSIST_DECREASE_TIME_BUG_FIX	// 버프 시간 동기화, 광속(그외 스킬 포함) 버프 클라이언트와 서버 시간 안맞는 것 수정(권상욱).

#define _BATTLEGROUP_QUEST_BUG_PIX		// [2010-05-19] 원정대 퀘스트 버그 수정(최영준) : 원정대 중 막타 친 캐릭터만 퀘스트가 조건이 카운트 되는 버그 수정.

#define TIME_IN_WARCASTLE_BUG_FIX		// 공성중 텔레키네시스 스킬 못쓰도록 막음.
#define BUGFIX_USE_SKILL_TO_NPC			// 업데이트1009와 함께 들어감. 자세한 버그수정의 자세한 내용은 항목별 패치방법..
#define BUGFIX_SPECIALBOX_DROPRATE      // 스페셜 상자 아이템 확률 프로세스 변경



#define BUGFIX_DEAD_AGAIN_DEAD			// 마을에 와서 한번더 죽는 문제 마을에서 3초간 무적으로 함.

#define BUGFIX_CASHSHOP_PURCHASE_CONFIRM// 캐시샵 버그 수정

#define BUGFIX_GUILD_SKILL_COOLTIME		// 길드스킬 쿨타임 감소옵션 적용안되게 수정



#define BUGFIX_ALLOW_EXPED_TYPE_SET		// 원정대 타입 동기화
#define BUGFIX_CUBE_DELETE_MEMLIST		// 길드 탈퇴 시 큐브 멤버리스트에서 삭제
#define BUGFIX_REMOVE_SPACE
#define BUGFIX_REFINE_SPECIAL_ITEM								// 128레벨 아이템 변환시 100% 고급제련석으로 주는 버그 수정. 방어구와 똑같이 처리하도록 수정.
/////////////// 버그 수정 ////////////////
//////// UPDATE 1106 //////// 42주차 빌드

#define REFORM_PK_PENALTY_201108



//////// UPDATE 1106 ////////
#define DEV_EVENT_AUTO    // 이벤트 설정 파일로 로딩

#define DEV_GUILD_MARK          // 길드 마크
#define DEV_GUILD_STASH         // 길드 창고

#define NOTICE_SYSTEM           // 공지 시스템

#define GUILD_REMOTE_INVITE             // 길드 원격 가입 시스템

//////// UPDATE 121Q-1 (12년 1월 30일 ) ////////


////// 운영자 레벨 제한 ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define     GM_LEVEL_ECHO               8
#define     GM_LEVEL_GO_ZONE            3
#define     GM_LEVEL_GOTO               3
#define     GM_LEVEL_ITEMDROP           10
#define     GM_LEVEL_COUNT_PC           10
#define     GM_LEVEL_LOOPCOUNT          10
#define     GM_LEVEL_IMMORTAL           3
#define     GM_LEVEL_SHUTDOWN           9
#define     GM_LEVEL_REBOOT             9
#define     GM_LEVEL_COUNT_NPC          9
#define     GM_LEVEL_VISIBLE            3
#define     GM_LEVEL_GO_PC              3
#define     GM_LEVEL_GO_NPC             3
#define     GM_LEVEL_LEVELUP            8
#define     GM_LEVEL_KICK               5
#define     GM_LEVEL_COMON              8
#define     GM_LEVEL_SUMMON             8
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
#define     GM_LEVEL_EVENTSETTING       9
#define     GM_LEVEL_CHANCE_EVENT       9
#define     GM_LEVEL_GMCHATMONITOR      10
#define     GM_LEVEL_DOUBLE_ITEM_EVENT  9
#define     GM_LEVEL_DOUBLE_PET_EXP_EVENT   9
#define     GM_LEVEL_DOUBLE_ATTACK_EVENT    9
#define     GM_LEVEL_EVENT_DROPITEM     9
#define     GM_LEVEL_KICK_PC_MULRI      9
#define     GM_LEVEL_SILENCE_PC_MULTI   9
#define     GM_LEVEL_SET_EXPLIMIT       9
#define     GM_LEVEL_DOUBLE_EVENT_AUTO  9
#define     GM_LEVEL_UPGRADE_EVENT      9
#define     GM_LEVEL_KICK_GUILDMEMBER   9
#define     GM_LEVEL_APPLY_SKILL        9
#define		GM_LEVEL_STATE_FLAG			10
#define		GM_LEVEL_DATE				1
#define		GM_LEVEL_PLAY_TRIGGER		10
#define		GM_LEVEL_BATTLE_START		9
#define		GM_LEVEL_BATTLE_STOP		9
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

#define ASSIST_DECREASE_SKILL_MP
#define ASSIST_REDUCE_SKILL
#define SKILL_TYPE_NO_COOL_TIME
#define RUS_MD5									// 러시아 bg_user 패스워드 md5로 변경
// ALTER TABLE `bg_user` CHANGE COLUMN `passwd` `passwd` VARCHAR(32) NOT NULL DEFAULT '0' AFTER `user_id`;
// UPDATE bg_user SET passwd = MD5(passwd);
#define EVENT_PACKAGE_ITEM                      // 패키지 시리얼을 이용한 아이템 지급
#define CHECKIP_USA
#define MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING

#ifdef BILL_SERVER_VERSION
#undef BILL_SERVER_VERSION
#define BILL_SERVER_VERSION "TEnterBillV1.0"
#define USE_TENTER_BILLING
#endif

#define DOUBLE_PET_EXP				// 펫 경험치 이벤트 자동화

#ifdef DOUBLE_PET_EXP
#define DOUBLE_PET_EXP_AUTO
#define DOUBLE_PET_EXP_SAVEFILE	".doublepetexp"
#endif

#define EVENT_EUR2_PROMOTION

#define JUNO_RENEWAL_MESSAGEBOX					// 독일용, 신규캐릭터 이동 시 쥬노로 갔다가 버뱅크로 간다.

#define EX_ROGUE	// EX 로그 캐릭터
#define EX_MAGE			// EX메이지 캐릭터 추가.

#define EGEHA_ZONE_PAYMENT              500000          // 에게하 입장료

#define ENABLE_TUTORIAL
#endif // LC_KOR