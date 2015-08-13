// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_)
#define AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_

#define COMPOSE_ITEM_FILENAME "/data/itemCompose.bin"

#include <vector>
#include <boost/thread.hpp>

#include "CPos.h"
#include "Option.h"
#include "RareOptionProto.h"
#include "SetItemProto.h"
#include "SocketSystem.h"
#include "InventoryManager.h"
#include "../ShareLib/MemoryPoolBase.h"

class CAffinityProto;

class CItemProto
{
public:

	CItemProto();
	virtual ~CItemProto();

private:
	//////////////////
	// DB 정보 변수
	int			m_index;		// 아이템번호
	int			m_typeIdx;		// 타입종류
	int			m_subtypeIdx;	// 서브타입종류
	CLCString	m_name;			// 아이템이름
	int			m_jobFlag;		// 사용종족
	LONGLONG	m_flag;			// 속성
	int			m_maxUse;		// 내구도
	int			m_fame;			// 명성치

	int m_num0;
	int	m_num1;
	int	m_num2;
	int	m_num3;
	int	m_num4;

	int m_set0;
	//int m_QuestZone;

	int m_set1;
	//int m_questX;

	int m_set2;
	//int m_questZ;

	int m_set3;
	//int m_questY;

	int m_set4;			// 세트 아이템 Index
	//int m_questRange ;	// 퀘스트 아이템 반경

	int			m_grade;			// 공격형 펫무기타입으로 사용
	int			m_stackcount;		// 과거 무게를 현재는 스택 갯수로 사용함
	LONGLONG	m_price;			// 가격

	int			m_wearing;			// 착용 위치

	// 아이템 재료
	int		m_materialIndex[MAX_MAKE_ITEM_MATERIAL];
	int		m_materialCount[MAX_MAKE_ITEM_MATERIAL];

	int		m_needSSkillIndex;
	int		m_needSSkillLevel;

	int		m_needSSkillIndex2;
	int		m_needSSkillLevel2;

	int		m_nRareIndex[MAX_RARE_OPTION_ITEM];				// 레어 Index, 옵션/스킬 Index(0 : 귀속 설정, 1~6 : 옵션, 7~9 : 스킬)
	int		m_nRareProb[MAX_RARE_OPTION_ITEM];				// 레어 확률,  옵션/스킬 Level(1~6 : 옵션, 7~9 : 스킬)

	int		m_level;			// 레벨

	int		m_level2;			// 아이템 레벨2
	int m_MaxOriginVariation[MAX_VARIATION_COUNT];

	int		m_rvr_type;
	int		m_rvr_grade;

#ifdef DURABILITY
	int		m_durability;		// 장비 내구도
#endif
	int		m_castle_war;
// ---------------------------------------------
public:
	int getItemProtoOriginVariation(int nIndex) const
	{
		return m_MaxOriginVariation[nIndex];
	}
	int getItemIndex() const
	{
		return m_index;
	}

	int getItemTypeIdx() const
	{
		return m_typeIdx;
	}

	int getItemSubTypeIdx() const
	{
		return m_subtypeIdx;
	}

	CLCString getItemName() const
	{
		return m_name;
	}

	int getItemJobFlag() const
	{
		return m_jobFlag;
	}

	LONGLONG getItemFlag() const
	{
		return m_flag;
	}

	int getItemMaxUse() const
	{
		return m_maxUse; // 내구도
	}

	int getItemFrame() const
	{
		return m_fame;	// 명성치.
	}

	int getItemNum0() const
	{
		return m_num0;
	}

	int getItemNum1() const
	{
		return m_num1;
	}

	int getItemNum2() const
	{
		return m_num2;
	}

	int getItemNum3() const
	{
		return m_num3;
	}

	int getItemNum4() const
	{
		return m_num4;
	}

	int getItemSet0() const
	{
		return m_set0;
	}

	int getItemQuestZone() const
	{
		return m_set0;
	}

	int getItemSet1() const
	{
		return m_set1;
	}

	int getItemQuestX() const
	{
		return m_set1;
	}

	int getItemSet2() const
	{
		return m_set2;
	}

	int getItemQuestZ() const
	{
		return m_set2;
	}

	int getItemSet3() const
	{
		return m_set3;
	}

	int getItemQuestY() const
	{
		return m_set3;
	}

	int getItemSet4() const
	{
		return m_set4;
	}

	int getItemQuestRange() const
	{
		return m_set4;
	}

	int getItemGrade() const
	{
		return m_grade;
	}

	int getStackCount() const
	{
		return m_stackcount;
	}

	LONGLONG getItemPrice() const
	{
		return m_price;
	}

