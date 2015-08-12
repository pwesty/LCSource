#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_old_do_affinity.h"


//////////////////////////////////////////////////////////////////////////
void do_AffinityConnect(CPC* ch, CNetMsg::SP& msg);
void do_AffinityContributeReq(CPC* ch, CNetMsg::SP& msg);
void do_AffinityShopUseReq(CPC* ch, CNetMsg::SP& msg);
void do_AffinityGiftInfoReq(CPC* ch, CNetMsg::SP& msg);
void do_AffinityGiftReq(CPC* ch, CNetMsg::SP& msg);
void do_AffinityInfoTabReq(CPC* ch, CNetMsg::SP& msg);
//////////////////////////////////////////////////////////////////////////

void do_Affinity(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;
	switch (subtype)
	{
	case MSG_EX_AFFINITY_CONNECT_REQ:
		do_AffinityConnect(ch, msg);
		break;
	case MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REQ:
		do_AffinityContributeReq(ch, msg);
		break;
	case MSG_EX_AFFINITY_SHOPUSE_REQ:
		do_AffinityShopUseReq(ch, msg);
		break;
	case MSG_EX_AFFINITY_GIFTINFO_REQ:
		do_AffinityGiftInfoReq(ch, msg);
		break;
	case MSG_EX_AFFINITY_GIFT_REQ:
		do_AffinityGiftReq(ch, msg);
		break;
	case MSG_EX_AFFINITY_INFOTAB_REQ:
		do_AffinityInfoTabReq(ch, msg);
		break;
	default:
		return ;
	}
}

