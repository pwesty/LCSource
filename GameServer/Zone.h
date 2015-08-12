#ifndef __AZONE_H__
#define __AZONE_H__

#include <map>
#include <vector>
#include "MapAttr.h"
#include "Area.h"
#include "Shop.h"

#ifdef SYSTEM_TREASURE_MAP
class TREASUREBOXINFO;
#endif

class CZone
{
public:
	CZone();
	~CZone();

	int				m_index;		// 존 번호
	bool			m_bRemote;		// 원격 존
	CLCString		m_remoteIP;		// 원격지 주소
	unsigned short	m_remotePort;	// 원격지 포트
	int				m_countY;		// 층 수
	CMapAttr*		m_attrMap;		// 속성 맵
	CArea*			m_area;			// CArea
	int				m_countArea;	// Area 레이어 수 ( >= 0 )
	unsigned char	m_weather;		// 날씨
	bool			m_bCanMemPos;	// 장소 기억 장치 사용 가능 여부
	bool			m_bCanSummonPet;// 펫 소환 가능 여부
	bool			m_bCanMountPet;	// 펫 타기 가능 여부
#ifdef CONTINENT_PARTY_RECALL
	int				m_iContinent_no;//대륙번호 -1:변수INIT 0:아이리스대륙(존0~31) 1:아스카디아 대륙(존32~)
#endif //CONTINENT_PARTY_RECALL

	int				m_minLevel;
	int				m_maxLevel;

	int				m_countZonePos;	// 시작 장소 수
	int**			m_zonePos;		// 시작 장소 위치 : int [count][5]
	// 0부터 y, left, top, right, bottom
	// left, top, right, bottom은 0.5m 단위의 정수
	// Shop 관련
	typedef std::map<int, CShop*> map_t;
	map_t			map_;
	CShop*			m_shopList;		// Shop List
	int				m_nShopCount;	// shop count
	int				m_sellRate;		// sell rate
	int				m_buyRate;		// buy rate

	bool LoadZone(int seq);
	bool LoadNPC();
	bool LoadShop();
	CShop* FindShop(int npcIdx);
	void ChangeWeather();

	int FindEmptyArea();
	int SetEnableArea();
	void SetDisableArea(int idx);

	bool isRVRZone();

	bool IsPersonalDungeon()
	{
		switch (m_index)
		{
		case ZONE_SINGLE_DUNGEON1:
		case ZONE_SINGLE_DUNGEON2:
		case ZONE_SINGLE_DUNGEON_TUTORIAL:
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
			return true;
		default:
			return false;
		}
	}

	bool IsPartyRecallZone()
	{
		if( IsSummonZone() )
			return false;
		if( IsExpedRaidZone() || IsPartyRaidZone() )
			return false;

		switch( m_index)
		{
		case ZONE_GUILDROOM:
			return false;
			break;
		}

		return true;
	}

	bool IsSummonZone()
	{
		switch(m_index)
		{
		case ZONE_SINGLE_DUNGEON1:
		case ZONE_SINGLE_DUNGEON2:
		case ZONE_SINGLE_DUNGEON_TUTORIAL:
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
		case ZONE_OXQUIZROOM:
		case ZONE_FREE_PK_DUNGEON:
		case ZONE_COMBO_DUNGEON:
#ifdef EXTREME_CUBE
		case ZONE_EXTREME_CUBE:
#endif // EXTREME_CUBE
		case ZONE_CAPPELLA_2:
		case ZONE_DRATAN_CASTLE_DUNGEON:
			return true;
		default:
			return false;
		}
	}

	bool IsGuildRoom()
	{
		return (m_index == ZONE_GUILDROOM);
	}

	bool IsOXQuizRoom()
	{
		return (m_index == ZONE_OXQUIZROOM);
	}

	bool IsComboZone()
	{
		return (m_index == ZONE_COMBO_DUNGEON);
	}

	bool IsExtremeCube()
	{
		return (m_index == ZONE_EXTREME_CUBE);
	}

	bool IsPkZone()
	{
		return (m_index == ZONE_PK_TOURNAMENT);
	}

	bool IsWarGroundZone()									// 전장, 로얄럼블
	{
		return (m_index == ZONE_ROYAL_RUMBLE);
	}

	bool IsPersonalRaid()									// 망각의 신전 리뉴얼(퍼스널 레이드)
	{
		return (m_index == ZONE_DUNGEON4);
	}

