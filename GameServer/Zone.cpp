#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Zone.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"

CZone::CZone()
	: m_remoteIP(HOST_LENGTH + 1)
#ifdef IMP_SPEED_SERVER
	, m_zone_exp(100), m_zone_sp(100), m_zone_drop(100)
#endif //IMP_SPEED_SERVER
{
	m_index = -1;
	m_bRemote = false;
	m_remotePort = 0;
	m_countY = 0;
	m_attrMap = NULL;
	m_area = NULL;
	m_countArea = 0;
	m_weather = 0;

	m_zonePos = NULL;

	m_shopList = NULL;
	m_nShopCount = 0;
	m_sellRate = 100;
	m_buyRate = 100;

	m_bCanMemPos = false;
	m_bCanSummonPet = false;
	m_bCanMountPet = false;
//#endif
#ifdef CONTINENT_PARTY_RECALL
	m_iContinent_no = -1;
#endif //CONTINENT_PARTY_RECALL

#ifdef SYSTEM_TREASURE_MAP
	m_flag = 0;
	m_treasureAreaCnt = 0;
	m_treasureAreaRotate = 0;
	m_treasureLinkZoneCnt = 0;
	m_treasureLinkZoneIdx = NULL;
	m_pTreasureBoxInfo = NULL;
#endif
}

CZone::~CZone()
{
	if (m_area)
		delete[] m_area;
	if (m_attrMap)
		delete[] m_attrMap;

	if (m_zonePos)
	{
		for (int i = 0; i < m_countZonePos; i++)
		{
			delete [] m_zonePos[i];
		}
		delete [] m_zonePos;
		m_zonePos = NULL;
	}

	m_index = -1;
	m_bRemote = false;
	m_remotePort = 0;
	m_countY = 0;
	m_attrMap = NULL;
	m_area = NULL;

	if(m_shopList)
		delete [] m_shopList;
	m_shopList = NULL;

	m_nShopCount = 0;
	m_weather = 0;
	m_sellRate = 100;
	m_buyRate = 100;

#ifdef SYSTEM_TREASURE_MAP
	if( m_treasureLinkZoneIdx )
		delete m_treasureLinkZoneIdx;
	if( m_pTreasureBoxInfo )
		delete [] m_pTreasureBoxInfo;
#endif
}

