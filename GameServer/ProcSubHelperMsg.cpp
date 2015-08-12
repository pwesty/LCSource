#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/DBCmd.h"
//#include "RewardEventItem.h"
#include "DBManager.h"
#include "../ShareLib/packetType/ptype_express_system.h"
#include "../ShareLib/packetType/ptype_tradeAgent_system.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"

// Definietion
void ProcSubHelperTradeAgentErrorRep( CNetMsg::SP& msg );
void ProcSubHelperTradeAgentRegListRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentRegRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentRegCancelRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentSearchRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentBuyRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentLikeListRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentLikeRegRep( CNetMsg::SP& msg);
void ProcSubHelperTradeAgentLikeCancelRep( CNetMsg::SP& msg);
void ProcSubHelperTitleSystemInsertTitleRep(CNetMsg::SP& msg);
void ProcSubHelperTitleSystemDisableRep(CNetMsg::SP& msg);
void ProcSubHelperTitleSystemDeleteRep(CNetMsg::SP& msg);
// [selo: 101105] 종료 시간 강제 변경
void ProcSubHelperTitleSystemForceChangeTime(CNetMsg::SP& msg);

void ProcSubHelperLCBallinfo( CNetMsg::SP& msg);
void ProcSubHelperLCBalluse( CNetMsg::SP& msg);
void ProcSubHelperLCBall_sucess( CNetMsg::SP& msg);

void ProcSubHelperRakingList(CNetMsg::SP& msg);
void ProcSubHelperRankingSearch(CNetMsg::SP& msg);
void ProcSubHelperRankingRefresh(CNetMsg::SP& msg);
void ProcSubHelperRankingError(CNetMsg::SP& msg);

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
void ProcSubHelperPKPenaltyReformRep(CNetMsg::SP& msg);
void ProcSubHelperPKPenaltyReformRewardInfoRep(CPC* pc, CNetMsg::SP& msg);
void ProcSubHelperPKPenaltyReformRewardRep(CPC* pc, CNetMsg::SP& msg);
#endif

#ifdef NOTICE_SYSTEM
void ProcSubHelperNoticeSystem(CNetMsg::SP& msg);
void ProcSubHelperNoticeSystemRegRep(CNetMsg::SP& msg);
void ProcSubHelperUserNoticePrint(CNetMsg::SP& msg);
#endif // NOTICE_SYSTEM

void ProcSubHelperExtCharSlotItemUseRep(CNetMsg::SP& msg);

extern void ProcSubHelperReservedGMCommand(CNetMsg::SP& msg);

extern float g_TradeAgent_Fee;
extern int g_TradeAgent_Deposit;

