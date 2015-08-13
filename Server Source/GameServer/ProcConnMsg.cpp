#include "stdhdrs.h"

#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "Cmd.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "DBManager.h"
#include "DescManager.h"

void OnConnEventGomdori2007(CNetMsg::SP& msg);

#ifdef EVENT_PACKAGE_ITEM
void OnCouponConfirm(CNetMsg::SP& msg);
void OnCouponUse(CNetMsg::SP& msg);
#endif

void OnLimitCatalog(CNetMsg::SP& msg);

void OnCashItemPurchase(CNetMsg::SP& msg);
void OnCashBalance(CNetMsg::SP& msg);
void OnCashItemPurchaselist(CNetMsg::SP& msg);
void OnCashItemPurchaseHistory(CNetMsg::SP& msg);
void OnCashItemBring(CNetMsg::SP& msg);
void OnCashItemBringProcess(CNetMsg::SP& msg, bool bPresent);

void OnPreopenGift(CNetMsg::SP& msg);
void OnLoginRep(CNetMsg::SP& msg);



#ifdef RANKER_NOTICE
void OnRankerRep(CNetMsg::SP& msg);
#endif

void OnCashItemGift(CNetMsg::SP& msg);
void OnCashItemGiftSendHistory(CNetMsg::SP& msg);
void OnCashItemGiftRecvHistory(CNetMsg::SP& msg);
void OnCashItemGiftRecvList(CNetMsg::SP& msg);
void OnCashItemGiftRecv(CNetMsg::SP& msg);
void OnCashItemGiftPresentHistory(CNetMsg::SP& msg, bool bSend);

void OnMoveServerOK(CNetMsg::SP& msg);

void OnGuildPointRankingRep( CNetMsg::SP& msg );

void OnConnEventXmas2007( CNetMsg::SP& msg );

#ifdef LACARETTE_SYSTEM
void OnLacaretteSystem(CNetMsg::SP& msg);
#endif

#ifdef DEV_EVENT_PROMOTION2
void OnEventPromotion2(CNetMsg::SP& msg);
void OnEventPromotion2GiveItem(CPC* ch, CNetMsg::SP& msg);
#endif

void OnCashShopWishList(CNetMsg::SP& msg);

#ifdef EVENT_USER_COMEBACK
void onEventUserComeback(CNetMsg::SP& msg);
#endif

#ifdef EVENT_EUR2_PROMOTION
void onEventEUR2Promotion(CNetMsg::SP& msg);
void GiveItem(CPC* pc);
#endif

void CServer::ProcessBilling(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char subtype;

	RefMsg(msg) >> subtype;

	if (msg->m_mtype != MSG_CONN_REP)
		return;

	//LOG_INFO("DEBUG_FUNC : START : subtype : %d", subtype);
	switch (subtype)
	{
	case MSG_CONN_EVENT_XMAS_2007:
		OnConnEventXmas2007(msg);
		break;

	case MSG_CONN_GUILD_POINT_RANKER_REP :
		OnGuildPointRankingRep( msg );
		break;

	case MSG_CONN_EVENT_GOMDORI_2007:
		OnConnEventGomdori2007(msg);
		break;

	case MSG_CONN_GPARA_PROMOTION:
		break;

	case MSG_CONN_OCN_GOO_PROMOTION:
		break;

	case MSG_CONN_MSN_PROMOTION:
		break;

	case MSG_CONN_CREATEEVENT:
		break;

	case MSG_CONN_MOVESERVER_OK:
		OnMoveServerOK(msg);
		break;

#ifdef EVENT_PACKAGE_ITEM
	case MSG_CONN_COUPON_CONFIRM:
		OnCouponConfirm(msg);
		break;
	case MSG_CONN_COUPON_USE:
		OnCouponUse(msg);
		break;
#endif
#ifdef RANKER_NOTICE
	case MSG_CONN_RANKER_REP:
		OnRankerRep(msg);
		break;
#endif

	case MSG_CONN_CASHITEM_BRING_REP:
		OnCashItemBring(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASE_REP:
		OnCashItemPurchase(msg);
		break;
	case MSG_CONN_CASHITEM_BALANCE_REP:
		OnCashBalance(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASELIST_REP:
		OnCashItemPurchaselist(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASEHISTORY_REP:
		OnCashItemPurchaseHistory(msg);
		break;

	case MSG_CONN_LIMIT_CATALOG:
		OnLimitCatalog(msg);
		break;

	case XXX_MSG_CONN_STASH_DELETE_PASSWORD:
		break;
	case XXX_MSG_CONN_STASH_SET_NEW_PASSWORD:
		break;
	case XXX_MSG_CONN_STASH_SEAL:
		break;
	case XXX_MSG_CONN_STASH_CHANGE_PASSWORD:
		break;
	case XXX_MSG_CONN_STASH_CHECK_PASSWORD:
		break;
	case XXX_MSG_CONN_STASH_STATE:
		break;

	case MSG_CONN_PREOPEN_GIFT:
		OnPreopenGift(msg);
		break;

	case MSG_CONN_LOGIN_REP:
		OnLoginRep(msg);
		break;

	case MSG_CONN_CASHITEM_GIFT:
		OnCashItemGift(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_SENDHISTORY:
		OnCashItemGiftSendHistory(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECVHISTORY:
		OnCashItemGiftRecvHistory(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECVLIST:
		OnCashItemGiftRecvList(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECV:
		OnCashItemGiftRecv(msg);
		break;

#ifdef LACARETTE_SYSTEM
	case MSG_CONN_LACARETTE_SYSTEM:
		OnLacaretteSystem(msg);
		break;
#endif

#ifdef DEV_EVENT_PROMOTION2
	case MSG_CONN_EVENT_PROMOTION2:
		OnEventPromotion2(msg);
		break;
#endif
	case MSG_CONN_WISHLIST:
		OnCashShopWishList(msg);
		break;

#ifdef EVENT_USER_COMEBACK
	case MSG_CONN_EVENT_USER_COMEBACK:
		onEventUserComeback(msg);
		break;
#endif
#ifdef EVENT_EUR2_PROMOTION
	case MSG_CONN_EVENT_EUR2_PROMOTION:
		onEventEUR2Promotion(msg);
		break;
#endif
	default:
		break;
	}
	//LOG_INFO("DEBUG_FUNC : END : subtype : %d", subtype);
}

// 물품이 팔렸을때 커넥터가 한정판매 물품 갱신 메세지를 보냄
void OnLimitCatalog(CNetMsg::SP& msg)
{
	int ctid, sell, count, i;

	RefMsg(msg) >> count;
	CCatalog* catal = NULL;
	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> ctid
					>> sell;
		catal = gserver->m_catalogList.Find(ctid);
		if( catal )
			catal->SetToSell(catal->GetLimit() - sell);
	}
}

//XX 로그인 2
void OnLoginRep(CNetMsg::SP& msg)
{
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	unsigned char errcode, usertype, paytype, location;
	int index, timeremain;

	RefMsg(msg) >> id
				>> errcode;

	CDescriptor* desc = DescManager::instance()->getDescById(id);

	if (desc == NULL)
	{
		// 빌링 메시지에 해당하는 사용자 없음
		GAMELOG << init("SYS_ERR") << "Connector descriptor not fount :" << id << ": " << errcode << end;

		// TODO : 로그아웃 시켜야 하는 오류 코드 검사
		if (errcode == MSG_CONN_ERROR_SUCCESS)
		{
			// 로그아웃 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			ConnLogoutMsg(rmsg, id);
			SEND_Q(rmsg, gserver->m_connector);
		}
		return;
	}

	if (errcode == MSG_CONN_ERROR_SUCCESS)
	{
		desc->m_logined = LOGIN_STATE_SUCCESS;

		RefMsg(msg) >> index
					>> usertype
					>> paytype
					>> location
					>> timeremain;

#if defined(EVENT_PCBANG_2ND)
		desc->m_location = location;
#endif
		//0627
		RefMsg(msg) >> desc->m_userFlag;
#ifdef CHARDEL_CHECKID
#ifndef JUMIN_DB_CRYPT
		RefMsg(msg) >> desc->m_ident;
#endif // JUMIN_DB_CRYPT
#endif
		CLCString proSite(4);
		RefMsg(msg) >> proSite;
		desc->m_proSite = proSite;

#ifdef HANARO_EVENT
		// 코드 조작을 로그인 한 유저를 방지 하기 위해.
		if (strcmp(desc->m_proSite, "HF") != 0 && strcmp(desc->m_proSite, "hf") != 0)
		{
			desc->m_hanaroCode = "";
		}
		GAMELOG << init("HANARO_EVENT", id)
				<< "PROSITE : " << desc->m_proSite << delim
				<< "HAHARO_CODE : " << desc->m_hanaroCode << end;
#endif

		DescManager::instance()->changeUserIndex(desc->m_index, index);
		desc->m_index = index;
		
		// Connect server로 부터 인증을 잘 받았다.
		STATE(desc) = CON_GET_IDEN_MSG;
		DBManager::instance()->PushCharacterList(desc); // DBManager로 캐릭터 리스트정보를 요청함

		return;
	}

	if ( errcode != MSG_CONN_ERROR_ALREADY)
		desc->m_logined = LOGIN_STATE_FAIL;

	// 각 경우에 해당하는 로그 남기기고, 세션을 종료한다.
	switch (errcode)
	{
	case MSG_CONN_ERROR_ALREADY:
		{
			GAMELOG << init("CONN_ERR", id) << "ALREADY CONNECT" << end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrLogoutReqMsg(rmsg, 0, -1, -1, -1, id);
				SEND_Q(rmsg, gserver->m_messenger);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				FailMsg(rmsg, MSG_FAIL_BILLING_ALREADY_CONNECT);
				SEND_Q(rmsg, desc);
			}
		}
		break;

	case MSG_CONN_ERROR_FULLUSER:
	case MSG_CONN_ERROR_SYSTEM:
		{
			GAMELOG << init("CONN_ERR", id) << "CONNECTOR SYSTEM error" << end;
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_BILLING_OTHER);
			SEND_Q(rmsg, desc);
		}
		break;

	case MSG_CONN_ERROR_NOMATCH:
		{
			GAMELOG << init("CONN_ERR", id) << "NO MATCH" << end;
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_BILLING_WRONGPW);
			SEND_Q(rmsg, desc);
		}
		break;

	case MSG_CONN_ERROR_BLOCKEDUSER:
		{
			GAMELOG << init("CONN_ERR", id) << "BLOCKED USER" << end;
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_BILLING_BLOCKED);
			SEND_Q(rmsg, desc);
		}
		break;

	case MSG_CONN_ERROR_DOESNOT_LASTCHAOSID:
		{
			GAMELOG << init("CONN_ERR", id) << "DOESNOT LASTCHAOSID" << end;
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_DOESNOT_LASTCHAOSID);
			SEND_Q(rmsg, desc);
		}
		break;

	default:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_BILLING_OTHER);
			SEND_Q(rmsg, desc);
		}
		break;
	}

	desc->Close("Login check fail from connect server");
}

