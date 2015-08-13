#ifndef __SKILL_H__
#define __SKILL_H__

#include "../ShareLib/MemoryPoolBase.h"

#define SKILL_RATE_UNIT		100		// 스킬 rate 단위 10000

////////////////////////
// 스킬 관련
#define MAX_SKILL_LEARN_SKILL			3			// 스킬 배우는데 필요한 스킬 최대 종류
#define MAX_SKILL_LEARN_ITEM			3			// 스킬 배우는데 필요한 아이템 최대 종류
#define MAX_SKILL_CHANGE				4			// 하나의 스킬로 인한 상태 변화 최대 종류
#define MAX_SKILL_NEED_ITEM				2			// 스킬 사용시 소모되는 아이템 종류
#define MAX_SKILL_APP_MAGIC				3			// 스킬당 적용조건 효과 수
#define MAX_SKILL_USE_MAGIC				3			// 스킬당 사용조건 효과 수
#define MAX_SKILL_MAGIC					3			// 스킬당 효과 수
#define MAX_SKILL_LEVEL					10			// 스킬 최대 레벨
#define UNIT_SKILLPOINT					10000		// 스킬 포인트 사용 단위
#define MAX_SOUL_COUNT					10			// 나이트 쉐도우 영혼 최대 소유 개수

#define FACTORY_UNION_REG_SKILL			655			// 제작 연합 등록 스킬 번호

////////////
// 스킬 상태
#define SKILL_CON_NORMAL				0			// 일반 상태
#define SKILL_CON_READY					1			// 시전 상태
#define SKILL_CON_FIRE					2			// 발동 상태

// OLD ATTRIBUTE VARIABLE REUSE, Elenoa 2013.01.09
// 속성 상수
#define	AT_NONE					0			// 무
#define	AT_FIRE					1			// 화
#define	AT_WATER				2			// 수
#define	AT_EARTH				3			// 지
#define	AT_WIND					4			// 풍
#define	AT_DARK					5			// 암
#define	AT_LIGHT				6			// 광
#define AT_RANDOM				7			// random 서브타입
#define AT_LEVELMAX				5
#define AT_MASK			0xFL
#define AT_LVVEC		4
#define AT_MIX(a, l)	(((l) & AT_MASK) << AT_LVVEC) | ((a) & AT_MASK)
#define GET_AT_VAR(m)	((m) & AT_MASK)
#define GET_AT_LV(m)	(((m) >> AT_LVVEC) & AT_MASK)
#define AT_AD_MASK		0xFFL
#define AT_ADVEC		8
#define AT_ADMIX(a, d)	((((a) & AT_AD_MASK) << AT_ADVEC) | ((d) & AT_AD_MASK))
#define GET_AT_DEF(m)	((m) & AT_AD_MASK)
#define GET_AT_ATT(m)	(((m) >> AT_ADVEC) & AT_AD_MASK)

///////////
// 효과상수

// 효과 타입
#define MT_STAT					0			// 스탯
// OLD ATTRIBUTE VARIABLE REUSE, Elenoa 2013.01.09
#define MT_ATTRIBUTE			1			// 속성지속
#define MT_ASSIST				2			// 상태변화
#define MT_ATTACK				3			// 공격
#define MT_RECOVER				4			// 회복
#define MT_CURE					5			// 치유
#define MT_OTHER				6			// 그외
#define MT_REDUCE				7			// 대미지 흡수
#define MT_IMMUNE				8			// 면역
#define MT_CASTLE_WAR			9			// 공성 전용
#define MT_MONEY				10			// 돈

// 효과 서브타입 - 스탯
#define MST_STAT_ATTACK			0			// 공격
#define MST_STAT_DEFENSE		1			// 방어
#define MST_STAT_MAGIC			2			// 마법
#define MST_STAT_RESIST			3			// 저항
#define MST_STAT_HITRATE		4			// 명중
#define MST_STAT_AVOID			5			// 회피
#define MST_STAT_CRITICAL		6			// 크리
#define MST_STAT_ATTACKSPD		7			// 공속
#define MST_STAT_MAGICSPD		8			// 마속
#define MST_STAT_MOVESPD		9			// 이속
#define MST_STAT_RECOVERHP		10			// HP회복
#define MST_STAT_RECOVERMP		11			// MP회복
#define MST_STAT_MAXHP			12			// 최대HP 증가
#define MST_STAT_MAXMP			13			// 최대MP 증가
#define MST_STAT_DEADLY			14			// 데들리 확률 증가
#define MST_STAT_MAGICHITRATE	15			// 마법 명중
#define MST_STAT_MAGICAVOID		16			// 마법 회피
#define MST_STAT_ATTACKDIST		17			// 공격거리 증가
#define MST_STAT_ATTACK_MELEE	18			// 근접공격 증가
#define MST_STAT_ATTACK_RANGE	19			// 원기리물리 공격 증가
#define MST_STAT_HITRATE_SKILL	20			// 스킬 명중률
#define MST_STAT_ATTACK_80		21			// 공격력 80
#define MST_STAT_MAXHP_450		22			// 최대HP 증가 450
#define MST_STAT_SKILLSPD		23			// 스킬 시전 속도증가
#define	MST_STAT_VALOR			24			// 파티원수에 비례하여 공격력 향상
#define MST_STAT_STATPALL		25			// 스탯 증감
#define MST_STAT_ATTACK_PER		26			// 공격 %
#define MST_STAT_DEFENSE_PER	27			// 방어 %
#define MST_STAT_STATPALL_PER	28			// 스탯 증감 %
#define MST_STAT_STR			29			// 힘
#define MST_STAT_DEX			30			// 민첩
#define MST_STAT_INT			31			// 지혜
#define MST_STAT_CON			32			// 체질
//  [5/21/2009 KwonYongDae]
#define MST_STAT_HARD			33			// 강한공격
#define MST_STAT_STRONG			34			// 강인함
#define MST_STAT_NPCATTACK		35			// NPC 에게만 물리/원거리 공격력 증가
#define MST_STAT_NPCMAGIC		36			// NPC 에게만 마법 공격력 증가
#define MST_STAT_SKILLCOOLTIME	37			// 스킬 쿨타임
#define MST_STAT_DECREASE_MANA_SPEND 38		// 마나소비 감소

