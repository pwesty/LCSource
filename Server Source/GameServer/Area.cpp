#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Area.h"
#include "CmdMsg.h"
#include "Zone.h"
#include "Server.h"
#include "Env.h"
#include "../ShareLib/DBCmd.h"
#include "WarCastle.h"
#include "Battle.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_old_do_action.h"
#include "../ShareLib/packetType/ptype_old_do_move.h"

std::set<CNPC*> CArea::m_lifetime_set;

CArea::CArea()
	: m_zone(NULL)
{
	m_nDelayOpenGate = 0;
	memset(m_AkanPCList, 0, sizeof(CPC*)*8); // 8명. 인원제한이 풀릴 때 맞춰줘야 함.
	m_pBossAppearNPC = NULL;
	m_nPauseIndex = 0;
	m_nPauseMobActivity = 0;
	m_nCountDown = -1;
	m_nObjectIndex = -1;
	m_nRaidRoomNo = -1;
	m_RaidDifficulty = -1;
	m_index = -1;
	m_size[0] = m_size[1] = 0;
	m_ycnt = 0;
	m_env = NULL;
	m_cell = NULL;
	m_npcList = NULL;
	m_nCountNPC = 0;
	m_nJunkItems = 0;
	m_nMakeNPC = 0;
	m_guildIndex = -1;
	m_bBoss = false;
	m_regenList = NULL;

	m_monsterCombo = NULL;

	m_nNPC_Killed_NPC = 0;

	m_bUseTriggerEvent = false;
	m_CTriggerList.InitTriggerList();
	m_mobAISeq = -1;
	m_bFreeze = false;
	m_freezePulse = 0;
	m_inCount = 0;
}

CArea::~CArea()
{
	if (m_env)
	{
		int i, y;
		for (y = 0; y < m_ycnt; y++)
		{
			for (i = 0; i < m_size[0]; i++)
				delete[] m_env[y][i];
			delete [] m_env[y];
		}
		delete[] m_env;
	}

	if (m_cell)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_cell[i];
		delete[] m_cell;
	}

	if (m_npcList)
	{
		CNPC* p;
		CNPC* pn = m_npcList;
		while ((p = pn))
		{
			pn = p->m_pNPCNext;
			p->DeleteNPC();
			p = NULL;
		}
	}

	m_zone = NULL;
	m_size[0] = m_size[1] = 0;
	m_ycnt = 0;
	m_env = NULL;
	m_cell = NULL;

	vec_raidnpcrregenlist_t::iterator it = m_raidNPCRegenList.begin();
	vec_raidnpcrregenlist_t::iterator endit = m_raidNPCRegenList.end();
	for (; it != endit; ++it)
	{
		delete (*it);
	}
	m_raidNPCRegenList.clear();

	if(m_monsterCombo)
	{
		delete m_monsterCombo;
		m_monsterCombo = NULL;
	}

	m_nNPC_Killed_NPC = 0;

	m_NpcPortal_NpcList.clear();
	m_NpcPortal_MobList.clear();
}

bool CArea::IsNpcPortalList_Npc(int index)
{
	if( !m_zone )
		return false;

	int zoneNum = m_zone->m_index;
	CZone* pZone = gserver->FindZone(zoneNum);
	if (pZone == NULL)
		return false;

	if( pZone->m_area->m_NpcPortal_NpcList.empty() )
		return false;

	set_int_t::iterator it = pZone->m_area->m_NpcPortal_NpcList.find(index);
	return (it != pZone->m_area->m_NpcPortal_NpcList.end()) ? true : false;
}

bool CArea::IsNpcPortalList_Mob(int index)
{
	if( !m_zone )
		return false;

	int zoneNum = m_zone->m_index;
	CZone* pZone = gserver->FindZone(zoneNum);
	if (pZone == NULL)
		return false;

	set_int_t::iterator it = pZone->m_area->m_NpcPortal_MobList.find(index);
	return (it != pZone->m_area->m_NpcPortal_MobList.end()) ? true : false;
}

void CArea::Init(CZone* z, int index, int w, int h, int ycnt)
{
	int i, j;

	if(!z)
		return;

	m_bEnable = false;
	m_zone = z;
	m_index = index;

	m_size[0] = (int)(w / CELL_SIZE);
	m_size[1] = (int)(h / CELL_SIZE);
	m_ycnt = ycnt;

	m_env = new CEnv**[m_ycnt];
	for (i = 0; i < m_ycnt; i++)
	{
		m_env[i] = new CEnv*[m_size[0]];
		for (j = 0; j < m_size[0]; j++)
			m_env[i][j] = new CEnv[m_size[1]];
	}

	// 싱글던전의 경우 한 Area에 Cell 하나
	if (z->IsGuildRoom() || z->IsPersonalDungeon() || z->IsComboZone() || z->IsExtremeCube())
	{
		m_size[0] = 1;
		m_size[1] = 1;

		m_cell = new CCell*[m_size[0]];
		m_cell[0] = new CCell[m_size[1]];
	}
	else
	{
		m_cell = new CCell*[m_size[0]];
		for (i = 0; i < m_size[0]; i++)
			m_cell[i] = new CCell[m_size[1]];
	}

	if(z->IsComboZone())
	{
		m_monsterCombo = new CMonsterCombo;
		m_monsterCombo->SetArea(this);
	}

	// zone index로 트리거 세트 타입을 알아 낸 뒤
	int triggerSetType = gserver->GetTriggerSetTypeToZoneIndex(z->m_index);

	// 트리거 기본 DB정보
	if( m_CTriggerList.LoadTriggerInfo(triggerSetType) )
	{
		// 불러오기 성공하면 트리거 사용하는 Area로 표시
		m_bUseTriggerEvent = true;

		// 표시 후 Area 주소 정보를 저장한다.
		m_CTriggerList.SetArea(this);

		GAMELOG << init("TRIGGER_SYSTEM : AREA LOADING SUCCESS")
				<< z->m_index << delim
				<< triggerSetType << delim
				<< end;
	}

	if(z->m_index == ZONE_AKAN_TEMPLE)
	{
		if(!m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].LoadRaidInfo(ZONE_AKAN_TEMPLE, DUNGEON_DIFFICULTY_NORMAL))
			GAMELOG << init("FAILED LOADING RAID INFO(DIFFICULTY NORMAL:36)") << end;
		if(!m_RaidInfoList[DUNGEON_DIFFICULTY_HARD].LoadRaidInfo(ZONE_AKAN_TEMPLE, DUNGEON_DIFFICULTY_HARD))
			GAMELOG << init("FAILED LOADING RAID INFO(DIFFICULTY HARD:36)") << end;
	}
	else if(z->m_index == ZONE_DUNGEON4)
	{
		if(!m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].LoadRaidInfo(ZONE_DUNGEON4, DUNGEON_DIFFICULTY_NORMAL))
			GAMELOG << init("FAILED LOADING RAID INFO(DIFFICULTY NORMAL:12)") << end;
		if(!m_RaidInfoList[DUNGEON_DIFFICULTY_HARD].LoadRaidInfo(ZONE_DUNGEON4, DUNGEON_DIFFICULTY_HARD))
			GAMELOG << init("FAILED LOADING RAID INFO(DIFFICULTY HARD:12)") << end;
	}
	else if(z->m_index == ZONE_TARIAN_DUNGEON)
	{
		if(m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].LoadRaidInfo(ZONE_TARIAN_DUNGEON, DUNGEON_DIFFICULTY_NORMAL))
			GAMELOG << init("SUCCESS LOADING RAID INFO(DIFFICULTY NORMAL:41)") << end;
		if(m_RaidInfoList[DUNGEON_DIFFICULTY_HARD].LoadRaidInfo(ZONE_TARIAN_DUNGEON, DUNGEON_DIFFICULTY_HARD))
			GAMELOG << init("SUCCESS LOADING RAID INFO(DIFFICULTY HARD:41)") << end;
	}
}

void CArea::PointToCellNum(float x, float z, int* cx, int* cz)
{
	*cx = (int)(x / CELL_SIZE);
	*cz = (int)(z / CELL_SIZE);

	if (*cx < 0)			*cx = 0;
	if (*cx >= m_size[0])	*cx = m_size[0] - 1;
	if (*cz < 0)			*cz = 0;
	if (*cz >= m_size[1])	*cz = m_size[1] - 1;
}

void CArea::CharToCell(CCharacter* ch, char cy, int cx, int cz)
{
	if (IS_IN_CELL(ch))
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
//				<< "AREA:CharToCell:IS_IN_CELL" << delim
//				<< ch->m_type << delim
//				<< ch->m_cellX << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	//if (cy < 0 || cy >= m_ycnt)
	// yLayer 수정
	if (cy < 0 || cy > ((m_ycnt - 1) * 2))
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
//				<< "AREA:CharToCell:YLAYER" << delim
//				<< ch->m_type
//				<< end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	m_cell[cx][cz].CharToCell(ch);

	ch->m_pZone = m_zone;
	ch->m_pArea = this;
	ch->m_cellX = cx;
	ch->m_cellZ = cz;
	GET_YLAYER(ch) = cy;
}

void CArea::CharToCellForSummonNpc(CCharacter* owner, CCharacter* ch, char cy, int cx, int cz)
{
	if (IS_IN_CELL(ch))
	{
		//#ifdef MONSTER_COMBO_BUGFIX
		//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
		//				<< "AREA:CharToCell:IS_IN_CELL" << delim
		//				<< ch->m_type << delim
		//				<< ch->m_cellX << end;
		//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	//if (cy < 0 || cy >= m_ycnt)
	// yLayer 수정
	if (cy < 0 || cy > ((m_ycnt - 1) * 2))
	{
		//#ifdef MONSTER_COMBO_BUGFIX
		//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
		//				<< "AREA:CharToCell:YLAYER" << delim
		//				<< ch->m_type
		//				<< end;
		//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	m_cell[cx][cz].CharToCellForSummonNpc(owner, ch);

	ch->m_pZone = m_zone;
	ch->m_pArea = this;
	ch->m_cellX = cx;
	ch->m_cellZ = cz;
	GET_YLAYER(ch) = cy;
}

void CArea::CharFromCell(CCharacter* ch, bool bDeleteAttackList)
{
	if (!IS_IN_CELL(ch))
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
//				<< "AREA:CharFromCell:IS_IN_CELL" << delim
//				<< ch->m_type << delim
//				<< ch->m_cellX << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	if (ch->m_pArea != this)
	{
//#ifdef MONSTER_COMBO_BUGFIX
//		GAMELOG << init("MONSTER_COMBO_BUGFIX")
//				<< "AREA:CharFromCell:DIFF" << delim
//				<< ch->m_type << delim
//				<< ch->m_pArea->m_index << delim
//				<< m_index << end;
//#endif // MONSTER_COMBO_BUGFIX
		return ;
	}

	m_cell[ch->m_cellX][ch->m_cellZ].CharFromCell(ch);

	ch->m_cellX = NO_CELL;
	ch->m_cellZ = NO_CELL;

	if (bDeleteAttackList)
		DelAttackList(ch);
}

void CArea::ItemToCell(CItem* item, char cy, int cx, int cz)
{
	//if (cy < 0 || cy >= m_ycnt)
	// yLayer 수정
	if (cy < 0 || cy > ((m_ycnt - 1) * 2))
		return ;

	m_cell[cx][cz].ItemToCell(item);

	item->m_cellX =cx;
	item->m_cellZ = cz;
	GET_YLAYER(item) = cy;
}

void CArea::ItemFromCell(CItem* item)
{
	if (!IS_IN_CELL(item))
		return ;

	m_cell[item->m_cellX][item->m_cellZ].ItemFromCell(item);

	item->m_cellX = NO_CELL;
	item->m_cellZ = NO_CELL;
}

void CArea::SendCellInfo(CPC* ch, bool bExcludeOwnPetElemental)
{
	if (!IS_IN_CELL(ch))
		return ;

	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;

	int x, z;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue;

			CCharacter* pChar;
			CCharacter* pNext = m_cell[x][z].m_listChar;

			while ((pChar = pNext))
			{
				pNext = pChar->m_pCellNext;
				// 원래는 pc가 아니면 죽은 것은 Appear 안되나 무조건 되도록 수정 : bs : 050806
				if (pChar != ch)
				{
					if (bExcludeOwnPetElemental && IS_ELEMENTAL(pChar) && TO_ELEMENTAL(pChar)->GetOwner() == ch)
						continue ;
					if (bExcludeOwnPetElemental && IS_PET(pChar) && TO_PET(pChar)->GetOwner() == ch)
						continue ;
					{
						CNetMsg::SP rmsg(new CNetMsg);
						AppearMsg(rmsg, pChar);
						SEND_Q(rmsg, ch->m_desc);
					}

					if(IS_APET(pChar))
					{
						CAPet* apet = TO_APET(pChar);
						
						CNetMsg::SP rmsg(new CNetMsg);
						ExAPetStatusMsg(rmsg, apet);
						SEND_Q(rmsg, apet->GetOwner()->m_desc);
					}

					CPC* pc = TO_PC(pChar);
					if( pc )
					{
						if( pc->m_guildInfo && pc->m_guildInfo->guild() )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							GuildPointRankingMsg( rmsg, pc->m_index, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->GetGuildPointRanking() );
							SEND_Q(rmsg, ch->m_desc);
						}
						if(pc->holy_water_item != NULL)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							UpdateClient::holyWaterStateMsg(rmsg, pc->m_index, pc->holy_water_item->getDBIndex());
							SEND_Q(rmsg, ch->m_desc);
						}
					}
					if( TO_PC(pChar) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						GuildNameColorStateMsg(rmsg, TO_PC(pChar) );
						SEND_Q(rmsg, ch->m_desc);
					}
				}
			}

			CItem* pItem;
			CItem* pItemNext = m_cell[x][z].m_listItem;
			while ((pItem = pItemNext))
			{
				pItemNext = pItem->m_pCellNext;
				// 나타남
				CNetMsg::SP rmsg(new CNetMsg);
				ItemAppearMsg(rmsg, pItem);
				SEND_Q(rmsg, ch->m_desc);
			} // end while
		}
	}
}