void OnMoveServerOK(CNetMsg::SP& msg)
{
	int userindex = -1;
	RefMsg(msg) >> userindex;

	CDescriptor* desc = DescManager::instance()->getDescByUserIndex(userindex);
	if (desc == NULL)
		return;

	SEND_Q(desc->m_msgGoZone, desc);
	desc->m_bGoZoneSent = true;
}

void OnCashItemPurchaseHistory(CNetMsg::SP& msg)
{
	//MSG_CONN_CASHITEM_PURCHASEHISTORY_REP,//응답		: charindex(n) errorcode(uc) count(n) ctid(n)
	int charindex, count, ctid, i, ctcount;
	unsigned char errorcode;
	//MSG_EX_CASHITEM_PURCHASEHISTORY_REP,// 응답				: errorcode(uc) count(n) ctid(n)

	RefMsg(msg) >> charindex
				>> errorcode
				>> count;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( !pc )
	{
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_CASHITEM
					 << (unsigned char) MSG_EX_CASHITEM_PURCHASEHISTORY_REP
					 << errorcode
					 << count;

		for(i = 0; i < count; i++)
		{
			RefMsg(msg) >> ctcount
						>> ctid;
			RefMsg(rmsg) << ctcount
						 << ctid;
		}

		SEND_Q(rmsg, pc->m_desc);
	}
}
void OnCashItemBring(CNetMsg::SP& msg)
{
	OnCashItemBringProcess(msg, false);
}

void OnCashItemBringProcess(CNetMsg::SP& msg, bool bPresent)
{
	//MSG_CONN_CASHITEM_BRING_REP,	// 응답				: charindex(n) errorCode(uc) count(n) ctid(n)
	int charindex, count, i;
	unsigned char errorcode;
	int ctid[10];

	RefMsg(msg) >> charindex
				>> errorcode
				>> count;

	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> ctid[i];
	}

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( !pc )
	{
		GAMELOG << init("CASH_ITEM_BRING_ERROR-NotChar")
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< charindex << delim
				<< errorcode << delim
				<< count << delim;

		for(i = 0; i < count; i++)
		{
			GAMELOG << ctid[i] << delim;
		}
		GAMELOG << end;

		return;
	}

	if((!bPresent && errorcode == MSG_EX_CASHITEM_ERROR_SUCCESS) ||
			(bPresent && errorcode == MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS))
	{
		// 성공 메세지를 보냄
		// 케릭터 락해제
		pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
		pc->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemBringRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, bPresent);
		SEND_Q(rmsg, pc->m_desc);

		GAMELOG << init("CASH_ITEM_BRING_SUCCESS", pc)
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< count << delim;

		CCatalog* catal = NULL;
		for(i = 0; i < count; i++)
		{
			catal = gserver->m_catalogList.Find(ctid[i]);
			if( catal )
			{
				GAMELOG << catal->GetIndex() << delim
						<< catal->GetName() << delim
						<< catal->GetCash() << delim
						<< catal->GetMileage() << delim;
			}
		}
		GAMELOG << end;

		return;
	}

	CCtItemList* ctItemList = NULL;
	// 롤백
	for(i = 0; i < count; i++)
	{
		int j = 0;

		CCatalog* m_catalog = gserver->m_catalogList.Find(ctid[i]);

		if(!m_catalog)
		{
			// 상품 목록이 없어서 롤백 몬한다
			return;
		}

		ctItemList = m_catalog->GetCtItemList();

		if( !ctItemList )
		{
			return;
		}

		int count = ctItemList->GetCount();

		for(j = 0; j < count; j++)
		{
			CItem* ctitem = ctItemList->GetItem(j);
			if (ctitem == NULL)
				continue;

			item_search_t vec;
			int sc = pc->m_inventory.searchItemByCondition(ctitem->m_itemProto->getItemIndex(), ctitem->getPlus(), ctitem->getFlag(), vec);
			if (sc == 0 || sc < ctitem->Count())
				continue;

			pc->m_inventory.deleteItem(vec, ctitem->Count());
		}
	}

	pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
	pc->m_billReqTime = 0;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemBringRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, bPresent);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void OnCashItemPurchaselist(CNetMsg::SP& msg)
{
	int charindex, count, i;
	unsigned char errorcode;
	int idx[25];
	int ctid[25];

	RefMsg(msg) >> charindex
				>> errorcode
				>> count;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( !pc )
	{
		GAMELOG << init("CASH_ITEM_PURCHASELIST_ERROR-NotChar")
				<< charindex << end;
		return;
	}

	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> idx[i]
					>> ctid[i];
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemPurchaseListRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, count, idx, ctid);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void OnCashItemPurchase(CNetMsg::SP& msg)
{
	//MSG_CONN_CASHITEM_PURCHASE_REP,	// 응답				: userid(str) errorcode(c) cashBalance(n)
	unsigned char errorcode;
	int cashBalance;
	int userindex;

	RefMsg(msg) >> userindex
				>> errorcode
				>> cashBalance;

	CPC *pc = NULL;

	pc = PCManager::instance()->getPlayerByUserIndex(userindex);
	if (pc == NULL)
	{
		GAMELOG << init("CASH_ITEM_PURCHASE_ERROR")
				<< userindex;
		return;
	}

	GAMELOG << init("CASH_ITEM_PURCHASE", pc);
	if( !errorcode )
		GAMELOG	<< "SUCCESS";
	else
		GAMELOG << "ERROR";

	GAMELOG	<< errorcode << delim
			<< pc->m_cashBalance << delim
			<< cashBalance << end;

	pc->m_cashBalance = cashBalance;

	// 캐쉬 메세지 보냄
	CNetMsg::SP rmsg(new CNetMsg);
	pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
	pc->m_billReqTime = 0;
	CashItemPurchaseRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, cashBalance);
	SEND_Q(rmsg, pc->m_desc);

	return;
}

void OnCashBalance(CNetMsg::SP& msg)
{
	unsigned char errorcode;
	int cashBalance;
	int userindex;
	int i;

	RefMsg(msg) >> userindex
				>> errorcode
				>> cashBalance;

	CPC *pc = NULL;
	pc = PCManager::instance()->getPlayerByUserIndex(userindex);
	if (pc == NULL)
	{
		GAMELOG << init("CASH_BALANCE_ERROR")
				<< userindex;
		return;
	}

	pc->m_cashBalance = cashBalance;
	// 캐쉬 메세지 보냄
	CNetMsg::SP rmsg(new CNetMsg);
	CashItemBalanceRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, cashBalance);
	SEND_Q(rmsg, pc->m_desc);
	return;
}

