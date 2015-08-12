#ifndef __GM_COMMAND_H__
#define __GM_COMMAND_H__

// 운영자 명령어 설정
#define		GM_CMD_HPMP				"hpmp"
#define		GM_CMD_HP				"hp"
#define		GM_CMD_MP				"mp"

#define		GM_CMD_GUILD_POINT		"guildpoint"
#define		GM_CMD_EVENT_COLLECT	"event_collect"
#define		GM_CMD_BLOCK			"block"
#define		GM_CMD_RELEASE			"release"
#define		GM_CMD_COUPON			"coupon"
#define		GM_CMD_PROMOTION		"promotion"
#define		GM_CMD_SETWARTIME		"setwartime"
#define		GM_CMD_SETWARTIME2		"setwartime2"
#define		GM_CMD_SETWARTIMEEND	"setwartimeend"
#define		GM_CMD_SETWARTIME2END	"setwartime2end"
#define		GM_CMD_LOADHP5			"loadhp5"
#define		GM_CMD_GUILDCUBESTART	"guildcubestart"
#define		GM_CMD_GUILDCUBEEND		"guildcubeend"
#define		GM_CMD_GUILDCUBESTATE	"guildcubestate"
#define		GM_CMD_GUILDCUBETIMECHECK	"guildcubetimecheck"
#define		GM_CMD_SETOWNER			"setowner"
#define		GM_CMD_LORDCHAT			"lordchat"
#define		GM_CMD_CREATEPET		"create_pet"
#define		GM_CMD_APETSTAT			"apet_stat"
#define		GM_CMD_APETHPMP			"apet_hpmp"
#define		GM_CMD_APETEXP			"apet_exp"
#define		GM_CMD_APETLEVELUP		"apet_levelup"
#define		GM_CMD_APETUSE			"apet_use"
#define		GM_CMD_APETCOOLTIME		"apet_cooltime"
#define		GM_CMD_APETACCEXP		"apet_accexp"
#define		GM_CMD_SVRINFO			"svrinfo"
#define		GM_CMD_EP				"ep"
#define		GM_CMD_RANKREFRESH0831	"rankrefresh0831"
#define		GM_CMD_PETLEVELUP		"petlevelup"
#define		GM_CMD_PETDEADTIME		"petdeadtime"
#define		GM_CMD_PETRESET			"petreset"
#define		GM_CMD_PETEXPUP			"petexpup"
#define		GM_CMD_PETMOUNTRESET	"petmountreset"
#define		GM_CMD_PETCHANGENAME	"petchangename"
#define		GM_CMD_PKCOUNT			"pkcount"
#define		GM_CMD_PKPENALTY		"pkpenalty"
#define		GM_CMD_SHOWHP			"showhp"
#define		GM_CMD_GOPOS			"gopos"
#define		GM_CMD_PDNPC			"pd_npc"
#define		GM_CMD_CASHBALANCE		"cashbalance"
#define		GM_CMD_CASHPURCHASE		"cashpurchase"
#define		GM_CMD_CASHPURCHASELIST	"cashpurchaselist"
#define		GM_CMD_CASHPURCHASEHISTORY	"cashpurchasehistory"