bool CArea::IsNotBlocked(CCharacter* ch, CCharacter* tch, bool battack, char& outYlayer)
{
	return IsNotBlocked(ch, GET_YLAYER(tch), GET_X(tch), GET_Z(tch), battack, outYlayer);
}

bool CArea::IsNotBlocked(char sy, float sx, float sz, float tx, float tz, bool bCastleNPC, char& ty)
{
	ty = sy;

	// 영역 검사
	if (tx < 0 || tx >= m_zone->m_attrMap[0].m_size[0] || tz < 0 || tz >= m_zone->m_attrMap[0].m_size[1])
		return false;

	// 목표 지점의 속성
	unsigned short newatt;
	// 현재 지점의 속성
	unsigned short curattr;
	// 계단 검사 루틴에 들어갔었는지 검사
	bool bTestStair = false;

	char maxY = (m_zone->m_countY - 1) * 2;

RETRY:
	// 층 유효성 검사
	if (sy < 0 || sy > maxY)
		return false;

	// 목표 지점의 속성
	newatt = GetAttr(sy, tx, tz);
	// 현재 지점의 속성
	curattr = GetAttr(sy, sx, sz);

	// 목표 지점이 생산지역, 피스존이면 블럭과 같게 처리
	if( (newatt & MATT_PEACE) || (newatt & MATT_PRODUCT_PUBLIC) || (newatt & MATT_PRODUCT_PRIVATE) )
	{
		newatt |= MATT_UNWALKABLE;
	}

	// 공성 수호병이면 MAPATT_WARZONE이 아닌곳은 블럭
	if(bCastleNPC && !(newatt & MATT_WAR))
		newatt |= MATT_UNWALKABLE;
	
	// 블럭을 당하면
	if (newatt & MATT_UNWALKABLE)
	{
		// 이미 계단을 적용했으면 블럭
		if (bTestStair)
			return false;

		// 현재 계단이면 윗층 검사
		if (sy % 2 == 1)
		{
			bTestStair = true;
			sy++;
			goto RETRY;
		}
		else
			return false;
	}

	// 검사는 정상 완료가 되었으므로 최종 y값을 구한다
	// 이동한 영역이 상향 계단
	if (newatt & MATT_STAIR_UP)
	{
		// 일반영역에서 간 경우
		if (ty % 2 == 0)
			ty++;
		// 계단이었으면
		else
			ty = sy - sy % 2 + 1;
	}
	// 이동한 영역이 하향 계단
	else if (newatt & MATT_STAIR_DOWN)
	{
		// 일반영역에서 간 경우
		if (ty % 2 == 0)
			ty--;
		// 계단이었으면
		else
			ty = sy - sy % 2 - 1;
	}
	// 그외의 경우는 계단이 아닌 층번호
	else
		ty = sy - sy % 2;

	return true;
}

bool CArea::IsNotBlocked(CCharacter* ch, char y, float x, float z, bool battack, char& outYlayer)
{
	outYlayer = y;

	// 영역 검사
	if (x < 0 || x >= m_zone->m_attrMap[(int)y/2].m_size[0] || z < 0 || z >= m_zone->m_attrMap[(int)y/2].m_size[1])
	{
		outYlayer = GET_YLAYER(ch);
		return false;
	}

	// 몹이 아니면 클라이언트를 믿고
	if (!IS_NPC(ch) && !battack)
		return true;

	bool bCastleNPC = false;
	if (IS_NPC(ch))
	{
		CNPC* npc = TO_NPC(ch);
		bCastleNPC = (npc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER)) ? true  : false;
	}

	// 공격일때는 내가 있는 자리는 검사 안한다
	int inc_start = 0;
	if (battack && !IS_PC(ch))
		inc_start = 1;

	float x_diff = x - GET_X(ch);
	float z_diff = z - GET_Z(ch);
	float x_diff2 = ABS(x_diff);
	float z_diff2 = ABS(z_diff);

	if (x_diff2 < EPSILON && z_diff2 < EPSILON)
		return IsNotBlocked(GET_YLAYER(ch), GET_X(ch), GET_Z(ch), x, z, bCastleNPC, outYlayer);

	int i;
	float x_inc;
	float z_inc;

	float sx, sz, tx, tz;
	char sy, ty;

	sx = GET_X(ch);
	sz = GET_Z(ch);
	sy = GET_YLAYER(ch);
	ty = sy;

	if (x_diff2 > z_diff2)
	{
		x_inc = (x_diff < 0) ? -1.0f : 1.0f;
		z_inc = ((z_diff < 0) ? -1.0f : 1.0f) * z_diff2 / x_diff2;

		for (i = inc_start; (x_diff > 0 && (int)(GET_X(ch) + (x_inc * i)) <= (int)x) || (x_diff < 0 && (int)(GET_X(ch) + (x_inc * i)) >= (int)x); i++)
		{
			tx = GET_X(ch) + (x_inc * i);
			tz = GET_Z(ch) + (z_inc * i);
			if (!IsNotBlocked(sy, sx, sz, tx, tz, bCastleNPC, ty))
				return false;
			sx = tx;
			sz = tz;
			sy = ty;
		} // end for (i = inc_start; (x_diff > 0 && GET_X(ch) + (x_inc * i) < x) || (x_diff < 0 && GET_X(ch) + (x_inc * i) > x); i++)
	} // end if

	else
	{
		x_inc = ((x_diff < 0) ? -1.0f : 1.0f) * x_diff2 / z_diff2;
		z_inc = (z_diff < 0) ? -1.0f : 1.0f;

		for (i = inc_start; (z_diff > 0 && (int)(GET_Z(ch) + (z_inc * i)) <= (int)z) || (z_diff < 0 && (int)(GET_Z(ch) + (z_inc * i)) >= (int)z); i++)
		{
			tx = GET_X(ch) + (x_inc * i);
			tz = GET_Z(ch) + (z_inc * i);
			if (!IsNotBlocked(sy, sx, sz, tx, tz, bCastleNPC, ty))
				return false;
			sx = tx;
			sz = tz;
			sy = ty;
		} // end for (i = inc_start; (z_diff > 0 && GET_Z(ch) + (z_inc * i) < z) || (z_diff < 0 && GET_Z(ch) + (z_inc * i) > z); i++)
	} // end else

	outYlayer = ty;

	return IsNotBlocked(ty, x, z, x, z, bCastleNPC, sy);
}

unsigned short CArea::GetAttr(char y, float x, float z)
{
	int x_i = (int) x;
	int z_i = (int) z;

	y /= 2;

	if (y < 0 || y >= m_zone->m_countY)
		return MATT_UNWALKABLE;

	if (x_i < 0 || x_i >= m_zone->m_attrMap[y].m_size[0] || z_i < 0 || z_i >= m_zone->m_attrMap[y].m_size[1])
		return MATT_UNWALKABLE;

	return m_zone->m_attrMap[y].m_attr[x_i][z_i];
}

float CArea::GetHeight(char y, float x, float z)
{
	int x_i = (int) x;
	int z_i = (int) z;

	int ty = y / 2;
	if(m_zone == NULL)
	{
		return 0;
	}
	if (ty < 0 || ty >= m_zone->m_countY)
		return 0.0f;

	if (x_i < 0 || x_i >= m_zone->m_attrMap[ty].m_size[0] || z_i < 0 || z_i >= m_zone->m_attrMap[ty].m_size[1])
		return 0.0f;

	if ( y % 2 != 0 && (GetAttr(y, x, z) & MATT_UNWALKABLE) )
		return GetHeight(y + 1, x, z);
	else
	{
		return m_zone->m_attrMap[ty].m_height[x_i][z_i];
	}
}

