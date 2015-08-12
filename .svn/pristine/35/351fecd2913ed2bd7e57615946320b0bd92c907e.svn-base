#include <boost/scoped_ptr.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "Descriptor.h"
#include "EventProcessWithThread.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "Guild.h"
#include "Item.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "DescManager.h"

extern void getCharIndexByGPS_return_DBThread(boost::any& any);

EventProcessForDB::EventProcessForDB( void )
{
	handle_ = bnf::instance()->CreateEvent(this);
	session_ = (EventSession*)bnf::instance()->GetSessionPointer(handle_);
}

EventProcessForDB::~EventProcessForDB( void )
{
	bnf::instance()->RemoveSession(session_);
}

EventProcessForDB* EventProcessForDB::instance()
{
	static EventProcessForDB __instace;
	return &__instace;
}

//XX 로그인 5
void EventProcessForDB::pushCharList( charlist& list )
{
	EventSession::type_t pushdata(EVENT_TYPE_CHAR_LIST, list);

	session_->push(pushdata);
}

//XX 캐릭터생성 4
void EventProcessForDB::pushCreateCharacter( returnCreateChar& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_CREATE_CHARACTER, data);

	session_->push(pushdata);
}

//XX 캐릭터삭제 4
void EventProcessForDB::pushDeleteCharacter( deleteChar& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_DELETE_CHARACTER, data);

	session_->push(pushdata);
}

//XX 캐릭터복원 4
void EventProcessForDB::pushCancelDeleteCharacter( cancleDeleteChar& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_CANCEL_DELETE_CHARACTER, data);

	session_->push(pushdata);
}

//XX 캐릭터선택 4
void EventProcessForDB::pushSelectCharacter( selectChar& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_SELECT_CHARACTER, data);

	session_->push(pushdata);
}

void EventProcessForDB::pushSelectCharacterOtherZone( moveOtherZone& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_SELECT_CHARACTER_OTHER_ZONE, data);

	session_->push(pushdata);
}

//XX 거래대행 - 찾기 4
void EventProcessForDB::pushExpressTake( expressTake& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_EXPRESS_TAKE, data);

	session_->push(pushdata);
}

void EventProcessForDB::pushSendMessageToClient( sendMessageToClient& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_SEND_MESSAGE_TO_CLIENT, data);

	session_->push(pushdata);
}

void EventProcessForDB::pushGetCharIndexByGPS( getCharIndexByGPS& data )
{
	EventSession::type_t pushdata(EVENT_TYPE_GET_CHAR_INDEX_BY_GPS, data);
	session_->push(pushdata);
}

//////////////////////////////////////////////////////////////////////////

void EventProcessForDB::operate( rnSocketIOService* service )
{
	EventSession::type_t r = ((EventSession*)service)->pop();

	//LOG_INFO("DEBUG_FUNC : START : r.first : %d", r.first);
	switch (r.first)
	{
	case EVENT_TYPE_CHAR_LIST:
		this->process_char_list(r.second);
		break;

	case EVENT_TYPE_CREATE_CHARACTER:
		this->process_create_character(r.second);
		break;

	case EVENT_TYPE_DELETE_CHARACTER:
		this->process_delete_character(r.second);
		break;

	case EVENT_TYPE_CANCEL_DELETE_CHARACTER:
		this->process_cancel_delete_character(r.second);
		break;

	case EVENT_TYPE_SELECT_CHARACTER:
		this->process_select_character(r.second);
		break;

	case EVENT_TYPE_SELECT_CHARACTER_OTHER_ZONE:
		this->process_select_character_other_zone(r.second);
		break;

	case EVENT_TYPE_EXPRESS_TAKE:
		this->process_express_take(r.second);
		break;

	case EVENT_TYPE_SEND_MESSAGE_TO_CLIENT:
		this->process_send_message_to_client(r.second);
		break;

	case EVENT_TYPE_GET_CHAR_INDEX_BY_GPS:
		getCharIndexByGPS_return_DBThread(r.second);
		break;

	default:
		break;
	}
	//LOG_INFO("DEBUG_FUNC : END : r.first : %d", r.first);
}