bool CZone::LoadZone(int seq)
{
	if (seq < 0 || seq >= gserver->m_numZone)
		return false;

	GAMELOG << init("SYSTEM")
			<< "Loading zone ";

	CLCString zonegroup(100);
	zonegroup.Format("Zone_%d", seq);

	const char* p;
	int i;

	if (!(p = gserver->m_config.Find(zonegroup, "No")))
		return false;
	m_index = atoi(p);

	GAMELOG << m_index
			<< end;

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	std::string select_zonedata_query = boost::str(boost::format("SELECT * FROM t_zonedata WHERE a_zone_index = %d") % m_index);
	cmd.SetQuery(select_zonedata_query);
	if (!cmd.Open() || !cmd.MoveFirst())
		return false;

	p = gserver->m_config.Find(zonegroup, "Remote");
	if (!p || strcmp(p, "FALSE") == 0)
	{
		m_bRemote = false;

		int sizex, sizez;
		if (!cmd.GetRec("a_width", sizex))
			return false;
		if (!cmd.GetRec("a_height", sizez))
			return false;

		if (!cmd.GetRec("a_ylayer", m_countY))
			return false;

		CLCString attrmaps(65536);
		CLCString heightmaps(65536);

		if (!cmd.GetRec("a_attrmap", attrmaps))
			return false;
		if (!cmd.GetRec("a_heightmap", heightmaps))
			return false;

		if (!cmd.GetRec("a_minlevel", m_minLevel))
			return false;
		if (!cmd.GetRec("a_maxlevel", m_maxLevel))
			return false;

		const char* pattr = attrmaps;
		const char* pheight = heightmaps;

		m_attrMap = new CMapAttr[m_countY];
		char tmpBuf[1024] = {0,};
		char tmpBuf1[1024] = {0,};
		for (i = 0; i < m_countY; i++)
		{
			pattr = AnyOneArg(pattr, tmpBuf);
			pheight = AnyOneArg(pheight, tmpBuf1);

			if (!m_attrMap[i].Load(seq, i, sizex, sizez, tmpBuf, tmpBuf1))
				return false;
		}

#ifdef SYSTEM_TREASURE_MAP
		CLCString treasureArea(256);
		CLCString treasureDungeon(256);
		if( !cmd.GetRec("a_flag", m_flag))
			return false;
		if( !cmd.GetRec("a_treasurecount", m_treasureAreaCnt))
			return false;
		if( !cmd.GetRec("a_treasurecount_dungeon", m_treasureLinkZoneCnt))
			return false;
		if( m_treasureAreaCnt > 0)
		{
			if( !cmd.GetRec("a_treasure_area", treasureArea))
				return false;
			m_pTreasureBoxInfo = new TREASUREBOXINFO[m_treasureAreaCnt];

			const char* ta = treasureArea;
			char left[8];
			char top[8];
			char right[8];
			char bottom[8];
			char regenMax[8];
			int i=0;

			for(i=0; i<m_treasureAreaCnt; i++)
			{
				ta = AnyOneArg(ta, left);
				ta = AnyOneArg(ta, top);
				ta = AnyOneArg(ta, right);
				ta = AnyOneArg(ta, bottom);
				ta = AnyOneArg(ta, regenMax);

				m_pTreasureBoxInfo[i].rect.left = atof(left);
				m_pTreasureBoxInfo[i].rect.top = atof(top);
				m_pTreasureBoxInfo[i].rect.right = atof(right);
				m_pTreasureBoxInfo[i].rect.bottom = atof(bottom);
				m_pTreasureBoxInfo[i].max = atoi(regenMax);
			}
		}

		if( m_treasureLinkZoneCnt == 0)
		{
			m_treasureLinkZoneIdx = NULL;
		}
		else
		{
			if( !cmd.GetRec("a_treasure_dengeon", treasureDungeon))
				return false;

			m_treasureLinkZoneIdx = new int[m_treasureLinkZoneCnt];
			int i=0;
			const char* td = treasureDungeon;
			char dIdx[8];
			for(i=0; i<m_treasureLinkZoneCnt; i++)
			{
				td = AnyOneArg(td, dIdx);
				m_treasureLinkZoneIdx[i] = atoi(dIdx);
			}
		}

#endif
		if( !cmd.GetRec("a_areacount", m_countArea) )
			return false;
		if( m_countArea < 1 )
			return false;

		if(m_index != ZONE_GUILDROOM && m_index != ZONE_SINGLE_DUNGEON_TUTORIAL)
		{
			if( m_countArea > MAX_AREA_COUNT)
			{
				LOG_ERROR("a_areacount over flow... areaCount[%d], maxCount[%d]", m_countArea, MAX_AREA_COUNT);
				return false;
			}
		}
		
		m_area = new CArea[m_countArea];
		for (i = 0; i < m_countArea; i++)
			m_area[i].Init(this, i, sizex, sizez, m_countY);
	}
	else
	{
		m_bRemote = true;

		p = gserver->m_config.Find(zonegroup, "Remote_IP");
		if (!p)
			return false;
		m_remoteIP = p;

		p = gserver->m_config.Find(zonegroup, "Remote_Port");
		if (!p)
			return false;
		m_remotePort = (unsigned short)atoi(p);
	}

	char canusemempos;
	if (!cmd.GetRec("a_canusemempos", canusemempos))
		return false;
	m_bCanMemPos = (canusemempos) ? true : false;

	char cansummonpet;
	if (!cmd.GetRec("a_cansummonpet", cansummonpet))
		return false;
	m_bCanSummonPet = (cansummonpet) ? true : false;

	char canmountpet;
	if (!cmd.GetRec("a_canmountpet", canmountpet))
		return false;
	m_bCanMountPet = (canmountpet) ? true : false;

#ifdef CONTINENT_PARTY_RECALL // 대륙별 파티리콜에 의한 대륙번호 추가.
	int continent_no;
	if(!cmd.GetRec("a_continent_no", continent_no))
		return false;
	m_iContinent_no = continent_no;
#endif //CONTINENT_PARTY_RECALL

	// 존의 마을 위치 읽기
	char poscount;
	CLCString posylayer(256);
	CLCString posleft(256);
	CLCString postop(256);
	CLCString posright(256);
	CLCString posbottom(256);

	if (!cmd.GetRec("a_poscount", poscount))
		return false;
	if (!cmd.GetRec("a_posylayer", posylayer))
		return false;
	if (!cmd.GetRec("a_posleft", posleft))
		return false;
	if (!cmd.GetRec("a_postop", postop))
		return false;
	if (!cmd.GetRec("a_posright", posright))
		return false;
	if (!cmd.GetRec("a_posbottom", posbottom))
		return false;

	const char* pylayer = posylayer;
	const char* pleft = posleft;
	const char* ptop = postop;
	const char* pright = posright;
	const char* pbottom = posbottom;

	m_countZonePos = poscount;
	if (m_index == ZONE_MERAC && m_countZonePos < 17)
	{
		GAMELOG << init("ERROR")
				<< "ZONE MERAC MUST HAVE 17 REGEN POSITION"
				<< end;
		return false;
	}
	m_zonePos = new int*[m_countZonePos];
	for (i = 0; i < m_countZonePos; i++)
	{
		m_zonePos[i] = new int[5];

		char sy[256];
		char sl[256];
		char st[256];
		char sr[256];
		char sb[256];

		pylayer = AnyOneArg(pylayer, sy);
		pleft = AnyOneArg(pleft, sl);
		ptop = AnyOneArg(ptop, st);
		pright = AnyOneArg(pright, sr);
		pbottom = AnyOneArg(pbottom, sb);

		m_zonePos[i][0] = atoi(sy);
		m_zonePos[i][1] = (int)(atof(sl) * 2);
		m_zonePos[i][2] = (int)(atof(st) * 2);
		m_zonePos[i][3] = (int)(atof(sr) * 2);
		m_zonePos[i][4] = (int)(atof(sb) * 2);
	}

	// 레이드 존은 npc 리젠 리스트가 따로 관리된다.
	if ( m_index == ZONE_CAPPELLA_1 || m_index == ZONE_ALTER_OF_DARK || m_index == ZONE_AKAN_TEMPLE
			|| m_index == ZONE_DUNGEON4
			|| m_index == ZONE_TARIAN_DUNGEON
	   )
	{
		GAMELOG << init("SYSTEM")
				<< m_index << delim
				<< " : Load_NpcRegenRaidList()"
				<< end;
		if(!Load_NpcRegenRaidList())
		{
			GAMELOG << init("ERROR")
					<< m_index << delim
					<< " : Load_NpcRegenRaidList() error"
					<< end;
			return false;
		}
	}

#ifdef IMP_SPEED_SERVER
	std::string select_speed_zone_query = boost::str(boost::format("SELECT a_exp, a_sp, a_drop FROM t_speed_zone WHERE a_zone_index = %d") % m_index);
	cmd.SetQuery( select_speed_zone_query );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec("a_exp",m_zone_exp);
		cmd.GetRec("a_exp",m_zone_sp);
		cmd.GetRec("a_exp",m_zone_drop);
	}
