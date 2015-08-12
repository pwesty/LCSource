#include <map>
#include <boost/assign/list_of.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "CmdMsg.h"
#include "Character.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_old_do_changejob.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_old_do_sskill.h"
//////////////////////////////////////////////////////////////////////////
// 스텟 초기화
bool local_initStat(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	ch->m_statpt_remain += ch->m_statpt_str + ch->m_statpt_dex + ch->m_statpt_int + ch->m_statpt_con;

	ch->m_statpt_str = ch->m_statpt_dex = ch->m_statpt_int = ch->m_statpt_con = 0;
	ch->InitStat();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);
	ch->m_hp = ch->m_maxHP;
	ch->m_mp = ch->m_maxMP;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool local_apetStatInit(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	CAPet* apet = ch->GetAPet();

	if (apet == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_STATINIT, NULL, 1);
		SEND_Q( rmsg , ch->m_desc );

		return false;
	}

	CItem *pPetItem = ch->m_wearInventory.wearItemInfo[ WEARING_PET ];
	if( !pPetItem )		// 착용중인 펫 아이템 없음
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_STATINIT, NULL, 1);
		SEND_Q( rmsg , ch->m_desc );
		return false;
	}

	if( pPetItem->getFlag() & FLAG_ITEM_SEALED || DEAD( apet ) )	// 봉인또는 죽었음
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_STATINIT, NULL, 2);
		SEND_Q( rmsg , ch->m_desc );
		return false;
	}

	if( !( apet->m_nPlusStr || apet->m_nPlusCon || apet->m_nPlusDex || apet->m_nPlusInt ) )		// 초기화할 스텟 없음
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_STATINIT, NULL, 3);
		SEND_Q( rmsg , ch->m_desc );
		return false;
	}

	// 이전 스텟 로그
	GAMELOG << init("APET_STAT_INIT BEFORE",ch ) << "REMAIN:" << apet->m_nRemainStat << delim
			<< "STR:" << apet->m_nPlusStr << delim << "DEX:" << apet->m_nPlusDex << delim
			<< "INT:" << apet->m_nPlusInt << delim << "CON:" << apet->m_nPlusCon <<  end;

	apet->InitStat();

	// 사용후 스텟 로그
	GAMELOG << init("APET_STAT_INIT AFTER",ch ) << "REMAIN:" << apet->m_nRemainStat << delim
			<< "STR:" << apet->m_nPlusStr << delim << "DEX:" << apet->m_nPlusDex << delim
			<< "INT:" << apet->m_nPlusInt << delim << "CON:" << apet->m_nPlusCon << end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetFuntionMsg( rmsg, MSG_SUB_STATINIT, NULL, 0);
		SEND_Q( rmsg , ch->m_desc );
	}

	return true;
}