// Command
void CServer::ProcessSubHelperMessage(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	switch (msg->m_mtype)
	{
	case MSG_TRADEAGENT:
		{
			ResponseClient::TradeAgentSystemList* packet = reinterpret_cast<ResponseClient::TradeAgentSystemList*>(msg->m_buf);
			
			int subtype = packet->subType;			
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			
			switch(packet->subType)
			{
			case MSG_TRADEAGENT_LIST:
				ProcSubHelperTradeAgentSearchRep(msg);
				break;
			case MSG_TRADEAGENT_REG_LIST:
				ProcSubHelperTradeAgentRegListRep(msg);
				break;
			case MSG_TRADEAGENT_REG:
				ProcSubHelperTradeAgentRegRep(msg);
				break;
			case MSG_TRADEAGENT_REG_CANCEL:
				ProcSubHelperTradeAgentRegCancelRep(msg);
				break;
			case MSG_TRADEAGENT_BUY:
				ProcSubHelperTradeAgentBuyRep(msg);
				break;
			case MSG_TRADEAGENT_LIKE_LIST:
				ProcSubHelperTradeAgentLikeListRep(msg);
				break;
			case MSG_TRADEAGENT_LIKE_REG:
				ProcSubHelperTradeAgentLikeRegRep(msg);
				break;
			case MSG_TRADEAGENT_LIKE_CANCEL:
				ProcSubHelperTradeAgentLikeCancelRep(msg);
				break;
			case MSG_TRADEAGENT_ERROR:
				ProcSubHelperTradeAgentErrorRep(msg);
				break;
			case MSG_TRADEAGENT_AFTER_SELL:
				{
					ResponseClient::TradeAgentAfterSell* packet = reinterpret_cast<ResponseClient::TradeAgentAfterSell*>(msg->m_buf);
					CPC* ch = PCManager::instance()->getPlayerByCharIndex(packet->charIndex);
					if (ch)
					{
						SEND_Q(msg, ch->m_desc);
					}
				}
				break;
			case MSG_TRADEAGENT_AFTER_RETURNED:
				{
					ResponseClient::TradeAgentAfterReturned* packet = reinterpret_cast<ResponseClient::TradeAgentAfterReturned*>(msg->m_buf);
					CPC* ch = PCManager::instance()->getPlayerByCharIndex(packet->charIndex);
					if (ch)
					{
						SEND_Q(msg, ch->m_desc);
					}
				}
				break;
			case MSG_TRADEAGENT_CHANGE_FEE:
				{
					ResponseClient::TradeAgentChangeFee* packet = reinterpret_cast<ResponseClient::TradeAgentChangeFee*>(msg->m_buf);
					g_TradeAgent_Fee = packet->fee;

					PCManager::instance()->sendToAll(msg);
				}
				break;
			case MSG_TRADEAGENT_CHANGE_DEPOSIT:
				{
					ResponseClient::TradeAgentChangeDeposit* packet = reinterpret_cast<ResponseClient::TradeAgentChangeDeposit*>(msg->m_buf);
					g_TradeAgent_Deposit = packet->deposit;

					PCManager::instance()->sendToAll(msg);
				}
				break;
			case MSG_TRADEAGENT_RETURN_NAS:
				{
					ResponseClient::TradeAgentReturnMoney* packet = reinterpret_cast<ResponseClient::TradeAgentReturnMoney*>(msg->m_buf);
					CPC* pc = PCManager::instance()->getPlayerByCharIndex(packet->charIndex);
					if(pc == NULL)
					{
						std::string tstr = boost::str(boost::format(
														  "TradeAgent Return Nas - Not Found char : charIndex : %1% : nas : %2%") % packet->charIndex % packet->nas);
						LOG_INFO(tstr.c_str());
					}
					else
					{
						pc->m_inventory.increaseMoney(packet->nas);

						std::string tstr = boost::str(boost::format(
														  "TradeAgent Return Nas - OK : charIndex : %1% : nas : %2%") % packet->charIndex % packet->nas);
						LOG_INFO(tstr.c_str());
					}
				}
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_RESERVED_GM_COMMAND:
		//LOG_INFO("DEBUG_FUNC : START : type : %d", msg->m_mtype);
		ProcSubHelperReservedGMCommand(msg);
		//LOG_INFO("DEBUG_FUNC : END : type : %d", msg->m_mtype);
		break;

	case MSG_EXPRESS_SYSTEM:
		{
			pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
			
			int subtype = pBase->subType;
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			
			switch (pBase->subType)
			{
			case MSG_SUB_EXPRESS_ONE_USER:
				{
					RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(msg->m_buf);
					CPC* ch = PCManager::instance()->getPlayerByCharIndex(packet->char_index);
					if (ch == NULL)
						return;

					DBManager::instance()->PushExpressExist(ch->m_desc, packet->send_type);
				}
				break;

			default:
				LOG_ERROR("Express system : invalid subtype[%d]", pBase->subType);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_SUBHELPER_COMMAND :
		{
			int subtype;
			RefMsg(msg) >> subtype;
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_SUBHELPER_TITLESYSTEM_INSERT_TITLE_REP:
				ProcSubHelperTitleSystemInsertTitleRep(msg);
				break;
			case MSG_SUBHELPER_TITLESYSTEM_CHECK_EXPIRE_TITLE_REP:
				ProcSubHelperTitleSystemDisableRep(msg); // 호칭 리스트 열때 시간 확인 후 disable함.
				break;
			case MSG_SUBHELPER_TITLE_SYSTEM_TITLE_DELETE_REP:
				ProcSubHelperTitleSystemDeleteRep(msg); // 사용자가 수동삭제를 할때 disable함.
				break;
			case MSG_SUBHELPER_TITLE_SYSTEM_FORCE_CHANGE_TIME:
				ProcSubHelperTitleSystemForceChangeTime(msg);	// [selo: 101105] 종료 시간 강제 변경
				break;
			case MSG_SUBHELPER_LCBALL_INFO:
				ProcSubHelperLCBallinfo( msg );
				break;
			case MSG_SUBHELPER_LCBALL_USE:
				ProcSubHelperLCBalluse( msg );
				break;
			case MSG_SUBHELPER_LCBALL_USE_SUCCESS:
				ProcSubHelperLCBall_sucess( msg );
				break;
			case MSG_SUBHELPER_RANKING_EX_LIST:
				ProcSubHelperRakingList(msg);
				break;
			case MSG_SUBHELPER_RANKING_EX_SEARCH:
				ProcSubHelperRankingSearch(msg);
				break;
			case MSG_SUBHELPER_RANKING_EX_REFRESH:
				ProcSubHelperRankingRefresh(msg);
				break;
			case MSG_SUBHELPER_RANKING_EX_ERROR:
				ProcSubHelperRankingError(msg);
				break;
#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
			case MSG_SUBHELPER_PKPENALTY_REFORM:
				ProcSubHelperPKPenaltyReformRep(msg);
				break;
#endif
			case MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_REP:
				ProcSubHelperExtCharSlotItemUseRep(msg);
				break;
			default:
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;
#ifdef NOTICE_SYSTEM
	case MSG_SUBHELPER_NOTICE:
		{
			//LOG_INFO("DEBUG_FUNC : START : type : %d", msg->m_mtype);
			ProcSubHelperNoticeSystem(msg);
			//LOG_INFO("DEBUG_FUNC : END : type : %d", msg->m_mtype);
		}
		break;
#endif // NOTICE_SYSTEM

	default:
		break;
	}
}

// Description
//에러 메세지
void ProcSubHelperTradeAgentErrorRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemError *p = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);
	if( pChar == NULL)
		return;

	SEND_Q(msg, pChar->m_desc);
}

void ProcSubHelperTradeAgentRegListRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemRegList *p = reinterpret_cast<ResponseClient::TradeAgentSystemRegList*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);

	if(pChar == NULL)
		return;

	pChar->m_tradeAgentRegCount = p->count;	//현재 등록한 개수를 저장해 놓고 물건을 맡길시에 선 처리 검사하기 위해서

	SEND_Q(msg, pChar->m_desc);
}

//등록
void ProcSubHelperTradeAgentRegRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemRegWithServer *sp = reinterpret_cast<ResponseClient::TradeAgentSystemRegWithServer*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(sp->charIndex);

	if( pChar == NULL)
	{
		GAMELOG << init("TRADE AGENT REGIST") << delim
				<< "not found user. char index" << delim
				<< sp->charIndex << end;
		return;
	}

	GAMELOG << init("TRADE AGENT REGIST",  pChar)
			<< " CHAR INDEX : "	<< pChar->m_index << delim
			<< " ITEM INDEX : "  << sp->itemIndex << delim
			<< " ITEM SERIAL: "  << sp->serial << delim
			<< " ITEM COUNT : "  << sp->quantity << delim
			<< " TOTAL MONEY: "  << sp->TotalMoney << delim
			<< " GUARANTY:"  << sp->Guarantee << end;

	pChar->m_bTradeAgentRegIng = false;

	//거래대행 패킷 전송
	msg->setSize(sizeof(ResponseClient::TradeAgentSystemReg));
	SEND_Q(msg, pChar->m_desc);
}

