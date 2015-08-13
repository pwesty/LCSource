// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Item.h"
#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "Exp.h"
#include "../ShareLib/packetType/ptype_char_status.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	//////////////////
	// DB 정보 변수
	m_itemProto = NULL;

	/////////////////////
	// 인벤토리 위치 관련
	m_tab_idx = -1;								// 탭
	m_invenIndex = -1;							// 인벤 인덱스

	//////////////
	// 아이템 속성
	m_virualIndex = -1;								// 가상 인덱스
	m_dbIndex = -1;								// DB 인덱스
	m_wearPos = -1;								// 착용 안한 상태
	m_plus = 0;									// + 수치
	m_flag = 0;									// 속성
	m_count = 0;								// 개수
	m_used = -1;								// 내구도
	m_used_2 = -1;								// 결합 시간

	////////////
	// 기타 속성
	m_pArea = NULL;
	m_pos = CPos(0, 0, 0, 0, 0);				// 좌표
	m_cellX = NO_CELL;							// 셀 위치
	m_cellZ = NO_CELL;
	m_pCellPrev = NULL;							// 셀 링크드 리스트
	m_pCellNext = NULL;

	m_groundPulse = 0;							// 아이템이 떨어진 시간 저장
	m_preferenceIndex = -1;						// 이 아이템에 우선권이 있는 pc의 인덱스 저장 없으면 -1

	m_nOption = 0;								// 아이템 옵션 수

	m_nCompositeItem = -1;

	m_pRareOptionProto = NULL;
	m_nRareOptionIndex = 0;
	m_nRareOptionBit = 0;

	m_plus2 = 0;
	m_pSetItemProto = 0;
	m_nSetWearCnt = 0;
	m_nSetOption = 0;
	m_MercenaryIndex = -1;;	// 서환한 몬스터 용병의 index;
	memset(m_OriginVar, 0, sizeof(int) * MAX_VARIATION_COUNT);

#ifdef DURABILITY
	now_durability = 0;
	max_durability = 0;
#endif
	m_toggle = false;
}

CItem::~CItem()
{
}


void CItem::IdentifyRareOption(bool reform_, unsigned int count_)
{
	if (m_nOption && reform_ == false)
		return ;

	int nRandom = GetRandom(1, 10000);
	int i;
	for (i = 0; i < MAX_RARE_OPTION_ITEM; i++)
	{
		if (m_itemProto->getItemRareIndex(i) > 0)
		{
			if (nRandom <= m_itemProto->getItemRareProb(i))
			{
				const CRareOptionProto* pRareProto = gserver->m_rareOptionList.Find(m_itemProto->getItemRareIndex(i));
				if (pRareProto)
				{
					int nRareIndex = pRareProto->GetIndex();
					int nRareBit = 0;
					int j;
					for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
					{
						const RAREOPTIONDATA* rod = pRareProto->GetData(j);
						if (rod && rod->optiontype != -1)
						{
							int finalProb = (10 * count_) + rod->optionprob;
							if (finalProb > 10000)
								finalProb = 10000;
							if (GetRandom(1, 10000) <= finalProb)
							{
								nRareBit |= rod->rarebit;
							}
						}
					}

					m_nOption = 2;
					m_option[0].GetDBValue((short)nRareIndex);
					m_option[1].GetDBValue((short)nRareBit);
					m_pRareOptionProto = pRareProto;
					m_nRareOptionIndex = nRareIndex;
					m_nRareOptionBit = nRareBit;
					return ;
				}
			}
			nRandom -= m_itemProto->getItemRareProb(i);
		}
	}
}

#ifdef PREMIUM_CHAR
//XX - 프리미엄캐릭터 : 모든 장비를 3레벨 먼저 작용
int CItem::GetItemLevelForPremiumChar(int premiumType) const
{
	int get_level = this->GetItemLevel();
	int pre_level = m_itemProto->GetItemProtoLevel();

	if (premiumType > 0)
	{
		pre_level -= 3;
	}

	if (pre_level < 1)
		pre_level = 1;

	return (get_level < pre_level) ? get_level : pre_level;
}
#endif