//XX 로그인 6 - end
void EventProcessForDB::process_char_list( boost::any& any )
{
	charlist* data = boost::any_cast<charlist>(&any);
	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}
	desc->setExtendCharacterSlotTime(data->m_tExtendCharacterSlotTime);
	desc->m_bNotCreateNightShadow = data->m_bNotCreateNightShadow;
	desc->m_bCreate90Lv = data->m_bCreate90Lv;
	desc->m_bIsNSCreateCardUse = data->m_bIsNSCreateCardUse;

#if defined(LC_TLD) && defined(GMTOOL)
	if (desc->m_userFlag & IS_GM_CHAR)
	{
		desc->request_start_game_flag_ = false;
		DBManager::instance()->PushSelectCharacter(desc, data->char_index, NULL);
	}
#endif
}

//XX 캐릭터생성 5 - end
void EventProcessForDB::process_create_character( boost::any& any )
{
	returnCreateChar* data = boost::any_cast<returnCreateChar>(&any);

	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}

	GAMELOG	<< init("CHAR_CREATE", data->name.c_str(), data->name.c_str(), desc->m_idname)
			<< end;

	// 클라이언트에게 캐릭터 리스트를 다시 내려준다.
	DBManager::instance()->PushCharacterList(desc);

	if (desc->m_bCreate90Lv && desc->m_bIsNSCreateCardUse && data->job == JOB_NIGHTSHADOW)
	{
		// NS 생성카드 사용 정보 지우기
		desc->m_bIsNSCreateCardUse = false;
		desc->m_bNotCreateNightShadow = true;

		GAMELOG << init("NS_CREATE_CARD_USE")
				<< desc->m_index << delim
				<< desc->m_idname << end;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperNSCreateCardUse(rmsg, MSG_HELPER_NS_CARD_DELETE_DATA, desc->m_index, data->char_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_CREATED")
			   << LOG_VAL("account-id", desc->m_idname ) << blank
			   << LOG_VAL("character-id", data->name.c_str()) << blank
			   << LOG_VAL("char-type", data->job) << blank
			   << endGer;
#endif // GER_LOG
}

//XX 캐릭터삭제 5 - end
void EventProcessForDB::process_delete_character( boost::any& any )
{
	// 삭제, 복원의 경우 클라이언트에게 캐릭터 리스트를 다시 내려준다.
	deleteChar* data = boost::any_cast<deleteChar>(&any);
	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}

	DBManager::instance()->PushCharacterList(desc);

	GAMELOG << init("CHAR_DELETE", desc->m_idname)
			<< "Char Index" << delim << data->m_index << delim
			<< "IP" << delim << desc->service_->ip().c_str()
			<< end;

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_DELETE")
			   << LOG_VAL("account-id", desc->m_idname ) << blank
			   << LOG_VAL("character-id", data->name.c_str() ) << blank
			   << LOG_VAL("char-type", data->job ) << blank
			   << endGer;
#endif // GER_LOG
}

//XX 캐릭터복원 5 - end
void EventProcessForDB::process_cancel_delete_character( boost::any& any )
{
	// 삭제, 복원의 경우 클라이언트에게 캐릭터 리스트를 다시 내려준다.
	cancleDeleteChar* data = boost::any_cast<cancleDeleteChar>(&any);
	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}

	DBManager::instance()->PushCharacterList(desc);
}