//등록 취소
void ProcSubHelperTradeAgentRegCancelRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemRegCancelWithServer *sp = reinterpret_cast<ResponseClient::TradeAgentSystemRegCancelWithServer*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(sp->charIndex);

	if(pChar == NULL)
	{
		GAMELOG << init("TRADE AGENT REGIST CANCEL") << delim
				<< "not found user. char index" << delim
				<< sp->charIndex << end;
		return;
	}

	GAMELOG << init("TRADE AGENT REGIST CANCEL", pChar )
			<< " CHAR INDEX: " << pChar->m_index << delim
			<< "ITEM INDEX : " << sp->itemIndex << delim
			<< "ITEM SERIAL: " << sp->serial << delim
			<< "ITEM COUNT : " << sp->itemCount
			<< end;

	msg->setSize(sizeof(ResponseClient::TradeAgentSystemRegCancel));
	SEND_Q(msg, pChar->m_desc);
}

//조회
void ProcSubHelperTradeAgentSearchRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemList* packet = reinterpret_cast<ResponseClient::TradeAgentSystemList*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(packet->charIndex);
	if (pChar == NULL)
		return;

	//캐릭터의 맵 리스트에 저장하기.
	pChar->m_tradeAgentViewMap.clear();
	for( int i = 0; i< packet->count; i++)
	{
		pChar->m_tradeAgentViewMap.insert(std::pair<int, GoldType_t>(packet->list[i].index, packet->list[i].nas));
	}

	//정보 저장 다 하고나서 받아온 패킷 그대로 전송
	SEND_Q(msg, pChar->m_desc);
}

//구매
void ProcSubHelperTradeAgentBuyRep( CNetMsg::SP& msg )
{
	ResponseClient::TradeAgentSystemBuyWithServer* sp = reinterpret_cast<ResponseClient::TradeAgentSystemBuyWithServer*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(sp->charIndex);

	if (pChar == NULL)
	{
		GAMELOG << init("TRADE AGENT ITEM BUY") << delim
				<< "not found user. char index" << delim
				<< sp->charIndex << delim
				<< "Need Decrease Money" << delim
				<< sp->needNas << end;

		std::string query = boost::str(boost::format("update t_characters set a_nas = a_nas - %1% where a_index = %2%") % sp->needNas % sp->charIndex);
		DBManager::instance()->pushQuery(0, query);
		return;
	}

	pChar->m_inventory.sendMoneyInfo();

	GAMELOG << init("TRADE AGENT ITEM BUY", pChar )
			<< " CHAR INDEX: " << pChar->m_index << delim
			<< " ITEM_INDEX : " << sp->itemIndex << delim
			<< " ITEM_SERIAL : " << sp->itemSerial << delim
			<< " ITEM_COUNT : " << sp->itemCount << delim
			<< " ITEM_PRICE : " << sp->itemPrice << delim
			<< " SELLER : " << sp->sellerName
			<< end;

	msg->setSize(sizeof(ResponseClient::TradeAgentSystemBuy));
	SEND_Q(msg, pChar->m_desc);
}

