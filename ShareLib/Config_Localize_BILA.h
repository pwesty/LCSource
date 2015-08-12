#define LC_BILA

#define JUNO_RENEWAL_MESSAGEBOX					// 독일용, 신규캐릭터 이동 시 쥬노로 갔다가 버뱅크로 간다.
#define SERVER_AUTHENTICATION		// 서버 기동 제한 시스템
#define GIVE_ITEM_IN_CHANGE_JOB		// 전직시 아이템 지급하기

////// 국가 필드 설정 //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (LC_BRZ)
#define ITEM_NAME_FIELD         "a_name_brz"
#define NPC_NAME_FIELD          "a_name_brz"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_brz"
#define EVENT_EXTRA_FILED       "a_extra_brz"
#elif defined (LC_MEX)
#define ITEM_NAME_FIELD         "a_name_mex"
#define NPC_NAME_FIELD          "a_name_mex"
#define ITEM_PRICE_FIELD        "a_price"
#define EVENT_ENABLE_FIELD      "a_enable_mex"
#define EVENT_EXTRA_FILED       "a_extra_mex"
#endif

////// IP 자동 제한 설정 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define IP_NATION_CHECK					// 로그인 서버, 접속한 유저가 어느 국가인지 확인하고 t_users에 기록함.

////// 해외 로컬 제한 설정 /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LC_MEX
#define INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS			// IP 제한
#endif // #ifdef LC_MEX

////// OX QUIZ 국가 코드 ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (LC_BRZ)
#define OX_QUIZ_COUNTRY_CODE    10
#elif defined (LC_MEX)
#define OX_QUIZ_COUNTRY_CODE	20
#endif

////// 상시 이벤트 /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define EVENT_TREASUREBOX               1	// 보물상자 이벤트
#define HANARO_EVENT		59				// [2010-05-19 T-Srv] 멕시코와 브라질이 틀리다.

#define DOUBLE_PET_EXP						// 펫 경험치 이벤트 자동화
#ifdef DOUBLE_PET_EXP
#define DOUBLE_PET_EXP_AUTO
#define DOUBLE_PET_EXP_SAVEFILE	".doublepetexp"
#endif

////// 업데이트 시스템 /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

#define XTRAP

#define DROP_MAKE_DOCUMENT
#define DURABILITY
#define PREMIUM_CHAR
#define NO_PERIOD_COMPOSITE						// 조합아이템 중 기간이 없는 아이템 추가
#define ENABLE_SUBJOB							// 상인

#define SETTING_IF_INNER_IP_NEW			// 외부 아이피와 내부 아이피가 나뉘면 사용 ...yhj

#define RECOMMEND_SERVER_POTION_INDEX   1393
#define RECOMMEND_SERVER_LEVEL          30

#define MAX_UPGRADE_PLUS    25          // 아이템 업그레이드 최대 플러스 수치
#define DISABLE_REFINE_SPECIAL_STONE    // 고급 제련석 교환 불가능

#define PARTY_QUEST_ITEM_BUG_			// 파티 퀘스트 아이템 지급이 되지 않던 문제
#define HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER	//파티원이 내 퀘스트 해주기(몹 잡아주면 카운트 되게 함. 기존엔 파티원도 동일한 퀘가 있어야 같이 되었음)


#define DEFAULT_NAS_PERCENT     50      // 나스 드롭확률 조절
#define DEFAULT_NAS_GET_PERCENT 100     // 나스 습득량 조절
#define DEFAULT_EXP_PERCENT     100     // 경험치 획득량 조절
#define DEFAULT_SP_PERCENT      100     // sp 획득량 조절
#define DEFAULT_PRO_PERCENT     80      // 생산 습득 확률 조절
#define DEFAULT_PRO_GET_NUMBER  1       // 생산 습득 갯수


#ifdef BILL_SERVER_VERSION              // 브라질 테스트용
#undef BILL_SERVER_VERSION
#define BILL_SERVER_VERSION "TEnterBillV1.0"
//#define USE_TENTER_BILLING
#endif

#define NEW_ACCERY_ADD              // 새로운 악세사리 4종

#define ENABLE_ROGUE_SKILL125_BRZ

#define RAID_NPC_RAGE_SKILL             394     // 레이드 몹 분노 스킬 인덱스
#define RAID_NPC_RAGE_SKILL_LEVEL       2       // 레이드 몹 분노 스킬 레벨
#define RAID_NPC_NOTICE_CELL_EXT        5
#define MAX_RARE_OPTION_SETTING         10      // 레어 옵션당 옵션 세팅 수
#define MAX_RARE_OPTION_ITEM            10      // 아이템당 레어 옵션 수
#define PULSE_MONSTER_RAID_MOVE     (14 * PULSE_REAL_MIN)   // 몬스터 레이드 이동 시간


#define PET_TURNTO_NPC_ITEM
#define EVENT_DROPITEM      // 브라질 요청 운영자 명령어

#define DEFAULT_LIMIT_EXP 250

#define BRZ_FAIL_PASSWORD

