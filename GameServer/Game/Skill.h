#ifndef __SKILL_H__
#define __SKILL_H__

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "MessageType.h"
#include "Config.h"
#include "NetMsg.h"
#define NULL 0
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

#ifdef FACTORY_SYSTEM
#define FACTORY_UNION_REG_SKILL			655			// 제작 연합 등록 스킬 번호
#endif

////////////
// 스킬 상태
#define SKILL_CON_NORMAL				0			// 일반 상태
#define SKILL_CON_READY					1			// 시전 상태
#define SKILL_CON_FIRE					2			// 발동 상태

#ifdef ATTR_SYSTEM
// OLD ATTRIBUTE VARIABLE REUSE, Elenoa 2013.01.09
// 속성 상수
#define	AT_NONE					0			// 무
#define	AT_FIRE					1			// 화
#define	AT_WATER				2			// 수
#define	AT_EARTH				3			// 지
#define	AT_WIND					4			// 풍
#define	AT_DARK					5			// 암
#define	AT_LIGHT				6			// 광
#if 0 // NOT USE AT_ALL, Elenoa 2013.01.09
#define AT_ALL					99			// all
#endif
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
#endif // ATTR_SYSTEM

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
#define MST_ASSIST_MERCENARY		43		// 몬스터를 용병으로 바꿈. //#ifdef SYSTEM_MONSTER_MERCENARY_CARD
#define MST_ASSIST_SOUL_TOTEM_BUFF	44		// 고대 무기(토템))의 정령(버프형)
#define MST_ASSIST_SOUL_TOTEM_ATTK	45		// 고대 무기(토템)의 정령(공격형)
#define MST_ASSIST_TRAP				46		// 몬스터를 트랩으로
#define MST_ASSIST_PARASITE			47		// 패러사이트 감염 (감염된 상태로 죽으면 자폭몬스터를 소환하여 동족을 죽임)
#define MST_ASSIST_SUICIDE			48		// 몬스터를 자폭시킴
#define MST_ASSIST_INVINCIBILITY	49		// 무적 모드

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
//#ifdef AFFINITY_BOOSTER
#define MST_OTHER_AFFINITY		23			// 친화도 부스터
//#endif  AFFINITY_BOOSTER

// 효과 서브타입 - 대미지 흡수
#define MST_REDUCE_MELEE		0			// 근접 공격 흡수
#define MST_REDUCE_RANGE		1			// 원거리 공격 흡수
#define MST_REDUCE_MAGIC		2			// 마법 공격 흡수
#define MST_REDUCE_SKILL		3			// 스킬 공격 흡수

// 효과 서브타입 - 면역
#define MST_IMMUNE_BLIND		0			// 블라인드

// 대미지 타입
#define MDT_ONLYPOWER			0			// 스킬 위력만
#define MDT_ADDITION			1			// 스탯에 덧셈
#define MDT_RATE				2			// 스탯에 비율로 곱셈

// 명중 타입
#define MHT_CONSTANT			0			// 고정
#define MHT_VARIABLE			1			// 수식

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
#ifdef NEW_GUILD
#define ST_GUILD_SKILL_PASSIVE	7			// 길드 스킬(패시브)
#endif // NEW_GUILD
#ifdef FACTORY_SYSTEM
#define ST_SEAL					8			// 제작 시스템 스킬(증표)
#endif
//#ifdef SYSTEM_SUMMON_SKILL
#define ST_SUMMON_SKILL	9			// 토템 소환 스킬
//#endif

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
#ifdef GUILD_SKILL
#define STT_GUILD_ALL			9			// 길드 전체에게 주는 HELP 스킬
#define STT_GUILD_ONE			10			// 길드 한명에게 주는 HELP 스킬
#define STT_GUILD_SELF_RANGE	11			// 시전자 주위 NPC에게 주는 ATTACK SKILL
#endif // GUILD_SKILL

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

// 사용/적용 조건
#define SCT_DEATH				(1 << 0)	// 사망
#define SCT_WEAPON				(1 << 1)	// 무기장비
#define SCT_SITDOWN				(1 << 2)	// 앉기
#define SCT_PEACEZONE			(1 << 3)	// 평화지역
#define SCT_SHIELD				(1 << 4)	// 방패착용
#define SCT_STAND				(1 << 5)	// 서기
#ifdef NIGHTSHADOW_SKILL
#define SCT_DARKNESS			(1 << 6)    // 나이트 쉐도우 사도 모드에서만 사용하는 스킬인지 확인
#endif // NIGHTSHADOW_SKILL
#define SCT_NO_COOL_TIME		(1 << 7)	// 쿨타임감소 적용 불가