#endif //IMP_SPEED_SERVER

	return true;
}

bool CZone::LoadNPC()
{
	if (m_bRemote)
		return true;

	std::vector<tag_npc_regen_info> vec;

	{
		// 모든 존의 NPC 인덱스를 한번에 읽어들여 처리함
		CDBCmd dbNpcRegen;
		dbNpcRegen.Init(&gserver->m_dbdata);
		std::string qry = boost::str(boost::format(
			"SELECT * FROM t_npc_regen WHERE a_zone_num = %d ORDER BY a_index")	% this->m_index);
		dbNpcRegen.SetQuery(qry);
		if (dbNpcRegen.Open() == false)
			return false;

		vec.reserve(dbNpcRegen.m_nrecords);

		tag_npc_regen_info info;

		while (dbNpcRegen.MoveNext())
		{
			dbNpcRegen.GetRec("a_index",		info.index);
			dbNpcRegen.GetRec("a_npc_idx",		info.npcIndex);
			dbNpcRegen.GetRec("a_pos_x",		info.x);
			dbNpcRegen.GetRec("a_y_layer",		info.yLayer);
			dbNpcRegen.GetRec("a_pos_z",		info.z);
			dbNpcRegen.GetRec("a_pos_r",		info.r);
			dbNpcRegen.GetRec("a_regen_sec",	info.regenSec);
			dbNpcRegen.GetRec("a_total_num",	info.totalNum);
	
			info.pNPCProto = gserver->m_npcProtoList.FindProto(info.npcIndex);
			if (info.pNPCProto == NULL)
			{
				LOG_ERROR("Not found NPC[%d] in npc_protolist. zone_no[%d]", info.npcIndex, this->m_index);
				continue;
			}

#if defined (LC_GAMIGO) || defined (LC_BILA) || defined (LC_USA) || defined (LC_RUS)
			if ( (info.npcIndex == 1624 || info.npcIndex == 1620) && gserver->m_subno != 4)
				continue;
#endif

			vec.push_back(info);
		}
	}

	// 각 존의 Area마다 NPC 리젠 정보 읽기
	bool setenable_flag = false;

#ifdef EXTREME_CUBE
	if(IsExtremeCube())
		setenable_flag = true;
#endif

	if(IsWarGroundZone())
		setenable_flag = true;

	for (int i = 0; i < m_countArea; i++)
	{
		m_area[i].LoadNPC(vec);

		if (setenable_flag)
			m_area[i].SetEnable();		

		if ( CanFreezeZone() )
			m_area[i].SetFreeze(gserver->m_pulse);
	}

	if (m_countArea == 1)
		m_area[0].SetEnable();

	return true;
}