#define PULSE_GUILD_POINT_UPDATE        (5 * 60)    // 길드 랭킬용 경험치 저장 간격


#define EVENT_PCBANG_2ND




//#define DISCONNECT_HACK_CHARACTER     // 핵 프로그램 유저 방출



#define MONSTER_AI                      // 몬스터 A.I
#define EXTREME_CUBE                    // 익스트림 큐브
#define	ENDGAME_COUNTDOWN_ADD			// 운영자 명령어 endgame시 10초 남기고 카운트

// UPDATE14
//#define RESTRICT_PVP_SKILL         3			// [2010/11/02] 요청에 의한 기능 삭제  // 엄격한 스킬 규칙 : 몹에 타겟팅 되어 있을 경우, PK모드, 정당방위 모드 인비/데스모션 불가능
// 암흑의 제단에서 인비저빌리티가 안써지려면 있어야 하는 define



//----- BRZ - MEX COMMON SETTING :: START ------//



//	## 2010-10-25

#define	_COMPOSITE_ITEM_UPGRADE_BUG_FIX // 2010-04-06 결합아이템 업그레이드 버그 패치

// ##### LiveSrv : 2010-10-14 ( Test : 2010-10-11 )
#define EGEHA_ZONE_PAYMENT		     500000		// 말레이시아 에게하 요금 500,000골드 지급

// ##### LiveSrv : 2010-11-9/10 (Test : 2010-11-02 )
#define FREE_PK_SYSTEM	// 요청에 의한 추가

// ##### update 1005-1,2 :: LiveSrv 2010-10-14 ( TestSrv 2010-09-20 ) // 브라질 적용 일
// ##### update 1005-1,2 :: LiveSrv 2010-11-23 ( TestSrv 2010-11-11 ) // 멕시코 적용 일
#define LACARETTE_SYSTEM						// 라카렛

#define DUNGEON_TIME_SAVEFILE	".dungeontime"	// 던전타임 세팅 저장 파일.

#define LOW_LEVEL_SUPPORT_QUEST					// 초보지원 퀘스트

// ##### LiveSrv 2010-11-23 ( TestSrv 2010-11-11
#define CHECK_CASTLE_AREA						// 공성지역 체크 안되는 버그 수정

// ##### LiveSrv 2010-11-10 ( TestSrv 2010-10-29 ) // 브라질
// ##### LiveSrv 2010-12-24 ( TestSrv 2010-11-22 ) // 멕시코

// ##### LiveSrv 2010-12-21 ( TestSrv 2010-11-22 ) :: 1006, 1007, 1008  멕시코
// ##### LiveSrv 2010-12-22 ( TestSrv 2010-11-23 ) :: 1006, 1007, 1008  브라질

///// BRZ 110217-110222
///// MEX 110216-110228
#define DONT_USE_ATTACK_ACTION			// 빌라요청으로 퀵 슬롯 안의 공격 액션은 무조건 뺀다. 공격 액션을 사용못하도록 막는다.

///// BRZ 110224-110330
///// MEX 110224-110308
#ifdef IP_NATION_CHECK
#define LOCAL_LOGIN_LOG				// 유저 국가별 로그 DB에 남기기
#endif

///// BRZ 110224-110330
///// MEX 110224-110308


#define UPGRADE_EVENT_AUTO			// 업그레이드 이벤트 자동화
#define UPGRADE_EVENT_AUTO_SAVEFILE		".upgradeevent"




///// BRZ 110406-110412
///// MEX 110407-110413

///// BRZ 110414-110420
///// MEX 110414-110419
//#define EVENT_EGGS_HUNT_BILA			// 빌라는 부활절 달걀을 여러개 소유할 수 있다.

///// BRZ 110421-110427
///// MEX 110420-110426

//// BRZ 110502-110518
//// MEX 110502-110517
//// 미니패키지

#define WARCASTLE_MOVE_MESSAGE_LIMIT	// 공성전 이동 메시지 팝업창 제한

//// BRZ 110519-110525
//// MEX 110518-110524
#define GM_COMMAND_NOTICE				// GM COMMAND kick, Silence 사용 시, 공지로 알림

//// BRZ 110526-110601
//// MEX 110525-110531

//// MEX 110610-110614
//// BRZ 110610-110615

//// MEX 110617-110621
//// BRZ 110617-110622
#define BUGFIX_REMOVE_SPACE				// config 파일 로드 시, 공백 문자 제거 후 설정

//// MEX 110622-110712
//// BRZ 110623-110713
//// UPDATE 1104 - 1105
#ifdef EXTREME_CUBE
#define EXTREME_CUBE_VER2									// 큐브2
#endif // EXTREME_CUBE
#define SYSTEM_MONSTER_MERCENARY_CARD_EX
#define SYSTEM_TREASURE_MAP									// 보물 찾기 시스템
#define SYSTEM_TREASURE_MAP_GAMIGO
#define GM_TREASURE_MAP_SAVE_FILE    ".treasuremapsave"
#define SYSTEM_TREASURE_MAP_LINKZONE_DROP                     // 던전 보물 지도 드랍
#define DEV_LETS_PARTYTIME										// 파티형 아이템


