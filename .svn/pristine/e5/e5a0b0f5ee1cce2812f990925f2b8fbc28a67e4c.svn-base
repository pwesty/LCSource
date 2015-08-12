#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/DBCmd.h"

void doCashItemPurchaseHistory(CPC* ch, CNetMsg::SP& msg)
{
	int y;
	char m;
	char d;

	RefMsg(msg) >> y
				>> m
				>> d;

	CNetMsg::SP rmsg(new CNetMsg);
	ConnCashItemPurchaseHistoryReq(rmsg, ch->m_desc->m_index, ch->m_index, y, m, d);
	SEND_Q(rmsg, gserver->m_connector);
}

void doCashItemPurchaseList(CPC* ch, CNetMsg::SP& msg)
{
	//MSG_CONN_CASHITEM_PURCHASELIST_REQ, // 구입목록     : userindex(n) charindex(n)

	if(gserver->isRunConnector())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnCashItemPurchaselistReq(rmsg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(rmsg, gserver->m_connector);
		return;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemPurchaseListRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CONN, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}

void CashItemBringProcess(CPC* ch, CNetMsg::SP& msg, bool bPresent)
{
	// 케릭터 롹 걸고 현재 인벤칸/ 소지량 정보를 확인후 결과에따라 에러 메세지를 보내거나 아이템을 지급후 커넥터에 세팅 요청을 한다
	// 커넥터가 제대로 세팅 못했을 경우 롤백한다.
	//MSG_EX_CASHITEM_BRING_REQ,			// 가져오기			: count(n) idx(n) ctid(n)
	int count = 0;
	int ctid[10], idx[10], i;
	int invenSpace = 0;
	int sumItem = 0;
	memset(idx, -1, sizeof(idx) );
	memset(ctid, -1, sizeof(ctid) );

	RefMsg(msg) >> count;

	if(count == 0)
	{
		// 옮길 물품이 없어요
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;
	invenSpace = ch->m_inventory.getEmptyCount();
	CCtItemList* ctItemList = NULL;
	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> idx[i]
					>> ctid[i];

		int j = 0;
		CCatalog* m_catalog = gserver->m_catalogList.Find(ctid[i]);

		if( !m_catalog )
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
			CNetMsg::SP rmsg(new CNetMsg);
			CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ctItemList = m_catalog->GetCtItemList();
		while(ctItemList && j < ctItemList->GetCount())
		{
			CItem* item = ctItemList->GetItem(j);
			if( !item )
			{
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				CNetMsg::SP rmsg(new CNetMsg);
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			sumItem++;
			j++;
		}
	}

	if (sumItem > invenSpace)
	{
		// 케릭터 락해제
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_LACKINVEN, bPresent);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(gserver->isRunConnector())
	{
		GAMELOG << init("CASH_ITEM_BRING_REQ", ch )
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< count << delim;
		CCtItemList* ctItemList = NULL;
		for(i = 0; i < count; i++)
		{
			int j = 0;
			CCatalog* m_catalog = gserver->m_catalogList.Find(ctid[i]);

			if( !m_catalog )
			{
				// 케릭터 락해제
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CNetMsg::SP rmsg(new CNetMsg);
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			GAMELOG << m_catalog->GetIndex() << delim
					<< m_catalog->GetName()  << delim;

			ctItemList = m_catalog->GetCtItemList();

			if( !ctItemList )
			{
				// 케릭터 락해제
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CNetMsg::SP rmsg(new CNetMsg);
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			int itemcount = ctItemList->GetCount();

			GAMELOG << itemcount << delim;

			CItem* item;
			for(j = 0; j < itemcount; j++)
			{
				item = ctItemList->GetItem(j);

				if( !item )
				{
					// 케릭터 락해제
					ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
					ch->m_billReqTime = 0;
					CNetMsg::SP rmsg(new CNetMsg);
					CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				ch->GiveItem(item->m_itemProto->getItemIndex(),
							 item->getPlus(),
							 item->getFlag(),
							 item->Count() );

//				GAMELOG << itemlog(item);		생성되는 아이템 로그로 대체
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemBringReq(rmsg, bPresent, ch->m_desc->m_index, ch->m_index, count, idx, ctid);
			SEND_Q(rmsg, gserver->m_connector);
		}

		GAMELOG << end;
	}
	else
	{
		// 콘넥터 에라 메세지
		// 케릭터 락해제
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CONN, bPresent);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void doCashItemBring(CPC* ch, CNetMsg::SP& msg)
{
	CashItemBringProcess(ch, msg, false);
}

void doCashItemBalanceReq(CPC* ch, CNetMsg::SP& msg)
{
	CNetMsg::SP rmsg(new CNetMsg);
	ConnCashItemBalanceReq(rmsg, ch->m_desc->m_index, ch->m_desc->m_idname);
	SEND_Q(rmsg, gserver->m_connector);
}

void doCashItemPuchase(CPC* ch, CNetMsg::SP& msg)
{
	int cash, count, i, requestcash;
	int ctid[16];
	int sumCash = 0;

	memset(ctid, -1, sizeof(ctid) );

	RefMsg(msg) >> cash
				>> count;

	// 케릭터 락
	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;

	CCatalog* catal = NULL;

	GAMELOG << init("CASH_ITEM_PURCHASE_REQ", ch)
			<< ch->m_cashBalance << delim
			<< count << delim;

	int limitCount;
	limitCount = 0;
	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> ctid[i];

		catal = gserver->m_catalogList.Find(ctid[i]);

		if( !catal || !catal->GetEnable())
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;

			CNetMsg::SP rmsg(new CNetMsg);
			CashItemPurchaseRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, cash);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		sumCash +=catal->GetCash();

		GAMELOG << catal->GetIndex() << delim
				<< catal->GetName() << delim
				<< catal->GetCash() << delim
				<< catal->GetMileage() << delim;

		// 구입요청한 물품이 한정 판매 물품일 경우 리미트가 남아있지 않으면 에러 메세지를 보냄
		std::set<int>::iterator it = gserver->m_limitCT.find(ctid[i]);
		if( it != gserver->m_limitCT.end() )
		{
			limitCount++;

			if(  catal->GetToSell() < limitCount )
			{
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;

				CNetMsg::SP rmsg(new CNetMsg);
				CashItemPurchaseRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTHAVECT, cash);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		GAMELOG << catal->GetToSell() << delim;
	}

	RefMsg(msg) >> requestcash;

	if(sumCash != requestcash || sumCash > cash)
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemPurchaseRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_LACKCASH, cash);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(gserver->isRunConnector())
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef USE_TENTER_BILLING
		ConnCashItemPurchaseReqMsg(rmsg, ch->m_desc->m_index, ch->m_desc->m_idname, ch->m_index, cash, count, ctid, requestcash);
#else
		ConnCashItemPurchaseReqMsg(rmsg, ch->m_desc->m_index, ch->m_desc->m_idname, ch->m_index, cash, count, ctid);
#endif
		SEND_Q(rmsg, gserver->m_connector);

		GAMELOG << requestcash << end;
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemPurchaseRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CONN, cash);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void doCashItemGift(CPC* ch, CNetMsg::SP& msg)
{
	// 보내는 케릭터 lock 걸고,
	// t_character에서 케릭터가 있는지 확인,
	// 가져오기 프로세스를 이용하여 선물 디비에 세팅
	// 세팅 성공여부에 따라 rep 메세지 보내기

	int count = 0;
	int idx[10], ctid[10], i;

	CLCString recvcharName( MAX_CHAR_NAME_LENGTH + 1);
	CLCString strMsg( MAX_GIFT_MESSAGE_LENGTH + 1);

	RefMsg(msg) >> recvcharName
				>> strMsg
				>> count;

	if(count > 10)
		return ;

	// 락걸기
	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;

	if( count < 1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_NOCTID);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for( i = 0; i < count; i++ )
	{
		RefMsg(msg) >> idx[i]
					>> ctid[i];

#ifdef	CASH_SHOP_NO_GIFT_FLAG
		CCatalog* pCatalog = gserver->m_catalogList.Find(ctid[i]);

		if(pCatalog && (pCatalog->GetFlag() & CATALOG_FLAG_NOGIFT))
		{
			//MSG_EX_CASHITEM_ERROR_CANT_GIFT
			CNetMsg::SP rmsg(new CNetMsg);
			CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CANT_GIFT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#endif	// #ifdef	CASH_SHOP_NO_GIFT_FLAG

#ifdef CASH_SHOP_NO_GIFT_BILA
		if(ctid[i] == 157 || ctid[i] == 552 || ctid[i] == 553)
		{
			//MSG_EX_CASHITEM_ERROR_CANT_GIFT
			CNetMsg::SP rmsg(new CNetMsg);
			CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CANT_GIFT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#endif // CASH_SHOP_NO_GIFT_BILA
		//return ;
	}

	// <<< kjban 2008.04.21
	// 선물 받을 대상의 캐릭명 길이 체크
	if(recvcharName.Length() == 0)
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// >>>

	if( strcmp(ch->GetName(), recvcharName) == 0 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGiftCharReq(rmsg, ch->m_desc->m_index, ch->m_index, recvcharName, strMsg, count, idx, ctid);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
	}
}

void doCashItemGiftSendHistory(CPC* ch, CNetMsg::SP& msg)
{
	int y;
	char m;
	char d;

	RefMsg(msg) >> y
				>> m
				>> d;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnCashItemPresentHistoryReq(rmsg, true, ch->m_desc->m_index, ch->m_index, y, m, d);
		SEND_Q(rmsg, gserver->m_connector);
	}
}
void doCashItemGiftRecvHistory(CPC* ch, CNetMsg::SP& msg)
{
	int y;
	char m;
	char d;

	RefMsg(msg) >> y
				>> m
				>> d;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnCashItemPresentHistoryReq(rmsg, false, ch->m_desc->m_index, ch->m_index, y, m, d);
		SEND_Q(rmsg, gserver->m_connector);
	}
}

void doCashItemGiftRecvList(CPC* ch, CNetMsg::SP& msg)
{
	//// listflag					: (1 << 0): start, (1 << 1): end, (1 << 2): empty
	// 받는 케릭터 락걸고, 선물 받은 리스트를 커넥터에 요청
	// 리스트를 받아 보냄
	CNetMsg::SP rmsg(new CNetMsg);
	ConnCashItemGiftRecvListReqMsg(rmsg, ch->m_desc->m_index, ch->m_index);
	SEND_Q(rmsg, gserver->m_connector);
}

void doCashItemGiftRecv(CPC* ch, CNetMsg::SP& msg)
{
	// 받는 케릭터 락걸고 가져오기 프로세스 돌리기
	CashItemBringProcess(ch, msg, true);
}

void doCashItemShopOpenReq(CPC* ch, CNetMsg::SP& msg)
{
	// 받는 데이터 없음.
	// 처음 열었을때.. 무엇을 할까....
	// 타입 리스트들을 보내야 겠지
	// 캐쉬 잔고도 보내야 되는데 잔고 요청이 오나? 확인 해볼것
	if(!ch)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		// 타입리스트 보내기
		MsgCashshopTypeListRep(rmsg, gserver->m_catalogList.m_typeCount, gserver->m_catalogList.m_typelist, gserver->m_catalogList.m_subtypeCount, gserver->m_catalogList.m_subtypelist);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 첫페이지 리스트 보내기 doCashItemListReq 루틴을 돌고...보내져야함...
	CashItemListProcess(ch, CATEGORY_HOTNNEW, 1);

	// 잔고 보내기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnCashItemBalanceReq(rmsg, ch->m_desc->m_index, ch->m_desc->m_idname);
		SEND_Q(rmsg, gserver->m_connector);
	}
}

void doCashItemListReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;

	int SearchKey = 0;
	int ReqPage = 1;

	RefMsg(msg) >> SearchKey
				>> ReqPage;
	CashItemListProcess(ch, SearchKey, ReqPage);
}

void doCashItemRecommandReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;

	int category = 10000;
	RefMsg(msg) >> category;
	CashItemRecommandProcess(ch, category);
}

void doCashItemWishListReq(CPC* ch, CNetMsg::SP& msg)
{
	// 소망상자 요청
	if(!ch)
		return ;

	CNetMsg::SP rmsg(new CNetMsg);
	ConnWishlistReq(rmsg, ch);
	SEND_Q(rmsg, gserver->m_connector);
}

void doCashItemWishListSaveReq(CPC* ch, CNetMsg::SP& msg)
{
	// 소망상자 저장 요청
	if(!ch)
		return ;
	int ctid;
	RefMsg(msg) >> ctid;

	CNetMsg::SP rmsg(new CNetMsg);
	ConnWishlistSaveReq(rmsg, ch, ctid);
	SEND_Q(rmsg, gserver->m_connector);
}

void doCashItemWishListDelReq(CPC* ch, CNetMsg::SP& msg)
{
	// 소망상자 삭제 요청
	if(!ch)
		return ;

	int ctid;
	RefMsg(msg) >> ctid;

	CNetMsg::SP rmsg(new CNetMsg);
	ConnWishlistDelReq(rmsg, ch, ctid);
	SEND_Q(rmsg, gserver->m_connector);
}

void CashItemListProcess(CPC* ch, int SearchKey, int ReqPage)
{
	// 리스트 요청
	int i;
	int depth_level = 0;	// 검색 레벨 (1:카테고리, 2:타입, 3:서브타입)
	int ct_count = 0;
	int find_count = 0;
	int* ctid = NULL;
	int exposure_ctid[8];
	int exposure_type[8];
	int exposure_remain[8];
	int exposure_price[8];
	int total_page = 0;
	int getflag = 0;
	int enable = 0;
	CCatalog* pCatalog;

	// 10으로 나누어서 나머지가 0이 아니면 검색레벨 3
	// 1000으로 나누어서 나머지가 0이 아니면 검색레벨 2
	// 그외는 검색레벨 1

	if(SearchKey % 10 != 0)				depth_level = 3;
	else if(SearchKey % 1000 != 0)		depth_level = 2;
	else if(SearchKey % 10000 == 0)		depth_level = 1;

	if(depth_level == 0)
	{
		GAMELOG << init("Search Key is wrong.", ch) << end;
		return ;
	}

	ct_count = gserver->m_catalogList.GetCount();

	if(ct_count != 0)
	{
		ctid = new int[ct_count];
		memset(ctid, -1, sizeof(int)*ct_count);
		memset(exposure_ctid, 0, sizeof(int)*8);
		memset(exposure_remain, -1, sizeof(int)*8);
		memset(exposure_price, 0, sizeof(int)*8);
		memset(exposure_type, 0, sizeof(int)*8);
	}
	else
		return ;

	for(i = 0; i < ct_count; i++)
	{
		//pCatalog = gserver->m_catalogList.FindUseArray(i);
		pCatalog = gserver->m_catalogList.getCatalog(i);
		enable = pCatalog->GetEnable();

		if(enable == 0)
			continue;

		if(depth_level == 1)
		{
			if(SearchKey == CATEGORY_HOTNNEW)
			{
				getflag = pCatalog->GetFlag();

				if( (getflag & CATALOG_FLAG_NEW ||
						getflag & CATALOG_FLAG_POPULAR ||
						getflag & CATALOG_FLAG_DISCOUNT) && enable == 1)
				{
					ctid[find_count] = pCatalog->GetIndex();
					find_count++;
				}
			}
			else if((SearchKey == pCatalog->GetCategory()) && enable == 1)
			{
				ctid[find_count] = pCatalog->GetIndex();
				find_count++;
			}
		}
		else if(depth_level == 2)
		{
			getflag = pCatalog->GetFlag();
			if((SearchKey == pCatalog->GetType()) && enable == 1)
			{
				ctid[find_count] = pCatalog->GetIndex();
				find_count++;
			}
			switch(SearchKey)
			{
			case TYPE_HOTNNEW_NEW :
				{
					if((getflag & CATALOG_FLAG_NEW) && enable == 1)
					{
						ctid[find_count] = pCatalog->GetIndex();
						find_count++;
					}
				}
				break;
			case TYPE_HOTNNEW_DISCOUNT :
				{
					if((getflag & CATALOG_FLAG_DISCOUNT) && enable == 1)
					{
						ctid[find_count] = pCatalog->GetIndex();
						find_count++;
					}
				}
				break;
			case TYPE_HOTNNEW_HOT :
				{
					if((getflag & CATALOG_FLAG_POPULAR) && enable == 1)	//HOT=POPULAR
					{
						ctid[find_count] = pCatalog->GetIndex();
						find_count++;
					}
				}
				break;
			default :
				break;
			}
		}
		else if(depth_level == 3)
		{
			if((SearchKey == pCatalog->GetSubtype()) && enable == 1)
			{
				ctid[find_count] = pCatalog->GetIndex();
				find_count++;
			}
		}
	}
	// 해당 되는 아이템을 전부찾았으니..이제 페이지를 나누어 봅니다.
	total_page = find_count / 8;

	if((find_count % 8) != 0)
		total_page++;

	if(ReqPage > total_page)
	{
		GAMELOG << init("Request Page Error") << end;
		ReqPage = 1;
	}

	int temp;
	int j;
	int send_count = 0;

	temp = (ReqPage - 1) * 8;
	for(i = 0; i < 8; i++)
	{
		exposure_ctid[i] = ctid[temp];
		if(ctid[temp] > 0)
		{
			send_count++;
		}
		CCatalog* p = gserver->m_catalogList.Find(ctid[temp]);

		//if(p == NULL)
		//	continue;
		if(p)
		{
			exposure_type[i] = p->GetSubtype();
			if(exposure_type[i] % 100 == 0)
			{
				exposure_type[i] = p->GetType();
				if(exposure_type[i] % 10000 == 0)
				{
					exposure_type[i] = p->GetCategory();
				}
			}
			exposure_price[i] = p->GetCash();
		}

		std::set<int>::iterator it = gserver->m_limitCT.begin();
		std::set<int>::iterator endit = gserver->m_limitCT.end();
		for(; it != endit; ++it)
		{
			CCatalog* catal = gserver->m_catalogList.Find(*it);
			if(catal)
			{
				if(ctid[temp] == catal->GetIndex())
				{
					exposure_remain[i] = catal->GetToSell();
				}
			}
		}

		temp++;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgCashshopListRep(rmsg, SearchKey, send_count, exposure_ctid, exposure_type, exposure_remain, exposure_price, ReqPage, total_page);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 함수 종료 시..
	if(ctid)
	{
		delete[] ctid;
		ctid = NULL;
	}
	// 추천아이템 리스트 보내기
	if(SearchKey % 10000 == 0)
	{
		CashItemRecommandProcess(ch, SearchKey);
	}
}
void CashItemRecommandProcess(CPC* ch, int category)
{
	// 추천 리스트 요청
//	int category = 0;
	int temp;
	int recommand_ctid[5];
	int recommand_type[5];
	int recommand_price[5];
	int recommand_remain[5];
	int ct_count  = 0;
	int recommand_count = 0;
	int i, j;
	int flag;
	void* p = NULL;
	CCatalog* pLimitCatalog = NULL;
	CCatalog* pCatalog = NULL;

	memset(recommand_ctid, 0, sizeof(int)*5);
	memset(recommand_price, 0, sizeof(int)*5);
	memset(recommand_remain, -1, sizeof(int)*5);
	memset(recommand_type, 0, sizeof(int)*5);

//	RefMsg(msg) >> category;

	if(category == CATEGORY_HOTNNEW)
		category = CATEGORY_PLATINUM;

	ct_count = gserver->m_catalogList.GetCount();
	for(i = 0; i < ct_count; i++)
	{
		//pCatalog = gserver->m_catalogList.FindUseArray(i);
		pCatalog = gserver->m_catalogList.getCatalog(i);
		if(!pCatalog)
			continue;

		if(pCatalog->GetCategory() == category)
		{
			flag = pCatalog->GetFlag();
			temp = pCatalog->GetSubtype();
			if(temp % 100 == 0)
			{
				temp = pCatalog->GetType();
				if(temp % 10000 == 0)
				{
					temp = pCatalog->GetCategory();
				}
			}
			if(flag & CATALOG_FLAG_RECOMM_1)
			{
				if(pCatalog->GetEnable())
				{
					recommand_ctid[0] = pCatalog->GetIndex();
					recommand_type[0] = temp;
					recommand_price[0] = pCatalog->GetCash();
					recommand_count++;
				}
			}
			else if(flag & CATALOG_FLAG_RECOMM_2)
			{
				if(pCatalog->GetEnable())
				{
					recommand_ctid[1] = pCatalog->GetIndex();
					recommand_type[1] = temp;
					recommand_price[1] = pCatalog->GetCash();
					recommand_count++;
				}
			}
			else if(flag & CATALOG_FLAG_RECOMM_3)
			{
				if(pCatalog->GetEnable())
				{
					recommand_ctid[2] = pCatalog->GetIndex();
					recommand_type[2] = temp;
					recommand_price[2] = pCatalog->GetCash();
					recommand_count++;
				}
			}
			else if(flag & CATALOG_FLAG_RECOMM_4)
			{
				if(pCatalog->GetEnable())
				{
					recommand_ctid[3] = pCatalog->GetIndex();
					recommand_type[3] = temp;
					recommand_price[3] = pCatalog->GetCash();
					recommand_count++;
				}
			}
			else if(flag & CATALOG_FLAG_RECOMM_5)
			{
				if(pCatalog->GetEnable())
				{
					recommand_ctid[4] = pCatalog->GetIndex();
					recommand_type[4] = temp;
					recommand_price[4] = pCatalog->GetCash();
					recommand_count++;
				}
			}
		}
	}

	std::set<int>::iterator it = gserver->m_limitCT.begin();
	std::set<int>::iterator endit = gserver->m_limitCT.end();
	for(; it != endit; ++it)
	{
		CCatalog* pLimitCatalog = gserver->m_catalogList.Find(*it);
		if(pLimitCatalog == NULL)
			continue;

		if(recommand_ctid[0] == pLimitCatalog->GetIndex())
			recommand_remain[0] = pLimitCatalog->GetToSell();

		else if(recommand_ctid[1] == pLimitCatalog->GetIndex())
			recommand_remain[1] = pLimitCatalog->GetToSell();

		else if(recommand_ctid[2] == pLimitCatalog->GetIndex())
			recommand_remain[2] = pLimitCatalog->GetToSell();

		else if(recommand_ctid[3] == pLimitCatalog->GetIndex())
			recommand_remain[3] = pLimitCatalog->GetToSell();

		else if(recommand_ctid[4] == pLimitCatalog->GetIndex())
			recommand_remain[4] = pLimitCatalog->GetToSell();
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgCashshopRecommandRep(rmsg, 5, recommand_ctid, recommand_type, recommand_remain, recommand_price);
		SEND_Q(rmsg, ch->m_desc);
	}
}

//////////////////////////////////////////////////////////////////////////
void do_Ex_CashItem(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;

	RefMsg(msg) >> subtype;
	switch(subtype)
	{
	case MSG_EX_CASHITEM_BALANCE_REQ:
		doCashItemBalanceReq(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASE_REQ:
		doCashItemPuchase(ch, msg);
		break;
	case MSG_EX_CASHITEM_BRING_REQ:
		doCashItemBring(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASELIST_REQ:
		doCashItemPurchaseList(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASEHISTORY_REQ:
		doCashItemPurchaseHistory(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_REQ:			// 선물 보내기 요청	: charName(str) Msg(str) count(n) idx(n) ctid(n)
		doCashItemGift(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ:  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
		doCashItemGiftSendHistory(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ:  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
		doCashItemGiftRecvHistory(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECVLIST_REQ:	   // 선물 받은 리스트 요청
		doCashItemGiftRecvList(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECV_REQ:			// 선물 받기		: count(n) idx(n) ctid(n)
		doCashItemGiftRecv(ch, msg);
		break;

	case MSG_EX_CASHITEM_SHOP_OPEN_REQ:
		doCashItemShopOpenReq(ch, msg);
		break;
	case MSG_EX_CASHITEM_LIST_REQ:
		doCashItemListReq(ch, msg);
		break;

	case MSG_EX_CASHITEM_RECOMMAND_REQ:
		doCashItemRecommandReq(ch, msg);
		break;

	case MSG_EX_CASHITEM_WISHLIST_REQ:
		doCashItemWishListReq(ch, msg);
		break;

	case MSG_EX_CASHITEM_WISHLIST_SAVE_REQ:
		doCashItemWishListSaveReq(ch, msg);
		break;

	case MSG_EX_CASHITEM_WISHLIST_DEL_REQ:
		doCashItemWishListDelReq(ch, msg);
		break;
	}
}