bool CZone::LoadShop()
{
	bool bRet = true;

	if (m_bRemote)
		return true;

	if (m_shopList)
		delete [] m_shopList;
	m_shopList = NULL;

	GAMELOG << init("SYSTEM")
			<< "Shop Loading Zone "
			<< m_index
			<< end;

	// area가 속한 해당 존 넘버의 Shop정보 READ
	CDBCmd dbShop;
	dbShop.Init(&gserver->m_dbdata);
	std::string select_shop_query = boost::str(boost::format("SELECT * FROM t_shop WHERE a_zone_num = %d ORDER BY a_keeper_idx") % m_index);
	dbShop.SetQuery(select_shop_query);
	dbShop.Open();

	m_nShopCount = dbShop.m_nrecords;

	if (m_nShopCount < 1)
		return true;

	m_shopList = new CShop[m_nShopCount];

	int idx = 0;

	CDBCmd dbShopItem;
	dbShopItem.Init(&gserver->m_dbdata);

	//CNPC* npc;
	while (dbShop.MoveNext())
	{
		dbShop.GetRec("a_keeper_idx",	m_shopList[idx].m_keeperIdx);
		dbShop.GetRec("a_sell_rate",	m_shopList[idx].m_sellRate);
		dbShop.GetRec("a_buy_rate",		m_shopList[idx].m_buyRate);

		dbShop.GetRec("a_pos_x",		m_shopList[idx].m_x);
		dbShop.GetRec("a_pos_z",		m_shopList[idx].m_z);
		dbShop.GetRec("a_pos_r",		m_shopList[idx].m_r);
		dbShop.GetRec("a_pos_h",		m_shopList[idx].m_h);
		dbShop.GetRec("a_y_layer",		m_shopList[idx].m_yLayer);

		CLCString sql(2048);

#ifdef LC_KOR
		sql.Format(
			"SELECT * FROM t_shopitem where a_keeper_idx = %d ORDER BY a_item_idx"
			, m_shopList[idx].m_keeperIdx);
#elif defined (LC_GAMIGO)
		//가미고는 독일 버전 기준으로 shop 아이템을 통일한다.
		sql.Format(
			"SELECT * FROM t_shopitem where a_keeper_idx = %d ORDER BY a_item_idx"
			, m_shopList[idx].m_keeperIdx, 13);			//LC_GER = 13
#else
		sql.Format(
			"SELECT * FROM t_shopitem where a_keeper_idx = %d and !(a_national & (1 << %d) ) ORDER BY a_item_idx"
			, m_shopList[idx].m_keeperIdx, gserver->m_national);
#endif
		dbShopItem.SetQuery(sql);
		dbShopItem.Open();

		// 상점 보유 아이템
		m_shopList[idx].m_itemCount = dbShopItem.m_nrecords;
		m_shopList[idx].m_itemDBIdx = new int[m_shopList[idx].m_itemCount];

		int itemDBIdx = 0;
		while(dbShopItem.MoveNext())
			m_shopList[idx].m_itemDBIdx[itemDBIdx++] = atoi(dbShopItem.GetRec("a_item_idx"));

		if (m_shopList[idx].m_sellRate < 0 || m_shopList[idx].m_buyRate < 0)
		{
			GAMELOG << init("ERROR: SHOP SELL/BUY RATE")
					<< m_index << delim
					<< m_shopList[idx].m_keeperIdx << delim
					<< m_shopList[idx].m_sellRate << delim
					<< m_shopList[idx].m_buyRate
					<< end;
			bRet = false;
		}

		map_.insert(map_t::value_type(m_shopList[idx].m_keeperIdx, &m_shopList[idx]));

		idx++;
	}

	return bRet;
}