int CItem::GetItemLevel() const
{
	int nItemLevel = m_itemProto->GetItemProtoLevel();
	int i;

	if (m_nOption == 0 && !IsOriginItem())
	{
		if(IsSocketItem() && m_socketList.GetJewelOptionAt(0) && m_socketList.GetJewelOptionAt(0)->m_type == OPTION_DOWN_LIMITLEVEL)
		{
			nItemLevel -= m_socketList.GetJewelOptionAt(0)->m_value;
		}

		if (nItemLevel < 1)
			nItemLevel = 1;

		return nItemLevel;
	}

	if (IsRareItem())
	{
		for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
		{
			const RAREOPTIONDATA* rod = m_pRareOptionProto->GetData(i);
			if (m_nRareOptionBit & rod->rarebit)
			{
				switch (rod->optiontype)
				{
				case OPTION_DOWN_LIMITLEVEL:
					if (m_itemProto->GetItemProtoLevel() - rod->optionval < nItemLevel)
						nItemLevel = m_itemProto->GetItemProtoLevel() - rod->optionval;
					break;
				default:
					break;
				}
			}
		}
	}
	else if (IsOriginItem())
	{
		for (i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			switch (m_itemProto->GetOriginIndex(i))
			{
			case OPTION_DOWN_LIMITLEVEL:
				{
					COptionProto * pOptionProto = gserver->m_optionProtoList.FindProto(m_itemProto->GetOriginIndex(i));
					int nLevel = m_itemProto->GetOriginLevel(i);
					int nValue = 0;
					if (pOptionProto && pOptionProto->m_levelValue[nLevel - 1])
					{
						nValue = pOptionProto->m_levelValue[nLevel -1];
					}

					if (m_itemProto->GetItemProtoLevel() - nValue < nItemLevel)
						nItemLevel = m_itemProto->GetItemProtoLevel() - nValue;
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < MAX_WEAPON_OPTION; i++)
		{
			if (m_option[i].m_level <= 0)
				continue;
			switch (m_option[i].m_type)
			{
			case OPTION_DOWN_LIMITLEVEL:
				if (m_itemProto->GetItemProtoLevel() - m_option[i].m_value < nItemLevel)
					nItemLevel = m_itemProto->GetItemProtoLevel() - m_option[i].m_value;
				break;
			default:
				break;
			}
		}
	}

	if(IsSocketItem() && m_socketList.GetJewelOptionAt(0) && m_socketList.GetJewelOptionAt(0)->m_type == OPTION_DOWN_LIMITLEVEL)
	{
		nItemLevel -= m_socketList.GetJewelOptionAt(0)->m_value;
	}

	if (nItemLevel < 1)
		nItemLevel = 1;

	return nItemLevel;
}

CItemProto::CItemProto()
{
	m_index = -1;
	m_typeIdx = -1;
	m_subtypeIdx = -1;
	m_jobFlag = 0;
	m_flag = 0;
	m_maxUse = -1;
	m_fame = -1;

	m_num0 = 0;
	m_num1 = 0;
	m_num2 = 0;
	m_num3 = 0;
	m_num4 = 0;
	m_stackcount = 0;
	m_level = 0;
	m_level2 = 0;
	m_price = 0;
	m_wearing = -1;

	memset(m_materialIndex, -1, sizeof(int) * MAX_MAKE_ITEM_MATERIAL);
	memset(m_materialCount, 0, sizeof(int) * MAX_MAKE_ITEM_MATERIAL);

	m_needSSkillIndex = -1;
	m_needSSkillLevel = 0;

	m_needSSkillIndex2 = -1;
	m_needSSkillLevel2 = 0;

	m_set4 = 0;
	m_set0 = -1;
	memset(m_MaxOriginVariation, 0, sizeof(int) * MAX_VARIATION_COUNT);
	is_syndicate_ = false;
}

CItemProto::~CItemProto()
{
}

//////////////////////////////////////////////////////////////////////
// CItemList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemList::CItemList()
{
	m_protoItems = NULL;
	m_virtualIndex = 1;
	m_nCount = 0;

	m_normalRefineItem = NULL;
	m_specialRefineItem = NULL;

	m_sTicketItem1 = NULL;
	m_sTicketItem2 = NULL;

	for (int i = 0; i < MAX_MAKE_ITEM_MATERIAL; ++i)
	{
		a_need_item_str[i] = boost::str(boost::format("a_need_item%d") % i);
		a_need_item_count_str[i] = boost::str(boost::format("a_need_item_count%d") % i);
	}

	for (int i = 0; i < MAX_RARE_OPTION_ITEM; i++)
	{
		a_rare_index_str[i] = boost::str(boost::format("a_rare_index_%d") % i);
		a_rare_prob_str[i] = boost::str(boost::format("a_rare_prob_%d") % i);
	}

	for (int i = 0; i < MAX_VARIATION_COUNT; i++)
	{
		a_origin_variation_str[i] = boost::str(boost::format("a_origin_variation%d") % (i + 1));
	}
}

CItemList::~CItemList()
{
	if (m_protoItems)
		delete [] m_protoItems;

	m_protoItems = NULL;
	m_nCount = 0;
}

bool CItemList::Load()
{
	bool bRet = true;

	CDBCmd DBItem;
	DBItem.Init(&gserver->m_dbdata);

	std::string sql = "SELECT * FROM t_item WHERE a_enable=1 ORDER BY a_index";

	DBItem.SetQuery(sql);
	if (!DBItem.Open())
		return false;

	if (DBItem.m_nrecords == 0)
	{
		LOG_ERROR("item db is empty.");
		return false;
	}

	m_nCount = DBItem.m_nrecords;
	m_protoItems = new CItemProto[m_nCount];
	m_flagload.reserve(DBItem.m_nrecords);
	m_flagchange.reserve(DBItem.m_nrecords);
	m_flagGuildWar.reserve(DBItem.m_nrecords);

	int ridx = 0;
	int check_flag = ITEM_FLAG_RARE | ITEM_FLAG_COUNT;
	while (DBItem.MoveNext())
	{
		CItemProto* pItemProto = &m_protoItems[ridx];
		LONGLONG nFlag;

		DBItem.GetRec("a_flag",			nFlag);
		if ( (nFlag & check_flag) == check_flag)
			continue ;

		pItemProto->m_flag = nFlag;

		DBItem.GetRec("a_index",		pItemProto->m_index);
		DBItem.GetRec("a_type_idx",		pItemProto->m_typeIdx);
		DBItem.GetRec("a_subtype_idx",	pItemProto->m_subtypeIdx);
		DBItem.GetRec(ITEM_NAME_FIELD,	pItemProto->m_name, true);
		DBItem.GetRec("a_job_flag",		pItemProto->m_jobFlag);
		DBItem.GetRec("a_num_0",		pItemProto->m_num0);
		DBItem.GetRec("a_num_1",		pItemProto->m_num1);
		DBItem.GetRec("a_num_2",		pItemProto->m_num2);
		DBItem.GetRec("a_num_3",		pItemProto->m_num3);
		DBItem.GetRec("a_num_4",		pItemProto->m_num4);
		DBItem.GetRec("a_weight",		pItemProto->m_stackcount);
		DBItem.GetRec("a_level",		pItemProto->m_level);
		DBItem.GetRec("a_level2",		pItemProto->m_level2);
		DBItem.GetRec(ITEM_PRICE_FIELD,	pItemProto->m_price);
		DBItem.GetRec("a_wearing",		pItemProto->m_wearing);
		DBItem.GetRec("a_max_use",		pItemProto->m_maxUse);
		DBItem.GetRec("a_fame",			pItemProto->m_fame);

		DBItem.GetRec("a_set_0",		pItemProto->m_set0);
		DBItem.GetRec("a_set_1",		pItemProto->m_set1);
		DBItem.GetRec("a_set_2",		pItemProto->m_set2);
		DBItem.GetRec("a_set_3",		pItemProto->m_set3);
		DBItem.GetRec("a_set_4",		pItemProto->m_set4);
		DBItem.GetRec("a_grade",		pItemProto->m_grade);	// 펫무기타입으로 사용.

		for (int i = 0; i < MAX_MAKE_ITEM_MATERIAL; i++)
		{
			DBItem.GetRec(a_need_item_str[i].c_str(), pItemProto->m_materialIndex[i]);
			DBItem.GetRec(a_need_item_count_str[i].c_str(), pItemProto->m_materialCount[i]);
		}

		DBItem.GetRec("a_need_sskill",			pItemProto->m_needSSkillIndex);
		DBItem.GetRec("a_need_sskill_level",	pItemProto->m_needSSkillLevel);

		DBItem.GetRec("a_need_sskill2",			pItemProto->m_needSSkillIndex2);
		DBItem.GetRec("a_need_sskill_level2",	pItemProto->m_needSSkillLevel2);

		for (int i = 0; i < MAX_RARE_OPTION_ITEM; i++)
		{
			if (!DBItem.GetRec(a_rare_index_str[i].c_str(), pItemProto->m_nRareIndex[i]))
				return false;

			if (!DBItem.GetRec(a_rare_prob_str[i].c_str(),	pItemProto->m_nRareProb[i]))
				return false;
		}
		unsigned char val;
		for (int i = 0; i < MAX_VARIATION_COUNT; i++)
		{
			if (!DBItem.GetRec(a_origin_variation_str[i].c_str(), val))
				return false;

			pItemProto->m_MaxOriginVariation[i] = (int)(val * 10);
		}

		DBItem.GetRec("a_rvr_value", pItemProto->m_rvr_type);
		DBItem.GetRec("a_rvr_grade", pItemProto->m_rvr_grade);

#ifdef DURABILITY
		DBItem.GetRec("a_durability", pItemProto->m_durability);
#endif
		DBItem.GetRec("a_castle_war", pItemProto->m_castle_war);

		if(pItemProto->m_castle_war != 0)
		{
			m_flagGuildWar.push_back(&m_protoItems[ridx]);
		}

		// 특정 아이템 찾기 : 인덱스로
		switch (pItemProto->m_index)
		{
		case 84:	// 일반 제련석
			m_normalRefineItem = m_protoItems + ridx;
			break;

		case 85:	// 고급 제련석
			m_specialRefineItem = m_protoItems + ridx;
			break;

		case 141:	// 싱글던전2 입장권
			m_sTicketItem2 = m_protoItems + ridx;
			break;

		case 142:	// 싱글던전1 입장권
			m_sTicketItem1 = m_protoItems + ridx;
			break;

		default:
			break;
		}

		// 무기 대여용 아이템 리스트 만들기
		// 무기 타입 중
		// ITEM_FLAG_LENT
		// 가격이 설정 되어 있을 때
		// 레벨 37/41/45/49/53/57/61/65
		static const int nNormalWeaponFlag = ITEM_FLAG_LENT;
		if (pItemProto->m_typeIdx == ITYPE_WEAPON && (pItemProto->m_flag & nNormalWeaponFlag) == nNormalWeaponFlag && pItemProto->m_price > 0)
		{
			switch (pItemProto->m_subtypeIdx)
			{
			case IWEAPON_NIGHT:
			case IWEAPON_CROSSBOW:
			case IWEAPON_STAFF:
			case IWEAPON_BIGSWORD:
			case IWEAPON_AXE:
			case IWEAPON_SHORTSTAFF:
			case IWEAPON_BOW:
			case IWEAPON_SHORTGUM:
			case IWEAPON_TWOSWORD:
			case IWEAPON_WAND:
			case IWEAPON_SCYTHE:
			case IWEAPON_POLEARM:
				switch (pItemProto->m_level)
				{
				case 37:
				case 41:
				case 45:
				case 49:
				case 53:
				case 57:
				case 61:
				case 65:
					m_listLendItem.insert(pItemProto->m_index);
					break;

				default:
					break;
				} // 레벨
				break;

			case IWEAPON_MINING:
			case IWEAPON_GATHERING:
			case IWEAPON_CHARGE:
			default:
				break;
			} // 서브 타입
		} // 무기 && 일반플래그

		if (pItemProto->m_price < 0)
		{
			GAMELOG << init("ERROR: ITEM PRICE")
					<< pItemProto->m_index << delim
					<< pItemProto->m_name << delim
					<< pItemProto->m_price
					<< end;
			bRet = false;
		}

		{
			pItemProto->pre_origin_option_count_ = 0;
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if (pItemProto->GetOriginIndex(i) > -1)
					++pItemProto->pre_origin_option_count_;
			}
		}

		{
			pItemProto->pre_origin_skill_count_ = 0;
			for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				if (pItemProto->GetOriginIndex(i) > -1)
					++pItemProto->pre_origin_skill_count_;
			}
		}

		{
			pItemProto->is_range_weapon_ = false;

			if (pItemProto->getItemTypeIdx() == ITYPE_WEAPON)
			{
				switch (pItemProto->getItemSubTypeIdx())
				{
				case IWEAPON_CROSSBOW:
				case IWEAPON_BOW:
					pItemProto->is_range_weapon_ = true;
					break;
				}
			}
		}

		{
			pItemProto->is_magic_weapon_ = false;

			if (pItemProto->getItemTypeIdx() == ITYPE_WEAPON)
			{
				switch (pItemProto->getItemSubTypeIdx())
				{
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_STAFF:
				case IWEAPON_WAND:
				case IWEAPON_SCYTHE:
				case IWEAPON_POLEARM:
				case IWEAPON_SOUL:
					pItemProto->is_magic_weapon_ = true;
					break;
				}
			}
		}

		{
			pItemProto->get_using_stat_ = -1;

			if (pItemProto->getItemTypeIdx() == ITYPE_WEAPON)
			{
				switch (pItemProto->getItemSubTypeIdx())
				{
				case IWEAPON_CROSSBOW:
				case IWEAPON_BOW:
				case IWEAPON_SHORTGUM:
					pItemProto->get_using_stat_ = USING_DEX;
					break;

				case IWEAPON_SOUL:
				case IWEAPON_WAND:
				case IWEAPON_POLEARM:
				case IWEAPON_SCYTHE:
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_STAFF:
					pItemProto->get_using_stat_ = USING_INT;
					break;

				default:
					pItemProto->get_using_stat_ = USING_STR;
					break;
				}
			}
		}

		{
			pItemProto->is_weapon_type_ = false;

			if (pItemProto->getItemTypeIdx() == ITYPE_WEAPON)
			{
				switch (pItemProto->getItemSubTypeIdx())
				{
				case IWEAPON_NIGHT:
				case IWEAPON_CROSSBOW:
				case IWEAPON_STAFF:
				case IWEAPON_BIGSWORD:
				case IWEAPON_AXE:
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_BOW:
				case IWEAPON_SHORTGUM:
				case IWEAPON_TWOSWORD:
				case IWEAPON_WAND:
				case IWEAPON_SCYTHE:
				case IWEAPON_POLEARM:
				case IWEAPON_SOUL:
					pItemProto->is_weapon_type_ = true;
					break;
				}
			}
		}

		{
			pItemProto->is_armor_type_ = false;

			if (pItemProto->getItemTypeIdx() == ITYPE_WEAR)
			{
				switch (pItemProto->getItemSubTypeIdx())
				{
				case		IWEAR_HELMET	:
				case		IWEAR_ARMOR		:
				case		IWEAR_PANTS		:
				case		IWEAR_GLOVE		:
				case		IWEAR_SHOES		:
				case		IWEAR_SHIELD	:
				case		IWEAR_BACKWING	:
					pItemProto->is_armor_type_ = true;
					break;
				}
			}
		}

		pItemProto->is_accessary_ = (pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pItemProto->getItemSubTypeIdx() != IACCESSORY_PET );
		pItemProto->is_pet_ = (pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pItemProto->getItemSubTypeIdx() == IACCESSORY_PET );
		pItemProto->is_apet_ = (pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pItemProto->getItemSubTypeIdx() == IACCESSORY_ATTACK_PET );

		{
			pItemProto->is_upgrade_rune_ = false;

			if (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_UPGRADE)
			{
				switch (pItemProto->getItemNum0())
				{
				case IETC_UPGRADE_PURITY_RUNE:
				case IETC_UPGRADE_DEVIL_RUNE:
				case IETC_UPGRADE_CHAOS_RUNE:
				case IETC_UPGRADE_SUPER_RUNE:
				case IETC_UPGRADE_LUCKY_RUNE:
					pItemProto->is_upgrade_rune_ = true;
					break;
				}
			}
		}

		pItemProto->is_jewel_type_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_JEWEL );

		pItemProto->is_chaos_jewel_type_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_CHAOSJEWEL);

		pItemProto->is_stable_type_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_STABILIZER );
		pItemProto->is_func_type_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_FUNCTIONS );

		pItemProto->is_reformer_item_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_REFORMER);
		if(pItemProto->getItemTypeIdx() == ITYPE_WEAPON || pItemProto->getItemTypeIdx() == ITYPE_WEAR)
		{
			pItemProto->is_allow_socket = true;
		}
		else if(pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY)
		{
			switch(pItemProto->getItemSubTypeIdx())
			{
			case IACCESSORY_CHARM:
			case IACCESSORY_MAGICSTONE:
			case IACCESSORY_LIGHTSTONE:
			case IACCESSORY_EARING:
			case IACCESSORY_RING:
			case IACCESSORY_NECKLACE:
				pItemProto->is_allow_socket = true;
				break;
			}
		}

		{
			pItemProto->is_socket_type_ = true;

			if( !(pItemProto->getItemFlag() & ITEM_FLAG_SOCKET) )
			{
				pItemProto->is_socket_type_ = false;
			}
			if (pItemProto->isAllowSocketItem() == false)
			{
				pItemProto->is_socket_type_ = false;
			}
			if ((pItemProto->getItemTypeIdx() == ITYPE_WEAPON) &&
					 (pItemProto->getItemSubTypeIdx() == IWEAPON_MINING || pItemProto->getItemSubTypeIdx() == IWEAPON_GATHERING || pItemProto->getItemSubTypeIdx() == IWEAPON_CHARGE))
			{
				pItemProto->is_socket_type_ = false;
			}
			if (pItemProto->getItemJobFlag() & (1<<JOB_APET))
			{
				pItemProto->is_socket_type_ = false;
			}
			if (pItemProto->getItemFlag() & ITEM_FLAG_COMPOSITE || pItemProto->getItemFlag() & ITEM_FLAG_COSTUME2 || pItemProto->getItemFlag() & ITEM_FLAG_LORD)
			{
				pItemProto->is_socket_type_ = false;
			}
		}
		
		pItemProto->is_allow_socket = (pItemProto->getItemTypeIdx() == ITYPE_WEAPON || pItemProto->getItemTypeIdx() == ITYPE_WEAR);
		pItemProto->is_process_scroll_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_PROCESS_SCROLL);

		pItemProto->is_rebirth_item_ = false;
		switch (pItemProto->getItemIndex())
		{
		case 846:
		case 2667:
		case 3218:
		case 4933:
		case 5958:
		case 7056:
		case 9194:
		case 9195:
		case 9196:
			pItemProto->is_rebirth_item_ = true;
			break;
		}

		switch(pItemProto->getItemIndex())
		{
		case 6252:
		case 6253:
		case 6254:
		case 6255:
			pItemProto->is_monster_card_ = true;
			break;

		default:
			pItemProto->is_monster_card_ = false;
			break;
		}

		// RVR 지역에서 사용할 수 없는 아이템
		pItemProto->is_rvr_not_use_ = false;
		switch(pItemProto->getItemIndex())
		{
		case 2407:	// 광속 5일
		case 2408:	// 광속 30일
		case 2500:	// 이벤트 광속 5일
		case 2609:	// 크리스마스 광속 5일
		case 5018:	// 광속2 5일
		case 5019:	// 광속2 30일
		case 47:	// 메모리 스크롤
		case 858:	// 소환 마법서
		case 857:	// 순간 이동 마법서
		case 1298:	// 파티 리콜 1일 정액권
		case 1299:	// 파티 리콜 7일 정액권
		case 1300:	// 파티 리콜 30일 정액권
		case 1413:	// 파티 리콜 3시간 정액권
		case 1415:	// 파티 리콜 1시간 정액권
		case 2945:	// 아이리스 파티 리콜 정액권
		case 2946:	// 아스카디아 파티 리콜 정액권
		case 6252:	// 몬스터 용병 카드 1시간
		case 6253:	// 몬스터 용병 카드 3시간
		case 6254:	// 몬스터 용병 카드 9시간
		case 6255:	// 몬스터 용병 카드 24시간
		case 6805:	// 초보자 몬스터 용병 카드
		case 6985:	// 몬스터 용병 카드 1일
		case 6986:	// 몬스터 용병 카드 7일
		case 6987:	// 몬스터 용병 카드 30일
			pItemProto->is_rvr_not_use_ = true;
			break;

		default:
			pItemProto->is_rvr_not_use_ = false;
			break;
		}

		if (m_protoItems[ridx].getItemFlag() & ITEM_FLAG_LORD)
		{
			m_flagload.push_back(&m_protoItems[ridx]);
		}

		if (m_protoItems[ridx].getItemFlag() & ITEM_FLAG_CHANGE)
		{
			m_flagchange.push_back(&m_protoItems[ridx]);
		}

		// 전장에서 사용가능한 아이템인지 구분
		if( pItemProto->getItemTypeIdx() == ITYPE_ONCE && pItemProto->getItemSubTypeIdx() == IONCE_WARP
				|| pItemProto->getItemTypeIdx() == ITYPE_ONCE && pItemProto->getItemSubTypeIdx() == IONCE_SUMMON
				|| pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pItemProto->getItemSubTypeIdx() == IACCESSORY_PET
				|| pItemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pItemProto->getItemSubTypeIdx() == IACCESSORY_ATTACK_PET
				|| pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD
		  )
		{
			pItemProto->is_useinwarzone_ = false;
		}
		else
		{
			pItemProto->is_useinwarzone_ = true;
		}


		pItemProto->is_rvr_jewel_ = (pItemProto->getItemTypeIdx() == ITYPE_ETC && pItemProto->getItemSubTypeIdx() == IETC_RVR_JEWEL) ? true : false;

		pItemProto->is_timer_ = checkTimerItem(pItemProto);

		pItemProto->is_syndicate_ = (pItemProto->getItemRvRType() > 0) ? true : false;

		pItemProto->is_gps_ = (pItemProto->getItemTypeIdx() == ITYPE_ONCE && pItemProto->getItemSubTypeIdx() == IONCE_GPS) ? true : false;

		pItemProto->is_questGive = (pItemProto->getItemFlag() & ITEM_FLAG_QUESTGIVE) ? true : false;

		map_.insert(map_t::value_type(pItemProto->m_index, &m_protoItems[ridx]));

		ridx++;
	}

	m_nCount = ridx;

	return bRet;
}

