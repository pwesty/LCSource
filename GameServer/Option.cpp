#include "stdhdrs.h"

#include "Option.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "WarCastle.h"

// 레벨업시 각 스탯당 hp, mp 증가량
extern int levelup_hp[JOBCOUNT][4];
extern int levelup_mp[JOBCOUNT][4];

//////////////////
// COptionProto

COptionProto::COptionProto()
{
	m_idNum = 0;
	m_type = -1;

	m_weaponType = 0;
	m_wearType = 0;
	m_accessoryType = 0;
}

////////////////////
// COptionProtoList

COptionProtoList::COptionProtoList()
{
	m_proto = NULL;
	m_nCount = 0;
}

COptionProtoList::~COptionProtoList()
{
	if (m_proto)
		delete [] m_proto;

	m_proto= NULL;
	m_nCount = 0;
}

bool COptionProtoList::Load()
{
	CDBCmd dbOption;
	dbOption.Init(&gserver->m_dbdata);
	dbOption.SetQuery("SELECT * FROM t_option ORDER BY a_index");

	if (!dbOption.Open())
		return false;

	m_nCount = dbOption.m_nrecords;
	m_proto = new COptionProto[m_nCount];

	// 스킬별 설정
	int i, j;

	if(!dbOption.MoveFirst())
		return true;

	CLCString optionLevel(256);
	CLCString optionProb(256);

	char buf[10];

	for (i=0; i < m_nCount; i++)
	{
		dbOption.GetRec("a_index",			m_proto[i].m_idNum);
		dbOption.GetRec("a_type",			m_proto[i].m_type);

		const char* pLevel = optionLevel;
		const char* pProb = optionProb;

		dbOption.GetRec("a_level",			optionLevel);
		dbOption.GetRec("a_prob",			optionProb);

		j = 0;
		while (*pLevel && *pProb)
		{
			pLevel = AnyOneArg(pLevel, buf);
			m_proto[i].m_levelValue.push_back( atoi(buf) );

			pProb = AnyOneArg(pProb, buf);
			m_proto[i].m_probValue.push_back( atoi(buf) );
			j++;
		}

		dbOption.GetRec("a_weapon_type",	m_proto[i].m_weaponType);
		dbOption.GetRec("a_wear_type",		m_proto[i].m_wearType);
		dbOption.GetRec("a_accessory_type",	m_proto[i].m_accessoryType);

		dbOption.MoveNext();

		map_.insert(map_t::value_type(m_proto[i].m_type, &m_proto[i]));
	}

	return true;
}

COptionProto* COptionProtoList::FindProto(int type)
{
	map_t::iterator it = map_.find(type);
	return (it != map_.end()) ? it->second : NULL;
}

COption::COption()
{
	m_proto = NULL;		// Option Proto
	m_type = -1;		// Option Type
	m_level = 0;		// Option Level

	m_value = 0;		// Option Prob

	m_dbValue = 0;		// Option DB value
}

void COption::MakeOptionValue(COptionProto* proto, int factLevel, int num)
{
	if (!proto)
		return;

	m_proto = proto;
	m_type = proto->m_type;

	// 순서대로 5,4,3,2 레벨로 붙을 확률
	// 악세사리 드롭시 옵션 레벨
	int factor[] =
	{
		m_proto->m_probValue[4],
		m_proto->m_probValue[3],
		m_proto->m_probValue[2],
		m_proto->m_probValue[1]
	};

	static const int limit[] = {100, 500, 2000, 4000};

	int prob[OPTION_MAX_LEVEL - 3];

	if (num == 0)
		num = 1;

	int i;
	for (i=0; i < OPTION_MAX_LEVEL - 3; i++)
	{
		prob[i] = factLevel * factor[i] / num;
		if (prob[i] > limit[i])
			prob[i] = limit[i];
		if (i > 1)
			prob[i] += prob[i - 1];
	}

	// Level 결정
	int result = GetRandom(1, 10000);
	int optionLevel;

	// 일단 3레벨까지만 나올 수 있도록 한다
	/*if (result <= prob[0])
		optionLevel = 3;
	else if (result <= prob[1])
		optionLevel = 3;
	else*/ if (result <= prob[2])
		optionLevel = 3;
	else if (result <= prob[3])
		optionLevel = 2;
	else
		optionLevel = 1;

	m_level = optionLevel;
	m_value = m_proto->m_levelValue[optionLevel - 1];
}