// 상태이상 비트 필드 값
#ifdef NIGHTSHADOW_SKILL
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

#else // NIGHTSHADOW_SKILL

#define AST_POISON				(1 << MST_ASSIST_POISON)		// 독
#define AST_HOLD				(1 << MST_ASSIST_HOLD)			// 홀드
#define AST_CONFUSION			(1 << MST_ASSIST_CONFUSION)		// 홀란
#define AST_STONE				(1 << MST_ASSIST_STONE)			// 석화
#define AST_SILENT				(1 << MST_ASSIST_SILENT)		// 봉인
#define AST_BLOOD				(1 << MST_ASSIST_BLOOD)			// 출혈
#define AST_BLIND				(1 << MST_ASSIST_BLIND)			// 블라인드
#define AST_STURN				(1 << MST_ASSIST_STURN)			// 스턴
#define AST_SLEEP				(1 << MST_ASSIST_SLEEP)			// 슬립
#define AST_HP					(1 << MST_ASSIST_HP)			// 체력
#define AST_MP					(1 << MST_ASSIST_MP)			// 마나
#define AST_MOVESPD				(1 << MST_ASSIST_MOVESPD)		// 이속 향상
#define AST_HP_CANCEL			(1 << MST_ASSIST_HP_CANCEL)		// HP변화(0, MAX에서 취소)
#define AST_MP_CANCEL			(1 << MST_ASSIST_MP_CANCEL)		// MP변화(0, MAX에서 취소)
#define AST_DIZZY				(1 << MST_ASSIST_DIZZY)			// 현기증
#define AST_INVISIBLE			(1 << MST_ASSIST_INVISIBLE)		// 인비저빌리티
#define AST_SLOTH				(1 << MST_ASSIST_SLOTH)			// 슬로스(공속하락)
#define AST_FEAR				(1 << MST_ASSIST_FEAR)			// 공포
#define AST_FAKEDEATH			(1 << MST_ASSIST_FAKEDEATH)		// 죽은척하기
#define AST_PERFECTBODY			(1 << MST_ASSIST_PERFECTBODY)	// 타이탄 퍼펙 바디
#define AST_FRENZY				(1 << MST_ASSIST_FRENZY)		// 타이탄 프렌지
#define AST_DAMAGELINK			(1 << MST_ASSIST_DAMAGELINK)	// 데미지 링크
#define AST_BERSERK				(1 << MST_ASSIST_BERSERK)		// 버서크
#define AST_DESPAIR				(1 << MST_ASSIST_DESPAIR)		// 절망
#define AST_MANASCREEN			(1 << MST_ASSIST_MANASCREEN)	// 마나 스크린
#define AST_BLESS				(1 << MST_ASSIST_BLESS)			// 블레싱
#define AST_SAFEGUARD			(1 << MST_ASSIST_SAFEGUARD)		// 보호망
#define AST_MANTLE				(1 << MST_ASSIST_MANTLE)		// 망토
#define AST_GUARD				(1 << MST_ASSIST_GUARD)			// 경비병 소환
//-- (-2) 장탄 2종 사용안함
#define AST_DISEASE				(1 << (MST_ASSIST_DISEASE-2) )		// 망토
#define AST_CURSE				(1 << (MST_ASSIST_CURSE-2) )		// 경비병 소환
#endif // NIGHTSHADOW_SKILL

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

// --- skill

#if 0 // OLD ATTRIBUTE BLOCK, Elenoa 2013.01.09
class CAttributeData
{
	int m_water;		// 수
	int m_wind;			// 풍
	int m_earth;		// 지
	int m_fire;			// 화
	int m_dark;			// 암
	int m_light;		// 광

public:
	CAttributeData() { Reset(); }

	void Reset() { memset(this, 0, sizeof(*this)); }
	void Add(int type, int val, bool bFromItem);

	int GetValue(int type) const;
};
#endif

class CCharacter;
class CPC;
//#ifdef ENABLE_PET
class CPet;
//#endif
class CAPet;

