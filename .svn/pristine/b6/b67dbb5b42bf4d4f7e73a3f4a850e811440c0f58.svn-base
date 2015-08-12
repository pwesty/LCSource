#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DratanCastle.h"
#include "DBManager.h"
#include "Artifact_Manager.h"

/////////////////
// 교환 관련 함수

void do_Exchange(CPC* ch, CNetMsg::SP& msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckRespond(ch);

	// 안보이면 무시
	if (!ch->m_bVisible || ch->m_personalshop != NULL)
		return ;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_EXCHANGE_REQ:
		do_ExchangeReq(ch, msg);
		break;
	case MSG_EXCHANGE_ITEM:
		do_ExchangeItem(ch, msg);
		break;
	}
}

void do_ExchangeReq(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char subtype, reqtype;
	RefMsg(msg) >> subtype
				>> reqtype;

	switch (reqtype)
	{
	case MSG_EXCHANGE_REQ_REQ:
		do_ExchangeReqReq(ch, msg);
		break;
	case MSG_EXCHANGE_REQ_REP:
		do_ExchangeReqRep(ch, msg);
		break;
	case MSG_EXCHANGE_REQ_REJECT:
		do_ExchangeReqReject(ch, msg);
		break;
	case MSG_EXCHANGE_REQ_READY:
		do_ExchangeReqReady(ch, msg);
		break;
	case MSG_EXCHANGE_REQ_OK:
		do_ExchangeReqOK(ch, msg);
		break;
	}
}

void do_ExchangeItem(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char subtype, action;
	RefMsg(msg) >> subtype
				>> action;

	switch (action)
	{
	case MSG_EXCHANGE_ITEM_ADD:
		do_ExchangeItemAdd(ch, msg);
		break;
	case MSG_EXCHANGE_ITEM_DEL:
// 050224 : bs : 교환에서 아이템 내리기 막기
//		do_ExchangeItemDel(ch, msg);
// --- 050224 : bs : 교환에서 아이템 내리기 막기
		break;
	}
}

void do_ExchangeReqReq(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char subtype, reqtype;
	int srcIndex, destIndex;
	CLCString srcName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString destName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> subtype
				>> reqtype
				>> srcIndex >> srcName
				>> destIndex >> destName;

	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE))
		return;

	// 보낸 사람이 그 사람 맞나?
	if (ch->m_index != srcIndex)
		return ;

	// 상대방을 찾자
	CPC* dest = TO_PC(ch->m_pArea->FindCharInCell(ch, destIndex, MSG_CHAR_PC));

	if (!dest)
	{
		// 상대가 없다
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_EXCHANGE_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_exchange || dest->m_exchange)
	{
		// 이미 교환 중
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_EXCHANGE_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pZone->isRVRZone() && dest->m_pZone->isRVRZone() && ch->getSyndicateType() != dest->getSyndicateType())
	{
		// rvr 존에서는 다른 결사대와는 거래가 성립 되지 않음.
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_SYSTEM_NOT_EQ_RVRTYPE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 교환 설정
	CExchangeItems* exchange = new CExchangeItems;
	exchange->SetCharacter(ch, dest);
	ch->m_exchange = dest->m_exchange = exchange;

	GAMELOG << init("do_ExchangeReqReq()") << delim;

	if (ch)		GAMELOG << "src " << delim << ch->m_index << delim << ch->GetName() << delim;
	else		GAMELOG << "src is null" << delim;

	if (dest)	GAMELOG << "dest " << delim << dest->m_index << delim << dest->GetName() << end;
	else		GAMELOG << "dest is null" << end;

	{
		// 상대방에게 의사를 묻기
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REQ, ch, dest);
		SEND_Q(rmsg, dest->m_desc);
	}
}

void do_ExchangeReqRep(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_exchange == NULL)
		return ;

	msg->MoveFirst();

	unsigned char subtype, reqtype;

	RefMsg(msg) >> subtype
				>> reqtype;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = exc->m_src.m_ch;
	CPC* dest = exc->m_dest.m_ch;

	// 교환 상태가 요청 의사를 묻고 있나?
	if (exc->m_status != MSG_EXCHANGE_REQ_REQ)
		return ;

	exc->m_status = MSG_EXCHANGE_REQ_REP;

	{
		// 교환 시작 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REP, src, dest);
		SEND_Q(rmsg, src->m_desc);
		SEND_Q(rmsg, dest->m_desc);
	}
}

