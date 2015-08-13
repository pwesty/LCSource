#ifndef __PTYPE_OLD_DO_SKILL_H__
#define __PTYPE_OLD_DO_SKILL_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct skillLearn : public pTypeBase
{
	int			npcIndex;		// memory index
	int			skillIndex;
};

struct skillReady : public pTypeBase
{
	int			skillIndex;

	char		cMoveChar;
	char		cYlayer;

	char		charType;
	char		targetType;

	int			charIndex;
	int			targetIndex;

	int			nDummySkillSpeed;
	float		fx;
	float		fz;
	float		fh;
	float		fr;
};

struct skillCancel : public pTypeBase
{
	char		charType;
	int			charIndex;
};

struct skillFire : public pTypeBase
{
	int			skillIndex;

	char		cMoveChar;
	char		charType;
	char		targetType;
	int			charIndex;
	int			targetIndex;

	int			nDummySkillSpeed;
	float		fx;
	float		fz;
	float		fh;
	float		fr;
	char		cYlayer;

	char		listCount;
	struct tag_list
	{
		char	mtargettype;
		int		mtargetindex;
	} list[0];
};

#ifdef HANARO_EVENT
struct skillHanaroEvent : public pTypeBase
{
	int			npcIndex;		// memory index
};
#endif

struct sealSkillLearn : public pTypeBase
{
	int			npcIndex;		// memory index
	int			skillIndex;
};

struct skillMidLevelSupport : public pTypeBase
{
};

struct skillUseEP : public pTypeBase
{
};
}

namespace ResponseClient
{
struct sealSkillLearn : public pTypeBase
{
	int			skillIndex;
	LONGLONG	exp;
};

struct skillList : public pTypeBase
{
	char		count;
	struct tag_list
	{
		int			index;
		char		level;
		int			sendTime;
		int			enable;
		LONGLONG	sealExp;
	} list[0];
};

struct skillCancelMsg : public pTypeBase
{
	int			charIndex;
	char		charType;
};

struct skillLearnMsg : public pTypeBase
{
	int			skillIndex;
	char		skillLevel_flag;
	char		skillLevel;
	LONGLONG	skillPoint;
};

struct skillReadyMsg : public pTypeBase
{
	char		charType;
	int			charIndex;
	int			skillIndex;
	char		targetType;
	int			targetIndex;
	int			skillSpeed;
};

struct skillCancel : public pTypeBase
{
	char		charType;
	int			charIndex;
};

struct skillLearnErrorMsg : public pTypeBase
{
	char		errorCode;
};

struct skillErrorMsg : public pTypeBase
{
	int			errorCode;
	int			skillIndex;
	int			extraInfo;
};

struct skillSupportMsg : public pTypeBase
{
	char		flag;
};

struct skillSoulCountMsg : public pTypeBase
{
	int			soulCount;
};

struct skillAutoUseMsg : public pTypeBase
{
	int			skillIndex;
};

struct skillFireMsg : public pTypeBase
{
	char		charType;
	int			charIndex;
	int			skillIndex;
	char		targetType;
	int			targetIndex;

	int			skillSpeed;
	char		cMoveChar;
	char		yLayer;
	float		x;
	float		z;
	float		h;
	float		r;

	short		listCount;
	struct tag_list
	{
		char	multiType;
		int		multiIndex;
	} list[0];
};

#ifndef _CLIENT_
inline void makeSealSkillLeranMsg(CNetMsg::SP& msg, CPC* ch, CSkill* pSkill)
{
	sealSkillLearn* packet = reinterpret_cast<sealSkillLearn*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_LEARN_SEAL;
	packet->skillIndex = pSkill->m_proto->m_index;
	packet->exp = ch->GetSealExp(pSkill->m_proto->m_index);
	msg->setSize(sizeof(sealSkillLearn));
}

inline void makeSkillListMsg(CNetMsg::SP& msg, CPC* ch)
{
	skillList* packet = reinterpret_cast<skillList*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_LIST;
	packet->count = 0;

	void* pos = ch->m_activeSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_activeSkillList.GetNext(pos);
		if (p == NULL)
			continue;

		int sendTime = 0;
		if (p->m_usetime > 0)
		{
			sendTime = int(gserver->m_nowseconds) - ((gserver->m_pulse - p->m_usetime) / 10);
		}

		packet->list[packet->count].index = p->m_proto->m_index;
		packet->list[packet->count].level = p->m_level;
		packet->list[packet->count].sendTime = sendTime;
		packet->list[packet->count].enable = p->m_enable;
		++packet->count;
	}

	pos = ch->m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_passiveSkillList.GetNext(pos);
		if (p == NULL)
			continue;

		packet->list[packet->count].index = p->m_proto->m_index;
		packet->list[packet->count].level =  p->m_level;
		++packet->count;
	}

	pos = ch->m_etcSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_etcSkillList.GetNext(pos);
		if (p == NULL)
			continue;

		packet->list[packet->count].index = p->m_proto->m_index;
		packet->list[packet->count].level =  p->m_level;
		++packet->count;
	}

	pos = ch->m_sealSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_sealSkillList.GetNext(pos);
		if (p == NULL)
			continue;

		packet->list[packet->count].index = p->m_proto->m_index;
		packet->list[packet->count].sealExp =  ch->GetSealExp(p->m_proto->m_index);
		++packet->count;
	}

	msg->setSize(sizeof(skillList) + (packet->count * sizeof(skillList::tag_list)));
}