// 효과 서브타입 - 상태이상
#define MST_ASSIST_POISON		0			// 독
#define MST_ASSIST_HOLD			1			// 홀드
#define MST_ASSIST_CONFUSION	2			// 혼란
#define MST_ASSIST_STONE		3			// 석화
#define MST_ASSIST_SILENT		4			// 봉인
#define MST_ASSIST_BLOOD		5			// 출혈
#define MST_ASSIST_BLIND		6			// 블라인드
#define MST_ASSIST_STURN		7			// 스턴
#define MST_ASSIST_SLEEP		8			// 슬립
#define MST_ASSIST_HP			9			// HP회복
#define MST_ASSIST_MP			10			// MP회복
#define MST_ASSIST_MOVESPD		11			// 이속 향상
#define MST_ASSIST_HP_CANCEL	12			// HP변화(0, MAX에서 취소)
#define MST_ASSIST_MP_CANCEL	13			// MP변화(0, MAX에서 취소)
#define MST_ASSIST_DIZZY		14			// 현기증
#define MST_ASSIST_INVISIBLE	15			// 인비저블
#define MST_ASSIST_SLOTH		16			// 슬로스 : 공속하락
#define MST_ASSIST_FEAR			17			// 공포 : 해당 NPC가 도망
#define MST_ASSIST_FAKEDEATH	18			// 죽은척
#define MST_ASSIST_PERFECTBODY	19			// 타이탄 퍼펙 바디
#define MST_ASSIST_FRENZY		20			// 타이탄 프렌지
#define MST_ASSIST_DAMAGELINK	21			// 대미지링크
#define MST_ASSIST_BERSERK		22			// 버서크
#define MST_ASSIST_DESPAIR		23			// 절망
#define MST_ASSIST_MANASCREEN	24			// 마나 스크린
#define MST_ASSIST_BLESS		25			// 블레싱
#define MST_ASSIST_SAFEGUARD	26			// 보호망(전투불가능)
#define MST_ASSIST_MANTLE		27			// 망토
#define MST_ASSIST_GUARD		28			// 경비병 소환

#define MST_ASSIST_CHARGEATC	29			// 장탄(attack)
#define MST_ASSIST_CHARGEMGC	30			// 장탄(magic)

#define MST_ASSIST_DISEASE		31			// 질병
#define MST_ASSIST_CURSE		32			// 저주
#define MST_ASSIST_CONFUSED			33			// 몬스터만 혼란에 걸리게 함
#define MST_ASSIST_TAMING			34			// 몬스터를 일정시간 아군으로 만듬
#define MST_ASSIST_FREEZE			35			// 결계 (서로 공격 못함, 걸린 것은 이동 못함)
#define MST_ASSIST_INVERSE_DAMAGE	36			// 피격당하면 데미지 만큼 HP 회복
#define MST_ASSIST_HP_DOT			37			// 지속데미지 입음
#define MST_ASSIST_REBIRTH			38			// 부활
#define MST_ASSIST_DARKNESS_MODE	39			// 사도 모드
#define MST_ASSIST_AURA_DARKNESS	40			// 오오라 - 다크니스
#define MST_ASSIST_AURA_WEAKNESS	41			// 오오라 - 위크니스
#define MST_ASSIST_AURA_ILLUSION	42			// 오오라 - 일루젼
#define MST_ASSIST_MERCENARY		43		// 몬스터를 용병으로 바꿈.
#define MST_ASSIST_SOUL_TOTEM_BUFF	44		// 고대 무기(토템))의 정령(버프형)
#define MST_ASSIST_SOUL_TOTEM_ATTK	45		// 고대 무기(토템)의 정령(공격형)
#define MST_ASSIST_TRAP				46		// 몬스터를 트랩으로
#define MST_ASSIST_PARASITE			47		// 패러사이트 감염 (감염된 상태로 죽으면 자폭몬스터를 소환하여 동족을 죽임)
#define MST_ASSIST_SUICIDE			48		// 몬스터를 자폭시킴
#define MST_ASSIST_INVINCIBILITY	49		// 무적 모드
#define MST_ASSIST_GPS				50		// GPS
#define MST_ASSIST_ATTACK_TOWER		51		// 공성 타워 데미지 추가
#define MST_ASSIST_ARTIFACT_GPS		52		// 유물 gps
#define MST_ASSIST_TOTEM_ITEM_BUFF	53		// 토템 아이템 버프
#define MST_ASSIST_TOTEM_ITEM_ATTK	54		// 토템 아이템 버프

// 효과 서브타입 - 공격
#define MST_ATTACK_NORMAL		0			// 일반 공격
#define MST_ATTACK_CRITICAL		1			// 크리티컬 공격
#define MST_ATTACK_DRAIN		2			// 드레인 : 적 hp를 내것으로 흡수
#define MST_ATTACK_ONESHOTKILL	3			// 즉사
#define MST_ATTACK_DEADLY		4			// 데들리 공격
#define MST_ATTACK_HARD			5			// 매우 강한 공격

