#include "stdh.h"

#include "StatusEffect.h"
#include <Engine/Base/Timer.h>

/*
//skill.h에서 정의.
#define EST_ASSIST_POISON		0			// 독
#define EST_ASSIST_HOLD			1			// 홀드
#define EST_ASSIST_CONFUSION	2			// 혼란
#define EST_ASSIST_STONE		3			// 석화
#define EST_ASSIST_SILENT		4			// 봉인
#define EST_ASSIST_BLOOD		5			// 출혈
#define EST_ASSIST_BLIND		6			// 블라인드
#define EST_ASSIST_STURN		7			// 스턴
#define EST_ASSIST_SLEEP		8			// 슬립
#define EST_ASSIST_HP			9			// HP회복
#define EST_ASSIST_MP			10			// MP회복
#define EST_ASSIST_MOVESPD		11			// 이속 향상
#define EST_ASSIST_HP_CANCEL	12			// HP변화(0, MAX에서 취소)
#define EST_ASSIST_MP_CANCEL	13			// MP변화(0, MAX에서 취소)
#define EST_ASSIST_DIZZY		14			// 현기증
#define EST_ASSIST_INVISIBLE	15			// 인비저블
#define EST_ASSIST_SLOTH		16			// 슬로스 : 공속하락
#define EST_ASSIST_FEAR			17			// 공포 : 해당 NPC가 도망
#define EST_ASSIST_FAKEDEATH	18			// 죽은척
#define EST_ASSIST_PERFECTBODY	19			// 타이탄 퍼펙 바디
#define EST_ASSIST_FRENZY		20			// 타이탄 프렌지
#define EST_ASSIST_DAMAGELINK	21			// 데미지링크
#define EST_ASSIST_BERSERK		22			// 그랜드 몬스터의 분노- 공속
#define EST_ASSIST_DESPAIR		23			// 힐러 디세이어 마크
#define EST_ASSIST_MANA_SCREEN	24			// 소서러 마나 스크린
#define EST_ASSIST_BLESS		25			// 소서러 윈즈 블레싱
#define EST_ASSIST_SAFEGUARD	26			// 보호망(전투불가능)
#define EST_ASSIST_MANTLE		27			// 망토
#define EST_ASSIST_GUARD		28			// 경비병 소환
#define EST_ASSIST_CHARGEATC	29			// 장탄(attack)
#define EST_ASSIST_CHARGEMGC	30			// 장탄(magic)
#define EST_ASSIST_DISEASE		31			// 질병
#define EST_ASSIST_CURSE		32			// 저주
#define EST_ASSIST_CONFUSED		33			// 몬스터만 혼란에 걸리게 함
#define EST_ASSIST_TAMING		34			// 몬스터를 일정시간 아군으로 만듬
#define EST_ASSIST_FREEZE		35			// 결계(서로 공격 못함, 걸린 것은 이동 못함)
#define EST_ASSIST_INVERSE_DAMAGE	36		// 피격 당하면 데미지 만큼 HP 회복
#define EST_ASSIST_HP_DOT		37			// 지속 데미지 입음
#define EST_ASSIST_REBIRTH		38			// 부활
#define EST_ASSIST_DARKNESS_MODE	39		// 다크니스 모드
#define EST_ASSIST_AURA_DARKNESS	40			// 오오라 - 다크니스
#define EST_ASSIST_AURA_WEAKNESS	41			// 오오라 - 위크니스
#define EST_ASSIST_AURA_ILLUSION	42			// 오오라 - 일루젼
#define EST_ASSIST_COUNT		43
*/
const char	*CStatusEffect::m_szStatusEffectName[EST_ASSIST_COUNT][T_COUNT] = 
{
	{NULL, NULL, NULL}//"STATE_POISON"
	, {NULL, NULL, NULL}//"STATE_HOLD"
	, {NULL, NULL, NULL}//"STATE_CONFUSION"
	, {"STATE_STONE", "STATE_STONE", "STATE_STONE"}
	, {"STATE_SILENT", "STATE_SILENT", "STATE_SILENT"}
	, {"croits_skill02_hits", "croits_skill02_hits", NULL}//"STATE_BLOOD"
	, {"STATE_BLIND", NULL, NULL}
	, {"STATE_STUN", "STATE_STUN", "STATE_STUN"}
	, {"STATE_SLEEP","STATE_SLEEP","STATE_SLEEP"}//"STATE_SLEEP"
	, {NULL, NULL, NULL}//"STATE_HP"
	, {NULL, NULL, NULL}//"STATE_MP"
	, {"DK Normal State", "DK Normal State", NULL}//"STATE_MOVESPD"
	, {NULL, NULL, NULL}//"HP_CANCEL"
	, {NULL, NULL, NULL}//"MP_CANCEL"
	, {"STATE_DIZZY", "STATE_DIZZY", "STATE_DIZZY"}
	, {NULL, NULL, NULL}//EST_ASSIST_INVISIBLE
	, {"TGT_MISTY_CHAIN", "TGT_MISTY_CHAIN", "TGT_MISTY_CHAIN"}//EST_ASSIST_SLOTH
	, {NULL, NULL, NULL}//EST_ASSIST_FEAR
	, {NULL, NULL, NULL}//EST_ASSIST_FAKEDEATH
	, {"STATE_PERFECTBODY", "STATE_PERFECTBODY", "STATE_PERFECTBODY"}
	, {"STATE_FRENZY", "STATE_FRENZY", "STATE_FRENZY"}
	, {"STATE_DAMAGELINK", "STATE_DAMAGELINK", "STATE_DAMAGELINK"}
	, {NULL, NULL, NULL}//EST_ASSIST_BERSERK
	, {NULL, NULL, NULL}//EST_ASSIST_DESPAIR
	, {NULL, NULL, NULL}//EST_ASSIST_MANA_SCREEN
	, {NULL, NULL, NULL}//EST_ASSIST_BLESS
	, {NULL, NULL, NULL}//EST_ASSIST_SAFEGUARD
	, {NULL, NULL, NULL}//EST_ASSIST_MANTLE
	, {NULL, NULL, NULL}//EST_ASSIST_GUARD
	, {NULL, NULL, NULL}//EST_ASSIST_CHARGEATC
	, {NULL, NULL, NULL}//EST_ASSIST_CHARGEMGC
	, {NULL, NULL, NULL}//EST_ASSIST_DISEASE
	, {NULL, NULL, NULL}//EST_ASSIST_CURSE
	, {NULL, NULL, "STATE_DIZZY"}//EST_ASSIST_CONFUSED
	, {NULL, NULL, "STATE_RED"}//EST_ASSIST_TAMING
	, {"TGT_COPE_OF_LIGHT1", "TGT_COPE_OF_LIGHT1", "TGT_COPE_OF_LIGHT1"}//EST_ASSIST_FREEZE
	, {NULL, NULL, NULL}//EST_ASSIST_INVERSE_DAMAGE
	, {"TGT_PAIN_SHOT", "TGT_PAIN_SHOT", "TGT_PAIN_SHOT"}//EST_ASSIST_HP_DOT
	, {NULL, NULL, NULL}//EST_ASSIST_REBIRTH
	, {"STATE_STONE", "STATE_STONE", NULL}//EST_ASSIST_DARKNESS_MODE
	, {"OWN_AURA_OF_DARKNESS", "OWN_AURA_OF_DARKNESS", NULL}//EST_ASSIST_AURA_DARKNESS				// 오오라 - 다크니스
	, {"OWN_AURA_OF_ILLUSION", "OWN_AURA_OF_ILLUSION", NULL}//EST_ASSIST_AURA_WEAKNESS	41			// 오오라 - 위크니스
	, {"OWN_AURA_OF_WEAKNESS", "OWN_AURA_OF_WEAKNESS", NULL}//EST_ASSIST_AURA_ILLUSION	42			// 오오라 - 일루젼
	, {NULL, NULL, NULL}//EST_ASSIST_MERCENARY	43
	, {NULL, NULL, NULL}//EST_ASSIST_SOUL_TOTEM_BUFF 44
	, {NULL, NULL, NULL}//EST_ASSIST_SOUL_TOTEM_ATTK 45
	, {NULL, NULL, NULL}//EST_ASSIST_TRAP 46
	, {NULL, NULL, NULL}//EST_ASSIST_PARASITE 47
	, {NULL, NULL, NULL}//EST_ASSIST_SUICIDE  48
};