void OnPreopenGift(CNetMsg::SP& msg)
{
	int userindex = 0;
	int charindex = 0;
	char errorcode = -1;

	RefMsg(msg) >> userindex
				>> charindex
				>> errorcode;

	if (errorcode == MSG_CONN_ERROR_SUCCESS)
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if (pc && pc->m_desc->m_index == userindex)
		{
			int giftindex = -1;
			switch (pc->m_job)
			{
			case JOB_TITAN:
				giftindex = 460;
				break;
			case JOB_KNIGHT:
				giftindex = 427;
				break;
			case JOB_HEALER:
				giftindex = 462;
				break;
			case JOB_MAGE:
#ifdef EX_MAGE
			case JOB_EX_MAGE:
#endif // EX_MAGE
				giftindex = 461;
				break;
			case JOB_ROGUE:
#ifdef EX_ROGUE
			case JOB_EX_ROGUE:
#endif // EX_ROGUE
				giftindex = 951;
				break;
			case JOB_SORCERER:
				giftindex = 1069;
				break;
			}

			CItem* item = gserver->m_itemProtoList.CreateItem(giftindex, -1, 0, 0, 1);
			if (item)
			{
				if (pc->m_inventory.addItem(item) == false)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, pc->m_desc);


					delete item;
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				SEND_Q(rmsg, pc->m_desc);
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnPreopenGiftMsg(rmsg, userindex, charindex, true);
			SEND_Q(rmsg, gserver->m_connector);
		}
	}
	else
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if (pc == NULL)
			return ;

		CNetMsg::SP rmsg(new CNetMsg);
		switch (errorcode)
		{
		case MSG_CONN_ERROR_PREOPEN_GIFT_INSUFF_CONDITION:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
			break;
		case MSG_CONN_ERROR_PREOPEN_GIFT_ALREADY_GIVE:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_ALREADY_GIVE);
			break;
		default:
			return ;
		}
		SEND_Q(rmsg, pc->m_desc);
	}
}

#ifdef RANKER_NOTICE
void OnRankerRep(CNetMsg::SP& msg)
{
	int charindex, i;
	RefMsg(msg) >> charindex;

	char tmpBuf[1024] = {0,};
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( pc)
	{
		pc->m_bRanker = true;

		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(tmpBuf, "%d", pc->m_job);

		SayMsg(rmsg, MSG_CHAT_RANKER_CONN, pc->m_index, pc->GetName(), "", tmpBuf);

		PCManager::instance()->sendToAll(rmsg);

		GAMELOG << init("RANKER_CONN", pc) << end;
	}
}
#endif

#ifdef EVENT_PACKAGE_ITEM

#if defined (LC_GAMIGO)
void OnCouponConfirm(CNetMsg::SP& msg)
{
	int charindex;
	unsigned char errorCode;
	int cIndex, type;
	CLCString strKind(31);

	RefMsg(msg) >> charindex
				>> errorCode
				>> cIndex
				>> type
				>> strKind;

	int * pItemIdx = NULL;
	int * pItemCnt = NULL;
	int   nCount = 0;

#if defined (LC_GAMIGO)
	int nPromoItem[2][6] = {{-1, -1, -1, -1, -1, -1}, {1, 1, 1, 1, 1, 1}};

	if (strcmp(strKind, "Boxed") == 0)
	{
		nPromoItem[0][0] = 1706;
		nPromoItem[0][1] = 2360;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 2;
	}
	else if (strcmp(strKind, "Magazin") == 0)
	{
		nPromoItem[0][0] = 3769;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 2;
	}
	else if(strcmp(strKind, "GCO") == 0)
	{
		nPromoItem[0][0] = 1053;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 2;
	}
	else if(strcmp(strKind, "PCGames") == 0)
	{
		nPromoItem[0][0] = 5093;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 1;
	}
	else if(strcmp(strKind, "CBSblau") == 0)
	{
		nPromoItem[0][0] = 1707;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 1;
	}
	else if(strcmp(strKind, "cbs1111") == 0)
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if( !pc)
			return;

		switch( pc->m_job )
		{
		case JOB_TITAN:
			nPromoItem[0][0] = 7558;
			break;
		case JOB_KNIGHT:
			nPromoItem[0][0] = 7559;
			break;
		case JOB_HEALER:
			nPromoItem[0][0] = 7560;
			break;
		case JOB_MAGE:
			nPromoItem[0][0] = 7561;
			break;
		case JOB_ROGUE:
			nPromoItem[0][0] = 7562;
			break;
		case JOB_SORCERER:
			nPromoItem[0][0] = 7563;
			break;
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			nPromoItem[0][0] = 7562;
			break;
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			nPromoItem[0][0] = 7561;
			break;
#endif // EX_MAGE
		default:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				SEND_Q(rmsg, pc->m_desc);
			}
			return;
		}

		nPromoItem[0][1] = -1;
		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 1;
	}
	else if(strcmp(strKind, "CBS1303") == 0)
	{
		nPromoItem[0][0] = 4969;
		nPromoItem[0][1] = 4868;
		nPromoItem[0][2] = 4867;
		nPromoItem[0][3] = 2914;
		nPromoItem[0][4] = 6644;
		nPromoItem[0][5] = 4966;

		nPromoItem[1][0] = 1;
		nPromoItem[1][1] = 1;
		nPromoItem[1][2] = 8;
		nPromoItem[1][3] = 4;
		nPromoItem[1][4] = 3;
		nPromoItem[1][5] = 3;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 6;
	}
	else
	{
		return;
	}
#endif

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		if (errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS)
		{
			for (int i = 0; i < nCount; i++)
			{
				if (pItemIdx[i] <= 0)
					continue;

				if(!pc->GiveItem(pItemIdx[i], 0, 0, pItemCnt[i], false))
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
							<< pItemIdx[i] << end;

					// 롤백
					for(int j = i - 1; j >= 0; j--)
					{
						CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pItemIdx[j]);
						if (!pItemProto)
							continue;

						item_search_t vec;
						int sc = pc->m_inventory.searchItemByCondition(pItemIdx[j], 0, 0, vec);
						if (sc > 0)
						{
							pc->m_inventory.deleteItem(vec, pItemCnt[j]);
						}
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(rmsg, pc->m_desc);
					}
					return;
				} // GiveItem end
			}

			if(gserver->isRunConnector())
			{
				CNetMsg::SP rmsg(new CNetMsg);
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(rmsg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName());
#else
				ConnCouponUse(rmsg, pc->m_index, cIndex, type);
#endif

#if defined (LC_GAMIGO)
				RefMsg(rmsg) << pc->m_desc->m_index
							 << strKind;
#endif // LC_GAMIGO
				SEND_Q(rmsg, gserver->m_connector);
			}
			else
			{
				for(int i = 0; i < nCount; i++)
				{
					CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pItemIdx[i]);
					if (!pItemProto)
						continue;

					item_search_t vec;
					int sc = pc->m_inventory.searchItemByCondition(pItemIdx[i], 0, 0, vec);
					if (sc > 0)
					{
						pc->m_inventory.deleteItem(vec, pItemCnt[i]);
					}
				}

				//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(msg, pc->m_desc);
				return;
			} // end if( gserver->isRunConnector())

			return;
		}

		{
			// 실패이면 에러코드를 그대로 클라이언트에게 전달
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, (MSG_EVENT_ERROR_TYPE) errorCode );
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void OnCouponUse(CNetMsg::SP& msg)
{
	int charindex, type;
	unsigned char errorCode;
	CLCString strKind(31);

	RefMsg(msg) >> charindex
				>> errorCode
				>> type
				>> strKind;

	int * pItemIdx = NULL;
	int * pItemCnt = NULL;
	int   nCount = 0;

#if defined (LC_GAMIGO)
	int nPromoItem[2][6] = {{-1, -1, -1, -1, -1, -1}, {1, 1, 1, 1, 1, 1}};

	if (strcmp(strKind, "Boxed") == 0)
	{
		nPromoItem[0][0] = 1706;
		nPromoItem[0][1] = 2360;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 2;
	}
	else if (strcmp(strKind, "Magazin") == 0)
	{
		nPromoItem[0][0] = 3769;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 2;
	}
	else if(strcmp(strKind, "GCO") == 0)
	{
		nPromoItem[0][0] = 1053;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 2;
	}
	else if(strcmp(strKind, "PCGames") == 0)
	{
		nPromoItem[0][0] = 5093;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 1;
	}
	else if(strcmp(strKind, "CBSblau") == 0)
	{
		nPromoItem[0][0] = 1707;
		nPromoItem[0][1] = -1;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];

		nCount = 1;
	}
	else if(strcmp(strKind, "cbs1111") == 0)
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if( !pc)
			return;

		switch( pc->m_job )
		{
		case JOB_TITAN:
			nPromoItem[0][0] = 7558;
			break;
		case JOB_KNIGHT:
			nPromoItem[0][0] = 7559;
			break;
		case JOB_HEALER:
			nPromoItem[0][0] = 7560;
			break;
		case JOB_MAGE:
			nPromoItem[0][0] = 7561;
			break;
		case JOB_ROGUE:
			nPromoItem[0][0] = 7562;
			break;
		case JOB_SORCERER:
			nPromoItem[0][0] = 7563;
			break;
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			nPromoItem[0][0] = 7562;
			break;
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			nPromoItem[0][0] = 7561;
			break;
#endif // EX_MAGE
		default:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				SEND_Q(rmsg, pc->m_desc);
			}
			return;
		}

		nPromoItem[0][1] = -1;
		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 1;
	}
	else if(strcmp(strKind, "CBS1303") == 0)
	{
		nPromoItem[0][0] = 4969;
		nPromoItem[0][1] = 4868;
		nPromoItem[0][2] = 4867;
		nPromoItem[0][3] = 2914;
		nPromoItem[0][4] = 6644;
		nPromoItem[0][5] = 4966;

		nPromoItem[1][0] = 1;
		nPromoItem[1][1] = 1;
		nPromoItem[1][2] = 8;
		nPromoItem[1][3] = 4;
		nPromoItem[1][4] = 3;
		nPromoItem[1][5] = 3;

		pItemIdx = nPromoItem[0];
		pItemCnt = nPromoItem[1];
		nCount = 6;
	}
	else
	{
		return;
	}
