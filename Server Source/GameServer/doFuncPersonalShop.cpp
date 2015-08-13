#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBManager.h"
#include "Artifact_Manager.h"

void do_PersonalShop(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();
	unsigned char subtype = (unsigned char)-1;
	RefMsg(msg) >> subtype;
	switch (subtype)
	{
	case MSG_PERSONALSHOP_SELL_START:
		do_PersonalShopSellStart(ch, msg);
		break;

	case MSG_PERSONALSHOP_SELL_LIST:
		do_PersonalShopSellList(ch, msg);
		break;

	case MSG_PERSONALSHOP_CHANGE:
		do_PersonalShopChange(ch, msg);
		break;

	case MSG_PERSONALSHOP_BUY:
		do_PersonalShopBuy(ch, msg);
		break;
	}
}

void do_PersonalShopSellStart(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pArea == NULL)
		return ;

	if ( !(ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) & MATT_PEACE ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->GetMapAttr() & MATT_FREEPKZONE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pArea->m_zone->isRVRZone() == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 브라질 요청 업데이트, OX 퀴즈룸에선 개인상점 금지...yhj 090227
	if ( ch->m_pZone->IsOXQuizRoom()
			|| ch->m_pZone->IsWarGroundZone()
	   )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#ifdef LC_USA
	if ( ch->m_level < 10 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_USA

#if defined (LC_GAMIGO)
	if ( ch->m_level < 15 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	if (ch->m_exchange)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_personalshop)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	CLCString shopname(PS_MAX_SHOPNAME + 1);
	RefMsg(msg) >> charindex
				>> shoptype
				>> shopname;
	shopname.Trim();

	if(findPercentChar(shopname.getBuffer()) != NULL)
	{
		//방이름에 % 문자열 입력 불가
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (charindex < 1 || shoptype < 0 || (shoptype & PST_SELL) == 0 || shopname.Length() > PS_MAX_SHOPNAME)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	char normalCount = -1;
	RefMsg(msg) >> normalCount;
	if (normalCount < 0 || normalCount > PS_MAX_NORMAL_ITEM)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (normalCount == 0 && (shoptype & PST_PACKAGE) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPersonalShop::SP ps(new CPersonalShop(shoptype, shopname));

	int i;
	for (i = 0; i < normalCount; i++)
	{
		unsigned short tab =0;
		unsigned short invenIndex = 0;
		int itemindex = -1;
		int itemdbindex = -1;
		LONGLONG count = -1;
		LONGLONG price = -1;
		RefMsg(msg) >> tab
					>> invenIndex
					>> itemindex
					>> itemdbindex
					>> count
					>> price;
		if (itemdbindex == NAS_ITEM_DB_INDEX)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		if (ch->m_inventory.isValidNormalInventory(tab, invenIndex) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, tab, invenIndex);
			ch->m_desc->Close("invalid packet");
			return;
		}

		if (ch->m_inventory.isUsable(tab) == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 060406 : bs : 봉인된 아이템 교환 불가
		CItem* item = ch->m_inventory.getItem(tab, invenIndex);
		if (item == NULL)
		{
			LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, tab, invenIndex);
			ch->m_desc->Close("not found item");
			return;
		}

#ifdef ENABLE_SUBJOB
		if( item && item->CanUseTrader( TRADE_PERSONAL_SHOP, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
		{
			// 판매할수 없는 아이템
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
#endif //ENABLE_SUBJOB

		if(item->m_itemProto->getItemIndex() == ARTIFACT_LEVEL1_ITEM_INDEX ||
			item->m_itemProto->getItemIndex() == ARTIFACT_LEVEL2_ITEM_INDEX ||
			item->m_itemProto->getItemIndex() == ARTIFACT_LEVEL3_ITEM_INDEX)
		{
			// 판매할수 없는 아이템
			LOG_ERROR("HACKING? : Artifact Item. char_index[%d], item_index[%d]",
				ch->m_index, item->getDBIndex());
			ch->m_desc->Close("not found item");
			return;
		}

		if(item->IsBelong() == true)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if ( item == NULL || item->getVIndex() != itemindex || item->getDBIndex() != itemdbindex	|| item->Count() < count || price < 1 || !item->CanExchange() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
		if (item->IsPet())
		{
			CPet* pet = ch->GetPet(item->getPlus());
			if (!pet || pet->GetSkillList()->count() != 0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		if( item->IsAPet() )
		{
			CAPet* apet = ch->GetAPet(item->getPlus());
			if( apet && apet->IsPetEquip() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		if (ps->FindItem(true, itemindex) != -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (!ps->AddItem(true, itemindex, count, price))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (shoptype & PST_PACKAGE)
	{
		LONGLONG packprice = -1;
		char packcount = -1;
		RefMsg(msg) >> packprice
					>> packcount;
		if (packprice < 1 || packcount < 1 || packcount > PS_MAX_PACKAGE_ITEM)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		int i;
		for (i = 0; i < packcount; i++)
		{
			unsigned short tab = 0;
			unsigned short invenIndex = 0;
			int itemindex = -1;
			int itemdbindex = -1;
			LONGLONG count = -1;
			RefMsg(msg) >> tab
						>> invenIndex
						>> itemindex
						>> itemdbindex
						>> count;
			if (itemdbindex == NAS_ITEM_DB_INDEX)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_inventory.isValidNormalInventory(tab, invenIndex) == false)
			{
				LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, tab, invenIndex);
				ch->m_desc->Close("invalid packet");
				return;
			}

			if (ch->m_inventory.isUsable(tab) == false)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 060406 : bs : 봉인된 아이템 교환 불가
			CItem* item = ch->m_inventory.getItem(tab, invenIndex);
			if (item == NULL)
			{
				LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, tab, invenIndex);
				ch->m_desc->Close("not found item");
				return;
			}

			if(item->IsBelong() == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if (item->getVIndex() != itemindex || item->getDBIndex() != itemdbindex
					|| item->Count() < count || !item->CanExchange())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ps->FindItem(true, itemindex) != -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (ps->FindItem(false, itemindex) != -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
			if (item->IsPet())
			{
				CPet* pet = ch->GetPet(item->getPlus());
				if (!pet || pet->GetSkillList()->count() != 0 )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
			if( item->IsAPet() )
			{
				CAPet* apet = ch->GetAPet(item->getPlus());
				if( apet && apet->IsPetEquip() )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			if (!ps->AddItem(false, itemindex, count, packprice))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}

	ch->m_personalshop = ps;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopSellStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	GAMELOG << init("PERSONAL SHOP START", ch)
			<< "TYPE" << delim << ps->GetType() << delim << "ITEMS";
	int pos = -1;
	int count = (int)ps->GetNormalCount();
	for (i = 0; i < count; i++)
	{
		pos = ps->GetNextNormalItem(pos);
		int itemindex = ps->GetNormalItemIndex(pos);
		CItem* item = ch->m_inventory.FindByVirtualIndex(itemindex);
		GAMELOG << delim << itemlog(item)
				<< delim << ps->GetNormalItemCount(pos)
				<< delim << ps->GetNormalItemPrice(pos);
	}
	if (ps->GetType() & PST_PACKAGE)
	{
		GAMELOG << delim << "PACKAGE"
				<< delim << ps->GetPackagePrice();

		pos = -1;
		int packageCount = ps->GetPackageCount();
		for (i = 0; i < packageCount; i++)
		{
			pos = ps->GetNextPackageItem(pos);
			int itemindex = ps->GetPackageItemIndex(pos);
			CItem* item = ch->m_inventory.FindByVirtualIndex(itemindex);
			GAMELOG << delim << itemlog(item)
					<< delim << ps->GetPackageItemCount(pos);
		}
	}
	GAMELOG << delim << ps->GetName() << end;
}

void do_PersonalShopSellList(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_personalshop)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	RefMsg(msg) >> charindex;
	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
	if (tch == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 일반 아이템에서 펫이면 펫 정보 보내기
	if (tch->m_personalshop->GetNormalCount())
	{
		int i;
		int pos = -1;
		int personalshopCount = tch->m_personalshop->GetNormalCount();
		for (i = 0; i < personalshopCount; i++)
		{
			pos = tch->m_personalshop->GetNextNormalItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->m_personalshop->GetNormalItemIndex(pos);
				CItem* pitem = tch->m_inventory.FindByVirtualIndex(itemindex);
				if (pitem)
				{
					if (pitem->IsPet())
					{
						CPet* pet = tch->GetPet(pitem->getPlus());
						if (pet)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ExPetSellInfoMsg(rmsg, pet);
							SEND_Q(rmsg, ch->m_desc);
						}
					}
				}
			}
		}
	}
	// 패키지 아이템에서 펫이면 펫 정보 보내기
	if (tch->m_personalshop->GetType() & PST_PACKAGE)
	{
		int i;
		int pos = -1;
		int personalshopCount = tch->m_personalshop->GetPackageCount();
		for (i = 0; i < personalshopCount; i++)
		{
			pos = tch->m_personalshop->GetNextPackageItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->m_personalshop->GetPackageItemIndex(pos);
				CItem* pitem = ch->m_inventory.FindByVirtualIndex(itemindex);
				if (pitem)
				{
					if (pitem->IsPet())
					{
						CPet* pet = tch->GetPet(pitem->getPlus());
						if (pet)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ExPetSellInfoMsg(rmsg, pet);
							SEND_Q(rmsg, ch->m_desc);
						}
					}
				}
			}
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopSellListMsg(rmsg, tch);

		if (rmsg->m_size >= MAX_PACKET_SIZE)
		{
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_CANNOT_USE);
		}
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_PersonalShopChange(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_exchange)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	RefMsg(msg) >> charindex
				>> shoptype;

	if (shoptype != PST_NOSHOP)
		return ;
	if (ch->m_personalshop == NULL)
		return ;
	if (ch->m_index != charindex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ch->m_personalshop.reset();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopChangeMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_PersonalShopBuy(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pArea == NULL)
		return ;

	if (ch->m_personalshop)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 구매시 필요한 나스
	LONGLONG needmoney = 0;

	// 상점 찾기
	int charindex = -1;
	char packbuy = -1;
	RefMsg(msg) >> charindex
				>> packbuy;
	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
	if (tch == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPersonalShop* ps = tch->m_personalshop.get();
	char shoptype = ps->GetType();

	int nCountOfPet = 0;
	if (ch->m_petList)
	{
		CPet* pPetOfBuyer = ch->m_petList;
		while (pPetOfBuyer)
		{
			nCountOfPet++;
			pPetOfBuyer = pPetOfBuyer->m_nextPet;
		}
	}

	// 구매 리스트
	CPersonalShop list(shoptype, "");

	// 패키지 검사
	if (packbuy)
	{
		if ((shoptype & PST_PACKAGE) == 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTPACKAGE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needmoney = ps->GetPackagePrice();
		if (needmoney < 0 || needmoney > ch->m_inventory.getMoney())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 패키지 구매 리스트에 추가
		int i, j = -1;
		int packageCount = ps->GetPackageCount();
		for (i = 0; i < packageCount; i++)
		{
			j = ps->GetNextPackageItem(j);
			if (j == -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			CItem* item = tch->m_inventory.FindByVirtualIndex(ps->GetPackageItemIndex(j));
			if(item == NULL)
				return ;
			if (item->IsPet())
				nCountOfPet++;

			if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
			{
				int error_ = ArtifactManager::instance()->checkValidUser(ch);
				if(error_ != 0)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, (MSG_SYS_TYPE)error_);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			list.AddItem(false, ps->GetPackageItemIndex(j), ps->GetPackageItemCount(j), ps->GetPackagePrice());
		}
	}

	// 일반 상품 검사
	char normalcount = -1;
	RefMsg(msg) >> normalcount;
	if (normalcount == 0 && !packbuy)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (normalcount < 0 || normalcount > ps->GetNormalCount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int i;
	for (i = 0; i < normalcount; i++)
	{
		int itemindex = -1;
		LONGLONG count = -1;
		RefMsg(msg) >> itemindex
					>> count;

		int pos = ps->FindItem(true, itemindex);
		if (pos == -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (count < 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (ps->GetNormalItemCount(pos) < count)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		CItem* item = tch->m_inventory.FindByVirtualIndex(itemindex);
		if(item == NULL)
			return ;

		if (item->IsPet())
			nCountOfPet++;

		if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
		{
			int error_ = ArtifactManager::instance()->checkValidUser(ch);
			if(error_ != 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, (MSG_SYS_TYPE)error_);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}

		needmoney += ps->GetNormalItemPrice(pos) * count;
		if (needmoney < 0 || needmoney > ch->m_inventory.getMoney())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 같은 아이템 여러번 사려고 하면 안됨
		if (list.FindItem(true, itemindex) != -1 || list.FindItem(false, itemindex) != -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("HACK PERSONAL SHOP BUY", ch) << end;
			return ;
		}
		list.AddItem(true, ps->GetNormalItemIndex(pos), count, ps->GetNormalItemPrice(pos));
	}

	if (nCountOfPet > MAX_OWN_PET)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_ANYMORE_PET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int needspace = 0;
	int pos;
	pos = list.GetNextNormalItem(-1);
	while (pos != -1)
	{
		// 파는 아이템 검사
		int itemindex = list.GetNormalItemIndex(pos);
		CItem* pitem = tch->m_inventory.FindByVirtualIndex(itemindex);
		if (!pitem)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetNormalItemCount(pos))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 인벤토리 가능 검사
		if (!(pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT) || !ch->m_inventory.FindByDBIndex(pitem->getDBIndex(), pitem->getPlus(), pitem->getFlag()))
			needspace++;

		pos = list.GetNextNormalItem(pos);
	}
	pos = list.GetNextPackageItem(-1);
	while (pos != -1)
	{
		// 파는 아이템 검사
		int itemindex = list.GetPackageItemIndex(pos);
		CItem* pitem = tch->m_inventory.FindByVirtualIndex(itemindex);
		if (!pitem)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetPackageItemCount(pos))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 인벤토리 가능 검사
		if (!(pitem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT) || !ch->m_inventory.FindByDBIndex(pitem->getDBIndex(), pitem->getPlus(), pitem->getFlag()))
			needspace++;

		pos = list.GetNextPackageItem(pos);
	}

	// 인벤토리 검사
	if (ch->m_inventory.getEmptyCount() < needspace)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("PERSONAL SHOP BUY", ch);
	GAMELOG << "SHOP" << delim << tch->m_name << delim
			<< tch->m_nick << delim
			<< tch->m_desc->m_idname << delim;
	if (packbuy)
		GAMELOG << "PACKAGE" << delim;
	else
		GAMELOG << "NO PACKAGE" << delim;
	GAMELOG << "NEEDMONEY" << delim << needmoney << delim;
	GAMELOG << "ITEMS";

	// 일반 아이템 구매/판매
	pos = list.GetNextNormalItem(-1);
	while (pos != -1)
	{
		int itemindex = list.GetNormalItemIndex(pos);
		LONGLONG count = list.GetNormalItemCount(pos);
		CItem* pitem = tch->m_inventory.FindByVirtualIndex(itemindex);
		pitem->unWearPos();

		if (pitem->IsPet())
		{
			CPet* pet = tch->GetPet(pitem->getPlus());
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);

				std::string str = boost::str(boost::format(
												 "UPDATE t_pet SET a_owner=%1% WHERE a_index=%2% LIMIT 1")
											 % ch->m_index % pitem->getPlus());
				DBManager::instance()->pushQuery(ch->m_desc->m_index, str);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetSkillListMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		if( pitem->IsAPet() )
		{
			CAPet* apet = tch->GetAPet(pitem->getPlus());
			if( apet )
			{
				REMOVE_FROM_BILIST(apet, tch->m_pApetlist, m_pPrevPet, m_pNextPet);
				apet->SetOwner(ch);
				apet->AddFaith(-10);
				ADD_TO_BILIST(apet, ch->m_pApetlist, m_pPrevPet, m_pNextPet);

				std::string str = boost::str(boost::format(
												 "UPDATE t_apets SET a_owner=%1% WHERE a_index=%2% LIMIT 1")
											 % ch->m_index % pitem->getPlus());
				DBManager::instance()->pushQuery(ch->m_desc->m_index, str);
			}
		}

		CItem* nitem = NULL;

		// 구매 리스트의 카운트보다 크면
		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			nitem = gserver->m_itemProtoList.CreateItem(pitem->getDBIndex(), -1, pitem->getPlus(), pitem->getFlag(), count);
			ch->m_inventory.addItem(nitem);

			// 빼고
			tch->m_inventory.decreaseItemCount(pitem, count);
		}
		else
		{
			int tab = pitem->tab();
			int invenIndex = pitem->getInvenIndex();

			ch->m_inventory.addItem(pitem);
			nitem = pitem;

			// 빼고
			tch->m_inventory.eraseNotFreeByInvenIndex(tab, invenIndex);
		}

		GAMELOG << delim << itemlog(nitem)
				<< delim << count;

		// 판매 리스트 갱신
		ps->RemoveNormalItem(itemindex, count);

		pos = list.GetNextNormalItem(pos);

		if(pitem->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pitem->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
		{
			ArtifactManager::instance()->changeOnwer(ch, pitem);
		}
	}

	// 패키지 아이템 구매 판매
	pos = list.GetNextPackageItem(-1);
	while (pos != -1)
	{
		int itemindex = list.GetPackageItemIndex(pos);
		LONGLONG count = list.GetPackageItemCount(pos);
		CItem* pitem = tch->m_inventory.FindByVirtualIndex(itemindex);

		if (pitem->IsPet())
		{
			CPet* pet = tch->GetPet(pitem->getPlus());
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);

				std::string str = boost::str(boost::format(
												 "UPDATE t_pet SET a_owner=%1% WHERE a_index=%2% LIMIT 1")
											 % ch->m_index % pitem->getPlus());
				DBManager::instance()->pushQuery(ch->m_desc->m_index, str);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetSkillListMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		if( pitem->IsAPet() )
		{
			CAPet* apet = tch->GetAPet(pitem->getPlus());
			if( apet )
			{
				REMOVE_FROM_BILIST(apet, tch->m_pApetlist, m_pPrevPet, m_pNextPet);
				apet->SetOwner(ch);
				apet->AddFaith(-10);
				ADD_TO_BILIST(apet, ch->m_pApetlist, m_pPrevPet, m_pNextPet);

				std::string str = boost::str(boost::format(
												 "UPDATE t_apets SET a_owner=%1% WHERE a_index=%2% LIMIT 1")
											 % ch->m_index % pitem->getPlus());
				DBManager::instance()->pushQuery(ch->m_desc->m_index, str);
			}
		}

		// 구매 리스트의 카운트보다 크면
		CItem* nitem = NULL;

		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			// 만들고
			nitem = gserver->m_itemProtoList.CreateItem(pitem->getDBIndex(), -1, pitem->getPlus(), pitem->getFlag(), count);
			ch->m_inventory.addItem(nitem);

			// 빼고
			tch->m_inventory.decreaseItemCount(pitem, count);
		}
		else
		{
			int tab = pitem->tab();
			int invenIndex = pitem->getInvenIndex();

			ch->m_inventory.addItem(pitem);
			nitem = pitem;

			// 빼고
			tch->m_inventory.eraseNotFreeByInvenIndex(tab, invenIndex);
		}

		GAMELOG << delim << itemlog(nitem)
				<< delim << count;

		// 판매 리스트 갱신
		ps->RemovePackage();

		pos = list.GetNextPackageItem(pos);

		if(pitem->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && pitem->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
		{
			ArtifactManager::instance()->changeOnwer(ch, pitem);
		}
	}

	GAMELOG << end;

	// 돈 넣고 빼기
	if (ch->m_inventory.getMoney() >= needmoney)
	{
		ch->m_inventory.decreaseMoney(needmoney);
	}

	int rate = (shoptype & (PST_PACKAGE | PST_PREMIUM)) ? PS_TEX_PREMIUM : PS_TEX_NORMAL;

	tch->m_inventory.increaseMoney(needmoney - (needmoney * rate / 10000));

	// 실시간 저장
	DBManager::instance()->SaveCharacterInfo(ch->m_desc, false);
	DBManager::instance()->SaveCharacterInfo(tch->m_desc, false);

	// 판매자에게 판매됨 알림
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopBuyMsg(rmsg, msg, ch);
		SEND_Q(rmsg, tch->m_desc);
	}

	// 상점 종료 및 상태 변화 검사
	// 패키지 상태 검사
	if (ps->GetType() != shoptype)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PersonalShopChangeMsg(rmsg, tch);
		tch->m_pArea->SendToCell(rmsg, tch, true);
		if (ps->GetType() == PST_NOSHOP)
		{
			tch->m_personalshop.reset();
		}
	}
}