void do_AffinityConnect(CPC* ch, CNetMsg::SP& msg)
{
	CAffinityProto* proto;
	CNPC* npc;

	int npcidx;

	RefMsg(msg) >> npcidx;

	npc = (CNPC*)(ch->m_pArea->FindCharInCell(ch, npcidx, MSG_CHAR_NPC, false));
	if(!npc)
		return ;

	proto = gserver->m_affinityProtoList.FindProtoByNPCIdx(npc->m_proto->m_index);
	if(!proto)
		return ;

	CAffinityNPC* affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if(!affinitynpc)
		return;
	if(!affinitynpc->GetFlag(AF_CONNECT))
	{
		return;
	}

	if(ch->m_affinityList.FindAffinity(proto->m_index))
	{
		// 이미 있음
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 캐릭터 레벨 검사
	if( ch->m_level < proto->m_needPCLevel )
	{
		// 레벨 검사
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_PCLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

//m_needAffinIdx;
//m_needAffinPoint;

	if(proto->m_needAffinIdx > 0)
	{
		if(!gserver->m_affinityProtoList.FindProtoByAffinityIdx(proto->m_needAffinIdx))
			return ;

		CAffinity* affinity = ch->m_affinityList.FindAffinity(proto->m_needAffinIdx);
		if(!affinity)
			return ;

		if(affinity->m_point < proto->m_needAffinPoint)
			return ;
	}

	// 아이템 검사
	int row, col;
	CItem* item = NULL;
	if(proto->m_needItemIdx > 0)
	{
		if(proto->m_needItemIdx == NAS_ITEM_DB_INDEX)
		{
			if(ch->m_inventory.getMoney() < proto->m_needItemCount)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			ch->m_inventory.decreaseMoney(proto->m_needItemCount);
		}
		else
		{
			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(proto->m_needItemIdx, 0, 0, vec);
			if (sc == 0 || sc < proto->m_needItemCount )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			ch->m_inventory.deleteItem(vec, proto->m_needItemCount);
		}
	}

	if(!ch->m_affinityList.AddAffinity(proto->m_index, 0))
	{
		return ;
	}

	{
		// 친화도 추가 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityAddInfoMsg(rmsg, proto->m_index, proto->m_maxvalue);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("AFFINITY CONNECT", ch)
			<< "INDEX" << delim
			<< proto->m_index << end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_AffinityContributeReq(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doAffinityContribute* packet = reinterpret_cast<RequestClient::doAffinityContribute*>(msg->m_buf);

	if (packet->listCount <= 0 || packet->listCount > MAX_ITEM_CONTRIBUTE)
	{
		LOG_ERROR("HACKING? : invalid list count. char_index[%d] list_count[%d]", ch->m_index, packet->listCount);
		ch->m_desc->Close("invalid list count");
		return;
	}

	CNPC* npc = (CNPC*)(ch->m_pArea->FindCharInCell(ch, packet->npcIndex, MSG_CHAR_NPC, false));
	if (npc == NULL)
	{
		LOG_ERROR("not found npc. char_index[%d] npcIndex[%d]", ch->m_index, packet->npcIndex);
		ch->m_desc->Close("not found npc");
		return;
	}

	CAffinityProto* proto = gserver->m_affinityProtoList.FindProtoByNPCIdx(npc->m_proto->m_index);
	if (proto == NULL)
	{
		LOG_ERROR("not found affinity proto. char_index[%d] affinityProto[%d]", ch->m_index, npc->m_proto->m_index);
		ch->m_desc->Close("not found affinity proto");
		return;
	}

	CAffinityNPC* affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if (affinitynpc == NULL)
	{
		LOG_ERROR("not found affinity npc. char_index[%d] affinityProto[%d]", ch->m_index, npc->m_proto->m_index);
		ch->m_desc->Close("not found affinity npc");
		return;
	}

	if(!affinitynpc->GetFlag(AF_CONTRIBUTE))
	{
		// 관리자 설정 에러
		return;
	}

	CAffinity* affinity = ch->m_affinityList.FindAffinity(proto->m_index);

	// 친화도 보유중인지 확인
	if (affinity == NULL)
	{
		// 친화도가 없음.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_NOT_CONNECTED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(affinity->m_point >= proto->m_maxvalue)
	{
		// 더이상 point 를 올릴 수 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MAX);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	item_search_t vec[MAX_ITEM_CONTRIBUTE];
	std::set<int> check_duplication;
	int serverPoint = 0;

	for (int i = 0 ; i < packet->listCount; ++i)
	{
		if (packet->list[i].count <= 0)
		{
			LOG_ERROR("HACKING? : invalid count[%d]. char_index[%d]", packet->list[i].count, ch->m_index);
			ch->m_desc->Close("invalid count");
			return;
		}

		int sc = ch->m_inventory.searchItemByDBIndexWithoutWearItem(packet->list[i].itemDBIndex, vec[i]);
		if (sc == 0 || sc < packet->list[i].count)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		int point = proto->GetAffintiyPointOfItem(packet->list[i].itemDBIndex) ;
		if(point < 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MISSMATCH);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if (check_duplication.insert(packet->list[i].itemDBIndex).second == false)
		{
			LOG_ERROR("HACKING? : duplicate item db index[%d]. char_index[%d]", packet->list[i].itemDBIndex, ch->m_index);
			ch->m_desc->Close("duplicate item db index");
			return;
		}

		// 아이템이 기부목록에 포함되는지 확인
		serverPoint += (point * packet->list[i].count);
	}

	if (packet->clientPoint != serverPoint)
	{
		LOG_ERROR("HACKING? : not equal point. clientPoint[%d] serverPoint[%d]", packet->clientPoint, serverPoint);
		ch->m_desc->Close("not equal point");
		return;
	}

	int bonus = 0;
	if(ch->m_avPassiveAddition.affinity_item > 0)
	{
		bonus += ch->m_avPassiveAddition.affinity_item;
	}
	if(ch->m_avPassiveRate.affinity_item > 0)
	{
		bonus = serverPoint * (ch->m_avPassiveRate.affinity_item - 100) / SKILL_RATE_UNIT;
	}

	affinity->AddPoint(serverPoint, ch, bonus);

	// 아이템 제거
	for (int i = 0; i < packet->listCount; ++i)
	{
		ch->m_inventory.deleteItem(vec[i], packet->list[i].count);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REP);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("AFFINITY CONTRIBUTE", ch)
			<< "INDEX" << delim
			<< affinity->m_affinityidx << delim
			<< "ADD" << delim
			<< serverPoint << delim
			<< "TOTAL" << delim
			<< affinity->m_point
			<< end;
}

void do_AffinityShopUseReq(CPC* ch, CNetMsg::SP& msg)
{
	CAffinity* affinity;
	CAffinityProto* proto;
	CAffinityNPC* affinitynpc;
	int npcidx;
	CNPC* npc;
	RefMsg(msg) >> npcidx;

	npc = (CNPC*)(ch->m_pArea->FindCharInCell(ch, npcidx, MSG_CHAR_NPC, false));
	if(!npc)
		return ;

	proto = gserver->m_affinityProtoList.FindProtoByNPCIdx(npc->m_proto->m_index);
	if(!proto)
		return ;

	affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if(!affinitynpc)
		return;
	if(!affinitynpc->GetFlag(AF_SHOP))
	{
		return;
	}

	affinity = ch->m_affinityList.FindAffinity(proto->m_index);
	if(!affinity)
	{
		// 관련 친화도가 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_NOT_CONNECTED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if(!affinitynpc)
		return ;

	if(affinity->m_point < affinitynpc->m_shoppoint)
	{
		// 친화도 점수 부족
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_USESHOP_POINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_SHOPUSE_REP);
		RefMsg(rmsg) << npc->m_idNum;
		SEND_Q(rmsg, ch->m_desc);
	}
}

// 몇번째 선물을 받을건지 물어봄
void do_AffinityGiftInfoReq(CPC* ch, CNetMsg::SP& msg)
{
	CAffinity* affinity = NULL;
	CAffinityProto* proto = NULL;
	CNPC* npc;
	int npcidx;
	RefMsg(msg) >> npcidx;

	npc = (CNPC*)(ch->m_pArea->FindCharInCell(ch, npcidx, MSG_CHAR_NPC, false));
	if(!npc)
		return ;

	proto = gserver->m_affinityProtoList.FindProtoByNPCIdx(npc->m_proto->m_index);
	if(!proto)
		return ;

	CAffinityNPC* affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if(!affinitynpc)
		return;
	if(!affinitynpc->GetFlag(AF_REWARD))
	{
		// 관리자 설정 에러
		return;
	}

	affinity = ch->m_affinityList.FindAffinity(proto->m_index);

	if(!affinity)
	{
		// NPC와 연결된 친화도를 보유하고 있지 않다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_NOT_CONNECTED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	REWARDITEM* nextreward = affinity->GetNextReward(npc->m_proto->m_index);
	if(!nextreward)
	{
		// 받을 보상아이템이 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_GIFTINFO_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 캐릭터 레벨 검사
	if( nextreward->needPCLevel > ch->m_level )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_PCLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(nextreward->needItemIdx > 0)
	{
		item_search_t vec;
		int sc = ch->m_inventory.searchItemByCondition(nextreward->needItemIdx, 0, 0, vec);
		if (sc == 0 || sc < nextreward->needItemCount )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if(nextreward->allowpoint > affinity->m_point)
	{
		// 포인트가 부족하여 받지 못한다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_GIFTINFO_NOPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_GIFTINFO_REP);
		RefMsg(rmsg) << nextreward->allowpoint
					 << nextreward->itemidx
					 << nextreward->itemcount;

		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_AffinityGiftReq(CPC* ch, CNetMsg::SP& msg)
{
	CAffinity* affinity = NULL;
	CAffinityProto* proto = NULL;
	int npcidx;
	CNPC* npc;
	RefMsg(msg) >> npcidx;

	// npc가 주위에 있는지
	npc = (CNPC*)(ch->m_pArea->FindCharInCell(ch, npcidx, MSG_CHAR_NPC, false));
	if(!npc)
		return ;

	proto = gserver->m_affinityProtoList.FindProtoByNPCIdx(npc->m_proto->m_index);
	if(!proto)
		return ;

	CAffinityNPC* affinitynpc = proto->FindAffinityNPC(npc->m_proto->m_index);
	if(!affinitynpc)
		return;
	if(!affinitynpc->GetFlag(AF_REWARD))
	{
		return;
	}

	affinity = ch->m_affinityList.FindAffinity(proto->m_index);

	if(!affinity)
	{
		// NPC와 연결된 친화도를 보유하고 있지 않다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_NOT_CONNECTED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	REWARDITEM* nextreward = affinity->GetNextReward(npc->m_proto->m_index);
	if(!nextreward)
	{
		// 받을 보상아이템이 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_GIFTINFO_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(nextreward->allowpoint > affinity->m_point)
	{
		// 포인트가 부족하여 받지 못한다.
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_GIFTINFO_NOPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->m_inventory.getEmptyCount() < 1)
	{
		// 인벤공간 부족
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_GIFT_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 캐릭터 레벨 검사
	if( nextreward->needPCLevel > ch->m_level )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_PCLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(nextreward->needItemIdx > 0)
	{
		// 필요 아이템 검사 및 삭제
		item_search_t vec;
		int sc = ch->m_inventory.searchItemByCondition(nextreward->needItemIdx, 0, 0, vec);
		if (sc == 0 || sc < nextreward->needItemCount )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			AffinityErrorMsg(rmsg, MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		ch->m_inventory.deleteItem(vec, nextreward->needItemCount);
	}


	CItem* item = gserver->m_itemProtoList.CreateItem(nextreward->itemidx, -1, 0, nextreward->itemflag, nextreward->itemcount);
	if (item == NULL)
		return ;

	if (ch->m_inventory.addItem(item))
	{
		int exp = nextreward->exp;
		int sp = nextreward->sp;

		if( exp < 0) exp = 0;
		if( sp < 0) sp = 0;

		if(exp > 0 || sp > 0)
			ch->AddExpSP((LONGLONG)exp, sp, false, false, true);

		affinity->UpdateRewardStep(npc->m_proto->m_index, nextreward->allowpoint);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AffinityMsg(rmsg, MSG_EX_AFFINITY_GIFT_REP);
			RefMsg(rmsg) << npc->m_proto->m_index;
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_affinityList.SendRewardNotice(ch);
	}
	else
	{
		delete item;

		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_AffinityInfoTabReq(CPC* ch, CNetMsg::SP& msg)
{
	CAffinity* affinity = NULL;
	CAffinityProto* proto = NULL;
	int affinityidx;
	int errorcode = 0;
	int allowpoint = 0;
	RefMsg(msg) >> affinityidx;

	proto = gserver->m_affinityProtoList.FindProtoByAffinityIdx(affinityidx);
	if(!proto)
		return ;

	affinity = ch->m_affinityList.FindAffinity(proto->m_index);
	if(!affinity)
	{
		// NPC와 연결된 친화도를 보유하고 있지 않다.
		errorcode |= AFFINITY_INFOTAB_ERROR_NOTEXIST;

		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_INFOTAB_REP);
		RefMsg(rmsg) << allowpoint
					 << affinityidx
					 << errorcode;

		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	REWARDITEM* nextreward = affinity->GetNextReward(proto->m_affinityNPC->m_npcidx);

	if(!nextreward)
	{
		// 받을 보상아이템이 없다.
		REWARDSTEP* step = affinity->FindRewardInfo(proto->m_affinityNPC->m_npcidx);
		allowpoint = step->point;
		errorcode |= AFFINITY_INFOTAB_NOT_REWARD;
	}
	else
	{
		allowpoint = nextreward->allowpoint;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityMsg(rmsg, MSG_EX_AFFINITY_INFOTAB_REP);
		RefMsg(rmsg) << allowpoint
					 << affinityidx
					 << errorcode;

		SEND_Q(rmsg, ch->m_desc);
	}
}
