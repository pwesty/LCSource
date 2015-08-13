#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "Battle.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DratanCastle.h"
#include "LuckyDrawBox.h"
#include "Party_recall_manager.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "itemExchangeProto.h"
#include "EquipExchangeExtend.h"
#include "DBManager.h"

//////////////
// 아이템 관련
extern bool do_useCashItem(CPC* ch, CItem* item, bool bprolong, RequestClient::doItemUse* packet);

#if defined (SAKURA_EVENT_2008_REWARD)
bool do_ItemUse_SakuraBox(CPC * pc, const CItemProto * pItemProto, int nExtra1);
#endif
bool do_ItemUse_2046(CPC* ch, const CItemProto* itemproto, int nExtra1);
bool do_ItemUse_2148(CPC* ch, const CItemProto* itemproto);
bool do_itemUse_MoonStoneBox(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2146_2147( CPC* ch, const CItemProto* itemproto );

void do_GroceryItemBuy(CPC* ch, CNetMsg::SP& msg);
void do_GroceryItemSell(CPC* ch, CNetMsg::SP& msg);

bool do_ItemUse_Coin(CPC * pc, const CItemProto * pItemProto, int nExtra1);

bool do_ItemUse_2037(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2597(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_1838_1839(CPC* ch, const CItemProto* itemproto, int nExtar1);
bool do_ItemUse_IONCE_ETC(CPC* ch, const CItemProto* itemproto, int nExtar1,CItem* item);
bool do_ItemUse_IETC_PRODUCT(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_IONCE_SUMMON(CPC* ch, CItem* item);
bool do_ItemUse_IONCE_QUEST_SCROLL(CPC* ch, const CItemProto* itemproto, int extra1);
bool do_ItemUse_IONCE_CHANGE_DOC(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_IONCE_WARP_RETURN(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_IETC_SKILLUP(CPC* ch, CItem* item);
void do_ItemUsePartyRecall(CPC* ch, CItem* pItem);
bool do_ItemUse_5952(CPC* ch, const CItemProto* itemproto);

void do_ItemTake_RecommendPotion(CPC* ch, CItem* pItem);
//#endif

bool do_ItemTake_EventCollectBug(CPC* ch, CItem* pItem);

void do_ItemLendWeapon(CPC* ch, CNetMsg::SP& msg);

bool do_ItemUse_Castle_Guard(CPC* ch, CItem* item);

void do_ItemUseExchagneEquip( CPC* ch, CNetMsg::SP& msg );

// 대전용 아이템
bool do_useCashItem_CompetitionItem( CPC* ch, CItem* pItem );

bool do_useEventItem_HalloweenCandy( CPC* ch, CItem* pItem );
void do_itemUse_WarpDoc(CPC* ch, CNetMsg::SP& msg);

bool do_ItemUse_2887(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2888(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2889(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2890(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2891(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2892(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2893(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2968(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_GoldBox(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_3769(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_1053(CPC* ch, const CItemProto* itemproto);

bool do_ItemUse_4664(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_4786(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_4787(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_4788(CPC* ch, const CItemProto* itemproto);

bool do_ItemUse_4709(CPC* ch, const CItemProto* itemproto,CItem* item);

bool do_ItemUse_IETC_TriggerItem4567(CPC* ch);
void do_ItemUse_DeleteRaidCharacter_4911(CPC* ch, const CItemProto* itemproto);

bool do_ItemUse_LuckyDrawBox(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_LuckyDrawBoxResult_Prob_Random(CPC* ch, const CItemProto* itemproto, LuckyDrawBoxData * pLucky);
bool do_ItemUse_LuckyDrawBoxResult_All(CPC* ch, const CItemProto* itemproto, LuckyDrawBoxData * pLucky);

bool do_ItemUse_WorldFestivalBox(CPC* ch, CItem* pItem);

//#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
bool do_ItemUse_WorldFestivalBox(CPC* ch, CItem* pItem);
//#endif

bool do_ItemUse_5329(CPC* ch, const CItemProto* itemproto, int nExtar1);

void do_ItemUse_IETC_MonsterMercearyToggle(CPC* pPc, CItem* pItem);

bool do_ItemUse_6593(CPC* ch, const CItemProto* itemproto);

#include "JumpingSystem.h"
bool canJumping(CPC* ch, int jumpingLevel);

int getUpgradeRuneResult(CItem *reItem, CItem *upItem, CPC *ch);

void do_ItemDivide(CPC* ch, CNetMsg::SP& msg);

void do_ItemExchange(CPC* ch, CNetMsg::SP& msg);

void do_ItemCompose(CPC* ch, CNetMsg::SP& msg);

typedef struct _GiftItemInfo
{
	int index;
	int count;
} GIFTITEMINFO;

void do_Item(CPC* ch, CNetMsg::SP& msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckRespond(ch);

	// 죽거나 교환중이면 무시
	if (DEAD(ch) || ch->m_exchange != NULL || ch->m_currentSkill != NULL || ch->m_personalshop != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_DO_NOT_MEET_CONDITION );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_ITEM_USE:
		do_ItemUse(ch, msg);
		break;

	case MSG_ITEM_TAKE:
		do_ItemTake(ch, msg);
		break;

	case MSG_ITEM_THROW:
		do_ItemThrow(ch, msg);
		break;

	case MSG_ITEM_DELETE:
		do_ItemDelete(ch, msg);
		break;

	case MSG_ITEM_WEAR:
		do_ItemWear(ch, msg);
		break;

	case MSG_ITEM_WEAR_TAKE_OFF:
		do_ItemWearTakeOff(ch, msg);
		break;

	case MSG_ITEM_SWAP:
		do_ItemSwap(ch, msg);
		break;

	case MSG_ITEM_BUY:
		do_ItemBuy(ch, msg);
		break;

	case MSG_ITEM_SELL:
		do_ItemSell(ch, msg);
		break;

	case MSG_ITEM_UPGRADE_REQ:
		do_ItemUpgradeReq(ch, msg);
		break;

	case MSG_ITEM_REFINE_REQ:
		do_ItemRefinReq(ch, msg);
		break;

	case MSG_ITEM_OPTION_ADD_REQ:
		do_ItemOptionAddReq(ch, msg);
		break;

	case MSG_ITEM_OPTION_DEL_REQ:
		do_ItemOptionDelReq(ch, msg);
		break;

	case MSG_ITEM_PROCESS_REQ:
		do_ItemProcessReq(ch, msg);
		break;

	case MSG_ITEM_MAKE_REQ:
		do_ItemMakeReq(ch, msg);
		break;

	case MSG_ITEM_MIX_REQ:
		do_ItemMixReq(ch, msg);
		break;

	case MSG_ITEM_ARCANE_REQ:
		do_ItemArcaneReq(ch, msg);
		break;

	case MSG_ITEM_CHANGEWEAPON_REQ:
		do_ItemChangeWeaponReq(ch, msg);
		break;

	case MSG_ITEM_PROCESS_NPC:
		do_ItemProcessNPC(ch, msg);
		break;

	case MSG_ITEM_ADD_BOOSTER:
		do_ItemAddBooster(ch, msg);
		break;

	case MSG_ITEM_MIX_WARITEM:
		do_ItemMixWarItem(ch, msg);
		break;

	case MSG_ITEM_USE_PROLONG:
		do_ItemUse(ch, msg, true);
		break;

	case MSG_ITEM_LEND_WEAPON:
		do_ItemLendWeapon(ch, msg);
		break;

	case MSG_ITEM_TARGET:
		do_ItemTarget(ch, msg);
		break;

	case MSG_ITEM_EXCHANGE_EQUIP_REQ:
		do_ItemUseExchagneEquip( ch, msg );
		break;

	case MSG_ITEM_USE_GROCERY_BUY:
		do_GroceryItemBuy(ch, msg);
		break;
	case MSG_ITEM_USE_GROCERY_SELL:
		do_GroceryItemSell(ch, msg);
		break;

	case MSG_ITEM_USE_WARPDOC:
		do_itemUse_WarpDoc(ch, msg);
		break;

	case MSG_ITEM_USE_PRESSCORPS:
		do_itemUse_PressCorps(ch, msg);
		break;

	case MSG_ITEM_CHECK_COMPOSITION:
		do_itemCheck_Composition( ch, msg );
		break;

	case MSG_ITEM_WEAR_COSTUME:
		do_ItemWear_Costume(ch, msg);
		break;
	case MSG_ITEM_WEAR_COSTUME_TAKEOFF:
		do_Itemwear_Costume_TakeOff(ch, msg);
		break;

// [110207:selo] 한 벌 의상
	case MSG_ITEM_WEAR_COSTUME_SUIT:
		do_ItemWear_Costume_Suit(ch, msg);
		break;
	case MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF:
		do_ItemWear_Costuem_Suit_TakeOff(ch, msg);
		break;

	case MSG_ITEM_COSTUME_SUIT_FORTUNE:
		do_Item_Make_Fortune(ch, msg);
		break;

	case MSG_ITEM_MASTERSTONE_USE :
		do_ItemUse_MasterStone_USA(ch, msg);
		break;

	case MSG_ITEM_DIVIDE:
		do_ItemDivide(ch, msg);
		break;

	case MSG_ITEM_SORT_INVENTORY:
		ch->m_inventory.sort();
		break;

	case MSG_ITEM_EXCHANGE:
		do_ItemExchange(ch, msg);
		break;

	case MSG_ITEM_COMPOSE:
		do_ItemCompose(ch, msg);
		break;

	default:
		{
			LOG_ERROR("invalid packet. charIndex[%d] / subType[%d]", ch->m_index, pBase->subType);
			ch->m_desc->Close("invalid packet");
		}
		break;
	}

	// 아이템 관련 메시지 처리후 착용 정보에 따라 소환/강신 취소
	CItem* pItemWeapon = ch->m_wearInventory.wearItemInfo[WEARING_WEAPON];
	bool bCancelSummonEvocation = false;
	if (!pItemWeapon || !pItemWeapon->IsWeaponType())
	{
		// 무기 미착용
		bCancelSummonEvocation = true;
	}
	else
	{
		// 무기 착용
		// 직업에 따라
		switch (ch->m_job2)
		{
		case JOB_2ND_ELEMENTALLIST:
			// 엘리멘탈리스트는 폴암 아니면 소환 취소
			if (pItemWeapon->m_itemProto->getItemSubTypeIdx() != IWEAPON_POLEARM)
				bCancelSummonEvocation = true;
			break;

		case JOB_2ND_SPECIALLIST:
			// 스페셜리스트는 사이드 아니면 강신 취소
			if (pItemWeapon->m_itemProto->getItemSubTypeIdx() != IWEAPON_SCYTHE)
				bCancelSummonEvocation = true;
			break;

		default:
			// 전직 전에는 폴암 사이드 둘다 됨
			if (pItemWeapon->m_itemProto->getItemSubTypeIdx() != IWEAPON_POLEARM && pItemWeapon->m_itemProto->getItemSubTypeIdx() != IWEAPON_SCYTHE)
				bCancelSummonEvocation = true;
			break;
		}
	}
	if (bCancelSummonEvocation)
	{
		// 소환 취소
		while (ch->m_elementalList)
			ch->UnsummonElemental(ch->m_elementalList);
		// 강신 취소
		if (ch->m_evocationIndex != EVOCATION_NONE)
			ch->Unevocation();
	}
}

void do_ItemUse(CPC* ch, CNetMsg::SP& msg, bool bprolong)
{
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 아이템 찾기
	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (item->getVIndex() != packet->virtualIndex)
	{
		LOG_ERROR("HACKING? : invalid virtual index. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid virtual index");
		return;
	}

	if (item->getWearPos() != WEARING_NONE)
	{
		LOG_ERROR("HACKING? : item is wearing. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid virtual index");
		return;
	}

	if (item->Count() < 1)
	{
		ch->m_inventory.deleteItemByItem(item);
		return;
	}

	if ( ch->m_level < item->m_itemProto->GetItemProtoLevel() || ch->m_level > item->m_itemProto->GetItemProtoLevel2() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_LOWLEVEL );
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItemProto* itemproto = item->m_itemProto;
	if(itemproto->getItemFlag() & ITEM_FLAG_NOTSINGLE && ch->m_pZone->IsPersonalDungeon() == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if ( (ch->m_pZone->isRVRZone()) && (itemproto->getItemFlag() & ITEM_FLAG_NONRVR) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if ( itemproto->getItemRvRType() > 0 && (ch->getSyndicateType() != itemproto->getItemRvRType()) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_DO_NOT_MEET_CONDITION, 0, 0, itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if ( ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType()) < itemproto->getItemRvRGrade() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_DO_NOT_MEET_CONDITION, 0, 0, itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( itemproto->getCastleWar() > 0 )
	{
		if ( ch->GetMapAttr() & MATT_WAR || ch->m_pZone->IsWarZone(GET_X(ch), GET_Z(ch)) == true ) 
		{
			CWarCastle* castle = NULL;

			if(ch->m_pZone->m_index == ZONE_DRATAN)
			{
				castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
				if (castle == NULL)
					return ;
			}
			else if(ch->m_pZone->m_index == ZONE_MERAC)
			{
				castle = CWarCastle::GetCastleObject(ZONE_MERAC);
				if (castle == NULL)
					return ;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, itemproto->getItemIndex());
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if (castle->GetState() != WCSF_WAR_CASTLE)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, itemproto->getItemIndex());
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			switch(itemproto->getCastleWar())
			{
			case ITEM_CASTLE_WAR_FLAG_MERAC:
				if(ch->m_pZone->m_index != ZONE_MERAC)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, itemproto->getItemIndex());
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				break;
			case ITEM_CASTLE_WAR_FLAG_DRATAN:
				if(ch->m_pZone->m_index != ZONE_DRATAN)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, itemproto->getItemIndex());
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				break;
			}
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, itemproto->getItemIndex());
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	
	// 스톤 걸려도 스톤 치료물약은 되게
	if (ch->m_assist.m_state & AST_STONE)
	{
		switch(item->m_itemProto->getItemIndex())
		{
		case 874:
		case 3582:
		case 4912:
			break;

		default:
			return;
		}
	}

	switch(item->m_itemProto->getItemIndex())
	{
	case 857:					// 순간 이동 마법서
	case 858:					// 소환 마법서
	case 1298:					// 파티 리콜 1일 정액
	case 1299:					// 파티 리콜 7일 정액
	case 1300:					// 파티 리콜 30일 정액
	case 1413:					// 파티 리콜 3시간 정액
	case 1414:					// 파티 리콜 7시간 정액
	case 1415:					// 파티 리콜 1시간 정액
		if ((ch->m_nJoinInzone_ZoneNo >=0 && ch->m_nJoinInzone_RoomNo >= 0) || (ch->m_pZone->IsPersonalRaid()))
			return;
		break;

	case 3218:
		if(ch->m_level > 100)
			return;
		break;

	case 4933:		// 부활주문서2 사용할 수 없음
	case 5958:		// 부활 주문서2 초보자용 사용할 수 없음
		return;

	case 6101:
	case 6102:
		if(ch->m_pZone->m_index != ZONE_AKAN_TEMPLE)
			return;
		break;
	}

	if (ch->m_pZone->IsWarGroundZone() && itemproto->isUseInWarZone() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ItemFlag()에 따를 처리 - 시작
	if(itemproto->getItemFlag() & ITEM_FLAG_CASTLLAN)
	{
		int zonenum;
		char castellan;
		bool ret;
		ret = ch->CheckCastellanType(zonenum, castellan);

		if(itemproto->getItemNum3() & 0xffff0000)  // 성주멤버용 아이템
		{
			if(!ret || castellan)
			{
				// 성주멤버가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN_GUILD);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if(zonenum != (itemproto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				return ;
			}
		}
		else		// 성주용 아이템
		{
			if(!ret || !castellan)
			{
				// 성주가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if(zonenum != (itemproto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				return ;
			}
		}
	}
	else if( itemproto->getItemFlag() & ITEM_FLAG_QUEST )
	{
		if( ch->m_pZone->m_index != itemproto->getItemQuestZone() ||
				GetDistance(itemproto->getItemQuestX(), GET_X(ch), itemproto->getItemQuestZ(), GET_Z(ch), 0, 0) >= itemproto->getItemQuestRange() )
			return;
	}
	// ItemFlag()에 따를 처리 - 끝
	//////////////////////////////////////////////////////////////////////////

	// 비행중엔 포션 만 사용가능
	if( ch->IsSetPlayerState(PLAYER_STATE_FLYING) && itemproto->getItemTypeIdx() != ITYPE_POTION )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_FLYING );
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// Item 효과 적용
	switch (itemproto->getItemTypeIdx())
	{
	case ITYPE_ONCE:				// 일회용 아이템
		if( itemproto->getItemFlag() & ITEM_FLAG_BOX  )
		{
			if( do_ItemUse_LuckyDrawBox(ch, itemproto) )
				break;
			else
				return;
		}

		switch (itemproto->getItemSubTypeIdx())
		{
		case IONCE_WARP:
			switch (itemproto->getItemNum0())
			{
			case IONCE_WARP_RETURN:			// 귀환 스크롤
				if (!do_ItemUse_IONCE_WARP_RETURN(ch, itemproto))
					return;
				break;

			// 060227 : bs : 파티리콜 : 사용후에도 수량 감소 없음
			case IONCE_WARP_PARTYRECALL:
				do_ItemUsePartyRecall(ch, item);
				return;

			default:
				return;
			}
			break;

		case IONCE_CHANGE_DOC:
			if (!do_ItemUse_IONCE_CHANGE_DOC(ch, itemproto))
				return;
			break;

		case IONCE_QUEST_SCROLL:
			if (!do_ItemUse_IONCE_QUEST_SCROLL(ch, itemproto, packet->extra_1))
				return;
			break;

		case IONCE_CASH:
			if(item->getDBIndex() == 10421)
			{
				if(ch->m_assist.FindBySkillIndex(1771))
				{
					//아이템을 사용 할 수 없습니다.
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_DO_NOT_USE_ITEM_STATE);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				bool Apply;
				CSkill* skill = gserver->m_skillProtoList.Create(itemproto->getItemNum0(), itemproto->getItemNum1());
				ApplySkill(ch, ch, skill, item->getDBIndex(), Apply);

				ch->m_assist.CureBySkillIndex(1060);

				ch->m_attendanceManager.setUseAssure(true);
				break;
			}
			
			if (do_useCashItem(ch, item, bprolong, packet) == false)
				return;

			break;
			

		case IONCE_SUMMON:
			//공성 중이고, 공성 지역에서는 사용이 불가능
			if( ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE && (item->getDBIndex() != 2380 && item->getDBIndex() != 2381) )
				return;

			if (!do_ItemUse_IONCE_SUMMON(ch, item))
				return;
			break;

		case IONCE_BOX:
			switch(itemproto->getItemIndex())
			{
			case 2046:
			case 2632:
				if(!do_ItemUse_2046(ch, itemproto, packet->extra_1))
					return ;
				break;
			case 2740:
			case 2741:
			case 2742:
			case 2743:
			case 2744:
				if(do_ItemUse_Coin(ch, itemproto, packet->extra_1) == false)
				{
					return;
				}
				break;
#if defined (SAKURA_EVENT_2008_REWARD)
			case 2747:
				if(do_ItemUse_SakuraBox(ch, itemproto, packet->extra_1) == false)
				{
					return;
				}
				break;
#endif //defined (SAKURA_EVENT_2008_REWARD)

			case 3576:
			case 3713:
			case 3714:
			case 3715:
			case 3716:
			case 3717:
			case 3718:
			case 3762:
				if(!do_ItemUse_GoldBox(ch, itemproto))
					return ;
				break;

			default:
				return;
			}
			break;
		case IONCE_ETC:
			if( itemproto->getItemIndex() == EXPERIENCE_COMPETITION_ITEM
					|| itemproto->getItemIndex() == PERIOD_COMPETITION_ITEM
					|| itemproto->getItemIndex() == 2609
					|| itemproto->getItemIndex() == 5018 || itemproto->getItemIndex() == 5019 // 광속2
			  )
			{
				if(do_useCashItem_CompetitionItem( ch, item ))
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::ItemUseMsg(rmsg, packet->tab, packet->invenIndex, itemproto->getItemIndex(), packet->extra_1);
					SEND_Q(rmsg, ch->m_desc);

					GAMELOG << init("CASH_ITEM_USE", ch)
							<< itemlog(item) << delim
							<< end;
				}
				return;
				break;
			}
			if( itemproto->getItemIndex() == 3769 )			// 독일 보너스 패키지
			{
				if( !do_ItemUse_3769( ch , itemproto ) )
					return;
				break;
			}
			// 레이드 던전 초기화 카드
			if (itemproto->getItemIndex() == 4911)
			{
				// 여기선 아이템을 무조건 남겨놔야 한다.
				// 헬퍼를 갔다와서 성공하면 아이템을 삭제한다.
				do_ItemUse_DeleteRaidCharacter_4911(ch, itemproto);
				return;
				break;
			}

			//#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
			if( itemproto->getItemIndex() == 5347 ) // 월드 축제 박스
			{
				do_ItemUse_WorldFestivalBox(ch, item);
				return;
			}
			//#endif

			if( itemproto->getItemIndex() == 10022 )
			{
				if(ch->m_job != JOB_NIGHTSHADOW)
				{
					LOG_ERROR("HACKING? : invalid packet. char_index[%d], char_job[%d]",
							  ch->m_index, ch->m_job);
					ch->m_desc->Close("invalid packet");
					return;
				}
				if(ch->m_nSoul_Count >= MAX_SOUL_COUNT)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_SOUL_COUNT_ALREADY_FULL, -1, -1);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				ch->m_nSoul_Count += 1;

				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillSoulCountMsg(rmsg, ch->GetSoul_Count());
				SEND_Q(rmsg, ch->m_desc);

				break;
			}

			if( itemproto->getItemIndex() == 10286)
			{
				CAPet* apet = ch->GetAPet();
				if (apet == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetFuntionMsg( rmsg, MSG_SUB_INIT_COOLTIME, NULL, 1);
					SEND_Q( rmsg , ch->m_desc );
					return;
				}

				if(apet->GetCooltime() <= 0)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetFuntionMsg( rmsg, MSG_SUB_INIT_COOLTIME, NULL, 2);
					SEND_Q( rmsg , ch->m_desc );
					return;
				}

				apet->ResetCooltime();

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetStatusMsg( rmsg, apet );
					SEND_Q( rmsg, ch->m_desc );
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetFuntionMsg( rmsg, MSG_SUB_INIT_COOLTIME, NULL, 0);
					SEND_Q( rmsg , ch->m_desc );
				}
				break;
			}

			if (!do_ItemUse_IONCE_ETC(ch, itemproto, packet->extra_1, item))
				return;
			break;

		case IONCE_TITLE:    //  아이템 테이블의 'a_num_0' 필드에서 호칭 아이템 인덱스 가져 오기

			if(ch->m_titleList.HaveTitle(itemproto->getItemNum0()))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TitleItemCantUseMsg(rmsg, itemproto->getItemNum0());  // 호칭 중복(이미 보유 중인 호칭)
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if(ch->m_titleList.m_nCount >= MAX_TITLE_COUNT)
			{
				return;
			}

			CTitleProto *proto;
			int endtime;

			proto = gserver->m_titleProtoList.FindProto(itemproto->getItemNum0());

			if(proto == NULL)
				return;

			if (proto->m_time != -1)
				endtime = gserver->getNowSecond() + (proto->m_time * 24 * 60 * 60);
			else
				endtime = proto->m_time;

			if(gserver->isRunSubHelper())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperTitleSystemInsertReq(rmsg, ch->m_index, itemproto->getItemNum0(), endtime, packet->tab, packet->invenIndex, item->m_serial.c_str());
				SEND_Q(rmsg, gserver->m_subHelper);
				return;
			}
			else
				return;
//////////////////////////////////////////////////////////////////////////////////////////
			break;

		case IONCE_REWARD_PACKAGE:
			{
				// TODO 1 :: 아이템 지급
				int _rewardIndex = itemproto->getItemNum0();
				if ( _rewardIndex > 0 )
				{
					if ( !gserver->m_rewardMgr.setRewardData(ch, _rewardIndex) || !gserver->m_rewardMgr.doReward(ch, _rewardIndex) )
					{
						GAMELOG << init("REWARD ITEM USE FAIL")
								<< _rewardIndex << end;
						return;
					}
					else
					{
						GAMELOG << init("REWARD ITEM USE SUCCESS") << end;
					}
				}
				else
				{
					return;
				}
			}
			break;
		case IONCE_JUMPING_POTION:
			{
				// TODO 1 :: 아이템 프로토에서 num0(점핑 레벨)을 읽는다.
				int _jumpingLevel = itemproto->getItemNum0();
				if ( _jumpingLevel <= 0 )
				{
					return;
				}

				// TODO 2 :: 점핑 가능한지 체크
				if ( !canJumping(ch, _jumpingLevel) )
				{
					return;
				}

				// TODO 3 :: 점핑
				CJumpingSystem jp(ch, item->getDBIndex());
				if ( !jp.check() || !jp.jump() )
				{
					return;
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					JumpingResultMsg(rmsg, MSG_EX_JUMPING_SUCCESS);
					SEND_Q(rmsg, ch->m_desc);
				}

				GAMELOG << init("JUMPING SYSTEM")
						<< "JUMPING SUCCESS" << delim
						<< ch->m_index << delim << ch->GetName() << end;
			}
			break;
		case IONCE_EXTEND_CHARACTER_SLOT:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperCharSlotExtUse(rmsg, ch->m_desc->m_index, ch->m_index, packet->tab, packet->invenIndex, item->m_serial.c_str(), itemproto->getItemNum0());
				SEND_Q(rmsg, gserver->m_subHelper);

				return;
			}
			break;
		case IONCE_REMOTE_EXPRESS:
			{
				ch->m_TimerItem.appendByDay(TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM, itemproto->getItemNum4());
			}
			break;

		case IONCE_JEWEL_POCKET:
			{
				//빈 공간 검색
				if(ch->m_inventory.getEmptyCount() < 1)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysFullInventoryMsg(rmsg, 0);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
				int grade = item->m_itemProto->getItemNum0();
				int jewelIndex = 0;
				int rand = 0;

				while(jewelIndex <= 0)
				{
					rand = GetRandom(1, gserver->m_jewelDataList.m_nJewelItemKindCount);
					jewelIndex = gserver->m_jewelDataList.getRandomJewel(grade, rand);
				}

				CItem *jewelItem = gserver->m_itemProtoList.CreateItem(jewelIndex, -1, 0, 0, 1);
				if (jewelItem == NULL)
				{
					LOG_ERROR("can't create item. charIndex[%d] itemdbIndex[%d]", ch->m_index, jewelIndex);
					return;
				}

				if (ch->m_inventory.addItem(jewelItem) == false)
				{
					delete jewelItem;

					CNetMsg::SP rmsg(new CNetMsg);
					SysFullInventoryMsg(rmsg, 0);
					SEND_Q(rmsg, ch->m_desc);

					return;
				}
			}

			break;

		case IONCE_CHAOS_JEWEL_POCKET:
			{
				//빈 공간 검색
				if(ch->m_inventory.getEmptyCount() < 1)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysFullInventoryMsg(rmsg, 0);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
				int grade = item->m_itemProto->getItemNum0();
				int jewelIndex = 0;
				int rand = 0;

				while(jewelIndex <= 0)
				{
					rand = GetRandom(1, gserver->m_jewelDataList.m_nJewelItemKindCount_Chaos);
					jewelIndex = gserver->m_jewelDataList.getRandomChaosJewel(grade, rand);
				}

				CItem *jewelItem = gserver->m_itemProtoList.CreateItem(jewelIndex, -1, 0, 0, 1);
				if (jewelItem == NULL)
				{
					LOG_ERROR("can't create item. charIndex[%d] itemdbIndex[%d]", ch->m_index, jewelIndex);
					return;
				}

				if (ch->m_inventory.addItem(jewelItem) == false)
				{
					delete jewelItem;

					CNetMsg::SP rmsg(new CNetMsg);
					SysFullInventoryMsg(rmsg, 0);
					SEND_Q(rmsg, ch->m_desc);

					return;
				}
			}

			break;

		case IONCE_CASH_INVENTORY:
			{
				if (itemproto->getItemNum0() == 1)
				{
					ch->m_TimerItem.appendByDay(TIMERITEM_TYPE_CASH_INVENTORY_1, itemproto->getItemNum4());
				}
				else if (itemproto->getItemNum0() == 2)
				{
					ch->m_TimerItem.appendByDay(TIMERITEM_TYPE_CASH_INVENTORY_2, itemproto->getItemNum4());
				}
				else
				{
					LOG_ERROR("Item(IONCE_CASH_INVENTORY) valud(%d) is error.", itemproto->getItemNum0());
				}
			}
			break;

		case IONCE_PET_STASH:
			ch->m_TimerItem.appendByDay(TIMERITEM_TYPE_PET_STASH, itemproto->getItemNum4());
			//이펙트 메시지 0~5 만들어서 보내주고 캐릭터 이펙트 정보에 저장 한다.
			ch->m_petStashManager.ApplyEffect();
			break;

		case IONCE_GPS:
			if( do_UseGpsItem(item, ch) == false)
				return;
			break;

		case IONCE_HOLY_WATER:
			//이그니션 상태이거나 증폭제 버프가 걸려있는 상태에서는 사용할 수 없다.
			if(ch->m_assist.FindBySkillIndex(1062) || ch->m_assist.FindBySkillIndex(1063) || ch->m_assist.FindBySkillIndex(1064) || ch->m_assist.FindBySkillIndex(1065))
			{
				//사용할 수 없음
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_DO_NOT_USE_ITEM_STATE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			if(ch->m_assist.FindByItemIndex(2875))
			{
				//사용할 수 없음
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_DO_NOT_USE_ITEM_STATE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			//사용중인 성수 아이템이 존재하고 있다면
			if(ch->holy_water_item != NULL && item->getVIndex() != ch->holy_water_item->getVIndex())
				ch->changeToggleState(ch->holy_water_item->getVIndex(), TOGGLE_ITEM);

			if(item->isToggle() == false)
			{
				ch->SendHolyWaterStateMsg(item);
			}
			else
			{
				ch->SendHolyWaterStateMsg(NULL);
			}
			break;
		case IONCE_PROTECT_PVP:
			//pvp 보호 아이템 사용시에 유물 아이템을 들고 있다면 사용 불가 메시지 전송
			if(ArtifactManager::instance()->isOwnerPC(ch->m_index) == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_ARTIFACT_ITEM_DO_NOT_USE_PVPPROTECT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			if( do_UsePvPProtect(item, ch) == false )
				return;
			break;
		default:
			return;
		}

		break;

	case ITYPE_POTION:
		{
			switch( itemproto->getItemSubTypeIdx() )
			{
			case IPOTION_NPC_PORTAL:
				if( ch->m_pZone->m_index == itemproto->getItemNum0() ) // 현재 있는 존과 사용한 스크롤 존이 같으면 리스트를 보내준다.
				{
					CNetMsg::SP rmsg(new CNetMsg);
					NpcPortalListMsg(rmsg, itemproto->getItemNum0());
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					CNetMsg::SP rmsg(new CNetMsg);
					NpcPortalListErrorMsg(rmsg, MSG_NPC_PORTAL_ERROR_LIST);
					SEND_Q(rmsg, ch->m_desc);
				}
				return;
				break;
			}

			// 눈물
			if (itemproto->getItemSubTypeIdx() == IPOTION_TEARS)
			{
				switch (itemproto->getItemNum0())
				{
				// 구원의 눈물
				case IPOTION_TEARS_TYPE_SAFE:

					if (ch->m_pkPenalty < -15)
					{
						ch->m_pkPenalty++;
						ch->m_bChangeStatus = true;
					}
					else
						return;

					break;

				// 용서의 눈물
				case IPOTION_TEARS_TYPE_FORGIVE:

					if (ch->m_exp < 0)
					{
						LONGLONG plusexp = GetLevelupExp(ch->m_level) * 5 / 100;
						ch->m_exp += plusexp;

						if (ch->m_exp > 0)
							ch->m_exp = 0;

						ch->m_bChangeStatus = true;
					}
					else
						return;

					break;

				default:
					return;
				}
			}
			else
			{
				switch (itemproto->getItemIndex())
				{
				case 844:		// 경험치복구 주문서
				case 845:		// 숙련도복구 주문서
				case 2035:		// 럭키 경험치복구 주문서
				case 2036:		// 럭키 숙련도복구 주문서
					// 부활주문서의 이펙트 인덱스 / 각레벨
					if( ch->m_assist.Find(184, 2) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXP_SP);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if( ch->m_assist.Find(185, 2) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SP_EXP);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				}

				switch (itemproto->getItemIndex())
				{
				case 8165:											//신경 교란 장치(퀘스트 필요 아이템)
					if(ch->m_questList.FindQuest(636, QUEST_STATE_RUN) == false)
					{
						return;
					}

				case 2855:											// 플래티늄 드랍 증폭제
					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4939);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					if(ch->m_assist.FindByItemIndex(884)
							|| ch->m_assist.FindByItemIndex(838)
							|| ch->m_assist.FindByItemIndex(972)
							|| ch->m_assist.FindByItemIndex(1629)
							|| ch->m_assist.FindByItemIndex(2856)
							|| ch->m_assist.FindByItemIndex(6096)
							|| ch->m_assist.FindByItemIndex(5080) //강운의 스크롤
							|| ch->m_assist.FindByItemIndex(5081) // 복운의 스크롤
					  )
						return;
					break;

				case 2856:											// 플래티늄 행운의 스크롤
					if(ch->m_assist.FindByItemIndex(884)
							|| ch->m_assist.FindByItemIndex(838)
							|| ch->m_assist.FindByItemIndex(972)
							|| ch->m_assist.FindByItemIndex(1629)
							|| ch->m_assist.FindByItemIndex(2855)
							|| ch->m_assist.FindByItemIndex(6096)
							|| ch->m_assist.FindByItemIndex(5080) //강운의 스크롤
							|| ch->m_assist.FindByItemIndex(5081) // 복운의 스크롤
					  )
						return;
					break;

#if defined (PLATINUM_SKILL_POTION_ITEM ) || defined (SKILL_POTION_ITEM)
				case 2452:		// 숙련의 묘약
					if (ch->m_assist.FindByItemIndex(2453) || ch->m_assist.FindByItemIndex(7611))
					{
						// 플래티늄 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2453);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 2453:		// 플래티늄 숙련의 묘약
				case 2659:		// 초보자용 플래티늄 숙련의 묘약
				case 7611:		// [이벤트] 플래티늄 숙련의 묘약
					if (ch->m_assist.FindByItemIndex(2452))
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2452);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
#endif // defined (PLATINUM_SKILL_POTION_ITEM ) || defined (SKILL_POTION_ITEM)

				case 2582:
					if (ch->m_assist.FindByItemIndex(2583))
					{
						// 훈련 주문서(M)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2583);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2139))
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2139);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2140))
					{
						// 플래티늄 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2140);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if (ch->m_assist.FindByItemIndex(4937))
					{
						// 이벤트용 플래티늄 훈련 주문서 와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4937);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 2583:
					if (ch->m_assist.FindByItemIndex(2582))
					{
						// 훈련 주문서(L)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2582);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2139))
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2139);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2140))
					{
						// 플래티늄 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2140);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if (ch->m_assist.FindByItemIndex(4937))
					{
						// 이벤트용 플래티늄 훈련 주문서 와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4937);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 2139:		// 훈련 주문서
					if (ch->m_assist.FindByItemIndex(2582))
					{
						// 훈련 주문서(L)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2582);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2583))
					{
						// 훈련 주문서(M)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2583);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2140))
					{
						// 플래티늄 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2140);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if (ch->m_assist.FindByItemIndex(4937))
					{
						// 이벤트용 플래티늄 훈련 주문서 와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4937);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 2140:		// 플래티늄 훈련 주문서
					if (ch->m_assist.FindByItemIndex(2582))
					{
						// 훈련 주문서(L)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2582);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2583))
					{
						// 훈련 주문서(M)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2583);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2139))
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2139);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if (ch->m_assist.FindByItemIndex(4937))
					{
						// 이벤트용 플래티늄 훈련 주문서 와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4937);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 4937:		// (이벤트용) 플래티늄 훈련 주문서
					if (ch->m_assist.FindByItemIndex(2582))
					{
						// 훈련 주문서(L)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2582);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2583))
					{
						// 훈련 주문서(M)와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2583);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(2139))
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2139);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					else if (ch->m_assist.FindByItemIndex(2140))
					{
						// 플래티늄 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2140);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 836:		// 수박
					// 경험치 증폭제는 수박과 불가
					if (ch->m_assist.FindByItemIndex(882))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 882);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 유료 경험치 증폭제는 수박과 불가
					if (ch->m_assist.FindByItemIndex(6094))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6094);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4939);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 837:		// 참외
					// SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(883))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 883);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 유료 SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(6095))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6095);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4939);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 달콤한 참외
					if (ch->m_assist.FindByItemIndex(5082))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5082);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 838:		// 자두
					// 아이템 증폭제는 자두와 불가
					if (ch->m_assist.FindByItemIndex(884))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 884);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 유료 아이템 증폭제는 자두와 불가
					if (ch->m_assist.FindByItemIndex(6096))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6096);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 드롭 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 4939);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					//강운의 스크롤
					if (ch->m_assist.FindByItemIndex(5080))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5080);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					//복운의 스크롤
					if (ch->m_assist.FindByItemIndex(5081))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5081);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 882:		// 경험치
				case 6094:
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->getItemIndex()))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), itemproto->getItemIndex());
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 경험치 증폭제는 수박과 불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 836);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 경험치 증폭제는 유료 경험치 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(6094))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6094);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 유료 경험치 증폭제는 경험치 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(882))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 882);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 883:		// SP
				case 6095:
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->getItemIndex()))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), itemproto->getItemIndex());
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(837))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 837);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// SP 증폭제는 유료 SP 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(6095))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6095);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 유료 SP 증폭제는 SP 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(883))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 883);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 달콤한 참외
					if (ch->m_assist.FindByItemIndex(5082))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5082);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 884:		// 아이템
				case 6096:
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->getItemIndex()))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), itemproto->getItemIndex());
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					if (ch->m_assist.FindByItemIndex(838))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 838);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 유료 증폭제와 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(884))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 884);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if (ch->m_assist.FindByItemIndex(6096))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 6096);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 드랍 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					//강운의 스크롤
					if (ch->m_assist.FindByItemIndex(5080))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5080);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					//복운의 스크롤
					if (ch->m_assist.FindByItemIndex(5081))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5081);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 885:		// 나스
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->getItemIndex()))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), itemproto->getItemIndex());
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 972:
				case 1629:
					// 플래티늄 드랍 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 4939:		// 랜덤 EXP&SP&드롭률 증폭제
					{
						// 증폭제는 중복 복용 불가
						if (ch->m_assist.FindByItemIndex(itemproto->getItemIndex()))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), itemproto->getItemIndex());
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
						// 수박과 불가
						if (ch->m_assist.FindByItemIndex(836))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 836);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
						// 참외와 불가
						if (ch->m_assist.FindByItemIndex(837))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 837);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}

						//자두와 불가
						if (ch->m_assist.FindByItemIndex(838))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 838);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}

						// 플래티늄 드랍 증폭제와 불가
						if (ch->m_assist.FindByItemIndex(2855))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}

						// 플래티늄 행운의 스크롤과 불가
						if (ch->m_assist.FindByItemIndex(2856))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;

				case 1288:		// 아이리스의 축복
				case 1416:		// 플래티늄 아이리스의 축복
				case 2658:		// 초보자용 플래티늄 아이리스의 축복
				case 2032:		// 럭키 아이리스의 축복
					if( ch->m_assist.FindBySkillIndex(348) )
					{
						if( !packet->extra_1 )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPUP, packet->tab, packet->invenIndex, packet->virtualIndex);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;

				case 3218:
					if( ch->m_assist.FindByItemIndex(3218) )
					{
						return;
					}
					break;

				case 4912:
					if( ch->m_assist.GetAssistCurseCount() <= 0 )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 5080:
					// 복운의 스크롤
					if (ch->m_assist.FindByItemIndex(5081))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5081);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					//자두와 불가
					if (ch->m_assist.FindByItemIndex(838))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 838);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}	// 플래티늄 드롭 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 836);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 5081: // 복운의 스크롤
					// 강운의 스크롤
					if (ch->m_assist.FindByItemIndex(5080))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 5080);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					//자두와 불가
					if (ch->m_assist.FindByItemIndex(838))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 838);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// 플래티늄 드롭 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2855);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2856);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 836);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 5082: // 달콤한 참외

					//자두와 불가
					if (ch->m_assist.FindByItemIndex(837))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 837);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					// SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(883))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 883);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					// 랜덤 EXP&SP&드롭률 증폭제와 사용불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 836);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 5085: // 플래티늄 아리리스의 축복 lv1
					if( ch->m_level <1 || ch->m_level > 30)
						return;

					if( ch->m_assist.FindBySkillIndex(348)
//						|| ch->m_assist.FindBySkillIndex(5086)
//						|| ch->m_assist.FindBySkillIndex(5087)
					  )
					{
						if( !packet->extra_1 )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPUP, packet->tab, packet->invenIndex, packet->virtualIndex);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;
				case 5086: // 플래티늄 아이리스의 축복 lv31
					if( ch->m_level <31 || ch->m_level > 60)
						return;

					if( ch->m_assist.FindBySkillIndex(348)
//						|| ch->m_assist.FindBySkillIndex(5085)
//						|| ch->m_assist.FindBySkillIndex(5087)
					  )
					{
						if( !packet->extra_1 )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPUP, packet->tab, packet->invenIndex, packet->virtualIndex);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;
				case 5087: // 플래티늄 아이리스의 축복 lv61
					if( ch->m_level <61 || ch->m_level > 90)
						return;

					if( ch->m_assist.FindBySkillIndex(348)
//						|| ch->m_assist.FindBySkillIndex(5085)
//						|| ch->m_assist.FindBySkillIndex(5086)
					  )
					{
						if( !packet->extra_1 )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPUP, packet->tab, packet->invenIndex, packet->virtualIndex);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					break;
				case 5088: // 플래티늄 숙련의 묘약 lv1
					if( ch->m_level <1 || ch->m_level > 30)
						return;

					if (ch->m_assist.FindByItemIndex(2452)
//						|| ch->m_assist.FindByItemIndex(5089)
//						|| ch->m_assist.FindByItemIndex(5090)
					   )
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2452);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				case 5089: // 플래티늄 숙련의 묘약 lv31
					if( ch->m_level <31 || ch->m_level > 60)
						return;

					if (ch->m_assist.FindByItemIndex(2452)
//						|| ch->m_assist.FindByItemIndex(5088)
//						|| ch->m_assist.FindByItemIndex(5090)
					   )
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2452);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					break;
				case 5090: // 플래티늄 숙련의 묘약 lv61
					if( ch->m_level <61 || ch->m_level > 90)
						return;

					if (ch->m_assist.FindByItemIndex(2452)
//						|| ch->m_assist.FindByItemIndex(5088)
//						|| ch->m_assist.FindByItemIndex(5089)
					   )
					{
						// 훈련 주문서와 중복 불가
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2452);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				}

				// 경험의 묘약은 아이리스를 사용 중일때 불가능
				if (itemproto->getItemNum0() == 348)
				{
					if (ch->m_assist.FindBySkillIndex(349))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 1288);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}

				if( itemproto->getItemIndex() >= 2482 && itemproto->getItemIndex() <= 2487 )
				{
					if( do_useEventItem_HalloweenCandy( ch, item ) )
					{
						goto SKIP_SKILL;
					}
					else
						return;
				}

#ifdef REFORM_PK_PENALTY_201108
				switch( itemproto->getItemIndex() )
				{
				case 7474:
				case 7475:
				case 7476:
					if (ch->m_assist.FindByItemIndex(7474) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7474);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if(  ch->m_assist.FindByItemIndex(7475) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7475);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if ( ch->m_assist.FindByItemIndex(7476) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7476);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;

				case 7477:
				case 7478:
				case 7479:
					if (ch->m_assist.FindByItemIndex(7477) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7477);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if(  ch->m_assist.FindByItemIndex(7478) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7478);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					if ( ch->m_assist.FindByItemIndex(7479) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 7479);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					break;
				}
#endif

				CSkill* skill = NULL;

				// 포션중에 IPOTION_UP 은 아이템 플래그에서 스킬레벨을 가져온다
				if (itemproto->getItemSubTypeIdx() == IPOTION_UP)
					skill = gserver->m_skillProtoList.Create(itemproto->getItemNum0(), item->getFlag());
				else
					skill = gserver->m_skillProtoList.Create(itemproto->getItemNum0(), itemproto->getItemNum1());

				if (skill == NULL)
					return;

				// 트루시잉 코덱스와 상급 트루시잉 코덱스
				if (itemproto->getItemIndex() == 677 || itemproto->getItemIndex() == 3579)
				{
					if (ch->IsInPeaceZone(false) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						delete skill;
						rmsg->Init(MSG_EXTEND);
						RefMsg(rmsg) << MSG_EX_STRING
									 << (unsigned char) MSG_EX_STRING_OUTPUT_SYS
									 << 393
									 << 0;

						SEND_Q(rmsg, ch->m_desc);
						return;
					}
					ApplyItem677_or_3579(ch, skill, itemproto->getItemIndex());
				}
				else
				{
					// 060227 : bs : 중복안되는 스킬인지 검사
					if ((skill->m_proto->m_flag & SF_NOTDUPLICATE) && ch->m_assist.FindBySkillIndex(skill->m_proto->m_index))
					{
						/*중복사용 불가 에러 메시지*/
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_DUPLICATE, skill->m_proto->m_index, 0);
						SEND_Q(rmsg, ch->m_desc);
						delete skill;
						return;
					}
					// 확장 포션 : 듀얼 사용중일때 따로 따로 수행 못함
					if (( skill->m_proto->m_index == 350 || skill->m_proto->m_index == 351 ) && ch->m_assist.FindBySkillIndex(352) )
					{
						delete skill;
						return;
					}
					bool bApply = false;
					if( skill->m_proto->CheckSorcererFlag(SSF_APP_APET) )
					{
						if( ch->GetAPet() == NULL )
						{
							delete skill;
							return;
						}
						ApplySkill(ch, ch->GetAPet() , skill, itemproto->getItemIndex(), bApply);
					}
					else
					{
						// 여기서 스킬의 Target Type을 확인하고..파티나 원정대에게도 넣어준다.
						if(skill->m_proto->m_targetType == STT_PARTY_ALL)
						{
							if (ch->IsExped())
							{
								ApplySkillExped(ch, skill, itemproto->getItemIndex(), bApply);
							}
							else if(ch->IsParty())
							{
								ApplySkillParty(ch, skill, itemproto->getItemIndex(), bApply);
							}
						}
						else
						{
							ApplySkill(ch, ch, skill, itemproto->getItemIndex(), bApply);
						}
					}

					if (!bApply)
					{
						delete skill;
						return;
					}

					if( itemproto->getItemIndex() == 846 ||	// 부활 주문서
							itemproto->getItemIndex() == 7456 ||	// 이벤트 부활 주문서
							itemproto->getItemIndex() == 2667      // 초보자용 부활 주문서
					  )
					{
						// 숙련도 / 경험치 주문서 버프 삭제
						ch->m_assist.CureByItemIndex(844);
						ch->m_assist.CureByItemIndex(845);
						ch->m_assist.CureByItemIndex(2035);
						ch->m_assist.CureByItemIndex(2036);
					}
				}
				delete skill;
			}

			{
				// 포션 사용 이펙트 메시지
				CNetMsg::SP rmsg(new CNetMsg);
				EffectItemMsg(rmsg, ch, item);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		break;

	case ITYPE_ETC:
		// 트리거 아이템 사용
		if ((itemproto->getItemFlag() & ITEM_FLAG_TRIGGER))
		{
			switch(itemproto->getItemIndex())
			{
			case 4567:
				// 강화 에스트럴라이트는 사라지지 않는다.
				do_ItemUse_IETC_TriggerItem4567(ch);
				return;
				break;
			default:
				return;
				break;
			}
		}
		else

			if( itemproto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD )
			{
				do_ItemUse_IETC_MonsterMercearyToggle(ch, item);
				return;
			}

		if ( itemproto->getItemSubTypeIdx() == IETC_SKILLUP )
		{
			if (!do_ItemUse_IETC_SKILLUP(ch, item))
				return;
		}

		if (itemproto->getItemSubTypeIdx() == IETC_PRODUCT)
		{
			if (!do_ItemUse_IETC_PRODUCT(ch, itemproto))
				return;
		}

		else
			return;

		break;

	default:
		return;
	}

#ifdef DEV_LETS_PARTYTIME
	if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_LETSPARTY ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemPartyTypeUseMsg( rmsg, item->getDBIndex(), ch->m_nick );
		if( ch->IsParty() )
		{
			// 파티에 메세지 전송
			ch->m_party->SendToAllPC( rmsg );
		}
		else if ( ch->IsExped() )
		{
			// 원정대에 메세지 전송
			ch->m_Exped->SendToAllPC( rmsg );
		}
		else
		{
			SEND_Q(rmsg, ch->m_desc);
		}
	}
#endif // DEV_LETS_PARTYTIME

SKIP_SKILL:
	if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_CASH) )
	{
		GAMELOG << init("CASH_ITEM_USE", ch)
				<< itemlog(item) << delim
				<< end;
	}

	{
		// 아이템 사용하고
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemUseMsg(rmsg, packet->tab, packet->invenIndex, packet->virtualIndex, packet->extra_1);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 피닉스 부활 아이템은 삭제하지 않는다.
	if ( itemproto->getItemIndex() == 3218 )
	{
		return;
	}

	if(itemproto->getItemFlag() & ITEM_FLAG_QUEST )
	{
		CQuest* pQuest = ch->m_questList.FindQuestByItem(itemproto->getItemIndex());
		if( pQuest )
		{
			if( ch->m_pZone->m_index == itemproto->getItemQuestZone()
					&& ( GetDistance(itemproto->getItemQuestX(), GET_X(ch), itemproto->getItemQuestZ(), GET_Z(ch), 0, 0) <= itemproto->getItemQuestRange() ))
			{
				for( int i=0; i<QUEST_MAX_CONDITION; i++)
				{
					if( pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_USE &&
							pQuest->GetQuestProto()->m_conditionIndex[i] == itemproto->getItemIndex() )
					{
						if( pQuest->GetQuestValue(i) < pQuest->GetQuestProto()->m_conditionNum[i] )
						{
							pQuest->IncreaseQuestValue(i);
							pQuest->CheckComplete(ch);
							CNetMsg::SP rmsg(new CNetMsg);
							QuestStatusMsg(rmsg, pQuest);
							SEND_Q(rmsg, ch->m_desc);
						}
					}
				}
			}
		}
	}

	//토글 아이템은 토글 동작만 시키고 아이템 수량 변경을 하지 않는다.
	if(item->m_itemProto->getItemFlag() & ITEM_FLAG_TOGGLE)
	{
		ch->changeToggleState(item->getVIndex(), TOGGLE_ITEM);
		return;
	}
	
	// Item 수량 변경
	ch->m_inventory.decreaseItemCount(item, 1);
}

void do_ItemTake(CPC* ch, CNetMsg::SP& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;

	RequestClient::doItemTake* packet = reinterpret_cast<RequestClient::doItemTake*>(msg->m_buf);

	CCharacter* takeChar;

	switch (packet->char_type)
	{
	case MSG_CHAR_PC:
		{
			if (ch->m_index != packet->char_index)
				return ;

			takeChar = ch;
		}
		break;

	case MSG_CHAR_PET:
		{
			if (!ch->GetPet() || !ch->GetPet()->IsSummon() || ch->GetPet()->m_index != packet->char_index || ch->GetPet()->GetOwner() != ch)
				return ;

			takeChar = ch->GetPet();
		}
		break;

	case MSG_CHAR_APET:
		if(!ch->GetAPet() || !ch->GetAPet()->IsSummon() || ch->GetAPet()->m_index != packet->char_index || ch->GetAPet()->GetOwner() != ch)
			return;
		takeChar = ch->GetAPet();
		break;

	default:
		return ;
	}

	int sx = takeChar->m_cellX - 1;
	int ex = takeChar->m_cellX + 1;
	int sz = takeChar->m_cellZ - 1;
	int ez = takeChar->m_cellZ + 1;

	int x, z;
	int deleteVIndex;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= takeChar->m_pArea->m_size[0])
			continue ;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= takeChar->m_pArea->m_size[1])
				continue ;
			CItem* item;
			CItem* itemNext = takeChar->m_pArea->m_cell[x][z].m_listItem;
			while ((item = itemNext))
			{
				itemNext = item->m_pCellNext;
				deleteVIndex = item->getVIndex();

				if (ABS(GET_YLAYER(takeChar) - GET_YLAYER(item)) > 1)
					continue ;

				if (item->getVIndex() == packet->virtualIndex)
				{
					if(item->m_itemProto->getItemIndex() == NAS_ITEM_DB_INDEX)
						return;

					if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_TRIGGER) && ch->IsExped() )
					{
						if( GetItemGiveToBoss( ch, item ) )
							return;
					}

					if (item->getDBIndex() == RECOMMEND_SERVER_POTION_INDEX)
					{
						do_ItemTake_RecommendPotion(ch, item);
						return ;
					}

					if( gserver->isActiveEvent(A_EVENT_WHITE_DAY) )
					{
						bool bCantWDItem = false;
						if( item->getDBIndex() == 2133 ) //하늘색 사탕 타이탄 나이트 소서러만 획득 가능
						{
							if( ch->m_job == JOB_TITAN || ch->m_job == JOB_KNIGHT || ch->m_job == JOB_SORCERER || ch->m_job == JOB_NIGHTSHADOW )
							{
								bCantWDItem = true;
							}
						}

						if( item->getDBIndex() == 2134 ) //분홍색 사탕 힐러 메이지 로그 획득 가능
						{
#ifdef EX_ROGUE
#ifdef EX_MAGE
							if( ch->m_job == JOB_HEALER || ch->m_job == JOB_MAGE || ch->m_job == JOB_ROGUE || ch->m_job == JOB_EX_ROGUE || ch->m_job == JOB_EX_MAGE )
#else // EX_MAGE
							if( ch->m_job == JOB_HEALER || ch->m_job == JOB_MAGE || ch->m_job == JOB_ROGUE || ch->m_job == JOB_EX_ROGUE )
#endif
#else
							if( ch->m_job == JOB_HEALER || ch->m_job == JOB_MAGE || ch->m_job == JOB_ROGUE )
#endif // EX_ROGUE
							{
								bCantWDItem = true;
							}
						}

						if( item->getDBIndex() == 2133 || item->getDBIndex() == 2134 )
						{
							if( !bCantWDItem )
							{
								CNetMsg::SP rmsg(new CNetMsg);
								SysMsg(rmsg, MSG_SYS_MATCHSEX );
								SEND_Q(rmsg, ch->m_desc);
								return;
							}
						}
					}

					if( gserver->isActiveEvent( A_EVENT_COLLECT_BUG) )
					{
						switch (item->getDBIndex())
						{
						case 1579:
						case 1580:
						case 1581:
						case 1582:
						case 1583:
						case 1584:
						case 1585:
						case 1586:
						case 1587:
						case 1588:
							do_ItemTake_EventCollectBug(ch, item);
							return ;

						default:
							break;
						}
					}


					switch (item->getDBIndex())
					{
					// 크리스 마스 선물 중복 습득 불가
					case 2590:
					case 2591:
					case 2592:
					case 2593:
					case 2594:
					case 2595:
					case 2596:
						{
							if( gserver->isActiveEvent( A_EVENT_XMAS ) )
							{
								if (ch->m_inventory.FindByDBIndex(item->getDBIndex()))
								{
									CNetMsg::SP rmsg(new CNetMsg);
									SysMsg(rmsg, MSG_SYS_CANNOT_HAVE_DUPLICATE);	// 아이템을 중복해서 가질 수 없습니다.
									SEND_Q(rmsg, ch->m_desc);
									return;
								}
							}
							else
							{
								// 이벤트 중이라면 집지도 못하도록 하자
								return;
							}
						}
						break;
					}

					// 균등파티이거나 새파티이면 아이템 랜덤 먹기
					if (ch->IsParty() && ( ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == MSG_PARTY_TYPE_RANDOM || ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == MSG_PARTY_TYPE_BATTLE) )
					{
						if (GetItemRandomParty(ch, item))
							return;
					}

					// 파티 레이드 스페셜 - 상자 열기
					if(ch->IsParty() && ( ch->m_party->GetPartyType(MSG_DIVITYPE_SPECIAL) == MSG_PARTY_TYPE_OPENBOX ))
					{
						if(item->getDBIndex() == 4709)
						{
							GetItemRaidOpenBox(ch, item);
							return;
						}
					}

					// 균등 원정대 아이템 랜덤 먹기

					if(ch->IsExped() && ( ch->m_Exped->GetExpedType(MSG_DIVITYPE_ITEM) == MSG_EXPED_TYPE_RANDOM || ch->m_Exped->GetExpedType(MSG_DIVITYPE_ITEM) == MSG_EXPED_TYPE_BATTLE) )
					{
						GetItemRandomExped(ch, item);
						return;
					}

					// 원정대 레이드 스페셜 - 상자 열기
					if(ch->IsExped() && ( ch->m_Exped->GetExpedType(MSG_DIVITYPE_SPECIAL) == MSG_EXPED_TYPE_OPENBOX ))
					{
						if(item->getDBIndex() == 4709)
						{
							GetItemRaidOpenBox(ch, item);
							return;
						}
					}

					// 아이템 소유권 변경: 050413
					if (item->m_preferenceIndex != -1 && item->m_preferenceIndex != ch->m_index)
					{
						// 입수우선 파티
						if (ch->IsParty() && ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == MSG_PARTY_TYPE_FIRSTGET)
						{
							CParty* party = ch->m_party;
							if (!party)
								return;

							int i;
							// 소유권자가 파티에 속해있다
							bool bParty = false;
							for (i=0; i < MAX_PARTY_MEMBER; i++)
							{
								if(item->m_itemProto->getItemFlag() & ITEM_FLAG_NO_STASH)					// ITEM_FLAG_NO_STASH인 경우 다른 파티원이 획득하지 못함.
									break;
								int nMemberIndex = party->GetMemberCharIndex(i);
								if (nMemberIndex != -1 && nMemberIndex == item->m_preferenceIndex)
								{
									bParty = true;
									break;
								}
							}

							if (!bParty)
							{
								// 우선권이 없습니다 메시지 전송
								CNetMsg::SP rmsg(new CNetMsg);
								SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
								SEND_Q(rmsg, ch->m_desc);
								return;
							}
						}
						//임수우선 원정대(아이템)
						else if (ch->IsExped() && ch->m_Exped->GetExpedType(MSG_DIVITYPE_ITEM) == MSG_EXPED_TYPE_FIRSTGET)
						{
							CExpedition* Exped = ch->m_Exped;
							if (!Exped)
								return;

							int i,j;
							// 소유권자가 원정대에 속해있다
							bool bExped = false;
							for (i=0; i < MAX_EXPED_GROUP; i++)
							{
								if(item->m_itemProto->getItemFlag() & ITEM_FLAG_NO_STASH)					// ITEM_FLAG_NO_STASH인 경우 다른 파티원이 획득하지 못함.
									break;
								for (j=0; j < MAX_EXPED_GMEMBER; j++)
								{
									int nMemberIndex = Exped->GetMemberCharIndex(i,j);
									if (nMemberIndex != -1 && nMemberIndex == item->m_preferenceIndex)
									{
										bExped = true;
										break;
									}
								}
							}

							if (!bExped)
							{
								// 우선권이 없습니다 메시지 전송
								CNetMsg::SP rmsg(new CNetMsg);
								SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
								SEND_Q(rmsg, ch->m_desc);
								return;
							}
						}
						else
						{
							// 우선권이 없습니다 메시지 전송
							CNetMsg::SP rmsg(new CNetMsg);
							SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}

					CNetMsg::SP disappmsg(new CNetMsg);
					CNetMsg::SP takemsg(new CNetMsg);
					CNetMsg::SP itemmsg(new CNetMsg);

					int cx, cz;

					cx = item->m_cellX;
					cz = item->m_cellZ;

					ItemTakeMsg(takemsg, takeChar, item);
					ItemDisappearMsg(disappmsg, deleteVIndex);

#ifdef LACARETTE_SYSTEM
					//if(item->m_idNum == 5123)	//일반토큰
					//{
					//	if(item->Count() > 100)
					//	{
					//		CNetMsg rmsg;
					//		//소지제한 오버
					//		lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_TOKEN_OVER);
					//		SEND_Q(rmsg, ch->m_desc);
					//		return ;
					//	}
					//	int r, c;
					//	if(inven->FindItem(&r, &c, item->m_idNum, -1, -1))
					//	{
					//		CItem *pItem;
					//		pItem = inven->GetItem(r, c);
					//		if(pItem != NULL)
					//		{
					//			if(pItem->Count() >= 100)	// 100개 소지 제한.
					//			{
					//				CNetMsg rmsg;
					//				// 소지제한 오버.
					//				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_TOKEN_OVER);
					//				SEND_Q(rmsg, ch->m_desc);
					//				return ;
					//			}
					//			if (pItem->Count() + item->Count() > 100)
					//			{
					//				CNetMsg rmsg;
					//				// 소지제한 오버.
					//				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_TOKEN_OVER);
					//				SEND_Q(rmsg, ch->m_desc);
					//				return ;
					//			}
					//		}
					//	}
					//}
#endif

#ifndef EVENT_EGGS_HUNT_BILA	// Bila 부활절 설정
					if( gserver->isActiveEvent( A_EVENT_EGGS_HUNT) && item->getDBIndex() == 2148)
					{
						if (ch->m_inventory.FindByDBIndex(item->getDBIndex()))
						{
							// EGG 아이템 이미 소유하고 있음
							CNetMsg::SP rmsg(new CNetMsg);
							EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST);
							SEND_Q(rmsg, ch->m_desc);
							return ;
						}
					}
#endif // EVENT_EGGS_HUNT_BILA
					//줓은 아이템이 유물 아이템이라고 하면
					int bArtifact = false;

					if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
					{
						bArtifact = true;
						int error_ = ArtifactManager::instance()->checkValidUser(ch);
						if( error_ != 0)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysMsg(rmsg, (MSG_SYS_TYPE)error_);
							SEND_Q(rmsg, ch->m_desc);
							return ;
						}
					}

					// 인벤에 넣기
					if (ch->m_inventory.addItem(item) == false)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysFullInventoryMsg(rmsg, 0);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}

					//줓은 아이템이 유물 아이템이라고 하면
					if(bArtifact == true)
					{
						ArtifactManager::instance()->addOwner(ch, item, true);
					}

					// 우선권 제거
					item->m_preferenceIndex = -1;

					// 땅에서 제거
					takeChar->m_pArea->ItemFromCell(item);

					// 메시지 보내고
					takeChar->m_pArea->SendToCell(takemsg, takeChar, true);
					SEND_Q(itemmsg, ch->m_desc);
					takeChar->m_pArea->SendToCell(disappmsg, GET_YLAYER(takeChar), cx, cz);

					if (IS_PC(takeChar))
					{
						CPC* pPC = TO_PC(takeChar);
						if (pPC && pPC->IsParty())
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ItemGetMsg(rmsg, pPC, item);
							pPC->m_party->SendToPCInSameZone(pPC->m_pZone->m_index, pPC->m_pArea->m_index, rmsg);
						}
					}
#ifdef GER_LOG
					GAMELOGGEM << init( 0, "CHAR_LOOT")
							   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
							   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
							   << LOG_VAL("item-id", item->getDBIndex() ) << blank
							   << LOG_VAL("amount",item->Count() ) << blank
							   //<< LOG_VAL("amount", money ) << blank
							   <<endGer;
#endif // GER_LOG
					// LOG
					itemPickGameLog(ch, item, packet->char_type);

					return ;
				}
			}
		}
	}
}

//입기
void do_ItemWear(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemWear* packet = reinterpret_cast<RequestClient::doItemWear*>(msg->m_buf);

	//강신 상태일때는 펫을 제외한 모든 아이템 입기 벗기를 할 수 없다.
	if(ch->m_evocationIndex != EVOCATION_NONE && packet->wearPos != WEARING_PET)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	if (packet->wearPos < 0 || packet->wearPos >= MAX_WEARING)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemWearMsg(rmsg, ResponseClient::WEAR_ERR_INVALID_POS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);	// 입는 아이템

	// 입기
	if( ch->m_wearInventory.AddNormalItem(item, packet->wearPos) == false )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemWearMsg(rmsg, ch->m_wearInventory.GetLastError());
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	switch (packet->wearPos)
	{
	case WEARING_HELMET:
		{
			char cOldHairStyle = ch->m_hairstyle;
			ch->m_hairstyle = ch->m_hairstyle % 10;
			if (ch->m_wearInventory.wearItemInfo[WEARING_HELMET])
			{
				switch (ch->m_wearInventory.wearItemInfo[WEARING_HELMET]->m_itemProto->getItemIndex())
				{
				case 1843:			// 빨강 산타 모자 : HAIR_RED_CAP + n
					ch->m_hairstyle += HAIR_RED_CAP;
					break;

				case 1844:			// 녹색 산타 모자 : HAIR_GREEN_CAP + n
					ch->m_hairstyle += HAIR_GREEN_CAP;
					break;
				}
			}
			if (cOldHairStyle != ch->m_hairstyle)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExHairChangeMsg( rmsg, ch );
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		break;

	case WEARING_ACCESSORY1:
	case WEARING_ACCESSORY2:
	case WEARING_ACCESSORY3:
		{
			int nOldState = ch->GetPlayerState();
			ch->ResetPlayerState(PLAYER_STATE_SUPPORTER);
			int i;
			for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
			{
				if (ch->m_wearInventory.wearItemInfo[i])
				{
					// 서포터 아이템
					if (ch->m_wearInventory.wearItemInfo[i]->m_itemProto->getItemIndex() == 1912)
						ch->SetPlayerState(PLAYER_STATE_SUPPORTER);
				}
			}
			if (nOldState != ch->GetPlayerState())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPlayerStateChangeMsg(rmsg, ch);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		break;
	}
}

void do_ItemWearTakeOff(CPC* ch, CNetMsg::SP& msg)
{
	//벗기
	RequestClient::doItemWearTakeOff* packet = reinterpret_cast<RequestClient::doItemWearTakeOff*>(msg->m_buf);

	if(ch->m_evocationIndex != EVOCATION_NONE && packet->wearPos != WEARING_PET)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	if (packet->wearPos < 0 || packet->wearPos >= MAX_WEARING)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemTakeoffMsg(rmsg, ResponseClient::WEAR_ERR_CANNOT_TAKEOFF);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* item = ch->m_wearInventory.wearItemInfo[(int)packet->wearPos];		// 벗는 아이템

	if (ch->m_wearInventory.DelNormalItem(packet->wearPos, packet->tab, packet->invenIndex) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemTakeoffMsg(rmsg, ch->m_wearInventory.GetLastError());
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_ItemSwap(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemSwap* packet = reinterpret_cast<RequestClient::doItemSwap*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->stab, packet->sinvenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->stab, packet->sinvenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->ttab, packet->tinvenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->ttab, packet->tinvenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isUsable(packet->ttab) == false)
	{
		LOG_INFO("inventory is expire. char_index[%d] tab[%d]", ch->m_index, packet->ttab);
		return;
	}

	if (packet->stab == packet->ttab && packet->sinvenIndex == packet->tinvenIndex)
	{
		ch->m_bChatMonitor = true;
		GAMELOG << init("DEBUG SWAP", ch)
				<< ch->m_desc->getHostString()
				<< end;

		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	CItem* sItem = ch->m_inventory.getItem(packet->stab, packet->sinvenIndex);
	CItem* tItem = ch->m_inventory.getItem(packet->ttab, packet->tinvenIndex);

	if (sItem == NULL)
	{
		LOG_ERROR("HACKING? : source item is NULL. char_index[%d]", ch->m_index);
		ch->m_desc->Close("source item is NULL");
		return;
	}

	// 작업 시작
	if (tItem == NULL) // 목적지가 비어있을때는 무조건 교환
	{
		ch->m_inventory.swapItem(packet->stab, packet->sinvenIndex, packet->ttab, packet->tinvenIndex);
	}
	else
	{
		// 두개의 아이템(DB Index)이 서로 같고 겹치기를 할수 있는 놈이면...
		if (sItem->getDBIndex() == tItem->getDBIndex() && (sItem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
				&& (sItem->getPlus() == tItem->getPlus()) && (sItem->getFlag() == tItem->getFlag()))
		{
			LONGLONG target_sum_count = sItem->getItemCount() + tItem->getItemCount();
			if (target_sum_count <= sItem->m_itemProto->getStackCount()) // 겹칠 수 있다.
			{
				ch->m_inventory.decreaseItemCount(sItem, sItem->Count());

				tItem->setItemCount(target_sum_count);

				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeUpdateItemCountWithSwap(rmsg, tItem->tab(), tItem->getInvenIndex(), tItem->Count());
				SEND_Q(rmsg, ch->m_desc);
			}
			else // 겹칠 수 없다.
			{
				if (ch->m_inventory.isUsable(packet->stab) == false)
				{
					LOG_INFO("inventory is expire. char_index[%d] tab[%d]", ch->m_index, packet->stab);
					return;
				}

				ch->m_inventory.swapItem(packet->stab, packet->sinvenIndex, packet->ttab, packet->tinvenIndex);
			}
		}
		else
		{
			// 서로 같지 않거나, 겹치기를 할 수 없다면...
			if (ch->m_inventory.isUsable(packet->stab) == false)
			{
				LOG_INFO("inventory is expire. char_index[%d] tab[%d]", ch->m_index, packet->stab);
				return;
			}

			ch->m_inventory.swapItem(packet->stab, packet->sinvenIndex, packet->ttab, packet->tinvenIndex);
		}
	}
}

void do_ItemDelete(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemDelete* packet = reinterpret_cast<RequestClient::doItemDelete*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isUsable(packet->tab) == false)
	{
		LOG_INFO("inventory is expire. char_index[%d] tab[%d]", ch->m_index, packet->tab);
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (!item || item->getVIndex() != packet->virualIndex || item->getWearPos() != WEARING_NONE)
		return ;

	//결사대 가입 주문서(나중에 파괴불가능한 플래그 도입이 필요. flag2 확장할때 체크할 것, flag : 파괴 불가능한 아이템)
	if(item->getDBIndex() == 10027 || item->getDBIndex() == 10028)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_DESTROY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_admin != 10)
	{
		// GM들은 모든 아이템을 다 삭제할 수 있음
		if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_QUEST) ||
				(item->m_itemProto->getItemTypeIdx() == ITYPE_ETC && item->m_itemProto->getItemSubTypeIdx() == IETC_QUEST))
			return;
	}

	if( item->IsPet() && ch->m_petStashManager.GetPetItemByVIndex(item->getVIndex()) == NULL )
	{
		ch->DelPet(item->getPlus());
		std::string delete_query = boost::str(boost::format("delete from t_pet where a_owner = %d and a_index = %d") % ch->m_index % item->getPlus());
		DBManager::instance()->pushQuery(0, delete_query);
		return ;
	}

	if( item->IsAPet() && ch->m_petStashManager.GetPetItemByVIndex(item->getVIndex()) == NULL )
	{
		ch->DelAPet(item->getPlus());
		return ;
	}

	if(item->getFlag() & FLAG_ITEM_COMPOSITION)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_DESTROY);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ITEM CANT DELETE", ch)
				<< itemlog(item) << delim
				<< "ITEM WAS COMPOSITED" << end;
		return ;
	}

	if( item->m_itemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_DESTROY);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ITEM CANT DELETE", ch)
				<< itemlog(item) << delim
				<< "ITEM WAS MONSTER_MERCENARY" << end;
		return ;
	}

	// Log
	GAMELOG << init("ITEM_DELETE", ch)
			<< itemlog(item)
			<< end;

	ch->m_inventory.deleteItemByItem(item);
}

void do_ItemThrow(CPC* ch, CNetMsg::SP& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;

	RequestClient::doItemThrow* packet = reinterpret_cast<RequestClient::doItemThrow*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isUsable(packet->tab) == false)
	{
		LOG_INFO("inventory is expire. char_index[%d] tab[%d]", ch->m_index, packet->tab);
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);

	if(item == NULL)
	{
		LOG_ERROR("HACKING? : invalid packet item is null. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet item is null");
		return;
	}

	if (item->getVIndex() != packet->virtualIndex)
	{
		LOG_ERROR("HACKING? : invalid packet virtualIndex. char_index[%d], item_vIndex[%d], packet_vIndex[%d] ",
			ch->m_index, item->getVIndex(), packet->virtualIndex);
		ch->m_desc->Close("invalid packet virtualIndex");
		return;
	}

	if(item->Count() < packet->count)
	{
		LOG_ERROR("HACKING? : invalid packet item count. char_index[%d], item_count[%d], packet_count[%d] ",
			ch->m_index, item->Count(), packet->count);
		ch->m_desc->Close("invalid packet item count");
		return;
	}

#ifdef ENABLE_SUBJOB
	if( item->CanUseTrader( TRADE_DROP, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
	{
		return;
	}
#endif //ENABLE_SUBJOB
	
	if(item->IsBelong() == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	if(item->CanDrop())
	{
		return;
	}
	
	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		ArtifactManager::instance()->item_drop(item);
	}

	CItem* dropitem = NULL;

	// 버리는 개수와 소지 개수가 다르면
	if (item->Count() != packet->count)
	{
		// 복사해서 count 설정
		dropitem = ch->m_pArea->DropItem(item->getDBIndex(), ch, item->getPlus(), item->getFlag(), packet->count);
		if (!dropitem)
			return ;

		// 옵션 복사
		if (!(item->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT))
		{
			int i;
			for (i=0; i < MAX_ITEM_OPTION; i++)
			{
				dropitem->m_option[i] = item->m_option[i];
				dropitem->m_nOption = item->m_nOption;
			}
		}

		ch->m_inventory.decreaseItemCount(item, packet->count);
	}
	else
	{
		dropitem = item;

		if(ch->holy_water_item == item)
		{
			ch->changeToggleState(ch->holy_water_item->getVIndex(), TOGGLE_ITEM);
			ch->SendHolyWaterStateMsg(NULL);
		}
		
		ch->m_inventory.eraseNotFree(item);

		ch->m_pArea->DropItem(dropitem, ch);
	}

	{
		// 드롭 보내기
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, dropitem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_ITEM_DROP" )
			   << LOG_VAL("account-id", ch->m_desc->m_idname) << blank
			   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name) << blank
			   << LOG_VAL("item-id", dropitem->getDBIndex() ) << blank
			   << LOG_VAL("amount", packet->count ) << blank
			   << endGer;
#endif // GER_LOG
	// Log
	GAMELOG << init("ITEM_DROP", ch)
			<< itemlog(dropitem)
			<< end;
}

// Buy & Sell 모두 PC위주로 구성
// Player가 살때 Item Buy Msg, Buy Rate 적용
void do_ItemBuy(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemBuy* packet = reinterpret_cast<RequestClient::doItemBuy*>(msg->m_buf);

	CItem**	item = NULL;

	try
	{
		// 최대 살수 있는 아이템 종류의 수
		if (packet->buyCount <= 0 || packet->buyCount > MAX_ITEM_BUY)
		{
			LOG_ERROR("HACKING? : invalid buy count[%d]. charIndex[%d]", packet->buyCount, ch->m_index);
			ch->m_desc->Close("invalid buy count");
			return;

		}

		if (packet->clientPrice <= 0)
		{
			std::string str = boost::str(boost::format(
											 "HACKING? : invalid clinetPrice[%1%]. clientIndex[%2%]") % packet->clientPrice % ch->m_index);
			LOG_ERROR(str.c_str());
			ch->m_desc->Close("invalid clientPrice");
			return;
		}

		// validation shop
		CShop* shop = ch->m_pZone->FindShop(packet->npcIndex);
		if (shop == NULL)
		{
			LOG_ERROR("HACKING? : not found shop. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
			ch->m_desc->Close("not found shop");
			return;
		}

		CNPCProto* npcProto = gserver->m_npcProtoList.FindProto(packet->npcIndex);
		if (npcProto == NULL)
		{
			LOG_ERROR("HACKING? : not found npc proto. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
			ch->m_desc->Close("not found npc proto");
			return;
		}

		if(npcProto->m_rvr_value > 0 && npcProto->m_rvr_value != ch->getSyndicateType())
		{
			LOG_ERROR("HACKING? : not equal rvr type. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
			ch->m_desc->Close("not equal rvr type");
			return;
		}

		if(npcProto->CheckFlag1(NPC1_AFFINITY))	// 상점이 있는 npc 이고, npc proto에 있고, npc가 친화도 npc 이면 캐릭터는 친화도가 있어야 한다.
		{
			int affinityIndex = 0;
			int affinityPoint = 0;
			CAffinityProto* affinityProto = gserver->m_affinityProtoList.FindProtoByNPCIdx(packet->npcIndex);
			if(affinityProto)
			{
				for(int i = 0; i < affinityProto->m_affinityNPCCount; i++)
				{
					if(affinityProto->m_affinityNPC[i].m_npcidx == packet->npcIndex)
					{
						affinityIndex = affinityProto->m_index;
						affinityPoint = affinityProto->m_affinityNPC[i].m_shoppoint;
					}
				}
				if(affinityIndex == 0)
				{
					throw 1;
				}
				else
				{
					CAffinity* affinity = ch->m_affinityList.FindAffinity(affinityIndex);
					if(!affinity)
					{
						throw 1;
					}
					else
					{
						if(affinityPoint > affinity->m_point)
						{
							throw 1;
						}
					}
				}
			}
			else
			{
				throw 1;
			}
		}

		bool	bBingoBoxDup = false;
		for (int i = 0; i < packet->buyCount; ++i)
		{
			if (packet->list[i].count < 1)
			{
				LOG_ERROR("HACKING? : invalid item count[%d]. charIndex[%d]", packet->list[i].count, ch->m_index);
				ch->m_desc->Close("invalid item count");
				return;
			}

			CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(packet->list[i].dbIndex);
			if (itemproto == NULL)
			{
				LOG_ERROR("HACKING? : not found item proto. charIndex[%d], itemdbIndex[%d]", ch->m_index, packet->list[i].dbIndex);
				ch->m_desc->Close("not found item proto");
				return;
			}

			if (itemproto->getStackCount() < packet->list[i].count)
			{
				LOG_ERROR("HACKING? : invalid item stack count. charIndex[%d] itemdbindex[%d], itemstackcount[%d], packet_itemstackcount[%d]",
						  ch->m_index, itemproto->getItemIndex(), itemproto->getStackCount(), packet->list[i].count);
				ch->m_desc->Close("invalid item stack count");
				return;
			}

			// 마법상점이고, 빙고박스를 사려고 할 때
			if ((packet->list[i].dbIndex == 2047 || packet->list[i].dbIndex == 2633) && gserver->isActiveEvent( A_EVENT_VALENTINE_DAY ))
			{
				if(ch->m_inventory.FindByDBIndex(packet->list[i].dbIndex))	// 이미 빙고 박스를 가지고 있으면
					throw 10;

				if(bBingoBoxDup)
					throw 10;

				bBingoBoxDup = true;
			}

			// 상점에 아이템이 있는지 검사 throw 6
			if (!shop->ThersIsItem(packet->list[i].dbIndex))
				throw 6;		// 상점에 아이템 없음
		}

		// 누적합계
		LONGLONG serverPrice = 0;
		item = new CItem*[packet->buyCount];
		memset(item, 0, sizeof(CItem*) * packet->buyCount);
		std::vector<possible_search_t> search_vec;

		for (int i = 0; i < packet->buyCount; ++i)
		{
			item[i] = gserver->m_itemProtoList.CreateItem(packet->list[i].dbIndex, WEARING_NONE, 0, 0, packet->list[i].count);
			if (item[i] == NULL)
			{
				LOG_ERROR("HACKING? : not found item[%d]. charIndex[%d]", packet->list[i].dbIndex, ch->m_index);
				ch->m_desc->Close("not found item");
				return;
			}

			search_vec.push_back(possible_search_t(item[i], packet->list[i].count));

			// 아이템 명성치 검사
			if (item[i]->m_itemProto->getItemFrame() != -1)
			{
				// 명성치 딸린다
				if (ch->m_fame < item[i]->m_itemProto->getItemFrame())
					throw 9;
			}

#ifdef REFORM_PK_PENALTY_SHOP_201108 // PK 패널티 리폼 :: 카오는 카오 점수만큼의 구입 금액이 비싸진다.
			LONGLONG itemPrice;
			if( ch->IsChaotic() )
			{
				itemPrice = item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100;
				itemPrice = itemPrice + ( itemPrice * ABS(ch->m_pkPenalty) / PK_HUNTER_POINT_MAX * 20 );
				itemPrice *= packet->list[i].count;
			}
			else
				itemPrice = ( item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100) * packet->list[i].count;
#else
			LONGLONG itemPrice = ( item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100) * packet->list[i].count;
#endif
			if( itemPrice <= 0 )
				throw 5;

			serverPrice += itemPrice;
		}

		// 구매한 모든 아이템을 가방에 넣을 수 있는지 검사
		if (ch->m_inventory.isPossibleAdd(search_vec) == false)
			throw 4;

		// 합계가 다르다! -->
		if (serverPrice != packet->clientPrice)
			throw 5;		// 클라이언트 데이터 오류 : 가격 틀림

		if (serverPrice <= 0)
			throw 5;

		//패시브 스킬 적용
		if(ch->m_avPassiveAddition.money_buy)
		{
			serverPrice += ch->m_avPassiveAddition.money_buy;
		}
		if(ch->m_avPassiveRate.money_buy)
		{
			serverPrice += serverPrice * (ch->m_avPassiveRate.money_buy - 100) / SKILL_RATE_UNIT;
		}

		// 돈검사
		if ((ch->m_inventory.getMoney() < serverPrice))
			throw 8;

		ch->m_inventory.decreaseMoney(serverPrice);

		for (int i = 0; i < packet->buyCount; ++i)
		{
			ch->m_inventory.addItem(item[i]);

			// Log
			CNetMsg::SP itemmsg(new CNetMsg);
#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_ITEM_BUY")
					   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
					   << LOG_VAL("vendor-id", packet->npcIndex ) << blank
					   << LOG_VAL("item-id", item[i]->getDBIndex() ) << blank
					   << LOG_VAL("amount", packet->buyCount ) << blank
					   << endGer;
#endif // GER_LOG
			GAMELOG << init("ITEM_BUY", ch)
					<< packet->npcIndex << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ch->m_inventory.getMoney()
					<< end;
		}


		STATISTICS(ItemBuy, serverPrice);

		// 세금 징수 : 구입금액의 10%
		if(ch->m_pZone->m_index == ZONE_MERAC || ch->m_pZone->m_index == ZONE_EGEHA)
			gserver->AddTaxItem(serverPrice / 10);

		if(ch->m_pZone->m_index == ZONE_START || ch->m_pZone->m_index == ZONE_DRATAN)
			gserver->AddTaxItemDratan(serverPrice / 10);
	}
	catch (const int errCode)
	{
		// 오류 발생시 만들었던 아이템 제거
		if (item)
		{
			for (int i = 0; i < packet->buyCount; ++i)
			{
				if (item[i])
					delete item[i];
			}

			delete [] item;
		}

		{
			// 오류 메시지 보내기
			CNetMsg::SP rmsg(new CNetMsg);

			switch (errCode)
			{
			case 0:			// 메시지 내의 아이템 종류 수 오류
			case 1:			// 상점이 없음
			case 2:			// DB에 없는 아이템
			case 3:			// DB에 잘못된 값
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				break;
			case 4:			// 인벤토리 가득 참
				SysFullInventoryMsg(rmsg, 0);
				break;
			case 5:			// 클라이언트 데이터 오류 : 가격 틀림
			case 6:			// 상점에 아이템 없음
			case 7:			// 돈 빼기 실패
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				break;
			case 8:			// 돈 모질람
				SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
				break;

			case 9:			// 명성 부족
				SysMsg(rmsg, MSG_SYS_SHORT_FAME);
				break;
			case 10:		// 빙고박스가 이미 있음
				SysMsg(rmsg, MSG_SYS_CANNOT_HAVE_DUPLICATE);
				break;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

// Buy & Sell 모두 PC위주로 구성
// Player가 팔때 Item Sell Msg, Sell Rate 적용
void do_ItemSell(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemSell* packet = reinterpret_cast<RequestClient::doItemSell*>(msg->m_buf);

	CItem* item[MAX_ITEM_SELL] = { NULL, };

	try
	{
		if (packet->clientPrice < 0)
		{
			std::string tstr = boost::str(boost::format(
				"HACKING? : invalid clientPice[%1%]. charIndex[%2%]") % packet->clientPrice % ch->m_index);
			LOG_ERROR(tstr.c_str());
			ch->m_desc->Close("invalid clientPice");
			return;
		}

		if (packet->sellCount > MAX_ITEM_SELL || packet->sellCount <= 0)
		{
			LOG_ERROR("HACKING? : invalid sellCount[%d]. charIndex[%d]", packet->sellCount, ch->m_index);
			ch->m_desc->Close("invalid sellCount");
			return;
		}

		// validation shop
		CShop* shop = ch->m_pZone->FindShop(packet->npcIndex);

		if (shop == NULL)
		{
			LOG_ERROR("HACKING? : not found shop. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
			ch->m_desc->Close("not found shop");
			return;
		}

		{
			CNPCProto* npcproto = gserver->m_npcProtoList.FindProto(shop->m_keeperIdx);

			if(npcproto == NULL)
			{
				LOG_ERROR("HACKING? : not found npc. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
				ch->m_desc->Close("not found npc");
				return;
			}

			if(npcproto->m_rvr_value > 0 && npcproto->m_rvr_value != ch->getSyndicateType())
			{
				LOG_ERROR("HACKING? : not equal rvr type. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
				ch->m_desc->Close("not equal rvr type");
				return;
			}
		}

		std::set<int> check_duplication;
		LONGLONG serverPrice = 0;

		// 판매 아이템 만들기
		for (int i = 0; i < packet->sellCount; ++i)
		{
			if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
			{
				LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("invalid packet");
				return;
			}

			if (packet->list[i].count <= 0)
			{
				LOG_ERROR("HACKING? : invalid count[%d]. charIndex[%d]", packet->list[i].count, ch->m_index);
				ch->m_desc->Close("invalid count");
				return;
			}

			// 클라이언트가 보내준 패킷 내용중 중복 검사
			if (check_duplication.insert(packet->list[i].tab << 16 | packet->list[i].invenIndex).second == false)
			{
				LOG_ERROR("HACKING? : duplicate index. charIndex[%d]", ch->m_index);
				ch->m_desc->Close("duplicate index");
				return;
			}

			item[i] = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
			if (item[i] == NULL)
			{
				LOG_ERROR("HACKING? : not found item. charIndex[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("not found item");
				return;
			}

			// 실제 아이템의 갯수가 클라이언트에서 팔려던 갯수보다 작다.
			if (item[i]->Count() < packet->list[i].count)
				throw 2;

#ifdef ENABLE_SUBJOB
			if( item[i] && item[i]->CanUseTrader( TRADE_SELL, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
			{
				throw 6;
			}
#endif //ENABLE_SUBJOB

			// 입고 있거나 상점 불가 아이템인 경우
			if (!item[i]->CanSellToShop())
				throw 6;

#ifdef LC_USA
			// 미국은 97레벨 이상의 아이템은 매각가격의 절반만 준다.
#ifdef	BUGFIX_ITEMSELL_HACKUSER

			LONGLONG itemPrice = item[i]->m_itemProto->getItemPrice() * shop->m_sellRate;
			if (item[i]->m_itemProto->GetItemProtoLevel() >= 97)
				itemPrice = itemPrice / 2;
			itemPrice = (itemPrice / 100) * packet->list[i].count;

			if( itemPrice < 0 )
				throw 3;

			serverPrice += itemPrice;
#else
			int price = item[i]->m_itemProto->getItemPrice() * shop->m_sellRate;
			if (item[i]->m_itemProto->GetItemProtoLevel() >= 97)
				price = price / 2;

			serverPrice += ( price / 100) * packet->list[i].count;
#endif //BUGFIX_ITEMSELL_HACKUSER
#else
#ifdef	BUGFIX_ITEMSELL_HACKUSER
			LONGLONG itemPrice = ( item[i]->m_itemProto->getItemPrice() * shop->m_sellRate / 100) * packet->list[i].count;

			if( itemPrice < 0 )
				throw 3;
			serverPrice += itemPrice;
#else	// BUGFIX_ITEMSELL_HACKUSER
			serverPrice += (item[i]->m_itemProto->getItemPrice() * shop->m_sellRate / 100) * packet->list[i].count;
#endif // BUGFIX_ITEMSELL_HACKUSER
#endif // LC_USA
		}

		//패시브 스킬 적용
		int bonus = 0;
		if(ch->m_avPassiveAddition.money_sell)
		{
			bonus += ch->m_avPassiveAddition.money_sell;
		}
		if(ch->m_avPassiveRate.money_sell)
		{
			bonus = serverPrice * (ch->m_avPassiveRate.money_sell - 100) / SKILL_RATE_UNIT;
		}

		// 가격의 차이가 10나스 이하이면 클라이언트 가격에 맞춘다.
		// 판매할 때 최대 10개 가능, 2로 나누면 나머지가 1씩 나오므로 오차는 최대 10... 그러므로 10까지만 보정을 함
#ifdef	BUGFIX_ITEMSELL_HACKUSER
		LONGLONG gapPrice = (serverPrice > packet->clientPrice) ? (serverPrice - packet->clientPrice) : (packet->clientPrice - serverPrice);
#else
		int gapPrice = serverPrice - packet->clientPrice;
		if (gapPrice < 0)
			gapPrice = gapPrice * -1;
#endif //BUGFIX_ITEMSELL_HACKUSER
		if (gapPrice <= 10)
		{
			serverPrice = packet->clientPrice;
		}
		else
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림

		// 합계가 다르다! -->
		if (serverPrice != packet->clientPrice)
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림
		if (serverPrice < 0)
			throw 3;

		// 인벤에서 파는 아이템 완전 제거
		for (int i = 0; i < packet->sellCount; ++i)
		{
#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_ITEM_SELL")
					   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name) << blank
					   << LOG_VAL("item-id", item[i]->getDBIndex() ) << blank
					   << LOG_VAL("amount", packet->sellCount ) << blank
					   << LOG_VAL("vender-id", packet->npcIndex ) << blank
					   << endGer;
			//<< LOG_VAL("item-id", item[i]->get
#endif // GER_LOG
			// Log
			GAMELOG << init("ITEM_SELL", ch)
					<< packet->npcIndex << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ch->m_inventory.getMoney()
					<< end;

			ch->m_inventory.decreaseItemCount(item[i], packet->list[i].count);
		}

		STATISTICS(ItemSell, serverPrice * 3 / 2);
//#endif

		// 세금 징수 : 판매 금액의 40% 의 10% => 전체 금액의 4%
		if(ch->m_pZone->m_index == ZONE_MERAC || ch->m_pZone->m_index == ZONE_EGEHA)
			gserver->AddTaxItem(serverPrice / 25);

		if(ch->m_pZone->m_index == ZONE_START || ch->m_pZone->m_index == ZONE_DRATAN)
			gserver->AddTaxItemDratan(serverPrice / 25);

		//GoldType_t Decrease Nas Function
		ch->m_inventory.increaseMoney(serverPrice, bonus);
	}
	catch (const int errCode)
	{
		// 오류 메시지 보내기
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// 인벤에 없는 물건
		case 3:			// 클라이언트 데이터 오류 : 가격 틀림
		case 4:			// 돈 넣기 실패
		case 5:			// 인벤에서 제거 실패
		case 6:			// 팔수 없는 아이템
		case 7:			// 팔려는 아이템의 개수가 너무 많음
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;
		}
	}
}

// 새로 정리한 do_ItemUpgradeReq()
//  - 확장팩 적용 시 New인첸트 시스템과 함께 적용됨.
void do_ItemUpgradeReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemUpgrade* packet = reinterpret_cast<RequestClient::doItemUpgrade*>(msg->m_buf);

	{
		std::set<int> checkdup;
		int key = 0;

		key = makeCheckDupKey(packet->tab_2, packet->invenIndex_2);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}

		if (packet->runeItemDBIndex != -1)
		{
			key = makeCheckDupKey(packet->tab_3, packet->invenIndex_3);
			if (checkdup.insert(key).second == false)
			{
				LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
				ch->m_desc->Close("dup index");
				return;
			}
		}
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_2, packet->invenIndex_2) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (packet->runeItemDBIndex != -1)
	{
		if (ch->m_inventory.isValidNormalInventory(packet->tab_3, packet->invenIndex_3) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->tab_3, packet->invenIndex_3);
			ch->m_desc->Close("invalid packet");
			return;
		}
	}

	//CItem* upItem = ch->m_inventory.getItem(packet->tab_1, packet->invenIndex_1);
	CItem* upItem = ch->m_wearInventory.FindByVirtualIndex(packet->vIndex);
	CItem* reItem = ch->m_inventory.getItem(packet->tab_2, packet->invenIndex_2);
	CItem* wearItem = upItem;

	if (upItem == NULL)
	{
		LOG_ERROR("HACKING? : tab1 item not found. char_index[%d] wearPos[%d] vIndex[%d]", ch->m_index, packet->wearPos, packet->vIndex);
		ch->m_desc->Close("tab1 item not found.");
		return;
	}

	if (reItem == NULL)
	{
		LOG_ERROR("HACKING? : tab2 item not found. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("tab2 item not found.");
		return;
	}

	if(upItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		LOG_INFO("upItem flag is ITEM_FLAG_COSTUME2");
		ch->m_desc->Close("upItem flag is ITEM_FLAG_COSTUME2");
		return;
	}

	if(upItem->IsLent() == true)
	{
		LOG_INFO("upItem flag is ITEM_FLAG_LENT");
		ch->m_desc->Close("upItem flag is ITEM_FLAG_LENT");
		return;
	}

	char wearPos = upItem->getWearPos();
	CItem* runeItem = NULL;								// 제련 보호 아이템
	if (packet->runeItemDBIndex != -1)
	{
		runeItem = ch->m_inventory.getItem(packet->tab_3, packet->invenIndex_3);
		if (runeItem == NULL)
		{
			LOG_ERROR("HACKING? : tab3 item not found. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->tab_3, packet->invenIndex_3);
			ch->m_desc->Close("tab3 item not found.");
			return;
		}

		if (runeItem->m_itemProto->getItemIndex() != packet->runeItemDBIndex)
		{
			LOG_ERROR("HACKING? : not equal item db index. char_index[%d] dbIndex[%d] packet_dbIndex[%d]",
					  ch->m_index, runeItem->m_itemProto->getItemIndex(), packet->runeItemDBIndex);
			ch->m_desc->Close("reitem not found.");
			return;
		}

		// 최상급제련보호석 강화 레벨 제한 체크
		if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
		{
			if(!(upItem->getPlus() >= 20 && upItem->getPlus() <= 25))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}

		CItem* equipItem = ch->m_inventory.FindByVirtualIndex(wearItem->m_nCompositeItem);
		if (!equipItem)
			equipItem = wearItem;

		if( runeItem->m_itemProto->getItemLevel() > equipItem->m_itemProto->getItemLevel() || runeItem->m_itemProto->getItemLevel2() < equipItem->m_itemProto->getItemLevel() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 제련석 검사
	if (	reItem->m_itemProto->getItemTypeIdx()		!= ITYPE_ETC
			||	reItem->m_itemProto->getItemSubTypeIdx()	!= IETC_UPGRADE
	   )
	{
		LOG_INFO("reItem is not ITYPE_ETC or IETC_UPGRADE");
		return;
	}

	CItem* pItemClothes = NULL;		// 실패시 함께 사라질 아이템
	if (upItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
	{
		if ((upItem->getFlag() & FLAG_ITEM_COMPOSITION) == 0)
		{
			LOG_INFO("upItem flag is not FLAG_ITEM_COMPOSITION");
			return;
		}

		CItem* pItemEquip = ch->m_inventory.FindByVirtualIndex(upItem->m_nCompositeItem);
		if (pItemEquip == NULL)
		{
			LOG_INFO("not found composite item with upItem");
			return;
		}

		pItemClothes = upItem;
		upItem = pItemEquip;
	}
	else
	{
		if (upItem->getPlus() != packet->plus)
		{
			LOG_ERROR("upItem plus is not plus(packet). char_index[%d] upItem plus[%d] packet plus[%d]",
					  ch->m_index, upItem->getPlus(), packet->plus);
			return;
		}
	}

	// 업그레이드 가능 검사
	if (upItem->CanUpgrade() == false)
	{
		LOG_ERROR("upItem is not upgrade item or lent item. char_index[%d]", ch->m_index);
		return;
	}

	// 부스터는 업그레이드 아님
	if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_BOOSTER)
	{
		LOG_ERROR("reItem is IETC_UPGRADE_BOOSTER. char_index[%d]", ch->m_index);
		return;
	}

	// 룬 제련석일 경우 146이상부터 업그레이드 가능
	if( reItem->IsUpgradeRune() )
	{
		if( upItem->m_itemProto->GetItemProtoLevel() < ITEM_UPGRADE_GENERAL_MAXLEVEL )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 146 이상부터는 룬으로만 가능
	if ( upItem->m_itemProto->GetItemProtoLevel() >= ITEM_UPGRADE_GENERAL_MAXLEVEL )
	{
		if (!reItem->IsUpgradeRune() && reItem->m_itemProto->getItemNum0() != IETC_UPGRADE_PLATINUM )		// 플래티넘 재련석
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	else
	{
		// 일반 제련석의 경우 1 - 60 레벨이 맞는가
		if (reItem->getFlag() != 0 && (reItem->getFlag() < ITEM_UPGRADE_GENERAL_MINLEVEL || reItem->getFlag() > ITEM_UPGRADE_GENERAL_MAXLEVEL))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 일반제련석의 경우 제련석의 레벨 이하만 업그레이드 가능
		if (reItem->getFlag() != 0 && reItem->getFlag() < upItem->GetItemLevel())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	//슈퍼고제를 이미 슈퍼고제 바른 아이템에 업그레이드 하려고 할경우
	if(reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SPECIAL_SUPER && upItem->getFlag() & FLAG_ITEM_SUPER_STONE_USED)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_UPGRADE_CANT_SUPERSTONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int result = ITEM_UPGRADE_RESULT_NOCHANGE;

	if (
		(upItem->getPlus() >= MAX_UPGRADE_PLUS && reItem->m_itemProto->getItemNum0() != IETC_UPGRADE_PLATINUM ) ||
		(upItem->IsUsedPlatinumSpecial() &&  reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PLATINUM )
	)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if((reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PLATINUM) &&
			(upItem->getFlag() & FLAG_ITEM_COMPOSITION))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DUP_PREFINE_COMPOSITE);
		SEND_Q(rmsg, ch->m_desc);

		return;
	}
	else
	{
		// 일반 제련석
		if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_GENERAL)
		{
			int upprob = 0;
			
			if(upItem->getPlus() >= 18)
			{
				upprob = 50 - upItem->getPlus() + 17;
			}
			else
			{
				upprob = 50 - upItem->getPlus() * 2;
			}

			if (upprob < 1)
				upprob = 1;

			// 성공/변화 없음(+3미만)/파괴
			if (GetRandom(1, 100) <= upprob)
			{
				result = ITEM_UPGRADE_RESULT_PLUS;
			}
			else
			{
				result = ITEM_UPGRADE_RESULT_BROKEN;
				if (upItem->getPlus() < 3)
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
			}
		}
		// 행운의 제련석
		else if( reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_LUCKY)
		{
			if( upItem->getPlus() > 5)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_UPGRADE_CANT_6LEVEL_LUCKY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			result = ITEM_UPGRADE_RESULT_PLUS;
		}
		// 고급제련석 - 0627
		else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SPECIAL)
		{
			// 잠수함 : 고제 확률 조정
			// +14에 고제 사용시에는 확률 조정, 그외는 기존과 동일
			if(upItem->getPlus() >= 18)
			{
				int prob = GetRandom(1, 100);
				int upprob = 60 - (upItem->getPlus() + 32);

				if(g_bUpgradeEvent)
					upprob = (upprob * g_nUpgradeProb) / 100;

				if(upprob < 0)
					upprob = 1;

				// 성공/변함없음(+3미만)/실패
				if(prob > 0 && prob <= 10)
				{
					int rand = GetRandom(1, 2);
					switch(rand)
					{
					case 1:
						result = ITEM_UPGRADE_RESULT_MINUS;
						break;
					case 2:
						result = ITEM_UPGRADE_RESULT_BROKEN;
						break;
					}
				}
				else if(prob > 10 && prob <= 10 + upprob)
				{
					result = ITEM_UPGRADE_RESULT_PLUS;
				}
				else
				{
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
				}
			}
			else
			{
				int prob = GetRandom(1, 100);
				int upprob = 60 - (upItem->getPlus() * 3);

				if(g_bUpgradeEvent)
					upprob = (upprob * g_nUpgradeProb) / 100;

				if(prob > 0 && prob <= 7)
				{
					result = ITEM_UPGRADE_RESULT_BROKEN;
					
					if (upItem->getPlus() < 3)
						result = ITEM_UPGRADE_RESULT_NOCHANGE;
				}
				else if( prob > 7 && prob <= (7 + 13) )
				{
					result = ITEM_UPGRADE_RESULT_MINUS;
					
					if (upItem->getPlus() < 3)
						result = ITEM_UPGRADE_RESULT_NOCHANGE;
				}
				else if( prob > 20 && prob <= (20 + upprob) )
				{
					result = ITEM_UPGRADE_RESULT_PLUS;
				}
				else
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
			}
		}
		// 플래티늄 제련석
		else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PLATINUM)
		{
			if ( upItem->getPlus() > MAX_UPGRADE_PLUS || reItem->getPlus() >= PLATINUM_MAX_PLUS )
			{
				if( reItem->m_itemProto->getItemNum1() == 0	 ||							// 마스터 스톤이 아니거나
						( reItem->m_itemProto->getItemNum1() - upItem->getPlus() <= 0 )		// 인첸 수치 보다 같거나 낮은 마스터 스톤일때
				  )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
			}
#ifdef _COMPOSITE_ITEM_UPGRADE_BUG_FIX
			if( pItemClothes )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			else
#endif
				result = ITEM_UPGRADE_PLATINUM_RESULT_PLUS;
		}
		// 카오스 제련석
		else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_CHAOS)
		{
			if (upItem->getPlus() < 12)
				result = ITEM_UPGRADE_RESULT_PLUS;
			else
				result = ITEM_UPGRADE_RESULT_NOCHANGE;
		}
		//슈퍼고제 - 0627
		else if(reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SPECIAL_SUPER)
		{
			//슈퍼고제는 +14 아이템에는 사용할수 없다.
			if (upItem->getPlus() > 13)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_UPGRADE_CANT_14LEVEL);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			//100프로 성공 / 한번밖에 사용 못함.
			result = ITEM_UPGRADE_RESULT_PLUS;
			upItem->setFlag(upItem->getFlag() | FLAG_ITEM_SUPER_STONE_USED);
		}
		// 룬
		else if (reItem->IsUpgradeRune())
		{
			result = getUpgradeRuneResult(reItem, upItem, ch);
			if (result < 0) return;
		}
	}

// 제련 보호석
	if ((result == ITEM_UPGRADE_RESULT_BROKEN || result == ITEM_UPGRADE_RESULT_MINUS) && runeItem)
	{
		int PlatinumPlus = 0;
		int real_plus = upItem->getPlus();
		FLAG_ITEM_PLATINUM_GET(upItem->getFlag(), PlatinumPlus);
		if(PlatinumPlus > 0)
		{
			real_plus = real_plus - PlatinumPlus;
		}

		if ( reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_GENERAL     ||
				reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SPECIAL
				|| reItem->IsUpgradeRune()
		   )
		{
#ifdef _COMPOSITE_ITEM_UPGRADE_BUG_FIX
			if( pItemClothes )
			{
				if( upItem->getPlus() > 2 && upItem->getPlus() < 10 )
				{
					if (runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM)
					{
						result = ITEM_UPGRADE_RESULT_NOCHANGE;
					}
					else if(packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM
							|| packet->runeItemDBIndex == SUPER_HIGH_RUNE_PROTECTION_ITEM
							|| packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
							|| packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
							|| packet->runeItemDBIndex == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
						   )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
				else if(upItem->getPlus() > 9 && upItem->getPlus() < 20)
				{
					if( runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
							|| runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
					  )
					{
						result = ITEM_UPGRADE_RESULT_MINUS;
					}
					else if (
						runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM
						|| runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM
					)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
				else if(upItem->getPlus() > 19 && upItem->getPlus() < 26)
				{
					if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
					{
						result = ITEM_UPGRADE_RESULT_MINUS;
					}
					else if (
						runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
						|| runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
					)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
			}
			else
#endif // _COMPOSITE_ITEM_UPGRADE_BUG_FIX
				if (real_plus > 2 && real_plus < 10)
				{
					if (runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM)
					{
						result = ITEM_UPGRADE_RESULT_NOCHANGE;
					}
					else if(packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM
						|| packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
						|| packet->runeItemDBIndex == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
						|| packet->runeItemDBIndex == SUPER_HIGH_RUNE_PROTECTION_ITEM
						)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
				else if ( real_plus > 9 && real_plus < 20)
				{
					if(real_plus > 14 && real_plus < 20)
					{
						if( runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
							|| runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
							)
						{
							result = ITEM_UPGRADE_RESULT_MINUS;
						}
						else if (
							runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM
							|| packet->runeItemDBIndex == SUPER_HIGH_RUNE_PROTECTION_ITEM
							)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
					else
					{
						if( runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
							|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
							)
						{
							result = ITEM_UPGRADE_RESULT_MINUS;
						}
						else if (
							runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM
							|| packet->runeItemDBIndex == SUPER_HIGH_RUNE_PROTECTION_ITEM
							)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}
				}
				else if(real_plus > 19 && real_plus < 26)
				{
					if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
					{
						result = ITEM_UPGRADE_RESULT_MINUS;
					}
					else if (
						runeItem->m_itemProto->getItemIndex() == LOW_RUNE_PROTECTION_ITEM
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
						|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
						)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysMsg(rmsg, MSG_SYS_UPGRADE_NOCONDITION);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
		}
	}
//#endif

	// 업글할 아이템 update 여부
	bool bUpdate = false;
	int nPlusNum = 0;           // +,- 수치

	CNetMsg::SP repMsg(new CNetMsg);

	switch (result)
	{
	case ITEM_UPGRADE_RESULT_PLUS:
		{
			nPlusNum = 1;

			if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_DEVIL_RUNE)
			{
				int nPlusProb = GetRandom(1, 100);
				if (nPlusProb < 11)	nPlusNum = 3;
				else if (nPlusProb < 41)	nPlusNum = 2;
				else	nPlusNum = 1;
			}

			{
				ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_PLUS, reItem->IsUpgradeRune(), nPlusNum);
			}

			upItem->setPlus(upItem->getPlus() + nPlusNum);
			if (pItemClothes)
				pItemClothes->setPlus(pItemClothes->getPlus() + nPlusNum);
			bUpdate = true;

			GAMELOG << init("ITEM_UPGRADE", ch) << "+" << nPlusNum << delim << end;
		}
		break;
	case ITEM_UPGRADE_RESULT_MINUS:
		{
			GAMELOG << init("ITEM_UPGRADE", ch);

			if (runeItem)
			{
#ifdef _COMPOSITE_ITEM_UPGRADE_BUG_FIX
				if( pItemClothes )
				{
					if( upItem->getPlus() > 9 && upItem->getPlus() < 26)
					{
						if( runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
								|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
								|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
								|| runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
								|| runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM
						  )
						{
							if( upItem->getPlus() >= 20 )
							{
								pItemClothes->setPlus(20 + pItemClothes->getPlus() - upItem->getPlus());
								upItem->setPlus(20);
								GAMELOG << "20" << delim;
								ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM, true, upItem->getPlus());
							}
							else if(upItem->getPlus() >= 15 && runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM)
							{
								pItemClothes->setPlus(15 + pItemClothes->getPlus() - upItem->getPlus());
								upItem->setPlus(15);
								GAMELOG << "15" << delim;
								ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM, true, upItem->getPlus());
							}
							else if( upItem->getPlus() >= 10 )
							{
								pItemClothes->setPlus(10 + pItemClothes->getPlus() - upItem->getPlus());
								upItem->setPlus(10);
								GAMELOG << "10" << delim;
								ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM, true, upItem->getPlus());
							}
							else
							{
								upItem->setPlus(upItem->getPlus() - 1);
								pItemClothes->setPlus(pItemClothes->getPlus() - 1);
								GAMELOG << "-1" << delim;
								ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS, false);
							}
							bUpdate = true;
						}
					}
					else if (pItemClothes->getPlus() > 3 && pItemClothes->getPlus() < 10 )
					{
						if( packet->runeItemDBIndex == LOW_RUNE_PROTECTION_ITEM  )
						{
							upItem->setPlus(upItem->getPlus() - 1);
							pItemClothes->setPlus(pItemClothes->getPlus() - 1);
							bUpdate = true;
							ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS);
						}
					}
				}
				else
#endif	// _COMPOSITE_ITEM_UPGRADE_BUG_FIX
					if (upItem->getPlus() > 9 && upItem->getPlus() < 26)
					{
						if( runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM
								|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60
								|| runeItem->m_itemProto->getItemIndex() == HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90
								|| runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM
								|| runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM
						  )
						{
							if( pItemClothes )
							{
								if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
								{
									pItemClothes->setPlus(20 + pItemClothes->getPlus() - upItem->getPlus());
								}
								else if(runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM)
								{
									pItemClothes->setPlus(15 + pItemClothes->getPlus() - upItem->getPlus());
								}
								else
								{
									pItemClothes->setPlus(10 + pItemClothes->getPlus() - upItem->getPlus());
								}
							}
							int PlatinumPlus = 0;
							int plusproto = 0;
							FLAG_ITEM_PLATINUM_GET(upItem->getFlag(), PlatinumPlus);
							
							if (PlatinumPlus > 0)
							{
								plusproto = upItem->getPlus() - PlatinumPlus;

								if(plusproto > 9)
								{
									if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
									{
										upItem->setPlus(20 + PlatinumPlus);
									}
									else if(runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM)
									{
										upItem->setPlus(15 + PlatinumPlus);
									}
									else
									{
										upItem->setPlus(10 + PlatinumPlus);
									}
									ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM, true, upItem->getPlus() - PlatinumPlus);
								}
							}
							else
							{
								if(runeItem->m_itemProto->getItemIndex() == SUPER_HIGH_RUNE_PROTECTION_ITEM)
								{
									upItem->setPlus(20);
									GAMELOG << "20" << delim;
								}
								else if(runeItem->m_itemProto->getItemIndex() == MIDDLE_HIGH_RUNE_PROTECTION_ITEM)
								{
									upItem->setPlus(15);
									GAMELOG << "20" << delim;
								}
								else
								{
									upItem->setPlus(10);
									GAMELOG << "10" << delim;
								}
								ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM, true, upItem->getPlus());
							}
							bUpdate = true;
						}
					}
#ifdef LC_GAMIGO
					else if( upItem->getPlus() > 3 && upItem->getPlus() < 10 )
					{
						if( packet->runeItemDBIndex == LOW_RUNE_PROTECTION_ITEM  )
						{
							upItem->setPlus(upItem->getPlus() - 1);
							if (pItemClothes)
								pItemClothes->setPlus(pItemClothes->getPlus() - 1);
							bUpdate = true;
							ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS);
						}
					}
#endif // LC_GAMIGO
			}
			else
//#endif
			{
				upItem->setPlus(upItem->getPlus() - 1);
				if (pItemClothes)
					pItemClothes->setPlus(pItemClothes->getPlus() - 1);

				bUpdate = true;
				ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS, reItem->IsUpgradeRune(), nPlusNum);
				GAMELOG << "-1" << delim;
			}
		}
		break;
	case ITEM_UPGRADE_RESULT_NOCHANGE:
		{
			GAMELOG << init("ITEM_UPGRADE", ch)
					<< "No change" << delim
					<< itemlog(upItem);

			if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_GENERAL)
				GAMELOG << delim << "USE GENERAL" << end;
			else
				GAMELOG << delim << "USE SPECIAL" << end;

			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_NOCHANGE, false);
		}
		break;
	case ITEM_UPGRADE_RESULT_BROKEN:
		{
			GAMELOG << init("ITEM_UPGRADE", ch)
				<< "Broken" << delim
				<< itemlog(upItem)
				<< end;

			if(wearPos != WEARING_NONE)
			{
				ch->m_wearInventory.RemoveItem(wearPos);
			}

			// 의상 아이템의 경우 실패시 결합 아이템 함께 제거(upItem : 결합 아이템, pItemClothes : 의상 아이템)
			if (pItemClothes)
			{
				GAMELOG << init("ITEM_UPGRADE", ch)
						<< "BROKEN COMPOSITED ITEM" << delim
						<< itemlog(upItem)
						<< end;

				ch->m_inventory.deleteItemByItem(upItem);
			}

			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_BROKEN, false);
		}
		break;
	case ITEM_UPGRADE_PLATINUM_RESULT_PLUS:
		{
			int platinumPlused;
			FLAG_ITEM_PLATINUM_GET(upItem->getFlag(), platinumPlused);

			if( reItem->m_itemProto->getItemNum1() > 0 )		// 플러스 값이 적용된 플래티늄 제련석은 밤~밤바바밤바~~ 발라버려
			{
				reItem->setPlus(reItem->m_itemProto->getItemNum1() - upItem->getPlus());		// DB의 설정된 값과의 차이를 +시켜줌

				if( reItem->getPlus() <= 0 )		// 위에서 검사해서 여기까지 음수가 될 경우는 없지만 만약을 대비한 에러처리
				{
					reItem->setPlus(0);
				}

				platinumPlused += reItem->getPlus();
				upItem->setPlus(upItem->getPlus() + reItem->getPlus());
			}
			else if( upItem->getPlus() + reItem->getPlus() > 26 )
			{
				int differ = 26 - upItem->getPlus();
				upItem->setPlus(26);
				platinumPlused += differ;
			}
			else
			{
				platinumPlused += reItem->getPlus();
				upItem->setPlus(upItem->getPlus() + reItem->getPlus());
			}

			upItem->setFlag(FLAG_ITEM_PLATINUM_SET(upItem->getFlag(), platinumPlused));

			// 시간 세팅
			upItem->setUsed(gserver->getNowSecond() + reItem->m_itemProto->getItemNum3() * 60 * 60);
			bUpdate = true;
			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_PLATINUM_RESULT_PLUS, false);

			GAMELOG << init("ITEM_UPGRADE", ch) << "PLATINUM" << delim << upItem->getUsed() << delim;
		}
		break;
	default:
		break;
	}

	// upgrade 된 결과 알리기
	if (bUpdate)
	{
		GAMELOG << itemlog(upItem);

		if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_GENERAL)		GAMELOG << delim << "USE GENERAL" << delim << reItem->getDBIndex() << delim << reItem->m_itemProto->getItemName() << end;
		else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SPECIAL_SUPER)	GAMELOG << delim << "USE SUPER-SPECIAL" << delim << reItem->getDBIndex() << delim << reItem->m_itemProto->getItemName()  << end;
		else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PLATINUM)		GAMELOG << delim << "USE PLATINUM-SPECIAL" << delim << reItem->getDBIndex() << delim << reItem->m_itemProto->getItemName() << end;
		else GAMELOG << delim << "USE SPECIAL" << delim << reItem->getDBIndex() << delim << reItem->m_itemProto->getItemName() << end;

		// 의상 아이템의 경우 FLAG_ITEM_PLATINUM_SET
		if (pItemClothes)
		{
			pItemClothes->setFlag(FLAG_ITEM_PLATINUM_SET(pItemClothes->getFlag(), upItem->getPlus()));
			ch->m_inventory.sendOneItemInfo(upItem);
		}

		ch->m_wearInventory.sendOneItemInfo(wearPos);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			WearingMsg(rmsg, ch, upItem->getWearPos(), upItem->getDBIndex(), upItem->getPlus());
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}
	}

	{
		// 제련석 하나 감소
		ch->m_inventory.decreaseItemCount(reItem, 1);
	}

	SEND_Q(repMsg, ch->m_desc);

	// 제련 보호 아이템 하나 감소
	if (runeItem)
	{
		ch->m_inventory.decreaseItemCount(runeItem, 1);
		GAMELOG << delim << " USE RUNE_PROTECTION_ITEM" << delim << packet->runeItemDBIndex << end;
	}

	ch->m_assist.CheckApplyConditions();
	ch->CalcStatus(true);
}

void do_ItemRefinReq(CPC* ch, CNetMsg::SP& msg)
{
#ifndef BUGFIX_REFINE_SPECIAL_ITEM
	static const int nProbFactorWeaponLow10				= 5;
	static const int nProbFactorWeaponHigh10			= 5;
#endif // BUGFIX_REFINE_SPECIAL_ITEM
	static const int nProbFactorArmorLow10				= 15;
	static const int nProbFactorArmorHigh10				= 15;

	RequestClient::doItemRefin* packet = reinterpret_cast<RequestClient::doItemRefin*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 전환 아이템
	CItem* cItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (cItem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item.");
		return;
	}

	// 전환 아이템이 무기 or 방어구 인가
	if (!(cItem->IsWeaponType() || cItem->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_KIND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (cItem->GetItemLevel() < 1 || cItem->GetItemLevel() > 145 )
	{
		// 레벨이 1보다 작으면 불가능
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_CANNOT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 전환아이템의 plus가 0이 아니면 제련석 변환 불가
	if (cItem->getPlus() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_PLUS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (cItem->getWearPos() != WEARING_NONE)
	{
		// 착용중인 아이템은 못올린다 벗고해라
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_WEARING);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 아이템 레벨에 따른 소요 비용
	int price = ((cItem->GetItemLevel() + 1) * (cItem->GetItemLevel() + 3) * (cItem->GetItemLevel() - 1) + 100) / 4;

	// 돈검사
	if (ch->m_inventory.getMoney() < price)
	{
		// 돈이 모질라
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_SHORT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int result = ITEM_REFINE_RESULT_ONE;

	int spb = 0;

#ifndef BUGFIX_REFINE_SPECIAL_ITEM
	if (cItem->IsWeaponType())
	{
		if (cItem->GetItemLevel() == 128)
			spb = 10000;
		else if (cItem->GetItemLevel() > 10)
			spb = ABS(100 * (cItem->GetItemLevel() + 128) / (cItem->GetItemLevel() - 128) / nProbFactorWeaponHigh10);
		else
			spb = ABS(100 * (cItem->GetItemLevel() + 128) / (cItem->GetItemLevel() - 128) / nProbFactorWeaponLow10);
		result = ITEM_REFINE_RESULT_TWO;
	}
	else if (cItem->IsArmorType())
#endif // BUGFIX_REFINE_SPECIAL_ITEM
	{
		if (cItem->GetItemLevel() == 256)
			spb = 10000;
		else if (cItem->GetItemLevel() > 10)
			spb = ABS(100 * (cItem->GetItemLevel() + 256) / (cItem->GetItemLevel() - 256) / nProbFactorArmorHigh10);
		else
			spb = ABS(100 * (cItem->GetItemLevel() + 256) / (cItem->GetItemLevel() - 256) / nProbFactorArmorLow10);
#ifdef BUGFIX_REFINE_SPECIAL_ITEM
		if( cItem->IsWeaponType() )
			result = ITEM_REFINE_RESULT_TWO;
		else if(cItem->IsArmorType())
#endif
			result = ITEM_REFINE_RESULT_ONE;
	}

	// 말레이시아에서는 고제가 나오지 않는다
#ifdef DISABLE_REFINE_SPECIAL_STONE
#else
	if (GetRandom(1, 10000) <= spb)
		result = ITEM_REFINE_RESULT_SPECIAL;
#endif

	CItem* reItem = NULL;
	if (result == ITEM_REFINE_RESULT_TWO)
	{
		reItem = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_normalRefineItem->getItemIndex(), WEARING_NONE, 0, cItem->GetItemLevel(), 2);
	}
	else if (result == ITEM_REFINE_RESULT_ONE)
	{
		reItem = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_normalRefineItem->getItemIndex(), WEARING_NONE, 0, cItem->GetItemLevel(), 1);
	}
	else if (result == ITEM_REFINE_RESULT_SPECIAL)
	{
		reItem = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_specialRefineItem->getItemIndex(), WEARING_NONE, 0, 0, 1);
	}

	if (!reItem)
		return;

	if (ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		delete reItem;
		return;
	}

	GAMELOG << init("ITEM_REFINE", ch)
			<< itemlog(cItem) << delim
			<< itemlog(reItem);

	// 전환된 아이템 인벤에서 제거
	ch->m_inventory.decreaseItemCount(cItem, 1);

	bool bDrop = false;
	if (ch->m_inventory.addItem(reItem) == false)
	{
		bDrop = true;
		ch->m_pArea->DropItem(reItem, ch);
		reItem->m_preferenceIndex = ch->m_index;
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, reItem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	if (bDrop)
		GAMELOG << delim << "DROP" << end;
	else
		GAMELOG << delim << "INVEN" << end;

	ch->m_inventory.decreaseMoney(price);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemRefineRepMsg(rmsg, (char)result);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);
}

void do_ItemOptionAddReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemAddOption* packet = reinterpret_cast<RequestClient::doItemAddOption*>(msg->m_buf);

	if (packet->weapon_wearPos > WEARING_BOOTS)
	{
		LOG_ERROR("HACKING : invalid wearPos[%d]. charIndex[%d]", packet->weapon_wearPos, ch->m_index);
		ch->m_desc->Close("invalid wearPos");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->jam_tab, packet->jam_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->jam_tab, packet->jam_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 옵션 붙일 아이템
	CItem* wearItem = ch->m_wearInventory.getWearItem(packet->weapon_wearPos);
	if (wearItem == NULL)
	{
		LOG_ERROR("HACKING : not found wearItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not found wearItem");
		return;
	}

	if (wearItem->getVIndex() != packet->weapon_virtualIndex)
	{
		LOG_ERROR("HACKING : invalid virtualIndex for wearItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid virtualIndex for wearItem");
		return;
	}

	if (wearItem->getWearPos() != packet->weapon_wearPos) // 아이템이 없거나 wearpos가 다르거나
	{
		LOG_ERROR("HACKING : invalid wearPos. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid wearPos");
		return;
	}

	CItem* jamItem = ch->m_inventory.getItem(packet->jam_tab, packet->jam_invenIndex);
	if (jamItem == NULL)
	{
		LOG_ERROR("HACKING : not found jamItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not found jamItem");
		return;
	}

	if (jamItem->getVIndex() != packet->jam_virtualIndex)
	{
		LOG_ERROR("HACKING : invalid virtualIndex for jamItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid virtualIndex for jamItem");
		return;
	}

	if (wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 옵션 붙일 아이템이 무기 or 방어구 인가
	if (!(wearItem->IsWeaponType() || wearItem->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_KIND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (wearItem->IsWeaponType() && wearItem->m_nOption >= MAX_WEAPON_OPTION)
	{
		// 최대 옵션 수 초과
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if (wearItem->IsArmorType() && wearItem->m_nOption >= MAX_ARMOR_OPTION)
	{
		// 최대 옵션 수 초과
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( jamItem->m_itemProto->getItemIndex() !=  2361 )
	{
		if (!(wearItem->getFlag() & FLAG_ITEM_OPTION_ENABLE))
		{
			// enable 셋팅 되어 있지 않다
			CNetMsg::SP rmsg(new CNetMsg);
			ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	else
	{
		if( (wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_RARE ) || (wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		switch (wearItem->getDBIndex())
		{
		case 887 :
		case 907 :
		case 908 :
		case 909 :
		case 910 :
		case 911 :
		case 912 :
		case 913 :
		case 914 :
		case 915 :
		case 916 :
		case 917 :
		case 918 :
		case 919 :
		case 920 :
		case 921 :
		case 922 :
		case 923 :
		case 924 :
		case 925 :
		case 926 :
		case 927 :
		case 928 :
		case 929 :
		case 930 :
		case 931 :
		case 932 :
		case 933 :
		case 934 :
		case 935 :
		case 936 :
		case 937 :
		case 938 :
		case 939 :
		case 940 :
		case 941 :
		case 942 :
		case 943 :
		case 944 :
		case 945 :
		case 946 :
		case 947: // 유니크 아이템 피와땀 적용 안되게 처리

		case 1843 :
		case 1844 : // 산타 모자 피와담 적용 안되게 처리
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}

	// 블러드 젬 확인

	if( jamItem->m_itemProto->getItemIndex() != 2361 )
	{
		if (jamItem->m_itemProto->getItemTypeIdx() != ITYPE_ETC || jamItem->m_itemProto->getItemSubTypeIdx() != IETC_OPTION || jamItem->m_itemProto->getItemNum0() != IETC_OPTION_TYPE_BLOOD)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체
	// 기존의 옵션 비트 필드에 저장
	LONGLONG bitfield = 0;
	int i;
	for (i=0; i < wearItem->m_nOption; i++)
		bitfield |= ((LONGLONG)1 << wearItem->m_option[i].m_type);

	int type;
	COptionProto* proto = NULL;
	while (true)
	{
		// type 결정
		type = GetRandom(0, maxnumoption - 1);
		for (i = 0; i < maxnumoption; i++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type + i) % maxnumoption))) == 0)
				break ;
		}

		// 더이상 옵션을 붙일 수 없음
		if (i == maxnumoption)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		else
			type = (type + i) % maxnumoption;

		bitfield |= ((LONGLONG)1 << type);

		// type 가능 검사
		proto = gserver->m_optionProtoList.FindProto(type);

		if (!proto)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 붙일 수 있는 옵션인지 검사

		// 무기이면
		if (wearItem->IsWeaponType())
		{
			if (!((1 << wearItem->m_itemProto->getItemSubTypeIdx()) & proto->m_weaponType))
				continue ;
		}

		// 방어구이면
		if (wearItem->IsArmorType())
		{
			if (!((1 << wearItem->m_itemProto->getItemSubTypeIdx()) & proto->m_wearType))
				continue ;
		}

		break;
	}

	wearItem->m_option[wearItem->m_nOption].MakeOptionValue(proto, wearItem->GetItemLevel(), wearItem->m_nOption);
	++wearItem->m_nOption;

	// 옵션을 붙일수 없는 상태로
	wearItem->setFlag(wearItem->getFlag() & ~(FLAG_ITEM_OPTION_ENABLE));

	if( jamItem->m_itemProto->getItemIndex() != 2361 )
	{
		GAMELOG << init("ITEM_OPTION_ADD", ch)
				<< itemlog(wearItem)
				<< end;
	}
	else
	{
		GAMELOG << init( "CASH_ITEM_USE", ch )
				<< itemlog( jamItem ) << delim
				<< end;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if( jamItem->getDBIndex() != 2361 )
			ItemOptionAddRepMsg(rmsg, wearItem, MSG_OPTION_ADD_SUCCESS);
		else if( jamItem->getDBIndex() == 2361 )
			ItemOptionAddRepMsg(rmsg, wearItem, MSG_OPTION_BLOOD_SWEAT_FRUIT_ADD_SUCCESS);

		// 결과 메시지
		SEND_Q(rmsg, ch->m_desc);
	}

	// 블러드 젬 없애기
	ch->m_inventory.decreaseItemCount(jamItem, 1);

	// 옵션 업데이트 메시지
	ch->m_wearInventory.sendOneItemInfo(wearItem->getWearPos());

	ch->CalcStatus(true);
}

void do_ItemOptionDelReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemDelOption* packet = reinterpret_cast<RequestClient::doItemDelOption*>(msg->m_buf);

	// 착용한 아이템이어야지
	if (packet->weapon_wearPos > WEARING_BOOTS)
	{
		LOG_ERROR("HACKING : invalid wearPos[%d]. charIndex[%d]", packet->weapon_wearPos, ch->m_index);
		ch->m_desc->Close("invalid wearPos");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->jam_tab, packet->jam_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->jam_tab, packet->jam_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 옵션 지울 아이템
	CItem* wearItem = ch->m_wearInventory.getWearItem(packet->weapon_wearPos);

	if (wearItem == NULL)
	{
		LOG_ERROR("HACKING : not found wearItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not found wearItem");
		return;
	}

	if (wearItem->getVIndex() != packet->weapon_virtualIndex)
	{
		LOG_ERROR("HACKING : invalid virtualIndex. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid virtualIndex");
		return;
	}

	if (wearItem->getWearPos() != packet->weapon_wearPos)
	{
		LOG_ERROR("HACKING : invalid wearPos. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid wearPos");
		return;
	}

	// 정화석 확인
	CItem* jamItem = ch->m_inventory.getItem(packet->jam_tab, packet->jam_invenIndex);
	if (jamItem == NULL)
	{
		LOG_ERROR("HACKING : not found jamItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not found jamItem");
		return;
	}

	if (jamItem->getVIndex() != packet->jam_virtualIndex)
	{
		LOG_ERROR("HACKING : invalid virtualIndex for jamItem. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid virtualIndex for jamItem");
		return;
	}

	if(wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (wearItem->IsRareItem())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (wearItem->IsOriginItem())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 옵션 지울 아이템이 무기 or 방어구 인가
	if (!(wearItem->IsWeaponType() || wearItem->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, NULL, MSG_OPTION_DEL_ERROR_KIND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (wearItem->m_nOption < MAX_ITEM_OPTION_DEL)
	{
		// 최대 옵션 수 초과
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, NULL, MSG_OPTION_DEL_ERROR_NUM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (jamItem->m_itemProto->getItemTypeIdx() != ITYPE_ETC || jamItem->m_itemProto->getItemSubTypeIdx() != IETC_OPTION || jamItem->m_itemProto->getItemNum0() != IETC_OPTION_TYPE_CLEAR)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionAddRepMsg(rmsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		// 정화석 없애기
		ch->m_inventory.decreaseItemCount(jamItem, 1);
	}

	int i;
	for (i=0; i < wearItem->m_nOption; i++)
	{
		wearItem->m_option[i].m_proto = NULL;
		wearItem->m_option[i].m_type = -1;
		wearItem->m_option[i].m_level = 0;
		wearItem->m_option[i].m_value = 0;
		wearItem->m_option[i].m_dbValue = 0;
	}

	wearItem->m_nOption = 0;

	// 옵션을 붙일수 없는 상태로
	wearItem->setFlag(wearItem->getFlag() & ~(FLAG_ITEM_OPTION_ENABLE));

	GAMELOG << init("ITEM_OPTION_DEL", ch)
			<< itemlog(wearItem)
			<< end;

	// 옵션 업데이트 메시지
	ch->m_wearInventory.sendOneItemInfo(wearItem->getWearPos());

	{
		// 결과 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		ItemOptionDelRepMsg(rmsg, wearItem, MSG_OPTION_DEL_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);
}

void do_ItemProcessReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemProcess* packet = reinterpret_cast<RequestClient::doItemProcess*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->useitem_tab, packet->useitem_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	try
	{
		// 사용한 가공문서 확인
		CItem* doc = ch->m_inventory.getItem(packet->useitem_tab, packet->useitem_invenIndex);
		if (doc == NULL)
		{
			LOG_ERROR("HACKING? : not found use item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
			return;
		}

		if (doc->m_itemProto->getItemTypeIdx() != ITYPE_ONCE || doc->m_itemProto->getItemSubTypeIdx() != IONCE_PROCESS_DOC)
		{
			LOG_ERROR("use item is not ITYPE_ONCE or IONCE_PROCESS_DOC");
			throw 0;
		}

		// 만드려는 가공품 확인
		CItemProto* proto = gserver->m_itemProtoList.FindIndex(packet->resultItemDBIndex);
		if (proto == NULL)
		{
			LOG_ERROR("not found result item proto. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		// 가공품이 아니면 return
		if (proto->getItemTypeIdx() != ITYPE_ETC || proto->getItemSubTypeIdx() != IETC_PROCESS)
		{
			LOG_ERROR("result item is not ITYPE_ETC or IETC_PROCESS. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		// 만들수 있지 않으면
		if (!(proto->getItemFlag() & ITEM_FLAG_MADE))
		{
			LOG_ERROR("result item flag is not ITEM_FLAG_MADE. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		// 가공문서로 만들 수 있는 가공품 인지 확인
		if (doc->m_itemProto->getItemNum0() != proto->getItemNum0() || doc->m_itemProto->getItemNum1() != proto->getItemNum1())
		{
			LOG_ERROR("not equal result item by use item. char_index[%d] result_item[%d] use_item", ch->m_index, packet->resultItemDBIndex, doc->getDBIndex());
			throw 0;
		}

		// 여기에서 NUM하고 아이템 프로토의 10개 세팅 상태하고 검사
		if (packet->listCount <= 0 || packet->listCount > MAX_MAKE_ITEM_MATERIAL)
		{
			LOG_ERROR("list count is invalid. char_index[%d] list_count[%d]", ch->m_index, packet->listCount);
			throw 0;
		}

		{
			int count = 0;
			for (int i = 0; i < MAX_MAKE_ITEM_MATERIAL; ++i)
			{
				if (proto->getItemMaterialIndex(i) != -1)
					++count;
			}

			if (count != packet->listCount)
			{
				LOG_ERROR("list count is not equal. char_index[%d] db_list_count[%d] packet_list_count[%d]", ch->m_index, count, packet->listCount);
				throw 1;
			}
		}

// [101216: selo] 보상 아이템 드롭 수정
		if (ch->CheckInvenForProcessPrize(proto) == false)
		{
			LOG_ERROR("check is fail.");
			return;
		}

		// 재료 하나하나 프로토의 재료와 비교 (DB Index, Count)
		for (int i = 0; i < packet->listCount; ++i)
		{
			if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
			{
				LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("invalid packet");
				return;
			}

			CItem* material = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
			if (material == NULL)
			{
				// 해당 위치에 재료 없음
				LOG_ERROR("HACKING? : not found material item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				throw 1;
			}

			bool bCondition = false;

			for (int j = 0; j < MAX_MAKE_ITEM_MATERIAL; ++j)
			{
				if (proto->getItemMaterialIndex(j) == -1)
					continue;

				// 재료의 인덱스와 가공품의 재료 인덱스가 같으면
				if (material->getDBIndex() == proto->getItemMaterialIndex(j))
				{
					// 원하는 재료의수가 같은지
					if (packet->list[i].count != proto->getItemMaterialCount(j))
						throw 1;
					// 수량 확인
					if (material->Count() >= proto->getItemMaterialCount(j))
						bCondition = true;
				}
			}

			if (!bCondition)
			{
				// 프로트의 재료와 다르다
				throw 1;
			}
		}

		// 시료 생성 : 6%
		if (GetRandom(1, 10000) <= MAKE_SAMPLE_ON_PROCESS_PROB)
		{
			// 하드코딩 : 시료를 만들어 인벤으로
			int makeIdx = -1;

			switch (proto->getItemNum0())
			{
			// 채광
			case IETC_PROCESS_STONE:
				makeIdx = GetRandom(0, 1) ? 503 : 504;
				break;

			// 채집
			case IETC_PROCESS_PLANT:
				makeIdx = GetRandom(0, 1) ? 502 : 504;
				break;

			// 차지
			case IETC_PROCESS_ELEMENT:
				makeIdx = GetRandom(0, 1) ? 496 : 502;
				break;

			default:
				break;
			}

			CItem* item = gserver->m_itemProtoList.CreateItem(makeIdx, -1, 0, 0, 1);

			if (!item)
				goto SAMPLE_SKIP;

			if (ch->m_inventory.addItem(item) == false)
			{
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				CNetMsg::SP rmsg(new CNetMsg);
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, item);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}

			// Item LOG
			GAMELOG << init("ITEM_PROCESS_SAMPLE", ch)
					<< itemlog(item)
					<< end;
		}

SAMPLE_SKIP:

		// 무게 최대치 확인! 재료무게 총합이 만드려는 아이템 무게보다 작고 무게 오바시
		// bs 수정	: weight(재료), proto(생성 아이템)
		//			: ch->m_weight - weight + proto->m_weight => 최종 무게
		//			: ch->m_maxWeight * 15 / 10 => 최대 소지 무게
		// 가공문서 없애기
		// 수량 변경
		ch->m_inventory.decreaseItemCount(doc, 1);

		// 아이템의 필요 스킬 확인 && SSkill 로 확률 굴리기
		CSSkillProto* sskillProto  = gserver->m_sSkillProtoList.FindProto(proto->getItemNeedSSkillIndex());
		if (!sskillProto)
			throw 0;

		CSSkillNode* node = ch->m_sSkillList.m_head;
		while (node)
		{
			// 필요한 스킬에 우선순위가 있고
			if (sskillProto->m_preference != -1)
			{
				// 타입이 같고 필요 스킬보다 우선순위가 같거나 높고 필요레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_type == sskillProto->m_type &&
						node->m_sskill->m_proto->m_preference >= sskillProto->m_preference &&
						node->m_sskill->m_level >= proto->getItemNeedSSkillLevel())
					break;
			}
			// 운선순위가 없으면
			else
			{
				// 인덱스가 같고 필요 레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_index == proto->getItemNeedSSkillIndex() &&
						node->m_sskill->m_level >= proto->getItemNeedSSkillLevel())
					break;
			}

			node = node->m_next;
		}

		// 필요 가공 스킬이 없음
		if (!node)
			throw 2;
		// 재료 없애기
		for (int i = 0; i < packet->listCount; ++i)
		{
			CItem* material = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
			if (!material)
				throw 0;

			// 수량 변경
			ch->m_inventory.decreaseItemCount(material, packet->list[i].count);
		}

		int prob = GetRandom(1, 10000);
		//  [2/23/2007 KwonYongDae] 확률 100%
		// 실행중인 퀘스트가 가공 퀘스트인경우
		CQuest *pQuest = NULL;
		if( ch->m_questList.IsQuestType0( QTYPE_KIND_PROCESS_EXPERIENCE ) )
		{
			int	iQuestIndex = 0;

			switch( packet->resultItemDBIndex )
			{
			case 214 :
				iQuestIndex = 150;
				break;		// 구리 - 스톤정련 체험
			case 239 :
				iQuestIndex = 151;
				break;		// E등급 불의 결정 - 원소 정제 체험
			case 251 :
				iQuestIndex = 152;
				break;		// 크락 추출액 E - 식물 가공 체험
			}

			pQuest = ch->m_questList.FindQuest(iQuestIndex);
			if( pQuest != NULL && !pQuest->IsCompleted() )
			{
				prob = 0;	//100% 성공률
			}
		}

		if (prob > node->m_sskill->m_proto->m_num0[node->m_sskill->m_level - 1])
		{
			// 확룰 실패
			throw 3;
		}

		// 가공품 만들어 인벤으로
		CItem* item = gserver->m_itemProtoList.CreateItem(packet->resultItemDBIndex, -1, 0, 0, 1);
		if (item == NULL)
		{
			LOG_ERROR("can't make result item. char_index[%d] result_item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		if (ch->m_inventory.addItem(item) == false)
		{
			// 인젠토리 꽉차서 못 받을 때 Drop
			item = ch->m_pArea->DropItem(item, ch);
			if (!item)
				return;

			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		// Item LOG
		GAMELOG << init("ITEM_PROCESS", ch)
				<< itemlog(item)
				<< end;

		{
			// 성공 메시지 보내기
			CNetMsg::SP rMsg(new CNetMsg);	// 결과 메세지
			ItemProcessRepMsg(rMsg, MSG_PROCESS_SUCCESS);
			SEND_Q(rMsg, ch->m_desc);
		}

		{
			// 가공 성공 이펙트 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_ITEM_PROCESS);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		ch->m_questList.CheckComplete( ch );
	}

	catch (const int err)
	{
		CNetMsg::SP rmsg(new CNetMsg);

		// 오류 메시지 보내기
		switch (err)
		{
		case 0:			// 있어서는 안될 오류
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_SYSTEM);
			break;
		case 1:			// 재료 정보 잘못됨
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_MATERIAL);
			break;
		case 2:			// 가공 스킬이 없음
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_NO_SSKILL);
			break;
		case 3:			// 확률 실패
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_PROB);
			break;
		case 4:			// 무게초과로 인한 실패
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_OVER_WEIGHT);

		default:
			ItemProcessRepMsg(rmsg, MSG_PROCESS_FAIL_SYSTEM);
			break;
		}

		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemMakeReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemMake* packet = reinterpret_cast<RequestClient::doItemMake*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->useitem_tab, packet->useitem_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	try
	{
		// 사용한 제작문서 확인
		CItem* doc = ch->m_inventory.getItem(packet->useitem_tab, packet->useitem_invenIndex);
		if (doc == NULL)
		{
			LOG_ERROR("HACKING? : not found use item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
			ch->m_desc->Close("not found use item");
			return;
		}

		if (doc->m_itemProto->getItemTypeIdx() != ITYPE_ONCE)
		{
			LOG_ERROR("HACKING? : use item is not ITYPE_ONCE");
			ch->m_desc->Close("use item is not ITYPE_ONCE");
			return;
		}

		// 무기방어구 제작문서도 포션제작문서도 아니면
		if (doc->m_itemProto->getItemSubTypeIdx() != IONCE_MAKE_TYPE_DOC && doc->m_itemProto->getItemSubTypeIdx() != IONCE_MAKE_POTION_DOC)
		{
			LOG_ERROR("HACKING? : use item proto is not IONCE_MAKE_TYPE_DOC or IONCE_MAKE_POTION_DOC");
			ch->m_desc->Close("use item proto is not IONCE_MAKE_TYPE_DOC or IONCE_MAKE_POTION_DOC");
			return;
		}

		// 만드려는 아이템 확인
		CItemProto* proto = gserver->m_itemProtoList.FindIndex(packet->resultItemDBIndex);
		if (proto == NULL)
		{
			LOG_ERROR("HACKING? : not found result item proto. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			ch->m_desc->Close("not found result item proto");
			return;
		}

		// 무기나 방어구 포션 아니면 return
		if (proto->getItemTypeIdx() != ITYPE_WEAPON && proto->getItemTypeIdx() != ITYPE_WEAR && proto->getItemTypeIdx() != ITYPE_POTION)
		{
			LOG_ERROR("result item is not ITYPE_WEAPON or ITYPE_WEAR or ITYPE_POTION. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		// 만들수 있지 않으면
		if (!(proto->getItemFlag() & ITEM_FLAG_MADE))
		{
			LOG_ERROR("result item flag is not ITEM_FLAG_MADE. char_index[%d] item_db_index[%d]", ch->m_index, packet->resultItemDBIndex);
			throw 0;
		}

		// 제작문서로 만들 수 있는 아이템 타입 인지 확인
		if (doc->m_itemProto->getItemNum0() != proto->getItemTypeIdx() || !(doc->m_itemProto->getItemNum1() & (1 << proto->getItemSubTypeIdx())))
		{
			LOG_ERROR("not equal result item by use item. char_index[%d] result_item[%d] use_item", ch->m_index, packet->resultItemDBIndex, doc->getDBIndex());
			throw 0;
		}

		// 포션인경우 만들 수 있는 등급인지 확인
		if (proto->getItemTypeIdx() == ITYPE_POTION)
		{
			if (doc->m_itemProto->getItemNum2() != proto->getItemNum2())
			{
				LOG_ERROR("not equal itemNum2. char_index[%d] result_itemNum2[%d] use_itemNum2[%d]", ch->m_index, proto->getItemNum2(), doc->m_itemProto->getItemNum2());
				throw 0;
			}
		}
		// 무기, 방어구 제작문서로 만들 수 있는 아이템 레벨 인지 확인
		else
		{
			if (doc->GetItemLevel() != proto->GetItemProtoLevel())
			{
				LOG_ERROR("not equal item level. char_index[%d] result_level[%d] use_level[%d]", ch->m_index, proto->getItemNum2(), doc->m_itemProto->getItemNum2());
				throw 0;
			}
		}

// [110125: selo] 보상 아이템 드롭 수정
		int nItemCount = 1;
		// 포션이면 갯수만큼
		if (doc->m_itemProto->getItemSubTypeIdx() == IONCE_MAKE_POTION_DOC)
			nItemCount = doc->m_itemProto->getItemNum3();
		if( !ch->CheckInvenForPrize(proto, 0, 0, nItemCount) )
		{
			return;
		}

		if (packet->listCount <= 0 || packet->listCount > MAX_MAKE_ITEM_MATERIAL)
		{
			LOG_ERROR("HACKING? : list count is invalid. char_index[%d] list_count[%d]", ch->m_index, packet->listCount);
			ch->m_desc->Close("list count is invalid");
			return;
		}

		{
			// 제작 아이템 재료수와 비교
			int count = 0;
			for (int i = 0; i < MAX_MAKE_ITEM_MATERIAL; ++i)
			{
				if (proto->getItemMaterialIndex(i) != -1)
					++count;
			}

			if (count != packet->listCount)
			{
				LOG_ERROR("HACKING?: list count is not equal. char_index[%d] db_list_count[%d] packet_list_count[%d]", ch->m_index, count, packet->listCount);
				ch->m_desc->Close("list count is not equal");
				return;
			}
		}

		// 재료 하나하나 프로토의 재료와 비교 (DB Index, Count)
		item_search_t vec[MAX_MAKE_ITEM_MATERIAL];
		for (int i = 0; i < packet->listCount; ++i)
		{
			int sc = ch->m_inventory.searchItemByDBIndex(packet->list[i].dbIndex, vec[i]);
			if (sc == 0)
			{
				LOG_ERROR("HACKING? : not found item. charIndex[%d] itemDBIndex[%d]", ch->m_index, packet->list[i].dbIndex);
				ch->m_desc->Close("not found item");
				return;
			}

			for (int j = 0; j < MAX_MAKE_ITEM_MATERIAL; ++j)
			{
				if (proto->getItemMaterialIndex(j) == -1)
					continue;

				// 재료의 인덱스와 아이템 재료의 인덱스가 같지 않으면
				if (vec[i][0].pItem->getDBIndex() != proto->getItemMaterialIndex(j))
					continue;

				// 원하는 재료의수가 같은지
				if (packet->list[i].count != proto->getItemMaterialCount(j))
				{
					LOG_ERROR("HACKING? : not equal count. charIndex[%d]", ch->m_index);
					ch->m_desc->Close("not equal count");
					return;
				}

				// 수량 확인
				if (sc < proto->getItemMaterialCount(j))
				{
					LOG_ERROR("HACKING? : not enought item count. charIndex[%d]", ch->m_index);
					ch->m_desc->Close("not enought item count");
					return;
				}

				break;
			}
		}

		// 아이템의 필요 스킬 확인 && SSkill 로 확률 굴리기
		CSSkillProto* sskillProto  = gserver->m_sSkillProtoList.FindProto(proto->getItemNeedSSkillIndex());

		// 없으면
		if (!sskillProto)
			throw 0;

		// sskillProto
		CSSkillNode* node = ch->m_sSkillList.m_head;
		node = ch->m_sSkillList.m_head;

		while (node)
		{
			// 필요한 스킬에 우선순위가 있고
			if (sskillProto->m_preference != -1)
			{
				// 타입이 같고 필요 스킬보다 우선순위가 같거나 높으면 선택
				if (node->m_sskill->m_proto->m_type == sskillProto->m_type &&
						node->m_sskill->m_proto->m_preference >= sskillProto->m_preference)
					break;
			}
			// 운선순위가 없으면
			else
			{
				// 인덱스가 같고 필요 레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_index == proto->getItemNeedSSkillIndex() &&
						node->m_sskill->m_level >= proto->getItemNeedSSkillLevel())
					break;
			}

			node = node->m_next;
		}

		// 필요 제작 스킬이 없음
		if (!node)
			throw 2;

		// 재료 없애기
		for (int i = 0; i < packet->listCount; ++i)
		{
			ch->m_inventory.deleteItem(vec[i], packet->list[i].count);
		}

		int successProb;
		// 포션
		if (doc->m_itemProto->getItemSubTypeIdx() == IONCE_MAKE_POTION_DOC)
			successProb= GetMakeProb(node->m_sskill->m_proto->m_num0[node->m_sskill->m_level-1], proto->getItemNum2()) + node->m_sskill->m_proto->m_num1[node->m_sskill->m_level-1];
		else
			successProb= GetMakeProb(node->m_sskill->m_proto->m_num0[node->m_sskill->m_level-1], proto->GetItemProtoLevel()) + node->m_sskill->m_proto->m_num1[node->m_sskill->m_level-1];

		// 최대 성공률 80%
		if (successProb > MAX_ITEM_MAKE_SUCCESS_PROB)
			successProb = MAX_ITEM_MAKE_SUCCESS_PROB;

		int prob = GetRandom(1, 10000);

		//  [2/23/2007 KwonYongDae] 확률 100%
		// 실행중인 퀘스트가 제작 퀘스트인경우
		CQuest *pQuest = NULL;
		if( ch->m_questList.IsQuestType0( QTYPE_KIND_MAKE_EXPERIENCE ) )
		{
			int	iQuestIndex = 0;

			switch( packet->resultItemDBIndex )
			{
			case 308 :
				iQuestIndex = 154;
				break;		// 타바르 - 무기제작 체험
			}

			pQuest = ch->m_questList.FindQuest(iQuestIndex);
			if( pQuest != NULL && !pQuest->IsCompleted() )
			{
				prob = 0;	//100% 성공률
			}
		}
		// 확룰 실패
		if ( prob > successProb)
			throw 3;

		// 아이템 만들어 인벤으로
		CItem* item = NULL;

		// 포션이면 갯수만큼
		if (doc->m_itemProto->getItemSubTypeIdx() == IONCE_MAKE_POTION_DOC)
			item = gserver->m_itemProtoList.CreateItem(packet->resultItemDBIndex, -1, 0, 0, doc->m_itemProto->getItemNum3());
		else
			item = gserver->m_itemProtoList.CreateItem(packet->resultItemDBIndex, -1, 0, 0, 1);

		if (!item)
			throw 0;

		// 방어구 무기 제작문서의 경우 확률 실패해도 재료만 사라진다 : 제작문서 없애기
		// 수량 변경
		ch->m_inventory.decreaseItemCount(doc, 1);

		if (ch->m_inventory.addItem(item) == false)
		{
			// 인젠토리 꽉차서 못 받을 때 Drop
			item = ch->m_pArea->DropItem(item, ch);
			if (!item)
				return;

			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		// Item LOG
		GAMELOG << init("ITEM_MAKE", ch)
				<< itemlog(item)
				<< end;

		{
			// 성공 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMakeRepMsg(rmsg, MSG_MAKE_SUCCESS);
			SEND_Q(rmsg, ch->m_desc);
		}

		{
			// 제작 성공 이펙트 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_ITEM_MAKE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		ch->m_questList.CheckComplete( ch );		// 퀘스트 완료 체크
	}

	catch (const int err)
	{
		CNetMsg::SP rmsg(new CNetMsg);

		// 오류 메시지 보내기
		switch (err)
		{
		case 0:			// 있어서는 안될 오류
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_SYSTEM);
			break;
		case 1:			// 재료 정보 잘못됨
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_MATERIAL);
			break;
		case 2:			// 제작 스킬이 없음
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_NO_SSKILL);
			break;
		case 3:			// 확률 실패
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_PROB);
			break;
		case 4:			// 현재 스킬로 제작 불가능 아이템
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_ITEM_LEVEL);
			break;
		case 5:			// 무게초과
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_OVER_WEIGHT);
			break;

		default:
			ItemMakeRepMsg(rmsg, MSG_MAKE_FAIL_SYSTEM);
			break;
		}

		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemMixReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemMix* packet = reinterpret_cast<RequestClient::doItemMix*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->useitem_tab, packet->useitem_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* use = ch->m_inventory.getItem(packet->useitem_tab, packet->useitem_invenIndex);
	// 재활의 상자인가
	if (use == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (use->m_itemProto->getItemTypeIdx() != ITYPE_ONCE || use->m_itemProto->getItemSubTypeIdx() != IONCE_BOX || use->m_itemProto->getItemNum0() != IONCE_BOX_REMAKE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixRepMsg(rmsg, MSG_MIX_FAIL_REMAKEBOX);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 악세사리 확인 작업
	CItem* oItem[MAX_ACCESSORY_MIX] = { NULL, };

	// 옵션 한개 붙은 악세사리가 있다
	bool bOption = false;

	for (int i = 0; i < MAX_ACCESSORY_MIX; ++i)
	{
		if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("invalid packet");
			return;
		}

		oItem[i] = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
		if (oItem[i] == NULL)
		{
			LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("not found item");
			return;
		}

		// 아이템이 없거나 악세사리가 아니거나 착용하고 있거나
		if (!(oItem[i]->IsAccessary()) || oItem[i]->getWearPos() != WEARING_NONE || oItem[i]->m_itemProto->getItemFlag() & ITEM_FLAG_CASH)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMixRepMsg(rmsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if(oItem[i]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMixRepMsg(rmsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if (oItem[i]->IsRareItem())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMixRepMsg(rmsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if (oItem[i]->IsOriginItem())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMixRepMsg(rmsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 옵션이 1개인 아이템이 있다
		if (oItem[i]->m_nOption == 1)
			bOption = true;
	}

	// 재활의 상자 없애기
	// 수량 변경
	ch->m_inventory.decreaseItemCount(use, 1);

	// MAX_ACCESSORY_MIX 중 하나 선택
	int idx = GetRandom(0, MAX_ACCESSORY_MIX - 1);

	// 새로 생기는 아이템
	CItem* item = gserver->m_itemProtoList.CreateItem(oItem[idx]->m_itemProto->getItemIndex(), -1, 0, 0, 1);

	if (!item)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixRepMsg(rmsg, MSG_MIX_FAIL_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int num = 0;	// 옵션 붙는 갯수

	if (bOption)
		num = GetRandom(2, 4);
	else
		num = GetRandom(3, 5);

	boost::scoped_array<int> type(new int[num]);

	LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

	// 옵션 타입 결정
	int i = 0;
	while (i < num)
	{
		// type[i] 결정
		type[i] = GetRandom(0, maxnumoption - 1);

		// type[i]가 중복인 검사
		// 중복이면 type[i]를 1씩 증가하면서 중복 검사
		int j = 0;
		for (j = 0; j < maxnumoption; j++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
				break;
		}

		// 더 이상 옵션 붙일 수 없음
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
		if (!((1 << item->m_itemProto->getItemSubTypeIdx()) & proto->m_accessoryType))
			continue;

		i++;
	}

	// 옵션 부여
	for (int i = 0; i < num; ++i)
	{
		COptionProto* proto = gserver->m_optionProtoList.FindProto(type[i]);

		if (!proto)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemMixRepMsg(rmsg, MSG_MIX_FAIL_SYSTEM);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// Level, Value 셋팅
		item->m_option[item->m_nOption].MakeOptionValue(proto, ch->m_level, num);
		++item->m_nOption;
	}

	// 악세사리 삭제하기
	for (int i = 0; i < MAX_ACCESSORY_MIX; ++i)
	{
		ch->m_inventory.decreaseItemCount(oItem[i], 1);
	}

	// 새 악세사리 인벤에 넣기
	if (ch->m_inventory.addItem(item) == false)
	{
		// 인젠토리 꽉차서 못 받을 때 Drop
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
			return;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}

	// Item LOG
	GAMELOG << init("ITEM_MIX", ch)
			<< itemlog(item)
			<< end;

	{
		// 성공 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixRepMsg(rmsg, MSG_MIX_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemArcaneReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemArcane* packet = reinterpret_cast<RequestClient::doItemArcane*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->useitem_tab, packet->useitem_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* use = ch->m_inventory.getItem(packet->useitem_tab, packet->useitem_invenIndex);
	// 비밀의 상자인가
	if (use == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->useitem_tab, packet->useitem_invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (use->m_itemProto->getItemTypeIdx() != ITYPE_ONCE || use->m_itemProto->getItemSubTypeIdx() != IONCE_BOX || use->m_itemProto->getItemNum0() != IONCE_BOX_ARCANE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_ARCANEBOX);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 재료 확인 작업 : 클라이언트가 보낸 그대로의 순서
	CItem* oItem[MAX_ARCANE_MIX] = { NULL, };

	bool bMaterial[MAX_ARCANE_MIX];
	memset(bMaterial, false, sizeof(bool) * MAX_ARCANE_MIX);

	for (int i = 0; i < MAX_ARCANE_MIX; ++i)
	{
		if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("invalid packet");
			return;
		}

		oItem[i] = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
		if (oItem[i] == NULL)
		{
			LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("not found item");
			return;
		}

		// 아이템이 없거나 착용하고 있거나
		if (oItem[i]->getWearPos() != WEARING_NONE)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_MATERIAL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if(oItem[i]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_MATERIAL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		switch (i)
		{
		case ITEM_ARCANE_MATERIAL_UPGRADE:
			// 제련석이 아니다
			if (oItem[i]->m_itemProto->getItemTypeIdx() != ITYPE_ETC || oItem[i]->m_itemProto->getItemSubTypeIdx() != IETC_UPGRADE || oItem[i]->m_itemProto->getItemNum0() != IETC_UPGRADE_GENERAL || oItem[i]->getFlag() == 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_UPGRADE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			break;

		case ITEM_ARCANE_MATERIAL_ACCESSORY:
			// 악세사리가 아니다
			if (!oItem[i]->IsAccessary())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_ACCESSORY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			break;

		case ITEM_ARCANE_MATERIAL_SAMPLE:
			// 시료가 아니다
			if (oItem[i]->m_itemProto->getItemTypeIdx() != ITYPE_ETC || oItem[i]->m_itemProto->getItemSubTypeIdx() != IETC_SAMPLE)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_SAMPLE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			break;
		}
	}

	// 비밀의 상자 없애기
	// 수량 변경
	ch->m_inventory.decreaseItemCount(use, 1);

	// 새로 생기는 아이템 : 포션
	CItem* item = gserver->m_itemProtoList.CreateItem(oItem[ITEM_ARCANE_MATERIAL_SAMPLE]->m_itemProto->getItemNum0(), -1, 0, 0, 1);

	if (!item)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemArcaneRepMsg(rmsg, MSG_ARCANE_FAIL_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 포션의 스킬레벨 셋팅
	if (oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->getFlag() % 5 == 0)
		item->setFlag(oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->getFlag() / 5);
	else
		item->setFlag(oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->getFlag() / 5 + 1);

	// 060111 : BS : 아이템 60레벨 이상은 12레벨로 고정
	if (item->getFlag() > 12)
		item->setFlag(12);

	// 재료 삭제하기
	for (int i = 0; i < MAX_ARCANE_MIX; ++i)
	{
		ch->m_inventory.decreaseItemCount(oItem[i], 1);
	}

	if (ch->m_inventory.addItem(item) == false)
	{
		// 인젠토리 꽉차서 못 받을 때 Drop
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
			return;

		CNetMsg::SP rmsg(new CNetMsg);
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, item);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	// Item LOG
	GAMELOG << init("ITEM_ARCANE", ch)
			<< itemlog(item)
			<< end;

	{
		// 성공 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		ItemArcaneRepMsg(rmsg, MSG_ARCANE_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemChangeWeaponReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemChangeWeapon* packet = reinterpret_cast<RequestClient::doItemChangeWeapon*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* olditem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (olditem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return ;
	}

	if (olditem->getVIndex() != packet->virtualIndex)
	{
		LOG_ERROR("HACKING? : not equal vIndex. char_index[%d] itemVIndex[%d], packetVIndex[%d]", ch->m_index, olditem->getVIndex(), packet->virtualIndex);
		ch->m_desc->Close("not equal vIndex");
		return ;
	}
	
	if( CEquipExchangeExtend::instance()->checkValid(olditem, packet->changeType) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* goldToken;
	bool isRare = false;

	if(olditem->IsRareItem() == true)
	{
		goldToken = ch->m_inventory.FindByVirtualIndex(packet->goldTokenItemVIndex);
		if(goldToken == NULL)
		{
			LOG_ERROR("HACKING? : Wrong Gold Token. char_index[%d]", ch->m_index);
			ch->m_desc->Close("Wrong Gold Token");
			return ;
		}
		if(goldToken->getDBIndex() != 5124)
		{
			LOG_ERROR("HACKING? : Wrong Gold Token. char_index[%d]", ch->m_index);
			ch->m_desc->Close("Wrong Gold Token");
			return ;
		}		
		isRare = true;
	}

	int goldTokenCount = 0;

	if(isRare == true)
	{
		
		int itemLevel = olditem->m_itemProto->getItemLevel();
		
		if(itemLevel >= 70 && itemLevel <= 100)
			goldTokenCount = 1;

		else if(itemLevel >= 101 && itemLevel <= 130)
			goldTokenCount = 2;

		else if(itemLevel >= 131 && itemLevel <= 140)
			goldTokenCount = 3;

		else if(itemLevel >= 141 && itemLevel <= 150)
			goldTokenCount = 4;

		else if(itemLevel >= 151 && itemLevel <= 175)
			goldTokenCount = 5;
		else
		{
			LOG_ERROR("HACKING? : Item LevelInfo. itemLevel[%d]", itemLevel);
			ch->m_desc->Close("Item LevelInfo");
			return ;
		}
		
		if(goldTokenCount != packet->goldTokenCount)
		{
			LOG_ERROR("HACKING? : Wrong Gold Token Count. goldTokenCount[%d], packetCounct[%d]", ch->m_index, packet->goldTokenCount);
			ch->m_desc->Close("Wrong Gold Token Count");
			return ;
		}

		if(goldToken->getItemCount() < goldTokenCount)
		{
			LOG_ERROR("HACKING? : Not Enough Gold Token. userCount[%d], packetCount[%d]", goldToken->getItemCount(), packet->goldTokenCount);
			ch->m_desc->Close("Not Enough Gold Token");
			return ;
		}
	}
	
	LONGLONG needmoney = CEquipExchangeExtend::instance()->calNeedNas(olditem, isRare);

	if (needmoney > 0 && ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_NOT_ENOUGH_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(CEquipExchangeExtend::instance()->find_(olditem, packet->exchangeDBIndex, olditem->m_itemProto->getItemSubTypeIdx()) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	int nNewPlus = olditem->getPlus();
	int nNewFlag = olditem->getFlag() & ~FLAG_ITEM_OPTION_ENABLE;

	CItem* newitem = gserver->m_itemProtoList.CreateItem(packet->exchangeDBIndex, -1, nNewPlus, nNewFlag, 1);
	if (newitem == NULL)
	{
		LOG_ERROR("Not Exist Item. ITEM DB INDEX[%d]", packet->exchangeDBIndex);
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	if(newitem->IsSocketItem() && olditem->IsSocketItem())
	{
		int newitemFlag = newitem->getFlag();
		newitem->m_socketList.CopySocketInfo(olditem->m_socketList, newitemFlag);
		newitem->setFlag(newitemFlag);
	}

	GAMELOG << init("ITEM CHANGE WEAPON", ch)
			<< "NEED MONEY" << delim
			<< needmoney << delim
			<< "OLD ITEM" << delim
			<< itemlog(olditem) << delim
			<< "NEW ITEM" << delim
			<< itemlog(newitem)
			<< end;

	int olddb = olditem->m_itemProto->getItemIndex();
	int newdb = newitem->m_itemProto->getItemIndex();

	if (ch->m_inventory.addItem(newitem))
	{
		ch->m_inventory.decreaseItemCount(olditem, 1);
		if(isRare == true)
			ch->m_inventory.decreaseItemCount(goldToken, goldTokenCount);

		ch->m_inventory.decreaseMoney(needmoney);
	}
	else
	{
		LOG_ERROR("ITEM ADD FAIL. ITEM_INDEX[%d]", packet->exchangeDBIndex);
		delete newitem;

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_INVENTORY_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, olddb, newdb, ResponseClient::EXCHANGE_ERR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemProcessNPC(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemProcessNpc* packet = reinterpret_cast<RequestClient::doItemProcessNpc*>(msg->m_buf);

	CItemProto* proto = gserver->m_itemProtoList.FindIndex(packet->itemDBIndex);
	if (proto == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] db_index[%d]", ch->m_index, packet->itemDBIndex);
		return ;
	}

	if (proto->getItemTypeIdx() != ITYPE_ETC || proto->getItemSubTypeIdx() != IETC_PROCESS)
	{
		LOG_ERROR("HACKING? : proto is not ITYPE_ETC or IETC_PROCESS. char_index[%d] db_index[%d]", ch->m_index, packet->itemDBIndex);
		return ;
	}

	// 확률 구하기 : a_level1_num0를 확률로 사용 (m_num0[1])
	int sskillindex = proto->getItemNeedSSkillIndex();
	CSSkillProto* skillproto = gserver->m_sSkillProtoList.FindProto(sskillindex);
	if (skillproto == NULL)
	{
		LOG_ERROR("not found skill. char_index[%d] skill_index[%d]", ch->m_index, sskillindex);
		return ;
	}

	int skillprob = skillproto->m_num0[1];
	if (packet->count < 1 || packet->count	 > 10)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 필요한 재료 구하기 : 가공품은 필요 재료를 무조건 0번 인덱스에 저장
	int needItemIndex = proto->getItemMaterialIndex(0);
	int needItemCount = proto->getItemMaterialCount(0);
	if (needItemIndex < 1 || needItemCount < 1)
		return ;

	LONGLONG totalCount = packet->count * needItemCount;

	// 재료를 가지고 있는지 검사
	item_search_t vec;
	int sc = ch->m_inventory.searchItemByCondition(needItemIndex, 0, 0, vec);
	if (sc == 0 || sc < totalCount)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 종류 구하기 : 가공품의 종류는 a_num0에 저장되고 스톤, 크락, 원소 순으로
	int type = proto->getItemNum0();
	// 등급 구하기 : 가공품의 등급은 a_num1에 저장되고 E 등급이 0으로 시작해서 A 등급이 4로 끝남
	int grade = proto->getItemNum1();

	if (type < 0 || type >= 3 || grade < 0 || grade >=5)
		return ;

	// type과 grade에 해당하는 수수료 테이블
	int pricetable[3][5] =
	{
		{10, 21, 40, 63, 94},		// 스톤
		{10, 21, 40, 63, 94},		// 크락
		{10, 21, 40, 63, 94}		// 원소
	};

	// 소유금액 검사
	LONGLONG needmoney = pricetable[type][grade] * totalCount;
	//bool bDeleteMoney = false;

	if (ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템 감소
	ch->m_inventory.deleteItem(vec, totalCount);

	// 확률로 성공 판단
	if (GetRandom(1, 10000) <= skillprob)
	{
		// item 생성
		CItem* item = gserver->m_itemProtoList.CreateItem(packet->itemDBIndex, -1, 0, 0, packet->count);
		if (!item)
		{
			GAMELOG << init("ITEM PROCESS NPC ERROR", ch)
					<< "ITEM" << delim
					<< packet->itemDBIndex << delim
					<< "COUNT" << delim
					<< packet->count
					<< end;
			CNetMsg::SP rmsg(new CNetMsg);
			ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		ch->m_inventory.decreaseMoney(needmoney);

		if (ch->m_inventory.addItem(item) == false)
		{
			// 땅에 버리기
			ch->m_pArea->DropItem(item, ch);
			item->m_preferenceIndex = ch->m_index;
			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_OK);
			SEND_Q(rmsg, ch->m_desc);
		}

		GAMELOG << init("ITEM PROCESS NPC", ch)
				<< "ITEM" << delim
				<< proto->getItemIndex() << delim
				<< proto->getItemName() << delim
				<< "SERIAL" << delim
				<< item->m_serial.c_str() << delim
				<< "COUNT" << delim
				<< packet->count << delim
				<< "TOTAL" << delim
				<< item->Count()
				<< end;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("ITEM PROCESS NPC FAIL", ch)
				<< "ITEM" << delim
				<< proto->getItemIndex() << delim
				<< proto->getItemName() << delim
				<< "COUNT" << delim
				<< packet->count
				<< end;
	}
}

void do_ItemAddBooster(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemAddBooster* packet = reinterpret_cast<RequestClient::doItemAddBooster*>(msg->m_buf);

	// 부스터 아이템 찾기
	CItem* boost = ch->m_inventory.FindByVirtualIndex(packet->boostitem_vitualIndex);
	if (boost == NULL)
	{
		LOG_ERROR("not found boost item. char_index[%d] boost_item_virtual_index[%d]", ch->m_index, packet->boostitem_vitualIndex);
		return ;
	}

	// 부스터 아이템이 맞나? : a_num0가 IETC_UPGRADE_BOOSTER(3)이어야 부스터
	if (boost->m_itemProto->getItemTypeIdx() != ITYPE_ETC || boost->m_itemProto->getItemSubTypeIdx() != IETC_UPGRADE || boost->m_itemProto->getItemNum0() != IETC_UPGRADE_BOOSTER)
	{
		LOG_ERROR("boost item is not ITYPE_ETC or IETC_UPGRADE or IETC_UPGRADE_BOOSTER, char_index[%d] boost_item_virtual_index[%d]", ch->m_index, packet->boostitem_vitualIndex);
		return ;
	}

	// 착용중인 아이템 검사하기
	CItem* tool = ch->m_wearInventory.wearItemInfo[WEARING_WEAPON];
	if (tool == NULL)
	{
		LOG_ERROR("not found waer item(in add booster). char_index[%d] boost_item_virtual_index[%d]", ch->m_index, packet->boostitem_vitualIndex);
		return ;
	}

	// 무기이고?
	if (tool->m_itemProto->getItemTypeIdx() != ITYPE_WEAPON)
	{
		LOG_ERROR("tool item is not ITYPE_WEAPON char_index[%d] boost_item_virtual_index[%d]", ch->m_index, packet->boostitem_vitualIndex);
		return ;
	}

	// 생산도구이고?
	if (tool->m_itemProto->getItemSubTypeIdx() != IWEAPON_MINING && tool->m_itemProto->getItemSubTypeIdx() != IWEAPON_GATHERING && tool->m_itemProto->getItemSubTypeIdx() != IWEAPON_CHARGE)
	{
		LOG_ERROR("tool item is not IWEAPON_MINING or IWEAPON_GATHERING or IWEAPON_CHARGE. char_index[%d] boost_item_virtual_index[%d]", ch->m_index, packet->boostitem_vitualIndex);
		return ;
	}

	// 부스터 안 붙였나?
	if (tool->getFlag() & FLAG_ITEM_BOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_SILVERBOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_GOLDBOOSTER_ADDED
			|| tool->getFlag() & FLAG_ITEM_PLATINUMBOOSTER_ADDED
	   )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// 내구도 검사
	if (tool->getUsed() < 5)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_USED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 플래그 변경 : 할일 부스터 나오면 거기에 따라 코딩
	switch(boost->m_itemProto->getItemIndex())
	{
	case 786:
		tool->setFlag(tool->getFlag() | FLAG_ITEM_BOOSTER_ADDED);
		break;
	case 1283:
		tool->setFlag(tool->getFlag() | FLAG_ITEM_SILVERBOOSTER_ADDED);
		break;
	case 1284:
		tool->setFlag(tool->getFlag() | FLAG_ITEM_GOLDBOOSTER_ADDED);
		break;
	case 1417:
		tool->setFlag(tool->getFlag() | FLAG_ITEM_PLATINUMBOOSTER_ADDED);
		break;
	default:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_USED);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateWearItemFlag(rmsg, tool->getWearPos(), tool->getVIndex(), tool->getFlag());
		SEND_Q(rmsg, ch->m_desc);
	}

	// 부스터 감소
	ch->m_inventory.decreaseItemCount(boost, 1);

	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ItemMixWarItem(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemMixWar* packet = reinterpret_cast<RequestClient::doItemMixWar*>(msg->m_buf);

	{
		std::set<int> checkdup;
		int key = 0;

		key = makeCheckDupKey(packet->tab_1, packet->invenIndex_1);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}

		key = makeCheckDupKey(packet->tab_2, packet->invenIndex_2);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}

		key = makeCheckDupKey(packet->tab_3, packet->invenIndex_3);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_1, packet->invenIndex_1) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_2, packet->invenIndex_2) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_3, packet->invenIndex_3) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_3, packet->invenIndex_3);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* itemType1 = ch->m_inventory.getItem(packet->tab_1, packet->invenIndex_1);
	CItem* itemType2 = ch->m_inventory.getItem(packet->tab_2, packet->invenIndex_2);
	CItem* itemType3 = ch->m_inventory.getItem(packet->tab_3, packet->invenIndex_3);

	if (itemType1 == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("not found item");
		return;
	}

	if (itemType2 == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("not found item");
		return;
	}

	if (itemType3 == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_3, packet->invenIndex_3);
		ch->m_desc->Close("not found item");
		return;
	}

	// 각 아이템 타입 검사
	// 대분류 기타
	if (itemType1->m_itemProto->getItemTypeIdx() != ITYPE_ETC
			|| itemType1->m_itemProto->getItemTypeIdx() != itemType2->m_itemProto->getItemTypeIdx()
			|| itemType1->m_itemProto->getItemTypeIdx() != itemType3->m_itemProto->getItemTypeIdx())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// type1, type2, type3
	if (itemType1->m_itemProto->getItemSubTypeIdx() != IETC_MIX_TYPE1
			|| itemType2->m_itemProto->getItemSubTypeIdx() != IETC_MIX_TYPE2
			|| itemType3->m_itemProto->getItemSubTypeIdx() != IETC_MIX_TYPE3)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성될 아이템 결정 : type3의 a_num1가 생성될 아이템 인덱스
	int itemindex = itemType3->m_itemProto->getItemNum1();
	// 생성될 옵션 결정 : 각 재료의 a_num0가 옵션 index
	int option1 = itemType1->m_itemProto->getItemNum0();
	int option2 = itemType2->m_itemProto->getItemNum0();
	int option3 = itemType3->m_itemProto->getItemNum0();
	COptionProto* optionProto1 = gserver->m_optionProtoList.FindProto(option1);
	COptionProto* optionProto2 = gserver->m_optionProtoList.FindProto(option2);
	COptionProto* optionProto3 = gserver->m_optionProtoList.FindProto(option3);

	// 생성될 아이템이 조합 아이템인지 검사
	CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(itemindex);
	if (!itemproto || itemproto->getItemTypeIdx() != ITYPE_ACCESSORY || itemproto->getItemSubTypeIdx() == IACCESSORY_PET || !(itemproto->getItemFlag() & ITEM_FLAG_MIX))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// 생성될 옵션이 합당한지 검사
	if (!optionProto1 || !optionProto2 || !optionProto3)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템 생성
	CItem* mixItem = gserver->m_itemProtoList.CreateItem(itemindex, -1, 0, 0, 1);
	if (!mixItem)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// Type1
	mixItem->m_option[0].m_type = optionProto1->m_type;
	mixItem->m_option[0].m_level = 1;
	mixItem->m_option[0].m_proto = optionProto1;
	mixItem->m_option[0].m_value = optionProto1->m_levelValue[0];

	// Type2
	mixItem->m_option[1].m_type = optionProto2->m_type;
	mixItem->m_option[1].m_level = 1;
	mixItem->m_option[1].m_proto = optionProto2;
	mixItem->m_option[1].m_value = optionProto2->m_levelValue[0];

	// Type3
	mixItem->m_option[2].m_type = optionProto3->m_type;
	mixItem->m_option[2].m_level = 1;
	mixItem->m_option[2].m_proto = optionProto3;
	mixItem->m_option[2].m_value = optionProto3->m_levelValue[0];

	mixItem->m_nOption = 3;
	mixItem->m_option[0].SetDBValue();
	mixItem->m_option[1].SetDBValue();
	mixItem->m_option[2].SetDBValue();

	// GAMELOG 남기기

	GAMELOG << init("ITEM MIX WAR", ch)
			<< "TYPE1" << delim
			<< itemType1->m_itemProto->getItemName() << delim
			<< "TYPE2" << delim
			<< itemType2->m_itemProto->getItemName() << delim
			<< "TYPE3" << delim
			<< itemType3->m_itemProto->getItemName() << delim
			<< "MIX ITEM" << delim
			<< itemlog(mixItem);

	// 아이템 추가 및 제거
	ch->m_inventory.decreaseItemCount(itemType1, 1);
	ch->m_inventory.decreaseItemCount(itemType2, 1);
	ch->m_inventory.decreaseItemCount(itemType3, 1);

	if (ch->m_inventory.addItem(mixItem) == false)
	{
		// 못 넣었으면 바닥으로
		GAMELOG << delim << "DROP" << end;

		ch->m_pArea->DropItem(mixItem, ch);
		mixItem->m_preferenceIndex = ch->m_index;
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, mixItem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->m_bCreateMixItem = true;
}
//#endif

// 060227 : bs : 파티리콜
void do_ItemUsePartyRecall(CPC* ch, CItem* pItem)
{
	if (!ch->IsParty())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyRecallErrorMsg(rmsg, MSG_EX_PARTY_RECALL_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(pItem->getDBIndex() == 10803 && ch->m_party->m_warPartyRecall.empty() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyRecallErrorMsg(rmsg, MSG_EX_PARTY_RECALL_AREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 파티 리콜 사용시 좌표 초기화
	ch->getPartyRecallInfo()->resetPartyRecallInfo();

	bool bUseContinent = false;
#ifdef CONTINENT_PARTY_RECALL

	if(pItem->getDBIndex() == 2945 || pItem->getDBIndex() == 2946)
	{
		if((ch->m_pZone->m_iContinent_no == 1 && pItem->getDBIndex() == 2945) || (ch->m_pZone->m_iContinent_no == 0 && pItem->getDBIndex() == 2946)) // 현재대륙에서 사용할 수 없는 아이템 일때.
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyRecallErrorMsg(rmsg, MSG_EX_PARTY_RECALL_ERROR_CANT_USE_THIS_CONTINENT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		bUseContinent = true;
	}
#endif // CONTINENT_PARTY_RECALL

	// 리콜 사용 가능 검사
	if (pItem->getUsed() < gserver->getNowSecond())
		return ;

	// 원정대는 사용 불가.
	if( ch->IsExped())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyRecallErrorMsg(rmsg, MSG_EX_PARTY_RECALL_ERROR_CANT_USE_EXPEND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pZone == NULL)
		return ;

	if( !ch->m_pZone->IsPartyRecallZone() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyRecallErrorMsg(rmsg, MSG_EX_PARTY_RECALL_ERROR_CANT_USE_INSTANT_DUNGEON);
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	///yhj  081023  파티리콜 pvp아레나 존에서는 금지
	if ( ch->m_pZone->m_index == ZONE_PK_TOURNAMENT )
	{
		// 경고 메시지 보내고
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_WARN_PARTYRECALL);
		SEND_Q(rmsg, ch->m_desc);

		//pvp아레나에서 PARTY_RECALL 이동 로그
		GAMELOG << init(" PARTY_RECALL IN PK_TOURNAMENT " , ch ) << end;
		return;
	}

	// 파티원에게 알림 메시지 보내기
	if (gserver->isRunHelper())
	{
		int nGuildIndex = 0;
		char cIsInCastle = 0;

		CWarCastle* pCastle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
		if (pCastle && pCastle->GetState() != WCSF_NORMAL && pCastle->CheckSubServer())
		{
			// 공성 진행중인 서브 서버이면
			// 성안 영역인가 확인하고
			if (pCastle->IsInInnerCastle(ch))
			{
				if(ch->m_pZone->m_index == ZONE_DRATAN || ch->m_pZone->m_index == ZONE_MERAC )
				{
					// 드라탄 공성이나 메라크 공성은 파티 리콜 사용 금지
					// 경고 메시지 보내고
					// 경고 메시지 보내고
					if(pItem->getDBIndex() != 10803)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_WARN_PARTYRECALL);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}

				//공성시간에 PARTY_RECALL 이동 로그
				GAMELOG << init(" PARTY_RECALL IN WARTIME " , ch ) << end;

				// 길드 정보를 구해서
				if (ch->m_guildInfo && ch->m_guildInfo->guild())
					nGuildIndex = ch->m_guildInfo->guild()->index();
				cIsInCastle = 1;
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyRecallPromptMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index, ch->GetName(), cIsInCastle, nGuildIndex, bUseContinent, ch->m_pZone->m_index, ch->m_pos);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	{
		// 결과 보내기
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyRecallNoticeMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
	}

	CParty_recall_manager *pInstance = CParty_recall_manager::Get_instance();
	pInstance->Insert_map(ch->m_index, pItem->getDBIndex());
}

void do_ItemTake_RecommendPotion(CPC* ch, CItem* pItem)
{
	bool bCanTake = false;
	// 30레벨 이하
	if (ch->m_level <= RECOMMEND_SERVER_LEVEL)
	{
		// 우선권 검사
		if (pItem->m_preferenceIndex != -1)
		{
			// 우선권이 있을 때
			if (pItem->m_preferenceIndex == ch->m_index)
			{
				bCanTake = true;
			}
			else if (ch->IsParty())
			{
				// 파티원 우선권 검사
				if (ch->m_party->FindMember(pItem->m_preferenceIndex) != -1)
				{
					bCanTake = true;
				}
			}
		}
		else
		{
			// 우선권 없으면 집을 수 있음
			bCanTake = true;
		}
	}

	if (!bCanTake && ch->m_admin < 2)
	{
		// 우선권이 없습니다 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	else
	{
		// 효과 랜덤 결정
		const int nCountSkill = 6;
		int nSkillIndex[nCountSkill] =
		{
			355,
			356,
			357,
			358,
			359,
			360
		};
		int nIndex = GetRandom(0, nCountSkill - 1);

		CSkill* skill = gserver->m_skillProtoList.Create(nSkillIndex[nIndex], 1);
		bool bSkipApply = false;
		// 060227 : bs : 중복안되는 스킬인지 검사
		if ((skill->m_proto->m_flag & SF_NOTDUPLICATE) && ch->m_assist.FindBySkillIndex(skill->m_proto->m_index))
		{
			{
				/*중복사용 불가 에러 메시지*/
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_DUPLICATE, skill->m_proto->m_index, 0);
				SEND_Q(rmsg, ch->m_desc);
			}
			bSkipApply = true;
		}
		// 확장 포션 : 듀얼 사용중일때 따로 따로 수행 못함
		if (( skill->m_proto->m_index == 350 || skill->m_proto->m_index == 351 ) && ch->m_assist.FindBySkillIndex(352) )
			bSkipApply = true;

		if (!bSkipApply)
		{
			bool bApply;
			ApplySkill(ch, ch, skill, -1, bApply);
		}

		delete skill;

		int cx = pItem->m_cellX;
		int cz = pItem->m_cellZ;
		char yLayer = GET_YLAYER(pItem);
		CArea* pArea = pItem->m_pArea;

		{
			// 포션 사용 이펙트 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			EffectItemMsg(rmsg, ch, pItem);
			pArea->SendToCell(rmsg, ch, true);
		}

		CNetMsg::SP disappmsg(new CNetMsg);
		CNetMsg::SP takemsg(new CNetMsg);

		ItemTakeMsg(takemsg, ch, pItem);
		ItemDisappearMsg(disappmsg, pItem->getVIndex());

		// 우선권 제거
		pItem->m_preferenceIndex = -1;

		// 땅에서 제거
		pArea->ItemFromCell(pItem);

		// 메시지 보내고
		pArea->SendToCell(takemsg, ch, true);
		pArea->SendToCell(disappmsg, yLayer, cx, cz);

		GAMELOG << init("RECOMMEND SERVER ITEM PICK", ch)
				<< "SKILL" << delim
				<< nSkillIndex[nIndex]
				<< end;

		delete pItem;
	}
}


void do_ItemLendWeapon(CPC* ch, CNetMsg::SP& msg)
{
	const static int	nMinPrice			= 1;				// 대여 가능 아이템 최소 금액
	const static int	nMoneyFactorMul		= 3;				// 대여 금액 산출시 아이템 가격에 곱해지는 수
	const static int	nMoneyFactorDiv		= 2;				// 대여 금액 산출시 아이템 가격에 나누는 수
	const static int	nMoneyFactorAdd		= 50000;			// 대여 금액 산출시 기본으로 더해지는 수
#if defined (LC_KOR) || defined(LC_BILA) || defined(LC_RUS) || defined(LC_USA) || defined(LC_TLD)
	const static int	nPlusLendItem		= 12;				// 대여 아이템 기본 Plus 수치
#else
	const static int	nPlusLendItem		= 6;				// 대여 아이템 기본 Plus 수치
#endif
	const static int	nFlagLendItem		= FLAG_ITEM_LENT;	// 대여 아이템 기본 Flag

	void*			posLendItem		= NULL;		// 대여 가능 리스트 검색용
	CItemProto*		pItemProto		= NULL;		// 생성할 아이템 프로토
	LONGLONG		nNeedMoney		= 0;		// 필요 금액
	CItem*			pItem			= NULL;		// 지급할 아이템

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> nItemDBIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) && !npc->IsFlag(NPC_SHOPPER) )
	{
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	RequestClient::doItemLendWeapon* packet = reinterpret_cast<RequestClient::doItemLendWeapon*>(msg->m_buf);

	// 1. 대여가능 리스트에서 찾는다
	// 2. 소지금을 계산한다
	// 3. 소지금 검사
	// 4. 아이템 생성
	// 5. 아이템 지급
	// 6. 소지금 감소
	// 7. 게임 로그
	// 8. 결과 알림

	// 1. 대여가능 리스트에서 찾는다
	std::set<int>::iterator it = gserver->m_itemProtoList.m_listLendItem.find(packet->itemDBIndex);
	if ( (it == gserver->m_itemProtoList.m_listLendItem.end())
			|| (pItemProto = gserver->m_itemProtoList.FindIndex(packet->itemDBIndex)) == NULL
			|| pItemProto->getItemPrice() < nMinPrice)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemLendWeaponMsg(rmsg, packet->itemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 2. 소지금을 계산한다
	nNeedMoney = pItemProto->getItemPrice() * nMoneyFactorMul / nMoneyFactorDiv + nMoneyFactorAdd;

	// 3. 소지금 검사
	if (ch->m_inventory.getMoney() < nNeedMoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemLendWeaponMsg(rmsg, packet->itemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_NOMONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 4. 아이템 생성
	pItem = gserver->m_itemProtoList.CreateItem(packet->itemDBIndex, -1, nPlusLendItem, nFlagLendItem, 1);
	if (!pItem)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemLendWeaponMsg(rmsg, packet->itemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 5. 아이템 지급
	if (ch->m_inventory.addItem(pItem))
	{
		// 6. 소지금 감소
		ch->m_inventory.decreaseMoney(nNeedMoney);

		// 7. 게임 로그
		GAMELOG << init("LEND ITEM", ch)
				<< "MONEY" << delim
				<< nNeedMoney << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;

		{
			// 8. 결과 알림
			CNetMsg::SP rmsg(new CNetMsg);
			ItemLendWeaponMsg(rmsg, packet->itemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_OK);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		delete pItem;

		CNetMsg::SP rmsg(new CNetMsg);
		ItemLendWeaponMsg(rmsg, packet->itemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void TO_BUGDATA(CItem* pItem, unsigned char ucBug[16]);
void FROM_BUGDATA(unsigned char ucBug[16], CItem* pItem);

bool do_ItemTake_EventCollectBug(CPC* ch, CItem* pItem)
{
	if( !gserver->isActiveEvent( A_EVENT_COLLECT_BUG ) )
		return false;

	// 곤충 테이블 : 인덱스, 포인트(번호)
	static const int tableBug[10][2] =
	{
		{1579,  1},		// 벌
		{1580,  2},		// 흰나비
		{1581,  3},		// 호랑나비
		{1582,  4},		// 잠자리
		{1583,  5},		// 고추잠자리
		{1584,  6},		// 매미
		{1585,  7},		// 무당벌레
		{1586,  8},		// 개똥벌레
		{1587,  9},		// 사슴벌레
		{1588, 10},		// 풍뎅이
	};
	static const int	nNormalBoxIndex = 1577;		// 일반 상자 인덱스
	static const int	nGoldenBoxIndex = 1578;		// 골든 상자 인덱스

	CNetMsg			rmsg;
	bool			bCanTake = false;		// 집을 수 있는지 검사
	CItem*			pItemCollectBox = NULL;	// 채집 상자 아이템
	unsigned char	ucBug[16];				// 곤충 데이터
	int				nSpace = 0;				// 채집 상자 빈칸
	int				nRow, nCol;
	int				i;

	memset(ucBug, 0, sizeof(ucBug));

	// 1. 우선권 검사
	// 2. 빈칸있는 채집 상자 찾기
	// 3. 채집 상자에 넣기 : flag에 포인트 증가, plus, used 설정
	// 4. 바닥에서 제거
	// 5. 게임 로그

	// 1. 우선권 검사
	if (pItem->m_preferenceIndex != -1)
	{
		// 우선권이 있을 때
		if (pItem->m_preferenceIndex == ch->m_index)
		{
			bCanTake = true;
		}
		else if (ch->IsParty())
		{
			// 파티원 우선권 검사
			if (ch->m_party->FindMember(pItem->m_preferenceIndex) != -1)
			{
				bCanTake = true;
			}
		}
	}
	else
	{
		// 우선권 없으면 집을 수 있음
		bCanTake = true;
	}
	if (!bCanTake && ch->m_admin < 2)
	{
		// 우선권이 없습니다 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 2. 빈칸있는 채집 상자 찾기
	pItemCollectBox = ch->m_inventory.FindByDBIndex(nGoldenBoxIndex);
	if (pItemCollectBox)
	{
		TO_BUGDATA(pItemCollectBox, ucBug);
		for (nSpace = 0; nSpace < 16; nSpace++)
		{
			if (ucBug[nSpace] == 0)
				break;
		}
		if (nSpace == 16)
			pItemCollectBox = NULL;
	}

	if (pItemCollectBox == NULL)
	{
		pItemCollectBox = ch->m_inventory.FindByDBIndex(nNormalBoxIndex);
		if (pItemCollectBox)
		{
			TO_BUGDATA(pItemCollectBox, ucBug);
			for (nSpace = 0; nSpace < 16; nSpace++)
			{
				if (ucBug[nSpace] == 0)
					break;
			}
			if (nSpace == 16)
				pItemCollectBox = NULL;
		}
	}

	if (!pItemCollectBox)
	{
		// 채집 상자 없음
		CNetMsg::SP rmsg(new CNetMsg);
		EventCollectBugMsg(rmsg, (nSpace == 16) ? MSG_EVENT_COLLECT_BUG_PICK_FULL : MSG_EVENT_COLLECT_BUG_PICK_NOITEM, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 3. 채집 상자에 넣기
	for (i = 0; i < 10; i++)
	{
		if (tableBug[i][0] == pItem->getDBIndex())
		{
			ucBug[nSpace] = tableBug[i][1];
			pItemCollectBox->setFlag(pItemCollectBox->getFlag() + tableBug[i][1]);
			FROM_BUGDATA(ucBug, pItemCollectBox);

			ch->m_inventory.sendOneItemInfo(pItemCollectBox);

			// 4. 바닥에서 제거
			CNetMsg		disappmsg;
			CNetMsg		takemsg;
			int			cx = pItem->m_cellX;
			int			cz = pItem->m_cellZ;
			char		yLayer = GET_YLAYER(pItem);
			CArea*		pArea = pItem->m_pArea;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemTakeMsg(rmsg, ch, pItem);
				pArea->SendToCell(rmsg, ch, true);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDisappearMsg(rmsg, pItem->getVIndex());
				pArea->SendToCell(rmsg, yLayer, cx, cz);
			}

			pItem->m_preferenceIndex = -1;
			pArea->ItemFromCell(pItem);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_PICK_OK, pItem->getDBIndex());
				SEND_Q(rmsg, ch->m_desc);
			}

			// 5. 게임 로그
			GAMELOG << init("EVENT COLLECT BUG PICK", ch)
					<< "BUG" << delim
					<< itemlog(pItem) << delim
					<< "BOX" << delim
					<< itemlog(pItemCollectBox)
					<< end;

			delete pItem;

			return true;
		}
	}

	return false;
}

bool do_ItemUse_IONCE_WARP_RETURN(CPC* ch, const CItemProto* itemproto)
{
	// 이동 가능 검사
	int canWarp = ch->CanWarp();
	if (canWarp != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 10초후 이동하게 하기
	ch->m_reqWarpType = IONCE_WARP_RETURN;
	ch->m_reqWarpData = itemproto->getItemNum1();
	ch->m_reqWarpTime = PULSE_WARPDELAY;
	ch->SetPlayerState(PLAYER_STATE_WARP);

	{
		// 사용했음을 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		WarpStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	return true;
}

bool do_ItemUse_IONCE_CHANGE_DOC(CPC* ch, const CItemProto* itemproto)
{
	int canChange = ch->CanChange();

	if (canChange != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChangeErrMsg(rmsg, (MSG_CHANGE_ERR_TYPE) canChange);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	// 레벨 검사
	if (ch->m_level < itemproto->getItemNum2())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChangeErrMsg(rmsg, MSG_CHANGE_ERR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 존 검사
	if (ch->m_pZone->m_index != itemproto->getItemNum3())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChangeErrMsg(rmsg, MSG_CHANGE_ERR_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	DelAttackList(ch);

	ch->m_changePulse = itemproto->getItemNum1() / 10;
	ch->m_changeIndex = itemproto->getItemNum0();
	ch->SetPlayerState(PLAYER_STATE_CHANGE);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChangeStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	return true;
}

bool do_ItemUse_IONCE_QUEST_SCROLL(CPC* ch, const CItemProto* itemproto, int extra1)
{
	if(itemproto->getItemNum0() == 0 || itemproto->getItemNum0() == -1)
	{
		return false;
	}

	if(extra1 != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init( MSG_QUEST );
		RefMsg(rmsg) << (unsigned char) 0 << itemproto->getItemNum0();
		if( !do_QuestStart( ch, rmsg ) )
			return false;
	}
	return true;
}

bool do_ItemUse_Castle_Guard(CPC* ch, CItem* item)
{
	// 공성 지역인지 확인
	if( !(ch->GetMapAttr() & MATT_WAR)
			|| ch->m_pZone->m_index != ZONE_DRATAN)
	{
		return false;
	}

	// 피라미드 상층이면 불가
	if(ch->m_pos.m_yLayer != 0)
	{
		return false;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();

	// 해당 성의 성주인지 확인
	if(pCastle->GetOwnerCharIndex() != ch->m_index)
	{
		return false;
	}

	// 종류별 개수 확인
	int mobindex = 0;
	switch(item->m_itemProto->getItemIndex())
	{
	case 2380:
		mobindex = 405;
		break;
	case 2381:
		mobindex = 406;
		break;

	default:
		mobindex = 0;
		break;
	}

	if(mobindex == 0)
	{
		return false;
	}

	if (pCastle->m_GuardList.GetCount(mobindex) >= pCastle->m_GuardList.GetMaxKindCount())
	{
		return false;
	}

	// 사용 가능 시간 확인(공성전중에는  사용불가)
	if(pCastle->GetState() != WCSF_NORMAL)
	{
		return false;
	}

	if(gserver->isRunHelper())
	{
		// 가디언 추가
		if( pCastle->m_GuardList.InsertGuard(mobindex, ch->m_pos.m_x, ch->m_pos.m_z, 0) == false)
		{
			return false;
		}

		int regensec = 20*60;

		float r = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		int idx = ch->m_pZone->m_area->m_npcRegenList.m_wPos;
		ch->m_pZone->m_area->m_npcRegenList.AddRegenInfo(
			idx, mobindex, regensec, ch->m_pos.m_x, 0, ch->m_pos.m_z, r, 1, ZONE_DRATAN);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperInsertGuardMsg(rmsg, ch, item);
			SEND_Q(rmsg, gserver->m_helper);
		}

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(mobindex);
		if (proto && proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			CNPCRegenInfo * p = ch->m_pZone->m_area->m_npcRegenList.GetNpcRegenInfo(idx);
			CNPC* npc = gserver->m_npcProtoList.Create(mobindex, p);
			CArea * area = ch->m_pZone->m_area;
			if (npc == NULL || p == NULL)
			{
				return false;
			}

			GET_X(npc) = p->m_regenX;
			GET_Z(npc) = p->m_regenZ;
			GET_R(npc) = p->m_regenR;
			GET_YLAYER(npc) = p->m_regenY;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				area->SendToCell(rmsg, npc);
			}
		}

//		InsertGuardMsg(msg, ch, item);
//		SEND_Q(msg, ch->m_desc);

		return true;
	}

	return false;
}

bool do_ItemUse_IONCE_SUMMON(CPC* ch, CItem* item)
{
	if(item->m_itemProto->getItemIndex() == 2381		// [용병]스핑크스 창병
			|| item->m_itemProto->getItemIndex() == 2380)	// [용병]스핑크스 파이터
	{
		return do_ItemUse_Castle_Guard(ch, item);
	}

	// 보스 몬스터 소환서
	// 공성중에 공성 지역(MAPATT_WARZONE) 으로 이동 불가
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle)
	{
		if (castle->GetState() != WCSF_NORMAL)
		{
			if ( ch->GetMapAttr() & MATT_WAR
					|| ch->m_pZone->IsWarZone((int)GET_X(ch), (int)GET_Z(ch))
			   )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}
	}
//#endif
	// ox 퀴즈룸 길드룸 쪽도 사용하지 못하도록 변경
	if( ch->IsInPeaceZone(true)
			|| ch->m_pZone->IsPersonalDungeon() ||  ch->GetMapAttr() & MATT_FREEPKZONE
			|| ch->m_pZone->IsGuildRoom() || ch->m_pZone->IsOXQuizRoom() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	int npc_index = item->m_itemProto->getItemNum0();

	static const int patterns[2][13] =
	{
		{1, 297, 0, 9, 133, 132, 264, 125, 126, 158, 68, 69, 250,},	// 2144
		{2, 190, 267, 8, 250, 251, 252, 189, 188, 236, 190, 189, 0,},	// 2145
	};

	int pattern_index;

	if (item->m_itemProto->getItemIndex() == 2144
			|| item->m_itemProto->getItemIndex() == 2145)
	{
		pattern_index = item->m_itemProto->getItemIndex() - 2144;
		int temp = GetRandom(4, 4 + patterns[pattern_index][3] - 1);
		npc_index = patterns[pattern_index][temp];
	}

	CNPCProto* proto = gserver->m_npcProtoList.FindProto(npc_index);
	if (!proto)
	{
		return false;
	}

	CNPC* npc = gserver->m_npcProtoList.Create(npc_index, NULL);
	if (!npc)
		return false;

	if (item->m_itemProto->getItemIndex() == 2144
			|| item->m_itemProto->getItemIndex() == 2145)
	{
		// 몹에 스크롤 정보 저장
		npc->m_MobScrollType = pattern_index;
		npc->m_NextMobIndex = 0;
		npc->m_UseCharIndex = ch->m_index;
	}

	int nTry = 0;
	do
	{
		GET_X(npc) = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		GET_YLAYER(npc) = GET_YLAYER(ch);
		GET_Z(npc) = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		if (ch->m_pArea->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) & MATT_WALKABLE)
			break;
		nTry++;
	}
	while (nTry <= 10);

	if (nTry > 10)
	{
		GET_X(npc) = GET_X(ch);
		GET_Z(npc) = GET_Z(ch);
		GET_YLAYER(npc) = GET_YLAYER(ch);
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
	}

	npc->m_regenX = GET_X(npc);
	npc->m_regenY = GET_YLAYER(npc);
	npc->m_regenZ = GET_Z(npc);

	int cx, cz;
	ch->m_pArea->AddNPC(npc);
	ch->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	ch->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, npc, true);
		ch->m_pArea->SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
	}

	npc->m_postregendelay += 2 * PULSE_REAL_SEC;

	return true;
}

bool do_ItemUse_IETC_PRODUCT(CPC* ch, const CItemProto* itemproto)
{
	// 펫 사료 먹이기
	if (ch->m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if (!pet)
			return false;

		// 등급에 따라 수치
		int upVal = 0;
		switch (itemproto->getItemIndex())
		{
		case 157:		// A등급 원소
		case 152:		// 스톤 웰스던
		case 162:		// 크락의 꽃
			upVal = 9;
			break;

		case 158:		// B등급 원소
		case 153:		// 스톤 비스트
		case 163:		// 크락의 가시
			upVal = 7;
			break;

		case 159:		// C등급 원소
		case 154:		// 스톤 웨버
		case 197:		// 크락의 파란잎
			upVal = 5;
			break;

		case 160:		// D등급 원소
		case 155:		// 스톤 리스
		case 198:		// 크락의 줄기
			upVal = 3;
			break;

		case 161:		// E등급 원소
		case 156:		// 스톤 온
		case 199:		// 크락의 노란잎
			upVal = 1;
			break;

		default:
			return false;
		}

		bool bUpdate = false;
		// 종류에 따라 회복 되는 파리미터 다름
		switch (itemproto->getItemIndex())
		{
		case 157:		// A등급 원소
		case 158:		// B등급 원소
		case 159:		// C등급 원소
		case 160:		// D등급 원소
		case 161:		// E등급 원소
			// 교감도 회복
			bUpdate = pet->IncreaseSympathyPoint(upVal);
			break;

		case 152:		// 스톤 웰스던
		case 153:		// 스톤 비스트
		case 154:		// 스톤 웨버
		case 155:		// 스톤 리스
		case 156:		// 스톤 온
			// 배고픔 회복
			bUpdate = pet->IncreaseHungryPoint(upVal);
			break;

		case 162:		// 크락의 꽃
		case 163:		// 크락의 가시
		case 197:		// 크락의 파란잎
		case 198:		// 크락의 줄기
		case 199:		// 크락의 노란잎
			// HP 회복
			if (pet->m_hp < pet->m_maxHP)
			{
				pet->m_hp += upVal;
				if (pet->m_hp > pet->m_maxHP)
					pet->m_hp = pet->m_maxHP;
				bUpdate = true;
			}
			break;

		default:
			return false;
		}

		if (bUpdate)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
		return false;

	return true;
}

bool do_ItemUse_IETC_SKILLUP(CPC* ch, CItem* item)
{
	CItem* pDestItem = ch->m_wearInventory.wearItemInfo[WEARING_ACCESSORY1];
	CItem* pSrcItem = item;

	if( !pDestItem || !pSrcItem )
		return false;

	if( pSrcItem->m_itemProto->IsRareItem() != pDestItem->m_itemProto->IsRareItem()  )
		return false;

	int i = 0;
	if( pSrcItem->m_itemProto->IsOriginItem() )
	{
		int originalOptionCount = pSrcItem->GetOriginOptionCnt();
		for( i=0 ; i < originalOptionCount ; i++ )
		{
			COptionProto* proto = gserver->m_optionProtoList.FindProto( pSrcItem->GetOriginIndex(i) );
			if( !proto || pSrcItem->GetOriginLevel(i) < 1 )
				continue;

			//Level, Value 셋팅
			pDestItem->m_option[pDestItem->m_nOption].MakeOptionValue( proto, ch->m_level, 1 );
			pDestItem->m_option[pDestItem->m_nOption].m_level = pSrcItem->GetOriginLevel(i);
			pDestItem->m_option[pDestItem->m_nOption].m_value = proto->m_levelValue[pDestItem->m_option[i].m_level-1];
			++pDestItem->m_nOption;
		}
	}
	else
	{
		pDestItem->m_nOption = pSrcItem->m_nOption;
		for( i = 0 ; i< pSrcItem->m_nOption; i++ )
		{
			pDestItem->m_option[i] = pSrcItem->m_option[i];
		}

		pDestItem->m_pRareOptionProto = pSrcItem->m_pRareOptionProto;
		pDestItem->m_pSetItemProto = pSrcItem->m_pSetItemProto;
		pDestItem->m_nRareOptionBit		= pSrcItem->m_nRareOptionBit;
		pDestItem->m_nRareOptionIndex	= pSrcItem->m_nRareOptionIndex;
		pDestItem->m_nSetOption			= pSrcItem->m_nSetOption;
	}

	ch->m_wearInventory.sendOneItemInfo(WEARING_ACCESSORY1);

	ch->m_inventory.deleteItemByItem(item);

	return true;
}

bool do_ItemUse_IONCE_ETC(CPC* ch, const CItemProto* itemproto, int nExtar1,CItem* item)
{
	switch (itemproto->getItemIndex())
	{
	case 1838:		// 돋보기
	case 1839:		// 황금 돋보기
	case 2666:		// 초보자용 돋보기
		return do_ItemUse_1838_1839(ch, itemproto, nExtar1);
		break;

	case 2037:		// 럭키 드로우 상자
		return do_ItemUse_2037(ch, itemproto);
		break;

	case 2597:
		return do_ItemUse_2597(ch, itemproto);
		break;

	case 2887:
		return do_ItemUse_2887(ch, itemproto);
		break;
	case 2888:
		return do_ItemUse_2888(ch, itemproto);
		break;
	case 2889:
		return do_ItemUse_2889(ch, itemproto);
		break;
	case 2890:
		return do_ItemUse_2890(ch, itemproto);
		break;
	case 2891:
		return do_ItemUse_2891(ch, itemproto);
		break;
	case 2892:
		return do_ItemUse_2892(ch, itemproto);
		break;
	case 2893:
		return do_ItemUse_2893(ch, itemproto);
		break;
	case 2968:
		return do_ItemUse_2968(ch, itemproto);
		break;
	case 2146:
	case 2147:
		return do_ItemUse_2146_2147( ch, itemproto );
		break;
	case 2148:
		return do_ItemUse_2148( ch, itemproto );
		break;
	case 2544:
		return do_itemUse_MoonStoneBox( ch, itemproto );
	case 2859:
	case 2982:
		{
			// 스트레이아나 백신 물약
			CSkill* pSkill = gserver->m_skillProtoList.Create(itemproto->getItemNum0(), itemproto->getItemNum1());
			if(pSkill)
			{
				bool bApply;
				ApplySkill(ch, ch, pSkill, itemproto->getItemIndex(), bApply);
				if(!bApply)
				{
					delete pSkill;
					return false;
				}
			}

			ch->m_assist.CureBySkillIndex(511);
			ch->m_SkillTime_511 = 0;
			return true;
		}
		break;

	case 4709:	//레이드 박스 아이템(아이템 번호 미정)
		return do_ItemUse_4709(ch, itemproto,item);
		break;

	case 4664:
		return do_ItemUse_4664(ch, itemproto);
		break;
	case 4786:
		return do_ItemUse_4786(ch, itemproto);
		break;
	case 4787:
		return do_ItemUse_4787(ch, itemproto);
		break;
	case 4788:
		return do_ItemUse_4788(ch, itemproto);
		break;

#ifdef LC_GAMIGO
	case 1053:
		return do_ItemUse_1053(ch, itemproto);
		break;
#endif // LC_GAMIGO

	case 5329:
		return do_ItemUse_5329(ch, itemproto, nExtar1);
		break;

	case 5952: // 후견인의 증표
		return do_ItemUse_5952(ch, itemproto);
		break;

	case 6593:
		return do_ItemUse_6593(ch, itemproto);
		break;
	default:
		return false;
		break;
	}
	return true;
}

bool do_ItemUse_5952(CPC* ch, const CItemProto* itemproto)
{
	CNetMsg::SP rmsg(new CNetMsg);
	HelperTeachFameUpReqMsg(rmsg, ch->m_index, -1, NULL, 15);
	SEND_Q(rmsg, gserver->m_helper);
	return true;
}

bool do_ItemUse_1838_1839(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
	int nProb = 0;
	switch (itemproto->getItemIndex())
	{
	case 1838:	// 돋보기
#ifdef HOLY_WATER
//		nProb = 40;
		nProb = 30;		//  2007.05.02
#else
		nProb = 50;
#endif	// HOLY_WATER
		break;
	case 1839:	// 황금 돋보기
		nProb = 100;
		break;
	case 2666:	// 초보자용 돋보기
		nProb = 100;
		break;
	default:
		return false;
	}

	CItem* pItemRare = ch->m_inventory.FindByVirtualIndex(nExtar1);
	if (!pItemRare)
		return false;
	if (!pItemRare->IsRareItem())
		return false;
	if (pItemRare->m_pRareOptionProto)
		return false;

	if (pItemRare->IsOriginItem())
		return false;

	if (GetRandom(1, 100) <= nProb)
	{
		GAMELOG << init("ITEM IDENTIFY RARE", ch)
				<< "BEFORE" << delim
				<< itemlog(pItemRare) << delim;

		pItemRare->IdentifyRareOption();

		GAMELOG << "AFTER" << delim
				<< itemlog(pItemRare)
				<< end;

		ch->m_inventory.sendOneItemInfo(pItemRare);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->CalcStatus(true);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_IDENTIFY_RARE_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2146_2147( CPC* ch, const CItemProto* itemproto )
{
	if( !gserver->isActiveEvent(A_EVENT_SONGKRAN) )
		return false;

	int rewardIndex = -1;
	switch( itemproto->getItemIndex() )
	{
	case 2146:
		rewardIndex = 22;
		break; // 금그릇
	case 2147:
		rewardIndex = 23;
		break; // 은그릇
	default:
		return false;
	}

	if( !gserver->m_rewardMgr.setRewardData( ch, rewardIndex )
			|| !gserver->m_rewardMgr.doReward(ch, rewardIndex ) )
		return false;

	return true;
}

bool do_ItemUse_2597(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 10;

	int nTable[nTableSize][2]	=
	{
		{ 853, 2209, },		// 대형 퀵 체력 회복 물약
		{ 854, 2200, },		// 대형 퀵 마나 회복 물약
		{ 1315, 2000, },		// 공격력 향상제 (대)
		{ 1314, 2000, },		// 방어력 향상제 (대)
		{ 1418, 500, },		// 플래티늄 제련석
		{ 974, 500, },		// 행운의 제련석
		{ 1706, 290, },		// 플래티늄 제련석
		{ 2394, 200, },		// 플래티늄 하급 제련 보호석
		{ 2395, 100, },		// 플래티늄 상급 제련 보호석
		{ 1835, 1, },		// 그림자의 돌
	};

	int nRate = GetRandom(0, 10000);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate < nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

// [101229: selo] 보상 아이템 드롭 수정
	if( !ch->CheckInvenForPrize(nTable[nIndex][0], 0, 0) )
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2037(CPC* ch, const CItemProto* itemproto)
{
	const int nTableSize	= 6;
	int nTable[nTableSize]	=
	{
		2032,		// 럭키 아이리스의 축복
		1840,		// 소형 경험치 포션
		2033,		// 럭키 소울 닉스
		2034,		// 럭키 다지 피어
		2035,		// 럭키 경험치복구 주문서
		2036,		// 럭키 숙련도복구 주문서
	};
	int nIndex = GetRandom(0, nTableSize - 1);

// [101229: selo] 보상 아이템 드롭 수정
	if( !ch->CheckInvenForPrize(nTable[nIndex], 0, 0) )
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_Coin(CPC * pc, const CItemProto * pItemProto, int nExtra1)
{
	// 지급 아이템 테이블
	const int nItems[5][5] =
	{
		{ 2715, 2716, 2717, 2718, 2719, },
		{ 2720, 2721, 2722, 2723, 2724, },
		{ 2725, 2726, 2727, 2728, 2729, },
		{ 2730, 2731, 2732, 2733, 2734, },
		{ 2735, 2736, 2737, 2738, 2739, },
	};

	int item_index = -1, coin_index = -1, coin_count = -1, op_level = -1;
	switch(pItemProto->getItemIndex())
	{
	case 2740:
		item_index = 0;
		coin_index = 2710;
		coin_count = 10;
		op_level = 1;
		break;

	case 2741:
		item_index = 1;
		coin_index = 2711;
		coin_count = 30;
		op_level = 2;
		break;

	case 2742:
		item_index = 2;
		coin_index = 2712;
		coin_count = 50;
		op_level = 3;
		break;

	case 2743:
		item_index = 3;
		coin_index = 2713;
		coin_count = 70;
//		coin_count = 100;
		op_level = 4;
		break;

	case 2744:
		item_index = 4;
		coin_index = 2714;
		coin_count = 100;
//		coin_count = 200;
		op_level = 5;
		break;

	default:
		return false;
	}

	// 인벤에 해당 코인 확인
	item_search_t vec;
	int sc = pc->m_inventory.searchItemByCondition(coin_index, 0, 0, vec);
	if (sc == 0 || sc < coin_count)
	{
		return false;
	}

	// 아이템 생성
	int random_item = GetRandom(0, 4);
	CItem * pChange = gserver->m_itemProtoList.CreateItem(nItems[item_index][random_item], -1, 0, 0, 1);
	if(pChange == NULL)
	{
		return false;
	}

	// 옵션 붙는 갯수
	int num = GetRandom(1, 5);

	// 옵션 테이블
	int nOptions[5][8] =
	{
		{3, 6, 12, 19, 23, 52, 56, 59,},
		{1, 5, 12, 19, 23, 53, 56, 59,},
		{2, 5, 12, 19, 23, 53, 56, 59,},
		{4, 7, 17, 21, 24, 22, 56, 59,},
		{4, 5, 53, 52, 23, 22, 56, 59}
	};

	GAMELOG << init("MONSTER_COMBO_COIN_CHANGE", pc )
			<< nItems[item_index][random_item] << delim
			<< "Option count: " << num;

	// 옵션 붙이기
	int coolTimeDecreaseOptionID = 0;
	pChange->m_nOption	 = 0;
	int rand;
	for(int i = 0; i < num; ++i)
	{
#ifdef LC_TLD
		rand = GetRandom(0, 7);
		if(rand == 7)
		{
			rand = GetRandom(0, 10000);
			if(rand < 340)
			{
				rand = 7;
			}
			else
			{
				rand = GetRandom(0, 6);
			}
		}
		int tmpOp = nOptions[random_item][rand];
#else
		int tmpOp = nOptions[random_item][GetRandom(0, 7)];
#endif

		if (coolTimeDecreaseOptionID == tmpOp)
			continue;

		if (tmpOp == 59)
		{
			coolTimeDecreaseOptionID = tmpOp;
		}

		// 옵션번호로 붙을 수 있는 옵션인지 검사
		COptionProto * proto = gserver->m_optionProtoList.FindProto(tmpOp - 1);
		if(proto == NULL)
		{
			continue;
		}

		// Level, Value 셋팅
		pChange->m_option[pChange->m_nOption].m_proto = proto;
		pChange->m_option[pChange->m_nOption].m_type = proto->m_type;
		pChange->m_option[pChange->m_nOption].m_level = op_level;
		pChange->m_option[pChange->m_nOption].m_value = proto->m_levelValue[op_level - 1];
		pChange->m_option[pChange->m_nOption].SetDBValue();
		++pChange->m_nOption;

		GAMELOG << delim << tmpOp;
	}

	GAMELOG << end;

	// 아이템 지급
	if (pc->m_inventory.addItem(pChange) == false)
	{
		delete pChange;
		return false;
	}

	// 코인 제거
	pc->m_inventory.deleteItem(vec, coin_count);

	return true;
}

bool do_ItemUse_2046(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
	if( !gserver->isActiveEvent( A_EVENT_VALENTINE_DAY) )
		return false;

	CItem* pCacaoItem = ch->m_inventory.FindByVirtualIndex(nExtar1);
	if(!pCacaoItem)
	{
		// 인벤에서 아이템 찾지 못했음
		return false;
	}

	CItem* pPackagedItem;
	switch(pCacaoItem->m_itemProto->getItemIndex())
	{
	case 2043: // 카카오 30
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2048, -1, 0, 0, 1);
		break;
	case 2044: // 카카오 60
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2049, -1, 0, 0, 1);
		break;
	case 2045: // 카카오 60
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2050, -1, 0, 0, 1);
		break;

	case 2629: // 파랑 복주머니
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2634, -1, 0, 0, 1);
		break;
	case 2630: // 노랑 복주머니
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2635, -1, 0, 0, 1);
		break;
	case 2631: // 빨강 복주머니
		pPackagedItem = gserver->m_itemProtoList.CreateItem(2636, -1, 0, 0, 1);
		break;

	default:				// 카카오 30, 60, 90 인 것만 포장할 수 있다	// 복주머니 셋트 추가
		return false;
	}

	if (pPackagedItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pPackagedItem) == false)
	{
		delete pPackagedItem;

		CNetMsg::SP rmsg(new CNetMsg);
		EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);

		return false;
	}

	// 포장한 카카오 아이템 제거
	ch->m_inventory.decreaseItemCount(pCacaoItem, 1);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2148(CPC* ch, const CItemProto* itemproto)
{
	if( !gserver->isActiveEvent( A_EVENT_EGGS_HUNT) )
		return false;

	int rewardIndex = 1;

	// 지급할 아이템 추출 및 지급 가능 여부 확인
	if( !gserver->m_rewardMgr.setRewardData(ch, rewardIndex ) )
	{
		GAMELOG << init("EVENT EGGSHUNT REWARD ERROR", ch) 	<< end;
		return false;
	}

	// 달걀이 인벤에 있으면 지급
	CItem* pEggItem = ch->m_inventory.FindByDBIndex(2148);
	if (pEggItem == NULL)
	{
		// 인벤에서 찾지 못했다.
		CNetMsg::SP rmsg(new CNetMsg);
		EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	GAMELOG << init("EVENT EGGSHUNT REWARD START", ch) 	<< end;

	if( !gserver->m_rewardMgr.doReward(ch, rewardIndex ) )
	{
		GAMELOG << init("EVENT EGGSHUNT REWARD FAIL", ch) 	<< end;
		CNetMsg::SP rmsg(new CNetMsg);
		EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_GIFT_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	GAMELOG << init("EVENT EGGSHUNT REWARD END", ch) 	<< end;
	return true;
}

void do_ItemTarget(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemTarget* packet = reinterpret_cast<RequestClient::doItemTarget*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (item->getVIndex() != packet->virtualIndex)
	{
		LOG_ERROR("HACKING? : not equal virtual index. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (item->getWearPos() != WEARING_NONE)
	{
		LOG_ERROR("already weard. char_index[%d]", ch->m_index);
		return ;
	}

	const CItemProto* itemproto = item->m_itemProto;
	if (!itemproto || itemproto->getItemTypeIdx() != ITYPE_ONCE || itemproto->getItemSubTypeIdx() != IONCE_TARGET)
	{
		if( itemproto )
			GAMELOG << init("ERROR") << itemproto->getItemTypeIdx() << itemproto->getItemSubTypeIdx() << end;
		return ;
	}

	bool bApply = false;

	CCharacter* tch = ch->m_pArea->FindCharInCell(ch, packet->charIndex, (MSG_CHAR_TYPE)packet->charType, false);
	if (tch == NULL)
	{
		LOG_ERROR("not found character in cell. char_index[%d]", packet->charIndex);
		return ;
	}

	if(ch->IsEnemy(tch) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_DO_NOT_MEET_CONDITION);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CSkill* pSkill = gserver->m_skillProtoList.Create(itemproto->getItemNum0(), itemproto->getItemNum1());
	if (pSkill == NULL)
	{
		LOG_ERROR("can't create skill. char_index[%d] skill_index[%d] skill_level[%d]", ch->m_index, itemproto->getItemNum0(), itemproto->getItemNum1());
		return ;
	}

	switch(itemproto->getItemIndex())
	{
	case 2374:
		{
			if (tch->m_assist.FindByItemIndex(2374))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysCannotDuplcationMsg(rmsg, itemproto->getItemIndex(), 2374);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;
	}

	int nRetApplySkill = ApplySkill(ch, tch, pSkill, itemproto->getItemIndex(), bApply);
	delete pSkill;
	pSkill = NULL;

	if(bApply == false)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemUseMsg(rmsg, item->tab(), item->getInvenIndex(), item->getDBIndex(), 0);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->m_inventory.decreaseItemCount(item, 1);

	//속성 아이템 사용시 시스템 메시지 전달
	switch(itemproto->getItemIndex())
	{
	case 10848:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::AttrItemUseMsg(rmsg, MDT_ATTACK, tch->m_assist.getAttrAtt());
			SEND_Q(rmsg, ch->m_desc);

			if(IS_NPC(tch))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::AttrNpcStateMsg(rmsg, MDT_ATTACK, tch->m_assist.getAttrAtt(), tch->m_index);
				tch->m_pArea->SendToCell(rmsg, tch);
			}
		}
		break;
	case 10849:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::AttrItemUseMsg(rmsg, MDT_DEFENCE, tch->m_assist.getAttrDef());
			SEND_Q(rmsg, ch->m_desc);

			if(IS_NPC(tch))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::AttrNpcStateMsg(rmsg, MDT_DEFENCE, tch->m_assist.getAttrDef(), tch->m_index);
				tch->m_pArea->SendToCell(rmsg, tch);
			}
		}
		break;
	}

	if (bApply == false)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EffectItemMsg(rmsg, ch, item);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

bool do_itemUse_MoonStoneBox(CPC* ch, const CItemProto* itemproto)
{
	const int nMoonStoneGradeCount = 4;
	static const int MoonStoneGradeProb[nMoonStoneGradeCount][2] =     // 아이템인덱스, 확률
	{
		{2545, 520},						// 최하급문스톤		52
		{2546, 300},						// 하급문스톤		30
		{723, 150},						// 문스톤			15
		{2547, 30},							// 상급문스톤		3
	};

	CItem* MoonStoneBox = ch->m_inventory.FindByDBIndex(2544);
	if (MoonStoneBox == NULL)
		return false;

	int rand = GetRandom(1, 1000);
	int prob = 0;
	int i;
	for(i = 0; i < nMoonStoneGradeCount; ++i)
	{
		prob += MoonStoneGradeProb[i][1];
		if(rand <= prob)
		{
// [101229: selo] 보상 아이템 드롭 수정
			if( !ch->CheckInvenForPrize(MoonStoneGradeProb[i][0], 0, 0) )
			{
				return false;
			}

			CItem* MoonStone = gserver->m_itemProtoList.CreateItem(MoonStoneGradeProb[i][0], -1, 0, 0, 1);
			if(!MoonStone)
				return false;

			GAMELOG << init("MOONSTONE BOX OPEN", ch)
					<< "MOONSTONE" << itemlog(MoonStone) << end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTON_OK);
				RefMsg(rmsg) << MoonStone->getDBIndex();
				SEND_Q(rmsg, ch->m_desc);
			}

			if (ch->m_inventory.addItem(MoonStone) == false)
			{
				ch->m_pArea->DropItem(MoonStone, ch);
				MoonStone->m_preferenceIndex = ch->m_index;

				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, ch, MoonStone);
				SEND_Q(rmsg, ch->m_desc);

				delete MoonStone;
			}
			break;
		}
	}

	if(i == nMoonStoneGradeCount)
		return false;
	return true;
}

void do_ItemUseExchagneEquip( CPC* ch, CNetMsg::SP& msg )
{
	RequestClient::doItemUseExchangeEquip* packet = reinterpret_cast<RequestClient::doItemUseExchangeEquip*>(msg->m_buf);

	{
		std::set<int> checkdup;
		int key = 0;

		key = makeCheckDupKey(packet->tab_1, packet->invenIndex_1);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}

		key = makeCheckDupKey(packet->tab_2, packet->invenIndex_2);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_1, packet->invenIndex_1) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_2, packet->invenIndex_2) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab_2, packet->invenIndex_2);
	if( item == NULL )
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if( item->getDBIndex() != 975 && item->getDBIndex() != 2355 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* olditem = ch->m_inventory.getItem(packet->tab_1, packet->invenIndex_1);
	if (olditem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (olditem->getVIndex() != packet->virtualIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( CEquipExchangeExtend::instance()->checkValid(olditem, packet->changeType) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->m_itemProto->getItemFlag() & FLAG_ITEM_SOCKET)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SocketErrorMsg(rmsg, MSG_EX_SOCKET_ERROR_ITEM_MISSMATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(CEquipExchangeExtend::instance()->find_(olditem, packet->exchangeDBIndex, olditem->m_itemProto->getItemSubTypeIdx()) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int nNewPlus = olditem->getPlus();
	int nNewFlag = olditem->getFlag() & ~FLAG_ITEM_OPTION_ENABLE;

	CItem* newitem = gserver->m_itemProtoList.CreateItem(packet->exchangeDBIndex, -1, nNewPlus, nNewFlag, 1);
	if (newitem == NULL)
	{
		LOG_ERROR("Not Exist Item. ITEM DB INDEX[%d]", packet->exchangeDBIndex);
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_WRONG_ITEM_INFO);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(newitem->IsSocketItem() && olditem->IsSocketItem())
	{
		int newitemFlag = newitem->getFlag();
		newitem->m_socketList.CopySocketInfo(olditem->m_socketList, newitemFlag);
		newitem->setFlag(newitemFlag);
	}

	GAMELOG << init("ITEM CHANGE WEAPON", ch)
		<< "NEED MONEY" << delim		
		<< "OLD ITEM" << delim
		<< itemlog(olditem) << delim
		<< "NEW ITEM" << delim
		<< itemlog(newitem)
		<< end;

	int olddb = olditem->m_itemProto->getItemIndex();
	int newdb = newitem->m_itemProto->getItemIndex();

	if (ch->m_inventory.addItem(newitem))
	{
		ch->m_inventory.decreaseItemCount(olditem, 1);		
		ch->m_inventory.decreaseItemCount(item, 1);
	}
	else
	{
		LOG_ERROR("ITEM ADD FAIL. ITEM_INDEX[%d]", packet->exchangeDBIndex);
		delete newitem;

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, 0, -1, ResponseClient::EXCHANGE_ERR_INVENTORY_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemChangeWeapon(rmsg, olddb, newdb, ResponseClient::EXCHANGE_ERR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

// 대전용 아이템
bool do_useCashItem_CompetitionItem( CPC* ch, CItem* pItem )
{
	bool bUsed = false;

// 광속2 디버프
	if( (pItem->getDBIndex() == 5018 || pItem->getDBIndex() == 5019 ) &&
			(/*ch->m_cooltime_Competition > gserver->m_gameTime||*/ ch->m_assist.FindBySkillIndex(995) ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_COMPETITION, 995, -1);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	else if ( ch->m_cooltime_Competition > gserver->getNowSecond() || ch->m_assist.FindBySkillIndex(590) || ch->m_assist.FindBySkillIndex(995) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillErrorMsg(rmsg, MSG_SKILL_ERROR_COMPETITION, 590, -1);
		SEND_Q(rmsg, ch->m_desc);

		return false;
	}
#if defined(LC_USA)
	if( ch->m_inventory.decreaseMoney(ch->m_level * (5 * USE_COMPETITION_ITEM_NAS)) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
#else
	if( ch->m_inventory.decreaseMoney(ch->m_level * USE_COMPETITION_ITEM_NAS) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
#endif

	if( pItem->getUsed() == -1 )
	{
		pItem->setUsed(gserver->getNowSecond() + pItem->m_itemProto->getItemNum3() * 60 * 60);
		bUsed = true;
	}

	if( pItem->getUsed() < gserver->getNowSecond() )
		return false;

	if ( bUsed )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemUsed(rmsg, pItem->tab(), pItem->getInvenIndex(), pItem->getUsed());
		SEND_Q( rmsg, ch->m_desc );
	}

	CSkill* skill = NULL;
	skill = gserver->m_skillProtoList.Create( pItem->m_itemProto->getItemNum0(), pItem->m_itemProto->getItemNum1() );
	if( skill == NULL )
		return false;

	bool bApply;
	ApplySkill( ch, ch, skill, pItem->m_itemProto->getItemIndex(), bApply );
	if( !bApply )
	{
		delete skill;
		return false;
	}

	CSkill* pDebuff = NULL;
	if( pItem->getDBIndex() == 5018 || pItem->getDBIndex() == 5019 )
		pDebuff = gserver->m_skillProtoList.Create(995);
	else
		pDebuff = gserver->m_skillProtoList.Create(590);

	if( pDebuff == NULL )
		return false;

	bool bDebuffApply = false;
	ApplySkill( ch, ch, pDebuff, -1, bDebuffApply);

	if (!bDebuffApply)
	{
		delete pDebuff;
		return false;
	}
	delete pDebuff;
	delete skill;

	{
		// 포션 사용 이펙트 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		EffectItemMsg(rmsg, ch, pItem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	return true;
}

bool do_useEventItem_HalloweenCandy( CPC* ch, CItem* pItem )
{
//	if( !gserver->isActiveEvent( A_EVENT_HALLOWEEN ) )
//		return false;

	if( !gserver->m_rewardMgr.setRewardData(ch, 32) )
		return false;

	// 보상이 스킬이 아니면 문제가 있다. 에러 처리 한다.
	if( gserver->m_rewardMgr.getRewardDataType(0) != 5 )
		return false;

	int	skillIndex = gserver->m_rewardMgr.getRewardDataIndex(0);

	if( !gserver->m_rewardMgr.doReward(ch, 32) )
		return false;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventHalloween2007Msg( rmsg, MSG_EVENT_TASTE_CANDY,  (unsigned char)(skillIndex-470) );
		SEND_Q( rmsg, ch->m_desc);
	}

	int nPriceItemIndex = -1;

	switch( skillIndex )
	{
	case 470 :
		nPriceItemIndex=0;
		break;// 퀘스트 아이템 제공 2489 (최악의맛 증표)
	case 474 :
		nPriceItemIndex=1;
		break;// 퀘스트 아이템 제공 2488 (최고의맛 증표)
	default:
		return true;
	}

	gserver->m_rewardMgr.setRewardData( ch, 33, -1, nPriceItemIndex );
	gserver->m_rewardMgr.doReward( ch, 33 );

	return true;
}

void do_GroceryItemBuy(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemGroceryItemBuy* packet = reinterpret_cast<RequestClient::doItemGroceryItemBuy*>(msg->m_buf);

	const int npcIdx  = 130;
	CItem**	item = NULL;

	try
	{
		// 최대 살수 있는 아이템 종류의 수
		if (packet->buyCount > MAX_ITEM_BUY || packet->buyCount <= 0 || packet->clientPrice <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류

		// validation shop
		CZone* pZone = gserver->FindZone( ZONE_DRATAN );
		if (pZone == NULL)
			throw 1;

		CShop* shop;
		if (!(shop = pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음

		for(int i = 0; i < packet->buyCount; ++i)
		{
			if (packet->list[i].count < 1)
				throw 5;

			// 상점에 아이템이 있는지 검사 throw 6
			if (shop->ThersIsItem(packet->list[i].itemDBIndex) == false)
				throw 6;		// 상점에 아이템 없음
		}

		// 누적합계
		LONGLONG serverPrice = 0;
		item = new CItem * [packet->buyCount];
		memset(item, 0, sizeof(CItem*) * packet->buyCount);

		for (int i = 0; i < packet->buyCount; ++i)
		{
			item[i] = gserver->m_itemProtoList.CreateItem(packet->list[i].itemDBIndex, WEARING_NONE, 0, 0, packet->list[i].count);

			if (!item[i])
				throw 2;		// DB에 없는 아이템

			// 아이템 명성치 검사
			if (item[i]->m_itemProto->getItemFrame() != -1)
			{
				// 명성치 딸린다
				if (ch->m_fame < item[i]->m_itemProto->getItemFrame())
					throw 9;
			}

			if (ch->m_inventory.addItem(item[i]) == false)
			{
				delete item[i];
				item[i] = NULL;
				throw 4;		// 인벤토리 가득 참
			}

			LONGLONG itemPrice = 0;

#ifdef REFORM_PK_PENALTY_SHOP_201108
			if(ch->IsChaotic())
			{
				itemPrice = (item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100);
				itemPrice = itemPrice + (itemPrice * ABS(ch->m_pkPenalty) / PK_HUNTER_POINT_MAX * 20);
				itemPrice *= packet->list[i].count;
			}
			else
				itemPrice = (item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100) * packet->list[i].count;
#else
			itemPrice = (item[i]->m_itemProto->getItemPrice() * shop->m_buyRate / 100) * packet->list[i].count;
#endif // REFORM_PK_PENALTY_SHOP_201108
			if(itemPrice <= 0)
				throw 5;

			serverPrice += itemPrice;
		}

		// 합계가 다르다! -->
		if (serverPrice != packet->clientPrice)
			throw 5;		// 클라이언트 데이터 오류 : 가격 틀림

		if (serverPrice <= 0)
			throw 5;

		// 돈검사
		if ((ch->m_inventory.getMoney() >= serverPrice))
		{
			ch->m_inventory.decreaseMoney(serverPrice);
		}
		else
			throw 8;	// 돈 모질람

		for (int i = 0; i < packet->buyCount; ++i)
		{
			if (item[i] == NULL)
				continue;

#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_ITEM_BUY")
					   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
					   << LOG_VAL("vendor-id", npcIdx ) << blank
					   << LOG_VAL("item-id", item[i]->getDBIndex() ) << blank
					   << LOG_VAL("amount", packet->buyCount ) << blank
					   << endGer;
#endif // GER_LOG
			GAMELOG << init("ITEM_BUY", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<<ch->m_inventory.getMoney()
					<< end;
		}

		STATISTICS(ItemBuy, serverPrice);
	}
	catch (const int errCode)
	{
		// 오류 발생시 만들었던 아이템 제거
		if (item)
		{
			for (int i = 0; i < packet->buyCount; ++i)
			{
				if (item[i])
				{
					item_search_t vec;
					int sc = ch->m_inventory.searchItemByCondition(item[i]->getDBIndex(), item[i]->getPlus(), item[i]->getFlag(), vec);
					ch->m_inventory.deleteItem(vec, packet->list[i].count);
				}
			}
		}

		{
			// 오류 메시지 보내기
			CNetMsg::SP rmsg(new CNetMsg);

			switch (errCode)
			{
			case 0:			// 메시지 내의 아이템 종류 수 오류
			case 1:			// 상점이 없음
			case 2:			// DB에 없는 아이템
			case 3:			// DB에 잘못된 값
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				break;
			case 4:			// 인벤토리 가득 참
				SysFullInventoryMsg(rmsg, 0);
				break;
			case 5:			// 클라이언트 데이터 오류 : 가격 틀림
			case 6:			// 상점에 아이템 없음
			case 7:			// 돈 빼기 실패
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				break;
			case 8:			// 돈 모질람
				SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
				break;

			case 9:			// 명성 부족
				SysMsg(rmsg, MSG_SYS_SHORT_FAME);
				break;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
	}

	if (item)
		delete [] item;
}

// Buy & Sell 모두 PC위주로 구성
// Player가 팔때 Item Sell Msg, Sell Rate 적용
void do_GroceryItemSell(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemGroceryItemSell* packet = reinterpret_cast<RequestClient::doItemGroceryItemSell*>(msg->m_buf);

	const int npcIdx  = 130;
	CItem**		item = NULL;

	try
	{
		if (packet->clientPrice <= 0)
		{
			GAMELOG << init("HACK USER BY ITEM SELL", ch) << "Client Sell Price" << delim << packet->clientPrice << end;
			throw 0;
		}

		// 최대 팔수 있는 아이템 종류의 수
		if (packet->sellCount > MAX_ITEM_SELL || packet->sellCount <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류

		std::set<int> check_duplication;
		for (int i = 0; i < packet->sellCount; ++i)
		{
			if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
			{
				LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("invalid packet");
				return;
			}

			if (packet->list[i].count < 1)
			{
				GAMELOG << init("HACK USER BY ITEM SELL", ch) << "Item Sell Count" << delim << packet->list[i].count << end;
				throw 3;
			}

			if (check_duplication.insert(packet->list[i].tab << 16 | packet->list[i].invenIndex).second == false)
				throw 2;
		}

		CZone* pZone = gserver->FindZone( ZONE_DRATAN );
		if (pZone == NULL)
			throw 1;

		CShop* shop;
		if (!(shop = pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음

		LONGLONG serverPrice = 0;
		item = new CItem * [packet->sellCount];
		memset(item, 0, sizeof(CItem*) * packet->sellCount);

		// 판매 아이템 만들기
		for (int i = 0; i < packet->sellCount; ++i)
		{
			item[i] = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
			if (item[i] == NULL)
				throw 2;	// 인벤에 없는 아이템

			// 실제 아이템의 갯수가 클라이언트에서 팔려던 갯수보다 작다.
			if (item[i]->Count() < packet->list[i].count)
				throw 2;

#ifdef ENABLE_SUBJOB
			if( item[i] && item[i]->CanUseTrader( TRADE_SELL, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
			{
				throw 6;
			}
#endif //ENABLE_SUBJOB

			
			if(item[i]->IsBelong() == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 입고 있거나 상점 불가 아이템인 경우
			if (!item[i]->CanSellToShop())
				throw 6;

#ifdef LC_USA
			// 미국은 97레벨 이상의 아이템은 매각가격의 절반만 준다.
#ifdef BUGFIX_ITEMSELL_HACKUSER
			LONGLONG itemPrice = item[i]->m_itemProto->getItemPrice() * shop->m_sellRate;
			if (item[i]->m_itemProto->GetItemProtoLevel() >= 97)
				itemPrice = itemPrice / 2;
			itemPrice = (itemPrice / 100) * packet->list[i].count;

			if( itemPrice < 0 )
				throw 3;

			serverPrice += itemPrice;
#else
			int price = item[i]->m_itemProto->getItemPrice() * shop->m_sellRate;
			if (item[i]->m_itemProto->GetItemProtoLevel() >= 97)
				price = price / 2;

			serverPrice += ( price / 100) * packet->list[i].count;
#endif // BUGFIX_ITEMSELL_HACKUSER
#else
#ifdef BUGFIX_ITEMSELL_HACKUSER
			LONGLONG itemPrice = ( item[i]->m_itemProto->getItemPrice() * shop->m_sellRate / 100) * packet->list[i].count;

			if( itemPrice < 0 )
				throw 3;
			serverPrice += itemPrice;
#else
			serverPrice += (item[i]->m_itemProto->getItemPrice() * shop->m_sellRate / 100) * packet->list[i].count;
#endif // BUGFIX_ITEMSELL_HACKUSER
#endif // LC_USA
		}

//#ifdef LC_USA
		// 가격의 차이가 10나스 이하이면 클라이언트 가격에 맞춘다.
		// 판매할 때 최대 10개 가능, 2로 나누면 나머지가 1씩 나오므로 오차는 최대 10... 그러므로 10까지만 보정을 함
#ifdef	BUGFIX_ITEMSELL_HACKUSER
		LONGLONG gapPrice = (serverPrice > packet->clientPrice) ? (serverPrice - packet->clientPrice) : (packet->clientPrice - serverPrice);
#else
		int gapPrice = serverPrice - packet->clientPrice;
		if (gapPrice < 0)
			gapPrice = gapPrice * -1;
#endif //BUGFIX_ITEMSELL_HACKUSER
		if (gapPrice <= 10)
		{
			serverPrice = packet->clientPrice;
		}
		else
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림
//#else
		// 합계가 다르다! -->
		if (serverPrice != packet->clientPrice)
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림
//#endif // LC_USA
		if (serverPrice <= 0)
			throw 3;
		// 돈 더하기
		LONGLONG nowMoney = ch->m_inventory.getMoney();
		if (ch->m_inventory.increaseMoney(serverPrice) == nowMoney)
			throw 4;		// 돈 넣기 실패

		// 인벤에서 파는 아이템 완전 제거
		for (int i = 0; i < packet->sellCount; ++i)
		{
#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_ITEM_SELL")
					   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", ch->m_desc->m_pChar->m_name) << blank
					   << LOG_VAL("item-id", item[i]->getDBIndex() ) << blank
					   << LOG_VAL("amount", packet->sellCount ) << blank
					   << LOG_VAL("vender-id", npcIdx ) << blank
					   << endGer;
#endif // GER_LOG
			// Log
			GAMELOG << init("ITEM_SELL", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<<ch->m_inventory.getMoney()
					<< end;

			ch->m_inventory.decreaseItemCount(item[i], packet->list[i].count);
		}

		STATISTICS(ItemSell, serverPrice * 3 / 2);
	}
	catch (const int errCode)
	{
		// 오류 메시지 보내기
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// 인벤에 없는 물건
		case 3:			// 클라이언트 데이터 오류 : 가격 틀림
		case 4:			// 돈 넣기 실패
		case 5:			// 인벤에서 제거 실패
		case 6:			// 팔수 없는 아이템
		case 7:			// 팔려는 아이템의 개수가 너무 많음
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_SHOP_CANCEL);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;
		}
	}

	if (item)
		delete [] item;
}

#if defined (SAKURA_EVENT_2008_REWARD)
bool do_ItemUse_SakuraBox( CPC * pc, const CItemProto * pItemProto, int nExtra1 )
{
	// 보상아이템 제공
	int nEventItem[10][3] =   // ItemIndex, Count, Percent
	{
		{ 85, 1, 5}, { 2548, 2, 15	}, { 2547, 5, 30}, { 884, 2, 50 }, { 786, 1, 50 }, { 1310, 1, 100 },
		{ 1311, 1, 100 }, { 1308, 1, 200 }, { 1309, 1, 200 }, { 391, 1, 250 }
	};

	int prob = GetRandom(0, 1000 );
	int nProbIndex = 0;
	do
	{
		prob -= nEventItem[nProbIndex][2];
		nProbIndex++;
	}
	while( prob > 0 );
	nProbIndex--;

	if( nProbIndex < 0 || nProbIndex > 9 )
		return false;

	if( pc->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	CItem* pEventItem =  gserver->m_itemProtoList.CreateItem( nEventItem[nProbIndex][0], -1, 0, 0, nEventItem[nProbIndex][1] );
	if( !pEventItem )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	if (pc->m_inventory.addItem(pEventItem))
	{
		GAMELOG << init( "USE SAKURA EVENT ITEM BOX", pc ) << itemlog(pEventItem) << end;
		if( nEventItem[nProbIndex][0] == 85 || nEventItem[nProbIndex][0] == 2548 )	// 고제거나 , 최상급 문스톤 이면 서버에 알림
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventSakuraEvent2008Msg( rmsg, MSG_EVNET_SAKURAEVENT_2008_ITEMCHANGE );
			RefMsg(rmsg) << pc->m_nick << nEventItem[nProbIndex][0];
			PCManager::instance()->sendToAll( rmsg );
		}
	}
	else // 실패
	{
		delete pEventItem;

		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
		SEND_Q(rmsg, pc->m_desc);

		return false;
	}

	return true;
}
#endif //defined (SAKURA_EVENT_2008_REWARD)

void do_itemUse_WarpDoc(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemUseWarpDoc* packet = reinterpret_cast<RequestClient::doItemUseWarpDoc*>(msg->m_buf);

	CItem* item = ch->m_inventory.FindByVirtualIndex(packet->virtualIndex);
	if (item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] virtualIndex[%d]", ch->m_index, packet->virtualIndex);
		ch->m_desc->Close("not found item");

		return;
	}

	CZone* pZone = gserver->FindZone(packet->zone);
	if (pZone == NULL)
	{
		LOG_ERROR("HACKING? : not found zone. char_index[%d] zone[%d]", ch->m_index, packet->zone);
		ch->m_desc->Close("not found zone");
		return;
	}

	if (packet->extra < 0 || packet->extra >= pZone->m_countZonePos)
	{
		LOG_ERROR("HACKING? : invalid extra. char_index[%d] extra[%d]", ch->m_index, packet->extra);
		ch->m_desc->Close("invalid extra");
		return;
	}

	GoZone(ch, packet->zone, pZone->m_zonePos[packet->extra][0],
		   GetRandom(pZone->m_zonePos[packet->extra][1], pZone->m_zonePos[packet->extra][3]) / 2.0f,
		   GetRandom(pZone->m_zonePos[packet->extra][2], pZone->m_zonePos[packet->extra][4]) / 2.0f);

	if(ch->m_pZone->m_index == packet->zone)
	{
		GAMELOG << init("CASH_ITEM_USE", ch)
				<< itemlog(item) << delim
				<< end;

		ch->m_inventory.decreaseItemCount(item, 1);
	}
}

void do_itemUse_PressCorps(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemUsePressCorps* packet = reinterpret_cast<RequestClient::doItemUsePressCorps*>(msg->m_buf);
	packet->str[1023] = '\0';

	if (ch->m_inventory.FindByVirtualIndex(packet->virtuaIndex) == NULL)
	{
		LOG_ERROR("not found item. char_index[%d] virtualIndex[%d]", ch->m_index, packet->virtuaIndex);
		ch->m_desc->Close("not found item");
		return ;
	}

	if(ch->m_bPressCorps)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE_PRESSCORPS, 0, "", "", packet->str);
		PCManager::instance()->sendToAll(rmsg);
	}
}

void do_itemCheck_Composition(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemCheckComposition* packet = reinterpret_cast<RequestClient::doItemCheckComposition*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// COMPOSITION 아이템 찾기, NORMAL INVEN 이라고 확정
	CItem* pCompotionItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if( pCompotionItem && pCompotionItem->getVIndex() == packet->virtualIndex && (pCompotionItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) )
	{
		if ( pCompotionItem->getFlag() & FLAG_ITEM_COMPOSITION )
		{
			CItem* pItemEquip = ch->m_inventory.FindByVirtualIndex(pCompotionItem->m_nCompositeItem);
			if (pItemEquip)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ItemCheckCompotion( rmsg, pItemEquip->tab(), pItemEquip->getInvenIndex(), pItemEquip->getVIndex() );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemCheckCompotion( rmsg, -1, -1, -1 );			// 결합된 아이템을 찾을 수 없음
		SEND_Q( rmsg, ch->m_desc );
	}
}

bool do_ItemUse_2887(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 7;

	int nTable[nTableSize][3]	=
	{
		{ 884, 20, 1},
		{ 2141, 15, 1},
		{ 1315, 15, 1},
		{ 882, 15, 1},
		{ 2356, 15, 1},
		{ 972, 15, 1},
		{ 1839, 5, 1},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2888(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 8;

	int nTable[nTableSize][3]	=
	{
		{ 671, 15, 1},
		{ 672, 20, 1},
		{ 673, 5, 1},
		{ 676, 15, 5},
		{ 844, 10, 1},
		{ 846, 10, 1},
		{ 674, 10, 1},
		{ 845, 15, 1},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2889(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 10;

	int nTable[nTableSize][3]	=
	{
		{ 1840, 10, 1},
		{ 1841, 10, 1},
		{ 1842, 10, 1},
		{ 1416, 10, 1},
		{ 1288, 10, 1},
		{ 1285, 15, 1},
		{ 1286, 14, 1},
		{ 1287, 1, 1},
		{ 971, 10, 1},
		{ 882, 10, 5},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2890(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 5;

	int nTable[nTableSize][3]	=
	{
		{ 883, 20, 5},
		{ 2452, 20, 1},
		{ 2453, 20, 1},
		{ 973, 20, 1},
		{ 2139, 20, 1},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2891(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 6;

	int nTable[nTableSize][3]	=
	{
		{ 840, 17, 1},
		{ 841, 17, 1},
		{ 1300, 15, 1},
		{ 2460, 17, 1},
		{ 2463, 17, 1},
		{ 2457, 17, 1},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2892(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 8;

	int nTable[nTableSize][3]	=
	{
		{ 1420, 20, 5},
		{ 1418, 20, 5},
		{ 2361, 20, 2},
		{ 85, 8, 1},
		{ 779, 8, 1},
		{ 974, 8, 1},
		{ 2394, 8, 1},
		{ 2395, 8, 1},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2893(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 12;

	int nTable[nTableSize][3]	=
	{
		{ 1706, 8, 1},
		{ 1707, 8, 1},
		{ 2568, 8, 1},
		{ 2569, 8, 1},
		{ 2399, 8, 1},
		{ 2396, 8, 1},
		{ 2401, 8, 1},
		{ 2398, 8, 1},
		{ 2397, 8, 1},
		{ 2402, 8, 1},
		{ 2359, 10, 5},
		{ 2454, 10, 100},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_2968(CPC* ch, const CItemProto* itemproto)
{
	// 럭키드로우로  DB 로 변경 이함수는 사용하지 않음 // 삭제예정

	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	const int nTableSize	= 34;
	int nTable[nTableSize][5]	=  		// 아이템인덱스, 확률, 개수, plus, flag
	{
		{2844, 2, 1, 0, 0},
		{19, 2, 1000000000, 0, 0},
		{85, 10, 1, 0, 0},
		{2356, 10, 1, 0, 0},
		{2357, 10, 1, 0, 0},
		{884, 6, 1, 0, 0},
		{1288, 5, 1, 0, 0},
		{2569, 15, 1, 0, 0},
		{2568, 15, 1, 0, 0},
		{2404, 15, 1, 0, 0},
		{2894, 5, 1, 0, 0},
		{860, 30, 1, 0, 0},
		{1404, 15, 1, 0, 0},
		{859, 30, 1, 0, 0},
		{1283, 30, 1, 0, 0},
		{972, 30, 1, 0, 0},
		{840, 30, 1, 0, 0},
		{609, 30, 1, 10, 0},
		{1113, 30, 1, 10, 0},
		{479, 30, 1, 10, 0},
		{828, 35, 1, 10, 0},
		{1007, 35, 1, 10, 0},
		{795, 35, 1, 10, 0},
		{476, 35, 1, 10, 0},
		{465, 35, 1, 10, 0},
		{482, 35, 1, 10, 0},
		{826, 35, 1, 10, 0},
		{467, 35, 1, 10, 0},
		{813, 35, 1, 10, 0},
		{499, 80, 1, 0, 12},
		{501, 80, 1, 0, 12},
		{500, 80, 1, 0, 12},
		{498, 80, 1, 0, 12},
		{19, 15, 10000, 0, 0},
	};

	int nRate = GetRandom(1, 1000);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex][0], -1, nTable[nIndex][3], nTable[nIndex][4], nTable[nIndex][2]);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_GoldBox(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	int needitemcount = 100;
	item_search_t vec;
	int sc = ch->m_inventory.searchItemByCondition(3575, 0, 0, vec);
	if (sc == 0 || sc < needitemcount)
	{
		return false;
	}

	int nMaxItemCount, nRandom, nRow, nCol;

	switch(itemproto->getItemIndex())
	{
	case 3576:
		{
			nRow = 0;
			nMaxItemCount = 12;
		}
		break;
	case 3713:
		{
			nRow = 2;
			nMaxItemCount = 6;
		}
		break;
	case 3714:
		{
			nRow = 3;
			nMaxItemCount = 6;
		}
		break;
	case 3715:
		{
			nRow = 4;
			nMaxItemCount = 6;
		}
		break;
	case 3716:
		{
			nRow = 5;
			nMaxItemCount = 6;
		}
		break;
	case 3717:
		{
			nRow = 6;
			nMaxItemCount = 6;
		}
		break;
	case 3762:
		{
			nRow = 7;
			nMaxItemCount = 22;
		}
		break;
	case 3718:
		{
			nRow = 11;
			nMaxItemCount = 1;
		}
		break;

	default:
		return false;
	}

	static const int nTable[][6] =
	{
		{3719, 3720, 3721, 3722, 3723, 3724},		// 3576
		{3725, 3726, 3727, 3728, 3729, 3730},		// 3576
		{3731, 3736, 3742, 3747, 3752, 3757},		// 3713
		{3732, 3737, 3743, 3748, 3753, 3758},		// 3714
		{3733, 3738, 3744, 3749, 3754, 3759},		// 3715
		{3734, 3739, 3745, 3750, 3755, 3760},		// 3716
		{3735, 3740, 3746, 3751, 3756, 3761},		// 3717
		{3553, 3554, 3555, 3556, 3557, 3558},		// 3762
		{3559, 3560, 3561, 3562, 3563, 3564},		// 3762
		{3565, 3566, 3567, 3568, 3569, 3570},		// 3762
		{3571, 3572, 3573, 3574, 0, 0},				// 3762
		{3741,0,},									// 3718
	};

	nRandom = GetRandom(0, nMaxItemCount - 1);

	nRow = nRow + nRandom / 6;
	nCol = nRandom % 6;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nRow][nCol], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	ch->m_inventory.deleteItem(vec, needitemcount);

	return true;
}

bool do_ItemUse_3769 ( CPC* ch, const CItemProto* itemproto )
{
	if( ch->m_level > 5 )							// 사용 레벨 제한
		return false;

	//if( ch->m_job < 0 || ch->m_job > 5)
	if( ch->m_job > 5 )								// 직업제한
		return false;

	if( ch->m_inventory.getEmptyCount() < 7 )			// 장비 7 돈 1
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q( rmsg , ch->m_desc );
		return false;
	}

	int wepon[6][7] = { { 609, 469, 470, 471, 472, 473, 311},
		{ 618, 464, 465, 466, 467, 468, 326},
		{ 345, 474, 475, 476, 477, 478, 627},
		{ 636, 479, 480, 481, 482, 483, 365},
		{ 644, 570, 572, 574, 576, 578, 539},
		{ 997, 1047, 1007, 1017, 1027, 1037, 985 },
	};

	CItem* item[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };

	int i=0;
	for( i=0 ; i<7; i++ )
	{
		item[i] = gserver->m_itemProtoList.CreateItem(wepon[ch->m_job][i], -1, 4, 0, 1);
		if (!item[i])
			break;
	}

	for( i = 0 ; i < 7; i++ )
	{
		if (ch->m_inventory.addItem(item[i]) == false)
		{
			for (int j = i - 1; j >= 0; ++j)
			{
				ch->m_inventory.decreaseItemCount(item[i], item[i]->Count());
			}

			// 집어넣지 못한 아이템 삭제
			for (; i < 7; ++i)
			{
				delete item[i];
			}

			return false;
		}
	}

	ch->m_inventory.increaseMoney(2000000);

	{
		ch->m_statpt_remain += 44;
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->m_exp = 0;
	ch->m_skillPoint += 40000000;
	ch->m_bChangeStatus = true;
	while(ch->m_level < 35)
	{
#ifdef GER_LOG
		GAMELOGGEM << init( 0, "CHAR_LEVEL_UP")
				   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
				   << LOG_VAL("character-id", ch->m_name ) << blank
				   << LOG_VAL("level", ch->m_level+1 ) << blank
				   << endGer;
#endif // GER_LOG
		GAMELOG << init("LEVEL UP", ch)
				<< ch->m_level
				<< " ==> "
				<< ch->m_level + 1
				<< end;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLevelup(rmsg, ch->m_index);
			SEND_Q(rmsg, gserver->m_subHelper);
		}

		ch->m_level++;
		ch->LevelUp(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchMemberChangeInfoMsg(rmsg, ch->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", ch->m_level, 0);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
	ch->CalcStatus(true);

	ch->m_hp = ch->m_maxHP;
	ch->m_mp = ch->m_maxMP;

	return true;
}

bool do_ItemUse_1053(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_level > 5)
		return false;

	int nPromoItem[2][4] =
	{
		{2658, 2659, 2667, 2668},
		{   2, 2   , 2   , 2},
	};

	int* pItemIdx;
	int* pItemCnt;

	pItemIdx = nPromoItem[0];
	pItemCnt = nPromoItem[1];

	for (int i = 0; i < 4; i++)
	{
		if (pItemIdx[i] <= 0)
			continue;

		if(!ch->GiveItem(pItemIdx[i], 0, 0, pItemCnt[i], false))
		{
			GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", ch->m_name, ch->m_desc->m_idname)
					<< pItemIdx[i] << end;

			// 롤백
			for(int j = i - 1; j >= 0; j--)
			{
				CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pItemIdx[j]);
				if (!pItemProto)
					continue;

				item_search_t vec;
				int sc = ch->m_inventory.searchItemByCondition(pItemIdx[j], 0, 0, vec);
				if (sc > 0)
					ch->m_inventory.deleteItem(vec, pItemCnt[i]);
			}

			return false;
		} // GiveItem end
	}

	return true;
}

// 레이드 박스 아이템(일회용)
// 꽝 처리(보상 아이템 : -1)
#ifndef BUGFIX_SPECIALBOX_DROPRATE
bool do_ItemUse_4709(CPC* ch, const CItemProto* itemproto, CItem* item)
{
	if(!item)
		return false;

	bool bEmpty = false;	// 꽝 변수
	bool bInvenFull=false;	// 인벤 풀
	int				nProb;
	CItem* pItem =NULL;
	int nSelItemIndex = 0;
	int nGetCnt=0;

	int nDropNpcIndex = item->GetDropNpc();
	if(nDropNpcIndex == -1)
		return false;

	CNPC* npc = gserver->m_npcProtoList.Create(nDropNpcIndex, NULL);
	if (!npc || !(npc->m_proto))
		return false;

	vec_raiddropdata_t::iterator it = npc->m_proto->m_listRaidDrop.begin();
	vec_raiddropdata_t::iterator endit = npc->m_proto->m_listRaidDrop.end();
	RAIDDROPDATA& rdd = *(it);

	for(; it != endit; ++it)
	{
		rdd = *(it);

		// 확률 적용
		nProb = GetRandom(1, 100);
		if (nProb <= rdd.spec_prob)
		{
			bEmpty = false;
			break;
		}
		else
		{
			bEmpty = true;
			break;
		}
	}

	if(!bEmpty)
	{
		for(int i=1; i <= 14; i++)
		{
			switch(i)
			{
			case 1:
				if(rdd.spec_itemindex1 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex1, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex1;
				}
				break;
			case 2:
				if(rdd.spec_itemindex2 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex2, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex2;
				}
				break;
			case 3:
				if(rdd.spec_itemindex3 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex3, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex3;
				}
				break;
			case 4:
				if(rdd.spec_itemindex4 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex4, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex4;
				}
				break;

			case 5:
				if(rdd.spec_itemindex5 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex5, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex5;
				}
				break;
			case 6:
				if(rdd.spec_itemindex6 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex6, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex6;
				}
				break;
			case 7:
				if(rdd.spec_itemindex7 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex7, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex7;
				}
				break;
			case 8:
				if(rdd.spec_itemindex8 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex8, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex8;
				}
				break;
			case 9:
				if(rdd.spec_itemindex9 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex9, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex9;
				}
				break;
			case 10:
				if(rdd.spec_itemindex10 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex10, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex10;
				}
				break;
			case 11:
				if(rdd.spec_itemindex11 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex11, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex11;
				}
				break;
			case 12:
				if(rdd.spec_itemindex12 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex12, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex12;
				}
				break;
			case 13:
				if(rdd.spec_itemindex13 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex13, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex13;
				}
				break;
			case 14:
				if(rdd.spec_itemindex14 > 0)
				{
					pItem = gserver->m_itemProtoList.CreateItem(rdd.spec_itemindex14, -1, 0, 0, 1);
					nSelItemIndex  = rdd.spec_itemindex14;
				}
				break;
			}

			if (pItem == NULL || !(pItem->m_itemProto))
				continue;

			// 현재 번호의 아이템이 나올지 아니면 다음 아이템인지 절반 확률임
			int nItemProb = GetRandom(1, 2);
			if(nItemProb < 2)
				continue;

			if (ch->m_inventory.addItem(pItem) == false)
			{
				bInvenFull = true;

				//인벤풀 이면 아이템 드랍
				if(nSelItemIndex > 0 && ch->m_pArea)
				{
					pItem = ch->m_pArea->DropItem(nSelItemIndex, ch, 0, 0, 1, true);

					if(pItem && pItem->m_pArea)
					{
						pItem->m_preferenceIndex = -1;
						CNetMsg::SP rmsg(new CNetMsg);
						ItemDropMsg(rmsg, ch, pItem);
						pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
						GAMELOG << init("USE RAID BOX", ch) << "ITEM" << delim << itemlog(pItem) << end;
					}
				}
			}

			nGetCnt++;

			if(nGetCnt >= rdd.spec_count)
				break;
		}
	}

	//꽝에만 보냄
	else
	{
		// 변경 시스템 메세지(꽝)
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeRaidBoxMsg(rmsg, -1, -1);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("USE RAID BOX", ch) << delim
				<< "ITEM GET FAILED"
				<< rdd.prob
				<< end;
	}

	return true;
}
#else // BUGFIX_SPECIALBOX_DROPRATE
bool do_ItemUse_4709(CPC* ch, const CItemProto* itemproto, CItem* item)
{
	if(!item)
		return false;

	int				nProb;

	bool			bSuccess = false;

	int nDropNpcIndex = item->GetDropNpc();
	if(nDropNpcIndex == -1)
		return false;

	CNPC* npc = gserver->m_npcProtoList.Create(nDropNpcIndex, NULL);
	if (!npc || !(npc->m_proto))
		return false;

	vec_raiddropdata_t::iterator it = npc->m_proto->m_listRaidDrop.begin();
	vec_raiddropdata_t::iterator endit = npc->m_proto->m_listRaidDrop.end();
	for(; it != endit; ++it)
	{
		RAIDDROPDATA& rdd = *(it);

		// 확률 적용
		nProb = GetRandom(1, 100);
		if (nProb <= rdd.spec_prob)
		{
			// 여기까지 오면 무조건 드랍
			int		dropbit = 0;

			// 아이템 드랍 플래그 생성
			int dropCount = GetRandom( rdd.spec_Min, rdd.spec_Max );
			int itemCount = 0 , maxRandom = 30;
			while (  itemCount < dropCount )
			{
				int	bitrand = GetRandom( 0, 13 );
				if( rdd.spec_itemindex[bitrand] == -1 || dropbit & (1 << bitrand ) )
				{
					maxRandom--;
					if( maxRandom < 0 )
						break;
					// 아이템이 없는목록이거나 이미 플래그 있음
				}
				else
				{
					dropbit |= (1<< bitrand );
					itemCount++;
				}
			}

			if ( itemCount == 0 )
			{
				bSuccess = false;
				continue;
			}

			// 플래그에 따른 아이템 드랍
			for( int dropidx = 0; dropidx < 14; dropidx++ )
			{
				if ( dropbit & (1<<dropidx ) )
				{
					// 드랍 Bit 가 걸려 있으면 드랍
					int dropItemIndex = -1;

					if( rdd.spec_itemindex[dropidx] > 0 )
						dropItemIndex = rdd.spec_itemindex[dropidx];
					else
						continue;	// 아이템 비었음

					CItem* pItem = gserver->m_itemProtoList.CreateItem( dropItemIndex , -1, 0, 0, 1);
					if (pItem == NULL)
					{
						LOG_ERROR("can't create item. item index[%d]", dropItemIndex);
						continue;
					}

					if (ch->m_inventory.addItem(pItem) == false)
					{
						if(dropItemIndex > 0 && ch->m_pArea)
						{
							{
								//인벤 풀
								CNetMsg::SP rmsg(new CNetMsg);
								SysFullInventoryMsg(rmsg, 0);
								SEND_Q(rmsg, ch->m_desc);
							}

							pItem = ch->m_pArea->DropItem(pItem, (CCharacter*)ch);

							if(pItem && pItem->m_pArea)
							{
								pItem->m_preferenceIndex = ch->m_index;
								CNetMsg::SP rmsg(new CNetMsg);
								ItemDropMsg(rmsg, ch, pItem);
								pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
								GAMELOG << init("USE RAID BOX", ch) << "ITEM" << delim << itemlog(pItem) << end;
							}
						}
					}

					bSuccess = true;
				}
			}
		}
		else
		{
			if( !bSuccess )
				bSuccess = false;
		}
	}

	if( !bSuccess )	// 꽝임
	{
		// 변경 시스템 메세지(꽝)
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeRaidBoxMsg(rmsg, -1, -1);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("USE RAID BOX", ch) << delim
				<< "ITEM GET FAILED" << end;
	}

	if( npc )
		delete npc;

	return true;
}
#endif // BUGFIX_SPECIALBOX_DROPRATE

bool do_ItemUse_IETC_TriggerItem4567(CPC* ch)
{
	int i;
	int debuffIndex[3] = {852, 853, 854};	// 암흑의 제단 1차 네임드 npc가 받을 디버프 3가지

	if (ch->m_pArea == NULL || ch->m_pArea->m_npcList == NULL)
		return false;

	CNPC* npc = NULL;
	CNPC* tnpc = ch->m_pArea->m_npcList;

	if ( tnpc == NULL)
		return false;

	// npcList를 검사한다.
	while ((npc = tnpc))
	{
		tnpc = npc->m_pNPCNext;

		// 암흑의 제단 1차 네임드 npc
		if(npc->m_proto->m_index != 1002)
			continue;

		for (i=0; i < 3; i++)
		{
			CSkill* pDebuff = NULL;
			pDebuff = gserver->m_skillProtoList.Create(debuffIndex[i]);
			if( pDebuff == NULL )
			{
				GAMELOG << init("TriggerItem4567 : USE_DEBUF 1")
						<< "debuf index" << delim
						<< debuffIndex[i]
						<< end;
				return false;
			}

			// 디버프가 걸린다.
			bool bDebuffApply = false;
			ApplySkill( ch, npc, pDebuff, -1, bDebuffApply);

			if (!bDebuffApply)
			{
				delete pDebuff;
				GAMELOG << init("TriggerItem4567 : USE_DEBUF 2")
						<< "debuf index" << delim
						<< debuffIndex[i]
						<< end;
				return false;
			}
			delete pDebuff;
			pDebuff = NULL;
		}

		return true;
	}

	return false;
}

void do_ItemUse_DeleteRaidCharacter_4911(CPC* ch, const CItemProto* itemproto)
{
	// 파티나 원정대 상태이면 사용할 수 없다.
	if (ch->IsParty() || ch->IsExped())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PARTY_OR_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 원정대 레이드 존 안에서는 사용할 수 없다.
	if (ch->m_pZone)
	{
		if (ch->m_pZone->m_index == ZONE_ALTER_OF_DARK || ch->m_pZone->m_index == ZONE_AKAN_TEMPLE)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_RAIDZONE);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	else
		return;

	// 인벤토리 아이템 검사
	if (ch->m_inventory.FindByDBIndex(itemproto->getItemIndex(), 0, 0) == NULL)
		return;

	{
		// CharDB에서 레이드 귀속 정보를 지우기 위해 메시지를 보낸다.
		// 해당 아이템 삭제는 헬퍼에서 성공했다는 메세지를 받으면 한다.
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDeleteRaidCharacterMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	return;
}

bool do_ItemUse_4664(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	static const int nTableSize	= 21;

	static const int nTable[nTableSize]	=
	{
		3774, 3780, 3776, 3784,
		3778, 3812, 3782, 3830,
		3831, 3838, 3839, 3846,
		3847, 3854, 3855, 3862,
		3863, 4778, 4779, 4599,
		4603,
	};

	int nIndex = GetRandom(0, nTableSize-1);
	if(nIndex < 0 || nIndex >= nTableSize)
		return false;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_4786(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	static const int nTableSize	= 21;

	static const int nTable[nTableSize]	=
	{
		3788, 3794, 3790, 3786,
		3792, 3814, 3796, 3832,
		3833, 3840, 3841, 3848,
		3849, 3856, 3857, 3864,
		3865, 4780, 4781, 4600,
		4604,
	};

	int nIndex = GetRandom(0, nTableSize-1);
	if(nIndex < 0 || nIndex >= nTableSize)
		return false;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_4787(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	static const int nTableSize	= 21;

	static const int nTable[nTableSize]	=
	{
		3802, 3808, 3804, 3798,
		3806, 3826, 3810, 3834,
		3835, 3842, 3843, 3850,
		3851, 3858, 3859, 3866,
		3867, 4782, 4783, 4601,
		4605,
	};

	int nIndex = GetRandom(0, nTableSize-1);
	if(nIndex < 0 || nIndex >= nTableSize)
		return false;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

bool do_ItemUse_4788(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤에 공간이 없으면 실패
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	static const int nTableSize	= 21;

	static const int nTable[nTableSize]	=
	{
		3816, 3822, 3818, 3800,
		3820, 3828, 3824, 3836,
		3837, 3844, 3845, 3852,
		3853, 3860,	3861, 3868,
		3869, 4784, 4785, 4602,
		4606,
	};

	int nIndex = GetRandom(0, nTableSize-1);
	if(nIndex < 0 || nIndex >= nTableSize)
		return false;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return false;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}


bool do_ItemUse_LuckyDrawBox(CPC* ch, const CItemProto* itemproto)
{
	LuckyDrawBoxData * pLucky = gserver->m_LuckyDrawBox.GetData(itemproto->getItemNum0());
	if( pLucky == NULL)
		return false;

	// Random Type 별로 나눠서 처리 하자
	switch( pLucky->random )
	{
	case 0: // prob
	case 1: // random
		return do_ItemUse_LuckyDrawBoxResult_Prob_Random(ch, itemproto, pLucky);
		break;
	case 2: // all
		return do_ItemUse_LuckyDrawBoxResult_All(ch, itemproto, pLucky);
		break;
	}
	return false;
}

bool do_ItemUse_LuckyDrawBoxResult_Prob_Random(CPC* ch, const CItemProto* itemproto, LuckyDrawBoxData * pLucky)
{
	boost::scoped_array<item_search_t> vec(new item_search_t[pLucky->nNeedCount]);

	for(int i = 0; i < pLucky->nNeedCount; i++)
	{
		int sc = ch->m_inventory.searchItemByCondition(pLucky->needList[i].item_idx, 0, 0, vec[i]);
		if (sc == 0 || sc < pLucky->needList[i].count)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_NEED);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}

	int nTotalRate = 0;
	int nResultIdx = -1;

	// 지급아이템 축출
	switch( pLucky->random )
	{
	case 0: // prob
		{
			int nRate = GetRandom(0, 10000);
			for(int i = 0; i < pLucky->nResultCount; i++)
			{
				nTotalRate += pLucky->resultList[i].prob;
				if( nTotalRate  >= nRate)
				{
					nResultIdx = i;
					break;
				}
			}
		}
		break;
	case 1: // random
		nResultIdx = GetRandom(0, pLucky->nResultCount-1);
		break;
	}

	if( nResultIdx == -1 || nResultIdx >= pLucky->nResultCount )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 인벤에 공간이 없으면 실패
	CItemProto * pProto = gserver->m_itemProtoList.FindIndex(pLucky->resultList[nResultIdx].item_idx);
	if( pProto == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
	// 원하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
	LONGLONG checkCount = pLucky->resultList[nResultIdx].count;
	if( pProto->getItemFlag() & ITEM_FLAG_COUNT)
		checkCount = 1;

	if((LONGLONG)ch->m_inventory.getEmptyCount() < checkCount)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 결과 아이템 생성
	CItem * pRItem = gserver->m_itemProtoList.CreateItem(pLucky->resultList[nResultIdx].item_idx, -1,
					 pLucky->resultList[nResultIdx].upgrade,
					 pLucky->resultList[nResultIdx].flag,
					 pLucky->resultList[nResultIdx].count);

	if(pRItem == NULL)
		return false;

	GAMELOG << init("LUCKYDRAWBOX_RESULT_ITEM_CREATE", ch )
			<< itemlog(pRItem)
			<< end;

	// 아이템 지급
	if (ch->m_inventory.addItem(pRItem) == false)
	{
		delete pRItem;

		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
		SEND_Q(rmsg, ch->m_desc);

		return false;
	}

	// 필요 아이템 삭제
	for(int i = 0; i < pLucky->nNeedCount; i++)
	{
		ch->m_inventory.deleteItem(vec[i], pLucky->needList[i].count);
	}

	return true;

}

// 거의 같은 기능... 나중에 위 소스를 하나로합치자... 좀 지져분 해지겠지만. ..ㅡㅡ;;
bool do_ItemUse_LuckyDrawBoxResult_All(CPC* ch, const CItemProto* itemproto, LuckyDrawBoxData * pLucky)
{
	// 인벤에 need 아이템 확인
	boost::scoped_array<item_search_t> vec(new item_search_t[pLucky->nNeedCount]);
	for (int i = 0; i < pLucky->nNeedCount; i++)
	{
		int sc = ch->m_inventory.searchItemByCondition(pLucky->needList[i].item_idx, 0, 0, vec[i]);
		if (sc == 0 || sc < pLucky->needList[i].count )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_NEED);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
	}

	int totalCount = 0;
	CItemProto * pProto;
	std::vector<LuckyDrawBoxResult> resultList;

	for (int i = 0; i < pLucky->nResultCount; i++)
	{
		pProto = gserver->m_itemProtoList.FindIndex(pLucky->resultList[i].item_idx);
		if( pProto == NULL )
		{
			totalCount =0;
			break;
		}

		if( pProto->getItemFlag() & ITEM_FLAG_COUNT)
		{
			LuckyDrawBoxResult LuckyResult;
			LuckyResult = pLucky->resultList[i];
			resultList.push_back(LuckyResult);
		}
		else
		{
			int j=0;
			for(j=0; j<pLucky->resultList[i].count; j++)
			{
				LuckyDrawBoxResult LuckyResult;
				LuckyResult = pLucky->resultList[i];
				LuckyResult.count = 1;
				resultList.push_back(LuckyResult);
			}
		}
	}

	totalCount = resultList.size();

	if( totalCount == 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
		SEND_Q(rmsg, ch->m_desc);
		resultList.clear();
		return false;
	}

	// 인벤에 공간이 없으면 실패
	if (ch->m_inventory.getEmptyCount() < totalCount)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		resultList.clear();
		return false;
	}

	// 결과 아이템 생성
	CItem ** pRItem = new CItem*[totalCount];
	memset(pRItem, 0x00, sizeof(pRItem));

	int resultListSize = (int)resultList.size();
	for(int i = 0, n = 0; i < resultListSize; i++, n++)
	{
		pRItem[n] = gserver->m_itemProtoList.CreateItem(resultList[i].item_idx, -1,
					resultList[i].upgrade, resultList[i].flag, resultList[i].count);

		if(pRItem[n] == NULL)
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
				SEND_Q(rmsg, ch->m_desc);
			}

			for(int del = 0; del < totalCount; del++)
			{
				if( pRItem[del] )
				{
					delete pRItem[del];
					pRItem[del] =NULL;
				}
			}

			delete [] pRItem;
			return false;
		}

		GAMELOG << init("LUCKYDRAWBOX_RESULT_ITEM_CREATE", ch )
				<< itemlog(pRItem[n])
				<< end;
	}
	// 아이템 지급
	for(int i = 0; i < totalCount; i++)
	{
		if (pRItem[i] == NULL)
			continue;

		if (ch->m_inventory.addItem(pRItem[i]) == false)
		{
			// 지급 했던 아이템을 삭제 한다..
			for (int j = 0; j < i; j++)
			{
				item_search_t delvec;
				int sc = ch->m_inventory.searchItemByCondition(pRItem[j]->getDBIndex(), pRItem[j]->getPlus(), pRItem[j]->getFlag(), delvec);
				if (sc > 0)
				{
					ch->m_inventory.deleteItem(delvec, resultList[j].count);
				}
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				LuckyDrawBoxErrorMsg(rmsg, MSG_LUCKYDRAWBOX_ERROR_USE);
				SEND_Q(rmsg, ch->m_desc);
			}

			resultList.clear();
			for (int j = 0; j < totalCount; j++)
			{
				if( pRItem[j] )
				{
					delete  pRItem[j] ;
					pRItem[j] = NULL;
				}
			}
			delete [] pRItem;
			return false;
		}
	}

	// 필요 아이템 삭제
	for (int i = 0; i < pLucky->nNeedCount; i++)
	{
		ch->m_inventory.deleteItem(vec[i], pLucky->needList[i].count);
	}

	resultList.clear();

	delete [] pRItem;

	return true;
}

void do_ItemWear_Costume(CPC* ch, CNetMsg::SP& msg)
{
	if(ch->m_evocationIndex != EVOCATION_NONE)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	RequestClient::doItemWearCostume* packet = reinterpret_cast<RequestClient::doItemWearCostume*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);

	if( ch->m_wearInventory.AddCostumItem(item, packet->wearPos) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::CostItemWearMsg(rmsg, ch->m_wearInventory.GetLastError());
		SEND_Q(rmsg, ch->m_desc);
	}
	ch->CalcStatus(true);
}
void do_Itemwear_Costume_TakeOff(CPC* ch, CNetMsg::SP& msg)
{
	if(ch->m_evocationIndex != EVOCATION_NONE)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	RequestClient::doItemWearCostumeTakeOff* packet = reinterpret_cast<RequestClient::doItemWearCostumeTakeOff*>(msg->m_buf);

	if (packet->wearPos < 0 || packet->wearPos >= MAX_WEARING)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::CostItemTakeOffMsg(rmsg, ResponseClient::WEAR_ERR_CANNOT_TAKEOFF);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* item = ch->m_wearInventory.wearItemInfo[(int)packet->wearPos];		// 벗는 아이템

	if (ch->m_wearInventory.DelCostumItem(packet->wearPos, packet->tab, packet->invenIndex) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::CostItemTakeOffMsg(rmsg, ch->m_wearInventory.GetLastError());
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ch->CalcStatus(true);
}

void do_ItemWear_Costume_Suit(CPC* ch, CNetMsg::SP& msg)
{
	if(ch->m_evocationIndex != EVOCATION_NONE)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	RequestClient::doItemWearCostumeSuit* packet = reinterpret_cast<RequestClient::doItemWearCostumeSuit*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* pSuit = ch->m_inventory.getItem(packet->tab, packet->invenIndex);

	//캐릭터 업데이트
	ch->m_wearInventory.AddCostumSuitItem(pSuit);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::CostSuitItemWearMsg(rmsg, ch->m_wearInventory.GetLastError());
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}
void do_ItemWear_Costuem_Suit_TakeOff(CPC* ch, CNetMsg::SP& msg)
{
	if(ch->m_evocationIndex != EVOCATION_NONE)
	{
		LOG_ERROR("HACKING? player is none evocation state charIndex[%d]", ch->m_index);
		ch->m_desc->Close("Invalid state");
		return;
	}

	RequestClient::doItemWearCostumeSuitTakeOff* packet = reinterpret_cast<RequestClient::doItemWearCostumeSuitTakeOff*>(msg->m_buf);

	if(ch->m_wearInventory.getCosWearSuitItem() == NULL)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] Not wear Costum Suit Item",
				  ch->m_index);
		ch->m_desc->Close("invalid packet");
		return;
	}

	ch->m_wearInventory.DelCostumSuitItem(packet->tab, packet->invenIndex);
}
void do_Item_Make_Fortune(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemMakeFortune* packet = reinterpret_cast<RequestClient::doItemMakeFortune*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* pSuit = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (pSuit == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return ;
	}

	int ret = 0;
	int skill_index = 0;
	int skill_level = 0;

	if ((ret = gserver->m_Fortunelist.Make_fortune(pSuit)) != 0)
	{
		GAMELOG << init("ERROR GET FORTUNE") << "ERROR_TYPE" << delim << ret << end;
	}
	else
	{
		gserver->m_Fortunelist.Resolve_fortune_value(skill_index, skill_level, pSuit->getPlus());

		ch->m_inventory.sendOneItemInfo(pSuit);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		NoticeFortuneMsg(rmsg, ret, pSuit->m_itemProto->getItemIndex(), skill_index, skill_level);
		SEND_Q(rmsg, ch->m_desc);
	}
}

bool do_ItemUse_WorldFestivalBox(CPC* ch, CItem* pItem)
{
	// 1번 항목 월드컵 32강 국가 국기 아이템
	int giftTable1[32][3] =
	{
		{1485,  1, 100},	//브라질 국기 
		{1486,	1, 100},	//독일 국기 
		{1487,	1, 100},	//이탈리아 국기 
		{1488,	1, 100},	//영국 국기 
		{1489,	1, 100},	//아르헨티나 국기 
		{1491,	1, 100},	//네덜란드 국기 
		{1492,	1, 100},	//스페인 국기 
		{1493,	1, 100},	//포르투갈 국기 
		{5332,	1, 100},	//우루과이 국기 
		{1490,	1, 200},	//프랑스 국기 
		{1496,	1, 200},	//멕시코 국기 
		{1498,	1, 200},	//크로아티아
		{1499,	1, 200},	//미국 국기 
		{1501,	1, 200},	//코트디부아르 국기 
		{10297,	1, 200},	//벨기에
		{10298,	1, 200},	//콜롬비아
		{10300,	1, 200},	//러시아
		{1500,	1, 500},	//스위스 국기 
		{1505,	1, 500},	//대한민국 국기 
		{1506,	1, 500},	//호주 국기 
		{1507,	1, 500},	//일본 국기 
		{1508,	1, 500},	//가나 국기 
		{1510,	1, 500},	//에콰도르
		{1513,	1, 500},	//이란
		{1515,	1, 500},	//코스타리카
		{5333,	1, 500},	//나이지리아 국기 
		{5334,	1, 500},	//그리스 국기 
		{5338,	1, 500},	//카메룬 국기 
		{5342,	1, 500},	//온두라스 국기 
		{5343,	1, 500},	//칠레 국기 
		{5344,	1, 500},	//알제리 국기 
		{10299,	1, 500},	//보스니아 헤르체고비나

	};

	// 2번 항목
	int giftTable2[8][3] =
	{
		// 아이템 인덱스, 개수, 확률
		{3768,  1,  500}, //  1. (이벤트용) 고급 제련석
		{4968,  1,  500}, //  1. (이벤트용) 악마의 룬 아르
		{2664,  1,  500}, //  3. (이벤트용) 결합주문서
		{2033,  1, 1000}, //  4. 럭키 소울닉스 포션
		{1404,  1, 1000}, //  5. 소형 HP,MP확장 포션
		{1283, 10, 2000}, //  6. 실버 부스터
		{1315, 10, 2500}, //  7. 공격력 향상제(대)
		{2875,  1, 2000}, //  8. 데미지 증폭제
	};

	// 3번 항목
	int giftTable3[7][3] =
	{
		// 아이템 인덱스, 개수, 확률
		{4966, 1, 500},		//  1. 플래티늄 아이리스의 축복
		{4938, 1, 500},		//  2. (이벤트용) 중형 경험의 묘약
		{4937, 1, 500},		//  3. (이벤트용) 플래티늄 훈련주문서
		{2034, 1, 1000},	//  4. 럭키 다지피어 포션
		{4967, 5, 2000},	//  5. (이벤트용) 중형 특효약
		{1314, 10, 2500},	//  6. 방어력 향상제 (대)
		{2596, 20, 3500},	//  7. 알록달록 막대사탕
	};

	int giftCount = 3;
	int giftItemCount[]= {32,8,7};
	
	boost::scoped_array< GIFTITEMINFO >	giftItemInfo(new GIFTITEMINFO[giftCount]);

	// 지급할 상품 3개를 뽑아보자.
	int i,j;
	for(i=0; i<giftCount; i++)
	{
		int totalProb = 0;
		int prob = GetRandom(1,10000);

		for(j=0; j<giftItemCount[i]; j++)
		{
			switch(i)
			{
			case 0:
				totalProb += giftTable1[j][2];
				giftItemInfo[i].index = giftTable1[j][0];
				giftItemInfo[i].count = giftTable1[j][1];
				break;
			case 1:
				totalProb += giftTable2[j][2];
				giftItemInfo[i].index = giftTable2[j][0];
				giftItemInfo[i].count = giftTable2[j][1];
				break;
			case 2:
				totalProb += giftTable3[j][2];
				giftItemInfo[i].index = giftTable3[j][0];
				giftItemInfo[i].count = giftTable3[j][1];
				break;
			}
			if( prob <= totalProb )
				break;
		}
	}

	// 교환 아이템 생성
	boost::scoped_array< CItem* > pGItem(new CItem*[giftCount]);

	for(i=0; i<giftCount; i++)
	{
		pGItem[i] = gserver->m_itemProtoList.CreateItem( giftItemInfo[i].index, -1, 0, 0, giftItemInfo[i].count);
		if( pGItem[i] == NULL )
		{
			for(j=0; j<i; j++)
			{
				if( pGItem[j] )
				{
					delete pGItem[j];
					pGItem[j] = NULL;
				}
			}

			return false;
		}
	}

	int needSlot = 0;

	for(i=0; i<giftCount; i++)
	{
		if( i > 0)
		{
			if( pGItem[i]->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT)
				needSlot += 1;
			else
				needSlot += pGItem[i]->Count();
		}
	}

	if( ch->m_inventory.getEmptyCount() < (needSlot + 1))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 요기까지 모든 아이템 지급 준비 완룡....
	// 상자를 삭제하고 아이템을 지급 해봅시당.
	// 상자 아이템은 이함수로 넘어오기전에 검사가 되었으므로
	// 업데이트인지 삭제인지만 확인 하장

	// 월드 축제 상자 삭제
	if (pItem)
	{
		ch->m_inventory.decreaseItemCount(pItem, 1);
	}

	for(i = 0; i < giftCount; i++)
	{
		ch->m_inventory.addItem(pGItem[i]);
	}

	return true;
}

bool do_ItemUse_5329(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
	/*
	 * remove all jewel
	 * close all socket
	 */
	CItem* pEquipItem = ch->m_inventory.FindByVirtualIndex(nExtar1);

	if(!pEquipItem || !pEquipItem->IsSocketItem() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SocketErrorMsg(rmsg, MSG_EX_SOCKET_ERROR_NOWEARITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	// 결합된 상태에서는 안됨
	if(pEquipItem->getFlag() & FLAG_ITEM_COMPOSITION)
		return false;

	// 착용한 경우는 안됨
	if(pEquipItem->getWearPos() != WEARING_NONE)
		return false;

	if(!(pEquipItem->getFlag() & FLAG_ITEM_SOCKET))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SocketErrorMsg(rmsg, MSG_EX_SOCKET_ERROR_FLAG_SOCKET);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	pEquipItem->setFlag(pEquipItem->getFlag() & ~FLAG_ITEM_SOCKET);
	pEquipItem->m_socketList.InitSocketData();
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SOCKET_CARD_USE_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->m_inventory.sendOneItemInfo(pEquipItem);

	GAMELOG << init("ITEM_USE_SOCKET_CARD_OK", ch)
			<< itemlog(pEquipItem)
			<< end;
	return true;
}

void do_ItemUse_IETC_MonsterMercearyToggle(CPC* pPc, CItem* pItem)
{
	if( pPc->IsInPeaceZone(true) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_AREA);
		SEND_Q(rmsg, pPc->m_desc);
		return;
	}

	if( pPc->m_pZone->IsPersonalDungeon())
		//	|| pPc->m_pZone->m_index == ZONE_GUILDROOM || pPc->m_pZone->m_index == ZONE_COMBO_DUNGEON ) // || pPc->IsInRaidZone() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_DONT_SUMMON);
		SEND_Q(rmsg, pPc->m_desc);
		return;
	}

	if( pPc->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) ) // 활성화 되어있다.
	{
		// 사용한 카드가 아니거나 다른 카드이면 그냥 리턴
		if( !(pItem->getUsed() != -1 && pItem->m_MercenaryIndex != -1
				&& pItem->m_MercenaryIndex == pPc->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY)->m_index
				&& pItem->getPlus() == pPc->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY)->m_idNum) )// index
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_SUMMON_ALREADY);
			SEND_Q(rmsg, pPc->m_desc);
			return;
		}

		pItem->m_MercenaryIndex = -1;
		pPc->SummonNpcRemove(SUMMON_NPC_TYPE_MERCENARY, true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemUseMsg( rmsg, pItem->tab(), pItem->getInvenIndex(), pItem->m_itemProto->getItemIndex(), 0 );
			SEND_Q( rmsg, pPc->m_desc );
		}

		{
			// 몬스터 용병 소환 해지를 알리는 메시지를 보내자
			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenarySummonMsg(rmsg, pItem->getVIndex(), -1 );
			SEND_Q(rmsg, pPc->m_desc);
		}

		return;
	}
	else // 몬스터를 add 시키고 상태를 변경하자.
	{
		if( pItem->getUsed() == -1) // 한번이라도 사용하지 않은 카드라면 바로 시간 세팅
		{
			pItem->setUsed(gserver->getNowSecond() + pItem->m_itemProto->getItemNum2()  * 60 * 60); // m_num2 = hour
			pItem->setFlag(pItem->getFlag() | FLAG_ITEM_SEALED); // 거래가 않되도록 봉인 해버린다.

			pPc->m_inventory.sendOneItemInfo(pItem);
		}

		CNPCProto * proto = gserver->m_npcProtoList.FindProto( pItem->getPlus() );
		if( proto == NULL )
		{
			GAMELOG << init("MONSTER MERCENARY FAIL :: create monster", pPc)
					<< "npc index : " << pItem->getUsed() << end;

			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_USE_FAIL);
			SEND_Q(rmsg, pPc->m_desc);
			return;
		}

		CNPC * pMercenayNpc = gserver->m_npcProtoList.Create(proto->m_index, NULL );
		if( pMercenayNpc == NULL )
		{
			GAMELOG << init("MONSTER MERCENARY FAIL :: create monster", pPc)
					<< "npc index : " << proto->m_index << end;

			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_USE_FAIL);
			SEND_Q(rmsg, pPc->m_desc);
			return;
		}
		pItem->m_MercenaryIndex = pMercenayNpc->m_index;
		// 스킬을 먼저 걸자
		CSkill * pSkill = gserver->m_skillProtoList.Create( pItem->m_itemProto->getItemNum0(), pItem->m_itemProto->getItemNum1());
		if( pSkill == NULL )
		{
			GAMELOG << init("MONSTER MERCENARY FAIL :: create skill", pPc)
					<< "npc index : " << proto->m_index << end;

			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_USE_FAIL);
			SEND_Q(rmsg, pPc->m_desc);

			delete pMercenayNpc;
			pMercenayNpc = NULL;
			return ;
		}

		bool bApply;
		// 스킬 사용 남은 시간을 여기서 강제 세팅 해보자.
		pSkill->m_usetime =( pItem->getUsed() - gserver->getNowSecond() ) * 10;
		ApplySkill(pPc, pMercenayNpc, pSkill, -1, bApply);
		delete pSkill;
		pSkill = NULL;

		if( bApply == false )
			return;

		// 아이템이 가진 클레스번호로 정보를 가져온다.
		MonsterMercenaryData * pMCData = NULL;
#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
		if( pItem->getDBIndex() == 6805 ) // 초보 아이템의 경우
			pMCData = gserver->m_MonsterMercenary.CreateClassData(pItem->getUsed_2());
		else
			pMCData = gserver->m_MonsterMercenary.CreateClassData(pItem->getUsed_2(), pItem->getPlus());
#else
		pMCData = gserver->m_MonsterMercenary.CreateClassData(pItem->getUsed_2());
#endif
		if( !pMCData )
		{
			GAMELOG << init("MOSTER MERCENARY FAIL : get MercenaryInfo") << end;
			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_USE_FAIL);
			SEND_Q(rmsg, pPc->m_desc);
			return;
		}

		pMCData->nBonusAtt = (pItem->getPlus_2() >> 8) & 0x00000ff;
		pMCData->nBonusDef = pItem->getPlus_2() & 0x000000ff;

		pMercenayNpc->SetMercenaryClassData( pMCData );

		// 필드로 올리자
		GET_X(pMercenayNpc) = GET_X(pPc);
		GET_Z(pMercenayNpc) = GET_Z(pPc);
		GET_R(pMercenayNpc) = GET_R(pPc);
		GET_YLAYER(pMercenayNpc) = GET_YLAYER(pPc);

		pMercenayNpc->m_regenX = GET_X(pMercenayNpc);
		pMercenayNpc->m_regenZ = GET_Z(pMercenayNpc);
		pMercenayNpc->m_regenY = GET_YLAYER(pMercenayNpc);

		pMercenayNpc->CalcStatus(false);

		if( pMercenayNpc->m_maxHP < pMercenayNpc->m_hp)
			pMercenayNpc->m_hp = pMercenayNpc->m_maxHP;

		int cx, cz;
		pPc->m_pArea->AddNPC(pMercenayNpc);
		pPc->m_pArea->PointToCellNum(GET_X(pMercenayNpc), GET_Z(pMercenayNpc), &cx, &cz);
		pPc->m_pArea->CharToCell(pMercenayNpc, GET_YLAYER(pMercenayNpc), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, pMercenayNpc, true);
			pPc->m_pArea->SendToCell(rmsg, GET_YLAYER(pMercenayNpc), cx, cz);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			CharStatusMsg(rmsg, pMercenayNpc, 0);
			pPc->m_pArea->SendToCell(rmsg, pMercenayNpc, false);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemUseMsg( rmsg, pItem->tab(), pItem->getInvenIndex(), pItem->m_itemProto->getItemIndex(), 0 );
			SEND_Q( rmsg, pPc->m_desc );
		}

		{
			// 몬스터 용병 소환 메시지를 보내자.
			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenarySummonMsg(rmsg, pItem->getVIndex(), 1 );
			SEND_Q(rmsg, pPc->m_desc);
		}

		return;
	}
}


bool do_ItemUse_6593(CPC* ch, const CItemProto* itemproto)
{
	int rand =  GetRandom(0, 4);
	int skillindex;
	switch(rand)
	{
	case 0:
		skillindex = 1198;
		break;
	case 1:
		skillindex = 1199;
		break;
	case 2:
		skillindex = 1200;
		break;
	case 3:
		skillindex = 1201;
		break;
	case 4:
		skillindex = 1202;
		break;
	default:
		return false;
	}

	CSkill* skill = gserver->m_skillProtoList.Create(skillindex);
	if(!skill)
		return false;

	bool bApply;
	ApplySkill(ch, ch, skill, -1, bApply);
	delete skill;
	skill = NULL;
	return true;
}

void do_ItemUse_MasterStone_USA(CPC* ch, CNetMsg::SP& msg)
{
	enum MasterStone { LOWER_MASTERSTONE=6111, LOW_MASTERSTONE=6112, NORMAL_MASTERSTONE=6113, HIGH_MASTERSTONE=6114, HIGHER_MASTERSTONE=6115, MASTERSTONE=6124, ARMOR_MASTERSTONE=6120 ,
					   LOWER_MASTERTONE2=7730, LOW_MASTERSTONE2=7731, NORMAL_MASTERSTONE2=7732, HIGH_MASTERSTONE2=7733, MASTERSTONE2=7734, MASTERSTONE3=7735, MASTERSTONE4=10970, MASTERSTONE5=10971,
					 };
	
	RequestClient::doItemUseMasterStoneUSA* packet = reinterpret_cast<RequestClient::doItemUseMasterStoneUSA*>(msg->m_buf);

	{
		std::set<int> checkdup;
		int key = 0;

		key = makeCheckDupKey(packet->tab_1, packet->invenIndex_1);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}

		key = makeCheckDupKey(packet->tab_2, packet->invenIndex_2);
		if (checkdup.insert(key).second == false)
		{
			LOG_ERROR("HACKING? : dup index. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("dup index");
			return;
		}
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_1, packet->invenIndex_1) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab_2, packet->invenIndex_2) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_2, packet->invenIndex_2);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 아이템 확인
	CItem	*pMasterStone = ch->m_inventory.getItem(packet->tab_1, packet->invenIndex_1);
	CItem	*pUpgradeItem = ch->m_inventory.getItem(packet->tab_2, packet->invenIndex_2);

	if( pMasterStone == NULL || pMasterStone->getDBIndex() != packet->itemDBIndex_1 )
	{
		LOG_ERROR("HACKING? : not found master stone item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("not found master stone item");

		return;
	}

	if( pUpgradeItem == NULL || pUpgradeItem->getDBIndex() != packet->itemDBIndex_2 )
	{
		LOG_ERROR("HACKING? : not found upgrade item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_1, packet->invenIndex_1);
		ch->m_desc->Close("not found upgrade item");

		return;
	}

	if (pUpgradeItem->IsLent() == true)
	{
		LOG_INFO("upItem flag is Lent item");
		ch->m_desc->Close("upItem flag is Lent item");
		return;
	}

	
	CItem* compose_item = NULL;

	if (pUpgradeItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
	{
		if ((pUpgradeItem->getFlag() & FLAG_ITEM_COMPOSITION) != 0)
		{
			CItem* dummy_item = NULL;

			dummy_item = ch->m_inventory.FindByVirtualIndex(pUpgradeItem->m_nCompositeItem);
			if (dummy_item == NULL)
			{
				LOG_INFO("not found composite item with upItem");
				return;
			}
			compose_item = pUpgradeItem;
			pUpgradeItem = dummy_item;
		}
	}
	
	// 조건 확인
	char error = 0;
	switch( packet->itemDBIndex_1 )
	{
	case LOWER_MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 16 )										error=2;
		}
		break;
	case LOW_MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 18 )										error=2;
		}
		break;
	case NORMAL_MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 20 )										error=2;
		}
		break;
	case HIGH_MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 22 )										error=2;
		}
		break;
	case HIGHER_MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 24 )										error=2;
		}
		break;
	case MASTERSTONE:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 25 )										error=2;
		}
		break;
	case ARMOR_MASTERSTONE:
		{
			if( !pUpgradeItem->IsArmorType() )											error=1;
			else if ( pUpgradeItem->getPlus() > 24 )										error=2;
		}
		break;
	case LOWER_MASTERTONE2:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 17 )										error=2;
		}
		break;
	case LOW_MASTERSTONE2:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 19 )										error=2;
		}
		break;
	case NORMAL_MASTERSTONE2:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 21 )										error=2;
		}
		break;
	case HIGH_MASTERSTONE2:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 23 )										error=2;
		}
		break;
	case MASTERSTONE2:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 26 )										error=2;
		}
		break;
	case MASTERSTONE3:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 27 )										error=2;
		}
		break;
	case MASTERSTONE4:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 28 )										error=2;
		}
		break;
	case MASTERSTONE5:
		{
			if( !pUpgradeItem->IsWeaponType() && !pUpgradeItem->IsArmorType() )			error=1;
			else if ( pUpgradeItem->getPlus() > 29 )										error=2;
		}
		break;
	default:	// 마스터 스톤이 아님
		{
			error = 1;
		}
		break;
	}

	int random_success = 0;

	if( error == 0 )		// 사용합시다.
	{
		// 로그 부터 남기고
		GAMELOG << init("USE_MASTER_STONE", ch ) << delim
				<< "USE :" << itemlog( pMasterStone, true ) << delim ;

		// 마스터 스톤 회수
		ch->m_inventory.decreaseItemCount(pMasterStone, 1);

		GAMELOG << "[BEFORE] " << itemlog(pUpgradeItem) << delim ;

		//마스터 스톤 확률 추가 1 : 상승, 2 : 유지, 3 : 하락
		random_success = GetRandom(MASTERSTONE_INCREASE, MASTERSTONE_DECREASE);

		//16이하 아이템은 무조건 상승
		if(pUpgradeItem->getPlus() <= 16)
		{
			random_success = MASTERSTONE_INCREASE;
		}

		switch(random_success)
		{
			//상승
		case MASTERSTONE_INCREASE:
			pUpgradeItem->setPlus(pUpgradeItem->getPlus() + 1);
			if(compose_item != NULL)
			{
				compose_item->setFlag(FLAG_ITEM_PLATINUM_SET(compose_item->getFlag(), pUpgradeItem->getPlus()));
				compose_item->setPlus(compose_item->getPlus() + 1);
			}
			break;
			//유지
		case MASTERSTONE_NOT_CHANGE:
			break;
			//하락
		case MASTERSTONE_DECREASE:
			pUpgradeItem->setPlus(pUpgradeItem->getPlus() - 1);
			if(compose_item != NULL)
			{
				compose_item->setFlag(FLAG_ITEM_PLATINUM_SET(compose_item->getFlag(), pUpgradeItem->getPlus()));
				compose_item->setPlus(compose_item->getPlus() - 1);
			}
			break;
		}

		//아이템 귀속.
		pUpgradeItem->setFlag(pUpgradeItem->getFlag() | FLAG_ITEM_BELONG);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ch->m_inventory.sendOneItemInfo(pUpgradeItem);
			SEND_Q(rmsg, ch->m_desc);
		}

		if(compose_item != NULL)
		{
			ch->m_inventory.sendOneItemInfo(compose_item);
		}

		GAMELOG << "[AFTER] " << itemlog(pUpgradeItem) << delim << random_success << delim << end ;

		ch->m_assist.CheckApplyConditions();
		ch->CalcStatus(true);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeMasterStone(rmsg, random_success, error);
		SEND_Q(rmsg, ch->m_desc);
	}
}

bool canJumping(CPC* ch, int _jumpingLevel)
{
	// TODO 1 :: 점핑 레벨 체크
	if ( _jumpingLevel <= 0 )
	{
		return false;
	}

	// TODO 2 :: 현재 캐릭터 레벨 < 점핑 레벨 검사
	if ( ch->m_level >= _jumpingLevel )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		JumpingResultMsg(rmsg, MSG_EX_JUMPING_ERROR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// TODO 3 :: 같은 계정 내 캐릭터들의 레벨 구해서 점핑레벨과 비교
	int userIndex = ch->m_desc->m_index;
	CLCString compareQuery(512);
	compareQuery.Format("SELECT a_level FROM t_characters WHERE a_index != %d AND a_user_index = %d AND a_enable = 1 ORDER BY a_level DESC LIMIT 1", ch->m_index, userIndex);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcharingame);
	cmd.SetQuery(compareQuery);
	if ( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		JumpingResultMsg(rmsg, MSG_EX_JUMPING_ERROR_NOT_EXIST_CHAR, _jumpingLevel);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	int _level = 0;
	cmd.GetRec("a_level", _level);

	if ( _level >= _jumpingLevel )
	{
		return true;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		JumpingResultMsg(rmsg, MSG_EX_JUMPING_ERROR_NOT_EXIST_CHAR, _jumpingLevel);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
}

int getUpgradeRuneResult(CItem *reItem, CItem *upItem, CPC *ch)
{
	int ret = -1;

	if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_CHAOS_RUNE)
	{
		if (upItem->getPlus() < 12)
		{
			ret = ITEM_UPGRADE_RESULT_PLUS;
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_USE_CHAOS_RUNE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_SUPER_RUNE)
	{
		if (!(upItem->getFlag() & FLAG_ITEM_SUPER_RUNE_USED) && (upItem->getPlus() < MAX_UPGRADE_PLUS))
		{
			ret = ITEM_UPGRADE_RESULT_PLUS;
			upItem->setFlag(upItem->getFlag() | FLAG_ITEM_SUPER_RUNE_USED);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_SUPER_RUNE_USED);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_LUCKY_RUNE)
	{
		if (upItem->getPlus() < 6)
		{
			ret = ITEM_UPGRADE_RESULT_PLUS;
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_USE_LUCKY_RUNE);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PURITY_RUNE
			 || reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_DEVIL_RUNE)
	{
		if (upItem->getPlus() < 5)
		{
			ret = ITEM_UPGRADE_RESULT_PLUS;
		}
		else
		{
			int nProb = GetRandom(1, 100);

			int nLevelDiff = reItem->m_itemProto->GetItemProtoLevel() - upItem->GetItemLevel();

			// 성공 공식
			// //90 - (itemplus * 6) + (룬 레벨 - 아이템 레벨)
			if (nLevelDiff > 9)
				nLevelDiff = 9;
			else if (nLevelDiff < -9)
				nLevelDiff = -9;

			int nUpProb[4] = { 0,0,100,0 };

			if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_PURITY_RUNE)
			{
				if(upItem->getPlus() >= 18)
				{
					nUpProb[0] = 70 - ((upItem->getPlus() + 102 ) / 2) + nLevelDiff;	// 성공
					if(g_bUpgradeEvent)
						nUpProb[0] = (nUpProb[0] * g_nUpgradeProb) / 100;
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = upItem->getPlus();							// 하락
					nUpProb[2] = 30 - upItem->getPlus();						// 유지
					nUpProb[3] = 70 - nUpProb[0];							// 파괴
				}
				else
				{
					nUpProb[0] = 75 - (upItem->getPlus() * 4 ) + nLevelDiff;	// 성공
					if(g_bUpgradeEvent)
						nUpProb[0] = (nUpProb[0] * g_nUpgradeProb) / 100;
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = upItem->getPlus();							// 하락
					nUpProb[2] = 25 - upItem->getPlus();						// 유지
					nUpProb[3] = 75 - nUpProb[0];							// 파괴
				}
			}
			else if (reItem->m_itemProto->getItemNum0() == IETC_UPGRADE_DEVIL_RUNE)
			{
				if(upItem->getPlus() >= 18)
				{
					nUpProb[0] = 100 - (upItem->getPlus() * 3.5 ) + nLevelDiff;	// 성공
					if(g_bUpgradeEvent)
						nUpProb[0] = (nUpProb[0] * g_nUpgradeProb) / 100;
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = 0;											// 하락
					nUpProb[2] = 0;											// 유지
					nUpProb[3] = 100 - nUpProb[0];							// 파괴
				}
				else
				{
					nUpProb[0] = 95 - (upItem->getPlus() * 5 ) + nLevelDiff;	// 성공
					if(g_bUpgradeEvent)
						nUpProb[0] = (nUpProb[0] * g_nUpgradeProb) / 100;
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = 0;											// 하락
					nUpProb[2] = 0;											// 유지
					nUpProb[3] = 100 - nUpProb[0];							// 파괴
				}
			}

			int idx = 0;
			while( nProb > 0 )
			{
				nProb -= nUpProb[idx];
				if(  nProb > 0 )
					idx++;
			}

			switch( idx )
			{
			case 0:
				ret = ITEM_UPGRADE_RESULT_PLUS;
				break;		// 성공
			case 1:
				ret = ITEM_UPGRADE_RESULT_MINUS;
				break;		// 하락
			case 2:
				ret = ITEM_UPGRADE_RESULT_NOCHANGE;
				break;		// 유지
			case 3:
				ret = ITEM_UPGRADE_RESULT_BROKEN;
				break;		// 파괴
			default:
				ret = ITEM_UPGRADE_RESULT_NOCHANGE;
			}
		}
	}

	return ret;
}

void itemPickGameLog(CPC *pc, CItem* pItem, unsigned char char_type)
{
	GAMELOG << init("ITEM_PICK", pc)
			<< itemlog(pItem);

	if (char_type == MSG_CHAR_PET)
		GAMELOG << delim << "BY PET";
#if defined (ITEM_PICK_ZONE_IP_LOG)
	GAMELOG << delim << "ZONE" << delim << pc->m_pZone->m_index << delim << "IP" << delim << pc->m_desc->getHostString();
#endif // LC_TLD
	GAMELOG	<< end;
}

void do_ItemDivide(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doItemDivide* packet = reinterpret_cast<RequestClient::doItemDivide*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->ttab, packet->tinvenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->ttab, packet->tinvenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isUsable(packet->ttab) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPIRE_CASH_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* sItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (sItem == NULL)
	{
		LOG_ERROR("HACKING? : not fount item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if ((sItem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT) == 0)
	{
		LOG_ERROR("HACKING? : not countable item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not countable item");
		return;
	}

	if (packet->divide_count <= 0)
	{
		LOG_ERROR("HACKING? : invalid count. char_index[%d] count[%d]",
				  ch->m_index, packet->divide_count);
		ch->m_desc->Close("invalid count");
		return;
	}

	if (sItem->Count() < packet->divide_count)
	{
		LOG_ERROR("HACKING? : not enought count. count[%d]",
				  ch->m_index, packet->divide_count);
		ch->m_desc->Close("not enought count");
		return;
	}

	if (ch->m_inventory.getItem(packet->ttab, packet->tinvenIndex) != NULL)
	{
		LOG_ERROR("HACKING? : target is not empty. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->ttab, packet->tinvenIndex);
		ch->m_desc->Close("target is not empty");
		return;
	}

	ch->m_inventory.divideItem(sItem, packet->ttab, packet->tinvenIndex, packet->divide_count);
}

//XX 아이템 교환
void do_ItemExchange(CPC* ch, CNetMsg::SP& msg)
{
	// 현재는 송크란 이벤트일때만 처리함
	if (gserver->isActiveEvent(A_EVENT_SONGKRAN) == false)
	{
		return;
	}

	RequestClient::doItemExchange* packet = reinterpret_cast<RequestClient::doItemExchange*>(msg->m_buf);

	std::set<int> check_dup;
	std::vector<itemExchangeProto::tag_sourceInfo> source_vec;
	item_search_t search_vec[ITEM_EXCHANGE_SOURCE_MAX];
	for (int i = 0; i < packet->listCount; ++i)
	{
		if (packet->list[i].itemIndex <= 0)
		{
			LOG_ERROR("HACKING : invalid item index[%d]. charIndex[%d]", packet->list[i].itemIndex, ch->m_index);
			ch->m_desc->Close("invalid item index");
			return;
		}

		if (packet->list[i].itemCount <= 0)
		{
			LOG_ERROR("HACKING : invalid item count[%d]. charIndex[%d]", packet->list[i].itemCount, ch->m_index);
			ch->m_desc->Close("invalid item count");
			return;
		}

		int sc = ch->m_inventory.searchItemByDBIndex(packet->list[i].itemIndex, search_vec[i]);
		if (sc == 0 || sc < packet->list[i].itemCount)
		{
			LOG_ERROR("HACKING : not enough item count. packet[%d] / inven[%d]. charIndex[%d]",
					  packet->list[i].itemCount, sc, ch->m_index);
			ch->m_desc->Close("not enough item count");
			return;
		}

		if (check_dup.insert(packet->list[i].itemIndex).second == false)
		{
			LOG_ERROR("HACKING : item index is duplicated. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("item index is duplicated");
			return;
		}

		itemExchangeProto::tag_sourceInfo sinfo;
		sinfo.itemIndex = packet->list[i].itemIndex;
		sinfo.itemCount = packet->list[i].itemCount;
		source_vec.push_back(sinfo);
	}

	itemExchangeProto::tag_resultInfo result = itemExchangeProto::instance()->exchange(packet->exchangeIndex, source_vec);
	if (result.error_code != ERR_ITEM_EXCHANGE_OK)
	{
		LOG_ERROR("HACKING : searching result is error[%d]. charIndex", result.error_code, ch->m_index);
		ch->m_desc->Close("searching result is error");
		return;
	}

	// OK
	// 결과 아이템 생성
	CItem* result_item = gserver->m_itemProtoList.CreateItem(result.itemIndex, WEARING_NONE, 0, 0, result.itemCount);
	if (result_item == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemExchange(rmsg, ERR_ITEM_EXCHANGE_CANT_CREATE_RESULT_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_inventory.addItem(result_item))
	{
		// 재료 아이템 삭제
		for (int i = 0; i < packet->listCount; ++i)
		{
			ch->m_inventory.deleteItem(search_vec[i], packet->list[i].itemCount);
		}

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemExchange(rmsg, ERR_ITEM_EXCHANGE_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		delete result_item;

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemExchange(rmsg, ERR_ITEM_EXCHANGE_NOT_ENOUGH_SPACE);
		SEND_Q(rmsg, ch->m_desc);
	}
}


bool do_UseGpsItem(CItem* item, CPC* ch)
{
	bool bApply = false;
	CSkill *skill = gserver->m_skillProtoList.Create(item->m_itemProto->getItemNum0(), item->m_itemProto->getItemNum1());
	ApplySkill(ch, ch, skill, item->m_itemProto->getItemIndex(), bApply);

	if(bApply == false)
	{
		delete skill;
		return false;
	}

	if(item->getDBIndex() == ARTIFACT_FIND_GPS_ITEM_INDEX)
	{
		ch->m_arti_gpsManager.sendGpsOwnerCountInfo();
	}

	return true;
}


bool do_UsePvPProtect( CItem* item, CPC* ch )
{
	if( ch->IsSetPlayerState(PLAYER_STATE_PKMODE) == true )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_USE_ITEM_STATE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if(gserver->m_bNonPK == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_USE_ITEM_STATE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if(ch->m_pArea->m_zone->IsPkZone() == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE, 0, 0, item->m_itemProto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if(ch->m_level <= 15)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_LOWLEVEL, 0, 0, item->m_itemProto->getItemIndex());
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if(ch->m_assist.FindBySkillIndex(PVP_PROTECT_SKILL_INDEX) != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_AREADY_USE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	bool apply;
	CSkill* skill = gserver->m_skillProtoList.Create(item->m_itemProto->getItemNum0(), item->m_itemProto->getItemNum1());
	ApplySkill(ch, ch, skill, item->getDBIndex(), apply);

	//정당 방위 모드 초기화
	{
		CRaChar* raList;
		CRaChar* raListNext = ch->m_raList;

		while ((raList = raListNext))
		{
			raListNext = raList->m_next;
			
			if( NULL == TO_PC(raList->m_raTarget) )
				continue;
			CPC* praTarget = PCManager::instance()->getPlayerByCharIndex(raList->m_raTarget->m_index);
			if (praTarget)
			{
				DelRaList(raList->m_raTarget);
			}
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_PVP_PROTECT_SUCCESS, 0, 0, item->m_itemProto->getItemIndex());
	SEND_Q(rmsg, ch->m_desc);
	return true;
}

void do_ItemCompose(CPC* ch, CNetMsg::SP& msg)
{
	//합성 시스템 추가 개발
	RequestClient::doItemCompose* packet = reinterpret_cast<RequestClient::doItemCompose*>(msg->m_buf);

	int count = 0;
	CItem* item = NULL;

	std::set<CItem*> _set;

	for(count = 0; count < MAX_COMPOSE_SLOT_COUNT; count++)
	{
		if(packet->matInfo[count].virtual_index == -1)
			break;

		if(ch->m_inventory.isValidNormalInventory(packet->matInfo[count].tab, packet->matInfo[count].inven_index) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				ch->m_index, packet->matInfo[count].tab, packet->matInfo[count].inven_index);
			ch->m_desc->Close("invalid packet");
			return;
		}

		item = ch->m_inventory.getItem(packet->matInfo[count].tab, packet->matInfo[count].inven_index);

		if(item == NULL)
		{
			LOG_ERROR("HACKING? : not found item. charIndex[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->matInfo[count].tab, packet->matInfo[count].inven_index);
			ch->m_desc->Close("not found item");
			return;
		}

		if(item->getVIndex() != packet->matInfo[count].virtual_index)
		{
			LOG_ERROR("HACKING? : virtual_index error. charIndex[%d] packet_virtual_index[%d], item_virtual_index[%d]", ch->m_index, packet->matInfo[count].virtual_index, item->getVIndex());
			ch->m_desc->Close("not found item");
			return;
		}

		if(item->IsComposeItem() == false)
		{
			LOG_ERROR("HACKING? : item data error. char_index[%d], item_db_index[%d]", ch->m_index, item->getDBIndex());
			ch->m_desc->Close("not compose item");
			return;
		}

		if(_set.find(item) != _set.end())
		{
			LOG_ERROR("HACKING? : duplicate item. char_index[%d]", ch->m_index);
			ch->m_desc->Close("duplicate item");
			return;
		}
		
		_set.insert(item);

	}

	//for문 첫번째 루틴의 break에 의한 NULL처리 해킹 시도로 판단
	if(item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. charIndex[%d] tab[%d] invenIndex[%d]", ch->m_index, packet->matInfo[count].tab, packet->matInfo[count].inven_index);
		ch->m_desc->Close("not found item");
		return;
	}

	stComposeData* data = ComposeItem::instance()->find(item->getDBIndex());

	if(data == NULL)
	{
		//에러 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCompose(rmsg, -1, MSG_ITEM_COMPOSE_ERROR_NOT_RESULT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(data->Material_count > count)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCompose(rmsg, -1, MSG_ITEM_COMPOSE_ERROR_NOT_ENOUGH_MAT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(data->need_nas > ch->m_inventory.getMoney())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCompose(rmsg, -1, MSG_ITEM_COMPOSE_ERROR_NOT_ENOUGH_NAS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	//확률 계산
	int prob = 0;
	if(data->prob >= GetRandom(1, 10000))
	{
		//성공

		//새로운 아이템 넣어준다.
		CItem* result_item = gserver->m_itemProtoList.CreateItem(data->result_item_index, -1, 0, 0, 1);

		if(result_item == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeItemCompose(rmsg, data->result_item_index, MSG_ITEM_COMPOSE_ERROR_NOT_RESULT);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		//합성 하는 아이템이 유물 아이템일 경우 처리하는부분
		if(result_item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && result_item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
		{
			CItem* item1 = ch->m_inventory.getItem(packet->matInfo[0].tab, packet->matInfo[0].inven_index);
			CItem* item2 = ch->m_inventory.getItem(packet->matInfo[1].tab, packet->matInfo[1].inven_index);
			ArtifactManager::instance()->compose(ch, item1, item2, result_item);
		}

		ch->m_inventory.addItem(result_item);

		//성공 메시지 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCompose(rmsg, result_item->getDBIndex(), MSG_ITEM_COMPOSE_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);

		//아이템 합성 성공 로그 추가
		LOG_INFO("ARTIFACT. COMPOSE SUCCESS. ARTIFACT INDEX[%d], CHARACTER_INDEX[%d]", result_item->getDBIndex(), ch->m_index);
	}
	else
	{
		//실	패
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCompose(rmsg, -1, MSG_ITEM_COMPOSE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}

	//재료 차감
	for(int i = 0; i < count; i++)
	{
		if(packet->matInfo[i].virtual_index == -1)
			break;

		CItem* _item = ch->m_inventory.getItem(packet->matInfo[i].tab, packet->matInfo[i].inven_index);
		ch->m_inventory.decreaseItemCount(_item ,1);
	}

	ch->m_inventory.decreaseMoney(data->need_nas);
}