int CZone::SetEnableArea()
{
	int idx;
	for (idx = 0; idx < m_countArea; idx++)
	{
		if (!m_area[idx].m_bEnable)
			break;
	}
	if (idx == m_countArea)
	{
		LOG_INFO("AREA_LOG. ENABLE_ERROR : CountArea[%d], ZoneIndex[%d], AreaIndex[%d]", m_countArea, this->m_index, idx);
		return -1;
	}

	m_area[idx].SetEnable();
	LOG_INFO("AREA_LOG. ENABLE_AREA : CountArea[%d], ZoneIndex[%d], AreaIndex[%d]", m_countArea, this->m_index, idx);

	return idx;
}

void CZone::SetDisableArea(int idx)
{
	if (idx < 0  || idx >= m_countArea)
	{
		LOG_INFO("AREA_LOG. DISABLE_ERROR : CountArea[%d], ZoneIndex[%d], AreaIndex[%d]", m_countArea, this->m_index, idx);
		return ;
	}
	m_area[idx].SetDisable();
	LOG_INFO("AREA_LOG. DISABLE_AREA : CountArea[%d], ZoneIndex[%d], AreaIndex[%d]", m_countArea, this->m_index, idx);
}

// npc index에 따른 shop 찾기
CShop* CZone::FindShop(int npcIdx)
{
	map_t::iterator it = map_.find(npcIdx);
	return (it != map_.end()) ? it->second : NULL;
}

void CZone::ChangeWeather()
{
	int i;
	unsigned char before = m_weather;

	switch (m_weather)
	{
	case 0:// 맑음
		if (!GetRandom(0, 5))
			m_weather = 1;
		break;
	case 1: // 흐림
		i = GetRandom(0, 2);
		if (i == 0)
			m_weather = 0;
		else if (i == 1)
			m_weather = 2;
		break;
	case 2: // 비 조금옴...
		if (!GetRandom(0, 1))
			m_weather = 1;
		else
			m_weather = 3;
		break;
	case 3: // 비 쪼까 옴...
		if (!GetRandom(0, 1))
			m_weather = 2;
		else
			m_weather = 4;
		break;
	case 4: //비 왕창 옴....
		if (!GetRandom(0, 1))
			m_weather = 3;
		break;
	default:
		m_weather = 2;
		break;
	}

	if (before != m_weather)
	{
		for (i=0; i < m_countArea; i++)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EnvWeatherMsg(rmsg, this->m_index);
			this->m_area[i].SendToAllClient(rmsg);
		}
	}
}