CItemProto* CItemList::FindIndex(int idx)
{
	map_t::iterator it = map_.find(idx);
	return (it != map_.end()) ? it->second : NULL;
}

#ifdef DURABILITY
CItem* CItemList::CreateDBItem( int idx, char wearPos, int plus, int flag, int used,
								int used_2, const char* serial, LONGLONG count, short* option
								, char socketcount, int* jewel
								, int* OriginVar, int now_dur, int max_dur
							  )
#else
CItem* CItemList::CreateDBItem( int idx, char wearPos, int plus, int flag, int used,
								int used_2, const char* serial, LONGLONG count, short* option
								, char socketcount, int* jewel
								, int* OriginVar
							  )
#endif
{
	if (count == 0)
		return NULL;

	if (idx == 0)
		idx = NAS_ITEM_DB_INDEX;

	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		return NULL;
	}

	// 아이템 설정
	CItem* item = new CItem;

	item->m_itemProto = proto;

	item->m_tab_idx = -1;

	item->m_virualIndex = GetNextVirtualIndex();
	item->m_dbIndex = proto->m_index;
	item->setWearPos(wearPos);
	if (idx == 1577 || idx == 1578 || idx == 2047)
		item->setPlus(plus);
	else if( proto->getItemTypeIdx() == ITYPE_ACCESSORY && ( proto->getItemSubTypeIdx() == IACCESSORY_PET || proto->getItemSubTypeIdx() == IACCESSORY_ATTACK_PET ) )
	{
		item->setPlus(plus);
		item->setPlus_2(0);
	}
	else if (item->Is_fortune_item())
	{
		item->setPlus(plus);
	}
	else
	{
		item->setPlus(plus & 0x0000ffff);
		item->setPlus_2((plus >> 16) & 0x0000ffff);
	}
	item->setFlag(flag);
	item->setUsed(used);
	item->setUsed_2(used_2);

	if(serial == NULL)
	{
		item->m_serial = GetSerial();
	}
	else if(strcmp(serial, "") == 0)
	{
		item->m_serial = GetSerial();
	}
	else
	{
		item->m_serial = serial;
	}

	item->setItemCount(this->calcItemCount(proto, count));

	bool bRareItem = false;

	if (item->IsRareItem())
		bRareItem = true;

	if (bRareItem)
	{
		if (option[1] != 0)
		{
			item->m_pRareOptionProto = gserver->m_rareOptionList.Find(option[0]);
			if (item->m_pRareOptionProto)
			{
				item->m_nRareOptionIndex = option[0];
				item->m_nRareOptionBit = option[1];
				item->m_nOption = 2;
				item->m_option[0].GetDBValue(option[0]);
				item->m_option[1].GetDBValue(option[1]);
			}
		}
	}
	else if (item->IsOriginItem())
	{
		item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
	}
	else
	{
		// Option Setting
		int i;
		for (i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(option[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver->m_optionProtoList.FindProto(item->m_option[i].m_type);

			if (!item->m_option[i].m_proto)
				continue;

			item->m_option[item->m_nOption].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
			++item->m_nOption;
		}
	}

	if( ( proto->m_index == 2047 || proto->m_index == 2633 )
			&& item->getUsed() == -1)					// 초코수집상자 or 새해 소망상자
	{
		item->setUsed(0);
	}

	if (proto->m_index == 1577 || proto->m_index == 1578)
	{
		// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로
		// -1로 생성시 0으로 초기화
		if(item->getUsed() == -1)
		{
			item->setUsed(0);
		}
	}

	if(proto->getItemTypeIdx() == ITYPE_ETC &&
			proto->getItemSubTypeIdx() == IETC_JEWEL &&
			proto->getItemSet1() == 0 &&
			proto->m_nRareIndex[0] >= 0 && proto->m_nRareProb[0] > 0)
	{
		COptionProto* OptionProto = gserver->m_optionProtoList.FindProto(proto->m_nRareIndex[0]);
		if(OptionProto)
		{
			item->m_option[0].m_proto = OptionProto;
			item->m_option[0].m_type = OptionProto->m_type;
			item->m_option[0].m_level = proto->m_nRareProb[0];
			item->m_option[0].m_value = OptionProto->m_levelValue[item->m_option[0].m_level - 1];
			item->m_option[0].SetDBValue();
			item->m_nOption = 1;
		}
	}

	if(item->IsSocketItem())
	{
		int flag = item->getFlag();
		item->m_socketList.InitSocketFromDB(proto->getItemTypeIdx(), proto->getItemSubTypeIdx(), socketcount, jewel, &flag);
		item->setFlag(flag);
	}
	if (item->IsOriginItem() && OriginVar != NULL)
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if (OriginVar[i] > 0 && item->GetOriginIndex(i) >= 0)
			{
				item->setOriginVar(i, OriginVar[i]);
			}
			else if (OriginVar[i] == 0 && item->GetOriginIndex(i) >= 0)
			{
				item->setOriginVar(i, 100);
			}
			else item->setOriginVar(i, 0);
		}
	}