void CArea::LoadNPC(std::vector<tag_npc_regen_info>& vec)
{
	m_nCountNPC = vec.size();

	int nRealCountNPC = 0;
	int nNonPeachfulNPCIndex = 0;

	if( m_zone->m_index == ZONE_START || m_zone->m_index == ZONE_DRATAN || m_zone->m_index == ZONE_MERAC )
		m_nCountNPC += 5;

	if(m_zone->m_index == ZONE_DRATAN)
	{
		///=== 이부분 변경시 밑의 nRealCountNPC 로 변경 부분도 수정이 필요
		///=== 설계상의 잘못 리펙토링 필요!!!
		m_nCountNPC += 30;
	}

	m_npcRegenList.SetCount( m_nCountNPC );

	int last_index = 0;
	BOOST_FOREACH(tag_npc_regen_info& data, vec)
	{
		last_index = data.index;

		m_npcRegenList.m_type = 0;

		if (!data.pNPCProto->CheckFlag(NPC_PEACEFUL))
			nNonPeachfulNPCIndex = m_npcRegenList.m_wPos;

		if (data.pNPCProto->CheckFlag(NPC_RAID))
		{
#if !defined (LC_TLD) && !defined (LC_GAMIGO)  && !defined(LC_KOR) && !defined(LC_RUS) && !defined(LC_BILA)
			if ( m_zone->m_index != ZONE_START
			   )
			{
#endif // !defined (LC_TLD)
				CNPCRegenInfo* pNewRegenInfo = new CNPCRegenInfo;
				pNewRegenInfo->m_index = data.index;
				pNewRegenInfo->m_npcIdx = data.npcIndex;
				pNewRegenInfo->m_zoneNo = m_zone->m_index;
				pNewRegenInfo->m_regenX = data.x;
				pNewRegenInfo->m_regenY = data.yLayer;
				pNewRegenInfo->m_regenZ = data.z;
				pNewRegenInfo->m_regenR = data.r;
				pNewRegenInfo->m_totalNum = data.totalNum;

				pNewRegenInfo->m_bAlive = false;
				pNewRegenInfo->m_lastDieTime = 0;
				pNewRegenInfo->m_regenSec = data.regenSec;
				pNewRegenInfo->m_npcProto = data.pNPCProto;
				m_raidNPCRegenList.push_back(pNewRegenInfo);

#if !defined (LC_TLD) && !defined (LC_GAMIGO)  && !defined(LC_KOR) && !defined(LC_RUS) && !defined(LC_BILA)
			}
#endif // !defined (LC_TLD)
		}
		else
		{
			{
				m_npcRegenList.AddRegenInfo(data.index, data.npcIndex, data.regenSec, 
					data.x, data.yLayer, data.z, data.r, data.totalNum, m_zone->m_index, true, data.pNPCProto);
				nRealCountNPC++;
			}

			gserver->addNpcRegenInfo( m_npcRegenList.GetNpcRegenInfo(m_npcRegenList.m_wPos-1 ) );

			if (data.pNPCProto->CheckFlag(NPC_PEACEFUL))
			{
				if( !(data.pNPCProto->CheckFlag(NPC_MINERAL | NPC_ENERGY | NPC_CROPS | NPC_GUARD)) )
				{
					if (data.npcIndex != 484 && data.npcIndex != 486)  // 크리스 마스 이벤트용 트리 NPC는 뺀다
					{
						m_NpcPortal_NpcList.insert(data.npcIndex);
					}
				}
			}
			else
			{
				if(data.npcIndex <= 0)
				{
					LOG_ERROR("Invalid Npc Index. regenIndex[%d], npcIndex[%d]", data.index, data.npcIndex);
					continue;
				}
				m_NpcPortal_MobList.insert(data.npcIndex);
			}
		}
	}

	{
		// shop npc 정보도 담아주자
		for(int i = 0; i < m_zone->m_nShopCount; i++)
		{
			m_NpcPortal_NpcList.insert(m_zone->m_shopList[i].m_keeperIdx);
		}
	}

	if (m_zone->m_index == ZONE_START || m_zone->m_index == ZONE_DRATAN || m_zone->m_index == ZONE_MERAC)
	{
		for ( int i = 1; i < 6 ; i++ )
		{
			m_npcRegenList.AddRegenInfo( last_index + i, 314, 300,
										 m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenX,
										 m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenY,
										 m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenZ,
										 m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenR, -1, m_zone->m_index);
			nRealCountNPC++;
			gserver->addNpcRegenInfo( m_npcRegenList.GetNpcRegenInfo(m_npcRegenList.m_wPos-1 ) );
		}
	} // zone_Index 가 0, 4, 7, 일경우만 MPC 등록 - 쥬노, 드라탄, 메라크 지역

	m_npcRegenList.m_nCount = nRealCountNPC;
	if(m_zone->m_index == ZONE_DRATAN)
	{
		// 드라탄이면 수호병의 최대치 만큼
		///== SetCount()를 사용하지 않기 때문에 실제 사이즈를 늘리려면
		///== SetCount()호출하는 곳에서도 늘려야 한다.
		m_npcRegenList.m_nCount += 30;
	}

	int nRegenCellX = 0;
	int nRegenCellZ = 0;
	for (int nIndexRegen = 0; nIndexRegen < m_npcRegenList.m_nCount; nIndexRegen++)
	{
		PointToCellNum(m_npcRegenList.m_infoList[nIndexRegen].m_regenX, m_npcRegenList.m_infoList[nIndexRegen].m_regenZ, &nRegenCellX, &nRegenCellZ);
		m_cell[nRegenCellX][nRegenCellZ].m_nTotalRegen++;
	}
}

int CArea::GetPCCount(CCharacter* ch, int findArea)
{
	return GetPCCount(ch->m_cellX, ch->m_cellZ, findArea);
}

int CArea::GetPCCount(int cx, int cz, int findArea)
{
	int i, j;
	int ret = 0;

	for (i = cx - findArea; i <= cx + findArea; i++)
	{
		if (i < 0 || i >= m_size[0]) continue ;
		for (j = cz - findArea; j <= cz + findArea; j++)
		{
			if (j < 0 || j >= m_size[1]) continue ;
			ret += m_cell[i][j].m_nCountPC;
		}
	}

	return ret;
}

CCharacter* CArea::FindChar(int idx, MSG_CHAR_TYPE tchType)
{
	CCharacter* ret;
	int i, j;
	for (i = 0; i < m_size[0]; i++)
	{
		for (j = 0; j < m_size[1]; j++)
		{
			ret = m_cell[i][j].m_listChar;
			while (ret)
			{
				if (ret->m_type == tchType && ret->m_index == idx && !DEAD(ret))
					return ret;
				ret = ret->m_pCellNext;
			}
		}
	}

	return NULL;
}

void CArea::SetEnable(int nRaidRoomNum, int nDifficulty)
{
	if (m_bEnable)
		return ;

	m_bEnable = true;
	m_pulseNPCAI = gserver->m_pulse;
	m_pulseNPCRegen = gserver->m_pulse;
	m_pulseProcTrigger = gserver->m_pulse;

	int i;
	int cx, cz;

	m_nCountNPC = 0;

	// 싱글던전
	m_nMakeNPC = 0;

	CNPC* npc;

	vec_raidnpcrregenlist_t::iterator it = m_raidNPCRegenList.begin();
	vec_raidnpcrregenlist_t::iterator endit = m_raidNPCRegenList.end();
	for (; it != endit; ++it)
	{
		CNPCRegenInfo* p = *(it);
		p->m_lastDieTime = gserver->m_pulse + GetRandom(1, p->m_regenSec / 2);
		p->m_bAlive = false;
	}

	if(m_RaidDifficulty != -1)
		m_RaidInfoList[this->m_RaidDifficulty].m_pArea = this;

	if(nRaidRoomNum != -1)
		m_nRaidRoomNo = nRaidRoomNum;

	// 레이드 던전일 경우 리젠 방식이 다르므로 독립적으로 관리한다.
	if (m_zone->m_index == ZONE_ALTER_OF_DARK
			|| m_zone->m_index == ZONE_CAPPELLA_1)
	{
		// 트리거 리스트를 먼저 로딩한다.
		m_CTriggerList.LoadTriggerSaveInfo(nRaidRoomNum);

		// 서버 재부팅시, 유지시 모두 이 플래그 사용
		if (m_CTriggerList.Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1018_BEFORE))
		{
			Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1018);
		}
		else if (m_CTriggerList.Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE))
		{
			Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1003);
		}
		else if (m_CTriggerList.Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE))
		{
			Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1002);
		}
		else
		{
			// 초반 리젠은 level 0 부터이다.
			Change_NpcRegenRaid(0);
		}
	}
	else

		// 추후 클라이언트로 부터 난이도 설정을 받을 때 아래 주석을 제거필요.
		// m_RaidDifficulty = 난이도 설정
		// m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].m_pArea = this;
		// m_RaidInfoList[DUNGEON_DIFFICULTY_HARD].m_pArea = this;
		if (m_zone->m_index == ZONE_AKAN_TEMPLE)
		{
			// 현재 SAVE POINT를 찾아서 설정한다.
			//m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].LoadSavePointRaidInfo(ZONE_AKAN_TEMPLE, nRaidRoomNum);
			//m_RaidInfoList[DUNGEON_DIFFICULTY_HARD].LoadSavePointRaidInfo(ZONE_AKAN_TEMPLE, nRaidRoomNum);
			m_RaidInfoList[this->m_RaidDifficulty].LoadSavePointRaidInfo(ZONE_AKAN_TEMPLE, nRaidRoomNum);
			m_GroundEffect2.Init(m_zone->m_index, this);
		}
		else if(m_zone->m_index == ZONE_DUNGEON4)
		{
			m_RaidInfoList[this->m_RaidDifficulty].LoadSavePointRaidInfo(ZONE_DUNGEON4, nRaidRoomNum);
			m_GroundEffect2.Init(m_zone->m_index, this);
		}
		else if(m_zone->m_index == ZONE_TARIAN_DUNGEON)
		{
			m_RaidInfoList[this->m_RaidDifficulty].LoadSavePointRaidInfo(ZONE_TARIAN_DUNGEON, nRaidRoomNum);
		}

		else

			// 싱글던전이 아닌경우에만  일반 NPC 리젠
			// TODO : 추후 싱글던전에 Mob이 아닌 NPC가 들어갈 경우 수정 요.
			if (!m_zone->IsPersonalDungeon())
			{
				for (i = 0; i < m_npcRegenList.m_nCount; i++)
				{
					CNPCRegenInfo* p = m_npcRegenList.m_infoList + i;

					if (p->m_totalNum == 0)
						continue ;

					if ( !p->m_bRegen )
						continue;

					// 층적용 몹배치 확인
					npc = gserver->m_npcProtoList.Create(p->m_npcIdx, p);
					if (!npc)	continue ;

					// 속성맵상 피스존이고 피스 NPC가 아니면 리젠안함
					if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
					{
						unsigned short mapattr = GetAttr(p->m_regenY, p->m_regenX, p->m_regenZ);

						if(mapattr & MATT_UNWALKABLE || mapattr & MATT_PEACE)
						{
							GAMELOG << init("REGEN error")
								<< "NPC" << delim
								<< npc->m_name << delim
								<< p->m_npcIdx << delim
								<< "ZONE" << delim
								<< m_zone->m_index << delim
								<< "YLAYER" << delim
								<< p->m_regenY << delim
								<< "X" << delim
								<< p->m_regenX << delim
								<< "Z" << delim
								<< p->m_regenZ
								<< end;
							delete npc;
							continue;
						}
					}

					GET_X(npc) = p->m_regenX;
					GET_YLAYER(npc) = p->m_regenY;
					GET_Z(npc) = p->m_regenZ;

					// 어택 가능한 NPC
					if (npc->m_proto->CheckFlag(NPC_ATTACK) && npc->m_proto->m_index != 339)
						GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
					else
						GET_R(npc) = p->m_regenR;

					npc->m_regenX = GET_X(npc);
					npc->m_regenY = GET_YLAYER(npc);
					npc->m_regenZ = GET_Z(npc);

					p->m_bAlive = true;
					p->m_numRegen++;
					//p->m_lastDieTime = 0;

#if defined (SAKURA_EVENT_2008_REWARD)
					if( npc->m_proto->m_index == 342 )
						npc->m_hp = 1;
#endif // defined (SAKURA_EVENT_2008_REWARD)

#ifdef TLD_EVENT_SONG
					if( npc->m_proto->m_index == 1622 || npc->m_proto->m_index == 1623 )
					{
						if (gserver->tld_event == false)
						{
							p->m_lastDieTime = gserver->m_pulse - p->m_regenSec;
							continue;
						}
					}
#endif

					if( npc->m_proto->m_index == 314 )
					{
						p->m_lastDieTime = gserver->m_pulse - p->m_regenSec;
						continue;
					}
					else
					{
						p->m_lastDieTime = 0;
					}

					CDratanCastle * pCastle = CDratanCastle::CreateInstance();
					if(m_zone->m_index == ZONE_DRATAN_CASTLE_DUNGEON && !npc->m_proto->CheckFlag(NPC_ZONEMOVER))
					{
						int nMobRate[10][4] =  	// 공격력, 방어력, 최대체력, 시야범위
						{
							{20, 20, 30, 0},
							{10, 10, 15, 0},
							{5, 5, 10, 0},
							{0, 0, 0, 0},
							{-5, -5, -5, 0},
							{-7, -7, -7, 0},
							{-10, -10, -10, 0},
							{-15, -15, -15, 0},
							{-20, -20, -20, 0},
							{-25, -25, -25, -80},
						};

						int midx = pCastle->m_dvd.GetMobRate()/10;
						if(midx > 9)
						{
							midx = 9;
						}

						// 공격력 적용
						if(npc->m_attackType == MSG_DAMAGE_MELEE)
						{
							npc->m_eqMelee = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
							if(npc->m_eqMelee < 0)
							{
								npc->m_eqMelee = 1;
							}
						}
						else if(npc->m_attackType == MSG_DAMAGE_RANGE)
						{
							npc->m_eqRange = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
							if(npc->m_eqRange < 0)
							{
								npc->m_eqRange = 1;
							}
						}
						else if(npc->m_attackType == MSG_DAMAGE_MAGIC)
						{
							npc->m_eqMagic = npc->m_proto->m_magic + npc->m_proto->m_magic * nMobRate[midx][0] / 100;
							if(npc->m_eqMagic < 0)
							{
								npc->m_eqMagic = 1;
							}
						}

						// 방어력 적용
						npc->m_eqDefense = npc->m_proto->m_defense + npc->m_proto->m_defense * nMobRate[midx][1] / 100;

						// 최대 체력 적용
						npc->m_maxHP = npc->m_proto->m_hp + npc->m_proto->m_hp * nMobRate[midx][2] / 100;
						if(npc->m_maxHP < 0)
						{
							npc->m_maxHP = 1;
						}
						npc->m_hp = npc->m_maxHP;

						// 시야 거리 적용
						if(nMobRate[midx][3] != 0 && npc->m_attackRange != 0)
						{
							npc->m_attackRange = npc->m_proto->m_attackArea + npc->m_proto->m_attackArea * nMobRate[midx][3] / 100;
						}
					}

					if(npc->m_proto->m_index == 1152)
						gserver->m_listNpcDropItemAtTime.push_back(npc);

					AddNPC(npc);
					PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
					CharToCell(npc, GET_YLAYER(npc), cx, cz);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						AppearMsg(rmsg, npc, true);
						SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
					}
				}
			}
	// 싱글 던전일 경우 리젠리스트세팅
			else
			{
				m_regenList = new int[m_npcRegenList.m_nCount];

				for(i = 0; i < m_npcRegenList.m_nCount; i++)
				{
					m_regenList[i] = m_npcRegenList.m_infoList[i].m_index;
				}
			}

	// 상점 리젠
	for (i=0; i < m_zone->m_nShopCount; i++)
	{
		if (GetAttr(m_zone->m_shopList[i].m_yLayer, m_zone->m_shopList[i].m_x, m_zone->m_shopList[i].m_z) & MATT_WAR)
			continue ;
		npc = gserver->m_npcProtoList.Create(m_zone->m_shopList[i].m_keeperIdx, NULL);
		if (!npc)	continue;

		GET_R(npc) = m_zone->m_shopList[i].m_r;
		npc->m_regenX = GET_X(npc) = m_zone->m_shopList[i].m_x;
		npc->m_regenZ = GET_Z(npc) = m_zone->m_shopList[i].m_z;
		npc->m_regenY = GET_YLAYER(npc) = m_zone->m_shopList[i].m_yLayer;

		AddNPC(npc);
		PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		CharToCell(npc, GET_YLAYER(npc), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, npc, true);
			SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
		}
	}
	m_mobAISeq = gserver->AddMobAISeq(m_npcRegenList.m_nCount);
}

