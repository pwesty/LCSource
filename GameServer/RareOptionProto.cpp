#include "stdhdrs.h"

#include "Server.h"
#include "RareOptionProto.h"
#include "../ShareLib/DBCmd.h"


///////////////////
// CRareOptionProto
CRareOptionProto::CRareOptionProto()
{
	m_nIndex = 0;
	m_nDamageUp = 0;
	m_nDefenseUp = 0;
	m_nMagicUp = 0;
	m_nResistUp = 0;
	memset(m_rod, 0, sizeof(m_rod));
}

CRareOptionProto::~CRareOptionProto()
{
}

void CRareOptionProto::InitData(	int nIndex,
									int nDamageUp,
									int nDefenseUp,
									int nMagicUp,
									int nResistUp,
									int nGrade,
									int optiontype[MAX_RARE_OPTION_SETTING],
									int optionlevel[MAX_RARE_OPTION_SETTING],
									int optionval[MAX_RARE_OPTION_SETTING],
									int optionprob[MAX_RARE_OPTION_SETTING])
{
	m_nIndex		= nIndex;
	m_nDamageUp		= nDamageUp;
	m_nDefenseUp	= nDefenseUp;
	m_nMagicUp		= nMagicUp;
	m_nResistUp		= nResistUp;
	m_nGrade		= nGrade;

	int i;
	for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
	{
		m_rod[i].optiontype		= optiontype[i];
		m_rod[i].optionlevel	= optionlevel[i];
		m_rod[i].optionval		= optionval[i];
		m_rod[i].optionprob		= optionprob[i];
		m_rod[i].rarebit		= (1 << i);
	}
}

///////////////////////
// CRareOptionProtoList
CRareOptionProtoList::CRareOptionProtoList()
{
	m_listRareOption = NULL;
	m_nCount = 0;
}

CRareOptionProtoList::~CRareOptionProtoList()
{
	while (m_listRareOption)
	{
		delete [] m_listRareOption;
		m_listRareOption = NULL;
	}
	m_nCount = 0;
}

const CRareOptionProto* CRareOptionProtoList::Find(int nIndex)
{
	map_t::iterator it = map_.find(nIndex);
	return (it != map_.end()) ? it->second : NULL;
}

bool CRareOptionProtoList::Load()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_rareoption WHERE a_grade!=-1 ORDER BY a_index");
	if (!cmd.Open())
		return false;
	m_listRareOption = new CRareOptionProto[cmd.m_nrecords];

	int		a_index;
	int		a_attack;
	int		a_grade;
	int		a_defense;
	int		a_magic;
	int		a_resist;
	int		a_option_index[MAX_RARE_OPTION_SETTING];
	int		a_option_level[MAX_RARE_OPTION_SETTING];
	int		a_option_prob[MAX_RARE_OPTION_SETTING];
	int		optionvalues[MAX_RARE_OPTION_SETTING];

	m_nCount = 0;
	while (cmd.MoveNext())
	{
		if (!cmd.GetRec("a_index",			a_index))			return false;
		if (!cmd.GetRec("a_attack",			a_attack))			return false;
		if (!cmd.GetRec("a_defense",		a_defense))			return false;
		if (!cmd.GetRec("a_magic",			a_magic))			return false;
		if (!cmd.GetRec("a_resist",			a_resist))			return false;
		if (!cmd.GetRec("a_grade",			a_grade))			return false;
		if (!cmd.GetRec("a_option_index0",	a_option_index[0]))	return false;
		if (!cmd.GetRec("a_option_level0",	a_option_level[0]))	return false;
		if (!cmd.GetRec("a_option_prob0",	a_option_prob[0]))	return false;
		if (!cmd.GetRec("a_option_index1",	a_option_index[1]))	return false;
		if (!cmd.GetRec("a_option_level1",	a_option_level[1]))	return false;
		if (!cmd.GetRec("a_option_prob1",	a_option_prob[1]))	return false;
		if (!cmd.GetRec("a_option_index2",	a_option_index[2]))	return false;
		if (!cmd.GetRec("a_option_level2",	a_option_level[2]))	return false;
		if (!cmd.GetRec("a_option_prob2",	a_option_prob[2]))	return false;
		if (!cmd.GetRec("a_option_index3",	a_option_index[3]))	return false;
		if (!cmd.GetRec("a_option_level3",	a_option_level[3]))	return false;
		if (!cmd.GetRec("a_option_prob3",	a_option_prob[3]))	return false;
		if (!cmd.GetRec("a_option_index4",	a_option_index[4]))	return false;
		if (!cmd.GetRec("a_option_level4",	a_option_level[4]))	return false;
		if (!cmd.GetRec("a_option_prob4",	a_option_prob[4]))	return false;
		if (!cmd.GetRec("a_option_index5",	a_option_index[5]))	return false;
		if (!cmd.GetRec("a_option_level5",	a_option_level[5]))	return false;
		if (!cmd.GetRec("a_option_prob5",	a_option_prob[5]))	return false;
		if (!cmd.GetRec("a_option_index6",	a_option_index[6]))	return false;
		if (!cmd.GetRec("a_option_level6",	a_option_level[6]))	return false;
		if (!cmd.GetRec("a_option_prob6",	a_option_prob[6]))	return false;
		if (!cmd.GetRec("a_option_index7",	a_option_index[7]))	return false;
		if (!cmd.GetRec("a_option_level7",	a_option_level[7]))	return false;
		if (!cmd.GetRec("a_option_prob7",	a_option_prob[7]))	return false;
		if (!cmd.GetRec("a_option_index8",	a_option_index[8]))	return false;
		if (!cmd.GetRec("a_option_level8",	a_option_level[8]))	return false;
		if (!cmd.GetRec("a_option_prob8",	a_option_prob[8]))	return false;
		if (!cmd.GetRec("a_option_index9",	a_option_index[9]))	return false;
		if (!cmd.GetRec("a_option_level9",	a_option_level[9]))	return false;
		if (!cmd.GetRec("a_option_prob9",	a_option_prob[9]))	return false;

		int i;
		for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
		{
			if (a_option_level[i] == 0)
				optionvalues[i] = 0;
			else
			{
				COptionProto* pOptionProto = gserver->m_optionProtoList.FindProto(a_option_index[i]);
				if (pOptionProto == NULL)
					return false;
				optionvalues[i] = pOptionProto->m_levelValue[a_option_level[i] - 1];
			}
		}

		m_listRareOption[m_nCount].InitData(
			a_index,
			a_attack,
			a_defense,
			a_magic,
			a_resist,
			a_grade,
			a_option_index,
			a_option_level,
			optionvalues,
			a_option_prob);

		map_.insert(map_t::value_type(m_listRareOption[m_nCount].GetIndex(), &m_listRareOption[m_nCount]));

		m_nCount++;
	}

	return true;
}

