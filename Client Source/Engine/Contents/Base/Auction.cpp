
#include "StdH.h"
#include "Auction.h"
#include <string>
#include <../Common/CommonDef.h>
#include <../Common/Packet/ptype_tradeAgent_system.h>

#include <Engine/Network/NetworkMessage.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Util.h>
#include <Engine/Contents/Base/UIAuctionNew.h>
#include <Engine/Contents/Base/ChattingUI.h>

Auction::Auction()
	: m_nSearchCount(0)
	, m_sCurPage(0)
	, m_pSearchList(NULL)
	, m_nRegCount(0)
	, m_pRegList(NULL)
	, m_nFavoriteCount(0)
	, m_pFavoriteList(NULL)	
	, m_b1stRegList(false)
	, m_b1stFavoriteList(false)
	, m_nDeposit(0)
	, m_fFee(0.f)
	, m_pSearchLast(NULL)
	, m_nServerTime(0)
	, m_bRegUpdate(false)
	, m_bNetWorkLock(false)
	, m_bPremiumChar(false)
{

}

Auction::~Auction()
{
	reset();
}

void Auction::reset()
{
	clearSearchList();
	SAFE_DELETE(m_pSearchLast);
	clearRegList();
	clearFavoriteList();

	m_b1stRegList = false;
	m_b1stFavoriteList = false;
}

void Auction::clearSearchList()
{
	m_sCurPage = 0;
	m_ucNextPage = 0;
	m_nSearchCount = 0;
	SAFE_ARRAY_DELETE(m_pSearchList);
}

TradeAgentItem* Auction::getSearchItem( int idx )
{
	if (idx >= 0 && idx < m_nSearchCount)
		return &m_pSearchList[idx];

	return NULL;
}

RequestClient::TradeAgentSystemListBase* Auction::getSearchPacket( int idx )
{
	SAFE_DELETE(m_pSearchLast);

	if (idx < 2)
	{
		m_pSearchLast = new RequestClient::TradeAgentSystemList;
		memset(m_pSearchLast, 0, sizeof(RequestClient::TradeAgentSystemList));		
	}
	else
	{
		m_pSearchLast = new RequestClient::TradeAgentSystemListBase;
		memset(m_pSearchLast, 0, sizeof(RequestClient::TradeAgentSystemListBase));
	}

	m_pSearchLast->itemTypeTab = idx;

	return m_pSearchLast;
}

void Auction::clearRegList()
{
	m_nRegCount = 0;
	SAFE_ARRAY_DELETE(m_pRegList);
}

TradeAgentItem* Auction::getRegItem( int idx )
{
	if (idx >= 0 && idx < m_nRegCount)
		return &m_pRegList[idx];
	
	return NULL;
}

void Auction::clearFavoriteList()
{
	m_nFavoriteCount = 0;
	SAFE_ARRAY_DELETE(m_pFavoriteList);
}

TradeAgentItem* Auction::getFavoriteItem( int idx )
{
	if (idx >= 0 && idx < m_nFavoriteCount)
		return &m_pFavoriteList[idx];

	return NULL;
}

//-----------------------------------------------------------------------------