// echo <zone | server | all / subserver> <chat>
#define     GM_CMD_ECHO             "echo"          // 운영자 알림 메시지
#define     GM_CMD_ECHO_ZONE        "zone"          // 존 알림 메시지
#define     GM_CMD_ECHO_SERVER      "server"        // 서버군 알림 메시지
#define     GM_CMD_ECHO_ALL         "all"           // 모든 서버 알림 메시지
#define     GM_CMD_ECHO_SUBSERVER   "subserver"     // 서버군 서브서버 알림 메시지
#define     GM_CMD_ECHO_ALLZONE     "allzone"       // 모든 서버군의 존에 알림
// go_zone <zone> <extra>
#define     GM_CMD_GO_ZONE          "go_zone"       // 존 이동
// goto <x> <z> <y>
#define     GM_CMD_GOTO             "goto"          // 장소 이동
// itemdrop <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
#define     GM_CMD_ITEMDROP         "itemdrop"      // 아이템 강제 드롭
// count_pc
#define     GM_CMD_COUNT_PC         "count_pcc"     // 사용자 수 조사
// loopcount
#define     GM_CMD_LOOPCOUNT        "loopcount"     // 루프 카운트 조사
// immortal
#define     GM_CMD_IMMORTAL         "immortal"      // 몹에게 공격 당하지 않음: 토글
// shutdown <time:sec> <msg>
#define     GM_CMD_SHUTDOWN         "shutdown"      // 셧다운: 자동 재실행 없음(서버군 다운)
// reboot <all | server | subserver> <time:sec> <msg>
#define     GM_CMD_REBOOT           "reboot"        // 리부트: 자동 재실행 함
// count_npc
#define     GM_CMD_COUNT_NPC        "count_npc"     // npc 수 조사
// visible
#define     GM_CMD_VISIBLE          "visible"       // 투명/불투명 설정
// go_pc <char name>
#define     GM_CMD_GO_PC            "go_pc"         // 지정한 캐릭터 옆으로 이동
// go_npc <name or index>
#define     GM_CMD_GO_NPC           "go_npc"        // 지정한 몹 옆으로 이동
// levelup <level>
#define     GM_CMD_LEVELUP          "levelup"       // 지정한 레벨로 변경
// kick <char name>
#define     GM_CMD_KICK             "kick"          // 캐릭터를 서버로부터 튕겨냄
// comon <char name>
#define     GM_CMD_COMON            "common"        // 캐릭터를 운영자 캐릭터 앞으로
// summon <name or index>
#define     GM_CMD_SUMMON           "summon"        // 지정한 몹 운영자 캐릭터 앞으로 소환
// silence <char name> <time:sec>
#define     GM_CMD_SILENCE          "silence"       // 캐릭터에게 시간동안 채팅 금지
// whereami
#define     GM_CMD_WHERE_AM_I       "whereami"      // 캐릭터의 zone, area 번호
// QuestComplete <QuestTypeNum>
#define     GM_CMD_QUEST_COMPLETE   "questcomplete" // 퀘스트 강제 완료
// statpoint <remain> [<str> <dex> <int> <con>]
#define     GM_CMD_STATPOINT        "statpoint"     // 스탯 포인트 강제 설정
// skillpoint <count>
#define     GM_CMD_SKILLPOINT       "skillpoint"    // 스킬 포인트 강제 설정
// set_regensec <index> <time:1/10sec>
#define     GM_CMD_SET_REGENSEC     "set_regensec"  // 해당 npc regensec 조정
// npc_regen <index>
#define     GM_CMD_NPC_REGEN        "npc_regen"     // 해당 npc 중 리젠 되야 하는것들 모두 강제 리젠
// kill_npc <meter :m>
#define     GM_CMD_KILL_NPC         "kill_npc"      // 운영자와 거리안에 있는 npc들 모두 강제 죽임
// expup <exp%>
#define     GM_CMD_EXPUP            "expup"         // 캐릭터 경험치 변경
// bloodpoint <bloodpoint>
#define     GM_CMD_BLOODPOINT       "bloodpoint"    // 캐릭터 현재 Blood Point 변경
// endgame <time:sec> <msg>
#define     GM_CMD_ENDGAME          "endgame"       // 메신저를 제외한 모든 서버 다운
// observe
#define     GM_CMD_OBSERVE          "observe"       // 관찰자 설정
// chatmonitor <character name>
#define     GM_CMD_CHATMONITOR      "chatmonitor"   // 채팅 감시
// double_event <"start" or "stop" or "ing">
#define     GM_CMD_DOUBLE_EVENT     "double_event"  // 경험치, 나스, SP, 생산품 2배 이벤트
// latto_event <"start" or "stop" or "ing">
#define     GM_CMD_LATTO_EVENT      "latto_event"   // 라또 이벤트
// doubleexp_event <serverno | all | cur> <start | stop | ing> <0~200%>
#define     GM_CMD_DOUBLE_EXP_EVENT     "doubleexp_event"   // 경험치 2배 이벤트(퍼센티지에 따라 바꾸는 것으로 수정)
// newyear_event <"start" or "stop" or "ing">
#define     GM_CMD_NEWYEAR_EVENT        "newyear_event" // 설날 이벤트
// valentine_event <"start" or "stop" or "ing">
#define     GM_CMD_VALENTINE_EVENT      "valentine_event"   // 발렌타인데이 이벤트
// whiteday_event <"start" or "stop" or "ing">
#define     GM_CMD_WHITEDAY_EVENT       "whiteday_event"    // 화이트데이 이벤트
// letter_event <"start" or "stop" or "ing">
#define     GM_CMD_LETTER_EVENT     "letter_event"  // 낱말 맞추기 이벤트
// reloadgmip
#define     GM_CMD_RELOAD_GMIP      "reloadgmip"    // GM IP 설정 테이블 다시 읽기
// speedup
#define     GM_CMD_SPEEDUP          "speedup"       // 빨리 달리기
// fameup
#define     GM_CMD_FAMEUP           "fameup"        // 명성치 조정
// event <name> <start | stop | ing>
#define     GM_CMD_EVENT            "event"         // 이벤트 통합
// pd4 <rankinit | rewardinit | rewardend>
#define     GM_CMD_PD4              "pd4"           // 차원의문 랭크
// kickid <id>
#define     GM_CMD_KICK_ID          "kickid"        // id로 강제 접속 해제
// resurrection <name>
#define     GM_CMD_RESURRECTION     "resurrection"  // 해당 케릭터 부활
// recommend <server no>
#define     GM_CMD_RECOMMEND        "recommend"     // 해당 서버군 추천 설정, -1이면 모두 추천 아닌 것으로
// setlimit  <ctid> <sellNo>
#define     GM_CMD_SETLIMIT         "setlimit"      // 해당 서버군의 판매상품 설정, sellNo 더 판매할 숫자. 캐쉬툴에 설정된 리미트보다 많을수는 없음
// disguise
#define     GM_CMD_DISGUISE         "disguise"      // 해당 서버군의 판매상품 설정, sellNo 더 판매할 숫자. 캐쉬툴에 설정된 리미트보다 많을수는 없음
// loginserver <open | close | reboot>
#define     GM_CMD_LOGINSERVER      "loginserver"   // 로그인 서버 제어
// caltime
#define     GM_CMD_CALTIME          "caltime"       // 루프 시간 조사
// kickchar <charindex>
#define     GM_CMD_KICK_CHAR_INDEX          "kickcharidx"       // id로 강제 접속 해제