#ifdef DURABILITY
	item->setNowDurability(now_dur);
	item->setMaxDurability(max_dur);
#endif

	return item;
}

CItem* CItemList::CreateItem(int idx, char wearPos, int plus, int flag, LONGLONG count)
{
	if (count == 0)
		return NULL;

	if (idx == 0)
		idx = NAS_ITEM_DB_INDEX;

	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		GAMELOG << init("SYS_ERR")
				<< "Can't Create Item Index"
				<< idx << delim
				<< plus << delim
				<< flag << delim
				<< count
				<< end;
		return NULL;
	}

	CItem* item = new CItem;

	item->m_itemProto = proto;

	item->m_tab_idx = -1;

	item->m_virualIndex = GetNextVirtualIndex();
	item->setDBIndex(proto->m_index);
	item->setWearPos(wearPos);
	item->setPlus(plus);
	item->setFlag(flag);

	// 생성하려는 아이템이 NAS일 경우 따로 처리하고 리턴함
	if (item->getDBIndex() == NAS_ITEM_DB_INDEX)
	{
		item->setItemCount(count);
		return item;
	}

	item->setUsed(proto->getItemMaxUse());

	if (item->IsWeaponType() || item->IsArmorType())
	{
		if (item->m_itemProto->getItemSet0() > 0)
		{
			int nRandom = GetRandom(item->m_itemProto->getItemNum0(), item->m_itemProto->getItemSet0());
			item->setPlus_2(nRandom - item->m_itemProto->getItemNum0());
		}
		else if (item->m_itemProto->getItemSet1() > 0)
		{
			int nRandom = GetRandom(item->m_itemProto->getItemNum1(), item->m_itemProto->getItemSet1());
			item->setPlus_2(nRandom - item->m_itemProto->getItemNum1());
		}
		else
		{
			item->setPlus_2(0);
		}
	}

	if (item->IsOriginItem())
	{
		item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
	}

	if (proto->m_index == 1577 || proto->m_index == 1578)
	{
		// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로
		// 생성시 0으로 초기화
		item->setUsed(0);
	}

	item->m_serial = GetSerial();

	item->setItemCount(this->calcItemCount(proto, count));

	// 060227 : bs : 파티리콜
	if (proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_WARP && proto->getItemNum0() == IONCE_WARP_PARTYRECALL)
	{
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);
	}