void ProcSubHelperTradeAgentLikeListRep( CNetMsg::SP& msg)
{
	ResponseClient::TradeAgentSystemLikeList *p = reinterpret_cast<ResponseClient::TradeAgentSystemLikeList*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);

	if(pChar == NULL)
		return;

	//캐릭터의 맵 리스트에 저장하기.
	pChar->m_tradeAgentViewMap2.clear();
	for( int i = 0; i< p->count; i++)
	{
		pChar->m_tradeAgentViewMap2.insert(std::pair<int, GoldType_t>(p->list[i].index, p->list[i].nas));
	}

	SEND_Q(msg, pChar->m_desc);
}
void ProcSubHelperTradeAgentLikeRegRep( CNetMsg::SP& msg)
{
	ResponseClient::TradeAgentSystemLikeReg *p = reinterpret_cast<ResponseClient::TradeAgentSystemLikeReg*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);

	if(pChar == NULL)
		return;

	SEND_Q(msg, pChar->m_desc);
}
void ProcSubHelperTradeAgentLikeCancelRep( CNetMsg::SP& msg)
{
	ResponseClient::TradeAgentSystemLikeCancel *p = reinterpret_cast<ResponseClient::TradeAgentSystemLikeCancel*>(msg->m_buf);
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);

	if(pChar == NULL)
		return;

	SEND_Q(msg, pChar->m_desc);
}

void ProcSubHelperTitleSystemInsertTitleRep(CNetMsg::SP& msg)
{
	int char_index = 0;
	int title_index = 0;
	int endtime = -1;
	int errortype = 0;
	int tab, invenIndex;
	CPC* ch;
	CTitle* title;
	CItem* item;
	CLCString serial;

	RefMsg(msg) >> char_index >> title_index >> endtime >> errortype >> tab >> invenIndex >> serial;

	if( errortype == 0 ) // DB insert error
		return ;

	else if(errortype == 1)
	{
		ch = PCManager::instance()->getPlayerByCharIndex(char_index);
		if(ch == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}
		title = gserver->m_titleProtoList.Create(title_index, endtime);

		if(title == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}
		item = ch->m_inventory.getItem(tab, invenIndex);
		if(item == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}

		if(item->Count() < 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}

		if (item->m_serial != (const char *)serial)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}

		if(ch->m_titleList.m_nCount >= MAX_TITLE_COUNT)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemTitleDelReq(rmsg, char_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);

			return ;
		}

		if(ch->m_titleList.Add(title))
		{
			ch->m_inventory.deleteItemByItem(item);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				TitleItemUseMsg(rmsg, title->m_proto->m_index, title->m_endtime);
				SEND_Q(rmsg, ch->m_desc);
			}

			GAMELOG << init("TITLE ITEM USE", ch)
					<< "ITEM INDEX" << delim
					<< item->getDBIndex() << end;
		}
	}
	// [selo: 101109] 서버에서 강제 추가
	else if( errortype == 2 )
	{
		ch = PCManager::instance()->getPlayerByCharIndex(char_index);
		if(ch == NULL)
			return ;
		title = gserver->m_titleProtoList.Create(title_index, endtime);

		if(title == NULL)
			return ;

		if(ch->m_titleList.Add(title))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleItemUseMsg(rmsg, title->m_proto->m_index, title->m_endtime);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("TITLE FORCE INSERT", ch)
					<< "TITLE INDEX" << delim
					<< title->m_proto->m_index << end;
		}
	}
}

