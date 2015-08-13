#ifndef __MSG_TYPE_H__
#define __MSG_TYPE_H__

// 상위 2비트는 압축 방법에 대한 설정으로 무조건 0
// 하위 6비트는 아래의 메시지 타입으로
// 메시지 추가시에는 뒤쪽에 추가
// 데이터 타입
//		unsigned	u
//		char		c
//		int			n
//		long		l
//		short		s
//		LONGLONG	ll
//		float		f
//		double		d
//		string		str
typedef enum _tagSummonNpcType {SUMMON_NPC_TYPE_MERCENARY, SUMMON_NPC_TYPE_TOTEM, SUMMON_NPC_TYPE_BOMB_1, SUMMON_NPC_TYPE_BOMB_2, SUMMON_NPC_TYPE_BOMB_3};
typedef enum __tagMessageType
{
	MSG_UNKNOWN		= 0,		// unknown

	// 서버 메시지
	MSG_FAIL,					// 오류 발생 알림	: errcode(uc)
	MSG_DB,						// DB 관련			: errcode(uc) ...


	// LOGIN 과정
	MSG_LOGIN,					// LOGIN			: version(n) mode(uc) id(str) pw(str) nation(c) seed(n)
	MSG_MENU,					// MENU 화면		: mode(uc) ...
	MSG_START_GAME,				// 게임 시작		:


	// 캐릭터 정보
	MSG_STATUS,					// 캐릭터 정보		: level(n) exp(ll) need_exp(ll) hp(n) maxhp(n) mp(n) maxmp(n) str(n) dex(n) int(n) con(n) opstr(n) opdex(n) opint(n) opcon(n) attack(n) magic(n) defense(n) resist(n) skillpoint(n) weight(n) maxweight(n) walkspeed(f) runspeed(f) attackspeed(c) magicspeed(c) pktitle(c) pkpenalty(n) pkcount(n) fame(n) attackrange(f) join_flag_merac(c) skillspeed(n) attrmap(uc) mountPet(c) evocationType(c) evocationRemain(n)

	MSG_APPEAR,					// 나타남			: PC : isNew(uc) type(c) index(n) name(str) job(c) job2(c) hairstyle(c) facestyle(c) x(f) z(f) h(f) r(f) yLayer(n) hp(n) maxHP(n) state(c) pkstate(c) pktitle(c) pluseffect(c) [wear_item_index(n) wear_item_plus(n)]:6 assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count shoptype(c) shopname(str) guildindex(n) guildname(str) guildposition(n) mobindex(n) map_attr(uc) join_flag_merac(c) mountPet(c) evocationType(c) issupporter(c)
								//					: NPC: isNew(uc) type(c) index(n) vnum(n) x(f) z(f) h(f) r(f) yLayer(n) hp(n) maxHP(n) assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count map_attr(uc)
								//					: Pet: isNew(uc) type(c) index(n) owner_index(n) owner_name(str) petTypeGrade(c) x(f) z(f) h(f) r(f) yLayer(n) hp(n) maxHP(n) map_attr(uc)
								//					: 소환수: isNew(uc) type(c) index(n) elemental_type(c) owner_index(n) owner_name(str) x(f) z(f) h(f) r(f) yLayer(n) hp(n) maxHP(n) assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count map_attr(uc) isbigsize(c)
	MSG_DISAPPEAR,				// 사라짐			: type(c) index(n)
	MSG_AT,						// 위치 정보		: index(n) name(str) job(c) job2(c) hair(c) face(c) zoneNo(n) areaNo(n) x(f) z(f) h(f) r(f) yLayer(n) userindex(n) guildoutdate(n) pluseffect(c) map_attr(uc)

	// 10
	MSG_INVENTORY,				// 인벤토리 정보	: resultArrange(c) tab_idx(c) row_idx(c) [item_idx(n) item_type(n) wear_pos(c) plus(n) flag(n) used(n) used_2(n) count(ll) option_count(c) {(option_type(c) option_level(c)} * option_count] * 5
								// 빈칸은 item_idx가 -1이고 이후는 무시, resultArrange 비트 필드(0비트-시작, 1비트-끝)
								// used_2(n) 은 COMPOSITE_TIME 사용시만
	MSG_WEARING,				// 착용 정보 변경	: char_index(n) wear_pos(c) item_db_index(n) item_plus(n)

	// 캐릭터 명령
	MSG_MOVE,					// 이동				: chartype(c) movetype(uc) index(n) speed(f) x(f) z(f) h(f) r(f) y(n) map_attr(uc)

	// 공격 관련
	MSG_ATTACK,					// 공격				: attackchartype(c) attackcharindex(n) targetchartype(c) targetindex(n) attackType(c) multicount(c) multiindex(n:multicount)
	MSG_DAMAGE,					// 공격후 상태		: attackchartype(c) attackcharindex(n) attackerattacktype(c) targetcount(n) [targetchartypc(c) targetindex(n) hp(n) mp(n) damage(n) flag(c)] * targetcount 반복

	// 채팅
	MSG_CHAT,					// 채팅				: chat_type(c) sender_index(n) sender_name(str) receiver_name(str) chat(str)

	// 아이템
	MSG_ITEM,					// 아이템 관련		: sub_type(uc) ...

	// 시스템 메시지
	MSG_SYS,					// 시스템 메시지	: sub_type(uc) ...

	// 존이동
	MSG_GO_ZONE,				// 존이동			: zone_no(n) extra(n) ip(str) port(n)
	MSG_GOTO,					// 같은 존내 이동	: zone_no(n) x(f) z(f) h(f) r(f) y(n)

	// 20
	// 교환
	MSG_EXCHANGE,				// 교환 관련		: sub_type(uc) ...

	// 장소 기억 관련
	MSG_MEMPOS,					// 장소 기억 관련	: sub_type(uc) ...

	// 환경 관련
	MSG_ENV,					// 환경 관련		: sub_type(uc) ...

	// 운영자 명령 관련
	MSG_GM,						// 운영자 명령 관련	: sub_type(uc) ...

	// 파티 관련
	MSG_PARTY,					// 파티 관련		: sub_type(uc) ...

	// Quick Slot 관련
	MSG_QUICKSLOT,				// 퀵슬롯 관련		: sub_type(uc) ...

	// 싱글던전 NPC 리젠
	MSG_NPC_REGEN,				// NPC 리젠 관련	: index(n) dbIndex(n) x(f) z(f) h(f) r(f) yLayer(int) entityindex(n)

	// 스킬 메시지
	MSG_SKILL,					// 스킬 관련		: subtype(uc) ...

	// 보조/저주 상태 메시지
	MSG_ASSIST,					// 보조 상태 메시지	: subtype(uc) ...

	// PC 상태 메시지
	MSG_CHAR_STATUS,			// PC 상태 메시지	: chartype(c) inedx(n) hp(n) maxHP(n) mp(n) maxMP(n) pkstate(c) pktitle(c) state(n) state2(n)

	// 30
	// 캐릭터 부활 메시지
	MSG_PC_REBIRTH,				// pc 부활 메시지	:

	// 이펙트 메시지
	MSG_EFFECT,					// 이펙트 메시지	: subtype(uc) ...

	// EXP, SP 메시지
	MSG_EXP_SP,					// EXP,SP 획득		: exp(n) sp(n)

	// 액션
	MSG_ACTION,					// 액션 사용		: charindex(n) type(c) action(n) state(c),
								//						targetcharindex(n) -> #ifdef EVENT_TLD_2007_SONGKRAN 일때만 전송

	// 로긴서버
	MSG_LOGINSERV_PLAYER,		// 사용자수 전송	: recentserver(n) recentsubnum(n) count(n) seqNo(n) serverno(n) subservercount(n) isrecommendserver(c) [subnum(n) playernum(n) ip_addr(str) portno(n)]... subservercount만큼 반복

	// 퀘스트
	MSG_QUEST,					// 퀘스트 관련		: subtype(uc) ...

	// 스탯 포인트 사용
	MSG_STATPOINT,				// 스탯포인트 사용	: subtype(uc) ...

	// 순간 이동 관련
	MSG_WARP,					// 순간이동 관련	: subtype(uc) ...

	// 펄스 검사 관련
	MSG_PULSE,					// 펄스 관련		: pulse(n), nationalindex(c), tickDelayTime(f)

	// 무작위 생산
	MSG_RANDOM_PRODUCE,			// 무작위 생산		: npcindex(n)

	// 40
	// Special Skill 관련
	MSG_SSKILL,					// 특수수킬 관련	: subtype(uc) .....

	// PK 관련
	MSG_PK,						// PK 관련			: subtype(uc) ...

	// 길드 관련
	MSG_GUILD,					// 길드 관련		: subtype(uc) ...

	// 이벤트 관련
	MSG_EVENT,					// 이벤트 관련		: subtype(uc) ...

	// 개인상점
	MSG_PERSONALSHOP,			// 개인 상점		: subtype(uc) ...

	// 정당방위
	MSG_RIGHT_ATTACK,			// 정당방위			: subtype(uc) target_type(c) target_index(n) 

	// 창고
	MSG_STASH,					// 창고				: subtype(uc) ...

	// 변신
	MSG_CHANGE,					// 변신				: subtype(uc) ...

	// 인터페이스 옵션
	MSG_UI,						// UI 옵션관련		: subtype(uc) ...

	// 사제 시스템
	MSG_TEACH,					// 사제				: subtype(uc) ...

	// 50
	// 전직관련
	MSG_CHANGEJOB,				// 전직 관련		: subtype(uc) ...

	MSG_FRIEND,					//메신저 관련.

	// 퍼스널 던전용 멀티 어택 및 무브(NPC)
	MSG_PD_ATTACK,				//어택				: targetchartype(c) targetindex(n) attackType(c) multicount(c) multiindex(n:multicount) 
	MSG_PD_MOVE,				//무브				: movetype(uc) speed(f) multicount(c) multiindex(n:multicount) x(f) z(f) h(f) r(f) y(n)

	// 선택 가공
	MSG_SELECT_PRODUCE,			// 선택 가공		: npc_server_index(n) itemdbindex(n)

	// 이후 메시지는 다음 메시지의 서브 타입으로
	MSG_EXTEND,					// 메시지 확장		: exmsg(n) ...
								// do_Extend에서 Movefirst와 exmsg를 읽으므로 서브타입 처리 루틴에서는 MoveFirst를 하면 안됨

	MSG_BILLINFO = 60,			// 빌링 관련		: SUBTYPE(UC) ...	
	MSG_TRADEAGENT,				// 거래 대행
	MSG_EXPEDITION,				// 원정대 
	MSG_LOGOUT,					// 로그아웃 
	MSG_FACTORY,				// 제작 시스템
	
	// -- 여기까지

	// 메신저
	MSG_MSGR_CONNECT = 100,		// 메신저 접속		: version(n) server_num(n) subnum(n) count_zone(n) ( zone_num(n) * count_zone )
	MSG_MSGR_REQ,				// 메신저 요청		: seq(n) server(n) subnum(n) zone(n) subtype(uc) ,,,
	MSG_MSGR_REP,				// 메신저 응답		: seq(n) server(n) subnum(n) zone(n) subtype(uc) ...

	// 60
	// 컨넥터
	MSG_CONN_CONNECT = 110,		// 컨넥터 접속		: version(n) server_no(n) subnum(n) server_ip(str) portno(n) count_zone(n) ( zone_num(n) * count_zone )
	MSG_CONN_REQ,				// 컨넥터 요청		: subtype(uc) ...
	MSG_CONN_REP,				// 컨넥터 응답		: subtype(uc) ...

	// 헬퍼
	MSG_HELPER_CONNECT = 120,	// 헬퍼 접속		: version(n) server_no(n) subnum(n) server_ip(str) portno(n) count_zone(n) ( zone_num(n) * count_zone )
	MSG_HELPER_REQ,				// 헬퍼 요청		: seq(n) server(n) subnum(n) zone(n) subtype(uc) ,,,
	MSG_HELPER_REP,				// 헬퍼 응답		: seq(n) server(n) subnum(n) zone(n) subtype(uc) ...
	MSG_HELPER_COMMAND,			// 헬퍼 명령		: subtype(n) ...

	MSG_GM_CHAT,				// GMTOOL 채팅

} MESSAGETYPE;


// MSG_EXTEND
typedef enum __tagMsgExtendType
{
	MSG_EX_PET_STATUS,			// 애완동물 상태	: index(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) ability(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n) x(f) z(f) h(f) r(f) y(n) mapatt(uc) remainRebirth(n)
								// 클라이언트 리스트에 해당 펫이 없으면 추가 있으면 업데이트 하도록 할 것
	MSG_EX_PET_CALL,			// 애완둥물 호출	: index(n)
								// Client->Server
	MSG_EX_PET_MOUNT,			// 애완동물에 타기	: owner_index(n) mountPetTypeGrade(c)
								// Server->Client
	MSG_EX_PET_LEARN,			// 펫 교육			: skillindex(n) level(c:server) errorcode(n:server)
	MSG_EX_PET_SKILLLIST,		// 펫 스킬 리스트	: index(n) count(n) [skillindex(n) skilllevel(c)] ...
	MSG_EX_PET_RESET_SKILL,		// 펫 스킬 초기화
	MSG_EX_PET_SELL_INFO,		// 펫 교환/판매 정보: ownerindex(n) petindex(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) ability(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n) remainRebirth(n)
	MSG_EX_PET_CHANGE_MOUNT,	// 펫 타입 변경		: errorcode(n:server)
	
	MSG_EX_NAMECHANGE,			// 이름 변경c->s	: item_idx(n) name(str)
								//			s->c	: errorcode(c) name(str) bguild(c)
	MSG_EX_CASHITEM,			// 아이템 구매		: subtype(uc) ...
	MSG_EX_PET_COMMAND,			// 10 펫 사교동작		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
	MSG_EX_PET_MIX_ITEM,		// 펫 이아템 조합	: 
								// Client->Server	: itemdbindex(n), method(n)
								// Server->Client	: errcode(n)
	MSG_EX_FRIENDNAMECHANGE,	// 이름변경에 따른 메신져 이름 변경 s->c : charindex(n) name(str)
	MSG_EX_PET_LEVELUP,			// 펫 레벨업		: petindex(n) typegrade(c) level(n)
	MSG_EX_PET_WARPTOWN,		// 마을로 귀환		: 

	MSG_EX_CASTLE_MAP_RECENT,	// 공성맵 최근 정보	: lord_x(f) lord_z(f) lord_hp(n) lord_maxhp(n) tower_count(n) [tower_dbindex(n) tower_x(f) tower_z(f) tower_hp(n) tower_maxhp(n)]*count regen_x(f) regen_z(f) signal_sender_flag(n) signal_sender_index(n) signal_x(f) signal_z(f)
								// 클라이언트에서 보낼때는 메시지 타입만
								// sender_flag 는 MSG_GUILD_POSITION_TYPE
	MSG_EX_CASTLE_MAP_SIGNAL,	// 공성맵 시그널	: x(f) z(f) sender_flag(n:server) sender_index(n:server)

	MSG_EX_ELEMENTAL_STATUS,	// 소환수 정보		: index(n) elementaltype(c) remain(n) hp(n) maxhp(n) attackspeed(c) magicspeed(c) skillspeed(n) walkspeed(f) runspeed(f) attackrange(f) x(f) z(f) h(f) r(f) ylayer(c) mapattr(uc) assist_state(n) assist_count(c) [itemidx(n) index(n) level(c) remain(n)]:count
	MSG_EX_ELEMENTAL_DELETE,	// 소환수 제거		: index(n)

	MSG_EX_EVOCATION_START,		// 강신 시작		: charindex(n) type(c)
	MSG_EX_EVOCATION_STOP,		// 20 강신 끝			: charindex(n)

	MSG_EX_GUILDMEMBERNAMECHANGE,// 이름변경에 따른 길드정보의 이름 변경 s->c : charindex(n) name(str)

	MSG_EX_KEYCHANGE,			// 암호키 변경		: key(unsigned int)

	MSG_EX_PET_CHANGE_ITEM,		// 펫 아이템 교환	: errorcode(n:server)

	MSG_EX_PET_REBIRTH,			// 펫 봉인 해제		: pet_index(n) errcode(n:server)

	MSG_EX_PARTY_RECALL,		// 파티 리콜		: subtype(n) ...

	MSG_EX_PARTY_MATCH,			// 파티 매칭		: subtype(n) ...

	MSG_EX_STRING,				// 클라이언트 메세지출력 : outputtype(uc) stringIndex(n) argcount(n) argtype(n) arg
								// GS->C만 급할때만 : 
	MSG_EX_MESSENGER,			// 메신저 추가 기능 : subtype(c)

	MSG_EX_HAIR_CHANGE,			// 헤어 변경 메시지 : charindex(n) hairstyle(c)

	MSG_EX_PLAYER_STATE_CHANGE,	// 30 플레이어 상태 변경 : charindex(n) state(n)
	MSG_EX_PLAYER_REALIP,			// User real IP 전송 : user_ip(s)

	MSG_EX_MEMPOSPLUS,			// 프리미엄 메모리 북 : sub_type(uc) ...									

	MSG_EX_NPROTECT,			// NPROTECT 관련 메시지 : gg_auth_index(n) gg_auth_val1(n) gg_auth_val2(n) gg_auth_val3(n)

	MSG_EX_PET_CHANGE_NAME,		// 펫 이름 변경 : pet_index(n), name(str)
	MSG_EX_PET_CHANGE_COLOR,		// 펫 색깔  변경 : pet_index(n), color( c );

	MSG_EX_GO_ZONE,				// 존 이동방식 추가 기능 : subtype(c) ...
	
	MSG_EX_CASTLE_WAR,			// 드라탄 공성 : subtype(uc) ...

	MSG_EX_SECURE_CARD,			// 홍콩 보안 카드 : gs->cl s1(c), s2(c), s3(c), s4(c)   cl->gs s1(c), s2(c), s3(c), s4(c)
	
	MSG_EX_DVD,					// 동적 가변 던전: subtype(uc) ...

	MSG_EX_ALTERNATE_MERCHANT,	// 40 대리 상인 : subtype( uc ) ....

	MSG_EX_PET_TURNTO_NPC,		// 펫 NPC 변신 (n)nPetIndex , (n)toNPC, (n)nOwnerIndex

	MSG_EX_MONSTERCOMBO,			// 몬스터 콤보

	MSG_EX_RESTART,				// Connector 에 임시 패스워드 연장과, 로그아웃 대기 15초를 제거후 클라이 언트와 Connection제거
	
	MSG_EX_CHAOSBALL,			// 카오스 볼 교환 : row(n), col(n)

	MSG_EX_UPDATE_PLAYTIME,		// Client에서 10분 간격으로 PLAYTME 누적 요청

	MSG_EX_ATTACK_PET,			// attack pet 관련 (c)subtype

	MSG_EX_EXTREME_CUBE,		// 익스트림 큐브

	MSG_EX_INIT_SSKILL,			// 특수스킬초기화

	MSG_EX_DISCONNECT_HACK_CHARACTER,	// 핵 유저 방출 : 

	MSG_EX_TRIGER_EVENT,		// 트리거 이벤트 시스템

	//new by Falo
	MSG_EX_AFFINITY,			// 평판

	MSG_EX_TUTORIAL,			// 튜토리얼 시스템
	
	MSG_EX_LOADING_END,			// 로딩시 서버로부터 받는 메시지의 끝을 전달하는 부분.	

	MSG_EX_RAID_INFO,			// 귀속된 존 정보 표시

	MSG_EX_NPC_PORTAL_SCROLL,		// connie [2009/9/8] - NPC 찾기 
	
	MSG_EX_LUCKYDRAW_BOX,

	MSG_EX_TITLE_SYSTEM,			// 호칭

	MSG_EX_TAKE_AGAIN_QUEST_ITEM,	// [100208: selo] 퀘스트 아이템 다시 받기
	
	MSG_EX_LACARETTE,				// [100324 : sora] 라카렛 시스템

	MSG_EX_COSTUME2,				// [100421 : kiny8216]코스튬2 시스템
	
	MSG_EX_DUNGEONTIME,				// 던전 타임

	MSG_EX_SOCKET,					// socket system. [4/8/2010 rumist]
	
	MSG_EX_ATTENDANCE_EXP_SYSTEM,	// 출석 시스템 : 누적출석일수별경험치REQ : (c)subtype 0:확인, 1:누적보상받기/ REP (c) subtype 0:게임접속1:확인2:출석완료3:누적d별보상(n)accCount 누적일, (c)upExp 추가경험치

	MSG_EX_XTRAP,
	
	MSG_EX_TUTORIAL_RENEWER_2010,	// 튜토리얼 개선
	
	MSG_EX_RAID_SCENE,				// 아칸사원 레이드

	MSG_EX_SUBJOB,					// [2010/08/25 : Sora] ADD_SUBJOB 보조직업(상인캐릭)

	// new:
	MSG_EX_LORDCOSTUME,

	MSG_EX_MONSTERMERCENARYCARD, // 69

	MSG_EX_FACEOFF, // 70

	MSG_EX_71, // 71

	MSG_EX_MSGBOX, // 72

	MSG_EX_LACABALL, // 73

	MSG_EX_ROYAL_RUMBLE, // 74

	MSG_EX_75, // 75  cash shop zeug?

	MSG_EX_76, // 76

	MSG_EX_RANKINGLIST, // 77

	MSG_EX_78, // 78

	MSG_EX_79, // 79

	MSG_EX_80, // 80

	MSG_EX_81, // 81

	MSG_EX_82, // 82

	MSG_EX_83, // 83

	MSG_EX_84, // 84

	MSG_EX_85, // 85

	MSG_EX_86, // 86

	MSG_EX_FGC, // 87

	MSG_EX_88, // 88

	MSG_EX_89, // 89

	MSG_EX_REFORM, // 90 Anvil
} MSG_EXTEND_TYPE;

typedef enum __tagMsgExTrigerEventType
{
	MSG_EX_TRIGER_EVENT_ERROR,					// 에러	
	MSG_EX_TRIGER_EVENT_ALREADY_PLAY,			// 이미 실행 됨
	MSG_EX_TRIGER_EVENT_DONT_ENOUGH_CONDITION,			// 조건이 성립이 안됨
	MSG_EX_TRIGER_EVENT_ITEM_COMPOSITION,		// 아이템 결합
	MSG_EX_TRIGER_EVENT_ITEM_DROP,				// 아이템 드랍
	MSG_EX_TRIGER_EVENT_OPEN_THE_DOOR,			// 문 열기
	MSG_EX_TRIGER_EVENT_START_RAID,				// 레이드 시작
	MSG_EX_TRIGER_EVENT_MESSAGE_PRINT,			// 게임 메세지 출력
	MSG_EX_TRIGER_EVENT_SUMMON_NPC,				// npc 소환
	MSG_EX_TRIGER_EVENT_USE_DEBUF,				// 디버프 걸림
	MSG_EX_TRIGER_EVENT_ACTIVE_OBJECT,			// object 활성화
	MSG_EX_TRIGER_EVENT_REMOVE_NPC,				// npc 자동 제거
}MSG_EX_TRIGER_EVENT_TYPE;

typedef enum __tagMsgExInitSSkillErrorType
{
 MSG_EX_INIT_SSkill_ERROR_SUCCESS, // 성공
 MSG_EX_INIT_SSkill_ERROR_FAIL,  // 실패
} MSG_EX_INIT_SSkill_ERROR_TYPE;

typedef enum _tagMsgExExtremeCubeType
{
	MSG_EX_EXTREME_CUBE_PARTY,		// 파티큐브 단체입장시
	MSG_EX_EXTREME_CUBE_PARTY_PERSON,	// 파티큐브 개인입장시
	MSG_EX_EXTREME_CUBE_GUILD,		// 길드큐브 단체입장시
	MSG_EX_EXTREME_CUBE_GUILD_PERSON,	// 길드큐브 개인입장시
	MSG_EX_EXTREME_CUBE_STATE_REQ,		// 큐브현황 창 호출시
	MSG_EX_EXTREME_CUBE_STATE_REP,		// cubemaster(s), master_guildname(s), 
										// count(c), rank(c), guildname(s), guildmaster(s), cubepoint(n)
	MSG_EX_EXTREME_CUBE_COMPLETE,		// 완료카운트 count(c)
	MSG_EX_EXTREME_CUBE_ERROR,		// errortype
	MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REQ,   // 큐브 개인포인트 확인시
	MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP,   // 
}MSG_EX_EXTREME_CUBE_TYPE;


typedef enum _tagMsgExExtremeCubeErrorType
{
	MSG_EX_EXTREME_CUBE_ERROR_NOITEM,  // 아이템이 없음
	MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT,  // 방이존재하지 않음
	MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE,  // 이동불가
	MSG_EX_EXTREME_CUBE_ERROR_CANNOT_CAHNGE_RATE, // 세율변경불가
	MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME, // 길드큐브시간아님
	MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME, // 파티큐브시간아님
	MSG_EX_EXTREME_CUBE_ERROR_PARTYCUBE,   // 파티큐브 들어감
	MSG_EX_EXTREME_CUBE_ERROR_GUILDCUBE,   // 길드큐브 들어감
	MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE,
	MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE,
	MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME,
	MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME,
}MSG_EX_EXTREME_CUBE_ERROR_TYPE;


typedef enum __tagMsgExAttackPetErrorType
{
	MSG_SUB_STAT,				// CAHR STATE MSG 
	MSG_SUB_ITEM_WEAR,			// 아이템 입기 벗기 관련
	MSG_SUB_ITEM_WEARING,		// 주위 장비 변화 알리기
	MSG_SUB_REBIRTH,			// 부활 요청
	MSG_SUB_SKILLLIST,			// 스킬리스트 요청
	MSG_SUB_SKILLLEAN,			// 스킬 배우기
	MSG_SUB_STATUP,				// 스텟 올리기
	MSG_SUB_LEVELUP,			// 레벨 업 알리기
	MSG_SUB_DELETE_EQUIP,		// 착용한 펫의 장비 제거
	MSG_SUB_APET_INFO,			// 펫 정보
	MSG_SUB_ADDITEM_MSG,		// 펫 정보창에 들어가는 ITEM 정보

	MSG_SUB_AI_LIST_REQ,		// AI 리스트 요청
	MSG_SUB_AI_LIST_NTF,		// AI 리스트 응답		: count(n), ( ai_index(n), item_index(n), skill_index(n) )*n
	MSG_SUB_AI_ONOFF_REQ,		// AI 활성화 설정       : enable(c)
	MSG_SUB_AI_ONOFF_REP,
	MSG_SUB_AI_UPDATE_REQ,		// AI 수정 요청			: count(n), ( ai_index(n), item_index(n), skill_index(n) )*n
	MSG_SUB_AI_UPDATE_REP,		// AI 수정 응답         : errcode(uc), ai_index(n), item_index(n)
} MSG_EX_ATTACK_PET_ERRORTYPE;

typedef enum _tagMsgAPetErrorType
{
	MSG_APET_ERROR_OK,
	MSG_APET_ERROR,
	MSG_APET_ERR_NOT_HAVE_ITEM,		// 갖고 있지 않은 아이템 : ai_index(n)
	MSG_APET_ERR_WRONG_AI			// 잘못된 AI 설정 : ai_index(n)
}MSG_APET_ERROR_TYPE;

typedef enum __tagMsgExChaosBallErrorType
{
	ERROR_CHAOSBALL_SUCCESS,				// 카오스 볼 아이템 성공
	ERROR_ITEM_COUNT,						// 아이템 갯수 오류
	ERROR_NOSPACE,							// 인벤토리에 빈 공간이 없다.
	ERROR_ITEM_FAULT						// 카오스 볼 아이템 실패
} MSG_EX_CHAOSBALL_ERRORTYPE;

typedef enum __tagMsgExMonsterComboType
{
	MSG_EX_MONSTERCOMBO_EDIT_COMPLETE,			// 미션케이스 편집완료시 count(c), index(n)
	MSG_EX_MONSTERCOMBO_EDIT_CANCEL,			// 취소시
	MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REQ,		// 편집된 내용보기요청
	MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP,		// 편집된 내용보기응답 count(c), index(n)
	MSG_EX_MONSTERCOMBO_GOTO_COMBO,			// 편집완료후 대기방으로 갈 때
	MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT,		// 확인메시지
	MSG_EX_MONSTERCOMBO_GOTO_COMBO_CONFIRM,		// 파티원이 확인, 취소버튼 눌렀을 때 yesno(c): (yes 1 no 0),  bossindex(n)
	MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO,		// 대기방에서 파티리콜할 때
	MSG_EX_MONSTERCOMBO_RECALL_TO_STAGE,		// 스테이지로 강제리콜
	MSG_EX_MONSTERCOMBO_GIVEUP,			// 몬스터 콤보 포기			zone(n), extra(n), npcidx(n)
	MSG_EX_MONSTERCOMBO_START,				// 몬스터 콤보 시작
	MSG_EX_MONSTERCOMBO_MISSION_COMPLETE,		// nextstage(n), complete(c) 0: 미션실패, 1: 미션성공
	MSG_EX_MONSTERCOMBO_GOTO_STAGE,			// 대기방에서 스테이지로
	MSG_EX_MONSTERCOMBO_GOTO_WAITROOM,			// 스테이지에서 대기방으로 감
	MSG_EX_MONSTERCOMBO_NOTICE_STAGE,		// stage(n) 시작시 stage 넘버메시지 전달
	MSG_EX_MONSTERCOMBO_ERROR,				// 몬스터 콤보 오류관련 error(c)
}MSG_EX_MONSTERCOMBO_TYPE;

 
typedef enum __tagMsgExMonserComboErrorType
{
	MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_OK,	// 편집완료
	MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_FAIL,	// 편집완료 실패
	MSG_EX_MONSTERCOMBO_ERROR_MONEY,			// 나스부족
	MSG_EX_MONSTERCOMBO_ERROR_FULL,			// 콤보존 공간이 없을 경우
	MSG_EX_MONSTERCOMBO_ERROR_NOT_EDIT,		// 편집완료가 안됨
	MSG_EX_MONSTERCOMBO_ERROR_NOT_COMPLETE_STAGE,	// 스테이지 완료되지 않았음
}MSG_EX_MONSTERCOMBO_ERROR_TYPE;


typedef enum __tagMsgExAlternateMerchant
{
	MSG_ALTERNATEMERCHANT_ERROR,						// 개인상점 관련 오류		: errorcode(uc)
	MSG_ALTERNATEMERCHANT_CHANGE,					// 상점 상태 변화(중지)		: charindex(n) shoptype(c)
	MSG_ALTERNATEMERCHANT_SELL_START,				// 판매시작					: charindex(n) shoptype(c) shopname(str) [normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:client
												// normal_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
												// pack_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	MSG_ALTERNATEMERCHANT_SELL_LIST,					// 판매리스트 요청			: charindex(n) [ shoptype(c) normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:server
												// normal_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) price(ll)
												// pack_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)
	MSG_ALTERNATEMERCHANT_BUY,						// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	
	MSG_ALTERNATEMERCHANT_END,					// 판매상 종료
	MSG_ALTERNATEMERCHANT_PRODUCT_RECOVERY,		// 상품 회수 :  
	MSG_ALTERNATEMERCHANT_PRODUCT_NAS,			// 나스 회수 :

}MSG_EX_ALTERNATE_MERCHANT_TYPE;

typedef enum __tagMsgExAlternateMerchatErrorType
{
	MSG_ALTERNATEMERCHANT_ERROR_OK,					// 정상
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL,		// 요청 캐릭터가 스킬 사용 중일때
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_WARP,		// 요청 캐릭터가 워프(이동장치) 중일때
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE,	// 요청 캐릭터가 교환 중일때
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_MONEY,		// 나스를 개인상점으로 거래하고자 할때
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC,			// 그외 상점을 열수 없는 상태(패킷내용오류 등)
	MSG_ALTERNATEMERCHANT_ERROR_AREA,				// 상점 개설 불가능 지역일때
	MSG_ALTERNATEMERCHANT_ERROR_DUPLICATION,			// 하나의 아이템을 여러 번 등록하려 할 때
	MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND,			// 대상 캐릭터가 없을 때
	MSG_ALTERNATEMERCHANT_ERROR_NOTSELL,				// 대상 캐릭터가 판매 상점이 아닐 때
	MSG_ALTERNATEMERCHANT_ERROR_NOTPACKAGE,			// 대상 캐릭터가 패키지 판매가 아닐 때
	MSG_ALTERNATEMERCHANT_ERROR_MONEY,				// 소지금 부족
	MSG_ALTERNATEMERCHANT_ERROR_COUNT,				// 대상 개인상점보다 요청 판매/구매 카운트가 클때
	MSG_ALTERNATEMERCHANT_ERROR_CANTPREMIUM,			// 소지금 부족으로 패키지 상점 개설 불가
	MSG_ALTERNATEMERCHANT_ERROR_BUY_FULLINVENTORY,	// 인벤토리 가득차서 못 삼
	MSG_ALTERNATEMERCHANT_ERROR_ANYMORE_PET,			// 펫 소유 제한이 넘어서 취소
	MSG_ALTERNATEMERCHANT_ERROR_NOSPACE,				// 인벤이 가득차서 판매 불가 : 돈 지급 받을 공간이 없음
	MSG_ALTERNATEMERCHANT_ERROR_NOCOLLECT_ITEM,			// 회수 안된 아이템이 있음
	MSG_ALTERNATEMERCHANT_ERROR_TIMEOVER,				// 시간 만료
	MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ALREADY,			// 대행 NPC가 존재함
	MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER,			// 헬퍼 서버 에러
	MSG_ALTERNATEMERCHANT_ERROR_NOTITEM,				// 아이템 없음
	MSG_ALTERNATEMERCHANT_ERROR_WEIGHT_OVER,						// 무게 초과로 찾을 수 없음
	
}MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE;


typedef enum __tagMsgExCastleWarType
{
	MSG_CASTLE_CRISTAL_RESPOND_START,	// 교감 시작	charindex(n), starttime(ll)	
	MSG_CASTLE_CRISTAL_RESPOND_END,		// 교감 완료	charindex(n)
	MSG_CASTLE_CRISTAL_RESPOND_FAIL,	// 교감 실패	charindex(n)
	MSG_CASTLE_TOWER_CONTRAL,			// 마스터 타워 타워 설정		7*control(c)
	MSG_CASTLE_TOWER_CONTRAL_LIST,		// 마스트 타워 설정 리스트		7*control(c)
	MSG_CASTLE_TOWER_REINFORCE,			// 마스터 타워 강화 설정		type(c), step(c)
	MSG_CASTLE_TOWER_REINFORCE_LIST,	// 마스터 타워 강화 리스트		cl->gs type(C) | gs->cl type(c), step(c)
	MSG_CASTLE_TOWER_REPAIRE,			// 공성 타워 수리				cl->gs idx(n) | gs->cl	idx(n)
	MSG_CASTLE_TOWER_REPAIRE_LIST,		// 공성 타워 수리 상태			cl->gs idx(n) | gs->cl	money(n)
	MSG_CASTLE_TOWER_WARP_LIST,			// 공성 워프 타워 유효 리스트   cl->gs idx(n) | gs->cl  cnt(c) * idx(n)	
	MSG_CASTLE_QUARTERS_INSTALL,		// 부활진지 설치				cl->gs idx(n) | gs->cl  idx(n), gidx(n)
	MSG_CASTLE_QUARTERS_CRUSH,			// 부활 진지 파괴				gs->cl idx(n)
	MSG_CASTLE_QUARTERS_LIST,			// 부활 진지 리스트				gs->cl 7*(nindex(n), gindex(n))
	MSG_CASTLE_WAIT_TIME,				// 부활 대기 시간				gs->cl wait_time(n)
	MSG_EX_CASTLE_ERROR,
	MSG_CASTLE_OWNER_INFO,				// 성주 정보					gs->cl gindex_dratan(n), gindex_merac(n)
} MSG_EX_CASTLE_WAR_TYPE;


typedef enum __tagMsgDVDInfoType
{
	MSG_DUNGEON_INFO,					// 던전 정보 요청
	MSG_MANAGEMENT,						// 던전 관리
	MSG_DUNGEON_ENTER,					// 던전 이동 
	MSG_GUILD_NAME_COLOR,				// 길드 이름 색 (n)charIndex, (c)Color
} MSG_DVD_INFO_TYPE;

typedef enum __tagMsgDVDInfoSubType		// 던전 관리 서브 타입
{
	MSG_MANAGEMENT_MANAGER_CONFIRM,		// 관리 요청				
	MSG_MANAGEMENT_ENV_INFO,			// 던전 내부환경 정보 요청
	MSG_MANAGEMENT_ENV_CHANGE,			// 던전 내부환경 변경
	MSG_MANAGEMENT_STATE_INFO,			// 던전 내부 몬스터 정보 요청
	MSG_MANAGEMENT_STATE_CHANGE,		// 던전 내부 몬스터 상태 변경
	MSG_MANAGEMENT_TAX_INFO,			// 공성 세금 정보 요청
	MSG_MANAGEMENT_TAX_CHANGE,			// 공성 세금 정보 변경 
	MSG_MANAGEMENT_HUNTER_TAX_INFO,		// 던전 수렵세금 요청 
	MSG_MANAGEMENT_HUNTER_TAX_CHANGE,	// 던전 수렵세금 변경
	MSG_MANAGEMENT_ETC,					// 기타 에러
	MSG_DUNGEON_ENTER_FEE,				// 던전 이동 요금 요청  REQ: NOT, REP: (ll) money
	MSG_DUNGEON_GO,						// 던전으로 이동 REQ: NOT 
} MSG_DVD_INFO_SUBTYPE;

typedef enum __tagMsgDVDInfoErrorType
{
	MSG_DVD_ERROR_OK,				// 성공
	MSG_DVD_ERROR_CONFIRM,			// 권한 없음
	MSG_DVD_ERROR_MONEY,			// 돈없음
	MSG_DVD_ERROR_ZONE,				// 공성이나 존에 대한 에러
	MSG_DVD_ERROR_GUILD,			// 길드가 없거나, 찾을 수 없을때
} MSG_DVD_INFO_ERROR_TYPE;


typedef enum __tagMsgExCastleErrorType
{
	MSG_EX_CASTLE_ERROR_NOT_JOIN,				// 공성 참가주이 아님
	MSG_EX_CASTLE_ERROR_NOT_WAR,				// 공성전 중이 아님
	MSG_EX_CASTLE_ERROR_POS_FAIL,				// 위치 이상
	MSG_EX_CASTLE_ERROR_MEM_5_FAIL,				// 교감 인원 자리 없음
	MSG_EX_CASTLE_ERROR_NOT_OWNER,				// 성주 길드 아님
	MSG_EX_CASTLE_ERROR_NOT_RESPOND,			// 교감 중이 아님
	MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME,		// 교감 시간 부족
	MSG_EX_CASTLE_ERROR_NO_MONEY,				// 돈 부족
	MSG_EX_CASTLE_ERROR_NOT_ATTACK,				// 공성 길드 아님
	MSG_EX_CASTLE_ERROR_NO_DATA,				// 데이터 이상
	MSG_EX_CASTLE_ERROR_NO_DIFFER,				// 이전 데이터와 동일
	MSG_EX_CASTLE_ERROR_NOT_BUY,				// 구매하지 않았음
	MSG_EX_CASTLE_ERROR_NO_NPC,					// 없는 타워
	MSG_EX_CASTLE_ERROR_DEAD_NPC,				// 죽은 타워
	MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD,			// 기드 원이 아님
	MSG_EX_CASTLE_ERROR_USE_NPC,				// 사용중 부활 진지
	MSG_EX_CASTLE_ERROR_USE_NPC_OTHER,			// 이미 다른 부활진지 사용중
	MSG_EX_CASTLE_ERROR_NO_NPC2,					// 없는 부활 진지
	MSG_EX_CASTLE_ERROR_DEAD_NPC2,				// 죽은 부활진지
	MSG_EX_CASTLE_ERROR_SAME_DATA,				// 같은 데이터
	MSG_EX_CASTLE_ERROR_NOT_BUY_NPC,			// 더이상 NPC를 구매 할수 없습니다,
} MSG_EX_CASTLE_ERROR_TYPE;

typedef enum __tagMsgExMessengerType
{
	MSG_EX_MESSENGER_INVITE,	// 해당 케릭터 초대 : makeCharIndex(n) chatIndex(n) charIndex(n)
								// chatIndex 가 -1이면 대화방 추가,
								// GS->C			: makeCharIndex(n) chatIndex(n) charName(str) 
	MSG_EX_MESSENGER_OUT,		// 해당 케릭터 대화방에서 나감 : makeCharIndex(n) chatIndex(n)
								// GS->C			: makeCharIndex(n) chatIndex(n) charName(str) 
	MSG_EX_MESSENGER_CHARLIST,	// 초대받은 케릭터에게 현재방 리스트를 보냄 
								// GS->C			: makeCharIndex(n) chatIndex(n) count(n) charName(str)
	MSG_EX_MESSENGER_CHAT,		// charName이 채팅 메세지 보냄 : makeCharIndex(n) chatIndex(n) chat(str)
								// GS->C			: makeCharIndex(n) chatIndex(n) color(n) charName(str) chat(str) 
	MSG_EX_MESSENGER_GROUP_ADD,	// 친구 그룹 추가	: groupName(str)
								// GS->C			: gIndex(n) groupName(str)  : gIndex 가 -1이면 중복된 이름 
	MSG_EX_MESSENGER_GROUP_DEL,	// 친구 그룹 삭제	: gIndex(n)
								// GS->C			: gIndex(n) : gIndex 가 -1이면 존재하지 않는 그룹
	MSG_EX_MESSENGER_GROUP_CHANGE,	// 친구 그룹 이름변경 : gIndex(n) newName(str)
								// GS->C			: gIndex(n) groupName(str)  : gIndex가 -1이면 중복된 이름 
	MSG_EX_MESSENGER_GROUP_MOVE,	// 친구 그룹 이동	  : gIndex(n) charName(str) 
								// GS->C			: gIndex(n) charName(str)   : gIndex가 -1이면 존재하지 않는 그룹 
	MSG_EX_MESSENGER_GROUP_LIST, // 그룹 리스트 보내기 : count(n) gIndex(n) gName(str)

	MSG_EX_MESSENGER_CHATCOLOR_CHAGNE, // 칼라 변경 : color(n)
	MSG_EX_MESSENGER_BLOCK,		// 해당유저 차단	: name(str)
								// GS->C			: errcode(c) charIndex(n) name(str)
	MSG_EX_MESSENGER_RELEASE_BLOCK,	// 해당유저 해제: charIndex(n)
								// GS->C			: errcode(c) charIndex(n) name(str)
	MSG_EX_MESSENGER_BLOCK_LIST, // 차단한 리스트   : count(n) charIndex(n) name(str)

	MSG_EX_MESSENGER_ONE_VS_ONE,	// 1:1 대화 : sCharIndex(n) tCharIndex(n) chat(str)
	MSG_EX_MESSENGER_ONE_OFF_LINE,	// 상태 오프라인중: sCharIndex(n), tCharIndex(n)
} MSG_EX_MESSENGER_TYPE;

typedef enum __tagMsgEMessengerErrorType
{
	MSG_EX_MESSENGER_SUCCESS,			// 성공
	MSG_EX_MESSENGER_BLOCK_NOTCHAR,		// 해당 케릭터가 없습니다
	MSG_EX_MESSENGER_BLOCK_INVALIDNAME,	// 이름이 잘못 입력 되었습니다
	MSG_EX_MESSENGER_NOMORE_BLOCK,		// 더이상 블럭할수 없습니다
	MSG_EX_MESSENGER_ALREADY_BLOCK,		// 이미 블럭되었습니다

} MSG_EX_MESSENGER_ERROR_TYPE;

typedef enum __tagMsgExStringOutputType
{
	MSG_EX_STRING_OUTPUT_WINDOW,		// 창
	MSG_EX_STRING_OUTPUT_SYS,			// 시스템창
	MSG_EX_STRING_OUTPUT_NOTICE,		// 공지
	MSG_EX_STRING_OUTPUT_CHAT,			// 챗창
} MSG_EX_STRING_Output_TYPE;

typedef enum __tagMsgExStringArgType
{
	MSG_EX_STRING_CHAR,			// char
	MSG_EX_STRING_INT,			// int
	MSG_EX_STRING_LONGLONG,		// LONGLONG
	MSG_EX_STRING_STR,			// str(50)
} MSG_EX_STRING_ARG_TYPE;

typedef enum __tagMsgExPartyMatchType
{
	MSG_EX_PARTY_MATCH_REG_MEMBER_REQ,		// 파티 매칭 일반 등록 요청 : partyType(c)
	MSG_EX_PARTY_MATCH_REG_MEMBER_REP,		// 파티 매칭 일반 등록 응답 : errorcode(n)

	MSG_EX_PARTY_MATCH_REG_PARTY_REQ,		// 파티 매칭 파티 등록 요청 : comment(str) jobFlag(n) limitlevel(c)
	MSG_EX_PARTY_MATCH_REG_PARTY_REP,		// 파티 매칭 파티 등록 응답 : errorcode(n)

	MSG_EX_PARTY_MATCH_MEMBER_LIST_REQ,		// 파티 매칭 일반 리스트 요청 : pageno(n) job(c) limitlevel(c) partytype(c)
											// job이 -1이면 모든 직업, partytype이 -1이면 모든 종류의 파티, pageno는 0-based index
	MSG_EX_PARTY_MATCH_MEMBER_LIST_REP,		// 파티 매칭 일반 리스트 응답 : pageno(n) count(n) [charindex(n) charname(str) leveldiff(c) zone(n) job(c) partytype(c)] * count
											// leveldiff가 0이면 +-2 사이, 1이면 +-3이상

	MSG_EX_PARTY_MATCH_PARTY_LIST_REQ,		// 파티 매칭 파티 리스트 요청 : pageno(n) job(c) limitlevel(c) partytype(c)
											// job이 -1이면 모든 직업, partytype이 -1이면 모든 종류의 파티, pageno는 0-based index
	MSG_EX_PARTY_MATCH_PARTY_LIST_REP,		// 파티 매칭 파티 리스트 응답 : pageno(n) count(n) [bossindex(n) leveldiff(c) zone(n) jobflag(n) partytype(c) limitlevel(c) comment(str)] * count
											// leveldiff가 0이면 +-2 사이, 1이면 +-3이상

	MSG_EX_PARTY_MATCH_DEL_REQ,				// 파티 매칭 등록 해제 요청
	MSG_EX_PARTY_MATCH_DEL_REP,				// 파티 매칭 등록 헤제 응답

	MSG_EX_PARTY_MATCH_INVITE_REQ,			// 파티 매칭 일반 초대 요청 : charindex(n)
	MSG_EX_PARTY_MATCH_INVITE_REP,			// 파티 매칭 일반 초대 응답 : errorcode(n) partytype(c) bossindex(n) bossname(str) destindex(n) destname(str)

	MSG_EX_PARTY_MATCH_JOIN_REQ,			// 파티 매칭 파티 조인 요청 : bossindex(n)
	MSG_EX_PARTY_MATCH_JOIN_REP,			// 파티 매칭 파티 조인 응답 : errorcode(n) partytype(c) bossindex(n) bossname(str) charindex(n) charname(str) charjob(c)
	MSG_EX_PARTY_MATCH_JOIN_ALLOW,			// 파티 매칭 파티 조인 허락 : reqindex(n)
	MSG_EX_PARTY_MATCH_JOIN_REJECT,			// 파티 매칭 파티 조인 거절 : reqindex(n)
											// reqindex는 조인을 요청한 사람일 경우 자신의 index, 파티장의 경우 조인을 요청한 사람의 인덱스 이다

} MSG_EX_PARTY_MATCH_TYPE;

typedef enum __tagMsgExPartyMatchErrorType
{
	MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_OK,					// 파티 매칭 일반 등록 성공
	MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY,		// 이미 파티 참여자가 등록 요청
	MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG,		// 이미 등록된 캐릭

	MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_OK,					// 파티 매칭 파티 등록 성공
	MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS,				// 파티장이 아닌 사람이 요청시
	MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_FULL,				// 파티 인원 가득참
	MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG,			// 이미 등록된 파티

	MSG_EX_PARTY_MATCH_ERROR_INVITE_OK,
	MSG_EX_PARTY_MATCH_ERROR_INVITE_NOT_FOUND,				// 파티 매칭에 없는 사람을 초대
	MSG_EX_PARTY_MATCH_ERROR_INVITE_LEVEL,					// 파티 타입이 PARTY_TYPE_BATTLE 일때 레벨 검사에 부적합함
	MSG_EX_PARTY_MATCH_ERROR_INVITE_NOBOSS,					// 파티장이 아닌 사람이 요청시
	MSG_EX_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE,				// 파티 타입이 맞지 않음
	MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET,			// 대상이 이미 파티 중
	MSG_EX_PARTY_MATCH_ERROR_INVITE_FULL,					// 파티 인원이 초과
	MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST,		// 다른 사람에게 요청 중일때

	MSG_EX_PARTY_MATCH_ERROR_JOIN_OK,
	MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET,			// 조인을 요청한 사라밍 이미 다른 파티에 가입 중
	MSG_EX_PARTY_MATCH_ERROR_JOIN_NOT_FOUND,				// 조인 하려고 하는 파티가 매칭 테이블에 없음
	MSG_EX_PARTY_MATCH_ERROR_JOIN_LEVEL,					// 파티 타입이 PARTY_TYPE_BATTLE이고 레벨 검사에 부적합
	MSG_EX_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL,				// 레벨 제한 조건에 부적합
	MSG_EX_PARTY_MATCH_ERROR_JOIN_JOB,						// 직업 부적합
	MSG_EX_PARTY_MATCH_ERROR_JOIN_FULL,						// 인원 가득참
	MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST,			// 기존 초대 요청자가 있어서 불가능

} MSG_EX_PARTY_MATCH_ERROR_TYPE;

typedef enum __tagMsgExPartyRecallType
{
	MSG_EX_PARTY_RECALL_NOTICE,				// 리콜요청자 확인 메시지, 파티원들에게 소환메시지를 보냈습니다.
	MSG_EX_PARTY_RECALL_PROMPT,				// 리콜대상자 확인 메시지	: request_char_index(n) request_char_name(str)
											// XXXX파티원께서 소환메시지를 보내왔습니다. 소환에 응하시겠습니까?
	MSG_EX_PARTY_RECALL_CONFIRM,			// 리콜 응답				: request_char_index(n) request_char_name(str) yes_or_no(c)
											// yes_or_no - yes(1), no(0)
	MSG_EX_PARTY_RECALL_CANCEL,				// 리콜 거부시				: request_char_index(n) request_char_name(str) target_char_index(n) target_char_name(str)
} MSG_EX_PARTY_RECALL_TYPE;

typedef enum __tagMsgExPetRebirthErrorType
{
	MSG_EX_PET_REBIRTH_ERROR_OK,			// 성공
	MSG_EX_PET_REBIRTH_ERROR_NOMONEY,		// 나스 부족
	MSG_EX_PET_REBIRTH_ERROR_NOPET,			// 펫 존재 안함
	MSG_EX_PET_REBIRTH_ERROR_NODEAD,		// 펫 살아있음
} MSG_EX_PET_REBIRTH_ERROR_TYPE;

typedef enum __tagMsgExPetChangeItem
{
	MSG_EX_PET_CHANGE_ITEM_ERROR_OK,		// 아이템 획득
	MSG_EX_PET_CHANGE_ITEM_ERROR_NOITEM,	// 아이템 획득 실패, 펫만 없어짐
	MSG_EX_PET_CHANGE_ITEM_ERROR_FAIL,		// 교환 실패 : 조건 미달
} MSG_EX_PET_CHANGE_ITEM_ERROR_TYPE;

typedef enum _tagMsgExPetChangeName
{
	MSG_EX_PET_CHANGE_NAME_ERROR_OK,	// 이름 변경 성공
	MSG_EX_PET_CHANGE_NAME_ERROR_FAIL,	// Pet 이름 변경 실패
	MSG_EX_PET_CHANGE_NAME_ERROR_ITEM,	// 아이템이 없음
	MSG_EX_PET_CHANGE_NAME_ERROR_NOPET, // Pet 존재 안함
	MSG_EX_PET_CHANGE_NAME_ERROR_DUPLICATE, // Pet 이름 중복
}MSG_EX_PET_CHANGE_NAME_ERROR_TYPE;

typedef enum __tagMsgExPetMixItemErrorType
{
	MSG_EX_PET_MIX_ITEM_ERROR_OK,		// 성공
	MSG_EX_PET_MIX_ITEM_ERROR_FAIL,		// 실패
	MSG_EX_PET_MIX_ITEM_ERROR_NOITEM,	// 재료부족
	MSG_EX_PET_MIX_ITEM_ERROR_NOMIX,	// 조합법 오류
} MSG_EX_PET_MIX_ITEM_ERROR_TYPE;

typedef enum __tagMsgExCashItemType
{
	MSG_EX_CASHITEM_BALANCE_REQ,		// 캐쉬 정보 요청
	MSG_EX_CASHITEM_BALANCE_REP,		// 캐쉬				: errorCode(uc) cashBalance(n)
	MSG_EX_CASHITEM_PURCHASE_REQ,		// 구매 요청		: count(n) ctid(n) 
	MSG_EX_CASHITEM_PURCHASE_REP,		// 응답				: errorCode(uc) cashBalance(n)
	MSG_EX_CASHITEM_BRING_REQ,			// 가져오기			: count(n) idx(n) ctid(n)
	MSG_EX_CASHITEM_BRING_REP,			// 응답				: errorCode(uc) 
	MSG_EX_CASHITEM_PURCHASELIST_REQ,	// 구매한 물품 목록
	MSG_EX_CASHITEM_PURCHASELIST_REP,   // 응답				: count(n) idx(n) ctid(n)
	MSG_EX_CASHITEM_PURCHASEHISTORY_REQ,// 구매내역			: y(n) m(c) d(c)
	MSG_EX_CASHITEM_PURCHASEHISTORY_REP,// 응답				: errorcode(uc) count(n) ctid(n)
	MSG_EX_CASHITEM_MOONSTONE_START,	// 아이템의 조건 확인: tab_idx(c) row_idx(c) col_idx(c) item_idx(n)
										//					   result(uc) grade(c)
	MSG_EX_CASHITEM_MOONSTONE_STOP,		// 아이템을 삭제하고 지급요청							   
	MSG_EX_CASHITEM_GIFT_REQ,			// 선물 보내기 요청	: charName(str) Msg(str) count(n) idx(n) ctid(n)
	MSG_EX_CASHITEM_GIFT_REP,			//			응답	: errcode(uc) 
	MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ,  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
	MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP,  // 응답				: errorcode(uc) count(n) ctid(n) recvcharName(str)
	MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ,  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
	MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP,  // 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	MSG_EX_CASHITEM_GIFT_RECVLIST_REQ,	   // 선물 받은 리스트 요청
	MSG_EX_CASHITEM_GIFT_RECVLIST_REP,		// 응답				: listflag(c) count(n) idx(n) ctid(n) date(un) sendcharName(str) msg(str)
	MSG_EX_CASHITEM_GIFT_RECV_REQ,			// 선물 받기		: count(n) idx(n) ctid(n)
	MSG_EX_CASHITEM_GIFT_RECV_REP,			// 선물 받기		: errCode(uc)
	MSG_EX_CASHITEM_GIFT_NOTICE,			// 선물이 도착하였습니다
	
	//쿠폰시스템
	MSG_EX_CASHITEM_CUPON_REQ,				// 보유 쿠폰 리스트 요청
	MSG_EX_CASHITEM_CUPON_REP,				// 보유 쿠폰 리스트 응답  : error code(c) 보유 쿠폰 수(n) { 쿠폰 이름(str), 쿠폰ID(n), 할인 금액(n) } ...
	MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REQ, //  쿠폰으로 구매 하기 : cash, 쿠폰ID(n) , couponAmount(n), Count(n) { stid(n) }... requestCash(n)
	MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REP, // 쿠폰으로 구매하기 응답 : error code(c) cashBalance(n)
} MSG_EX_CASHITEM_TYPE;

typedef enum _tagExCashItemMoonstoneErrorType
{
	MSG_EX_CASHITEM_MOONSTONE_ERROR_SUCCESS,				// 성공적으로 지급
	MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM,					// 교환할 아이템이 없습니다.
	MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN,					// 인벤토리가 부족합니다
	MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON,		// 캐쉬 문스톤을 사용할수 있는 아이템이 아닙니다.
} MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE;

typedef enum __tagMsgExCashItemErrorType
{
	MSG_EX_CASHITEM_ERROR_SUCCESS,				// 성공
	MSG_EX_CASHITEM_ERROR_LACKCASH,				// 보유캐쉬 부족
	MSG_EX_CASHITEM_ERROR_NOTUSER,				// 해당유저 없음
	MSG_EX_CASHITEM_ERROR_NOTITEM,				// 존재하지 않는 상품입니다
	MSG_EX_CASHITEM_ERROR_DB,					// DB오류
	MSG_EX_CASHITEM_ERROR_PACKET,				// 패킷 오류
	MSG_EX_CASHITEM_ERROR_ETC,					// 기타 오류
	MSG_EX_CASHITEM_ERROR_OVERITEM,				// 구입 상품 개수 초과(10개)
	MSG_EX_CASHITEM_ERROR_NOSPACE,				// 구매물품 보관소의 공간 부족
	MSG_EX_CASHITEM_ERROR_LACKINVEN,			// 케릭터 인벤 부족
	MSG_EX_CASHITEM_ERROR_CONN,					// 커넥터 서버 에러
	MSG_EX_CASHITEM_ERROR_BILL,					// 빌링 서버 에러
	MSG_EX_CASHITEM_ERROR_NOTACCOUNT	= 12,			// 결재 권한 없음 
	MSG_EX_CASHITEM_ERROR_NOTHAVECT,			// 남은 수량이 품절
	MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR,		// 선물시에 해당 케릭터가 없습니다
	MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS,			// 선물보내기 성공
	MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS,		// 선물 받은 물품들이 인벤토리로 옮겨졌음
	MSG_EX_CASHITEM_ERROR_GIFT_NOCTID,			// 보낼 상품이 없습니다
} MSG_EX_CASHITEM_ERROR_TYPE;

typedef enum __tagMsgExNamechangeErrorType
{
	MSG_EX_NAMECHANGE_ERROR_SUCCESS,		// 성공
	MSG_EX_NAMECHANGE_ERROR_DUPLICATE,		// 중복이름
	MSG_EX_NAMECHANGE_ERROR_HELPER,			// 헬퍼 서버 오류
	MSG_EX_NAMECHANGE_ERROR_INVALID,		// 잘못된 입력
	MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS,	// 길드장이 아님
} MSG_EX_NAMECHANGE_ERROR_TYPE;

typedef enum __tagMsgExPetLearnErrorType
{
	MSG_EX_PET_LEARN_ERROR_OK,		// 성공
	MSG_EX_PET_LEARN_ERROR_WEAR,	// 펫 미착용
	MSG_EX_PET_LEARN_ERROR_POINT,	// 기술포인트 부족
	MSG_EX_PET_LEARN_ERROR_LEVEL,	// 레벨 부족
	MSG_EX_PET_LEARN_ERROR_ITEM,	// 아이템 부족
	MSG_EX_PET_LEARN_ERROR_AUTO,	// 자동 습득됨
} MSG_EX_PET_LEARN_ERROR_TYPE;

typedef enum __tagMsgExPetChangeMountErrorType
{
	MSG_EX_PET_CHANGE_MOUNT_ERROR_OK,		// 성공
	MSG_EX_PET_CHANGE_MOUNT_ERROR_LEVEL,	// 레벨 부족
	MSG_EX_PET_CHANGE_MOUNT_ERROR_WEAR,		// 착용안함
	MSG_EX_PET_CHANGE_MOUNT_ERROR_GRADE,	// 이미 마운트 타입일 때
} MSG_EX_PET_CHANGE_MOUNT_ERROR_TYPE;

typedef enum _tagMsgMemposPlusType
{
	MSG_EX_MEMPOSPLUS_LIST,		// 기억장소 리스트 전달	: count(uc) [slot(c) zone(n) x(f) z(f) comment(str)] 반복 busetime(n)
	MSG_EX_MEMPOSPLUS_WRITE,	// 기억장소 쓰기		: slot(c) comment(str) zone(n) x(f) z(f)
	MSG_EX_MEMPOSPLUS_MOVE,		// 기억 장소로 이동		: slot(c)
} MSG_MEMPOSPLUS_TYPE;

// 오류 메시지 정의
typedef enum __tagMsgFailType
{
	// 일반
	MSG_FAIL_MANY_CONNECT,					// 접속 요청이 많음
	MSG_FAIL_WRONG_VERSION,					// 버전 오류
	MSG_FAIL_WRONG_CHAR,					// 잘못된 문자 포함
	MSG_FAIL_WRONG_PASSWORD,				// 암호에 잘못된 문자 포함
	MSG_FAIL_ALREADY_CONNECT,				// 이미 접속한 아이디 (한 서버에 재 접속)
	MSG_FAIL_CONNECT_SERVER,				// 접속 실패
	MSG_FAIL_BLOCK_CHAR,					// 캐릭터 블럭
	MSG_FAIL_CHECK_CHAR,					// 계정 오류
	MSG_FAIL_SYSTEM_ERROR,					// 시스템 오류
	MSG_FAIL_NOT_EXIST_CHAR,				// 캐릭터 없음

	// 데이터베이스 관련
	MSG_FAIL_DB_ALREADY_NAME,				// 10 중복된 이름
	MSG_FAIL_DB_NEW_CHAR,					// 새 캐릭터 생성 오류
	MSG_FAIL_DB_FULL_CHAR,					// 슬롯에 빈자리 없음
	MSG_FAIL_DB_NOT_EXIST_CHAR,				// 해당 슬롯에 캐릭 없음
	MSG_FAIL_DB_UNKNOWN,					// 알수없는 오류

	// 빌링 관련
	MSG_FAIL_BILLING_NOT_PAY,				// 이용 결제 확인 안됨
	MSG_FAIL_BILLING_NOT_RIGHT,				// 사용 권한 없음
	MSG_FAIL_BILLING_TIME_OUT,				// 계정 기간 완전히 끝남
	MSG_FAIL_BILLING_ALREADY_CONNECT,		// 중복 접속 (다수의 서버를 위한 것)
	MSG_FAIL_BILLING_OVER_IP,				// 게임방 허용 접속 아이피 제한 초과
	MSG_FAIL_BILLING_TIME_NOT_ENOUGH,		// 20 계정 기간 부족
	MSG_FAIL_BILLING_TIME_OUT_DISCONNECT,	// 계정 시간 종료
	MSG_FAIL_BILLING_OTHER,					// 그외
	MSG_FAIL_BILLING_WRONGPW,				// 아이디 패스워드 오류
	MSG_FAIL_BILLING_BLOCKED,				// 계정 블럭


	// 메신저 관련
	MSG_FAIL_MSGR_TIMEOUT_WHISPER,			// 귓속말 타임 아웃
	MSG_FAIL_MSGR_NOT_FOUND_CHAR,			// 귓말 대화 상대가 없음

	// 로긴서버 관련
	MSG_FAIL_LOGIN_SERVER,					// 접속 실패
	MSG_FAIL_LOGINSERV_MANY_CONNECT,		// 접속 요청이 많음
	MSG_FAIL_LOGINSERV_WRONG_VERSION,		// 버전 오류
	MSG_FAIL_LOGINSERV_WRONG_CHAR,			// 30 잘못된 문자 포함
	MSG_FAIL_LOGINSERV_WRONG_PASSWORD,		// 암호에 잘못된 문자 포함
	MSG_FAIL_LOGINSERV_ALREADY_CONNECT,		// 이미 접속한 아이디 (한 서버에 재 접속)
	MSG_FAIL_LOGINSERV_BLOCK_CHAR,			// 캐릭터 블럭
	MSG_FAIL_LOGINSERV_CHECK_CHAR,			// 아이디/암호 틀림
	MSG_FAIL_LOGINSERV_SYSTEM_ERROR,		// 시스템 오류
	MSG_FAIL_LOGINSERV_NOT_EXIST_CHAR,		// 캐릭터 없음
	MSG_FAIL_LOGINSERV_NO_SERVICE,			// 서비스 기간이 아님
	
	MSG_FAIL_WRONG_IDENTIFICATION,			// 주민번호 틀림
	//테스트 관련
	MSG_FAIL_TEST_WRONG,					// Test 메세지  존,영역 이동 실패
	MSG_NOT_IN_ZONE,						// 40 캐릭터가 엄한 존에
	MSG_FAIL_ENABLE_AREA,					// Enable area msg error
	MSG_FAIL_CANNT_ENABLE_AREA,				// area can't enable msg error

	MSG_FAIL_LOGINSERV_NOT_ADULT,			// 미성년자는 접속할수 없는 시간입니다
	MSG_FAIL_NOTLEVEL_FORDELETE,			// 지울수 있는 레벨이 아님 

	MSG_FAIL_DB_DELETE_DELAY_CHAR,			// 삭제 대기 캐릭터로 게임 시작 요청시

	MSG_FAIL_SCARD_NOT_MATCHING,			// 보안 카드 인증 실패

	MSG_FAIL_LOGINSERV_BLOCK_USER,			// 패스워드3번 틀려서 블럭

	MSG_FAIL_LOGINSERV_BLOCK_ACCOUNT,		// 계정블럭
	MSG_FAIL_LOGINSERV_USE_SECURE_SYSTEM,	// 전화보안 시스템 사용

	MSG_FAIL_END_MSG,						// 모든 MSG_FAIL_TYPE의 메시지는 이보다 앞에 있어야 함
} MSG_FAIL_TYPE;


// 로그인 관련
typedef enum __tagMsgLoginType
{
	MSG_LOGIN_NEW,				// 처음 접속
	MSG_LOGIN_RE,				// 서버 이동
	MSG_LOGIN_GM,				// GMTOOL 접속
} MSG_LOGIN_TYPE;


// 메뉴 관련
typedef enum __tagMsgMenuType
{
	MSG_MENU_NEW,				// NEW CHARACTER		: mode(uc) name(str) job(c) hairstyle(c) facestyle(c)
	MSG_MENU_DEL,				// DELETE CHARACTER		: mode(uc) index(n) ident(str) bcanceldelete(c)
								// ident				: 주민번호
								// bcanceldelete		: 삭제 취소시 1, 삭제시 0
	MSG_MENU_START,				// START				: mode(uc) index(n)
} MSG_MENU_TYPE;


// 이동 관련
typedef enum __tagMsgMoveType
{
	MSG_MOVE_WALK,				// 걷기
	MSG_MOVE_RUN,				// 뛰기
	MSG_MOVE_PLACE,				// 좌표
	MSG_MOVE_STOP,				// 멈춤
#ifdef NIGHT_SHADOW
	MSG_MOVE_FLY,				// 날기
#endif //NIGHT_SHADOW
} MSG_MOVE_TYPE;


// 캐릭터 종류 관련
typedef enum __tagMsgCharType
{
	MSG_CHAR_UNKNOWN = -1,		// 알 수 없음
	MSG_CHAR_PC = 0,			// 플레이어 캐릭터
	MSG_CHAR_NPC,				// NPC
	MSG_CHAR_PET,				// 애완동물
	MSG_CHAR_ELEMENTAL,			// 엘리멘탈 소환수
	MSG_CHAR_APET,				// 공격형 애완동물
} MSG_CHAR_TYPE;


// DB 관련 메시지
typedef enum __tagMsgDBType
{
	MSG_DB_SUCCESS,				// 요청 처리 성공			: 
	MSG_DB_OK,					// 캐릭터 정보 읽기 성공	: zone(n)
	MSG_DB_CHAR_EXIST,			// 캐릭터 정보				: index(n) name(str) job(c) job2(c) hair(c) face(c) level(n) exp(ll) needexp(ll) sp(n) hp(n) maxHP(n) mp(n) maxMP(n) [wearing_index(n) wearing_plus(n)]:6 deletedelayremain(n)
								// deletedelay : time_t로 사용, 0이 아니면 삭제 대기중 캐릭터, 0이면 일반 캐릭터
	MSG_DB_CHAR_END,			// 캐릭터 정보 끝			: slot1remain(n) slot2remain(n) seed(n)
	MSG_DB_OTHER_SERVER,		// 다른 서버 존의 캐릭		: zone(n) ip(str) port(n)
} MSG_DB_TYPE;


// 채팅관련
typedef enum __tagMsgChatType
{
	MSG_CHAT_SAY,				// 일반 채팅
	MSG_CHAT_PARTY,				// 파티 대화
	MSG_CHAT_GUILD,				// 길드 대화
	MSG_CHAT_TRADE,				// 매매 대화
	MSG_CHAT_WHISPER,			// 귀엣말
	MSG_CHAT_SHOUT,				// 외치기
	MSG_CHAT_NOTICE,			// 운영자 알림 메시지 : 서버 군 내의 모든 캐릭에게 전달
	MSG_CHAT_GM,				// 운영자 채팅
	MSG_CHAT_MESSENGER,			// 메신저 채팅
	MSG_CHAT_LORD,				// 성주 공지
	MSG_CHAT_RANKER,			// 랭커 쳇
	MSG_CHAT_RANKER_CONN,		// 랭커 입장 job(c) name(str)
	MSG_CHAT_NOTICE_SERVERDOWN,	// 숫자
	MSG_CHAT_GM_WHISPER,
	MSG_CHAT_GM_SAY, 
	MSG_CHAT_GMTOOL,
	MSG_CHAT_NOTICE_PRESSCORPS,		// 기자단 공지
	MSG_CHAT_EXPEDITION,		// 원정대 채팅	
} MSG_CHAT_TYPE;


// 아이템 관련
typedef enum __tagMsgItemType
{
	MSG_ITEM_USE,				// 아이템 사용			: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) extra1(n)
	MSG_ITEM_TAKE,				// 아이템 줍기			: char_type(c) char_idx(n) item_idx(n)
	MSG_ITEM_THROW,				// 아이템 버리기		: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) count(ll)
	MSG_ITEM_ARRANGE,			// 아이템 정렬			: tab_idx(c)
	MSG_ITEM_DELETE,			// 아이템 제거			: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) count(ll)
	MSG_ITEM_WEAR,				// 아이템 착탈			: wear_pos(c) tab_idx1(c) row_idx1(c) col_idx1(c) item_idx1(n) tab_idx2(c) row_idx2(c) col_idx2(c) item_idx2(n)
	MSG_ITEM_SWAP,				// 아이템 위치 변경		: tab_idx(c) row_idx1(c) col_idx1(c) row_idx2(c) col_idx2(c)
	MSG_ITEM_ADD,				// 아이템 습득			: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) item_db_idx(n) wear_pos(c) plus(n) flag(n) used(n), used_2(n) count(ll) option_count(c) [option_type(c) option_level(c)] * option_count
								// COMPOSITE_TIME 사용시 used_2(n) 추가
	MSG_ITEM_UPDATE,			// 아이템 갱신			: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) plus(n) flag(n) used(n) used_2(n) count(ll) change_count(ll) option_count(c) [option_type(c) option_level(c)] * option_count
								// COMPOSITE_TIME 사용시 used_2(n) 추가
	MSG_ITEM_DROP,				// 10 아이템 드롭			: item_idx(n) item_db_idx(n) count(ll) x(f) z(f) h(f) r(f) ylayer(n) char_type(c) char_index(n)
	MSG_ITEM_APPEAR,			// 아이템 시야 안		: item_idx(n) item_db_idx(n) count(ll) x(f) z(f) h(f) r(f) ylayer(n)
	MSG_ITEM_DISAPPEAR,			// 아이템 시야 밖		: item_idx(n)
	MSG_ITEM_BUY,				// 아이템 구입			: npc_idx(n) price(ll) item_count(n) item_db_idx(n) count(ll)....반복
	MSG_ITEM_SELL,				// 아이템 판매			: npc_idx(n) price(ll) item_count(n) tab_idx(c) row_idx(c) col_idx(c) count(ll)...반복
	MSG_ITEM_UPGRADE_REQ,		// 아이템 업글 요청		: row_idx1(c) col_idx1(c) plus(n) row_idx2(c) col_idx2(c) level(n)
	MSG_ITEM_UPGRADE_REP,		// 아이템 업글 응답		: result(uc)
	MSG_ITEM_REFINE_REQ,		// 제련석 전환 요청		: row(c) col(c)
	MSG_ITEM_REFINE_REP,		// 제련석 전환 응답		: result(uc)
	MSG_ITEM_OPTION_ADD_REQ,	// 아이템 옵션 요청		: row_idx1(c) col_idx1(c) wear_pos(c) row_idx2(c) col_idx2(c)
	MSG_ITEM_OPTION_ADD_REP,	// 20 아이템 옵션 결과		: result(uc)
	MSG_ITEM_OPTION_DEL_REQ,	// 아이템 옵션 요청		: row_idx1(c) col_idx1(c) wear_pos(c) row_idx2(c) col_idx2(c)
	MSG_ITEM_OPTION_DEL_REP,	// 아이템 옵션 결과		: result(uc)
	MSG_ITEM_PROCESS_REQ,		// 아이템 가공 요청		: use_row_idx(c) use_col_idx(c) make_db_idx(n) material_count(n) [tab(c) row(c) col(c) count(ll)].. 반복
	MSG_ITEM_PROCESS_REP,		// 아이템 가공 결과		: result(uc)
	MSG_ITEM_MAKE_REQ,			// 아이템 제작 요청		: use_row_idx(c) use_col_idx(c) make_db_idx(n) material_count(n) [tab(c) row(c) col(c) count(ll)].. 반복
	MSG_ITEM_MAKE_REP,			// 아이템 제작 결과		: result(uc)
	MSG_ITEM_MIX_REQ,			// 악세사리 조합 요청:	: use_row_idx(c) use_col_idx(c) row_idx1(c) col_idx1(c) row_idx2(c) col_idx2(c) row_idx3(c) col_idx3(c)
	MSG_ITEM_MIX_REP,			// 악세사리 조합 결과	: result (uc)
	MSG_ITEM_GET,				// 아이템 가져감(표시)	: char_name(str) item_db_idx(n) item_count(ll)
	MSG_ITEM_ARCANE_REQ,		// 30 비밀의 상자 요청:	: use_row_idx(c) use_col_idx(c) row_idx1(c) col_idx1(c) row_idx2(c) col_idx2(c) row_idx3(c) col_idx3(c)
	MSG_ITEM_ARCANE_REP,		// 비밀의 상자 결과		: result (uc)
	MSG_ITEM_CHANGEWEAPON_REQ,	// 무기 교환 요청		: row(c) col(c) itemindex(n) changetype(n)
	MSG_ITEM_CHANGEWEAPON_REP,	// 무기 교환 응답		: old_itemdbindex(n) new_itemdbindex(n)
								// 돈이 모자라는 경우 old == -1
								// 잘못된 교환을 요청하면 new == -1
	MSG_ITEM_PROCESS_NPC,		// NPC를 통해 가공		: processItemDBIndex(n) count(n) errcode(n:s)
	MSG_ITEM_ADD_BOOSTER,		// 생산 도구에 부스터 부착 : flag(n)
								// 클라이언트->서버		: 부스터 아이템 서버 인덱스
								// 서버->클라이언트		: 오류코드
	MSG_ITEM_MIX_WARITEM,		// 공성 아이템 조합		: 클라이언트 -> 서버	: row_type1(c) col_type1(c) row_type2(c) col_type2(c) row_type3(c) col_type3(c)
								//						: 서버 -> 클라이언트	: errcode(n)
	MSG_ITEM_USE_PROLONG,		// c->s 아이템 사용기간 연장	: tab_idx(uc) row_idx(uc) col_idx(uc) item_idx(n) extra1(n)
								// s->c						?	: item_idx(n) busetime(n) bprolong(c)
	MSG_ITEM_USE_ERROR,			// errorcode(uc)

	MSG_ITEM_LEND_WEAPON,		// 무기 대여			: itemdbindex(n) errorcode(n:s)

	MSG_ITEM_LEVELDOWN,			// 40 레벨 학락		   :  row1(c) col1(c) row2(c) col2(c)
								// MSG_ITEM_LEVELDOWN_ERROR_TYPE : errorcode(n)

	MSG_ITEM_TARGET,			// 타켓하는 아이템 사용 : tab_idx(c) row_idx(c) col_idx(c) item_idx(n) chartype(c) charindex(n)

	MSG_ITEM_EXCHANGE_EQUIP_REQ,	// 무기 교환 요청		: row(c) col(c) itemindex(n) changetype(n)
	MSG_ITEM_EXCHANGE_EQUIP_REP,	// 무기 교환 응답		: old_itemdbindex(n) new_itemdbindex(n)


	MSG_ITEM_USE_GROCERY_BUY,		// 잡화상 아이템을 이용한 아이템 구매 : price(ll) item_count(n) item_db_idx(n) count(ll)....반복
	MSG_ITEM_USE_GROCERY_SELL,		// 잡화상 아이템을 이용한 아이템 판매 : price(ll) item_count(n) tab_idx(c) row_idx(c) col_idx(c) count(ll)...반복

	MSG_ITEM_USE_WARPDOC,		// 이동주문서 사용하기 zone(n), npcidx(n)
	MSG_ITEM_USE_PRESSCORPS,	// 라카기자단 확성기 subtype(c) >> item_idx(n) >> msg(str)
	
	MSG_ITEM_END_MSG,			// MSG_ITEM_TYPE 마지막 메시지
} MSG_ITEM_TYPE;

typedef enum _tagItemLendWeaponErrorType
{
	MSG_ITEM_LEND_WEAPON_ERROR_OK,				// 무기 대여 : 성공
	MSG_ITEM_LEND_WEAPON_ERROR_FULLINVEN,		// 무기 대여 : 인벤 공간 부족
	MSG_ITEM_LEND_WEAPON_ERROR_NOMONEY,			// 무기 대여 : 소지금 부족
	MSG_ITEM_LEND_WEAPON_ERROR_FAIL,			// 무기 대여 : 실패 ( 잘못된 DB 인덱스 요청등 클라이언트 버그 )
	MSG_ITEM_LEND_TIMEOUT,						// 무기 대여 : 종료
} MSG_ITEM_LEND_WEAPON_ERROR_TYPE;				

typedef enum _tagItemUseErrorType
{
	MSG_ITEM_USE_ERROR_REVIVAL_EXP,			// 부활 주문서 때문에 경험치 복구 주문서 사용 할수 없음
	MSG_ITEM_USE_ERROR_REVIVAL_SP,			// 부활 주문서 때문에 숙련도 복구 주문서를 사용할수 없음
	MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE, // 피스존이므로 몬스터 복구 주문서를 사용할수 없음
	MSG_ITEM_USE_ERROR_EXP_SP,				// 경험치 주문서 떄문에 숙련도 사용 못함
	MSG_ITEM_USE_ERROR_SP_EXP,				// 숙련도 때문에 경험치 사용 못함.
	MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL,	// 플래티늄 제련석 감정이 완료 
	MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE, // 사용기간이 만료 되어서 효과가 사라짐
	MSG_ITEM_USE_ERROR_EXPUP,				// 경험의 묘약이 사용중입니다. 그래도 사용하시겠습니까?
	MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK,	// 레어 아이템 옵션 확인 성공
	MSG_ITEM_USE_ERROR_IDENTIFY_RARE_FAIL,	// 레어 아이템 옵션 확인 실패
	MSG_ITEM_USE_ERROR_WARN_PARTYRECALL,	// 공성 중 성안 영역에서 파티리콜 사용시 일부 리콜 안됨을 경고
	MSG_ITEM_USE_ERROR_SUMMON_GUARD,		// 경비병소환 실패
} MSG_ITEM_USE_ERROR_TYPE;

typedef enum _tagItemMixWarItemErrorType
{
	MSG_ITEM_MIX_WARITEM_ERROR_OK,			// 조합 성공
	MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND,	// 재료 부족
} MSG_ITEM_MIX_WARITEM_ERROR_TYPE;

typedef enum _tagMSG_ITEM_LEVELDOWN_ERROR_TYPE	// 레벨 하락 주문서
{
	MSG_ITEM_LEVELDOWN_ERROR_OK,			// 성공
	MSG_ITEM_LEVELDOWN_ERROR_ALREADY,		// 이미 레벨 하락
	MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP,		// 장비 아님
	MSG_ITEM_LEVELDOWN_ERROR_SCROLL,		// 주문서 아님
}MSG_ITEM_LEVELDOWN_ERROR_TYPE;

typedef enum _tagItemAddBoosterErrorType
{
	MSG_ITEM_ADD_BOOSTER_ERROR_OK,		// 성공
	MSG_ITEM_ADD_BOOSTER_ERROR_USED,	// 내구도 부족
	MSG_ITEM_ADD_BOOSTER_ERROR_ALREADY,	// 이미 붙어 있음
} MSG_ITEM_ADD_BOOSTER_ERROR_TYPE;

typedef enum _tagItemProcessNPCErrorType
{
	MSG_ITEM_PROCESS_NPC_ERROR_OK,		// 성공
	MSG_ITEM_PROCESS_NPC_ERROR_FAIL,	// 실패
	MSG_ITEM_PROCESS_NPC_ERROR_COUNT,	// 카운트 오류
	MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE,	// 재료 부족
	MSG_ITEM_PROCESS_NPC_ERROR_MONEY,	// 나스 부족
} MSG_ITEM_PROCESS_NPC_ERROR_TYPE;

typedef enum _tagProcessResult
{
	MSG_PROCESS_FAIL_SYSTEM,			// 있어서는 안되는 이유로 실패
	MSG_PROCESS_FAIL_MATERIAL,			// 재료 정보 오류
	MSG_PROCESS_FAIL_NO_SSKILL,			// 가공 스킬 없음
	MSG_PROCESS_FAIL_OVER_WEIGHT,		// 무게 초과로 인한 제작 실패
	MSG_PROCESS_FAIL_PROB,				// 확률 실패
	MSG_PROCESS_SUCCESS,				// 가공 성공
} MSG_ITEM_PROCESS_RESULT;

typedef enum _tagMakeResult
{
	MSG_MAKE_FAIL_SYSTEM,				// 있어서는 안되는 이유로 실패
	MSG_MAKE_FAIL_MATERIAL,				// 재료 정보 오류
	MSG_MAKE_FAIL_NO_SSKILL,			// 제작 스킬 없음
	MSG_MAKE_FAIL_ITEM_LEVEL,			// 현재 스킬로 제작 불가능 아이템 (레벨)
	MSG_MAKE_FAIL_OVER_WEIGHT,			// 무게 초과로 인한 제작 실패
	MSG_MAKE_FAIL_PROB,					// 확률 실패
	MSG_MAKE_SUCCESS,					// 제작 성공
} MSG_ITEM_MAKE_RESULT;

typedef enum _tagItemMixResult
{
	MSG_MIX_FAIL_SYSTEM,				// 있어서는 안되는 오류
	MSG_MIX_FAIL_REMAKEBOX,				// 재활의 상자 정보 오류
	MSG_MIX_FAIL_ACCESSORY,				// 보낸 악세사리 아이템 정보 오류
	MSG_MIX_SUCCESS,					// 조합 성공
} MSG_ITEM_MIX_RESULT;

typedef enum _tagItemArcaneResult
{
	MSG_ARCANE_FAIL_SYSTEM,				// 있어서는 안되는 오류
	MSG_ARCANE_FAIL_ARCANEBOX,			// 비밀의 상자 정보 오류
	MSG_ARCANE_FAIL_MATERIAL,			// 재료 정보 오류 (재료가 위치에 없거나 착용중)
	MSG_ARCANE_FAIL_UPGRADE,			// 제련석 오류
	MSG_ARCANE_FAIL_ACCESSORY,			// 악세사리 오류
	MSG_ARCANE_FAIL_SAMPLE,				// 시료 오류
	MSG_ARCANE_SUCCESS,					// 조합 성공
} MSG_ITEM_ARCANE_RESULT;

typedef enum _tagItemOptionAddResult
{
	MSG_OPTION_ADD_ERROR_SYSTEM,		// 있어서는 안되는 오류
	MSG_OPTION_ADD_ERROR_WEAR,			// 착용한 아이템어야지
	MSG_OPTION_ADD_ERROR_KIND,			// 무기나 방어구만 가능하다
	MSG_OPTION_ADD_ERROR_ENABLE,		// 옵션을 붙일 수 있는 아이템이 아니다
	MSG_OPTION_ADD_ERROR_MAX,			// 최대 옵션 수 초과
	MSG_OPTION_ADD_SUCCESS,				// 옵션 붙이기 성공
	MSG_OPTION_BLOOD_SWEAT_FRUIT_ADD_SUCCESS //피와 땀의 결정을 사용하여 블러드 옵션 붙이기에 성공
} MSG_ITEM_OPTION_ADD_RESULT;

typedef enum _tagItemOptionDelResult
{
	MSG_OPTION_DEL_ERROR_SYSTEM,		// 있어서는 안되는 오류
	MSG_OPTION_DEL_ERROR_WEAR,			// 착용한 아이템어야지
	MSG_OPTION_DEL_ERROR_KIND,			// 무기나 방어구만 가능하다
	MSG_OPTION_DEL_ERROR_NUM,			// 옵션3개 이상 있어야 지울수 있다
	MSG_OPTION_DEL_SUCCESS,				// 옵션 지우기 성공
} MSG_ITEM_OPTION_DEL_RESULT;

typedef enum _tagFactoryType
{
	MSG_FACTORY_ERROR,					// 제작 오류				: errorcode(uc)
	MSG_FACTORY_LEARN,					// 아이템 제작기술 배우기	: factory_index(n), exp(ll)
	MSG_FACTORY_ITEM_LIST,				// 제작 가능 아이템 리스트	: seal_type(c), item_type(c), count(n), [factory_index(n) * n]
	MSG_FACTORY_ITEM_MAKE,				// 아이템 제작				: factory_index(n), skill_index(n), skill_exp(ll) 
} MSG_FACTORY_TYPE;

typedef enum _tagFactoryErrorType
{
	MSG_FACTORY_ERROR_SYSTEM,
	MSG_FACTORY_ERROR_ALREADY_REG,		// 이미 배움
	MSG_FACTORY_ERROR_SKILL,			// 필요한 스킬 없음
	MSG_FACTORY_ERROR_NAS,				// 나스 부족
	MSG_FACTORY_ERROR_EXP,				// 숙련도 부족
	MSG_FACTORY_ERROR_ITEM,				// 재료 아이템 없음
	MSG_FACTORY_ERROR_INVEN,			// 인벤 부족
} MSG_FACTORY_ERROR_TYPE;


typedef enum __tagMsgExAffinityType
{
	MSG_EX_AFFINITY_CONNECT_REQ,		// 세력 친구 맺기		: npcidx(n)
	MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REQ,	// 친화도 아이템 기부	: npcidx(n) clientPoint(n) item_count(n) tab_idx(c) row_idx(c) col_idx(c) count(n) 												
	MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REP,	// 친화도 아이템 기부 응답	: affinityidx(n) point(n)	
	MSG_EX_AFFINITY_SHOPUSE_REQ,		// 상점 이용요청		: npcidx(n) 
	MSG_EX_AFFINITY_SHOPUSE_REP,		// 상점 이용응답		: npcidx(n) 
	MSG_EX_AFFINITY_GIFTINFO_REQ,		// 다음보상정보 요청	: npcidx(n)
	MSG_EX_AFFINITY_GIFTINFO_REP,		// 다음보상정보 응답	: point(n) itemidx(n) item_count(n)
	MSG_EX_AFFINITY_GIFT_REQ,			// 선물받기 요청		: npcidx(n)
	MSG_EX_AFFINITY_GIFT_REP,			// 선물받기 응답	
	MSG_EX_AFFINITY_UPDATE_POINT,		// 포인트갱신		: affinityidx(n) poiny(n)
	MSG_EX_AFFINITY_LISTINFO,			// 친화도 리스트 정보	: affinity_count(n) affinityidx(n) point(n)
	MSG_EX_AFFINITY_ADDINFO,			// 친화도 추가 정보		: affinityidx(n)
	MSG_EX_AFFINITY_ERROR,				// 친화도관련 ERROR 메시지 : errortype(uc)
	MSG_EX_AFFINITY_REWARD_NOTICE,		// 보상 알림 : noticecount(c) npcidx(n)		[100322: selo] 추가함
}MSG_EX_AFFINITY_TYPE;

typedef enum __tagMsgExAffinityErrorType
{
	MSG_EX_AFFINITY_ERROR_CONNECT_SUCCESS,   // 친화도 연결 성공
	MSG_EX_AFFINITY_ERROR_CONNECT_NOMONEY,   // 나스부족
	MSG_EX_AFFINITY_ERROR_CONNECT_ALREADY,   // 이미세력 친구맺음
 
	MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MAX,  // 친화도 포인트 최대로 찼음
	MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MISSMATCH, // 기부 부적합 아이템
 
	MSG_EX_AFFINITY_ERROR_USESHOP_POINT,   // 상점이용 포인트 부족
	MSG_EX_AFFINITY_ERROR_GIFTINFO_NOITEM,   // 보상받을 아이템이 없다
	MSG_EX_AFFINITY_ERROR_GIFTINFO_NOPOINT,   // 포인트부족으로 보상 못받음
	MSG_EX_AFFINITY_ERROR_GIFT_NOSPACE,   // 인벤에 아이템 받을 공간이 없다.
	MSG_EX_AFFINITY_ERROR_GIFT_MAXWEIGHT,   // 인벤 무게 초과

	MSG_EX_AFFINITY_ERROR_NOT_CONNECTED,   // 친화도가 연결되어있지 않다.
}MSG_EX_AFFINITY_ERROR_TYPE;


typedef enum _tagMsgMsgrType
{
	MSG_MSGR_WHISPER_REQ,			// 귓말 요청		: sender_index(n) sender_name(str) receiver_name(str) chat(str)
	MSG_MSGR_WHISPER_REP,			// 귓말 응답		: sender_index(n) sender_name(str) receiver_name(str) chat(str)
	MSG_MSGR_WHISPER_TIMEOUT,		// 귓말 타임아웃	: sender_index(n) sender_name(str)
	MSG_MSGR_WHISPER_NOTFOUND,		// 대상 없음		: sender_index(n) sender_name(str)
	MSG_MSGR_GUILDCHAT,				// 길드 채팅		: sender_index(n) sender_name(str) receiver_guild_index(n) chat(str)
	MSG_MSGR_NOTICECHAT,			// 운영자 채팅		: chat(str)
	MSG_MSGR_LOGOUT_REQ,			// 로그아웃 요청	: id(str)
	MSG_MSGR_PLAYER_COUNT_REQ,		// PC 수 조사 요청	: req_char(n)
	MSG_MSGR_PLAYER_COUNT_REP,		// PC 수 조사 응답	: req_char(n) count_zone(n) [zone_num(n) pc_count(n) produce_count(n) shop_count(n) play_count(n)]
	MSG_MSGR_LOGOUT_REP,			// 로그아웃 응답	: success(c) id(str)
	MSG_MSGR_RESERVE_060406_1,		// 10
	MSG_MSGR_RESERVE_060406_2,		// 
	MSG_MSGR_ENDGAME_REQ,			// 전체 서버 다운	: 
	MSG_MSGR_OBSERVER,				// 관찰자 메시지 전달 : chat(str)
	MSG_MSGR_DOUBLE_EVENT_REQ,		// 더블 이벤트 요청	: start(c)
	MSG_MSGR_LATTO_EVENT_REQ,		// 라또 이벤트 요청	: start(c)
	MSG_MSGR_DOUBLE_EXP_EVENT_REQ,	// 경험치 두배 이벤트 요청 : char_index(n) cmd(n)
									// cmd				: 0: start, 1: ing, 2: stop
	MSG_MSGR_NEWYEAR_EVENT_REQ,		// 설날 이벤트 요청 : start(c)
	MSG_MSGR_VALENTINE_EVENT_REQ,	// 발렌타인데이 이벤트 요청 : start(c)
	MSG_MSGR_WHITEDAY_EVENT_REQ,	// 화이트데이 이벤트 요청 : start(c)
	MSG_MSGR_LETTER_EVENT_REQ,		// 20 화이트데이 이벤트 요청 : start(c)
	MSG_MSGR_DOUBLE_EXP_EVENT_REP,	// 경험치 두배 이벤트 응답 : char_index(n) server_no(n) subno(n) cmd(n)

	MSG_MSGR_MESSENGERCHAT_REQ,		// 메신저채팅 요청		: sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)
	MSG_MSGR_MESSENGERCHAT_REP,		// 메신저채팅 응답		: sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)
	MSG_MSGR_MESSENGERCHAT_NOTFOUND,
	MSG_MSGR_EVENT_REQ,				// 이벤트 통합		: thisserver(n) thissubno(n) charindex(n) eventcmd(str) flag(n)
	MSG_MSGR_CHAT_LORD,				// 성주 공지		: charindex(n) charname(n) chatmsg(str)

	MSG_MSGR_OPEN_BETA_EVENT_REQ,	// 태국 오픈베타 이벤트 char_index(n) cmd(n)
									// cmd				: 0: start, 1: ing, 2: stop
	MSG_MSGR_OPEN_BETA_EVENT_REP,	//	: char_index(n) server_no(n) subno(n) cmd(n)
	MSG_MSGR_FLOWER_EVENT_REQ,		// 태국 플라워 이벤트 : 오픈베타와 같음
	MSG_MSGR_FLOWER_EVENT_REP,		// 30

	MSG_MSGR_MARGADUM_PVP_REQ,		// 태국 : 마르가둠 PVP(위와 같은 포멧) cmd :  0: free 1:16~25, 2: 26~35, 3: 36~
	MSG_MSGR_MARGADUM_PVP_REP,		// 

	MSG_MSGR_DOUBLE_EVENT_REP,		// 더블이벤트 응답 

	MSG_MSGR_EVENT_OX,				// OX 이벤트		: subtype(n) ...

	MSG_MSGR_REBOOT_REQ,			// 서버 리붓 메세지

	MSG_MSGR_RECOMMEND,				// 추천 서버 설정		: gmcharindex(n) recommendserver(n)
	MSG_MSGR_RECOMMEND_MOONSTONE,	// 추천 서버 문스톤 당첨: charindex(n) charname(str)

	MSG_MSGR_MESSENGER_ADD_CONFIRM,	// 메신저 대화방/ 맴버 추가 : makeCharIndex(n) chatIndex(n) charName(str)
	MSG_MSGR_MESSENGER_ADD,			// 메신저 대화방/ 맴버 추가 : makeCharIndex(n) chatIndex(n) charName(str)
	MSG_MSGR_MESSENGER_DEL,			// 40 메신저 대화방/ 맴버 삭제 : makeCharIndex(n) chatIndex(n) charName(str)
	MSG_MSGR_MESSENGER_CHAT,			// 메신저 대화방/ 맴버 삭제 : makeCharIndex(n) chatIndex(n) chat(str)

	MSG_MSGR_EVENT_GOLDENBALL,		// 골든볼 이벤트	: subtype(n) ...

	MSG_MSGR_LOGIN_SERVER,			// 로그인 서버 제어 : code(n)
									// code : 0-close, 1-open, 2-reboot
	MSG_MSGR_MESSENGER_ONE_VS_ONE,	// 1:1 채팅 : sCharindex(n), tCharindex(n) chatColor(n) sName(str) chat(str)

	MSG_MSGR_EVENT_SETTING_REQ,		// 이벤트 세팅 메시지

	MSG_MSGR_CHANCE_EVENT_REQ,		// 찬스 이벤트 요청
	MSG_MSGR_CHANCE_EVENT_REP,		// 찬스 이벤트 적용

	MSG_MSGR_GMTOOL_KICKID_REQ,		// 지엠툴 킥요청
	MSG_MSGR_GMTOOL_KICKID_REP,
	MSG_MSGR_GMTOOL_CHATMONITOR_REQ,	//  50 지엠툴 캐릭터 감시 요청
	MSG_MSGR_GMTOOL_CHATMONITOR_REP,
	MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE,
	MSG_MSGR_GMTOOL_CHATMONITOR_OFF,
	MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_PARTY,
	MSG_MSGR_GMTOOL_CHATMONITOR_PARTY_OFF,
	MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_GUILD,
	MSG_MSGR_GMTOOL_CHATMONITOR_GUILD_OFF,
	MSG_MSGR_GM_WHISPER_REQ,
	MSG_MSGR_GM_WHISPER_REP,
	MSG_MSGR_GM_WHISPER_NOTFOUND,	// 60
	MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_WHISPER,
	MSG_MSGR_GM_LOOPCOUNT_REQ,
	MSG_MSGR_GM_LOOPCOUNT_REP,
	MSG_MSGR_GM_RELOADGMIP_REP,
	MSG_MSGR_GMTOOL_SILENCE_REQ,
	MSG_MSGR_GMTOOL_SILENCE_REP,
	MSG_MSGR_GMTOOL_WHIPER_REQ,
	MSG_MSGR_GMTOOL_WHISPER_NOTFOUND,
	MSG_MSGR_GMTOOL_WHISPER_REP,

	MSG_MSGR_DOUBLE_ITEM_EVENT_REQ,		// 70 더블 이벤트 요청	: start(c)
	MSG_MSGR_DOUBLE_ITEM_EVENT_REP,		// 더블 이벤트 응답
	
	MSG_MSGR_DOUBLE_PET_EXP_EVENT_REQ,	// 펫 경험치 두배 이벤트 요청 : char_index(n) cmd(n)
	MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP,	// 펫 경험치 두배 이벤트 응답 : char_index(n) server_no(n) subno(n) cmd(n)

	MSG_MSGR_DOUBLE_ATTACK_EVENT_REQ,	// 물리, 마법 공격력 두배 이벤트 요청 : char_index(n) cmd(n)
	MSG_MSGR_DOUBLE_ATTACK_EVENT_REP,	// 물리, 마법 공격력 두배 이벤트 응답 : char_index(n) server_no(n) subno(n) cmd(n)

	MSG_MSGR_GMTOOL_KICK_REQ,
	MSG_MSGR_GMTOOL_KICK_REP,

	MSG_MSGR_GMTOOL_SHUTDOWN,

	MSG_MSGR_EVENT_DROPITEM_REQ,
	MSG_MSGR_EVENT_DROPITEM_REP,		// 80
	
	MSG_MSGR_SET_EXPLIMIT,			// 경험치 제한 셋팅 (n)Server (n)limit

	MSG_MSGR_DOUBLE_EVENT_AUTO_REQ, // 더블 이벤트 자동
	MSG_MSGR_DOUBLE_EVENT_AUTO_REP, // 더블 이벤트 자동

	MSG_MSGR_UPGRADE_EVENT_REQ,		// 고급제련석 확률상승
	MSG_MSGR_UPGRADE_EVENT_REP,		// 고급제련석 확률상승

	MSG_MSGR_END_MSG,				// 그 어떤 메신저 메시지는 이보다 작아야 한다
} MSG_MSGR_TYPE;

typedef enum _tagMsgMsgrEventGoldenballType
{
	MSG_MSGR_EVENT_GOLDENBALL_VOTE,	// 골든볼 이벤트 응모 설정	: gmcharindex(n) team1(str) team2(str) year(n) month(n) day(n) hour(n) minute(n)
	MSG_MSGR_EVENT_GOLDENBALL_GIFT,	// 골든볼 이벤트 보상 설정	: gmcharindex(n) team1(str) team1score(n) team2(str) team2score(n) year(n) month(n) day(n) hour(n) minute(n) endVoteTime(n)
	MSG_MSGR_EVENT_GOLDENBALL_END,	// 골든볼 이벤트 종료		: gmcharindex(n) 
} MSG_MSGR_EVENT_GOLDENBALL_TYPE;

typedef enum _tagMsgMsgrEventOXType
{
	MSG_MSGR_EVENT_OX_SET_START,	// 시작설정				: gmcharindex(n) year(n) mon(n) day(n) hour(n) min(n)
	MSG_MSGR_EVENT_OX_END,			// 종료					: gmcharindex(n)
	MSG_MSGR_EVENT_OX_QUIZ,			// 문제					: gmcharindex(n) quizno(n) sec(n)
} MSG_MSGR_EVENT_OX_TYPE;

typedef enum _tagMsgSysType
{
	MSG_SYS_WHISPER_NOTFOUND,		// 귓말 대상 없음		:
	MSG_SYS_TIMEOUT,				// 타임 아웃 발생		:
	MSG_SYS_CANNOT_WEAR,			// 착용 오류			:
	MSG_SYS_FULL_INVENTORY,			// 인벤토리 가득 참		: tab_index(c)
	MSG_SYS_CUSTOM,					// 커스텀 메시지		: msg(str)
	MSG_SYS_SHORT_MONEY,			// 돈 사용시 모질람		:
	MSG_SYS_SHOP_CANCEL,			// 상점 거래 취소		:
	MSG_SYS_EXCHANGE_NOTFOUND,		// 교환 대상 없음		:
	MSG_SYS_EXCHANGE_ALREADY,		// 이미 교환 중			:
	MSG_SYS_NOT_OWNER_ITEM,			// 아이템 우선권이 없음
	MSG_SYS_OVER_WEIGHT,			// 10 무게가 오바 됬음
	MSG_SYS_UPGRADE_NOCONDITION,	// 조건이 안맞아 업그레이드 못함 (일반제련석 레벨이 맞지 않음)
	MSG_SYS_MAKE_REFINE_KIND,		// 무기나 방어구가 아닌 다른걸 제련석으로 전환하려할 때
	MSG_SYS_MAKE_REFINE_WEARING,	// 입고 있는 아이템으로 제련석 전환 불가능
	MSG_SYS_MAKE_REFINE_PLUS,		// Plus 값이 셋팅되어 있는 아이템 제련석 전환 불가능
	MSG_SYS_MEMPOS_CANT_WRITE,		// 장소 기억 장치 사용 불가능 존
	MSG_SYS_LAYERDUNGEON_FULL,		// 레이어 존 가득 차서 입장 불가
	MSG_SYS_CANT_WARP_STATE,		// 이동 장치 불가능 : 캐릭터 상태(사망/앉기/스킬시전/파티신청/교환)
	MSG_SYS_CANT_WARP_OTHER,		// 이동 장치 불가능 : 다른장치이용
	MSG_SYS_MEMPOS_OTHERZONE,		// 다른 존으로는 장소 기억 이동 불가
	MSG_SYS_SINGLEDUNGEON_FULL,		// 20 싱글던전 꽉 차다
	MSG_SYS_MAKE_REFINE_SHORT,		// 제련석 만들때 돈 모질람
	MSG_SYS_PRODUCE_NOTOOL,			// 생산도구가 없음
	MSG_SYS_PRODUCE_WEAR_TOOL,		// 생산도구를 착용하시오
	MSG_SYS_PRODUCE_MATCH_TOOL,		// 맞는 생산도구를 착용하시오
	MSG_SYS_PRODUCE_MAXUSE_TOOL,	// 생산도구 다 썼음
	MSG_SYS_PRODUCE_MAX_ACCEPTABLE,	// 생산 가능 인원 초과
	MSG_SYS_PRODUCE_NO_SSKILL,		// 해당하는 생산 스킬이 없다!!
	MSG_SYS_PROCESS_DOC_NOT_SSKILL,	// 가공문서 쓰는데 가공스킬이 없음
	MSG_SYS_WARNING_WEIGHT,			// 무게 초과 경고
	MSG_SYS_BLOODITEM,				// 30 블러드 옵션 가능 상태	: itemdbindex(n)
	MSG_SYS_MAKE_REFINE_CANNOT,		// 대상 아이템은 제련석 교환 불가능
	MSG_SYS_PKMODE_LIMITLEVEL,		// PK 선언이 불가능한 레벨
	MSG_SYS_CANTCHAT_INVISIBLE,		// 인비저블 상태에서는 일반 채팅 불가
	MSG_SYS_SHORT_FAME,				// 명성치 부족
	MSG_SYS_FRIEND_NOTFOUND,        // 메신저 상대가 접속해있지 않음. 
	MSG_SYS_UPGRADE_CANT_SUPERSTONE,// 슈퍼고제를 이미 사용한 아이템//0627
	MSG_SYS_UPGRADE_CANT_14LEVEL,   // 14레벨 아이템에는 슈퍼고제를 사용못함
	MSG_SYS_FAME_NOT_ENOUGH,		// 명성치 부족해서 슈퍼 고제 지급받지 못함
	MSG_SYS_INVEN_NOT_ENOUGH,		// 인벤이 가득차서 슈퍼 고제 지급받지 못함
	MSG_SYS_NO_MORE_SUPERSTONE,		// 40 더이상 슈퍼 고제를 지급받지 못함(7개 다 받은 상태)
	MSG_SYS_CAN_RECEIVE_SUPERSTONE,	// 특정 명성치에 도달해서 슈퍼고제를 받을수 있음.
	MSG_SYS_APPEAR_NOVICE,			// 초보자가 접속했음.
	MSG_SYS_NOTICE,					// 게임내 이벤트 공지//0704
	MSG_SYS_MEMPOS_CASTLE,			// 공성도중에는 공성지역으로 존이동 불가
	MSG_SYS_PET_CANT_SUMMON,		// 소환 불가능 지역
	MSG_SYS_PET_CANT_MOUNT,			// 마운트 불가능 지역
	MSG_SYS_PRODUCE_NO_MONEY,		// 돈없어서 생산 불가
	MSG_SYS_PRODUCE_CANNT_BOOST,	// 부스터로는 랜덤 생산 불가
	MSG_SYS_INVALID_CHAR,			// 잘못된 문자가 포함되었습니다
	MSG_SYS_CANNOT_DUPLICATION,		// 50 아이템 중복 사용 불가 : new_item(n) cur_item(n)
	MSG_SYS_UPGRADE_CANT_6LEVEL_LUCKY, // 행운의 제련석은 6이상의 아이템에 발를수 없다.
	MSG_SYS_PET_NOT_HAVE_ONEMORE,	// 하나 이상의 펫을 소유할수 없습니다.
	MSG_SYS_TARGET_PKLIMITLEVEL,	// 타겟이 PVP 레벨이 되지 않아서 공격할수 없다
	MSG_SYS_NOT_ADULT,				// 성인이 아니면 10시에 접속한다 메세지
	MSG_SYS_NOTTIME_GOZONE,			// 지금은 이동할수 있는 시간이 아닙니다.
	MSG_SYS_CANNOT_WEAR_DEADPET,	// 060221 : bs : 사망한 펫 착용 불가 : petindex(n) remainRebirth(n)
	MSG_SYS_ITEMCOMPOSITED,			// 아이템 합성 성공
	MSG_SYS_EXPIRE_ITEMCOMPOSITE,	// 아이템 합성 만료 : itemdbindex(n)
	MSG_SYS_CANNOT_RESET_MOUNT,		// 펫 마운트 타입 취소 불가
	MSG_SYS_RESET_MOUNT,			// 60 펫 마운트 타입 취소
	MSG_SYS_CANNOT_SEPARATE_ITEM,	// 결합된 의상이 아닌 아이템 분리 요청시
	MSG_SYS_SEPARATE_ITEM,			// 결합된 의상 해제 : itemdbindex(n)
	MSG_SYS_RAIDMOB_REGEN,			// 레이드 몬스터 출현 알림 : npcdbindex(n) x(f) z(f)
	MSG_SYS_TIMEOUT_LUCKY_ACCESSORY,// 럭키 악세서리 사용기간 만료 : itemdbindex(n)
	MSG_SYS_CHANGE_LUCKY_BOX,		// 럭키 드로우 상자 교환 성공 : itemdbindex(n)
	MSG_SYS_CANNOT_HAVE_DUPLICATE,	// 아이템 중복해서 가질 수 없음
	MSG_SYS_MATCHSEX,				// 성별이 맞지 않습니다.  [3/8/2007 KwonYongDae] EVENT_WHITEDAY_2007

	MSG_SYS_MEMPOSPLUS_OPEN,		// 프리미엄 메모리북 개봉
	MSG_SYS_PKMODE_CANDO,			// PK가 가능한 레벨이 되었습니다.
	MSG_SYS_NO_AUTO_ITEM,			// 70 사용할수 있는 포션이 없습니다.
	MSG_SYS_EGEHA_MINLEVEL,			// 에게하 이동할수 있는 최소레벨
	MSG_SYS_IRISPOINT_ALARM,		// 아이리스 매달을 %d 개 획득하였습니다. (n)Count
	MSG_SYS_FACTORY_EXP,			// 숙련도가 xxx상승하였습니다. (ll)sealskill_exp
	MSG_SYS_END_MSG,				// MSG_SYS_TYPE의 마지막 메시지
} MSG_SYS_TYPE;

typedef enum _tagMsgExchangeType
{
	MSG_EXCHANGE_REQ,			// 교환 행동 		: req_type(uc) src_index(n) src_name(str) dest_index(n) dest_name(str)
	MSG_EXCHANGE_ITEM,			// 아이템 등록/변경	: action(uc) item_index(n) item_db_index(n) plus(n) flag(n) used(n) used_2(n) count(ll)
								// COMPOSITE_TIME 사용시 used_2(n) 추가
} MSG_EXCHANGE_TYPE;

typedef enum _tagMsgExchangeReqType
{
	MSG_EXCHANGE_REQ_REQ,			// 교환 요청	
	MSG_EXCHANGE_REQ_REP,			// 교환 수락
	MSG_EXCHANGE_REQ_REJECT,		// 교환 거절/취소
	MSG_EXCHANGE_REQ_REJECT_SRC,	// 교환 거절/취소 - 요청한 사람
	MSG_EXCHANGE_REQ_REJECT_DEST,	// 교환 거절/취소 - 요청받은 사람
	MSG_EXCHANGE_REQ_READY,			// 준비 완료
	MSG_EXCHANGE_REQ_READY_SRC,		// 준비 완료 - 요청한 사람
	MSG_EXCHANGE_REQ_READY_DEST,	// 준비 완료 - 요청받은 사람
	MSG_EXCHANGE_REQ_FULL_SRC,		// 인벤토리 오류 - 요청한 사람
	MSG_EXCHANGE_REQ_FULL_DEST,		// 인벤토리 오류 - 요청받은 사람
	MSG_EXCHANGE_REQ_OK,			// 확인
	MSG_EXCHANGE_REQ_OK_SRC,		// 확인 - 요청한 사람
	MSG_EXCHANGE_REQ_OK_DEST,		// 확인 - 요청받은 사람
	MSG_EXCHANGE_REQ_SRC_PET_ANYMORE,		// 펫 갯수 오류 - 요청한 사람
	MSG_EXCHANGE_REQ_DEST_PET_ANYMORE,		// 펫 갯수 오류 - 요청받은 사람
} MSG_EXCHANGE_REQ_TYPE;

typedef enum _tagMsgExchangeItemAction
{
	MSG_EXCHANGE_ITEM_ADD,			// 아이템 추가 - 모든 필드 사용
	MSG_EXCHANGE_ITEM_ADD_SRC,		// 아이템 추가 - 요청한 사람, 모든 필드 사용
	MSG_EXCHANGE_ITEM_ADD_DEST,		// 아이템 추가 - 요청받은 사람, 모든 필드 사용
	MSG_EXCHANGE_ITEM_DEL,			// 아이템 제거 - ACTION/ITEM INDEX/COUNT
	MSG_EXCHANGE_ITEM_DEL_SRC,		// 아이템 제거 - 요청한 사람, ACTION/ITEM INDEX/COUNT 사용
	MSG_EXCHANGE_ITEM_DEL_DEST,		// 아이템 제거 - 요청받은 사람, ACTION/ITEM INDEX/COUNT 사용
	MSG_EXCHANGE_ITEM_UPDATE_SRC,	// 아이템 수량 변경 - 요청한 사람, ACTION/ITEM INDEX/COUNT 사용
	MSG_EXCHANGE_ITEM_UPDATE_DEST,	// 아이템 수량 변경 - 요청받은 사람, ACTION/ITEM INDEX/COUNT 사용
} MSG_EXCHANGE_ITEM_ACTION;

typedef enum _tagMsgMemposType
{
	MSG_MEMPOS_LIST,			// 기억장소 리스트 전달	: count(uc) [slot(c) zone(n) x(f) z(f) comment(str)] 반복 busetime(n)
	MSG_MEMPOS_WRITE,			// 기억장소 쓰기		: slot(c) comment(str) zone(n) x(f) z(f)
	MSG_MEMPOS_MOVE,			// 기억 장소로 이동		: slot(c)
} MSG_MEMPOS_TYPE;

typedef enum _tagMsgEnvType
{
	MSG_ENV_TAX_CHANGE,			// 세율 변경	: zone(n) sellrate(n) buyrate(n)
	MSG_ENV_WEATHER,			// 날씨 변경	: weather(n)
	MSG_ENV_TIME,				// 시간 알림	: year(n) mon(c) day(c) hour(c) min(c) sec(c)
} MSG_ENV_TYPE;

typedef enum _tagMsgGMType
{
	MSG_GM_WHOAMI,				// 운영자 레벨 확인 : level(uc)
	MSG_GM_COMMAND,				// 운영자 명령	: command(str)
} MSG_GM_TYPE;

typedef enum _tagMsgPartyType
{
	MSG_PARTY_INVITE,			// 파티 초대			: party_type(c) src_index(n) src_name(str) : 클라이언트에서 서버로 올때는 dest의 정보
	MSG_PARTY_ALLOW,			// 초대 수락			: 
	MSG_PARTY_REJECT,			// 초대 거절/취소 요청	: 
	MSG_PARTY_REJECT_SRC,		// 초대 거절/취소 응답	: 
	MSG_PARTY_REJECT_DEST,		// 초대 거절/취소 응답	: 
	MSG_PARTY_ADD,				// 파티원 추가			: is_boss(c) char_index(n) char_name(str) job(c) job2(c) level(n) cur_hp(n) max_hp(n) cur_mp(n) max_mp(n) x(f) z(f) ylayer(c) zone(n)
	MSG_PARTY_QUIT,				// 파티 탈퇴			: char_index(n)
	MSG_PARTY_KICK,				// 파티원 추방			: char_index(n)
	MSG_PARTY_END,				// 파티 해체			: 
	MSG_PARTY_MEMBER_INFO,		// 파티원 정보			: char_index(n) level(n) cur_hp(n) max_hp(n) cur_mp(n) max_mp(n) x(f) z(f) h(f) r(f) ylayer(c) zone(n)
	MSG_PARTY_ERROR,			// 파티 관련 에러		: errorcode(c)
	MSG_PARTY_ASSISTINFO,		// 파티원 보조 효과 정보: char_index(n) assist_state(n:s) assist_count(c:s) [itemidx(n:s) index(n:s) level(c:s) remain(n:s)]:count
	MSG_PARTY_CHANGEBOSS,		// 파티장 위임	C->S	: charname(str)
								// S->C			        : bossname(str) newbossindex(n) newname(str) bmandate(c)
	MSG_PARTY_EMPTY,			// 파티 없음			: 
	MSG_PARTY_CHANGETYPE,		//						: party_type(c)

	MSG_PARTY_ENDPARTY_REQ,			// 파티 해제 요청
	MSG_PARTY_INZONE_CLEAR_REQ,		// 파티 인스턴트 존 초기화

} MSG_PARTY_TYPE;

typedef enum _tagMsgPartyErrorType
{
	MSG_PARTY_ERROR_NOTFOUND,			// 대상이 없음			:
	MSG_PARTY_ERROR_ALREADY_ME,			// 내가 이미 파티 중	:
	MSG_PARTY_ERROR_FULL,				// 파티 가득참			:
	MSG_PARTY_ERROR_ALREADY_TARGET,		// 대상이 이미 파티 중	:
	MSG_PARTY_ERROR_ALREADY_REQUEST,	// 이미 파티 신청 중	:
	MSG_PARTY_ERROR_DIFF_TYPE,			// 파티 형태가 틀림		:
	MSG_PARTY_ERROR_NOT_BOSS,			// 파티장이 아니라서 수행 불가
	MSG_PARTY_ERROR_INVALID_LEVEL,      // 레벨이 맞지 않아 파티 불가
	MSG_PARTY_ERROR_NOT_PARTY,			// 파티중이 아닙니다
	MSG_PARTY_ERROR_NOT_PARTY_MEMBER,	// 파티원이 아닙니다.
	MSG_PARTY_ERROR_PVP,				// PVP 중에는 파티 불가
} MSG_PARTY_ERROR_TYPE;


typedef enum _tagMsgQuickSlotType
{
	MSG_QUICKSLOT_LIST,			// 퀵슬롯 리스트		: page(uc) [slottype(c) skilltype(n) or actiontype(n) or  row(c) col(c)] 10회반복
	MSG_QUICKSLOT_ADD,			// 퀵슬롯 추가, 삭제	: page(uc) slot(uc) slottype(c) skilltype(n) or actiontype(n) or row(c) col(c)
	MSG_QUICKSLOT_SWAP,			// 퀵슬롯 스왑			: page(uc) slotNum1(uc) slotNum2(uc)
} MSG_QUICKSLOT_TYPE;

typedef enum _tagMsgConnType
{
	MSG_CONN_LOGIN_REQ,			// 로그인 요청			: id(str) pw(str) server(n) playmode(uc) ip(str)
	MSG_CONN_LOGIN_REP,			// 로그인 응답			: id(str) error_code(uc) usertype(uc) timestamp(n) paytype(uc) location(uc) timeremain(n)
	MSG_CONN_LOGOUT_REQ,		// 로그아웃 요청		: id(str)
	MSG_CONN_BLOGIN_REQ,		// 빌링 로그인 요청		: id(str) ip(str)
	MSG_CONN_BLOGOUT_REQ,		// 빌링 로그아웃 요청	: id(str)
	MSG_CONN_PLAYING,			// 상태 변경			: id(str) mode(uc) zone(n)
	MSG_CONN_PLAYER_REQ,		// 서버군별 유저수 요청	: 
	MSG_CONN_PLAYER_REP,		// 서버군별 유저수 응답	: serverno(n) count(n) [subno(n) usercount(n) ip(str) port(n)]:count
	MSG_CONN_REBOOT_REQ,		// 서버군 서버 종료		: 
	MSG_CONN_PREOPEN_GIFT,		// 프리오픈 이벤트		: GS->CS: userindex(n) charindex(n) cancel(c)
								//						: CS->GS: userindex(n) charindex(n) errorcode(c)
	MSG_CONN_LEVELUP_POINT,		// 10 레벨업 포인트		: userindex(n) point(n)
	MSG_CONN_STASH_STATE,		// 창고 상태 확인		: userindex(n) charindex(n) issetpassword(c:cs) issealed(c:cs)
	MSG_CONN_STASH_CHECK_PASSWORD,	// 창고 암호 확인	: userindex(n) charindex(n) password(str) valid(c:cs)
									// valid			: 0: 성공, 1: 이전 암호 틀림, 2: 잠김
	MSG_CONN_STASH_CHANGE_PASSWORD,	// 창고 암호 변경	: userindex(n) charindex(n) old(str) new(str) error(c:cs)
									// error			: 0: 성공, 1: 이전 암호 틀림, 2: 잠김
	MSG_CONN_STASH_SEAL,		// 창고 잠금			: userindex(n) charindex(n) valid(c:cs)
								// valid				: 0: 성공, 1: 이미 잠김, 2: 그외 오류
	MSG_CONN_STASH_DELETE_PASSWORD, // 창고 암호 삭제	: userindex(n) charindex(n) valid(c:cs)
									//					: 0: 성공, 1: 민증번호틀림
	MSG_CONN_STASH_SET_NEW_PASSWORD,// 창고 암호 변경	: userindex(n) charindex(n) new(str) error(c:cs)
									// error			: 0: 성공, 1: 실패, 2: 삭제

	MSG_CONN_CASHITEM_BALANCE_REQ,	// 총 캐쉬 요청	
	MSG_CONN_CASHITEM_BALANCE_REP,  // 응답				: userid(str) errorcode(c) cashBalance(n)
	MSG_CONN_CASHITEM_PURCHASE_REQ,	// 유료아이템 구매	: userid(str) charindex(n) cash(n) count(n) ctid(n)
	MSG_CONN_CASHITEM_PURCHASE_REP,	// 20 응답				: userid(str) errorcode(c) cashBalance(n)
	MSG_CONN_CASHITEM_BRING_REQ,	// 유료아이템 가져오기: userindex(n) charindex(n) count(n) idx(n)
	MSG_CONN_CASHITEM_BRING_REP,	// 응답				: charindex(n) errorCode(uc) count(n) ctid(n)
	MSG_CONN_CASHITEM_PURCHASELIST_REQ, // 구입목록     : userindex(n) charindex(n)
	MSG_CONN_CASHITEM_PURCHASELIST_REP, // 응답			: charindex(n) errorCode(uc) count(n) idx(n) ctid(n)
	MSG_CONN_CASHITEM_PURCHASEHISTORY_REQ,//구매내역	: userindex(n) charindex(n) y(n) m(c) d(c)
	MSG_CONN_CASHITEM_PURCHASEHISTORY_REP,//응답		: charindex(n) errorcode(uc) count(n) ctid(n)

	MSG_CONN_EVENT_2PAN4PAN_LETTER_REQ,				// 2판4판 리뉴얼 문자 이벤트 요청 : userindex(n) charindex(n) MSG_EVENT_2PAN4PAN_WORD_TYPE(n)
	MSG_CONN_EVENT_2PAN4PAN_LETTER_REP,				// 2판4판 리뉴얼 문자 이벤트 응답 : userindex(n) charindex(n) MSG_EVENT_2PAN4PAN_WORD_TYPE(n)
	MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REQ,	// 2판4판 리뉴얼 문자 이벤트 롤백 요청 : userindex(n) charindex(n) MSG_EVENT_2PAN4PAN_WORD_TYPE(n)
	MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REP,	// 30 2판4판 리뉴얼 문자 이벤트 롤백 응답 : userindex(n) charindex(n) MSG_EVENT_2PAN4PAN_WORD_TYPE(n)

	MSG_CONN_EVENT_2PAN4PAN_GOODS_REQ,				// 2판4판 리뉴얼 아이템 이벤트 지급 요청 : userindex(n) charindex(n)
	MSG_CONN_EVENT_2PAN4PAN_GOODS_REP,				// 2판4판 리뉴얼 아이템 이벤트 지급 응답 : userindex(n) charindex(n) count(n) [goodindex(n) itemdbindex(n) itemcount(ll)]*count
													// count는 최대 20
	MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT,			// 2판4판 리뉴얼 아이템 이벤트 지급 확정 : userindex(n) charindex(n) count(n) [goodindex(n)]*count
													// count는 최대 20

	MSG_CONN_EVENT_2PAN4PAN_BOX_REQ,				// 2판4판 상자 이벤트 지급 요청 : userindex(n) charindex(n)
	MSG_CONN_EVENT_2PAN4PAN_BOX_REP,				// 2판4판 상자 이벤트 지급 응답 : userindex(n) charindex(n) count(n) [goodindex(n) itemdbindex(n) itemcount(ll)]*count
													// count는 최대 20
	MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT,				// 2판4판 상자 이벤트 지급 확정 : userindex(n) charindex(n) count(n) [goodindex(n)]*count
													// count는 최대 20

	MSG_CONN_EVENT_2PAN4PAN_MONEY_REQ,				// 2판4판 머니 이벤트 요청		: userindex(n) charindex(n) itemindex(n)
	MSG_CONN_EVENT_2PAN4PAN_MONEY_REP,				// 2판4판 머니 이벤트 응답		: userindex(n) charindex(n) itemindex(n)

	MSG_CONN_RANKER_REQ,				// 랭커 인지 읽어 오기 : username(n) charindex(n)
	MSG_CONN_RANKER_REP,				// 40 랭커 응답		   : errorCode(uc) brank(c)

	MSG_CONN_COUPON_CONFIRM,		// 쿠폰번호 확인	: charindex(n) coupon(str)
									// CS->GS			: charindex(n) errorCode(c) : eventerror사용
	MSG_CONN_COUPON_USE,			// 쿠폰 사용		: charindex(n) couponIdx(n)
									// CS->GS			: charindex(n) errorCode(c) : eventerror사용
	MSG_CONN_PCBANG_ITEM_GIVE,		// 등록된 PC방 접속 케릭터에게 아이템 지급
									// CS->GS			: userindex(n) type(c)
	MSG_CONN_LCTIME,				// CS->GS 라카력 	: lctime(n) lcstartime(n)

	MSG_CONN_MOVESERVER_OK,			// 서버 이동을 완료하기 위해 컨넥터->서버 메시지 : userindex(n)
	
	MSG_CONN_LIMIT_CATALOG,			// 리미트가 팔린 상품의 갯수를 각 게임서버로 보냄  CS->GS : count(n) ctid(n) sell(n)
									//							   팔 상품 갯수를 세팅 GS->CS : ctid(n) sell(n)
	MSG_CONN_CASHITEM_GIFT,				// 선물 보내기 요청	: sendcharName(str) Msg(str) recvuserIndex(n) recvcharName(str) count(n) idx(n) ctid(n)
										//			응답	: errcode(uc) 
	MSG_CONN_CASHITEM_GIFT_SENDHISTORY,  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
										 // 응답				: errorcode(uc) count(n) ctid(n) recvcharName(str)
	MSG_CONN_CASHITEM_GIFT_RECVHISTORY,  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
										 // 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	MSG_CONN_CASHITEM_GIFT_RECVLIST,     // 50 선물 받은 리스트 요청
										 // 응답				: listflag(c) count(n) idx(n) ctid(n) date(un) sendcharName(str) msg(str)
	MSG_CONN_CASHITEM_GIFT_RECV,		// 선물 받기		: count(n) idx(n) ctid(n)
										// 선물 받기		: errCode(uc)

	MSG_CONN_EVENT_PARTNER_CODENEO_REQ,	// 파트너 이벤트 codeNEO 신규 가입자 결과 보내기

	MSG_CONN_OPEN_BETA_GIFT,		// 오픈 베타 이벤트		: GS->CS: userindex(n) charindex(n) cancel(c)
								   //						: CS->GS: userindex(n) charindex(n) errorcode(c)

	MSG_CONN_GPARA_PROMOTION,		// 일본 GPARA 프로모션 : subtype(n) userindex(n) charindex(n)

	MSG_CONN_COUPON_CONFIRM_NEW,		// 쿠폰번호 확인	: charindex(n) coupon(str)
									// CS->GS			: charindex(n) errorCode(c) : eventerror사용
	MSG_CONN_COUPON_USE_NEW,			// 쿠폰 사용		: charindex(n) couponIdx(n)
									// CS->GS			: charindex(n) errorCode(c) : eventerror사용

	MSG_CONN_EVENT_GOMDORI_2007,	// 2007년 4월 곰돌이 이벤트 : subtype(uc) ...

	MSG_CONN_EVENT_FLOWERTREE_2007, // 2007년 꽃놀이 이벤트 :subtype (uc), charIndex(n),

	MSG_CONN_MSN_PROMOTION,		// 일본 MSN 프로모션 : subtype(n) userindex(n) charindex(n)

	MSG_CONN_GMTOOL_GOMMAND,	// 60 gmtool이 날린 운영자 명령어 디비에 저장

	MSG_CONN_GMTOOL_CHATTING,	// gmtool과 유저와의 대화 내용 디비에 저장

	MSG_CONN_GMTOOL_CHATMONITOR,	//  gmtool이 감시하는 유저의 대화내용 디비에 저장
	MSG_CONN_GUILD_POINT_RANKER_REQ,	// 랭커 인지 읽어 오기 : guildindex(n) charindex(n)
	MSG_CONN_GUILD_POINT_RANKER_REP,	// 랭커 응답		   : errorCode(uc) brank(c)	

	MSG_CONN_CREATEEVENT,			// 신규 유저 이벤트 : subtype(n) userindex(n) charindex(n) stime(n) etime(n) tableid(n)

	MSG_CONN_EVENT_GIFT_2007,		// 아이템 증정 이벤트 gs->cs userindex(n), charindex(n)

	MSG_CONN_EVENT_BJMONO_2007,		// SK BJ Mono Event : REQ : userindex(n) charIndex(n), REP : count(n) 응모권 개수
	
	MSG_CONN_EVENT_SSHOT_2007,		// Screen Shot Event : REQ : userindex(n) charIndex(n), REP : error(n) 성공 여부

	MSG_CONN_EVENT_XMAS_2007,				// 2007년 크리스마스  이벤트

	MSG_CONN_CASHITEM_COUPON,		// 70

	MSG_CONN_CASHITEM_PURCHASE_WITH_COUPON,

	MSG_CONN_CLIENT_RESTART,		// userid(str) masterdb 에 임시 패스워드 만료시간 연장
	
	MSG_CONN_ATTENDANCE_COUNT,		// REQ: subtype
	
	MSG_CONN_ADD_IPOINT,			// REQ : (n)userIndex, (n)charIndex (n)type (i)fIpoint REP: (n)charIndex, (i)fIpoint
	
	MSG_CONN_EVENT_NOM,				// REQ : userIndex(n), charindex(n), REP : error(no) 성공 여부

	MSG_CONN_OCN_GOO_PROMOTION,		// 일본 OCN, GOO 프로모션 : subtype(n) userindex(n) charindex(n)

	MSG_CONN_CHAR_LOG,				//  통계용 캐릭터 로그

	MSG_CONN_EVENT_REQUITAL,		//  보상 이벤트 전용

	MSG_CONN_END_MSG,				// 79 어떤 CONNECTOR 메시지도 이것 이후에 오면 안된다
} MSG_CONN_TYPE;

typedef enum _tagMSgConnIPointType
{
	MSG_IPOINT_TYPE_TEACHER = 0,			// 후견인
	MSG_IPOINT_TYPE_LEVELUP,				// 레벨업
	MSG_IPOINT_TYPE_PD4,					// 퍼스널 던전
	MSG_IPOINT_TYPE_WARCASTLE,				// 공성
	MSG_IPOINT_TYPE_ATTENDANCE,				// 출석
	MSG_IPOINT_TYPE_PLAYTIME,	// 플레이 시간
	MSG_IPOINT_TYPE_ACC_ATTENDANCE,			// 연속 출석
	MSG_IPOINT_TYPE_ETC,
	MSG_IPOINT_TYPE_DELETE_CHAR,			// 캐릭터 삭제
	MSG_IPOINT_TYPE_CANCEL_DELETE_CHAR,		// 캐릭터 삭제 취소
	MSG_IPOINT_TYPE_USEITEM,				// 아이템 사용
} MSG_CONN_IPOINT_TYPE;

typedef enum _tagMSgConnEventAttendanceRewardType
{
	MSG_ATTENDANCE_COUNT,					// REQ userid(n) charid(n) REP : charid(n) count(n)
	MSG_ATTENDANCE_UPDATE,					// REQ userid(n) charid(n) REP : NONE
} MSG_CONN_EVENT_ATTENDANCE_EVENT_TYPE;

typedef enum _tagMSgConnEventXMas2007Type
{
	MSG_CONN_EVENT_XMASTREE_ADDPOINT,		// REQ : (un)point, 
	MSG_CONN_EVENT_XMASTREE_POINT,			// REQ : NOT , REP : (un) Point
	MSG_CONN_EVENT_XMASTREE_ITEMDROP,		// REQ:NONE , REP: (n)remainTime 남은 시간
} MSG_CONN_EVENT_XMAS_2007_TYPE;

typedef enum _tagMSgConnEventSshot2007Type
{
	MSG_CONN_EVENT_SSHOT_CHECK,			// REQ : userindex(n), charindex(n), REP charindex(n), error(n)
	MSG_CONN_EVENT_SSHOT_UPDATE,		
} MSG_CONN_EVENT_SSHOT_2007_TYPE;

typedef enum _tagMSgConnEventBjMono2007Type
{
	MSG_CONN_EVENT_BJMONO_TICKET_CHECK,			// REQ : userindex(n), charindex(n), REP charindex(n), giveTicketCount(n)
	MSG_CONN_EVENT_BJMONO_TICKET_UPDATE,	
} MSG_CONN_EVENT_BJMONO_2007_TYPE;

typedef enum _tagMSgConnEventGift2007Type
{
	MSG_CONN_EVENT_GIFT_2007_CHECK,
	MSG_CONN_EVENT_GIFT_2007_GIVE,
	MSG_CONN_EVENT_GIFT_2007_CHECK_ALLOW,
	MSG_CONN_EVENT_GIFT_2007_CHECK_DBERROR,
	MSG_CONN_EVENT_GIFT_2007_CHECK_DENY,
	MSG_CONN_EVENT_GIFT_2007_GIVE_DBERROR,
	MSG_CONN_EVENT_GIFT_2007_GIVE_FULL,
} MSG_CONN_EVENT_GIFT_2007_TYPE;

typedef enum _tagMsgConnEventFlowerTree2007Type
{
	MSG_CONN_EVENT_FLOWERTREE_2007_ADDPOINT,		// 포인트 증가 REQ : point(un) 
	MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT,		// 포인트 보기 REQ: not REP : point0 (un) 
	MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK,	// 전체 포인트 요청 REQ: not REP: servercnt(n) point0 (un) point1(un)...
}MSG_CONN_EVENT_FLOWERTREE_2007_TYPE;

typedef enum _tagMsgConnEventGomdori2007Type
{
	MSG_CONN_EVENT_GOMDORI_2007_COUNT,		// 카운트 증가(응답 없음)
	MSG_CONN_EVENT_GOMDORI_2007_STATUS,		// 상황 보기 : charindex(n) count(n) [status(n)]:count
} MSG_CONN_EVENT_GOMDORI_2007_TYPE;

typedef enum _tagMsgConnEventNomType
{
	MSG_CONN_EVENT_NOM_REQ,							// 응모 신청
	MSG_CONN_EVENT_NOM_OK,							// 응모 성공
	MSG_CONN_EVENT_ERROR_ALREADY_APPLY,				// 이미 응모 됨..
	MSG_CONN_EVENT_NOM_LOTTERY_NO_GIVE,				// 복권 미지급
	MSG_CONN_EVENT_NOM_MOVIE_TICKET_REQ,			// 영화 티켓 사용 요청
	MSG_CONN_EVENT_NOM_MOVIE_TICKET_OK,				// 영화 티켓 사용 성공
} MSG_CONN_EVENT_NOM_TYPE;

typedef enum _tagMsgConnEventRequitalType
{
	MSG_CONN_EVENT_REQUITAL_REQ,					// 보상 신청
	MSG_CONN_EVENT_REQUITAL_OK,						// 보상 성공
	MSG_CONN_EVENT_REQUITAL_FAIL,					// 보상 실패
	MSG_CONN_EVENT_REQUITAL_FAIL_TIME,				// 해당 날짜에 게임을 하지 않아서 실패
	MSG_CONN_EVENT_REQUITAL_FAIL_CREATE_TIME,		// 유저가 게임을 시작한 날짜가 너무 이름
	MSG_CONN_EVENT_REQUITAL_ALREADY_APPLY,			// 이미 보상 받았음..
} MSG_CONN_EVENT_REQUITAL_TYPE;

typedef enum _tagMsgConnGparaPromotionType
{
	MSG_CONN_GPARA_PROMOTION_QUERY,		// GS->CS, 해당 캐릭터에게 지급할지 질의 보냄
	MSG_CONN_GPARA_PROMOTION_ALLOW,		// CS->GS, 해당 캐릭터에게 지급 허용
	MSG_CONN_GPARA_PROMOTION_DENY,		// CS->GS, 해당 캐릭터에게 지급 불가
	MSG_CONN_GPARA_PROMOTION_COMMIT,	// GS->CS, 해당 캐릭터에게 지급되었음을 저장 요청
} MSG_CONN_GPARA_PROMOTION_TYPE;

typedef enum _tagMsgConnOcnGooPromotionType
{
	MSG_CONN_OCN_GOO_PROMOTION_QUERY,		// GS->CS, 해당 캐릭터에게 지급할지 질의 보냄
	MSG_CONN_OCN_GOO_PROMOTION_ALLOW,		// CS->GS, 해당 캐릭터에게 지급 허용
	MSG_CONN_OCN_GOO_PROMOTION_DENY,		// CS->GS, 해당 캐릭터에게 지급 불가
	MSG_CONN_OCN_GOO_PROMOTION_COMMIT,	// GS->CS, 해당 캐릭터에게 지급되었음을 저장 요청
} MSG_CONN_OCN_GOO_PROMOTION_TYPE;

typedef enum _tagMsgConnMSNPromotionType
{
	MSG_CONN_MSN_PROMOTION_QUERY,		// GS->CS, 해당 캐릭터에게 지급할지 질의 보냄
	MSG_CONN_MSN_PROMOTION_ALLOW,		// CS->GS, 해당 캐릭터에게 지급 허용
	MSG_CONN_MSN_PROMOTION_DENY,		// CS->GS, 해당 캐릭터에게 지급 불가
	MSG_CONN_MSN_PROMOTION_COMMIT,	// GS->CS, 해당 캐릭터에게 지급되었음을 저장 요청
} MSG_CONN_MSN_PROMOTION_TYPE;

typedef enum _tagMsgConnCreateEventType
{
	MSG_CONN_CREATEEVENT_CREATETABLE,	// GS->CS, 신규 유저 이벤트 용 테이블 생성 요청												// gs->cs : idx(n)
	MSG_CONN_CREATEEVENT_QUERY,		// GS->CS, 해당 캐릭터에게 지급할지 질의 보냄
	MSG_CONN_CREATEEVENT_ALLOW,		// CS->GS, 해당 캐릭터에게 지급 허용
	MSG_CONN_CREATEEVENT_DENY,		// CS->GS, 해당 캐릭터에게 지급 불가
	MSG_CONN_CREATEEVENT_COMMIT,	// GS->CS, 해당 캐릭터에게 지급되었음을 저장 요청
} MSG_CONN_CREATEEVENT_TYPE;

typedef enum _tagMsgConnErrorCode
{
	MSG_CONN_ERROR_SUCCESS,							// 성공
	MSG_CONN_ERROR_SYSTEM,							// 시스템 에러
	MSG_CONN_ERROR_NOMATCH,							// 아이디/패스워드 오류
	MSG_CONN_ERROR_ALREADY,							// 이미 접속 중
	MSG_CONN_ERROR_FULLUSER,						// 사용자 만땅
	MSG_CONN_ERROR_BLOCKEDUSER,						// 계정 블럭
	MSG_CONN_ERROR_PREOPEN_GIFT_INSUFF_CONDITION,	// 프리오픈 선물 증정 : 조건 불만족
	MSG_CONN_ERROR_PREOPEN_GIFT_ALREADY_GIVE,		// 프리오픈 선물 증정 : 지급 완료
	MSG_CONN_ERROR_BILLING,							// ERROR BILLING SERVER
	MSG_CONN_ERROR_OPEN_BETA_GIFT_INSUFF_CONDITION,	// 오픈 베타 선물 증정 : 조건 불만족
	MSG_CONN_ERROR_OPEN_BETA_GIFT_ALREADY_GIVE,		// 오픈 베타 선물 증정 : 지급 완료
} MSG_CONN_ERRORCODE;

typedef enum _tagMsgSkillType
{
	MSG_SKILL_LIST,				// 스킬 리스트			: count(n) [skill_index(n) skill_level(c)] * count
	MSG_SKILL_LEARN,			// 스킬 배우기			: skill_index(n) is_new(uc) skill_level(c)
	MSG_SKILL_READY,			// 스킬 시전			: char_type(c) char_index(n) skill_index(n) target_type(c) target_index(n) skillspeed(n)
	MSG_SKILL_FIRE,				// 스킬 발동			: char_type(c) char_index(n) skill_index(n) target_type(c) target_index(n) count(n) [target_type(c) target_index(n)] * count skillspeed(n) movechar(c) x(z) z(f) h(f) r(f) y(c)
	MSG_SKILL_CANCEL,			// 스킬 취소			: char_type(c) char_index(n)
	MSG_SKILL_LEARN_ERROR,		// 배우기 오류 발생		: errorcode(uc)
	MSG_SKILL_ERROR,			// 스킬 관련 오류		: errorcode(c) skillindex(n) extraInfo(n)
	MSG_SKILL_AUTO_USE,			// 스킬 자동 시전청 C->S: skillindex(n)
	MSG_SKILL_NEWYEAR_2008,		// 2008년 신년 이벤트 스킬 : charindex1(n), charindex2(n)
	MSG_SKILL_HANARO_EVENT,		// 하나로 오픈 이벤트
	MSG_SKILL_LEARN_SEAL,		// 제작 스킬 배우기(seal): skill_index(n) is_new(uc) skill_exp(ll)
} MSG_SKILL_TYPE;

typedef enum _tagMsgSkillErrorType
{
// 050401 : bs : 스킬 사용 불가 상태
	MSG_SKILL_ERROR_ALREADY_USE,			// 다른 스킬 사용중
	MSG_SKILL_ERROR_NOTFOUND,				// 그런 스킬 없어요
	MSG_SKILL_ERROR_NOTREADY,				// 스킬 준비 중... 즉, 이전 스킬 시전후 다음 스킬 가능 시간이 안되었을때
	MSG_SKILL_ERROR_INVAL_TARGET,			// 타겟이 잘못되었음
	MSG_SKILL_ERROR_INSUFF_HP,				// 조건이 부족함 HP
	MSG_SKILL_ERROR_INSUFF_MP,				// 조건이 부족함 MP
	MSG_SKILL_ERROR_INSUFF_ITEM,			// 조건이 부족함 Item
	MSG_SKILL_ERROR_LONG,					// 멀어서 안됨
	MSG_SKILL_ERROR_INVAL_STATE_DEATH,		// 상태 불만족 : 죽어서만 가능
	MSG_SKILL_ERROR_INVAL_STATE_WEAPON,		// 상태 불만족 : 무기 장비시만 가능
	MSG_SKILL_ERROR_INVAL_STATE_SITDOWN,	// 상태 불만족 : 앉아서만 가능
	MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE,	// 상태 불만족 : 피스존 불가능
	MSG_SKILL_ERROR_INVAL_STATE_STAND,		// 상태 불만족 : 서서만 가능
	MSG_SKILL_ERROR_INVAL_STATE_SHIELD,		// 상태 불만족 : 방패 미착용
	MSG_SKILL_ERROR_INVAL_STATE_MAGIC,		// 상태 불만족 : 필요 버프 불충분
	MSG_SKILL_ERROR_STATPOINT_STR,			// 힘 부족
	MSG_SKILL_ERROR_STATPOINT_DEX,			// 민첩 부족
	MSG_SKILL_ERROR_STATPOINT_INT,			// 지혜 부족
	MSG_SKILL_ERROR_STATPOINT_CON,			// 체질 부족
	MSG_SKILL_ERROR_CANNOT_SPELL,			// 스킬 사용 불가능 상태
	MSG_SKILL_ERROR_TARGET_PKLIMITLEVEL,			// 타겟 PK레벨 미달
	MSG_SKILL_ERROR_ELEMENTAL_TIME,			// 소환 불가능 : 소환수 사망후 5분동안 소환 불가
	MSG_SKILL_ERROR_ELEMENTAL_FIRE,			// 소환 불가능 : 불의정령과 중복 불가
	MSG_SKILL_ERROR_ELEMENTAL_WIND,			// 소환 불가능 : 바람의정령과 중복 불가
	MSG_SKILL_ERROR_ELEMENTAL_EARTH,		// 소환 불가능 : 대지의정령과 중복 불가
	MSG_SKILL_ERROR_ELEMENTAL_WATER,		// 소환 불가능 : 물의정령과 중복 불가
	MSG_SKILL_ERROR_EVOCATION_ALREADY,		// 강신 불가능 : 이미 강신상태
	MSG_SKILL_ERROR_EVOCATION_TIME,			// 강신 불가능 : 강신 종료후 5분동안 불가

	MSG_SKILL_ERROR_CANNOT_INVISIBLE,		// 몹에게 타게팅 되어 인비저빌리티 불가능
	MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC,	// 15미터 이내에 다른 유저가 있어 데스모션 불가능
	MSG_SKILL_ERROR_MISS_ROGUE_SKILL,		// 와일드 라쏘 스킬(index 125) 실패
	MSG_SKILL_ERROR_COMPETITION,			// 대전용 아이템 사용 시간이 안되었음
// --- 050401 : bs : 스킬 사용 불가 상태
} MSG_SKILL_ERROR_TYPE;

typedef enum _tagMsgAssistType
{
	MSG_ASSIST_ADD,				// 상태 변화 추가		: chartype(c) charidx(n) itemidx(n) index(n) level(c) time(n)
	MSG_ASSIST_DEL,				// 상태 변화 제거		: chartype(c) charidx(n) itemidx(n) index(n)
	MSG_ASSIST_LIST,			// 리스트 전달(자신에게): state(n) count(c) [itemidx(n) index(n) level(c) time(n)]:count
} MSG_ASSIST_TYPE;

typedef enum _tagMsgEffectType
{
	MSG_EFFECT_SKILL,			// 스킬 이펙트			: skill_index(n) targettype(c) target_index(n)
	MSG_EFFECT_ETC,				// 기타 이펙트			: type(c) chartype(c) char_index(n) effect_type(uc)
	MSG_EFFECT_PRODUCE,			// 생산 이펙트			: type(c) chartype(c) char_index(n) target_chartype(c) target_index(n) target_hp(n)
	MSG_EFFECT_ITEM,			// 아이템 사용 이펙트	: type(c) chartype(c) char_index(n) item_db_idx(n)
	MSG_EFFECT_FIRE,			// 불기둥 이펙트		: type(c) 
} MSG_EFFECT_TYPE;

typedef enum _tagMsgEffectEtcType
{
	MSG_EFFECT_ETC_LEVELUP,				// 레벨업 이펙트
	MSG_EFFECT_ETC_GOZONE,				// Gozone 이펙트
	MSG_EFFECT_ETC_PRODUCE_MINING,		// 광석 성공 이펙트
	MSG_EFFECT_ETC_PRODUCE_GATHERING,	// 식물 성공 이펙트
	MSG_EFFECT_ETC_PRODUCE_CHARGE,		// 원소 성공 이펙트
	MSG_EFFECT_ETC_ITEM_PROCESS,		// 가공 성공 이펙트
	MSG_EFFECT_ETC_ITEM_MAKE,			// 제작 성공 이펙트
	MSG_EFFECT_ETC_MP_STEEL,			// MP 스틸 성공 이펙트
	MSG_EFFECT_ETC_HP_STEEL,			// HP 스틸 성공 이펙트
	MSG_EFFECT_ETC_FIRECRACKER,			// 폭죽 아이템 
	MGS_EFFECT_ETC_COMPETITION,			// 대전용 아이템 이펙트
} MSG_EFFECT_ETC_TYPE;

typedef enum _tagQuestType
{
	MSG_QUEST_PC_LIST,			// 퀘스트 리스트(PC)	: quest_count(n) [quest_index(n) current_count0-2(n)] quest_count 만큼 반복
	MSG_QUEST_REQUEST,			// 퀘스트 요청			: npc_index(n)
	MSG_QUEST_NPC_LIST,			// 퀘스트 리스트(NPC)	: quest_count(n) [quest_index(n) flag(c)]
	MSG_QUEST_CREATE_LIST,		// 퀘스트 리스트(생성)	: quest_count(n) [quest_index(n)]
	MSG_QUEST_START,			// 퀘스트 시작			: quest_index(n)
	MSG_QUEST_COMPLETE,			// 퀘스트 완료			: quest_index(n)
	MSG_QUEST_STATUS,			// 퀘스트 현황			: quest_index(n) current_count0-2(n)
	MSG_QUEST_PRIZE,			// 퀘스트 보상			: quest_index(n)
	MSG_QUEST_GIVEUP,			// 퀘스트 포기			: quest_index(n)
	MSG_QUEST_FAIL,				// 퀘스트 실패			: quest_index(n)
	MSG_QUEST_ERROR,			// 퀘스트 에러			: error_type(c)

	MSG_QUEST_DONE_LIST,		// 수행완료한 퀘스트 리스트  : quest_count(n) [quest_index(n)]
	MSG_QUEST_ABANDON_LIST,		// 수행 포기한 퀘스트 리스트 : quest_count(n) [quest_index(n)]
	MSG_QUEST_FORCE_GIVEUP,		// 퀘스트 강제 포기(레벨업에 의한..) : quest_index(n)
	MSG_QUEST_UNCOMPLETE,			// 퀘스트 미완료.(완료->미완료 상태변환시)

	// 퍼스널 던젼 4용 메세지
	MSG_QUEST_PD4_RANK_VIEW_REQ,			// 랭크 뷰요청	: charjob(c)
	MSG_QUEST_PD4_RANK_VIEW_REP,			// 랭크 뷰응답	: multicount(c) charname(str) bclear(c) deadmon(n) time(ll)   
	MSG_QUEST_PD4_RANK_REWARD_RANK_REQ,	// 보상 뷰요청
	MSG_QUEST_PD4_RANK_REWARD_RANK_REP,	// 보상 뷰응답	: multicount(c) charname(str) itemindex(n) itemnum(c) breward(c)
	MSG_QUEST_PD4_REWARD,					// 보상 요청

	MSG_QUEST_PD4_ERROR,		// 퀘스트pd4에러		: error_type(c)

	MSG_QUEST_COLLECT,			// 수집 퀘스트			: npcserverindex(n) charindex(n:s) itemdbindex(n:s)
								// C->S로 MSG_QUEST_COLLECT가 8초에 걸쳐 4회오면 퀘스트 아이템을 지급함
								// itemdbindex > 0  : NPC는 사망하고 itemdbindex를 지급함
								// itemdbindex == 0 : NPC는 사망하고 아이템 지급 없음
								// itemdbindex < 0  : NPC는 그대로 있고 아이템 지급 없음

} MSG_QUEST_TYPE;

typedef enum _tagQuestListFlag
{
	MSG_QUEST_FLAG_NOT_LEVEL,		// 수행 불가능 : 레벨
	MSG_QUEST_FLAG_NOT_JOB,			// 수행 불가능 : 직업
	MSG_QUEST_FLAG_NOT_ITEM,		// 수행 불가능 : 아이템
	MSG_QUEST_FLAG_CAN,				// 수행 가능
	MSG_QUEST_FLAG_ING,				// 수행 중
	MSG_QUEST_FLAG_COMPLETE,		// 수행 완료
	MSG_QUEST_FLAG_NOT_EXP,			// 수행 불가능 : 경험치 부족.
	MSG_QUEST_FLAG_NOT_PREQUEST,	// 수행 불가능 : 선행 퀘스트 미완료.
	MSG_QUEST_FLAG_NOTENOUGH_AREA,	// 수행 불가능 : 퍼스널 던전 여유분 부족
	MSG_QUEST_FLAG_OVERCOUNT,		// 수행 불가능 : 퍼스널 던전 입장 제한 초과
	MSG_QUEST_FLAG_NOT_PENALTY	,	// 수행 불가능 : 성향치 부족
} MSG_QUEST_LIST_FLAG;

typedef enum _tagMsgQuestErrorType
{
	MSG_QUEST_ERR_START_ONCE,		// 퀘스트 시작시 에러 : 이미 수행한 1회용 퀘스트
	MSG_QUEST_ERR_START_ALREADY,	// 퀘스트 시작시 에러 : 이미 수행중임
	MSG_QUEST_ERR_START_LEVEL,		// 퀘스트 시작시 에러 : 레벨 조건 안맞음
	MSG_QUEST_ERR_START_JOB,		// 퀘스트 시작시 에러 : 직업 조건 안맞음
	MSG_QUEST_ERR_START_ITEM,		// 퀘스트 시작시 에러 : 아이템 조건 안맞음
	MSG_QUEST_ERR_START_MAX,		// 퀘스트 시작시 에러 : 최대 수행 가능 수 초과
	MSG_QUEST_ERR_START_FULL,		// 퀘스트 시작시 에러 : 싱글던전의 경우 싱글던전 꽉참

	MSG_QUEST_ERR_DELIVERY_FULL,	// 전달 퀘스트 시 : 인벤토리가 꽉차서 아이템 퀘스트 에러
	MSG_QUEST_ERR_PRIZE_FULL,		// 보상 받을때 인벤 꽉 차서 보상 실패
	MSG_QUEST_ERR_PRZIE_ITEM,		// 보상 받을때 아이템 때문에 오류
	MSG_QUEST_ERR_PET_NOT_HAVE_ONEMORE,		// 펫 하나 이상 소유 못함
	MSG_QUEST_ERR_START_PENALTY	,	// 퀘스트 시작시 에러 : 페널티 조건 안맞음

} MSG_QUEST_ERROR_TYPE;

typedef enum _tagMsgQuestPD4ErrorType
{

	MSG_QUEST_ERR_REWARD_NOT_RANK,	// 보상시 랭커가 아님
	MSG_QUEST_ERR_BREWARD,			// 이미 보상 받았음

} MSG_QUEST_PD4_ERROR_TYPE;


typedef enum _tagStatPointType
{
	MSG_STATPOINT_REMAIN,			// 남은 성장 포인트 : statpoint_remain(n)
	MSG_STATPOINT_USE,				// 성장 포인트 사용 : dest_stat(uc:client,server) change_value1(int:server:능력치변화량) change_value2(int:server:능력치변화량) statpoint_remain(uc:server:남은수)
	MSG_STATPOINT_ERROR,			// 오류				: errorcode(c)
	MSG_STATPOINT_RESET,			// 포인트 초기화	: reset_str(n) reset_dex(n) reset_int(n) reset_con(n)
} MSG_STATPOINT_TYPE;


typedef enum _tagStatPointUseType
{
	MSG_STATPOINT_USE_STR,
	MSG_STATPOINT_USE_DEX,
	MSG_STATPOINT_USE_INT,
	MSG_STATPOINT_USE_CON,
} MSG_STATPOINT_USE_TYPE;


typedef enum _tagStatPointErrorType
{
	MSG_STATPOINT_ERROR_NOTENOUGH,		// 스탯 포인트 부족
	MSG_STATPOINT_ERROR_CANTUSE,		// 스탯 포인트 사용 불가
	MSG_STATPOINT_ERROR_WRONGPACKET,	// 클라이언트에서 이상한 패킷이 왔다
	MSG_STATPOINT_ERROR_NOMONEY,		// 돈도 없이 초기화 요청
} MSG_STATPOINT_ERROR_TYPE;


typedef enum _tagSkillLearnErrorType
{
	MSG_SKILL_LEARN_ERROR_LEVEL,		// 레벨 부족
	MSG_SKILL_LEARN_ERROR_SP,			// 포인트 부족
	MSG_SKILL_LEARN_ERROR_ITEM,			// 아이템 부족
	MSG_SKILL_LEARN_ERROR_SKILL,		// 스킬 부족
	MSG_SKILL_LEARN_ERROR_SYSTEM,		// 그외 시스템 오류(스킬 최대 수치에 의한 오류 등등, 원래는 발생하면 안되는 경우!)
	MSG_SKILL_LEARN_ERROR_STR,			// 힘 부족
	MSG_SKILL_LEARN_ERROR_DEX,			// 덱스 부족
	MSG_SKILL_LEARN_ERROR_INT,			// 지혜 부족
	MSG_SKILL_LEARN_ERROR_CON,			// 체질 부족
} MSG_SKILL_LEARN_ERROR_TYPE;


typedef enum _tagDamageType
{
	MSG_DAMAGE_UNKNOWN = -1,			// 알수없는 오류!
	MSG_DAMAGE_MELEE = 0,				// 근거리 물리 공격
	MSG_DAMAGE_RANGE,					// 원거리 물리 공격
	MSG_DAMAGE_MAGIC,					// 스킬 공격
	MSG_DAMAGE_REFLEX,					// 반사 공격
	MSG_DAMAGE_LINK,					// 데미지 링크에 의한 공격
	MSG_DAMAGE_COMBO,					// 몬스터 콤보 이펙트 데미지 공격
} MSG_DAMAGE_TYPE;


typedef enum _tagWarpType
{
	MSG_WARP_START,						// 이동장치 사용 시작
	MSG_WARP_CANCEL,					// 이동장치 사용 중지
	MSG_WARP_END,						// 이동장치 사용 끝(10초 딜레이 끝)
	MSG_WARP_TELEPORT,					// 순간 이동				: pos(n) x(f:s) z(f:s) h(f:s) r(f:s) ylayer(c:s)
	MSG_WARP_PROMPT,					// 이동 확인				: zone(n) pos(n)
	MSG_WARP_TO_REQING,					// gs->c :순간이동 여부를 묻고 있음
	MSG_WARP_TO_REQ,					// PC로 순간이동요청 : charname(str)
										// gs-> c charname님을 승인? : charname(str)
	MSG_WARP_TO,						// C->GS charname님을 승인여부에 따라 워프 : charname(str) ballow(c)
	MSG_WARP_TAKE_REQ,					// pc를 소환 요청	 : charname(str)
										// gs-> c charname님을 승인? : charname(str)
	MSG_WARP_TAKE_REQING,				// gs->c 소환 여부 
	MSG_WARP_TAKE,						// C->GS charname님을 승인여부에 따라 소환 : charname(str) ballow(c)
	MSG_WARP_ERROR,						// errorcode(c)
} MSG_WARP_TYPE;

typedef enum _tagWarpErrorType
{
	MSG_WARP_ERROR_NOTCHAR,				// 접속중이 아니거나 존재하지 않음
	MSG_WARP_ERROR_NOTALLOW,			// 이동을 승인하지 않음
	MSG_WARP_ERROR_NOTALLOWZONE,		// 이동할수 없는 존에 있습니다
	MSG_WARP_ERROR_CANCEL,				// 신청자가 취소
} MSG_WARP_ERROR_TYPE;



typedef enum _tagProduceKind
{
	MSG_PRODUCE_MINING,					// 채굴
	MSG_PRODUCE_GATHERING,				// 채집
	MSG_PRODUCE_CHARGE,					// 차지
} MSG_PRODUCE_KIND;

typedef enum _tagProcessKind
{
	MSG_PROCESS_STONE,					// 광석 가공
	MSG_PROCESS_PLANT,					// 식물 가공
	MSG_PROCESS_ELEMENT,				// 원소 가공
} MSG_PROCESS_KIND;

typedef enum _tagMsgSSkillType
{
	MSG_SSKILL_LIST,					// 특수 스킬 리스트			: count(n) [sskill_index(n) sskill_level(c)] * count
	MSG_SSKILL_LEARN,					// 특수 스킬 배우기			: sskill_index(n) is_new(uc) sskill_level(c)
	MSG_SSKILL_REMOVE,					// 특수 스킬 삭제			: sskill_index(n)
	MSG_SSKILL_LEARN_ERROR,				// 배우기 오류 발생			: errorcode(uc)
} MSG_SSKILL_TYPE;

typedef enum _tagSSkillLearnErrorType
{
	MSG_SSKILL_LEARN_ERROR_LEVEL,		// 레벨 부족
	MSG_SSKILL_LEARN_ERROR_SP,			// SP 포인트 부족
	MSG_SSKILL_LEARN_ERROR_SSKILL,		// SSkill 아예 없음
	MSG_SSKILL_LEARN_ERROR_SSKILL_LEVEL,// SSkill Level이 안됨
	MSG_SSKILL_LEARN_ERROR_SYSTEM,		// 그외 시스템 오류(스킬 최대 수치에 의한 오류 등등, 원래는 발생하면 안되는 경우!)
} MSG_SSKILL_LEARN_ERROR_TYPE;

typedef enum _tagPKType
{
	MSG_PK_PENALTY_HPMP,				// hp, mp 패널티 카운트 전달 : hp_count(n) mp_count(n)
	MSG_PK_ITEMSEAL,					// item 봉인 됨				 : tab(c) row(c) col(c) itemindex(n) flag(n)
	MSG_PK_RECOVER_HPMP,				// hp, mp 면죄 요청/응답	 : hp_request(c) mp_request(c)
	MSG_PK_RECOVER_ITEMSEAL,			// item 봉인 해재 요청/응답	 : tab(c) row(c) col(c) itemindex(n) flag(n:s)
	MSG_PK_ERROR,						// 오류 전달				 : errorcode(c)
										//							 : 0 - 돈 부족, 1 - hp 실패, 2 - mp 실패, 3 - 아이템 실패
	MSG_PK_FREE,						// Free PK SYSTEM
	MSG_PK_CHANGE_KING,					// 카오킹 변경 메세지 : bChao(c) 0-헌터 1-카오, CharIndex(n) 캐릭터 인덱스 
	MSG_PK_MYZONE_ATT,					// REQ : NONE, REP : ZoneAtt(c) ( 0-Normal, 1-헌터 , 2-카오 )
	MSG_PK_REMOVE_PENALTY,				// REQ : type(c) 0-전부, 1-1단계 , REP : succecc(c) : 0-실패, 1-성공 
	MSG_PK_OTHERZONE_ALRAM,				// REQ : NONE , REP : bAram(c) 0-OFF 1-ON, charIndex(n) 캐릭터 인텍스 
	MSG_PK_TARGET_SHOWHP,				// REQ : On(c) 0-OFF 1-ON REP : NONE
} MSG_PK_TYPE;

typedef enum _tagEventType
{
	MSG_EVENT_ERROR,							// 에러코드					: errcode(uc)
	MSG_EVENT_PREOPEN_GIFT,						// 프리오픈 유저 선물 주기	: itemdbindex(n:s)
	MSG_EVENT_LATTO,							// 라또 이벤트 관련			: subtype(uc)
	MSG_EVENT_LETTER,							// 낱말 맞추기 이벤트		: subtype(uc)
	MSG_EVENT_CHANGEWEAPON,						// 무기 교환 이벤트			: olddbindex(n:s) newdbindex(n:s)
	MSG_EVENT_MOONSTONE,						// 문스톤 이벤트.
	MSG_EVENT_TREASUREBOX,						// 보물상자 이벤트.
	MSG_EVENT_SUPERSTONE,						// 슈퍼고제 이벤트.//0627
	MSG_EVENT_CHUSEOK_UPGRADE,					// 추석 이벤트 업그레이드	: 
	MSG_EVENT_CHUSEOK_EXCHANGE,					// 추석 이벤트 교환			: 
	MSG_EVENT_CHANGE_WITHOUT_OPTION,			// 10 2판4판 리뉴얼 무기 교체 이벤트 : old_itemindex(n) new_subtype(n)
	MSG_EVENT_2PAN4PAN_LETTER,					// 2판4판 리뉴얼 낱말 이벤트 : MSG_EVENT_2PAN4PAN_WORD_TYPE(n)

	MSG_EVENT_2PAN4PAN_GOODS_CHECK,				// 2판4판 리뉴얼 아이템 리스트 요청 : count(n:server)
	MSG_EVENT_2PAN4PAN_GOODS_REQ,				// 2판4판 리뉴얼 아이템 받기 요청 :
	MSG_EVENT_2PAN4PAN_GOODS_REP,				// 2판4판 리뉴얼 아이템 받기 응답 : MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE(n) count(n) [itemdbindex(n) itemcount(ll)]*count

	MSG_EVENT_2PAN4PAN_MONEY_REQ,				// 2판4판 머니 카드 교환 요청 : type(n)
												// type : 1 - 맞고, 2 - 포커
	MSG_EVENT_2PAN4PAN_MONEY_REP,				// 2판4판 머니 카드 교환 응답 : success(n)
												// success : 0 - 실패, 1 - 맞고 성공, 2 - 포커성공

	MSG_EVENT_2PAN4PAN_BOX_CHECK,				// 2판4판 상자 교환 검사	: count(n:server)
	MSG_EVENT_2PAN4PAN_BOX_REQ,					// 2판4판 상자 교환 요청	: 
	MSG_EVENT_2PAN4PAN_BOX_REP,					// 2판4판 상자 교환 응답	: MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE(n) count(n)  [itemdbindex(n) itemcount(ll)]*count

	MSG_EVENT_XMAS_2005,						// 20 2005 크리스마스 이벤트 : MSG_EVENT_XMAS_2005_ERROR_TYPE(n:server)
	MSG_EVENT_NEWYEAR_2006_CHECK,				// 2006 신년 이벤트 시간 요청 : time_sec(n:server)
	MSG_EVENT_NEWYEAR_2006_GOODS,				// 2006 신년 이벤트 보상  : MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TYPE(n:server)

	MSG_EVENT_SEARCHFRIEND_ADD_CHECK,			// 휴면케릭 등록 체크 : MSG_EVENT_SEARCHFRIEND_ERROR_TYPE(n)
	MSG_EVENT_SEARCHFRIEND_ADDSELECT_LIST,		// 등록한 리스트 부분 : listCount(n), startindex(n), nTotal(n), [requestindex(n), reqnick(char)]*listCount 
	MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK,		// 등록한 리스트중 선택하는 부분 :  requestindex(n), MSG_EVENT_SEARCHFRIEND_ERROR_TYPE(n)
	MSG_EVENT_SEARCHFRIEND_TIME_CHECK,			// 사냥시간 체크 : time_sec(n:server)
	MSG_EVENT_SEARCHFRIEND_ONETIME_CHECK,		// 사냥시간 한시간 단위 체크 : time_sec(n:server)
	MSG_EVENT_SEARCHFRIEND_LIST_GOODS,			// 등록된 친구 보상가능여부 : request_listmember(n), startindex(n), nTotal(n), (MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE)errcode, [reqnick[n], request_index[n], request_time[n], item_good[n]]
	MSG_EVENT_SEARCHFRIEND_GOODS_CHECK,			// 아이템 지급 개인 :MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE(n)

	MSG_EVENT_PACKAGE_ITEM_GIVE,				// 30 패키지 아이템 지급 요청  : couponNo(str)
												// gs->client				: errorcode(c)
	MSG_EVENT_OPENBETA_CONN,					// 오픈베타 아이템 지급 요청: 
												// gs->client				: errorcode(c)
	MSG_EVENT_SAKURABUD,						// 꽃봉오리 아이템 지급 요청 : cmd(c) : 0-상의하의,1-장갑신발,2-투구 

	MSG_EVENT_OXQUIZ,							// OX 퀴즈 이벤트			: subtype(n) ...

	MSG_EVENT_RECOMMEND,						// 추천서버					: subtype(n) ...
	MSG_EVENT_PCBANG,							// 피씨방 (넷까패 캠폐인)	: 상자 지급혹은 교환 응답  GS->C : itemindex(n)    -1이면 실패(상자가 없삼)
												//							: 교환 요청시 C->GS :

	MSG_EVENT_WORLDCUP,							// 월드컵 이벤트			: subtype(n) ...
	MSG_EVENT_GOLDENBALL,						// 골든볼 이벤트			: subtype(n) ...

	MSG_EVENT_RAIN_2006,						// 장마 이벤트				: errorcode(n:s)

	MSG_EVENT_TLD_BUDDHIST,						// 불교 촛불 이벤트			: requesttype(c) errorcode(n:s)
												// requesttype				: 0 - 양초, 1 - 성수병

	MSG_EVENT_COLLECT_BUG,						// 40 곤충 채집				: subtype(n) ...

	MSG_EVENT_CHUSEOK_2006,						// 2006년 추석 이벤트		: subtype(n) ...

	MSG_EVENT_OPEN_BETA_GIFT,					// 오픈 베타 사은품 주기	: itemdbindex(n:s)

	MSG_EVENT_XMAS_2006,						// 2006 크리스마스 이벤트 교환 : cakecount(n) errcode(n:s)

	MSG_EVENT_VALENTINE_2007,					// 2007 발렌타인데이 이벤트

	MSG_EVENT_WHITEDAY_2007,					// 2007 화이트데이 이벤트

	MSG_EVENT_SONGKRAN_2007,					// 2007 태국 송크란 이벤트 

	MSG_EVENT_EGGS_HUNT_2007,					// 2007 미국 부활절 이벤트

	MSG_NEW_EVENT_PACKAGE_ITEM_GIVE,			// NEW 패키지 아이템 지급 요청  : couponNo(str)
												// gs->client				: errorcode(c)

	MSG_EVENT_PARENTSDAY_2007,					// 2007 어버이날 이벤트

	MSG_EVENT_TEACH_2007,						// 50 2007년 스승의날 이벤트

	MSG_EVENT_GOMDORI_2007,						// 2007년 곰돌이 이벤트		: subtype(uc) ...
	
	MSG_EVENT_CHILDRENSDAY_2007,				// 2007년 어린이날 이벤트 : subtype(uc)..

	MSG_EVENT_FLOWERTREE_2007,					// 2007년 꽃놀이 이벤트 : subtype(uc)..

	MSG_EVENT_SUMMER_VACATION_2007,				// 2007년 여름 방학 이벤트 : subtype(uc)....

	MSG_EVENT_TLD_MOTHERDAY_2007,				// 어머니날 이벤트			: requesttype(c) errorcode(n:s)
												// requesttype				: 1 - 고급 제련석 교환 , 2 - Poem Book 교환 ,3Blessing of mother(red),
												//							: 4.Blessing of mother(yellow),5.Blessing of mother(blue)

	MSG_EVENT_OPEN_ADULT_SERVER,				// 성인서버 매직카드이벤트

	MSG_EVENT_GIFT_2007,						// 2007년 이벤트 기간 동안 유저들에게 아이템 무료증정
	
	MSG_EVENT_BJMONO_2007,						// SK bj mono 이벤트 : subtype(uc)

	MSG_EVENT_SSHOT_2007,						// Screen Shot Event : Subtype(uc)

	MSG_EVENT_RICHYEAR,							// 60 풍년 이벤트

	MSG_EVENT_LC_1000DAY,						// 라스트카오스 1000일 이벤트 
	
	MSG_EVENT_HALLOWEEN_2007,					// 2007년 할로윈 이벤트 

	MSG_EVENT_NEW_MOONSTONE,					// 문스톤 개편

	MSG_EVENT_XMAS_2007,						// 2007년 크리스마스 이벤트
	
	MSG_EVENT_NEWYEAR_2008,						// 2008년 신년 이벤트	gs->cl, cl->gs   charindex(n), charname(s)
	
	MSG_EVENT_TREASUREBOX_RED,					// 붉은 보석 상자 이벤트
	
	MSG_EVENT_SAKURA_2008,						// 2008년 벚꽃 이벤트

	MSG_EVENT_NOM,								// 놈놈놈 영화 이벤트

	MSG_EVENT_PHOENIX,							// 피닉스 이벤트 // yhj   081105  피닉스

	MSG_EVENT_PHOENIX_SUCCESS,					// 피닉스 이벤트 승락// yhj   081105  피닉스

	MSG_EVENT_REQUITAL,							// 보상 이벤트 
	
	MSG_EVENT_END_MSG,							// 모든 이벤트 관련 메시지는 이보다 앞에
	
} MSG_EVENT_TYPE;

typedef enum _tagEventNomErrorType
{
	MSG_EVENT_NOM_OK,						// 응모 성공
	MSG_EVENT_NOM_ERROR_ALREADY_APPLY,		// 이미 응모 함
	MSG_EVENT_NOM_ERROR_FULL_INVEN,			// 인벤토리가 꽉참
	MSG_EVENT_NOM_ERROR_ITEM_FAIL,			// 아이템 오류
	MSG_EVENT_NOM_LOTTERY_USE_OK,			// 복권 사용 성공
	MSG_EVENT_NOM_MOVIE_TICKET_OK			// 티켓 등록 성공	
} MSG_EVENT_NOM_ERROR_TYPE;

typedef enum _tagEventRequitalErrorType
{
	MSG_EVENT_REQUITALM_OK,						// 보상 성공
	MSG_EVENT_REQUITAL_FAIL,					// 보상 실패
	MSG_EVENT_REQUITAL_ERROR_ALREADY_APPLY,		// 이미 응모 함
	MSG_EVENT_REQUITAL_FAIL_TIME,				// 보상 시간이 지나감
	MSG_EVENT_REQUITAL_ERROR_FULL_INVEN,		// 인벤토리가 꽉참
	MSG_EVENT_REQUITAL_ERROR_TOO_HEAVY_INVEN,	// 인벤토리이 너무 무거움
	MSG_EVENT_REQUITAL_FAIL_TIME_NOPLAY,		// 해당 날짜에 게임을 하지 않아서 실패
	MSG_EVENT_REQUITAL_FAIL_CREATE_TIME,		// 유저가 게임을 시작한 날짜가 너무 이름
} MSG_EVENT_REQUITAL_ERROR_TYPE;

typedef enum _tagEventSakura2008Type
{
	MSG_EVNET_SAKURAEVENT_2008,					// non parm
	MSG_EVENT_SAKURAEVENT_2008_ERROR,			// (n) error code 0 - SUCCESS, 1-FAILED
	MSG_EVNET_SAKURAEVENT_2008_ITEMCHANGE,		//  (str) charName (n) itemIndex
} MSG_EVNET_SAKURA_2008_TYPE;

typedef enum _tagEventNewMoonStoneType
{
	MSG_EVENT_NEW_MOONSTONE_START_REQ,		// 문스톤 시작요청
	MSG_EVENT_NEW_MOONSTONE_START_REP,		// 문스톤 시작응답		nas(n)

	MSG_EVENT_NEW_MOONSTONE_TRY_REQ,		// 문스톤 복권 요청		moonstone_idx(n)
	MSG_EVENT_NEW_MOONSTONE_TRY_REP,		// 문스톤 복권 응답		giftitemindex(n) , nas(n)

	MSG_EVENT_NEW_MOONSTONE_RESULT_REQ,		// 문스톤 복권 결과 요청
	MSG_EVENT_NEW_MOONSTONE_RESULT_REP,		// 문스톤 복권 결과 응답	giftitemidx(n), giftitemcount(n)
	MSG_EVENT_NEW_MOONSTONE_JACK_POT,		// 잭팟	

	MSG_EVENT_NEW_MOONSTON_OK,				// 문스톤얻음		moonstone idx(n)

	MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE,	// 문스톤 없음		itemindex(없는문스톤index)
	MSG_EVENT_NEW_MOONSTONE_FULLINVEN,		// 인벤가득참	

	MSG_EVENT_NEW_MOONSTONE_MIX_REQ,		// 문스톤 조합 요청	moonstone_idx(n) 문스톤조합박스(n)
	MSG_EVENT_NEW_MOONSTONE_MIX_SUCCESS,	// 문스톤 조합 성공	new_moonstone_idx(n),
	MSG_EVENT_NEW_MOONSTONE_MIX_FAILED,		// 문스톤 조합 실패
	MSG_EVENT_NEW_MOONSTONE_MIX_MISS		// 문스톤 조합 공식에 어긋남
} MSG_EVENT_NEW_MOONSTONE_TYPE;

typedef enum _tagEventXMas2007Type
{
	MSG_EVENT_XMASTREE_ADD_POINT,		// REQ: NOT , REP (c)error 0:성공, 1:아이템 없음
	MSG_EVENT_XMASTREE_GET_BLESS,		// REQ: NOT , REP (c)error 0:성공, 1:아이템 없음
	MSG_EVENT_XMASTREE_DROP_ITEM,		// REQ: NONE, REP (n)remainTime 남은 시간(분)
} MSG_EVENT_XMAS_2007_TYPE;

typedef enum _tagMsgEventHallowWeen2007
{
	MSG_EVENT_TAKE_DEVILHEAIR,						// 악마날개 머리띠 받기 REQ: , REP: 0(SUCCESS), 1(받은적있음)
	MSG_EVENT_CHANGE_CANDYBASKET,					// 사탕바구니에 사탕 받기 REQ: , REP: 0(SUCCESS), 1(바구니 없음)
	MSG_EVENT_CHANGE_PUMKINHEAD,					// 호박머리 탈 교환받기 REQ: , REP: 0(SUCCESS) , 1(사탕모자람)
	MSG_EVENT_CHANGE_WITCHHAT,						// 마녀모자 교환받기 REQ: , REP: 0(SUCCESS), 1(증표없음)
	MSG_EVENT_CHANGE_PUMKIN,						// 호박 교환 받기 REQ: , REP: 0(SUCCESS), 1(호박없음)
	MSG_EVENT_TASTE_CANDY,							// 사탕 맛 REQ:NONE , REP : 0(절망적인맛),1(토나오는맛),2(그럭저럭),3(괜찮은맛),4(천상의맛)
	MSG_EVENT_FULL_INVEN,							// 인벤 가득참 REQ:NONE , REP : NOT
} MSG_EVENT_HALLOWEEN_2007_TYPE;

typedef enum _tagMsgEventLC1000Day
{
	MSG_EVENT_LC_1000DAY_HAT,						// 1000일 이벤트 모자 요청 
	MSG_EVENT_LC_1000DAY_HAT_UPDATE,				// 모자 옵션 업데이트 요청 
	MSG_EVENT_LC_1000DAY_FIRECRACKER,				// 폭죽 요청
	MSG_EVENT_LC_1000DAY_FULLINVEN,					// 인벤이 가득 참
	MSG_EVENT_LC_1000DAY_HAT_UPDATE_SUCCESS,		// 모자 옵션 업데이트 성공
	MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL,			// 모자 옵션 업데이트 실패
	MSG_EVENT_LC_1000DAY_FIRECRACKER_DUPLICATION,	// 폭죽 아이템이 인벤에 있음
	MSG_EVENT_LC_1000DAY_NOITEM,					// 아이템 부족
} MSG_EVENT_LC_1000DAY_TYPE;

typedef enum _tagMsgEventRichYear
{
	MSG_EVENT_RICHYEAR_UPGRADE,					// 아이템 업그레이드 요청
	MSG_EVENT_RICHYEAR_EXCHANGE,				// 보상품 교환
	MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM,	// 업그레이드할 재료 없음
	MSG_EVENT_RICHYEAR_ERROR_UPGRADE_FAILED,	// 업그레이드 실패
	MSG_EVENT_RICHYEAR_ERROR_NOCROP,			// 작물이 없을 경우
	MSG_EVENT_RICHYEAR_ERROR_ALREADY_UPGRADE,	// 더이상 업그레이드 불가능
	MSG_EVENT_RICHYEAR_ERROR_FULLINVEN,			// 인벤부족
	MSG_EVENT_RICHYEAR_UPGRADE_OK,				// 업그레이드 성공
	MSG_EVENT_RICHYEAR_EXCHAGE_OK,				// 보상 성공
}MSG_EVENT_RICHYEAR_TYPE;


typedef enum _tagMsgEventSshot
{
	MSG_EVENT_SSHOT_2007_RECIVE,			// 송편 받기 REQ : 요청 - REP : 성공
	MSG_EVENT_SSHOT_2007_ALREADY,			// 송편을 받은적이 있다.
	MSG_EVENT_SSHOT_2007_NOSSHOT,			// 응모한적 없음
	MSG_EVENT_SSHOT_2007_FULLINVEN			// 인벤이 가득 참
} MSG_EVENT_SSHOT_2007_TYPE;

typedef enum _tagMsgEventBjMono
{
	MSG_EVENT_BJMONO_2007_TICKET_REQ,			// 응모권 받기 REQ : 요청 - REP : 성공 count 
	MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ,	// 응모권 교환 REQ : 요청 - REP : 성공 itemIndex(n)
	MSG_EVENT_BJMONO_2007_NO_TICKET,			// 응모권 없음
	MSG_EVENT_BJMONO_2007_DONT_CHANGE_TICKET,	// 교환할 응모권 없음
	MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN		// 인벤이 가득 참
} MSG_EVENT_BJMONO_2007_TYPE;

typedef enum _tagMsgEventOpenAdultServer
{
	// 매직카드관련
	MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_REQ,		// 매직카드보상받기
	MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_OK,		// 스킬보상성공			exchangeType(b) 
											// exchangeType			0 : 바람의 러브매직
											//						1 : 힘의 러브매직
											//						2 : 민첩의 러브매직
											//						3 : 지혜의 러브매직
											//						4 : 체질의 러브매직
											//						5 : 로레인의 러브매직

	MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED,	// 교환 실패 ( 스킬적용 실패했을 때)
	MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM,	// 아이템 없음			

	// 전직관련
	MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE,		// 아이템 보상요청
	MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE_OK,		// 보상성공	: 등수(n), 절망의부적 보상개수(n)
	
	MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_ALREADY,	// 상품이미 받았음
	MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FAILED,		// 전직을 안해서 실패	
	MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FULLINVEN,	// 인벤부족

	// 아이템 조합관련
	MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_REQ,			// 아이템 업그레이드 요청 : tab(n), row(n), col(n)
	MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_OK,

	MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_NOT_FOUND,	// 재료 부족
	MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED,		// 업그레이드 실패
	MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FULLINVEN,	// 인벤부족
}MSG_EVENT_OPEN_ADULT_SERVER_TYPE;


typedef enum _tagMsgEventSummerVacation_2007
{
	MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES,	// 종이 접기 요청 REQ: papaerindex(n) papaerindex(n) papaerindex(n)
													//  REP : errorcode(n) figuretype(n) itemindex(n) itemcount(n)
	MSG_EVENT_SUMMER_VACATION_2007_INCHEN,			// 인첸 요청 : count(n) paperindex(uc) 
													// REP : err(n) itemindex(n)
	
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL,	// 인첸 실패
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK,
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM,		// 아이템 없음
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL,
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_OK,
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY,	// 인벤토리 가득차서 아이템 지급 불가능
	MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY,			// 돈 부족
} MSG_EVENT_SUMMER_VACATION_2007_TYPE;

typedef enum _tagMsgEventFlowerTree_2007
{
	MSG_EVENT_FLOWERTREE_2007_POINT, 			// 현재 꽃나무 누적 포인트   REQ : not REP: point(un)
	MSG_EVENT_FLOWERTREE_2007_POINT_RANK,		// 서버군 꽃나무 누적 포인트 REQ : not REP: servercnt(n) point0 (un) point1(un)...
	MSG_EVENT_FLOWERTREE_2007_GROWUP,			// 아이템을 사용하여 개화 돕기 REQ : not REP: givePoint(un)	
	MSG_EVENT_FLOWERTREE_2007_MY_POINT,			// 내가 기여한 포인트 REQ : not REP: point(un)
	MSG_EVENT_FLOWERTREE_2007_RECV_TICKET,		// 교환권 받기 REQ : not REP: count(n) 
	MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET,		// 교환권 확인 REQ : not REP: error(uc) 0-티켓없음, 1-티켓사용;
	MSG_EVENT_FLOWERTREE_2007_PRICE_MSG,		// 당첨 메세지 REP : charIndex(n), itemindex(n)
} MSG_EVENT_FLOWERTREE_2007_TYPE;

typedef enum _tagMsgEventChildrensDay_2007
{
	MSG_EVENT_CHILDRENSDAY_2007_REQ,		// 교환 요청	(n)ItemIndex, (n)개나리 갯수
	MSG_EVENT_CHILDRENSDAY_2007_REP,		// 에러코드		(n) 0- 교환 성공 + (n)ItemIndex, 1-개나리 갯수 부족, 2-이미 받은 장비 , 3-Unknown
}MSG_EVENT_CHILDRENSDAY_2007_TYPE;

typedef enum _tagMsgEventGomdori2007
{
	MSG_EVENT_GOMDORI_2007_CHECKITEM,			// 가위바위보 아이템 검사
	MSG_EVENT_GOMDORI_2007_START,				// 가위바위보 시작하기
	MSG_EVENT_GOMDORI_2007_SELECT,				// 가위바위보 선택하기	: type(c)
												// type					: 가위-0, 바위-1, 보-2
	MSG_EVENT_GOMDORI_2007_RESULT_WIN,			// 가위바위보 승리 알림	: wincount(c) usersel(c) npcsel(c)
	MSG_EVENT_GOMDORI_2007_RESULT_LOSE,			// 가위바위보 패배 알림	: wincount(c) usersel(c) npcsel(c)
	MSG_EVENT_GOMDORI_2007_RESULT_DRAW,			// 가위바위보 비김 알림	: wincount(c) usersel(c) npcsel(c)
	MSG_EVENT_GOMDORI_2007_CONTINUE,			// 가위바위보 계속 하기
	MSG_EVENT_GOMDORI_2007_END,					// 가위바위보 그만 하기	: wincount(c:s) prompttype(c:s) extar1(n:s) extar2(n:s)
												// prompttype			: 0 - 일반 보상: extra1(보상아이템인덱스) extra2(보상아이템수량)
												//						: 1 - 체험교복 보상: extra1(사용안함) extra2(사용안함)
												//						: 2 - 엽기 무기 선택 필요: extra1(선택아이템1) extra2(선택아이템2)
	MSG_EVENT_GOMDORI_2007_SELECT_GIFT,			// 엽기무기 선택		: itemdbindex(n)
	MSG_EVENT_GOMDORI_2007_VIEW_STATUS,			// 현황 보기			: curserver(n:s) count(n:s) [server(n:s)]:count
	MSG_EVENT_GOMDORI_2007_ERROR_NOITEM,		// 곰돌이 아이템 부족
	MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY,	// 인벤토리 가득차서 아이템 지급 불가능
} MSG_EVENT_GOMDORI_2007_TYPE;

typedef enum _tagMsgEventParentsDay2007
{
	MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST, // 어버이날 누적 포인트 리스트 전송 요청  c->s는  MessageType만
				  // s->c : error(c) count( n ) rankig( c ) savepoint( n ) gildname(str)
	MSG_EVENT_PARENTSDAY_2007_ADD_POINT,    // 포인트 등록
				  // c->s : CharIndex(n) echangeItemIndex(n)
				  // s->c : errorcode(c) point(n) totalpoint(n)
	MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET,   // 누적 포인트로 인한 교환권 요청
				  // s->c : errorcode(c) changeTicketCount(n)
	MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM,   // 교환권으로 아이템 교환 요청
	// s->c: errcode(c)
	MSG_EVENT_PARENTSDAY_2007_NOTICE, // 공지	//s-c : guildName(str) itemName(str)

	MSG_EVENT_PARENTSDAY_2007_RESULT, // s->c : ranking(n)

}MSG_EVENT_PARENTSDAY_2007_TYPE;

typedef enum _tagParentsDay2007ErrorType
{
	MSG_ERROR_PARENTSDAY_2007_FULLINVEN,			// 인벤 가득 참
	MSG_ERROR_PARENTSDAY_2007_NO_EXCHAGNE_ITEM,		// 교환권 없음
	MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER,		// 길드장이 아닙니다.
	MSG_ERROR_ACCUMULATEPOINT_NOT_RANKING,			// 랭킹 데이타가 없습니다.
	MSG_ERROR_ADD_POINT,							// 포인트 누적 실패
	MSG_ERROR_NOT_ENOUGH_POINT,						// 포인트 부족
	MSG_ERROR_NOT_FOUND_ACCUMULATEPOINT_LIST,		// 누적 포인트 정보가 없습니다.
	MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM,			// 아이템 없음
	MSG_PARENTSDAY_2007_SUCCESS,					// 성공
}MSG_PARENTSDAY_2007_ERROR_TYPE;


typedef enum _tagMsgEventSongkran2007
{
	MSG_EVENT_SONGKRAN_2007_EXCHANGE_OK,		// 교환 성공			exchangeType(b) 0-Silver,1-Gold	
	MSG_EVENT_SONGKRAN_2007_EXCHANGE_NOITEM,	// 아이템 없음			
	MSG_EVENT_SONGKRAN_2007_EXCHANGE_FULLINVEN,	// 인벤 가득 참
}MSG_EVENT_SONGKRAN_2007_TYPE;
	


typedef enum _tagMsgEventEggsHunt2007Type
{
	MSG_EVENT_EGGS_HUNT_2007_GIFT_REP_OK,					// 아이템 보상 성공
	MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST,			// 아이템 중복 소유 안됨
	MSG_EVENT_EGGS_HUNT_2007_ERROR_GIFT_FULLINVEN,			// 인벤토리 부족
	MSG_EVENT_EGGS_HUNT_2007_ERROR_NOITEM,					// 아이템 없음
} MSG_EVENT_EGGS_HUNT_2007_TYPE;


typedef enum _tagEventEggsHunt2007ChangeType
{
	MSG_EGGS_HUNT_CHANGE_50_NAS,				// 50나스 
	MSG_EGGS_HUNT_CHANGE_500_NAS,				// 500나스
	MSG_EGGS_HUNT_CHANGE_QUALITY_STONE,			// 스톤 웨버		10개
	MSG_EGGS_HUNT_CHANGE_GAMMA_WAVE_ENERGY,		// C등급 원소		10개
	MSG_EGGS_HUNT_CHANGE_HERB,					// 크락의 줄기		10개
	MSG_EGGS_HUNT_CHANGE_PORTION_SPEED,			// 속도향상 물약
	MSG_EGGS_HUNT_CHANGE_BOOSTER,				// 부스터
	MSG_EGGS_HUNT_CHANGE_GREEN_STONE,			// 푸른돌
	MSG_EGGS_HUNT_CHANGE_MAGIC_STONE,			// 마법돌
	MSG_EGGS_HUNT_CHANGE_HEAVEN_STONE,			// 고급제련석
	MSG_EGGS_HUNT_CHANGE_EXPERIENCE_SCROLL,		// 경험의 스크롤
	MSG_EGGS_HUNT_CHANGE_PLATINUM_BOOSTER,		// 플래티늄 부스터
	MSG_EGGS_HUNT_CHANGE_CAKE,					// 케이크
	MSG_EGGS_HUNT_CHANGE_CACAO_90,				// 카카오 90
	MSG_EGGS_HUNT_CHANGE_SNOWMAN,				// 눈사람 인형
	MSG_EGGS_HUNT_CHANGE_GREATEST_EXP_PORTION,	// 태형결험의 묘약
	MSG_EGGS_HUNT_CHANGE_25000_NAS,				// 25000 나스
}MSG_EVENT_EGGS_HUNT_2007_CHANGE_TYPE;


typedef enum _tagMsgEventWhiteday2007
{
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ,		// 교환 요청			exchangeType(b) 0-Skill,1-SkyLetter,2-PinkLetter 
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_OK,		// 교환 성공			exchangeType(b) 0-Skill,1-SkyLetter,2-PinkLetter  if(0) skillIdx(n)-SkillIndex
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED,	// 교환 실패 ( 스킬이 실패하거나, 보상 아이템 생성에 실패 할때)
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM,	// 아이템 없음			
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_FULLINVEN,	// 인벤 가득 참
	MSG_EVENT_WHITEDAY_2007_EXCHANGE_MAXITEM,	// 아이템 소유 최대갯수 초과
	MSG_EVENT_WHITEDAY_2007_LETTER_REQ,			// 편지지 사용 요청		itemIndex(n), colorIndex(n), msg(str)
	MSG_EVENT_WHITEDAY_2007_LETTER_REP,			// 편지지 사용 응답		colorIndex(n), msg(str)
} MSG_EVENT_WHITEDAY_2007_TYPE;


typedef enum _tagMsgEventXmas2006
{
	MSG_EVENT_XMAS_2006_ERROR_OK,				// 성공
	MSG_EVENT_XMAS_2006_ERROR_FULLINVEN,		// 인벤 가득 참
	MSG_EVENT_XMAS_2006_ERROR_NOITEM,			// 아이템 없음
} MSG_EVENT_XMAS_2006_ERROR_TYPE;


typedef enum _tagMsgEventValentine2007Type
{
	MSG_EVENT_VALENTINE_2007_PACKAGE_OK,					// 초코렛 아이템 포장 성공			errcode(n)
	MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN,				// 인벤부족							errcode(n)

	MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE,			// 초코렛 저장 요청					itempos(b), itemindex(n), bingoitemindex(n)
	MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_OK,			// 초코렛 저장 성공					errcode(n) score(n)
	MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_NOTPACKAGE,	// 포장된 초코렛 아이템이 아님		errcode(n)
	MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_ALREADY,	// 이미 저장된 자리					errcode(n)

	MSG_EVENT_VALENTINE_2007_BINGO_GIFT,					// 보상요청							bingoindex(n)
	MSG_EVENT_VALENTINE_2007_BINGO_GIFT_OK,					// 보상성공							errcode(n), num(n)
	MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM,				// 빙고가 하나도 없음				errcode(n)

	MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND,			// 빙고박스 아이템 없음				errcode(n)
} MSG_EVENT_VALENTINE_TYPE;


typedef enum _tagMsgEventChuseok2006Type
{
	MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE,					// 송편 만들기 요청
	MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_OK,				// 송편 만들기 성공
	MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FAIL,				// 송편 만들기 실패
	MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM,			// 송편 재료 부족
	MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FULLINVEN,			// 인벤토리 부족

	MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE,				// 오색 송편 만들기 요청
	MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_OK,			// 오색 송편 만들기 성공
	MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FAIL,			// 오색 송편 만들기 실패
	MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM,		// 오색 송편 재료 부족
	MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FULLINVEN,		// 인벤토리 부족

	MSG_EVENT_CHUSEOK_2006_GIFT,							// 오색 송편 교환 요청
	MSG_EVENT_CHUSEOK_2006_GIFT_OK,							// 오색 송편 교환 성공
	MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM,					// 오색 송편 부족
	MSG_EVENT_CHUSEOK_2006_GIFT_OK_FULLINVEN,				// 인벤토리 부족

} MSG_EVENT_CHUSEOK_2006_TYPE;

typedef enum _tagMsgEventCollectBugType
{
	MSG_EVENT_COLLECT_BUG_BUY_BOX_REQ,					// [C]상자 구매 요청
	MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_OK,				// [S]상자 구매 응답 1이면 골든: isgoldenbox(n)
	MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_DUPLICATE,	// [S]이미 가지고 있음
	MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN,	// [S]인벤 공간 없음
	MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_MONEY,		// [S]돈 없음

	MSG_EVENT_COLLECT_BUG_GIFT_REQ,						// [C]보상 요청
	MSG_EVENT_COLLECT_BUG_GIFT_REP_OK,					// [S]보상 0부터 11까지: gifttype(n)
	MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN,		// [S]인벤 공간 없음
	MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOITEM,		// [S]보상 가능 상자 없음
	MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOTFULL,		// [S]상자에 곤충이 부족

	MSG_EVENT_COLLECT_BUG_PICK_OK,						// [S]곤충 얻음 : bugitemdbindex(n)
	MSG_EVENT_COLLECT_BUG_PICK_NOITEM,					// [S]채집 상자 없음
	MSG_EVENT_COLLECT_BUG_PICK_FULL,					// [S]채집 상자 가득 참

	MSG_EVENT_COLLECT_BUG_DROP,							// [C,S]곤충 상자에서 n번 곤충 버림 : position(n)
														// plus -> 0 ~ 7, used 8 ~ 15
} MSG_EVENT_COLLECT_BUG_TYPE;

typedef enum _tagMsgEventTldMotherday2007ErrorType
{
	MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_OK,			// 교환 성공
	MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_FULLINVEN,	// inven full
	MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_NOITEM,		// itemd not exist
}MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_TYPE;

typedef enum _tagMsgEventTldBuddhistErrorType
{
	MSG_EVENT_TLD_BUDDHIST_ERROR_OK,
	MSG_EVENT_TLD_BUDDHIST_ERROR_FULLINVEN,
	MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM,
} MSG_EVENT_TLD_BUDDHIST_ERROR_TYPE;

typedef enum _tagMsgEventRain2006ErrorType
{
	MSG_EVENT_RAIN_2006_ERROR_OK,				// 보상 성공
	MSG_EVENT_RAIN_2006_ERROR_NOITEM,			// 빗방울/빗물병 없음
	MSG_EVENT_RAIN_2006_ERROR_FULLINVEN,		// 인벤토리 가득참
} MSG_EVENT_RAIN_2006_ERROR_TYPE;

typedef enum _tagMsgEventGoldenballType
{
	MSG_EVENT_GOLDENBALL_VOTE_START,			// 응모 시작(존이동시도 보냄)	: team1(str) team2(str) yyyy(n) mm(n) dd(n) hh(n) min(n)
	MSG_EVENT_GOLDENBALL_VOTE_END,				// 응모 마감					: team1(str) team2(str)
	MSG_EVENT_GOLDENBALL_GIFT_START,			// 보상 시작(존이동시도 보냄)	: team1(str) team1score(n) team2(str) team2score(n) yyyy(n) mm(n) dd(n) hh(n) min(n)
	MSG_EVENT_GOLDENBALL_GIFT_END,				// 보상 마감					: team1(str) team2(str)

	MSG_EVENT_GOLDENBALL_VOTE,					// 응모							: team1score(n) team2score(n) errorcode(n:s)
	MSG_EVENT_GOLDENBALL_GIFT,					// 보상							: errorcode(n:s)
} MSG_EVENT_GOLDENBALL_TYPE;

typedef enum _tagMsgEventGoldenballErrorType
{
	MSG_EVENT_GOLDENBALL_ERROR_VOTE_OK,			// 골든볼 응모 - 성공
	MSG_EVENT_GOLDENBALL_ERROR_VOTE_ALREADY,	// 골든볼 응모 - 이미 응모함
	MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM,		// 골든볼 응모 - 축구공 없음
	MSG_EVENT_GOLDENBALL_ERROR_VOTE_FULLINVEN,	// 골든볼 응모 - 인벤 가득 참
	MSG_EVENT_GOLDENBALL_ERROR_VOTE_TIME,		// 골든볼 응모 - 응모 기간 아님

	MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_EXACT,	// 골든볼 보상 - 승리국/점수 적중 보상 성공
	MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_WINNER,	// 골든볼 보상 - 승리국 적중 보상 성공
	MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_NONE,	// 골든볼 보상 - 참가상 보상 성공
	MSG_EVENT_GOLDENBALL_ERROR_GIFT_FULLINVEN,	// 골든볼 보상 - 인벤 가득 참
	MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM,		// 골든볼 보상 - 응모 카드 없음
	MSG_EVENT_GOLDENBALL_ERROR_GIFT_TIME,		// 골든볼 보상 - 보상 기간 아님
} MSG_EVENT_GOLDENBALL_ERROR_TYPE;

typedef enum _tagMsgEventWorldcupType
{
	MSG_EVENT_WORLDCUP_VOTE,					// 승리국가 응모			: country(n) errorcode(n:s)
	MSG_EVENT_WORLDCUP_GIFT,					// 당첨 선물 받기			: rank(n) errorcode(n:s)
												// rank : 1, 2, 3 - 각 순위 별 보상, 4 - 그외
} MSG_EVENT_WORLDCUP_TYPE;

typedef enum _tagMsgEventWorldcupErrorType
{
	MSG_EVENT_WORLDCUP_ERROR_VOTE_OK,			// 승리 국가 응모 : 성공
	MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM,		// 승리 국가 응모 : FIFA 컵 부족
	MSG_EVENT_WORLDCUP_ERROR_VOTE_NOTCHANGE,	// 승리 국가 응모 : 응모 기간 아님
	MSG_EVENT_WORLDCUP_ERROR_VOTE_FULLINVEN,	// 승리 국가 응모 : 인벤 공간 부족
	MSG_EVENT_WORLDCUP_ERROR_VOTE_ALREADY,		// 승리 국가 응모 : 이미 응모한 국가

	MSG_EVENT_WORLDCUP_ERROR_GIFT_OK,			// 승리 국가 응모 당첨 확인 : 지급 성공
	MSG_EVENT_WORLDCUP_ERROR_GIFT_NOTCHANGE,	// 승리 국가 응모 당첨 확인 : 교환 기간 아님
	MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM,		// 승리 국가 응모 당첨 확인 : 국기 아이템 없음
	MSG_EVENT_WORLDCUP_ERROR_GIFT_FULLINVEN,	// 승리 국가 응모 당첨 확인 : 인벤 공간 부족
} MSG_EVENT_WORLDCUP_ERROR_TYPE;

typedef enum _tagMsgEventRecommendType
{
	MSG_EVENT_RECOMMEND_SET,					// 추천 서버 설정			: flag(c)
	MSG_EVENT_RECOMMEND_POTION,					// 포션 드롭
	MSG_EVENT_RECOMMEND_MOONSTONE,				// 문스톤 당첨				: charindex(n) charname(str)
} MSG_EVENT_RECOMMEND_TYPE;

typedef enum _tagMsgEventOXQuizType
{
	MSG_EVENT_OXQUIZ_INVALID_TIME,				// 퀴즈 진행 시간이 아닌데 들어가려고 할 경우
	MSG_EVENT_OXQUIZ_START,						// OX 퀴즈 시작
	MSG_EVENT_OXQUIZ_QUIZ,						// 퀴즈 제시				: quizno(n) limitsec(n) quiz(str)
	MSG_EVENT_OXQUIZ_ANSWER,					// 정답 알림				: quizno(n) isAnswerO(c)
} MSG_EVENT_OXQUIZ_TYPE;

typedef enum _tagEventSearchFriendErrorType
{
	MSG_EVENT_SEARCHFRIEND_ERROR_OK,				 //정상.(친구등록OK, 삭제 OK)
	MSG_EVENT_SEARCHFRIEND_ERROR_PACKET,			 //패킷오류
	MSG_EVENT_SEARCHFRIEND_ERROR_NOT_EXIST,			 //존재하지 않는다.
	MSG_EVENT_SEARCHFRIEND_ERROR_FULLMEMBER,		 //친구 정원 초과.
	MSG_EVENT_SEARCHFRIEND_ERROR_ALREADY_EXIST,		 //이미 친구리스트에 존재.
	MSG_EVENT_SEARCHFRIEND_ERROR_NOT_MATCH_INDEX,	 //서로간  요청,승인 아이디가 일치하면 안됨.
	MSG_EVENT_SEARCHFRIEND_ERROR_HELPER_SERVER,		 //헬퍼서버와의 접속오류.
	MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER,		 //기간이 지났음.
	MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME,		 //이름이 잘못되었다.
} MSG_EVENT_SEARCHFRIEND_ERROR_TYPE;

typedef enum _tagEventSearchFriendGoodsErrorType
{
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK,				// 지급 성공
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TIME,			// 지급 실패 : 시간 부족
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_END,				// 지급 실패 : 기간 아님
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_SOMEREMAIN,		// 지급 성공, 일부 아이템 미지급
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOGOODS,			// 지급 실패, 받을 아이템 없음
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOSPACE,			// 지급 실패 : 아이템 칸이 모자름.
	MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_HELPER,			// 헬퍼 서버 오류.
} MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE;

typedef enum _tagEventNewYear2006ErrorType
{
	MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_OK,				// 지급 성공
	MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TIME,			// 지급 실패 : 시간 부족
	MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_END,				// 지급 실패 : 기간 아님
	MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_ALREADY,			// 지급 실패 : 이미 받음
} MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TYPE;

typedef enum _tagEventXMas2005ErrorType
{
	MSG_EVENT_XMAS_2005_ERROR_OK,				// 지급 성공
	MSG_EVENT_XMAS_2005_ERROR_NO_ITEM,			// 지급 실패
} MSG_EVENT_XMAS_2005_ERROR_TYPE;

typedef enum _tagEvent2pan4panGoodsErrorType
{
	MSG_EVENT_2PAN4PAN_GOODS_ERROR_OK,			// 지급 성공
	MSG_EVENT_2PAN4PAN_GOODS_ERROR_SOMEREMAIN,	// 지급 성공, 일부 아이템 미지급
	MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOGOODS,		// 지급 실패, 받을 아이템 없음
	MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOSPACE,		// 공간 부족
} MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE;

typedef enum _tagEvent2pan4panBoxErrorType
{
	MSG_EVENT_2PAN4PAN_BOX_ERROR_OK,			// 지급 성공
	MSG_EVENT_2PAN4PAN_BOX_ERROR_SOMEREMAIN,	// 지급 성공, 일부 아이템 미지급
	MSG_EVENT_2PAN4PAN_BOX_ERROR_NOBOX,			// 지급 실패, 받을 아이템 없음
	MSG_EVENT_2PAN4PAN_BOX_ERROR_NOSPACE,		// 공간 부족
} MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE;

typedef enum _tagEvent2pan4panWordType
{
	MSG_EVENT_2PAN4PAN_WORD_SSAWAR,				// 코믹액션싸워
	MSG_EVENT_2PAN4PAN_WORD_2PAN4PAN,			// 이판사판
	MSG_EVENT_2PAN4PAN_WORD_LASTCHAOS,			// 라스트카오스
	MSG_EVENT_2PAN4PAN_WORD_LAGHAIM,			// 라그하임
	MSG_EVENT_2PAN4PAN_WORD_NORABA,				// 노라바

	// 여기부터는 오류 코드
	MSG_EVENT_2PAN4PAN_WORD_OK,					// 지급성공
	MSG_EVENT_2PAN4PAN_WORD_NOITEM,				// 아이템부족
	MSG_EVENT_2PAN4PAN_WORD_ALREADY,			// 이미 지급
} MSG_EVENT_2PAN4PAN_WORD_TYPE;

typedef enum _tagEventErrorType
{
	MSG_EVENT_ERROR_ALREADY_GIVE,				// 이미 지급되었음
	MSG_EVENT_ERROR_INSUFF_CONDITION,			// 조건 불충분 : 프리오픈(참여안함) : 오픈 베타 ( 참여안함 )
	MSG_EVENT_ERROR_FULLINVENTORY,				// 인벤토리 가득참
	MSG_EVENT_ERROR_WRONG_PACKET,				// 무기교환 : 패킷 오류
	MSG_EVENT_ERROR_NO_MATCH,					// 무기교환 : 동레벨 교환 대상 아이템 없음
	MSG_EVENT_ERROR_CANT_OPTION,				// 무기교환 : 옵션 이동 불능
	MSG_EVENT_ERROR_CHUSEOK_NO_CROP,			// 추석 이벤트, 작물 없음
	MSG_EVENT_ERROR_CHUSEOK_NO_SOIL,			// 추석 이벤트, 배양토 없음
	MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE,	// 추석 이벤트, 더이상 업그레이드 불가능
	MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK,			// 추석 이벤트, 업그레이드 성공
	MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL,		// 추석 이벤트, 업그레이드 실패
	MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK,		// 추석 이벤트, 교환 성공
	MSG_EVENT_ERROR_SEPTEMBER_NAS,				// 9월 이벤트, 나스 당첨
	MSG_EVENT_ERROR_SEPTEMBER_DROP,				// 9월 이벤트, 드롭율 당첨
	MSG_EVENT_ERROR_SEPTEMBER_EXP,				// 9월 이벤트, 경험치 당첨
	MSG_EVENT_ERROR_SEPTEMBER_SP,				// 9월 이벤트, 숙련도 당첨
	MSG_EVENT_GUILD_REWARD,						// 태국 길드 오류 때문에 보상 : nas(ll) sp(ll) 
	MSG_EVENT_ERROR_NOT_COUPON,					// 패키지 아이템 지급, 없는 시리얼
	MSG_EVENT_ERROR_ALREADY_USE_COUNPON,		// 패키지 아이템 지급, 이미 지급 받은 쿠폰
	MSG_EVENT_ERROR_COUPON_SUCCESS,				// 패키지 아이템 지급, 지급 성공
	MSG_EVENT_ERROR_OPENBETA_SUCCESS,			// 오픈베타 아이템 지급 성공 : 일본  : 미국
	MSG_EVENT_ERROR_SAKURABUD_SUCCESS,			// 꽃봉오리 아이템 지급 성공 : cmd(c)
	MSG_EVENT_ERROR_SAKURABUD_LEAKITEM,			// 꽃봉오리 아이템 지급 실패 꽃봉오리 부족 : 일본
//	MSG_EVENT_ERROR_NOT_PROMOTION_ITEM,			// 패키지 아이템 지급, 없는 아이템
} MSG_EVENT_ERROR_TYPE;

typedef enum _tagEventLetterType
{
	MSG_EVENT_LETTER_REQ,						// 교환 요청	: count(n) itemdbindex(n):count		: C -> S
	MSG_EVENT_LETTER_LASTCHAOS,					// 라스트카오스 당첨								: S -> C
	MSG_EVENT_LETTER_NAKO,						// 나코 당첨										: S -> C
	MSG_EVENT_LETTER_KNIGHT,					// 나이트 당첨										: S -> C
	MSG_EVENT_LETTER_MERAC,						// 메라크 당첨										: S -> C
	MSG_EVENT_LETTER_CRIS,						// 크리스 당첨										: S -> C
	MSG_EVENT_LETTER_LAMIAN,					// 라미안 당첨										: S -> C
	MSG_EVENT_LETTER_ARCHWAND,					// 아크완드 당첨									: S -> C
	MSG_EVENT_LETTER_NOMATCH,					// 낱말 맞추기 실패									: S -> C
	MSG_EVENT_LETTER_WRONGPACKET,				// 이상한 패킷 보냄
} MSG_EVENT_LETTER_TYPE;

typedef enum _tagEventLattoType
{
	MSG_EVENT_LATTO_UPGRADE_SUCCESS,			// "인첸트 역전 라또 아이템 +2에 당첨 되었습니다"
	MSG_EVENT_LATTO_BLOOD_SUCCESS,				// "블러드 역전 라또 아이템 옵션에 당첨 되었습니다"
	MSG_EVENT_LATTO_PROCESS_SUCCESS,			// "가공 역전 라또 아이템에 당첨 되었습니다"
	MSG_EVENT_LATTO_EXPSP_SUCCESS,				// "레벨 역전 라또 (경험치 *%, 숙련도 *)에 당첨 되었습니다" : exp(n) sp(n)
	MSG_EVENT_LATTO_PARTY_SUCCESS,				// "파티 역전 라또 아이템 복주머니에 당첨 되었습니다" : 시스템 창으로 msg 처리
	MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REQ,		// 복주머니 교환 요청
	MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP,		// 복주머니 교환 응답		: type (c)
} MSG_EVENT_LATTO_TYPE;

typedef enum _tagEventLattoChangeLuckyBagType
{
	MSG_CHANGE_26LEVEL_GENERAL_STONE,			// 26레벨 일반 제련석
	MSG_CHANGE_SPECIAL_STONE,					// 고급제련석
	MSG_CHANGE_22LEVEL_EVENT_WEAPON,			// 22레벨 이벤트 무기 (종족별)
	MSG_CHANGE_31LEVEL_ARMOR,					// 31레벨 방어구 상의 (종족별)
	MSG_CHANGE_ATTACK_UP,						// 공격력 향상제
	MSG_CHANGE_DEFENSE_UP,						// 방어력 향상제
	MSG_CHANGE_50000NAS,						// 5만 나스
	MSG_CHANGE_NO_LUCKYBAG,						// 교환할 복주머니가 없다
} MSG_EVENT_LATTO_CHANGE_LUCKYBAG_TYPE;

typedef enum _tagPersonalShopType
{
	MSG_PERSONALSHOP_ERROR,						// 개인상점 관련 오류		: errorcode(uc)
	MSG_PERSONALSHOP_CHANGE,					// 상점 상태 변화(중지)		: charindex(n) shoptype(c)
	MSG_PERSONALSHOP_SELL_START,				// 판매시작					: charindex(n) shoptype(c) shopname(str) [normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:client
												// normal_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
												// pack_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	MSG_PERSONALSHOP_SELL_LIST,					// 판매리스트 요청			: charindex(n) [ shoptype(c) normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:server
												// normal_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) price(ll)
												// pack_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)
	MSG_PERSONALSHOP_BUY,						// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)	
} MSG_PERSONALSHOP_TYPE;

typedef enum _tagPersonalShopErrorType
{
	MSG_PERSONALSHOP_ERROR_OK,					// 정상
	MSG_PERSONALSHOP_ERROR_INSUFF_SKILL,		// 요청 캐릭터가 스킬 사용 중일때
	MSG_PERSONALSHOP_ERROR_INSUFF_WARP,			// 요청 캐릭터가 워프(이동장치) 중일때
	MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE,		// 요청 캐릭터가 교환 중일때
	MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY,		// 요청 캐릭터가 이미 개인상점 중일때
	MSG_PERSONALSHOP_ERROR_INSUFF_MONEY,		// 나스를 개인상점으로 거래하고자 할때
	MSG_PERSONALSHOP_ERROR_INSUFF_ETC,			// 그외 상점을 열수 없는 상태(패킷내용오류 등)
	MSG_PERSONALSHOP_ERROR_AREA,				// 상점 개설 불가능 지역일때
	MSG_PERSONALSHOP_ERROR_DUPLICATION,			// 하나의 아이템을 여러 번 등록하려 할 때
	MSG_PERSONALSHOP_ERROR_NOTFOUND,			// 대상 캐릭터가 없을 때
	MSG_PERSONALSHOP_ERROR_NOTSELL,				// 대상 캐릭터가 판매 상점이 아닐 때
	MSG_PERSONALSHOP_ERROR_NOTPACKAGE,			// 대상 캐릭터가 패키지 판매가 아닐 때
	MSG_PERSONALSHOP_ERROR_MONEY,				// 소지금 부족
	MSG_PERSONALSHOP_ERROR_COUNT,				// 대상 개인상점보다 요청 판매/구매 카운트가 클때
	MSG_PERSONALSHOP_ERROR_CANTPREMIUM,			// 소지금 부족으로 패키지 상점 개설 불가
	MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY,	// 인벤토리 가득차서 못 삼
	MSG_PERSONALSHOP_ERROR_ANYMORE_PET,			// 펫 소유 제한이 넘어서 취소
	MSG_PERSONALSHOP_ERROR_NOSPACE,				// 인벤이 가득차서 판매 불가 : 돈 지급 받을 공간이 없음
} MSG_PERSONALSHOP_ERROR_TYPE;



typedef enum _tagRightAttackType
{
	MSG_RIGHT_ATTACK_ADD,						// 정당방위 리스트 추가
	MSG_RIGHT_ATTACK_DELAY,						// 정당방위 리스트 해제 예정 : 5s
	MSG_RIGHT_ATTACK_DEL,						// 정당방위 리스트 삭제
} MSG_RIGHT_ATTACK_TYPE;

typedef enum _tagStashType
{
	MSG_STASH_ERROR,							// 창고 관련 오류			: errorcode(uc)
	MSG_STASH_ISSETPASSWORD,					// 창고 암호 설정 여부 검사	: isset(c:s)
	MSG_STASH_LIST_REQ,							// 창고 아이템 리스트 요청	: password(str)
	MSG_STASH_LIST_REP, 						// 창고 아이템 리스트		: listflag(c) item_idx(n) plus(n) flag(n) used(n), used_2(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) remain(n)
												// listflag					: (1 << 0): start, (1 << 1): end, (1 << 2): empty
												// COMPOSITE_TIME 사용시 used_2(n) 추가
	MSG_STASH_KEEP_REQ,							// 창고에 물건 맡기기		: password(str) keepcount(n) [row(c) col(c) item_idx(n) count(ll)]:keepcount
												// keepcount				: 1 ~ 10
	MSG_STASH_KEEP_REP,							// 맡기기 결과				: keepcount(n) [itemdbindex(n) count(ll)]:keepcount
	MSG_STASH_TAKE_REQ,							// 찾기 요청				: password(str) takecount(n) [item_idx(n) count(ll)]:takecount
												// takecount				: 1 ~ 10
	MSG_STASH_TAKE_REP,							// 찾기 결과				: takecount(n) [itemdbindex(n) count(ll)]:keepcount
	MSG_STASH_CHANGE_PASSWORD_REQ,				// 패스워드 변경 요청		: old(str) new(str)
	MSG_STASH_CHANGE_PASSWORD_REP,				// 패스워드 변경 결과		: isset(c:s)
	MSG_STASH_SEAL,								// 잠금 요청/결과

	MSG_STASH_DELETE_PASSWORD_REQ,				// 암호분실시 삭제 요청		: idnum(str)
	MSG_STASH_DELETE_PASSWORD_REP,				// 암호분실 결과			: success(c:s)

	MSG_STASH_CHECK_PASSWORD_REQ,				// 암호 확인				: pass(str)
	MSG_STASH_CHECK_PASSWORD_REP,				// 결과						
	MSG_STASH_SETTING_NEWPASSWORD_REQ,			// 암호 설정				: pass(str)	
	MSG_STASH_SETTING_NEWPASSWORD_REP,			// 결과						: bsuccess(c)
												//	bsuccess 0: 성공 1: 실패 2: 암호가 삭제
} MSG_STASH_TYPE;

typedef enum _tagStashErrorType
{
	MSG_STASH_ERROR_OK,							// 정상
	MSG_STASH_ERROR_CANNOT_STATE,				// 창고 이용을 할수 없는 상태: 죽은 상태, 전투중, 스킬 시전중, 워프중, 개인상점, 교환중에서는 불가능
	MSG_STASH_ERROR_NOTCOMPLETE,				// 이전 창고 요청이 마무리 되지 않았음
	MSG_STASH_ERROR_SEALED,						// 창고 긴급 잠김 상태
	MSG_STASH_ERROR_WRONG_PASSWORD,				// 암호 틀림
	MSG_STASH_ERROR_WRONG_PACKET,				// 패킷 내용 오류
	MSG_STASH_ERROR_MONEY,						// 돈 부족
	MSG_STASH_ERROR_STASH_FULL,					// 창고 가득 참
	MSG_STASH_ERROR_WEIGHT_OVER,				// 무게 초과로 찾을 수 없음
	MSG_STASH_ERROR_INVENTORY_FULL,				// 인벤토리 개수 초과로 찾을 수 없음
	MSG_STASH_ERROR_WRONG_CHAR,					// 암호에 잘못된 문자가 들어감
	MSG_STASH_ERROR_CANNOT_SEAL,				// 잠글수 없음
	MSG_STASH_ERROR_ALREADY_SEAL,				// 이미 잠김
} MSG_STASH_ERROR_TYPE;

typedef enum _tagGuildType {MSG_GUILD_ERROR, MSG_GUILD_CREATE, MSG_GUILD_INFO, MSG_GUILD_MEMBERLIST, MSG_GUILD_ONLINE, MSG_GUILD_LEVELUP, MSG_GUILD_LEVELINFO, MSG_GUILD_BREAKUP, MSG_GUILD_BREAKUP_NOTIFY, MSG_GUILD_REGIST_REQ, MSG_GUILD_REGIST_ALLOW, MSG_GUILD_REGIST_CANCEL, MSG_GUILD_OUT_REQ, MSG_GUILD_CHANGE_BOSS, MSG_GUILD_APPOINT_OFFICER, MSG_GUILD_FIRE_OFFICER, MSG_GUILD_KICK, MSG_GUILD_MEMBER_ADD, MSG_GUILD_MEMBER_OUT, MSG_GUILD_MEMBER_KICK, MSG_GUILD_INFO_CHANGE, MSG_GUILD_BATTLE_REQ_REQ, MSG_GUILD_BATTLE_REQ_REJECT, MSG_GUILD_BATTLE_REQ_ACCEPT, MSG_GUILD_BATTLE_STOP_REQ, MSG_GUILD_BATTLE_STOP_REJECT, MSG_GUILD_BATTLE_STOP_ACCEPT, MSG_GUILD_BATTLE_START, MSG_GUILD_BATTLE_STATUS, MSG_GUILD_BATTLE_END, MSG_GUILD_WAR_SET_TIME_REQ, MSG_GUILD_WAR_GET_TIME, MSG_GUILD_WAR_NOTICE_TIME, MSG_GUILD_WAR_NOTICE_TIME_REMAIN, MSG_GUILD_WAR_START, MSG_GUILD_WAR_JOIN_ATTACK_GUILD, MSG_GUILD_WAR_JOIN_DEFENSE_GUILD, MSG_GUILD_WAR_JOIN_ATTACK_CHAR, MSG_GUILD_WAR_JOIN_DEFENSE_CHAR, MSG_GUILD_WAR_POINT, MSG_GUILD_WAR_NOTICE_START_CASTLE, MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME, MSG_GUILD_WAR_CASTLE_STATE, MSG_GUILD_WAR_GATE_STATE, MSG_GUILD_WAR_ATTACK_SYMBOL, MSG_GUILD_WAR_END, MSG_GUILD_WAR_SET_TIME_REP, MSG_GUILD_WAR_INVALID_COMMAND, MSG_GUILD_STASH_HISTORY_REQ, MSG_GUILD_STASH_HISTORY_REP, MSG_GUILD_STASH_VIEW_REQ, MSG_GUILD_STASH_VIEW_REP, MSG_GUILD_STASH_TAKE_REQ, MSG_GUILD_STASH_TAKE_REP, MSG_NEW_GUILD_INCLINE_ESTABLISH, MSG_NEW_GUILD_MEMBER_ADJUST, MSG_NEW_GUILD_INFO, MSG_NEW_GUILD_MEMBERLIST, MSG_NEW_GUILD_SKILL, MSG_NEW_GUILD_NOTICE, MSG_NEW_GUILD_NOTICE_UPDATE, MSG_NEW_GUILD_NOTICE_REQUEST, MSG_NEW_GUILD_NOTICE_TRANSMISSION, MSG_NEW_GUILD_MANAGE, MSG_NEW_GUILD_SKILL_LEARN, MSG_NEW_GUILD_POINT_RANKING, MSG_GUILD_BATTLE_COMMAND_REQ, MSG_GUILD_BATTLE_COMMAND_START, MSG_GUILD_BATTLE_COMMAND_STOP, MSG_GUILD_BATTLE_COMMAND_END, MSG_GUILD_BATTLE_KILL_MASTER, MSG_GUILD_BATTLE_NOW_SCORE, MSG_GUILD_BATTLE_BEFORE, MSG_GUILD_BATTLE_END_NOTICE_SAME_ZONE, MSG_GUILD_BATTLE_START_NOTICE_SAME_ZONE, MSG_GUILD_GP_INFO, MSG_CASTLE_BUFF, MSG_GUILD_WAR_SET_TIME_MENU, MSG_GUILD_WAR_GET_TIME_UNI_REQ, MSG_GUILD_WAR_GET_TIME_UNI_REP, MSG_NEW_GUILD_MARK_EDIT_WND, MSG_NEW_GUILD_MARK_EDIT_FIN, MSG_NEW_GUILD_MARK_EXPIRE, MSG_NEW_GUILD_STASH_LIST, MSG_NEW_GUILD_STASH_KEEP, MSG_NEW_GUILD_STASH_TAKE, MSG_NEW_GUILD_STASH_LOG, MSG_NEW_GUILD_STASH_ERROR, MSG_GUILD_MASTER_KICK_REQ, MSG_GUILD_MASTER_KICK_REP, MSG_GUILD_MASTER_KICK_CANCEL_REQ, MSG_GUILD_MASTER_KICK_CANCEL_REP, MSG_GUILD_MASTER_KICK_STATUS, MSG_GUILD_REMOTE_JOIN_REQ, MSG_GUILD_REMOTE_JOIN_OK, MSG_GUILD_REMOTE_JOIN_NO};


//typedef enum _tagGuildType
//{
//	MSG_GUILD_ERROR,						// 길드 관련 오류		: errorcode(c)
//	MSG_GUILD_CREATE,						// 길드 생성			: name(str)
//	MSG_GUILD_INFO,							// 내 길드 정보 보내기	: guildindex(n) guildname(str) guildlevel(n) mypos(n) battle_guildindex(n) battle_killcount(n) battle_prize(n) battle_zone(n) battle_time(n)
//											// 길드 없으면 guildindex == -1, 이후 항목 생략
//	MSG_GUILD_MEMBERLIST,					// 길원 리스트			: guildindex(n) count(n) [index(n) name(str) position(n) online(c)]:count
//	MSG_GUILD_ONLINE,						// 길드원 온라인 상태	: guildindex(n) charindex(n) charname(str) online(c)
//	MSG_GUILD_LEVELUP,						// 길드 승급			: 
//	MSG_GUILD_LEVELINFO,					// 길드 레벨 정보		: guildindex(n) guildlevel(n)
//	MSG_GUILD_BREAKUP,						// 해산 요청			: 
//	MSG_GUILD_BREAKUP_NOTIFY,				// 길드 해산을 알림		: guildindex(n) guildname(str)
//	MSG_GUILD_REGIST_REQ,					// 가입 신청 요청		: guildindex(n) bossindex(n) requesterindex(n)
//	MSG_GUILD_REGIST_ALLOW,					// 10 가입 신청 허용		: guildindex(n) requesterindex(n)
//	MSG_GUILD_REGIST_CANCEL,				// 가입 신청 취소		: bCancelTarget(c)
//	MSG_GUILD_OUT_REQ,						// 탈퇴					: guildindex(n)
//	MSG_GUILD_CHANGE_BOSS,					// 단장 이임			: guildindex(n) current(n) newboss(n)
//	MSG_GUILD_APPOINT_OFFICER,				// 부단장 임명			: guildindex(n) charindex(n)
//	MSG_GUILD_FIRE_OFFICER,					// 부단장 해임			: guildindex(n) charindex(n)
//	MSG_GUILD_KICK,							// 퇴출	요청			: guildindex(n) charindex(n)
//	MSG_GUILD_MEMBER_ADD,					// 길원 추가 알림		: guildindex(n) charindex(n) charname(str)
//	MSG_GUILD_MEMBER_OUT,					// 길원 탈퇴 알림		: guildindex(n) charindex(n) charname(str)
//	MSG_GUILD_MEMBER_KICK,					// 길원 강퇴 알림		: guildindex(n) bossindex(n) charindex(n) charname(str)
//	MSG_GUILD_INFO_CHANGE,					// 20 길드 정보 변경		: charindex(n) guildindex(n) guildname(str) pos(n)
//
//	MSG_GUILD_BATTLE_REQ_REQ,				// 길드전 신청 요청		: charindex(n) or guildindex(n) guildname(n) prize(n) time(n)
//	MSG_GUILD_BATTLE_REQ_REJECT,			// 길드전 신청 거절		: reject_charindex(n)
//	MSG_GUILD_BATTLE_REQ_ACCEPT,			// 길드전 신청 허락		: guildindex1(n) guildname1(n) guildindex2(n) guildname2(n) prize(n) zone(n) time(n)
//	MSG_GUILD_BATTLE_STOP_REQ,				// 길드전 중단 요청		: charindex(n) or guildindex(n) guildname(n) 
//	MSG_GUILD_BATTLE_STOP_REJECT,			// 길드전 중단 거절		: reject_charindex(n)
//	MSG_GUILD_BATTLE_STOP_ACCEPT,			// 길드전 중단 허락		:
//	MSG_GUILD_BATTLE_START,					// 길드전 시작 알림		: guildindex1(n) guildname1(n) guildindex2(n) guildname2(n) prize(n) zone(n) time(n)
//	MSG_GUILD_BATTLE_STATUS,				// 길드전 현황			: guildindex1(n) guildname1(str) killcount1(n)guildindex2(n) guildname2(str) killcount2(n) battletime(n)
//	MSG_GUILD_BATTLE_END,					// 길드전 종료			: winner_guildindex(n) guildindex1(n) guildname1(n) guildindex2(n) guildname2(n) prize(n)
//
//	MSG_GUILD_WAR_SET_TIME_REQ,				// 30 공성 시간 설정 요청	: wday(n) hour(n)
//	MSG_GUILD_WAR_GET_TIME,					// 공성 시간 확인		: warerrorcode(n:s) guildindex(n:s) guildname(n:s) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_NOTICE_TIME,				// 공성 시간 확정 알림	: zoneindex(n) month(c) day(c) hour(c) min(c)
//	MSG_GUILD_WAR_NOTICE_TIME_REMAIN,		// 공성 시작 남은 시간 알림	: zoneindex(n) min(c)
//	MSG_GUILD_WAR_START,					// 공성 시작 알림		: zoneindex(n) remainSec(n)
//	MSG_GUILD_WAR_JOIN_ATTACK_GUILD,		// 공성 길드 신청 요청	: warerrorcode(n:s) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_JOIN_DEFENSE_GUILD,		// 수성 길드 신청 요청	: warerrorcode(n:s) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_JOIN_ATTACK_CHAR,			// 공성 용병 신청 요청	: warerrorcode(n:s) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_JOIN_DEFENSE_CHAR,		// 수성 용병 신청 요청	: warerrorcode(n:s) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_POINT,					// 야전 길드 포인트 알림: remain_sec(n) guild1_index(n) guild1_name(str) guild1_point(n) guild2_index(n) guild2_name(str) guild2_point(n) guild3_index(n) guild3_name(str) guild3_point(n) my_guild_point(n) defense_point(n)
//	MSG_GUILD_WAR_NOTICE_START_CASTLE,		// 40 야전끝 공성 시작 알림: zoneindex(n) remain_sec(n) guildindex1(n) guildname1(str) guildindex2(n) guildname2(str) guildindex3(n) guildname3(str)
//	MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME,	// 야전 남은 시간 알림	: zoneindex(n) remainSec(n)
//	MSG_GUILD_WAR_CASTLE_STATE,				// 공성 진행 상황 알림	: zoneindex(n) state(n) gatestate(n) remain_sec(n) guild1_index(n) guild1_name(str) guild1_point(n) guild2_index(n) guild2_name(str) guild2_point(n) guild3_index(n) guild3_name(str) guild3_point(n) my_guild_point(n) defense_point(n)
//											// state				: 0 - 일반, 1 - 야전, 2 - 공성
//	MSG_GUILD_WAR_GATE_STATE,				// 성문 정보 변경을 알림: old_gate_state(n) new_gate_state(n)
//	MSG_GUILD_WAR_ATTACK_SYMBOL,			// 권좌 공격 요청		: 
//	MSG_GUILD_WAR_END,						// 공성 끝남			: zoneindex(n) bWinDefense(c) owner_index(n) owner_name(str) char_index(n) char_name(str) nextMonth(n) nextDay(n) nextHour(n) wDay(n)
//	MSG_GUILD_WAR_SET_TIME_REP,				// 공성 시간 설정 응답	: warerrorcode(n) month(c:s) day(c:s) hour(c:s) min(c:s)
//	MSG_GUILD_WAR_INVALID_COMMAND,			// 공성중 불가능한 길드 명령을 요청 : req_msg_type(uc)
//											// req_msg_type : MSG_GUILD_TYPE
//
//	MSG_GUILD_STASH_HISTORY_REQ,			// 입금내역 요청		: 
//	MSG_GUILD_STASH_HISTORY_REP,			// 입금내역 응답		: errcode(n) [month(n) day(n) money(ll)]:7
//	MSG_GUILD_STASH_VIEW_REQ,				// 50 출금전 잔액 요청		: 
//	MSG_GUILD_STASH_VIEW_REP,				// 잔액 응답			: errcode(n) money(ll)
//	MSG_GUILD_STASH_TAKE_REQ,				// 출금 요청			: money(ll)
//	MSG_GUILD_STASH_TAKE_REP,				// 출금 응답			: errcode(n)
//
//	MSG_NEW_GUILD_INCLINE_ESTABLISH,			// 길드 성향 변경		: guildindex(n:s) charindex(n:s)  guildincline(c)
//	MSG_NEW_GUILD_MEMBER_ADJUST,				// 길드원 정보 수정	:charIndex(n) name(s) exp(n) fame(n)
//
//	MSG_NEW_GUILD_INFO,		// 길드의 새로운 정보 : guildname(str) guildlevel(n) bossname(str) membercount(n) maxmembercount(n) avelevel(n) guildpoint(n) contributepoint(n)
//	MSG_NEW_GUILD_MEMBERLIST,					// 새로운 길드의 멤버 리스트 : maxmembercount(n) charname(str) positionname(str) charlevel(n) bplaying(b) zoneindex(n) contributepoint(n) job
//	MSG_NEW_GUILD_SKILL,
//	MSG_NEW_GUILD_NOTICE,				// 길드의 공지 사항 요청 
//	MSG_NEW_GUILD_NOTICE_UPDATE,		// 60 길드 공지 사항 업데이트 
//	MSG_NEW_GUILD_NOTICE_REQUEST,	// 길드 공지 하기
//	MSG_NEW_GUILD_NOTICE_TRANSMISSION,	// 길드 공지 하기	
//	MSG_NEW_GUILD_MANAGE,				// 길드의 새로운 매니저 : errorcode(n), membercount(n), guildpos(n), membername(str) positionname(str) charlevel(n) bplaying(b) contributeExp(n) contributeFame(n)
//
//	MSG_NEW_GUILD_SKILL_LEARN,		// 길드 스킬 습득 c->s: skillindex(n)
//									// s->c: skill_index(n) is_new(uc) skill_level(c)
//	MSG_NEW_GUILD_POINT_RANKING,	// 길드 랭킹 전송 charindex(n) guildindex(n) ranking(n)	
//} MSG_GUILD_TYPE;

typedef enum _tagGuildStashErrorType
{
	MSG_GUILD_STASH_ERROR_OK,				// 성공
	MSG_GUILD_STASH_ERROR_NOHISTORY,		// 히스토리 없음
	MSG_GUILD_STASH_ERROR_NOTBOSS,			// 길드장/부관 아님
	MSG_GUILD_STASH_ERROR_NOTENOUGH,		// 잔액 부족
	MSG_GUILD_STASH_ERROR_FAIL_DB,			// 시스템 오류

	// 이전 메시지는 헬퍼의 길드 창고 오류와 동일해야함
	MSG_GUILD_STASH_ERROR_NOSPACE,			// 인벤토리 공간 부족으로 지급 불가
} MSG_GUILD_STASH_ERROR_TYPE;

typedef enum _tagGuildErrorType
{
	MSG_GUILD_ERROR_OK,							// 성공
	MSG_GUILD_ERROR_PACKET,						// 패킷 내용 오류
	MSG_GUILD_ERROR_GUILDNAME,					// 이름 길이(2~16)가 잘못되었거나, 길드 이름에 잘못된 문자(\'"%_) 포함
	MSG_GUILD_ERROR_ALREADYBOSS,				// 길드장이나 부장은 길드 창설 불가능
	MSG_GUILD_ERROR_CHARLEVEL,					// 캐릭터 레벨 부족
	MSG_GUILD_ERRRO_SKILLPOINT,					// 스킬 포인트 부족
	MSG_GUILD_ERROR_MONEY,						// 나스 부족
	MSG_GUILD_ERROR_GAMESERVER,					// 게임서버 오류로 인해서 생성 불가
	MSG_GUILD_ERROR_DUPNAME,					// 이름 중복
	MSG_GUILD_ERROR_NOGUILD,					// 길드 없는데 길드 관련 메시지를 요청할 때
	MSG_GUILD_ERROR_NOTBOSS,					// 길드장이 아닌데 관련 메시지를 요청할 때
	MSG_GUILD_ERROR_MAXLEVEL,					// 이미 최고레벨 길드인데 승급 요청할 때
	MSG_GUILD_ERROR_CREATE_OK,					// 만들기 성공
	MSG_GUILD_ERROR_LEVELUP_OK,					// 레벨업 성공
	MSG_GUILD_ERROR_BREAKUP_OK,					// 해산 성공
	MSG_GUILD_ERROR_TARGET_NOBOSS,				// 대상이 길드장이 아니어서 가입 신청을 할 수 없음
	MSG_GUILD_ERROR_TARGET_BUSY,				// 대상이 다른 사람에게 요청을 받음
	MSG_GUILD_ERROR_SOURCE_BUSY,				// 내가 다른 사람에게 요청한 후 결정 안된 상태에서 다시 제3자에게 요청
	MSG_GUILD_ERROR_FULLMEMBER,					// 길드 정원이 초과
	MSG_GUILD_ERROR_REGIST_OK,					// 길드원 가입 완료
	MSG_GUILD_ERROR_OUTBOSS,					// 길드장이 탈퇴하려면 뷁함
	MSG_GUILD_ERROR_OUT_OK,						// 탈퇴 성공
	MSG_GUILD_ERROR_NOTFOUNDMEMBER,				// 강퇴, 임명, 이임등에서 대상 길원이 없을때
	MSG_GUILD_ERROR_CANTKICKBOSS,				// 길장은 당퇴 불능
	MSG_GUILD_ERROR_KICK_OK,					// 퇴출 성공
	MSG_GUILD_ERROR_CHANGE_BOSS_OK,				// 보스 변경 성공
	MSG_GUILD_ERROR_CANTOFFICER,				// 길드장이나 부관을 다시 부관으로 임명시
	MSG_GUILD_ERROR_APPOINT_OFFICER_OK,			// 부관 임명 성공
	MSG_GUILD_ERROR_FULLOFFICER,				// 부관 가득 참
	MSG_GUILD_ERROR_NOTOFFICER,					// 부관이 아니라서 해임 불가
	MSG_GUILD_ERROR_FIRE_OFFICER_OK,			// 해임 성공S
	MSG_GUILD_ERROR_REGDELAY,					// 탈퇴후 7일이 지나지 않아 가입 불능
	MSG_GUILD_ERROR_CANTKICK,					// 강퇴 불가능(부단장이 부단장 강퇴시)
	MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE,	// 길드전 중에는 가입신청 불가
	MSG_GUILD_ERROR_CANNOT_BREAK_BATTLE,		// 갈드전 - 길드전 중 길드 해체 불가
	MSG_GUILD_ERROR_CANNOT_KICK_BATTLE,			// 갈드전 - 길드전 중 퇴출 불가
	MSG_GUILD_ERROR_CANNOT_OUT_BATTLE,			// 갈드전 - 길드전 중 탈퇴 불가

	MSG_GUILD_ERROR_BATTLE,						// 길드전 에러		: errtype(uc)
	MSG_GUILD_ERROR_CANNOT_BREAK_WAR,			// 현재는 성주 길드 해체가 불가능

	MSG_NEW_GUILD_ERRROR_GUILDPOINT,				// 길드 레벨업시 길드 포인트 부족 
	MSG_NEW_GUILD_ERROR_SAME_INCLNE,				// 길드 성향이 같음
	MSG_NEW_GUILD_ERROR_CHANG_INCLINE,				// 길드 성향 변경 실패
	MSG_NEW_GUILD_ERROR_INCLINE_OK,					// 길드 성향 변경 성공
	MSG_NEW_GUILD_ERROR_ADJUST_FAIL,				// 길드 멤버들의  설정 변경 실패
	MSG_NEW_GUILD_ERROR_ADJUST_OK,	

	MSG_NEW_GUILD_INFO_ERROR_OK,				// 새로운 길드 정보 요청 성공
	MSG_NEW_GUILD_MEMBERLIST_ERROR_OK,			// 새로운 길드 멤버 리스트 요청 성공
	MSG_NEW_GUILD_MANAGE_ERROR_OK,				// 새로운 길드 매니저 요청 성공
	MSG_NEW_GUILD_NOTICE_ERROR_OK,				// 새로운 길드 공지 요청 성공
	MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_OK,		// 새로운 길드 공지 업데이트 성공
	MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_FAIL,		// 새로운 길드 공지 업데이트 실패
	MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_OK,
	MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL,
	MSG_NEW_GUILD_SKILL_ERROR_OK,				// 길드 스킬 성공
	MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST,			// 길드 스킬이 없음
	MSG_NEW_GUILD_NOTICE_ERROR_NOT_MESSAGE,		// 메세지가 없음.
	MSG_NEW_GUILD_SKILL_LEARN_ERROR_OK,
	MSG_NEW_GUILD_SKILL_LEARN_ERROR_LEVEL,		// 레벨 부족
	MSG_NEW_GUILD_SKILL_ALEADY_LEARN,			//  이미 배운 스킬 
} MSG_GUILD_ERROR_TYPE;

typedef enum _tagGuildErrorBattleType
{
	MSG_GUILD_ERROR_BATTLE_NOGUILD,				// 길드전 - 관련 길드가 없음
	MSG_GUILD_ERROR_BATTLE_NOTBOSS,				// 길드전 - 길드장이 없음
	MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT,			// 길드전 - 인원 수 부족
	MSG_GUILD_ERROR_BATTLE_ZONE,				// 길드전 - 가능 존이 아님
	MSG_GUILD_ERROR_ALREADY_BATTLE,				// 길드전 - 이미 길드전 중
	MSG_GUILD_ERROR_BATTLE_PRIZE,				// 길드전 - 배팅금액 자체 오류 (길드전 배팅 허용금액이 아님)
	MSG_GUILD_ERROR_BATTLE_NOT_REQ,				// 길드전 - 신청한적 없음
	MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE,			// 길드전 - 배팅금액 모질라
	MSG_GUILD_ERROR_BATTLE_NOT_BATTLE,			// 길드전 = 길드전 중이 아님

	MSG_GUILD_ERROR_BATTLE_TIME,				// 길드전 - 시간이 잘못 됨

} MSG_GUILD_EROOR_BATTLE_TYPE;

typedef enum _tagGuildPositionType
{
	MSG_GUILD_POSITION_BOSS,					// 길드 마스터
	MSG_GUILD_POSITION_OFFICER,					// 길드 부관
	MSG_GUILD_POSITION_MEMBER,					// 길드원
	MSG_GUILD_POSITION_UNKNOWN = 99,			// 
} MSG_GUILD_POSITION_TYPE;

typedef enum _tagGuildWarErrorType
{
	MSG_GUILD_WAR_ERROR_OK,						// 성공
	MSG_GUILD_WAR_ERROR_PROGRESS_WAR,			// 공성 진행 중
	MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD,		// 수성측이 공성하겠다고 우길 때
	MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD,			// 길드 없는 넘이 공성 신청하겠다고 우길 때
	MSG_GUILD_WAR_ERROR_JOIN_ALREADY,			// 이미 신청한 넘이 자꾸 하겠다고 할 때
	MSG_GUILD_WAR_ERROR_JOIN_TIME,				// 신청 가능 시간이 아닐 때
	MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL,		// 길드 레벨 부족해서 공성 신청 불가
	MSG_GUILD_WAR_ERROR_JOIN_GUILDMEMBER,		// 길드원 부족해서 공성 신청 불가
	MSG_GUILD_WAR_ERROR_JOIN_ITEM,				// 영웅의 증표 없음
	MSG_GUILD_WAR_ERROR_JOIN_MONEY,				// 돈 없이 공성 신청
	MSG_GUILD_WAR_ERROR_JOIN_NO_OWNER,			// 성주 없는 성에는 수성용병 불가
	MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD,		// 길드의 방침에 따라 개인 용병 신청 불가
	MSG_GUILD_WAR_ERROR_JOIN_CHARLEVEL,			// 캐릭레벨 부족
	MSG_GUILD_WAR_ERROR_NOT_OWNER,				// 성주도 아니면서 설정을 바꾸려 할때
	MSG_GUILD_WAR_ERROR_NOT_SETTIME,			// 설정 가능 시간이 아님
	MSG_GUILD_WAR_ERROR_INVALID_TIME,			// 설정 불가능 시간대
	MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD_BOSS,	// 일반 길드원은 공성 신청 불가
} MSG_GUILD_WAR_ERROR_TYPE;

typedef enum _tagMsgHelperType
{
	MSG_HELPER_WHISPER_REQ,				// 귓말 요청		: sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)
	MSG_HELPER_WHISPER_REP,				// 귓말 응답		: sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)
	MSG_HELPER_WHISPER_TIMEOUT,			// 귓말 타임아웃	: sender_index(n) sender_name(str)
	MSG_HELPER_WHISPER_NOTFOUND,		// 대상 없음		: sender_index(n) sender_name(str)
	MSG_HELPER_WAR_NOTICE_TIME,			// 공성전 시간 알림	: zoneindex(n) month(c) day(c) hour(c) min(c)
	MSG_HELPER_WAR_NOTICE_TIME_REMAIN,	// 공성 남은 시간 알림	: zoneindex(n) min(c)
	MSG_HELPER_WAR_NOTICE_START,		// 공성전 시작 알림	: zoneindex(n) remainSec(n)
	MSG_HELPER_WAR_JOIN_ATTACK_GUILD,	// 공성 길드 추가	: zoneindex(n) guildindex(n)
	MSG_HELPER_WAR_JOIN_DEFENSE_GUILD,	// 수성 길드 추가	: zoneindex(n) guildindex(n)
	MSG_HELPER_WAR_NOTICE_START_ATTACK_CASTLE,	// 공성 시작 : zoneindex(n) remainSec(n) guildindex1(n) guildname1(str) guildindex2(n) guildname2(str) guildindex3(n) guildname3(str)
	MSG_HELPER_WAR_NOTICE_REMAIN_FIELD_TIME,	// 야전 남은 시간 알림	: zoneindex(n) remainSec(n)
	MSG_HELPER_WAR_NOTICE_END,			// 공성 끝남		: zoneindex(n) bWinDefense(c) owner_index(n) owner_name(str) char_index(n) char_name(str) nextMonth(n) nextDay(n) nextHour(n) wDay(n)

	MSG_HELPER_PARTY_MEMBER_CHANGE_JOB,	// 파티원 직업 변경 알림 : bossindex(n) charindex(n) job1(c) job2(c)
	MSG_HELPER_PARTY_CHAT,				// 파티 채팅		: bossindex(n) charindex(n) charname(str) chat(str)
	MSG_HELPER_PARTY_RECALL_PROMPT,		// 파티 리콜 요청	: bossindex(n) charindex(n) charname(str) isincastle(c) guildindex(n)
	MSG_HELPER_PARTY_RECALL_CONFIRM,	// 파티리콜 확인	: bossindex(n) reqcharindex(n) reqcharname(str) repcharindex(n) repcharname(str) yesno(c)
	MSG_HELPER_PARTY_RECALL_PROC,		// 파티리콜 실행	: bossindex(n) repcharindex(n) zone(n) x(f) z(f) y(c) isincastle(c) guildindex(n)

	MSG_HELPER_COMBO_GOTO_COMBO_PROMPT, // 콤보 파티리콜
	MSG_HELPER_COMBO_RECALL_TO_COMBO_PROMPT, //  콤보파티리콜 npc

	MSG_HELPER_END_MSG,					// 헬퍼 REQ, REP 메시지는 이 메시지보다 뒤에 오면 안됨
} MSG_HELPER_TYPE;

typedef enum _tagMsgHelperCommandType
{
	MSG_HELPER_SHUTDOWN,				// 0 헬퍼를 다운

	MSG_HELPER_GUILD_CREATE_REQ,		// 길드 생성 요청	: charindex(n) charname(str) guildname(str)
	MSG_HELPER_GUILD_CREATE_REP,		// 길드 생성 응답	: charindex(n) errorcode(uc)
	MSG_HELPER_GUILD_CREATE_NTF,		// 길드 추가 알림	: guildindex(n) guildlevel(n) guildname(str) bossindex(n) bossname(str)
	MSG_HELPER_GUILD_ONLINE_NTF,		// 길드원 온라인	: guildindex(n) charindex(n) online(c)
	MSG_HELPER_GUILD_MEMBER_LIST,		// 길드원 리스트	: guildindex(n) count(n) [charindex(n) charname(str) pos(n) online(c)]
	MSG_HELPER_GUILD_ONLINE,			// 온라인 변경		: guildindex(n) charindex(n) online(c)
	MSG_HELPER_GUILD_LOAD_REQ,			// 길드정보 요청	: idname(str) charindex(n)
	MSG_HELPER_GUILD_LOAD_REP,			// 길드정보 응답	: idname(str) charindex(n) errorcode(uc)
	MSG_HELPER_GUILD_LEVELUP_REQ,		// 길드 등업 요청	: guildindex(n) charindex(n)
	MSG_HELPER_GUILD_LEVELUP_REP,		// 10 길드 등업 응답	: guildindex(n) charindex(n) errorcode(uc)
	MSG_HELPER_GUILD_LEVELUP_NTF,		// 길드 등업 알림	: guildindex(n) guildlevel(n)
	MSG_HELPER_GUILD_BREAKUP_REQ,		// 길드 해산 요청	: bossindex(n) guildindex(n)
	MSG_HELPER_GUILD_BREAKUP_REP,		// 길드 해산 응답	: charindex(n) errcode(uc)
	MSG_HELPER_GUILD_BREAKUP_NTF,		// 길드 해산 알림	: guildindex(n)
	MSG_HELPER_GUILD_MEMBER_ADD_REQ,	// 멤버 추가 요청	: guildindex(n) boss(n) requester(n)
	MSG_HELPER_GUILD_MEMBER_ADD_NTF,	// 추가 알림		: guildindex(n) index(n) name(str) position(n)
	MSG_HELPER_GUILD_MEMBER_ADD_REP,	// 추가 응답		: guildindex(n) boss(n) requester(n) errcode(n)
	MSG_HELPER_GUILD_MEMBER_OUT_REQ,	// 탈퇴 요청		: guildindex(n) charindex(n)
	MSG_HELPER_GUILD_MEMBER_OUT_NTF,	// 탈퇴 알림		: guildindex(n) charindex(n) charname(str)
	MSG_HELPER_GUILD_MEMBER_OUT_REP,	// 20 탈퇴 응답		: guildindex(n) charindex(n) errcode(uc)
	MSG_HELPER_GUILD_MEMBER_KICK_REQ,	// 강퇴 요청		: guildindex(n) bossindex(n) charindex(n)
	MSG_HELPER_GUILD_MEMBER_KICK_REP,	// 강퇴 응답		: guildindex(n) bossindex(n) charindex(n) errcode(uc)
	MSG_HELPER_GUILD_MEMBER_KICK_NTF,	// 강퇴 강퇴 알림	: guildindex(n) bossindex(n) charindex(n)
	MSG_HELPER_GUILD_CHANGE_BOSS_REQ,	// 보스 변경 요청	: guildindex(n) current(n) change(n)
	MSG_HELPER_GUILD_CHANGE_BOSS_NTF,	// 보스 변경 알림	: guildindex(n) current(n) change(n)
	MSG_HELPER_GUILD_CHANGE_BOSS_REP,	// 보스 변경 응답	: guildindex(n) current(n) change(n) errcode(uc)
	MSG_HELPER_GUILD_APPOINT_OFFICER_REQ, // 부관 변경 요청 : guildindex(n) boss(n) officer(n)
	MSG_HELPER_GUILD_APPOINT_OFFICER_REP, // 부관 변경 응답 : guildindex(n) boss(n) officer(n) errcode(uc)
	MSG_HELPER_GUILD_APPOINT_OFFICER_NTF, // 부관 변경 알림 : guildindex(n) boss(n) officer(n)
	MSG_HELPER_GUILD_CHAT,				// 30 길드 채팅		: guildindex(n) charindex(n) charname(str) chat(str)
	MSG_HELPER_GUILD_FIRE_OFFICER_REQ,	// 부관 해임 요청	: guildindex(n) boss(n) officer(n)
	MSG_HELPER_GUILD_FIRE_OFFICER_REP,	// 부관 해임 응답	: guildindex(n) boss(n) officer(n) errcode(uc)
	MSG_HELPER_GUILD_FIRE_OFFICER_NTF,	// 부관 해임 알림	: guildindex(n) boss(n) officer(n)
	MSG_HELPER_GUILD_LOAD_NTF,			// 길드 읽기 응답	: guildindex(n) guildlevel(n) guildname(str) bossindex(n) bossname(str) battle_guildindex(n) battle_name(str) battle_prize(n) battle_time(n) battle_zone(n) battle_killcount(n) battle_state(c)

	MSG_HELPER_CHAR_DEL,				// 캐릭터 삭제		: charindex(n)

	MSG_HELPER_GUILD_BATTLE_REQ,		// 길드전 요청		: guildindex1(n) guildindex2(n) prize(n) zone(n) time(n)
	MSG_HELPER_GUILD_BATTLE_REP,		// 길드전 응답		: guildindex1(n) guildname1(str) guildindex2(n) guildname2(str) prize(n) zone(n) time(n)
	MSG_HELPER_GUILD_BATTLE_START,		// 길드전 시작		: guildindex1(n) guildindex2(n)
	MSG_HELPER_GUILD_BATTLE_STATUS,		// 길드전 현황		: guildindex1(n) guildname1(str) killcount1(n) guildindex2(n) guildname2(str) killcount2(n) battletime(n) battlezone(n)
	MSG_HELPER_GUILD_BATTLE_STOP_REQ,	// 40 길드전 종료 요청	: guildindex1(n) guildindex2(n)
	MSG_HELPER_GUILD_BATTLE_STOP_REP,	// 길드전 종료 응답	: winner_index(n) guildindex1(n) guildname1(str) guildindex2(n) guildname2(str) prize(n) zone(n)
	MSG_HELPER_GUILD_BATTLE_PEACE_REQ,	// 길드전 평화 요청	: guildindex(n)
	MSG_HELPER_GUILD_BATTLE_PEACE_REP,	// 길드전 평화 응답	: guildindex(n)
	MSG_HELPER_GUILD_BATTLE_KILL_REQ,	// 길드전 킬 증가	: of_guildindex(n) df_guildindex(n)

	MSG_HELPER_EVENT_MOONSTONE_LOAD,
	MSG_HELPER_EVENT_MOONSTONE_UPDATE_REQ,
	MSG_HELPER_EVENT_MOONSTONE_UPDATE_REP,
	MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REQ,
	MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REP,

	MSG_HELPER_FRIEND_MEMBER_ADD_REQ,		// 50 친구 추가 요청.
	MSG_HELPER_FRIEND_MEMBER_ADD_REP,		//친구 추가 응답.
	MSG_HELPER_FRIEND_MEMBER_DEL_REQ,		//친구 삭제 요청.
	MSG_HELPER_FRIEND_MEMBER_DEL_REP,		//친구 삭제 요청.
	MSG_HELPER_FRIEND_SET_CONDITION_REQ,	//상태변경요청.
	MSG_HELPER_FRIEND_SET_CONDITION_NOTIFY, //상태변경응답

	MSG_HELPER_PD4_RANK_INIT,			// pd4 rank DB init 
	MSG_HELPER_PD4_RANK_ADD,			// pd4 rank 시작	 : charIndex(n) 
	MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	MSG_HELPER_PD4_RANK_VIEW_REQ,		// : charindex(n) charjob(c)
	MSG_HELPER_PD4_RANK_VIEW_REP,		// 60 : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	MSG_HELPER_PD4_REWARD_VIEW_REQ,		// : charindex(n) charjob(c)
	MSG_HELPER_PD4_REWARD_VIEW_REP, 	// : charindex(n) multicount(c), charname(str) breward(c)
	MSG_HELPER_PD4_REWARD_REQ,			// : charindex(n)
	MSG_HELPER_PD4_REWARD_REP,			// : charindex(n) charrank(c) breward(c)
	MSG_HELPER_PD4_REWARD_INIT,			// pd4 reward를 0로 세팅
	MSG_HELPER_PD4_REWARD_END,			// pd4 reward를 1로 세팅 

	MSG_HELPER_PET_CREATE_REQ,			// 펫 생성 요청		: owner(n) type_grade(c)
	MSG_HELPER_PET_CREATE_REP,			// 펫 생성 응답		: index(n) owner(n) type_grade(c)
	MSG_HELPER_PET_DELETE_REQ_RESERVE,	// 미사용 : 펫 제거 요청		: index(n) owner(n)
	MSG_HELPER_PET_DELETE_REP_RESERVE,	// 70 미사용 : 펫 제거 응답	: index(n) owner(n)

	MSG_HELPER_GUILD_STASH_HISTORY_REQ,	// 입금내역 요청	: guildindex(n) charindex(n)
	MSG_HELPER_GUILD_STASH_HISTORY_REP,	// 입금내역 응답	: charindex(n) errcode(n) [month(n) day(n) money(ll)]:7
	MSG_HELPER_GUILD_STASH_VIEW_REQ,	// 잔액 요청		: guildindex(n) charindex(n)
	MSG_HELPER_GUILD_STASH_VIEW_REP,	// 잔액 응답		: charindex(n) errcode(n) money(ll)
	MSG_HELPER_GUILD_STASH_TAKE_REQ,	// 출금 요청		: guildindex(n) charindex(n) money(ll)
	MSG_HELPER_GUILD_STASH_TAKE_REP,	// 출금 응답		: guildindex(n) charindex(n) errcode(n) money(ll) balance(ll)
	MSG_HELPER_GUILD_STASH_ROLLBACK,	// 출금 취소		: guildindex(n) money(ll)
	MSG_HELPER_GUILD_STASH_SAVE_TAX_REQ,// 세금 입금		: guildindex(n) taxItem(ll) taxProduct(ll)
	MSG_HELPER_GUILD_STASH_SAVE_TAX_REP,// 세금 입금 응답	: errcode(n) guildindex(n) taxItem(ll) taxProduct(ll)
	
	MSG_HELPER_NAME_CHANGE_REQ,			// 이름 변경카드 사용 : gs -> hs: bguild(c) charindex(n) nickname(str) 
	MSG_HELPER_NAME_CHANGE_REP,			// bguild : 0 케릭닉 1 길드네임 hs->gs : bguild(c) charindex(n) error(c)

	MSG_HELPER_TEACHER_FAMEUP_REQ,			// 선생 명성치 업(선생, 학생): charindex(n) charindex(n) fame(n)
	MSG_HELPER_TEACHER_FAMEUP_REP,			

	MSG_HELPER_TEACHER_REGISTER_REQ,	// 선생 등록 요청	: teacheridx(n) bteacher(c) studentidx(n)
	MSG_HELPER_TEACHER_GIVEUP_REQ,		// 포기 요청		: teacheridx(n) studentidx(n)
	MSG_HELPER_TEACHER_REQ,				// 선생리스트		: charindex(n) bteacher(c)

	MSG_HELPER_TEACHER_TIMEOVER_REQ,	// 시간 오버 해제	: bteacher(c) teacheridx(n) studentidx(n)
	MSG_HELPER_TEACHER_SUPERSTONERECIEVE,// 초고제 받기		: charindex(c)

	MSG_HELPER_TEACHER_LOAD_REQ,		// 사제 시스템 읽기 : idname(str) charindex(n)
	MSG_HELPER_TEACHER_LOAD_REP,		// 사제 필요정보

	MSG_HELPER_MARGADUM_PVP_RANK,		// 마르가둠 랭킹 : count(n) charindex(n) damage(ll)

	MSG_HELPER_SEARCHFRIEND_ADD_REQ,	// 휴면 추가요청	: approvalindex(n) appnick(str) appjob(n) appuserindex(n) appserverold(n) reqnick(str)  			
	MSG_HELPER_SEARCHFRIEND_ADD_REP,	// 휴면 추가응답	: approvalindex(n) appnick(str) appjob(n) requestindex(n) reqnick(str) reqjob(n) errcode(uc)
	MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REQ,	// 휴면 선택 등록 요청		: approvalindex(n) requestindex(n)
	MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REP,	// 휴면 선택 등록 응답		: approvalindex(n) requestindex(n) errcode(uc)
	MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REQ,	// 한시간 단위저장요청		: timesec(n) appDormantindex(n) appDormantgood(n)
	MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REP,	// 한시간 단위저장응답		: timesec(n) appDormantindex(n) appDormantgood(n) errcode(uc)
	MSG_HELPER_SEARCHFRIEND_LISTGOOD_REQ,	// 아템 보상가능여부 요청	: approvalindex(n)
	MSG_HELPER_SEARCHFRIEND_LISTGOOD_REP,	// 아템 보상가능여부 응답	: approvalindex(n) request_listmember(n) startIndex(n) nTotal(n) requestnick(char)[] requestindex(n)[] approvalgood(n)[] 
	MSG_HELPER_SEARCHFRIEND_GOOD_REQ,		// 아템 보상결과 요청 : approvalindex(n) ndormantindex(n) itemgood(n)
	MSG_HELPER_SEARCHFRIEND_GOOD_REP,		// 아템 보상결과 응답 : approvalindex(n) errcode(uc)

	MSG_HELPER_PARTY_INVITE_REQ,		// 파티 초대 요청	: bossindex(n) bossname(str) bosslevel(n) targetindex(n) partytype(c)
	MSG_HELPER_PARTY_INVITE_REP,		// 파티 초대 응답	: bossindex(n) bossname(str) targetindex(n) targetname(str) partytype(c) errorcode(n)
	MSG_HELPER_PARTY_ALLOW_REQ,			// 파티 수락 요청	: bossindex(n) targetindex(n) targetname(str)
	MSG_HELPER_PARTY_ALLOW_REP,			// 파티 수락 응답	: bossindex(n) targetindex(n) targetname(str) errcode(n)
	MSG_HELPER_PARTY_REJECT_REQ,		// 파티 거부 요청	: bossindex(n) targetindex(n)
	MSG_HELPER_PARTY_REJECT_REP,		// 파티 거부 응답	: bossindex(n) targetindex(n) errcode(n)
	MSG_HELPER_PARTY_QUIT_REQ,			// 파티 탈퇴 요청	: bossindex(n) targetindex(n)
	MSG_HELPER_PARTY_QUIT_REP,			// 파티 탈퇴 응답	: bossindex(n) targetindex(n) errcode(n) newbossindex(n) newbossname(str)
	MSG_HELPER_PARTY_KICK_REQ,			// 파티 강퇴 요청	: bossindex(n) targetindex(n)
	MSG_HELPER_PARTY_KICK_REP,			// 파티 강퇴 응답	: bossindex(n) targetindex(n) errcode(n)
	MSG_HELPER_PARTY_CHANGE_BOSS_REQ,	// 파티장 변경 요청	: bossindex(n) newbossname(str)
	MSG_HELPER_PARTY_CHANGE_BOSS_REP,	// 파티장 변경 응답	: bossindex(n) bossname(str) newbossindex(n) newbossname(str)

	MSG_HELPER_PARTY_MATCH_REG_MEMBER_REQ,		// 파티 매칭 일반 등록 요청 : charIndex(n) charName(str) level(n) zone(n) job(c) partytype(c)
	MSG_HELPER_PARTY_MATCH_REG_MEMBER_REP,		// 파티 매칭 일반 등록 응답 : errorcode(n) charIndex(n) charName(str) level(n) zone(n) job(c) partytype(c)

	MSG_HELPER_PARTY_MATCH_REG_PARTY_REQ,		// 파티 매칭 파티 등록 요청 : bossindex(n) bossname(str) bosslevel(n) zone(n) jobflag(n) limitlevel(c) comment(str)
	MSG_HELPER_PARTY_MATCH_REG_PARTY_REP,		// 파티 매칭 파티 등록 응답 : errorcode(n) bossindex(n) bossname(str) bosslevel(n) zone(n) jobflag(n) limitlevel(c) partytype(c) comment(str)

	MSG_HELPER_PARTY_MATCH_DEL_REQ,				// 파티 매칭 등록 해제 요청 : charindex(n)
	MSG_HELPER_PARTY_MATCH_DEL_REP,				// 파티 매칭 등록 해제 요청 : charindex(n) errorcode(n)

	MSG_HELPER_PARTY_MATCH_INVITE_REQ,			// 파티 매칭 일반 초대 요청 : bossindex(n) bossname(str) bosslevel(n) charindex(n) partytype(c)
	MSG_HELPER_PARTY_MATCH_INVITE_REP,			// 파티 매칭 일반 초대 응답 : errorcode(n) bossindex(n) bossname(str) charindex(n) charname(str) partytype(c)

	MSG_HELPER_PARTY_MATCH_JOIN_REQ,			// 파티 매칭 파티 조인 요청 : bossindex(n) charindex(n) charname(str) charlevel(n) charjob(c)
	MSG_HELPER_PARTY_MATCH_JOIN_REP,			// 파티 매칭 파티 조인 응답 : errorcode(n) partytype(c) bossindex(n) bossname(str) charindex(n) charname(str) charjob(c)
	MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REQ,		// 파티 매칭 파티 조인 수락 : bossindex(n) charindex(n)
	MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REP,		// 파티 매칭 파티 조인 결과 : errorcode(n) bossindex(n) targetindex(n) targetname(str)

	MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REQ,		// 파티 매칭 파티 조인 거절 : joincharindex(n) rejectcharindex(n)
	MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REP,		// 파티 매칭 파티 거절 결과 : errorcode(n) joincharindex(n) rejectcharindex(n)

	MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO,	// 파티 일반 등록 정보 변경 : charindex(n) type(n) ...

	MSG_HELPER_PARTY_INFO_PARTY,				// 파티 정보 전달			: partytype(c) requestindex(n) requestname(str) membercount(n) [charindex(n) charname(str)] * membercount
	MSG_HELPER_PARTY_INFO_PARTY_MATCH_MEMBER,	// 파티 매칭 일반 등록 전달	: charindex(n) charname(str) level(n) zone(n) job(c) partytype(c)
	MSG_HELPER_PARTY_INFO_PARTY_MATCH_PARTY,	// 파티 매칭 파티 등록 전달	: bossindex(n) bossname(str) bosslevel(n) zone(n) jobflag(n) partytype(c) limitlevel(c) comment(str)
	MSG_HELPER_PARTY_INFO_END,					// 파티 정보 전달 완료

	MSG_HELPER_BLOCKPC_REQ,						// 블럭요청, 해당케릭터를 찾아서 정보를 보내줌 : reqIndex(n) blockName(str)
	MSG_HELPER_BLOCKPC_REP,						//							: reqIndex(n) blockIndex(n) blockName(str)

	MSG_HELPER_GIFT_RECVCHARNAME_REQ,			// 핼퍼에 해당 케릭터가 있는 : sendCharIndex(n) recvCharName(str) msg(str) count(n) idx(n) ctid(n)
	MSG_HELPER_GIFT_RECVCHARNAME_REP,			//							: sendCharIndex(n) recvUserIdx(n) recvCharIndex(n) revCharName(str) msg(str) count(n) idx(n) ctid(n)

	MSG_HELPER_SAVE_EXP_GUILD,					// 길드원 누적 경험치 저장	: guildindex(n) exp(d)

	MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST,	//어버이날 누적 포인트 리스트 전송 요청 gs->hs charindex
																// hs->gs :errorcode(n) charindex(n) count( n ) rankig( c ) savepoint( n ) gildname(str)
	MSG_HELPER_PARENTSDAY_2007_ADD_POINT,						// 포인트 등록 gc->hs : CharIndex(n) CarnationCount(n)
																// hs->gs : errorcode(c) CharIndex(n) Carnationcount(n) totalpoint(n)
	MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET,					// 누적 포인트로 인한 교환권 요청  gs->hs : GuildIndex( n ) CharIndex(n)
																// hs->gs : errorcode(c) CharIndex(n) GiftTicket(n)
	
	MSG_HELPER_CHILDRENSDAY_2007,				// subtype (uc)	
	
	MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM,					// 교환권으로 ITEM 교환  gs->hs : GuildIndex( n ) CharIndex(n)
																// hs->gs : errorcode(c) CharIndex(n) GiftItemIndex(n)
	MSG_HELPER_TEACH_2007,
	MSG_HELPER_TEACH_2007_ADDFLOWER,							// 노란 카네이션 지급해라 gs->hs : charindex(n)
	MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM_INSERT,			// Item Insert 쿼리  gs->hs : Charindex(n) GiftItemIndex(n)
	MSG_HELPER_PARENTSDAY_2007_NOTICE,
	MSG_HELPER_TEACH_2007_RECEIVE_FLOWER,						// 노란 카네이션 받았음 gs->hs : charindex(n)

	MSG_HELPER_FLOWERTREE_2007,									// subtype (uc) , Charindex(n)
	MSG_HELPER_UPDATE_CHAOKING,									// 카오킹 변경 REQ : bChao(c) charindex(n), charpenalty(n) 
																// REP : sucess(c) 0-실패 ,1-성공, bChao(c), charindex(n), charpenalty(n) //실패시 현재 킹 정보
	MSG_HELPER_ZONE_CHAO_ATTR,									// 카오존 속성 요청 subtype(uc)
	MSG_HELPER_UPDATE_FAMEUP,									// 헬퍼에 캐릭터 명성치 업데이트 : REQ : charIndex(n), fameup(n), REP:NONE
	



	MSG_HELPER_GUILD_MARK_TABLE,				// hs->gs : index1(n), index2(n), index3(n)
	MSG_HELPER_LEVELUP_LOG,						// gs->hs : charstr(str), level(n)
	MSG_HELPER_PET_COLOR_CHANGE,				// gs->hs : charindex(n) petindex(n) petcolor(c)
												
	MSG_HELPER_GUILD_INCLINE_ESTABLISH_REQ,			// 길드 성향 변경		: guildindex(n) charindex(n) guildincline(c)	
	MSG_HELPER_GUILD_INCLINE_ESTABLISH_REP,			// 길드 성향 변경		: guildindex(n) charindex(n) guildincline(c)
	
	MSG_HELPER_GUILD_MEMBER_ADJUST_REQ,				// 길드 멤버들의 기여도 및 포지션 이름 수정 요청: guildindex(n) count(n) charindex(n) positionname(str) contributeExp(n) contributeFame(n)
	MSG_HELPER_GUILD_MEMBER_ADJUST_REP,				// 길드 멤버들의 기여도 및 포지션 이름 수정 응답: guildindex(n) charindex(n) errorcode(n)

	MSG_HELPER_NEW_GUILD_INFO_REQ,					// 새로운 길드 정보 요청 : guildindex(n) charindex(n)
	MSG_HELPER_NEW_GUILD_INFO_REP,					// 새로운 길드 정보 응답 : charindex(n) errorcode(n)
	MSG_HELPER_NEW_GUILD_INFO_NTF,					// 새로운 길드 정보 전달

	MSG_HELPER_NEW_GUILD_MEMBERLIST_REQ,			// 새로운 길드원 정보 요청 : guildindx(n) charindex(n)
	MSG_HELPER_NEW_GUILD_MEMBERLIST_REP,			// 새로운 길드원 정보 응답 : charindex(n) guildindex(n) errorcode(n) membercount(n) membercharindex(n) cumulatePoint(n)

	MSG_HELPER_NEW_GUILD_MANAGE_REQ,				// 새로운 길드 매니저 요청 : guildindex(n) charindex(n)
	MSG_HELPER_NEW_GUILD_MANAGE_REP,				// 새로운 길드 매니저 응답 : 

	MSG_HELPER_NEW_GUILD_NOTICE_REQ,				// 새로운 길드 공지 요청 : guildindex(n) charindex(n)
	MSG_HELPER_NEW_GUILD_NOTICE_REP,				// 새로운 길드 공지 응답 : 

	MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REQ,			// 길드 공지 업데이트 요청 :
	MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REP,			// 길드 공지 업데이트 응답 :

	MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REQ,		// 길드원에게 공지 전송 요청 :
	MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_NTF,
	MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REP,

	MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REQ,
	MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REP,

	MSG_HELPER_NEW_GUILD_LOAD_NTF,
	MSG_HELPER_NEW_GUILD_MEMBER_LIST,
	MSG_HELPER_SAVE_GUILD_POINT,
	MSG_HELPER_SAVE_GUILD_MEMBER_POINT,
	MSG_HELPER_SAVE_GUILD_SKILL,
	
	MSG_HELPER_INSERT_GUARD,					// 공성 지역에 가디언 삽입 : gs->hs zoneidx(n) x(f) z(f) h(f) itemindex(n)
	MSG_HELPER_CASTLE_TOWER_ADD,				// 수호탑 추가 : zoneidx(n), 탑종류  i(n), 탑순서 j(n), 상태 nValue(b)

	MSG_HELPER_OPEN_ADULT_SERVER,
	MSG_HELPER_OPEN_ADULT_SERVER_REP,
	MSG_HELPER_UPDATE_GUILD_POINT,				// 길드 포인트 업데이트를 하기 위한 쿼리
	MSG_HELPER_PET_NAME_CHANGE,					// gs->hs : charindex(n) petindex(n) petname(str)
												// hs->gs : errorcode(c) charindex(n) petindex(n) petname(str)
												
	MSG_HELPER_CASTLE_TOWER_REINFORCE,			// 공성 지역 수호탑 강화 gs->hs zoneidx(n), type(c), step(c)
	MSG_HELPER_CASTLE_QUARTERS_INSTALL,			// 부활 진지 설치 qindex(n), gindex(n)
	MSG_HELPER_GUILD_NOTICE,					// 접속시 길드 공지 사항 전송
	MSG_HELPER_GUILD_SKILL_LEARN,				// 배운 스킬 리스트 DB에 저장

	MSG_HELPER_REWARD_IDC2007_REQ,		// 2007년 IDC이전으로 무조건 보상(요청)
	MSG_HELPER_REWARD_IDC2007_REP,		// 2007년 IDC이전으로 무조건 보상(응답)
	MSG_HELPER_HALLOWEEN_2007,			// REQ : (n)user_index	REP : (n)user_index, (c) sucess (0-성공, 1-이미 받은적 있음)

	MSG_HELPER_DVD_RATE_CHANGE,			// REQ : (c) nSubtype (n) rate REP : (c) nSubtype (n) rate

	MSG_HELPER_SAVE_MOONSTONE_MIX,		// 최상급문스톤 조합성공 저장
	MSG_HELPER_PET_TURNTO_NPC,			// 펫 모양 변경 (n)nCharIndex, (n)nPetIndex, (n)toNpc 변신 npc인덱스
	MSG_HELPER_ALTERNATEMERCHANT_START,	// 대리 상인 시작
	MSG_HELPER_ALTERNATEMERCHANT_BUY,	// 물건 구입
	MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY, // 물건 찾아오기
	MSG_HELPER_ALTERNATEMERCHANT_NAS_RECOVERY,	// 나스 찾아오기
	MSG_HELPER_ALTERNATEMERCHANT_END,		// 대리 상인 종료
	MSG_HELPER_ALTERNATEMERCHANT,			// 대리 상인 에러  전송

	MSG_HELPER_CREATE_COMBO,
	MSG_HELPER_TO_CONN_ADD_IPOINT,			// 헬퍼에서 게임서버를 통해 아이리스 포인트 적립 : REQ : NONE, REP : (n)UserIndex, (n)Type, (f)fIpoint
	
	MSG_HELPER_APET,						// AttackPet 에 관한 Message : REQ:Subtype(c) REP:Subtype(c)

	MSG_HELPER_PARTY_CHANGE_TYPE_REQ,
	MSG_HELPER_PARTY_CHANGE_TYPE_REP,
	MSG_HELPER_PARTY_END_PARTY_REQ,
	MSG_HELPER_PARTY_END_PARTY_REP,
	
	MSG_HELPER_PET_DELETE_REQ,
	MSG_HELPER_PET_DELETE_REP,

	MSG_HELPER_ADDCUBEPOINT_PERSONAL,
	MSG_HELPER_CUBESTATE_REQ,
	MSG_HELPER_CUBESTATE_REP,
	MSG_HELPER_CUBESTATE_PERSONAL_REQ,
	MSG_HELPER_CUBESTATE_PERSONAL_REP,
	MSG_HELPER_GUILDCUBETIME_NOTICE,
	MSG_HELPER_ADDCUBETAX_REQ,
	MSG_HELPER_ADDCUBETAX_REP,
	MSG_HELPER_ADDCUBEPOINT,				// 길드큐브포인트 획득
	MSG_HELPER_GUILDCUBETIME_END,
	MSG_HELPER_GUILDCUBETIME_START,			// 

	MSG_HELPER_EVENT_PHOENIX,				// 피닉스 이벤트 // yhj   081105  피닉스

	MSG_HELPER_TRADEAGENT_ERROR,			// 거래 대행 			
	MSG_HELPER_TRADEAGENT_REG_LIST_REQ,		
	MSG_HELPER_TRADEAGENT_REG_LIST_REP,
	MSG_HELPER_TRADEAGENT_REG_REQ,			
	MSG_HELPER_TRADEAGENT_REG_REP,
	MSG_HELPER_TRADEAGENT_REG_CANCEL_REQ,
	MSG_HELPER_TRADEAGENT_REG_CANCEL_REP,
	MSG_HELPER_TRADEAGENT_RETURNED,
	MSG_HELPER_TRADEAGENT_SEARCH_REQ,
	MSG_HELPER_TRADEAGENT_SEARCH_REP,
	MSG_HELPER_TRADEAGENT_BUY_REQ,
	MSG_HELPER_TRADEAGENT_BUY_REP,
	MSG_HELPER_TRADEAGENT_CALCLIST_REQ,
	MSG_HELPER_TRADEAGENT_CALCLIST_REP,	
	MSG_HELPER_TRADEAGENT_CALCULATE_REQ,
	MSG_HELPER_TRADEAGENT_CALCULATE_REP,
	MSG_HELPER_TRADEAGENT_CALCRESULT,
	MSG_HELPER_TRADEAGENT_CHECKCALC_REQ,	//정산 필요 체크(로그인시)
	MSG_HELPER_TRADEAGENT_CHECKCALC_REP,
	MSG_HELPER_TRADEAGENT_REGRESULT,		//등록 결과	

	MSG_HELPER_DISCONNECT_HACK_CHARACTER,	// 핵 캐릭터 디비로그 남기기

	MSG_HELPER_EXPED_ERROR,					// 원정대 -->
	MSG_HELPER_EXPED_CREATE_REQ,			
	MSG_HELPER_EXPED_CREATE_REP,			
	MSG_HELPER_EXPED_CREATE_RESULT,			
	MSG_HELPER_EXPED_INVITE_REQ,				
	MSG_HELPER_EXPED_INVITE_REP,			
	MSG_HELPER_EXPED_INVITE_RESULT,			
	MSG_HELPER_EXPED_ALLOW_REQ,			
	MSG_HELPER_EXPED_ALLOW_REP,
	MSG_HELPER_EXPED_ALLOW_RESULT,
	MSG_HELPER_EXPED_REJECT_REQ,			
	MSG_HELPER_EXPED_REJECT_REP,
	MSG_HELPER_EXPED_REJECT_RESULT,
	MSG_HELPER_EXPED_QUIT_REQ,				
	MSG_HELPER_EXPED_QUIT_REP,
	MSG_HELPER_EXPED_QUIT_RESULT,
	MSG_HELPER_EXPED_KICK_REQ,				
	MSG_HELPER_EXPED_KICK_REP,
	MSG_HELPER_EXPED_KICK_RESULT,
	MSG_HELPER_EXPED_MEMBERINFO_REQ,		
	MSG_HELPER_EXPED_MEMBERINFO_REP,
	MSG_HELPER_EXPED_ADMIN_REQ,				
	MSG_HELPER_EXPED_ADMIN_REP,
	MSG_HELPER_EXPED_ADMIN_RESULT,		
	MSG_HELPER_EXPED_CHANGETYPE_REQ,		
	MSG_HELPER_EXPED_CHANGETYPE_REP,
	MSG_HELPER_EXPED_CHANGETYPE_RESULT,	
	MSG_HELPER_EXPED_CHANGEBOSS_REQ,		
	MSG_HELPER_EXPED_CHANGEBOSS_REP,
	MSG_HELPER_EXPED_CHANGEBOSS_RESULT,	
	MSG_HELPER_EXPED_SETMBOSS_REQ,		
	MSG_HELPER_EXPED_SETMBOSS_REP,
	MSG_HELPER_EXPED_SETMBOSS_RESULT,  
	MSG_HELPER_EXPED_RESETMBOSS_REQ,		
	MSG_HELPER_EXPED_RESETMBOSS_REP,
	MSG_HELPER_EXPED_RESETMBOSS_RESULT,  
	MSG_HELPER_EXPED_VIEWDETAIL_REQ,		
	MSG_HELPER_EXPED_VIEWDETAIL_REP,	 	
	MSG_HELPER_EXPED_ENDEXPED_REQ,			
	MSG_HELPER_EXPED_ENDEXPED_REP,
	MSG_HELPER_EXPED_ENDEXPED_RESULT,  
	MSG_HELPER_EXPED_MOVEGROUP_REQ,			
	MSG_HELPER_EXPED_MOVEGROUP_REP,
	MSG_HELPER_EXPED_MOVEGROUP_RESULT,  
	MSG_HELPER_EXPED_ADDMEMBER_REQ,			
	MSG_HELPER_EXPED_ADDMEMBER_REP,
	MSG_HELPER_EXPED_ADDMEMBER_RESULT, 
	MSG_HELPER_EXPED_SET_LABEL_REQ,				
	MSG_HELPER_EXPED_SET_LABEL_REP,	
	MSG_HELPER_EXPED_SET_LABEL_RESULT,		// 원정대 <--	
	MSG_HELPER_EXPED_REJOIN_REQ,
	MSG_HELPER_EXPED_REJOIN_REP,
	MSG_HELPER_EXPED_CHAT_REQ,	
	MSG_HELPER_EXPED_CHAT_REP,	
} MSG_HELPER_COMMAND_TYPE;

typedef enum _tagHelperApetType
{
	MSG_HELPER_APET_CREATE_REQ,				// APET 생성 신청 (n) owner_indx, (n)proto_index
	MSG_HELPER_APET_CREATE_OK,				// 팻 생성 요청 성공
	MSG_HELPER_APET_CREATE_FAILED,			// 팻 생성 요청 실폐
	MSG_HELPER_APET_ACCENPT_REQ,			// APET 매모리 생성완료 (n)owner_index, (n) pet_index
	MSG_HELPER_APET_ACCENPT_OK,
	MSG_HELPER_APET_ACCENPT_FAILED,
	MSG_HELPER_APET_DELETE_REQ,				// 메모리에서 삭제 하고 DB에 삭제 요청
	MSG_HELPER_APET_CHANGE_OWNER_REQ,		// DB에 주인 변경 요청
	MSG_HELPER_APET_CHANGE_OWNER_OK,
	MSG_HELPER_APET_CHANGE_OWNER_FAILED,
} MSG_HELPER_APET_TYPE;

typedef enum _tagHelperZoneChaoAttType
{
	MSG_HELPER_ZONE_CHAO_ATT_REQ,								// 현재 전체 존 속성을 헬퍼에 요청 
	MSG_HELPER_ZONE_CHAO_ATT_CHANGE,							// 현화된 존 속성 변경 요청 count(n), zoneidx(n), extra(n), attribute(c), ( extra1, attribute1,) ...
	MSG_HELPER_ZONE_CHAO_ATT_REP,								// 횬재 또는 변경된 속성 count(n), ( zoneindex(n), attcount(n), attribute(c)... )....
} MSG_HELPER_ZONE_CHAO_ATT_TYPE;

typedef enum _tagHelperChildrensDay2007Type
{
	MSG_HELPER_CHILDRENSDAY_2007_CHECK,			// 아이템 지급 중복검사 : charindex(n), itemindex(n) ,needFlower(n) : 필요한 꽃의 개수, columnindex(c)
												//						: charindex(n), itemindex(n), needFlower(n), request(c) : 0-실패, 1-등록  
	MSG_HELPER_CHILDRENSDAY_2007_UPDATE,		// 아이템 지급 업데이트 : charindex(n), itemindex(n) 
} MSG_HELPER_CHILDRENSDAY_2007_TYPE;

typedef enum _tagHelperFlowerTree2007Type
{
	MSG_HELPER_FLOWERTREE_2007_MYPOINT,				// REQ : not / REP: point(un)....
	MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT,			// REQ : point(un)
	MSG_HELPER_FLOWERTREE_2007_RECV_TICKET,			// REQ : not / rep : count(n)
	MSG_HELPER_FLOWERTREE_2007_TICKET_UPDATE,		// REQ : count(n) 
	MSG_HELPER_FLOWERTREE_2007_PRICEITEM,			// REQ : not / REP : itemindex(n), count(n)
	MSG_HELPER_FLOWERTREE_2007_PRICEITEM_INSERT,	// REQ : isGM(uc) itemindex(n)
	MSG_HELPER_FLOWERTREE_2007_PRICEMSG,			// REP : itemindex(n)
}MSG_HELPER_FLOWERTREE_2007_TYPE;

typedef enum _tagHelperPartyMatchMemberChangeInfoType
{
	MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME,				// 이름 변경	: charname(str)
	MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL,			// 레벨 변경	: level(n)
	MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE,				// 존이동		: level(n) zone(n)
} MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE;

typedef enum _tagHelperPartyMatchErrorType
{
	MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_OK,					// 0 등록 성공
	MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY,		// 1 파티 중
	MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG,		// 2 기존 등록자

	MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK,					// 3 파티 매칭 파티 등록 성공
	MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS,				// 4 파티장이 아닌 사람이 요청시
	MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_FULL,				// 5 파티인원 가득참
	MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG,			// 6 이미 등록된 파티

	MSG_HELPER_PARTY_MATCH_ERROR_DEL_MEMBER,					// 7 일반 등록에서 해제됨
	MSG_HELPER_PARTY_MATCH_ERROR_DEL_PARTY,						// 8 파티 등록에서 해제됨

	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_OK,						// 9 정상
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOT_FOUND,				// 10 파티 매칭에 없는 사람을 초대
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_LEVEL,					// 11 파티 타입이 PARTY_TYPE_BATTLE 일때 레벨 검사에 부적합함
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOBOSS,					// 12 파티장이 아닌 사람이 요청시
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE,				// 13 파티 타입이 맞지 않음
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET,			// 14 대상이 이미 파티 중
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_FULL,					// 15 파티 인원이 초과
	MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST,		// 16 다른 사람에게 요청 중일때

	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_OK,						// 17 조인 성공
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET,			// 18 조인을 요청한 사라밍 이미 다른 파티에 가입 중
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND,				// 19 조인 하려고 하는 파티가 매칭 테이블에 없음
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL,					// 20 파티 타입이 PARTY_TYPE_BATTLE이고 레벨 검사에 부적합
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL,				// 21 레벨 제한 조건에 부적합
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_JOB,						// 22 직업 부적합
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_FULL,						// 23 인원 가득참
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST,			// 24 기존 초대 요청자가 있어서 불가능

	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_OK,					// 25 파티 조인 성공
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_NOT_PARTY,			// 26 파티가 없음

	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_OK,				// 27 파티 조인 거절 성공
	MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_FAIL,				// 28 파티 조인 거절 실패

} MSG_HELPER_PARTY_MATCH_ERROR_TYPE;

typedef enum _tagHelperPartyErrorType
{
	MSG_HELPER_PARTY_ERROR_INVITE_OK,				// 0 성공
	MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_ME,		// 1 요청자가 파티 중
	MSG_HELPER_PARTY_ERROR_INVITE_DIFF_TYPE,		// 2 파티 타입이 기존과 다름
	MSG_HELPER_PARTY_ERROR_INVITE_LEVEL,			// 3 대상자 레벨 조건 안됨
	MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_TARGET,	// 4 대상자가 파티 중
	MSG_HELPER_PARTY_ERROR_INVITE_FULL,				// 5 파티 인원 초과
	MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_REQUEST,	// 6 다른 사람에게 이미 요청 중

	MSG_HELPER_PARTY_ERROR_ALLOW_OK,				// 7 성공
	MSG_HELPER_PARTY_ERROR_ALLOW_NOT_PARTY,			// 8 파티 중이 아닐때

	MSG_HELPER_PARTY_ERROR_REJECT_OK,				// 9 성공
	MSG_HELPER_PARTY_ERROR_REJECT_FAIL,				// 10 실패

	MSG_HELPER_PARTY_ERROR_QUIT_OK,					// 11 성공
	MSG_HELPER_PARTY_ERROR_QUIT_END,				// 12 성공, 파티 해체

	MSG_HELPER_PARTY_ERROR_KICK_OK,					// 13 성공
	MSG_HELPER_PARTY_ERROR_KICK_END,				// 14 성공, 파티 해체

	MSG_HELPER_PARTY_ERROR_INVITE_PVP,				// 15 요청자나 대상자가 PVP 중

} MSG_HELPER_PARTY_ERROR_TYPE;

typedef enum _tagHelperGuildStashErrorType
{
	MSG_HELPER_GUILD_STASH_ERROR_OK,			// 성공
	MSG_HELPER_GUILD_STASH_ERROR_NOHISTORY,		// 히스토리 없음
	MSG_HELPER_GUILD_STASH_ERROR_NOTBOSS,		// 길드장/부관 아님
	MSG_HELPER_GUILD_STASH_ERROR_NOTENOUGH,		// 잔액 부족
	MSG_HELPER_GUILD_STASH_ERROR_FAIL_DB,		// 시스템 오류
} MSG_HELPER_GUILD_STASH_ERROR_TYPE;

typedef enum _tagHelperOpenAdultServerErrorType
{
	MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB,			// 보상요청
	MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_OK,		// 보상성공 등수(n)
	MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_ALREADY, // 이미 교환했음

	MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED,  // 보상실패

}MSG_HELPER_OPEN_ADULT_SERVER_ERROR_TYPE;

//typedef enum _tagHelperParentsDay2007Type
//{
//	MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST,	//어버이날 누적 포인트 리스트 전송 요청 gs->hs
//																// hs->gs :count( n ) rankig( c ) savepoint( n ) gildname(str) gildmaster(str)
//	MSG_HELPER_PARENTSDAY_2007_ADD_POINT,						// 포인트 등록 gc->hs : CharIndex(n) echangeItemIndex(n)
//																// hs->gs : errorcode(c) point(n) totalpoint(n)
//	MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET,					// 누적 포인트로 인한 교환권 요청  s->c : errorcode(c) changeTicketCount(n)
//}MSG_HELPER_PARENTSDAY_2007_TYPE;




typedef enum _tagMsgChangeType
{
	MSG_CHANGE_START,					// 변신성공 시작	: charindex(n) mobindex(n)
	MSG_CHANGE_STOP,					// 변신 중지		: charindex(n)
	MSG_CHANGE_CANCEL,					// 변신 취소 요청	: 
	MSG_CHANGE_ERR,						// 변신 요청시 에러	: errtype(uc)
} MSG_CHANGE_TYPE;

typedef enum _tagMsgChangeErrType
{
	MSG_CHANGE_ERR_DEAD,				// 변신에러			: 죽었을때 변신 불가
	MSG_CHANGE_ERR_SITDOWN,				// 변신에러			: 앉아있을때 변신 불가
	MSG_CHANGE_ERR_WRAP,				// 변신에러			: 이동장치 사용중 일때 변신 불가
	MSG_CHANGE_ERR_SKILL,				// 변신에러			: 스킬 사용중 일때 변신 불가
	MSG_CHANGE_ERR_EXCHANGE,			// 변신에러			: 교환중일때 변신 불가
	MSG_CHANGE_ERR_PARTY,				// 변신에러			: 파티 신청 중일때 변신 불가
	MSG_CHANGE_ERR_PK,					// 변신에러			: PK 중일때 변신 불가
	MSG_CHANGE_ERR_LEVEL,				// 변신에러			: 레벨이 모질라 변신 불가
	MSG_CHANGE_ERR_ZONE,				// 변신에러			: 존이 틀려 변신 불가
	MSG_CHANGE_ERR_BATTLE,				// 변신에러			: 전투중 변신 불가
	MSG_CHANGE_ERR_PERSONAL_SHOP,		// 변신에러			: 개인상점중 변신 불가
} MSG_CHANGE_ERR_TYPE;

typedef enum _tagMsgUIType
{
	MSG_UI_PLUS_EFFECT_REQ,				// 플러스이펙트요청	: pluseffect(c)
	MSG_UI_PLUS_EFFECT_REP,				// 플러스이펙트응답	: pluseffect(c) charindex(n)
} MSG_UI_TYPE;

typedef enum _tagMsgTeachType
{
	MSG_TEACH_TEACHER_LIST,				// 선생 리스트		: subtype(uc)
	MSG_TEACH_TEACHER_REQ,				// 선생 요청 과정	: subtype(uc)
	MSG_TEACH_END,						// 종료				: teacher_index(n) teacher_name(str) student_index(n) student_name(str) end_type(uc)
	MSG_TEACH_INFO,						// 사제 정보		: bteacher_list(c) char_type(c) teacher_index(n) teach_level(n) teacher_name(str) teacher_job(c) teacher_job2(c) or [student_index(n) student_level(n) student_name(str) student_job(c) student_job2(c)] * 8
	MSG_TEACH_STUDENT_LEVELUP,			// 학생 레벨업		: student_index(n) student_level(n) student_name(str) student_job(c) student_job2(c)

	MSG_TEACH_ERR,						// 에러				: errtype(uc)

} MSG_TEACH_TYPE;

typedef enum _tagMsgTeachTeacherListType
{
	MSG_TEACH_TEACHER_LIST_UP,			// 리스트 등록
	MSG_TEACH_TEACHER_LIST_DN,			// 리스트 등록 취소
	MSG_TEACH_TEACHER_LIST_SHOW,		// 리스트 목록		: [char_index(n) char_name(str) char_job(c) char_job2(c)] * 20

} MSG_TEACH_TEACHER_LIST_TYPE;

typedef enum _tagMsgTeachTeacherReqType
{
	MSG_TEACH_TEACHER_REQ_REQ,			// 선생 요청		: charindex(n) char_name(str)
	MSG_TEACH_TEACHER_REQ_REJECT,		// 선생 요청 거절	: reject_charindex(n) reject_charname(str)
	MSG_TEACH_TEACHER_REQ_ACCEPT,		// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
	MSG_TEACH_TEACHER_GIVEUP,			// 학생 포기 요청	: teacher_index(n) teacher_name(str) student_index(n) student_name(str)

} MSG_TEACH_TEACHER_REQ_TYPE;

typedef enum _tagMsgTeachCharType
{
	MSG_TEACH_NO_TYPE = -1,				// 사제 관계 없음				
	MSG_TEACH_TEACHER_TYPE = 0,			// 선생
	MSG_TEACH_STUDENT_TYPE,				// 학생
	MSG_TEACG_NO_STUDENT_TYPE,			// 한번 사제 했던 학생 : 더이상 불가능

} MSG_TEACH_CHAR_TYPE;

typedef enum _tagMsgTeachEndType
{
	MSG_TEACH_END_FAIL,					// 실패
	MSG_TEACH_END_SUCCESS,				// 성공
} MSG_TEACH_END_TYPE;

typedef enum _tagMsgTeachErrType
{
	MSG_TEACH_ERR_SYSTEM,					// 나오면 안되는 오류
	MSG_TEACH_ERR_TEACHER_LEVEL,			// 선생 가능레벨이 아님
	MSG_TEACH_ERR_TEACHER_LIST_ALREADY,		// 이미 선생 리스트에 있음
	MSG_TEACH_ERR_TEACHER_LIST_NOT,			// 선생 리스트에 있지 않음
	MSG_TEACH_ERR_TEACHER_REQ_ALREADY,		// 이미 신청중
	MSG_TEACH_ERR_STUDENT_LEVEL,			// 학생 가능레벨이 아님
	MSG_TEACH_ERR_HAVE_TEACHER,				// 이미 선생이 있음
	MSG_TEACH_ERR_TEACHER_NOT_CONNECT,		// 선생이 접속중이 아님
	MSG_TEACH_ERR_STUDENT_NOT_CONNECT,		// 학생이 접속중이 아님
	MSG_TEACH_ERR_TEACHER_FULL,				// 제자가 꽉 차 있음
	MSG_TEACH_ERR_NOE_REQ,					// 신청한 적 없음
	MSG_TEACH_ERR_NOMORE_STUDENT,			// 더이상 학생 할 수 없음

} MSG_TEACH_ERR_TYPE;

typedef enum _tagMsgChangeJobType
{
	MSG_CHANGEJOB_ERROR,					// 오류				: errcode(c)
	MSG_CHANGEJOB_REQ,						// 전직 요청		: job2(c)
	MSG_CHANGEJOB_REP,						// 전직 알림		: charindex(n) job(c) job2(c)
	MSG_CHANGEJOB_RESET_REQ,				// 직업 포기 요청	: 
	MSG_CHANGEJOB_RESET_REP,				// 직업 포기 알림	: charindex(n) job(c)
} MSG_CHANGEJOB_TYPE;

typedef enum _tagMsgChangeJobErrorType
{
	MSG_CHANGEJOB_ERROR_OK,							// RESERVED
	MSG_CHANGEJOB_ERROR_WRONGMSG,					// 클라이언트가 이상하다
	MSG_CHANGEJOB_ERROR_ALREADY,					// 이미 전직했음
	MSG_CHANGEJOB_ERROR_NOCONDITION_LEVEL,			// 전직 조건 불충분 : 레벨 부족
	MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL,			// 전직 조건 불충분 : 스킬 없음
	MSG_CHANGEJOB_ERROR_NOCONDITION_SKILLLEVEL,		// 전직 조건 불충분 : 스킬 레벨 부족
	MSG_CHANGEJOB_ERROR_NOMONEY,					// 돈이 부족함
} MSG_CHANGEJOB_ERROR_TYPE;

//0502 kwon
typedef enum _tagEventMoonStoneType
{
	MSG_EVENT_MOONSTONE_START_REQ,		// 문스톤 시작요청
	MSG_EVENT_MOONSTONE_START_REP,		// 문스톤 시작응답

	MSG_EVENT_MOONSTONE_TRY_REQ,		// 문스톤 복권 요청
	MSG_EVENT_MOONSTONE_TRY_REP,		// 문스톤 복권 응답

	MSG_EVENT_MOONSTONE_RESULT_REQ,		// 문스톤 복권 결과 요청
	MSG_EVENT_MOONSTONE_RESULT_REP,		// 문스톤 복권 결과 응답
	
} MSG_EVENT_MOONSTONE_TYPE;


typedef enum _tagEventMoonStoneChangeType
{
//5개 일치.
	MSG_CHANGE_JACK_POT,						// 잭팟.
	MSG_CHANGE_5PLUS_41LEVEL_WEAPON,			// 41레벨 +5 무기(종족별)	
	MSG_CHANGE_3PLUS_41LEVEL_WEAPON,			// 41레벨 +3 무기(종족별)	
	MSG_CHANGE_41LEVEL_WEAPON,					// 41레벨 무기 (종족별)
	MSG_CHANGE_3_SPECIAL_STONE,					// 고급제련석 3개
	MSG_CHANGE_2_SPECIAL_STONE,					// 고급제련석 2개
	MSG_CHANGE_1_SPECIAL_STONE,					// 고급제련석 1개
//4개일치
	MSG_CHANGE_CRYSTAL_OF_EFFORT,				// 노력의 결정.
	MSG_CHANGE_HIGH_CRYSTAL_OF_EXPERIMENCE,		// 상급 경험의 결정.
	MSG_CHANGE_150000_EXP,					    // 경험치 150,000
	MSG_CHANGE_MIDDLE_CRYSTAL_OF_EXPERIMENCE,	// 중급 경험의 결정
	MSG_CHANGE_10_PURE_STONE,					// 정화석 10개
	MSG_CHANGE_70000_EXP,						// 경험치 70,000
	MSG_CHANGE_3_MIDDLE_CRYSTAL_OF_EXPERIMENCE,	// 하급 경험의 결정 3개
//3개일치
	MSG_CHANGE_5_12GRADE_RED_KER,				//12레벨 붉은 케르 5개
	MSG_CHANGE_5_12GRADE_RED_NEN,				//12레벨 붉은 넨 5개
	MSG_CHANGE_5_12GRADE_BLUE_KER,				//12레벨 푸른 케르 5개
	MSG_CHANGE_5_BIG_MP_POTION,					//대형 MP물약 5개
	MSG_CHANGE_5_12GRADE_BLUE_NEN,				//12레벨 푸른 넨 5개
	MSG_CHANGE_5_MOONSTONE,						//문스톤 5개
	MSG_CHANGE_25000_EXP,						// 경험치 25,000

//2개일치
	MSG_CHANGE_3_6GRADE_RED_KER,				//6레벨 붉은 케르 3개 
	MSG_CHANGE_3_6GRADE_RED_NEN,				//6레벨 붉은 넨 3개
	MSG_CHANGE_3_6GRADE_BLUE_KER,				//6레벨 푸른 케르 3개
	MSG_CHANGE_3_BIG_HEAL_POTION,				//대형 HP물약 3개
	MSG_CHANGE_3_6GRADE_BLUE_NEN,				//6레벨 푸른넨 3개
	MSG_CHANGE_3_MIDDLE_MP_POTION,				//중형 MP물약 3개
	MSG_CHANGE_3_MEMORY_SCROLL,					//메모리 스크롤 3개

	MSG_CHANGE_3_C_GRADE_ELEMENTAL,				//c등급 원소 3개
	MSG_CHANGE_3_CRAK_BLUE_LEAF,			 	//크락 파란잎 3개
	MSG_CHANGE_3_STONE_WEAVER,					//스톤 웨버 3개
	MSG_CHANGE_3_D_GRADE_ELEMENTAL,				//D등급 원소 3개
	MSG_CHANGE_3_MIDIUM_HEAL_POTION,			//중형회복물약 5개
	MSG_CHANGE_3_CRAK_BRANCH,					//크락 줄기 3개
	MSG_CHANGE_3_STONELISE,						//스톤 리스

	MSG_CHANGE_NO_MOONSTONE,		// 문스톤이 없다.	
	MSG_CHANGE_MOONSTONE_RULLET,				// 5개의 룰렛값을 보낸다.	
	MSG_CHANGE_MOONSTONE_ACC_NAS,   //누적 나스.

} MSG_EVENT_MOONSTONE_CHANGE_TYPE;


typedef enum _tagFriendType
{
	MSG_FRIEND_ERROR,
	MSG_FRIEND_REGIST_REQUEST,
	MSG_FRIEND_REGIST_ALLOW,
	MSG_FRIEND_SET_CONDITION, 
	MSG_FRIEND_CHATTING,
	MSG_FRIEND_REGIST_CANCEL,
	MSG_FRIEND_DELETE_MEMBER,
	MSG_FRIEND_REGIST_MEMBER_NOTIFY,
	MSG_FRIEND_DELETE_MEMBER_NOTIFY,
	MSG_FRIEND_MEMBERLIST
} MSG_FRIEND_TYPE;

typedef enum _tagFriendErrorType
{
	MSG_FRIEND_ERROR_OK,		//정상.(친구등록OK, 삭제 OK)
	MSG_FRIEND_ERROR_PACKET,    //패킷오류
	MSG_FRIEND_ERROR_NOT_EXIST, //존재하지 않는다.
	MSG_FRIEND_ERROR_FULLMEMBER, //친구 정원 초과.
	MSG_FRIEND_ERROR_ALREADY_EXIST, //이미 친구리스트에 존재.
	MSG_FRIEND_ERROR_WAIT_OTHER, //다른 사람으로부터 친구등록을 요청받은 상태..
	MSG_FRIEND_ERROR_NOT_MATCH_INDEX, //서로간 친구 요청,승인 아이디가 일치하지 않는다.
	MSG_FRIEND_ERROR_HELPER_SERVER, //헬퍼서버와의 접속오류.
	MSG_FRIEND_ERROR_GAMESERVER,
	MSG_FRIEND_ERROR_FRIENDNAME, //이름이 잘못되었다..
	MSG_FRIEND_ERROR_ALREADY_BLOCK, //이미 블럭 되었다.
	MSG_FRIEND_ERROR_NOT_BLOCK, //이미 블럭 되지 않은 케릭터 
	MSG_FRIEND_ERROR_REGIST_REFUSE_PVP		// pvp 에서는 친구등록 거부
} MSG_FRIEND_ERROR_TYPE;

typedef enum _tagTreasureBoxType
{
	MSG_EVENT_TREASUREBOX_TRY_REQ,		
	MSG_EVENT_TREASUREBOX_TRY_REP,		

	MSG_EVENT_TREASUREBOX_OPEN_REQ,
	MSG_EVENT_TREASUREBOX_OPEN_REP,

	MSG_EVENT_TREASUREBOX_RED_TRY_REQ,		
	MSG_EVENT_TREASUREBOX_RED_TRY_REP,		

	MSG_EVENT_TREASUREBOX_RED_OPEN_REQ,
	MSG_EVENT_TREASUREBOX_RED_OPEN_REP,
} MSG_EVENT_TREASUREBOX_TYPE;

typedef enum _tagEventTreasureBoxLevelType
{
	MSG_EVENT_TREASUREBOX_10LEVEL,
	MSG_EVENT_TREASUREBOX_14LEVEL,
	MSG_EVENT_TREASUREBOX_18LEVEL,
	MSG_EVENT_TREASUREBOX_22LEVEL,
	MSG_EVENT_TREASUREBOX_26LEVEL,
	MSG_EVENT_TREASUREBOX_30LEVEL,

	MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL,  
	MSG_EVENT_TREASUREBOX_ERROR_NO_BOX,  

	MSG_EVENT_TREASUREBOX_RED_5LEVEL,
	MSG_EVENT_TREASUREBOX_RED_12LEVEL,
	MSG_EVENT_TREASUREBOX_RED_16LEVEL,

	MSG_EVENT_TREASUREBOX_RED_ERROR_NOT_LEVEL,  
	MSG_EVENT_TREASUREBOX_RED_ERROR_NO_BOX, 
	
} MSG_EVENT_TREASUREBOX_LEVEL_TYPE;

typedef enum _tagEventTreasureBoxChangeType
{
	MSG_EVENT_CHANGE_13LEVEL_WEAPON,
	MSG_EVENT_CHANGE_21LEVEL_WEAPON,
	MSG_EVENT_CHANGE_29LEVEL_WEAPON,
	MSG_EVENT_CHANGE_NAS,
	MSG_EVENT_CHANGE_CANDY,
	MSG_EVENT_CHANGE_ATTACK_POTION,
	MSG_EVENT_CHANGE_DEFENSE_POTION,

	MSG_EVENT_CHANGE_NO_TREASUREBOX,


	MSG_EVENT_CHANGE_HP_POTION,			// 소형  HP 확장 포션
	MSG_EVENT_CHANGE_MP_POTION,			// 소형  MP 확장 포션
	MSG_EVENT_CHANGE_EX_MEMORY,			// 메모리 스크롤 확장 카드
	MSG_EVENT_CHANGE_HPMP_POTION,		// 소형  HP/MP 확장 포션
	MSG_EVEVNT_CHAGE_EX_STAT,			// 개인 창고 확장 카드

	MSG_EVENT_CHANGE_PINK_DRAGON,		// 핑크 드레이크 알 교환 - 태국 30레벨 보물 상자
	MSG_EVENT_CHANGE_BLUE_HORSE,		// 파란 판의 피리 교환 - 태국 30레벨 보물 상자
} MSG_EVENT_TREASUREBOX_CHANGE_TYPE;

//0627
typedef enum _tagEventSuperStoneType
{
	MSG_EVENT_SUPERSTONE_REQ,		
	MSG_EVENT_SUPERSTONE_REP,		
			
} MSG_EVENT_SUPERSTONE_TYPE;

typedef enum __tagMsgExGoZoneType
{
	MSG_EX_GO_ZONE_FIND_REQ,		// 순간이동경로 발견 요청	: zone(n) extra(n) npcIdx(n)
	MSG_EX_GO_ZONE_SAVE_REQ,		// 마지막 위치 저장		: zone(n) extra(n) npcIdx(n)
	MSG_EX_GO_ZONE_MAP_REQ,			// 월드맵 경로 요청		: npcIdx(n)
	MSG_EX_GO_ZONE_MAP_REP,			// 월드맵 경로 응답 	: count(n) zone(n) extra(n)
	MSG_EX_GO_ZONE_MOVE_REQ,		// 공간 이동 요청		: zone(n) extra(n) npcIdx(n)
	MSG_EX_GO_ZONE_FIELD_DOUNGEON_TAX_REQ,	// 필드던전 이동시 요금요청	: npcIdx(n)
	MSG_EX_GO_ZONE_FIELD_DOUNGEON_TAX_REP,	// 필드던전 이동시 요금응답	: tax(n)
	MSG_EX_GO_ZONE_FIELD_DOUNGEON_MOVE_REQ, // 필드던전 이동 요청		: npcIdx(n), tax(n)
	MSG_EX_GO_ZONE_ERROR,			// 에러메시지			: error(n)
}MSG_EX_GO_ZONE_TYPE;

typedef enum __tagMsgExGoZoneErrorType
{
	MSG_EX_GO_ZONE_FIND_ERROR_OK,				// 새로운 경로 발견 성공	npcidx(n)
	MSG_EX_GO_ZONE_FIND_ERROR_ALREADY_EXIST,	// 이미 발견된 경로			npcidx(n)
	MSG_EX_GO_ZONE_SAVE_ERROR_OK,				// 위치저장 성공			npcidx(n)
	MSG_EX_GO_ZONE_SAVE_ERROR_FAILED,			// 위치저장 실패			npcidx(n)
	MSG_EX_GO_ZONE_MOVE_ERROR_NOMONEY,			// 돈부족					npcidx(n)
	MSG_EX_GO_ZONE_MOVE_ERROR_CANNOT_MOVE,		// 이동불가능 지역			npcidx(n)
	MSG_EX_GO_ZONE_MOVE_ERROR_OK,				// 이동성공					zone(n)	extra(n)
}MSG_EX_GO_ZONE_ERROR_TYPE;

typedef enum _tagHelperEventPhoenixErrorType			// yhj   081105  피닉스
{
	MSG_HELPER_EVENT_PHOENIX_OK,						// HS -> GS  피닉스 성공
	MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT,			// HS -> GS  피닉스 자격 미달임
} MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE;

typedef enum _tagEventPhoenixErrorType			// yhj   081105  피닉스
{
	MSG_EVENT_PHOENIX_OK,						// GS -> Client  피닉스 성공
	MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY,		// GS -> Client  이미 피닉스 등록함
	MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY,		// GS -> Client  인벤토리가 가득차서 이벤트 불가
	MSG_EVENT_PHOENIX_ERROR_REQUIREMENT,		// GS -> Client  피닉스 자격 미달임
} MSG_EVENT_PHOENIX_ERROR_TYPE;

typedef enum __tagMsgTradeAgentType
{
	MSG_TRADEAGENT_ERROR,						// 에러

	MSG_TRADEAGENT_REG_LIST_REQ,				// 상품 등록 리스트 요청 
	MSG_TRADEAGENT_REG_LIST_REP,				// 상품 등록 리스트 응답	
	MSG_TRADEAGENT_REG_REQ,						// 상품 등록 요청: tab_idx(c) row_idx(c) col_idx(c) item_idx(n) extra1(n)
	MSG_TRADEAGENT_REG_REP,						// 상품 등록 응답	
	MSG_TRADEAGENT_REG_CANCEL_REQ,				// 상품 등록 취소 요청
	MSG_TRADEAGENT_REG_CANCEL_REP,				// 상품 등록 응답	
	MSG_TRADEAGENT_SEARCH_REQ,					// 상품 조회 요청 
	MSG_TRADEAGENT_SEARCH_REP,					// 상품 조회 응답 
	MSG_TRADEAGENT_BUY_REQ,						// 상품 구매 요청 
	MSG_TRADEAGENT_BUY_REP,						// 상품 구매 응답 
	MSG_TRADEAGENT_CALCLIST_REQ,				// 정산 리스트 요청 
	MSG_TRADEAGENT_CALCLIST_REP,				// 정산 리스트 응답
	MSG_TRADEAGENT_CALCULATE_REQ,				// 정산 요청 
	MSG_TRADEAGENT_CALCULATE_REP,				// 정산 응답
	MSG_TRADEAGENT_CHECKCALC_REQ,				// 정산 필요 체크(로그인)
	MSG_TRADEAGENT_CHECKCALC_REP,				// 정산 필요 체크 응답	
	MSG_TRADEAGENT_NEEDCALC_NOTIFY,				// 정산 필요 알림(게임중)	

}MSG_TRADEAGENT_TYPE;

typedef enum __tagMsgTradeAgentErrorType
{
	MSG_TRADEAGENT_ERROR_NORMAL,				// 일반적 에러
	MSG_TRADEAGENT_ERROR_NOT_TRADEITEM,			// 거래 불가 아이템
	MSG_TRADEAGENT_ERROR_NOT_ITEMCOUNT,			// 아이템 개수 부정확 
	MSG_TRADEAGENT_ERROR_NOT_SELL_TOTALMONEY,	// 판매 총 금액 부정확
	MSG_TRADEAGENT_ERROR_NOT_ENUGH_GUARANTY,	// 보증금 부족

	MSG_TRADEAGENT_ERROR_NOT_ENUGH_MONEY,		// 소지금(나스) 부족
	MSG_TRADEAGENT_ERROR_ALREADY_SELLITEM,		// 이미 파매된 아이템
	MSG_TRADEAGENT_ERROR_FULL_INVENTORY,		// 인벤토리가 가득 참(정산 불가)
	MSG_TRADEAGENT_ERROR_NOT_BE_ITEM,			// 아이템이 없음
	MSG_TRADEAGENT_ERROR_MAXREGCOUNT_FULL,		// 서버 최대 등록 수 초과
	MSG_TRADEAGENT_ERROR_CHAR_MAXREGCOUNT_FULL,	// 캐릭터 최대 등록 수 초과

	MSG_TRADEAGENT_ERROR_REGIST_ING_NOREG,		// 현재 등록 처리중이라 등록 할수 없음(현재 등록 처리중)
	MSG_TRADEAGENT_ERROR_CALCULATE_ING_NOCALC	// 현재 정산 처리중이라 청산 할수 없음(현재 정산 처리중)
}MSG_TRADEAGENT_ERROR_TYPE;

typedef enum __tagMsgTradeAgentErrorPart
{
	MSG_TRADEAGENT_ERROR_PART_SEARCH,			// 에러 파트: 조회(구매)
	MSG_TRADEAGENT_ERROR_PART_REGIST,			// 에러 파트: 등록(등록 취소)
	MSG_TRADEAGENT_ERROR_PART_CALCULATE,		// 에러 파트: 정산
}MSG_TRADEAGENT_ERROR_PART;

typedef enum __tagMsgTradeAgentStateType		// 상품 상태
{	
	MSG_TRADEAGENT_STATE_SELL_BEFORE,			// 판매 전 
	MSG_TRADEAGENT_STATE_SELL_COMPLETE,			// 판매 완료
	MSG_TRADEAGENT_STATE_SELL_RETURNED,			// 판매 되지 않음(반송)
	MSG_TRADEAGENT_STATE_CALCULATE_COMPLETE,	// 정산 하였음
	MSG_TRADEAGENT_STATE_CALCULATE_OVERTIME,	// 정산 기한 초과
	MSG_TRADEAGENT_STATE_BUY,			        // 구입
}MSG_TRADEAGENT_STATE_TYPE;

typedef enum __tagMsgTradeAgentFinishDayType	// 상품 마감일 단위 
{	
	MSG_TRADEAGENT_FINISH_DAY,					// 일
	MSG_TRADEAGENT_FINISH_HOUR,					// 시간
	MSG_TRADEAGENT_FINISH_MIN,					// 분
}MSG_TRADEAGENT_FINISHDAY_TYPE;

typedef enum __tagMsgTradeAgentAlignType	// 리스트 정렬 타입
{	
	MSG_TRADEAGENT_ALIGN_NAME_ASC,			// 아이템 이름(오름 차순)
	MSG_TRADEAGENT_ALIGN_NAME_DESC,			// 아이템 이름(내림 차순)

	MSG_TRADEAGENT_ALIGN_QUANTITY_ASC,		// 수량 (오름 차순)
	MSG_TRADEAGENT_ALIGN_QUANTITY_DESC,		// 수량 (내림 차순)

	MSG_TRADEAGENT_ALIGN_TOTALMONEY_ASC,	// 총금액 (오름 차순)
	MSG_TRADEAGENT_ALIGN_TOTALMONEY_DESC,	// 총금액 (내림 차순)

	MSG_TRADEAGENT_ALIGN_LEVEL_ASC,			// 레벨 (오름 차순)
	MSG_TRADEAGENT_ALIGN_LEVEL_DESC,		// 레벨 (내림 차순)
}MSG_TRADEAGENT_ALIGN_TYPE;


typedef enum __tagGMGoZoneType		// 운영자 던전 이동 종류
{	
	GM_GOZONE_NONE = 0,
	GM_GOZONE_PERSONAL,
	GM_GOZONE_COMBO,
}GM_GOZONE_TYPE;


typedef enum _tagMsgExpedType			//원정대 관련 메세지 
{
	MSG_EXPED_ERROR,						// 에러
	
	MSG_CREATE_REQ,							// 원정대 생성(파티전환) 요청			
	MSG_CREATE_REP,							// 원정대 생성(파티전환) 응답	

	MSG_INVITE_REQ,							// 초대 요청
	MSG_INVITE_REP,							// 초대 응답

	MSG_EXPED_ADD,							// 캐릭 추가 
	MSG_EXPED_MEMBER_INFO,					// 대원 정보

	MSG_ALLOW_REQ,							// 초대 수락
	MSG_ALLOW_REP,							// 초대 응답

	MSG_REJECT_REQ,							// 초대 거부 요청
	MSG_REJECT_DEST,						// 거무	
	MSG_REJECT_SRC,

	MSG_QUIT_REQ,							// 탈퇴 요청
	MSG_QUIT_REP,							// 탈퇴 응답

	MSG_ENDEXPED_REQ,						// 원정대 해체
	MSG_ENDEXPED_REP,						// 원정대 해체	

	MSG_KICK_REQ,							// 추방 요청
	MSG_KICK_REP,							// 추방 응답

	MSG_CHANGETYPE_REQ,						// 원정대 타입 변경(경험치, 나스, 일반 아이템, 스페셜 아이템 분배 방법)
	MSG_CHANGETYPE_REP,		

	MSG_MEMBERINFO_REQ,						// 원정대 멤버(U.I)
	MSG_MEMBERINFO_REP,		

	MSG_ADMIN_REQ,							// 원정대 관리(U.I)
	MSG_ADMIN_REP,

	MSG_CHANGEBOSS_REQ,						// 원정 대장 위임(변경)
	MSG_CHANGEBOSS_REP,						// 원정 대장 위임

	MSG_SETMBOSS_REQ,						// 부대장 임명
	MSG_SETMBOSS_REP,

	MSG_RESETMBOSS_REQ,						// 부대장 임명 해제
	MSG_RESETMBOSS_REP,	

	MSG_MOVEGROUP_REQ,						// 그룹 이동 
	MSG_MOVEGROUP_REP,						

	MSG_ADDMEMBER_REQ,						// 대원 추가
	MSG_ADDMEMBER_REP,

	MSG_VIEWDETAIL_REQ,						// 살펴보기
	MSG_VIEWDETAIL_INVEN,					// 인벤정보
	MSG_VIEWDETAIL_PET,						// 펫정보
	MSG_VIEWDETAIL_APET,					// 공격펫

	MSG_SET_LABEL_REQ,						// 표식 지정
	MSG_SET_LABEL_REP,

} MSG_EXPEDITION_TYPE;

typedef enum __tagMsgExpedSetLabelIndex	// 표식 
{
	MSG_EXPED_SETLABEL_INDEX_1,			// 도끼
	MSG_EXPED_SETLABEL_INDEX_2,			// 유령 
	MSG_EXPED_SETLABEL_INDEX_3,			// 발바닥 
	MSG_EXPED_SETLABEL_INDEX_4,			// 조각칼
	MSG_EXPED_SETLABEL_INDEX_5,			// 똥 
	MSG_EXPED_SETLABEL_INDEX_6,			// 악마 
	MSG_EXPED_SETLABEL_INDEX_7,			// 해골	
}MSG_EXPEDITION_SETLABEL_INDEX;

typedef enum __tagMsgExpedSetLabelType	
{
	MSG_EXPED_SETLABEL_TYPE_PC,			
	MSG_EXPED_SETLABEL_TYPE_NPC,		
}MSG_EXPEDITION_SETLABEL_TYPE;

typedef enum __tagMsgExpedSetLabelMode	
{
	MSG_EXPED_SETLABEL_MODE_SET,			
	MSG_EXPED_SETLABEL_MODE_RESET,		
}MSG_EXPEDITION_SETLABEL_MODE;


typedef enum __tagMsgExpedErrorType
{
	MSG_EXPED_ERROR_NORMAL,					// 일반

	MSG_EXPED_ERROR_ALREADY_REQUEST,		// 다른 사람에게 이미 요청 중

	MSG_EXPED_ERROR_ALLOW_OK,				// 초대 수락 성공
	MSG_EXPED_ERROR_ALLOW_NOT_EXPED,		// 원정대 중이 아닐때

	MSG_EXPED_ERROR_REJECT_OK,				// 거절 성공
	MSG_EXPED_ERROR_REJECT_FAIL,			// 실패

	MSG_EXPED_ERROR_QUIT_OK,				// 탈퇴 성공
	MSG_EXPED_ERROR_QUIT_END,				// 탈퇴 성공, 원정대 해체

	MSG_EXPED_ERROR_KICK_OK,				// 강퇴 성공
	MSG_EXPED_ERROR_KICK_END,				// 강퇴 성공, 원정대 해체

	MSG_EXPED_ERROR_INVITE_PVP,				// 요청자나 대상자가 PVP 중
	MSG_EXPED_ERROR_NOT_PARTY,				// 파티에 소속되지 않음(파티 전환 불가)
	MSG_EXPED_ERROR_NOT_EXPED,				// 원정대 소속되지 않음
	MSG_EXPED_ERROR_NOT_PARTYBOSS,			// 파티장이 아니다.	(파티전환 불가) 
	MSG_EXPED_ERROR_NOT_EXPEDBOSS,			// 원정대장이 아니다.
	MSG_EXPED_ERROR_ALREADY_JOIN_ME,		// 이미 원정대에 소속되어 있음
	MSG_EXPED_ERROR_ALREADY_JOIN_OTHER,		// 다른 원정대에 소속되어 있음
	MSG_EXPED_ERROR_NOT_BE_CHAR,			// 없는 캐릭터 
	MSG_EXPED_ERROR_FULL_EXPED,				// 더 이상 원정대원 추가할 수 없음 
	MSG_EXPED_ERROR_NOT_5MIN_GETBOX,		// 더 이상 상자 주울 수 없음 
	MSG_EXPED_ERROR_ALREADY_GETBOX,         // 이전에 받은 상자 오픈 하고 다시 시도
	MSG_EXPED_ERROR_INVALID_LEVEL,			// 레벨이 맞지 않음
}MSG_EXPEDITION_ERROR_TYPE;

//원정대 그룹 타입 
typedef enum _tagMsgExpedGroupType
{
	MSG_EXPED_GROUP_1,						// 그룹 1
	MSG_EXPED_GROUP_2,						// 그룹 2	
	MSG_EXPED_GROUP_3,						// 그룹 3
	MSG_EXPED_GROUP_4,						// 그룹 4

	MSG_EXPED_GROUP_COUNT,

} MSG_EXPED_GROUP_TYPE;

//원정대 구성원 타입
typedef enum _tagMsgExpedMemberType
{
	MSG_EXPED_MEMBERTYPE_BOSS,				// 원정 대장 
	MSG_EXPED_MEMBERTYPE_MBOSS,				// 원정 부대장
	MSG_EXPED_MEMBERTYPE_NORMAL,			// 원정 대원
	
	MSG_EXPED_MEMBERTYPE_COUNT,
}MSG_EXPED_MEMBER_TYPE;

//원정대 정상,비정상 탈퇴 타입
typedef enum _tagMsgExpedQuitMode
{
	MSG_EXPED_QUITMODE_NORMAL,				// 정상 나감 
	MSG_EXPED_QUITMODE_UNUSUAL,				// 비정상 나감
}MSG_EXPED_QUIT_MODE;

//보스 위임(수동,자동) 모드
typedef enum _tagMsgExpedChangeBossMode
{
	MSG_EXPED_CHANGEBOSS_MANUAL,			// 수동 위임
	MSG_EXPED_CHANGEBOSS_AUTO,				// 자동 위임
}MSG_EXPED_CHANGEBOSS_MODE;

typedef enum _tagMsgPartyType2
{
	MSG_PARTY_TYPE_RANDOM=0,			// 균등 파티
	MSG_PARTY_TYPE_FIRSTGET,			// 입수 우선
	MSG_PARTY_TYPE_BATTLE,				// 전투형
	MSG_PARTY_TYPE_OPENBOX,				// 상자열기
}MSG_PARTY_TYPE2;

typedef enum _tagMsgExpedType2
{
	MSG_EXPED_TYPE_RANDOM=0,			// 균등 파티
	MSG_EXPED_TYPE_FIRSTGET,			// 입수 우선
	MSG_EXPED_TYPE_BATTLE,				// 전투형
	MSG_EXPED_TYPE_OPENBOX,				// 상자열기
}MSG_EXPED_TYPE2;

typedef enum _tagMsgDiviType
{
	MSG_DIVITYPE_EXP=0,						// 분배 방법(경험치)
	MSG_DIVITYPE_ITEM,		  				// 분배 방법(아이템)
	MSG_DIVITYPE_SPECIAL,				    // 분배 방법(스페셜 아이템)
}MSG_DIVI_TYPE;

//레이드 에러
typedef enum __tagMsgRaidErrorType
{
	MSG_RAID_ERROR_SUCESS=0,       // 성공
	MSG_RAID_ERROR_INZONE_JOIN_CLEAR_COMPLETE,   // 인존 입장 불가(인존 클리어 완료) - %일 %시 뒤에 입장가능
	MSG_RAID_ERROR_INZONE_JOIN_ROOMCOUNT_FULL,   // 현재 인스턴스 너무 많아 더이상 생성 불가
	MSG_RAID_ERROR_INZONE_JOIN_ALREADY,     // 인존 이미 로그인 
	MSG_RAID_ERROR_INZONE_JOIN_MEMBER_FULL,    // 인존 입장 가능한 최대 인원수 초과
	MSG_RAID_ERROR_INZONE_JOIN_ALREADY_OTHERTEAM,  // 해당 존에 다른 파티,원정대가 있습니다.
	MSG_RAID_ERROR_INZONE_JOIN_THISZONE_PARTYZONE,  // 파티 상태에서만 입장 가능 합니다
	MSG_RAID_ERROR_INZONE_JOIN_THISZONE_EXPEDZONE,  // 원정대 상태에서만 입장 가능 합니다  
	MSG_RAID_ERROR_INZONE_JOIN_NOTJOIN_PARTYOREXPED, // 파티나 원정대에 가입 되어  있지 않음
	MSG_RAID_ERROR_INZONE_QUIT_NOTINZONEJOINED,   // 해당 인존에 입장 하지 않았음 
	MSG_RAID_ERROR_INZONE_NOT_5MIN_GETBOX,    // 더 이상 상자 주울 수 없음(획득 시간 5분) 
	MSG_RAID_ERROR_INZONE_ALREADY_GETBOX,    // 이전에 받은 상자 오픈 하고 다시 시도
	MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL,			// 레벨 제한으로 입장할수 없음
	MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO,		// 귀속된 인존 정보가 원정대장 달라 접속 불가 (추가 에러 메세지)
	MSG_RAID_ERROR_INZONE_NOT_EXPED_RAID_SUBNUMBER,		// 지정된 원정대 레이드 채널이 아니라서 입장 불가
	MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS,				// 원정대장이 아직 레이드에 입장하지 않았다.
}MSG_RAID_ERROR_TYPE;


#endif

