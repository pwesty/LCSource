#ifndef __CSTATUSEFFECT_H__
#define __CSTATUSEFFECT_H__

#include <Engine/Base/Types.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Entities/Skill.h>

#include <vector>

class CTagManager;

class ENGINE_API CStatusEffect
{
public:
	enum eType
	{
		T_PLAYER	= 0,
		T_CHARACTER	= 1,
		T_NPC		= 2,
		T_COUNT		= 3,
	};
	enum eRelation
	{
		R_NONE = 0,
		R_PARTY,
		//R_GUILD,//길드를 넣으려면 flag방식으로 가야함.
		R_ENEMY,
		R_COUNT,
	};
public:
	CStatusEffect();
	~CStatusEffect();

	inline void SetType(eType type)	{ m_eType = type;	}
	inline eType GetType()			{ return m_eType;	}

	void ChangeStatus(CTagManager *pTM, SQUAD newStatus, eRelation relation);
	void Reset();
	void Refresh(CTagManager *pTM, eRelation relation);

	inline BOOL		IsStatusChanged(SQUAD newStatus)	{ return (m_slCurrentStatus != newStatus);	}
	inline SQUAD	GetStatus()							{ return m_slCurrentStatus;					}

	BOOL IsSturn();
	BOOL IsConfuse();
	BOOL IsHold();

	BOOL IsState(INDEX state);
	
	//Quest mark 관련(끼워넣기)
	void ChangeNPCQuestMark(CTagManager *pTM, CQuestSystem::eNpcQuestType newStatus);

	CStatusEffect(const CStatusEffect &);
	CStatusEffect &operator=(const CStatusEffect &);
protected:
	SQUAD				m_slCurrentStatus;
	eType				m_eType;
	CEffectGroup		*m_pStatusEffectArray[EST_ASSIST_COUNT];
	CQuestSystem::eNpcQuestType m_eCurrentNPCQuestType;
	CEffectGroup		*m_pNPCQuestMark[CQuestSystem::NQT_COUNT];
	static const char	*m_szStatusEffectName[EST_ASSIST_COUNT][T_COUNT];
	static const char	*m_szNPCQuestMark[CQuestSystem::NQT_COUNT];
};

#endif //__CSTATUSEFFECT_H__