#endif

//	int nPromoItem[2] = {1706, 2360};
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if (pc)
	{
		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백
			for(int i = 0; i < nCount; i++)
			{
				if (pItemIdx[i] <= 0)
					continue;

				CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pItemIdx[i]);
				if (!pItemProto)
					continue;

				item_search_t vec;
				int sc = pc->m_inventory.searchItemByCondition(pItemIdx[i], 0, 0, vec);
				if (sc > 0)
				{
					pc->m_inventory.deleteItem(vec, pItemCnt[i]);
				}
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, (MSG_EVENT_ERROR_TYPE) errorCode);
			SEND_Q(rmsg, pc->m_desc);
		}
	}// if(pc) end
}
#else // 말레이시아, 홍콩을 제외한 나라들
void OnCouponConfirm(CNetMsg::SP& msg)
{
	int charindex, i;
	unsigned char errorCode;
	int cIndex, type;

	RefMsg(msg) >> charindex
				>> errorCode
				>> cIndex
				>> type;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( pc )
	{
		// 아이템 인덱스를 일본 의상 아이템으로
		if( type == 1 && pc->m_job > JOB_SORCERER)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION );
			SEND_Q(rmsg, pc->m_desc);
			return;
		}

		if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 2차 아이템으로
			const static int japanItem_table[5][5] =
			{
				{ 1254, 1255, 1256, 1257, 1258 },		// 타이탄
				{ 1259, 1260, 1261, 1262, 1263 },		// 나이트
				{ 1264, 1265, 1266, 1267, 1268 },		// 힐러
				{ 1269, 1270, 1271, 1272, 1273 },		// 메이지
				{ 1274, 1275, 1276, 1277, 1278 }
			};		// 로그

			const static int japanEvent2_table[2][4]  =
			{
				{ 1316, 1317, 1315, 1314 },
				{	7,	15,		7,	15}
			};

			const static int japanEvent3_table =  2399;

			if( type == 1 )
			{
				for( i = 0; i < 5; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(japanItem_table[(int)pc->m_job][i], 0, 0, 1, false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< japanItem_table[(int)pc->m_job][i] << end;
						// 롤백
						for(i = i-1; i >= 0; i--)
						{
							CItem* item = pc->m_inventory.FindByDBIndex(japanItem_table[(int)pc->m_job][i], 0 , 0);
							if (item)
							{
								pc->m_inventory.decreaseItemCount(item, 1);
							}
						}

						return;
					} // end if
				} // end for
			}
			else if( type == 2)
			{
				for( i = 0; i < 4; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(japanEvent2_table[0][i], 0, 0, japanEvent2_table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< japanEvent2_table[0][i] << delim
								<< japanEvent2_table[1][i] << end;
						// 롤백
						for(i = i-1; i >= 0; i--)
						{
							item_search_t vec;
							int sc = pc->m_inventory.searchItemByCondition(japanEvent2_table[0][i], 0, 0, vec);
							pc->m_inventory.deleteItem(vec, japanEvent2_table[1][i]);
						}

						return;
					} // end if

					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< japanEvent2_table[0][i] << delim
							<< japanEvent2_table[1][i] << end;
				}	// end for
			}
			else if( type == 3)
			{
				if( !pc->GiveItem(japanEvent3_table, 0, 0, 1, false) )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
							<< japanEvent3_table << delim
							<< 1 << end;

					// 롤백
					CItem* item = pc->m_inventory.FindByDBIndex(japanEvent3_table, 0 , 0);
					if (item)
					{
						pc->m_inventory.decreaseItemCount(item, 1);
					}

					return;
				}	// end if

				GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
						<< japanEvent3_table << delim
						<< 1 << end;
			} // end if( type == 3)

			if( gserver->isRunConnector())
			{
				CNetMsg::SP rmsg(new CNetMsg);
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(rmsg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName());
#else
				ConnCouponUse(rmsg, pc->m_index, cIndex, type);
#endif
				SEND_Q(rmsg, gserver->m_connector);
			}
			else
			{
				// 롤백
				if( type == 1 )
				{
					for(i = i-1; i >= 0; i--)
					{
						CItem* item = pc->m_inventory.FindByDBIndex(japanItem_table[(int)pc->m_job][i], 0, 0);
						if (item)
						{
							pc->m_inventory.decreaseItemCount(item, 1);
						}
					}
				}
				else if( type == 2)
				{
					for(i = i-1; i >= 0; i--)
					{
						item_search_t vec;
						int sc = pc->m_inventory.searchItemByCondition(japanEvent2_table[0][i], 0, 0, vec);
						pc->m_inventory.deleteItem(vec, japanEvent2_table[1][i]);
					}
				}
				else if( type == 3)
				{
					CItem* item = pc->m_inventory.FindByDBIndex(japanEvent3_table, 0, 0);
					if (item)
					{
						pc->m_inventory.decreaseItemCount(item, 1);
					}
				}

				{
					// 지급한 아이템을 롤백 하고 없는 쿠폰번호라고 클라이언트에게 보냄
					CNetMsg::SP rmsg(new CNetMsg);
					EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
					SEND_Q(msg, pc->m_desc);
				}
			} // end if( gserver->isRunConnector())

			return;
		}  // end if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )

		{
			// 실패이면 에러코드를 그대로 클라이언트에게 전달
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode );
			SEND_Q(msg, pc->m_desc);
		}
	} // end if( pc )
}

void OnCouponUse(CNetMsg::SP& msg)
{
	int charindex, i, r,c, type;
	unsigned char errorCode;

	RefMsg(msg) >> charindex
				>> errorCode
				>> type;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( pc )
	{
		// 2차 아이템으로
		const static int japanItem_table[5][5] =
		{
			{ 1254, 1255, 1256, 1257, 1258 },		// 타이탄
			{ 1259, 1260, 1261, 1262, 1263 },		// 나이트
			{ 1264, 1265, 1266, 1267, 1268 },		// 힐러
			{ 1269, 1270, 1271, 1272, 1273 },		// 메이지
			{ 1274, 1275, 1276, 1277, 1278 }
		};		// 로그

		const static int japanEvent2_table[2][4]  =
		{
			{ 1316, 1317, 1315, 1314 },
			{	7,	15,		7,	15}
		};

		const static int japanEvent3_table =  2399;

		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백
			if( type == 1 )
			{
				for(i = 0; i < 5; i++)
				{
					CItem* item = pc->m_inventory.FindByDBIndex(japanItem_table[(int)pc->m_job][i], 0, 0);
					if (item)
					{
						pc->m_inventory.decreaseItemCount(item, 1);
					}
				}
			}
			else if( type == 2 )
			{
				for(i = 0; i < 4; i++)
				{
					item_search_t vec;
					int sc = pc->m_inventory.searchItemByCondition(japanEvent2_table[0][i], 0, 0, vec);
					pc->m_inventory.deleteItem(vec, japanEvent2_table[1][i]);
				}
			}
			else if( type == 3)
			{
				CItem* item = pc->m_inventory.FindByDBIndex(japanEvent3_table, 0, 0);
				if (item)
				{
					pc->m_inventory.decreaseItemCount(item, 1);
				}
			}
		}

		/*		GAMELOG << init("COUPON_ITEM_GIVE", pc->m_name, pc->m_desc->m_idname)
						<< "TYPE" << delim << type
						<< "ERROR_CODE" << delim << errorCode << end;	*/
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, (MSG_EVENT_ERROR_TYPE) errorCode);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}
#endif // #else
#endif //EVENT_PACKAGE_ITEM