//XX 캐릭터선택 5 - end
void EventProcessForDB::process_select_character( boost::any& any )
{
	selectChar* data = boost::any_cast<selectChar>(&any);
	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		delete data->pChar;

		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}

	if (STATE(desc) != CON_GET_IDEN_MSG)
	{
		LOG_ERROR("invalid STATE condition(desc) = %d user_index = %d", STATE(desc), desc->m_index);

		delete data->pChar;
		return;
	}

	if (desc->m_pChar != NULL)
	{
		LOG_ERROR("already runched pChar. user_index = %d", desc->m_index);

		delete data->pChar;
		return;
	}

	// CDescriptor와 CPC를 서로 연결해준다.
	desc->m_pChar = data->pChar;
	desc->m_pChar->m_desc = desc;
	desc->m_pChar->m_TimerItem.setOwner(desc->m_pChar);
	CDescriptor::makeAutoSaveSequence(desc);

	PCManager::instance()->addPlayer(desc->m_pChar);

	if(desc->m_pChar->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
	{
		gserver->m_kailuxUser.insert(desc->m_pChar);
		desc->m_pChar->m_syndicateManager.applySyndicateSkill(SYNDICATE::eSYNDICATE_KAILUX);
	}
	else if(desc->m_pChar->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		gserver->m_dealermoonUser.insert(desc->m_pChar);
		desc->m_pChar->m_syndicateManager.applySyndicateSkill(SYNDICATE::eSYNDICATE_DEALERMOON);
	}

	memcpy(desc->m_notice, data->m_notice, sizeof(desc->m_notice));

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachLoadReq(rmsg, desc->m_idname, desc->m_pChar->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	if (desc->request_start_game_flag_)
	{
		// DB에서 캐릭터 정보를 다 읽기전에 클라이언트가 MSG_START_GAME패킷을 보내면
		// 기다리지 말고 바로 데이터를 전송한다.
		desc->sendData_StartAndMoveZone();
	}

	{
		// Helper, SubHelper에게 접속 정보를 전달
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::LoginUserInfo* packet = reinterpret_cast<ServerToServerPacket::LoginUserInfo*>(rmsg->m_buf);
		rmsg->m_mtype = packet->type = MSG_SERVER_TO_SERVER;
		packet->subType = MSG_SUB_SERVERTOSERVER_USER_LOGIN;
		packet->userIndex = desc->m_index;
		packet->charIndex = desc->m_pChar->m_index;
		packet->serverNo = gserver->m_serverno;
		packet->zoneNo = desc->m_pChar->m_pZone->m_index;
		memcpy(packet->userId, (const char *)desc->m_idname, sizeof(packet->userId));
		packet->userId[MAX_ID_NAME_LENGTH] = '\0';
		memcpy(packet->charName, (const char *)desc->m_pChar->m_nick, sizeof(packet->charName));
		packet->charName[MAX_CHAR_NAME_LENGTH] = '\0';
		rmsg->setSize(sizeof(ServerToServerPacket::LoginUserInfo));

		SEND_Q(rmsg, gserver->m_helper);
		SEND_Q(rmsg, gserver->m_subHelper);
	}

#if defined(LC_TLD) && defined(GMTOOL)
	if (desc->m_userFlag & IS_GM_CHAR)
	{
		gserver->GMToolCharPrePlay(desc);
	}
#endif

#ifdef XTRAP
	desc->m_pChar->xTrapSessionInit();
#endif
}

//XX 캐릭터선택 5 - end (다른 존으로 이동을 처리하는곳)
void EventProcessForDB::process_select_character_other_zone( boost::any& any )
{
	moveOtherZone* data = boost::any_cast<moveOtherZone>(&any);

	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->m_index);
	if (desc == NULL || desc->m_seq_index != data->m_seq_index)
	{
		LOG_INFO("Not found userIndex[%d]", data->m_index);
		return;
	}

	desc->Close("Move Other Zone");
}