// 효과 서브타입 - 회복
#define MST_RECOVER_HP			0			// HP회복
#define MST_RECOVER_MP			1			// MP회복
#define MST_RECOVER_STM			2			// 공격형 펫 스테미너 회복
#define MST_RECOVER_FAITH		3			// 공격형 펫 충성도 회복
#define MST_RECOVER_EXP			4			// 경험치 복구
#define MST_RECOVER_SP			5			// 숙련도 복구

// 효과 서브타입 - 치유
#define MST_CURE_POISON			0			// 독
#define MST_CURE_HOLD			1			// 홀드 : TODO : 사용하지 않음
#define MST_CURE_CONFUSION		2			// 혼란
#define MST_CURE_STONE			3			// 석화
#define MST_CURE_SILENT			4			// 봉인
#define MST_CURE_BLOOD			5			// 출혈
#define MST_CURE_REBIRTH		6			// 부활
#define MST_CURE_INVISIBLE		7			// 인비저블 상태를 해제
#define MST_CURE_STURN			8			// 스턴 해제
#define MST_CURE_SLOTH			9			// 슬로스 해제
#define MST_CURE_NOTHELP		10			// not help인 assist를 모두 해제
#define MST_CURE_BLIND			11			// 블라인드

#define MST_CURE_DISEASE		12			// 질병 해제
#define MST_CURE_CURSE			13			// 저주 해제
#define MSG_CURE_ALL			14			// 디버프 모두 삭제 <<-- MST_CURE_NOTHELP 와 중복확인 필요
#define MST_CURE_INSTANTDEATH	15			// 즉사(죽음의 선고 해제)

// 효과 서브타입 - 그외
#define MST_OTHER_INSTANTDEATH	0			// 즉사
#define MST_OTHER_SKILLCANCEL	1			// 스킬 시전 취소
#define MST_OTHER_TACKLE		2			// 태클
#define MST_OTHER_TACKLE2		3			// 태클2
#define MST_OTHER_REFLEX		4			// 반사 대미지 (일반 공격, 25m내)
#define MST_OTHER_DEATHEXPPLUS	5			// 죽을때 경험치 패널티 줄여
#define MST_OTHER_DEATHSPPLUS	6			// 죽을때 SP 패널티 줄여
#define MST_OTHER_TELEKINESIS	7			// 텔레키네시스
#define MST_OTHER_TOUNT			8			// 타운트 : 헤이트 변경
#define MST_OTHER_SUMMON		9			// 소환
#define MST_OTHER_EVOCATION		10			// 강신
#define MST_OTHER_TARGETFREE	11			// 타겟 프리
#define MST_OTHER_CURSE			12			// 타켓 스탯 하락
#define MST_OTHER_PEACE			13			// 평화의 징표
#define MST_OTHER_SOUL_DRAIN	14			// 영혼 흡수, 개수 채움
#define MST_OTHER_KNOCKBACK		15			// 넉백
#define MST_OTHER_WARP			16			// 워프
#define MST_OTHER_FLY			17			// 비행
#define MST_OTHER_EXP			18			// EXP
#define MST_OTHER_SP			19			// SP
#define MST_OTHER_ITEMDROP		20			// 아이템 바닥에 드랍
#define MST_OTHER_SKILL			21			// 스킬사용
#define MST_OTHER_PK_DISPOSITION 22			// PK 성향 포인트
#define MST_OTHER_AFFINITY		23			// 친화도 부스터
#define MST_OTHER_AFFINITY_QUEST	24		// 친화도 퀘스트
#define MST_OTHER_AFFINITY_MONSTER	25		// 친화도 몬스터
#define MST_OTHER_AFFINITY_ITEM		26		// 친화도 아이템
#define MST_OTHER_QUEST_EXP			27		// 퀘스트 경험치
#define MST_OTHER_GUILD_PARTY_EXP	28		// 길드원 파티 경험치 획득량
#define MST_OTHER_GUILD_PARTY_SP	29		// 길드원 파티 스킬포인트 획득량
#define MSG_OTHER_SUMMON_NPC		30		// NPC 소환

// 효과 서브타입 - 대미지 흡수
#define MST_REDUCE_MELEE		0			// 근접 공격 흡수
#define MST_REDUCE_RANGE		1			// 원거리 공격 흡수
#define MST_REDUCE_MAGIC		2			// 마법 공격 흡수
#define MST_REDUCE_SKILL		3			// 스킬 공격 흡수

// 효과 서브타입 - 면역
#define MST_IMMUNE_BLIND		0			// 블라인드

// 효과 서브타입 - 공성
#define MST_WAR_REDUCE_MELEE	0
#define MST_WAR_REDUCE_RANGE	1
#define MST_WAR_REDUCE_MAGIC	2
#define MST_WAR_MAX_HP			3
#define MST_WAR_DEFENCE			4
#define MST_WAR_RESIST			5
#define MST_WAR_TOWER_ATTACK	6

// 대미지 타입
#define MDT_ONLYPOWER			0			// 스킬 위력만
#define MDT_ADDITION			1			// 스탯에 덧셈
#define MDT_RATE				2			// 스탯에 비율로 곱셈

// 속성 데미지 타입
#define MDT_NONE				0
#define MDT_ATTACK				1
#define MDT_DEFENCE				2

// 명중 타입
#define MHT_CONSTANT			0			// 고정
#define MHT_VARIABLE			1			// 수식

// 효과 서브타입 - 돈(MONEY)
#define MST_MONEY_BUY		0			// 구매
#define MST_MONEY_SELL		1			// 판매
#define MST_MONEY_NAS		2			// 추가 나스

////////////
// 스킬 상수