int CZone::FindEmptyArea()
{
	int idx;

	for (idx = 0; idx < m_countArea; idx++)
	{
		if (!m_area[idx].m_bEnable)
			return idx;
	}

	return -1;
}

int CZone::FindComboArea(int comboidx)
{
	if(m_index != ZONE_COMBO_DUNGEON)
		return -1;

	int i;
	for(i = 0 ; i < m_countArea; ++i)
	{
		if(!m_area[i].m_bEnable)
			continue;

		if(!m_area[i].m_monsterCombo)
			continue;

		if(m_area[i].m_monsterCombo->m_nIndex == comboidx)
			return i;
	}

	return -1;
}

int CZone::GetExtra(int x, int z)
{
	for (int i = 0; i < m_countZonePos; i++)
	{
		if ((m_zonePos[i][1] / 2) < x && (m_zonePos[i][2] / 2) < z && (m_zonePos[i][3] / 2) > x && (m_zonePos[i][4] / 2) > z)
		{
			return i;
		}
	}

	return -1;
}

bool CZone::isRVRZone()
{
	if(m_index == ZONE_RVR)
		return true;

	return false;
}

#ifdef WARCASTLE_MOVE_MESSAGE_LIMIT
bool CZone::InExtra(int x, int z, int extra)
{
	if( extra < 0 || extra >= m_countZonePos)
		return false;

	if( m_zonePos == NULL )
		return false;

	if (   (m_zonePos[extra][1] / 2) <= x && (m_zonePos[extra][2] / 2) <= z
			&& (m_zonePos[extra][3] / 2) >= x && (m_zonePos[extra][4] / 2) >= z)
		return true;

	return false;
}
#endif // #ifdef WARCASTLE_MOVE_MESSAGE_LIMIT

bool CZone::Load_NpcRegenRaidList()
{
	int i, j;
	CDBCmd dbcmd;
	dbcmd.Init(&gserver->m_dbdata);

	// 1. 존의 리젠 리스트에서 레벨의 개수를 구한다.
	std::string select_npc_regen_raid_query = boost::str(boost::format("SELECT count(a_store_middle) as level FROM t_npc_regen_raid WHERE a_zone_num= %d GROUP BY a_store_middle") % m_index);
	dbcmd.SetQuery(select_npc_regen_raid_query);
	if (!dbcmd.Open())
	{
		GAMELOG << init("LoadNpcRegen_Raid step 1")
				<< end;
		return false;
	}
	if(!dbcmd.MoveFirst())
	{
		GAMELOG << init("LoadNpcRegen_Raid : ZERO")
				<< end;
		return false;
	}

	m_MaxLevelNpcRegen = dbcmd.m_nrecords;
	m_CNpcRegenRaidList = new CNpc_Regen_Raid[m_MaxLevelNpcRegen];

	// 2. 이 zone에 해당하는 regen 정보를 레벨 별로 받아서 저장한다.
	for (i=0; i < m_MaxLevelNpcRegen; i++)
	{
		std::string select_npc_regen_raid_queyr = boost::str(boost::format(
					"SELECT a_npc_idx, a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer, a_store_middle FROM t_npc_regen_raid WHERE a_zone_num= %d AND a_store_middle = %d")
				% m_index % i);
		dbcmd.SetQuery(select_npc_regen_raid_queyr);
		if (!dbcmd.Open())
		{
			GAMELOG << init("LoadNpcRegen_Raid step 2")
					<< end;
			return false;
		}
		dbcmd.MoveFirst();

		m_CNpcRegenRaidList[i].m_nCount_regen = dbcmd.m_nrecords;
		int regenCount = dbcmd.m_nrecords;
		m_CNpcRegenRaidList[i].m_regenList_InstantDoungen = new NPC_REGEN_FOR_RAID[regenCount];

		for (j=0; j < regenCount; j++)
		{
			dbcmd.GetRec("a_npc_idx",		m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_npcIdx);
			dbcmd.GetRec("a_pos_x",			m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_x);
			dbcmd.GetRec("a_pos_z",			m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_z);
			dbcmd.GetRec("a_pos_h",			m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_h);
			dbcmd.GetRec("a_pos_r",			m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_r);
			dbcmd.GetRec("a_y_layer",		m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_y_layer);
			dbcmd.GetRec("a_store_middle",	m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_storeMiddle);

			dbcmd.MoveNext();
		}
	}

	return true;
}