void do_ExchangeReqReject(CPC* ch, CNetMsg::SP& msg)
{
	// 교환 중?
	if (ch->m_exchange == NULL)
		return ;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = TO_PC(exc->m_src.m_ch);
	CPC* dest = TO_PC(exc->m_dest.m_ch);

	if( !src || !dest )
		return;

	// 누가 교환 취소일까?
	bool bCancelBySrc;
	if (src == ch)
		bCancelBySrc = true;
	else if (dest == ch)
		bCancelBySrc = false;
	else
		return ;

	GAMELOG << init("do_ExchangeReqReject() - after 1 ") << delim;

	if (src)	GAMELOG << "src " << delim << src->m_index << delim << src->GetName() << delim;
	else		GAMELOG << "src is null" << delim;

	if (dest)	GAMELOG << "dest " << delim << dest->m_index << delim << dest->GetName() << delim;
	else		GAMELOG << "dest is null" << delim;

	GAMELOG << "bCancelBySrc" << bCancelBySrc << end;

	msg->MoveFirst();

	unsigned char subtype, reqtype;

	RefMsg(msg) >> subtype
				>> reqtype;

	// 교환 취소
	delete exc;
	exc = NULL;

	src->m_exchange = dest->m_exchange = NULL;

	if (dest && !(dest->GetName()))
	{
		GAMELOG << init("do_ExchangeReqReject() - after 2 ") << delim << "dest name is null" << end;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, ((bCancelBySrc) ? MSG_EXCHANGE_REQ_REJECT_SRC : MSG_EXCHANGE_REQ_REJECT_DEST), src, dest);
		SEND_Q(rmsg, src->m_desc);
		SEND_Q(rmsg, dest->m_desc);
	}
}

void do_ExchangeReqReady(CPC* ch, CNetMsg::SP& msg)
{
	// 교환 중?
	if (ch->m_exchange == NULL)
		return ;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = exc->m_src.m_ch;
	CPC* dest = exc->m_dest.m_ch;

	// 누가 준비 완료일까?
	bool bReadyBySrc;
	if (src == ch)
		bReadyBySrc = true;
	else if (dest == ch)
		bReadyBySrc = false;
	else
		return ;

	msg->MoveFirst();

	unsigned char subtype, reqtype;

	RefMsg(msg) >> subtype
				>> reqtype;

	if (exc->m_status != MSG_EXCHANGE_REQ_REP && exc->m_status != MSG_EXCHANGE_REQ_READY_SRC && exc->m_status != MSG_EXCHANGE_REQ_READY_DEST)
		return ;

	CNetMsg::SP retmsg(new CNetMsg);
	bool isSuccess = false;

	switch (exc->CanExchange())
	{
	case 0:		// 가능
		isSuccess = true;
		break;

	case 1:		// src 실패
		ExchangeReqMsg(retmsg, MSG_EXCHANGE_REQ_FULL_SRC, src, dest);
		exc->m_status = MSG_EXCHANGE_REQ_REP;
		goto SEND_AND_EXIT;

	case 2:		// dest 실패
		ExchangeReqMsg(retmsg, MSG_EXCHANGE_REQ_FULL_DEST, src, dest);
		exc->m_status = MSG_EXCHANGE_REQ_REP;
		goto SEND_AND_EXIT;
	case 3:		// dest 실패
		ExchangeReqMsg(retmsg, MSG_EXCHANGE_REQ_SRC_PET_ANYMORE, src, dest);
		exc->m_status = MSG_EXCHANGE_REQ_REP;
		goto SEND_AND_EXIT;
	case 4:		// dest 실패
		ExchangeReqMsg(retmsg, MSG_EXCHANGE_REQ_DEST_PET_ANYMORE, src, dest);
		exc->m_status = MSG_EXCHANGE_REQ_REP;
		goto SEND_AND_EXIT;
	}

	// 지금 상태에 따라서
	if (exc->m_status == MSG_EXCHANGE_REQ_REP)
	{
		// 아무도 준비 완료 안했다
		// 성공이면 1단계 준비 완료 상태로 설정
		exc->m_status = (bReadyBySrc) ? MSG_EXCHANGE_REQ_READY_SRC : MSG_EXCHANGE_REQ_READY_DEST;
		// 그 결과를 알리기
		ExchangeReqMsg(retmsg, exc->m_status, src, dest);
	}

	else if ( (exc->m_status == MSG_EXCHANGE_REQ_READY_SRC && !bReadyBySrc)
			  || (exc->m_status == MSG_EXCHANGE_REQ_READY_DEST && bReadyBySrc) )
	{
		// 둘 다 준비 완료를 했다
		// 그러면 2단계 준비 완료 상태로 전환
		exc->m_status = MSG_EXCHANGE_REQ_READY;
		// 그 결과를 알리기
		ExchangeReqMsg(retmsg, exc->m_status, src, dest);
	}
	else
	{
		// 이 경우 아니면 준비완료 메시지가 오면 안된다
		return ;
	}

SEND_AND_EXIT:
	SEND_Q(retmsg, src->m_desc);
	SEND_Q(retmsg, dest->m_desc);
	if(isSuccess == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT, NULL, NULL);
		if(src->m_exchange) do_Exchange(src, rmsg);
		if(dest->m_exchange) do_Exchange(src, rmsg);
	}
}