	int getItemWearing() const
	{
		return m_wearing;
	}

	int getItemMaterialIndex( int i ) const
	{
		return m_materialIndex[ i ];
	}

	int getItemMaterialCount( int i ) const
	{
		return m_materialCount[ i ];
	}

	int getItemNeedSSkillIndex() const
	{
		return m_needSSkillIndex;
	}

	int getItemNeedSSkillLevel() const
	{
		return m_needSSkillLevel;
	}

	int getItemNeedSSkillIndex2() const
	{
		return m_needSSkillIndex2;
	}


	int getItemRareIndex( int i ) const
	{
		return m_nRareIndex[i];
	}

	int getItemRareProb( int i ) const
	{
		return m_nRareProb[i];
	}

	int getItemLevel() const
	{
		return m_level;
	}

	int getItemLevel2() const
	{
		return m_level2;
	}

	int getItemRvRType() const
	{
		return m_rvr_type;
	}

	int getItemRvRGrade() const
	{
		return m_rvr_grade;
	}
	// DB변수 여기까지

#ifdef DURABILITY
	int getDurability() const
	{
		return m_durability;
	}
#endif

	int getCastleWar() const
	{
		return m_castle_war;
	}

public:

	friend class CItemList;

	bool IsRareItem() const
	{
		return (m_flag & ITEM_FLAG_RARE) ? true : false;
	}

	bool IsComposeItem() const
	{
		return (m_flag & ITEM_FLAG_COMPOSE) ? true : false;
	}

	// 블러드젬 사용 가능 여부 : ITEM_FLAG_UPGRADE && !ITEM_FLAG_COMPOSITE
	bool CanBloodGem() const
	{
		if ((m_flag & ITEM_FLAG_UPGRADE) == 0)
			return false;
		if (m_flag & ITEM_FLAG_COMPOSITE)
			return false;
		if (IsRareItem())
			return false;
		if (IsOriginItem())
			return false;

		return true;
	}

	int GetItemProtoLevel() const
	{
		return m_level;
	}

	int GetItemProtoLevel2() const
	{
		return m_level2;
	}

	bool IsOriginItem() const
	{
		return (m_flag & ITEM_FLAG_ORIGIN) ? true : false;
	}

	int GetSetIndex() const
	{
		return m_set4;
	}
	int GetBelongType() const
	{
		return m_nRareIndex[0];
	}
	int GetOriginIndex(int nPos) const
	{
		return m_nRareIndex[1 + nPos];
	}
	int GetOriginLevel(int nPos) const
	{
		return m_nRareProb[1 + nPos];
	}
	int GetOriginOptionCnt() const
	{
		return pre_origin_option_count_;
	}
	int GetOriginSkillCnt() const
	{
		return pre_origin_skill_count_;
	}

	std::vector<CAffinityProto*>	m_affinityList;

	bool IsRangeWeapon()
	{
		return is_range_weapon_;
	}
	bool IsMagicWeapon()
	{
		return is_magic_weapon_;
	}
	int GetUsingStat()
	{
		return get_using_stat_;
	}
	bool IsWeaponType()
	{
		return is_weapon_type_;
	}
	bool IsArmorType()
	{
		return is_armor_type_;
	}
	bool IsAccessary()
	{
		return is_accessary_;
	}
	bool IsPet()
	{
		return is_pet_;
	}
	bool IsAPet()
	{
		return is_apet_;
	}

	bool IsUpgradeRune()
	{
		return is_upgrade_rune_;
	}

	bool IsJewelType()
	{
		return is_jewel_type_;
	}

	bool IsChaosJewelType()
	{
		return is_chaos_jewel_type_;
	}

	bool IsStableType()
	{
		return is_stable_type_;
	}
	bool IsFuncType()
	{
		return is_func_type_;
	}
	bool IsSocketItem()
	{
		return is_socket_type_;
	}
	bool IsReformerItem()
	{
		return is_reformer_item_;
	}
	bool isAllowSocketItem()
	{
		return is_allow_socket;
	}
	bool isProcessScroll()
	{
		return is_process_scroll_;
	}
	bool isRebirthItem()
	{
		return is_rebirth_item_;
	}
	bool isMonsterCard()
	{
		return is_monster_card_;
	}
	bool isRVRNotUse() const
	{
		return is_rvr_not_use_;
	}
	bool isSyndicate()
	{
		return is_syndicate_;
	}
	bool isUseInWarZone()
	{
		return is_useinwarzone_;
	}
	bool isTimer()
	{
		return is_timer_;
	}
	bool isRvrJewel()
	{
		return is_rvr_jewel_;
	}
	bool isGPS()
	{
		return is_gps_;
	}
	bool isQuuestGive()
	{
		return is_questGive;
	}

private:
	int			pre_origin_option_count_;
	int			pre_origin_skill_count_;