void Auction::ReceiveTradeAgentMessage(CNetworkMessage *istr )
{
	BOOL bShowMessage = TRUE;

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	LOG_DEBUG("### ReceiveTradeAgentMessage (%d - %d)", pBase->type, pBase->subType);

	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIAuctionNew* pUIAuction = pUIManager->GetAuction();

	switch(pBase->subType)
	{
	case MSG_TRADEAGENT_LIST:
		RecvSearchList(istr);
		break;

	case MSG_TRADEAGENT_REG_LIST:
		RecvRegistList(istr);
		break;

	case MSG_TRADEAGENT_REG:
		{		
			pUIAuction->showRegOK();
			CUIFocus::getSingleton()->setUI(NULL);

			// 등록이 성공되었다면, 리스트 요청하자
			SendRegistListReq();
		}
		break;

	case MSG_TRADEAGENT_REG_CANCEL:
		{
			LOG_DEBUG( "MSG_TRADEAGENT_REG_CANCEL" );
			SendRegistListReq();
		}
		break;

	case MSG_TRADEAGENT_BUY:
		{
			// LCE로 들어가서 할게 없다.
			LOG_DEBUG( "MSG_TRADEAGENT_BUY" );

			SendSearchReq();
		}
		break;

	case MSG_TRADEAGENT_LIKE_LIST:
		RecvFavoriteList(istr);
		break;

	case MSG_TRADEAGENT_LIKE_REG:
		SendFavoriteListReq();
		break;

	case MSG_TRADEAGENT_LIKE_CANCEL:
		SendFavoriteListReq();
		break;

	case MSG_TRADEAGENT_ERROR:
		{
			ResponseClient::TradeAgentSystemError* pPack = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(istr->GetBuffer());
			// error
			LOG_DEBUG( "Error ReceiveTradeAgentMessage(%d)", pPack->errorCode );

			pUIAuction->ShowError( pPack->errorCode );

			// 둘 다 갱신해야 한다.
			SendSearchReq();
			SendFavoriteListReq();
		}
		break;

	case MSG_TRADEAGENT_AFTER_SELL:
		{
			ResponseClient::TradeAgentAfterSell* pPack = reinterpret_cast<ResponseClient::TradeAgentAfterSell*>(istr->GetBuffer());

			CItemData* pItemData = _pNetwork->GetItemData(pPack->itemIndex);
			CTString	strMsg;			
			strMsg.PrintF(_S(4362, "거래 대행중인 %s 아이템 %d개에 대한 판매가 이루어졌습니다."), pItemData->GetName(), pPack->itemCount);

			pUIManager->GetChattingUI()->AddSysMessage(strMsg, SYSMSG_NOTIFY);
		}
		break;

	case MSG_TRADEAGENT_AFTER_RETURNED:
		{
			ResponseClient::TradeAgentAfterReturned* pPack = reinterpret_cast<ResponseClient::TradeAgentAfterReturned*>(istr->GetBuffer());

			CItemData* pItemData = _pNetwork->GetItemData(pPack->itemIndex);
			CTString	strMsg;			
			strMsg.PrintF(_S(4363, "%s 아이템 %d개가 반송되었습니다. 정산 페이지를 확인하십시오."), pItemData->GetName(), pPack->itemCount);

			pUIManager->GetChattingUI()->AddSysMessage(strMsg, SYSMSG_NOTIFY);
		}
		break;

	case MSG_TRADEAGENT_CHANGE_FEE:
		{
			ResponseClient::TradeAgentChangeFee* pPack = 
				reinterpret_cast<ResponseClient::TradeAgentChangeFee*>(istr->GetBuffer());
			m_fFee = pPack->fee;

			if (pUIManager->isCreateVisible(UI_AUCTION) == true)
				pUIManager->GetAuction()->updateFeeDeposit(m_nDeposit, m_fFee);
		}
		break;
	case MSG_TRADEAGENT_CHANGE_DEPOSIT:
		{
			ResponseClient::TradeAgentChangeDeposit* pPack = 
				reinterpret_cast<ResponseClient::TradeAgentChangeDeposit*>(istr->GetBuffer());
			m_nDeposit = pPack->deposit * (m_nRegCount + 1);

			if (pUIManager->isCreateVisible(UI_AUCTION) == true)
				pUIManager->GetAuction()->updateFeeDeposit(m_nDeposit, m_fFee);
		}
		break;

	default:
		{
			LOG_DEBUG("INFO : ReceiveTradeAgentMessage Not Defined Message(%d : %d)", pBase->type, pBase->subType );
		}
		break;
	}

}

void Auction::updateNas()
{
	// UI가 열려 있다면,
	if (CUIManager::getSingleton()->isCreateVisible(UI_AUCTION))
		CUIManager::getSingleton()->GetAuction()->updateNas();
}