// 창고 이용 주문서
	if( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0)
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);

	if (proto->m_index == ONE_PERIOD_ITEM || proto->m_index == 2610 || proto->m_index == 4940 )
	{
		item->setUsed(gserver->getNowSecond() + 1 * 24 * 60 * 60);
	}

	if (proto->m_index == SEVEN_PERIOD_ITEM || proto->m_index == 4941 )
	{
		item->setUsed(gserver->getNowSecond() + 7 * 24 * 60 * 60);
	}

	if (proto->m_index == THIRTY_PERIOD_ITEM || proto->m_index == 4942 )
	{
		item->setUsed(gserver->getNowSecond() + 30 * 24 * 60 * 60);
	}

	// 유료 아이템 : num4에 일단위
	if (proto->getItemFlag() & ITEM_FLAG_COMPOSITE)
	{
#ifdef CHARGE_ITEM_PERIOD
		int nItemNo = item->m_dbIndex;
		if( ( 1631 < nItemNo && nItemNo < 1704 ) || ( 1721 < nItemNo && nItemNo < 1794 ) )
		{
			item->setUsed(gserver->getNowSecond() + 7 * 24 * 60 * 60);
		}
		else
#endif	// CHARGE_ITEM_PERIOD
#ifdef NO_PERIOD_COMPOSITE
			if(proto->getItemNum4() != -1)
#endif
				item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);
	}

	if (proto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		if(proto->getItemNum4() != -1)
		{
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);	// 실제 하루
		}
	}

	if (proto->getItemFlag() & ITEM_FLAG_ABS)
	{
		// 시간제 아이템은 m_num4에 시간이 들어간다
#ifdef SYSTEM_TREASURE_MAP
		if(proto->m_index == TREASURE_MAP_ITEM_INDEX ) // 이넘은 분으로 계산한다.
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 60);
		else
#endif
		//창조의 보석은 분단위로 계산한다.
		if(proto->m_typeIdx == ITYPE_ETC && proto->m_subtypeIdx == IETC_RVR_JEWEL)
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 60);
		else
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 60 * 60);
	}

	// 골든볼 이벤트 응모권 : GOLDENBALL_CARD_INDEX
	// used에 경기 시작 시간(응모 마감 시간)을 저장
	if (proto->m_index == GOLDENBALL_CARD_INDEX)
	{
		item->setUsed(gserver->m_clGoldenBall.GetVoteEndTime());
	}

	if (item->IsLent())
	{
		item->setUsed(gserver->getNowSecond() + 24 * 60 * 60);
	}

	if( proto->m_index == 2047 || proto->m_index == 2633 )	// 초코수집상자 or 새해 소망상자
	{
		item->setUsed(0);
	}

	if (proto->m_index == 2137)		// gpara 악세
	{
		short nGparaOption[5] =
		{
			((20 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			((21 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			(( 7 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			((16 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			(( 8 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
		};
		item->m_nOption = 0;
		for (int i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(nGparaOption[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver->m_optionProtoList.FindProto(item->m_option[i].m_type);

			if (!item->m_option[i].m_proto)
				continue;

			item->m_option[item->m_nOption].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
			++item->m_nOption;
		}
	}

	// hc 라카기자단 확성기 기간 30일
	if(proto->m_index == 2882)
	{
		item->setUsed(gserver->getNowSecond() + 30 * 24 * 60 * 60);
	}

	// 피닉스 투구 아이템일 경우 기간 무제한
	if( proto->m_index >= 3006 && proto->m_index <= 3011 )
	{
		item->setUsed(-1);
	}

	if( (proto->getItemTypeIdx() == ITYPE_ACCESSORY) && (proto->getItemNum4() > 0) )
	{
		item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);
	}

	if(proto->getItemTypeIdx() == ITYPE_ETC &&
			proto->getItemSubTypeIdx() == IETC_JEWEL &&
			proto->getItemSet0() == 0 &&
			proto->m_nRareIndex[0] >= 0 && proto->m_nRareProb[0] > 0)
	{
		COptionProto* OptionProto = gserver->m_optionProtoList.FindProto(proto->m_nRareIndex[0]);
		if(OptionProto)
		{
			item->m_option[0].m_proto = OptionProto;
			item->m_option[0].m_type = OptionProto->m_type;
			item->m_option[0].m_level = proto->m_nRareProb[0];
			item->m_option[0].m_value = OptionProto->m_levelValue[item->m_option[0].m_level - 1];
			item->m_option[0].SetDBValue();
			item->m_nOption = 1;
		}
	}

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ETC && item->m_itemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD  )
	{
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
		if( !gserver->m_MonsterMercenary.SetMercenaryData( item ) )
			return false;
#else
		MonsterMercenaryInfo * mmd = gserver->m_MonsterMercenary.GetClassInfo();
		if( mmd == NULL )
		{
			GAMELOG << init("MOSTER MERCENARY FAIL : get MercenaryInfo") << end;
			return false;
		}

		item->setUsed(-1);
		item->setPlus(gserver->m_MonsterMercenary.GetMercenaryIndex(mmd->nClassIdx)); // Mercenary npc index
		item->setUsed_2(mmd->nClassIdx);  // classIdx
		item->setPlus_2(0); // 앞의 16 비트는 공격력을 넣는다. 뒤의 16비트는 방어력을 넣는다.
		int att=0, def=0;

		int nRand = GetRandom(0,1);
		switch( nRand )
		{
		case 0: // 공격만 보너스준다.
			att = GetRandom(mmd->nBonusMini, mmd->nBonusMax);
			break;
		case 1: // 수비만 디펜스를 준다.
			def = GetRandom(mmd->nBonusMini, mmd->nBonusMax);
			break;
		}
		item->setPlus_2(item->getPlus_2() + (att << 8));
		item->setPlus_2(item->getPlus_2() + def);

		if( item->getUsed_2() == -1 )
		{
			GAMELOG << init("MOSTER MERCENARY FAIL : get MercenaryNpcIndex") << end;
			return false;
		}
#endif
	}
	if (item->IsOriginItem())
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if(item->GetOriginIndex(i) >= 0)
			{
				item->setOriginVar(i, 100);
			}
		}
	}

#ifdef DURABILITY
	item->setNowDurability(proto->getDurability());
	item->setMaxDurability(proto->getDurability());
#endif

	return item;
}

CItem* CItemList::CreateAutoGiveDBItem(int idx, char wearPos, int plus, int flag, int used,
									   int used_2,	const char* serial, LONGLONG count, short* option
									   ,char socketcount, int* jewelidx
									  )
{
	if (count == 0)
		return NULL;

	if (idx == 0)
		idx = NAS_ITEM_DB_INDEX;

	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		return NULL;
	}

	// 아이템 설정
	CItem* item = new CItem;

	item->m_itemProto = proto;

	item->m_tab_idx = -1;

	item->m_virualIndex = GetNextVirtualIndex();
	item->setDBIndex(proto->m_index);
	item->setWearPos(wearPos);
	item->setPlus(plus);
	item->setFlag(flag);
	item->setUsed(used);
	item->setUsed_2(used_2);
	item->m_serial = serial;
	item->setItemCount(this->calcItemCount(proto, count));
	item->setPlus_2(0);

	bool bRareItem = false;

	if (item->IsRareItem())
		bRareItem = true;

	if (bRareItem)
	{
		if (option[1] != 0)
		{
			item->m_pRareOptionProto = gserver->m_rareOptionList.Find(option[0]);
			if (item->m_pRareOptionProto)
			{
				item->m_nRareOptionIndex = option[0];
				item->m_nRareOptionBit = option[1];
				item->m_nOption = 2;
				item->m_option[0].GetDBValue(option[0]);
				item->m_option[1].GetDBValue(option[1]);
			}
		}
	}
	else
	{
		// Option Setting
		int i;
		for (i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(option[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver->m_optionProtoList.FindProto(item->m_option[i].m_type);

			if (!item->m_option[i].m_proto)
				continue;
			item->m_option[item->m_nOption].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
			++item->m_nOption;
		}
	}

	if (item->IsOriginItem())
	{
		item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
	}

	if( ( proto->m_index == 2047 || proto->m_index == 2633 )
			&& item->getUsed() == -1)					// 초코수집상자 or 새해 소망상자
	{
		item->setUsed(0);
	}

	if (proto->m_index == 1577 || proto->m_index == 1578)
	{
		// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로
		// -1로 생성시 0으로 초기화
		if(item->getUsed() == -1)
		{
			item->setUsed(0);
		}
	}

	// 유료 아이템 : num4에 일단위
	if (proto->getItemFlag() & ITEM_FLAG_COMPOSITE)
	{
#ifdef CHARGE_ITEM_PERIOD
		int nItemNo = item->m_dbIndex;
		if( ( 1631 < nItemNo && nItemNo < 1704 ) || ( 1721 < nItemNo && nItemNo < 1794 ) )
		{
			item->setUsed(gserver->getNowSecond() + 7 * 24 * 60 * 60);
		}
		else
#endif	// CHARGE_ITEM_PERIOD
#ifdef NO_PERIOD_COMPOSITE
			if(proto->getItemNum4() != -1)
#endif
			{
				item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);
			}
	}

	// hc 라카기자단 확성기 기간 30일
	if(proto->m_index == 2882)
	{
		item->setUsed(gserver->getNowSecond() + 30 * 24 * 60 * 60);
	}

	// 피닉스 투구 아이템일 경우 기간 무제한
	if( proto->m_index >= 3006 && proto->m_index <= 3011 )
	{
		item->setUsed(-1);
	}

	if (proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_WARP && proto->getItemNum0() == IONCE_WARP_PARTYRECALL)
	{
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);
	}
	// 창고 이용 주문서
	if( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0)
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);

	// 잡화상 이용 주문서
	if( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0)
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);

	// 써치 라이프
	if( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0)
		item->setUsed(gserver->getNowSecond() + proto->getItemNum3() * 60 * 60);
	if (proto->m_index == ONE_PERIOD_ITEM || proto->m_index == 2610 || proto->m_index == 4940 )
	{
		item->setUsed(gserver->getNowSecond() + 1 * 24 * 60 * 60);
	}

	if (proto->m_index == SEVEN_PERIOD_ITEM || proto->m_index == 4941 )
	{
		item->setUsed(gserver->getNowSecond() + 7 * 24 * 60 * 60);
	}

	if (proto->m_index == THIRTY_PERIOD_ITEM || proto->m_index == 4942 )
	{
		item->setUsed(gserver->getNowSecond() + 30 * 24 * 60 * 60);
	}

	if (proto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		if(proto->m_num4 != -1)
		{
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);	// 실제 하루
		}
	}

	if (proto->getItemFlag() & ITEM_FLAG_ABS)
	{
		// 시간제 아이템은 m_num4에 시간이 들어간다
#ifdef SYSTEM_TREASURE_MAP
		if(proto->m_index == TREASURE_MAP_ITEM_INDEX ) // 이넘은 분으로 계산한다.
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 60);
		else
#endif
			item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 60 * 60);
	}

	if (proto->m_index == GOLDENBALL_CARD_INDEX)
	{
		item->setUsed(gserver->m_clGoldenBall.GetVoteEndTime());
	}

	if (item->IsLent())
	{
		item->setUsed(gserver->getNowSecond() + 24 * 60 * 60);
	}

	if( (proto->getItemTypeIdx() == ITYPE_ACCESSORY) && (proto->getItemNum4() > 0) )
	{
		item->setUsed(gserver->getNowSecond() + proto->getItemNum4() * 24 * 60 * 60);
	}

	if(proto->getItemTypeIdx() == ITYPE_ETC &&
			proto->getItemSubTypeIdx() == IETC_JEWEL &&
			proto->m_nRareIndex[0] >= 0 && proto->m_nRareProb[0] > 0)
	{
		COptionProto* OptionProto = gserver->m_optionProtoList.FindProto(proto->m_nRareIndex[0]);
		if(OptionProto)
		{
			item->m_option[0].m_proto = OptionProto;
			item->m_option[0].m_type = OptionProto->m_type;
			item->m_option[0].m_level = proto->m_nRareProb[0];
			item->m_option[0].m_value = OptionProto->m_levelValue[item->m_option[0].m_level - 1];
			item->m_option[0].SetDBValue();
			item->m_nOption = 1;
		}
	}

	if(item->IsSocketItem())
	{
		int flag = item->getFlag();
		item->m_socketList.InitSocketFromDB(proto->getItemTypeIdx(), proto->getItemSubTypeIdx(), socketcount, jewelidx, &flag);
		item->setFlag(flag);
	}
	if (item->IsOriginItem())
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if(item->GetOriginIndex(i) >= 0)
			{
				item->setOriginVar(i, 100);
			}
		}
	}