//XX 거래대행 - 찾기 5 (end)
void EventProcessForDB::process_express_take( boost::any& any )
{
	expressTake* data = boost::any_cast<expressTake>(&any);

	CPC* ch = PCManager::instance()->getPlayerByUserIndex(data->m_index);
	if (ch == NULL)
		return;

	if (STATE(ch->m_desc) != CON_PLAYING)
	{
		LOG_ERROR("EXPRESS SYSTEM TAKE. not play user. user_index[%d]", ch->m_desc->m_index);
		return;
	}

	// 아이템 정보를 캐릭터 정보에 저장
	if (data->itemInfo.nas > 0) // 나스(돈)
	{
		ch->m_inventory.increaseMoney(data->itemInfo.nas);

		GAMELOG << init("EXPRESS SYSTEM TAKE. NAS", ch)
				<< "CHAR INDEX : " << ch->m_index << delim
				<< "NAS : " << data->itemInfo.nas << delim
				<< "SEND TYPE : " << data->itemInfo.send_type << end;
	}
	else if (data->itemInfo.item_index != -1) // 아이템 수령하기
	{
		// 소켓 갯수 구하기
		int socket_count = 0;
		for (int i = 0; i < MAX_SOCKET_COUNT; ++i, ++socket_count)
		{
			if (data->itemInfo.socket[i] == -1)
				break;
		}

		// DB item option -> option으로 변경
		short item_option[MAX_ITEM_OPTION] = {0,};
		for (int i = 0; i < MAX_ITEM_OPTION; ++i)
		{
			item_option[i] = COption::SetDBValue(data->itemInfo.option_type[i], data->itemInfo.option_level[i]);
		}

#ifdef DURABILITY
		CItem* pItem = gserver->m_itemProtoList.CreateDBItem(
						   data->itemInfo.item_index,
						   -1, // wearPos
						   data->itemInfo.plus,
						   data->itemInfo.flag,
						   0, // used
						   0, // used_2
						   data->itemInfo.serial,
						   (LONGLONG)data->itemInfo.item_count,
						   item_option,
						   socket_count,
						   data->itemInfo.socket,
						   data->itemInfo.item_origin,
						   data->itemInfo.now_durability,
						   data->itemInfo.max_durability
					   );
#else
		CItem* pItem = gserver->m_itemProtoList.CreateDBItem(
						   data->itemInfo.item_index,
						   -1, // wearPos
						   data->itemInfo.plus,
						   data->itemInfo.flag,
						   0, // used
						   0, // used_2
						   data->itemInfo.serial,
						   (LONGLONG)data->itemInfo.item_count,
						   item_option,
						   socket_count,
						   data->itemInfo.socket,
						   data->itemInfo.item_origin
					   );
#endif
		if (pItem == NULL)
		{
			LOG_FATAL("EXPRESS SYSTEM TAKE. NOT CREATE ITEM. item_index[%d] / item_plsu[%d] / item_serial[%s]",
					  data->itemInfo.item_index, data->itemInfo.plus, data->itemInfo.serial);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_CAN_NOT_CREATE_ITEM);
			SEND_Q(rmsg, ch->m_desc);

			return;
		}

		if (data->itemInfo.serial[0] == '\0' && !(pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COUNT))
		{
			// serial이 없고, 스택이 불가능한 아이템이라면 새로운 시리얼을 발급한다.
			pItem->m_serial = GetSerial();
		}

		pItem->setPlus_2(data->itemInfo.plus2);

		if (ch->m_inventory.addItem(pItem))
		{
			GAMELOG << init("EXPRESS SYSTEM TAKE. ITEM", ch)
					<< "CHAR INDEX : " << ch->m_index << delim
					<< "ITEM INDEX : " << pItem->getDBIndex() << delim
					<< "ITEM SERIAL : " << pItem->m_serial.c_str() << delim
					<< "ITEM COUNT : " << pItem->getItemCount() << delim
					<< "SEND TYPE : " << data->itemInfo.send_type << end;
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_FULL_INVENTORY);
			SEND_Q(rmsg, ch->m_desc);

			LOG_ERROR("EXPRESS SYSTEM TAKE. Inven is Full. charIndex : %d", ch->m_index);
			return;
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_INVALID_INFORMATION);
		SEND_Q(rmsg, ch->m_desc);

		LOG_FATAL("EXPRESS SYSTEM TAKE. item info is invalid. charIndex : %d : item_index : %d : nas : %lld", ch->m_index, data->itemInfo.item_index, data->itemInfo.nas);

		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_NO_ERROR);
	SEND_Q(rmsg, ch->m_desc);

	LOG_INFO("EXPRESS SYSTEM TAKE. TAKE SUCCESS. charIndex : %d, itemIndex : %d", ch->m_index, data->itemInfo.item_index);

	// 정상적으로 저장되었다면 DB에서 삭제
	DBManager::instance()->PushExpressDelete(ch->m_desc, data->itemInfo.index);
}

void EventProcessForDB::process_send_message_to_client( boost::any& any )
{
	sendMessageToClient* data = boost::any_cast<sendMessageToClient>(&any);

	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(data->user_index);
	if (desc == NULL)
		return;

	if (desc->m_seq_index == 0 || desc->m_seq_index != data->seq_index)
		return;

	if (STATE(desc) == CON_GET_LOGIN)
		return;

	SEND_Q(data->msg, desc);
}