void Auction::SendSearchReq()
{
	// 검색 요청
	CNetworkMessage nmList;

	// 마지막 검색 기록이 있을 경우에만 보냄
	if (m_pSearchLast == NULL)
		return;

	if (m_pSearchLast->itemTypeTab < 2)
	{
		RequestClient::TradeAgentSystemList* packet = reinterpret_cast<RequestClient::TradeAgentSystemList*>(nmList.nm_pubMessage);
		memcpy( packet, m_pSearchLast, sizeof(RequestClient::TradeAgentSystemList) );
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_LIST;
		nmList.setSize(sizeof(*packet));
	}
	else
	{
		RequestClient::TradeAgentSystemListBase* packet = reinterpret_cast<RequestClient::TradeAgentSystemListBase*>(nmList.nm_pubMessage);
		memcpy( packet, m_pSearchLast, sizeof(RequestClient::TradeAgentSystemListBase) );
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_LIST;
		nmList.setSize(sizeof(*packet));
	}
	
	_pNetwork->SendToServerNew(nmList);	
}

void Auction::SendBuyReq( UINT trade_index )
{
	if (m_bNetWorkLock == true)
		return;

	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemBuy* packet = reinterpret_cast<RequestClient::TradeAgentSystemBuy*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_BUY;
	packet->TradeAgentIndex = trade_index;
	nmList.setSize(sizeof(*packet));

	_pNetwork->SendToServerNew(nmList);
	m_bNetWorkLock = true;
}

//-----------------------------------------------------------------------------
// REGIST

void Auction::SendRegistListReq()
{
	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemRegList* packet = reinterpret_cast<RequestClient::TradeAgentSystemRegList*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_REG_LIST;
	nmList.setSize(sizeof(*packet));
	
	_pNetwork->SendToServerNew(nmList);
}

void Auction::RequestNextPage( bool bNext )
{
	if (m_pSearchLast == NULL)
		return;

	if (bNext == false)
	{
		// 이전 페이지 요청
		if (m_sCurPage <= 1)
		{
			LOG_DEBUG("제일 첫페이지여~");
			return;
		}

		m_pSearchLast->pageNo = m_sCurPage - 1;
		SendSearchReq();
	}
	else
	{
		if (m_ucNextPage <= 0)
		{
			LOG_DEBUG("다음 페이지 없음!!");
			return;
		}

		m_pSearchLast->pageNo = m_sCurPage + 1;
		SendSearchReq();
	}
}

// 아이템 등록
void Auction::SendRegistItemReq( SWORD iTab, SWORD inven_idx, int iIndex, int nCount, __int64 nTotalNas )
{
	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemReg* packet = reinterpret_cast<RequestClient::TradeAgentSystemReg*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_REG;
	packet->charIndex = 0;
	packet->itemSerial = 0;
	packet->tab	= iTab;
	packet->invenIndex = inven_idx;
	packet->itemIndex = iIndex;
	packet->quantity = nCount;
	packet->TotalMoney = nTotalNas;
	//Guaranty;
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);	
}

void Auction::SendRegistCancelReq( UINT index )
{
	if (index < 0 || index >= m_nRegCount)
		return;

	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemRegCancel* packet = reinterpret_cast<RequestClient::TradeAgentSystemRegCancel*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_REG_CANCEL;
	packet->index = m_pRegList[index].index;
	nmList.setSize(sizeof(*packet));

	_pNetwork->SendToServerNew(nmList);
}

//-----------------------------------------------------------------------------