void ProcSubHelperTitleSystemDisableRep(CNetMsg::SP& msg)
{
	int char_index = 0;
	int disable_count = 0;
	CPC* ch = NULL;
	RefMsg(msg) >> char_index;
	RefMsg(msg) >> disable_count;

	ch = PCManager::instance()->getPlayerByCharIndex(char_index);
	if(!ch)
		return ;

	if( disable_count == 0 )
	{
		// LIST 전송
		CTitleNode* temp;
		temp = ch->m_titleList.m_head;
		if(!temp)
			return ;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleSystemListRep(rmsg, MSG_EX_TITLE_SYSTEM_LIST_REP_EXIST_LIST, ch->m_titleList.m_nCount);
			while(temp)
			{
				RefMsg(rmsg) << temp->m_title->m_proto->m_index
							 << temp->m_title->m_endtime;
				temp = temp->m_next;
			}
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		int title_index = 0;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_TITLE_EXPIRED);
			RefMsg(rmsg) << disable_count;
			for(int i = 0; i < disable_count; i++)
			{
				RefMsg(msg) >> title_index;
				RefMsg(rmsg) << title_index;
				ch->m_titleList.Remove(ch->m_titleList.Find(title_index));
			}
			SEND_Q(rmsg, ch->m_desc);
		}

		if(ch->m_titleList.m_nCount == 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleSystemListRep(rmsg, MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST, ch->m_titleList.m_nCount);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			CTitleNode* temp;
			temp = ch->m_titleList.m_head;
			if(!temp)
				return ;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				TitleSystemListRep(rmsg, MSG_EX_TITLE_SYSTEM_LIST_REP_EXIST_LIST, ch->m_titleList.m_nCount);
				while(temp)
				{
					RefMsg(rmsg) << temp->m_title->m_proto->m_index
								 << temp->m_title->m_endtime;
					temp = temp->m_next;
				}
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void ProcSubHelperTitleSystemDeleteRep(CNetMsg::SP& msg)
{
	int char_index;
	int title_index;
	int errortype;
	CPC* ch;
	CTitle* title;
	bool bAppear = false;

	RefMsg(msg) >> char_index >> title_index >> errortype;

	ch = PCManager::instance()->getPlayerByCharIndex(char_index);
	if(!ch)
		return ;

	if((ch->m_nCurrentTitle == title_index) && errortype)
	{
		ch->m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;
		bAppear = true;
	}

	title = ch->m_titleList.Find(title_index);
	if(!title)
		return ;

	if(ch->m_titleList.Remove(title) && errortype)
	{
		GAMELOG << init("TITLE REMOVE TO LIST", ch) << delim
				<< "TITLE LIST" << delim << title_index << end;
		ch->CalcStatus(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_DELETE_SUCCESS, title_index, ch->m_index);
			if(bAppear)
			{
				ch->m_pArea->SendToCell(rmsg, ch);
			}
			SEND_Q(rmsg, ch->m_desc);
		}

		GAMELOG << init("TITLE DELETE SUCCESS(user delete)", ch) << delim
				<< "TITLE INDEX" << delim << title_index << end;
	}
	else if(!errortype)
	{
		GAMELOG << init("TITLE CANT REMOVE TO LIST", ch) << delim
				<< "TITLE LIST" << delim << title_index << end;
	}
}

// [selo: 101105] 종료 시간 강제 변경
void ProcSubHelperTitleSystemForceChangeTime(CNetMsg::SP& msg)
{
	int char_index;
	int title_index;
	int endtime;
	CPC* ch;
	CTitle* title;

	RefMsg(msg) >> char_index >> title_index >> endtime;

	ch = PCManager::instance()->getPlayerByCharIndex(char_index);
	if( !ch )
		return;

	title = ch->m_titleList.Find(title_index);
	if(!title)
		return;

	title->m_endtime = endtime;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSysstemForceChangeTimeMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_CHANGE_TIME, title_index, endtime);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("TITLE FORCE CHANGE TIME SUCCESS", ch) << delim
			<< "TITLE INDEX" << delim << title_index
			<< "END TIME" << delim << endtime << end;
}

void ProcSubHelperLCBallinfo( CNetMsg::SP& msg)
{
	// 클라 메세지 만들어서 보내기
	int charIndex=0 , curCoinInex = 0;
	char coinCount=0, courseCount=0, itemCount=0, curCourseIndex = 0;

	RefMsg(msg) >> charIndex ;

	// PC 찾기
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init( MSG_EXTEND );
	RefMsg(rmsg) << MSG_EX_LCBALL
				 << (char)MSG_EX_LCBALL_INFO;

	int idx=0;
	// 토큰 모든 정보
	RefMsg(msg) >> coinCount;
	RefMsg(rmsg) << coinCount;
	for( idx=0; idx < coinCount; idx++ )
	{
		int coinIndex = 0;
		RefMsg(msg) >> coinIndex;
		RefMsg(rmsg) << coinIndex;
	}
	// 코스 전체 새부 정보
	int nItem=0, nRemain = 0 , nMax =0;
	unsigned int nItemCount = 0;

	RefMsg(msg) >> courseCount;
	RefMsg(rmsg) << courseCount;
	for( idx=0; idx < courseCount; idx++ )
	{
		RefMsg(msg) >> nItem >> nItemCount >> nMax >> nRemain;
		RefMsg(rmsg) << nItem << nItemCount << nMax << nRemain;
	}
	// 현재 토큰인덱스 , 코스 인덱스
	RefMsg(msg) >> curCoinInex >> curCourseIndex;
	RefMsg(rmsg) << curCoinInex << curCourseIndex;

	// 현재 아이템 새부정보
	RefMsg(msg) >> itemCount;
	RefMsg(rmsg) << itemCount;
	for( idx=0; idx < itemCount; idx++ )
	{
		RefMsg(msg) >> nItem >> nItemCount >> nMax >> nRemain;
		RefMsg(rmsg) << nItem << nItemCount << nMax << nRemain;
	}
	// 요청자에게 보내기
	SEND_Q( rmsg , pc->m_desc );
}

