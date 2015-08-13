#ifdef LC_TLD

#define SERVER_AUTHENTICATION		// 서버 기동 제한 시스템

#define GIVE_ITEM_IN_CHANGE_JOB		// 전직시 아이템 지급하기
#define TLD_EVENT_SONG

////// 국가 필드 설정 //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//#define		STASH_PASSWORD

#define ITEM_NAME_FIELD         "a_name_thai"
#define NPC_NAME_FIELD          "a_name_thai"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_tld"
#define EVENT_EXTRA_FILED       "a_extra_tld"

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

#define OX_QUIZ_COUNTRY_CODE    4

////// 상시 이벤트 /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////// 업데이트 ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define DURABILITY
#define CHARDEL_CHECKID                 // 케릭터 삭제시 민증번호 체크

#define RECOMMEND_SERVER_POTION_INDEX   1393
#define RECOMMEND_SERVER_LEVEL          30
//#define CHARGE_ITEM_PERIOD              // 태국 유료 아이템 기간 변경 110803 주석
#define MAX_UPGRADE_PLUS    25          // 아이템 업그레이드 최대 플러스 수치
#define RANKER_NOTICE

#define PARTY_QUEST_ITEM_BUG_			// 파티 퀘스트 아이템 지급이 되지 않던 문제
#define HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER	//파티원이 내 퀘스트 해주기(몹 잡아주면 카운트 되게 함. 기존엔 파티원도 동일한 퀘가 있어야 같이 되었음)


#ifdef BILL_SERVER_VERSION
#undef BILL_SERVER_VERSION
#define BILL_SERVER_VERSION "TEnterBillV1.0"
#define USE_TENTER_BILLING
#endif
#define FREE_PK_SYSTEM          // 새로운 PK Server Open


#define NEW_DOUBLE_GM_SAVEFILE  ".doubleevent"      // DOUBLE EVENT 명령어 저장
#define DEFAULT_NAS_PERCENT     100     // 나스 드롭확률 조절
#define DEFAULT_NAS_GET_PERCENT 100     // 나스 습득량 조절
#define DEFAULT_EXP_PERCENT     100     // 경험치 획득량 조절
#define DEFAULT_SP_PERCENT      100     // sp 획득량 조절
#define DEFAULT_PRO_PERCENT     100     // 생산 습득 확률 조절
#define DEFAULT_PRO_GET_NUMBER  1       // 생산 습득 갯수

#define TLD_DOUBLE_EVENT    300 // 태국은 더블 이벤트 사용 시 300%까지 지원
#define DOUBLE_ITEM_DROP    // 아이템 드롭 2배 이벤트
#define DOUBLE_ITEM_DROP_SAVEFILE   ".doubleitemevent"      // DOUBLE EVENT 명령어 저장

// 070627 태국 레이드 몬스터 시스템 업데이트
#define RAID_NPC_RAGE_SKILL             394     // 레이드 몹 분노 스킬 인덱스
#define RAID_NPC_RAGE_SKILL_LEVEL       2       // 레이드 몹 분노 스킬 레벨
#define RAID_NPC_NOTICE_CELL_EXT        5
#define MAX_RARE_OPTION_SETTING         10      // 레어 옵션당 옵션 세팅 수
#define MAX_RARE_OPTION_ITEM            10      // 아이템당 레어 옵션 수

#define PULSE_MONSTER_RAID_MOVE     (14 * PULSE_REAL_MIN)   // 몬스터 레이드 이동 시간


// 추가 유료아이템 7종
#define DEFAULT_LIMIT_EXP           600

#define NEW_ACCERY_ADD              // 새로운 악세사리 4종

#define PULSE_GUILD_POINT_UPDATE        (5 * 60)    // 길드 랭킬용 경험치 저장 간격


#define DISABLE_REFINE_SPECIAL_STONE    // 고급 제련석 교환 불가능



///////////////////////// 2011 / 08 월 업데이트 /////////////////////////
#define DROP_MAKE_DOCUMENT										// 제작문서 드롭
#define MONSTER_COMBO_FIX										// 몬스터콤보 수정
#define CONTINENT_PARTY_RECALL									// 대륙별 파티리콜
#define NO_PERIOD_COMPOSITE										// 기간없는 합성 아이템
#define DYNAMIC_DUNGEON_BUG_FIX									// 다이나믹 던전 버그 수정
#define ATTACK_LIST_BUG_FIX										// 어택리스트 수정
#define RAIDBOX_DROP_COUNT_MODIFY								// 레이드 드롭 박스 개수 수정
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PackageII

#define NPC_PORTAL_SCROLL_DRATAN		// 드라탄 NPC포탈 스크롤


#define CHANGE_WARCASTLE_SETTING

#define DEV_EVENT_AUTO					// 국왕탄생일 이벤트
// PackageII
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PackageIII


#define HANARO_EVENT