// 스킬 종류
#define ST_MELEE				0			// 근접
#define ST_RANGE				1			// 원거리
#define ST_MAGIC				2			// 마법
#define ST_PASSIVE				3			// 패시브
#define ST_PET_COMMAND			4			// 펫 커맨드
#define ST_PET_SKILL_PASSIVE	5			// 펫 스킬(패시브)
#define ST_PET_SKILL_ACTIVE		6			// 펫 스킬(액티브)
#define ST_GUILD_SKILL_PASSIVE	7			// 길드 스킬(패시브)
#define ST_SEAL					8			// 제작 시스템 스킬(증표)
#define ST_SUMMON_SKILL			9			// 토템 소환 스킬

// 타겟팅 방식
#define STT_SELF_ONE			0			// 셀프
#define STT_SELF_RANGE			1			// 셀프 범위
#define STT_TARGET_ONE			2			// 타겟
#define STT_TARGET_RANGE		3			// 타겟 범위
#define STT_PARTY_ONE			4			// 파티 1인
#define STT_PARTY_ALL			5			// 파티 전체
#define STT_TARGET_D120			6			// 타겟 120도
#define STT_TARGET_RECT			7			// 타겟 직사각
#define STT_ELEMENTAL_ONE		8			// 엘리멘탈 대상 스킬
#define STT_GUILD_ALL			9			// 길드 전체에게 주는 HELP 스킬
#define STT_GUILD_ONE			10			// 길드 한명에게 주는 HELP 스킬
#define STT_GUILD_SELF_RANGE	11			// 시전자 주위 NPC에게 주는 ATTACK SKILL
#define STT_GUILD_MEMBER_SELF	12			// 길드멤버 셀프

// 플래그
#define	SF_SINGLEMODE			(1 << 0)	// 싱글모드
#define	SF_HELP					(1 << 1)	// 도움을 주는 스킬
#define	SF_NOTHELP				(1 << 2)	// 해꼬지 스킬
#define SF_ABSTIME				(1 << 3)	// 060227 : bs : 절대시간 사용
#define SF_NOTDUPLICATE			(1 << 4)	// 060227 : bs : 중복 사용 불가 : 중복되는 스킬이 있으면 레벨에 관계없이 사용이 안됨
#define SF_NOCANCEL				(1 << 5)	// 다른 스킬/사망에 의해 취소 안됨
#define	SF_COMBO				(1 << 6)	// 시간상관없이 적용됨(접속종료, 공간이동, 죽었을 때 사라짐)
#define	SF_LOGOUT				(1 << 7)	// 접속종료 시 사라짐
#define SF_INSTANT				(1 << 8)	// 100420 : swkwon : 아이템 스킬 로그아웃할때 스킬 리스트에서 삭제함.
#define SF_GUILD				(1 << 9)	// 100615 : swkwon : 길드스킬
#define SF_INFINITE				(1 << 10)	// 무한스킬 - 스킬 시간이 만료되면 다시 스킬이 적용됨
#define SF_ZONE					(1 << 11)	// 존이동시 버프 삭제
#define SF_SUMMON_NPC			(1 << 12)	// summon Npc가 사용하는 스킬인지 체크 // 체크하면 이펙트를 보여준다.
#define SF_NOTBLESS				(1 << 13)	// 플래그가 있으면 소서러 블레스의 영향을 받지 않는다.
#define SF_ITEMSKILL			(1 << 14)	// 아이템 스킬
#define SF_NOT_BOSS				(1 << 15)	// 플래그가 있으면 보스에게 사용할 수 없는 스킬이다.
#define SF_NOT_SINGLE			(1 << 16)	// 플래그가 있으면 개인던전에서는 스킬사용을 할 수 없다.
#define SF_TOGGLE				(1 << 17)	// 토글 스킬

// 사용/적용 조건
#define SCT_DEATH				(1 << 0)	// 사망
#define SCT_WEAPON				(1 << 1)	// 무기장비
#define SCT_SITDOWN				(1 << 2)	// 앉기
#define SCT_PEACEZONE			(1 << 3)	// 평화지역
#define SCT_SHIELD				(1 << 4)	// 방패착용
#define SCT_STAND				(1 << 5)	// 서기
#define SCT_DARKNESS			(1 << 6)    // 나이트 쉐도우 사도 모드에서만 사용하는 스킬인지 확인
#define SCT_NO_COOL_TIME		(1 << 7)	// 쿨타임감소 적용 불가