void ProcSubHelperLCBalluse( CNetMsg::SP& msg)
{
	// 반드시 서브핼퍼로 응답 보내야함 ( 현재 BeginTrans 상태 )
	// 인벤 검사하여 아이템 지급
	int			charIndex=0, coinIndex=0, itemIndex=0;
	unsigned int itemCount=0;
	char		courseIndex=0 , cOrder = 0;

	RefMsg(msg) >> charIndex >> coinIndex >> courseIndex >> cOrder >> itemIndex >> itemCount;

	// PC 찾기
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
	{
		CLCString	strVNick(20);
		strVNick.Format("dummy");
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLCBalluse_successMsg( rmsg, 1, strVNick, coinIndex, courseIndex, cOrder, itemIndex ); // 롤백
		SEND_Q( rmsg, gserver->m_subHelper );
		return;
	}

	// DB UPDATE ERROR at SubHelper
	if( coinIndex < 0 )
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLCBalluse_successMsg( rmsg, 1, pc->m_nick, coinIndex, courseIndex, cOrder, itemIndex ); // 롤백
			SEND_Q( rmsg, gserver->m_subHelper )
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			LCBallUseMsg( rmsg, 1 , 0 );
			SEND_Q(rmsg, pc->m_desc);
		}

		return;
	}

	// 빈공간 확인 코인확인
	if( pc->m_inventory.getEmptyCount() < 1 )
	{
		{
			// 빈공간 없음
			CNetMsg::SP rmsg(new CNetMsg);
			LCBallUseMsg( rmsg, 1 , itemIndex );
			SEND_Q(rmsg, pc->m_desc );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLCBalluse_successMsg( rmsg, 1, pc->m_nick, coinIndex, courseIndex, cOrder, itemIndex ); // 롤백
			SEND_Q( rmsg, gserver->m_subHelper );
		}

		return;
	}

	CItem *pCoinItem = pc->m_inventory.FindByDBIndex(coinIndex);
	if( pCoinItem == NULL )
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			LCBallUseMsg( rmsg, 2 , itemIndex );	// 코인이 없습니다.
			SEND_Q(rmsg, pc->m_desc );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLCBalluse_successMsg( rmsg, 1, pc->m_name, coinIndex, courseIndex, cOrder, itemIndex ); // 롤백
			SEND_Q( rmsg, gserver->m_subHelper );
		}

		return;
	}

	// 아이템(itemIndex) 지급
	CItem * pItem = gserver->m_itemProtoList.CreateItem( itemIndex, -1 , 0, 0, itemCount );
	if (pItem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLCBallinfoMsg( rmsg, pc->m_index, coinIndex, courseIndex );
		SEND_Q( rmsg, gserver->m_subHelper );

		return;
	}

	if (pc->m_inventory.addItem(pItem))
	{
		pc->m_inventory.decreaseItemCount(pCoinItem, 1);

		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLCBalluse_successMsg( rmsg, 0, pc->m_nick,  coinIndex , courseIndex, cOrder, itemIndex ); // 서브 핼프로 성공 메세지 보내기
		SEND_Q( rmsg , gserver->m_subHelper );

		if(cOrder == 0)
		{
			//캐릭터 정보, 아이템 정보 로그 남기기
			if(pc && pItem)
				GAMELOG << init("LacaBall First Place", pc) << itemlog(pItem) << end;
		}
		else
		{
			if(pc && pItem && pCoinItem)
				GAMELOG << init("LACA BALL SYSTEM", pc)
						<< pCoinItem->getDBIndex() << " ==> " << pItem->getDBIndex() << end;
		}
	}
	else
	{
		delete pItem;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLCBalluse_successMsg( rmsg, 1, pc->m_nick, coinIndex, courseIndex, cOrder, itemIndex ); // 롤백
			SEND_Q( rmsg, gserver->m_subHelper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			LCBallUseMsg( rmsg, 1 , itemIndex );
			SEND_Q(rmsg, pc->m_desc);
		}
	}

	{
		// 정보갱신 요청
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLCBallinfoMsg( rmsg, pc->m_index, coinIndex, courseIndex );
		SEND_Q( rmsg, gserver->m_subHelper );
	}
}

void ProcSubHelperLCBall_sucess( CNetMsg::SP& msg)
{
	// 모든사람에게 당첨메세지 뿌리기
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int	itemIndex=0;

	RefMsg(msg) >> name >> itemIndex ;

	CNetMsg::SP rmsg(new CNetMsg);
	// 메세지 만들어서
	LCBallSuccessMsg( rmsg, name, itemIndex );
	PCManager::instance()->sendToAll(rmsg);
	// 모든 캐릭터에 전달
}