	bool		is_range_weapon_;
	bool		is_magic_weapon_;
	int			get_using_stat_;
	bool		is_weapon_type_;
	bool		is_armor_type_;
	bool		is_accessary_;
	bool		is_pet_;
	bool		is_apet_;

	bool		is_upgrade_rune_;

	bool		is_jewel_type_;

	bool		is_chaos_jewel_type_;

	bool		is_stable_type_;
	bool		is_func_type_;
	bool		is_socket_type_;
	bool		is_reformer_item_;
	bool		is_allow_socket;
	bool		is_process_scroll_;
	bool		is_rebirth_item_;
	bool		is_monster_card_;
	bool		is_rvr_not_use_;
	bool		is_syndicate_;
	bool		is_useinwarzone_;		// 전장에서 사용가능한가?
	bool		is_timer_;
	bool		is_rvr_jewel_;
	bool		is_gps_;
	bool		is_questGive;
};

enum TRADE_TYPE { TRADE_EXCHANGE = 0, TRADE_PERSONAL_SHOP, TRADE_SELL, TRADE_STASH, TRADE_TRADEAGENT, TRADE_DROP };

class CItem : public MemoryPoolBaseWithMutex<CItem>
{
public:
	friend class Inventory;
	friend class InventoryManager;
	friend class CInventoryRow;
	friend class CItemList;
	friend class CStash;

private:
	/////////////////////
	// 인벤토리 위치 관련
	int			m_tab_idx;			// 탭
	int			m_invenIndex;		// 인벤토리내에서의 위치

	LONGLONG	m_count;			// 개수

	int			m_virualIndex;		// 가상 인덱스
	int			m_dbIndex;			// DB 인덱스

	// 아이템 속성
	char		m_wearPos;			// 착용위치/탭
	// 0 ~ 11	: 투구 상의 특수 무기 하의 방패 장갑 신발 액서서리1, 2, 3
	// -1		: 착용 안함

	int			m_plus;				// + 수치, 애완동물 아이템의 경우 해당 애완동물 INDEX
	// 곤충 채집 상자 : 채집 곤충 데이터 1 (4비트 * 8마리)
	// 몬스터 용병 카드 : npcIndex
	// 보물지도 아이템 : zoneIndex

	int			m_plus2;			// 세트 아이템 : "물리/마법&방어/공격" 수치 저장
	// 몬스터 용병 카드 : 공격력과 방어력 수치를 넣어준다.
	// 보물지도 아이템 : x, y 좌표 (비트를 반으로 나눠서 넣어준다.

	int			m_flag;				// 속성

	int			m_used;				// 내구도
	// 생산도구 내구도
	// 플래티늄 재련석 기간
	// 의상 기간
	// 의상 결합 기간
	// 곤충 채집 상자 : 채집 곤충 데이터 2 (4비트 * 8마리)

	int			m_used_2;			// 결합용의상:	결합 주문서 사용 기간, 기간제 아이템 남은 시간
	// 무기, 장비:	레벨 다운 주문서 사용 기간,
	// 몬스터 용병 카드 : classInx

#ifdef DURABILITY
	int			now_durability;		// 현재 장비 내구도
	int			max_durability;		// 최대 장비 내구도
#endif

public:
	CItem();
	~CItem();

	//////////////////
	// DB 정보 변수
	CItemProto*	m_itemProto;

	LONGLONG getItemCount()
	{
		return m_count;
	}
	void setItemCount(LONGLONG count);

	int getInvenIndex()
	{
		return m_invenIndex;
	}
	int getVIndex()
	{
		return m_virualIndex;
	}
	void setVIndex(int index)
	{
		m_virualIndex = index;
	}

	int getDBIndex()
	{
		return m_dbIndex;
	}
	void setDBIndex(int index)
	{
		m_dbIndex = index;
	}

	char getWearPos()
	{
		return m_wearPos;
	}
	void setWearPos(char pos)
	{
		m_wearPos = pos;
	}
	void unWearPos()
	{
		m_wearPos = WEARING_NONE;
	}

	int getPlus()
	{
		return m_plus;
	}
	void setPlus(int plus)
	{
		m_plus = plus;
	}

	int getPlus_2()
	{
		return m_plus2;
	}
	void setPlus_2(int plus)
	{
		m_plus2 = plus;
	}

	int getFlag()
	{
		return m_flag;
	}
	void setFlag(int flag)
	{
		m_flag = flag;
	}	