#define     GM_GMD_EVENTSHOW                "eventshow"         // 구라 이벤트 공지 이름, 아이템인덱스

#define     GM_CMD_EVENTSETTING     "eventsetting"  // 이벤트 세팅
#define     GM_CMD_CHANCE_EVENT     "chance_event"  // 일정레벨 유저의 성장 촉진
#define     GM_CMD_GMCHATMONITOR    "gmchatmonitor"

// doubleitem_event <zone | server | all / subserver> <chat>
#define     GM_CMD_DOUBLE_ITEM_DROP             "doubleitem_event"          // 운영자 알림 메시지

// doublepetexp_event <serverno | all | cur> <start | stop | ing> <0~200%>
#define     GM_CMD_DOUBLE_PET_EXP   "doublepetexp_event"    // 펫 경험치 2배
// doubleattack_event <serverno | all | cur> <start | stop | ing> <0~200%>
#define     GM_CMD_DOUBLE_ATTACK    "doubleattack_event"    // 물리, 마법 공격력 2배

// event_dropitem <serverno | all | cur> <start | stop | ing> <npcidx> <itemidx> <prob>
#define     GM_CMD_EVENT_DROPITEM   "event_dropitem"

// kick_multi   <meter :m>
#define     GM_CMD_KICK_PC_MULTI    "kick_pc"

// silence_multi <meter :m>  <sec>
#define     GM_CMD_SILENCE_PC_MULTI "silence_pc"

// set exp limit
#define     GM_CMD_SET_EXPLIMIIT    "exp_limit"

#define     GM_CMD_DOUBLE_EVENT_AUTO "double_event_auto"

// upgrade_event <all | cur | serverno> <start | ing | stop> <100 | 125 | 150 | 175 | 200>
#define     GM_CMD_UPGRADE_EVENT    "upgrade_event"

