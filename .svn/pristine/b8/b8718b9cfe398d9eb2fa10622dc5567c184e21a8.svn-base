#ifndef _DROPITEM_SYSTEM_HEADER__
#define _DROPITEM_SYSTEM_HEADER__

#include "mysql.h"

class CDropItemData
{
public:

	CDropItemData(int _index, int _plus, int _flag, LONGLONG _count, int _prob, int _jobFlag)
		: m_index(_index)
		, m_plus(_plus)
		, m_flag(_flag)
		, m_count(_count)
		, m_prob(_prob)
		, m_jobFlag(_jobFlag)
	{	}

	~ CDropItemData()
	{	}

	const int getIndex()
	{
		return m_index;
	}
	const int getPlus()
	{
		return m_plus;
	}
	const int getFlag()
	{
		return m_flag;
	}
	const LONGLONG getCount()
	{
		return m_count;
	}
	const int getProb()
	{
		return m_prob;
	}
	const int getJobFlag()
	{
		return m_jobFlag;
	}

private:
	int m_index;
	int m_plus;
	int m_flag;
	LONGLONG m_count;
	int m_prob;
	int m_jobFlag;
};

class CProbByLevelSection
{
public:
	CProbByLevelSection(int _start, int _end, int _prob) : m_start(_start), m_end(_end), m_prob(_prob)
	{}

	~CProbByLevelSection() {}

	const int getStart()
	{
		return m_start;
	}
	const int getEnd()
	{
		return m_end;
	}
	const int getProb()
	{
		return m_prob;
	}
private:
	int m_start;
	int m_end;
	int m_prob;
};

class CDropInfo
{
public:
	CDropInfo(int _index, int _dropType, int _levelMini, int _levelMaxi, int _prob, int _probPerLevel, int _npcTypeParty, int _npcTypeBoss)
		: m_index(_index)
		, m_dropType(_dropType)
		, m_levelMini(_levelMini)
		, m_levelMaxi(_levelMaxi)
		, m_prob(_prob)
		, m_probPerLevel(_probPerLevel)
		, m_npcTypeParty(_npcTypeParty)
		, m_npcTypeBoss(_npcTypeBoss)
	{	}

	~CDropInfo()
	{
		m_itemList.clear();
		m_npcList.clear();
		m_levelSectionList.clear();
	}

	const int getNpcCount()
	{
		return (int)m_npcList.size();
	}
	void addNpc(int _npcDBIndex)
	{
		m_npcList.push_back(_npcDBIndex);
	}
	const int getNpcIndex(int _pos)
	{
		if( getNpcCount() == 0 || _pos >= getNpcCount() || _pos < 0 )
			return -1;
		return m_npcList[_pos];
	}
	const int getItemCount()
	{
		return (int)m_itemList.size();
	}
	void addItem(CDropItemData _dropItemData)
	{
		m_itemList.push_back(_dropItemData);
	}

	CDropItemData * getItemData(int _pos)
	{
		if( getItemCount() == 0 || _pos >= getItemCount() || _pos < 0 )
			return NULL;
		return &m_itemList[_pos];
	}

	const int getLevelSectionCount()
	{
		return (int)m_levelSectionList.size();
	}
	void addLevelSection(CProbByLevelSection _levelSection)
	{
		m_levelSectionList.push_back(_levelSection);
	}
	CProbByLevelSection * getLevelSection(int _pos)
	{
		if( getLevelSectionCount() == 0 || _pos >= getLevelSectionCount() || _pos < 0 )
			return NULL;
		return &m_levelSectionList[_pos];
	}

	const int getIndex()
	{
		return m_index;
	}
	const int getDropType()
	{
		return m_dropType;
	}
	const int getLevelMini()
	{
		return m_levelMini;
	}
	const int getLevelMaxi()
	{
		return m_levelMaxi;
	}
	const int getProb()
	{
		return m_prob;
	}
	const int getProbPerLevel()
	{
		return m_probPerLevel;
	}
	const int getNpcTypeParty()
	{
		return m_npcTypeParty;
	}
	const int getNpcTypeBoss()
	{
		return m_npcTypeBoss;
	}

protected:

	int	m_index;
	int m_dropType;
	int m_levelMini;
	int m_levelMaxi;
	int m_prob;
	int m_probPerLevel;
	int m_npcTypeParty;
	int m_npcTypeBoss;

	std::vector<int>					m_npcList;
	std::vector<CDropItemData>			m_itemList;
	std::vector<CProbByLevelSection>	m_levelSectionList;
};

class CDropManager
{
public:
	CDropManager() {	}

	~CDropManager()
	{
		m_dataList.clear();
	}

	bool loadDropData(MYSQL * _dataDB);
	bool doDrop(CPC* _pPC, CNPC* _pNPC, const int _dropIndex, const int _prob = -1, const int _pos = -1, bool _bPreferenceIndex= true );
	int getDropItemListCount(const int _dropIndex);

private:
	CDropInfo * getDropInfo( const int _dropIndex);

	bool checkCondition(CPC* _pPC, CNPC* _pNPC, CDropInfo* _pDrop);
	bool checkNpc(CDropInfo* _pDrop, const int _npcDBIndex);
	bool checkLevelLimit(CDropInfo* _pDrop, const int _charlevel, const int _npcLevel);
	bool checkDropProb(CNPC* _pNPC, CDropInfo* _pDrop);
	int  getDropProb(CNPC* _pNPC, CDropInfo* _pDrop);
	int  getProbByLevelSection( CDropInfo* _pDrop, const int _npcLevel);

	bool doDropRandom(CPC* _pPC, CNPC* _pNPC,CDropInfo* _pDrop, bool _bPreferenceIndex);
	bool doDropOnceRandom(CPC* _pPC, CNPC* _pNPC,CDropInfo* _pDrop, const int _prob, bool _bPreferenceIndex);
	bool doDropOnceSelect(CPC* _pPC, CNPC* _pNPC,CDropInfo* _pDrop, const int _pos, bool _bPreferenceIndex);

	bool checkProbPerLevel( const int _prob, const int _levelDiff, const int _probPerLevel);
	int  getProbPerLevel(const int _levelDiff, const int _probPerLevel);
	bool checkJob( const int _jobFlag, const int _charJob );

	bool dropItem(CPC* _pPC, CNPC* _pNPC, CDropItemData * _dropItem, bool _bPreferenceIndex);

	int getRandom()
	{
		return GetRandom(1,10000);
	}

private:

	std::map<int, CDropInfo> m_dataList;
};

#endif // _DROPITEM_SYSTEM_HEADER__
//