// 상태이상 비트 필드 값
#define AST_POISON				((LONGLONG)1 << MST_ASSIST_POISON)		// 독
#define AST_HOLD				((LONGLONG)1 << MST_ASSIST_HOLD)			// 홀드
#define AST_CONFUSION			((LONGLONG)1 << MST_ASSIST_CONFUSION)		// 홀란
#define AST_STONE				((LONGLONG)1 << MST_ASSIST_STONE)			// 석화
#define AST_SILENT				((LONGLONG)1 << MST_ASSIST_SILENT)		// 봉인
#define AST_BLOOD				((LONGLONG)1 << MST_ASSIST_BLOOD)			// 출혈
#define AST_BLIND				((LONGLONG)1 << MST_ASSIST_BLIND)			// 블라인드
#define AST_STURN				((LONGLONG)1 << MST_ASSIST_STURN)			// 스턴
#define AST_SLEEP				((LONGLONG)1 << MST_ASSIST_SLEEP)			// 슬립
#define AST_HP					((LONGLONG)1 << MST_ASSIST_HP)			// 체력
#define AST_MP					((LONGLONG)1 << MST_ASSIST_MP)			// 마나
#define AST_MOVESPD				((LONGLONG)1 << MST_ASSIST_MOVESPD)		// 이속 향상
#define AST_HP_CANCEL			((LONGLONG)1 << MST_ASSIST_HP_CANCEL)		// HP변화(0, MAX에서 취소)
#define AST_MP_CANCEL			((LONGLONG)1 << MST_ASSIST_MP_CANCEL)		// MP변화(0, MAX에서 취소)
#define AST_DIZZY				((LONGLONG)1 << MST_ASSIST_DIZZY)			// 현기증
#define AST_INVISIBLE			((LONGLONG)1 << MST_ASSIST_INVISIBLE)		// 인비저빌리티
#define AST_SLOTH				((LONGLONG)1 << MST_ASSIST_SLOTH)			// 슬로스(공속하락)
#define AST_FEAR				((LONGLONG)1 << MST_ASSIST_FEAR)			// 공포
#define AST_FAKEDEATH			((LONGLONG)1 << MST_ASSIST_FAKEDEATH)		// 죽은척하기
#define AST_PERFECTBODY			((LONGLONG)1 << MST_ASSIST_PERFECTBODY)	// 타이탄 퍼펙 바디
#define AST_FRENZY				((LONGLONG)1 << MST_ASSIST_FRENZY)		// 타이탄 프렌지
#define AST_DAMAGELINK			((LONGLONG)1 << MST_ASSIST_DAMAGELINK)	// 데미지 링크
#define AST_BERSERK				((LONGLONG)1 << MST_ASSIST_BERSERK)		// 버서크
#define AST_DESPAIR				((LONGLONG)1 << MST_ASSIST_DESPAIR)		// 절망
#define AST_MANASCREEN			((LONGLONG)1 << MST_ASSIST_MANASCREEN)	// 마나 스크린
#define AST_BLESS				((LONGLONG)1 << MST_ASSIST_BLESS)			// 블레싱
#define AST_SAFEGUARD			((LONGLONG)1 << MST_ASSIST_SAFEGUARD)		// 보호망
#define AST_MANTLE				((LONGLONG)1 << MST_ASSIST_MANTLE)		// 망토
#define AST_GUARD				((LONGLONG)1 << MST_ASSIST_GUARD)			// 경비병 소환
//-- (-2) 장탄 2종 사용안함
#define AST_DISEASE				((LONGLONG)1 << (MST_ASSIST_DISEASE-2) )	// 망토
#define AST_CURSE				((LONGLONG)1 << (MST_ASSIST_CURSE-2) )		// 경비병 소환
#define AST_CONFUSED			((LONGLONG)1 << MST_ASSIST_CONFUSED)		// 몬스터만 혼란에 걸리게 함
#define AST_TAMING				((LONGLONG)1 << MST_ASSIST_TAMING)			// 몬스터를 일정시간 아군으로 만듬
#define AST_FREEZE				((LONGLONG)1 << MST_ASSIST_FREEZE)			// 결계 (서로 공격 못함, 걸린 것은 이동 못함)
#define AST_INVERSE_DAMAGE		((LONGLONG)1 << MST_ASSIST_INVERSE_DAMAGE)	// 피격당하면 데미지 만큼 HP 회복
#define AST_HP_DOT				((LONGLONG)1 << MST_ASSIST_HP_DOT)			// 지속데미지 입음
#define AST_REBIRTH				((LONGLONG)1 << MST_ASSIST_REBIRTH)			// 죽을때 이 버프가 있으면 바로 부활
#define AST_DARKNESS_MODE		((LONGLONG)1 << MST_ASSIST_DARKNESS_MODE)	// 사도 모드
#define AST_AURA_DARKNESS		((LONGLONG)1 << MST_ASSIST_AURA_DARKNESS)	// 오오라 - 다크니스
#define AST_AURA_WEAKNESS		((LONGLONG)1 << MST_ASSIST_AURA_WEAKNESS)	// 오오라 - 위크니스
#define AST_AURA_ILUSYON		((LONGLONG)1 << MST_ASSIST_AURA_ILLUSION)	// 오오라 - 일루젼
#define AST_MERCENARY			((LONGLONG)1 << MST_ASSIST_MERCENARY)		// 몬스터를 용병으로 만듬
#define AST_SOUL_TOTEM_BUFF		((LONGLONG)1 << MST_ASSIST_SOUL_TOTEM_BUFF)	// 몬스터 토템 버프형
#define AST_SOUL_TOTEM_ATTK		((LONGLONG)1 << MST_ASSIST_SOUL_TOTEM_ATTK)	// 몬스터 토템 공격형
#define AST_TRAP				((LONGLONG)1 << MST_ASSIST_TRAP)			// 트랩
#define AST_PARASITE			((LONGLONG)1 << MST_ASSIST_PARASITE)		// 기생충 감염
#define AST_SUICIDE				((LONGLONG)1 << MST_ASSIST_SUICIDE)			// 자폭
#define AST_INVINCIBILITY		((LONGLONG)1 << MST_ASSIST_INVINCIBILITY)	// 무적
#define AST_TOWER_ATTACK		((LONGLONG)1 << MST_ASSIST_ATTACK_TOWER)	// 공성 타워 데미지 추가

#define SKILL_PARAM_MIN			75
#define SKILL_PARAM_MAX			125

#define SPARAM_NONE				0
#define SPARAM_STR				1
#define SPARAM_DEX				2
#define SPARAM_INT				3
#define SPARAM_CON				4