void do_ExchangeReqOK(CPC* ch, CNetMsg::SP& msg)
{
	// 교환 중?
	if (ch->m_exchange == NULL)
		return ;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = exc->m_src.m_ch;
	CPC* dest = exc->m_dest.m_ch;

	// 누가 확인일까?
	bool bOKBySrc;
	if (src == ch)
		bOKBySrc = true;
	else if (dest == ch)
		bOKBySrc = false;
	else
		return ;

	msg->MoveFirst();

	unsigned char subtype, reqtype;

	RefMsg(msg) >> subtype
				>> reqtype;

	// 지금 상태에 따라서
	if (exc->m_status == MSG_EXCHANGE_REQ_READY)
	{
		// 아무도 확인 안했다
		// 원인 제공자 1단계 확인 상태로
		exc->m_status = (bOKBySrc) ? MSG_EXCHANGE_REQ_OK_SRC : MSG_EXCHANGE_REQ_OK_DEST;
		// 그 결과를 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, exc->m_status, src, dest);
		SEND_Q(rmsg, src->m_desc);
		SEND_Q(rmsg, dest->m_desc);
	}

	else if ( (exc->m_status == MSG_EXCHANGE_REQ_OK_SRC && !bOKBySrc)
			  || (exc->m_status == MSG_EXCHANGE_REQ_OK_DEST && bOKBySrc) )
	{
		// 교환 가능 검사
		switch (exc->CanExchange())
		{
		case 0:		// 교환 가능
			{
				// 교환 하기
				// 교환 함수 안에서 변경 메시지들 보냄
				exc->DoExchange();

				// 교환 성공
				CNetMsg::SP rmsg(new CNetMsg);
				ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_OK, src, dest);
				SEND_Q(rmsg, src->m_desc);
				SEND_Q(rmsg, dest->m_desc);

				// 실시간 저장
				DBManager::instance()->SaveCharacterInfo(src->m_desc, false);
				DBManager::instance()->SaveCharacterInfo(dest->m_desc, false);
			}
			break;

		case 1:		// src의 교환 불가
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT_SRC, src, dest);
				SEND_Q(rmsg, src->m_desc);
				SEND_Q(rmsg, dest->m_desc);
			}
			break;

		case 2:		// dest의 교환 불가
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT_DEST, src, dest);
				SEND_Q(rmsg, src->m_desc);
				SEND_Q(rmsg, dest->m_desc);
			}
			break;
		}

		// 교환 리셋
		delete exc;
		exc = NULL;

		src->m_exchange = dest->m_exchange = NULL;
	}

	return ;
}