	int getUsed()
	{
		return m_used;
	}
	void setUsed(int used)
	{
		m_used = used;
	}

	int getUsed_2()
	{
		return m_used_2;
	}
	void setUsed_2(int used)
	{
		m_used_2 = used;
	}

	int			m_MercenaryIndex;	// 서환한 몬스터 용병의 고유 index;

	COption		m_option[MAX_ITEM_OPTION];	// 아이템 옵션, 레어 아이템의 경우 0은 레어 인덱스 2바이트, 1은 레어 비트 필드 2바이트
	int			m_nOption;					// 아이템 옵션 수

	const CRareOptionProto* m_pRareOptionProto; // 레어옵션 프로토
	int			m_nRareOptionIndex;	// 레어 옵션 인덱스
	int			m_nRareOptionBit;	// 레어 옵션 비트 필드

	// 합성된 아이템 : 기간을 가진 아이템이 원본 아이템 서버인덱스를 가짐, 없으면 -1
	// DB 저장시에는 tab, invenindex로 구분하여 저장
	// 결합된 의상 : db flag / flag is set
	// 결합당한 의상 : flag is set
	int			m_nCompositeItem;

	const CSetItemProto* m_pSetItemProto;
	int			m_nSetWearCnt;		// 세트 아이템 착용 개수
	int			m_nSetOption;		// 세트 옵션 적용 여부 - 1: 옵션만 적용, 2: 스킬까지 적용
	////////////
	// 기타 속성
	CArea*		m_pArea;			// Area
	CPos		m_pos;				// 좌표
	int			m_cellX;			// 셀 위치
	int			m_cellZ;
	CItem*		m_pCellPrev;		// 셀 링크드 리스트
	CItem*		m_pCellNext;

	int			m_groundPulse;		// 아이템이 떨어진 시간 저장
	int			m_preferenceIndex;	// 아이템 줍는 우선권을 가진 pc의 인덱스

	int tab()
	{
		return m_tab_idx;
	}

	void SetDropNpc(int nIndex)
	{
		setPlus(nIndex);
	}
	int  GetDropNpc()
	{
		return getPlus();
	}

	LONGLONG Count() const
	{
		return m_count;
	}

	bool IsRangeWeapon()
	{
		return  m_itemProto->IsRangeWeapon();
	}
	bool IsMagicWeapon()
	{
		return m_itemProto->IsMagicWeapon();
	}

// 050311 : bs : 무기가 어떤 스탯을 사용하는지 검사
	int GetUsingStat()
	{
		return m_itemProto->GetUsingStat();
	}

// --- 050311 : bs : 무기가 어떤 스탯을 사용하는지 검사
	bool IsWeaponType() const
	{
		return m_itemProto->IsWeaponType();
	}

	bool IsArmorType() const
	{
		return m_itemProto->IsArmorType();
	}

	bool IsAccessary()
	{
		return m_itemProto->IsAccessary();
	}
	bool IsPet()
	{
		return m_itemProto->IsPet();
	}
	bool IsAPet()
	{
		return m_itemProto->IsAPet();
	}

	bool IsUsedPlatinumSpecial()
	{
		int platinumPlus;
		FLAG_ITEM_PLATINUM_GET(getFlag(), platinumPlus);
		return ( (m_itemProto->getItemTypeIdx() == ITYPE_WEAPON || m_itemProto->getItemTypeIdx() == ITYPE_WEAR) && platinumPlus > 0 );
	}

	bool IsUpgradeRune() const
	{
		return m_itemProto->IsUpgradeRune();
	}