// 소서러 플래그
#define SSF_APP_CHARACTER		(1 << 0)	// 캐릭터
#define SSF_APP_FIRE			(1 << 1)	// 불의정령
#define SSF_APP_WIND			(1 << 2)	// 바람의정령
#define SSF_APP_EARTH			(1 << 3)	// 대지의정령
#define SSF_APP_WATER			(1 << 4)	// 물의정령
#define SSF_APP_HELLOUND		(1 << 5)	// 헬운드
#define SSF_APP_ELENEN			(1 << 6)	// 엘레넨
#define SSF_APP_APET			(1 << 7)	// 공격형 펫
#define SSF_APP_ALL				(SSF_APP_CHARACTER | SSF_APP_FIRE | SSF_APP_WIND | SSF_APP_EARTH | SSF_APP_WATER | SSF_APP_HELLOUND | SSF_APP_ELENEN | SSF_APP_APET)
#define SSF_USE_CHARACTER		(1 << 16)	// 캐릭터
#define SSF_USE_FIRE			(1 << 17)	// 불의정령
#define SSF_USE_WIND			(1 << 18)	// 바람의정령
#define SSF_USE_EARTH			(1 << 19)	// 대지의정령
#define SSF_USE_WATER			(1 << 20)	// 물의정령
#define SSF_USE_HELLOUND		(1 << 21)	// 헬운드
#define SSF_USE_ELENEN			(1 << 22)	// 엘레넨
#define SSF_USE_APET			(1 << 23)	// 공격형 펫
#define SSF_USE_ALL				(SSF_USE_CHARACTER | SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER | SSF_USE_HELLOUND | SSF_USE_ELENEN | SSF_USE_APET)

#define IMMOTAL_BUF				469			//무적 버프 스킬인덱스
#define FLY_SKILL				688
#define DARKNESS_SKILL			682
#define PVP_PROTECT_SKILL_INDEX	1791

#define SKILL_GUILD_ROOM_WARP 1831
#define SKILL_GUILD_ROOM_RECALL 1827
#define SKILL_GUILD_PARTY_EXP	1825

// --- skill

class CCharacter;
class CPC;
class CPet;
class CAPet;

/////////////////////
// Class name	    : CMagicLevelProto
// Description	    : 효과 레벨별 설정
class CMagicLevelProto
{
public:
	int				m_nPowerValue;		// 위력
	int				m_nHitrateValue;	// 명중률
	int	GetSummonNpcIndex()
	{
		return m_nPowerValue;
	}
	CMagicLevelProto(int nPower, int nHitrate);
};

/////////////////////
// Class name	    : CMagicProto
// Description	    : 효과 설정
class CMagicProto
{
public:
	int					m_index;		// 번호
	char				m_maxlevel;		// 최대레벨
	char				m_type;			// 타입
	char				m_subtype;		// 서브타입
	char				m_damagetype;	// 대미지 타입
	char				m_hittype;		// 명중 타입
	char				m_attribute;	// 속성
	int					m_psp;			// 스킬 사용자의 대미지 파라미터
	int					m_ptp;			// 스킬 대상의 대미지 파라미터
	int					m_hsp;			// 스킬 사용자의 명중 파라미터
	int					m_htp;			// 스킬 대상의 명중 파라미터
	int					m_bTogel;		// 스킬 토글이 가능한가?

	CMagicLevelProto**	m_levelproto;	// 레벨 설정

	CMagicProto(int index, char maxlevel, char type, char subtype, char damagetype, char hittype, char attribute, int psp, int ptp, int hsp, int htp, int bTogel, CMagicLevelProto** levelproto);

	~CMagicProto();

	const CMagicLevelProto* Level(char level) const;
};

/////////////////////
// Class name	    : CSkillLevelProto
// Description	    : 스킬 레벨 설정
class CSkillLevelProto
{
public:
	int					m_needHP;
	int					m_needMP;
	int					m_needGP;
	int					m_durtime;
	int					m_hate;
	int					m_needItemIndex[MAX_SKILL_NEED_ITEM];
	int					m_needItemCount[MAX_SKILL_NEED_ITEM];
	int					m_learnLevel;
	int					m_learnSP;
	int					m_learnSkillIndex[MAX_SKILL_LEARN_SKILL];
	char				m_learnSkillLevel[MAX_SKILL_LEARN_SKILL];
	int					m_learnItemIndex[MAX_SKILL_LEARN_ITEM];
	int					m_learnItemCount[MAX_SKILL_LEARN_ITEM];
	int					m_learnstr;
	int					m_learndex;
	int					m_learnint;
	int					m_learncon;
	int					m_appMagicIndex[MAX_SKILL_APP_MAGIC];
	char				m_appMagicLevel[MAX_SKILL_APP_MAGIC];
	const CMagicProto*	m_magic[MAX_SKILL_MAGIC];
	char				m_magicLevel[MAX_SKILL_MAGIC];
	int					m_useCount;
	int					m_targetNum;
	int					m_learnGP;
	CSkillLevelProto(int needHP, int needMP, int needGP, int durtime, int hate, int needItemIndex[2], int needItemCount[2], int learnLevel, int learnSP, int learnSkillIndex[3], char learnSkillLevel[3], int learnItemIndex[3], int learnItemCount[3], int learnstr, int learndex, int learnint, int learncon, int appMagicIndex[3], char appMagicLevel[3], const CMagicProto* magic[3], char magicLevel[3], int learnGP
					 , int useCount
					 , int targetNum
					);
};

/////////////////////
// Class name	    : CSkillProto
// Description	    : 스킬 설정
class CSkillProto
{
public:
	int					m_index;
	char				m_type;
	int					m_flag;
	char				m_maxLevel;
	float				m_appRange;
	char				m_targetType;
	int					m_readyTime;
	int					m_stillTime;
	int					m_fireTime;
	int					m_reuseTime;
	int					m_selfparam;
	int					m_targetparam;

	int					m_applyState;
	int					m_applyWeaponType0;
	int					m_applyWeaponType1;

	int					m_nAPetIndex;

	int					m_soul_consum;
	int					m_useWearing;
	int					m_appWearing;

	int					m_summonIDX;

private:
	int					m_job;
	int					m_job2;
	float				m_fireRange;
	float				m_fireRange2;
	float				m_minRange;
	int					m_useState;
	int					m_useWeaponType0;
	int					m_useWeaponType1;
	int					m_useMagicIndex[MAX_SKILL_USE_MAGIC];
	char				m_useMagicLevel[MAX_SKILL_USE_MAGIC];
	int					m_sorcerer_flag;