void COption::ApplyOptionValue( CCharacter* ch, int nType, int nValue , CItem* pItem )
{
	if( IS_PC(ch) )
		COption::ApplyOptionValue( TO_PC(ch), nType, nValue, pItem );
	else if ( IS_APET(ch) )
		COption::ApplyOptionValue( TO_APET(ch), nType, nValue, pItem );
	else
		return;
}

void COption::ApplyOptionValue(CAPet* apet, CItem* item)
{
	COption::ApplyOptionValue(apet, m_type, m_value, item);
}

void COption::ApplyOptionValue(CAPet* apet, int nType, int nValue, CItem* pItem)
{
	if (nType < 0)
		return;

	switch(nType)
	{
	case OPTION_STR_UP:
		apet->m_opStr += nValue;
		break;

	case OPTION_DEX_UP:
		apet->m_opDex += nValue;
		break;

	case OPTION_INT_UP:
		apet->m_opInt += nValue;
		break;

	case OPTION_CON_UP:
		apet->m_opCon += nValue;
		break;

	case OPTION_DAMAGE_UP:
		apet->m_opMelee += nValue;
		apet->m_opRange += nValue;
		break;

	case OPTION_MELEE_DAMAGE_UP:
		apet->m_opMelee += nValue;
		break;

	case OPTION_RANGE_DAMAGE_UP:
		apet->m_opRange += nValue;
		break;

	case OPTION_MELEE_HIT_UP:
		apet->m_opMeleeHitRate += nValue;
		break;

	case OPTION_RANGE_HIT_UP:
		apet->m_opRangeHitRate += nValue;
		break;

	case OPTION_DEFENSE_UP:
		apet->m_opDMelee += nValue;
		apet->m_opDRange += nValue;
		break;

	case OPTION_MELEE_DEFENSE_UP:
		apet->m_opDMelee += nValue;
		break;

	case OPTION_RANGE_DEFENSE_UP:
		apet->m_opDRange += nValue;
		break;

	case OPTION_MELEE_AVOID_UP:
		apet->m_opMeleeAvoid += nValue;
		break;

	case OPTION_RANGE_AVOID_UP:
		apet->m_opRangeAvoid += nValue;
		break;

	case OPTION_MAGIC_UP:
		apet->m_opMagic += nValue;
		break;

	case OPTION_MAGIC_HIT_UP:
		apet->m_opMagicHitRate += nValue;
		break;

	case OPTION_RESIST_UP:
		apet->m_opResist += nValue;
		break;

	case OPTION_RESIST_AVOID_UP:
		apet->m_opResistAvoid += nValue;
		break;

	case OPTION_ALL_DAMAGE_UP:
		apet->m_opMelee += nValue;
		apet->m_opRange += nValue;
		apet->m_opMagic += nValue;
		break;

	case OPTION_ALL_HIT_UP:
		apet->m_opMeleeHitRate += nValue;
		apet->m_opRangeHitRate += nValue;
		apet->m_opMagicHitRate += nValue;
		break;

	case OPTION_ALL_DEFENSE_UP:
		apet->m_opDMelee += nValue;
		apet->m_opDRange += nValue;
		apet->m_opResist += nValue;
		break;

	case OPTION_ALL_AVOID_UP:
		apet->m_opMeleeAvoid += nValue;
		apet->m_opRangeAvoid += nValue;
		apet->m_opResistAvoid += nValue;
		break;

	case OPTION_RECOVER_HP:
		apet->m_recoverHP += nValue;
		break;

	case OPTION_RECOVER_MP:
		apet->m_recoverMP += nValue;
		break;

	case OPTION_MOVESPEED:
		//apet->m_runSpeed += apet->m_runSpeed * nValue / 1000;
		apet->SetRunSpeed(apet->GetRunSpeed() + apet->GetRunSpeed() * nValue / 1000);
		break;
	case OPTION_APET_TRANS_END:
		{
			if( nValue == 0 )	// 강제로 변신 해제
			{
				apet->m_cForceTrans = 2;
			}
			else if ( nValue > 99 ) // 강제로 변신
			{
				apet->m_cForceTrans = 1;
			}
			else
				apet->m_optTransEnd += nValue;
		}
		break;
	}
}

void COption::ApplyOptionValue(CPC* pc, CItem* item)
{
	COption::ApplyOptionValue(pc, m_type, m_value, item);
}