void CArea::SetDisable()
{
	if (!m_bEnable)
		return ;

	m_bEnable = false;

	// cell
	int i, j;
	for (i = 0; i < m_size[0]; i++)
	{
		for (j = 0; j < m_size[1]; j++)
		{
			// 캐릭터 빼기
			while (m_cell[i][j].m_listChar)
				CharFromCell(m_cell[i][j].m_listChar, true);

			// 아이템 빼기
			CItem* item;
			CItem* nItem;

			nItem = m_cell[i][j].m_listItem;

			while ((item = nItem))
			{
				nItem = nItem->m_pCellNext;

				// 셀 리스트에서 빼고 삭제
				ItemFromCell(item);
				item->m_pArea = NULL;
				delete item;
				item = NULL;
			}
		}
	}

	// TODO : ENV

	// npc
	CNPC* npc;
	while (m_npcList)
	{
		npc = m_npcList->m_pNPCNext;
		m_npcList->DeleteNPC();

		m_npcList = NULL;
		CNPC* cnpc = m_npcList;
		m_npcList = npc;
	}

	// npc regen
	for (i = 0; i < m_npcRegenList.m_nCount; i++)
	{
		m_npcRegenList.m_infoList[i].m_bAlive = false;
		m_npcRegenList.m_infoList[i].m_lastDieTime = 0;
		m_npcRegenList.m_infoList[i].m_numRegen = 0;
	}

	// 싱글 던전
	m_nMakeNPC = 0;
	m_bBoss = false;
	if( m_regenList)
		delete m_regenList;
	m_regenList = NULL;

	// 트리거 초기화도 시킨다.
	if(m_bUseTriggerEvent)
		m_CTriggerList.ResetTriggerInfo(false);

	if(m_zone->m_index == ZONE_AKAN_TEMPLE)
	{
		//Scene정보를 초기화 한다.
		m_GroundEffect2.Stop();
		//m_RaidInfoList[DUNGEON_DIFFICULTY_NORMAL].ResetRaidInfo(ZONE_AKAN_TEMPLE);
		if(this->m_RaidDifficulty != -1)
		{
			m_RaidInfoList[this->m_RaidDifficulty].ResetRaidInfo(ZONE_AKAN_TEMPLE);
			m_RaidDifficulty = -1;
		}
	}
	else if(m_zone->m_index == ZONE_DUNGEON4)
	{
		m_GroundEffect2.Stop();
		if(this->m_RaidDifficulty != -1)
		{
			m_RaidInfoList[this->m_RaidDifficulty].ResetRaidInfo(ZONE_DUNGEON4);
			m_RaidDifficulty = -1;
		}
	}
	else if(m_zone->m_index == ZONE_TARIAN_DUNGEON)
	{
		if(this->m_RaidDifficulty != -1)
		{
			m_RaidInfoList[this->m_RaidDifficulty].ResetRaidInfo(ZONE_TARIAN_DUNGEON);
			m_RaidDifficulty = -1;
		}
	}
	m_nRaidRoomNo = -1;

	if(gserver->m_RoyalRumble.m_pRookieArea == this)
	{
		gserver->m_RoyalRumble.SetInitRookieTime();
		gserver->m_RoyalRumble.SetRookiePlaying(false);
		gserver->m_RoyalRumble.SetRookieLastEnter(false);
	}
	else if(gserver->m_RoyalRumble.m_pSeniorArea == this)
	{
		gserver->m_RoyalRumble.SetInitSeniorTime();
		gserver->m_RoyalRumble.SetSeniorPlaying(false);
		gserver->m_RoyalRumble.SetSeniorLastEnter(false);
	}
	else if(gserver->m_RoyalRumble.m_pMasterArea == this)
	{
		gserver->m_RoyalRumble.SetInitMasterTime();
		gserver->m_RoyalRumble.SetMasterPlaying(false);
		gserver->m_RoyalRumble.SetMasterLastEnter(false);
	}

	gserver->RemoveMobAISeq(m_mobAISeq, m_npcRegenList.m_nCount);
	m_mobAISeq = -1;
}

CItem* CArea::DropItem(CItem* item, CCharacter* ch)
{
	if (item == NULL)
		return NULL;

	if( item->getDBIndex() == NAS_ITEM_DB_INDEX)
		return NULL;

	// 싱글던전 itemdrop
	if (m_zone->IsGuildRoom() || m_zone->IsPersonalDungeon())
	{
		item->m_pos = CPos(GET_X(ch), GET_Z(ch), ch->m_pos.m_h, GET_R(ch), GET_YLAYER(ch));
	}
	else
	{
		item->m_pos = CPos(GET_X(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
						   GET_Z(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
						   GET_H(ch),
						   GET_R(ch),
						   GET_YLAYER(ch));
	}

	// 좌표 보정
	if (GET_X(item) < 0)		GET_X(item) = 0;
	if (GET_Z(item) < 0)		GET_Z(item) = 0;
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	if (GET_X(item) >= m_zone->m_attrMap[0].m_size[0])
		GET_X(item) = m_zone->m_attrMap[0].m_size[0];
	if (GET_Z(item) >= m_zone->m_attrMap[0].m_size[1])
		GET_Z(item) = m_zone->m_attrMap[0].m_size[1];
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조

	if (GetAttr(GET_YLAYER(item), GET_X(item), GET_Z(item)) & MATT_UNWALKABLE)
		item->m_pos = CPos(GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch), GET_YLAYER(ch));

	// 셀에 넣기
	item->m_pArea = this;
	item->m_groundPulse = gserver->m_pulse;

	int cx, cz;
	PointToCellNum(GET_X(item), GET_Z(item), &cx, &cz);
	ItemToCell(item, GET_YLAYER(item), cx, cz);

	ArtifactManager::instance()->calcDropCount(item, false);

	return item;
}

CItem* CArea::DropItem(int item_db_idx, CCharacter* ch, int plus, int flag, LONGLONG count, bool bOption)
{
	if(item_db_idx == NAS_ITEM_DB_INDEX)
	{
		return NULL;
	}

	CItem* dropitem = gserver->m_itemProtoList.CreateItem(item_db_idx, WEARING_NONE, plus, flag, count);
	if (!dropitem)
		return NULL;

	// 싱글던전 itemdrop
	if (m_zone->IsGuildRoom() || m_zone->IsPersonalDungeon())
	{
		dropitem->m_pos = CPos(GET_X(ch), GET_Z(ch), ch->m_pos.m_h, GET_R(ch), GET_YLAYER(ch));
	}
	else
	{
		dropitem->m_pos = CPos(GET_X(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
							   GET_Z(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
							   GET_H(ch),
							   GET_R(ch),
							   GET_YLAYER(ch));
	}

	// 레어 아이템은 옵션 없음
	if (dropitem->IsRareItem())
		bOption = false;

	if (dropitem->IsOriginItem())
		bOption = false;

	// 악세사리 일때 Option Setting
	if (bOption && dropitem->IsAccessary() && dropitem->m_nOption < 1)
	{
		// 060227 : bs : 하드코딩 악세서리 검사, 옵션 없이
		bool bSkipAddOption = false;
		switch (dropitem->getDBIndex())
		{
		case 1301:		// 하드코딩 악세서리
		case 1302:
		case 1303:
		case 1304:
		case 1305:
		case 1306:
			bSkipAddOption = true;
			break;

		default:
			break;

		}

		if (!bSkipAddOption)
		{
			// 순서대로 5,4,3,1개 붙을 확률
			static const int factor[] = {10, 40, 80, 100};
			static const int limit[] = {500, 2000, 4000, 5000};

			int prob[MAX_ACCESSORY_OPTION-1];
			int i, j;

			for (i=0; i < MAX_ACCESSORY_OPTION - 1; i++)
			{
				if ((prob[i] = ch->m_level * factor[i]) > limit[0])
					prob[i] = limit[i];
			}

			// 옵션 수 결정
			int result = GetRandom(1, 10000);
			int num = 2;	// default 2개
			int* type;

			if (result <= prob[0])
				num = 5;
			else if (result <= prob[1])
				num = 4;
			else if (result <= prob[2])
				num = 3;
			else if (result <= prob[3])
				num = 1;
			else
				num = 2;

			type = new int[num];

			LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
			const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

			// 옵션 타입 결정
			i = 0;
			while (i < num)
			{
				// type[i] 결정
				type[i] = GetRandom(0, maxnumoption - 1);

				// type[i]가 중복인 검사
				// 중복이면 type[i]를 1씩 증가하면서 중복 검사
				for (j = 0; j < maxnumoption; j++)
				{
					if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
						break;
				}

				// 더이상 옵션을 붙일 수 없음
				if (j == maxnumoption)
					num = i;
				else
					type[i] = (type[i] + j) % maxnumoption;

				bitfield |= ((LONGLONG)1 << type[i]);

				// 옵션번호로 붙을 수 있는 옵션인지 검사
				COptionProto* proto = gserver->m_optionProtoList.FindProto(type[i]);
				if (!proto)
					continue;

				// 악세사리중에서 붙을 수 없으면 다시
				if (!((1 << dropitem->m_itemProto->getItemSubTypeIdx()) & proto->m_accessoryType))
					continue ;

				i++;
			}

			// 옵션 부여
			for (i=0; i < num; i++)
			{
				COptionProto* proto = gserver->m_optionProtoList.FindProto(type[i]);

				if (!proto)
					continue;

				// Level, Value 셋팅
				dropitem->m_option[dropitem->m_nOption].MakeOptionValue(proto, ch->m_level, num);
				++dropitem->m_nOption;
			}

			delete [] type;
		}
	}

	// 좌표 보정
	if (GET_X(dropitem) < 0)		GET_X(dropitem) = 0;
	if (GET_Z(dropitem) < 0)		GET_Z(dropitem) = 0;
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	if (GET_X(dropitem) >= m_zone->m_attrMap[0].m_size[0])
		GET_X(dropitem) = m_zone->m_attrMap[0].m_size[0];
	if (GET_Z(dropitem) >= m_zone->m_attrMap[0].m_size[1])
		GET_Z(dropitem) = m_zone->m_attrMap[0].m_size[1];
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조

	if (GetAttr(GET_YLAYER(dropitem), GET_X(dropitem), GET_Z(dropitem)) & MATT_UNWALKABLE)
		dropitem->m_pos = CPos(GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch), GET_YLAYER(ch));

	// 셀에 넣기
	dropitem->m_pArea = this;
	dropitem->m_groundPulse = gserver->m_pulse;

	int cx, cz;
	PointToCellNum(GET_X(dropitem), GET_Z(dropitem), &cx, &cz);
	ItemToCell(dropitem, GET_YLAYER(dropitem), cx, cz);

	return dropitem;
}

void CArea::GroundItemControl()
{
	int cx, cz;

	CItem* item;
	CItem* nItem;

	for (cx=0; cx < m_size[0]; cx++)
	{
		for (cz=0; cz < m_size[1]; cz++)
		{
			nItem = this->m_cell[cx][cz].m_listItem;

			while ((item = nItem))
			{
				nItem = nItem->m_pCellNext;
				// EGG 아이템 운영자가 떨어뜨린 것은 사라지지 않는다.
				if( item->getDBIndex() == 2148 && item->getUsed() == 1 && gserver->isActiveEvent( A_EVENT_EGGS_HUNT) )
					continue;

				if (gserver->m_pulse - item->m_groundPulse >= GROUND_ITEM_PULSE)	// 5 minutes
				{
					{
						// 없애는 Msg
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDisappearMsg(rmsg, item->getVIndex());
						SendToCell(rmsg, GET_YLAYER(item), cx, cz);
					}

					// ITEM_JUNK Log
					if(item->getDBIndex() != 6593) // 웜의 알 ITEM_JUNK 로그 남기지 않음
					{
						GAMELOG << init("ITEM_JUNK")
								<< itemlog(item)
								<< end;
					}

					if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY  && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
					{
						ArtifactManager::instance()->calcDropCount(item, true);
					}

					// 셀 리스트에서 빼고 삭제
					ItemFromCell(item);
					item->m_pArea = NULL;
					delete item;
					item = NULL;

					this->m_nJunkItems++;
				}

				// 우선권 해제
				else if (gserver->m_pulse - item->m_groundPulse >= ITEM_PREPERENCE_PULSE)
				{
					if(item->m_itemProto && !(item->m_itemProto->getItemFlag() & ITEM_FLAG_NO_STASH))
					{
						item->m_preferenceIndex = -1;
					}
				}
			}
		}
	}
}

void CArea::SendToAllClient(CNetMsg::SP& msg)
{
	int cx, cz;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCharacter* pChar = m_cell[cx][cz].m_listChar;
			while(pChar)
			{
				if (IS_PC(pChar))
					SEND_Q(msg, TO_PC(pChar)->m_desc);
				pChar = pChar->m_pCellNext;
			}
		}
	}
}