inline void makeSkillCancelMsg(CNetMsg::SP& msg, CCharacter* ch)
{
	skillCancelMsg* packet = reinterpret_cast<skillCancelMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_CANCEL;
	packet->charType = ch->m_type;
	packet->charIndex = ch->m_index;
	msg->setSize(sizeof(skillCancelMsg));
}

inline void makeSkillLearnMsg(CNetMsg::SP& msg, CSkill* pSkill, CCharacter* ch)
{
	skillLearnMsg* packet = reinterpret_cast<skillLearnMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_LEARN;
	packet->skillIndex = pSkill->m_proto->m_index;
	packet->skillLevel_flag = (pSkill->m_level == 1) ? 1 : 0;
	packet->skillLevel = pSkill->m_level;
	packet->skillPoint = ch->m_skillPoint;
	msg->setSize(sizeof(skillLearnMsg));
}

inline void makeSkillReadyMsg(CNetMsg::SP& msg, CCharacter* ch, CSkill* pSkill, CCharacter* tch)
{
	// 스킬이 셀프, 파티 타입이면 tch를 ch로 사용
	switch (pSkill->m_proto->m_targetType)
	{
	case STT_SELF_ONE:
	case STT_SELF_RANGE:
	case STT_PARTY_ALL:
	case STT_GUILD_SELF_RANGE:
	case STT_GUILD_ALL:
		tch = ch;
		break;
	}

	skillReadyMsg* packet = reinterpret_cast<skillReadyMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_READY;
	packet->charType = ch->m_type;
	packet->charIndex = ch->m_index;
	packet->skillIndex = pSkill->m_proto->m_index;
	packet->skillSpeed = ch->m_skillSpeed;
	if (tch == NULL)
	{
		packet->targetType = -1;
		packet->targetIndex = -1;
	}
	else
	{
		packet->targetType = tch->m_type;
		packet->targetIndex = tch->m_index;
	}
	msg->setSize(sizeof(skillReadyMsg));
}

inline void makeSkillLearnErrorMsg(CNetMsg::SP& msg, int errorCode)
{
	skillLearnErrorMsg* packet = reinterpret_cast<skillLearnErrorMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_LEARN_ERROR;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(skillLearnErrorMsg));
}

inline void makeSkillErrorMsg(CNetMsg::SP& msg, int errorCode, int skillIndex, int extraInfo)
{
	skillErrorMsg* packet = reinterpret_cast<skillErrorMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_ERROR;
	packet->errorCode = errorCode;
	packet->skillIndex = skillIndex;
	packet->extraInfo = extraInfo;
	msg->setSize(sizeof(skillErrorMsg));
}

inline void makeSkillSupportMsg(CNetMsg::SP& msg, int subType, int flag)
{
	skillSupportMsg* packet = reinterpret_cast<skillSupportMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = subType;
	packet->flag = flag;
	msg->setSize(sizeof(skillSupportMsg));
}

inline void makeSkillSoulCountMsg(CNetMsg::SP& msg, int soulCount)
{
	skillSoulCountMsg* packet = reinterpret_cast<skillSoulCountMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_SOUL;
	packet->soulCount = soulCount;
	msg->setSize(sizeof(skillSoulCountMsg));
}

inline void makeSkillAutoUseMSg(CNetMsg::SP& msg, int skillIndex)
{
	skillAutoUseMsg* packet = reinterpret_cast<skillAutoUseMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_AUTO_USE;
	packet->skillIndex = skillIndex;
	msg->setSize(sizeof(skillAutoUseMsg));
}

inline void makeSkillFireMsg(CNetMsg::SP& msg, CCharacter* ch, CSkill* pSkill, CCharacter* tch,
							 int count,  MultiTarget *pTarget, int cMoveChar, float x, float z, float h, float r, int yLayer)
{
	// 스킬이 셀프, 파티 타입이면 tch를 ch로 사용
	switch (pSkill->m_proto->m_targetType)
	{
	case STT_SELF_ONE:
	case STT_SELF_RANGE:
	case STT_PARTY_ALL:
	case STT_GUILD_SELF_RANGE:
	case STT_GUILD_ALL:
		tch = ch;
		break;
	}

	skillFireMsg* packet = reinterpret_cast<skillFireMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_FIRE;

	packet->charType = ch->m_type;
	packet->charIndex = ch->m_index;
	packet->skillIndex = pSkill->m_proto->m_index;

	if (tch == NULL)
	{
		packet->targetType = MSG_CHAR_UNKNOWN;
		packet->targetIndex = -1;
	}
	else
	{
		packet->targetType = tch->m_type;
		packet->targetIndex = tch->m_index;
	}

	packet->skillSpeed = ch->m_skillSpeed;
	packet->cMoveChar = cMoveChar;
	packet->x = x;
	packet->z = z;
	packet->h = h;
	packet->r = r;
	packet->yLayer = yLayer;

	packet->listCount = count;
	for (int i = 0; i < count; ++i)
	{
		packet->list[i].multiType = pTarget[i].mtargettype;
		packet->list[i].multiIndex = pTarget[i].mtargetindex;
	}

	msg->setSize(sizeof(skillFireMsg) + (count * sizeof(skillFireMsg::tag_list)));
}

#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