// kickguildmember <guildname> <charname>
#define     GM_CMD_KICK_GUILDMEMBER "kickguildmember"

// applyskill <skill index> <char name>
#define     GM_CMD_APPLY_SKILL "applyskill"

// stateflag <state flag>
#define     GM_CMD_STATE_FLAG "stateflag"

// date
#define		GM_CMD_DATE	"date"

// playtrigger <count> <playID> <playID> <playID> ...
#define     GM_CMD_PLAY_TRIGGER "playtrigger"

// guildbattlestart <guildindex1> <guildindex2> <time>
#define		GM_CMD_BATTLE_START "guildbattlestart"

// guildbattlestop <guildindex1> <guildindex2>
#define		GM_CMD_BATTLE_STOP	"guildbattleend"

// resetraid
#define     GM_CMD_RESET_RAID "resetraid"

// setsealexp <level>
#define		GM_CMD_SET_SEAL_EXP	"setsealexp"

// endsubhelper // 서브헬퍼만 종료
#define		GM_CMD_END_SUBHELPER "endsubhelper"

// date
#define		GM_CMD_MY_ADMIN	"myadmin"

#define		GM_CMD_DT_TIMESET		"dt_timeset"
#define		GM_CMD_DT_EXPSET		"dt_expset"
#define		GM_CMD_DT_TIMEINTERVAL	"dt_timeinterval"
#define		GM_CMD_DT_TOGGLE		"dt_toggle"

#define     GM_CMD_RELOAD_CATALOG   "reload_catalog"
//#define		GM_CMD_RAID_TODO		"raidtodo"
#define		GM_CMD_NPC_DROP			"npcdrop"