/////////////////////
// Class name	    : CMagicLevelProto
// Description	    : 효과 레벨별 설정
class CMagicLevelProto {
     int m_nPowerValue;
     int m_nHitrateValue;
   public:
     int GetSummonNpcIndex(void);
     CMagicLevelProto(int, int);
 };

/////////////////////
// Class name	    : CMagicProto
// Description	    : 효과 설정
class CMagicProto {
         int m_index;
         char m_maxlevel;
         char m_type;
         char m_subtype;
         char m_damagetype;
         char m_hittype;
         char m_attribute;
         int m_psp;
         int m_ptp;
         int m_hsp;
         int m_htp;
         int m_bTogel;
         CMagicLevelProto **m_levelproto;
       public:
         CMagicProto(int, char, char, char, char, char, char, int, int, int, int, int, CMagicLevelProto **);
         ~CMagicProto();
         const CMagicLevelProto * Level(char) const;
     };

/////////////////////
// Class name	    : CSkillLevelProto
// Description	    : 스킬 레벨 설정
class CSkillLevelProto
{
public:
	int m_needHP;
     int m_needMP;
     int m_needGP;
     int m_durtime;
     int m_hate;
     int m_needItemIndex[2];
     int m_needItemCount[2];
     int m_learnLevel;
     int m_learnSP;
     int m_learnSkillIndex[3];
     char m_learnSkillLevel[3];
     int m_learnItemIndex[3];
     int m_learnItemCount[3];
     int m_learnstr;
     int m_learndex;
     int m_learnint;
     int m_learncon;
     int m_appMagicIndex[3];
     char m_appMagicLevel[3];
     const CMagicProto *m_magic[3];
     char m_magicLevel[3];
     int m_learnGP;
   public:
	CSkillLevelProto(int, int, int, int, int, int *, int *, int, int, int *, char *, int *, int *, int, int, int, int, int *, char *, const CMagicProto **, char *, int);
};

/////////////////////
// Class name	    : CSkillProto
// Description	    : 스킬 설정
class CSkillProto
{
   public:
     int m_index;
     char m_type;
     int m_flag;
     char m_maxLevel;
     float m_appRange;
     char m_targetType;
     char m_targetNum;
     int m_readyTime;
     int m_stillTime;
     int m_fireTime;
     int m_reuseTime;
     int m_selfparam;
     int m_targetparam;
     int m_applyState;
     int m_applyWeaponType0;
     int m_applyWeaponType1;
     int m_nAPetIndex;
     int m_soul_consum;
     int m_useWearing;
     int m_appWearing;
     int m_summonIDX;
   private:
     int m_job;
     int m_job2;
     float m_fireRange;
     float m_fireRange2;
     float m_minRange;
     int m_useState;
     int m_useWeaponType0;
     int m_useWeaponType1;
     int m_useMagicIndex[3];
     char m_useMagicLevel[3];
     int m_sorcerer_flag;
   public:
     CSkillLevelProto **m_levelproto;

#ifdef ATTACK_PET
	#ifdef NIGHTSHADOW_SKILL
		#ifdef SYSTEM_SUMMON_SKILL
	CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex, int soul_consum, int useWearing, int appWearing, int summonIdx );
		#else
	CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex, int soul_consum, int useWearing, int appWearing );
		#endif
	#else
	CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex );
	#endif // NIGHTSHADOW_SKILL
#else // ATTACK_PET
	CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto);
#endif //ATTACK_PET

	~CSkillProto();

	const CSkillLevelProto* Level(char level) const;

	/////////////////////
	// Function name	: IsActive
	// Description	    : 액티브 스킬 검사
	bool IsActive() const;

	/////////////////////
	// Function name	: IsPassive
	// Description	    : 패시브 스킬 검사
	bool IsPassive() const;

	/////////////////////
	// Function name	: IsSummon
	// Description	    : 서먼 스킬 검사
	bool IsSummon() const;

#ifdef FACTORY_SYSTEM
	bool IsSeal() const;
#endif

	////////////////////
	// Function name	: IsSameJob
	// Description	    : 같은 직업인지 검사
	//bool IsSameJob(CPC* pc) const;
//#ifdef ENABLE_PET
	//bool IsSameJob(CPet* pet) const;
//#endif

#ifdef ATTACK_PET
	bool IsSameJob(CAPet* apet) const;