void OnCashItemGift(CNetMsg::SP& msg)
{
	unsigned char errCode;
	int sendCharIndex;
	int recvCharIndex;

	RefMsg(msg) >> errCode
				>> sendCharIndex
				>> recvCharIndex;

	CPC* sendPC = PCManager::instance()->getPlayerByCharIndex(sendCharIndex);
	if( sendPC )
	{
		sendPC->ResetPlayerState(PLAYER_STATE_CASHITEM);
		sendPC->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errCode);
		SEND_Q(rmsg, sendPC->m_desc);
	}

	if( errCode == MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS )
	{
		CPC* recvPC = PCManager::instance()->getPlayerByCharIndex( recvCharIndex);
		if( recvPC )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			CashItemGiftRecvNoticeRepMsg( rmsg, 1 );
			SEND_Q(rmsg, recvPC->m_desc);
		}
	}
}

void OnCashItemGiftSendHistory(CNetMsg::SP& msg)
{
	OnCashItemGiftPresentHistory(msg, true);
}
void OnCashItemGiftRecvHistory(CNetMsg::SP& msg)
{
	OnCashItemGiftPresentHistory(msg, false);
}

void OnCashItemGiftPresentHistory(CNetMsg::SP& msg, bool bSend)
{
	// 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	unsigned char errCode;
	int count, ctid, charindex;
	CLCString charName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> charindex
				>> errCode
				>> count;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( pc )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftHistoryRepMsg(rmsg, bSend, (MSG_EX_CASHITEM_ERROR_TYPE) errCode);
		RefMsg(rmsg) << count;

		for(int i = 0; i < count; i++ )
		{
			RefMsg(msg) >> ctid
						>> charName;

			RefMsg(rmsg) << ctid
						 << charName;
		}

		SEND_Q(rmsg, pc->m_desc);
	}
}

void OnCashItemGiftRecvList(CNetMsg::SP& msg)
{
	int charIndex;
	unsigned char listflag;

	RefMsg(msg) >> charIndex
				>> listflag;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftRecvListRepMsg(rmsg, listflag, msg);
		SEND_Q(rmsg, pc->m_desc);
	}
}
void OnCashItemGiftRecv(CNetMsg::SP& msg)
{
	OnCashItemBringProcess(msg, true);
}

void OnConnEventGomdori2007(CNetMsg::SP& msg)
{
	unsigned char		cSubType;
	int					nCharIndex;
	int					nCount;
	int*				nStatus = NULL;

	RefMsg(msg) >> cSubType;

	if (cSubType == MSG_CONN_EVENT_GOMDORI_2007_STATUS)
	{
		RefMsg(msg) >> nCharIndex
					>> nCount;
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
		if (!pPC)
			return ;

		if (nCount > 0)
			nStatus = new int[nCount];
		int i;
		for (i = 0; i < nCount; i++)
			RefMsg(msg) >> nStatus[i];

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventGomdori2007ViewStatusMsg(rmsg, nCount, nStatus);
			SEND_Q(rmsg, pPC->m_desc);
		}

		if (nStatus)
		{
			delete [] nStatus;
			nStatus = NULL;
		}
	}
}

void OnGuildPointRankingRep( CNetMsg::SP& msg )
{
	int nCharindex;
	int nGuildindex;
	int nRanking;

	RefMsg(msg) >> nCharindex
				>> nGuildindex
				>> nRanking;

	CPC* pc			= PCManager::instance()->getPlayerByCharIndex(nCharindex);
	CGuild* guild 	= gserver->m_guildlist.findguild( nGuildindex );

	if( !pc )
		return;
	if( guild )
	{
		guild->guildPointRanking( nRanking );

		CNetMsg::SP rmsg(new CNetMsg);
		GuildPointRankingMsg( rmsg, nCharindex, nGuildindex, nRanking );
		pc->m_pArea->SendToCell(rmsg, pc, true);
	}
}

void OnConnEventXmas2007( CNetMsg::SP& msg )
{
	if( !gserver->isActiveEvent( A_EVENT_XMAS ) )
		return;

	unsigned char	cSubtype = 0;
	unsigned int	unPoint = 0;
	CNetMsg			rmsg;

	RefMsg(msg) >> cSubtype;

	CZone* pZone = gserver->FindZone(ZONE_START);		//주노
	if (pZone == NULL)
		return;			// 주노지역 없음

	CArea *area = pZone->m_area;
	if ( area == NULL )
		return;			// 지역정보 없음

	switch( cSubtype )
	{
	case MSG_CONN_EVENT_XMASTREE_POINT :
		{
			RefMsg(msg) >> unPoint;

			gserver->m_unEventXmasTreePoint = unPoint;

			CNPC* npc = area->m_npcList;
			while ( npc )
			{
//#ifdef EVENT_XMAS_2007_GAMIGO_10TH
//				if( npc->m_idNum == 1304 )		//가미고 10주년 이벤트로 케익 npc로 사용한다.
//#else
				if( npc->m_idNum == 484 || npc->m_idNum == 486 /*크리스마스 트리 2종 */ )		// 크리스 마스 트리 NPC 다수
//#endif
				{
					npc->m_maxHP = npc->m_hp = (int) unPoint;
					CNetMsg::SP rmsg(new CNetMsg);
					CharStatusMsg(rmsg, npc , 0);
					npc->m_pArea->SendToCell(rmsg, npc, true);
				}
				npc = npc->m_pNPCNext;
			}
		}
		break;

	case MSG_CONN_EVENT_XMASTREE_ITEMDROP :
		{
			int nRemainTime = 0;
			RefMsg(msg) >> nRemainTime;

			if( nRemainTime == 5 )
			{
				// 모든 클라이언트에 아이템 드랍 까지 남은 시간 표시
				CNetMsg::SP rmsg(new CNetMsg);
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_DROP_ITEM );
				RefMsg(rmsg) << nRemainTime;
				PCManager::instance()->sendToAll(rmsg);
			}
			else if ( nRemainTime == 0 )
			{
				/*const int nXmasEventItem[7][2] = {
					{ 2590, 40 },			// 소형 빨간띠 막대사탕 40개
					{ 2591, 30 },			// 중형 빨간띠 막대사탕 30개
					{ 2592, 20 },			// 대형 빨간띠 막대사탕 20개
					{ 2593, 40 },			// 소형 파란띠 막대사탕 40개
					{ 2594, 30 },			// 중형 파란띠 막대사탕 30개
					{ 2595, 20 },			// 대형 파란띠 막대사탕 20개
					{ 2596, 20 },			// 할록달록 막대사탕 20개
				};*/

				CNPC* npc = area->m_npcList;
				while ( npc )
				{
					if( npc->m_idNum == 484/*메인 크리스마스 트리*/ )		// 크리스 마스 트리 NPC 다수
					{
						break;
					}
					npc = npc->m_pNPCNext;
				}

				if( npc == NULL )
					return;		// NPC 나무가 없다.

				int nDropItemPos = GetRandom(0,100) % gserver->m_dropItemMgr.getDropItemListCount(19);

				// 무조건 드랍이므로 가라 피씨를 구하자.

				CPC * pc = NULL; // npc와 비교할 가라 피씨가 필요하다 아무거나 대충 골라보자.

				PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
				PCManager::map_t::iterator iter		= playerMap.begin();
				PCManager::map_t::iterator endIter	= playerMap.end();
				for (; iter != endIter; ++iter)
				{
					pc = (*iter).pPlayer;
					if (pc)
						break;
				}

				if( pc )
				{
					for( int idx=0; idx<50; idx++ )
					{
						gserver->m_dropItemMgr.doDrop( pc, npc, 19, -1, nDropItemPos, false);
					}
				}
			}
		}
		break;
	}
}

#ifdef LACARETTE_SYSTEM