void Auction::RecvSearchList( CNetworkMessage *istr )
{
	ResponseClient::TradeAgentSystemList* pPack = reinterpret_cast<ResponseClient::TradeAgentSystemList*>(istr->GetBuffer());

	clearSearchList();

	m_sCurPage = pPack->pageNo;
	m_ucNextPage = pPack->nextPage;
	m_nServerTime = pPack->curServerTime;

	if (pPack->count > 0)
	{
		m_nSearchCount = pPack->count;
		m_pSearchList = new TradeAgentItem[pPack->count];

		memcpy(m_pSearchList, pPack->list, sizeof(TradeAgentItem) * pPack->count);		
	}

	// count 에 관계없이 (카운트가 '0' 이라면 삭제 목적) 
	if (CUIManager::getSingleton()->isCreateVisible(UI_AUCTION))
		CUIManager::getSingleton()->GetAuction()->updateSearchList();

	// 검색 목록이 갱신되면 구매 가능하도록 락을 푼다
	m_bNetWorkLock = false;
}

void Auction::RecvRegistList( CNetworkMessage *istr )
{
	ResponseClient::TradeAgentSystemRegList* pPack = reinterpret_cast<ResponseClient::TradeAgentSystemRegList*>(istr->GetBuffer());

	m_b1stRegList = true;
	clearRegList();

	m_fFee = pPack->tradeAgentFee;
	m_nDeposit = pPack->Guarantee;
	m_nServerTime = pPack->curServerTime;

	if (pPack->count > 0)
	{
		m_nRegCount = pPack->count;
		m_pRegList = new TradeAgentItem[pPack->count];

		memcpy(m_pRegList, pPack->list, sizeof(TradeAgentItem) * pPack->count);		
	}

	if (CUIManager::getSingleton()->isCreateVisible(UI_AUCTION))
		CUIManager::getSingleton()->GetAuction()->updateRegistList();
	else
		setRegUpdate(true);
}

void Auction::RecvFavoriteList( CNetworkMessage* istr )
{
	ResponseClient::TradeAgentSystemLikeList* pPack = reinterpret_cast<ResponseClient::TradeAgentSystemLikeList*>(istr->GetBuffer());

	m_b1stFavoriteList = true;

	clearFavoriteList();

	m_nFavoriteCount = pPack->count;
	m_nServerTime = pPack->curServerTime;

	if (pPack->count > 0)
	{		
		m_pFavoriteList = new TradeAgentItem[pPack->count];

		memcpy(m_pFavoriteList, pPack->list, sizeof(TradeAgentItem) * pPack->count);		
	}

	if (CUIManager::getSingleton()->isCreateVisible(UI_AUCTION))
		CUIManager::getSingleton()->GetAuction()->updateFavoriteList();
}

//-----------------------------------------------------------------------
// FAVORITE


void Auction::SendFavoriteListReq()
{
	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemLikeList* packet = reinterpret_cast<RequestClient::TradeAgentSystemLikeList*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_LIST;
	nmList.setSize(sizeof(*packet));

	_pNetwork->SendToServerNew(nmList);
}


void Auction::SendFavoriteRegReq( UINT trade_index )
{
	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemLikeReg* packet = reinterpret_cast<RequestClient::TradeAgentSystemLikeReg*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_REG;
	packet->index = trade_index;
	nmList.setSize(sizeof(*packet));

	_pNetwork->SendToServerNew(nmList);
}

void Auction::SendFavoriteCancelReq( UINT trade_index )
{
	CNetworkMessage nmList;
	RequestClient::TradeAgentSystemLikeCancel* packet = reinterpret_cast<RequestClient::TradeAgentSystemLikeCancel*>(nmList.nm_pubMessage);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_CANCEL;
	packet->index = trade_index;
	nmList.setSize(sizeof(*packet));

	_pNetwork->SendToServerNew(nmList);
}

bool Auction::findFavItem( int idx )
{
	int i;

	for (i = 0; i < m_nFavoriteCount; ++i)
	{
		if (m_pFavoriteList[i].index == idx)
			return true;
	}

	return false;
}

void Auction::SetPremiumBenefit( bool bUse )
{
	m_bPremiumChar = bUse;
	UIMGR()->GetAuction()->updatePremiumBenefit();
}