void CArea::MoveAllChar(int nIndex, int nExtra)
{
	int cx, cz;
	CZone * pZone = gserver->FindZone(nIndex);
	if (pZone == NULL)
		return;

	if (pZone->m_bRemote == true)
	{
		return;
	}

	if (nExtra < 0 || nExtra >= pZone->m_countZonePos)
	{
		return;
	}

	// 먼저 이동시킬 유저를 선별후 이동 // 다른셀로 이동시 또는 이동중 삭제시 문제 발생
	std::vector<CPC*> m_MovePCList;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCharacter* pChar = m_cell[cx][cz].m_listChar;
			while(pChar)
			{
				if (IS_PC(pChar))
				{
					CPC* pPC = TO_PC(pChar);
					if( pPC )
						m_MovePCList.push_back(pPC);
				}
				pChar = pChar->m_pCellNext;
			}
		}
	}

	std::vector<CPC*>::iterator it = m_MovePCList.begin();
	std::vector<CPC*>::iterator endit = m_MovePCList.end();
	for(; it != endit; ++it)
	{
		CPC* movePc = *(it);

		movePc->ResetPlayerState(PLAYER_STATE_SITDOWN);
		movePc->CalcStatus(true);

		GoZone( movePc, nIndex,
				pZone->m_zonePos[nExtra][0],														// ylayer
				GetRandom(pZone->m_zonePos[nExtra][1], pZone->m_zonePos[nExtra][3]) / 2.0f,			// x
				GetRandom(pZone->m_zonePos[nExtra][2], pZone->m_zonePos[nExtra][4]) / 2.0f);		// z
	}

	m_MovePCList.clear();
}

CNPC* CArea::FindNearestFamily(CNPC* npc, int aitype, int hpParam)
{
	if (!IS_IN_CELL(npc))
		return NULL;

	if (npc->m_proto->m_family == -1)
		return NULL;

	int sx = npc->m_cellX - CELL_EXT;
	int ex = npc->m_cellX + CELL_EXT;
	int sz = npc->m_cellZ - CELL_EXT;
	int ez = npc->m_cellZ + CELL_EXT;

	int x, z;

	float dist = 999.9f;
	CNPC* ret = NULL;

	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				// 자신은 제외하고
				if (pChar == npc)
					continue ;

				// 죽은넘 제외
				if (DEAD(pChar))
					continue ;

				// 다른층 제외
				if (ABS(GET_YLAYER(npc) - GET_YLAYER(pChar)) >= 2)
					continue ;

				// NPC만
				if (!IS_NPC(pChar))
					continue ;

				CNPC* tnpc = TO_NPC(pChar);

				// 동족이고
				if (tnpc->m_proto->m_family != npc->m_proto->m_family)
					continue ;

				// aitype이 일치하고
				if (aitype != -1 && aitype != tnpc->m_proto->m_aitype)
					continue ;

				// 체력 검사
				if (tnpc->m_hp > tnpc->m_maxHP * hpParam / 100)
					continue ;


				// 테이밍 중인 몬스터나 혼란 중인 몬스터는 동족으로 보지 않는다.
				if ( tnpc->Check_MobFlag( STATE_MONSTER_TAMING ) || tnpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
					continue;

				if ( tnpc->Check_MobFlag( STATE_MONSTER_MERCENARY) )
					continue;

				if ( tnpc->Check_MobFlag( STATE_MONSTER_TOTEM_BUFF) || tnpc->Check_MobFlag( STATE_MONSTER_TOTEM_ATTK ) )
					continue;

				// 거리 제일 가까운 넘 찾기
				if (GetDistance(npc, tnpc) < dist)
				{
					dist = GetDistance(npc, tnpc);
					ret = tnpc;
				}
			}
		}
	}

	return ret;
}

CCharacter* CArea::FindCharInCell(CCharacter* ch, int idx, MSG_CHAR_TYPE tchType, bool bIncludeDead)
{
	if (!IS_IN_CELL(ch))
		return NULL;

	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;
	int x, z;

	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (pChar->m_type == tchType && pChar->m_index == idx && (!DEAD(pChar) || bIncludeDead))
					return pChar;
			}
		}
	}

	return NULL;
}