const char	*CStatusEffect::m_szNPCQuestMark[CQuestSystem::NQT_COUNT] =
{
	NULL,
	"QMARK_HAVE_QUEST",
	"QMARK_CAN_PRIZE",
};


CStatusEffect::CStatusEffect()
: m_slCurrentStatus( 0 )
, m_eType( T_NPC )
, m_eCurrentNPCQuestType( CQuestSystem::NQT_NONE )
{
	ASSERT(EST_ASSIST_COUNT == 51);//뭔가 바뀌게 있으면 m_szStatusEffectName의 값을 고쳐라-_-
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	for (int i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
}

CStatusEffect::~CStatusEffect()
{
	Reset();
}

CStatusEffect::CStatusEffect(const CStatusEffect &other)
{
	int		i;

	m_slCurrentStatus = other.m_slCurrentStatus;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	
	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
}

CStatusEffect &CStatusEffect::operator=(const CStatusEffect &other)
{
	int		i;

	m_slCurrentStatus = other.m_slCurrentStatus;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
	return *this;
}

void CStatusEffect::Reset()
{
	int		i;

	m_slCurrentStatus = 0;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	m_eCurrentNPCQuestType = CQuestSystem::NQT_NONE;

	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		if(m_pNPCQuestMark[i]
		&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[i]))
		{
			m_pNPCQuestMark[i]->Stop(0.5f);
		}

		m_pNPCQuestMark[i] = NULL;
	}
}

