#ifndef _MONSTER_MERCENARY_SYSTME_HEADER_
#define _MONSTER_MERCENARY_SYSTME_HEADER_

#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
typedef struct _tagMercenaryNpcInfo
{
	int		index;
	float	walkSpeed;
	float	runSpeed;
	float	size;
	_tagMercenaryNpcInfo()
	{
		index		= -1;
		walkSpeed	= 0.0f;
		runSpeed	= 0.0f;
		size		= 0.0f;
	}
} MercenaryNpcInfo;
#endif

typedef struct _tagMonsterMercenaryInfo
{
	int nClassIdx;
	int nProb;
	int	nAttDef;
	int	nHpMp;
	int	nHit;
	int nDodge;
	int nBonusMini;
	int nBonusMax;
	int nNpcCount;
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
	MercenaryNpcInfo * npcInfo;
#else
	int * npcList;
#endif
	_tagMonsterMercenaryInfo()
	{
		nClassIdx	= 0;
		nProb		= 0;
		nAttDef		= 0;
		nHpMp		= 0;
		nHit		= 0;
		nDodge		= 0;
		nBonusMini	= 0;
		nBonusMax	= 0;
		nNpcCount	= 0;
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
		npcInfo		= NULL;
#else
		npcList		= NULL;
#endif
	}
} MonsterMercenaryInfo;

typedef struct _tagMonsterMercenaryData
{
	int nClassIdx;
	int	nAttDef;
	int	nHpMp;
	int	nHit;
	int nDodge;
	int nBonusAtt;
	int nBonusDef;
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
	float	nWalkSpeed;
	float	nRunSpeed;
	float	nSize;
#endif
	_tagMonsterMercenaryData()
	{
		nClassIdx	= 0;
		nAttDef		= 0;
		nHpMp		= 0;
		nHit		= 0;
		nDodge		= 0;
		nBonusAtt	= 0;
		nBonusDef	= 0;
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
		nWalkSpeed	= 0.0f;
		nRunSpeed	= 0.0f;
		nSize		= 0.0f;
#endif
	}
} MonsterMercenaryData;

class CMonsterMercenary
{
public:

	CMonsterMercenary();
	~CMonsterMercenary();

	int m_count;
	void Init();
	void ReleaseAll();
	bool LoadData();
	int GetMercenaryIndex(int classIdx);
	MonsterMercenaryInfo * GetClassInfo();
	MonsterMercenaryInfo * GetClassInfo(int classIdx);
	MonsterMercenaryData * CreateClassData(int classIdx, int npcIdx = -1);

#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
	bool SetMercenaryData( CItem * pItem, int classIdx = -1, int npcIndex = -1 );
#endif

protected:
	MonsterMercenaryInfo * m_Data;
};
#endif // _MONSTER_MERCENARY_SYSTME_HEADER_
//