void do_ExchangeItemAdd(CPC* ch, CNetMsg::SP& msg)
{
	// 교환 중인가?
	if (ch->m_exchange == NULL)
		return ;

	CPC* his;
	bool bSrc;
	// 내가 누구인가?
	if (ch->m_exchange->m_src.m_ch == ch)
	{
		his = ch->m_exchange->m_dest.m_ch;
		bSrc = true;
	}
	else if (ch->m_exchange->m_dest.m_ch == ch)
	{
		his = ch->m_exchange->m_src.m_ch;
		bSrc = false;
	}
	else
		return ;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = exc->m_src.m_ch;
	CPC* dest = exc->m_dest.m_ch;

	// 아이템 가능 상태가 아니면 무시
	switch (exc->m_status)
	{
	case MSG_EXCHANGE_REQ_READY_SRC:
		// 내가 src이고 준비 상태
		if (bSrc)
			return ;
		break;

	case MSG_EXCHANGE_REQ_READY_DEST:
		// 내가 dest이고 준비 상태
		if (!bSrc)
			return ;
		break;

	case MSG_EXCHANGE_REQ_REP:
		break;

	default:
		return ;
	}

	msg->MoveFirst();

	unsigned char subtype, action;
	int item_idx;
	LONGLONG count;
	CItem* item;

	RefMsg(msg) >> subtype
				>> action
				>> item_idx
				>> count;

	if (item_idx <= 0)
	{
		LOG_ERROR("do_ExchangeItemAdd : invalid item_idx[%d]", item_idx);
		return;
	}

	if (count <= 0)
	{
		LOG_ERROR("do_ExchangeItemAdd : invalid count[%d]", count);
		return;
	}

	if(item_idx != NAS_ITEM_DB_INDEX)
	{
		// 같은 아이템이 이미 추가되어 있는지 검사
		int exidx = exc->Find(ch, item_idx);
		LONGLONG preCount = 0;
		if (exidx != -1)
			preCount = (bSrc) ? exc->m_src.m_count[exidx] : exc->m_dest.m_count[exidx];

		// 더이상 등록 불가?
		if (exidx == -1 && ((bSrc && exc->m_src.m_regCount >= MAX_EXCHANGE_ITEMS) || (!bSrc && exc->m_dest.m_regCount >= MAX_EXCHANGE_ITEMS)))
			return ;

		// 인벤에 있고, 개수가 여유가 되고, 교환 가능하고, 미착용인가?
		// 060406 : bs : 봉인된 아이템 교환 불가
		CItem* item = ch->m_inventory.FindByVirtualIndex(item_idx);

		if(item->IsBelong() == true)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if (item == NULL || item->CanExchange() == false)
			return;

		if (((exidx == -1 && item->Count() < count) || (exidx != -1 && item->Count() < count + preCount)))
			return ;

#ifdef ENABLE_SUBJOB
		if( item && item->CanUseTrader( TRADE_EXCHANGE, ch->IsSubJob(JOB_SUB_TRADER) ) == false )
		{
			return;
		}
#endif //ENABLE_SUBJOB

		if( item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT )
		{
			if(dest == NULL || src == NULL)
				return ;

			int error_ = ArtifactManager::instance()->checkValidUser(dest);
			if(error_ != 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, (MSG_SYS_TYPE)error_);
				SEND_Q(rmsg, src->m_desc);
				return ;
			}

			error_ = ArtifactManager::instance()->checkValidUser(src);
			if(error_ != 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, (MSG_SYS_TYPE)error_);
				SEND_Q(rmsg, dest->m_desc);
				return ;
			}
		}

		// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
		CPet* pet = NULL;
		if (item->IsPet())
		{
			pet = ch->GetPet(item->getPlus());
			if (!pet || pet->GetSkillList()->count() != 0)
				return ;
		}
		if( item->IsAPet() )
			{
			CAPet* apet = ch->GetAPet(item->getPlus());
			if( apet && apet->IsPetEquip() )
				return ;
		}

		bool bUpdate;
		int idx = exc->Add(ch, item_idx, count, &bUpdate);
		if (idx == -1)
		{
			return ;
		}

		if (!bUpdate)
		{
			// 펫이면 먼저 펫정보 보내기
			if (pet)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPetSellInfoMsg(rmsg, pet);
				if (bSrc)
				{
					SEND_Q(rmsg, dest->m_desc);
				}
				else
				{
					SEND_Q(rmsg, src->m_desc);
				}
			}

			CNetMsg::SP rmsg(new CNetMsg);
			ExchangeItemMsg(rmsg, (bSrc) ? MSG_EXCHANGE_ITEM_ADD_SRC : MSG_EXCHANGE_ITEM_ADD_DEST, item, count, (bSrc) ? exc->m_src.m_nasCount : exc->m_dest.m_nasCount);
			SEND_Q(rmsg, src->m_desc);
			SEND_Q(rmsg, dest->m_desc);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);

			if (bSrc)
				ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_SRC, item, exc->m_src.m_count[idx], exc->m_src.m_nasCount);
			else
				ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_DEST, item, exc->m_dest.m_count[idx], exc->m_dest.m_nasCount);

			SEND_Q(rmsg, src->m_desc);
			SEND_Q(rmsg, dest->m_desc);
		}
	}
	else
	{
		item = gserver->m_itemProtoList.CreateItem(item_idx, WEARING_NONE, 0, 0, count);
		//보낸 패킷이 나스일때
		if(bSrc)
		{
			if( src->m_inventory.getMoney() < count )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_NOT_ENOUGH_MONEY);			//돈이 부족함
				SEND_Q(rmsg, src->m_desc);
			}
			else
				exc->m_src.m_nasCount = count;

			CNetMsg::SP rmsg(new CNetMsg);
			ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_SRC, item, 0, exc->m_src.m_nasCount);
			SEND_Q(rmsg, src->m_desc);
			SEND_Q(rmsg, dest->m_desc);
		}
		else
		{
			if( dest->m_inventory.getMoney() < count )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_NOT_ENOUGH_MONEY);			//돈이 부족함
				SEND_Q(rmsg, dest->m_desc);
			}
			else
				exc->m_dest.m_nasCount = count;

			CNetMsg::SP rmsg(new CNetMsg);
			ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_DEST, item, 0, exc->m_dest.m_nasCount);
			SEND_Q(rmsg, src->m_desc);
			SEND_Q(rmsg, dest->m_desc);
		}
	}
}