void CStatusEffect::Refresh(CTagManager *pTM, eRelation relation)
{
	int		i;

	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		if(m_pNPCQuestMark[i]
		&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[i]))
		{
			m_pNPCQuestMark[i]->Stop(0.5f);
		}

		m_pNPCQuestMark[i] = NULL;
	}

	if(pTM != NULL)
	{
		ChangeStatus(pTM, m_slCurrentStatus, relation);
		ChangeNPCQuestMark(pTM, m_eCurrentNPCQuestType);
	}
}

#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & (static_cast<SQUAD>((1L))<<(flagVal)) )

void CStatusEffect::ChangeStatus(CTagManager *pTM, SQUAD newStatus, eRelation relation)
{
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		if(m_szStatusEffectName[i][m_eType] == NULL) continue;
		if(i == EST_ASSIST_DAMAGELINK && relation != R_PARTY)  continue;
		
		SQUAD tmpStatus = IS_FLAG_ON(m_slCurrentStatus, i);
		SQUAD tmpNewStatus = IS_FLAG_ON(newStatus, i);

		//Off -> On, Create Effect
		if( !IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i))
		{
			ASSERT(m_pStatusEffectArray[i] == NULL);
			m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
		}
		//On -> Off, Destroy Effect
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && !IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] != NULL
			&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
			{
				m_pStatusEffectArray[i]->Stop(0.1f);
			}
			m_pStatusEffectArray[i] = NULL;
		}
		//On -> On, Effect Life Time Refresh
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] == NULL
				|| (
					m_pStatusEffectArray[i] != NULL
					&& !CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i])
				) )
			{
				m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
			}
		}
	}
	m_slCurrentStatus = newStatus;
}

BOOL CStatusEffect::IsState(INDEX state)
{
	if(IS_FLAG_ON(m_slCurrentStatus, state)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsSturn()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_STURN)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsConfuse()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_CONFUSION)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsHold()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_HOLD)) return TRUE;
	else return FALSE;
}

void CStatusEffect::ChangeNPCQuestMark(CTagManager *pTM, CQuestSystem::eNpcQuestType newStatus)
{
	if(m_pNPCQuestMark[m_eCurrentNPCQuestType] && CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[m_eCurrentNPCQuestType]))
	{
		m_pNPCQuestMark[m_eCurrentNPCQuestType]->Stop(0.f);
	}

	m_eCurrentNPCQuestType = newStatus;

	if(m_szNPCQuestMark[m_eCurrentNPCQuestType] != NULL)
	{
		m_pNPCQuestMark[m_eCurrentNPCQuestType] = StartEffectGroup(m_szNPCQuestMark[newStatus], pTM, _pTimer->GetLerpedCurrentTick() + 0.5f);
	}
}