#ifdef DURABILITY
	item->setNowDurability(proto->getDurability());
	item->setMaxDurability(proto->getDurability());
#endif

	return item;
}

//#ifdef DEV_GUILD_STASH
CItem* CItemList::CreateDBItem_2( int idx, char wearPos, int plus, int flag, int used, int used_2, const char* serial, LONGLONG count, short* option , const char* socket, int* itemOrigin )
{
	const char* pSocketInfo = socket;
	int socketcount = 0;
	int jewelidx[MAX_SOCKET_COUNT];
	if(*pSocketInfo)
	{
		char buf[10];
		pSocketInfo = AnyOneArg(pSocketInfo, buf);
		jewelidx[0] = atoi(buf);

		pSocketInfo = AnyOneArg(pSocketInfo, buf);
		socketcount = atoi(buf);
		if(socketcount >= 1 && socketcount <= MAX_SOCKET_COUNT)
		{
			int i;
			for(i = 1; i < MAX_SOCKET_COUNT && *pSocketInfo; i++)
			{
				pSocketInfo = AnyOneArg(pSocketInfo, buf);
				jewelidx[i] = atoi(buf);
			}
		}
		else
		{
			socketcount = 0;
			memset(jewelidx, -1, MAX_SOCKET_COUNT);
		}
	}
	else
	{
		socketcount = 0;
		memset(jewelidx, -1, MAX_SOCKET_COUNT);
	}

#ifdef DURABILITY
	return CreateDBItem( idx, wearPos, plus, flag, used, used_2, serial, count, option, socketcount, jewelidx, itemOrigin, 0, 0 );
#else
	return CreateDBItem( idx, wearPos, plus, flag, used, used_2, serial, count, option, socketcount, jewelidx, itemOrigin );
#endif
}

int CItemList::GetNextVirtualIndex()
{
	boost::mutex::scoped_lock lock(m_mutex_for_virtualIndex);

	++m_virtualIndex;

	if (m_virtualIndex < 1)
		m_virtualIndex = 1;

	return m_virtualIndex;
}

LONGLONG CItemList::calcItemCount( CItemProto* proto, LONGLONG count )
{
	if (proto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		if (proto->getStackCount() < count)
			count = proto->getStackCount();
	}
	else
	{
		count = 1;
	}

	return count;
}

bool CItemList::checkTimerItem( CItemProto* proto )
{
	if (proto->getItemFlag() & ITEM_FLAG_ABS)
		return true;

	if (proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0)
		return true;

	if (proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_WARP && proto->getItemNum0() == IONCE_WARP_PARTYRECALL)
		return true;

	switch (proto->m_index)
	{
	case ONE_PERIOD_ITEM:
	case 2610:
	case 4940:
	case SEVEN_PERIOD_ITEM:
	case 4941:
	case THIRTY_PERIOD_ITEM:
	case 4942:
	case GOLDENBALL_CARD_INDEX:
	case 2047:
	case 2633:
#ifdef SYSTEM_TREASURE_MAP
	case TREASURE_MAP_ITEM_INDEX:
#endif
	case 2882:
	case 3006:
	case 3007:
	case 3008:
	case 3009:
	case 3010:
	case 3011:
	case 2455:
	case 2456:
	case 2457:
	case 2458:
	case 2459:
	case 2460:
	case 2607:
	case 2608:
	case 2868:
	case 2461:
	case 2462:
	case 2463:
	case 2606:
	case EXPERIENCE_COMPETITION_ITEM:
	case PERIOD_COMPETITION_ITEM:
	case 2609:
	case 5018:
	case 5019:
	case 4913:

		return true;
	}

	if (proto->isRvrJewel())
		return true;

	if (proto->getItemFlag() & ITEM_FLAG_COSTUME2 && proto->getItemNum4() > 0)
		return true;

	if (proto->getItemTypeIdx() == ITYPE_ACCESSORY && proto->getItemNum4() > 0)
		return true;

	if (proto->getItemFlag() & ITEM_FLAG_COMPOSITE && proto->getItemNum4() > 0)
		return true;

	if (proto->getItemFlag() & ITEM_FLAG_TRIGGER)
		return true;

	if (proto->getItemTypeIdx() == ITYPE_ETC && proto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD)
		return true;

	return false;
}

//#endif //DEV_GUILD_STASH