void do_ExchangeItemDel(CPC* ch, CNetMsg::SP& msg)
{
	// 교환 중인가?
	if (ch->m_exchange == NULL)
		return ;

	CPC* his;
	bool bSrc;
	// 내가 누구인가?
	if (ch->m_exchange->m_src.m_ch == ch)
	{
		his = ch->m_exchange->m_dest.m_ch;
		bSrc = true;
	}
	else if (ch->m_exchange->m_dest.m_ch == ch)
	{
		his = ch->m_exchange->m_src.m_ch;
		bSrc = false;
	}
	else
		return ;

	CExchangeItems* exc = ch->m_exchange;
	CPC* src = exc->m_src.m_ch;
	CPC* dest = exc->m_dest.m_ch;

	// 아이템 가능 상태가 아니면 무시
	switch (exc->m_status)
	{
	case MSG_EXCHANGE_REQ_READY_SRC:
		// 내가 src이고 준비 상태
		if (bSrc)
			return ;
		break;

	case MSG_EXCHANGE_REQ_READY_DEST:
		// 내가 dest이고 준비 상태
		if (!bSrc)
			return ;
		break;

	case MSG_EXCHANGE_REQ_REP:
		break;

	default:
		return ;
	}

	msg->MoveFirst();

	unsigned char subtype, action;
	int item_idx;
	LONGLONG count;
	GoldType_t nasCount;

	RefMsg(msg) >> subtype
				>> action
				>> item_idx
				>> count
				>> nasCount;

	// 추가한 아이템인가?
	int exidx = exc->Find(ch, item_idx);
	if (exidx == -1)
		return ;

	CItem* item = NULL;
	if (bSrc)
		item = exc->m_src.m_ch->m_inventory.FindByVirtualIndex(exc->m_src.m_itemindex[exidx]);
	else
		item = exc->m_dest.m_ch->m_inventory.FindByVirtualIndex(exc->m_dest.m_itemindex[exidx]);

	if (item == NULL)
		return ;

	// 뺄 만큼 여유가 있나?
	LONGLONG preCount = (bSrc) ? exc->m_src.m_count[exidx] : exc->m_dest.m_count[exidx];
	if (preCount < count)
		return ;

	// 빼고
	int idx = exc->Del(ch, item_idx, count);

	// 나서 남은게 있나?
	if (idx == -1)
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if ((bSrc && exc->m_src.m_count[idx] == 0) || (!bSrc && exc->m_dest.m_count[idx] == 0))
		{
			// 남은 거 없음
			ExchangeItemMsg(rmsg, (bSrc) ? MSG_EXCHANGE_ITEM_DEL_SRC : MSG_EXCHANGE_ITEM_DEL_DEST, item, 0, nasCount);
		}
		else
		{
			// 남은 거 있음
			if (bSrc)
				ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_SRC, item, exc->m_src.m_count[idx], nasCount);
			else
				ExchangeItemMsg(rmsg, MSG_EXCHANGE_ITEM_UPDATE_DEST, item, exc->m_dest.m_count[idx], nasCount);
		}

		SEND_Q(rmsg, src->m_desc);
		SEND_Q(rmsg, dest->m_desc);
	}
}
//