	bool				is_active_flag;
	bool				is_passive_flag;
	bool				is_summon_flag;
	bool				is_rvr_zone_not_use;
public:

	CSkillLevelProto**	m_levelproto;

	CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex, int soul_consum, int useWearing, int appWearing, int summonIdx );

	~CSkillProto();

	const CSkillLevelProto* Level(char level) const;

	/////////////////////
	// Function name	: IsActive
	// Description	    : 액티브 스킬 검사
	bool IsActive() const
	{
		return is_active_flag;
	}

	/////////////////////
	// Function name	: IsPassive
	// Description	    : 패시브 스킬 검사
	bool IsPassive() const
	{
		return is_passive_flag;
	}

	/////////////////////
	// Function name	: IsSummon
	// Description	    : 서먼 스킬 검사
	bool IsSummon() const
	{
		return is_summon_flag;
	}

	bool isRVRNotUse() const
	{
		return is_rvr_zone_not_use;
	}

	bool IsSeal() const;

	////////////////////
	// Function name	: IsSameJob
	// Description	    : 같은 직업인지 검사
	bool IsSameJob(CPC* pc) const;
	bool IsSameJob(CPet* pet) const;

	bool IsSameJob(CAPet* apet) const;

	////////////////////
	// Function name	: GetFireRange
	// Description	    : 사정거리 검사, NPC는 1번, PC는 무기에 따라
	float GetFireRange(CCharacter* ch) const;

	////////////////////
	// Function name	: CheckUseState
	// Description	    : 사용 가능 검사
	// Return type		: int
	//                  : 사용 불가능시에 불충분한 상태 반환, -1은 magic 불능에서
	int CheckUseState(CCharacter* ch) const;

	// a_minRange
	float GetMinRange() const;

	int Get2ndJob() const
	{
		return m_job2;
	}

	int getJob() const
	{
		return m_job;
	}

	// 소서러용 플래그 확인
	int CheckSorcererFlag(int mask) const
	{
		return (m_sorcerer_flag & mask);
	}

#ifdef EXTREME_CUBE_VER2
	bool CheckAllowZone(char zoneindex) const;
	bool LoadAllowZone(const char* strZoneList);

private:
	struct SKILLALLOWZONE
	{
		char	count;
		char*	zoneindex;
		SKILLALLOWZONE(char nCount)
		{
			count = nCount;
			zoneindex = new char[nCount];
			memset(zoneindex, -1, sizeof(char) * nCount);
		}
		~SKILLALLOWZONE()
		{
			count = 0;
			if(zoneindex)
			{
				delete [] zoneindex;
				zoneindex = NULL;
			}
		}
	};

	SKILLALLOWZONE* m_allowzone;
#endif // EXTREME_CUBE_VER2
};

/////////////////////
// Class name	    : CMagicProtoList
// Description	    : 효과 설정 리스트
class CMagicProtoList
{
public:
	typedef std::map<int, CMagicProto*> map_t;

	map_t			map_;
	CMagicProto**	m_list;
	int				m_count;

public:
	CMagicProtoList();
	~CMagicProtoList();

	/////////////////////
	// Function name	: Load
	// Description	    : 효과 리스트 읽기
	// Return type		: bool
	//            		: 성공 여부
	bool Load();

	/////////////////////
	// Function name	: Find
	// Description	    : 특정 인덱스의 효과 찾기
	// Return type		: const CMagicProto*
	//            		: 찾은 효과 포인터, 실패시 NULL
	// Argument         : int index
	//                  : 찾고자 하는 효과 번호
	const CMagicProto* Find(int index);
};

// 몬스터 멀티타겟용 구조체
struct MultiTarget : public MemoryPoolBase<MultiTarget>
{
	CCharacter*		mtarget;
	char			mtargettype;
	int				mtargetindex;
	float			mdist;

	MultiTarget()
		: mtarget(NULL)
		, mtargettype(-1)
		, mtargetindex(0)
		, mdist(9999)
	{
	}
};

/////////////////////
// Class name	    : CSkill
// Description	    : 스킬
class CSkill : public MemoryPoolBaseWithMutex<CSkill>
{
public:
	const CSkillProto*	m_proto;			// 스킬 프로토
	char				m_level;			// 스킬 레벨

	int					m_state;
	int					m_usetime;
	MSG_CHAR_TYPE		m_targetType;
	int					m_targetIndex;
	int					m_enable;

	struct guild_skill_cooltime_data
	{
		int char_index;
		int usetime;
	};

	typedef std::map<int/*char_index*/, int/*usetime*/> GUILD_SKILL_USETIME_DATA;
	GUILD_SKILL_USETIME_DATA m_gskill_usetime;

	int					m_optionSkillProb;	//옵션 스킬 발동 확률

	bool				m_toggle;

	CSkill(const CSkillProto* proto, char level);

	/////////////////////
	// Function name	: IsReady
	// Description	    : 스킬 재사용 가능 검사
	// Argument         : CCharacter* ch
	//                  : 스킬 쓰는 사람
	bool IsReady(CCharacter* ch);

	/////////////////////
	// Function name	: Ready
	// Description	    : 시전 상태로 만듬
	void Ready(CCharacter* tch);

	/////////////////////
	// Function name	: Fire
	// Description	    : 발사 상탱로 만듬
	void Fire();

	/////////////////////
	// Function name	: Cancel
	// Description	    : 시전 취소
	void Cancel(CCharacter* pCharSkill);
};