#define GUILD_CONTRIBUTE_EXP_LIMIT		50				// 길두 기부 경험치를 max 50% 까지만 가능하도록 수정




///////////////////////// 버그수정 /////////////////////////


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

// Package 4

#define ITEM_PICK_ZONE_IP_LOG
//2013.05.20 던전 타임 적용
#define DUNGEON_TIME_SAVEFILE	".dungeontime"	// 던전타임 세팅 저장 파일.
#define  DISCONNECT_HACK_CHARACTER		// 핵 프로그램 유저 방출
///////////////////////// 버그수정 /////////////////////////
///////////////////////// 2011 / 08 월 업데이트 /////////////////////////

// -- LC_TLD

////// 운영자 레벨 제한 ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define     GM_LEVEL_ECHO               3
#define     GM_LEVEL_GO_ZONE            3
#define     GM_LEVEL_GOTO               3
#define     GM_LEVEL_ITEMDROP           10
#define     GM_LEVEL_COUNT_PC           8
#define     GM_LEVEL_LOOPCOUNT          3
#define     GM_LEVEL_IMMORTAL           8
#define     GM_LEVEL_SHUTDOWN           3
#define     GM_LEVEL_REBOOT             3
#define     GM_LEVEL_COUNT_NPC          8
#define     GM_LEVEL_VISIBLE            3
#define     GM_LEVEL_GO_PC              3
#define     GM_LEVEL_GO_NPC             8
#define     GM_LEVEL_LEVELUP            9               // 태국 9레벨로 변경 bw : 06526
#define     GM_LEVEL_KICK               3
#define     GM_LEVEL_COMON              3
#define     GM_LEVEL_SUMMON             9
#define     GM_LEVEL_SILENCE            3
#define     GM_LEVEL_WHERE_AM_I         3
#define     GM_LEVEL_QUEST_COMPLETE     8
#define     GM_LEVEL_STATPOINT          9
#define     GM_LEVEL_SKILLPOINT         8
#define     GM_LEVEL_SET_REGENSEC       8
#define     GM_LEVEL_NPC_REGEN          8
#define     GM_LEVEL_KILL_NPC           8
#define     GM_LEVEL_EXPUP              8
#define     GM_LEVEL_BLOODPOINT         8
#define     GM_LEVEL_ENDGAME            3
#define     GM_LEVEL_OBSERVE            8
#define     GM_LEVEL_CHATMONITOR        7
#define     GM_LEVEL_DOUBLE_EVENT       8
#define     GM_LEVEL_LATTO_EVENT        8
#define     GM_LEVEL_DOUBLE_EXP_EVENT   8
#define     GM_LEVEL_NEWYEAR_EVENT      8
#define     GM_LEVEL_VALENTINE_EVENT    8
#define     GM_LEVEL_WHITEDAY_EVENT     8
#define     GM_LEVEL_LETTER_EVENT       8
#define     GM_LEVEL_RELOAD_GMIP        8
#define     GM_LEVEL_SPEEDUP            3
#define     GM_LEVEL_FAMEUP             8
#define     GM_LEVEL_EVENT              8
#define     GM_LEVEL_PD4                8
#define     GM_LEVEL_KICK_ID            5
#define     GM_LEVEL_RESURRECTION       8
#define     GM_LEVEL_RECOMMEND          8
#define     GM_LEVEL_SETLIMIT           8
#define     GM_LEVEL_DISGUISE           8
#define     GM_LEVEL_LOGINSERVER        8
#define     GM_LEVEL_CALTIME            10
#define     GM_LEVEL_KICK_CHAR_INDEX    5
#define     GM_LEVEL_EVENTSHOW          5
#define     GM_LEVEL_EVENTSETTING       10
#define     GM_LEVEL_CHANCE_EVENT       9
#define     GM_LEVEL_GMCHATMONITOR      10
#define     GM_LEVEL_DOUBLE_ITEM_EVENT  8
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
#define		GM_LEVEL_RESET_RAID			8
#define		GM_LEVEL_SET_SEAL_EXP		10
#define		GM_LEVEL_END_SUBHELPER		10
#define		GM_LEVEL_LCBALL_RESET		10
#define		GM_LEVEL_ECHONPC			9
#define		GM_LEVEL_DT_EXPSET			10
#define     GM_LEVEL_ITEMGET			10
#define		GM_LEVEL_ITEMGETRANGE		10
#define		GM_LEVEL_JOBRESET			10
#define		GM_LEVEL_JOBCHANGE			10


#define SYSTEM_TREASURE_MAP
#define GM_TREASURE_MAP_SAVE_FILE		 ".treasuremapsave"
#define SYSTEM_TREASURE_MAP_LINKZONE_DROP						// 던전 보물 지도 드랍
#define SYSTEM_TREASURE_MAP_GAMIGO// 보물 찾기 시스템

#define ENABLE_TUTORIAL
#endif // LC_TLD