void OnLacaretteSystem(CNetMsg::SP& msg)
{
	unsigned char subtype, errortype;
	int userindex, charindex;

	RefMsg(msg) >> subtype
				>> errortype
				>> userindex
				>> charindex;

	CPC *ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( !ch )
		return;
	if( !(subtype == MSG_CONN_LACARETTE_RETTE_WRITE && errortype == MSG_CONN_LACARETTE_RETTE_ERROR_SUC) )
	{
		ch->m_lacaretteCosArrNum = -1;
		ch->m_lacaretteItemArrNum = -1;
		ch->m_lacaretteTokenArrNum = -1;
		ch->m_lacaretteRepeat = -1;
	}

	switch( subtype )
	{
	case MSG_CONN_LACARETTE_TOKEN_READY_REP:

		switch(errortype )
		{
		case MSG_CONN_LACARETTE_TOKEN_ERROR_SUC: // 토큰 지급 가능
			{
				int tokenIndex = 5123;

				item_search_t vec;
				int sc = ch->m_inventory.searchItemByCondition(tokenIndex, 0, 0, vec);
				if (sc + 15 > 100 )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_TOKEN_OVER);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				{
					// 해당 유저에게도 메시지를 날리자.
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteTokenReadyErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_SUC);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			break;

		case MSG_CONN_LACARETTE_TOKEN_ERROR_ALREADY: // 이미 지급
			{
				CNetMsg::SP rmsg(new CNetMsg);
				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_ALREADY);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;

		case MSG_CONN_LACARETTE_TOKEN_ERROR: // 기타 에러
			{
				CNetMsg::SP rmsg(new CNetMsg);
				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;
		}
		break;

	case MSG_CONN_LACARETTE_TOKEN_REP: // 토큰 지급 답변

		switch( errortype )
		{
		case MSG_CONN_LACARETTE_TOKEN_ERROR_SUC: // 토큰 지급 가능
			{
				// DB에 지급일자를 적자.
				CNetMsg::SP rmsg(new CNetMsg);
				ConnLacaretteTokenAddMag(rmsg, userindex, charindex);
				SEND_Q(rmsg, gserver->m_connector);
				return;
			}
			break;

		case MSG_CONN_LACARETTE_TOKEN_ERROR_ALREADY: // 이미 지급
			{
				CNetMsg::SP rmsg(new CNetMsg);
				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_ALREADY);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;

		case MSG_CONN_LACARETTE_TOKEN_ERROR: // 기타 에러
			{
				CNetMsg::SP rmsg(new CNetMsg);
				lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;
		}
		break;
	case MSG_CONN_LACARETTE_TOKEN_WRITE: // 토큰 지급 답변
		{
			switch( subtype )
			{
			case MSG_CONN_LACARETTE_TOKEN_ERROR_SUC:
				{
					int tokenIndex = 5123;
					int itemCount = 15;
					CItem* pItem = gserver->m_itemProtoList.CreateItem(tokenIndex, -1, 0, 0, itemCount);

					if( pItem ) // 아이템 생성에 실패 했을때
					{
						if (ch->m_inventory.addItem(pItem) == false)
						{
							delete pItem;
							pItem = NULL;

							{
								CNetMsg::SP rmsg(new CNetMsg);
								lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_INVEN);
								SEND_Q(rmsg, ch->m_desc);
							}
							{
								CNetMsg::SP rmsg(new CNetMsg);
								ConnLacaretteTokenAddMag(rmsg ,ch->m_desc->m_index, ch->m_index, -1);// DB 기록을 다시 되돌리자
								SEND_Q(rmsg,gserver->m_connector);
							}

							return;
						}

						{
							// 해당 유저에게 메시지를 날리자.
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_SUC);
							SEND_Q(rmsg, ch->m_desc);
						}
						return;
					}
					else
					{
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR);
							SEND_Q(rmsg, ch->m_desc);
						}

						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteTokenAddMag(rmsg, ch->m_desc->m_index, ch->m_index, -1);// DB 기록을 다시 되돌리자
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}
				}
				break;
			case MSG_CONN_LACARETTE_TOKEN_ERROR:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR);
					SEND_Q(rmsg, ch->m_desc);
				}
				return;
				break;
			}
		}
		break;
	case MSG_CONN_LACARETTE_RETTE_REP:

		switch( errortype )
		{
		case MSG_CONN_LACARETTE_RETTE_ERROR_SUC:
			{
				// 0. 사용횟수 통과 ( 커넥터 서버에서 확인)
				// 1. 토큰 갯수 확인
				// 2. 지급 상품 뽑기
				// 3. 지급 아이템 생성 가능 확인
				// 4. 인벤 빈자리 무게 확인
				// 5. 지급

				int cosArrNum, cosIndex, tokenIndex, repeat;
				RefMsg(msg) >> cosArrNum >> cosIndex >> tokenIndex >> repeat;

				CourseInfo * cos = gserver->m_lacarette.GetCourseData(cosArrNum);

				if( cos == NULL || cos->index != cosIndex )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteRetteErrorMSg(rmsg, MSG_EX_LACARETTE_RETTE_ERROR);
					SEND_Q(rmsg, ch->m_desc);

#ifdef LC_KOR
					if( cos )
						GAMELOG << init("LACARETTE RETTE ERROR : COURSE NOT MATCH" ) << delim << cosArrNum << delim << cosIndex << delim << cos->index << end;
					else
						GAMELOG << init("LACARETTE RETTE ERROR : COURSE NOT FIND" )  << delim << cosArrNum << delim << cosIndex  << end;

#endif
					return;
				}

				int giveprob = GetRandom(1, 10000);
				int totalprob = 0;
				int giveItemArrNum;
				GiveItemInfo * gItemInfo=NULL;

				for(giveItemArrNum=0; giveItemArrNum<10; giveItemArrNum++)
				{
					totalprob += cos->giveItem[giveItemArrNum].prob;
					if( giveprob <= totalprob )
					{
						gItemInfo = &cos->giveItem[giveItemArrNum];
						break;
					}
				}

				if( gItemInfo == NULL )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteRetteErrorMSg(rmsg, MSG_EX_LACARETTE_RETTE_ERROR);
					SEND_Q(rmsg, ch->m_desc);
#ifdef LC_KOR
					GAMELOG << init("LACARETTE RETTE ERROR : PROB" )  << delim << totalprob << delim << giveprob << end;
#endif
					return;
				}

				if (ch->m_inventory.getEmptyCount() < 1)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteTokenErrorMSg(rmsg, MSG_EX_LACARETTE_TOKEN_ERROR_INVEN);
					SEND_Q(rmsg, ch->m_desc);

					return;
				}

				// 토큰 아이템을 가지고 있는지 확인
				int tokenArrNum = -1;
				int needToeknCount = gserver->m_lacarette.GetNeedTokenCount(cosArrNum, tokenIndex, repeat, tokenArrNum );
				item_search_t vec;
				int sc = ch->m_inventory.searchItemByCondition(tokenIndex,0,0, vec);
				if (sc == 0 || sc < needToeknCount)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteRetteErrorMSg(rmsg, MSG_EX_LACARETTE_RETTE_ERROR_TOKEN);
					SEND_Q(rmsg, ch->m_desc);

					return;
				}

				ch->m_lacaretteCosArrNum = cosArrNum;
				ch->m_lacaretteItemArrNum = giveItemArrNum;
				ch->m_lacaretteTokenArrNum = tokenArrNum;
				ch->m_lacaretteRepeat = repeat;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteRetteErrorSucMSg(rmsg, giveItemArrNum, gItemInfo->index); //, cosArrNum, cosIndex );
					SEND_Q(rmsg, ch->m_desc);
				}

				return;
			}
			break;

		case MSG_CONN_LACARETTE_RETTE_ERROR:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				lacaretteRetteErrorMSg(rmsg, MSG_EX_LACARETTE_RETTE_ERROR);
				SEND_Q( rmsg, ch->m_desc);

#ifdef LC_KOR
				GAMELOG << init("LACARETTE RETTE ERROR : CONNECTOR SERVER ERROR RETURN" )  << end;