#define     GM_CMD_GODBLESS         "godbless"
#define		GM_CMD_ENCHANT			"enchant"
#define		GM_CMD_RELOAD_PRICE		"reload_price"
#define		GM_CMD_MODIFY_CASHSHOP	"modifycashshop"
#define		GM_CMD_LCBALL_RESET		"lcball_reset"
#define		GM_CMD_ATTEVENT_2011	"attevent"
#define		GM_CMD_FUNC				"func"
#define		GM_CMD_ECHONPC			"echo_npc"
#define		GM_CMD_CASTLE_STATE_CYNC	"warcastlestatecync" // 공성상태정보를 다른 채널 싱크하는 명령어
#define		GM_CMD_TREASURE_MAP_DROP	"treasuremapdrop"
#define		GM_CMD_DEATH_EVENT		"deathevent"
#define		GM_CMD_ATTENDANCE_CHANGE "attendance_change"
#define		GM_CMD_SET				"set"
#define		GM_CMD_CRAFTPOINT		"craftpoint"
#define		GM_CMD_EVENTAOUTOMATON	"automationevent"
#define		GM_CMD_SKILLLEARN		"skilllearn"
#define		GM_CMD_APET_HUNGRY		"apet_hungry"
#define		GM_CMD_APET_FAITH		"apet_faith"
#define		GM_CMD_COMPARE_BOXITEM	"compare_boxitem"
#define		GM_CMD_GUILDMASTER_KICK_RESET "kickreset"
#define		GM_CMD_GUILDMASTER_CHANGE	"changeboss"
// itemget <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
#define		GM_CMD_JOBRESET			"jobreset"
// jobchange <jobtype> <2ndjobtype>
#define		GM_CMD_JOBCHANGE		"jobchange"
#define     GM_CMD_ITEMGET			"itemget"      // 아이템 강제 인벤 생성
// itemget <indexfrom> <indexto> <plus> <flag> <count> <option_count> <option_type> <option_level>
#define		GM_CMD_ITEMGETRANGE		"itemgetrange"	// 인덱스 범위 아이템 강제 인벤 생성
#define		GM_CMD_SERVER_TRANS_BLOCK "trans_block" // 서버 이전 불가능 서버 지정
#define		GM_CMD_XMAS_PUZZLE		"xmaspuzzle"
#define		GM_CMD_ROYAL_RUMBLE_SETTING "royalrumble"
#define		GM_CMD_AFFINITY_SET				"affinity_set"			//친화도 카운트 수정
#define		GM_CMD_AFFINITY_REWARD_INIT		"affinity_reward_init"	//친화도 보상 초기화z
#define		GM_CMD_NAS_SET					"nas_set"				//나스 세팅
#define		GM_CMD_JEWELRYE					"jewelrye"				//보석 관련 이벤트값 조정
#define		GM_CMD_CRASH					"scrash"				// 테스트를 위해 서버를 강제로 크래쉬시킴
#define		GM_CMD_NOT_GM					"notgm"					// 순간적으로 GM이 아닌 상태로 만든다. 다시 GM이 되려면 재 접속
#define		GM_CMD_TRADEAGENT_FEE			"fee"					// 거래대행 수수료
#define		GM_CMD_TRADEAGENT_DEPOSIT		"deposit"				// 거래대행 보증금
#define		GM_CMD_GO_ZONE_FINDNPC			"go_zone_npc"			// npc가 존재하는 존으로 이동
#define		GM_CMD_STASHGETRANGE			"stashgetrange"			// 창고 채우기
#define		GM_CMD_STASHDEL					"stashdel"				// 창고 비우기
#define		GM_CMD_TIMERITEM_SET			"timeritem_set"			// 기간제 아이템의 시간을 변경
#define		GM_CMD_PICKUP_ITEM				"pickupitem"			// 캐릭터 주변의 아이템을 자동으로 줍기
#define		GM_CMD_MONSTER_MERCEARY			"monstercard"
#define		GM_CMD_VIEW_ALL_SHOP_LIST		"sshoplist"				// NPC가 들고있는 판매 목록을 보여줌
#define		GM_CMD_DEL_CHARACTER			"delchar"				// 자신 계정의 다른 캐릭터를 바로 지워준다.
#define		GM_CMD_LOGOUT					"logout"				// 캐릭터 선택창으로 이동
#define		GM_CMD_EXP						"exp"					// 현재 exp를 변경한다.
#define		GM_CMD_GO_NEXT_PC				"go_nextpc"				// 같은 존에 존재하는 다른 캐릭터에게 랜덤하게 이동 (가미고 요청)
#define		GM_CMD_VIRTUAL_REFORM			"vreform"				// 가상으로 리폼을 진행함
#define		GM_CMD_ROCK_PAPER_SCISSORS_WIN	"rpswin"				// 가위바위보 게임에서 무조건 이기게 함

#define		GM_CMD_RVR_INCREASE_POINT		"rvr_inc"				// 기여도 증가
#define		GM_CMD_RVR_DECREASE_POINT		"rvr_dec"				// 기여도 감소

#define		GM_CMD_THEOS_CHECK_REWARD_ITEM	"timeset_theos"			// 보상 체크 시간 설정(단위 시간)
#define		GM_CMD_THEOS_CHECK_POLLUTION	"theos_pollution"		// 무덤 쾌적함 상태 변경
#define		GM_CMD_THEOS_CHECK_DIFFICULTY	"theos_difficulty"		// 무덤 온순함 상태 변경

#ifdef DURABILITY
#define		GM_CMD_DURABILITY				"dur"					// 장비 내구도 : tab_no, invenIndex, now_dur, max_dur
#endif

#define		GM_CMD_RVR_APPLYSKILL			"rvr_applyskill"		// 1시간마다 걸리는 버프 바로 주기
#define		GM_CMD_RVR_SHOWJEWELPOINT		"rvr_show"				// 1시간마다 걸리는 버프 바로 주기
#define		GM_CMD_RVR_SET_USERCOUNT		"rvr_setuser"			// 세력 가입자수 강제로 세팅
#define		GM_CMD_RVR_SHOW_USERCOUNT		"rvr_showuser"			// 유저카운트 조회

#define		GM_CMD_ALL_ITEM_TIME_SET		"itemused_set"			// 인벤토리에 있는 모든 아이템의 used 값을 바꾸어 준다.
#define		GM_CMD_GET_EXCHANGE_ITEM		"exchangeitem"			// 교환에 필요한 아이템을 한번에 얻기
#define		GM_CMD_NO_SPECIAL_BOX_COOLTIME	"nocoolbox"				// 스페셜 박스 아이템 줍기 쿨타임 제거