	// 교환 가능한 아이템인가?
	bool CanExchange()
	{
#ifdef ENABLE_SUBJOB
		if( !(m_itemProto->getItemFlag()&ITEM_FLAG_SELLER) &&
				!(m_itemProto->getItemFlag()&ITEM_FLAG_EXCHANGE) )		// 트레이더 체크되어있는 교환 플래그 검사 무시
			return false;
#else
		// 프로토 검사
		if (!(m_itemProto->getItemFlag() & ITEM_FLAG_EXCHANGE))
			return false;
#endif //ENABLE_SUBJOB

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 봉인 아이템 불가능
		if (getFlag() & FLAG_ITEM_SEALED)
			return false;

		// 결합 아이템 불가능
		if (getFlag() & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

		if (IsUsedPlatinumSpecial())
			return false;

		// 귀속 아이템 불가능
		if (IsBelong())
			return false;

		return true;
	}

	// 드롭(유저가 버림) 가능한 아이템인가
	bool CanDrop()
	{
#ifdef ENABLE_SUBJOB
		if( !(m_itemProto->getItemFlag()&ITEM_FLAG_SELLER) &&
				!(m_itemProto->getItemFlag()&ITEM_FLAG_DROP) )		// 트레이더 체크되어있는 드랍 플래그 검사 무시
			return false;
#else
		if (!(m_itemProto->getItemFlag() & ITEM_FLAG_DROP))
			return false;
#endif //ENABLE_SUBJOB

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (m_flag & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

		if (IsUsedPlatinumSpecial())
			return false;

		// 귀속 아이템 불가능
		if (IsBelong())
			return false;

		// 퀘스트 아이템 불가능
		if (m_itemProto->getItemFlag() & ITEM_FLAG_QUEST)
			return false;

		if (m_itemProto->getItemTypeIdx() == ITYPE_ETC && m_itemProto->getItemSubTypeIdx() == IETC_QUEST)
			return false;

		return true;
	}

	// 상점에 판매 가능 여부
	bool CanSellToShop()
	{
#ifdef ENABLE_SUBJOB
		if( !(m_itemProto->getItemFlag()&ITEM_FLAG_SELLER) &&
				!(m_itemProto->getItemFlag()&ITEM_FLAG_TRADE) )		// 트레이더 체크되어있는 상점판매 플래그 검사 무시
			return false;
#else
		if (!(m_itemProto->getItemFlag() & ITEM_FLAG_TRADE))
			return false;
#endif //ENABLE_SUBJOB

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (getFlag() & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

		if (IsUsedPlatinumSpecial())
			return false;
		
		return true;
	}

	// 창고 보관 가능 여부
	bool CanKeepStash()
	{
#ifdef ENABLE_SUBJOB
		if( !(m_itemProto->getItemFlag()&ITEM_FLAG_SELLER) &&
				m_itemProto->getItemFlag() & ITEM_FLAG_NO_STASH )		// 트레이더 체크되어있는 창고 플래그 검사 무시
			return false;
#else
		if (m_itemProto->getItemFlag() & ITEM_FLAG_NO_STASH)
			return false;
#endif //ENABLE_SUBJOB

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (getFlag() & FLAG_ITEM_COMPOSITION)
			return false;

		// 시간제 아이템 불가능
		if (m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
		{
			return false;
		}

		if (IsUsedPlatinumSpecial())
			return false;


		// 귀속 아이템 불가능
		if (IsBelong())
			return false;

		return true;
	}

#ifdef ENABLE_SUBJOB
	bool	CheckTrader()
	{
		return m_itemProto->getItemFlag()&ITEM_FLAG_SELLER?true:false;
	}
	bool	CanUseTrader( int type , bool bTrader )
	{
		if( !(m_itemProto->getItemFlag() & ITEM_FLAG_SELLER) )
			return true;

		bool ret = false;

		switch( type )
		{
		case TRADE_EXCHANGE :
		case TRADE_PERSONAL_SHOP :
		case TRADE_TRADEAGENT :
		case TRADE_DROP :
			{
				if( (m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_ABS )) ==  ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_ABS ) ||
						(m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COSTUME2 )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COSTUME2 ) ||
						(m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COMPOSITE )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COMPOSITE ) )
					ret = false;
				else if( (m_itemProto->getItemFlag()&( ITEM_FLAG_SELLER | ITEM_FLAG_CASH )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH ) )
					ret = bTrader? true:false;
				else if(  !(m_itemProto->getItemFlag()&ITEM_FLAG_CASH) && m_itemProto->getItemFlag()&ITEM_FLAG_SELLER )
					ret = true;
				else
					ret = true;
			}
			break;
		case TRADE_SELL :
			{
				if( m_itemProto->getItemFlag() & ITEM_FLAG_SELLER )
					ret = false;
				else
					ret = true;
			}
			break;
		case TRADE_STASH :
			{
				if( (m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_ABS )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_ABS ) ||
						(m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COSTUME2 )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COSTUME2 ) ||
						(m_itemProto->getItemFlag() & ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COMPOSITE )) == ( ITEM_FLAG_SELLER | ITEM_FLAG_CASH | ITEM_FLAG_COMPOSITE ) )
					ret = false;
				else
					ret = true;
			}
			break;
		default:
			ret = true;
		}

		return ret;
	}