bool CArea::MoveChar(CCharacter* ch, char y, float x, float z, float h, float r, MSG_MOVE_TYPE movekind, CCharacter* target)
{
	// 몹인데 못 움직이거나, 리젠 후 얼마 지나지 않았으면 가만히
	if (IS_NPC(ch) && !ch->CanMove())
		return false;

	bool change_position = false; //좌표가 변경되었는지 여부

	char outY;
	if (!IsNotBlocked(ch, y, x, z, false, outY))
		return false;

	y = outY;

	int cx, cz;
	PointToCellNum(x, z, &cx, &cz);

	CPC* pc = (IS_PC(ch)) ? TO_PC(ch) : NULL;
	CNPC* npc = (IS_NPC(ch)) ? TO_NPC(ch) : NULL;

	// 몹일때
	if (IS_NPC(ch))
	{
		CCharacter* pChar;
		CCharacter* pnext = m_cell[cx][cz].m_listChar;
		while ((pChar = pnext))
		{
			pnext = pChar->m_pCellNext;
			if (GET_YLAYER(pChar) != y)
				continue ;

			if (pChar != ch && !DEAD(ch))
			{
				float p_X, p_Z, p_H;
				p_X = ABS(GET_X(pChar) - x);
				p_Z = ABS(GET_Z(pChar) - z);
				p_H = ABS(GET_H(pChar) - h);

				// pChar가 ch의 움직임에 걸리적 거린다
				if (p_X < 0.2 && p_Z < 0.2 && p_H < 0.2)
				{
					float nx, nz, nh;
					nx = GET_X(pChar) - (float)cos(r) * 0.5f;
					nz = GET_Z(pChar) - (float)sin(r) * 0.5f;
					nh = GetHeight(y, nx, nz);

					float dist= GetDistance(x, GET_X(ch), z, GET_Z(ch), h, GET_H(ch));
					float ndist = GetDistance(nx, GET_X(ch), nz, GET_Z(ch), nh, GET_H(ch));

					if ( dist > ndist )
					{
						r += GetRandom((int)(-PI_HALF * 1000), (int)(PI_HALF * 1000)) / 2000.0f;

						// 높이적용
						x = GET_X(ch) + (float)cos(r) * 0.5f;
						z = GET_Z(ch) + (float)sin(r) * 0.5f;
						h = GetHeight(GET_YLAYER(ch), x, z);

						float trh = (float)asin((h - GET_H(ch)) / (GetDistance(x, GET_X(ch), z, GET_Z(ch), h, GET_H(ch)) + 0.1f));

						x = GET_X(ch) + (float)cos(r) * 0.5 * (float)cos(trh);
						z = GET_Z(ch) + (float)sin(r) * 0.5 * (float)sin(trh);
						h = GetHeight(y, x, z);

						// delay를 반으루
						npc->m_delay = 0;
						change_position = true;
					}
					// 걸리적 거리는 넘이 타겟이면 이동한 척
					if (target == pChar)
						return true;
				}
			}
		}

		// 좌표 바끈데두 블럭인 경우
		if (change_position && !IsNotBlocked(npc, y, x, z, false, outY))
			return false;

		y = outY;

		// 안보이는 상태이면
		if (npc->m_disableTime != 0)
		{
			npc->Move(x, z, h, r, y);
			return true;
		}
	} // 몹일때

	// 이동시 스킬 해제되는 것 처리
	ch->m_assist.CancelFakeDeath();

	// 셀 안 이동과 셀 이동 구분 처리

	// 셀 안 이동
	if (ch->m_cellX == cx && ch->m_cellZ == cz)
	{
		{
			ch->Move(x, z, h, r, y);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeMoveMsg(rmsg, movekind, ch);
			SendToCell(rmsg, ch);
		}

		if (IS_PC(ch))
		{
#ifdef FREE_PK_SYSTEM
			if( gserver->m_bFreePk )
			{
				if(!pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && !pc->IsInPeaceZone(true))
				{
					pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
					pc->SetPlayerState(PLAYER_STATE_PKMODE);
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeAction(rmsg, pc, 0, AGT_PKMODE);
					SendToCell(rmsg, ch, true);
				}
			}
#endif
			// 블럭아닌 피스존으로 들어가면 PKMODE 해제
			if (pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && pc->IsInPeaceZone(false))
			{
				pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeAction(rmsg, pc, 0, AGT_PKMODE);
				SendToCell(rmsg, ch, true);
			}
		}
	}
	else
	{
		int ocx = ch->m_cellX;
		int ocz = ch->m_cellZ;

		CharFromCell(ch, false);
		ch->Move(x, z, h, r, y);
		CharToCell(ch, y, cx, cz);

		// 이동 메시지...
		CNetMsg::SP mMoveMsg(new CNetMsg);
		ResponseClient::makeMoveMsg(mMoveMsg, movekind, ch);

		int i, j;
		int sx, tx, sz, tz;
		bool bwas, bnew;
		if (ocx < cx)
		{
			sx = ocx;
			tx = cx;
		}
		else
		{
			tx = ocx;
			sx = cx;
		}
		if (ocz < cz)
		{
			sz = ocz;
			tz = cz;
		}
		else
		{
			tz = ocz;
			sz = cz;
		}

		if( sz > 10000 || tx > 10000 || sz > 10000 || tz > 10000 )
		{
			GAMELOG << init("MOVE ERROR") << "sx,sz,tx,tx : " << sx << delim << sz << delim << tx << delim << tx << end;
			return false;
		}

		for (i = sx - CELL_EXT; i <= tx + CELL_EXT; i++)
		{
			if (i < 0 || i >= m_size[0]) continue ;

			for (j = sz - CELL_EXT; j <= tz + CELL_EXT; j++)
			{
				if (j < 0 || j >= m_size[1]) continue ;

				// 원래 있던 방
				bwas = (ABS(ocx - i) <= CELL_EXT && ABS(ocz - j) <= CELL_EXT);
				// 지금 있는 방
				bnew = (ABS(cx - i) <= CELL_EXT && ABS(cz - j) <= CELL_EXT);

				if (!bwas && !bnew)
					continue;

				CCharacter* pChar;
				CCharacter* pNext = m_cell[i][j].m_listChar;
				while ((pChar = pNext))
				{
					pNext = pChar->m_pCellNext;
					if (pChar == ch)
						continue ;

					// 원래 있던 셀
					if (bwas)
					{
						// 지금도 있는 셀
						if (bnew)
						{
							// 상대에게 내가 움직임을 알림
							if (IS_PC(pChar))
							{
								SEND_Q(/*movemsg*/mMoveMsg, TO_PC(pChar)->m_desc);
							}
						}

						// 지금은 없는 셀
						else
						{
							// 상대에게 내가 사라짐을 알림
							if (IS_PC(pChar))
							{
								if(IS_APET(ch) && TO_APET(ch)->GetOwner() == pChar)
								{
									continue;
								}
								CNetMsg::SP rmsg(new CNetMsg);
								DisappearMsg(rmsg, ch);
								SEND_Q(rmsg, TO_PC(pChar)->m_desc);
							}

							if (pc)
							{
								// 나에게 상대가 사라짐을 알림
								if(pChar == pc->GetAPet() || pChar == pc->GetPet())
								{
									pChar->Move(GET_X(pc), GET_Z(pc), GET_H(pc), GET_R(pc), GET_YLAYER(pc));
									CNetMsg::SP rmsg(new CNetMsg);
									ResponseClient::makeMoveMsg(rmsg, MSG_MOVE_PLACE, pChar);
									SEND_Q(rmsg, pc->m_desc);
								}
								else
								{
									CNetMsg::SP rmsg(new CNetMsg);
									DisappearMsg(rmsg, pChar);
									SEND_Q(rmsg, pc->m_desc);
								}
							}
						}
					}

					// 이전에 없다가 지금 있는 셀
					else if (bnew)
					{
						// 상대에게 내가 나타남을 알림
						if (IS_PC(pChar))
						{
							{
								CNetMsg::SP rmsg(new CNetMsg);
								AppearMsg(rmsg, ch);
								SEND_Q(rmsg, TO_PC(pChar)->m_desc);
							}
							if( pc )
							{
								if( pc->m_guildInfo && pc->m_guildInfo->guild() )
								{
									CNetMsg::SP rmsg(new CNetMsg);
									GuildPointRankingMsg( rmsg, pc->m_index, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->GetGuildPointRanking() );
									SEND_Q(rmsg, TO_PC(pChar)->m_desc);
								}
							}
							if (pc)
							{
								CNetMsg::SP rmsg(new CNetMsg);
								GuildNameColorStateMsg(rmsg, pc);
								SEND_Q(rmsg, TO_PC(pChar)->m_desc);
							}
						}
						// 나에게 상대의 등장을 알림
						/*speed_ksw before*/
						//AppearMsg(tappmsg, pChar);
						/*speed_ksw before*/
						if (pc)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							AppearMsg(rmsg, pChar);
							SEND_Q(rmsg, pc->m_desc);
							CPC* tpc = TO_PC(pChar);
							if( tpc )
							{
								if( tpc->m_guildInfo && tpc->m_guildInfo->guild() )
								{
									CNetMsg::SP rmsg(new CNetMsg);
									GuildPointRankingMsg( rmsg, tpc->m_index, tpc->m_guildInfo->guild()->index(), tpc->m_guildInfo->guild()->GetGuildPointRanking() );
									SEND_Q(rmsg, pc->m_desc);
								}
							}
							if( TO_PC(pChar) )
							{
								CNetMsg::SP rmsg(new CNetMsg);
								GuildNameColorStateMsg(rmsg, TO_PC(pChar) );
								SEND_Q(rmsg, pc->m_desc);
							}
						}
					}
				} // end while

				CItem* pItem;
				CItem* pItemNext = m_cell[i][j].m_listItem;
				while (IS_PC(ch) && (pItem = pItemNext))
				{
					pItemNext = pItem->m_pCellNext;
					if (bnew && !bwas)
					{
						// 나타남
						CNetMsg::SP rmsg(new CNetMsg);
						ItemAppearMsg(rmsg, pItem);
						SEND_Q(rmsg, pc->m_desc);
					}
					else if (!bnew && bwas)
					{
						// 없어짐
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDisappearMsg(rmsg, pItem->getVIndex());
						SEND_Q(rmsg, pc->m_desc);
					}
				} // end while
			} // end for z
		} // end for x

		if (IS_PC(ch))
		{
#ifdef FREE_PK_SYSTEM
			if( gserver->m_bFreePk )
			{
				if(!pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && !pc->IsInPeaceZone(true))
				{
					pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
					pc->SetPlayerState(PLAYER_STATE_PKMODE);
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeAction(rmsg, pc, 0, AGT_PKMODE);
					SendToCell(rmsg, ch, true);
				}
			}
#endif

			// 블럭아닌 피스존으로 들어가면 PKMODE 해제
			if (pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && pc->IsInPeaceZone(false))
			{
				pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeAction(rmsg, pc, 0, AGT_PKMODE);
				SendToCell(rmsg, ch, true);
			}
		}
	}

	if (IS_PC(ch))
	{
		if (pc->IsParty())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyInfoMsg(rmsg, pc, true);
			pc->m_party->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, rmsg);
		}

		if (pc->IsExped())
		{
			const CExpedMember* pMember = pc->m_Exped->GetMemberByCharIndex(pc->m_index);
			if(pMember)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedInfoMsg(rmsg, pc, pMember->GetGroupType(), true);
				pc->m_Exped->SendToAllPC(rmsg);
			}
		}
	}

	if (IS_PET(ch))
	{
		CPet* pet = TO_PET(ch);
		if (pet->GetOwner())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, pet->GetOwner()->m_desc);
		}
	}

	return true;
}

bool CArea::SendToCell(CNetMsg::SP& msg, CCharacter* ch, bool withme, int cellext)
{
	if (!IS_IN_CELL(ch))
		return false;

	int sx = ch->m_cellX - cellext;
	int ex = ch->m_cellX + cellext;
	int sz = ch->m_cellZ - cellext;
	int ez = ch->m_cellZ + cellext;

	int x, z;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue ;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue ;

			CCharacter* pChar = m_cell[x][z].m_listChar;
			while (pChar)
			{
				if (IS_PC(pChar) && (pChar != ch || withme))
				{
					if (IS_PET(ch))
					{
						CPet* pet = TO_PET(ch);
						if (pet->GetOwner())
						{
							if (!withme && pet->GetOwner() == pChar)
								goto CONTINUE_LOOP;
						}
					}

					SEND_Q(msg, TO_PC(pChar)->m_desc);
				}
CONTINUE_LOOP:
				pChar = pChar->m_pCellNext;
			}
		}
	}
	return true;
}

bool CArea::SendToCell(CNetMsg::SP& msg, char /* y */, int cx, int cz, int cellext)
{
	int sx = cx - cellext;
	int ex = cx + cellext;
	int sz = cz - cellext;
	int ez = cz + cellext;

	int x, z;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue;

			CCharacter* pChar = m_cell[x][z].m_listChar;
			while (pChar)
			{
				if (IS_PC(pChar))
				{
					SEND_Q(msg, TO_PC(pChar)->m_desc);
				}
				pChar = pChar->m_pCellNext;
			}
		}
	}

	return true;
}