CCatalog* CCatalogList::FindItemToCatalog(int itemDBIndex)
{
	int i;
	int count = this->GetCount();

	CCtItemList* itemList = NULL;
	for( i = 0; i < count; i++ )
	{
		if( !catalog[i].GetEnable()
				|| catalog[i].GetCategory() == CATEGORY_PACKAGE
				|| catalog[i].GetType() == TYPE_EQUIP_EQUIPMENT )
			continue;

		itemList = this->catalog[i].GetCtItemList();
		if( itemList )
		{
			CItem* item = itemList->GetItem(0);
			if( !item || !item->m_itemProto )
				continue;

			if( item->m_itemProto->getItemIndex() == itemDBIndex && item->getPlus() == 0 && item->getFlag() == 0 && item->Count() == 1 )
				return &catalog[i];
		}
	}

	return NULL;
}

CCatalog* CCatalogList::FindItemToCatalog(CItem* pItem)
{
	int i;
	int count = this->GetCount();

	CCtItemList* itemList = NULL;
	for( i = 0; i < count; i++ )
	{
		if( !catalog[i].GetEnable()
				|| catalog[i].GetCategory() == CATEGORY_PACKAGE
				|| catalog[i].GetType() == TYPE_EQUIP_EQUIPMENT )
			continue;

		itemList = this->catalog[i].GetCtItemList();
		if( itemList )
		{
			CItem* item = itemList->GetItem(0);

			if( !item || !item->m_itemProto || !pItem || !pItem->m_itemProto )
				continue;

			if( item->getDBIndex() == pItem->getDBIndex() && item->getPlus() == pItem->getPlus() && item->getFlag() == pItem->getFlag() && item->Count() == 1 )
				return &catalog[i];
		}
	}

	return NULL;
}

bool CCatalogList::Load()
{
	memset(m_typelist, 0, sizeof(int)*TYPE_MAX_COUNT);
	memset(m_subtypelist, 0, sizeof(int)*SUBTYPE_MAX_COUNT);
	if (this->catalog)
		delete [] this->catalog;

	this->catalog = NULL;
	this->m_cCount = 0;

	CDBCmd cmd;

	CLCString sql(4096);

#if defined(USE_TENTER_BILLING)
	cmd.Init(&gserver->m_dbcatal);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_catalog_hardcore order by a_ctid DESC", GAMEID);
	}
	else
	{
		sql.Format("select * from t_catalog_%d order by a_ctid DESC", GAMEID);
	}
#else
	cmd.Init(&gserver->m_dbdata);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_catalog_hardcore order by a_ctid DESC");
	}
	else
	{
		sql.Format("select * from t_catalog order by a_ctid DESC");
	}
#endif
	cmd.SetQuery(sql);

	if( !cmd.Open() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	this->m_cCount = cmd.m_nrecords;
	this->catalog = new CCatalog[m_cCount];

	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	int i = 0;
	char enable = 0;

	do
	{
		//int		m_index;		// 카탈로그 인덱스
		//CLCString m_name;
		//int		m_type;			// 타입
		//int		m_cash;			// 가격
		//int		m_mileage;		// 마일리지
		//int		m_toSell;			// 판매 남은 갯수
		//int		m_flag;
		//bool		m_enable;
		int index, type, cash, mileage, flag;
		int category, subtype;
		CLCString name(MAX_ITEM_NAME_LENGTH+1);

		if( !cmd.GetRec("a_ctid", index)
				|| !cmd.GetRec("a_ctname", name)
				|| !cmd.GetRec("a_type", type)
				|| !cmd.GetRec("a_category", category)
				|| !cmd.GetRec("a_subtype", subtype)
				|| !cmd.GetRec("a_cash", cash)
				|| !cmd.GetRec("a_mileage", mileage)
				|| !cmd.GetRec("a_flag", flag)
				|| !cmd.GetRec("a_enable", enable)
		  )
		{
			GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
			return false;
		}
		CCtItemList* ctItemList = new CCtItemList;
		ctItemList->Load(index);
		if(category != CATEGORY_PACKAGE && ctItemList && ctItemList->GetCount() == 1 && ctItemList->GetItem(0) && ctItemList->GetItem(0)->m_itemProto->getItemIndex() == 855 && ctItemList->GetItem(0)->Count() == 1)
			m_recoveryCtid = i;

		catalog[i].SetCatalog(index, name, category, type, subtype, cash, mileage, enable == 1 ? true : false, flag, ctItemList);

		if(catalog[i].GetToSell() > CATALOG_FLAG_MAX_LIMIT || catalog[i].GetToSell() < 0 )
		{
			GAMELOG << init("CATALOGLIST_LOAD_FLAG_LIMIT_ERROR") << end;
			return false;
		}
#if defined( LC_USA ) || defined( LC_TLD )
		if( catalog[i].GetToSell() != 0 || catalog[i].GetCategory() == CATEGORY_PLATINUM )
#else
		if(catalog[i].GetToSell() != 0 )
#endif
		{
			gserver->m_limitCT.insert(catalog[i].GetIndex());
		}

		i++;
	}
	while( cmd.MoveNext() );
#if defined (USE_TENTER_BILLING)
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_type from t_catalog_hardcore where a_enable = 1  group by a_type order by a_type asc", GAMEID);
	}
	else
	{
		sql.Format("select a_type from t_catalog_%d where a_enable = 1  group by a_type order by a_type asc", GAMEID);
	}
#else
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_type from t_catalog_hardcore where a_enable = 1  group by a_type order by a_type asc");
	}
	else
	{
		sql.Format("select a_type from t_catalog where a_enable = 1  group by a_type order by a_type asc");
	}
#endif
	int temp = 0;
	int typelist = 0;
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;
	if(!cmd.MoveFirst())
		return false;

	do
	{
		if(cmd.GetRec("a_type", typelist))
		{
			if(typelist != 0)
			{
				if(typelist % 10000 != 0)
				{
					m_typelist[temp] = typelist;
					temp++;
				}
			}
		}
	}
	while(cmd.MoveNext());
	m_typeCount = temp;

#if defined (USE_TENTER_BILLING)
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_subtype from t_catalog_hardcore where a_enable = 1  group by a_subtype order by a_subtype asc", GAMEID);
	}
	else
	{
		sql.Format("select a_subtype from t_catalog_%d where a_enable = 1  group by a_subtype order by a_subtype asc", GAMEID);
	}
#else
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_subtype from t_catalog_hardcore where a_enable = 1  group by a_subtype order by a_subtype asc");
	}
	else
	{
		sql.Format("select a_subtype from t_catalog where a_enable = 1  group by a_subtype order by a_subtype asc");
	}
#endif
	temp = 0;
	int subtypelist = 0;
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;
	if(!cmd.MoveFirst())
		return false;

	do
	{
		if(cmd.GetRec("a_subtype", subtypelist))
		{
			if(subtypelist != 0)
			{
				if(subtypelist % 100 != 0)
				{
					m_subtypelist[temp] = subtypelist;
					temp++;
				}
			}
		}
	}
	while(cmd.MoveNext());
	m_subtypeCount = temp;

	return true;
}

bool CCatalogList::Reload()
{
	CDBCmd cmd;
	CLCString sql(1024);

	GAMELOG << init("CATALOG RELOAD TEST : BEFORE ") << m_cCount << delim << catalog[0].GetName() << delim << catalog[0].GetCash()
			<< delim << itemlog(catalog[0].GetCtItemList()->GetItem(0),false) << end;

	int			cNewCount=0;
	CCatalog*	pNewcatalog;

#if defined(USE_TENTER_BILLING)
	cmd.Init(&gserver->m_dbcatal);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_catalog_hardcore order by a_ctid DESC", GAMEID);
	}
	else
	{
		sql.Format("select * from t_catalog_%d order by a_ctid DESC", GAMEID);
	}
#else
	cmd.Init(&gserver->m_dbdata);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_catalog_hardcore order by a_ctid DESC");
	}
	else
	{
		sql.Format("select * from t_catalog order by a_ctid DESC");
	}