	bool IsExpedRaidZone()		//암흑의 재단
	{
		switch(m_index)
		{
		case ZONE_ALTER_OF_DARK:
		case ZONE_AKAN_TEMPLE:
		case ZONE_TARIAN_DUNGEON:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	bool IsPartyRaidZone()		//예배당 1, 2
	{
		switch(m_index)
		{
		case ZONE_CAPPELLA_1:
			return true;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	// 공성지역이지만 계단이나 입구는 따로 체크
	bool IsWarZone(int x, int z)
	{
		int extra = GetExtra(x, z);

		switch(m_index)
		{
		case ZONE_MERAC:
			if (extra == 16)
				return true;
		case ZONE_DRATAN:
			if (extra == 26)
				return true;
		default:
			return false;
		}
	}

	int FindComboArea(int comboidx);

	int GetExtra(int x, int z);
#ifdef WARCASTLE_MOVE_MESSAGE_LIMIT
	bool InExtra(int x, int z, int extra);
#endif

	CNpc_Regen_Raid*	m_CNpcRegenRaidList;		// 레이드존이라면 사용되는 레이드용 regen리스트
	int					m_MaxLevelNpcRegen;			// 레이드 엔피씨 리젠의 최고 레벨
	bool				Load_NpcRegenRaidList();	// 해당 area의 레이드 리스트를 가져온다.

	bool	IsFieldZone()
	{
		switch( m_index )
		{
		case ZONE_START:
		case ZONE_DRATAN:
		case ZONE_MERAC:
		case ZONE_EGEHA:
		case ZONE_STREIANA:
		case ZONE_MONDSHINE:
		case ZONE_TARIAN:
		case ZONE_BLOODYMIR:
			return true;
		default:
			return false;
		}
		return false;
	}
	bool CanFreezeZone()
	{
		switch( m_index )
		{
		case ZONE_START:
		case ZONE_DUNGEON1:
		case ZONE_DRATAN:
		case ZONE_MERAC:
		case ZONE_DUNGEON3:
		case ZONE_EGEHA:
		case ZONE_EGEHA_DUNGEON_1:
		case ZONE_EGEHA_DUNGEON_8:
		case ZONE_EGEHA_DUNGEON_9:
		case ZONE_EGEHA_DUNGEON_10:
		case ZONE_DRATAN_CASTLE_DUNGEON:
		case ZONE_STREIANA:
		case ZONE_EBONY_MINE:
		case ZONE_MISTY_CANYON:
		case ZONE_FLORAIM_CAVE:
		case ZONE_MONDSHINE:
		case ZONE_CAPPELLA_2:
		case ZONE_TRIVIA_CANYON:
		case ZONE_TARIAN:
		case ZONE_BLOODYMIR:
			return true;
		default:
			return false;
		}
		return false;
	}

#ifdef SYSTEM_TREASURE_MAP

private:
	int				m_flag;

public:
	int				CheckZoneFlag(int mask)
	{
		return m_flag  & mask;
	}
	void			SetTreasureDropFlag()
	{
		m_flag |= ZONE_FLAG_TREASUREBOX;
	}
	void			ResetTreasureDropFlag()
	{
		m_flag &= ~ZONE_FLAG_TREASUREBOX;
	}
	int				CheckTreasureDropFlag()
	{
		return m_flag & ZONE_FLAG_TREASUREBOX;
	}

	int				m_treasureAreaRotate;  // 리젠 지역 로테이트
	int				m_treasureAreaCnt;		// 보물상자 NPC 드랍 지역 갯수
	int				m_treasureLinkZoneCnt;	// 연계 존 개수
	int *			m_treasureLinkZoneIdx;	// 연계 존인덱스
	TREASUREBOXINFO * m_pTreasureBoxInfo;	// 각 드랍 지역별 보물 상자 정보.
	void	RemoveTreasureBoxNpc(CNPC * pNpc);
	bool	SetTresaureMapInfo(CItem * pItem);
	bool	SummonTreaseureBoxNpc(int& x, int& z, int& zonNum);
	int		NextAreaNum();

	bool	IsTreasureDropZone()
	{
		if( IsExtremeCube() || IsComboZone() || IsExpedRaidZone() || IsPartyRaidZone() || IsPersonalDungeon() ||
				IsPkZone() || IsOXQuizRoom() || IsGuildRoom() )
			return false;

		switch( m_index )
		{
		case ZONE_EBONY_MINE:
			return false;
		default:
			break;
		}
		return true;
	}

#endif // SYSTEM_TREASURE_MAP

#ifdef IMP_SPEED_SERVER
private:
	int		m_zone_exp;
	int		m_zone_sp;
	int		m_zone_drop;
public:
	int		GetZoneExp()
	{
		return m_zone_exp;
	}
	int		GetZoneSp()
	{
		return m_zone_sp;
	}
	int		GetZoneDrop()
	{
		return m_zone_drop;
	}
#endif //IMP_SPEED_SERVER
};

#ifdef SYSTEM_TREASURE_MAP
class TREASUREBOXINFO
{
public:
	int				max;		// 리젠 max
	RECTF			rect;		// 리젠 rect

	typedef std::vector<CNPC*> vec_t;
	vec_t			listNpc;		// 리젠한 npc 포인터

	TREASUREBOXINFO();
	~TREASUREBOXINFO();

	int	 GetEmptyRegenCount();
	bool SetTreasureBoxNpc(CNPC* pNpc);
	void RemoveTreasureBoxNpc(CNPC * pNpc);
	void RemoveAllTreasureBoxNpc()
	{
		listNpc.clear();
	}
};

#endif  // SYSTEM_TREASURE_MAP

#endif // __AZONE_H__
//