#endif
			}
			break;
		}
		break;

	case MSG_CONN_LACARETTE_RETTE_WRITE:
		{
			switch( errortype )
			{
			case MSG_CONN_LACARETTE_RETTE_ERROR_SUC:
				{
					// DB 기록이 되었다면 아이템 지급
					if( ch->m_lacaretteCosArrNum == -1
							|| ch->m_lacaretteItemArrNum == -1
							|| ch->m_lacaretteTokenArrNum == -1
							|| ch->m_lacaretteRepeat == -1 )
					{
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
							SEND_Q(rmsg, ch->m_desc);
						}

						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}

					int CosArrNum	= ch->m_lacaretteCosArrNum;
					int ItemArrNum	= ch->m_lacaretteItemArrNum;
					int TokenArrNum	= ch->m_lacaretteTokenArrNum;
					int RepeatCnt	= ch->m_lacaretteRepeat;

					ch->m_lacaretteCosArrNum	= -1;
					ch->m_lacaretteItemArrNum	= -1;
					ch->m_lacaretteTokenArrNum	= -1;
					ch->m_lacaretteRepeat		= -1;

					int giveitemindex;
					RefMsg(msg) >> giveitemindex;

					CourseInfo * cos = gserver->m_lacarette.GetCourseData(CosArrNum);

					if( cos == NULL )
					{
						GAMELOG << init("LACARETTE_FAIL COURSE DATA ERROR") << CosArrNum << end;
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
							SEND_Q(rmsg, ch->m_desc);
						}
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 ); // DB 기록을 다시 되돌리자
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}

					GiveItemInfo * gItemInfo=&cos->giveItem[ItemArrNum];

					if( gItemInfo == NULL || giveitemindex != gItemInfo->index)
					{
						GAMELOG << init("LACARETTE_FAIL GIVEITEM DATA ERROR") << cos->index << delim
								<< ItemArrNum << delim
								<< giveitemindex << end;
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
							SEND_Q(rmsg, ch->m_desc);
						}
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );// DB 기록을 다시 되돌리자
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}

					// 토큰 아이템을 가지고 있는지 확인
					int needToeknCount = 0;
					item_search_t vec;
					int sc = ch->m_inventory.searchItemByCondition(cos->coinIndex[TokenArrNum], 0, 0, vec);
					if (sc > 0)
					{
						// 아이템이 있다 갯수를 확인하자

						needToeknCount = gserver->m_lacarette.GetNeedTokenCount(CosArrNum
										 , cos->coinIndex[TokenArrNum]
										 , RepeatCnt
										 , TokenArrNum );
						if (needToeknCount > sc)
						{
							{
								CNetMsg::SP rmsg(new CNetMsg);
								lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR_TOKEN);
								SEND_Q(rmsg, ch->m_desc);
							}
							{
								CNetMsg::SP rmsg(new CNetMsg);
								ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );// DB 기록을 다시 되돌리자
								SEND_Q(rmsg, gserver->m_connector);
							}

							return;
						}
					}
					else
					{
						// 토큰이 없다.
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR_TOKEN);
							SEND_Q(rmsg, ch->m_desc);
						}
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );// DB 기록을 다시 되돌리자
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}

					CItem * pItem = gserver->m_itemProtoList.CreateItem(gItemInfo->index, -1, 0, gItemInfo->flag, gItemInfo->count);

					if( pItem )
					{
						if (ch->m_inventory.addItem(pItem) == false)
						{
							delete pItem;

							{
								CNetMsg::SP rmsg(new CNetMsg);
								lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
								SEND_Q(rmsg, ch->m_desc);
							}
							{
								CNetMsg::SP rmsg(new CNetMsg);
								ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );// DB 기록을 다시 되돌리자
								SEND_Q(rmsg, gserver->m_connector);
							}

							return;
						}
					}
					else
					{
						// 지급 아이템 주지 못했으니.... 토큰 아이템을 되돌려준다.
						{
							CNetMsg::SP rmsg(new CNetMsg);
							lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
							SEND_Q(rmsg, ch->m_desc);
						}
						{
							CNetMsg::SP rmsg(new CNetMsg);
							ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, 0, -1 );// DB 기록을 다시 되돌리자
							SEND_Q(rmsg, gserver->m_connector);
						}

						return;
					}

					LONGLONG tokencurrcont = 0;

					{
						item_search_t vec;
						int sc = ch->m_inventory.searchItemByCondition(cos->coinIndex[TokenArrNum], 0, 0, vec);
						if (sc > 0)
						{
							needToeknCount = gserver->m_lacarette.GetNeedTokenCount(CosArrNum
											 , cos->coinIndex[TokenArrNum]
											 , RepeatCnt
											 , TokenArrNum );

							ch->m_inventory.deleteItem(vec, needToeknCount);

							// 지운 후 남은 갯수를 다시 검색
							sc = ch->m_inventory.searchItemByCondition(cos->coinIndex[TokenArrNum], 0, 0, vec);
						}

						tokencurrcont = sc;
					}

					{
						// 결과 메시지롤 다시 보내자
						CNetMsg::SP rmsg(new CNetMsg);
						lacaretteResultErrorSucMSg(rmsg, RepeatCnt+1);
						SEND_Q(rmsg, ch->m_desc);
					}

					GAMELOG << init("LACARETTE_SUC", ch)
							<< cos->index << delim							// 코스 인덱스
							<< cos->coinIndex[TokenArrNum] << delim			// 토큰 인덱스
							<< tokencurrcont + needToeknCount << delim		// 토큰 보유량
							<< needToeknCount << delim						// 토큰 사용량
							<< tokencurrcont << delim						// 토큰 남은량
							<< RepeatCnt << delim							// 라카렛 횟수(일일 누적 횟수)
							<< gItemInfo->index << delim					// 보상 아이템 인덱스
							<< gItemInfo->flag << delim						// 보상 아이템 플레그
							<< gItemInfo->count << end;						// 보상 아이템 지급 개수

					// 1등 상품이 당첨되면 알리장.
					if(ItemArrNum == 0)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						lacaretteJackpotMsg(rmsg, gItemInfo->index, ch->GetName());
						PCManager::instance()->sendToAll(rmsg);
					}
					return;
				}
				break;
			case MSG_CONN_LACARETTE_RETTE_ERROR:
				{
					// DB update 실패
					CNetMsg::SP rmsg(new CNetMsg);
					lacaretteResultErrorMSg(rmsg, MSG_EX_LACARETTE_RESULT_ERROR);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				break;
			}
		}
		break;
	case MSG_CONN_LACARETTE_UI_REP:
		{
			int repeat;
			RefMsg(msg) >> repeat;
			CNetMsg::SP rmsg(new CNetMsg);
			lacaretteUIrepMsg(rmsg, repeat);
			SEND_Q( rmsg, ch->m_desc);
			break;
		}
		break;
	}
}

#endif // LACARETTE_SYSTEM

#ifdef DEV_EVENT_PROMOTION2

void OnEventPromotion2(CNetMsg::SP& msg)
{
	int charIndex;
	RefMsg(msg) >> charIndex;

	CPC * ch = PCManager::instance()->getPlayerByCharIndex( charIndex);

	if( ch == NULL )
		return;

	unsigned char subType;
	RefMsg(msg) >> subType;

	if( subType == MSG_CONN_PROMOTION2_REP_ERROR_SUC)
	{
		OnEventPromotion2GiveItem(ch, msg);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPromotion2RepErrorMsg(rmsg, subType );
		SEND_Q(rmsg, ch->m_desc);
	}
}