#endif
	cmd.SetQuery(sql);

	if( !cmd.Open() )
	{
		GAMELOG << init("CATALOGLIST_RELOAD_ERROR") << end;
		return false;
	}

	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("CATALOGLIST_RELOAD_ERROR") << end;
		return false;
	}

	cNewCount = cmd.m_nrecords;
	pNewcatalog = new CCatalog[cNewCount];

	int i = 0;
	char enable = 0;

	gserver->m_limitCT.clear();

	do
	{
		int index, type, cash, mileage, flag;
		int category, subtype;
		CLCString name(MAX_ITEM_NAME_LENGTH+1);

		if( !cmd.GetRec("a_ctid", index)
				|| !cmd.GetRec("a_ctname", name)
				|| !cmd.GetRec("a_type", type)
				|| !cmd.GetRec("a_category", category)
				|| !cmd.GetRec("a_subtype", subtype)
				|| !cmd.GetRec("a_cash", cash)
				|| !cmd.GetRec("a_mileage", mileage)
				|| !cmd.GetRec("a_flag", flag)
				|| !cmd.GetRec("a_enable", enable)
		  )
		{
			GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
			delete[] pNewcatalog;
			return false;
		}

		CCtItemList* ctItemList = new CCtItemList;
		ctItemList->Load(index);
		if( category != CATEGORY_PACKAGE && ctItemList && ctItemList->GetCount() == 1 && ctItemList->GetItem(0) &&
				ctItemList->GetItem(0)->m_itemProto->getItemIndex() == 855 && ctItemList->GetItem(0)->Count() == 1)
		{
			m_recoveryCtid = i;
		}

		if(pNewcatalog[i].GetToSell() > CATALOG_FLAG_MAX_LIMIT || pNewcatalog[i].GetToSell() < 0 )
		{
			GAMELOG << init("CATALOGLIST_RELOAD_FLAG_LIMIT_ERROR") << end;
			delete[] pNewcatalog;
			return false;
		}
#if defined( LC_USA ) || defined( LC_TLD )
		if( pNewcatalog[i].GetToSell() != 0 || pNewcatalog[i].GetCategory() == CATEGORY_PLATINUM )
#else
		if(pNewcatalog[i].GetToSell() != 0 )
#endif
		{
			gserver->m_limitCT.insert(pNewcatalog[i].GetIndex());
		}

		i++;
	}
	while( cmd.MoveNext() );

	// 로딩후 바꿔치기
	CCatalog* temp = catalog;
	catalog = pNewcatalog;
	delete[]	temp;
	m_cCount = cNewCount;

	GAMELOG << init("CATALOG RELOAD TEST : AFTER ") << m_cCount << delim << catalog[0].GetName() << delim << catalog[0].GetCash()
			<< delim << itemlog(catalog[0].GetCtItemList()->GetItem(0),false) << end;

	return true;
}
bool CCatalogList::ReloadPrice()
{
	GAMELOG << init("PRICE RELOADING START") << end;
	CDBCmd cmd;
	CLCString sql(1024);
	int ctid = 0;
	int price = 0;
#if defined(USE_TENTER_BILLING)
	cmd.Init(&gserver->m_dbcatal);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_ctid, a_cash from t_catalog_hardcore order by a_ctid DESC", GAMEID);
	}
	else
	{
		sql.Format("select a_ctid, a_cash from t_catalog_%d order by a_ctid DESC", GAMEID);
	}
#else
	cmd.Init(&gserver->m_dbdata);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select a_ctid, a_cash from t_catalog_hardcore order by a_ctid DESC");
	}
	else
	{
		sql.Format("select a_ctid, a_cash from t_catalog order by a_ctid DESC");
	}
#endif
	cmd.SetQuery(sql);
	CCatalog* pCatal;
	if(!cmd.Open())
	{
		GAMELOG << init("PRICE RELOADING ERROR") << "Table Open Error" << end;
		GAMELOG << init("PRICE RELOADING END") << end;
		return false;
	}
	if(!cmd.MoveFirst())
	{
		GAMELOG << init("PRICE RELOADING ERROR") << "MoveFirst Error" << end;
		GAMELOG << init("PRICE RELOADING END") << end;
		return false;
	}
	do
	{
		if( !cmd.GetRec("a_ctid", ctid) || !cmd.GetRec("a_cash", price) )
		{
			continue;
		}
		else
		{
			pCatal = gserver->m_catalogList.Find(ctid);
			if(pCatal)
			{
				pCatal->SetPrice(price);
			}
		}
	}
	while(cmd.MoveNext());
	GAMELOG << init("PRICE RELOADING END") << end;
	return true;
}

bool CCtItemList::Load(int ctIdx)
{ 
	_delete();

	int itemindex, itemflag, itemplus, itemnum;

	CLCString sql(4096);
	CDBCmd cmd;
#if defined(USE_TENTER_BILLING)
	cmd.Init(&gserver->m_dbcatal);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_ct_item_hardcore where a_ctid=%d", GAMEID, ctIdx);
	}
	else
	{
		sql.Format("select * from t_ct_item_%d where a_ctid=%d", GAMEID, ctIdx);
	}
#else
	cmd.Init(&gserver->m_dbdata);
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		sql.Format("select * from t_ct_item_hardcore where a_ctid=%d", ctIdx);
	}
	else
	{
		sql.Format("select * from t_ct_item where a_ctid=%d", ctIdx);
	}
#endif
	cmd.SetQuery(sql);

	if( !cmd.Open() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	int i =0;
	do
	{
		if( !cmd.GetRec("a_item_idx", itemindex)
				|| !cmd.GetRec("a_item_flag", itemflag)
				|| !cmd.GetRec("a_item_plus", itemplus)
				|| !cmd.GetRec("a_item_num", itemnum) )
		{
			GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
			return false;
		}

		CItem *item = gserver->m_itemProtoList.CreateItem(itemindex, -1, itemplus, itemflag, itemnum);
		ctItem.push_back(item);
		i++;
	}
	while( cmd.MoveNext() );

	return true;
}

CItem* CCtItemList::GetItem(CItem* pItem)
{
	int i;
	int count = this->GetCount();

	for(i = 0; i < count; i++)
	{
		if( this->GetItem(i) && GetItem(i) == pItem )
		{
			return GetItem(i);
		}
	}

	return NULL;
}

void CCtItemList::_delete()
{
	if(this->ctItem.empty() == false)
	{
		std::vector<CItem*>::iterator it = this->ctItem.begin();
		std::vector<CItem*>::iterator it_end = this->ctItem.end();

		for(; it != it_end; it++)
		{
			if((*it) != NULL)
				delete (*it);
		}

		ctItem.clear();
	}
}

void CItem::originReform(unsigned int count_)
{
	if (!this->IsOriginItem())
		return ;

	const int nRandomloop = 5;

	int i, j, min = 50, max, variation = 0;

	min = min + (count_ * 5 / 10);	// 황금 돋보기 하나당 0.5% 씩 증가 50은 minimum

	for (i = 0; i < MAX_ORIGIN_OPTION; i++)
	{
		if (this->GetOriginIndex(i) < 0)
			continue;
		max = this->m_itemProto->getItemProtoOriginVariation(i);
		if (max == 0)
			this->setOriginVar(i, 100);
		else if (min >= max)
			this->setOriginVar(i, max);
		else
		{
			for (j = 0; j < nRandomloop; j++)
			{
				variation = GetRandom(min, max);
				if (j == 0)
					this->setOriginVar(i, variation);
				else
				{
					if (variation < this->getOriginVar(i))
					{
						this->setOriginVar(i, variation);
					}
				}
			}
		}
	}
}

void CItem::setItemCount(LONGLONG count)
{
	if(count < 0) return ;
	m_count = count;
}

void CItem::clone( CItem& destItem )
{
	memcpy(&destItem, this, sizeof(*this) - sizeof(m_serial));
	destItem.m_serial = this->m_serial;
}

ComposeItem::ComposeItem()
{
	m_item_load_count = 0;
}

ComposeItem::~ComposeItem()
{
	delete[] data;
}

ComposeItem* ComposeItem::instance()
{
	static ComposeItem _instance;
	return &_instance;
}

void ComposeItem::load()
{
	FILE* fp = NULL;

	CLCString strTemp(1024);

	strTemp.Format("%s%s", gserver->m_serverpath, COMPOSE_ITEM_FILENAME);

	fp = fopen(strTemp, "rb");

	if(fp == NULL)
	{
		LOG_ERROR("ComposeItem Data Load Fail.....");
		return;
	}

	fread(&m_item_load_count, sizeof(int), 1, fp);

	if(m_item_load_count <= 0)
	{
		fclose(fp);
		return ;
	}

	data = new stComposeData[m_item_load_count];
	fread(data, sizeof(stComposeData) * m_item_load_count, 1, fp);
	fclose(fp);

	init();
}

void ComposeItem::init()
{
	LONGLONG key;
	int dummy_index;
	for(int i = 0; i < m_item_load_count; i++)
	{
		_map.insert(COMPOSE_MAP::value_type(data[i].Material_index, &data[i]));
	}
}

stComposeData* ComposeItem::find( int mat_item_index )
{
	COMPOSE_MAP::iterator it;
	it = _map.find(mat_item_index);

	if(it == _map.end())
		return NULL;

	return it->second;
}
