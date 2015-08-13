#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Log.h"
#include "InventoryManager.h"
#include "../ShareLib/packetType/ptype_old_do_stash.h"
#include "DBManager.h"

//////////////////////////////////////////////////////////////////////////
void do_StashList(CPC* ch, CNetMsg::SP& msg);
void do_StashKeep(CPC* ch, CNetMsg::SP& msg);
void do_StashTake(CPC* ch, CNetMsg::SP& msg);

#ifdef STASH_PASSWORD
void do_StashChangePassword(CPC* ch, CNetMsg::SP& msg);
void do_StashDeletePassword(CPC* ch, CNetMsg::SP& msg);
#endif

bool checkStashNpc(CPC* ch, int npcIndex);
//////////////////////////////////////////////////////////////////////////

void do_Stash(CPC* ch, CNetMsg::SP& msg)
{
	// 죽은 상태, 전투중, 스킬 시전중, 워프중, 개인상점, 교환중에서는 불가능
	if (
		DEAD(ch)
		|| ch->IsCombatMode()
		|| ch->m_currentSkill
		|| ch->IsSetPlayerState(PLAYER_STATE_WARP)
		|| ch->m_personalshop
		|| ch->m_exchange
	)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_STASH_LIST:
		do_StashList(ch, msg);
		break;

	case MSG_STASH_KEEP:
		do_StashKeep(ch, msg);
		break;

	case MSG_STASH_TAKE:
		do_StashTake(ch, msg);
		break;

#ifdef STASH_PASSWORD
	case MSG_STASH_CHANGE_PASSWORD:
		do_StashChangePassword(ch, msg);
		break;

	case MSG_STASH_DELETE_PASSWORD:
		do_StashDeletePassword(ch, msg);
		break;
#endif

	default:
		{
			LOG_ERROR("HACKING? : invalid subtype[%d]. char_index[%d]", pBase->subType, ch->m_index);
			ch->m_desc->Close("invalid subtype");
			return;
		}
	}
}

#ifdef STASH_PASSWORD
bool checkStashPassword(CPC* ch, char *password)
{
	if (ch->m_stash_password.empty())
		return true;

	if (ch->m_stash_password == password)
		return true;

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PASSWORD);
	SEND_Q(rmsg, ch->m_desc);

	return false;
}
#endif

void do_StashList(CPC* ch, CNetMsg::SP& msg)
{
#ifdef STASH_PASSWORD
	RequestClient::doStashList* packet = reinterpret_cast<RequestClient::doStashList*>(msg->m_buf);
	packet->password[MAX_STASH_PASSWORD_LENGTH] = '\0';
	if (checkStashPassword(ch, packet->password) == false)
		return;
#endif

	ch->m_inventory.sendInfoToClientInStash();
}