#ifdef PREMIUM_CHAR
#define		GM_CMD_PRIMIUM_CHAR_RESET				"pc_reset"				// 프리미엄 캐릭터 초기화
#define		GM_CMD_PRIMIUM_CHAR_SET_EXPIRE_TIME		"pc_expiretime"			// 프리미엄 캐릭터 유효 시간 조정
#define		GM_CMD_PRIMIUM_CHAR_SET_JUMP_COUNT		"pc_jumpcount"			// 캐릭터의 점핑 수를 조정
#define		GM_CMD_PRIMIUM_CHAR_SET_TYPE			"pc_type"				// 프리미엄 타입 변경
#endif

#define		GM_CMD_ATTENDANCE_ASSURE_EXPIRE_TIME	"at_expiretime"			// 출석 유지보험 남은 시간 조정
#define		GM_CMD_RAID_CHANGE_MAX_AREA_COUNT		"change_area_max"		// area MaxCount 조정
#define		GM_CMD_POPUP_NOTICE						"popup"
#define		GM_CMD_POPUP_NOTICEOFF					"popupoff"


#define		GM_CMD_TEST_COMMAND						"test"					// 테스트 명령어

// 전 로컬 공통 운영자 명령 제한 레벨
#define		GM_LEVEL_MY_ADMIN			1
#define		GM_LEVEL_DT_TIMESET			10
#define		GM_LEVEL_DT_TIMEINTERVAL	10
#define		GM_LEVEL_DT_TOGGLE			10
#define     GM_LEVEL_RELOAD_CATALOG     10
//#define		GM_LEVEL_RAID_TODO			10
#define		GM_LEVEL_NPC_DROP			10
#define     GM_LEVEL_GODBLESS           10
#define		GM_LEVEL_ENCHANT			10
#define		GM_LEVEL_MODIFY_CASHSHOP	10
#define		GM_LEVEL_RAIDINZONE			10
#define		GM_LEVEL_ATTEVENT_2011		10
#define		GM_LEVEL_FUNC				10
#define		GM_LEVEL_CASTLE_STATE_CYNC	10
#define		GM_LEVEL_TREASURE_MAP_DROP	10
#define		GM_LEVEL_DEATH_EVENT		10
#define		GM_LEVEL_ATTENDANCE_CHANGE	7
#define		GM_LEVEL_SET				10
#define		GM_LEVEL_CRAFTPOINT			10
#define		GM_LEVEL_EVENTAOUTOMATON	10
#define		GM_LEVEL_SKILLLEARN			10
#define		GM_LEVEL_APET_HUNGRY		10
#define		GM_LEVEL_APET_FAITH			10
#define		GM_LEVEL_COMPARE_BOXITEM	10
#define		GM_LEVEL_GUILD_MASTER_KICK_RESET	10
#define		GM_LEVEL_GUILD_MASTER_CHANGE		10
#define		GM_LEVEL_TRANS_BLOCK		10
#define		GM_LEVEL_XMAS_PUZZLE		10
#define		GM_LEVEL_ROYAL_RUMBLE_SETTING 10
#define		GM_LEVEL_AFFINITY_SET			10
#define		GM_LEVEL_AFFINITY_REWARD_INIT	10
#define		GM_LEVEL_NAS_SET				10
#define		GM_LEVEL_JEWEL_EVENT			10
#define		GM_LEVEL_CRASH					10
#define		GM_LEVEL_NOT_GM					10
#define		GM_LEVEL_TRADEAGENT_FEE					10
#define		GM_LEVEL_TRADEAGENT_DEPOSIT				10
#define		GM_LEVEL_TOGGLE_BUFF			10


//팝업 공지 서브 명령
#define		GM_POPUP_NOTICE_ALL			"all"
#define		GM_POPUP_NOTICE_SERVER		"servernumber"
#define		GM_POPUP_NOTICE_SUB_SERVER	"subserver"

// Some serious shit going on here
// autobuffs <switch>
#define GM_CMD_TOGGLE_BUFF "autobuffs"
#endif