//// BUG_FIX
#define BUGFIX_REFINE_SPECIAL_ITEM								// 128레벨 아이템 변환시 100% 고급제련석으로 주는 버그 수정. 방어구와 똑같이 처리하도록 수정.


// ================================================================================================================
//// UPDATE 1106 (MEX : 111109-111122)(BRZ : 111110-111130)


#define REFORM_PK_PENALTY_201108



#define BILA_RECOMMEND_SERVER_REWARD_CHANGE			// ITS : 6364 빌라 추천 서버 이벤트 보상 변경 ([2846]카오스 볼 10개)

#define INTERGRATION_SERVER
#define BILA_INTERGRATION_SERVER					// 멕시코, 브라질 통합 서버
#define INTERGRATION_BRAZIL		10
#define INTERGRATION_MEXICO		20

#define IMP_SPEED_SERVER							// ITS : 6502, 6503, 6505 스피드 서버

#define BILA_DUNGEON_TIME_EXP_CHANGE				// ITS : 6504 던전 타임 존 추가 (29, 30)

//// week02_120125
#define NEW_DOUBLE_GM_AUTO

#define NEW_DOUBLE_EVENT_AUTO

#ifdef NEW_DOUBLE_EVENT_AUTO
#define NEW_DOUBLE_EVENT_AUTO_TIME
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
#define NEW_DOUBLE_EVENT_AUTO_SAVEFILE ".doubleeventautotime"
#endif
#endif

#define EVENT_ITEMDROP_AUTO
#ifdef EVENT_ITEMDROP_AUTO
#define EVENT_ITEMDROP_AUTO_SAVEFILE ".eventitemdropauto"
#endif

#define GM_EXP_LIMIT_AUTO
#ifdef GM_EXP_LIMIT_AUTO
#define GM_EXP_LIMIT_AUTO_SAVEFILE ".explimitsave"
#endif
#define CASH_SHOP_NO_GIFT_BILA

//////// UPDATE 121Q (12년 1월 3일 ) ////////

#define DEV_GUILD_MARK		// 길드 마크
#define DEV_GUILD_STASH		// 길드 창고

#define NOTICE_SYSTEM		// 공지 시스템

#define GUILD_REMOTE_INVITE		// 길드 원격 가입 시스템

#define EX_ROGUE	// EX 로그 캐릭터
#define ASSIST_DECREASE_SKILL_MP
#define ASSIST_REDUCE_SKILL
#define SKILL_TYPE_NO_COOL_TIME
#define EX_MAGE
// ================================================================================================================

//------ BRZ - MEX COMMON SETTING :: END ------//

//------ Test define :: start -----//

#define MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING	// 메라크 공성 신청 길드 레벨 GM 명령어

//------ Test define :: end   -----//

//------ Brazil define (브라질 전용) :: start   -----//

#ifdef LC_BRZ

#endif //LC_BRZ

//------ Brazil define (브라질 전용) :: end   -----//

//------ Mexico define (멕시코 전용) :: start   -----//

#ifdef LC_MEX

#endif //LC_MEX

//------ Mexico define (멕시코 전용) :: end   -----//

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

#ifdef GM_COMMAND_NOTICE
#define     GM_LEVEL_KICK               1
#else
#define     GM_LEVEL_KICK               5
#endif	// GM_COMMAND_NOTICE

#define     GM_LEVEL_COMON              8
#define     GM_LEVEL_SUMMON             8

#ifdef GM_COMMAND_NOTICE
#define     GM_LEVEL_SILENCE            1
#else
#define     GM_LEVEL_SILENCE            5
#endif	// GM_COMMAND_NOTICE

#define     GM_LEVEL_WHERE_AM_I         3
#define     GM_LEVEL_QUEST_COMPLETE     7
#define     GM_LEVEL_STATPOINT          7
#define     GM_LEVEL_SKILLPOINT         7
#define     GM_LEVEL_SET_REGENSEC       8
#define     GM_LEVEL_NPC_REGEN          8
#ifdef GM_COMMAND_NOTICE
#define     GM_LEVEL_KILL_NPC           2
#else
#define     GM_LEVEL_KILL_NPC           1
#endif
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
#ifdef GM_COMMAND_NOTICE
#define		GM_LEVEL_DATE				2
#else
#define		GM_LEVEL_DATE				1
#endif
#define		GM_LEVEL_PLAY_TRIGGER		10
#define		GM_LEVEL_BATTLE_START		10
#define		GM_LEVEL_BATTLE_STOP		10
#define		GM_LEVEL_RESET_RAID			10
#define		GM_LEVEL_SET_SEAL_EXP		10
#define		GM_LEVEL_END_SUBHELPER		10
#define		GM_LEVEL_LCBALL_RESET		10
#define		GM_LEVEL_ECHONPC			9
#define		GM_LEVEL_DT_EXPSET			9
#define     GM_LEVEL_ITEMGET			10
#define		GM_LEVEL_ITEMGETRANGE		10
#define		GM_LEVEL_JOBRESET			10
#define		GM_LEVEL_JOBCHANGE			10