// 선공 npc의 경우 가까운 target 찾기
CCharacter* CArea::FindNearestTarget(CNPC* npc, float* dist)
{
	// 가까운 공격대상 찾기
	int cx, cz;
	PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);

	int sx = (cx - CELL_EXT < 0) ? 0 : cx - CELL_EXT;
	int ex = (cx + CELL_EXT < m_size[0]) ? cx + CELL_EXT : m_size[0] - 1;
	int sz = (cz - CELL_EXT < 0) ? 0 : cz - CELL_EXT;
	int ez = (cz + CELL_EXT < m_size[1]) ? cz + CELL_EXT : m_size[1] - 1;
	CCell* cell;

	float tdist = npc->m_proto->m_sight;

	CCharacter* ret = NULL;
	int towerSkillIndex = -1;	// 공성탑 스킬 인덱스

	int i, j;
	for (i = sx; i <= ex; i++)
	{
		for (j = sz; j <= ez; j++)
		{
			cell = &m_cell[i][j];
			if(npc->m_proto->CheckFlag(NPC_FIRSTATTACK) && cell->m_nCountPC <= 0)
				continue;

			CCharacter* pChar;
			CCharacter* pCharNext = cell->m_listChar;
			CPC* owner = NULL;			// pChar가 PC이면 자신, Pet이나 소환수면 owner

			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				owner = NULL;

				// 죽은 것도 아니고
				if (DEAD(pChar))
					continue ;
//#ifdef RAID_MONSTER_SKIP_FAKEDEATH
//				// 레이드 몹일 경우  죽은척 하고 잇을때도 공격
//				if (!npc->m_proto->CheckFlag(NPC_RAID))
//				{
//#endif // RAID_MONSTER_SKIP_FAKEDEATH
				// 죽은 척 하고 있지 않으면
				if (pChar->m_assist.m_state & AST_FAKEDEATH)
					continue ;
//#ifdef RAID_MONSTER_SKIP_FAKEDEATH
//				}
//#endif // RAID_MONSTER_SKIP_FAKEDEATH

				///*speed_ksw before*/
				//// 안보이면 skip
				//if (!npc->CanVisible(pChar))
				//	continue ;

				//// 시야 밖이거나 이전 값보다 멀면 스킵
				//if (tdist < GetDistance(pChar, npc))
				//	continue ;
				///*speed_ksw before*/
				// PC일 경우
				if (IS_PC(pChar))
				{
					CPC* pc = TO_PC(pChar);

					// immortal도 아니고
					if ( (pc->m_bImmortal && pc->m_bFirstDoIt == false)|| pc->IsDisable() )
						continue ;

					// 변신도 스킵
					if (pc->IsSetPlayerState(PLAYER_STATE_CHANGE))
						continue ;
					owner = pc;
				}

				// Pet일 경우
				else if (IS_PET(pChar))
				{
					CPet* pet = TO_PET(pChar);

					// 주인 없는 펫은 스킵
					if (pet->GetOwner() == NULL)
						continue ;

					owner = pet->GetOwner();
				}

				// 소환수
				else if (IS_ELEMENTAL(pChar))
				{
					CElemental* pElemental = TO_ELEMENTAL(pChar);
					CPC* pc = pElemental->GetOwner();
					if (!pc)
						continue ;

					// immortal도 아니고
					if ( pc->m_bImmortal || pc->IsDisable() )
						continue ;

					owner = pc;
				}
				else if ( IS_APET(pChar) )
				{
					CAPet* apet = TO_APET(pChar);

					if( apet->GetOwner() == NULL )
						continue;
					if( !apet->IsProtoFlag(APET_FLAG_ATTACK) )
						continue;
					owner = apet->GetOwner();
				}

				// npc 일때 각 state 에 따른 행동
				else if ( IS_NPC(pChar)  )
				{
					CNPC* pNpc = TO_NPC(pChar);

					if(!pNpc)
						continue;
					/* speed_ksw before */
					if( (npc->m_pZone->m_index == ZONE_COMBO_DUNGEON || npc->m_pZone->m_index == ZONE_EXTREME_CUBE)
							&& !npc->GetOwner() && npc->CanAttack() && pNpc->GetOwner())
					{
					}
					else if( npc->Check_MobFlag(STATE_MONSTER_SUICIDE) || npc->Check_MobFlag(STATE_MONSTER_TRAP) )
					{
						if( IS_NPC(pChar) && TO_NPC(pChar)->m_idNum == 491 ) // 바닦 데미지를 타겟으로 잡아 움직이지 않으니 넌 패스
							continue;

						if( pNpc->GetOwner() )
							owner = pNpc->GetOwner();
					}
					else
						// 일반 몬스터는 스킵
						if ( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
						{
							continue;
						}

					// 혼란 몬스터도 스킵
						else if( pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
						{
							continue;
						}

					// 테이밍된 몬스터는 공격
						else if( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
						{
						}
						else if( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
						{
							continue;
						}

						else if( pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
						{
							continue;
						}

						else if( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && pNpc->Check_MobFlag( STATE_MONSTER_MERCENARY ) )
						{
						}
						else if( pNpc->Check_MobFlag( STATE_MONSTER_TOTEM_BUFF) || pNpc->Check_MobFlag( STATE_MONSTER_TOTEM_ATTK) )
						{
							continue;
						}
					// 버그일 경우 이곳으로 오고 무조건 스킵
						else
						{
							// NPC_JUNK Log
							GAMELOG << init("NPC_JUNK")
									<< "Target_npc Index" << delim
									<< pNpc->m_index << delim
									<< "Target_npc Name" << delim
									<< pNpc->m_name << delim
									<< end;

							continue;
						}
				}
				// 이도 저도 아니면 스킵
				else
					continue ;

				if( npc->GetOwner() && ( npc->Check_MobFlag(STATE_MONSTER_TRAP) || npc->Check_MobFlag(STATE_MONSTER_SUICIDE) ) )
				{
					if( owner && ( npc->GetOwner() == owner || !( IsRaList(owner, npc->GetOwner()) || IsRaList(npc->GetOwner(),owner) ) ) )
					{
						continue;
					}
					else
					{
						int zoneindex = npc->GetOwner()->m_pZone->m_index;
						CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);

						if( castle != NULL && castle->GetState() != WCSF_NORMAL // 공성 중이고
								&& npc->GetOwner()->GetMapAttr() & MATT_WAR  // 공성 지역이고
								&& ( IS_ATTACK_TEAM( npc->GetOwner()->GetJoinFlag( npc->GetOwner()->m_pZone->m_index ) )
									 || IS_DEFENSE_TEAM( npc->GetOwner()->GetJoinFlag( npc->GetOwner()->m_pZone->m_index ) ) ) ) // 오너가 공성에 참여해 있다면
						{
							if(owner)
							{
								if( IS_ATTACK_TEAM(npc->GetOwner()->GetJoinFlag(npc->GetOwner()->m_pZone->m_index) ) )// 공성 측이라면
								{
									if( !IS_DEFENSE_TEAM( owner->GetJoinFlag(npc->GetOwner()->m_pZone->m_index) )  )
										continue;
								}
								else
								{
									if( !IS_ATTACK_TEAM( owner->GetJoinFlag(npc->GetOwner()->m_pZone->m_index) )  )
										continue;
								}
							}
							else if(IS_NPC(pChar) && IS_DEFENSE_TEAM(npc->GetOwner()->GetJoinFlag(npc->GetOwner()->m_pZone->m_index) ) ) // 수성 측이라면
							{
								if( TO_NPC(pChar)->m_proto->CheckFlag(NPC_WARCASTLE) )
									continue;
							}
						}
					}
				}

				// 수호병은 공성 용병/길드만 공격
				if (npc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
				{
					bool bTarget = false;
					if(owner != NULL)
					{
						switch (owner->GetJoinFlag(m_zone->m_index))
						{
						case WCJF_ATTACK_GUILD:
						case WCJF_ATTACK_CHAR:
							bTarget = true;
							break;
						}
					}
					if (!bTarget)
						continue ;
				}

				// 수호탑은 스킬 최소거리 이상되는 적에게만 공격 가능
				// 스킬 사용 가능 검사
				int sloop = -1;
				if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
				{
					for (sloop = 0; sloop < MAX_NPC_SKILL; sloop++)
					{
						// 스킬 사용 가능 검사
						if (npc->m_skills[sloop]
								&& (sloop == 0 || towerSkillIndex == -1 || npc->m_proto->m_skillProb[sloop] >= GetRandom(1, 10000))
								&& npc->m_skills[sloop]->IsReady(npc)
								&& npc->CanSpell()
								&& npc->m_hp > npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level)->m_needHP
								&& npc->m_mp > npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level)->m_needMP
								&& pChar->CanApplySkill(npc->m_skills[sloop]->m_proto, npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level))
								&& GetDistance(npc, pChar) <= npc->m_skills[sloop]->m_proto->GetFireRange(npc)
								&& GetDistance(npc, pChar) >= npc->m_skills[sloop]->m_proto->GetMinRange())
						{
							break ;
						}
					}
					// 스킬 없으면 수호탑은 타겟 없음
					if (sloop == MAX_NPC_SKILL)
						continue ;
				}
				/* speed_ksw after */
				// 안보이면 skip
				if (!npc->CanVisible(pChar))
					continue ;

				// 시야 밖이거나 이전 값보다 멀면 스킵
				if (tdist < GetDistance(pChar, npc))
					continue ;
				/* speed_ksw after */

				// 시야 안에 들어왔으면 타겟으로
				char outY;

				// npc -> pChar 시야안에서 발견 시 npc와 pChar의 직선경로에서 블럭이 있는 경우
				if (!IsNotBlocked(npc, pChar, true, outY))
					continue;

				tdist = GetDistance(pChar, npc);
				ret = pChar;

				// 수호탑 아니면 타겟 잡히는대로 리턴, 수호탑은 스킬 번호 저장
				if (!npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
					return ret;
				else
					towerSkillIndex = sloop;
			} // end : while ((pChar = pCharNext))
		}
	}

	if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
	{
		// 수호탑은 스킬 없으면 타겟도 없다
		if (towerSkillIndex == -1)
			return NULL;
		// 수호탑일 경우 스킬 세팅도 함께
		npc->m_currentSkill = npc->m_skills[towerSkillIndex];
		npc->m_currentSkill->m_targetType = ret->m_type;
		npc->m_currentSkill->m_targetIndex = ret->m_index;
	}

	if (ret)
	{
		*dist = GetDistance(ret, npc);
	}

	return ret;
}

CCharacter* CArea::FindNearestTarget_Taming_or_Confused(CNPC* npc, float* dist) // 테이밍된 npc가 가까운 npc를 찾기
{
	// 가까운 공격대상 찾기
	int cx, cz;
	PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);

	int sx = (cx - CELL_EXT < 0) ? 0 : cx - CELL_EXT;
	int ex = (cx + CELL_EXT < m_size[0]) ? cx + CELL_EXT : m_size[0] - 1;
	int sz = (cz - CELL_EXT < 0) ? 0 : cz - CELL_EXT;
	int ez = (cz + CELL_EXT < m_size[1]) ? cz + CELL_EXT : m_size[1] - 1;
	CCell* cell;

	float tdist = npc->m_proto->m_sight;

	int i, j;
	for (i = sx; i <= ex; i++)
	{
		for (j = sz; j <= ez; j++)
		{
			cell = &m_cell[i][j];

			CCharacter* pChar;
			CCharacter* pCharNext = cell->m_listChar;

			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if(!pChar)
					continue;

				// 죽은 것도 아니고
				if (DEAD(pChar))
					continue ;

				// 안보이면 skip
				if (!npc->CanVisible(pChar))
					continue ;

				// 시야 밖이거나 이전 값보다 멀면 스킵
				if (tdist < GetDistance(pChar, npc))
					continue ;

				// 시야 안에 들어왔으면 타겟으로
				char outY;

				// npc -> pChar 시야안에서 발견 시 npc와 pChar의 직선경로에서 블럭이 있는 경우
				if (!IsNotBlocked(npc, pChar, true, outY))
					continue;

				// 만약 PC가 타겟이면 테이밍된 몬스터가 힐을 쓸 때만 사용
				if (IS_PC(pChar))
				{
					// 몬스터가 혼란 중이면 스킵
					if ( npc->Check_MobFlag(STATE_MONSTER_CONFUSION) )
					{
						continue;
					}
					else
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}
				}

				// 테이밍 되지 않은 NPC만 공격
				else if (IS_NPC(pChar))
				{
					CNPC* target_is_npc = NULL;
					target_is_npc = TO_NPC(pChar);

					// 안보이는 상태이면 스킵
					if ( npc->m_disableTime != 0 )
					{
						continue;
					}

					// 몬스터 자신이면 스킵
					if ( target_is_npc == npc )
					{
						continue;
					}

					// 몬스터가 혼란 중이라면 혼란 중인 몬스터만 공격
					else if ( target_is_npc->Check_MobFlag(STATE_MONSTER_CONFUSION)
							  && npc->Check_MobFlag(STATE_MONSTER_CONFUSION) )
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}
					// 혼란 중이 아니면 테이밍 중이므로 일반 몬스터 공격
					else if ( !target_is_npc->Check_MobFlag(STATE_MONSTER_TAMING) && npc->Check_MobFlag(STATE_MONSTER_TAMING) )
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}
					else if ( !target_is_npc->Check_MobFlag(STATE_MONSTER_MERCENARY) && npc->Check_MobFlag(STATE_MONSTER_TAMING) )
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}

					// 위의 조건을 만족 못하면 새로운 타겟을 찾는다
					else
					{
						continue;
					}
				}
			} // end : while ((pChar = pCharNext))
		}
	}
	return NULL;
}

