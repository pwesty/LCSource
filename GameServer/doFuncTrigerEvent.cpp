#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

#include "TrigerSet.h"

void do_TriggerEvent(CPC* ch, CNetMsg::SP& msg)
{
	if (DEAD(ch))
		return;

	long subtype;
	int	touch_ID;
	int triggerCount, i;

	msg->MoveFirst();
	RefMsg(msg) >> subtype >> touch_ID;

	// ch의 pArea에 속한 triggerList 가져옴
	CTrigger_List* triggerList = &ch->m_pArea->m_CTriggerList;

	if (!triggerList || !triggerList->m_Trigger)
		return;

	// 저장된 트리거 총 수를 구한다.
	triggerCount = triggerList->m_nCount;
	for ( i=0; i < triggerCount; i++ )
	{
		// DB에 저장된 데이터 중 해당되는 touch_ID를 찾는다.
		int ID = triggerList->m_Trigger[i].m_trigger_touchID;

		if ( ID == touch_ID )
		{
			switch (triggerList->m_Trigger[i].m_trigger_type)
			{
			case TRIGGER_ITEM_COMPOSITION:
				do_TriggerEvent_ItemComposition( ch, msg, &triggerList->m_Trigger[i]);
				break;

			case TRIGGER_ITEM_DROP:
				do_TriggerEvent_ItemDrop( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_OPEN_THE_DOOR:
				do_TriggerEvent_OpenTheDoor( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_START_RAID:
				do_TriggerEvent_StartRaid( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_MESSAGE_PRINT:
				do_TriggerEvent_MessagePrint( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_SUMMON_NPC:
				do_TriggerEvent_SummonNpc( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_USE_DEBUF:
				do_TriggerEvent_UseDebuf( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_ACTIVE_OBJECT:
				do_TriggerEvent_ActiveObject( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_REMOVE_NPC:
				do_TriggerEvent_RemoveNpc( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_TIME_LIMIT:
				do_TriggerEvent_TimeLimit( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_QUEST_COMPLETE:
				do_TriggerEvent_QuestComplete( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_ITEM_GET_INVEN:
				do_TriggerEvent_ItemGetInven( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_CONSECUTIVE_DIALOGUE:
				do_TriggerEvent_ConsecutiveDialogue( ch, msg, &triggerList->m_Trigger[i] );
				break;

			case TRIGGER_ACTIVE_EFFECT:
				do_TriggerEvent_ActiveEffect( ch, msg, &triggerList->m_Trigger[i] );
				break;

			default:
				break;
			}
		} // if
	} // for
}

void do_TriggerEvent_ItemComposition(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 아이템 조합되는 것으로 해당 레이드 맴버 변수에 넣어 줌
	CItem* pItem = gserver->m_itemProtoList.CreateItem(trigger->m_nItem_index_ItemComposition, -1, 0, 0, 1);
	if ( pItem == NULL )
	{
		GAMELOG << init("TRIGGER_ERROR : ITEM COMPOSITION 1")
				<< "item index composition" << delim
				<< trigger->m_nItem_index_ItemComposition
				<< end;
		goto TRIGGER_ERROR;
	}

	// 해당 레이드에 조합에 필요한 아이템이 모두 모였는지 체크
	for (int i = 0; i < trigger->m_nMeterial_IndexCount; i++)
	{
		item_search_t vec;
		int search_count = ch->m_inventory.searchItemByCondition(trigger->m_nMeterial_index[i], 0, 0, vec);
		if( search_count == 0 || search_count >= trigger->m_nMeterial_count[i] )
		{
			//모은 조합 아이템들 지워주기
			ch->m_inventory.deleteItem(vec, trigger->m_nMeterial_count[i]);
		}
		else
		{
			goto TRIGGER_DONT_ENOUGH_CONDITION;
		}
	}

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		goto TRIGGER_ERROR;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ITEM_COMPOSITION, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;

TRIGGER_DONT_ENOUGH_CONDITION:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_DONT_ENOUGH_CONDITION, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_ItemDrop(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

#ifdef _DEBUG
	{
		GAMELOG << init("TRIGER_DEBUG_LOG", ch)
				<< touchID << delim << playID << end;
	}
#endif

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* pItem = gserver->m_itemProtoList.CreateItem( trigger->m_ItemDrop_index, -1, 0, 0, 1 );

	// 아이템이 드랍 될 확률로 체크
	int percent = GetRandom(0, 100);
	if ( percent > trigger->m_ItemDrop_percent )
	{
		pItem = NULL;
		goto TRIGGER_DONT_ENOUGH_CONDITION;
	}

	// 아이템 드랍
	if ( pItem == NULL )
	{
		GAMELOG << init("TRIGGER_ERROR : ITEM DROP 1")
				<< "item dropindex" << delim
				<< trigger->m_ItemDrop_index
				<< end;
		pItem = NULL;
		goto TRIGGER_ERROR;
	}

	{
		pItem->m_preferenceIndex = ch->m_index;
		ch->m_pArea->DropItem( pItem, ch );
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg( rmsg, ch, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ITEM_DROP, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}
	pItem = NULL;

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;

TRIGGER_DONT_ENOUGH_CONDITION:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_DONT_ENOUGH_CONDITION, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_OpenTheDoor(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 서브 타입에 따라 트리거 적용이 달라 짐
	switch( trigger->m_nType_sub )
	{
	case TRIGGER_SUBTYPE_NONE_OPEN_THE_DOOR:
		break;

	case TRIGGER_SUBTYPE_NPC_OPEN_THE_DOOR:
		break;

	case TRIGGER_SUBTYPE_ITEM_OPEN_THE_DOOR:
		{
			int loopcount = trigger->m_NeedItem_IndexCount;
			if (loopcount > 0)
			{
				boost::scoped_array<item_search_t> vec(new item_search_t[loopcount]);
				boost::scoped_array<int> search_count(new int[loopcount]);

				for (int i = 0; i < loopcount; i++)
				{
					search_count[i] = ch->m_inventory.searchItemByCondition(trigger->m_NeedItem_Index[i], 0, 0, vec[i]);
					if (search_count[i] == 0 || search_count[i] < trigger->m_NeedItem_Count[i])
						goto TRIGGER_DONT_ENOUGH_CONDITION;
				}

				// 다 있으면 아이템 필요 수량만큼 지워준다.
				for (int i = 0; i < loopcount; i++)
				{
					ch->m_inventory.deleteItem(vec[i], trigger->m_NeedItem_Count[i]);
				}
			}
		}
		break;

	default:
		GAMELOG << init("TRIGGER_ERROR : OPEN THE DOOR 3 : switch()")
				<< end;
		goto TRIGGER_ERROR;
		break;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_OPEN_THE_DOOR, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 문 열기는 문을 직접 클릭이 아니면서 활성화가 되야 한다면 오브젝트 활성화 메세지를 보내준다.
	if(touchID != playID && playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;

TRIGGER_DONT_ENOUGH_CONDITION:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_DONT_ENOUGH_CONDITION, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_StartRaid(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		// 레이드 스타트는 오브젝트 활성화가 따로 필요 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_START_RAID, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_MessagePrint(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	// 성공 했다는 메세지 보내 줌
	// MessagePrint는 StringIndex를 추가해서 클라에게 데이터 보내줌
	// StringIndex가 0 이면 메세지를 보내지 않는다.
	if (trigger->m_nStringIndex > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_MESSAGE_PRINT, touchID, playID, trigger->m_bContinue);
		RefMsg(rmsg) << trigger->m_nStringIndex;
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_SummonNpc(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// npc를 정해진 count 만큼 소환한다.
	int i, j;
	for(i=0; i < trigger->m_nSummonNpc_indexCount; i++)
	{
		for (j=0; j < trigger->m_nSummonNpc_count[i]; j++)
		{
			// 나중에 리젠 정보가 정리되면 이것을 사용한다.
			//CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
			//if (p->m_totalNum != 0)
			//	continue ;

			// 리젠 정보가 없어서 임시로 사용
			float x = 0.0f;
			float z = 0.0f;
			int nTry = 0;
			do
			{
				x = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
				z = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
				unsigned short mapAttr = ch->m_pArea->GetAttr(GET_YLAYER(ch), x, z);
				if(mapAttr & MATT_WALKABLE || mapAttr & MATT_STAIR_UP || mapAttr & MATT_STAIR_DOWN){}
				else
				{
					x = 0.0f;
					z = 0.0f;
				}
				nTry++;
			}
			while (nTry <= 10 && x < 0.1f && z < 0.1f);
			if (nTry > 10)
			{
				x = GET_X(ch);
				z = GET_Z(ch);
			}

			CNPC* pNPC = gserver->m_npcProtoList.Create(trigger->m_nSummonNpc_index[i], NULL);
			if (pNPC)
			{
				GET_X(pNPC)				= x;
				GET_Z(pNPC)				= z;
				GET_R(pNPC)				= GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				GET_YLAYER(pNPC)		= GET_YLAYER(ch);

				pNPC->m_regenX			= GET_X(pNPC);
				pNPC->m_regenZ			= GET_Z(pNPC);
				pNPC->m_regenY			= GET_YLAYER(pNPC);

				pNPC->m_recoverPulse	= gserver->m_pulse;

				pNPC->InitPointsToMax();
				pNPC->m_disableTime	= 0;
				pNPC->m_postregendelay = 0;

				ch->m_pArea->AddNPC(pNPC);

				int		nCellX;
				int		nCellZ;
				ch->m_pArea->PointToCellNum(GET_X(pNPC), GET_Z(pNPC), &nCellX, &nCellZ);
				ch->m_pArea->CharToCell(pNPC, GET_YLAYER(pNPC), nCellX, nCellZ);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					AppearMsg(rmsg, pNPC, true);
					ch->m_pArea->SendToCell(rmsg, GET_YLAYER(pNPC), nCellX, nCellZ);
				}

				pNPC->m_aipulse		= gserver->m_pulse;
				pNPC->m_delay		= PULSE_REAL_SEC * 2;
				pNPC->m_nBlockRegen	= ch->m_index;
			}
			else
			{
				GAMELOG << init("TRIGGER_ERROR : SUMMON_NPC 1")
						<< "npc index" << delim
						<< trigger->m_nSummonNpc_index[i]
						<< end;
				goto TRIGGER_ERROR;
			}
		} // for
	} // for

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_SUMMON_NPC, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_UseDebuf(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// debuf를 정해진 만큼 받는다.
	int i;
	for (i=0; i < trigger->m_nUseDebuf_indexCount; i++)
	{
		CSkill* pDebuff = NULL;
		pDebuff = gserver->m_skillProtoList.Create(trigger->m_nUseDebuf_index[i]);
		if( pDebuff == NULL )
		{
			GAMELOG << init("TRIGGER_ERROR : USE_DEBUF 1")
					<< "debuf index" << delim
					<< trigger->m_nUseDebuf_index[i]
					<< end;
			goto TRIGGER_ERROR;
		}

		// 아이템 인덱스에 -2는 트리거에 의한 스킬 발동임을 나타냄
		/* 추가 요망 존 검사해서 레이드 존이면 원정대가 모두 걸리게 하고 그 외는 파티가 모두 걸리게 한다.*/
		bool bDebuffApply = false;

		if (ch->m_pZone && ch->m_pArea
				&& (ch->m_pZone->m_index == ZONE_CAPPELLA_1
					|| ch->m_pZone->m_index == ZONE_ALTER_OF_DARK))
		{
			ch->m_pArea->ApplySkillToAllClient(pDebuff, bDebuffApply);
		}
		else
			ApplySkill( ch, ch, pDebuff, -2, bDebuffApply);

		if (!bDebuffApply)
		{
			pDebuff = NULL;
			GAMELOG << init("TRIGGER_ERROR : USE_DEBUF 2")
					<< "debuf index" << delim
					<< trigger->m_nUseDebuf_index[i]
					<< end;
		}
		pDebuff = NULL;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_USE_DEBUF, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_ActiveObject(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_RemoveNpc(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 서브 타입에 따라 트리거 적용이 달라 짐
	switch( trigger->m_nType_sub )
	{
	case TRIGGER_SUBTYPE_NONE_REMOVE_NPC:
		break;

	case TRIGGER_SUBTYPE_ITEM_REMOVE_NPC:
		{
			CItem* pItem = ch->m_inventory.FindByDBIndex(trigger->m_Item_Index_removeNPC, 0, 0);
			if (pItem == NULL)
			{
				goto TRIGGER_DONT_ENOUGH_CONDITION;
			}

			int itemCount = pItem->Count();

			// item당 죽은 npc 수를 이용해 총 죽는 npc를 구한다.
			int totalKilledNpc = itemCount * trigger->m_KillCount;

			// 죽일 npc를 찾아서 총 죽는 수만큼 제거한다.
			CNPC* tnpc = ch->m_pArea->m_npcList;
			if ( tnpc == NULL || tnpc->m_pArea == NULL )
			{
				GAMELOG << init("TRIGGER_ERROR : REMOVE_NPC 2 : npc NULL")
						<< end;
				goto TRIGGER_ERROR;
			}

			CNPC* npc = NULL;
			while ((npc = tnpc))
			{
				tnpc = npc->m_pNPCNext;

				if (npc == NULL)
					continue ;
				if ( npc->m_pArea == NULL )
					continue ;

				// 죽는 npc가 맞다면 제거, npc는 총 죽는 수만큼만 죽는다.
				if ( npc->m_proto->m_index == trigger->m_KilledNpc_Index_ForRemoveNpc && totalKilledNpc > 0)
				{
					npc->SendDisappearAllInCell(true);

					// 어택리스트에서 삭제
					DelAttackList(npc);

					// npc 삭제
					npc->m_pArea->CharFromCell(npc, true);
					npc->m_pArea->DelNPC(npc);

					// 총 죽는 수 하나 줄이기
					totalKilledNpc--;
				}
			}

			// 남는 item 개수 구하기
			int itemCount_rest;
			if ( totalKilledNpc <= 0 )
				itemCount_rest = 0;
			else
				itemCount_rest = totalKilledNpc / trigger->m_KillCount;
			// 죽는 npc 만큼 item도 줄어야 한다.
			itemCount = itemCount - itemCount_rest;
			ch->m_inventory.decreaseItemCount(pItem, itemCount);
		}
		break;

	default:
		GAMELOG << init("TRIGGER_ERROR : REMOVE_NPC 3 : switch() ")
				<< end;
		goto TRIGGER_ERROR;
		break;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_REMOVE_NPC, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;

TRIGGER_DONT_ENOUGH_CONDITION:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_DONT_ENOUGH_CONDITION, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_TimeLimit(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	time_t curTime;
	time(&curTime);

	// 타이머 함수로 권한을 넘긴다. 현재 시간 저장
	trigger->SetTriggerTime(curTime);

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		// TimeLimit는 제한 시간을 추가해서 클라에게 데이터 보내줌. 타이머를 쓰는 모든 트리거는 시간 정보까지 줘야 함
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_TIME_LIMIT, touchID, playID, trigger->m_bContinue);
		RefMsg(rmsg) << trigger->m_TimeLimitSec;
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_QuestComplete(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	bool bComplete = false;
	CQuest* pQuest = ch->m_questList.FindQuest(trigger->m_nQuestIndex);
	if(pQuest)
	{
		int i;
		for (i = 0; i < QUEST_MAX_CONDITION; i++)
		{
			switch (pQuest->GetQuestProto()->m_conditionType[i])
			{
			case QCONDITION_AREA:
				{
					if(touchID == pQuest->GetQuestProto()->m_conditionIndex[i] )
					{
						if (pQuest->GetQuestValue(i) < pQuest->GetQuestProto()->m_conditionNum[i] )
						{
							pQuest->IncreaseQuestValue(i);
						}

						if(pQuest->GetQuestValue(i) >= pQuest->GetQuestProto()->m_conditionNum[i] )
						{
							bComplete = true;
						}
					}
					else
						bComplete = true;
				}
				break;
			default:
				break;
			}
		}

		if(bComplete)
		{
			pQuest->SetComplete1(true);
			pQuest->SetComplete2(true);

			CNetMsg::SP rmsg(new CNetMsg);
			QuestCompleteMsg(rmsg, pQuest);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ITEM_DROP, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_ItemGetInven(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;
	int i;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 가능 검사
	if ( ch->m_inventory.getEmptyCount() < trigger->m_nGetItem_TotalCount )
		goto TRIGGER_ERROR;

	for (i=0; i < trigger->m_nGetItem_TotalCount; i++)
	{
		// 확률에 못미치면 스킵
		int rand = GetRandom(1, 100);
		int ItemProb = trigger->m_nGetItem_Prob[i];
		if (rand > ItemProb)
			continue;

		CItem* pItem;
		pItem = gserver->m_itemProtoList.CreateItem(trigger->m_nGetItem_Index[i], -1, 0, 0, trigger->m_nGetItem_Count[i]);

		if ( pItem == NULL )
			goto ITEM_FAIL;

		if (ch->m_inventory.addItem(pItem) == false)
		{
			delete pItem;
			goto ITEM_FAIL;
		}
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ITEM_GET_INVEN, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;

ITEM_FAIL:
	for (i=0; i < trigger->m_nGetItem_TotalCount; i++)
	{
		item_search_t vec;
		int sc = ch->m_inventory.searchItemByCondition(trigger->m_nGetItem_Index[i], 0, 0, vec);
		if (sc > 0)
		{
			ch->m_inventory.deleteItem(vec, trigger->m_nGetItem_Count[i]);
		}
	}
	// TRIGGER_ERROR로 넘어가서 메세지를 보내준다.

TRIGGER_ERROR:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ERROR, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;
}

void do_TriggerEvent_ConsecutiveDialogue(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		// 연속 대화는 트리거 아이디와 스트링 인덱스를 메세지로 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_CONSECUTIVE_DIALOGUE, touchID, playID, trigger->m_bContinue);
		RefMsg(rmsg) << trigger->m_nMainDialogueString
					 << trigger->m_nNextDialogueString1
					 << trigger->m_nNextDialogueTriggerID1
					 << trigger->m_nNextDialogueString2
					 << trigger->m_nNextDialogueTriggerID2
					 << trigger->m_nNextDialogueString3
					 << trigger->m_nNextDialogueTriggerID3
					 << trigger->m_nNextDialogueString4
					 << trigger->m_nNextDialogueTriggerID4
					 << trigger->m_nNextDialogueString5
					 << trigger->m_nNextDialogueTriggerID5;

		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_TriggerEvent_ActiveEffect(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger)
{
	int touchID, playID;

	if ( ch == NULL )
		return;

	if (trigger == NULL)
		return;

	touchID = trigger->m_trigger_touchID;
	playID	= trigger->m_trigger_playID;

	// 이미 실행했으면 스킵
	if(trigger->m_bUsed == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ALREADY_PLAY, touchID, playID, trigger->m_bContinue);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성공 했으므로 실행된 것으로 체크, 지속 트리거이면 다음에도 가능 함
	if (trigger->m_bContinue)
		trigger->m_bUsed = false;
	else
		trigger->m_bUsed = true;

	{
		// 성공 했다는 메세지 보내 줌
		// 이펙트 활성화는 이펙트 이름과 자신인지 다른 npc인지, npc의 인덱스를 메세지로 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_EFFECT, touchID, playID, trigger->m_bContinue);
		RefMsg(rmsg) << trigger->m_strEffectName
					 << trigger->m_nTargetType
					 << trigger->m_nEffectNPCIndex;

		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 오브젝트 활성화가 필요하면 활성화 시킨다.
	if(playID != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, touchID, playID, trigger->m_bContinue);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 도미노 형식의 연쇄적인 트리거 발동, touchID와 playID가 같으면 무한 루프에 빠지므로 체크해 준다.
	if (trigger->m_bDomino && touchID != playID)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
					 << playID;
		do_TriggerEvent(ch, rmsg);
	}
	return;
}

void do_ExRaidScene(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL)
		return ;
	msg->MoveFirst();
	int msgex;
	int TodoOutput;
	int object_type;

	RefMsg(msg) >> msgex
				>> TodoOutput
				>> object_type;

	if(TodoOutput == OBJECT_TYPE_TODO)
	{
		switch(object_type)
		{
		case TOUCH_FIELD:
			do_ExRaidSceneTodoTouchField(ch, msg);
			break;
		case KILL_NPC:
			do_ExRaidSceneTodoKillNPC(ch, msg);
			break;
		case TAKE_ITEM:
			do_ExRaidSceneTodoTakeItem(ch, msg);
			break;
		case ALL_IN_AREA:
			do_ExRaidSceneTodoAllInArea(ch, msg);
			break;
		case NO_IN_AREA:
			do_ExRaidSceneTodoNoInArea(ch, msg);
			break;
		case MOVE_IN_AREA:
			do_ExRaidSceneTodoMoveInArea(ch, msg);
			break;
		case MOVE_OUT_AREA:
			do_ExRaidSceneTodoMoveOutArea(ch, msg);
			break;
		case COUNT_DOWN:
			do_ExRaidSceneTodoCountDown(ch, msg);
			break;
		case START_RAID:
			do_ExRaidSceneTodoStartRaid(ch, msg);
			break;
		default:
			break;
		}
	}
	else if(TodoOutput == OBJECT_TYPE_OUTPUT)
	{
		switch(object_type)
		{
		case TOUCH_FIELD:
			do_ExRaidSceneTouchField(ch, msg);
			break;
		case SUMMON_EXTRA:
			do_ExRaidSceneSummonExtra(ch, msg);
			break;
		case SUMMON_DISTRICT:
			do_ExRaidSceneSummonDistrict(ch, msg);
			break;
		case SUMMON_MBOSS:
			do_ExRaidSceneSummonMBoss(ch, msg);
			break;
		case SUMMON_BOSS:
			do_ExRaidSceneSummonBoss(ch, msg);
			break;
		case GROUND_EFFECT_ON:
			do_ExRaidSceneGroundEffectOn(ch, msg);
			break;
		case GROUND_EFFECT_OFF:
			do_ExRaidSceneGroundEffectOff(ch, msg);
			break;
		case TOGGLE_GATE:
			do_ExRaidSceneToggleGate(ch, msg);
			break;
		case TOGGLE_PORTAL:
			do_ExRaidSceneTogglePortal(ch, msg);
			break;
		case CREATE_ITEM_OBJECT:
			do_ExRaidSceneCreateItemObject(ch, msg);
			break;
		case DEBUFF_SKILL:
			do_ExRaidSceneDebuffSkill(ch, msg);
			break;
		case END_RAID:
			do_ExRaidSceneEndRaid(ch, msg);
			break;
		case PAUSE_AREA:
			do_ExRaidScenePauseArea(ch, msg);
			break;
		case BUFF_SKILL:
			do_ExRaidSceneBuffSkill(ch, msg);
			break;
		case TOGGLE_CAMERA:
			do_ExRaidSceneToggleCamera(ch, msg);
			break;
		case COUNT_DOWN_START:
			do_ExRaidSceneCountDownStart(ch, msg);
			break;
		case SAVE_POINT:
			do_ExRaidSceneSavePoint(ch, msg);
			break;
		case MODEL_HOLDER_PADOX:
			do_ExRaidSceneModelHolder(ch, msg);
			break;
		case NPC_SAY:
			do_ExRaidSceneNpcSay(ch, msg);
			break;
		case APPLY_SKILL:
			do_ExRaidSceneApplySkill_TO_ch(ch, msg);
			break;
		case CURE_SKILL:
			do_ExRaidSceneCureSkill_TO_ch(ch, msg);
			break;
		default:
			break;
		}
	}
	else if(TodoOutput == OBJECT_TYPE_OBJECT_STATE)
	{
		switch(object_type)
		{
		case ZONE_AKAN_TEMPLE:
			{
				do_ExRaidSceneSendObjectState(ch, object_type);
			}
			break;
		case ZONE_TARIAN_DUNGEON:
			{
				do_ExRaidSceneSendObjectState(ch, object_type);
			}
			break;
		default:
			break;
		}
	}
}

///////////////////////////////// TODO /////////////////////////////////
void do_ExRaidSceneTodoTouchField(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(TOUCH_FIELD, index, data, ch);

	return ;
}
void do_ExRaidSceneTodoKillNPC(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(KILL_NPC, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoTakeItem(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(data, -1, 0, 0, 1);
	if(pItem == NULL)
		return ;

	if(ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].GetSavePoint() < 4)
	{
		if (ch->m_inventory.addItem(pItem))
		{
			GAMELOG << init("ADD TO INVENTORY AKAN TEMPLE RAID ITEM", ch) << itemlog(pItem) << end;

			ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].SetTheObjectState(OBJECT_ITEM, index, 0);		//object 상태 설정
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SendRaidSceneObjectRefreshMsg(rmsg, ch, OBJECT_ITEM, index, 0);
				ch->m_pArea->SendToAllClient(rmsg);
			}
			ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(TAKE_ITEM, index, data, ch);
		}
		else
		{
			delete pItem;

			// 인벤이 가득차서 아이템 습득 불가.
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);
			}
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SendRaidSceneObjectRefreshMsg(rmsg, ch, OBJECT_ITEM, index, 1);
				ch->m_pArea->SendToAllClient(rmsg);
			}

			return ;
		}
	}
}

void do_ExRaidSceneTodoAllInArea(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(ALL_IN_AREA, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoNoInArea(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(NO_IN_AREA, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoMoveInArea(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(MOVE_IN_AREA, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoMoveOutArea(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(MOVE_OUT_AREA, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoCountDown(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(COUNT_DOWN, index, data, ch);
	return ;
}
void do_ExRaidSceneTodoStartRaid(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;

	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteTodo(START_RAID, index, data, ch);
	return ;
}
///////////////////////////////// TODO /////////////////////////////////

//////////////////////////////// OUTPUT ////////////////////////////////
void do_ExRaidSceneTouchField(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].SetTheObjectState(OBJECT_TOUCH_FIELD, index, data);		//object 상태 설정
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(TOUCH_FIELD, index, data, ch);
	return ;
}
void do_ExRaidSceneSummonExtra(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(SUMMON_EXTRA, index, data, ch);
	return ;
}
void do_ExRaidSceneSummonDistrict(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;

	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(SUMMON_DISTRICT, index, data, ch);
	return ;
}
void do_ExRaidSceneSummonMBoss(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(SUMMON_MBOSS, index, data, ch);
	return ;
}
void do_ExRaidSceneSummonBoss(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(SUMMON_BOSS, index, data, ch);
	return ;
}
void do_ExRaidSceneGroundEffectOn(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(GROUND_EFFECT_ON, index, data, ch);
	return ;
}
void do_ExRaidSceneGroundEffectOff(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(GROUND_EFFECT_OFF, index, data, ch);
	return ;
}
void do_ExRaidSceneToggleGate(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].SetTheObjectState(OBJECT_GATE, index, data);	//object 상태 설정
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(TOGGLE_GATE, index, data, ch);
	return ;
}
void do_ExRaidSceneTogglePortal(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].SetTheObjectState(OBJECT_PORTAL, index, data);	//object 상태 설정
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(TOGGLE_PORTAL, index, data, ch);
	return ;
}
void do_ExRaidSceneCreateItemObject(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].SetTheObjectState(OBJECT_ITEM, index, data);	//object 상태 설정
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(CREATE_ITEM_OBJECT, index, data, ch);
	return ;
}
void do_ExRaidSceneDebuffSkill(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(DEBUFF_SKILL, index, data, ch);
	return ;
}
void do_ExRaidSceneEndRaid(CPC* ch, CNetMsg::SP& msg)
{
	// Room 정보 하고 레이드 참여 정보 초기화
	return ;
}
void do_ExRaidScenePauseArea(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(PAUSE_AREA, index, data, ch);
	return ;
}
void do_ExRaidSceneBuffSkill(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(BUFF_SKILL, index, data, ch);
	return ;
}
void do_ExRaidSceneToggleCamera(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL)
		return ;

	int nRoomNo;
	nRoomNo = ch->m_nJoinInzone_RoomNo;

	//헬퍼로 room no 초기화 발송.

	return ;
}
void do_ExRaidSceneCountDownStart(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(COUNT_DOWN_START, index, data, ch);
	return ;
}
void do_ExRaidSceneSavePoint(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(SAVE_POINT, index, data, ch);
	return ;
}
//////////////////////////////// OUTPUT ////////////////////////////////

void do_ExRaidSceneSendObjectState(CPC* ch, int zone)
{
	if(ch == NULL)
		return ;

	CNetMsg::SP rmsg(new CNetMsg);
	SendRaidSceneObjectStateMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
	return ;
}

void do_ExRaidSceneModelHolder(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(MODEL_HOLDER_PADOX, index, data, ch);
	return ;
}

void do_ExRaidSceneNpcSay(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(NPC_SAY, index, data, ch);
	return ;
}

void do_ExRaidSceneApplySkill_TO_ch(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(APPLY_SKILL, index, data, ch);
	return ;
}

void do_ExRaidSceneCureSkill_TO_ch(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL || ch->m_pArea == NULL)
		return ;
	int index, data;
	RefMsg(msg) >> index
				>> data;
	ch->m_pArea->m_RaidInfoList[ch->m_pArea->m_RaidDifficulty].ExcuteOutput(CURE_SKILL, index, data, ch);
	return ;
}