void OnEventPromotion2GiveItem(CPC* ch, CNetMsg::SP& msg )
{
	int kind, keyindex;
	RefMsg(msg) >> kind >> keyindex;

	Promorion2Info* promitem = gserver->m_promotion2.GetPromotionData(kind);

	if( promitem == NULL )
	{
		GAMELOG << init("EVENT_PROMOTION2 KIND ERROR", ch)
				<< "kind num" << delim << kind << end;

		CNetMsg::SP rmsg(new CNetMsg);
		EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 무게 체크
	int i;
	for(i=0; i<promitem->count; i++)
	{
		CItemProto * pItem = gserver->m_itemProtoList.FindIndex( promitem->itemlist[i].index );
		if( !pItem )
		{
			// 지급할 아이템이 서버에 없넹. 믄제가 있으니 error 메시지 날리자.
			CNetMsg::SP rmsg(new CNetMsg);
			EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 인벤 체크
	if( ch->m_inventory.getEmptyCount() < promitem->count )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (gserver->isRunConnector() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 아이템을 모두 생성하자.
	for(i=0; i<promitem->count; i++)
	{
		if( !ch->GiveItem(promitem->itemlist[i].index , promitem->itemlist[i].plus, promitem->itemlist[i].flag, promitem->itemlist[i].count ) )
		{
			// 지급에 실패 했다. 실패한 아이템을 빼주자
			for(int j = 0; j < i; j++)
			{
				CItem* item = ch->m_inventory.FindByDBIndex(promitem->itemlist[i].index, 0, 0);
				if (item)
				{
					ch->m_inventory.decreaseItemCount(item, 1);
				}
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL);
				SEND_Q(rmsg, ch->m_desc);
			}

			GAMELOG << init("EVENT_PROMOTION2 GIVE ITEM FAIL", ch)
					<< "keyIndex" << delim << keyindex << delim
					<< "kind" << delim << kind << end;
			return;
		}
	}

	// 지급에 성공 했다. DB에서 쿠폰을 사용하지 못하게 막자.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnPromotion2CouponItemReqMsg(rmsg, ch->m_desc->m_index, ch->m_index, keyindex );
		SEND_Q(rmsg, gserver->m_connector);

		GAMELOG << init("EVENT_PROMOTION2 GIVE ITEM SUC", ch)
				<< "keyIndex" << delim << keyindex << delim
				<< "kind" << delim << kind << end;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_SUC);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#endif // #ifdef DEV_EVENT_PROMOTION2

void OnCashShopWishList(CNetMsg::SP& msg)
{
	unsigned char command;
	unsigned char errorcode;
	int i;
	int j;
	int userindex;
	int charindex;
	int id;
	int tempid;
	CCatalog* pCatalog;
	void* pos;
	CCatalog* pLimitCatalog;
	RefMsg(msg) >> command
				>> userindex
				>> charindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc == NULL)
		return;

	switch(command)
	{
	case MSG_WISHLIST_TYPE_LIST_REP:
		{
			int arrayindex = 0;
			int count = 0;
			int ctid[8];
			int type[8];
			int price[8];
			int remain[8];
			memset(ctid, 0, sizeof(int)*8);
			memset(type, 0, sizeof(int)*8);
			memset(price, 0, sizeof(int)*8);
			memset(remain, -1, sizeof(int)*8);
			RefMsg(msg) >> count;
			for(i = 0; i < count; i++)
			{
				RefMsg(msg) >> tempid;
				pCatalog = gserver->m_catalogList.Find(tempid);
				if(pCatalog == NULL)
					continue;
				//if(!pCatalog->GetEnable())
				//	continue;

				ctid[arrayindex] = tempid;
				type[arrayindex] = pCatalog->GetSubtype();
				if(type[arrayindex] % 100 == 0)
				{
					type[arrayindex] = pCatalog->GetType();
					if(type[arrayindex] % 10000 == 0)
					{
						type[arrayindex] = pCatalog->GetCategory();
					}
				}
				// 가격과 남은개수를 찾아야 한다..ㅠ.ㅠ
				std::set<int>::iterator it = gserver->m_limitCT.begin();
				std::set<int>::iterator endit = gserver->m_limitCT.end();
				for(; it != endit; ++it)
				{
					CCatalog* pLimitCatalog = gserver->m_catalogList.Find(*it);
					if(pLimitCatalog)
					{
						if(ctid[arrayindex] == pLimitCatalog->GetIndex())
						{
							remain[arrayindex] = pLimitCatalog->GetToSell();
						}
					}
				}
				pCatalog = gserver->m_catalogList.Find(ctid[arrayindex]);
				if(pCatalog)
				{
					price[arrayindex] = pCatalog->GetCash();
				}
				arrayindex++;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgCashshopWishlistRep(rmsg, arrayindex, ctid, type, remain, price);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	case MSG_WISHLIST_TYPE_SAVE_REP:
		{
			RefMsg(msg) >> errorcode
						>> id;
			CNetMsg::SP rmsg(new CNetMsg);
			MsgCashshopWishlistSaveRep(rmsg, errorcode, id);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	case MSG_WISHLIST_TYPE_DEL_REP:
		{
			RefMsg(msg) >> errorcode
						>> id;
			CNetMsg::SP rmsg(new CNetMsg);
			MsgCashshopWishlistDelRep(rmsg, errorcode, id);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	}
}

#ifdef EVENT_USER_COMEBACK
void onEventUserComeback(CNetMsg::SP& msg)
{
	char errortype;
	int userindex;
	int charindex;

	RefMsg(msg) >> errortype
				>> userindex
				>> charindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( pc == NULL )
		return;

	if( errortype != (char)MSG_EVENT_USER_COMEBACK_ERROR_SUC ) // 오류
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventUserComebackErrorMsg(rmsg, (MSG_EVENT_USER_COMEBACK_ERROR_TYPE)errortype );
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	if( pc->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	int giftindex = 0;
	switch( pc->m_job)
	{
	case JOB_TITAN:
		giftindex=6711;
		break;
	case JOB_KNIGHT:
		giftindex=6712;
		break;
	case JOB_HEALER:
		giftindex=6713;
		break;
	case JOB_MAGE:
		giftindex=6714;
		break;
	case JOB_ROGUE:
		giftindex=6715;
		break;
	case JOB_SORCERER:
		giftindex=6716;
		break;
	case JOB_NIGHTSHADOW:
		giftindex=6717;
		break;
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
		giftindex=6715;
		break;
#endif // EX_ROGUE
#ifdef EX_MAGE
	case JOB_EX_MAGE:
		giftindex=6714;
		break;
#endif // EX_MAGE
	}

	if( !pc->GiveItem(giftindex, 0, 0, 1) )
	{
		GAMELOG << init("EVENT_USER_COMEBACK : ITEM GIVE FAIL", pc) << giftindex << end;
		CNetMsg::SP rmsg(new CNetMsg);
		EventUserComebackErrorMsg(rmsg, MSG_EVENT_USER_COMEBACK_ERROR_NOT_USER );
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	{
		GAMELOG << init("EVENT_USER_COMEBACK : ITEM GIVE SUC", pc) << giftindex << end;
		CNetMsg::SP rmsg(new CNetMsg);
		EventUserComebackErrorMsg(rmsg, MSG_EVENT_USER_COMEBACK_ERROR_SUC );
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif

#ifdef EVENT_EUR2_PROMOTION
void onEventEUR2Promotion(CNetMsg::SP& msg)
{
	int type = -1;
	int charindex = 0;
	RefMsg(msg) >> type >> charindex;

	CPC* pc = NULL;
	pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc == NULL)
		return ;

	switch(type)
	{
	case MSG_EVENT_EUR2_PROMOTION_SUCCESS:
		{
			// 성공 했으니 아이템 지급하자.
			GiveItem(pc);
			// 캐릭터에게 메시지를 보내자.
			CNetMsg::SP rmsg(new CNetMsg);
			EventEUR2PromotionMsg(rmsg, (MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE)type);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	case MSG_EVENT_EUR2_PROMOTION_WRONG_KEY:
	case MSG_EVENT_EUR2_PROMOTION_ALREADY_USED:
	case MSG_EVENT_EUR2_PROMOTION_SYS_ERROR_NOT_USED:
	case MSG_EVENT_EUR2_PROMOTION_CANT_USE_USER:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventEUR2PromotionMsg(rmsg, (MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE)type);
			SEND_Q(rmsg, pc->m_desc);
			return ;
		}
		break;
	case MSG_EVENT_EUR2_PROMOTION_LACK_INVENTORY:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysFullInventoryMsg(rmsg, 0);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	case MSG_EVENT_EUR2_PROMOTION_HEAVY_WEIGHT:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	default:
		break;
	}
}
void GiveItem(CPC* pc)
{
	if(pc == NULL)
		return ;

	static const int GiveItemCount = 5;
	static const int nItemList[7][5] =
	{
		{6531, 6532, 6533, 6534, 6535},		// 0: TITAN ITEM
		{6536, 6537, 6538, 6539, 6540},		// 1: KNIGHT ITEM
		{6541, 6542, 6543, 6544, 6545},		// 2: HEALER ITEM
		{6546, 6547, 6548, 6549, 6550},		// 3: MAGE ITEM
		{6551, 6552, 6553, 6554, 6555},		// 4: ROGUE ITEM
		{6556, 6557, 6558, 6559, 6560},		// 5: SORCERER ITEM
		{6561, 6562, 6563, 6564, 6565},		// 6: NIGHTSHADOW ITEM
	};

	for(int i = 0; i < GiveItemCount; i++)
	{
		CItem* pItem = gserver->m_itemProtoList.CreateItem(nItemList[pc->m_job][i], -1, 0, 0, 1);
		if(pItem == NULL)
		{
			GAMELOG << init("ITEM ERROR") << "PROMOTION ITEM NOT EXIST" << "ITEM INDEX" << delim << nItemList[pc->m_job][i] << end;
			continue;
		}

		if (pc->m_inventory.getEmptyCount() < 1)
		{
			GAMELOG << init("CANNOT INSERT INVEN(EUR2 PROMOTION)");
			GAMELOG << "charindex" << delim << pc->m_index << end;
			CNetMsg::SP rmsg(new CNetMsg);
			SysFullInventoryMsg(rmsg, 0);
			SEND_Q(rmsg, pc->m_desc);

			return;
		}

		// 인벤에 넣어준다.
		if (pc->m_inventory.addItem(pItem) == false)
		{
			delete pItem;
			return;
		}
	}
}
#endif