#endif //ATTACK_PET

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

//#ifdef ENABLE_CHANGEJOB
	int Get2ndJob() const { return m_job2; }
//#endif

	// 소서러용 플래그 확인
	int CheckSorcererFlag(int mask) const { return (m_sorcerer_flag & mask); }

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
};

/////////////////////
// Class name	    : CMagicProtoList
// Description	    : 효과 설정 리스트
class CMagicProtoList {
     CMagicProto **m_list;
     int m_count;
   public:
     CMagicProtoList(void);
     ~CMagicProtoList();
     bool Load(void);
     const CMagicProto * Find(int);
 };
// 몬스터 멀티타겟용 구조체
struct MultiTarget {
	CCharacter*		mtarget;
	char			mtargettype;
	int				mtargetindex;
	float			mdist;

	void Init( void ) // 초기화
	{
		mtarget = NULL;
		mtargetindex = 0;
		mtargettype = -1;
		mdist = 9999;
	}
	MultiTarget()
	{	Init();	}
};

/////////////////////
// Class name	    : CSkill
// Description	    : 스킬
class CSkill {
   public:
     const CSkillProto *m_proto;
     char m_level;
     int m_state;
     int m_usetime;
     MSG_CHAR_TYPE m_targetType;
     int m_targetIndex;
     int m_optionSkillProb;

     CSkill(const CSkillProto *, char);
     bool IsReady(CCharacter *);
     void Ready(CCharacter *);
     void Fire(void);
     void Cancel(CCharacter *);
 };

/////////////////////
// Class name	    : CSkillProtoList
// Description	    : 스킬 리스트
class CSkillProtoList {
   private:
	 typedef std::map<int, CSkillProto*, std::less<int>, std::allocator<std::pair<int const, CSkillProto*> > > map_t;
     CSkillProtoList::map_t map_;
     CSkillProto **m_list;
     int m_count;

   public:
     CSkillProtoList(void);
     ~CSkillProtoList();
     bool Load(void);
     const CSkillProto * Find(int);
     CSkill * Create(int, int);
     void LearnAllSkill(CPC *);
 };

class CSkillListNode {
   private:
     CSkill *m_skill;
     CSkillListNode *m_prev;
     CSkillListNode *m_next;

   public:
     CSkillListNode(CSkill *);
     ~CSkillListNode();
     CSkillListNode * prev(void);
     void prev(CSkillListNode *);
     CSkillListNode * next(void);
     void next(CSkillListNode *);
     CSkill * skill(void);
     void skill(CSkill *);
 };

/////////////////////
// Class name	    : CSkillList
// Description	    : 캐릭터 스킬 리스트
class CSkillList {
   private:
     CSkillListNode *m_head;
     CSkillListNode *m_tail;
     int m_count;
   public:
     int m_wearPos;

     void DeleteInstantSkill(int);
     CSkillList(void);
     ~CSkillList();
     bool Add(CSkill *&);
     CSkill * Find(int);
     int count(void);
     char * GetIndexString(char *);
     char * GetLevelString(char *);
     void * GetHeadPosition(void);
     CSkill * GetNext(void *&);
     int RemoveSkillFor2ndJob(void);
     void Clear(void);
     bool Remove(CSkill *);
     bool AddOptionSkill(CSkill *, int, int);
 };

typedef struct __tagSealSkillData {
       int nSkillIdx;
       CSkill *pSkill;
       LONGLONG llExp;
   } SEAL_SKILL_DATA; 

int ApplySkill(CCharacter* ch, CCharacter* tch, CSkill* skill, int itemidx, bool& bApply);
void ApplySkillParty(CPC* ch, CSkill* skill, int itemidx, bool& bApply);
int CalcSkillParam(const CCharacter* ch, const CCharacter* tch, int selfparam, int targetparam);

#ifdef EXPEDITION_RAID
void ApplySkillExped(CPC* ch, CSkill* skill, int itemidx, bool& bApply);
#endif //EXPEDITION_RAID
#ifdef GUILD_SKILL
void ApplySkillGuild(CPC* ch, CSkill* skill, int itemidx, bool& bApply);
#endif
#ifdef NEWYEAR_EVENT_2008
void ApplyNewYearSkill(CPC * ch, CPC * tch);
#endif // NEWYEAR_EVENT_2008
#endif
//