#ifdef SYSTEM_TREASURE_MAP
void CZone::RemoveTreasureBoxNpc(CNPC * pNpc)
{
	if( pNpc == NULL)
		return ;

	if( !m_pTreasureBoxInfo  )
		return ;

	int i;
	for(i=0; i<m_treasureAreaCnt; i++)
	{
		m_pTreasureBoxInfo[i].RemoveTreasureBoxNpc( pNpc );
	}
}

int CZone::NextAreaNum()
{
	if( m_treasureAreaCnt == 0 )
		return -1;
	m_treasureAreaRotate ++;
	if( m_treasureAreaRotate >= m_treasureAreaCnt )
		m_treasureAreaRotate = 0;

	return m_treasureAreaRotate;
}

bool CZone::SummonTreaseureBoxNpc(int& x, int& z, int& zoneNum)
{
	if( !IsFieldZone() )
		return false;

	CZone * pZone = this;

	// 현재 존의 에어리어 설정이 없으면 연계던전으로 떨군다.
	// 연계 존이 있으면 연계존으로 10% 확률로 떨군다.
	if( gserver->bTreasureMapLinkZoneDrop && m_treasureLinkZoneIdx
			&& ( m_treasureAreaCnt == 0
				 || ( m_treasureLinkZoneCnt > 0 && GetRandom(0, 10000) < 1000 ) ) )
	{
		int LinkZoneIndex = GetRandom(0, m_treasureLinkZoneCnt-1 );

		// 연계 존의 인덱스를 가져온다.
		LinkZoneIndex = m_treasureLinkZoneIdx[LinkZoneIndex];
		pZone = gserver->FindZone(LinkZoneIndex);
		if (pZone == NULL)
			return false;

		// 연계존인데 떨굴 수 없는 지역으로 설정한 경우 실패 처리
		if( !pZone->CheckTreasureDropFlag() )
		{
			//return false; // 실패처리하면 떨굴 확률이 낮아지니까. 현재 맵에서 떨구도록 처리해주자.
			pZone = this;
		}
	}

	// 현제 존에 떨굴 지역을 찾아본다.
	int areaNum = pZone->NextAreaNum();
	if( areaNum == -1 )
		return false;

	// 리젠 개수만 큼 꽉 찾다면 랜덤하게 하나의 좌표를 반납 하자.
	if( pZone->m_pTreasureBoxInfo && pZone->m_pTreasureBoxInfo[areaNum].GetEmptyRegenCount() <= 0 )
	{
		int rndNpcNum = GetRandom(0,pZone->m_pTreasureBoxInfo[areaNum].max -1 );
		if (rndNpcNum >= pZone->m_pTreasureBoxInfo[areaNum].listNpc.size())
		{
			return false;
		}

		CNPC* pNpc = pZone->m_pTreasureBoxInfo[areaNum].listNpc[rndNpcNum];
		x = (int)GET_X(pNpc);
		z = (int)GET_Z(pNpc);
		zoneNum = pZone->m_index;
		return true;
	}

	// npc summon
	CNPC* pTBoxNpc = gserver->m_npcProtoList.Create( TREASURE_BOX_NPC_INDEX , NULL);
	if( !pTBoxNpc )
		return false;

	GET_YLAYER(pTBoxNpc) = 0;

	int repeat = 0;
	while(1)
	{
		GET_X(pTBoxNpc) = GetRandom((int)pZone->m_pTreasureBoxInfo[areaNum].rect.left, (int)pZone->m_pTreasureBoxInfo[areaNum].rect.right);
		GET_Z(pTBoxNpc) = GetRandom((int)pZone->m_pTreasureBoxInfo[areaNum].rect.top, (int)pZone->m_pTreasureBoxInfo[areaNum].rect.bottom);

		if( m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_WALKABLE
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_PEACE)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_PRODUCT_PUBLIC)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_PRODUCT_PRIVATE)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_STAIR_UP)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_STAIR_DOWN)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_WAR)
				&& !(m_area->GetAttr(0, GET_X(pTBoxNpc), GET_Z(pTBoxNpc)) & MATT_FREEPKZONE)	)
			break;

		repeat ++;
		if( repeat > 1000 ) // 혹시나.... 툴에서 이상한 지역을 세팅해서 무한 루프를 돌지 모르니... ㅡㅡ;;
		{
			GAMELOG << init("TBNPC REGEN error" )
					<< "zonenum : " << pZone->m_index << delim << "areanum : " << areaNum << end;
			delete pTBoxNpc;
			pTBoxNpc = NULL;
			return false;
		}
	}

	pTBoxNpc->m_regenX = GET_X(pTBoxNpc);
	pTBoxNpc->m_regenZ = GET_Z(pTBoxNpc);
	pTBoxNpc->m_regenY = GET_YLAYER(pTBoxNpc);

	x = (int)pTBoxNpc->m_regenX;
	z = (int)pTBoxNpc->m_regenZ;
	zoneNum = pZone->m_index;

	int cx, cz;
	pZone->m_area->AddNPC(pTBoxNpc);
	pTBoxNpc->m_pArea->PointToCellNum(GET_X(pTBoxNpc), GET_Z(pTBoxNpc), &cx, &cz);
	pTBoxNpc->m_pArea->CharToCell(pTBoxNpc, GET_YLAYER(pTBoxNpc), cx, cz);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, pTBoxNpc, true);
		pTBoxNpc->m_pArea->SendToCell(rmsg, GET_YLAYER(pTBoxNpc), cx, cz);
	}

	pZone->m_pTreasureBoxInfo[areaNum].listNpc.push_back( pTBoxNpc );
	return true;
}