bool local_initSkill(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	ch->InitSkill(&ch->m_activeSkillList);
	ch->InitSkill(&ch->m_passiveSkillList, true);
	if( ch->m_job != JOB_NIGHTSHADOW )
		ch->m_job2 = 0;

	ch->InitStat();

	ch->CalcStatus(true);

	ch->resetCoolTime(&ch->m_activeSkillList);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobReset(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool local_initSpecialSkill(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	ch->InitSSkill();
	ch->CalcStatus(true);
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::makeSSkillList(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool local_initNormalAndSpecialSkill(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	if(ch->m_evocationIndex > EVOCATION_NONE)
		ch->ChangeEvocationState(ch->m_evocationIndex);

	ch->InitSkill(&ch->m_activeSkillList);
	ch->InitSkill(&ch->m_passiveSkillList, true);
	ch->InitSSkill();
	if( ch->m_job != JOB_NIGHTSHADOW )
		ch->m_job2 = 0;
	ch->InitStat();

	ch->CalcStatus(true);
	
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if (ch->m_quickSlot[i].m_slotType[j] == QUICKSLOT_TYPE_SKILL)
			{
				ch->m_quickSlot[i].m_skillType[j] = -1;
				ch->m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;
			}
		}
	}

	ch->resetCoolTime(&ch->m_activeSkillList);
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobReset(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
	return true;
}

bool local_initAllStatAndSkill(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	ch->m_statpt_remain += ch->m_statpt_str + ch->m_statpt_dex + ch->m_statpt_int + ch->m_statpt_con;

	ch->m_statpt_str = ch->m_statpt_dex = ch->m_statpt_int = ch->m_statpt_con = 0;
	ch->InitStat();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);
	ch->m_hp = ch->m_maxHP;
	ch->m_mp = ch->m_maxMP;

	ch->InitSkill(&ch->m_activeSkillList);
	ch->InitSkill(&ch->m_passiveSkillList, true);
	ch->InitSSkill();
	if( ch->m_job != JOB_NIGHTSHADOW )
		ch->m_job2 = 0;
	ch->InitStat();

	ch->CalcStatus(true);

	ch->resetCoolTime(&ch->m_activeSkillList);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobReset(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

// bprolong은 연기신청 창을 띄울것인가로 바꿈
// return 값은 완료 기간
int usePeriodItem(int* useTime, bool* bprolong)
{
	bool reqProlong = false;

	if(*bprolong)
	{
		if( *useTime >= gserver->getNowSecond() )
		{
			*useTime += TIME_ONE_MONTH;
		}
		else
		{
			*useTime = gserver->getNowSecond() + TIME_ONE_MONTH;
		}
	}
	else
	{
		if( *useTime <= gserver->getNowSecond() )
		{
			*useTime = gserver->getNowSecond() + TIME_ONE_MONTH;
		}
		else
		{
			reqProlong = true;
		}
	}

	*bprolong = reqProlong;
	if( *useTime - gserver->getNowSecond() <= 0)
	{
		*useTime = 0;
	}
	return *useTime;
}
int usePeriodItem_7(int* useTime, bool* bprolong)
{
	bool reqProlong = false;

	if(*bprolong)
	{
		if( *useTime >= gserver->getNowSecond() )
		{
			*useTime += TIME_ONE_WEEK;
		}
		else
		{
			*useTime = gserver->getNowSecond() + TIME_ONE_WEEK;
		}
	}
	else
	{
		if( *useTime <= gserver->getNowSecond() )
		{
			*useTime = gserver->getNowSecond() + TIME_ONE_WEEK;
		}
		else
		{
			reqProlong = true;
		}
	}

	*bprolong = reqProlong;
	if (*useTime - gserver->getNowSecond() <= 0)
	{
		*useTime = 0;
	}

	return *useTime;
}

bool local_period_840(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int itemUseTime = usePeriodItem(&ch->m_memposTime, &bprolong);

	CNetMsg::SP rmsg(new CNetMsg);
	ItemProlongMsg(rmsg, item->m_itemProto->getItemIndex(), itemUseTime, bprolong);
	SEND_Q(rmsg, ch->m_desc);

	return !bprolong;
}

bool local_period_841(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int itemUseTime = usePeriodItem(&ch->m_stashextTime, &bprolong);

	CNetMsg::SP rmsg(new CNetMsg);
	ItemProlongMsg(rmsg, item->m_itemProto->getItemIndex(), itemUseTime, bprolong);
	SEND_Q(rmsg, ch->m_desc);

	return !bprolong;
}

bool local_period_2347(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int itemUseTime = usePeriodItem_7( &ch->m_stashextTime, &bprolong );

	CNetMsg::SP rmsg(new CNetMsg);
	ItemProlongMsg(rmsg, item->m_itemProto->getItemIndex(), itemUseTime, bprolong);
	SEND_Q(rmsg, ch->m_desc);

	return !bprolong;
}

bool local_period_2348(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int itemUseTime = usePeriodItem_7(&ch->m_memposTime, &bprolong );

	CNetMsg::SP rmsg(new CNetMsg);
	ItemProlongMsg(rmsg, item->m_itemProto->getItemIndex(), itemUseTime, bprolong);
	SEND_Q(rmsg, ch->m_desc);

	return !bprolong;
}

bool local_exchangeWeapon(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int extra1 = packet->extra_1;
	int extra2 = packet->extra_2;

	CItem* vitem = ch->m_inventory.FindByVirtualIndex(extra1);
	if (!vitem)
		return false;

	if (!vitem->IsWeaponType())
		return false;

	if (vitem->m_itemProto->getItemSubTypeIdx() == extra2)
		return false;

	if (extra2 < 0 || extra2 >= 16)
		return false;

	int itemlevel = vitem->GetItemLevel();

	// 레벨, 한손검, 석궁, 스태프, 대검, 도끼, 숏스테프, 활, 단검, -1, -1, -1, 이도류, 완드, 사이드, 폴암 : 할일 : 소서러
	const static int matchTable[12][16] =
	{
		{1,48,530,600,12,558,356,50,528,-1,-1,-1,459,599,-1,-1},
		{5,53,666,628,51,601,357,56,529,-1,-1,-1,610,619,-1,-1},
		{9,55,637,629,52,602,358,57,532,-1,-1,-1,611,620,-1,-1},
		{13,107,638,630,105,603,359,106,533,-1,-1,-1,612,621,-1,-1},
		{17,180,639,631,306,604,360,185,534,-1,-1,-1,613,622,-1,-1},
		{21,322,640,632,307,605,361,341,535,-1,-1,-1,614,623,-1,-1},
		{25,323,641,633,308,606,362,342,536,-1,-1,-1,615,624,-1,-1},
		{29,324,642,634,309,607,363,343,537,-1,-1,-1,616,625,983,995},
		{33,325,643,635,310,608,364,344,538,-1,-1,-1,617,626,984,-996},
		{37,326,644,636,311,609,365,345,539,-1,-1,-1,618,627,985,997},
		{41,681,721,719,715,645,720,718,722,-1,-1,-1,716,717,986,998},
		{45,800,823,816,793,794,815,808,822,-1,-1,-1,801,809,987,999}
	};

	CItemProto* proto = NULL;
	int i;
	for (i = 0; i < 12; i++)
	{
		if (matchTable[i][0] == itemlevel)
		{
			proto = gserver->m_itemProtoList.FindIndex(matchTable[i][extra2 + 1]);
			break;
		}
	}

	if (proto == NULL)
		return false;

	CItem* newitem = gserver->m_itemProtoList.CreateItem(proto->getItemIndex(), -1, vitem->getPlus(), vitem->getFlag(), 1);
	if (!newitem)
		return false;

	GAMELOG << init("CASHITEM_CHANGE_WEAPON_WITHOUT OPTION", ch)
			<< "OLD" << delim
			<< itemlog(vitem) << delim;

	GAMELOG << "NEW" << delim
			<< itemlog(newitem) << delim;

	ch->m_inventory.decreaseItemCount(vitem, 1);

	if (ch->m_inventory.addItem(newitem) == false)
	{
		ch->m_pArea->DropItem(newitem, ch);
		newitem->m_preferenceIndex = ch->m_index;

		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, newitem);
		ch->m_pArea->SendToCell(rmsg, GET_YLAYER(newitem), newitem->m_cellX, newitem->m_cellZ);
		GAMELOG << "DROP"
				<< end;
	}

	return true;
}

bool local_platinumSmelt(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	int giveItemIndex=0;
	if( item->m_itemProto->getItemIndex() == 1418 )
		giveItemIndex=1419;
	else if( item->m_itemProto->getItemIndex() == 2894 )
		giveItemIndex=2895;
	else if( item->m_itemProto->getItemIndex() == 3269 )
		giveItemIndex=3270;

	// 인벤토리에 최소 무게가 보장되어야 한다.
	const CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(giveItemIndex);
	if (pItemProto == NULL)
		return false;

	// 플러스를 붙인 플래티늄 제련석을 지급한다
	int rand = GetRandom(1, 100);

	// 5프로 +3, 15프로 +2, 나머지 +1
	//+1 60%, +2 25%, +3 15% 로 변경 /bw : 060613
	if( rand <= 15 )
	{
		rand = 3;
	}
	else if( rand <= 40 )
	{
		rand = 2;
	}
	else
	{
		rand = 1;
	}

	if( giveItemIndex == 3270 )
		rand = 3;			// +3 전용 제련 아이템

	if( !ch->GiveItem(giveItemIndex, rand, 0, 1) )
	{
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

// 미감정 결합 아이템 감정
bool local_useCashItem_1420(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	int nProb = GetRandom(1, 100);
	const int nNewItemIndex = 1421;
	int nNewItemPlus = 0;
	if (nProb <= 80)					nNewItemPlus = 1;
	else if (nProb <= 80 + 15)			nNewItemPlus = 2;
	else if (nProb <= 80 + 15 + 5)		nNewItemPlus = 3;
	if (nNewItemPlus == 0)
		return false;

	CItem* pNewItem = gserver->m_itemProtoList.CreateItem(nNewItemIndex, -1, nNewItemPlus, 0, 1);
	if (!pNewItem)
	{
		GAMELOG << init("USE CASHITEM 1420 ERROR CANT CREATE ITEM 1421", ch) << end;
		return false;
	}

	if (ch->m_inventory.addItem(pNewItem) == false)
	{
		GAMELOG << init("USE CASHITEM 1420 ERROR FULL INVENTORY", ch) << end;

		delete pNewItem;

		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	return true;
}

// 미감정 초보자용 결합 아이템 감정
bool local_useCashItem_2664(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	int nProb = GetRandom(1, 100);
	const int nNewItemIndex = 2665;
	int nNewItemPlus = 0;
	if (nProb <= 80)					nNewItemPlus = 1;
	else if (nProb <= 80 + 15)			nNewItemPlus = 2;
	else if (nProb <= 80 + 15 + 5)		nNewItemPlus = 3;
	if (nNewItemPlus == 0)
		return false;

	CItem* pNewItem = gserver->m_itemProtoList.CreateItem(nNewItemIndex, -1, nNewItemPlus, 0, 1);
	if (!pNewItem)
	{
		GAMELOG << init("USE CASHITEM 2664 ERROR CANT CREATE ITEM 2665", ch) << end;
		return false;
	}

	if (ch->m_inventory.addItem(pNewItem) == false)
	{
		GAMELOG << init("USE CASHITEM 2664 ERROR FULL INVENTORY", ch) << end;

		delete pNewItem;

		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	return true;
}

void GetSendErrorType(MSG_ITEM_USE_ERROR_TYPE& type, const int& result)
{
	switch(result)
	{
	case 15:
		type = MSG_ITEM_USE_ERROR_DUP_COMP_PREFINE;
		break;
	default:
		type = MSG_ITEM_USE_ERROR_CANNOT_USE_COMP;
		break;
	}
}

//XXX 결합의상 처리루틴
int local_useCashItem_1421_Sub(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int extra1 = packet->extra_1;
	int extra2 = packet->extra_2;

	CItem* pItemClothes = ch->m_inventory.FindByVirtualIndex(extra1);
	CItem* pItemEquip   = ch->m_inventory.FindByVirtualIndex(extra2);

	if (pItemClothes == NULL || pItemEquip == NULL || pItemClothes == pItemEquip || pItemClothes == item || pItemEquip == item)
		return 1;

	if (pItemEquip->tab() >= INVENTORY_TAB_STASH)
		return 1;

	// pItemClothes 검사
	// 1. ITEM_FLAG_COMPOSITE		ON
	// 2. FLAG_ITEM_COMPOSITION		OFF
	// 3. USED						< NOW
	// 4. PLUS						NOT SET
	// 5. COMPOSITEITEM				NOT SET
	// 6. WEARPOS					OFF
	if (!(pItemClothes->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
		return 2;
	if (pItemClothes->getFlag() & FLAG_ITEM_COMPOSITION)
		return 3;
	if (pItemClothes->getUsed() < gserver->getNowSecond())
	{
		// pItemClothes->m_used == -1 일 경우 기간 무제한 yhj
		// 피닉스 이벤트 아이템이 아닐경우
		if( pItemClothes->getUsed() == -1 )
		{
			/* 무제한 이므로 넘어감*/
		}
		else
		{
			return 4;
		}
	}
	if (pItemClothes->getPlus() != 0)
		return 5;
	if (pItemClothes->m_nCompositeItem >= 0)
		return 6;
	if (pItemClothes->getWearPos() != WEARING_NONE)
		return 7;

	// pItemEquip 검사
	// 1. FLAG_ITEM_COMPOSITION		OFF
	// 2. TYPE						== pItemClothes.TYPE
	// 3. SUBTYPE					== pItemClothes.SUBTYPE
	// 4. WEARPOS					OFF
	// 5. UPGRADE FLAG				ON
	// 6. ITEM_FLAG_COMPOSITE		OFF
	// 7. JOB						== pItemClothes.JOB
	// 8. PLATINUM					OFF
	if (pItemEquip->getFlag() & FLAG_ITEM_COMPOSITION)
		return 8;
	if (pItemEquip->m_itemProto->getItemTypeIdx() != pItemClothes->m_itemProto->getItemTypeIdx())
		return 9;
	if (pItemEquip->m_itemProto->getItemSubTypeIdx() != pItemClothes->m_itemProto->getItemSubTypeIdx())
		return 10;
	if (pItemEquip->getWearPos() != WEARING_NONE)
		return 11;
	if (!pItemEquip->CanCompositeEquipItem())
		return 12;
	if (pItemEquip->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
		return 13;
	if (pItemEquip->m_itemProto->getItemJobFlag() != pItemClothes->m_itemProto->getItemJobFlag())
		return 14;
	if (pItemEquip->IsUsedPlatinumSpecial())
		return 15;

	// 처리
	// 1. FLAG						FLAG_ITEM_COMPOSITION
	// 2. COMPOSITEITEM				pItemEquip.INDEX
	// 3. pItemClothes.PLUS			item.PLUS + pItemEquip.PLUS
	// 4. PLATINUM SET				FLAG_ITEM_PLATINUM_SET

	pItemClothes->setFlag(pItemClothes->getFlag() | FLAG_ITEM_COMPOSITION);
	pItemEquip->setFlag(pItemEquip->getFlag() | FLAG_ITEM_COMPOSITION);
	pItemClothes->m_nCompositeItem = pItemEquip->getVIndex();
	pItemClothes->setPlus(item->getPlus() + pItemEquip->getPlus());
	pItemClothes->setFlag(FLAG_ITEM_PLATINUM_SET(pItemClothes->getFlag(), pItemEquip->getPlus()));

	// 결합 주문서 만료 시간 저장
	if (pItemClothes->m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
	{
		// 시간제 아이템
		pItemClothes->setUsed_2(pItemClothes->getUsed());
	}
	else
	{
		// 기간제 아이템
		static const int one_month = 60*60*24*30;

		if (pItemClothes->getUsed() - gserver->getNowSecond() < one_month)
		{
			// 1개월 미만인 경우 결합시간은 유료 아이템 만료 시간과 같다
			pItemClothes->setUsed_2(pItemClothes->getUsed());
		}
		else
		{
			// 1개월 이상인 경우 결합시간은 1개월이다.
			pItemClothes->setUsed_2(gserver->getNowSecond() + one_month);
		}
	}

	// 로그 남김
	GAMELOG << init("ITEM COMPOSITION", ch)
			<< "COMPOSITION DOC" << delim
			<< itemlog(item, false) << delim
			<< "CLOTHES" << delim
			<< itemlog(pItemClothes) << delim
			<< "EQUIP" << delim
			<< itemlog(pItemEquip)
			<< end;

	// 결과 전달
	ch->m_inventory.sendOneItemInfo(pItemClothes);
	ch->m_inventory.sendOneItemInfo(pItemEquip);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_ITEMCOMPOSITED);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);

	return 0;
}

bool local_useCashItem_1421(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int result = local_useCashItem_1421_Sub(ch, item, bprolong, packet);

	if(result == 0) // 성공
	{
		return true;
	}
	else
	{
		MSG_ITEM_USE_ERROR_TYPE sendtype;
		GetSendErrorType(sendtype, result);
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, sendtype);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
}

// 감정된 결합 주문서 사용
int local_useCashItem_2665_Sub(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int extra1 = packet->extra_1;
	int extra2 = packet->extra_2;

	CItem* pItemClothes = ch->m_inventory.FindByVirtualIndex(extra1);
	CItem* pItemEquip   = ch->m_inventory.FindByVirtualIndex(extra2);

	if (pItemClothes == NULL || pItemEquip == NULL || pItemClothes == pItemEquip || pItemClothes == item || pItemEquip == item)
		return 1;

	// pItemClothes 검사
	// 1. ITEM_FLAG_COMPOSITE		ON
	// 2. FLAG_ITEM_COMPOSITION		OFF
	// 3. USED						< NOW
	// 4. PLUS						NOT SET
	// 5. COMPOSITEITEM				NOT SET
	// 6. WEARPOS					OFF
	if (!(pItemClothes->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
		return 2;
	if (pItemClothes->getFlag() & FLAG_ITEM_COMPOSITION)
		return 3;
	if (pItemClothes->getUsed() < gserver->getNowSecond())
	{
		// pItemClothes->m_used == -1 일 경우 기간 무제한 yhj
		// 피닉스 이벤트 아이템이 아닐경우
		if( pItemClothes->getUsed() == -1 )
		{
			/* 무제한 이므로 넘어감*/
		}
		else
		{
			return 4;
		}
	}
	if (pItemClothes->getPlus() != 0)
		return 5;
	if (pItemClothes->m_nCompositeItem >= 0)
		return 6;
	if (pItemClothes->getWearPos() != WEARING_NONE)
		return 7;

	// pItemEquip 검사
	// 1. FLAG_ITEM_COMPOSITION		OFF
	// 2. TYPE						== pItemClothes.TYPE
	// 3. SUBTYPE					== pItemClothes.SUBTYPE
	// 4. WEARPOS					OFF
	// 5. UPGRADE FLAG				ON
	// 6. ITEM_FLAG_COMPOSITE		OFF
	// 7. JOB						== pItemClothes.JOB
	// 8. PLATINUM					OFF
	if (pItemEquip->getFlag() & FLAG_ITEM_COMPOSITION)
		return 8;
	if (pItemEquip->m_itemProto->getItemTypeIdx() != pItemClothes->m_itemProto->getItemTypeIdx())
		return 9;
	if (pItemEquip->m_itemProto->getItemSubTypeIdx() != pItemClothes->m_itemProto->getItemSubTypeIdx())
		return 10;
	if (pItemEquip->getWearPos() != WEARING_NONE)
		return 11;
	if (!pItemEquip->CanCompositeEquipItem())
		return 12;
	if (pItemEquip->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
		return 13;
	if (pItemEquip->m_itemProto->getItemJobFlag() != pItemClothes->m_itemProto->getItemJobFlag())
		return 14;
	if (pItemEquip->IsUsedPlatinumSpecial())
		return 15;

	// 처리
	// 1. FLAG						FLAG_ITEM_COMPOSITION
	// 2. COMPOSITEITEM				pItemEquip.INDEX
	// 3. pItemClothes.PLUS			item.PLUS + pItemEquip.PLUS
	// 4. PLATINUM SET				FLAG_ITEM_PLATINUM_SET

	pItemClothes->setFlag(pItemClothes->getFlag() | FLAG_ITEM_COMPOSITION);
	pItemEquip->setFlag(pItemEquip->getFlag() | FLAG_ITEM_COMPOSITION);
	pItemClothes->m_nCompositeItem = pItemEquip->getVIndex();
	pItemClothes->setPlus(item->getPlus() + pItemEquip->getPlus());
	pItemClothes->setFlag(FLAG_ITEM_PLATINUM_SET(pItemClothes->getFlag(), pItemEquip->getPlus()));

	// 결합 주문서 만료 시간 저장
	if (pItemClothes->m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
	{
		// 시간제 아이템
		pItemClothes->setUsed_2(pItemClothes->getUsed());
	}
	else
	{
		// 기간제 아이템
		static const int one_month = 60*60*24*30;

		if (pItemClothes->getUsed() - gserver->getNowSecond() < one_month)
		{
			// 1개월 미만인 경우 결합시간은 유료 아이템 만료 시간과 같다
			pItemClothes->setUsed_2(pItemClothes->getUsed());
		}
		else
		{
			// 1개월 이상인 경우 결합시간은 1개월이다.
			pItemClothes->setUsed_2(gserver->getNowSecond() + one_month);
		}
	}
	// 로그 남김
	GAMELOG << init("ITEM COMPOSITION", ch)
			<< "COMPOSITION DOC" << delim
			<< itemlog(item, false) << delim
			<< "CLOTHES" << delim
			<< itemlog(pItemClothes) << delim
			<< "EQUIP" << delim
			<< itemlog(pItemEquip)
			<< end;

	// 결과 전달
	ch->m_inventory.sendOneItemInfo(pItemClothes);
	ch->m_inventory.sendOneItemInfo(pItemEquip);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_ITEMCOMPOSITED);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);

	return 0;
}

bool local_useCashItem_2665(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int result = local_useCashItem_2665_Sub(ch, item, bprolong, packet);

	if(result == 0) // 성공
	{
		return true;
	}
	else
	{
		MSG_ITEM_USE_ERROR_TYPE sendtype;
		GetSendErrorType(sendtype, result);
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, sendtype);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
}

bool local_useCashItem_1540(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int extra1 = packet->extra_1;

	CItem*		pItemClothes = NULL;
	CItem*		pItemEquip = NULL;

	// 1. nIndexClothes로 아이템 찾기
	// 2. 아이템 합성 여부 검사
	// 3. 합성된 아이템 찾기
	// 4. 분리 처리
	// 5. 게임 로그
	// 6. 결과 알림

	// 1. nIndexClothes로 아이템 찾기
	pItemClothes = ch->m_inventory.FindByVirtualIndex(extra1);
	if (pItemClothes == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_SEPARATE_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 관련 수정... m_used == -1 이면 무한인데 여기서 분리가 안되게 되어있음
	// 고로 m_used == -1 이면 건너 뛰게 수정
	// 2. 아이템 합성 여부 검사
	if (	   (pItemClothes->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
			   && ( pItemClothes->getUsed() == -1 || pItemClothes->getUsed() > gserver->getNowSecond() )
			   && pItemClothes->getFlag() & FLAG_ITEM_COMPOSITION
	   )
	{
		// 3. 합성된 아이템 찾기
		pItemEquip = ch->m_inventory.FindByVirtualIndex(pItemClothes->m_nCompositeItem);
		if (!pItemEquip)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_SEPARATE_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}

		// 4. 분리 처리
		pItemClothes->setFlag(pItemClothes->getFlag() & ~FLAG_ITEM_COMPOSITION);
		pItemEquip->setFlag(pItemEquip->getFlag() & ~FLAG_ITEM_COMPOSITION);
		pItemClothes->m_nCompositeItem = -1;
		pItemClothes->setPlus(0);
		pItemClothes->setFlag(FLAG_ITEM_PLATINUM_SET(pItemClothes->getFlag(), 0));

		pItemClothes->setUsed_2(-1);		// 결합 주문서 만료 시간 제거

		// 5. 게임 로그
		GAMELOG << init("ITEM COMPOSITE SEPARATE", ch)
				<< "CLOTHES" << delim
				<< itemlog(pItemClothes) << delim
				<< "EQUIP" << delim
				<< itemlog(pItemEquip)
				<< end;

		{
			// 6. 결과 알림
			CNetMsg::SP rmsg(new CNetMsg);
			SysSeparateItemMsg(rmsg, pItemClothes->getDBIndex());
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_inventory.sendOneItemInfo(pItemClothes);
		ch->m_inventory.sendOneItemInfo(pItemEquip);

		ch->CalcStatus(true);

		return true;
	}

	return false;
}

bool local_useCashItem_1519(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 1. nPetIndex 아이템 찾고
	// 2. 펫 착용 해제 상태인지 보고
	// 3. 해당 아이템의 펫을 찾고
	// 4. 펫의 타입과 스킬 검사하고
	// 5. 펫 마운트 취소후
	// 6. 펫 상태 알리고
	// 7. 결과 알림
	int extra1 = packet->extra_1;
	bool bFail = false;

	// 1. extra1으로 아이템 찾고
	CItem* pItemPet = ch->m_inventory.FindByVirtualIndex(extra1);
	if (!pItemPet)
		bFail = true;

	// 2. 펫 착용 해제 상태인지 보고
	else if (pItemPet->getWearPos() != WEARING_NONE)
		bFail = true;
	else
	{
		// 3. 해당 아이템의 펫을 찾고
		CPet* pPet = ch->GetPet(pItemPet->getPlus());
		if (!pPet)
			bFail = true;

		// 4. 펫의 타입과 스킬 검사하고
		else if (!pPet->IsMountType() || pPet->GetSkillList()->count() > 1)
			bFail = true;

		else
		{
			// TODO : petlog
			GAMELOG << init("PET RESET MOUNT", ch)
					<< "PET ITEM" << delim
					<< itemlog(pItemPet) << delim
					<< "PET" << delim
					<< pPet->GetPetTypeGrade() << delim
					<< "INDEX" << delim
					<< pPet->m_index << delim
					<< "LEVEL" << delim
					<< pPet->m_level
					<< end;

			pPet->SetPetColor( (char)NON_COLOR );

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPetColorChange( rmsg, ch->m_index, pPet->m_index, (char)NON_COLOR );
				SEND_Q( rmsg, gserver->m_helper );
			}

			// 5. 펫 마운트 취소후
			pPet->ResetMountType();

			{
				// 6. 펫 상태 알리고
				CNetMsg::SP rmsg(new CNetMsg);
				ExPetStatusMsg(rmsg, pPet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}

	// 7. 결과 알림
	if (bFail)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_RESET_MOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_RESET_MOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return true;
	}
}

// pet 색깔 변경하기
bool local_useCashItem_PetColorChange(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	int extra1 = packet->extra_1;
	CItemProto* itemproto = item->m_itemProto;

	if( ch->m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if( !pet )
			return false;
		char PetColorType = 0 ;

		// 말만 가능한 것들
		if( (itemproto->getItemIndex() == PET_BLACK_HEART_COLOR_ITEM) ||
				(itemproto->getItemIndex() == PET_WHITE_HEART_COLOR_ITEM) ||
				(itemproto->getItemIndex() == PET_YELLOW_HEART_COLOR_ITEM) )
		{
			if( (pet->GetPetType() != PET_TYPE_HORSE) &&
					(pet->GetPetType() != PET_TYPE_BLUE_HORSE) &&
					(pet->GetPetType() != PET_TYPE_UNKOWN_HORSE) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}

		// 타는말만 가능한 것들 William,Alber,Red eye,Zebra,Harold,Violet
		if( (itemproto->getItemIndex() == HORSE_WILLIAM_COLOR_ITEM)	||
				(itemproto->getItemIndex() == HORSE_ALBER_COLOR_ITEM)	||
				(itemproto->getItemIndex() == HORSE_REDEYE_COLOR_ITEM)	 ||
				(itemproto->getItemIndex() == HORSE_ZEBRA_COLOR_ITEM)	||
				(itemproto->getItemIndex() == HORSE_HAROLD_COLOR_ITEM)	||
				(itemproto->getItemIndex() == HORSE_VIOLET_COLOR_ITEM)	 )
		{
			if( !pet->IsMountType() ||
					(
						(pet->GetPetType() != PET_TYPE_HORSE) &&
						(pet->GetPetType() != PET_TYPE_BLUE_HORSE) &&
						(pet->GetPetType() != PET_TYPE_UNKOWN_HORSE)
					) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}

		switch( itemproto->getItemIndex() )
		{
		case PET_RED_COLOR_ITEM:
			PetColorType = (char)RED_COLOR;
			break;
		case PET_SCARLET_COLOR_ITEM:
			PetColorType = (char)SCARLET_COLOR;
			break;
		case PET_YELLOW_COLOR_ITEM:
			PetColorType = (char)YELLOW_COLOR;
			break;
		case PET_GREEN_COLOR_ITEM:
			PetColorType = (char)GREEN_COLOR;
			break;
		case PET_BLUE_COLOR_ITEM:
			PetColorType = (char)BLUE_COLOR;
			break;
		case PET_DEEP_BLUE_COLOR_ITEM:
			PetColorType = (char)DEEP_BLUE_COLOR;
			break;
		case PET_VIOLET_COLOR_ITEM:
			PetColorType = (char)VIOLET_COLOR;
			break;
		case PET_BLACK_COLOR_ITEM:
			PetColorType = (char)BLACK_COLOR;
			break;
		case PET_WHITE_COLOR_ITEM:
			PetColorType = (char)WHITE_COLOR;
			break;
		case PET_NON_COLOR_ITEM:
			PetColorType = (char)NON_COLOR;
			break;
		// 피닉스의 결정
		case 3262:
			PetColorType = (char)PHOENIX_COLOR;
			break;
		case PET_BLACK_HEART_COLOR_ITEM :
			PetColorType = (char) BLACK_HEART_COLOR;
			break;

		case PET_WHITE_HEART_COLOR_ITEM :
			PetColorType = (char) WHITE_HEART_COLOR;
			break;

		case PET_YELLOW_HEART_COLOR_ITEM :
			PetColorType = (char) YELLOW_HEART_COLOR;
			break;
		case HORSE_COLOR_CHANGE_ITEM:
			{
				if(	(pet->GetPetType() != PET_TYPE_HORSE) &&
						(pet->GetPetType() != PET_TYPE_BLUE_HORSE) &&
						(pet->GetPetType() != PET_TYPE_UNKOWN_HORSE))
				{
					CNetMsg::SP rmsg(new CNetMsg);
					CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
					SEND_Q(rmsg, ch->m_desc);
					return false;
				}

				if(extra1 < RED_COLOR || extra1 > VIOLET_SKELETON_COLOR )
					return false;

				PetColorType = (char)extra1;
			}
			break;
		case DRAGON_COLOR_CHANGE_ITEM:
			{
				if(	(pet->GetPetType() != PET_TYPE_DRAGON) &&
						(pet->GetPetType() != PET_TYPE_PINK_DRAGON) &&
						(pet->GetPetType() != PET_TYPE_UNKOWN_DRAGON))
				{
					CNetMsg::SP rmsg(new CNetMsg);
					CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
					SEND_Q(rmsg, ch->m_desc);
					return false;
				}

				if(extra1 < RED_COLOR || extra1 > VIOLET_SKELETON_COLOR )
					return false;

				if(extra1 >= BLACK_HEART_COLOR && extra1 <= VIOLET_SKELETON_COLOR)
				{
					if( !pet->IsMountType() )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
						SEND_Q(rmsg, ch->m_desc);
						return false;
					}
				}

				PetColorType = (char)extra1;
			}
			break;

		case HORSE_WILLIAM_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_WILLIAM_COLOR;
			}
			break;

		case HORSE_ALBER_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_ALBER_COLOR;
			}
			break;

		case HORSE_REDEYE_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_REDEYE_COLOR;
			}
			break;

		case HORSE_ZEBRA_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_ZEBRA_COLOR;
			}
			break;

		case HORSE_HAROLD_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_HAROLD_COLOR;
			}
			break;

		case HORSE_VIOLET_COLOR_ITEM:
			{
				PetColorType = (char) HORSE_VIOLET_COLOR;
			}
			break;
		}

		pet->SetPetColor( PetColorType );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPetColorChange( rmsg, ch->m_index, pet->m_index, PetColorType );
			SEND_Q( rmsg, gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			PetChangeColor( rmsg,  pet->m_index, PetColorType, ch->m_index, pet->GetPetTypeGrade()  );
			ch->m_pArea->SendToCell( rmsg, ch, true );
		}

		return true;
	}
	return false;
}

bool local_useCashItem_PetTurnToNPC(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 사용조건 검사
	// 착용하고 있는 펫에 적용
	// 펫 레벨이 16 이상의 말이고, 탈수 없을때
	CItemProto* itemproto = item->m_itemProto;

	if( ch->m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if( !pet )
			return false;

		int toNpc = -1 ;
		int	toNpcSize = 10;

		if( !pet->IsMountType() )
		{
			if( pet->GetPetTurnToNpc() == 0 ) // 변신중이 아니다
			{
				// 아이템이 말타입(1)이고 펫이 말이거나 아이템이 용타입(1)이고 펫이 용이거나 ,아이템 타입이 모두(0)일때
				if( pet->m_level > 15 &&
						( (itemproto->getItemNum4() == 1 && ( pet->GetPetType() & (0x10)) > 0 )
						  || (itemproto->getItemNum4() == 2 && ( pet->GetPetType() & (0x00)) > 0 )
						  || itemproto->getItemNum4() == 0
						)
				  )
				{
					toNpc = itemproto->getItemNum0();	// 변신 NPC 인덱스
					toNpcSize = itemproto->getItemNum1(); // 변신 npc size
				}
			}
			else	// 변신중이면 변신 해제만 가능
			{
				if( itemproto->getItemIndex() == 2605 )	// 변신 해제 아이템
					toNpc = 0;
			}

			if( toNpc > -1 )
			{
				// 이부분은 이벤트 활성화 여부와 상관 없이 막을지 결정해야함.
				if( toNpc > 0 )		// 변신 성공
				{
					//			int	nEventItemIndex = 2360;		// 펫의 명찰
					int nEventItemIndex = 2605;		// 펫 리턴주문서 2009 Xmas 이벤트 변경

					CItem* pEventItem =  gserver->m_itemProtoList.CreateItem( nEventItemIndex, -1, 0, 0, 1 );
					if( !pEventItem )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
						SEND_Q(rmsg, ch->m_desc);
						return false;
					}

					if (ch->m_inventory.addItem(pEventItem) == false)
					{
						delete pEventItem;

						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
						SEND_Q(rmsg, ch->m_desc);
						return false;
					}
				}
				pet->SetPetTurnToNpc( toNpc );
#ifdef PET_TURNTO_NPC_ITEM
				pet->SetPetTurnToNpcSize(toNpcSize);
#endif //PET_TURNTO_NPC_ITEM

				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperPetTurnToNPCMsg( rmsg, ch->m_index, pet->m_index, toNpc , toNpcSize );
					SEND_Q( rmsg, gserver->m_helper );
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					PetTurnToNPCMsg( rmsg,  pet->m_index, toNpc, ch->m_index , toNpcSize );
					//SEND_Q( rmsg, ch->m_desc );
					ch->m_pArea->SendToCell( rmsg, ch, true );
				}
				return true;
			}
		}
		//else
		//{
		// 조건에 맞지 않음
		//}
	}
	return false;
}

bool local_useCashItem_PetHPRecover(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	if (ch->m_wearInventory.wearItemInfo[WEARING_PET] == NULL)
		return false;

	CPet* pet = ch->GetPet();
	if (pet == false)
		return false;

	if( item->m_itemProto->getItemIndex() != PET_HPRECOVER_ITEM )
		return false;

	int upVal = 30;
	bool bUpdate = false;
	if( pet->m_hp < pet->m_maxHP )
	{
		pet->m_hp += upVal;
		if( pet->m_hp > pet->m_maxHP )
			pet->m_hp = pet->m_maxHP;
		bUpdate = true;
	}

	if( bUpdate )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetStatusMsg( rmsg, pet );
		SEND_Q( rmsg, ch->m_desc );
	}

	return true;
}

bool local_squib(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	CNetMsg::SP rmsg(new CNetMsg);
	EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_FIRECRACKER);
	SEND_Q( rmsg, ch->m_desc );
	ch->m_pArea->SendToCell(rmsg, ch, true);

	return true;
}

bool local_NSCreateCard(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	// 100레벨이상 캐릭터가 있으면 안됨
	// 나이트 쉐도우 캐릭터가 있으면 안됨
	// 나이트 쉐도우 생성카드 사용했는지 확인
	if( ch->m_desc->m_bCreate90Lv
			|| ch->m_level >= 90
			|| ch->m_desc->m_bNotCreateNightShadow
			|| ch->m_desc->m_bIsNSCreateCardUse)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_NS_CARD_INSUFF);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNSCreateCardUse(rmsg, MSG_HELPER_NS_CARD_USE, ch->m_desc->m_index, ch->m_index);
		RefMsg(rmsg) << item->getVIndex();
		SEND_Q(rmsg, gserver->m_helper);

		return false;
	}
}

bool local_useCashItem_WareHouse_Grocery_ShowHP_Sub(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	if( item->getUsed() < gserver->getNowSecond() )
		return false;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemUsed(rmsg, item->tab(), item->getInvenIndex(), item->getUsed());
		SEND_Q( rmsg, ch->m_desc );
	}

	switch (item->m_itemProto->getItemIndex())
	{
	case SHOW_HP_ITEM1:
	case SHOW_HP_ITEM2:
	case SHOW_HP_ITEM3:
	case 2606:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_PK);
				RefMsg(rmsg) << (unsigned char)MSG_PK_TARGET_SHOWHP
							 << (char)1 ;
				SEND_Q( rmsg, ch->m_desc );
			}

			CSkill* skill = NULL;
			skill = gserver->m_skillProtoList.Create( item->m_itemProto->getItemNum0(), item->m_itemProto->getItemNum1() );
			if( skill == NULL )
				return false;

			bool bApply;
			ApplySkill( ch, ch, skill, item->m_itemProto->getItemIndex(), bApply );
			if( !bApply )
			{
				delete skill;
				return true;
			}
			ch->setSearchLife(true);
			delete skill;
		}
		break;
	}

	// 포션 사용 이펙트 메시지
	CNetMsg::SP effectMsg(new CNetMsg);
	EffectItemMsg(effectMsg, ch, item);
	ch->m_pArea->SendToCell(effectMsg, ch, true);

	return true;
}