/////////////////////
// Class name	    : CSkillProtoList
// Description	    : 스킬 리스트
class CSkillProtoList
{
public:
	typedef std::map<int, CSkillProto*> map_t;

	CSkillProtoList();
	~CSkillProtoList();

	/////////////////////
	// Function name	: Load
	// Description	    : 스킬 리스트 읽기
	// Return type		: bool
	//            		: 성공 여부
	bool Load();

	/////////////////////
	// Function name	: Find
	// Description	    : 특정 스킬 찾기
	// Return type		: const CSkillProto*
	//            		: 찾은 스킬 프로토, 실패시 NULL
	// Argument         : int skillIndex
	//                  : 찾고자 하는 스킬 번호
	const CSkillProto* Find(int skillIndex);

	/////////////////////
	// Function name	: Create
	// Description	    : 스킬 생성
	// Return type		: CSkill*
	//            		: 생성된 스킬 포인터
	// Argument         : int skillIndex
	//                  : 생성할 스킬 번호
	// Argument         : int level = 1
	//                  : 생성할 스킬 레벨
	CSkill* Create(int skillIndex, int level = 1);

	void LearnAllSkill(CPC* ch);

private:
	map_t			map_;
	CSkillProto**	m_list;		// 스킬 정보
	int				m_count;	// 스킬 정보 개수
};

class CSkillListNode
{
	CSkill*			m_skill;	// 스킬 데이터
	CSkillListNode*	m_prev;		// 이전 포인터
	CSkillListNode* m_next;		// 이후 포인터

public:
	CSkillListNode(CSkill* skill);
	~CSkillListNode()
	{
		if (m_skill) delete m_skill;
		m_skill = NULL;
	}

	CSkillListNode* prev();
	void prev(CSkillListNode* p);
	CSkillListNode* next();
	void next(CSkillListNode* p);

	CSkill* skill();
	void skill(CSkill* p);

	int m_wearPos;
};

/////////////////////
// Class name	    : CSkillList
// Description	    : 캐릭터 스킬 리스트
class CSkillList
{
	CSkillListNode*		m_head;		// 헤더
	CSkillListNode*		m_tail;		// 테일
	int					m_count;	// 개수

public:
	void DeleteInstantSkill(int flag);
	CSkillList();
	~CSkillList();

	/////////////////////
	// Function name	: Add
	// Description	    : 스킬 추가, 기존의 스킬과 중복 될 경우 레벨만 재설정
	// Return type		: bool
	//            		: 성공여부
	// Argument         : CSkill* skill
	//                  : 추가할 스킬, 기존 스킬과 중복 될 경우 메모리 해제후, 기존 스킬로 설정됨
	bool Add(CSkill*& skill);

	/////////////////////
	// Function name	: Find
	// Description	    : 스킬 찾기
	// Return type		: CSkill*
	//            		: 찾은 스킬, 실패시 NULL
	// Argument         : int index
	//                  : 찾을 스킬 번호
	CSkill* Find(int index);
	//node 찾기
	CSkillListNode* FindNode(int index);

	/////////////////////
	// Function name	: count
	// Description	    : 스킬 개수
	int count();

	/////////////////////
	// Function name	: GetIndexString
	// Description	    : 스킬 인덱스 스트링 얻기
	// Return type		: char*
	//            		: 입력된 문자열 포인터
	// Argument         : char* buf
	//                  : 스트링 버퍼
	char* GetIndexString(char* buf);
	void GetIndexString(std::string& bnf);

	/////////////////////
	// Function name	: GetLevelString
	// Description	    : 스킬 레벨 스트링 얻기
	// Return type		: char*
	//            		: 입력된 스트링 포인터
	// Argument         : char* buf
	//                  : 스트링 버퍼
	char* GetLevelString(char* buf);
	void GetLevelString(std::string& bnf);

	/////////////////////
	// Function name	: GetHeadPosition
	// Description	    : 리스트 처음 위치를 반환, 비었으면 NULL
	void* GetHeadPosition();

	/////////////////////
	// Function name	: GetNext
	// Description	    : 리스트의 스킬을 반환
	// Argument         : void*& p
	//                  : in: 읽을 데이터 위치, out: 다음 데이터 위치, 더이상 없으면 NULL
	CSkill* GetNext(void*& p);

	////////////////////
	// Function name	: RemoveSkillFor2ndJob
	// Description	    : 전직 직업용 스킬 지우기
	int RemoveSkillFor2ndJob();		// reutrn 소요된 skillpoint

	// 스킬 초기화
	void Clear();
	void clearReadySkill();

	/////////////////////
	// Function name	: Remove
	// Description	    : 스킬 삭제 (오리지날 옵션/세트 아이템으로 인한 스킬 삭제)
	// Return type		: bool
	//            		: 성공여부
	// Argument         : CSkill* skill
	//                  : 삭제할 스킬
	bool Remove(CSkill* skill);

	bool Disable(CSkill* skill);

	bool AddOptionSkill(CSkill* skill, int prob, int wearPos);

	void SaveSkillCoolTime(CPC* pc, std::vector<std::string>& vec);
};

int ApplySkill(CCharacter* ch, CCharacter* tch, CSkill* skill, int itemidx, bool& bApply);
void ApplySkillParty(CPC* ch, CSkill* skill, int itemidx, bool& bApply);
int CalcSkillParam(const CCharacter* ch, const CCharacter* tch, int selfparam, int targetparam);

void ApplySkillExped(CPC* ch, CSkill* skill, int itemidx, bool& bApply);
void ApplySkillGuild(CPC* ch, CSkill* skill, int itemidx, bool& bApply);

bool ApplyWarSkill(CPC* ch, int zone_index);
void ClearWarSkill(CPC* ch);
void ClearWarItemSkill(CPC* ch);
#endif
//