bool CZone::SetTresaureMapInfo(CItem * pItem)
{
	int plus = pItem->getPlus();
	int plus2 = pItem->getPlus_2();
	int used2 = pItem->getUsed_2();

	bool retbool = SummonTreaseureBoxNpc(plus, plus2, used2);

	pItem->setPlus(plus);
	pItem->setPlus_2(plus2);
	pItem->setUsed_2(used2);

	return retbool;
}

TREASUREBOXINFO::TREASUREBOXINFO()
{
	max=0;
}

TREASUREBOXINFO::~TREASUREBOXINFO()
{
}

int	TREASUREBOXINFO::GetEmptyRegenCount()
{
	return max - listNpc.size();
}
bool TREASUREBOXINFO::SetTreasureBoxNpc(CNPC* pNpc)
{
	if( listNpc.size() >= max )
		return false;

	listNpc.push_back(pNpc);
	return true;
}

void TREASUREBOXINFO::RemoveTreasureBoxNpc(CNPC * pNpc)
{
// 	vec_t::iterator it = listNpc.begin();
// 	vec_t::iterator endit = listNpc.end();
// 	for (; it != endit;)
// 	{
// 		if ((*it) == pNpc)
// 		{
// 			it = listNpc.erase(it);
// 			endit = listNpc.end();
// 		}
// 		else
// 		{
// 			++it;
// 		}
// 	}
	listNpc.erase(std::remove(listNpc.begin(), listNpc.end(), pNpc), listNpc.end());
}

#endif // SYSTEM_TREASURE_MAP
//