#endif //ENABLE_SUBJOB

	// 제련석 붙이기 가능 여부
	bool CanUpgrade()
	{
		if ((m_itemProto->getItemFlag() & ITEM_FLAG_UPGRADE) == 0)
			return false;
		if (IsLent())
			return false;
		return true;
	}

	// 블러드젬 사용 가능 여부 : ITEM_FLAG_UPGRADE && !ITEM_FLAG_COMPOSITE
	bool CanBloodGem()
	{
		if (!m_itemProto->CanBloodGem())
			return false;
		return true;
	}

	// 착용 가능 검사
	bool CanWear()
	{
		if (m_itemProto->getItemWearing() == -1)
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합된 일반 아이템 불가능
		if ((getFlag() & FLAG_ITEM_COMPOSITION) && !(m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
			return false;

		// 봉인된 아이템 작용 불가능
		if( getFlag() & FLAG_ITEM_SEALED )
			return false;

		return true;
	}

	bool IsLent()
	{
		if (getFlag() & FLAG_ITEM_LENT)
			return true;
		else
			return false;
	}

	bool IsBelong()
	{
		if (getFlag() & FLAG_ITEM_BELONG)
			return true;
		return false;
	}

	bool IsOriginItem() const
	{
		return m_itemProto->IsOriginItem();
	}
	int GetBelongType() const
	{
		return m_itemProto->GetBelongType();
	}
	int GetOriginIndex(int nPos) const
	{
		return m_itemProto->GetOriginIndex(nPos);
	}
	int GetOriginLevel(int nPos) const
	{
		return m_itemProto->GetOriginLevel(nPos);
	}
	int GetOriginOptionCnt() const
	{
		return m_itemProto->GetOriginOptionCnt();
	}
	int GetOriginSkillCnt() const
	{
		return m_itemProto->GetOriginSkillCnt();
	}

	bool IsRareItem() const
	{
		return m_itemProto->IsRareItem();
	}

	bool IsComposeItem() const
	{
		return m_itemProto->IsComposeItem();
	}

	void IdentifyRareOption(bool reform_ = false, unsigned int count_ = 0);


	int GetItemLevel() const;

#ifdef PREMIUM_CHAR
	int GetItemLevelForPremiumChar(int premiumType) const;
#endif

	int GetItemPlus()
	{
		if( m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && (m_itemProto->getItemSubTypeIdx() == IACCESSORY_PET || m_itemProto->getItemSubTypeIdx() == IACCESSORY_ATTACK_PET ) )
		{
			return getPlus();
		}
		else if (Is_fortune_item())
		{
			return getPlus();
		}
		else
		{
			return ((getPlus_2() << 16) & 0xffff0000) | (getPlus() & 0x0000ffff);
		}
	}

	// 의상과 결합 가능한 장비인가?
	bool CanCompositeEquipItem()
	{
		if (!IsWeaponType() && !IsArmorType())
			return false;
		if (!CanUpgrade())
			return false;

		return true;
	}

	CSocketList	m_socketList;

	bool IsJewelType() const
	{
		return m_itemProto->IsJewelType();
	}
	bool IsChaosJewelType() const
	{
		return m_itemProto->IsChaosJewelType();
	}

	bool IsStableType() const
	{
		return m_itemProto->IsStableType();
	}
	bool IsFuncType() const
	{
		return m_itemProto->IsFuncType();
	}
	bool IsSocketItem() const
	{
		return m_itemProto->IsSocketItem();
	}

public:
	int			m_OriginVar[MAX_VARIATION_COUNT];

	bool IsReformerItem() const
	{
		return m_itemProto->IsReformerItem();
	}

	void setOriginVar(int nIndex_, int value_)
	{
		if (nIndex_ < 0 || nIndex_ >= MAX_VARIATION_COUNT)
			return ;

		m_OriginVar[nIndex_]  = value_;
	}
	int getOriginVar(int nIndex_) const
	{
		if (nIndex_ < 0 || nIndex_ >= MAX_VARIATION_COUNT)
			return (100);

		return m_OriginVar[nIndex_];
	}

	void originReform(unsigned int count_);

public:
	/**
	* Fortune을 받았는지 안받았는지 검사
	*/
	bool Aleady_got_fortune()
	{
		if (Is_fortune_item() && getPlus() != 0)
			return true;
		return false;
	}

	/**
	* 한벌의상만 fortune을 받을 수 있다.
	*/
	bool Is_fortune_item() const
	{
		if (m_itemProto->getItemTypeIdx() == ITYPE_WEAR && m_itemProto->getItemSubTypeIdx() == IWEAR_SUIT)
			return true;
		return false;
	}

	bool isAllowSocketItem() const
	{
		return m_itemProto->isAllowSocketItem();
	}
	bool isProcessScroll() const
	{
		return m_itemProto->isProcessScroll();
	}

	bool isSyndicate() const
	{
		return m_itemProto->isSyndicate();
	}

	void clone(CItem& destItem);
#ifdef DURABILITY
	int getNowDurability()
	{
		return now_durability;
	}
	void setNowDurability(int dur)
	{
		now_durability = dur;
	}

	int getMaxDurability()
	{
		return max_durability;
	}
	void setMaxDurability(int dur)
	{
		max_durability = dur;
	}
#endif
	bool	m_toggle;
	bool	isToggle(){ return m_toggle ? true : false;}

	// memcpy 에서 제외하고 복사해야 하기에 제일 아래에 위치 시킨다.
	std::string m_serial;			// 시리얼 번호
};

class CItemList
{
private:
	std::string a_need_item_str[MAX_MAKE_ITEM_MATERIAL];
	std::string a_need_item_count_str[MAX_MAKE_ITEM_MATERIAL];
	std::string a_rare_index_str[MAX_RARE_OPTION_ITEM];
	std::string a_rare_prob_str[MAX_RARE_OPTION_ITEM];
	std::string a_origin_variation_str[MAX_VARIATION_COUNT];

	// 얘넨 대입도 복사도 하면 안됨. (갖다 쓰지 못하도록 선언만 하고, 정의는 하지 않았음)
	CItemList(const CItemList&);
	CItemList& operator=(const CItemList&);

	bool checkTimerItem(CItemProto* proto);

public:
	typedef std::map<int, CItemProto*> map_t;

	CItemList();
	virtual ~CItemList();

	// 아이템 리스트 읽기
	bool Load();

	LONGLONG calcItemCount(CItemProto* proto, LONGLONG count);

	// 아이템 인덱스로 아이템 프로토타입 얻기
	CItemProto* FindIndex(int idx);

	// DB 의 인벤 등 아이템을 복원
#ifdef DURABILITY
	CItem* CreateDBItem(int idx, char wearPos, int plus, int flag, int used,
						int used_2, const char* serial, LONGLONG count, short* option
						, char socketcount, int* jewel
						, int* OriginVar, int now_dur, int max_dur
					   );
#else
	CItem* CreateDBItem(int idx, char wearPos, int plus, int flag, int used,
						int used_2, const char* serial, LONGLONG count, short* option
						, char socketcount, int* jewel
						, int* OriginVar
					   );
#endif

	CItem* CreateAutoGiveDBItem(int idx, char wearPos, int plus, int flag, int used,
								int used_2,	const char* serial, LONGLONG count, short* option
								, char socketcount, int* jewel
							   );

	// 새로운 아이템 생성
	CItem* CreateItem(int idx, char wearPos, int plus, int flag, LONGLONG count);

//#ifdef DEV_GUILD_STASH
	CItem* CreateDBItem_2( int idx, char wearPos, int plus, int flag, int used, int used_2, const char* serial, LONGLONG count, short* option , const char* socket, int* itemOrigin );
//#endif //DEV_GUILD_STASH

	int				m_nCount;		// 개수
	int				m_virtualIndex;	// 가상
	CItemProto*		m_protoItems;	// 기본아이템

	// 특정 아이템 프로토타입들
	CItemProto*		m_normalRefineItem;		// 일반 제련석
	CItemProto*		m_specialRefineItem;	// 고급 제련석
	CItemProto*		m_sTicketItem1;			// 싱글던전1 입장권
	CItemProto*		m_sTicketItem2;			// 싱글던전2 입장권

	// 무기 대여용 아이템 리스트
	std::set<int>	m_listLendItem;

	// 성주용 아이템
	std::vector<CItemProto*>	m_flagload;

	// 교환용 아이템
	std::vector<CItemProto*>	m_flagchange;

	std::vector<CItemProto*>	m_flagGuildWar;

protected:
	boost::mutex	m_mutex_for_virtualIndex;

	int GetNextVirtualIndex();

private:
	map_t			map_;
};

class CCtItemList
{
	friend class CItem;
public:
	CCtItemList()
	{
	}
	virtual ~CCtItemList()
	{
		this->_delete();
	}

	bool Load(int ctIdx);

	CItem*			GetItem(int i)
	{
		return this->ctItem[i];
	}

	CItem*			GetItem(CItem* pItem);
	int				GetCount()
	{
		return ctItem.size();
	}

private:

	std::vector<CItem*> ctItem;

	void _delete();
};

class CCatalog
{
	friend class CCtItemList;
public:
	CCatalog()
		: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = 0;
		m_category = 0;
		m_subtype = 0;
		m_type = -1;
		m_cash = 0;
		m_mileage = 0;
		m_flag = 0;
		m_ctItemList = NULL;
		m_enable = false;
	}

	CCatalog(int index)		// 바이너리 서치용
		: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = index;
		m_category = 0;
		m_subtype = 0;
		m_type = -1;
		m_cash = 0;
		m_mileage = 0;
		m_ctItemList = NULL;
		m_flag = 0;
		m_enable = false;
	}
	CCatalog(int index, const char* name, int category, int type, int subtype, int cash, int mileage, bool enable, int m_flag)
		: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = index;
		m_name = name;
		m_category = category;
		m_subtype = subtype;
		m_type = type;
		m_cash = cash;
		m_mileage = mileage;
		m_ctItemList = NULL;
		m_flag = 0;
		m_enable = enable;
	}
	virtual ~CCatalog()
	{
		if( m_ctItemList)
			delete m_ctItemList;
		m_ctItemList = NULL;
	}

	void SetCatalog(int index, const char* name, int category, int type, int subtype, int cash, int mileage, bool enable, int flag, CCtItemList* ctItemList)
	{
		m_index = index;
		m_name = name;
		m_category = category;
		m_subtype = subtype;
		m_type = type;
		m_cash = cash;
		m_mileage = mileage;
		m_ctItemList = NULL;
		m_enable = enable;
		m_flag = flag;
		m_ctItemList = ctItemList;
		CATALOG_FALG_TO_LIMIT(flag, m_toSell);
	}
	void	SetPrice(int price)
	{
		m_cash = price;
	}
	int		GetIndex()
	{
		return m_index;
	}
	const char*	GetName()
	{
		return m_name;
	}
	int		GetCategory()
	{
		return m_category;
	}
	int		GetSubtype()
	{
		return m_subtype;
	}
	int		GetFlag()
	{
		return m_flag;
	}
	int		GetType()
	{
		return m_type;
	}
	int		GetCash()
	{
		return m_cash;
	}
	int		GetMileage()
	{
		return m_mileage;
	}
	int		GetToSell()
	{
		return m_toSell;
	}
	void	SetToSell(int toSell)
	{
		m_toSell = toSell;
	}
	int		GetLimit()
	{
		int limit;
		CATALOG_FALG_TO_LIMIT(m_flag, limit);
		return limit;
	}
	bool	GetEnable()
	{
		return m_enable;
	}
	CCtItemList* GetCtItemList()
	{
		return m_ctItemList;
	}