void ProcSubHelperRakingList(CNetMsg::SP& msg)
{
	CLCString name(20), levelup_date(20);
	int total_count, list_count, charIdx, rank, rank_pre, exp, exp_pre, level;
	unsigned char type, subtype, job;

	RefMsg(msg) >> type
				>> subtype
				>> charIdx
				>> total_count
				>> list_count;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charIdx);
	if (ch == NULL)
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RankingEXList(rmsg, MSG_EX_RANKING_LIST, type, subtype);
		RefMsg(rmsg) << total_count
					 << list_count;

		for (int i=0 ; i<list_count; ++i)
		{
			RefMsg(msg) >> name
						>> rank
						>> rank_pre
						>> exp
						>> exp_pre
						>> level
						>> job
						>> levelup_date;

			RefMsg(rmsg) << name
						 << rank
						 << rank_pre
						 << exp
						 << exp_pre
						 << level
						 << job
						 << levelup_date;
		}
		SEND_Q(rmsg, ch->m_desc);
	}
}
void ProcSubHelperRankingSearch(CNetMsg::SP& msg)
{
	CLCString name(20), levelup_date(20);
	int total_count, charIdx, rank, rank_pre, exp, exp_pre, level;
	unsigned char type, subtype, job;

	RefMsg(msg) >> type
				>> subtype
				>> charIdx
				>> total_count;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charIdx);
	if (ch == NULL)
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RankingEXList(rmsg, MSG_EX_RANKING_SEARCH, type, subtype);
		RefMsg(rmsg) << total_count;
		for (int i=0 ; i<total_count; ++i)
		{
			RefMsg(msg) >> name
						>> rank
						>> rank_pre
						>> exp
						>> exp_pre
						>> level
						>> job
						>> levelup_date;

			RefMsg(rmsg) << name
						 << rank
						 << rank_pre
						 << exp
						 << exp_pre
						 << level
						 << job
						 << levelup_date;
		}
		SEND_Q(rmsg, ch->m_desc);
	}
}
void ProcSubHelperRankingRefresh(CNetMsg::SP& msg)
{
	// 현재 게임에 접속한 모든 유저의 m_exp_weekly값을 초기화한다.
	// 이 루틴이 없으면, 게임하고 있는 유저들의 m_exp_weekly값이 유지되어 잘못된 랭킹을 산정하게된다.
	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator it		= playerMap.begin();
	PCManager::map_t::iterator endit	= playerMap.end();
	for (; it != endit; ++it)
	{
		CPC* pChar = (*it).pPlayer;
		if (pChar == NULL)
			continue;
		pChar->m_exp_weekly = 0;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RankingEXRefresh(rmsg);
		PCManager::instance()->sendToAll(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		int endTime = gserver->getNowSecond() + (7 * 24 * 60 * 60);
		SubHelperRankingRewardReqEx(rmsg, endTime);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
}
void ProcSubHelperRankingError(CNetMsg::SP& msg)
{
	int type, char_index;
	RefMsg(msg) >> type >> char_index;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(char_index);
	if ( ch )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RankingEXError(rmsg, type);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
void ProcSubHelperPKPenaltyReformRep(CNetMsg::SP& msg)
{
	unsigned char subType;
	int charIndex;
	RefMsg(msg) >> subType
				>> charIndex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);

	if( !pc )
		return;

	switch( subType )
	{
	case MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_INFO_REP:
		ProcSubHelperPKPenaltyReformRewardInfoRep(pc, msg);
		break;

	case MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_REP:
		ProcSubHelperPKPenaltyReformRewardRep(pc, msg);
		break;
	}
}

void ProcSubHelperPKPenaltyReformRewardInfoRep(CPC* pc, CNetMsg::SP& msg)
{
	int rewardFlag;
	RefMsg(msg) >> rewardFlag;

	pc->m_pkPenaltyReward = rewardFlag;
	CNetMsg::SP rmsg(new CNetMsg);
	MsgPKPenaltyRewardInfoRep(rmsg, rewardFlag);
	SEND_Q(rmsg, pc->m_desc);
}

void ProcSubHelperPKPenaltyReformRewardRep(CPC* pc, CNetMsg::SP& msg)
{
	int rewardNum;
	int titleIndex;
	unsigned char errorType;

	RefMsg(msg) >> rewardNum
				>> titleIndex
				>> errorType;

	if( errorType == MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_SUC )
	{
		if( pc->m_titleList.m_nCount > MAX_TITLE_COUNT )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_COUNT);
			SEND_Q(rmsg, pc->m_desc);
			return;
		}

		CTitleProto * titleProto =  gserver->m_titleProtoList.FindProto( titleIndex );

		if( !titleProto )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL);
			SEND_Q(rmsg, pc->m_desc);
			return;
		}

		CTitle * title = gserver->m_titleProtoList.Create(titleProto->m_index, titleProto->m_time);

		if( !title )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL);
			SEND_Q(rmsg, pc->m_desc);
			return;
		}

		if( !pc->m_titleList.Add(title) )
		{
			delete title;
			title = NULL;

			CNetMsg::SP rmsg(new CNetMsg);
			MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL);
			SEND_Q(rmsg, pc->m_desc);
			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleItemUseMsg(rmsg, title->m_proto->m_index, title->m_endtime);
			SEND_Q(rmsg, pc->m_desc);
		}

		GAMELOG << init("PK PENALTY REWARD : TITLE INSERT", pc)
				<< "REWARD NUM" << delim << rewardNum
				<< "TITLE INDEX" << delim << title->m_proto->m_index << end;

		// 보상 정보 갱신
		pc->SetPKPenaltyReward( rewardNum );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgPKPenaltyRewardInfoRep( rmsg, pc->m_pkPenaltyReward );
			SEND_Q(rmsg, pc->m_desc);
		}
	}

	{
		// 결과 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, errorType);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼

#ifdef NOTICE_SYSTEM
void ProcSubHelperNoticeSystem(CNetMsg::SP& msg)
{
	int _type;

	RefMsg(msg) >> _type;

	switch ( _type )
	{
	case MSG_SUBHELPER_USER_NOTICE_REG_REP:
		ProcSubHelperNoticeSystemRegRep(msg);
		break;
	case MSG_SUBHELPER_USER_NOTICE_PRINT:
		ProcSubHelperUserNoticePrint(msg);
		break;
	}
}

void ProcSubHelperNoticeSystemRegRep(CNetMsg::SP& msg)
{
	//unsigned char _result = '0';	// default : fail ( 0 : fail, 1 : success )
	int _characterIndex = 0;

	RefMsg(msg) >> _characterIndex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(_characterIndex);
	if ( !pc )
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	UserNoticeRegRepMsg(rmsg);
	SEND_Q(rmsg, pc->m_desc);

	GAMELOG << init("[USER NOTICE] REGIST SUCCESS") << delim
		<< pc->GetName() << end;
}

void ProcSubHelperUserNoticePrint(CNetMsg::SP& msg)
{
	CLCString _name, _contents(81);
	int _count;

	RefMsg(msg) >> _name
		>> _contents
		>> _count;

	if ( _name.Length() <= 0 || _contents.Length() <= 0 )
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	UserNoticePrintMsg(rmsg, _name, _contents);
	PCManager::instance()->sendToAll(rmsg);

	GAMELOG << init("[USER NOTICE] PRINT") << delim
		<< _name << delim
		<< _contents << delim
		<< _count << end;
}
#endif // NOTICE_SYSTEM

void ProcSubHelperExtCharSlotItemUseRep(CNetMsg::SP& msg)
{
	unsigned char _cmd;
	int _userIndex;
	int _charIndex;
	int _tab;
	int _invenIndex;
	CLCString _serial;
	long _time;

	RefMsg(msg) >> _cmd >> _userIndex >> _charIndex >> _tab >> _invenIndex >> _serial >> _time;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(_charIndex);

	if(!ch)
	{
		GAMELOG << init("ERROR") << "CANNOT FIND PLAYER" << delim << _charIndex << end;
		if(_cmd == 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperCharSlotExtUseResult(rmsg, _userIndex);
			SEND_Q(rmsg, gserver->m_subHelper);
		}
		return ;
	}

	switch((int)_cmd)
	{
	case 0:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ItemUseCharSlotExt(rmsg, _cmd);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("ERROR") << "CANNOT USE EXTEND CHARACTERS SLOT ITEM" << end;
		}
		break;
	case 1:
		{
			CItem* item = ch->m_inventory.getItem(_tab, _invenIndex);
			if(item == NULL || item->Count() < 1 || (item->m_serial != (const char *)_serial))
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemUseCharSlotExt(rmsg, 0); /* 여기까지 왔으면 아이템이 있어야 하는데 없어졌다! 에러 처리 */
					SEND_Q(rmsg, ch->m_desc);
				}

				{
					// roll back
					CNetMsg::SP rmsg(new CNetMsg);
					SubHelperCharSlotExtUseResult(rmsg, ch->m_desc->m_index);
					SEND_Q(rmsg, gserver->m_subHelper);
				}
			}
			else
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemUseCharSlotExt(rmsg, _cmd);
					SEND_Q(rmsg, ch->m_desc);
				}

				ch->m_inventory.decreaseItemCount(item, 1);
				ch->m_desc->setExtendCharacterSlotTime(_time);
				GAMELOG << init("USE EXTEND CHARACTER SLOT ITEM", ch) << end;
			}
		}
		break;
	case 2:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXT_CHAR_SLOT);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("CANNOT USE EXTEND CHARACTER SLOT ITEM") << delim << "ALREADY USED" << end;
		}
		break;
	default:
		break;
	}
}