bool local_useCashItem_WareHouse_Grocery(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	if( local_useCashItem_WareHouse_Grocery_ShowHP_Sub(ch, item, bprolong, packet))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemUseMsg( rmsg, packet->tab, packet->invenIndex, item->m_itemProto->getItemIndex(), packet->extra_1 );
		SEND_Q( rmsg, ch->m_desc );

		GAMELOG << init("CASH_ITEM_WAREHOUSE_GROCERY_SHOWHP", ch )
				<< itemlog(item) << delim
				<< end;

		return false;
	}

	return true;
}

bool local_useCashItem_ShowHP(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	if( local_useCashItem_WareHouse_Grocery_ShowHP_Sub(ch, item, bprolong, packet))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemUseMsg( rmsg, packet->tab, packet->invenIndex, item->m_itemProto->getItemIndex(), packet->extra_1 );
		SEND_Q( rmsg, ch->m_desc );

		GAMELOG << init("CASH_ITEM_WAREHOUSE_GROCERY_SHOWHP", ch )
			<< itemlog(item) << delim
			<< end;

		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
typedef bool (*usecashItem_callFunc_)(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet);

typedef std::map<int, usecashItem_callFunc_> local_map_t;

local_map_t _local_map = boost::assign::map_list_of
						 (847, local_initStat)
						 (7617, local_initStat)
						 (6281, local_apetStatInit)
						 (848, local_initSkill)
						 (849, local_initSpecialSkill)
						 (850, local_initNormalAndSpecialSkill)
						 (851, local_initAllStatAndSkill)
						 (975, local_exchangeWeapon)

						 (1418, local_platinumSmelt)
						 (2894, local_platinumSmelt)
						 (3269, local_platinumSmelt)

						 (840, local_period_840)
						 (841, local_period_841)
						 (2347, local_period_2347)
						 (2348, local_period_2348)

						 (1420, local_useCashItem_1420)
						 (2664, local_useCashItem_2664)
						 (1421, local_useCashItem_1421)
						 (2665, local_useCashItem_2665)
						 (1540, local_useCashItem_1540)
						 (1519, local_useCashItem_1519)

						 (PET_RED_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_SCARLET_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_YELLOW_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_GREEN_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_BLUE_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_DEEP_BLUE_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_VIOLET_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_BLACK_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_WHITE_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (PET_NON_COLOR_ITEM, local_useCashItem_PetColorChange)
						 (3262, local_useCashItem_PetColorChange)
						 (PET_BLACK_HEART_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (PET_WHITE_HEART_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (PET_YELLOW_HEART_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_COLOR_CHANGE_ITEM , local_useCashItem_PetColorChange)
						 (DRAGON_COLOR_CHANGE_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_WILLIAM_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_ALBER_COLOR_ITEM	, local_useCashItem_PetColorChange)
						 (HORSE_REDEYE_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_ZEBRA_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_HAROLD_COLOR_ITEM , local_useCashItem_PetColorChange)
						 (HORSE_VIOLET_COLOR_ITEM , local_useCashItem_PetColorChange)

						 (2604, local_useCashItem_PetTurnToNPC)
						 (2763, local_useCashItem_PetTurnToNPC)
						 (2764, local_useCashItem_PetTurnToNPC)
						 (2765, local_useCashItem_PetTurnToNPC)
						 (2605, local_useCashItem_PetTurnToNPC)

#ifdef PET_HPRECOVER_ITEM		// 펫 HP 회복
						 (2454, local_useCashItem_PetHPRecover)
#endif

						 (2403, local_squib)
						 (4914, local_NSCreateCard)

						 (2455, local_useCashItem_WareHouse_Grocery)
						 (2456, local_useCashItem_WareHouse_Grocery)
						 (2457, local_useCashItem_WareHouse_Grocery)
						 (2458, local_useCashItem_WareHouse_Grocery)
						 (2459, local_useCashItem_WareHouse_Grocery)
						 (2460, local_useCashItem_WareHouse_Grocery)
						 (2606, local_useCashItem_WareHouse_Grocery)
						 (2607, local_useCashItem_WareHouse_Grocery)
						 (2608, local_useCashItem_WareHouse_Grocery)
						 (2867, local_useCashItem_WareHouse_Grocery)
						 (2868, local_useCashItem_WareHouse_Grocery)

						 (2461, local_useCashItem_ShowHP)
						 (2462, local_useCashItem_ShowHP)
						 (2463, local_useCashItem_ShowHP)
						 ;

//////////////////////////////////////////////////////////////////////////

bool do_useCashItem(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet)
{
	local_map_t::iterator it = _local_map.find(item->getDBIndex());
	if (it == _local_map.end())
		return false;

	return (*it->second)(ch, item, bprolong, packet);
}