CNPC* CArea::FindProtoNPCInCell(CCharacter* ch, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead)
{
	if (!IS_IN_CELL(ch))
		return NULL;

	return FindProtoNPCInCell(GET_X(ch), GET_Z(ch), nData, bIsFlagMask, findNumCell, bIncludeDead);
}

CNPC* CArea::FindProtoNPCInCell(float fx, float fz, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead)
{
	int nCellX = 0;
	int nCellZ = 0;
	PointToCellNum(fx, fz, &nCellX, &nCellZ);

	int sx = nCellX - findNumCell;
	int ex = nCellX + findNumCell;
	int sz = nCellZ - findNumCell;
	int ez = nCellZ + findNumCell;

	int x, z;

	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (!IS_NPC(pChar) )
					continue;

				CNPC* npc = TO_NPC(pChar);
				if (bIsFlagMask)
				{
					if ( npc->m_proto->CheckFlag(nData) && (!DEAD(npc) || bIncludeDead))
						return npc;
				}
				else
				{
					if ( npc->m_proto->m_index == nData && (!DEAD(npc) || bIncludeDead))
						return npc;
				}
			}
		}
	}

	return NULL;
}

void CArea::RunComboZone()
{
	if(!m_monsterCombo)
		return ;

	// ret: 0 정상완료  1 진행중 2 오류
	char ret;
	ret = m_monsterCombo->Run();

	switch(ret)
	{
	case 2:
		{
			GAMELOG << init("CLOSE COMBO AREA")
					<< "BY Combo Run() Error"
					<< "AREA INDEX: " << m_index
					<< "COMBO INDEX: " << m_monsterCombo->m_nIndex
					<< end;

			CloseComboZone();

			SetDisable();
		}
		break;
	}
}

int CArea::CountPCInExtra(int extra, bool bIncludeDead)
{
	int count = 0;
	CCharacter* pChar;
	CCharacter* pCharNext;
	int i,j;
	for(i = 0; i < m_size[0]; ++i)
	{
		for(j = 0 ; j < m_size[1]; ++j)
		{
			pCharNext = m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				if(!IS_PC(pChar))
					continue;

				if(TO_PC(pChar)->m_gmGoZoneType == GM_GOZONE_COMBO)
					continue;

				if( (!bIncludeDead && DEAD(pChar)) )
					continue;

				if( ((int)(pChar->m_pos.m_x) >= pChar->m_pZone->m_zonePos[extra][1]/2) && ((int)(pChar->m_pos.m_x) <= pChar->m_pZone->m_zonePos[extra][3]/2) &&
						((int)(pChar->m_pos.m_z) >= pChar->m_pZone->m_zonePos[extra][2]/2) && ((int)(pChar->m_pos.m_z) <= pChar->m_pZone->m_zonePos[extra][4]/2) &&
						((int)(pChar->m_pos.m_yLayer) == pChar->m_pZone->m_zonePos[extra][0]))
					count++;
			}
		}
	}

	return count;
}

int CArea::CountMobInCombo()
{
	int count = 0;
	CNPC* npc;
	CNPC* npcNext = m_npcList;

	while((npc = npcNext))
	{
		npcNext = npc->m_pNPCNext;

		// 평화몹에 있는 npc 이거나 이펙트 npc 인경우 제외
		if(npc->m_proto->CheckFlag(NPC_PEACEFUL) || npc->m_proto->m_index == 491)
			continue;

		if( npc->GetOwner() && !npc->Check_MobFlag(STATE_MONSTER_TAMING | STATE_MONSTER_PARASITE) )
			continue;

		count++;
	}
	return count;
}

void CArea::CloseComboZone()
{
	// 캐릭터 드라탄으로 이동
	if(m_zone->m_index != ZONE_COMBO_DUNGEON)
		return ;

	CCharacter* pChar;
	CCharacter* pCharNext;
	CZone*		pZone;

	std::vector<CPC*> listPC;

	int zone	= ZONE_DRATAN;
	int extra	= 0;
	int i, j;

	for(i = 0 ; i < m_size[0]; ++i)
	{
		for(j = 0; j < m_size[1]; ++j)
		{
			pCharNext = m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;

				if(IS_PC(pChar))
					listPC.push_back(TO_PC(pChar));
			}
		}
	}

	std::vector<CPC*>::iterator it = listPC.begin();
	std::vector<CPC*>::iterator endit = listPC.end();
	for(; it != endit; ++it)
	{
		pChar = *(it);

		pZone = gserver->FindZone(zone);
		if (pZone == NULL)
			return;

		GoZone((CPC*)pChar, zone,
			   pZone->m_zonePos[extra][0],														// ylayer
			   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
	}

	listPC.clear();
}

void CArea::GotoAllClient_StartPoint()
{
	int cx, cz;

	CZone* pZone = gserver->FindZone(ZONE_MONDSHINE);
	if (pZone == NULL)
		return;

	// 먼저 이동시킬 유저를 선별후 이동 // 다른셀로 이동시 또는 이동중 삭제시 문제 발생
	std::vector<CPC*> m_MovePCList;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCell& rCell = m_cell[cx][cz];
			CCharacter* pCharNext = rCell.m_listChar;
			CCharacter* pChar;
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if (IS_PC(pChar) && !DEAD(pChar))
				{
					CPC* pPC = TO_PC(pChar);
					if (pPC)
					{
						m_MovePCList.push_back( pPC );
						
						//타겟 초기화(클라이언트 요청에의해)
						pPC->m_targetPC = NULL;
						CNetMsg::SP rmsg(new CNetMsg);
						TargetClearMsg(rmsg, -1);
						SEND_Q(rmsg, pPC->m_desc);
					}
				}
			}
		}
	}

	std::vector<CPC*>::iterator it = m_MovePCList.begin();
	std::vector<CPC*>::iterator endit = m_MovePCList.end();	
	for(; it != endit; ++it)
	{
		CPC* movePc = *(it);
		GoZone( movePc, ZONE_MONDSHINE, pZone->m_zonePos[0][0],						// ylayer
				GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,	// x
				GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);	// z
	}

	m_MovePCList.clear();
}

void CArea::ApplySkillToAllClient(CSkill* skill, bool &bApply)
{
	int cx, cz;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCell& rCell = m_cell[cx][cz];
			CCharacter* pCharNext = rCell.m_listChar;
			CCharacter* pChar;
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if (IS_PC(pChar) && !DEAD(pChar))
					ApplySkill( pChar, pChar, skill, -2, bApply);
			}
		}
	}
}

void CArea::Change_NpcRegenRaid( int level, int exceptingNpcIndex )
{
	CNPC* npc;
	CNPC* npcNext = m_npcList;

	// 최대 레벨을 지나면 스킵
	if(!m_zone || m_zone->m_MaxLevelNpcRegen <= level)
		return;

	// npc를 현재 area에서 완전히 제거
	while((npc = npcNext))
	{
		if(!npc)
			break;

		npcNext = npc->m_pNPCNext;

		if(!npc->m_proto)
			continue;

		if( npc->GetOwner() &&
				(npc->Check_MobFlag(STATE_MONSTER_MERCENARY)
				 || npc->Check_MobFlag(STATE_MONSTER_TOTEM_BUFF)
				 || npc->Check_MobFlag(STATE_MONSTER_TOTEM_ATTK)
				 || npc->Check_MobFlag(STATE_MONSTER_TRAP)
				 || npc->Check_MobFlag(STATE_MONSTER_SUICIDE) )
		  )
			continue;

		// 제외해야 하는 npc 검사, 죽은 네임드 npc 여기서 지우면 서버 다운된다..
		if(npc->m_proto->m_index == exceptingNpcIndex)
			continue;

		// 1003 죠슈는 지우는 것 제외... 트리거에 의해 소환이 되므로 리젠 리스트에 없어서~
		if(npc->m_proto->m_index == 1003)
			continue;

		npc->SendDisappearAllInCell(true);

		// 어택리스트에서 삭제
		DelAttackList(npc);

		// npc 삭제
		CharFromCell(npc, true);
		DelNPC(npc);
	}

	// 해당 level의 npc를 리젠한다.
	if (m_zone && m_zone->m_CNpcRegenRaidList)
	{
		for (int i=0; i < m_zone->m_CNpcRegenRaidList[level].m_nCount_regen; i++)
		{
			npc =  gserver->m_npcProtoList.Create(m_zone->m_CNpcRegenRaidList[level].m_regenList_InstantDoungen[i].m_npcIdx, NULL);
			if(!npc)
				continue;

			GET_X(npc) = m_zone->m_CNpcRegenRaidList[level].m_regenList_InstantDoungen[i].m_pos_x;
			GET_Z(npc) = m_zone->m_CNpcRegenRaidList[level].m_regenList_InstantDoungen[i].m_pos_z;
			GET_YLAYER(npc) = m_zone->m_CNpcRegenRaidList[level].m_regenList_InstantDoungen[i].m_y_layer;
			GET_R(npc) = 0;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			int cx, cz;
			AddNPC(npc);
			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			CharToCell(npc, GET_YLAYER(npc), cx, cz);
		}
	}
}

int CArea::GetAllClientCount_Area()
{
	int cx, cz, pcCount=0;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCell& rCell = m_cell[cx][cz];
			CCharacter* pCharNext = rCell.m_listChar;
			CCharacter* pChar;
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if ( IS_PC(pChar) )
					pcCount++;
			}
		}
	}

	return pcCount;
}

int CArea::GetPCCount()
{
	int totalCount = 0;
	int cx, cz;

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCharacter* pChar = m_cell[cx][cz].m_listChar;
			while(pChar)
			{
				if (IS_PC(pChar))
					totalCount++;
				pChar = pChar->m_pCellNext;
			}
		}
	}

	return totalCount;
}

void CArea::SetFreeze(int m_pulse)
{
	m_bFreeze = true;
	m_freezePulse = m_pulse + 10 RL_SEC;
}
void CArea::SetMelted(void)
{
	m_bFreeze = false;
	m_freezePulse = 0;
}
bool CArea::IsFreeze(int m_pulse)
{
	if ( m_bFreeze && m_freezePulse < m_pulse )
		return true;
	return false;
}

int CArea::findNpcVIndex( int index )
{
	CNPC* npc = this->m_npcList;
	while(npc != NULL)
	{
		npc = npc->m_pNPCNext;

		if(npc->m_idNum == index)
			return npc->m_index;
	}
	return -1;
}

void CArea::checkLifeTime()
{
	if (m_lifetime_set.empty())
		return;

	std::vector<CNPC*> t_vec;
	std::set<CNPC*>::iterator it = m_lifetime_set.begin();
	std::set<CNPC*>::iterator endit = m_lifetime_set.end();

	for (; it != endit; ++it)
	{
		CNPC* p = *it;
		if (p->m_type == MSG_CHAR_NPC &&
			p->m_lifetime < gserver->getNowSecond())
		{
			t_vec.push_back(p);
		}
	}

	if (t_vec.empty())
		return;
	
	BOOST_FOREACH(CNPC* tp, t_vec)
	{
		if(tp->m_proto != NULL &&
			tp->m_proto->CheckFlag1(NPC1_TOTEM_ITEM))
		{
			if(tp->GetOwner() != NULL)
				tp->SendRemoveAllInCellForClientEffect(tp->GetOwner());
		}
		else
		{
			tp->SendDisappearAllInCell(true);
		}
		
		
		if(tp->GetOwner() != NULL)
		{
			CNPC* sumNpc = tp->GetOwner()->GetSummonNpc(tp);
			if( sumNpc )
			{
#ifdef BUGFIX_MERCNERAY_DELETE
				sumNpc->GetOwner()->SummonNpcRemove(tp, false);
#else
				sumNpc->GetOwner()->SummonNpcRemove(tp);
#endif
			}
		}
		else
		{
			tp->m_hp = 0;
			if (tp->m_pArea)
				tp->m_pArea->m_nNPC_Killed_NPC ++;
		}
	}
}