private:
	CCtItemList* m_ctItemList;

	int		m_index;		// 카탈로그 인덱스
	CLCString m_name;
	int		m_type;			// 타입
	int		m_category;
	int		m_subtype;
	int		m_cash;			// 가격
	int		m_mileage;		// 마일리지
	int		m_toSell;			// 판매 남은 갯수
	int		m_flag;
	bool	m_enable;
};

class CCatalogList
{
	friend class CCatalog;
public:
	int m_typelist[TYPE_MAX_COUNT];
	int m_typeCount;
	int m_subtypelist[SUBTYPE_MAX_COUNT];
	int m_subtypeCount;
	CCatalogList()
	{
		catalog = NULL;
		m_cCount = 0;
		m_recoveryCtid = -1;
	}
	virtual ~CCatalogList()
	{
		if (catalog)
			delete [] catalog;

		catalog = NULL;
	}
	bool Load();
	bool Reload();
	bool ReloadPrice();

	CCatalog* Find(int idx)
	{
		int i = 0;
		for(i = 0; i < m_cCount; i++)
		{
			if(this->catalog[i].GetIndex() == idx)
			{
				return &catalog[i];
			}
		}

		return NULL;
	}

	CCatalog* FindItemToCatalog(CItem* pItem);
	CCatalog* FindItemToCatalog(int itemDBIndex);

	int GetCount()
	{
		return m_cCount;
	}
	CCatalog* getCatalog(int index)
	{
		return &catalog[index] ? &catalog[index] : NULL;
	}

	// 대형 퀵 체력 회복물약
	int m_recoveryCtid;
private:
	CCatalog* catalog;

	int m_cCount;
};

struct stComposeData
{
	int Material_index;
	int Material_count;
	int need_nas;
	int prob;
	int result_item_index;
	int enable;

	stComposeData()
	{
		Material_index = 0;
		Material_count = 0;
		need_nas = 0;
		prob = 0;
		result_item_index = 0;
		enable = 0;
	}
};

class ComposeItem
{
private:
	typedef std::map<int/*mat_item_index*/, stComposeData*> COMPOSE_MAP;
	COMPOSE_MAP _map;
	int m_item_load_count;

public:
	ComposeItem();
	~ComposeItem();
	static ComposeItem* instance();

	void load();
	void init();
	stComposeData* find(int mat_item_index);

	stComposeData* data;
};

bool do_UseGpsItem(CItem* item, CPC* pc);
bool do_UsePvPProtect(CItem* item, CPC* ch);

#endif // !defined(AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_)
//