void COption::ApplyOptionValue(CPC* pc, int nType, int nValue, CItem* pItem, int nPosition)
{
	int idx;

	// 전직 이전
	switch (pc->m_job)
	{
	case JOB_TITAN:
	case JOB_KNIGHT:
	case JOB_HEALER:
	case JOB_MAGE:
	case JOB_ROGUE:
	case JOB_SORCERER:
	case JOB_NIGHTSHADOW:
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
#endif // EX_ROGUE
#ifdef EX_MAGE
	case JOB_EX_MAGE:
#endif // EX_MAGE
		idx = pc->m_job;
		break;

	default:
		return;
	}

	if (nType < 0)
		return;
	if (nPosition > -1)
	{
		nValue = nValue * pItem->getOriginVar(nPosition) / 1000;
	}
	switch(nType)
	{
	case OPTION_STR_UP:
		pc->m_opStr += nValue;
		pc->m_str += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][0];
		break;

	case OPTION_DEX_UP:
		pc->m_opDex += nValue;
		pc->m_dex += nValue;
		break;

	case OPTION_INT_UP:
		pc->m_opInt += nValue;
		pc->m_int += nValue;
		pc->m_maxMP += nValue * levelup_mp[idx][2];
		break;

	case OPTION_CON_UP:
		pc->m_opCon += nValue;
		pc->m_con += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][3];
		pc->m_maxMP += nValue * levelup_mp[idx][3];
		break;

	case OPTION_HP_UP:
		pc->m_maxHP += nValue;
		break;

	case OPTION_MP_UP:
		pc->m_maxMP += nValue;
		break;

	case OPTION_DAMAGE_UP:
		pc->m_opMelee += nValue;
		pc->m_opRange += nValue;
		break;

	case OPTION_MELEE_DAMAGE_UP:
		pc->m_opMelee += nValue;
		break;

	case OPTION_RANGE_DAMAGE_UP:
		pc->m_opRange += nValue;
		break;

	case OPTION_MELEE_HIT_UP:
		pc->m_opMeleeHitRate += nValue;
		break;

	case OPTION_RANGE_HIT_UP:
		pc->m_opRangeHitRate += nValue;
		break;

	case OPTION_DEFENSE_UP:
		pc->m_opDMelee += nValue;
		pc->m_opDRange += nValue;
		break;

	case OPTION_MELEE_DEFENSE_UP:
		pc->m_opDMelee += nValue;
		break;

	case OPTION_RANGE_DEFENSE_UP:
		pc->m_opDRange += nValue;
		break;

	case OPTION_MELEE_AVOID_UP:
		pc->m_opMeleeAvoid += nValue;
		break;

	case OPTION_RANGE_AVOID_UP:
		pc->m_opRangeAvoid += nValue;
		break;

	case OPTION_MAGIC_UP:
		pc->m_opMagic += nValue;
		break;

	case OPTION_MAGIC_HIT_UP:
		pc->m_opMagicHitRate += nValue;
		break;

	case OPTION_RESIST_UP:
		pc->m_opResist += nValue;
		break;

	case OPTION_RESIST_AVOID_UP:
		pc->m_opResistAvoid += nValue;
		break;

	case OPTION_ALL_DAMAGE_UP:
		pc->m_opMelee += nValue;
		pc->m_opRange += nValue;
		pc->m_opMagic += nValue;
		break;

	case OPTION_ALL_HIT_UP:
		pc->m_opMeleeHitRate += nValue;
		pc->m_opRangeHitRate += nValue;
		pc->m_opMagicHitRate += nValue;
		break;

	case OPTION_ALL_DEFENSE_UP:
		pc->m_opDMelee += nValue;
		pc->m_opDRange += nValue;
		pc->m_opResist += nValue;
		break;

	case OPTION_ALL_AVOID_UP:
		pc->m_opMeleeAvoid += nValue;
		pc->m_opRangeAvoid += nValue;
		pc->m_opResistAvoid += nValue;
		break;

	case OPTION_INCREASE_INVEN:
		pc->m_opRecoverHP_NoRate += nValue;
		break;

	case OPTION_STEAL_MP:
		pc->m_opMPSteal += nValue;
		break;

	case OPTION_STEAL_HP:
		pc->m_opHPSteal += nValue;
		break;

	case OPTION_ATTACK_BLIND:
		if (pc->m_opAttackBlind < nValue)
			pc->m_opAttackBlind = nValue;
		break;

	case OPTION_ATTACK_POISON:
		if (pc->m_opAttackPoison < nValue)
			pc->m_opAttackPoison = nValue;
		break;

	case OPTION_ATTACK_CRITICAL:
		pc->m_opAttackCritical += nValue;
		break;

	case OPTION_RECOVER_HP:
		pc->m_opRecoverHP += nValue;
		break;

	case OPTION_RECOVER_MP:
		pc->m_opRecoverMP += nValue;
		break;

	case OPTION_DECREASE_SKILL_DELAY:
		pc->m_opDecreaseSkillDelay += nValue;
		break;

	case OPTION_DECREASE_SKILL_MP:
		pc->m_opDecreaseSkillMP += nValue;
		break;

	case OPTION_RESIST_STONE:
		pc->m_opResistStone += nValue;
		if(pc->m_opResistStone > 100)
			pc->m_opResistStone = 100;
		break;

	case OPTION_RESIST_STURN:
		pc->m_opResistSturn += nValue;
		if(pc->m_opResistSturn > 100)
			pc->m_opResistSturn = 100;
		break;

	case OPTION_RESIST_SILENT:
		pc->m_opResistSilent += nValue;
		if(pc->m_opResistSilent > 100)
			pc->m_opResistSilent = 100;
		break;

	case OPTION_BLOCKING:
		pc->m_opBlocking += nValue;
		break;

	case OPTION_MOVESPEED:
		//pc->m_runSpeed += pc->m_runSpeed * nValue / 100;
		pc->SetRunSpeed(pc->GetRunSpeed() + pc->GetRunSpeed() * nValue / 100);
		break;

	case OPTION_FLYSPEED :
		pc->m_flySpeed += (float)nValue / 10.0f;		//이동속도는 10단위
		break;

	case OPTION_ATTACK_DEADLY :
		pc->m_opAttackDeadly += nValue;
		break;
	case OPTION_STR_UP_RATE:
		nValue = pc->m_dbStr * nValue / 100;
		pc->m_opStr += nValue;
		pc->m_str += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][0];
		break;
	case OPTION_DEX_UP_RATE:
		nValue = pc->m_dbDex * nValue / 100;
		pc->m_opDex += nValue;
		pc->m_dex += nValue;
		break;
	case OPTION_INT_UP_RATE:
		nValue = pc->m_dbInt * nValue / 100;
		pc->m_opInt += nValue;
		pc->m_int += nValue;
		pc->m_maxMP += nValue * levelup_mp[idx][2];
		break;
	case OPTION_CON_UP_RATE:
		nValue = pc->m_dbCon * nValue / 100;
		pc->m_opCon += nValue;
		pc->m_con += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][3];
		pc->m_maxMP += nValue * levelup_mp[idx][3];
		break;
	case OPTION_HP_UP_RATE:
		nValue = pc->m_dbHP * nValue / 100;
		pc->m_maxHP += nValue;
		break;
	case OPTION_MP_UP_RATE:
		nValue = pc->m_dbMP * nValue / 100;
		pc->m_maxMP += nValue;
		break;
	case OPTION_WEAPON_UP_RATE:
		if(pc->m_wearInventory.wearItemInfo[WEARING_WEAPON])
		{
			CItem* item = pc->m_wearInventory.wearItemInfo[WEARING_WEAPON];
			if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (item->getFlag() & FLAG_ITEM_COMPOSITION) )
			{
				CItem* equipitem = pc->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
				if(equipitem)
					item = equipitem;
			}

			if(item->IsRangeWeapon())
			{
				pc->m_opRange += item->m_itemProto->getItemNum0() * nValue / 100;
			}
			else
			{
				pc->m_opMelee += item->m_itemProto->getItemNum0() * nValue / 100;
			}

			pc->m_opMagic += item->m_itemProto->getItemNum1() * nValue / 100;
		}
		break;
	case OPTION_ARMOR_UP_RATE:
		{
			const int amor_count = 7;
			static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS, WEARING_BACKWING };

			int i;
			CItem *item;
			int deffence = 0, deffence2 = 0;
			for (i = 0; i < amor_count; i++)
			{
				item = pc->m_wearInventory.wearItemInfo[armors[i]];
				if (item && item->IsArmorType())
				{
					if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (item->getFlag() & FLAG_ITEM_COMPOSITION) )
					{
						CItem* equipitem = pc->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
						if(equipitem)
							item = equipitem;
					}

					deffence += item->m_itemProto->getItemNum0();
					deffence2 += item->m_itemProto->getItemNum1();
				}
			}

			pc->m_opDMelee += deffence * nValue / 100;
			pc->m_opDRange += deffence * nValue / 100;
			pc->m_opResist += deffence2 * nValue / 100;
		}
		break;
	case OPTION_MELEE_HIT_UP_RATE:
		nValue = (int)pc->GetHitrate(pc, MSG_DAMAGE_MELEE) * nValue / 100;
		pc->m_opMeleeHitRate += nValue;
		pc->m_opRangeHitRate += nValue;
		break;
	case OPTION_MAGIC_HIT_UP_RATE:
		nValue = (int)pc->GetHitrate(pc, MSG_DAMAGE_MAGIC) * nValue / 100;
		pc->m_opMagicHitRate += nValue;
		break;
	case OPTION_MELEE_AVOID_RATE:
		nValue = (int)pc->GetAvoid(pc, MSG_DAMAGE_MELEE) * nValue / 100;
		pc->m_opMeleeAvoid += nValue;
		pc->m_opRangeAvoid += nValue;
		break;
	case OPTION_MAGIC_AVOID_RATE:
		nValue = (int)pc->GetAvoid(pc, MSG_DAMAGE_MAGIC) * nValue / 100;
		pc->m_opResistAvoid += nValue;
		break;
	case OPTION_RECOVER_HP_RATE:
		nValue = pc->m_recoverHP * nValue / 100;
		pc->m_opRecoverHP += nValue;
		break;
	case OPTION_RECOVER_MP_RATE:
		nValue = pc->m_recoverMP * nValue / 100;
		pc->m_opRecoverMP += nValue;
		break;
	case OPTION_EXP_UP_RATE:
		pc->m_opExpRate += nValue;
		break;
	case OPTION_SP_UP_RATE:
		pc->m_opSPRate += nValue;
		break;
	case OPTION_APET_ELEMENT_HPUP:
		pc->m_opJewelElementPetHPUP += nValue;
		break;
	case OPTION_APET_ELEMENT_ATTUP:
		if(pc->GetAPet())
		{
			pc->GetAPet()->m_nJewelAttack += nValue;
			pc->GetAPet()->m_nJewelMagicAttack += nValue;
		}
		pc->m_opJewelElementAttUP += nValue;
		pc->m_opJewelElementMagicAttUP += nValue;
		break;
	case OPTION_ATT_FIRE_UP:
		pc->m_opAttratt = AT_MIX(1, nValue);
		break;
	case OPTION_ATT_WATER_UP:
		pc->m_opAttratt = AT_MIX(2, nValue);
		break;
	case OPTION_ATT_WIND_UP:
		pc->m_opAttratt = AT_MIX(4, nValue);
		break;
	case OPTION_ATT_EARTH_UP:
		pc->m_opAttratt = AT_MIX(3, nValue);
		break;
	case OPTION_ATT_DARK_UP:
		pc->m_opAttratt = AT_MIX(5, nValue);
		break;
	case OPTION_ATT_LIGHT_UP:
		pc->m_opAttratt = AT_MIX(6, nValue);
		break;

	case OPTION_DEF_FIRE_UP:
		pc->m_opAttrdef = AT_MIX(1, nValue);
		break;
	case OPTION_DEF_WATER_UP:
		pc->m_opAttrdef = AT_MIX(2, nValue);
		break;
	case OPTION_DEF_WIND_UP:
		pc->m_opAttrdef = AT_MIX(4, nValue);
		break;
	case OPTION_DEF_EARTH_UP:
		pc->m_opAttrdef = AT_MIX(3, nValue);
		break;
	case OPTION_DEF_DARK_UP:
		pc->m_opAttrdef = AT_MIX(5, nValue);
		break;
	case OPTION_DEF_LIGHT_UP:
		pc->m_opAttrdef = AT_MIX(6, nValue);
		break;

	case OPTION_ALL_STAT_UP:
		pc->m_opStr += nValue;
		pc->m_str += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][0];

		pc->m_opDex += nValue;
		pc->m_dex += nValue;

		pc->m_opInt += nValue;
		pc->m_int += nValue;
		pc->m_maxMP += nValue * levelup_mp[idx][2];

		pc->m_opCon += nValue;
		pc->m_con += nValue;
		pc->m_maxHP += nValue * levelup_hp[idx][3];
		pc->m_maxMP += nValue * levelup_mp[idx][3];
		break;
	case OPTION_PVP_DAMAGE_ABSOLB:
		pc->m_absorbPVPDamageRate += nValue;
		break;
	case OPTION_DEBUF_DECR_TIME:
		pc->m_decreaseDBufTimeRate += nValue;
		break;
	case OPTION_RECOVER_HP_NOTRATE:
		pc->m_opRecoverHP_NoRate += nValue;
		break;
	case OPTION_RECOVER_MP_NOTRATE:
		pc->m_opRecoverMP_NoRate += nValue;
		break;
	case OPTION_INCREASE_STRONG:
		pc->m_opStrong += nValue;
		break;
	case OPTION_INCREASE_HARD:
		pc->m_plusHard += nValue;
		break;
	case OPTION_INCREASE_HP:
		pc->m_maxHP += nValue;
		break;
	}

	// 공성 지역 안에서만
	if (pc->GetMapAttr() & MATT_WAR)
	{
		// 공성 진행 중에 공성 참여자만
		CWarCastle* castle = CWarCastle::GetCastleObject(pc->m_pZone->m_index);
		if (castle && castle->GetState() != WCSF_NORMAL && pc->GetJoinFlag(pc->m_pZone->m_index) != WCJF_NONE)
		{
			switch(nType)
			{
			case OPTION_MIX_STR:
				pc->m_opStr += nValue;
				pc->m_str += nValue;
				pc->m_maxHP += nValue * levelup_hp[idx][0];
				break;

			case OPTION_MIX_DEX:
				pc->m_opDex += nValue;
				pc->m_dex += nValue;
				break;

			case OPTION_MIX_INT:
				pc->m_opInt += nValue;
				pc->m_int += nValue;
				pc->m_maxMP += nValue * levelup_mp[idx][2];
				break;

			case OPTION_MIX_CON:
				pc->m_opCon += nValue;
				pc->m_con += nValue;
				pc->m_maxHP += nValue * levelup_hp[idx][3];
				pc->m_maxMP += nValue * levelup_mp[idx][3];
				break;

			case OPTION_MIX_ATTACK:
				pc->m_opMelee += nValue;
				pc->m_opRange += nValue;
				break;

			case OPTION_MIX_DEFENSE:
				pc->m_opDMelee += nValue;
				pc->m_opDRange += nValue;
				break;

			case OPTION_MIX_MAGIC:
				pc->m_opMagic += nValue;
				break;

			case OPTION_MIX_RESIST:
				pc->m_opResist += nValue;
				break;

			case OPTION_MIX_STURN:
				if (pItem)
				{
					pc->m_opSturnLevel++;
					pc->m_opSturnIndex = pItem->m_itemProto->getItemNum0();
				}
				break;

			case OPTION_MIX_BLOOD:
				if (pItem)
				{
					pc->m_opBloodLevel++;
					pc->m_opBloodIndex = pItem->m_itemProto->getItemNum0();
				}
				break;

			case OPTION_MIX_MOVE:
				if (pItem)
				{
					pc->m_opMoveLevel++;
					pc->m_opMoveIndex = pItem->m_itemProto->getItemNum0();
				}
				break;

			case OPTION_MIX_POISON:
				if (pItem)
				{
					pc->m_opPoisonLevel++;
					pc->m_opPoisonIndex = pItem->m_itemProto->getItemNum0();
				}
				break;

			case OPTION_MIX_SLOW:
				if (pItem)
				{
					pc->m_opSlowLevel++;
					pc->m_opSlowIndex = pItem->m_itemProto->getItemNum0();
				}
				break;
			}
		}
	}
//#endif
}

void COption::GetDBValue(short dbValue)
{
	m_dbValue = dbValue;

	m_type = (dbValue >> OPTION_VALUE_SHIFT);
	m_level = dbValue & 0x00ff;

	if (m_level <= 0)
		m_type = -1;
}

void COption::SetDBValue()
{
	m_dbValue = 0;

	m_dbValue |= (m_type << OPTION_VALUE_SHIFT) & 0xff00;
	m_dbValue |= m_level & 0x00ff;
}

short COption::SetDBValue( int type, int level )
{
	int retValue = 0;

	retValue |= (type << OPTION_VALUE_SHIFT) & 0xff00;
	retValue |= level & 0x00ff;

	return retValue;
}

//