void do_StashKeep(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doStashKeep* packet = reinterpret_cast<RequestClient::doStashKeep*>(msg->m_buf);

#ifdef STASH_PASSWORD
	packet->password[MAX_STASH_PASSWORD_LENGTH] = '\0';
	if (checkStashPassword(ch, packet->password) == false)
	{
		LOG_ERROR("invalid stash password in keep. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid stash password in keep");
		return;
	}
#endif

	if (checkStashNpc(ch, packet->npcKind) == false)
	{
		LOG_ERROR("HACKING? : not found npc. char_index[%d]", ch->m_index);
		ch->m_desc->Close("not found npc");
		return;
	}

	if( packet->npcKind != -1)
	{
		CNPC* npc = (CNPC *)ch->m_pArea->FindCharInCell(ch, packet->npcKind, MSG_CHAR_NPC);

		if(npc == NULL)
		{
			LOG_ERROR("HACKING? : not found npc. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcKind);
			ch->m_desc->Close("not found npc");
			return;
		}

		if(npc->m_proto->m_rvr_value > 0 && npc->m_proto->m_rvr_value != ch->getSyndicateType())
		{
			LOG_ERROR("HACKING? : not equal rvr type. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcKind);
			ch->m_desc->Close("not equal rvr type");
			return;
		}
	}

	if (packet->keepMoney == 0 && packet->keepCount == 0)
	{
		LOG_ERROR("HACKING? : KeepCount and KeepMoney is zero. char_index[%d]", ch->m_index);
		ch->m_desc->Close("KeepCount and KeepMoney is zero");
		return;
	}

	// 전체 보관할 아이템 종류 수 검사
	if (packet->keepCount < 0 || packet->keepCount > MAX_STASH_KEEP)
	{
		LOG_ERROR("HACKING? : invalid keepcount. char_index[%d] keepcount[%d]", ch->m_index, packet->keepCount);
		ch->m_desc->Close("invalid keepcount");
		return;
	}

	if (packet->keepMoney < 0)
	{
		std::string str = boost::str(boost::format(
										 "HACKING? : invalid keep money. char_index[%1%] keepMoney[%2%]") % ch->m_index % packet->keepMoney);
		LOG_ERROR(str.c_str());
		ch->m_desc->Close("invalid keep money");
		return;
	}

	CItem* item[MAX_STASH_KEEP];
	LONGLONG needmoney = 0;

	// 보관할 아이템 검사
	std::set<int> check_duplication;
	for (int i = 0; i < packet->keepCount; ++i)
	{
		if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
		{
			LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("invalid packet");
			return;
		}

		if (check_duplication.insert(packet->list[i].tab << 16 | packet->list[i].invenIndex).second == false)
		{
			LOG_ERROR("HACKING? : duplicate inven index. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("duplicate inven index");
			return;
		}

		CItem* pitem = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);
		if (pitem == NULL)
		{
			LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("not found item");
			return;
		}

		if (pitem->getVIndex() != packet->list[i].virtualIndex)
		{
			LOG_ERROR("HACKING? : invalid virtual index. char_index[%d] tab[%d] invenIndex[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
			ch->m_desc->Close("invalid virtual index");
			return;
		}

		if (packet->list[i].count <= 0)
		{
			LOG_ERROR("HACKING? : invalid item count. char_index[%d] tab[%d] invenIndex[%d] itemCount[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex, packet->list[i].count);
			ch->m_desc->Close("invalid item count");
			return;
		}

		if (pitem->Count() < packet->list[i].count)
		{
			LOG_ERROR("HACKING? : not enought item count. char_index[%d] tab[%d] invenIndex[%d] itemCount[%d]",
					  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex, packet->list[i].count);
			ch->m_desc->Close("not enought item count");
			return;
		}


#ifdef ENABLE_SUBJOB
		if( pitem->CanUseTrader( TRADE_STASH, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
		{
			return;
		}
#endif //ENABLE_SUBJOB

		if(pitem->IsBelong() == true)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if (pitem->CanKeepStash() == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if( pitem->isToggle() == true )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PACKET);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		item[i] = pitem;
		needmoney += (packet->list[i].count * 100);
	}

	// 보관료 검사 및 보관 금액 검사
	needmoney += packet->keepMoney;
	if (ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 모든 아이템을 창고에 넣을 수 있는지 검사
	std::vector<possible_search_t> search_vec;
	for (int i = 0; i < packet->keepCount; ++i)
	{
		search_vec.push_back(possible_search_t(item[i], packet->list[i].count));
	}

	if (ch->m_inventory.isPossibleAddInStash(search_vec) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_STASH_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 넣고 빼고
	GAMELOG << init("STASH KEEP", ch)
			<< "NEED Money" << delim << needmoney << delim;

	for (int i = 0; i < packet->keepCount; ++i)
	{
		GAMELOG << "KEEP COUNT" << delim
				<< packet->list[i].count << delim
				<< "ITEM" << delim
				<< itemlog(item[i]) << delim;

		// 창고에 넣기
		if (item[i]->Count() == packet->list[i].count)	// 모두 이전
		{
			ch->m_inventory.eraseNotFree(item[i]);

			ch->m_inventory.addItemInStash(item[i]);
		}
		else // 부분 이전
		{
			CItem* titem = gserver->m_itemProtoList.CreateItem(item[i]->getDBIndex(), -1,
						   item[i]->getPlus(), item[i]->getFlag(), packet->list[i].count);

			ch->m_inventory.decreaseItemCount(item[i], packet->list[i].count);

			ch->m_inventory.addItemInStash(titem);
		}
	}

	GAMELOG << end;

	// 돈 줄이고
	ch->m_inventory.decreaseMoney(needmoney);

	if (packet->keepMoney > 0) // 보관할 돈이 있다면...
	{
		ch->m_inventory.increaseMoneyInStash(packet->keepMoney);

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateMoneyDescReason(rmsg, NAS_DESC_REASON_STASH_KEEP, packet->keepMoney);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		// 결과 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_StashTake(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doStashTake* packet = reinterpret_cast<RequestClient::doStashTake*>(msg->m_buf);

#ifdef STASH_PASSWORD
	packet->password[MAX_STASH_PASSWORD_LENGTH] = '\0';
	if (checkStashPassword(ch, packet->password) == false)
	{
		LOG_ERROR("invalid stash password in take. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid stash password in take");
		return;
	}
#endif

	if (checkStashNpc(ch, packet->npcKind) == false)
	{
		LOG_ERROR("HACKING? : not found npc. char_index[%d]", ch->m_index);
		ch->m_desc->Close("not found npc");
		return;
	}

	if( packet->npcKind != -1)
	{
		CNPC* npc = (CNPC *)ch->m_pArea->FindCharInCell(ch, packet->npcKind, MSG_CHAR_NPC);

		if(npc == NULL)
		{
			LOG_ERROR("HACKING? : not found npc. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcKind);
			ch->m_desc->Close("not found npc");
			return;
		}

		if(npc->m_proto->m_rvr_value > 0 && npc->m_proto->m_rvr_value != ch->getSyndicateType())
		{
			LOG_ERROR("HACKING? : not equal rvr type. charIndex[%d] npcIndex[%d]", ch->m_index, packet->npcKind);
			ch->m_desc->Close("not equal rvr type");
			return;
		}
	}

	if (packet->takeMoney == 0 && packet->takeCount == 0)
	{
		LOG_ERROR("HACKING? : TakeCount and TakeMoney is zero. char_index[%d]", ch->m_index);
		ch->m_desc->Close("TakeCount and TakeMoney is zero");
		return;
	}

	if (packet->takeCount < 0 || packet->takeCount > MAX_STASH_TAKE)
	{
		LOG_ERROR("HACKING? : invalid takecount. char_index[%d] keepcount[%d]", ch->m_index, packet->takeCount);
		ch->m_desc->Close("invalid takecount");
		return;
	}

	if (packet->takeMoney < 0)
	{
		std::string str = boost::str(boost::format(
										 "HACKING? : invalid take money. char_index[%1%] keepMoney[%2%]") % ch->m_index % packet->takeMoney);
		LOG_ERROR(str.c_str());
		ch->m_desc->Close("invalid take money");
		return;
	}

	if (packet->takeMoney > 0)
	{
		if (ch->m_inventory.getMoneyInStash() < packet->takeMoney)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 찾을 아이템 검사
	std::vector<possible_search_t> possible_vec;
	std::set<int> check_duplication;
	CItem* item[MAX_STASH_TAKE] = { NULL, };

	for (int i = 0; i < packet->takeCount; ++i)
	{
		CItem* pitem = ch->m_inventory.getItem(INVENTORY_TAB_STASH, packet->list[i].invenIndex);
		if (pitem == NULL)
		{
			LOG_ERROR("HACKING? : not found item. char_index[%d] invenIndex[%d]", ch->m_index, packet->list[i].invenIndex);
			ch->m_desc->Close("not found item");
			return;
		}

		if (pitem->getVIndex() != packet->list[i].virtualIndex)
		{
			LOG_ERROR("HACKING? : not equal virtual index. char_index[%d] virtualIndex[%d]", ch->m_index, packet->list[i].virtualIndex);
			ch->m_desc->Close("not equal virtual index");
			return;
		}

		if (packet->list[i].count <= 0)
		{
			LOG_ERROR("HACKING? : invalid count. char_index[%d] count[%d]", ch->m_index, packet->list[i].count);
			ch->m_desc->Close("invalid count");
			return;
		}

		if (pitem->Count() < packet->list[i].count)
		{
			LOG_ERROR("HACKING? : not enought count. char_index[%d] count[%d]", ch->m_index, packet->list[i].count);
			ch->m_desc->Close("not enought count");
			return;
		}

		if (check_duplication.insert(packet->list[i].invenIndex).second == false)
		{
			LOG_ERROR("HACKING? : duplicate invenIndex. char_index[%d] count[%d]", ch->m_index, packet->list[i].invenIndex);
			ch->m_desc->Close("duplicate invenIndex");
			return;
		}

		item[i] = pitem;
		possible_vec.push_back(possible_search_t(item[i], packet->list[i].count));
	}

	if (ch->m_inventory.isPossibleAdd(possible_vec) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_INVENTORY_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 아이템 넣고 빼기
	for (int i = 0; i < packet->takeCount; ++i)
	{
		if (item[i]->Count() == packet->list[i].count)	// 모두 빼는 경우
		{
			ch->m_inventory.eraseNotFreeByInvenIndex(INVENTORY_TAB_STASH, item[i]->getInvenIndex());
			ch->m_inventory.addItem(item[i]);

			GAMELOG << init("STASH TAKE", ch)
					<< "TAKE COUNT" << delim
					<< packet->list[i].count << delim
					<< "ITEM" << delim
					<< itemlog(item[i]) << delim
					<< "REST COUNT" << delim << "take all" << end;
		}
		else // 부분을 빼는 경우
		{
			CItem* titem = gserver->m_itemProtoList.CreateItem(item[i]->getDBIndex(), -1,
						   item[i]->getPlus(), item[i]->getFlag(), packet->list[i].count);

			ch->m_inventory.decreaseItemCount(INVENTORY_TAB_STASH, item[i]->getInvenIndex(), packet->list[i].count);

			ch->m_inventory.addItem(titem);

			GAMELOG << init("STASH TAKE", ch)
					<< "TAKE COUNT" << delim
					<< packet->list[i].count << delim
					<< "ITEM" << delim
					<< itemlog(item[i]) << delim
					<< "REST COUNT" << delim << item[i]->Count() << end;
		}
	}

	// 찾을 돈이 있다면
	if (packet->takeMoney > 0)
	{
		ch->m_inventory.decreaseMoneyInStash(packet->takeMoney);
		ch->m_inventory.increaseMoney(packet->takeMoney);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::StashErrorMsg(rmsg, MSG_STASH_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

bool checkStashNpc(CPC* ch, int npcIndex)
{
	return true;
// 	CNPC* npc = (CNPC *)ch->m_pArea->FindCharInCell(ch, npcIndex, MSG_CHAR_NPC);
// 	if (npc != NULL)
// 		return true;
//
// 	static int remoteStashItemIndex[] = { 2455, 2456, 2457, 2607 };
// 	for (int i = 0; i < sizeof(remoteStashItemIndex) / sizeof(remoteStashItemIndex[0]); ++i)
// 	{
// 		if (ch->m_inventory.FindByDBIndex(remoteStashItemIndex[i]))
// 			return true;
// 	}
//
// 	return false;
}

#ifdef STASH_PASSWORD
void do_StashChangePassword(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doStashChangePassword* packet = reinterpret_cast<RequestClient::doStashChangePassword*>(msg->m_buf);
	packet->old_password[MAX_STASH_PASSWORD_LENGTH] = '\0';
	packet->new_password[MAX_STASH_PASSWORD_LENGTH] = '\0';

	if (checkStashPassword(ch, packet->old_password) == false)
		return;

	int passwd_length = strlen(packet->new_password);
	if (passwd_length == 0 || passwd_length < MIN_STASH_PASSWORD_LENGTH)
	{
		LOG_ERROR("invalid length of stash password. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("invalid length of stash password");
		return;
	}

	ch->m_stash_password = packet->new_password;

	char tempstr[MAX_STASH_PASSWORD_LENGTH * 2];
	mysql_escape_string(tempstr, ch->m_stash_password.c_str(), ch->m_stash_password.length());

	std::string query = boost::str(boost::format(
									   "UPDATE t_users SET a_stash_password='%s' WHERE a_idname='%s' LIMIT 1") % tempstr % (const char *)ch->m_desc->m_idname);
	DBManager::instance()->pushQueryForAuthDB(ch->m_desc->m_index, query);

	{
		unsigned char flag = (ch->m_stash_password.empty()) ? 0 : 1;
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeStashPasswordFlag(rmsg, flag);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_StashDeletePassword(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doStashDeletePassword* packet = reinterpret_cast<RequestClient::doStashDeletePassword*>(msg->m_buf);
	packet->identity[A_CID_LENGTH] = '\0';

	if (ch->m_a_cid != packet->identity)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::DeleteStashPasswordErrorMsg(rmsg, 1);
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	ch->m_stash_password = "";

	std::string query = boost::str(boost::format(
									   "UPDATE t_users SET a_stash_password='' WHERE a_idname='%s' LIMIT 1") % (const char *)ch->m_desc->m_idname);
	DBManager::instance()->pushQueryForAuthDB(ch->m_desc->m_index, query);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::DeleteStashPasswordErrorMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		unsigned char flag = (ch->m_stash_password.empty()) ? 0 : 1;
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeStashPasswordFlag(rmsg, flag);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#endif
