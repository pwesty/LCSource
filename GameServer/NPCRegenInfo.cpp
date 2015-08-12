#include "stdhdrs.h"

#include "NPCRegenInfo.h"
#include "Zone.h"
#include "Area.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

///////////////////////
// CNPCRegenInfo member

CNPCRegenInfo::CNPCRegenInfo()
	: m_index(-1)
	, m_npcIdx(-1)
	, m_zoneNo(-1)
	, m_regenX(0.0f)
	, m_regenY(0)
	, m_regenZ(0.0f)
	, m_regenR(0.0f)
	, m_regenSec(0)
	, m_totalNum(0)
	, m_bAlive(false)
	, m_lastDieTime(0)
	, m_numRegen(0)
	, m_paramHP(0)
	, m_bRaidMoveRandom(false)
	, m_bRegen(true)
	, m_npcProto(NULL)
{
}

CNPCRegenInfo::~CNPCRegenInfo()
{
}

void CNPCRegenInfo::clean()
{
	m_index = -1;
	m_npcIdx = -1;
	m_zoneNo = -1;
	m_regenSec = 0;
	m_lastDieTime = 0;
	m_regenX = 0.0f;
	m_regenY = 0;
	m_regenZ = 0.0f;
	m_regenR = 0.0f;
	m_regenSec = 0;
	m_bAlive = false;
	m_numRegen = 0;
	m_totalNum = 0;

	m_paramHP = 0;

	m_bRaidMoveRandom = false;

	m_bRegen = true;			// npc 리제 여부 결정

	m_npcProto = NULL;
}

///////////////////////
// CNPCRegenList member

CNPCRegenList::CNPCRegenList()
{
	m_type = -1;
	m_infoList = NULL;
	m_nCount = 0;
	m_wPos = 0;
}

CNPCRegenList::~CNPCRegenList()
{
	if (m_infoList)
		delete [] m_infoList;
	m_infoList = NULL;
	m_nCount = 0;
}

void CNPCRegenList::SetCount(int n)
{
	if (m_infoList)
		delete [] m_infoList;
	m_infoList = NULL;
	m_nCount = n;
	m_wPos = 0;
	if (n)
		m_infoList = new CNPCRegenInfo[n];
}

void CNPCRegenList::AddRegenInfo(int index, int npc_index, int regenSec, float regenX, int yLayer, float regenZ, float regenR, int totalNum, int zoneNo, bool bRegen, CNPCProto* pNpcProto)
{
	if (m_wPos >= m_nCount)
		return ;

	m_infoList[m_wPos].m_index = index;
	m_infoList[m_wPos].m_npcIdx = npc_index;
	m_infoList[m_wPos].m_zoneNo = zoneNo;
	m_infoList[m_wPos].m_regenSec = regenSec;
	m_infoList[m_wPos].m_regenX = regenX;
	m_infoList[m_wPos].m_regenY = yLayer;
	m_infoList[m_wPos].m_regenZ = regenZ;
	m_infoList[m_wPos].m_regenR = regenR;
	m_infoList[m_wPos].m_totalNum = totalNum;

	m_infoList[m_wPos].m_bAlive = false;
	m_infoList[m_wPos].m_lastDieTime = 0;
	m_infoList[m_wPos].m_bRegen = bRegen;

	if (pNpcProto)
	{
		m_infoList[m_wPos].m_npcProto = pNpcProto;
	}
	else
	{
		m_infoList[m_wPos].m_npcProto = gserver->m_npcProtoList.FindProto(npc_index);
		if (m_infoList[m_wPos].m_npcProto == NULL)
		{
			LOG_ERROR("Not found Npc[%d] in Npc_proto", npc_index);
			return;
		}
	}

	m_wPos++;
}

// void CNPCRegenList::AddRegenInfo(CNPCRegenInfo* info)
// {
// 	if (m_wPos >= m_nCount)
// 		return ;
// 
// 	memcpy(m_infoList + m_wPos, info, sizeof(CNPCRegenInfo));
// 	m_wPos++;
// }

void CNPCRegenList::Init()
{
	if (m_infoList)
		delete [] m_infoList;
	m_infoList = NULL;
	m_nCount = 0;

	m_type = -1;
	m_infoList = NULL;
	m_nCount = 0;
	m_wPos = 0;
}

bool CNPCRegenList::SetRegened(CArea* pArea, int idx, int zoneNo, float x, int y, float z)
{
	int i, j;
	if( !pArea ) return false;

	if( !pArea->m_regenList ) return false;

	bool find = false;

	for(i = 0; i < m_nCount; i++)
	{
		if( m_infoList[i].m_zoneNo == zoneNo )
		{
			if ( m_infoList[i].m_npcIdx == idx )
			{
				switch(zoneNo)
				{
				case ZONE_SINGLE_DUNGEON2:
					if( this->Find(idx, zoneNo) )
						find = true;
					break;
				case ZONE_SINGLE_DUNGEON3:
				case ZONE_SINGLE_DUNGEON1:
				case ZONE_SINGLE_DUNGEON4:
				case ZONE_SINGLE_DUNGEON_TUTORIAL:
					if( this->Find(idx, zoneNo, true, x, y, z) )
						find = true;
					break;
				default:
					break;
				}
				if( find )
				{
					for(j = 0; j < m_nCount; j++)
					{
						if( m_infoList[i].m_index == pArea->m_regenList[j] )
						{
							pArea->m_regenList[j] = -1;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool CNPCRegenList::Find(int idx, int zoneNo, bool pdZone, float x, int y, float z)
{
	for(int i = 0; i < m_nCount; i++)
	{
		if ( m_infoList[i].m_zoneNo == zoneNo )
		{
			if( !pdZone )
			{
				if ( m_infoList[i].m_npcIdx == idx)
					return true;
			}
			else
			{
				bool bCurrectX = false;
				bool bCurrectY = true;
				bool bCurrectZ = false;

				if( (ceil(m_infoList[i].m_regenX) > ceil(x-10)) && (ceil(m_infoList[i].m_regenX) < ceil(x+10)))
					bCurrectX = true;
				// 우선 Y는 검사 안함 : bw 060513
				//if( (ceil(m_infoList[i].m_regenY) > ceil(y-10)) && (ceil(m_infoList[i].m_regenY) < ceil(y+10)) )
				//	bCurrectY = true;
				if( (ceil(m_infoList[i].m_regenZ) > ceil(z-10)) && (ceil(m_infoList[i].m_regenZ) < ceil(z+10)) )
					bCurrectZ = true;

				if ( m_infoList[i].m_npcIdx == idx && bCurrectX && bCurrectY && bCurrectZ )
					return true;
			}
		}
	}

	return false;
}

CNPCRegenInfo * CNPCRegenList::GetNpcRegenInfo(int idx)
{
	if (idx >= m_wPos)
	{
		return NULL;
	}

	return &m_infoList[idx];
}

bool CNPCRegenList::DelRegenInfo(CNPCRegenInfo * p)
{
	for(int i = 0; i < m_wPos; i++)
	{
		if(p == &m_infoList[i])
		{
			if(i+1 == m_wPos)
			{
				// 끝부분
				p->clean();
			}
			else
			{
				memcpy(&m_infoList[i], &m_infoList[i+1], sizeof(CNPCRegenInfo)*(m_wPos-(i+1)));
			}

			m_wPos--;

			return true;
		}
	}

	return false;
}
//